#ifndef BUILDMREGEX
#define BUILDMREGEX

#include <math.h>
#include <stack>
#include <queue>
#include <unordered_set>
#include <myregex/main/main.hpp>

namespace myregex
{
    template <typename charT, typename idT>
    class basic_builder
    {
        static_assert(std::is_same_v<charT, char> || std::is_same_v<charT, wchar_t>, "Error: no se permiten tipos de datos que no sean de caracteres(solo char o wchar_t)");

    private: // enums, structs
        using _M_begin_transitions = std::pair<size_t, std::vector<size_t>>;
        using _M_range_cualifiquer = std::pair<size_t, size_t>;

    private: // Funtions regex -> myregex::basic_nfa
        // Funciones para el parseo de expresiones regex a myregex::basic_nfa
        static charT _S_parser_code_caracter_hexunicode(basic_string_range<charT> &, const unsigned char);
        static charT _S_parser_code_caracter_octal(basic_string_range<charT> &);
        static charT _S_parser_code_caracter(basic_string_range<charT> &);
        static _M_range_cualifiquer _S_parser_cualifiquer_range(basic_string_range<charT> &);
        static void _S_parser_cualifiquer(basic_string_range<charT> &, myregex::basic_nfa<charT, idT> &, const _M_begin_transitions &, bool);
        static _M_begin_transitions _S_parser_nfa_parser_class_expresions(basic_string_range<charT> &, myregex::basic_nfa<charT, idT> &);
        static size_t _S_build_nfa_parser_regular_expresions_basic(basic_string_range<charT> &, myregex::basic_nfa<charT, idT> &, bool, idT);
        static size_t _S_build_nfa_parser_or_expresions(basic_string_range<charT> &, myregex::basic_nfa<charT, idT> &, bool, idT);

        constexpr inline static size_t _S_transition(charT caracter)
        {
            size_t transition;
            if constexpr (std::is_same_v<charT, char>)
                return (unsigned char)(caracter);
            else
            {
                if constexpr (sizeof(wchar_t) == 2)
                    return (unsigned short)(caracter);
                else
                    return (unsigned int)(caracter);
            }
        }

    public: // Funtions myregex::basic_nfa -> basic_dfa
        // Funciones adicionales del convertidor de myregex::basic_nfa a basic_dfa
        static std::set<size_t> _S_elipson_cloursers(const std::set<size_t> &, const myregex::basic_nfa<charT, idT> &);
        static std::set<size_t> _S_move(const std::set<size_t> &, charT, const myregex::basic_nfa<charT, idT> &);
        inline static std::set<size_t> _S_move_elipson_cloursers(const std::set<size_t> &states, charT a, const myregex::basic_nfa<charT, idT> &nfa) { return _S_elipson_cloursers(_S_move(states, a, nfa), nfa); }

    private:
        std::variant<basic_nfa<charT, idT>, basic_dfa<charT, idT>, basic_table<charT, idT>> _M_expresions;

    public:
        basic_builder(const std::initializer_list<std::pair<idT, std::__cxx11::basic_string<charT>>> &list) { _M_expresions = std::move(build_nfa(list)); }
        basic_builder(const std::vector<std::pair<idT, std::__cxx11::basic_string<charT>>> &list) { _M_expresions = std::move(build_nfa(list)); }
        static myregex::basic_nfa<charT, idT> build_nfa(const std::initializer_list<std::pair<idT, std::__cxx11::basic_string<charT>>> &list);
        static myregex::basic_nfa<charT, idT> build_nfa(const std::vector<std::pair<idT, std::__cxx11::basic_string<charT>>> &list);
        static myregex::basic_dfa<charT, idT> build_dfa(const basic_nfa<charT, idT> &nfa);
        static myregex::basic_table<charT, idT> build_table(const basic_dfa<charT, idT> &dfa);
        myregex::basic_builder<charT, idT> &convert_to_dfa();
        myregex::basic_builder<charT, idT> &convert_to_table();
        inline myregex::basic_regex<charT, idT> build() { return std::move(_M_expresions); }
        friend basic_regex<charT, idT>;
    };

    template <typename idT>
    using CompatibleBuilder = myregex::basic_builder<char, idT>;
    template <typename idT>
    using builder = myregex::basic_builder<char, idT>;
    /////////////////////////////////////////////////////////////////////
    template <typename idT>
    using UnicodeBuilder = myregex::basic_builder<wchar_t, idT>;
    template <typename idT>
    using wbuilder = myregex::basic_builder<wchar_t, idT>;

} // namespace myregex

#include "builder.tpp"

#endif