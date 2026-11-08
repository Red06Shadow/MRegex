template <typename charT, typename idT>
size_t myregex::basic_dfa<charT, idT>::size() const
{
    size_t _size = 0;
    _size += sizeof(size_t);
    _size += Q_dfa.size() * (sizeof(myregex::state<idT>) * 2) + sizeof(Qdfa);
    _size += Q_transitions.size() * (sizeof(size_t) * 2 + sizeof(charT)) + sizeof(Transitions);
    return _size;
}