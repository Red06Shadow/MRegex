#ifndef _NFA
#define _NFA

#include <iostream>
#include <vector>
#include <type_traits>
#include <stdexcept>
#include <map>
#include <set>
#include <myregex/utilities/selector.hpp>

namespace myregex
{

    template <typename charT, typename idT>
    class basic_builder;

    template <typename charT, typename idT>
    class basic_nfa
    {
    public:
        static_assert(std::is_same_v<charT, char> || std::is_same_v<charT, wchar_t>, "Error: no se permiten tipos de datos que no sean de caracteres(solo char o wchar_t)");
        // Estados
        using Qnfa = std::vector<size_t>;
        // Transicion estandar
        using Transitions = std::map<std::pair<size_t, size_t>, std::vector<size_t>>;
        using Dictionary = std::set<charT>;
        using Fnfa = std::map<size_t, idT>;

    private:
        /// @brief Estados
        Qnfa Q_nfa;
        /// @brief Estados de inicio
        Qnfa begin_Q_nfa;
        /// @brief Transiciones
        Transitions Q_transitions;
        /// @brief Diccionario
        Dictionary Q_dictionary;
        /// @brief Estados de Acpetacion
        Fnfa F_nfa;
        friend basic_builder<charT, idT>;

    public:
        basic_nfa() : Q_nfa(), begin_Q_nfa(), Q_transitions(), Q_dictionary(), F_nfa() {}
        inline const Qnfa &status() const { return Q_nfa; }
        inline const Qnfa &begin_status() const { return begin_Q_nfa; }
        inline const Transitions &transitions() const { return Q_transitions; }
        inline const Dictionary &dictionary() const { return Q_dictionary; }
        inline const Fnfa &accepted_status() const { return F_nfa; }
        void view() const;
        size_t size() const;
        ~basic_nfa() {}
    };

    template <typename charT, typename idT>
    size_t basic_nfa<charT, idT>::size() const
    {
        size_t _size = 0;
        _size += Q_nfa.size() * sizeof(size_t);
        _size += Q_dictionary.size() * sizeof(charT) + sizeof(Dictionary);
        _size += begin_Q_nfa.size() * sizeof(size_t) + sizeof(Qnfa);
        _size += F_nfa.size() * (sizeof(size_t) * 2) + sizeof(Fnfa);
        _size += Q_transitions.size() * (sizeof(size_t) * 2 + sizeof(charT)) + sizeof(Transitions);
        return _size;
    }

    template <typename charT, typename idT>
    void basic_nfa<charT, idT>::view() const
    {
        myregex::selector<charT>::stream() << "Diccionario (dictionary): { ";

        for (auto &&letter : Q_dictionary)
            myregex::selector<charT>::stream() << letter << ' ';

        myregex::selector<charT>::stream() << '}' << std::endl
                   << "Transiciones (Q_transitions): " << std::endl
                   << '{' << std::endl;
        for (auto &&[key, states] : Q_transitions)
        {
            if (key.second == -1ULL)
            {
                myregex::selector<charT>::stream() << "    {" << key.first << "} -> { ";
                for (size_t state = 0; state < states.size(); state++)
                    myregex::selector<charT>::stream() << states[state] << (state >= states.size() - 1ULL ? " }" : ", ");
                myregex::selector<charT>::stream() << std::endl;
            }
            else
                myregex::selector<charT>::stream() << "    {" << key.first << ", " << charT(key.second) << "} -> { " << states.back() << " }" << std::endl;
        }
        myregex::selector<charT>::stream() << '}' << std::endl 
                   << "Estados de Inicio (begin_Q_nfa): { ";

        for (auto &&state : begin_Q_nfa)
            myregex::selector<charT>::stream() << state << ' ';

        myregex::selector<charT>::stream() << "}" << std::endl
                   << "Estados de aceptacion (F_dfa): { " << std::endl;

        for (auto &&[key, value] : F_nfa)
            myregex::selector<charT>::stream() << '{' << key << "; id: " << value << '}' << std::endl;

        myregex::selector<charT>::stream() << '}' << std::endl;
    }
    template <typename idT>
    using CompatibleNfa = myregex::basic_nfa<char, idT>;
    template <typename idT>
    using nfa = myregex::basic_nfa<char, idT>;
    /////////////////////////////////////////////////////////////////////
    template <typename idT>
    using UnicodeNfa = myregex::basic_nfa<wchar_t, idT>;
    template <typename idT>
    using wnfa = myregex::basic_nfa<wchar_t, idT>;
} // namespace myregex
#endif