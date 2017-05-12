#pragma once
#include <stdio.h>
#include "feeder.hpp"
#include "noncopyable.hpp"
namespace lo {
class Parser {
   public:
    Parser(Feeder &feeder) : feeder_(feeder) {}
    int parse() {
        char c;
        while ((c = feeder_.next())) {
            putchar(c);
        }
        return 0;
    }

   private:
    Feeder &feeder_;
};
} /* lo */
