#ifndef _DFA
#define _DFA

#include <corecrt.h>
#include <type_traits>
#include <map>
#include <set>
#include <iostream>
#include <vector>

namespace myregex
{

    template <typename CharT>
    class basic_build;

    template <typename CharT>
    class DFA
    {
    public: //  Structs, Enums, Class
        static_assert(std::is_same_v<CharT, char> || std::is_same_v<CharT, wchar_t>, "Error: no se permiten tipos de datos que no sean de caracteres(solo char o wchar_t)");
        using States = std::set<size_t>;
        using Qdfa = std::vector<States>;
        using Transitions = std::map<std::pair<size_t, CharT>, size_t>;
        using Dictionary = std::set<CharT>;
        using Fdfa = std::map<size_t, size_t>;

    private:
        /// @brief Estados
        Qdfa Q_dfa;
        /// @brief Estados de inicio
        States begin_Q_dfa;
        /// @brief Transiciones
        Transitions Q_transitions;
        /// @brief Diccionario
        Dictionary Q_dictionary;
        /// @brief Estados de Acpetacion
        Fdfa F_dfa;

    public:
        DFA() : Q_dfa(), Q_transitions({}), Q_dictionary({}), F_dfa({}), begin_Q_dfa({}) {}
        inline const Qdfa& status() const { return Q_dfa;}
        inline const States& begin_status() const { return begin_Q_dfa;}
        inline const Transitions& transitions() const { return Q_transitions;}
        inline const Dictionary& dictionary() const { return Q_dictionary;}
        inline const Fdfa& accepted_status() const { return F_dfa;}
        void view() const;
        ~DFA() {}
        friend basic_build<CharT>;
    };
    template <typename CharT>
    void DFA<CharT>::view() const
    {
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

        std::wcout << L'}' << std::endl
                   << L"Transiciones (Q_transitions): " << std::endl
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
        std::wcout << L'}' << std::endl
                   << L"Estados de Inicio (begin_Q_dfa): { ";

        for (auto &&state : begin_Q_dfa)
            std::wcout << state << L' ';

        std::wcout << L"}" << std::endl
                   << L"Estados de aceptacion (F_dfa): { ";

        for (auto &&[key, value] : F_dfa)
            std::wcout << key << ':' << value << std::endl;

        std::wcout << L'}' << std::endl;
    }

} // namespace myregex

#endif
