#include <mregex.hpp>
#include <stack>
#include <queue>
#include <unordered_set>
#include <math.h>

char MRegex::_S_parser_code_caracter_octal(basic_string_range<char> &range)
{
    char result;
    for (char i = 0; i < 3; i++)
    {
        if (range.peak() >= range.end() && i < 1)
            throw std::runtime_error("Error: Octal caracter parsing error, end unspect");
        char caracter = *range.peak();
        if (caracter >= '0' && caracter <= '7')
            result = result * 8 + (caracter - '0');
        else
            break;
        range.next();
    }
    return result;
}

char MRegex::_S_parser_code_caracter(basic_string_range<char> &range)
{
    char c;
    if (range.peak() >= range.end())
        throw std::runtime_error("Error: end-caption caracter especial unespect");
    switch (c = *range.peak())
    {
    case 'n':
        range.next();
        return '\n';
        break;
    case 't':
        range.next();
        return '\t';
        break;
    case 'v':
        range.next();
        return '\v';
        break;
    case 'f':
        range.next();
        return '\f';
        break;
    case 'b':
        range.next();
        return '\b';
        break;
    case 'r':
        range.next();
        return '\r';
        break;
    case 'x':
        range.next();
        return MRegex::_S_parser_code_caracter_hexunicode<2>(range);
        break;
    case 'u':
        range.next();
        return MRegex::_S_parser_code_caracter_hexunicode<4>(range);
        break;
    case 'U':
        range.next();
        return MRegex::_S_parser_code_caracter_hexunicode<8>(range);
        break;
    default:
        if ('0' <= c && c <= '7')
            return MRegex::_S_parser_code_caracter_octal(range);
        range.next();
        return c;
        break;
    }
}

MRegex::range_cualifiquer MRegex::_S_parser_cualifiquer_range(basic_string_range<char> &range)
{
    size_t n1 = 0;    // minimo
    size_t n2 = 0;    // maximo
    size_t *rn = &n1; // puntero al numero actual
    char caracter;
    bool invalid = true; // Determina si es valido el resultado
    // Recorremos la cadena de caracteres
    while (range.peak() < range.end() and (caracter = *range.peak()) != '}')
    {
        // Si es un numero
        if ('0' <= caracter && caracter <= '9')
        {
            // Realizamos los calculos para la conversion
            *rn = *rn * 10 + caracter - '0';
            // Y desactivamos la invalidacion
            invalid = false;
        }
        else if (',' == caracter) // Si es una coma
        {
            // Y el rn apunta a el numero 2(maximo)
            if (rn == &n2)
                throw std::runtime_error("Error: Invalid subdivision"); // Lanzamos excepcion
            // Cambiamos el puntero de n1(minimo) a n2(maximo)
            rn = &n2;
            // Y activamos la invalidacion
            invalid = true;
        }
        else // Si no solo se emite el error de caracter invalido
            throw std::runtime_error("Error: Invalid caracter inserted");
        range.next(); // Y por ultimo pasamos al siguiente
    }
    if (range.peak() == range.end()) // Si no se llego al final del cualificador, emitir error
        throw std::runtime_error("Error: Unespect termination of range");
    if (invalid) // si es invalida, emitir tambien un error
        throw std::runtime_error("Error: Invalid format inserted");
    if (rn == &n1) // si el puntero permanece en n1(minimo), significa que el rango es {n1, n1} minimo igual a maximo
        n2 = n1;
    // Retornar el par de numeros
    return {n1, n2};
}

