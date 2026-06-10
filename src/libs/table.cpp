#include <table.hpp>
#include <iostream>

TableDFA::_S_Table_Transitions TableDFA::build(size_t nstates, size_t alphabet_size)
{
    TableDFA::_S_Table_Transitions transitions = new size_t *[nstates];
    for (size_t state = 0; state < nstates; state++)
    {
        transitions[state] = new size_t[alphabet_size];
        for (size_t letter = 0; letter < alphabet_size; letter++)
            transitions[state][letter] = -1ULL;
    }
    return transitions;
}

void TableDFA::copy(TableDFA &destine, const TableDFA &sources)
{
    destine.Q_transitions = new size_t *[sources.Q_dfa];
    for (size_t state = 0; state < sources.Q_dfa; state++)
    {
        destine.Q_transitions[state] = new size_t[sources.dictionary];
        for (size_t letter = 0; letter < sources.dictionary; letter++)
            destine.Q_transitions[state][letter] = sources.Q_transitions[state][letter];
    }
}

TableDFA::TableDFA(size_t nstates, size_t alphabet_size, const Fdfa &f_dfa) : Q_transitions(TableDFA::build(nstates, alphabet_size)),
                                                                                          Q_dfa(nstates),
                                                                                          dictionary(alphabet_size),
                                                                                          F_dfa(f_dfa) {}
TableDFA::TableDFA(size_t nstates, size_t alphabet_size, Fdfa &&f_dfa) : Q_transitions(TableDFA::build(nstates, alphabet_size)),
                                                                                     Q_dfa(nstates),
                                                                                     dictionary(alphabet_size),
                                                                                     F_dfa(std::move(f_dfa)) {}

TableDFA::TableDFA(const TableDFA &other) : Q_transitions(nullptr),
                                            Q_dfa(other.Q_dfa),
                                            dictionary(other.dictionary),
                                            F_dfa(other.F_dfa)
{
    if (other.Q_transitions != nullptr && other.Q_dfa != 0 && other.dictionary != 0)
        TableDFA::copy(*this, other);
}
TableDFA::TableDFA(TableDFA &&other) : Q_transitions(nullptr),
                                       Q_dfa(other.Q_dfa),
                                       dictionary(other.dictionary),
                                       F_dfa(std::move(other.F_dfa))
{
    if (other.Q_transitions != nullptr && other.Q_dfa != 0 && other.dictionary != 0)
    {
        Q_transitions = other.Q_transitions;
        other.Q_transitions = nullptr;
    }
}

void TableDFA::view() const
{
    std::wcout << L"Q_dfa: " << this->Q_dfa << std::endl
               << L"Q_transitions:" << std::endl << L'\t';

    for (size_t letter = 32; letter < 128; letter++)
        std::wcout << char(letter) << L' ';

    std::wcout << std::endl;

    for (size_t state = 0; state < this->Q_dfa; state++)
    {
        std::wcout << state << L":      ";
        for (size_t letter = 32; letter < 128; letter++)
        {
            size_t next = (long long)(this->Q_transitions[state][letter]);
            if (next == -1ULL)
                std::wcout << wchar_t(255) << L' ';
            else
                std::wcout << next << L' ';
        }
        std::wcout << std::endl;
    }

    std::wcout << L"Estados de aceptacion (F_dfa): { ";

    for (auto &&[key, value] : F_dfa)
        std::wcout << key << ':' << value << std::endl;
    std::wcout << L'}' << std::endl;
}

TableDFA::~TableDFA() {
    if (this->Q_transitions != nullptr)
    {
        for (size_t state = 0; state < Q_dfa; state++)
            if(Q_transitions[state] != nullptr) delete[] Q_transitions[state];
        delete[] Q_transitions;
        Q_transitions = nullptr;
    }
}