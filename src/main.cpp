#include <myregex/myregex.hpp>
#include <io.h>
#include <fcntl.h>

#define RANGE_STR false
#define VIEW_NFA true
#define VIEW_DFA true
#define VIEW_TABLE false

int main(int argc, char const *argv[])
{
    std::string patron;
    std::cout << "Inserte patron: ";
    std::getline(std::cin, patron);
    myregex::regex<myregex::nfa> _regex = myregex::regex<myregex::nfa>({{0, patron}});
#if VIEW_NFA
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);
    _regex.view();
    std::wcout << _regex.size() << L" bytes" << std::endl;
#else

    std::cout << _regex.size() << " bytes" << std::endl;

    std::string str;
    std::cout << "Inserte cadena: ";
    std::getline(std::cin, str);

    struct token
    {
        size_t id = 0;
        std::string str;
    };

    std::vector<token> tokens;
    stringrange range = str;
    while (range.peak() < range.end())
    {
        myregex::caption<char> cap = _regex.match<myregex::constants::match_maximun>(range);
        if (cap.id() != 4)
            tokens.push_back({cap.id(), cap.str()});
    }
    for (auto &&i : tokens)
        std::cout << "{ " << i.str << " ; " << i.id << " }" << std::endl;
#endif

    return 0;
}