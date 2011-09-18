#ifndef LIB_DLPP_H
#define LIB_DLPP_H

#include <cassert>
#include <dlfcn.h>
#include <stdexcept>

/** @short Wrapper for POSIX dynamic libraries.
 * Link with -ldl */
class Dl {
public:
    /** @short Open a dynamic library file.
     * @exception std::runtime_error If the library cannot be opened.
     * @param filename Library file name.
     * @param flags Flags to pass to dlopen(), RTLD_LAZY by default. */
    explicit Dl(std::string const &filename, int flags = RTLD_LAZY)
        : m_library(dlopen(filename.c_str(), flags)) {

        if (m_library == 0)
            throw std::runtime_error(std::string("Dl::Dl(\"" + filename + "\"): ") + dlerror());
    }

    /** @short Close a dynamic library file. */
    ~Dl() {
        dlclose(m_library);
    }

    /** @short Get a pointer to a symbol in a library.
     * @param Name of the symbol to search for.
     * @return A pointer to given symbol of type T.
     * @exception std::runtime_error If the given symbol cannot be found. */ 
    template <typename T>
    T* symbol(std::string const &symbol_name) const {
        // Convert a void pointer to a function pointer without warnings.
        // Since we're type punning, make sure that pointer sizes do not differ.
        assert(sizeof(void*) == sizeof(void(*)()));
        union {
            void *m_void;
            T *m_real;
        } ptr;

        ptr.m_void = dlsym(m_library, symbol_name.c_str());
        if (ptr.m_void == 0)
            throw std::runtime_error(std::string("Dl::symbol(\"" + symbol_name + "\"): ") + dlerror());

        return ptr.m_real;
    }

#ifdef _GNU_SOURCE // GNU extensions

    // Dlvsym() is supported through function overloading.
    /** @short Get a pointer to a symbol of certain version in a library.
     * @param symbol_name Name of the symbol to search for.
     * @param version Version of the symbol to search for.
     * @return A pointer to given symbol of type T.
     * @exception std::runtime_error If the given symbol cannot be found. */ 
    template <typename T>
    T* symbol(std::string const &symbol_name, std::string const &version) const {
        // Convert a void pointer to a function pointer without warnings.
        // Since we're type punning, make sure that pointer sizes do not differ.
        assert(sizeof(void*) == sizeof(void(*)()));
        union {
            void *m_void;
            T *m_real;
        } ptr;

        ptr.m_void = dlvsym(m_library, symbol_name.c_str(), version.c_str());
        if (ptr.m_void == 0)
            throw std::runtime_error(std::string("Dl::symbol(\"" + symbol_name + "\", \"" + version + "\"): ") + dlerror());

        return ptr.m_real;
    }

    // Dladdr() is not yet supported in a meaningful way.

#endif // _GNU_SOURCE
    

private:
    void *m_library; // Pointer to the opened library file
};

#endif // LIB_DLPP_H
