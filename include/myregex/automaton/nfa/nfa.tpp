template <typename charT, typename idT>
size_t myregex::basic_nfa<charT, idT>::size() const
{
    size_t _size = 0;
    _size += status().size() * sizeof(myregex::state<idT>) + sizeof(Qnfa);
    _size += Q_dictionary.size() * sizeof(charT) + sizeof(Dictionary);
    _size += begin_Q_nfa.size() * sizeof(size_t) + sizeof(SQnfa);
    _size += Q_transitions.size() * (sizeof(size_t) * 2 + sizeof(charT)) + sizeof(Transitions);
    return _size;
}