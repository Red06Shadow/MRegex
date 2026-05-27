#include <dfa.hpp>
#include <iostream>

void DFA::view()
{
    std::cout << "Qdfa " << '[';
    for (size_t i = 0; i < Q_dfa.size(); i++)
    {
        std::cout << std::endl
                  << i << ": { ";
        for (auto &&j : Q_dfa[i])
        {
            if (j == -1ULL)
                std::cout << 'A' << ' ';
            else
                std::cout << j << ' ';
        }
        std::cout << '}';
    }
    std::cout << std::endl
              << ']' << std::endl
              << "Transitions: " << '{' << std::endl;

    for (auto &&[key, value] : delta_dfa)
    {
        std::cout << '{' << key.first << ", " << key.second << '}' << " -> { ";
        if (value.first == -1ULL)
            std::cout << 'A';
        else
            std::cout << value.first << " :: [ " << (long long)(value.second.min) << ", " << (long long)(value.second.max) << " ] }" << std::endl;
    }
    std::cout << '}' << std::endl;

    std::cout << "Begin State: { ";
    for (auto &&i : _M_begins_states)
        std::cout << i << ' ';
    std::cout << "}" << std::endl;

    std::cout << "Fdfa: " << "{ ";
    for (auto &&i : F_dfa)
        std::cout << i << ' ';
    std::cout << '}' << std::endl;
}