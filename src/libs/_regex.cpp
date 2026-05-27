#include <_regex.hpp>
#include <stack>
#include <queue>

NFA _regex::build_NFA(const std::initializer_list<std::pair<int, std::string>> &regexs)
{
    NFA nfa = NFA();
    // Reserva espacios mas para evitar realloc inecesarias
    nfa._M_nfa_begins.reserve(nfa._M_nfa_begins.size() + regexs.size());
    for (auto &&i : regexs)
    {
        // Obtenemos el iterador de la cadena string
        _C_char_range<char> range = i.second;
        // Agregamos la posicion de inicial de la rega procesada en _S_parser en el vector de primcipios
        nfa._M_nfa_begins.push_back({_regex::_S_parser(range, nfa, -1ULL), i.first});
    }
    return nfa;
}

void _regex::_S_position_assigned(size_t &strats, size_t &before, size_t actual, NFA &nfa)
{
    // Si el strats de _S_parser_nodes es -1ULL lo actualizamos junto a la posicion con el indice del ultimo elemento
    if (strats == -1ULL)
        strats = before = actual;
    else
    {
        // Si no simplemente actualizamos el actual del elemento agregado y actualizamos before con el indice del ultimo elemento
        nfa._M_nfa_state[before]._M_next(actual);
        before = actual;
    }
}

NFA::_M_Cualifiquer _regex::_S_parser_cualifiquer(_C_char_range<char> &range)
{
    // Por defecto las reglas tienen un minimo y un maximo de 1, una lectura normal
    if (range.endp())
        return {1, 1};
    char c = *range.peak();
    switch (c)
    {
    case '?':
    {
        range.next();
        return {0, 1};
        break;
    }
    case '*':
    {
        range.next();
        return {0, -1ULL};
        break;
    }
    case '+':
    {
        range.next();
        return {1, -1ULL};
        break;
    }
    default:
        return {1, 1};
        break;
    }
}

NFA::_M_Class _regex::_S_parser_class(_C_char_range<char> &range)
{
    NFA::_M_Class _class = NFA::_M_Class();
    if (*range.peak() == '^')
    {
        _class._M_inverter = true;
        range.next();
    }
    char a;
    while (!range.endp() and (a = *range.peak()) != ']')
    {
        if (a == '\\')
            a = _regex::_S_parser_escape(range);
        if (range.endp(range.offset(1)) or *(range.offset(1)) != '-')
            _class._M_push(NFA::_M_Range(a));
        else
        {
            auto offset = range.offset(2);
            char b;
            if (range.endp(offset) or (b = *offset) == ']')
                throw std::runtime_error("Error: termination max incurret");
            // _regex::_S_parser_escape(range);
            _class._M_push(NFA::_M_Range(a, b));
            range.set(2);
        }
        range.next();
    }
    // Error empty
    return _class;
}

size_t _regex::_S_parser_nodes(_C_char_range<char> &range, NFA &nfa, size_t NFA_group)
{
    size_t starts = -1ULL;
    size_t before = nfa._M_nfa_state.size() - 1ULL;
    size_t actual = nfa._M_nfa_state.size() - 1ULL;
    while (!range.endp())
    {
        char c = *range.peak();
        if (c == '|' or c == ')')
            break;
        range.next();
        switch (c)
        {
        case '(':
        {
            actual = nfa._M_nfa_state.size();
            nfa._M_nfa_state.push_back(NFA::_M_State(NFA::_M_Group(-1ULL)));
            nfa._M_nfa_state[actual] = NFA::_M_State(NFA::_M_Group(_regex::_S_parser(range, nfa, actual)));
            if (range.endp() or *range.peak() != ')')
                throw std::runtime_error("Error: Bad termination in group");
            range.next();
            break;
        }
        case '[':
        {
            actual = nfa._M_nfa_state.size();
            nfa._M_nfa_state.push_back(NFA::_M_State(_regex::_S_parser_class(range)));
            if (range.endp() or *range.peak() != ']')
                throw std::runtime_error("Error: Bad termination in class");
            range.next();
            break;
        }
        case '\\':
        {
            c = _regex::_S_parser_escape(range);
        }
        default:
            actual = nfa._M_nfa_state.size();
            nfa._M_nfa_state.push_back(NFA::_M_State(NFA::_M_Char(c)));
            break;
        }
        if (NFA_group != -1ULL)
            nfa._M_nfa_groups[NFA_group].insert(actual);
        _regex::_S_position_assigned(starts, before, actual, nfa);
        nfa._M_nfa_state[actual]._M_set_cualifiquer(_regex::_S_parser_cualifiquer(range));
    }
    return starts;
}