void MRegex::_S_parser_cualifiquer(basic_string_range<char> &range, NFA &nfa, const MRegex::pair_begin_state_transitions &_T_qAtr, bool isGroup)
{
    char c = 0;
    // Si el rango llega al final salta a la sentemcia por defecto del switch
    if (range.peak() >= range.end())
        goto labelForEnd;
    // Si no obtenemos el caracter en esa posicion
    c = *range.peak();
    switch (c)
    {
    case '?':
    {
        // Si se cumple se agrega la opcionalidad al estado
        range.next();
        // Primera se agregan las transiciones generadas siempre y cuando no sean un grupo
        if (!isGroup)
            for (auto &&transition : _T_qAtr.second)
                nfa.Q_transitions[{_T_qAtr.first, transition}].push_back(nfa.Q_nfa.size());
        // luego se agrega la trasicion de la opcionalidad
        nfa.Q_transitions[{_T_qAtr.first, -1ULL}].push_back(nfa.Q_nfa.size());
        break;
    }
    case '*':
    {
        // Si se cumple se agrega la repeticion de cero a multiples veces
        range.next();
        // Si es un grupo las transiciones de repeticion se realizan desde el final hasta el principio
        if (isGroup)
            for (auto &&transition : _T_qAtr.second)
                nfa.Q_transitions[{nfa.Q_nfa.size(), transition}].push_back(_T_qAtr.first);
        else // si no simplemente se agregan al mismo estado
            for (auto &&transition : _T_qAtr.second)
                nfa.Q_transitions[{_T_qAtr.first, transition}].push_back(_T_qAtr.first);
        nfa.Q_transitions[{_T_qAtr.first, -1ULL}].push_back(nfa.Q_nfa.size());
        break;
    }
    case '+':
    {
        // Si se cumple se agrega la repeticion de cero a multiples veces
        range.next();
        if (!isGroup)
            for (auto &&transition : _T_qAtr.second)
                nfa.Q_transitions[{_T_qAtr.first, transition}].push_back(nfa.Q_nfa.size());
        nfa.Q_transitions[{nfa.Q_nfa.size(), -1ULL}].push_back(_T_qAtr.first);
        break;
    }
    case '{':
    {
        range.next();
        range_cualifiquer cualifiquer_range = MRegex::_S_parser_cualifiquer_range(range);
        range.next();
        if (isGroup)
        {
            size_t n_state = nfa.Q_nfa.size() - _T_qAtr.first;
            std::vector<size_t> optionals_groups = {};
            if (cualifiquer_range.first == 0)
                optionals_groups.push_back(_T_qAtr.first);
            for (size_t state = 1; state < cualifiquer_range.second; state++)
            {
                size_t state_group_begin = nfa.Q_nfa.size();
                if (cualifiquer_range.first <= state && state < cualifiquer_range.second)
                    // Agregar una trasicion de opcionalidad
                    optionals_groups.push_back(state_group_begin);
                for (size_t state_group = 0; state_group < n_state; state_group++)
                {
                    nfa.Q_nfa.push_back(nfa.Q_nfa.size());
                    for (auto &&[key, value] : nfa.Q_transitions)
                    {
                        if (key.first != _T_qAtr.first + state_group)
                            continue;
                        nfa.Q_transitions[{state_group_begin + state_group, key.second}].push_back(nfa.Q_nfa.size());
                    }
                }
            }
            for (auto &&state : optionals_groups)
                nfa.Q_transitions[{state, -1ULL}].push_back(nfa.Q_nfa.size());
        }
        else // si no simplemente se agregan al mismo estado
        {
            size_t state_iterator = _T_qAtr.first;
            for (size_t state = 0; state < cualifiquer_range.second; state++)
            {
                // Si el estado actual esta entre el minimo y el maximo
                if (cualifiquer_range.first <= state && state < cualifiquer_range.second)
                    // Agregar una trasicion de opcionalidad
                    nfa.Q_transitions[{state_iterator, -1ULL}].push_back(_T_qAtr.first + cualifiquer_range.second);
                // Y luego se agregan todas las transiciones desde el estado actual hasta el proximo estado
                for (auto &&transition : _T_qAtr.second)
                    nfa.Q_transitions[{state_iterator, transition}].push_back(nfa.Q_nfa.size());
                // por ultimo se guarda el nuevo estado
                state_iterator = nfa.Q_nfa.size();
                // Y si el rango es menor que el maximo - 1 se agrega el nuevo estado generado
                if (state < cualifiquer_range.second - 1ULL)
                    nfa.Q_nfa.push_back(state_iterator);
            }
        }
        break;
    }
    default:
    labelForEnd:
        // Por ultimo en caso de no cumplirse los casos anteriores o la condicion de final de cadena
        if (isGroup) // si es un grupo, no se agregan dichas transiciones
            break;
        // Pero si no, se agregan las transiciones del estado
        for (auto &&transition : _T_qAtr.second)
            nfa.Q_transitions[{_T_qAtr.first, transition}].push_back(nfa.Q_nfa.size());
        break;
    }
}

