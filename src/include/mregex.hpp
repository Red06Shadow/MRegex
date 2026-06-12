#ifndef MREGEX
#define MREGEX

#include "nfa.hpp"
#include "dfa.hpp"
#include "iterators.hpp"
#include "table.hpp"

#include <stack>
#include <queue>
#include <unordered_set>
#include <math.h>

#define TESTCODE false

enum Selector : unsigned char
{
    first_sequence,
    verification,
    maximun_munch
};

template <typename CharT>
class MRegex
{
public: // enums, structs
    static_assert(std::is_same_v<CharT, char> || std::is_same_v<CharT, wchar_t>, "Error: no se permiten tipos de datos que no sean de caracteres(solo char o wchar_t)");

private: // enums, structs
    using pair_begin_state_transitions = std::pair<size_t, std::vector<size_t>>;
    using range_cualifiquer = std::pair<size_t, size_t>;

private: // Funtions regex -> NFA
    // Funciones para el parseo de expresiones regex a NFA
    static CharT _S_parser_code_caracter_hexunicode(basic_string_range<CharT> &, const unsigned char);
    static CharT _S_parser_code_caracter_octal(basic_string_range<CharT> &);
    static CharT _S_parser_code_caracter(basic_string_range<CharT> &);
    static range_cualifiquer _S_parser_cualifiquer_range(basic_string_range<CharT> &);
    static void _S_parser_cualifiquer(basic_string_range<CharT> &, NFA<CharT> &, const pair_begin_state_transitions &, bool);
    static pair_begin_state_transitions _S_parser_nfa_parser_class_expresions(basic_string_range<CharT> &, NFA<CharT> &);
    static size_t _S_build_nfa_parser_regular_expresions_basic(basic_string_range<CharT> &, NFA<CharT> &, bool, size_t);
    static pair_begin_state_transitions _S_build_nfa_parser_or_expresions(basic_string_range<CharT> &, NFA<CharT> &, bool, size_t);

private: // Funtions NFA -> DFA
    // Funciones adicionales del convertidor de NFA a DFA
    static typename DFA<CharT>::States _S_elipson_cloursers(typename DFA<CharT>::States, const NFA<CharT> &);
    static typename DFA<CharT>::States _S_move(typename DFA<CharT>::States, CharT, const NFA<CharT> &);
    inline static typename DFA<CharT>::States _S_move_elipson_cloursers(typename DFA<CharT>::States states, CharT a, const NFA<CharT> &nfa) { return _S_elipson_cloursers(_S_move(states, a, nfa), nfa); }

public: // Funtions
    /// @brief Genera un NFA basado en una cadena de caracteres que represente un regex
    /// @param str cadena de caracteres
    /// @return Devuelve el nfa generado
    static NFA<CharT> build_nfa(const std::__cxx11::basic_string<CharT> &str);
#if TESTCODE
    /// @brief Genera un NFA basado en una losta de cadenas de caracteres que representen las expresiones regulares (Nota: Esta funcion es para usarla junto a los argumentos de la funcion main)
    /// @param argv arreglo de cadenas de caracteres
    /// @param size cantidad de elementos dentro del arreglo
    /// @return Devuelve el nfa resultante
    static NFA<CharT> build_nfa(const CharT *[], size_t);
#endif
    /// @brief Genera un NFA basado en una losta de cadenas de caracteres que representen las expresiones regulares (Nota: Esta funcion es para usarla junto a los argumentos de la funcion main)
    /// @param list lista de cadenas de caracteres
    /// @return Devuelve el nfa resultante
    static NFA<CharT> build_nfa(const std::initializer_list<std::pair<size_t, std::__cxx11::basic_string<CharT>>> &list);
    /// @brief Convierte el NFA a DFA
    /// @param nfa
    /// @return Devuelve el dfa obtenido a partir del nfa
    static DFA<CharT> convert_nfa_to_dfa(const NFA<CharT> &nfa);
    /// @brief Convierte el DFA a una tabla de transiciones de DFA
    /// @param dfa
    /// @return Devuelve la tabla dfa
    static TableDFA convert_dfa_to_table(const DFA<CharT> &dfa);
    /// @brief Captador de cadenas
    /// @param str Cadena a leer
    /// @param dfa DFA generado
    /// @return Retorna los iteradores de la cadena
    template <Selector S, typename T>
    static std::pair<const CharT *, size_t> caption(const std::__cxx11::basic_string<CharT> &str, const T &stru);
    template <Selector S, typename T>
    static std::pair<const CharT *, size_t> caption(basic_string_range<CharT> &range, const T &stru);
};

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

