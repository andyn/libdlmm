libdlmm-0.2
===========

About the library
-----------------

Libdlmm is a thin wrapper for POSIX libdl (dlopen, dlsym, dlclose, dlerror, 
linked with -ldl). GNU dlvsym is also supported.

The name libdlmm is derived from other projects (namely gtkmm) that provide
C++ wrappers for C libraries.

Usage
-----

Link with -ldl. A simple usage example is illustrated below. 


	#include <dlmm.hh>
	#include <iostream>
	
	int main(void) {
	    try {
	        Dl dl("libuseless.so");
	        std::cout << *dl.symbol<int>("variable") << std::endl;
	        std::cout << dl.symbol<int(std::string, int)>("function")("a", 1) << std::endl;
	    }
	    catch (std::runtime_error e) {
	        std::cerr << "Could not open library file" << std::endl;
	    }
	}


Author(s)
---------

Antti Nilakari <antti.nilakari@gmail.com>


License
-------

No warranty is given. You are at your own risk. You have been warned.

You are allowed to use and redistribute this library wrapper and all accompanying
material as you see fit. However, linking your program against libdl likely
requires you to adhere to its license.
