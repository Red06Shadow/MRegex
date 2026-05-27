#ifndef ITERATORS
#define ITERATORS

#include <string>

template <typename T>
class _C_String_Iterators
{
private:
    const typename std::__cxx11::basic_string<T>::const_iterator __begin;
    typename std::__cxx11::basic_string<T>::const_iterator __iterator;
    const typename std::__cxx11::basic_string<T>::const_iterator __end;

public:
    _C_String_Iterators(const std::__cxx11::basic_string<T> &str) : __begin(str.begin()), __iterator(__begin), __end(str.end()) {}
    inline const typename std::__cxx11::basic_string<T>::const_iterator &begin() { return __begin; }
    inline const typename std::__cxx11::basic_string<T>::const_iterator next() { return __iterator++; }
    inline const typename std::__cxx11::basic_string<T>::const_iterator &peak() const { return __iterator; }
    inline const typename std::__cxx11::basic_string<T>::const_iterator offset(size_t position) const { return __iterator + position; }
    inline const typename std::__cxx11::basic_string<T>::const_iterator &end() const { return __end; }
    ~_C_String_Iterators() {}
};

#endif