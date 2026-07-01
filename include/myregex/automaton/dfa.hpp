#ifndef _DFA
#define _DFA

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <corecrt.h>
#include <type_traits>
#include <myregex/utilities/selector.hpp>

namespace myregex
{
    template <typename charT, typename idT>
    class basic_builder;

    template <typename charT, typename idT>
    class basic_dfa
    {
    public: //  Structs, Enums, Class
        static_assert(std::is_same_v<charT, char> || std::is_same_v<charT, wchar_t>, "Error: no se permiten tipos de datos que no sean de caracteres(solo char o wchar_t)");
        using Transitions = std::map<std::pair<size_t, charT>, idT>;
        using Fdfa = std::map<size_t, idT>;

    private:
        /// @brief Numero de Estados
        size_t Q_dfa;
        /// @brief Transiciones
        Transitions Q_transitions;
        /// @brief Estados de Acpetacion
        Fdfa F_dfa;

    public:
        basic_dfa() : Q_transitions({}), F_dfa({}) {}
        inline const Transitions &transitions() const { return Q_transitions; }
        inline const Fdfa &accepted_status() const { return F_dfa; }
        size_t size() const;
        void view() const;
        ~basic_dfa() {}
        friend basic_builder<charT, idT>;
    };
    template <typename charT, typename idT>
    size_t basic_dfa<charT, idT>::size() const
    {
        size_t _size = 0;
        _size += sizeof(size_t);
        _size += F_dfa.size() * (sizeof(size_t) * 2) + sizeof(Fdfa);
        _size += Q_transitions.size() * (sizeof(size_t) * 2 + sizeof(charT)) + sizeof(Transitions);
        return _size;
    }
    template <typename charT, typename idT>
    void basic_dfa<charT, idT>::view() const
    {
        myregex::selector<charT>::stream() << "Transiciones (Q_transitions): " << std::endl
                                  << '{' << std::endl;

        for (auto &&[key, state] : Q_transitions)
            myregex::selector<charT>::stream() << "    {" << key.first << ", " << charT(key.second) << "} -> { " << state << " }" << std::endl;
        myregex::selector<charT>::stream() << '}' << std::endl;
        myregex::selector<charT>::stream() << "Estados de aceptacion (F_dfa): { " << std::endl;

        for (auto &&[key, value] : F_dfa)
            myregex::selector<charT>::stream() << '{' << key << "; id: " << value << '}' << std::endl;

        myregex::selector<charT>::stream() << '}' << std::endl;
    }
    template <typename idT>
    using CompatibleDfa = basic_dfa<char, idT>;
    template <typename idT>
    using dfa = basic_dfa<char, idT>;
    /////////////////////////////////////////////////////////////////////
    template <typename idT>
    using UnicodeDfa = basic_dfa<wchar_t, idT>;
    template <typename idT>
    using wdfa = basic_dfa<wchar_t, idT>;
} // namespace myregex

#endif
