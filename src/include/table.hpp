#ifndef _TABLEDFA
#define _TABLEDFA

#include <corecrt.h>
#include <set>

class MRegex;

class TableDFA
{
private:
    using _S_Table_Transitions = size_t **;
    const size_t Q_dfa, dictionary;
    std::set<size_t> F_dfa;
    _S_Table_Transitions Q_transitions;
    TableDFA() : Q_transitions(nullptr), Q_dfa(0), dictionary(0), F_dfa() {}
    TableDFA(size_t, size_t, const std::set<size_t>&);
    TableDFA(size_t, size_t, std::set<size_t>&&);
    inline static _S_Table_Transitions build(size_t, size_t);
    inline static void copy(TableDFA&, const TableDFA&);
public:
    TableDFA(const TableDFA&);
    TableDFA(TableDFA&&);
    // TableDFA& operator=(const TableDFA&);
    // TableDFA& operator=(TableDFA&&);
    ~TableDFA();
    void view() const;
    inline size_t size() const { return Q_dfa * dictionary * sizeof(size_t);}
    friend MRegex;
};
#endif