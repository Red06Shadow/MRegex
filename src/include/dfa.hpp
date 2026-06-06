#ifndef _DFA
#define _DFA

#include <corecrt.h>
#include <map>
#include <set>
#include <vector>

class MRegex;

class DFA
{
public: //  Structs, Enums, Class
    using States = std::set<size_t>;
    using Qdfa = std::vector<States>;
    using Transitions = std::map<std::pair<size_t, char>, size_t>;
    using Dictionary = std::set<char>;
    using Fdfa = std::set<size_t>;

private:
    /// @brief Estados
    Qdfa Q_dfa;
    /// @brief Estados de inicio
    States begin_Q_dfa;
    /// @brief Transiciones
    Transitions Q_transitions;
    /// @brief Diccionario
    Dictionary dictionary;
    /// @brief Estados de Acpetacion
    Fdfa F_dfa;

public:
    DFA() : Q_dfa(), Q_transitions({}), dictionary({}), F_dfa({}), begin_Q_dfa({}) {}
    void view();
    ~DFA() {}
    friend MRegex;
};

#endif
