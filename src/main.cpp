#include <myregex/myregex.hpp>
#include <io.h>
#include <fcntl.h>

int main(int argc, char const *argv[])
{
    std::string patron;
    std::cout << "Inserte patron: ";
    std::getline(std::cin, patron);
    std::vector<std::pair<size_t, std::string>> v = {{0, patron}};
    // std::vector<std::pair<size_t, std::string>> v = {{0, "abc"}};
    myregex::regex<size_t> _regex = myregex::builder<size_t>(v).convert_to_dfa().convert_to_table().build();
    // _setmode(_fileno(stdout), _O_U16TEXT);
    // _setmode(_fileno(stdin), _O_U16TEXT);
    std::string str;
    std::cout << "Inserte cadena: ";
    std::getline(std::cin, str);

    // struct token
    // {
    //     size_t id = 0;
    //     std::string str;
    // };

    // std::vector<token> tokens;
    stringrange range = str;
    myregex::caption<char, size_t> cap = _regex.match<myregex::constants::match_maximun>(range);
    std::cout << "{ " << cap.str() << " ; " << cap.id() << " }" << std::endl;
    cap = _regex.match<myregex::constants::match_maximun>(str);
    std::cout << "{ " << cap.str() << " ; " << cap.id() << " }" << std::endl;
    stringrange range2 = str;
    bool cap2 = _regex.verification(range2);
    std::cout << std::boolalpha << cap2 << std::endl;
    cap2 = _regex.verification(str);
    std::cout << std::boolalpha << cap2 << std::endl;


    // while (range.peak() < range.end())
    // {
    //     myregex::caption<char, size_t> cap = _regex.match<myregex::constants::match_maximun>(range);
    //     if (cap.id() != 4)
    //         tokens.push_back({cap.id(), cap.str()});
    // }
    // for (auto &&i : tokens)
    // std::cout << "{ " << i.str << " ; " << i.id << " }" << std::endl;
    return 0;
}