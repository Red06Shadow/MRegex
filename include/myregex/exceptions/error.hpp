#ifndef MYREGEXERROR
#define MYREGEXERROR

#include <stdexcept>
#include <stdint.h>
#include <string>

namespace myregex
{
    enum class error_type : uint8_t
    {
        _S_syntax_error,
        _S_runtime_error,
        _S_input_error,
        _S_none
    };

    template <typename charT>
    class basic_lexicar_error : public std::exception
    {
    private:
        std::basic_string_view<char> _M_message;
        uint8_t _M_id;
        std::basic_string_view<charT> _M_ocurrence;
        typename std::basic_string_view<charT>::const_iterator _M_start;
        typename std::basic_string_view<charT>::const_iterator _M_end;

    
    public:
        explicit basic_lexicar_error(std::basic_string_view<char> _message, typename std::basic_string_view<charT>::const_iterator _index, const std::basic_string_view<charT>& _ocurrence, uint8_t _id)
        :
        _M_message(_message),
        _M_id(_id),
        _M_ocurrence(_ocurrence),
        _M_start(_index),
        _M_end(_index)
        {}
        explicit basic_lexicar_error(std::basic_string_view<char> _message, typename std::basic_string_view<charT>::const_iterator _start, typename std::basic_string_view<charT>::const_iterator _end, const std::basic_string_view<charT>& _ocurrence, uint8_t _id)
        :
        _M_message(_message),
        _M_id(_id),
        _M_ocurrence(_ocurrence),
        _M_start(_start),
        _M_end(_end)
        {}
        const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override { return _M_message.data(); }
        void view(std::basic_ostream<charT>& out) const;
        virtual ~basic_lexicar_error() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW {}
    };

    template <typename charT>
    class basic_analizer_error : public std::exception
    {
    private:
        std::basic_string_view<char> _M_message;
        uint8_t _M_id;
        std::basic_string_view<charT> _M_ocurrence;
        typename std::basic_string_view<charT>::const_iterator _M_index;
    
    public:
        explicit basic_analizer_error(std::basic_string_view<char> _message, std::basic_string_view<charT> _ocurrence, uint8_t _id, typename std::basic_string_view<charT>::const_iterator _index)
        :
        _M_message(_message),
        _M_ocurrence(_ocurrence),
        _M_id(_id),
        _M_index(_index)
        {}
        const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override { return _M_message.data(); }
        void view(std::basic_ostream<charT>& out) const;
        virtual ~basic_analizer_error() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW {}
    };
    
    
} // namespace myregex

template <typename charT>
void myregex::basic_lexicar_error<charT>::view(std::basic_ostream<charT>& out) const {
    std::basic_string<charT> line {};
    line.reserve(_M_ocurrence.size());
    if constexpr (std::is_same_v<charT, wchar_t>)
        out << L"error in generate regex : " << _M_message << std::endl
        << _M_ocurrence << std::endl;
    else
        out << "error in generate regex : " << _M_message << std::endl
        << _M_ocurrence << std::endl;
    
    for (typename std::basic_string_view<charT>::const_iterator i = _M_ocurrence.begin(); i != _M_ocurrence.end() + 1ULL; i++)
    {
        
        if (i == _M_start)
            line.push_back(charT('^'));
        else if (_M_start < i && i <= _M_end)
            line.push_back(charT('~'));
        else
            line.push_back(charT(' '));
        if (i != _M_ocurrence.end() && *i == charT('\n'))
            line.push_back(charT('\n'));
    }
    out << line << std::endl;
}


template <typename charT>
void myregex::basic_analizer_error<charT>::view(std::basic_ostream<charT>& out) const {
    std::basic_string<charT> line {};
    line.reserve(_M_ocurrence.size());
    if constexpr (std::is_same_v<charT, wchar_t>)
        out << L"error analizer : " << _M_message << std::endl
        << _M_ocurrence << std::endl;
    else
        out << "error analizer : " << _M_message << std::endl
        << _M_ocurrence << std::endl;
    
    for (typename std::basic_string_view<charT>::const_iterator i = _M_ocurrence.begin(); i != _M_ocurrence.end(); i++)
    {
        
        if (i == _M_index)
            line.push_back(charT('^'));
        else
            line.push_back(charT(' '));
        if (*i == charT('\n'))
            line.push_back(charT('\n'));
    }
    out << line << std::endl;
}

#endif