size_t _regex::_S_parser(_C_char_range<char> &range, NFA &nfa, size_t NFA_group)
{
    size_t lhs = _regex::_S_parser_nodes(range, nfa, NFA_group);
    if (range.endp())
        return lhs;
    if (*range.peak() == '|')
    {
        range.next();
        size_t rhs = _regex::_S_parser(range, nfa, NFA_group);
        size_t or_sentences = nfa._M_nfa_state.size();
        nfa._M_nfa_state.push_back(NFA::_M_State(NFA::_M_Bifucacion(lhs, rhs)));
        if (NFA_group != -1ULL)
            nfa._M_nfa_groups[NFA_group].insert(or_sentences);
        return or_sentences;
    }
    return lhs;
}
char _regex::_S_parser_escape(_C_char_range<char> &range)
{
    range.next();
    if (range.endp())
        throw std::runtime_error("Error: termination indefinated");
    char c = *range.peak();
    if (c != 'x' and c != 'u' and c != 'b' and (c < '0' or c > '9'))
        return c;
    return 0;
}

const char *_regex::_S_Intern_caption_NFA(_C_char_range<char> &range, const NFA &nfa, size_t begin)
{
    size_t position = begin;
    const char *end = nullptr;
    while (position != -1ULL)
    {
        end = std::visit([&range, &nfa, begin, &position](auto &&value) -> const char *
                         {
            using _Type = std::decay_t<decltype(value)>;
            size_t n = 0;
            if constexpr (std::is_same_v<_Type, NFA::_M_Char> | std::is_same_v<_Type, NFA::_M_Class>) {
                //Funcion base para _M_Char yy _M_Class
                while (range.iterator != range.end && value == *range.peak() && n < value._M_cualifiquer.max)
                {
                    n ++;
                    range.next();
                }
                if (n < value._M_cualifiquer.min)
                    return nullptr;
                position = value._M_next;
                return range.iterator;
            }
            else if constexpr (std::is_same_v<_Type, NFA::_M_Group>) {
                //Funcion base para _M_Group
                const char *finaly = range.iterator;
                while (range.iterator != range.end && _regex::_S_Intern_caption_NFA(range, nfa, value._M_reference) && n < value._M_cualifiquer.max){
                    finaly = range.iterator;
                    n ++;
                }
                if (n < value._M_cualifiquer.min)
                    return nullptr;
                position = value._M_next;
                return (range.iterator = finaly);
            }
            else 
            {
                position = -1ULL;
                const char* lhs = _regex::_S_Intern_caption_NFA(range, nfa, value._M_lhs);
                if (lhs != 0)
                    return lhs;
                return _regex::_S_Intern_caption_NFA(range, nfa, value._M_rhs);
            } }, nfa._M_nfa_state[position]._M_refrence);
        if (!end)
            break;
    }
    return end;
}

std::pair<const char *, const char *> _regex::caption(const std::string &str, const NFA &nfa)
{
    const char *end = nullptr;
    _C_char_range<char> range = _C_char_range<char>(str);
    for (auto &&i : nfa._M_nfa_begins)
    {
        end = _regex::_S_Intern_caption_NFA(range, nfa, i.first);
        if (end != nullptr)
            break;
        range.iterator = range.begin;
    }
    return {range.begin, end};
}
std::pair<const char *, const char *> _regex::caption(const std::string &str, const DFA &dfa)
{
    int estado_actual = 0; // dfa._M_begin_state;
    std::string::const_iterator end = str.begin();

    for (std::string::const_iterator i = str.begin(); i != str.end(); i++)
    {
        char c = *i;
        auto it = dfa.delta_dfa.find({estado_actual, c});
        if (it == dfa.delta_dfa.end())
            break; // No hay transición, rechazar
        estado_actual = it->second.first;
        end++;
    }
    return {str.begin().base(), dfa.F_dfa.count(estado_actual) > 0 ? end.base() : nullptr};
}

