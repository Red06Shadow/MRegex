// #include <gtest/gtest.h>
#include <myregex/myregex.hpp>

int main(int argc, char const *argv[])
{
    try
    {
        if (argc > 1)
        {
            myregex::regex<size_t> r = myregex::builder<size_t>({
                {1, argv[1]}
            })
            .convert_to_dfa()
            .convert_to_table()
            .build();
            for (int i = 2; i < argc; i++)
            {
                auto v = r.match<myregex::constants::match_maximun>(argv[i]);
                std::cout << v.str() << ':' << v.id() << std::endl;
            }
        } 
    }
    catch (const myregex::basic_lexicar_error<char> &e)
    {
        e.view(std::cerr);
    }
    catch (const myregex::basic_analizer_error<char> &e)
    {
        e.view(std::cerr);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
