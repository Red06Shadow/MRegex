#include <myregex/myregex.hpp>
#include <io.h>
#include <fcntl.h>
#include <chrono>

int main(int argc, char const *argv[])
{
    std::string patron;
    typedef std::pair<std::basic_string<char>, size_t> &(*handle)(std::pair<std::basic_string<char>, size_t> &);
    // using idT = std::pair<size_t, handle>;
    using idT = size_t;
    // std::cout << "Inserte patron: ";
    // std::getline(std::cin, patron);
    // std::vector<std::pair<idT, std::string>> v = {{1, patron}};

    std::vector<std::pair<idT, std::string>> v = {
        {1, "fun|if|else|return|while|var"},
        {2, "int8|int16"},
        {3, "true|false"},
        {4, "[ \\t\\n\\0]+"},
        {5, "\\+\\+?|--?>?|>=?>?|<=?<?|==?|\\?|\\|\\|?|&&?|,|.|:|\\^|!=?|%|/|\\*"},
        {6, "\\{|\\}|\\[|\\]|\\(|\\)"},
        {7, "(0|[1-9][0-9]*)(.[0-9]+)?([eE][\\-+]?[0-9]+)?"},
        {8, "\"[^\\x00-\\x1F\"\\x7F]*\""},
        {9, "[a-zA-Z_][a-zA-Z0-9_]*"},
    };
    // std::vector<std::pair<idT, std::string>> v = {{0, "abc"}};
    std::chrono::high_resolution_clock::time_point __start = std::chrono::high_resolution_clock::now();
    myregex::regex<idT> _regex1 = myregex::builder<idT>(v).build();
    std::chrono::high_resolution_clock::time_point __end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(__end - __start).count() << " ns" << std::endl;
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(__end - __start).count() << " micros" << std::endl;
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(__end - __start).count() << " millis" << std::endl;

    __start = std::chrono::high_resolution_clock::now();
    myregex::regex<idT> _regex2 = myregex::builder<idT>(v).convert_to_dfa().build();
    __end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(__end - __start).count() << " ns" << std::endl;
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(__end - __start).count() << " micros" << std::endl;
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(__end - __start).count() << " millis" << std::endl;

    __start = std::chrono::high_resolution_clock::now();
    myregex::regex<idT> _regex3 = myregex::builder<idT>(v).convert_to_dfa().convert_to_table().build();
    __end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(__end - __start).count() << " ns" << std::endl;
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(__end - __start).count() << " micros" << std::endl;
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(__end - __start).count() << " millis" << std::endl;
    // _setmode(_fileno(stdout), _O_U16TEXT);
    // _setmode(_fileno(stdin), _O_U16TEXT);
    std::string str;
    std::cout << "Inserte cadena: ";
    std::getline(std::cin, str);

    // struct token
    // {
    //     idT id = 0;
    //     std::string str;
    // };

    // std::vector<token> tokens;
    stringrange range = str;
    __start = std::chrono::high_resolution_clock::now();
    myregex::caption<char, idT> cap = _regex3.match<myregex::constants::match_maximun>(range);
    // cap = _regex1.match<myregex::constants::match_maximun>(str);
    // std::cout << "{ " << cap.str() << " ; " << cap.id() << " }" << std::endl;
    // stringrange range2 = str;
    // bool cap2 = _regex1.verification(range2);
    // std::cout << std::boolalpha << cap2 << std::endl;
    // cap2 = _regex1.verification(str);
    // std::cout << std::boolalpha << cap2 << std::endl;
    __end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(__end - __start).count() << " ns" << std::endl;
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(__end - __start).count() << " micros" << std::endl;
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(__end - __start).count() << " millis" << std::endl;
    std::cout << "{ " << cap.str() << " ; " << cap.id() << " }" << std::endl;

    // std::chrono::high_resolution_clock::time_point ___time = std::chrono::high_resolution_clock::now();
    // stringrange range = str;
    // myregex::caption<char, idT> cap = _regex1.match<myregex::constants::match_maximun>(range);
    // std::cout << "{ " << cap.str() << " ; " << cap.id() << " }" << std::endl;
    // cap = _regex1.match<myregex::constants::match_maximun>(str);
    // std::cout << "{ " << cap.str() << " ; " << cap.id() << " }" << std::endl;
    // stringrange range2 = str;
    // bool cap2 = _regex1.verification(range2);
    // std::cout << std::boolalpha << cap2 << std::endl;
    // cap2 = _regex1.verification(str);
    // std::cout << std::boolalpha << cap2 << std::endl;
    // std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - ___time).count() << " ms" << std::endl;

    // ___time = std::chrono::high_resolution_clock::now();
    // stringrange range3 = str;
    // cap = _regex2.match<myregex::constants::match_maximun>(range3);
    // std::cout << "{ " << cap.str() << " ; " << cap.id() << " }" << std::endl;
    // cap = _regex2.match<myregex::constants::match_maximun>(str);
    // std::cout << "{ " << cap.str() << " ; " << cap.id() << " }" << std::endl;
    // stringrange range4 = str;
    // cap2 = _regex2.verification(range4);
    // std::cout << std::boolalpha << cap2 << std::endl;
    // cap2 = _regex2.verification(str);
    // std::cout << std::boolalpha << cap2 << std::endl;
    // std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - ___time).count() << " ms" << std::endl;

    // ___time = std::chrono::high_resolution_clock::now();
    // stringrange range5 = str;
    // cap = _regex3.match<myregex::constants::match_maximun>(range5);
    // std::cout << "{ " << cap.str() << " ; " << cap.id() << " }" << std::endl;
    // cap = _regex3.match<myregex::constants::match_maximun>(str);
    // std::cout << "{ " << cap.str() << " ; " << cap.id() << " }" << std::endl;
    // stringrange range6 = str;
    // cap2 = _regex3.verification(range6);
    // std::cout << std::boolalpha << cap2 << std::endl;
    // cap2 = _regex3.verification(str);
    // std::cout << std::boolalpha << cap2 << std::endl;
    // std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - ___time).count() << " ms" << std::endl;

    // while (range.peak() < range.end())
    // {
    //     myregex::caption<char, idT> cap = _regex.match<myregex::constants::match_maximun>(range);
    //     if (cap.id() != 4)
    //         tokens.push_back({cap.id(), cap.str()});
    // }
    // for (auto &&i : tokens)
    // std::cout << "{ " << i.str << " ; " << i.id << " }" << std::endl;
    return 0;
}