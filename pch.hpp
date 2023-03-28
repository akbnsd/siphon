#include <iostream>
#include <string>


// build configuration specific code
#ifndef NDEBUG
    // logging enabled in debug mode onlu
    #define LG(...) std::cout << __VA_ARGS__


#else
    #define LG(...)
#endif


// platform specific code
#ifdef _WIN32

// add apple specific code here
#elif defined(_APPLE)

// add linux specific code here
#elif defined(_LINUX)

#endif
