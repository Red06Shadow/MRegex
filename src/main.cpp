#include <mregex.hpp>
#include <io.h>
#include <fcntl.h>

#define RANGE_STR false
#define VIEW_NFA false
#define VIEW_DFA true
#define VIEW_TABLE false

int main(int argc, char const *argv[])
{
    #if VIEW_NFA
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);
    #endif
    // "[a-zA-Z_][a-zA-Z_0-9]*", "0|[1-9][0-9]*"
    // [a-zA-Z_0-9]+@gmail.com
    // "(0|[1-9][0-9]*)(.[0-9]+)?([eE][\\+\\-]?[0-9]+)?"

    // if (argc < 2)
    //     nfa = MRegex::build_nfa({"a{1,4}"});
    // else
    //     nfa = MRegex::build_nfa(argv, argc);
    // nfa = MRegex::build_nfa("\\+\\+?|--?|&&?|\\|\\|?|\\*|/|\\{|}|\\?|::?|^|!|->|.");
    // NFA nfa = MRegex<char>::build_nfa("\\n\\t\\v\\b\\r\\f\\0\\001\\123\\u1fa5\\U1fe345a4\\x2f");
    // nfa = MRegex::build_nfa("\\n\\t\\v\\b\\r\\f\\0\\001\\123\\u1fa4\\U1fe345a4\\x1f");
    // NFA nfa = MRegex<char>::build_nfa("\\+\\+?|--?|&&?|\\|\\|?|\\*|/|\\{|}|\\?|::?|^|!|->|.");
    NFA nfa = MRegex<char>::build_nfa({{0, "constexpr"}, {1, "[a-zA-Z_][a-zA-Z_0-9]*"}, {2, "(0|[1-9][0-9]*)(.[0-9]+)?([eE][\\+\\-]?[0-9]+)?"}, {3, "\\+\\+?|--?|&&?|\\|\\|?|\\*|/|\\{|}|\\?|::?|^|!|->|."}});
    #if VIEW_NFA
    nfa.view();
    std::wcout << std::endl;
    #endif
    DFA<char> dfa = MRegex<char>::convert_nfa_to_dfa(nfa);
    #if VIEW_DFA
    dfa.view();
    #endif
    TableDFA table = MRegex<char>::convert_dfa_to_table(dfa);
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
            std::cout << "dfa: " << std::boolalpha << true << ": " << std::string(range.begin().base(), pai.first) << " : " << pai.second << std::endl;
#else
        auto pai = MRegex<char>::caption<Selector::maximun_munch>(str, table);
        if (pai.first)
            std::cout << "dfa: " << std::boolalpha << true << ": " << std::string(const_cast<const char*>(str.begin().base()), pai.first) << " : " << pai.second << std::endl;
#endif
        else
            std::wcout << "dfa: " << std::boolalpha << false << std::endl;
    }
#endif
    return 0;
}