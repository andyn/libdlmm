#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <iostream>
#include <dlmm.hh>

int main(void) {
    Dl dl("libuseless.so");

    std::cout << "Testing valid library calls..." << std::endl;

    std::cout << "Variable from library should be 42..." << std::endl;
    if (*dl.symbol<int>("variable") != 42) {
        std::cout << "FAILED: WAS " << *dl.symbol<int>("variable") << std::endl;
        return -1;
    }

    std::cout << "Function call from library should print \"Hello world!\" and increment 100 to 101..." << std::endl;
    int i = dl.symbol<int(std::string, int)>("function")("Hello World!", 100);
    if (i != 101) {
        std::cout << "FAILED: WAS " << i << std::endl;
        return -1;
    }

    std::cout << "Testing invalid library calls..." << std::endl;
    
    try {
        Dl dl2("nonexistent_library");
        std::cout << *dl.symbol<int>("should_not_reach_me");
        std::cout << "*** TEST FAILED: INVALID LIBRARY DID NOT THROW" << std::endl;
        return -1;
    }
    catch (std::runtime_error e) {
    }
    
    try {
        std::cout << *dl.symbol<int>("nonexistent_symbol");
        std::cout << "*** TEST FAILED: NONEXISTENT SYMBOL DID NOT THROW" << std::endl;
        return -1;
    }
    catch (std::runtime_error e) {
    }
    
    try {
        std::cout << *dl.symbol<int>("variable", "nonexistent_version");
        std::cout << "*** TEST FAILED: NONEXISTENT VERSION OF SYMBOL DID NOT THROW" << std::endl;
        return -1;
    }
    catch (std::runtime_error e) {
    }
    
    std::cout << "Done testing." << std::endl;    
}   
