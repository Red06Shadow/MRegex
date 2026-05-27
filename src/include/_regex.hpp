#ifndef _REGEX
#define _REGEX

#include "nfa.hpp"
#include "dfa.hpp"
#include <iostream>

template <typename CharT>
/// @brief Estructura que contiene el inicio, el iterador y el final de las cadenas analizadas
class _C_char_range
{
public:
    /// @brief Inicio de la cadena
    const CharT *const begin;
    /// @brief Iterador
    const CharT *iterator;
    /// @brief Fin de la cadena
    const CharT *const end;
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    _C_char_range(const CharT *_begin, const CharT *_iterator, const CharT *_end) : begin(_begin), iterator(_iterator), end(_end) {}
    _C_char_range(const CharT *str, size_t size) : begin(str), iterator(str), end(str + size) {}
    _C_char_range(const std::__cxx11::basic_string<CharT> &str) : begin(const_cast<CharT *>(str.c_str())), iterator(const_cast<CharT *>(begin)), end(begin + str.size()) {}
    inline const CharT *set(size_t position) { return iterator += position; }
    inline const CharT *next() { return iterator++; }
    inline const CharT *peak() const { return iterator; }
    inline const CharT *offset(size_t position) const { return iterator + position; }
    inline bool endp() const { return iterator >= end; }
    inline bool endp(const CharT *_iterator) const { return _iterator >= end; }
};

class _regex
{
private: // Structs, Enums, Class
private: // Funtions
    static size_t _S_parser(_C_char_range<char> &, NFA &, size_t);
    static size_t _S_parser_nodes(_C_char_range<char> &, NFA &, size_t);
    static NFA::_M_Class _S_parser_class(_C_char_range<char> &);
    static NFA::_M_Cualifiquer _S_parser_cualifiquer(_C_char_range<char> &);
    static char _S_parser_escape(_C_char_range<char> &);
    static void _S_position_assigned(size_t &, size_t &, size_t, NFA &);

    static const char *_S_Intern_caption_NFA(_C_char_range<char> &, const NFA &, size_t);

    static void elipson_closure_repeats(DFA::_M_State, char, const NFA &, DFA::_M_Map_Transitions&);
    static void generate_transitions(DFA::_M_Map_Transitions&, char, const NFA &, DFA::_M_State, DFA::_M_State);

    static DFA::_M_Set_State elipson_closure(DFA::_M_Set_State, const NFA &);
    static DFA::_M_Set_State move(DFA::_M_Set_State, const NFA &, char);
    
    static DFA::_M_Set_State elipson_closure_e(DFA::_M_Set_State, const NFA &);
    static DFA::_M_Set_State move_e(DFA::_M_Set_State, const NFA &, char);

    inline static DFA::_M_Set_State move_elipson_closure(DFA::_M_Set_State _Set_states, const NFA &nfa, char c) { return elipson_closure(move(_Set_states, nfa, c), nfa/*, ECFR*/); }
    static size_t _S_index_for_nfa_groups(const NFA &, size_t);

public: // Funtions
    _regex() {}
    static NFA build_NFA(const std::initializer_list<std::pair<int, std::string>> &regexs);
    static DFA convert_NFA_to_DFA(const NFA &nfa);
    static std::pair<const char *, const char *> caption(const std::string &str, const NFA &nfa);
    static std::pair<const char *, const char *> caption(const std::string &str, const DFA &dfa);
    // inline void view();
    // inline std::pair<const char *, const char *> caption(const std::string &str);
    ~_regex() {}
};
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
#endif