
myregex::transitions::transitions(const std::initializer_list<size_t> &list)
    : _M_ptr(nullptr), _M_size(0)
{
    if (list.size())
    {
        _M_ptr = new size_t[list.size()];
        std::copy(list.begin(), list.end(), _M_ptr);
        _M_size = list.size();
    }
}
myregex::transitions::transitions(const std::vector<size_t> &list)
    : _M_ptr(nullptr), _M_size(0)
{
    if (!list.empty())
    {
        _M_ptr = new size_t[list.size()];
        std::copy(list.begin(), list.end(), _M_ptr);
        _M_size = list.size();
    }
}

myregex::transitions::transitions(const myregex::transitions &other) : _M_ptr(nullptr), _M_size(0)
{
    if (other._M_ptr && other._M_size > 0)
    {
        _M_ptr = new size_t[other._M_size];
        std::copy(other._M_ptr, other._M_ptr + other._M_size, _M_ptr);
        _M_size = other._M_size;
    }
}


myregex::transitions::transitions(myregex::transitions &&other) noexcept
    : _M_ptr(other._M_ptr), _M_size(other._M_size)
{
    other._M_ptr = nullptr;
    other._M_size = 0;
}

myregex::transitions &myregex::transitions::operator=(const myregex::transitions &other)
{
    if (this != &other)
    {
        if (_M_ptr)
        {
            delete[] _M_ptr;
            _M_ptr = nullptr;
            _M_size = 0;
        }

        if (other._M_ptr && other._M_size > 0)
        {
            _M_ptr = new size_t[other._M_size];
            std::copy(other._M_ptr, other._M_ptr + other._M_size, _M_ptr);
            _M_size = other._M_size;
        }
    }
    return *this;
}
myregex::transitions &myregex::transitions::operator=(myregex::transitions &&other) noexcept
{
    if (this != &other)
    {
        if (_M_ptr)
        {
            delete[] _M_ptr;
            _M_ptr = nullptr;
            _M_size = 0;
        }
        _M_ptr = other._M_ptr;
        _M_size = other._M_size;
        other._M_ptr = nullptr;
        other._M_size = 0;
    }
    return *this;
}

myregex::transitions::~transitions() {
    if (_M_ptr)
        delete[] _M_ptr;
    _M_ptr = nullptr;
    _M_size = 0;
}

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