size_t _regex::_S_index_for_nfa_groups(const NFA &nfa, size_t _value)
{
    for (auto &&[key, Gset] : nfa._M_nfa_groups)
        if (Gset.count(_value))
            return std::get<NFA::_M_Group>(nfa._M_nfa_state[key]._M_refrence)._M_next;
        
        // {
        //     return std::visit([](auto &&value) -> size_t
        //                       {
        //                         using _Type = std::decay_t<decltype(value)>;
        //                         if constexpr (std::is_same_v<_Type, NFA::_M_Group>) 
        //                             return value._M_next; 
        //                         return -1ULL; }, nfa._M_nfa_state[key]._M_refrence);
        // }
    return -1ULL;
}

void _regex::generate_transitions(DFA::_M_Map_Transitions &transitions, char c, const NFA &nfa, DFA::_M_State begin, DFA::_M_State end)
{
    auto& element = nfa._M_nfa_state[begin]._M_refrence;
    std::visit([begin, end, nfa, &transitions, element, c](auto &&value) -> void
               {
            using _Type = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<_Type, NFA::_M_Class> | std::is_same_v<_Type, NFA::_M_Char>) {
                // Primero generamos la transicion base al siguinte estado
                transitions[{begin, c}] = std::pair<DFA::_M_State, DFA::_M_Cualifiquer>(end, {1, 1});
                // Si el siguiente esatdo es reiterable se agrega 
                if (value._M_cualifiquer.max > 1)
                    transitions[{end, c}] = std::pair<DFA::_M_State, DFA::_M_Cualifiquer>(end, {1, value._M_cualifiquer.max});
                // Luego verificamos que si pertenece a un grupo y si es final
                size_t result = (value._M_next != -1ULL ? -1ULL : _regex::_S_index_for_nfa_groups(nfa, begin));
                if (result == -1ULL)
                    return ;
                // Si es la regla final y es un grupo lo obtenemos y si la cantidad de transiciones maximas supera la unidad(1)
                const NFA::_M_Group& group = std::get<NFA::_M_Group>(nfa._M_nfa_state[result]._M_refrence);
                if (group._M_cualifiquer.max < 2)
                    return ;
                // En caso de cumplirse todo lo anterior agregamos una transicion de retroceso hacia el primer elemento ( por ahora no debe funcionar)
                // transitions[{end, c}] = std::pair<DFA::_M_State, DFA::_M_Cualifiquer>(end, {1, value._M_cualifiquer.max});
            }
            // else if constexpr (std::is_same_v<_Type, NFA::_M_Group>) {
            //     transitions[{begin, c}] = std::pair<DFA::_M_State, DFA::_M_Cualifiquer>(end, {1, 1});
            // }
            else {
                transitions[{begin, c}] = std::pair<DFA::_M_State, DFA::_M_Cualifiquer>(end, {1, 1});
            } }, element);
    // Agregar Transicion
}

void _regex::elipson_closure_repeats(DFA::_M_State state, char c, const NFA &nfa, DFA::_M_Map_Transitions &transitions)
{
    // std::visit([state, &transitions, c](auto &&value) -> void
    //            {
    //         using _Type = std::decay_t<decltype(value)>;
    //         if constexpr (std::is_same_v<_Type, NFA::_M_Class> | std::is_same_v<_Type, NFA::_M_Char> | std::is_same_v<_Type, NFA::_M_Group>) {
    //             if (value._M_cualifiquer.max > 1) {
    //                 transitions[{state + 1, c}] = {state + 1, {}};
    //             }
    //         } }, nfa._M_nfa_state[state]._M_refrence);
}

DFA::_M_Set_State _regex::move(DFA::_M_Set_State _Set_states, const NFA &nfa, char c)
{
    DFA::_M_Set_State _closure = {};
    for (auto &&idx : _Set_states)
    {
        if (idx == -1ULL)
            continue;
        std::visit([&_closure, &nfa, idx, c](auto &&value) -> void
                   {
            using _Type = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<_Type, NFA::_M_Char>) {
                if (c == value._M_char)
                    _closure.insert(value._M_next != -1ULL ? value._M_next : _regex::_S_index_for_nfa_groups(nfa, idx));
            }
            else if constexpr (std::is_same_v<_Type, NFA::_M_Class>) {
                bool boolean = value == c;
                if (value._M_inverter)
                    boolean = !boolean;
                if (boolean)
                    _closure.insert(value._M_next != -1ULL ? value._M_next : _regex::_S_index_for_nfa_groups(nfa, idx));
            } }, nfa._M_nfa_state[idx]._M_refrence);
    }
    return _closure;
}

