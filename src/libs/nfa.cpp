#include <nfa.hpp>
#include <iostream>

void NFA::view() const
{
    std::wcout << L"Estados (Q_nfa) [ ";
    for (size_t i = 0; i < Q_nfa.size(); i++)
        std::wcout << i << L' ';

    std::wcout << L"]" << std::endl
               << "Diccionario (dictionary): { ";

    for (auto &&letter : dictionary)
        std::wcout << int(letter) << L' ';

    std::wcout << L'}' << std::endl
               << L"Transiciones (Q_transitions): " << std::endl
               << L'{' << std::endl;

#if NFAVARIATION
    for (size_t transitor = 0; transitor < Q_transitions.size(); transitor++)
    {
        for (auto &&[transitionValue, statesTransitors] : Q_transitions[transitor])
        {
            std::wcout << L"    {" << transitor << L", ";
            if (transitionValue == -1ULL)
                std::wcout << wchar_t(0x025d); // L'∊';
            else
                std::wcout << wchar_t(transitionValue);
            std::wcout << L"} -> { ";
            for (size_t state = 0; state < statesTransitors.size(); state++)
                std::wcout << statesTransitors[state] << (state >= statesTransitors.size() - 1ULL ? L"" : L", ");
            std::wcout << L' ' << L'}' << std::endl;
        }
    }
    std::wcout << L'}' << std::endl
#else
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

#endif
               << L"Estados de Inicio (begin_Q_nfa): { ";

    for (auto &&state : begin_Q_nfa)
        std::wcout << state << L' ';

    std::wcout << L"}" << std::endl
               << L"Estados de aceptacion (F_nfa): { ";

    for (auto &&[key, value] : F_nfa)
        std::wcout << key << ':' << value << std::endl;

    std::wcout << L'}' << std::endl;
}