MRegex::pair_begin_state_transitions MRegex::_S_parser_nfa_parser_class_expresions(basic_string_range<char> &range, NFA &nfa)
{
    size_t qA = nfa.Q_nfa.size();
    std::vector<size_t> transitions = {};
    std::unordered_set<char> alphabet = {};
    bool inverter = false;

    nfa.Q_nfa.push_back(qA);
    // Chequeamos si es una clase invertida
    if (*range.peak() == '^')
    {
        // Si lo es hacer operaciones de inversion, por ahora no se ha implementado
        inverter = true;
        range.next();
    }
    char a;
    // Recorremos la cadena hasta llegar el final
    while (range.peak() < range.end() and (a = *range.peak()) != ']')
    {
        if (a == '\\')
        {
            range.next();
            a = MRegex::_S_parser_code_caracter(range);
        }
        // Si se cumple la siguiente condicion solo se agrgara una transicion
        if (range.offset(1) >= range.end() or *(range.offset(1)) != '-')
        {
            alphabet.insert(a);
        }
        else
        {
            // En caso contrario obtenemos el rango de transiciones
            auto offset = range.offset(2);
            char b;
            if (offset >= range.end() or (b = *offset) == ']')
                throw std::runtime_error("Error: termination max incurret");
            if (b == '\\')
            {
                range.next();
                b = MRegex::_S_parser_code_caracter(range);
            }
            for (int i = a; i <= b; i++)
                alphabet.insert(char(i));
            range.next();
            range.next();
        }
        range.next();
    }
    // Si la clase esta invertida se agregan todos los caracteres que no fueron detectados dentro del su definicion
    if (inverter)
    {
        // Se recorre todos los caracteres
        for (int i = 0; i < 256; i++)
        {
            // Si el caracter no se encuentra dentro del conjunto
            if (alphabet.count(char(i)) < 1)
            {
                // Se agrega la transicion y luego el caracter al alfabeto
                transitions.push_back(char(i));
                nfa.dictionary.insert(char(i));
            }
        }
    }
    else
    {
        // Si no solo se agregan las transiciones y caracteres al alfabeto definidos en la clase
        for (auto &&i : alphabet)
        {
            transitions.push_back(i);
            nfa.dictionary.insert(i);
        }
    }
    // Retorna el estado inicial y las transiciones
    return {qA, transitions};
}

