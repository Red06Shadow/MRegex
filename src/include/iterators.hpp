#ifndef ITERATORS
#define ITERATORS

#include <string>

template <typename T>
class basic_string_range
{
public:// struts
    using iterator = typename std::__cxx11::basic_string<T>::const_iterator;
private: // Data
    const typename std::__cxx11::basic_string<T>::const_iterator __begin;
    typename std::__cxx11::basic_string<T>::const_iterator __iterator;
    const typename std::__cxx11::basic_string<T>::const_iterator __end;

public://Funtions
    basic_string_range(const std::__cxx11::basic_string<T> &str) : __begin(str.begin()), __iterator(__begin), __end(str.end()) {}
    inline const typename std::__cxx11::basic_string<T>::const_iterator &begin() const { return __begin; }
    inline const typename std::__cxx11::basic_string<T>::const_iterator next() { return __iterator++; }
    inline const typename std::__cxx11::basic_string<T>::const_iterator &peak() const { return __iterator; }
    inline const typename std::__cxx11::basic_string<T>::const_iterator offset(size_t position) const { return __iterator + position; }
    inline const typename std::__cxx11::basic_string<T>::const_iterator &end() const { return __end; }
    ~basic_string_range() {}
};

#endif