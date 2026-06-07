#ifndef _NFA
#define _NFA

#include <variant>
#include <vector>
#include <stdexcept>
#include <map>
#include <set>
#include <iostream>

#define NFAVARIATION false

class NFA
{
public:
    using Qnfa = std::vector<size_t>;
#if NFAVARIATION
    // Transicion con optimizacion creo
    using Transitions = std::vector<std::map<size_t, std::vector<size_t>>>;
#else
    // Transicion estadnar
    using Transitions = std::map<std::pair<size_t, size_t>, std::vector<size_t>>;
#endif
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
    inline void view() const
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
                    std::wcout << statesTransitors[state] << (state >= statesTransitors.size() - 1ULL ? L"" : L", " );
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
                    std::wcout << states[state] << (state >= states.size() - 1ULL ? L"" : L", " );
            std::wcout << L' ' << L'}' << std::endl;
        }
        std::wcout << L'}' << std::endl

#endif
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