size_t MRegex::_S_build_nfa_parser_regular_expresions_basic(basic_string_range<char> &range, NFA &nfa, bool isGroup, size_t id)
{
    // Dado el rango de caracteres
    while (range.peak() < range.end())
    {
        // Obtenemos el caracter actual
        MRegex::pair_begin_state_transitions _T_qAtr;
        char c = *range.peak();
        if (c == '|' or c == ')')
            break;
        // Y pasamos al siguiente
        range.next();
        // Segun el caracter que sea
        switch (c)
        {
        case '(':
        {
            // Genera un grupo de reglas, se utiliza la recursion de la fincion recursiva
            _T_qAtr = MRegex::_S_build_nfa_parser_or_expresions(range, nfa, true, id);
            // Si no termina en la posicion correcta, error
            if (range.peak() >= range.end() or *range.peak() != ')')
                throw std::runtime_error("Error: Bad termination in group");
            // Saltamos el final del grupo
            range.next();
            // Y agregamos su respectivo cualificador
            MRegex::_S_parser_cualifiquer(range, nfa, _T_qAtr, true);
            break;
        }
        case '[':
        {
            // Genera un estado para clases de caracteres
            _T_qAtr = MRegex::_S_parser_nfa_parser_class_expresions(range, nfa);
            // Si no termina en la posicion correcta, error
            if (range.peak() >= range.end() or *range.peak() != ']')
                throw std::runtime_error("Error: Bad termination in class");
            // Saltamos el final del grupo
            range.next();
            // Y agregamos su respectivo cualificador
            MRegex::_S_parser_cualifiquer(range, nfa, _T_qAtr, false);
            break;
        }
        case '\\':
        {
            c = MRegex::_S_parser_code_caracter(range);
        }
        default:
        {
            // Simplemente creamos el estado y agregamos la trsnicion
            size_t qA = nfa.Q_nfa.size();
            nfa.dictionary.insert(c);
            nfa.Q_nfa.push_back(qA);
            // Y agregamos su respectivo cualificador
            MRegex::_S_parser_cualifiquer(range, nfa, {qA, {size_t(c)}}, false);
            break;
        }
        }
    }
    size_t qEnd = nfa.Q_nfa.size();
    nfa.Q_nfa.push_back(qEnd);
    return qEnd;
}

MRegex::pair_begin_state_transitions MRegex::_S_build_nfa_parser_or_expresions(basic_string_range<char> &range, NFA &nfa, bool isGroup, size_t id)
{
    // Preguntamos si el inicio es incorrect y lanzamos un error si es asi
    if (range.peak() == range.end() or *(range.peak()) == '|')
        throw std::runtime_error("Error falta operation");
    std::vector<size_t> ends = {};
    // Obtenemos el estado final y lo agregamos
    size_t q0 = nfa.Q_nfa.size();
    nfa.Q_nfa.push_back(q0);
    // Obtenemos el primer estado
    // Agregamos una ∊-transition hacia ese estado
    // nfa.Q_transitions[{q0, -1ULL}] = nfa.Q_nfa.size();
    nfa.Q_transitions[{q0, -1ULL}].push_back(nfa.Q_nfa.size());
    // Guardamos el estado final del resultado de la cadena
    ends.push_back(MRegex::_S_build_nfa_parser_regular_expresions_basic(range, nfa, isGroup, id));
    // Repite la condicion en caso de ser una operacion or
    while (range.peak() < range.end() && *(range.peak()) == '|')
    {
        range.next();
        // Agregamos una ∊-transition hacia ese estado
        // nfa.Q_transitions[{q0, -1ULL}] = nfa.Q_nfa.size();
        nfa.Q_transitions[{q0, -1ULL}].push_back(nfa.Q_nfa.size());
        // Guardamos el estado final del resultado de la cadena
        ends.push_back(MRegex::_S_build_nfa_parser_regular_expresions_basic(range, nfa, isGroup, id));
        // Si se detuvo en una tuberia volver a realizar el procedimiento
    }
    // Para cada estado final se le agregara una ∊-transition al estado de union de la tuberia
    for (auto &&i : ends)
        nfa.Q_transitions[{i, -1ULL}].push_back(nfa.Q_nfa.size());
    // Por ultimo generamos correctamente el estado
    if (!isGroup)
    {
        nfa.Q_nfa.push_back(nfa.Q_nfa.size());
        nfa.F_nfa.emplace(nfa.Q_nfa.size() - 1, id);
    }
    return {q0, {-1ULL}};
}

NFA MRegex::build_nfa(const std::__cxx11::basic_string<char> &str)
{
    basic_string_range<char> range = str;
    NFA nfa;
    nfa.begin_Q_nfa.push_back(MRegex::_S_build_nfa_parser_or_expresions(range, nfa, false, 0).first);
    return nfa;
}

