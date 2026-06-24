#ifndef _TABLEDFA
#define _TABLEDFA

#include <corecrt.h>
#include <type_traits>
#include <iostream>
#include <map>
#include <math.h>

namespace myregex
{
    template <typename CharT>
    class basic_build;

    template <typename CharT>
    class basic_table
    {
        static_assert(std::is_same_v<CharT, char> || std::is_same_v<CharT, wchar_t>, "Error: no se permiten tipos de datos que no sean de caracteres(solo char o wchar_t)");

    private:
        using Transitions = size_t **;
        using Fdfa = std::map<size_t, size_t>;
        const size_t Q_dfa;
        Fdfa F_dfa;
        Transitions Q_transitions;
        basic_table() : Q_transitions(nullptr), Q_dfa(0), F_dfa() {}
        basic_table(size_t, const Fdfa &);
        basic_table(size_t, Fdfa &&);
        inline static Transitions build(size_t);
        inline static void copy(basic_table &, const basic_table &);

    public:
        basic_table(const basic_table &);
        basic_table(basic_table &&);
        inline size_t status() const { return Q_dfa; }
        inline const Transitions &transitions() const { return Q_transitions; }
        inline static constexpr size_t dictionary = std::pow(256ULL, sizeof(CharT));
        inline const Fdfa &accepted_status() const { return F_dfa; }
        inline size_t size() const { return Q_dfa * myregex::basic_table<CharT>::dictionary * sizeof(size_t); }
        void view() const;
        ~basic_table();
        friend basic_build<CharT>;
    };
    using CompatibleTable = myregex::basic_table<char>;
    using table = myregex::basic_table<char>;
    /////////////////////////////////////////////////////////////////////
    using UnicodeTable = myregex::basic_table<wchar_t>;
    using wtable = myregex::basic_table<wchar_t>;
} // namespace myregex

template <typename CharT>
typename myregex::basic_table<CharT>::Transitions myregex::basic_table<CharT>::build(size_t nstates)
{
    myregex::basic_table<CharT>::Transitions transitions = new size_t *[nstates];
    for (size_t state = 0; state < nstates; state++)
    {
        transitions[state] = new size_t[myregex::basic_table<CharT>::dictionary];
        for (size_t letter = 0; letter < myregex::basic_table<CharT>::dictionary; letter++)
            transitions[state][letter] = -1ULL;
    }
    return transitions;
}

template <typename CharT>
void myregex::basic_table<CharT>::copy(myregex::basic_table<CharT> &destine, const myregex::basic_table<CharT> &sources)
{
    destine.Q_transitions = new size_t *[sources.Q_dfa];
    for (size_t state = 0; state < sources.Q_dfa; state++)
    {
        destine.Q_transitions[state] = new size_t[myregex::basic_table<CharT>::dictionary];
        for (size_t letter = 0; letter < myregex::basic_table<CharT>::dictionary; letter++)
            destine.Q_transitions[state][letter] = sources.Q_transitions[state][letter];
    }
}

template <typename CharT>
myregex::basic_table<CharT>::basic_table(size_t nstates, const Fdfa &f_dfa) : Q_transitions(myregex::basic_table<CharT>::build(nstates)),
                                                                              Q_dfa(nstates),
                                                                              F_dfa(f_dfa) {}
template <typename CharT>
myregex::basic_table<CharT>::basic_table(size_t nstates, Fdfa &&f_dfa) : Q_transitions(myregex::basic_table<CharT>::build(nstates)),
                                                                         Q_dfa(nstates),
                                                                         F_dfa(std::move(f_dfa)) {}

template <typename CharT>
myregex::basic_table<CharT>::basic_table(const myregex::basic_table<CharT> &other) : Q_transitions(nullptr),
                                                                                     Q_dfa(other.Q_dfa),
                                                                                     F_dfa(other.F_dfa)
{
    if (other.Q_transitions != nullptr && other.Q_dfa != 0 && other.Q_dictionary != 0)
        myregex::basic_table<CharT>::copy(*this, other);
}
template <typename CharT>
myregex::basic_table<CharT>::basic_table(myregex::basic_table<CharT> &&other) : Q_transitions(nullptr),
                                                                                Q_dfa(other.Q_dfa),
                                                                                F_dfa(std::move(other.F_dfa))
{
    if (other.Q_transitions != nullptr && other.Q_dfa != 0 && other.Q_dictionary != 0)
    {
        Q_transitions = other.Q_transitions;
        other.Q_transitions = nullptr;
    }
}

template <typename CharT>
void myregex::basic_table<CharT>::view() const
{
    std::wcout << L"Q_dfa: " << this->Q_dfa << std::endl
               << L"Q_transitions:" << std::endl
               << L'\t';

    for (size_t letter = 0; letter < myregex::basic_table<CharT>::dictionary; letter++)
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

    std::wcout << L"Estados de aceptacion (F_dfa): { " << std::endl;

    for (auto &&[key, value] : F_dfa)
        std::wcout << L'{' << key << L"; id: " << value << L'}' << std::endl;

    std::wcout << L'}' << std::endl;
}

template <typename CharT>
myregex::basic_table<CharT>::~basic_table()
{
    if (this->Q_transitions != nullptr)
    {
        for (size_t state = 0; state < Q_dfa; state++)
            if (Q_transitions[state] != nullptr)
                delete[] Q_transitions[state];
        delete[] Q_transitions;
        Q_transitions = nullptr;
    }
}

#endif