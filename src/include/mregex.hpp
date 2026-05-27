#ifndef MREGEX
#define MREGEX

#include "nfa.hpp"
#include "dfa.hpp"
#include "iterators.hpp"


class MRegex
{
private: // Funtions
    static void _S_parser_cualifiquer(_C_String_Iterators<char> &range, NFA &nfa, size_t qA, bool sentenceGroup);
    static size_t _S_parser_nfa_parser_class_expresions(_C_String_Iterators<char> &rang, NFA &nfa);
    static size_t _S_build_nfa_parser_regular_expresions_basic(_C_String_Iterators<char>  &range, NFA &nfa, bool isGroup);
    static size_t _S_build_nfa_parser_or_expresions(_C_String_Iterators<char> &range, NFA &nfa, bool isGroup);
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