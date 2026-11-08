template <typename charT, typename idT>
bool myregex::basic_regex<charT, idT>::verification(typename std::basic_string_view<charT>::const_iterator& iterator, const std::basic_string_view<charT> &str, const myregex::basic_nfa<charT, idT> &_nfa)
{
    std::set<size_t> status = myregex::basic_builder<charT, idT>::_S_elipson_cloursers({0}, _nfa);
    while (iterator < str.end())
    {
        charT letter = *iterator;
        status = myregex::basic_builder<charT, idT>::_S_move_elipson_cloursers(status, letter, _nfa);
        if (status.empty())
            break;
        iterator++;
    }
    for (auto &&state : status)
        if (_nfa.status()[state].valid())
            return true;
    return false;
}

template <typename charT, typename idT>
bool myregex::basic_regex<charT, idT>::verification(typename std::basic_string_view<charT>::const_iterator& iterator, const std::basic_string_view<charT> &str, const myregex::basic_dfa<charT, idT> &_dfa)
{
    size_t status = 0;
    while (iterator < str.end())
    {
        charT letter = *iterator;
        auto transition = _dfa.transitions().find({status, letter});
        if (transition == _dfa.transitions().end())
            return false;
        status = transition->second;
        iterator++;
    }
    return iterator == str.end() && _dfa.status()[status].valid();
}
template <typename charT, typename idT>
bool myregex::basic_regex<charT, idT>::verification(typename std::basic_string_view<charT>::const_iterator& iterator, const std::basic_string_view<charT> &str, const myregex::basic_table<charT, idT> &_table)
{
    size_t status = 0;
    while (iterator < str.end())
    {
        charT letter = *iterator;
        if ((status = _table.transitions()[(status * _table.dictionary) + letter]) == -1ULL)
            return false; // No hay transición, rechazar
        iterator++;
    }
    return iterator == str.end() && _table.status()[status].valid();
}

template <typename charT, typename idT>
bool myregex::basic_regex<charT, idT>::verification(typename std::basic_string_view<charT>::const_iterator& iterator, const std::basic_string_view<charT> &str) const
{
    return std::visit([&iterator, str](auto &&value) -> bool
                      {
            using type = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<type, basic_nfa<charT, idT>> || std::is_same_v<type, basic_dfa<charT, idT>> || std::is_same_v<type, basic_table<charT, idT>>)
                return myregex::basic_regex<charT, idT>::verification(iterator, str, value);
            else
                return false; }, _M_expresions);
}

template <typename charT, typename idT>
template <myregex::constants::match_options option>
myregex::caption<charT, idT> myregex::basic_regex<charT, idT>::match(typename std::basic_string_view<charT>::const_iterator& iterator, const std::basic_string_view<charT> &str, const myregex::basic_nfa<charT, idT> &_nfa)
{
    std::__cxx11::basic_string<charT> _M_string{};
    idT id{};
    bool _M_valid = false;
    std::set<size_t> status = myregex::basic_builder<charT, idT>::_S_elipson_cloursers({0}, _nfa);
    while (iterator < str.end())
    {
        idT __id{};
        charT letter = *iterator;
        status = myregex::basic_builder<charT, idT>::_S_move_elipson_cloursers(status, letter, _nfa);
        if (status.empty())
            break;
        _M_string.push_back(letter);
        for (auto &&state : status)
        {
            if (_nfa.status()[state].valid() && (!_M_valid || _nfa.status()[state].get() < __id))
            {
                __id = _nfa.status()[state].get();
                _M_valid = true;
            }
        }
        if constexpr (option == myregex::constants::match_options::_S_first_sequence)
            return {_M_string, __id};
        else if constexpr (option == myregex::constants::match_options::_S_maximun_sequence)
            id = __id;
        iterator++;
    }
    if constexpr (option == myregex::constants::match_options::_S_maximun_sequence)
        return myregex::caption<charT, idT>(_M_string, id);
    else if constexpr (option == myregex::constants::match_options::_S_first_sequence)
        return myregex::caption<charT, idT>({}, {});
}

