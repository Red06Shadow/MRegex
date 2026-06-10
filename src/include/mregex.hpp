#ifndef MREGEX
#define MREGEX

#include "nfa.hpp"
#include "dfa.hpp"
#include "iterators.hpp"
#include "table.hpp"

#define TESTCODE false

class MRegex
{
public: // enums, structs
    enum Selector : unsigned char
    {
        _first_sequence,
        _verification,
        _maximun_munch
    };

private: // enums, structs
    using pair_begin_state_transitions = std::pair<size_t, std::vector<size_t>>;
    using range_cualifiquer = std::pair<size_t, size_t>;

private: // Funtions
    // Funciones para el parseo de expresiones regex a NFA
    template <char Selector>
    static char _S_parser_code_caracter_hexunicode(basic_string_range<char> &);
    static char _S_parser_code_caracter_octal(basic_string_range<char> &);
    static char _S_parser_code_caracter(basic_string_range<char> &);
    static range_cualifiquer _S_parser_cualifiquer_range(basic_string_range<char> &);
    static void _S_parser_cualifiquer(basic_string_range<char> &, NFA &, const pair_begin_state_transitions &, bool);
    static pair_begin_state_transitions _S_parser_nfa_parser_class_expresions(basic_string_range<char> &, NFA &);
    static size_t _S_build_nfa_parser_regular_expresions_basic(basic_string_range<char> &, NFA &, bool, size_t);
    static pair_begin_state_transitions _S_build_nfa_parser_or_expresions(basic_string_range<char> &, NFA &, bool, size_t);

private: // Funtions
    // Funciones adicionales del convertidor de NFA a DFA
    static DFA::States elipson_cloursers(DFA::States, const NFA &);
    static DFA::States move(DFA::States, char, const NFA &);
    inline static DFA::States move_elipson_cloursers(DFA::States states, char a, const NFA &nfa) { return elipson_cloursers(move(states, a, nfa), nfa); }

public: // Funtions
    /// @brief Genera un NFA basado en una cadena de caracteres que represente un regex
    /// @param str cadena de caracteres
    /// @return Devuelve el nfa generado
    static NFA build_nfa(const std::__cxx11::basic_string<char> &str);
#if TESTCODE
    /// @brief Genera un NFA basado en una losta de cadenas de caracteres que representen las expresiones regulares (Nota: Esta funcion es para usarla junto a los argumentos de la funcion main)
    /// @param argv arreglo de cadenas de caracteres
    /// @param size cantidad de elementos dentro del arreglo
    /// @return Devuelve el nfa resultante
    static NFA build_nfa(const char *[], size_t);
#endif
    /// @brief Genera un NFA basado en una losta de cadenas de caracteres que representen las expresiones regulares (Nota: Esta funcion es para usarla junto a los argumentos de la funcion main)
    /// @param list lista de cadenas de caracteres
    /// @return Devuelve el nfa resultante
    static NFA build_nfa(const std::initializer_list<std::pair<size_t, std::__cxx11::basic_string<char>>> &list);
    /// @brief Convierte el NFA a DFA
    /// @param nfa
    /// @return Devuelve el dfa obtenido a partir del nfa
    static DFA convert_nfa_to_dfa(const NFA &nfa);
    /// @brief Convierte el DFA a una tabla de transiciones de DFA
    /// @param dfa
    /// @return Devuelve la tabla dfa
    static TableDFA convert_dfa_to_table(const DFA &dfa);
    /// @brief Captador de cadenas
    /// @param str Cadena a leer
    /// @param dfa DFA generado
    /// @return Retorna los iteradores de la cadena
    template <Selector S, typename T>
    static std::pair<const char *, size_t> caption(const std::__cxx11::basic_string<char> &str, const T &stru);
    template <Selector S, typename T>
    static std::pair<const char *, size_t> caption(basic_string_range<char> &range, const T &stru);
};

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

