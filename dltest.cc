#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <iostream>
#include "Dl.hh"

int main(void) {
    Dl dl("libuseless.so");

    std::cout << "Variable from library: " << std::endl
              << *dl.symbol<int>("variable") << std::endl;

    std::cout << "Function call from library: " << std::endl;
    std::cout << dl.symbol<int(std::string, int)>("function")("Hello World!", 100) << std::endl;


    std::cout << "Invalid system calls, each should throw:" << std::endl;
    
    try {
        Dl dl2("nonexistant_library");
        std::cout << *dl.symbol<int>("should_not_reach_me");
    }
    catch (std::runtime_error e) {
        std::cout << e.what() << std::endl;
    }
    
    try {
        std::cout << *dl.symbol<int>("nonexistant_symbol");
    }
    catch (std::runtime_error e) {
        std::cout << e.what() << std::endl;
    }
    
    try {
        std::cout << *dl.symbol<int>("variable", "nonexistant_version");
    }
    catch (std::runtime_error e) {
        std::cout << e.what() << std::endl;
    }
        
}   
