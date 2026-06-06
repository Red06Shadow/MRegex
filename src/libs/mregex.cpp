#include <mregex.hpp>
#include <stack>
#include <queue>
#include <unordered_set>

char MRegex::_S_scape_code_caracter(_C_String_Iterators<char> &range)
{
    char c;
    if (range.peak() >= range.end())
        throw std::runtime_error("Error: end-caption caracter especial unespect");
    switch (c = *range.peak())
    {
    case 'n':
        return '\n';
        break;
    case 't':
        return '\t';
        break;
    case 'v':
        return '\v';
        break;
    case 'f':
        return '\f';
        break;
    default:
        return c;
        break;
    }
}

void MRegex::_S_parser_cualifiquer(_C_String_Iterators<char> &range, NFA &nfa, const MRegex::_T_nfa_return_transitions &_T_qAtr, bool isGroup)
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
        // if (!isGroup)
        //     for (auto &&transition : _T_qAtr.second)
        //         nfa.Q_transitions[{_T_qAtr.first, transition}].push_back(nfa.Q_nfa.size());
        // nfa.Q_transitions[{nfa.Q_nfa.size(), -1ULL}].push_back(_T_qAtr.first);
        // luego se agrega la trasicion de la opcionalidad
        nfa.Q_transitions[{_T_qAtr.first, -1ULL}].push_back(nfa.Q_nfa.size());
        break;
    }
    case '+':
    {
        // Si se cumple se agrega la repeticion de cero a multiples veces
        range.next();
        // Si es un grupo las transiciones de repeticion se realizan desde el final hasta el principio
        // if (isGroup)
        //     for (auto &&transition : _T_qAtr.second)
        //         nfa.Q_transitions[{nfa.Q_nfa.size(), transition}].push_back(_T_qAtr.first);
        // else // si no
        // {
        //     for (auto &&transition : _T_qAtr.second)
        //     {
        //         // se agregan las transiciones hasta el estado de 'aceptacion' del repetidor ( para que se cumpla una vez )
        //         nfa.Q_transitions[{_T_qAtr.first, transition}].push_back(nfa.Q_nfa.size());
        //         // Y luego la repitencia sobre ese estado
        //         nfa.Q_transitions[{nfa.Q_nfa.size(), transition}].push_back(nfa.Q_nfa.size());
        //     }
        // }
        if (!isGroup)
            for (auto &&transition : _T_qAtr.second)
                nfa.Q_transitions[{_T_qAtr.first, transition}].push_back(nfa.Q_nfa.size());
        nfa.Q_transitions[{nfa.Q_nfa.size(), -1ULL}].push_back(_T_qAtr.first);
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

MRegex::_T_nfa_return_transitions MRegex::_S_parser_nfa_parser_class_expresions(_C_String_Iterators<char> &range, NFA &nfa)
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
            a = MRegex::_S_scape_code_caracter(range);
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
                b = MRegex::_S_scape_code_caracter(range);
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

size_t MRegex::_S_build_nfa_parser_regular_expresions_basic(_C_String_Iterators<char> &range, NFA &nfa, bool isGroup)
{
    // Dado el rango de caracteres
    while (range.peak() < range.end())
    {
        // Obtenemos el caracter actual
        MRegex::_T_nfa_return_transitions _T_qAtr;
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
            _T_qAtr = MRegex::_S_build_nfa_parser_or_expresions(range, nfa, true);
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
            c = MRegex::_S_scape_code_caracter(range);
            range.next();
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

MRegex::_T_nfa_return_transitions MRegex::_S_build_nfa_parser_or_expresions(_C_String_Iterators<char> &range, NFA &nfa, bool isGroup)
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
    ends.push_back(MRegex::_S_build_nfa_parser_regular_expresions_basic(range, nfa, isGroup));
    // Repite la condicion en caso de ser una operacion or
    while (range.peak() < range.end() && *(range.peak()) == '|')
    {
        range.next();
        // Agregamos una ∊-transition hacia ese estado
        // nfa.Q_transitions[{q0, -1ULL}] = nfa.Q_nfa.size();
        nfa.Q_transitions[{q0, -1ULL}].push_back(nfa.Q_nfa.size());
        // Guardamos el estado final del resultado de la cadena
        ends.push_back(MRegex::_S_build_nfa_parser_regular_expresions_basic(range, nfa, isGroup));
        // Si se detuvo en una tuberia volver a realizar el procedimiento
    }
    // Para cada estado final se le agregara una ∊-transition al estado de union de la tuberia
    for (auto &&i : ends)
        nfa.Q_transitions[{i, -1ULL}].push_back(nfa.Q_nfa.size());
    // Por ultimo generamos correctamente el estado
    if (!isGroup)
    {
        nfa.Q_nfa.push_back(nfa.Q_nfa.size());
        nfa.F_nfa.insert(nfa.Q_nfa.size() - 1);
    }
    return {q0, {-1ULL}};
}

NFA MRegex::build_nfa(const std::__cxx11::basic_string<char> &str)
{
    _C_String_Iterators<char> range = str;
    NFA nfa;
    nfa.begin_Q_nfa.push_back(MRegex::_S_build_nfa_parser_or_expresions(range, nfa, false).first);
    return nfa;
}

NFA MRegex::build_nfa(const std::initializer_list<std::__cxx11::basic_string<char>> &list)
{
    NFA nfa;
    nfa.Q_nfa.push_back(0);
    nfa.begin_Q_nfa.push_back(0);
    for (auto &&expresion : list)
    {
        _C_String_Iterators<char> range = expresion;
        nfa.Q_transitions[{0, -1ULL}].push_back(MRegex::_S_build_nfa_parser_or_expresions(range, nfa, false).first);
    }
    return nfa;
}

NFA MRegex::build_nfa(const char *argv[], size_t size)
{
    NFA nfa;
    nfa.Q_nfa.push_back(0);
    nfa.begin_Q_nfa.push_back(0);
    for (size_t i = 1; i < size; i++)
    {
        _C_String_Iterators<char> range = std::string(argv[i]);
        nfa.Q_transitions[{0, -1ULL}].push_back(MRegex::_S_build_nfa_parser_or_expresions(range, nfa, false).first);
    }
    return nfa;
}

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
            if (nfa.F_nfa.count(state) > 0)
            {
                dfa.F_dfa.insert(actual);
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