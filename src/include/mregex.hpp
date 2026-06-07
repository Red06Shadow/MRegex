#ifndef MREGEX
#define MREGEX

#include "nfa.hpp"
#include "dfa.hpp"
#include "iterators.hpp"
#include "table.hpp"

class MRegex
{
public: // enums, structs
    using _T_nfa_return_transitions = std::pair<size_t, std::vector<size_t>>;

    enum Selector : unsigned char
    {
        _first_sequence,
        _verification,
        _maximun_munch
    };

private: // Funtions
    // Funciones para el parseo de expresiones regex a NFA
    static char _S_scape_code_caracter(_C_String_Iterators<char> &);
    static void _S_parser_cualifiquer(_C_String_Iterators<char> &, NFA &, const _T_nfa_return_transitions &, bool);
    static _T_nfa_return_transitions _S_parser_nfa_parser_class_expresions(_C_String_Iterators<char> &, NFA &);
    static size_t _S_build_nfa_parser_regular_expresions_basic(_C_String_Iterators<char> &, NFA &, bool);
    static _T_nfa_return_transitions _S_build_nfa_parser_or_expresions(_C_String_Iterators<char> &, NFA &, bool);

    // Funciones adicionales del convertidor de NFA a DFA
    static DFA::States elipson_cloursers(DFA::States, const NFA &);
    static DFA::States move(DFA::States, char, const NFA &);
    inline static DFA::States move_elipson_cloursers(DFA::States states, char a, const NFA &nfa) { return elipson_cloursers(move(states, a, nfa), nfa); }

public: // Funtions
    /// @brief Genera un NFA basado en una cadena de caracteres que represente un regex
    /// @param str cadena de caracteres
    /// @return Devuelve el nfa generado
    static NFA build_nfa(const std::__cxx11::basic_string<char> &str);
    /// @brief Genera un NFA basado en una losta de cadenas de caracteres que representen las expresiones regulares (Nota: Esta funcion es para usarla junto a los argumentos de la funcion main)
    /// @param argv arreglo de cadenas de caracteres
    /// @param size cantidad de elementos dentro del arreglo
    /// @return Devuelve el nfa resultante
    static NFA build_nfa(const char *[], size_t);
    /// @brief Genera un NFA basado en una losta de cadenas de caracteres que representen las expresiones regulares (Nota: Esta funcion es para usarla junto a los argumentos de la funcion main)
    /// @param list lista de cadenas de caracteres
    /// @return Devuelve el nfa resultante
    static NFA build_nfa(const std::initializer_list<std::__cxx11::basic_string<char>> &list);
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
    static std::pair<const char *, const char *> caption(const std::__cxx11::basic_string<char> &str, const T &stru);

    // template <Selector T>
    // static std::pair<const char *, const char *> caption(const std::__cxx11::basic_string<char> &str, const TableDFA &table);
};
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

template <MRegex::Selector S, typename T>
std::pair<const char *, const char *> MRegex::caption(const std::__cxx11::basic_string<char> &str, const T &stru)
{
    size_t state = 0;
    size_t acept_state = -1ULL;
    std::string::const_iterator iterator_end = str.begin();

    if constexpr (!std::is_same_v<T, DFA> && !std::is_same_v<T, TableDFA>)
        throw std::runtime_error("");

    for (std::string::const_iterator iterator_caption = str.begin(); iterator_caption < str.end(); iterator_caption++)
    {
        char letter = *iterator_caption;

        if constexpr (S == MRegex::Selector::_maximun_munch || S == MRegex::Selector::_first_sequence)
        {
            if (stru.F_dfa.count(state) > 0)
                acept_state = state;
            if constexpr (S == MRegex::Selector::_first_sequence)
                break;
        }

        if constexpr (std::is_same_v<T, DFA>)
        {
            auto transition = stru.Q_transitions.find({state, letter});
            if (transition == stru.Q_transitions.end())
                break; // No hay transición, rechazar
            state = transition->second;
        }
        else if constexpr (std::is_same_v<T, TableDFA>)
        {
            size_t next_state = stru.Q_transitions[state][letter];
            if (next_state == -1ULL)
                break; // No hay transición, rechazar
            state = next_state;
        }

        iterator_end++;
    }
    if constexpr (S == MRegex::Selector::_maximun_munch)
        return {str.begin().base(), acept_state != -1ULL ? iterator_end.base() : nullptr};
    else if constexpr (S == MRegex::Selector::_first_sequence)
        return {str.begin().base(), stru.F_dfa.count(state) > 0 ? iterator_end.base() : nullptr};
    else
        return {str.begin().base(), (iterator_end == str.end() && stru.F_dfa.count(state) > 0) ? iterator_end.base() : nullptr};
}

#endif