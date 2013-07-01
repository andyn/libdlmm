#ifndef LIBDLPP_DL_HH
#define LIBDLPP_DL_HH

#include <cassert>
#include <stdexcept>

//source: https://github.com/anilakar/libdlmm

#ifndef _MSC_VER //linux and mac
#include <dlfcn.h>


/** @short Wrapper for POSIX dynamic libraries.
 * Link with -ldl */
class Dl {
public:
    /** @short Open a dynamic library file.
     * @exception std::runtime_error If the library cannot be opened.
     * @param filename Library file name.
     * @param flags Flags to pass to dlopen(), RTLD_LAZY by default. */
    explicit Dl(const std::string &filename, int flags = RTLD_LAZY) throw (std::runtime_error)
        : m_library(dlopen(filename.c_str(), flags)) {

        if (m_library == nullptr)
            throw std::runtime_error(std::string("Dl::Dl(\"" + filename + "\"): ") + dlerror());
    }

    /** @short Open a dynamic library file.
     * @exception std::runtime_error If the library cannot be opened.
     * @param filename Library file name.
     * @param flags Flags to pass to dlopen(), RTLD_LAZY by default. */
    explicit Dl(const char* filename, int flags = RTLD_LAZY) throw (std::runtime_error)
        : m_library(dlopen(filename, flags)) {

        if (m_library == nullptr)
            throw std::runtime_error(std::string("Dl::Dl(\"" + std::string(filename) + "\"): ") + dlerror());
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
    T* symbol(std::string const &symbol_name) const throw (std::runtime_error) {
        // Convert a void pointer to a function pointer without warnings.
        // Since we're type punning, make sure that pointer sizes do not differ.
        static_assert(sizeof(void*) == sizeof(T*), "pointers aren't the same size!");
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
    T* symbol(std::string const &symbol_name, std::string const &version) const throw (std::runtime_error) {
        // Convert a void pointer to a function pointer without warnings.
        // Since we're type punning, make sure that pointer sizes do not differ.
        static_assert(sizeof(void*) == sizeof(T*), "pointers aren't the same size!");
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


#else //windows
#include <windows.h>

#warning "untested code ahead!"

/** @short Wrapper for windows dynamic libraries. */
class Dl {
public:
    /** @short Open a dynamic library file.
     * @exception std::runtime_error If the library cannot be opened.
     * @param filename Library file name. */
    explicit Dl(const std::string &filename, DWORD flags = 0) throw (std::runtime_error)
        : m_library(LoadLibraryEx(TEXT( filename.c_str() ), nullptr, flags) ) {
        //see http://msdn.microsoft.com/en-us/library/windows/desktop/ms684179(v=vs.85).aspx
        //for flags
        if (m_library == nullptr) {
            throw std::runtime_error(std::string("Dl::Dl(\"" + filename + "\"): ") + "library not found! " + filename);
        }
    }

    /** @short Open a dynamic library file.
     * @exception std::runtime_error If the library cannot be opened.
     * @param filename Library file name. */
    explicit Dl(const LPCSTR &filename, DWORD flags = 0) throw (std::runtime_error)
        : m_library(LoadLibraryEx(TEXT(filename), nullptr, flags) ) {
        //see http://msdn.microsoft.com/en-us/library/windows/desktop/ms684179(v=vs.85).aspx
        //for flags
        if (m_library == nullptr) {
            throw std::runtime_error( std::string("Dl::Dl(\"" + std::string(filename) + "\"): ") + "library not found! " + std::string(filename) );
        }
    }

    /** @short Close a dynamic library file. */
    ~Dl() {
        if ( m_library != nullptr ) FreeLibrary(m_library);
    }

    /** @short Get a pointer to a symbol in a library.
     * @param Name of the symbol to search for.
     * @return A pointer to given symbol of type T.
     * @exception std::runtime_error If the given symbol cannot be found. */ 
    template <typename T>
    T* symbol(std::string const &symbol_name) const throw (std::runtime_error) {
        // Convert a void pointer to a function pointer without warnings.
        // Since we're type punning, make sure that pointer sizes do not differ.
        static_assert( sizeof(HINSTANCE) == sizeof(T*), "pointers aren't the same size!");
        union {
            HINSTANCE m_void;
            T *m_real;
        } ptr;

        ptr.m_void = GetProcAddress(m_library, symbol_name.c_str());
        if (ptr.m_void == nullptr)
            throw std::runtime_error(std::string("Dl::symbol(\"" + symbol_name + "\"): ") + "symbol not found! ");

        return ptr.m_real;
    }    

private:
    HINSTANCE m_library; // Pointer to the opened library file
};

#endif
#endif // LIBDLPP_DL_HH
