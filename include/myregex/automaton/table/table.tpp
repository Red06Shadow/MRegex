
template <typename charT, typename idT>
myregex::basic_table<charT, idT> &myregex::basic_table<charT, idT>::operator=(const basic_table &other)
{
    if (&other != this)
    {
        Q_transitions = other.Q_transitions;
        Q_table = other.Q_table;
    }
    return *this;
}
template <typename charT, typename idT>
myregex::basic_table<charT, idT> &myregex::basic_table<charT, idT>::operator=(basic_table &&other)
{
    if (&other != this)
    {
        Q_transitions = std::move(other.Q_transitions);
        Q_table = std::move(other.Q_table);
    }
    return *this;
}
template <typename charT, typename idT>
myregex::basic_table<charT, idT>::~basic_table() {}
//<--...