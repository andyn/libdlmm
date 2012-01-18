#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <iostream>
#include <dlmm.hh>

int main(void) {
    Dl dl("libuseless.so");

    std::cout << "Testing valid library calls:\n" << std::endl;

    std::cout << "Variable from library: " << std::endl
              << *dl.symbol<int>("variable") << std::endl;
    std::cout << "Function call from library: " << std::endl;
    std::cout << dl.symbol<int(std::string, int)>("function")("Hello World!", 100) << std::endl;
    
    std::cout << "\nTesting invalid library calls, each should throw:\n***" << std::endl;
    
    try {
        Dl dl2("nonexistant_library");
        std::cout << *dl.symbol<int>("should_not_reach_me");
        std::cout << "*** TESTING FOR NONEXISTANT LIBRARY FAILED!" << std::endl;
    }
    catch (std::runtime_error e) {
        std::cout << e.what() << std::endl;
    }
    
    try {
        std::cout << *dl.symbol<int>("nonexistant_symbol");
        std::cout << "*** TESTING FOR NONEXISTANT SYMBOL FAILED!" << std::endl;
    }
    catch (std::runtime_error e) {
        std::cout << e.what() << std::endl;
    }
    
    try {
        std::cout << *dl.symbol<int>("variable", "nonexistant_version");
        std::cout << "*** TESTING FOR NONEXISTANT SYMBOL VERSION FAILED!" << std::endl;
    }
    catch (std::runtime_error e) {
        std::cout << e.what() << std::endl;
    }
    
    std::cout << "\nDone testing." << std::endl;    
}   
