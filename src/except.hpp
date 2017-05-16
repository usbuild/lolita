#pragma once
#include <string>
#include <exception>
#include <stdarg.h>
namespace lo {
#define DEFINE_EXCEPTION_CTOR(CLASS)             \
    CLASS(const std::string &wh) { what_ = wh; } \
    CLASS(const char *format, ...) {             \
        char buf[128] = {0};                     \
        va_list argptr;                          \
        va_start(argptr, format);                \
        vsnprintf(buf, 127, format, argptr);     \
        va_end(argptr);                          \
        what_ = buf;                             \
    }

class Error : public std::exception {
   public:
    const char *what() const noexcept override { return what_.c_str(); }

   protected:
    std::string what_;
};

} /* lo */
