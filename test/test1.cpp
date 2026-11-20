// #include <gtest/gtest.h>
#include <myregex/myregex.hpp>
#include <iostream>

int main()
{
    try
    {
        myregex::regex<size_t> nfa_regex = myregex::builder<size_t>({
            {1, "a+"},
            {2, "b(c|d)"}
        }).build();

        if (!nfa_regex.verification("aaaa"))
        {
            std::cerr << "Fallo: debería aceptarse 'aaaa' mediante NFA\n";
            return EXIT_FAILURE;
        }
        if (nfa_regex.verification("bax"))
        {
            std::cerr << "Fallo: no debería aceptarse 'bax' mediante NFA\n";
            return EXIT_FAILURE;
        }

        myregex::regex<size_t> dfa_regex = myregex::builder<size_t>({
            {1, "a+"},
            {2, "b(c|d)"}
        }).convert_to_dfa().build();

        if (!dfa_regex.verification("bc") || !dfa_regex.verification("bd"))
        {
            std::cerr << "Fallo: deberían aceptarse 'bc' y 'bd' mediante DFA\n";
            return EXIT_FAILURE;
        }

        myregex::regex<size_t> table_regex = myregex::builder<size_t>({
            {1, "a+"},
            {2, "b(c|d)"}
        }).convert_to_dfa().convert_to_table().build();

        if (!table_regex.verification("bc") || table_regex.verification("bx"))
        {
            std::cerr << "Fallo inesperado en la prueba mediante tabla\n";
            return EXIT_FAILURE;
        }

        myregex::wregex<size_t> table_wregex = myregex::wbuilder<size_t>({
            {1, L"a+"},
            {2, L"b(c|d)"}
        }).convert_to_dfa().convert_to_table().build();

        if (!table_wregex.verification(L"bc") || table_wregex.verification(L"bx"))
        {
            std::cerr << "Fallo inesperado en la prueba mediante tabla\n";
            return EXIT_FAILURE;
        }

        std::cout << "OK: pruebas básicas de NFA, DFA y tabla\n";
        return EXIT_SUCCESS;
    }
    catch (const myregex::basic_lexicar_error<char>& error)
    {
        error.view(std::cerr);
    }
    catch (const myregex::basic_lexicar_error<wchar_t>& error)
    {
        error.view(std::wcerr);
    }
    catch (const std::exception& error)
    {
        std::cerr << error.what() << '\n';
    }
    return EXIT_FAILURE;
}
