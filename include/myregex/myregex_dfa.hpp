#ifndef _DFA
#define _DFA

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <corecrt.h>
#include <type_traits>


namespace myregex
{

    template <typename CharT>
    class basic_build;

    template <typename CharT>
    class basic_dfa
    {
    public: //  Structs, Enums, Class
        static_assert(std::is_same_v<CharT, char> || std::is_same_v<CharT, wchar_t>, "Error: no se permiten tipos de datos que no sean de caracteres(solo char o wchar_t)");
        using States = std::set<size_t>;

#if !(OPTIMIZATION_DFA)
        using Qdfa = std::vector<States>;
        using Dictionary = std::set<CharT>;
#endif
        using Transitions = std::map<std::pair<size_t, CharT>, size_t>;
        using Fdfa = std::map<size_t, size_t>;

    private:
#if !(OPTIMIZATION_DFA)
        /// @brief Estados
        Qdfa Q_dfa;
        /// @brief Estados de inicio
        States begin_Q_dfa;
        /// @brief Diccionario
        Dictionary Q_dictionary;
#endif
        /// @brief Transiciones
        Transitions Q_transitions;
        /// @brief Estados de Acpetacion
        Fdfa F_dfa;

    public:
#if (OPTIMIZATION_DFA)
        basic_dfa() : Q_transitions({}), F_dfa({}) {}
#else
        basic_dfa() : Q_dfa(), Q_transitions({}), Q_dictionary({}), F_dfa({}), begin_Q_dfa({}) {}

#endif
#if !(OPTIMIZATION_DFA)
        inline const Qdfa &status() const { return Q_dfa; }
        inline const States &begin_status() const { return begin_Q_dfa; }
        inline const Dictionary &dictionary() const { return Q_dictionary; }
#endif
        inline const Transitions &transitions() const { return Q_transitions; }
        inline const Fdfa &accepted_status() const { return F_dfa; }
        size_t size() const;
        void view() const;
        ~basic_dfa() {}
        friend basic_build<CharT>;
    };
    template <typename CharT>
    size_t basic_dfa<CharT>::size() const
    {
        size_t _size = 0;
#if !(OPTIMIZATION_DFA)
        for (auto &&i : Q_dfa)
            _size += i.size() * sizeof(size_t);
        _size += sizeof(Qdfa);
        _size += Q_dictionary.size() * sizeof(CharT) + sizeof(Dictionary);
        _size += begin_Q_dfa.size() * sizeof(size_t) + sizeof(States);
#endif
        _size += F_dfa.size() * (sizeof(size_t) * 2) + sizeof(Fdfa);
        _size += Q_transitions.size() * (sizeof(size_t) * 2 + sizeof(CharT)) + sizeof(Transitions);
        return _size;
    }
    template <typename CharT>
    void basic_dfa<CharT>::view() const
    {
#if !(OPTIMIZATION_DFA)
        std::wcout << L"Estados (Q_dfa) [ ";

        for (size_t state = 0; state < Q_dfa.size(); state++)
        {
            std::wcout << std::endl
                       << state << L": { ";
            for (auto &&nextstate : Q_dfa[state])
                std::wcout << nextstate << L' ';
            std::wcout << L'}';
        }

        std::wcout << std::endl
                   << L"]" << std::endl
                   << "Diccionario (Q_dictionary): { ";

        for (auto &&letter : Q_dictionary)
            std::wcout << letter << L' ';

        std::wcout << L'}' << std::endl;
#endif
        std::wcout << L"Transiciones (Q_transitions): " << std::endl
                   << L'{' << std::endl;

        for (auto &&[key, state] : Q_transitions)
        {
            std::wcout << L"    {" << key.first << L", ";
            if (key.second == -1ULL)
                std::wcout << wchar_t(0x025d); // L'∊';
            else
                std::wcout << wchar_t(key.second);
            std::wcout << L"} -> { " << state << L' ' << L'}' << std::endl;
        }
        std::wcout << L'}' << std::endl;
#if !(OPTIMIZATION_DFA)
        std::wcout << L"Estados de Inicio (begin_Q_dfa): { ";

        for (auto &&state : begin_Q_dfa)
            std::wcout << state << L' ';
        std::wcout << L"}" << std::endl;
#endif
        std::wcout << L"Estados de aceptacion (F_dfa): { " << std::endl;

        for (auto &&[key, value] : F_dfa)
            std::wcout << L'{' << key << L"; id: " << value << L'}' << std::endl;

        std::wcout << L'}' << std::endl;
    }
    using CompatibleDfa = myregex::basic_dfa<char>;
    using dfa = myregex::basic_dfa<char>;
    /////////////////////////////////////////////////////////////////////
    using UnicodeDfa = myregex::basic_dfa<wchar_t>;
    using wdfa = myregex::basic_dfa<wchar_t>;
} // namespace myregex

#endif