DFA::_M_Set_State _regex::move_e(DFA::_M_Set_State _Set_states, const NFA &nfa, char c)
{
    DFA::_M_Set_State _closure = {};

    return _closure;
}

DFA::_M_Set_State _regex::elipson_closure_e(DFA::_M_Set_State _Set_states, const NFA &nfa)
{
    DFA::_M_Set_State _closure = {};
    
    return _closure;
}

DFA::_M_Set_State _regex::elipson_closure(DFA::_M_Set_State _Set_states, const NFA &nfa)
{
    DFA::_M_Set_State _closure = _Set_states;
    std::stack<DFA::_M_State> _stack = {};

    for (auto &&e : _Set_states)
        _stack.push(e);

    while (!_stack.empty())
    {
        DFA::_M_State idx = _stack.top();
        _stack.pop();
        if (idx == -1ULL)
            continue;
        std::visit([&_closure, &nfa, idx, &_stack](auto &&value) -> void
                   {
            using _Type = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<_Type, NFA::_M_Group>) {
                if (!_closure.count(value._M_reference)) { 
                    size_t result = (value._M_reference != -1ULL ? value._M_reference : _regex::_S_index_for_nfa_groups(nfa, idx));
                    _closure.insert(result); _stack.push(result);
                }
                if (!value._M_cualifiquer.min && !_closure.count(value._M_next)) {
                    size_t result = (value._M_next != -1ULL ? value._M_next : _regex::_S_index_for_nfa_groups(nfa, idx));
                    _closure.insert(result); _stack.push(result);
                }
            }
            else if constexpr (std::is_same_v<_Type, NFA::_M_Bifucacion>) {
                if (!_closure.count(value._M_lhs)) { 
                    size_t result = value._M_lhs != -1ULL ? value._M_lhs : _regex::_S_index_for_nfa_groups(nfa, idx);
                    _closure.insert(result); _stack.push(result);
                }
                if (!_closure.count(value._M_rhs)) { 
                    size_t result = value._M_rhs != -1ULL ? value._M_rhs : _regex::_S_index_for_nfa_groups(nfa, idx);
                    _closure.insert(result); _stack.push(result);
                }
            }
            else
            {
                if (!value._M_cualifiquer.min && !_closure.count(value._M_next)) {
                    size_t result = (value._M_next != -1ULL ? value._M_next : _regex::_S_index_for_nfa_groups(nfa, idx));
                    _closure.insert(result); _stack.push(result);
                }
            } }, nfa._M_nfa_state[idx]._M_refrence);
    }
    return _closure;
}

DFA _regex::convert_NFA_to_DFA(const NFA &nfa)
{
    DFA dfa = DFA();
    std::queue<DFA::_M_Set_State> _M_Set_Queue;
    DFA::_M_Set_State q0;

    for (auto &&begin : nfa._M_nfa_begins)
    {
        q0 = _regex::elipson_closure({begin.first}, nfa);
        dfa.Q_dfa = {q0};
        dfa._M_begins_states.push_back(begin.first);
        _M_Set_Queue.push(q0);

        






        // std::map<DFA::_M_Set_State, size_t> id_Status = {{q0, 0}};
        
        // while (!_M_Set_Queue.empty())
        // {
        //     DFA::_M_Set_State _M_Set_Actual = _M_Set_Queue.front();
        //     _M_Set_Queue.pop();
        //     DFA::_M_State status_actual = id_Status[_M_Set_Actual];

        //     if (_M_Set_Actual.find(-1ULL) != _M_Set_Actual.end())
        //         dfa.F_dfa.insert(status_actual);

            

            // for (int c = char(32); c < 128; c++)
            // {
            //     // Obtenemos todos los posibles estados alcanzables desde el estado actual
            //     DFA::_M_Set_State _M_set_next = _regex::move_elipson_closure(_M_Set_Actual, nfa, char(c));
            //     if (!_M_set_next.empty())
            //     {
            //         if (!id_Status.count(_M_set_next))
            //         {
            //             DFA::_M_State _M_new_Status = dfa.Q_dfa.size();
            //             dfa.Q_dfa.push_back(_M_set_next);
            //             id_Status[_M_set_next] = _M_new_Status;
            //             _M_Set_Queue.push(_M_set_next);
            //         }
            //         DFA::_M_State _M_next_Status = id_Status[_M_set_next];
            //         _regex::generate_transitions(dfa.delta_dfa, c, nfa, status_actual, _M_next_Status);
            //     }
            // }
        // }
    }
    return dfa;
}