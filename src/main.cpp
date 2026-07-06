#include <myregex/myregex.hpp>
#include <io.h>
#include <fcntl.h>
#include <chrono>
#include <fstream>

int main(int argc, char const *argv[])
{
    size_t *_trransition = new size_t[512]{-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1ULL,-1ULL,-1ULL,-1ULL,1,-1ULL,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,1,1,1,1,1,1,1,1,1,1,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1ULL,-1ULL,-1ULL,-1ULL,1,-1ULL,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL,-1ULL};
    myregex::table<size_t> _table = myregex::table<size_t>(
        {{}, 0}, _trransition
        );
    // std::string patron;
    // typedef std::pair<std::basic_string<char>, size_t> &(*handle)(std::pair<std::basic_string<char>, size_t> &);
    // // using idT = std::pair<size_t, handle>;
    using idT = size_t;

    std::vector<std::pair<idT, std::string>> v = {
        {1, "fun|if|else|return|while|var"},
        {2, "int8|int16"},
        {3, "true|false"},
        {4, "[ \\t\\n\\0]+"},
        {5, "\\+\\+?|--?>?|>=?>?|<=?<?|==?|\\?|\\|\\|?|&&?|,|.|:|\\^|!=?|%|/|\\*"},
        {6, "\\{|\\}|\\[|\\]|\\(|\\)"},
        {7, "(0|[1-9][0-9]*)(.[0-9]+)?([eE][\\-+]?[0-9]+)?"},
        {8, "\"[^\\x00-\\x1F\"\\x7F]*\""},
        {9, "[a-zA-Z_][a-zA-Z0-9_]*"}
    };
    // std::chrono::high_resolution_clock::time_point __start = std::chrono::high_resolution_clock::now();
    // myregex::regex<idT> _regex1 = myregex::builder<idT>(v).build();
    // std::chrono::high_resolution_clock::time_point __end = std::chrono::high_resolution_clock::now();
    // std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(__end - __start).count() << " ns" << std::endl;
    // std::cout << std::chrono::duration_cast<std::chrono::microseconds>(__end - __start).count() << " micros" << std::endl;
    // std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(__end - __start).count() << " millis" << std::endl;

    // __start = std::chrono::high_resolution_clock::now();
    // myregex::regex<idT> _regex2 = myregex::builder<idT>(v).convert_to_dfa().build();
    // __end = std::chrono::high_resolution_clock::now();
    // std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(__end - __start).count() << " ns" << std::endl;
    // std::cout << std::chrono::duration_cast<std::chrono::microseconds>(__end - __start).count() << " micros" << std::endl;
    // std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(__end - __start).count() << " millis" << std::endl;

    // __start = std::chrono::high_resolution_clock::now();
    myregex::regex<idT> _regex3 = myregex::builder<idT>(v).convert_to_dfa().convert_to_table().build();
    std::ofstream out {"C:\\Proyectos(Red06Shadow)\\c++\\regex\\test\\test0001.txt"};
    std::cout << _regex3.size();
    _regex3.export_automaton(out);
    // __end = std::chrono::high_resolution_clock::now();
    // std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(__end - __start).count() << " ns" << std::endl;
    // std::cout << std::chrono::duration_cast<std::chrono::microseconds>(__end - __start).count() << " micros" << std::endl;
    // std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(__end - __start).count() << " millis" << std::endl;

    // std::string str;
    // std::cout << "Inserte cadena: ";
    // std::getline(std::cin, str);

    // stringrange range = str;
    // __start = std::chrono::high_resolution_clock::now();
    // myregex::caption<char, idT> cap = _regex3.match<myregex::constants::match_maximun>(range);
    // __end = std::chrono::high_resolution_clock::now();
    // std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(__end - __start).count() << " ns" << std::endl;
    // std::cout << std::chrono::duration_cast<std::chrono::microseconds>(__end - __start).count() << " micros" << std::endl;
    // std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(__end - __start).count() << " millis" << std::endl;
    // std::cout << "{ " << cap.str() << " ; " << cap.id() << " }" << std::endl;

    // _regex3.view();

    return 0;
}