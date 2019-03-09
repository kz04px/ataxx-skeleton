#include <iostream>
#include <string>
#include "protocol.hpp"
#include "test.hpp"

int main()
{
    std::string input;
    std::cin >> input;

    if(input == "uai")
    {
        UAI::listen();
    }
    else if(input == "test")
    {
        test();
    }
    else if(input == "about")
    {
        std::cout << "AtaxxEngine" << std::endl;
        std::cout << "Written in C++" << std::endl;
    }
    else
    {
        std::cout << "Unknown protocol" << std::endl;
    }

    return 0;
}
