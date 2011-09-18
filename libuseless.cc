#include <iostream>

extern "C" {
    int variable = 42;

    int function(std::string const &str, int increment) {
        std::cout << str << std::endl;
        return increment + 1;
    }
}
