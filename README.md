libdlmm-0.3
===========

About the library
-----------------

Libdlmm is a thin wrapper for POSIX libdl (dlopen, dlsym, dlclose, dlerror, 
linked with -ldl) and MS Windows (LoadLibraryEx, FreeLibrary, GetProcAddress). GNU dlvsym is also supported.

The name libdlmm is derived from other projects (namely gtkmm) that provide
C++ wrappers for C libraries.

Usage
-----

Link with -ldl. A simple usage example is illustrated below.
on MS Windows a linking flag isn't necissary

```cpp
#include <dlmm.hh>
#include <iostream>

int main(void) {
    try {
        Dl dl("some_library.so");
        std::cout << dl.symbol<int>("variable_name") << std::endl;
        std::cout << dl.symbol<int(std::string const &, int)>("function_name")("a", 1) << std::endl;
    }
    catch (std::runtime_error e) {
        std::cerr << "Library error" << std::endl;
        return 1;
    }
}
```

Author(s)
---------

Antti Nilakari <antti.nilakari@gmail.com>


License
-------

No warranty is given. You are at your own risk. You have been warned.

You are allowed to use and redistribute this library wrapper and all accompanying
material as you see fit. However, linking your program against libdl likely
requires you to adhere to its license.
