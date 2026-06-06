#include <mregex.hpp>
#include <io.h>
#include <fcntl.h>

int main(int argc, char const *argv[])
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    NFA nfa;
    nfa = MRegex::build_nfa(argv, argc);
    nfa.view();
    std::wcout << std::endl;
    MRegex::convert_nfa_to_dfa(nfa).view();

    

    
    // _regex reg = _regex();
    // NFA nfa = _regex::build_NFA({
    //     // {1, "a+b?"}
    //     // {0, "-?(0|[1-9][0-9]*)(.[0-9]+)?([eE][\\-+]?[0-9]+)?"},
    //     {0, "ab"},
    //     // {2, "if|else"},
    //     // {1, "[a-zA-Z0-9_]+"}
    // });
    // std::vector<std::string> strs = {"ab", "a", "aab", "b", "c", "aaaaaaaaaaaaaaaaaaab", "aab", "abb"};
    // std::vector<std::string> strs = {"if", "ifnull", "if ", "els9", "f", "else", "elfe", ""};
    // std::vector<std::string> strs = {"12", "-12", "0.9", "-0.9", "0.56", "-0.56", "4e5", "-4e5", "4e-5", "-4e-5", "2.0e5", "-2.0e5", "8.0e-5", "-8.0e+5"};
    // std::vector<std::string> strs = {"126", "-126", "0", "-0", "09"};
    // std::vector<std::string> strs = {"-.e4", "12 34", "123 ", "1234+", "12,345", "$123", "12.34.56", "infinity", "Nan", "1e", "e2", "1.2e", "1e2.5", "1e+", ".rt", "-.adbc", "123.", "", "  89 ", "1.2e 3", "1.2e++3", "1.2e3.4"};
    // nfa.view();
    // std::cout << std::endl;
    // DFA dfa = _regex::convert_NFA_to_DFA(nfa);
    // dfa.view();
    // for (auto &&str : strs)
    // {
    //     auto pai = reg.caption(str, nfa);
    //     if (pai.second)
    //         std::cout << "nfa: " << std::boolalpha << true << ": " << std::string(pai.first, pai.second) << std::endl;
    //     else
    //         std::cout << "nfa: " << std::boolalpha << false << std::endl;
    //     pai = reg.caption(str, dfa);
    //     if (pai.second)
    //         std::cout << "dfa: " << std::boolalpha << true << ": " << std::string(pai.first, pai.second) << std::endl;
    //     else
    //         std::cout << "dfa: " << std::boolalpha << false << std::endl;
    // }
    return 0;
}
// Crear dos algoritmos, DFA puro interpretado o Tabla compilada