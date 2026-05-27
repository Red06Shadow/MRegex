#ifndef _NFA
#define _NFA

#include <variant>
#include <vector>
#include <stdexcept>
#include <map>
#include <set>
#include <iostream>

class NFA
{
public:
    using Qnfa = std::vector<size_t>;
    using Transitions = std::map<std::pair<size_t, size_t>, std::vector<size_t>>;
    using Dictionary = std::set<char>;
    using Fnfa = std::set<size_t>;

public:
    /// @brief Estados
    Qnfa Q_nfa;
    /// @brief Estados de inicio
    Qnfa begin_Q_nfa;
    /// @brief Transiciones
    Transitions Q_transitions;
    /// @brief Diccionario
    Dictionary dictionary;
    /// @brief Estados de Acpetacion
    Fnfa F_nfa;

public:
    NFA() : Q_nfa(), begin_Q_nfa(), Q_transitions(), dictionary(), F_nfa() {}
    inline void view()
    {
        std::wcout << L"Estados (Q_nfa) [ ";
        for (size_t i = 0; i < Q_nfa.size(); i++)
            std::wcout << i << L' ';

        std::wcout << L"]" << std::endl
                   << "Diccionario (dictionary): { ";

        for (auto &&letter : dictionary)
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
            for (auto &&state : states)
                std::wcout << state << L' ';
            std::wcout << L'}' << std::endl;
        }
        std::wcout << L'}' << std::endl
                   << L"Estados de Inicio (begin_Q_nfa): { ";

        for (auto &&state : begin_Q_nfa)
            std::wcout << state << L' ';

        std::wcout << L"}" << std::endl
                   << L"Estados de aceptacion (F_nfa): { ";

        for (auto &&state : F_nfa)
            std::wcout << state << L' ';

        std::wcout << L'}' << std::endl;
    }
    ~NFA() {}
};

#endif