template <typename charT, typename idT>
template <myregex::constants::match_options option>
myregex::caption<charT, idT> myregex::basic_regex<charT, idT>::match(typename std::basic_string_view<charT>::const_iterator& iterator, const std::basic_string_view<charT> &str, const myregex::basic_dfa<charT, idT> &_dfa)
{
    std::__cxx11::basic_string<charT> _M_string{};
    idT id{};
    size_t status = 0;
    size_t acceptance_status = -1ULL;
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    if (status == acceptance_status)
        ; // Agregue esta condicional para que si el estado actual es igual a un estado de aceptacion(casos como * o +) automaticamente evite la comprobacion
    else if (_dfa.status()[status].valid())
    {
        acceptance_status = status;
        if constexpr (option == myregex::constants::match_options::_S_first_sequence)
            return {_M_string, _dfa.accepted_status().at(acceptance_status)};
        else if constexpr (option == myregex::constants::match_options::_S_maximun_sequence)
            id = _dfa.status()[acceptance_status].get();
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    while (iterator < str.end())
    {
        charT letter = *iterator;
        auto transition = _dfa.transitions().find({status, letter});
        if (transition == _dfa.transitions().end())
            break; // No hay transición, rechazar
        status = transition->second;
        _M_string.push_back(letter);
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        if (status == acceptance_status)
            ; // Agregue esta condicional para que si el estado actual es igual a un estado de aceptacion(casos como * o +) automaticamente evite la comprobacion
        else if (_dfa.status()[status].valid())
        {
            acceptance_status = status;
            if constexpr (option == myregex::constants::match_options::_S_first_sequence)
                return {_M_string, _dfa.accepted_status().at(acceptance_status)};
            else if constexpr (option == myregex::constants::match_options::_S_maximun_sequence)
                id = _dfa.status()[acceptance_status].get();
        }
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        iterator++;
    }
    if constexpr (option == myregex::constants::match_options::_S_maximun_sequence)
        return myregex::caption<charT, idT>(_M_string, id);
    else if constexpr (option == myregex::constants::match_options::_S_first_sequence)
        return myregex::caption<charT, idT>({}, {});
}
template <typename charT, typename idT>
template <myregex::constants::match_options option>
myregex::caption<charT, idT> myregex::basic_regex<charT, idT>::match(typename std::basic_string_view<charT>::const_iterator& iterator, const std::basic_string_view<charT> &str, const myregex::basic_table<charT, idT> &_table)
{
    std::__cxx11::basic_string<charT> _M_string{};
    idT id{};
    size_t status = 0;
    size_t acceptance_status = -1ULL;
    acceptance_status = status;
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    if (_table.status()[status].valid())
    {
        acceptance_status = status;
        if constexpr (option == myregex::constants::match_options::_S_first_sequence)
            return {_M_string, _table.accepted_status().at(acceptance_status)};
        else if constexpr (option == myregex::constants::match_options::_S_maximun_sequence)
            id = _table.status()[acceptance_status].get();
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    while (iterator < str.end())
    {
        charT letter = *iterator;
        size_t next_state = _table.transitions()[(status * _table.dictionary) + myregex::basic_builder<charT, idT>::_S_transition(letter)];
        if (next_state == -1ULL)
            break; // No hay transición, rechazar
        status = next_state;
        _M_string.push_back(letter);
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        if (status == acceptance_status)
            ;
        else if (_table.status()[status].valid())
        {
            acceptance_status = status;
            if constexpr (option == myregex::constants::match_options::_S_first_sequence)
                return {_M_string, _table.accepted_status().at(acceptance_status)};
            else if constexpr (option == myregex::constants::match_options::_S_maximun_sequence)
                id = _table.status()[acceptance_status].get();
        }
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        iterator++;
    }
    if constexpr (option == myregex::constants::match_options::_S_maximun_sequence)
        return myregex::caption<charT, idT>(_M_string, id);
    else if constexpr (option == myregex::constants::match_options::_S_first_sequence)
        return myregex::caption<charT, idT>({}, {});
}

template <typename charT, typename idT>
template <myregex::constants::match_options option>
myregex::caption<charT, idT> myregex::basic_regex<charT, idT>::match(typename std::basic_string_view<charT>::const_iterator& iterator, const std::basic_string_view<charT> &str) const
{
    return std::visit([&iterator, str](auto &&value) -> myregex::caption<charT, idT>
                      {
            using type = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<type, basic_nfa<charT, idT>> || std::is_same_v<type, basic_dfa<charT, idT>> || std::is_same_v<type, basic_table<charT, idT>>)
                return myregex::basic_regex<charT, idT>::template match<option>(iterator, str, value);
            else
                return false; }, _M_expresions);
}

template <typename charT, typename idT>
size_t myregex::basic_regex<charT, idT>::size() const
{
    return std::visit([](auto &&value) -> size_t
                      {
            using type = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<type, basic_nfa<charT, idT>> || std::is_same_v<type, basic_dfa<charT, idT>> || std::is_same_v<type, basic_table<charT, idT>>)
                return value.size();
            else
                return 0; }, _M_expresions);
}
template <typename charT, typename idT>
void myregex::basic_regex<charT, idT>::export_automaton(std::basic_ostream<charT> &out) const
{
    std::visit([&out](auto &&value) -> void
               {
            using type = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<type, basic_nfa<charT, idT>> || std::is_same_v<type, basic_dfa<charT, idT>> || std::is_same_v<type, basic_table<charT, idT>>)
                out << value; }, _M_expresions);
}