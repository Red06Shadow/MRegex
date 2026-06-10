#include <dfa.hpp>
#include <iostream>

void DFA::view() const
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

    std::wcout << std::endl << L"]" << std::endl
               << "Diccionario (dictionary): { ";

    for (auto &&letter : dictionary)
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