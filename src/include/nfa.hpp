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
    using Fnfa = std::map<size_t, size_t>;

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
    void view() const;
    ~NFA() {}
};

#endif