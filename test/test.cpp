#include <iostream>

namespace Mregex
{
    namespace
    {
        int funtion1(int number) { return number + 9;}
    } // namespace
    class Main
    {
    private:
        int n = 0;
    public:
        Main(int number) {
            n = funtion1(number);
        }
        ~Main() {}
    };
} // namespace Mregex


int main(int argc, char const *argv[])
{
    Mregex::Main m = Mregex::Main(9);
    Mregex::funtion1(9);
    return 0;
}
