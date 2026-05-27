#include <mregex.hpp>
#include <stack>
#include <queue>

void MRegex::_S_parser_cualifiquer(_C_String_Iterators<char> &range, NFA &nfa, size_t qA, bool sentenceGroup)
{
    // Por defecto las reglas tienen un minimo y un maximo de 1, una lectura normal
    if (range.peak() >= range.end())
        return;
    char c = *range.peak();
    switch (c)
    {
    case '?':
    {
        range.next();
        nfa.Q_transitions[{qA, -1ULL}].push_back(nfa.Q_nfa.size());
        break;
    }
    case '*':
    {
        range.next();
        if (sentenceGroup)
        {
            for (auto &&[key, state] : nfa.Q_transitions)
                if (key.first == qA)
                    nfa.Q_transitions[{nfa.Q_nfa.size(), key.second}] = state;
        }
        else
        {
            for (auto &&iterator : nfa.Q_transitions)
                if (iterator.first.first == qA)
                    iterator.second[0] = qA;
        }
        nfa.Q_transitions[{qA, -1ULL}].push_back(nfa.Q_nfa.size());
        break;
    }
    case '+':
    {
        range.next();
        if (sentenceGroup)
        {
            for (auto &&[key, state] : nfa.Q_transitions)
                if (key.first == qA)
                    nfa.Q_transitions[{nfa.Q_nfa.size(), key.second}] = state;
        }
        else
        {
            size_t nqA = nfa.Q_nfa.size();
            for (auto &&[key, value] : nfa.Q_transitions)
                if (key.first == qA)
                    nfa.Q_transitions[{nqA, key.second}].push_back(nqA);
        }
        break;
    }
    default:
        break;
    }
}


size_t MRegex::_S_parser_nfa_parser_class_expresions(_C_String_Iterators<char> &range, NFA &nfa)
{
    size_t qA = nfa.Q_nfa.size();
    nfa.Q_nfa.push_back(qA);
    // Chequeamos si es una clase invertida
    if (*range.peak() == '^')
    {
        // Si lo es hacer operaciones de inversion, por ahora no se ha implementado
        //  _class._M_inverter = true;
        range.next();
    }
    char a;
    // Recorremos la cadena hasta llegar el final
    while (range.peak() < range.end() and (a = *range.peak()) != ']')
    {
        // if (a == '\\')
        //     a = _regex::_S_parser_escape(range);
        // Si se cumple la siguiente condicion solo se agrgara una transicion
        if (range.offset(1) >= range.end() or *(range.offset(1)) != '-')
        {
            nfa.dictionary.insert(a);
            nfa.Q_transitions[{qA, a}].push_back(nfa.Q_nfa.size());
        }
        else
        {
            // En caso contrario obtenemos el rango de transiciones
            auto offset = range.offset(2);
            char b;
            if (offset >= range.end() or (b = *offset) == ']')
                throw std::runtime_error("Error: termination max incurret");
            for (int i = a; i <= b; i++)
            {
                nfa.dictionary.insert(char(i));
                nfa.Q_transitions[{qA, char(i)}].push_back(nfa.Q_nfa.size());
            }
            range.next();
            range.next();
        }
        range.next();
    }
    return qA;
}


size_t MRegex::_S_build_nfa_parser_regular_expresions_basic(_C_String_Iterators<char>& range, NFA &nfa, bool isGroup)
{
    // Dado el rango de caracteres
    while (range.peak() < range.end())
    {
        // Obtenemos el caracter actual
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
            size_t qA = MRegex::_S_build_nfa_parser_or_expresions(range, nfa, true);
            // Si no termina en la posicion correcta, error
            if (range.peak() >= range.end() or *range.peak() != ')')
                throw std::runtime_error("Error: Bad termination in group");
            // Saltamos el final del grupo
            range.next();
            // Y agregamos su respectivo cualificador
            MRegex::_S_parser_cualifiquer(range, nfa, qA, false);
            break;
        }
        case '[':
        {
            // Genera un estado para clases de caracteres
            size_t qA = MRegex::_S_parser_nfa_parser_class_expresions(range, nfa);
            // Si no termina en la posicion correcta, error
            if (range.peak() >= range.end() or *range.peak() != ']')
                throw std::runtime_error("Error: Bad termination in class");
            // Saltamos el final del grupo
            range.next();
            // Y agregamos su respectivo cualificador
            MRegex::_S_parser_cualifiquer(range, nfa, qA, false);
            break;
        }
        default:
        {
            // Simplemente creamos el estado y agregamos la trsnicion
            size_t qA = nfa.Q_nfa.size();
            nfa.dictionary.insert(c);
            nfa.Q_nfa.push_back(qA);
            nfa.Q_transitions[{qA, c}].push_back(nfa.Q_nfa.size());
            // Y agregamos su respectivo cualificador
            MRegex::_S_parser_cualifiquer(range, nfa, qA, false);
            break;
        }
        }
    }
    size_t qEnd = nfa.Q_nfa.size();
    nfa.Q_nfa.push_back(qEnd);
    return qEnd;
}

size_t MRegex::_S_build_nfa_parser_or_expresions(_C_String_Iterators<char>& range, NFA &nfa, bool isGroup)
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
    return q0;
}



// static NFA build_Nfa(const std::initializer_list<std::string> &list)
NFA MRegex::build_nfa(const std::string &str)
{
    _C_String_Iterators<char> range = str;
    NFA nfa;
    nfa.begin_Q_nfa.push_back(MRegex::_S_build_nfa_parser_or_expresions(range, nfa, false));
    return nfa;
}
