#include <mregex.hpp>
#include <io.h>
#include <fcntl.h>

#define RANGE_STR false
#define VIEW_NFA false
#define VIEW_DFA false
#define VIEW_TABLE false

int main(int argc, char const *argv[])
{
    #if VIEW_NFA
    _setmode(_fileno(stdout), _O_U16TEXT);
    #endif
    // "[a-zA-Z_][a-zA-Z_0-9]*", "0|[1-9][0-9]*"
    // [a-zA-Z_0-9]+@gmail.com
    // "(0|[1-9][0-9]*)(.[0-9]+)?([eE][\\+\\-]?[0-9]+)?"

    NFA nfa;
    // if (argc < 2)
    //     nfa = MRegex::build_nfa({"a{1,4}"});
    // else
    //     nfa = MRegex::build_nfa(argv, argc);
    // nfa = MRegex::build_nfa("\\+\\+?|--?|&&?|\\|\\|?|\\*|/|\\{|}|\\?|::?|^|!|->|.");
    nfa = MRegex::build_nfa({"constexpr", "[a-zA-Z_][a-zA-Z_0-9]*", "(0|[1-9][0-9]*)(.[0-9]+)?([eE][\\+\\-]?[0-9]+)?", "\\+\\+?|--?|&&?|\\|\\|?|\\*|/|\\{|}|\\?|::?|^|!|->|."});
    #if VIEW_NFA
    nfa.view();
    std::wcout << std::endl;
    #endif
    DFA dfa = MRegex::convert_nfa_to_dfa(nfa);
    #if VIEW_DFA
    dfa.view();
    #endif
    TableDFA table = MRegex::convert_dfa_to_table(dfa);
    #if VIEW_TABLE and not VIEW_NFA
    table.view();
    #endif

    std::wcout << table.size() << L" bytes" << std::endl;
#if not VIEW_NFA
    // Caption
    bool condition = true;
    std::vector<std::string> strs;
    while (condition)
    {
        std::string str;
        std::getline(std::cin, str);
        if (str.empty())
            condition = false;
        else
        {
            strs.push_back(str);
            str.clear();
        }
    }
    for (auto &&str : strs)
    {
#if RANGE_STR
        basic_string_range range = str;
        auto pai = MRegex::caption<MRegex::Selector::_maximun_munch>(range, table);
        if (pai.first)
            std::cout << "dfa: " << std::boolalpha << true << ": " << std::string(range.begin().base(), pai.first) << std::endl;
#else
        auto pai = MRegex::caption<MRegex::Selector::_maximun_munch>(str, table);
        if (pai.first)
            std::cout << "dfa: " << std::boolalpha << true << ": " << std::string(const_cast<const char*>(str.begin().base()), pai.first) << std::endl;
#endif
        else
            std::wcout << "dfa: " << std::boolalpha << false << std::endl;
    }
#endif
    return 0;
}
// Crear dos algoritmos, DFA puro interpretado o Tabla compilada