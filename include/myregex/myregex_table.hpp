#ifndef _TABLEDFA
#define _TABLEDFA

#include <corecrt.h>
#include <type_traits>
#include <iostream>
#include <map>

namespace myregex
{
    template <typename CharT>
    class basic_build;

    class TableDFA
    {
    private:
        using _S_Table_Transitions = size_t **;
        using Fdfa = std::map<size_t, size_t>;
        const size_t Q_dfa, Q_dictionary;
        Fdfa F_dfa;
        _S_Table_Transitions Q_transitions;
        TableDFA() : Q_transitions(nullptr), Q_dfa(0), Q_dictionary(0), F_dfa() {}
        TableDFA(size_t, size_t, const Fdfa &);
        TableDFA(size_t, size_t, Fdfa &&);
        inline static _S_Table_Transitions build(size_t, size_t);
        inline static void copy(TableDFA &, const TableDFA &);

    public:
        TableDFA(const TableDFA &);
        TableDFA(TableDFA &&);
        inline size_t status() const { return Q_dfa;}
        inline const _S_Table_Transitions& transitions() const { return Q_transitions;}
        inline const size_t& dictionary() const { return Q_dictionary;}
        inline const Fdfa& accepted_status() const { return F_dfa;}
        inline size_t size() const { return Q_dfa * Q_dictionary * sizeof(size_t); }
        void view() const;
        ~TableDFA();
        friend basic_build<char>;
        friend basic_build<wchar_t>;
    };
} // namespace myregex


myregex::TableDFA::_S_Table_Transitions myregex::TableDFA::build(size_t nstates, size_t alphabet_size)
{
    myregex::TableDFA::_S_Table_Transitions transitions = new size_t *[nstates];
    for (size_t state = 0; state < nstates; state++)
    {
        transitions[state] = new size_t[alphabet_size];
        for (size_t letter = 0; letter < alphabet_size; letter++)
            transitions[state][letter] = -1ULL;
    }
    return transitions;
}

void myregex::TableDFA::copy(myregex::TableDFA &destine, const myregex::TableDFA &sources)
{
    destine.Q_transitions = new size_t *[sources.Q_dfa];
    for (size_t state = 0; state < sources.Q_dfa; state++)
    {
        destine.Q_transitions[state] = new size_t[sources.Q_dictionary];
        for (size_t letter = 0; letter < sources.Q_dictionary; letter++)
            destine.Q_transitions[state][letter] = sources.Q_transitions[state][letter];
    }
}

myregex::TableDFA::TableDFA(size_t nstates, size_t alphabet_size, const Fdfa &f_dfa) : Q_transitions(myregex::TableDFA::build(nstates, alphabet_size)),
                                                                                          Q_dfa(nstates),
                                                                                          Q_dictionary(alphabet_size),
                                                                                          F_dfa(f_dfa) {}
myregex::TableDFA::TableDFA(size_t nstates, size_t alphabet_size, Fdfa &&f_dfa) : Q_transitions(myregex::TableDFA::build(nstates, alphabet_size)),
                                                                                     Q_dfa(nstates),
                                                                                     Q_dictionary(alphabet_size),
                                                                                     F_dfa(std::move(f_dfa)) {}

myregex::TableDFA::TableDFA(const myregex::TableDFA &other) : Q_transitions(nullptr),
                                            Q_dfa(other.Q_dfa),
                                            Q_dictionary(other.Q_dictionary),
                                            F_dfa(other.F_dfa)
{
    if (other.Q_transitions != nullptr && other.Q_dfa != 0 && other.Q_dictionary != 0)
        myregex::TableDFA::copy(*this, other);
}
myregex::TableDFA::TableDFA(myregex::TableDFA &&other) : Q_transitions(nullptr),
                                       Q_dfa(other.Q_dfa),
                                       Q_dictionary(other.Q_dictionary),
                                       F_dfa(std::move(other.F_dfa))
{
    if (other.Q_transitions != nullptr && other.Q_dfa != 0 && other.Q_dictionary != 0)
    {
        Q_transitions = other.Q_transitions;
        other.Q_transitions = nullptr;
    }
}

void myregex::TableDFA::view() const
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

myregex::TableDFA::~TableDFA() {
    if (this->Q_transitions != nullptr)
    {
        for (size_t state = 0; state < Q_dfa; state++)
            if(Q_transitions[state] != nullptr) delete[] Q_transitions[state];
        delete[] Q_transitions;
        Q_transitions = nullptr;
    }
}

#endif