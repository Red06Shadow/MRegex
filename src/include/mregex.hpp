#ifndef MREGEX
#define MREGEX

#include "nfa.hpp"
#include "dfa.hpp"
#include "iterators.hpp"


class MRegex
{
private: // Funtions
    using _T_nfa_return_transitions = std::pair<size_t, std::vector<size_t>>;

    // Funciones para el parseo de expresiones regex a NFA
    static char _S_scape_code_caracter(_C_String_Iterators<char>&);
    static void _S_parser_cualifiquer(_C_String_Iterators<char>&, NFA&, const _T_nfa_return_transitions&, bool);
    static _T_nfa_return_transitions _S_parser_nfa_parser_class_expresions(_C_String_Iterators<char>&, NFA&);
    static size_t _S_build_nfa_parser_regular_expresions_basic(_C_String_Iterators<char>&, NFA&, bool);
    static _T_nfa_return_transitions _S_build_nfa_parser_or_expresions(_C_String_Iterators<char>&, NFA&, bool);

    // Funciones adicionales del convertidor de NFA a DFA
    static std::set<size_t> elipson_cloursers(std::set<size_t>, const NFA&);
    static std::set<size_t> move(std::set<size_t>, char, const NFA&);
    inline static std::set<size_t> move_elipson_cloursers(std::set<size_t> states, char a, const NFA& nfa) { return elipson_cloursers(move(states, a, nfa), nfa);}

    // Fusionador de Automatas
    static void _S_get_transitions_nfa(const NFA& nfa, std::vector<std::pair<size_t, std::vector<size_t>>>& nfa_transitions, size_t status_actual);

public: // Funtions
    /// @brief Genera un NFA basado en una cadena de caracteres que represente un regex
    /// @param str cadena de caracteres
    /// @return Devuelve el nfa generado
    static NFA build_nfa(const std::__cxx11::basic_string<char> &str);
    /// @brief Genera un NFA basado en una losta de cadenas de caracteres que representen las expresiones regulares
    /// @param  
    /// @param  
    /// @return 
    static NFA build_nfa(const char *[], size_t);
    static NFA build_nfa(const std::initializer_list<std::__cxx11::basic_string<char>> &list);
    /// @brief Convierte el NFA a DFA
    /// @param nfa
    /// @return Devuelve el dfa obtenido a partir del nfa
    static DFA convert_nfa_to_dfa(const NFA& nfa);
};
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
#endif