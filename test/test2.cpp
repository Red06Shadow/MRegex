#include <iostream>
#include <myregex/utilities/scoverter.hpp>

template <typename CharT>
bool fail(const char *name, const std::basic_string<CharT> &actual,
          const std::basic_string<CharT> &expected)
{
    if (actual != expected)
    {
        std::cerr << "Fallo: " << name << '\n';
        return true;
    }
    std::cout << "OK: " << name << '\n';
    return false;
}

int main()
{
    int failures = 0;

    // failures += fail("char vacio", myregex::str_converter<char, char>(""), std::string{});
    // failures += fail("char ASCII", myregex::str_converter<char, char>("Hola 123"), std::string("Hola 123"));

    // std::string utf8 = "ñáéíóú 世界 😀";
    // std::wstring wide = myregex::str_converter<char, wchar_t>(utf8);
    // failures += fail("UTF-8 -> wchar_t", myregex::str_converter<wchar_t, wchar_t>(wide), wide);
    // failures += fail("wchar_t -> UTF-8", myregex::str_converter<wchar_t, char>(wide), utf8);

    std::wstring utf16 = L"ñáéíóú 世界 😀";
    std::string u8str = myregex::str_converter<wchar_t, char>(utf16);
    failures += fail("wchar_t -> UTF-8", myregex::str_converter<char, char>(u8str), u8str);
    failures += fail("UTF-8 -> wchar_t", myregex::str_converter<char, wchar_t>(u8str), utf16);

    // std::u32string u32 = U"añ世界😀";
    // std::wstring u32_to_wide = myregex::str_converter<char32_t, wchar_t>(u32);
    // failures += fail("UTF-32 -> wchar_t", myregex::str_converter<wchar_t, char32_t>(u32_to_wide), u32);

    // failures += fail("UTF-32 vacio", myregex::str_converter<char32_t, char32_t>(U""), std::u32string{});

    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}