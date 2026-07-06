#ifndef _TABLEDFA
#define _TABLEDFA

#include <corecrt.h>
#include <type_traits>
#include <iostream>
#include <vector>
#include <math.h>
#include <myregex/automaton/base/state.hpp>

#define DEBUG false

namespace myregex
{
    template <typename charT, typename idT>
    class basic_builder;

    template <typename charT, typename idT>
    class basic_table
    {
        static_assert(std::is_same_v<charT, char> || std::is_same_v<charT, wchar_t>, "Error: no se permiten tipos de datos que no sean de caracteres(solo char o wchar_t)");

    public:
        using Qtable = std::vector<myregex::state<idT>>;
        using Transitions = size_t *;

    private:
        Qtable Q_table;
        Transitions Q_transitions;
        bool _M_transitions_deletable;
        inline static Transitions build(size_t);
        inline static void copy(basic_table &, const basic_table &);

    public:
        basic_table() : Q_transitions(nullptr), Q_table({}), _M_transitions_deletable(false) {}
        basic_table(const Qtable &_states, const Transitions _transitions) : Q_transitions(_transitions), Q_table(_states), _M_transitions_deletable(false) {}
        basic_table(const Qtable &_states, Transitions&& _transitions) : Q_transitions(_transitions), Q_table(_states), _M_transitions_deletable(true) { _transitions = nullptr;}
        basic_table(const std::vector<myregex::state<idT>> &);
        basic_table(std::vector<myregex::state<idT>> &&);
        basic_table(const basic_table &);
        basic_table(basic_table &&);
        basic_table &operator=(const basic_table &);
        basic_table &operator=(basic_table &&);
        inline const Qtable &status() const { return Q_table; }
        inline const Transitions &transitions() const { return Q_transitions; }
        inline static constexpr size_t dictionary = std::pow(256ULL, sizeof(charT));
        inline size_t size() const { return Q_table.size() * myregex::basic_table<charT, idT>::dictionary * sizeof(size_t); }
        friend std::basic_ostream<charT> &operator<<(std::basic_ostream<charT> &out, basic_table<charT, idT> other)
        {
            out << '{';
            for (size_t state = 0; state < other.Q_table.size(); state++)
            {
                if (other.Q_table[state].valid())
                {
                    if constexpr (std::is_enum_v<idT>)
                        out << static_cast<size_t>(other.Q_table[state].get());
                    else
                        out << other.Q_table[state].get();
                }
                else
                    out << "{}";
                out << ((state >= other.Q_table.size() - 1ULL) ? '}' : ',');
            }
            out << std::endl
                << '{';
            for (size_t state = 0; state < other.Q_table.size(); state++)
            {
                for (size_t letter = 0; letter < myregex::basic_table<charT, idT>::dictionary; letter++)
                    out << (long long)(other.Q_transitions[(state * myregex::basic_table<charT, idT>::dictionary) + letter]) << charT(',');
                out << std::endl;
            }
            out << '}';
            return out;
        }
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
    myregex::basic_table<charT, idT>::Transitions transitions;
    transitions = new size_t[nstates * myregex::basic_table<charT, idT>::dictionary];
    return transitions;
}

template <typename charT, typename idT>
void myregex::basic_table<charT, idT>::copy(myregex::basic_table<charT, idT> &destine, const myregex::basic_table<charT, idT> &sources)
{
    destine.Q_transitions = new size_t[sources.Q_table.size() * myregex::basic_table<charT, idT>::dictionary];
    for (size_t state = 0; state < sources.Q_table.size(); state++)
    {
        for (size_t letter = 0; letter < myregex::basic_table<charT, idT>::dictionary; letter++)
            destine.Q_transitions[(state * myregex::basic_table<charT, idT>::dictionary) + letter] = sources.Q_transitions[(state * myregex::basic_table<charT, idT>::dictionary) + letter];
    }
    destine.Q_table = sources.Q_table;
}

template <typename charT, typename idT>
myregex::basic_table<charT, idT>::basic_table(const std::vector<myregex::state<idT>> &status) : Q_transitions(myregex::basic_table<charT, idT>::build(status.size())),
                                                                                                Q_table(status), _M_transitions_deletable(true) {}
template <typename charT, typename idT>
myregex::basic_table<charT, idT>::basic_table(std::vector<myregex::state<idT>> &&status) : Q_transitions(myregex::basic_table<charT, idT>::build(status.size())),
                                                                                           Q_table(std::move(status)), _M_transitions_deletable(true) {}

template <typename charT, typename idT>
myregex::basic_table<charT, idT>::basic_table(const myregex::basic_table<charT, idT> &other) : Q_transitions(nullptr),
                                                                                               Q_table(other.Q_table), _M_transitions_deletable(true)
{
    if (!other.Q_transitions)
        throw std::runtime_error("error: empty basic_allocator in copy contructor: basic_allocator(const std::basic_allocator& other)");
    myregex::basic_table<charT, idT>::copy(*this, other);
}
template <typename charT, typename idT>
myregex::basic_table<charT, idT>::basic_table(myregex::basic_table<charT, idT> &&other) : Q_transitions(nullptr), _M_transitions_deletable(true)
{
    if (other.Q_transitions)
    {
        Q_transitions = other.Q_transitions;
        other.Q_transitions = nullptr;
        Q_table = std::move(other.Q_table);
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
        _M_transitions_deletable = true;
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
            Q_table = std::move(other.Q_table);
        }
        _M_transitions_deletable = true;
    }
    return *this;
}
template <typename charT, typename idT>
myregex::basic_table<charT, idT>::~basic_table()
{
    if (Q_transitions != nullptr && _M_transitions_deletable)
    {
        delete[] Q_transitions;
        Q_transitions = nullptr;
    }
}

#endif