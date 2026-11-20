#ifndef MYREGEXUTILITIES
#define MYREGEXUTILITIES
#include <string>
#include <cstdlib>
#include <vector>
#include <stdint.h>

namespace myregex
{
    /// @brief Convierte una cadena de caracteres de tipo fromcharT a tocharT
    /// @tparam fromcharT Parametro de la cadena de entrada
    /// @param __str cadena de caracteres a convertir std::basic_string_view<fromcharT>
    /// @return Devuelve un vector con los ednpoints
    template <typename fromcharT>
    std::vector<uint32_t> _S_str_codepoints(std::basic_string_view<fromcharT> __str)
    {
        std::vector<uint32_t> __codepoints;
        __codepoints.reserve(__str.size());
        if constexpr (sizeof(fromcharT) == 1)
        {
            for (std::size_t i = 0; i < __str.size();)
            {
                uint32_t c = static_cast<unsigned char>(__str[i++]);
                uint32_t code;
                if (c < 0x80)
                {
                    code = c;
                }
                else if ((c & 0xe0) == 0xc0 && i < __str.size())
                {
                    code = ((c & 0x1f) << 6);
                    code |= (static_cast<unsigned char>(__str[i++]) & 0x3f);
                }
                else if ((c & 0xf0) == 0xe0 && i + 1 < __str.size())
                {
                    code = ((c & 0x0f) << 12);
                    code |= (static_cast<unsigned char>(__str[i++]) & 0x3f) << 6;
                    code |= (static_cast<unsigned char>(__str[i++]) & 0x3f);
                }
                else if ((c & 0xf8) == 0xf0 && i + 2 < __str.size())
                {
                    code = ((c & 0x07) << 18);
                    code |= (static_cast<unsigned char>(__str[i++]) & 0x3f) << 12;
                    code |= (static_cast<unsigned char>(__str[i++]) & 0x3f) << 6;
                    code |= (static_cast<unsigned char>(__str[i++]) & 0x3f);
                }
                else
                {
                    code = 0xfffd;
                }
                __codepoints.push_back(code);
            }
        }
        else
        {
            for (std::size_t i = 0; i < __str.size(); ++i)
            {
                uint32_t code = static_cast<uint32_t>(__str[i]);
                if constexpr (sizeof(fromcharT) == 2)
                {
                    if (code >= 0xd800 && code <= 0xdbff && i + 1 < __str.size())
                    {
                        uint32_t low = static_cast<uint32_t>(__str[++i]);
                        if (low >= 0xdc00 && low <= 0xdfff)
                            code = 0x10000 + ((code - 0xd800) << 10) + (low - 0xdc00);
                        else
                            code = 0xfffd;
                    }
                    else if (code >= 0xd800 && code <= 0xdfff)
                    {
                        code = 0xfffd;
                    }
                }
                __codepoints.push_back(code);
            }
        }
        return __codepoints;
    }

    /// @brief Convierte una cadena de caracteres de tipo fromcharT a tocharT
    /// @tparam tocharT Parametro de la cadena de salida
    /// @param __str cadena de caracteres a convertir std::basic_string_view<fromcharT>
    /// @param __codepoints los codepints de la cadena convertida
    template <typename tocharT>
    void _S_str_copy(std::basic_string<tocharT> &__str, const std::vector<uint32_t> &__codepoints)
    {
        for (uint32_t code : __codepoints)
        {
            if constexpr (sizeof(tocharT) == 1)
            {
                if (code <= 0x7f)
                {
                    __str.push_back(static_cast<tocharT>(code));
                }
                else if (code <= 0x7ff)
                {
                    __str.push_back(static_cast<tocharT>(0xc0 | (code >> 6)));
                    __str.push_back(static_cast<tocharT>(0x80 | (code & 0x3f)));
                }
                else if (code <= 0xffff)
                {
                    __str.push_back(static_cast<tocharT>(0xe0 | (code >> 12)));
                    __str.push_back(static_cast<tocharT>(0x80 | ((code >> 6) & 0x3f)));
                    __str.push_back(static_cast<tocharT>(0x80 | (code & 0x3f)));
                }
                else
                {
                    __str.push_back(static_cast<tocharT>(0xf0 | (code >> 18)));
                    __str.push_back(static_cast<tocharT>(0x80 | ((code >> 12) & 0x3f)));
                    __str.push_back(static_cast<tocharT>(0x80 | ((code >> 6) & 0x3f)));
                    __str.push_back(static_cast<tocharT>(0x80 | (code & 0x3f)));
                }
            }
            else if constexpr (sizeof(tocharT) == 2)
            {
                if (code <= 0xffff)
                    __str.push_back(static_cast<tocharT>(code));
                else
                {
                    code -= 0x10000;
                    __str.push_back(static_cast<tocharT>(0xd800 + (code >> 10)));
                    __str.push_back(static_cast<tocharT>(0xdc00 + (code & 0x3ff)));
                }
            }
            else
            {
                __str.push_back(static_cast<tocharT>(code));
            }
        }
    }

    /// @brief Convierte una cadena de caracteres de tipo fromcharT a tocharT
    /// @tparam fromcharT Parametro de la cadena de entrada
    /// @tparam tocharT Parametro de la cadena de salida
    /// @param __str cadena de caracteres a convertir std::basic_string_view<fromcharT>
    /// @return Devuelve una cadena de caractres del tipo std::basic_string<tocharT>
    template <typename fromcharT, typename tocharT>
    std::basic_string<tocharT> str_converter(std::basic_string_view<fromcharT> __str)
    {
        /////////////////////////////////////////////////////////////////////////////////////////////
        if constexpr (std::is_same_v<fromcharT, tocharT>)
            return std::basic_string<tocharT>(__str);
        /////////////////////////////////////////////////////////////////////////////////////////////
        std::vector<uint32_t> __codepoints = std::move(_S_str_codepoints<fromcharT>(__str));
        /////////////////////////////////////////////////////////////////////////////////////////////
        std::basic_string<tocharT> __rstr;
        __rstr.reserve(__codepoints.size());
        /////////////////////////////////////////////////////////////////////////////////////////////
        _S_str_copy<tocharT>(__rstr, __codepoints);
        /////////////////////////////////////////////////////////////////////////////////////////////
        return __rstr;
        /////////////////////////////////////////////////////////////////////////////////////////////
    }
}

#endif