#include <iostream>
#include <string>

extern "C" {
    int meaning_of_life = 42;

    int print_and_increment(std::ostream &out, std::string const &str, int increment) {
        out << str;
        return increment + 1;
    }
}
