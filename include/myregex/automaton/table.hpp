#ifndef _TABLEDFA
#define _TABLEDFA

#include <corecrt.h>
#include <type_traits>
#include <iostream>
#include <map>
#include <math.h>
#include <myregex/utilities/selector.hpp>

namespace myregex
{
    template <typename charT, typename idT>
    class basic_builder;

    template <typename charT, typename idT>
    class basic_table
    {
        static_assert(std::is_same_v<charT, char> || std::is_same_v<charT, wchar_t>, "Error: no se permiten tipos de datos que no sean de caracteres(solo char o wchar_t)");

    private:
        using Transitions = size_t **;
        using Fdfa = std::map<size_t, idT>;
        size_t Q_dfa;
        Fdfa F_dfa;
        Transitions Q_transitions;
        inline static Transitions build(size_t);
        inline static void copy(basic_table &, const basic_table &);

    public:
        basic_table() : Q_transitions(nullptr), Q_dfa(0), F_dfa() {}
        basic_table(size_t, const Fdfa &);
        basic_table(size_t, Fdfa &&);
        basic_table(const basic_table &);
        basic_table(basic_table &&);
        basic_table &operator=(const basic_table &);
        basic_table &operator=(basic_table &&);
        inline size_t status() const { return Q_dfa; }
        inline const Transitions &transitions() const { return Q_transitions; }
        inline static constexpr size_t dictionary = std::pow(256ULL, sizeof(charT));
        inline const Fdfa &accepted_status() const { return F_dfa; }
        inline size_t size() const { return Q_dfa * myregex::basic_table<charT, idT>::dictionary * sizeof(size_t); }
        void view() const;
        ~basic_table();
        friend basic_builder<charT, idT>;
    };
    template <typename idT>
    using CompatibleTable = basic_table<char, idT>;
    template <typename idT>
    using table = basic_table<char, idT>;
    /////////////////////////////////////////////////////////////////////
    template <typename idT>
    using UnicodeTable = basic_table<wchar_t, idT>;
    template <typename idT>
    using wtable = basic_table<wchar_t, idT>;
} // namespace myregex

template <typename charT, typename idT>
typename myregex::basic_table<charT, idT>::Transitions myregex::basic_table<charT, idT>::build(size_t nstates)
{
    myregex::basic_table<charT, idT>::Transitions transitions = new size_t *[nstates];
    for (size_t state = 0; state < nstates; state++)
    {
        transitions[state] = new size_t[myregex::basic_table<charT, idT>::dictionary];
        for (size_t letter = 0; letter < myregex::basic_table<charT, idT>::dictionary; letter++)
            transitions[state][letter] = -1ULL;
    }
    return transitions;
}

template <typename charT, typename idT>
void myregex::basic_table<charT, idT>::copy(myregex::basic_table<charT, idT> &destine, const myregex::basic_table<charT, idT> &sources)
{
    destine.Q_transitions = new size_t *[sources.Q_dfa];
    for (size_t state = 0; state < sources.Q_dfa; state++)
    {
        destine.Q_transitions[state] = new size_t[myregex::basic_table<charT, idT>::dictionary];
        for (size_t letter = 0; letter < myregex::basic_table<charT, idT>::dictionary; letter++)
            destine.Q_transitions[state][letter] = sources.Q_transitions[state][letter];
    }
    destine.Q_dfa = sources.Q_dfa;
    destine.F_dfa = sources.F_dfa;
}

template <typename charT, typename idT>
myregex::basic_table<charT, idT>::basic_table(size_t nstates, const Fdfa &f_dfa) : Q_transitions(myregex::basic_table<charT, idT>::build(nstates)),
                                                                              Q_dfa(nstates),
                                                                              F_dfa(f_dfa) {}
template <typename charT, typename idT>
myregex::basic_table<charT, idT>::basic_table(size_t nstates, Fdfa &&f_dfa) : Q_transitions(myregex::basic_table<charT, idT>::build(nstates)),
                                                                         Q_dfa(nstates),
                                                                         F_dfa(std::move(f_dfa)) {}

template <typename charT, typename idT>
myregex::basic_table<charT, idT>::basic_table(const myregex::basic_table<charT, idT> &other) : Q_transitions(nullptr),
                                                                                     Q_dfa(other.Q_dfa),
                                                                                     F_dfa(other.F_dfa)
{
    if (!other.Q_transitions)
        throw std::runtime_error("error: empty basic_allocator in copy contructor: basic_allocator(const std::basic_allocator& other)");
    myregex::basic_table<charT, idT>::copy(*this, other);
}
template <typename charT, typename idT>
myregex::basic_table<charT, idT>::basic_table(myregex::basic_table<charT, idT> &&other) : Q_transitions(nullptr)
{
    if (other.Q_transitions)
    {
        Q_transitions = other.Q_transitions;
        other.Q_transitions = nullptr;
        Q_dfa = other.Q_dfa;
        F_dfa = std::move(other.F_dfa);
    }
}

template <typename charT, typename idT>
myregex::basic_table<charT, idT> &myregex::basic_table<charT, idT>::operator=(const basic_table &other)
{
    if (&other != this)
    {
        if (!other.Q_transitions)
            throw std::runtime_error("error: empty basic_allocator in copy contructor: basic_allocator(const std::basic_allocator& other)");
        myregex::basic_table<charT, idT>::copy(*this, other);
    }
    return *this;
}
template <typename charT, typename idT>
myregex::basic_table<charT, idT> &myregex::basic_table<charT, idT>::operator=(basic_table &&other)
{
    if (&other != this)
    {
        if (other.Q_transitions)
        {
            Q_transitions = other.Q_transitions;
            other.Q_transitions = nullptr;
            Q_dfa = other.Q_dfa;
            F_dfa = std::move(other.F_dfa);
        }
    }
    return *this;
}

template <typename charT, typename idT>
void myregex::basic_table<charT, idT>::view() const
{
    myregex::selector<charT>::stream() << "Q_dfa: " << this->Q_dfa << std::endl
               << "Q_transitions:" << std::endl
               << '\t';

    for (size_t letter = 0; letter < myregex::basic_table<charT, idT>::dictionary; letter++)
        myregex::selector<charT>::stream() << charT(letter) << charT(' ');

    myregex::selector<charT>::stream() << std::endl;

    for (size_t state = 0; state < this->Q_dfa; state++)
    {
        myregex::selector<charT>::stream() << state << ":      ";
        for (size_t letter = 0; letter < myregex::basic_table<charT, idT>::dictionary; letter++)
            myregex::selector<charT>::stream() << (long long)(this->Q_transitions[state][letter]) << charT(' ');
        myregex::selector<charT>::stream() << std::endl;
    }

    myregex::selector<charT>::stream() << "Estados de aceptacion (F_dfa): { " << std::endl;

    for (auto &&[key, value] : F_dfa)
        myregex::selector<charT>::stream() << charT('{') << key << "; id: " << value << charT('}') << std::endl;

    myregex::selector<charT>::stream() << charT('}') << std::endl;
}

template <typename charT, typename idT>
myregex::basic_table<charT, idT>::~basic_table()
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