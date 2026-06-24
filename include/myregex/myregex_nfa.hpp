#ifndef _NFA
#define _NFA

#include <iostream>
#include <vector>
#include <type_traits>
#include <stdexcept>
#include <map>
#include <set>
#include "myregex_global.hpp"
#include "myregex_status_control.hpp"

namespace myregex
{

    template <typename CharT>
    class basic_build;

    template <typename CharT>
    class basic_nfa
    {
    public:
        static_assert(std::is_same_v<CharT, char> || std::is_same_v<CharT, wchar_t>, "Error: no se permiten tipos de datos que no sean de caracteres(solo char o wchar_t)");
        // Estados
        using Qnfa = std::vector<myregex::basic_control>;
        using BQnfa = std::vector<size_t>;
        // Transicion estandar
        using Transitions = std::map<std::pair<size_t, size_t>, std::vector<size_t>>;
        using Dictionary = std::set<CharT>;
        using Fnfa = std::map<size_t, size_t>;

    private:
        /// @brief Estados
        Qnfa Q_nfa;
        /// @brief Estados de inicio
        BQnfa begin_Q_nfa;
        /// @brief Transiciones
        Transitions Q_transitions;
        /// @brief Diccionario
        Dictionary Q_dictionary;
        /// @brief Estados de Acpetacion
        Fnfa F_nfa;
        friend basic_build<CharT>;

    public:
        basic_nfa() : Q_nfa(), begin_Q_nfa(), Q_transitions(), Q_dictionary(), F_nfa() {}
        inline const Qnfa &status() const { return Q_nfa; }
        inline const BQnfa &begin_status() const { return begin_Q_nfa; }
        inline const Transitions &transitions() const { return Q_transitions; }
        inline const Dictionary &dictionary() const { return Q_dictionary; }
        inline const Fnfa &accepted_status() const { return F_nfa; }
        void view() const;
        size_t size() const;
        ~basic_nfa() {}
    };

    template <typename CharT>
    size_t basic_nfa<CharT>::size() const
    {
        size_t _size = 0;
#if !(OPTIMIZATION_NFA)
        _size += Q_nfa.size() * sizeof(myregex::basic_control) + sizeof(Qnfa);
        _size += Q_dictionary.size() * sizeof(CharT) + sizeof(Dictionary);
        _size += begin_Q_nfa.size() * sizeof(size_t) + sizeof(Qnfa);
#endif
        _size += F_nfa.size() * (sizeof(size_t) * 2) + sizeof(Fnfa);
        _size += Q_transitions.size() * (sizeof(size_t) * 2 + sizeof(CharT)) + sizeof(Transitions);
        return _size;
    }

    template <typename CharT>
    void basic_nfa<CharT>::view() const
    {
        std::wcout << L"Estados (Q_nfa) [ " << std::endl;
        for (size_t i = 0; i < Q_nfa.size(); i++)
            std::wcout << L'\t' << i << L": {" << Q_nfa[i] << std::endl << L'}' << std::endl;

        std::wcout << L"]" << std::endl
                   << "Diccionario (dictionary): { ";

        for (auto &&letter : Q_dictionary)
            std::wcout << letter << L' ';

        std::wcout << L'}' << std::endl
                   << L"Transiciones (Q_transitions): " << std::endl
                   << L'{' << std::endl;
        for (auto &&[key, states] : Q_transitions)
        {
            std::wcout << L"    {" << key.first << L", ";
            if (key.second == -1ULL)
                std::wcout << wchar_t(0x025d); // L'∊';
            else
                std::wcout << wchar_t(key.second);
            std::wcout << L"} -> { ";
            for (size_t state = 0; state < states.size(); state++)
                std::wcout << states[state] << (state >= states.size() - 1ULL ? L"" : L", ");
            std::wcout << L' ' << L'}' << std::endl;
        }
        std::wcout << L'}' << std::endl 
                   << L"Estados de Inicio (begin_Q_nfa): { ";

        for (auto &&state : begin_Q_nfa)
            std::wcout << state << L' ';

        std::wcout << L"}" << std::endl
                   << L"Estados de aceptacion (F_dfa): { " << std::endl;

        for (auto &&[key, value] : F_nfa)
            std::wcout << L'{' << key << L"; id: " << value << L'}' << std::endl;

        std::wcout << L'}' << std::endl;
    }
    using CompatibleNfa = myregex::basic_nfa<char>;
    using nfa = myregex::basic_nfa<char>;
    /////////////////////////////////////////////////////////////////////
    using UnicodeNfa = myregex::basic_nfa<wchar_t>;
    using wnfa = myregex::basic_nfa<wchar_t>;
} // namespace myregex
#endif