NFA MRegex::build_nfa(const std::initializer_list<std::pair<size_t, std::__cxx11::basic_string<char>>> &list)
{
    NFA nfa;
    nfa.Q_nfa.push_back(0);
    nfa.begin_Q_nfa.push_back(0);
    for (auto &&expresion : list)
    {
        basic_string_range<char> range = expresion.second;
        nfa.Q_transitions[{0, -1ULL}].push_back(MRegex::_S_build_nfa_parser_or_expresions(range, nfa, false, expresion.first).first);
    }
    return nfa;
}
#if TESTCODE
NFA MRegex::build_nfa(const char *argv[], size_t size)
{
    NFA nfa;
    nfa.Q_nfa.push_back(0);
    nfa.begin_Q_nfa.push_back(0);
    for (size_t i = 1; i < size; i++)
    {
        basic_string_range<char> range = std::string(argv[i]);
        nfa.Q_transitions[{0, -1ULL}].push_back(MRegex::_S_build_nfa_parser_or_expresions(range, nfa, false).first);
    }
    return nfa;
}
#endif
DFA::States MRegex::elipson_cloursers(DFA::States states, const NFA &nfa)
{
    std::stack<size_t> stack_status;
    DFA::States clourser = states;
    for (auto &&state : states)
        stack_status.push(state);
    while (!stack_status.empty())
    {
        size_t index = stack_status.top();
        stack_status.pop();
        NFA::Transitions::const_iterator iterator = nfa.Q_transitions.begin();
        if ((iterator = nfa.Q_transitions.find({index, -1ULL})) == nfa.Q_transitions.end())
            continue;
        for (auto &&state : iterator->second)
        {
            stack_status.push(state);
            clourser.insert(state);
        }
    }
    return clourser;
}

DFA::States MRegex::move(DFA::States states, char a, const NFA &nfa)
{
    DFA::States result = {};
    for (auto &&state : states)
    {
        NFA::Transitions::const_iterator iterator = nfa.Q_transitions.begin();
        if ((iterator = nfa.Q_transitions.find({state, a})) != nfa.Q_transitions.end())
            result.insert(iterator->second[0]);
    }
    return result;
}

DFA MRegex::convert_nfa_to_dfa(const NFA &nfa)
{
    DFA dfa = DFA();
    DFA::States q0 = MRegex::elipson_cloursers({0}, nfa);
    std::map<DFA::States, size_t> status_map = {{q0, 0}};
    std::queue<DFA::States> queue_status;
    queue_status.push(q0);
    dfa.begin_Q_dfa.insert(0);
    dfa.dictionary = nfa.dictionary;
    dfa.Q_dfa.push_back(q0);

    while (!queue_status.empty())
    {
        DFA::States index = queue_status.front();
        queue_status.pop();
        size_t actual = status_map[index];

        for (auto &&state : index)
        {
            auto fiterator = nfa.F_nfa.find(state);
            if (fiterator != nfa.F_nfa.end())
            {
                dfa.F_dfa.emplace(actual, fiterator->second);
                break;
            }
        }

        for (auto &&caraceter : nfa.dictionary)
        {
            DFA::States newindex = MRegex::move_elipson_cloursers(index, caraceter, nfa);
            if (newindex.empty())
                continue;
            if (status_map.count(newindex) < 1)
            {
                status_map[newindex] = dfa.Q_dfa.size();
                dfa.Q_dfa.push_back(newindex);
                queue_status.push(newindex);
            }
            dfa.Q_transitions[{actual, caraceter}] = status_map[newindex];
        }
    }
    return dfa;
}

TableDFA MRegex::convert_dfa_to_table(const DFA &dfa)
{
    size_t sizeAlphabet = std::pow(256, sizeof(char));
    TableDFA table = TableDFA(dfa.Q_dfa.size(), sizeAlphabet, dfa.F_dfa);
    for (size_t state = 0; state < dfa.Q_dfa.size(); state++)
    {
        for (size_t letter = 0; letter < sizeAlphabet; letter++)
        {
            auto transition = dfa.Q_transitions.find({state, char(letter)});
            if (transition != dfa.Q_transitions.end())
                table.Q_transitions[state][letter] = transition->second;
        }
    }
    return table;
}