template <typename CharT>
CharT MRegex<CharT>::_S_parser_code_caracter_hexunicode(basic_string_range<CharT> &range, const unsigned char _S)
{
    CharT result;
    for (CharT i = 0; i < _S; i++)
    {
        if (range.peak() >= range.end())
            throw std::runtime_error("");
        CharT caracter = *range.peak();
        if (caracter >= CharT('a') && caracter <= CharT('f'))
            result = result * 16 + (caracter - CharT('a') + 10);
        else if (caracter >= CharT('A') && caracter <= CharT('F'))
            result = result * 16 + (caracter - CharT('A') + 10);
        else if (caracter >= CharT('0') && caracter <= CharT('9'))
            result = result * 16 + (caracter - CharT('0'));
        else
            throw std::runtime_error("");
        range.next();
    }
    return result;
}

template <typename CharT>
CharT MRegex<CharT>::_S_parser_code_caracter_octal(basic_string_range<CharT> &range)
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

template <typename CharT>
CharT MRegex<CharT>::_S_parser_code_caracter(basic_string_range<CharT> &range)
{
    CharT c;
    if (range.peak() >= range.end())
        throw std::runtime_error("Error: end-caption caracter especial unespect");
    switch (c = *range.peak())
    {
    case CharT('n'):
        range.next();
        return CharT('\n');
        break;
    case CharT('t'):
        range.next();
        return CharT('\t');
        break;
    case CharT('v'):
        range.next();
        return CharT('\v');
        break;
    case CharT('f'):
        range.next();
        return CharT('\f');
        break;
    case CharT('b'):
        range.next();
        return CharT('\b');
        break;
    case CharT('r'):
        range.next();
        return CharT('\r');
        break;
    case CharT('x'):
        range.next();
        return MRegex<CharT>::_S_parser_code_caracter_hexunicode(range, 2);
        break;
    case CharT('u'):
        range.next();
        return MRegex<CharT>::_S_parser_code_caracter_hexunicode(range, 4);
        break;
    case CharT('U'):
        range.next();
        return MRegex<CharT>::_S_parser_code_caracter_hexunicode(range, 8);
        break;
    default:
        if ('0' <= c && c <= '7')
            return MRegex::_S_parser_code_caracter_octal(range);
        range.next();
        return c;
        break;
    }
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////Parser////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

template <typename CharT>
typename MRegex<CharT>::range_cualifiquer MRegex<CharT>::_S_parser_cualifiquer_range(basic_string_range<CharT> &range)
{
    size_t n1 = 0;    // minimo
    size_t n2 = 0;    // maximo
    size_t *rn = &n1; // puntero al numero actual
    CharT caracter;
    bool invalid = true; // Determina si es valido el resultado
    // Recorremos la cadena de caracteres
    while (range.peak() < range.end() and (caracter = *range.peak()) != '}')
    {
        // Si es un numero
        if (CharT('0') <= caracter && caracter <= CharT('9'))
        {
            // Realizamos los calculos para la conversion
            *rn = *rn * 10 + caracter - CharT('0');
            // Y desactivamos la invalidacion
            invalid = false;
        }
        else if (CharT(',') == caracter) // Si es una coma
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

template <typename CharT>
void MRegex<CharT>::_S_parser_cualifiquer(basic_string_range<CharT> &range, NFA<CharT> &nfa, const MRegex<CharT>::pair_begin_state_transitions &_T_qAtr, bool isGroup)
{
    CharT c = 0;
    // Si el rango llega al final salta a la sentemcia por defecto del switch
    if (range.peak() >= range.end())
        goto labelForEnd;
    // Si no obtenemos el caracter en esa posicion
    c = *range.peak();
    switch (c)
    {
    case CharT('?'):
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
    case CharT('*'):
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
    case CharT('+'):
    {
        // Si se cumple se agrega la repeticion de cero a multiples veces
        range.next();
        if (!isGroup)
            for (auto &&transition : _T_qAtr.second)
                nfa.Q_transitions[{_T_qAtr.first, transition}].push_back(nfa.Q_nfa.size());
        nfa.Q_transitions[{nfa.Q_nfa.size(), -1ULL}].push_back(_T_qAtr.first);
        break;
    }
    case CharT('{'):
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

template <typename CharT>
typename MRegex<CharT>::pair_begin_state_transitions MRegex<CharT>::_S_parser_nfa_parser_class_expresions(basic_string_range<CharT> &range, NFA<CharT> &nfa)
{
    size_t qA = nfa.Q_nfa.size();
    std::vector<size_t> transitions = {};
    std::unordered_set<CharT> alphabet = {};
    bool inverter = false;

    nfa.Q_nfa.push_back(qA);
    // Chequeamos si es una clase invertida
    if (*range.peak() == CharT('^'))
    {
        // Si lo es hacer operaciones de inversion, por ahora no se ha implementado
        inverter = true;
        range.next();
    }
    CharT a;
    // Recorremos la cadena hasta llegar el final
    while (range.peak() < range.end() and (a = *range.peak()) != CharT(']'))
    {
        // Pasamos al siguiente valor
        range.next();
        if (a == CharT('\\'))                           // Si es un caracter de escape
            a = MRegex::_S_parser_code_caracter(range); // Ejecutar caracteres de escape

        // Si el iterador llego a su fin
        if (range.peak() >= range.end())
            throw std::runtime_error("Error: Bad termination in class");

        // Si el caracter proximo no es un caracter '-'
        if (*(range.peak()) != CharT('-'))
            alphabet.insert(a); // Solamente insertar el caracter actual
        else
        {
            // En caso contrario obtenemos el rango de transiciones
            auto offset = range.offset(1);
            char b;
            if (offset >= range.end() or (b = *offset) == CharT(']'))
                throw std::runtime_error("Error: termination max incurret");
            range.next();
            if (b == CharT('\\'))
                b = MRegex::_S_parser_code_caracter(range);
            else
                range.next();
            for (size_t i = a; i <= b; i++)
                alphabet.insert(CharT(i));
        }
    }
    // Si la clase esta invertida se agregan todos los caracteres que no fueron detectados dentro del su definicion
    if (inverter)
    {
        // Se recorre todos los caracteres
        for (size_t i = 0; i < size_t(std::pow(256, sizeof(CharT))); i++)
        {
            // Si el caracter no se encuentra dentro del conjunto
            if (alphabet.count(CharT(i)) < 1)
            {
                // Se agrega la transicion y luego el caracter al alfabeto
                transitions.push_back(CharT(i));
                nfa.dictionary.insert(CharT(i));
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

template <typename CharT>
size_t MRegex<CharT>::_S_build_nfa_parser_regular_expresions_basic(basic_string_range<CharT> &range, NFA<CharT> &nfa, bool isGroup, size_t id)
{
    // Dado el rango de caracteres
    while (range.peak() < range.end())
    {
        // Obtenemos el caracter actual
        MRegex::pair_begin_state_transitions _T_qAtr;
        CharT c = *range.peak();
        if (c == CharT('|') or c == CharT(')'))
            break;
        // Y pasamos al siguiente
        range.next();
        // Segun el caracter que sea
        switch (c)
        {
        case CharT('('):
        {
            // Genera un grupo de reglas, se utiliza la recursion de la fincion recursiva
            _T_qAtr = MRegex<CharT>::_S_build_nfa_parser_or_expresions(range, nfa, true, id);
            // Si no termina en la posicion correcta, error
            if (range.peak() >= range.end() or *range.peak() != CharT(')'))
                throw std::runtime_error("Error: Bad termination in group");
            // Saltamos el final del grupo
            range.next();
            // Y agregamos su respectivo cualificador
            MRegex<CharT>::_S_parser_cualifiquer(range, nfa, _T_qAtr, true);
            break;
        }
        case CharT('['):
        {
            // Genera un estado para clases de caracteres
            _T_qAtr = MRegex<CharT>::_S_parser_nfa_parser_class_expresions(range, nfa);
            // Si no termina en la posicion correcta, error
            if (range.peak() >= range.end() or *range.peak() != CharT(']'))
                throw std::runtime_error("Error: Bad termination in class");
            // Saltamos el final del grupo
            range.next();
            // Y agregamos su respectivo cualificador
            MRegex<CharT>::_S_parser_cualifiquer(range, nfa, _T_qAtr, false);
            break;
        }
        case CharT('\\'):
            c = MRegex<CharT>::_S_parser_code_caracter(range);
        default:
        {
            // Simplemente creamos el estado y agregamos la trsnicion
            size_t qA = nfa.Q_nfa.size();
            nfa.dictionary.insert(c);
            nfa.Q_nfa.push_back(qA);
            // Y agregamos su respectivo cualificador
            MRegex<CharT>::_S_parser_cualifiquer(range, nfa, {qA, {size_t(c)}}, false);
            break;
        }
        }
    }
    size_t qEnd = nfa.Q_nfa.size();
    nfa.Q_nfa.push_back(qEnd);
    return qEnd;
}

template <typename CharT>
typename MRegex<CharT>::pair_begin_state_transitions MRegex<CharT>::_S_build_nfa_parser_or_expresions(basic_string_range<CharT> &range, NFA<CharT> &nfa, bool isGroup, size_t id)
{
    // Preguntamos si el inicio es incorrect y lanzamos un error si es asi
    if (range.peak() == range.end() or *(range.peak()) == CharT('|'))
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
    ends.push_back(MRegex<CharT>::_S_build_nfa_parser_regular_expresions_basic(range, nfa, isGroup, id));
    // Repite la condicion en caso de ser una operacion or
    while (range.peak() < range.end() && *(range.peak()) == CharT('|'))
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

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////DFA////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

template <typename CharT>
typename DFA<CharT>::States MRegex<CharT>::_S_elipson_cloursers(typename DFA<CharT>::States states, const NFA<CharT> &nfa)
{
    std::stack<size_t> stack_status;
    typename DFA<CharT>::States clourser = states;
    for (auto &&state : states)
        stack_status.push(state);
    while (!stack_status.empty())
    {
        size_t index = stack_status.top();
        stack_status.pop();
        typename NFA<CharT>::Transitions::const_iterator iterator = nfa.Q_transitions.begin();
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

template <typename CharT>
typename DFA<CharT>::States MRegex<CharT>::_S_move(typename DFA<CharT>::States states, CharT a, const NFA<CharT> &nfa)
{
    typename DFA<CharT>::States result = {};
    for (auto &&state : states)
    {
        typename NFA<CharT>::Transitions::const_iterator iterator = nfa.Q_transitions.begin();
        if ((iterator = nfa.Q_transitions.find({state, a})) != nfa.Q_transitions.end())
            result.insert(iterator->second[0]);
    }
    return result;
}

template <typename CharT>
DFA<CharT> MRegex<CharT>::convert_nfa_to_dfa(const NFA<CharT> &nfa)
{
    DFA<CharT> dfa = DFA<CharT>();
    typename DFA<CharT>::States q0 = MRegex<CharT>::_S_elipson_cloursers({0}, nfa);
    std::map<typename DFA<CharT>::States, size_t> status_map = {{q0, 0}};
    std::queue<typename DFA<CharT>::States> queue_status;
    queue_status.push(q0);
    dfa.begin_Q_dfa.insert(0);
    dfa.dictionary = nfa.dictionary;
    dfa.Q_dfa.push_back(q0);

    while (!queue_status.empty())
    {
        typename DFA<CharT>::States index = queue_status.front();
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
            typename DFA<CharT>::States newindex = MRegex<CharT>::_S_move_elipson_cloursers(index, caraceter, nfa);
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

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////TableDFA//////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

template <typename CharT>
TableDFA MRegex<CharT>::convert_dfa_to_table(const DFA<CharT> &dfa)
{
    size_t sizeAlphabet = std::pow(256, sizeof(CharT));
    TableDFA table = TableDFA(dfa.Q_dfa.size(), sizeAlphabet, dfa.F_dfa);
    for (size_t state = 0; state < dfa.Q_dfa.size(); state++)
    {
        for (size_t letter = 0; letter < sizeAlphabet; letter++)
        {
            auto transition = dfa.Q_transitions.find({state, CharT(letter)});
            if (transition != dfa.Q_transitions.end())
                table.Q_transitions[state][letter] = transition->second;
        }
    }
    return table;
}
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////Regex////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

template <typename CharT>
NFA<CharT> MRegex<CharT>::build_nfa(const std::__cxx11::basic_string<CharT> &str)
{
    basic_string_range<CharT> range = str;
    NFA<CharT> nfa;
    nfa.begin_Q_nfa.push_back(MRegex<CharT>::_S_build_nfa_parser_or_expresions(range, nfa, false, 0).first);
    return nfa;
}

template <typename CharT>
NFA<CharT> MRegex<CharT>::build_nfa(const std::initializer_list<std::pair<size_t, std::__cxx11::basic_string<CharT>>> &list)
{
    NFA<CharT> nfa;
    nfa.Q_nfa.push_back(0);
    nfa.begin_Q_nfa.push_back(0);
    for (auto &&expresion : list)
    {
        basic_string_range<CharT> range = expresion.second;
        nfa.Q_transitions[{0, -1ULL}].push_back(MRegex::_S_build_nfa_parser_or_expresions(range, nfa, false, expresion.first).first);
    }
    return nfa;
}

#if TESTCODE
template <typename CharT>
NFA<CharT> MRegex<CharT>::build_nfa(const CharT *argv[], size_t size)
{
    NFA<CharT> nfa;
    nfa.Q_nfa.push_back(0);
    nfa.begin_Q_nfa.push_back(0);
    for (size_t i = 1; i < size; i++)
    {
        basic_string_range<CharT> range = std::string(argv[i]);
        nfa.Q_transitions[{0, -1ULL}].push_back(MRegex<CharT>::_S_build_nfa_parser_or_expresions(range, nfa, false).first);
    }
    return nfa;
}
#endif

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////Caption///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

template <typename CharT>
template <Selector S, typename T>
std::pair<const CharT *, size_t> MRegex<CharT>::caption(const std::__cxx11::basic_string<CharT> &str, const T &stru)
{
    size_t status = 0;
    size_t acceptance_status = -1ULL;
    size_t id = -1ULL;
    std::__cxx11::basic_string<char>::const_iterator end = str.begin();

    if constexpr (!std::is_same_v<T, DFA<CharT>> && !std::is_same_v<T, TableDFA>)
        throw std::runtime_error("");

    for (std::__cxx11::basic_string<char>::const_iterator iterator_caption = str.begin(); iterator_caption < str.end(); iterator_caption++)
    {
        char letter = *iterator_caption;
        if constexpr (std::is_same_v<T, DFA<CharT>>)
        {
            auto transition = stru.Q_transitions.find({status, letter});
            if (transition == stru.Q_transitions.end())
                break; // No hay transición, rechazar
            status = transition->second;
        }
        else if constexpr (std::is_same_v<T, TableDFA>)
        {
            size_t next_state = stru.Q_transitions[status][letter];
            if (next_state == -1ULL)
                break; // No hay transición, rechazar
            status = next_state;
        }
        if constexpr (S == Selector::maximun_munch || S == Selector::first_sequence)
        {
            if (status == acceptance_status)
                end = iterator_caption + 1;
            else if (stru.F_dfa.count(status) > 0)
            {
                end = iterator_caption + 1;
                acceptance_status = status;
                if constexpr (std::is_same_v<T, DFA<CharT>> || std::is_same_v<T, TableDFA>)
                    id = stru.F_dfa.at(acceptance_status);
            }
            if constexpr (S == Selector::first_sequence)
                break;
        }
    }
    if constexpr (S == Selector::maximun_munch)
        return {acceptance_status != -1ULL ? end.base() : nullptr, id};
    else if constexpr (S == Selector::first_sequence)
        return {stru.F_dfa.count(status) > 0 ? end.base() : nullptr, id};
    else
        return {(end == str.end() && stru.F_dfa.count(status) > 0) ? end.base() : nullptr, id};
}

template <typename CharT>
template <Selector S, typename T>
std::pair<const CharT *, size_t> MRegex<CharT>::caption(basic_string_range<CharT> &range, const T &stru)
{
    size_t status = 0;
    size_t acceptance_status = -1ULL;
    size_t id = -1ULL;
    basic_string_range<char>::iterator end = range.begin();

    if constexpr (!std::is_same_v<T, DFA<CharT>> && !std::is_same_v<T, TableDFA>)
        throw std::runtime_error("");
    char letter;
    while (range.peak() < range.end())
    {
        letter = *range.peak();
        // Code
        if constexpr (std::is_same_v<T, DFA<CharT>>)
        {
            auto transition = stru.Q_transitions.find({status, letter});
            if (transition == stru.Q_transitions.end())
                break; // No hay transición, rechazar
            status = transition->second;
        }
        else if constexpr (std::is_same_v<T, TableDFA>)
        {
            size_t next_state = stru.Q_transitions[status][letter];
            if (next_state == -1ULL)
                break; // No hay transición, rechazar
            status = next_state;
        }
        if constexpr (S == Selector::maximun_munch || S == Selector::first_sequence)
        {
            if (status == acceptance_status)
                end = range.peak() + 1;
            else if (stru.F_dfa.count(status) > 0)
            {
                end = range.peak() + 1;
                acceptance_status = status;
                if constexpr (std::is_same_v<T, DFA<CharT>> || std::is_same_v<T, TableDFA>)
                    id = stru.F_dfa.at(acceptance_status);
            }
            if constexpr (S == Selector::first_sequence)
                break;
        }
        // Code
        range.next();
    }
    if constexpr (S == Selector::maximun_munch)
        return {acceptance_status != -1ULL ? end.base() : nullptr, id};
    else if constexpr (S == Selector::first_sequence)
        return {stru.F_dfa.count(status) > 0 ? end.base() : nullptr, id};
    else
        return {(end == range.end() && stru.F_dfa.count(status) > 0) ? end.base() : nullptr, id};
}

#endif