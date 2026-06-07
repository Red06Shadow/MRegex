#include <mregex.hpp>
#include <io.h>
#include <fcntl.h>

int main(int argc, char const *argv[])
{
    // _setmode(_fileno(stdout), _O_U16TEXT); "[a-zA-Z_][a-zA-Z_0-9]*", "0|[1-9][0-9]*"
    // [a-zA-Z_0-9]+@gmail.com
    // "(0|[1-9][0-9]*)(.[0-9]+)?([eE][\\+\\-]?[0-9]+)?"
    NFA nfa;
    if (argc < 2)
        nfa = MRegex::build_nfa({"[a-zA-Z_0-9]+@gmail.com"});
    else
        nfa = MRegex::build_nfa(argv, argc);
    // nfa.view();
    std::wcout << std::endl;
    DFA dfa = MRegex::convert_nfa_to_dfa(nfa);
    dfa.view();
    
    TableDFA table = MRegex::convert_dfa_to_table(dfa);
    table.view();
    std::wcout << table.size() << L" bytes" << std::endl;
    
    bool condition = true;
    std::vector<std::string> strs;
    while (condition)
    {
        std::string str;
        std::getline(std::cin, str);
        if (str.empty())
            condition = false;
        else
            { strs.push_back(str); str.clear(); }
    }
    // _setmode(_fileno(stdout), _O_U8TEXT);
    for (auto &&str : strs)
    {
        auto pai = MRegex::caption<MRegex::Selector::_maximun_munch>(str, table);
        if (pai.second)
            std::cout << "dfa: " << std::boolalpha << true << ": " << std::string(pai.first, pai.second) << std::endl;
        else
            std::wcout << "dfa: " << std::boolalpha << false << std::endl;
    }

    return 0;
}
// Crear dos algoritmos, DFA puro interpretado o Tabla compilada