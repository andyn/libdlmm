#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <iostream>
#include <sstream>
#include <string>

#include "dlmm.hh"

int main(void) {
    Dl dl("./libuseless.so");

    // Reading variables from a dynamic library

    std::cout << "Reading and modifying a local copy of variable in library..."
              << std::endl;
    int my_answer =
        dl.symbol<int>("answer_to_life_the_universe_and_everything");
    my_answer = -1;
    (void)my_answer;
    if (dl.symbol<int>("answer_to_life_the_universe_and_everything") != 42) {
        std::cout << "TEST FAILED: variable was changed inside library."
                  << std::endl;
        return -1;
    }

    std::cout << "Reading and changing a referenced variable in library..."
              << std::endl;
    int &my_better_answer =
        dl.symbol<int>("answer_to_life_the_universe_and_everything");
    if (my_better_answer != 42) {
        std::cout << "TEST FAILED: Expected 42, got " << my_better_answer
                  << std::endl;
        return -1;
    }
    my_better_answer = 11; // This one goes to eleven!
    if (dl.symbol<int>("answer_to_life_the_universe_and_everything") != 11) {
        std::cout << "TEST FAILED: could not change variable inside library."
                  << std::endl;
        return -1;
    }

    // Calling functions from a dynamic library

    std::cout << "Calling function in library... " << std::endl;
    auto print_and_increment =
        dl.symbol<int(std::ostringstream &, std::string const &, int)>(
            "print_and_increment");

    std::string input_string = "Hello World!";
    std::ostringstream output_string;
    int incremented_via_reference =
        print_and_increment(output_string, input_string, 100);
    if (output_string.str() != input_string) {
        std::cout << "TEST FAILED: function did not stream correct value "
                  << "(expected " << input_string << ", got "
                  << output_string.str() << ")." << std::endl;
        return -1;
    }
    if (101 != incremented_via_reference) {
        std::cout << "TEST FAILED: function did not return correct value "
                  << "(expected " << 101 << ", got "
                  << incremented_via_reference << ")." << std::endl;
        return -1;
    }

    // Catching exceptions

    std::cout << "Loading nonexistent library..." << std::endl;
    try {
        Dl dl2("DOES_NOT_EXIST");
        std::cout << "TEST FAILED: nonexistent library did not throw."
                  << std::endl;
        return -1;
    } catch (std::runtime_error &) {
    }

    std::cout << "Loading nonexistent symbol..." << std::endl;
    try {
        std::cout << dl.symbol<int>("DOES_NOT_EXIST");
        std::cout << "TEST FAILED: nonexistent symbol did not throw."
                  << std::endl;
        return -1;
    } catch (std::runtime_error &) {
    }

    std::cout << "Loading nonexistent version of an existing symbol..."
              << std::endl;
    try {
        std::cout << dl.symbol<int>(
            "answer_to_life_the_universe_and_everything", "DOES_NOT_EXIST");
        std::cout << "TEST FAILED: nonexistent version of existing symbol did "
                     "not throw."
                  << std::endl;
        return -1;
    } catch (std::runtime_error &) {
    }

    std::cout << "Done testing." << std::endl;
}
