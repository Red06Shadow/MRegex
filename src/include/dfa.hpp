#ifndef _DFA
#define _DFA

#include <corecrt.h>
#include <map>
#include <set>
#include <vector>

class _regex;


class DFA
{
public: //  Structs, Enums, Class
    // En la teoria estandar de DFA no existe esta estructura, en este caso se utiliza para manejar transiciones con mayor precision y ahorrando memoria
    struct _M_Cualifiquer
    {
        size_t min;
        size_t max;
        _M_Cualifiquer() : min(1), max(1) {}
        _M_Cualifiquer(size_t _a, size_t _b) : min(_a), max(_b) {}
    };

    using _M_State = size_t;
    using _M_Set_State = std::set<size_t>;
    using _M_Pair_Key_Transition = std::pair<size_t, char>;
    // Solo remplazamos el _M_State del valor por un std::pair<_M_State, _M_Cualifiquer>
    // using _M_Map_Transitions = std::map<_M_Pair_Key_Transition, _M_State>;
    using _M_Map_Transitions = std::map<_M_Pair_Key_Transition, std::pair<_M_State, _M_Cualifiquer>>;

private:
    std::vector<_M_Set_State> Q_dfa;
    _M_Map_Transitions delta_dfa;
    _M_Set_State F_dfa;
    std::vector<_M_State> _M_begins_states;

public:
    DFA() : Q_dfa(), delta_dfa({}), F_dfa({}), _M_begins_states({}) {}
    void view();
    ~DFA() {}
    friend _regex;
};

#endif
