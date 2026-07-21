#include <myregex/myregex.hpp>
#include <io.h>
#include <fcntl.h>
#include <chrono>
#include <fstream>

int main(int argc, char const *argv[])
{
    try
    {
        myregex::wregex<uint8_t> mregx = myregex::wbuilder<uint8_t>({{1, L"hola mundo"}}).build();
        mregx.export_automaton(std::wcout);
        myregex::basic_nfa<wchar_t, uint8_t> df{
            {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, 1},
            {0},
            {{{0, -1ULL}, {1}},
             {{1, -1ULL}, {2}},
             {{2, 104ULL}, {3}},
             {{3, 111ULL}, {4}},
             {{4, 108ULL}, {5}},
             {{5, 97ULL}, {6}},
             {{6, 32ULL}, {7}},
             {{7, 109ULL}, {8}},
             {{8, 117ULL}, {9}},
             {{9, 110ULL}, {10}},
             {{10, 100ULL}, {11}},
             {{11, 111ULL}, {12}},
             {{12, -1ULL}, {13}}},
            {L' ', L'a', L'd', L'h', L'l', L'm', L'n', L'o', L'u'}};
    }
    catch (const myregex::basic_regex_error<wchar_t> &e)
    {
        std::wcerr << e.what() << e.especification();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}