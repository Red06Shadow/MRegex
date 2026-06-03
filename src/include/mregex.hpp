#ifndef MREGEX
#define MREGEX

#include "nfa.hpp"
#include "dfa.hpp"
#include "iterators.hpp"


class MRegex
{
private: // Funtions
    using _T_nfa_return_transitions = std::pair<size_t, std::vector<size_t>>;

    static char _S_scape_code_caracter(_C_String_Iterators<char> &range);
    static void _S_parser_cualifiquer(_C_String_Iterators<char> &range, NFA &nfa, const _T_nfa_return_transitions& _T_qAtr, bool sentenceGroup);
    static _T_nfa_return_transitions _S_parser_nfa_parser_class_expresions(_C_String_Iterators<char> &rang, NFA &nfa);
    static size_t _S_build_nfa_parser_regular_expresions_basic(_C_String_Iterators<char>  &range, NFA &nfa, bool isGroup);
    static _T_nfa_return_transitions _S_build_nfa_parser_or_expresions(_C_String_Iterators<char> &range, NFA &nfa, bool isGroup);
public: // Funtions
    MRegex() {}
    static NFA build_nfa(const std::__cxx11::basic_string<char> &str);
    ~MRegex() {}
};
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
#endif