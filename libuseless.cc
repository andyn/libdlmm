#include <iostream>
#include <string>

extern "C" {
int answer_to_life_the_universe_and_everything = 42;

int print_and_increment(std::ostream &out, std::string const &str,
                        int increment) {
    out << str;
    return increment + 1;
}
}