template <char Selector>
char MRegex::_S_parser_code_caracter_hexunicode(basic_string_range<char> &range)
{
    char result;
    for (char i = 0; i < Selector; i++)
    {
        if (range.peak() >= range.end())
            throw std::runtime_error("");
        char caracter = *range.peak();
        if (caracter >= 'a' && caracter <= 'f')
            result = result * 16 + (caracter - 'a' + 10);
        else if (caracter >= 'A' && caracter <= 'F')
            result = result * 16 + (caracter - 'A' + 10);
        else if (caracter >= '0' && caracter <= '9')
            result = result * 16 + (caracter - '0');
        else
            throw std::runtime_error("");
        range.next();
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

template <MRegex::Selector S, typename T>
std::pair<const char *, size_t> MRegex::caption(const std::__cxx11::basic_string<char> &str, const T &stru)
{
    size_t status = 0;
    size_t acceptance_status = -1ULL;
    size_t id = -1ULL;
    std::__cxx11::basic_string<char>::const_iterator end = str.begin();

    if constexpr (!std::is_same_v<T, DFA> && !std::is_same_v<T, TableDFA>)
        throw std::runtime_error("");

    for (std::__cxx11::basic_string<char>::const_iterator iterator_caption = str.begin(); iterator_caption < str.end(); iterator_caption++)
    {
        char letter = *iterator_caption;
        if constexpr (std::is_same_v<T, DFA>)
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
        if constexpr (S == MRegex::Selector::_maximun_munch || S == MRegex::Selector::_first_sequence)
        {
            if (status == acceptance_status)
                end = iterator_caption + 1;
            else if (stru.F_dfa.count(status) > 0)
            {
                end = iterator_caption + 1;
                acceptance_status = status;
                if constexpr (std::is_same_v<T, DFA> || std::is_same_v<T, TableDFA>)
                    id = stru.F_dfa.at(acceptance_status);
            }
            if constexpr (S == MRegex::Selector::_first_sequence)
                break;
        }
    }
    if constexpr (S == MRegex::Selector::_maximun_munch)
        return {acceptance_status != -1ULL ? end.base() : nullptr, id};
    else if constexpr (S == MRegex::Selector::_first_sequence)
        return {stru.F_dfa.count(status) > 0 ? end.base() : nullptr, id};
    else
        return {(end == str.end() && stru.F_dfa.count(status) > 0) ? end.base() : nullptr, id};
}

template <MRegex::Selector S, typename T>
std::pair<const char *, size_t> MRegex::caption(basic_string_range<char> &range, const T &stru)
{
    size_t status = 0;
    size_t acceptance_status = -1ULL;
    size_t id = -1ULL;
    basic_string_range<char>::iterator end = range.begin();

    if constexpr (!std::is_same_v<T, DFA> && !std::is_same_v<T, TableDFA>)
        throw std::runtime_error("");
    char letter;
    while (range.peak() < range.end())
    {
        letter = *range.peak();
        // Code
        if constexpr (std::is_same_v<T, DFA>)
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
        if constexpr (S == MRegex::Selector::_maximun_munch || S == MRegex::Selector::_first_sequence)
        {
            if (status == acceptance_status)
                end = range.peak() + 1;
            else if (stru.F_dfa.count(status) > 0)
            {
                end = range.peak() + 1;
                acceptance_status = status;
                if constexpr (std::is_same_v<T, DFA> || std::is_same_v<T, TableDFA>)
                    id = stru.F_dfa.at(acceptance_status);
            }
            if constexpr (S == MRegex::Selector::_first_sequence)
                break;
        }
        // Code
        range.next();
    }
    if constexpr (S == MRegex::Selector::_maximun_munch)
        return {acceptance_status != -1ULL ? end.base() : nullptr, id};
    else if constexpr (S == MRegex::Selector::_first_sequence)
        return {stru.F_dfa.count(status) > 0 ? end.base() : nullptr, id};
    else
        return {(end == range.end() && stru.F_dfa.count(status) > 0) ? end.base() : nullptr, id};
}

#endif