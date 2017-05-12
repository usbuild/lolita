#pragma once
#include <stdio.h>
#include "feeder.hpp"
#include "lex.hpp"
#include "noncopyable.hpp"
namespace lo {
class Parser {
   public:
    Parser(Feeder &feeder) : lex_(feeder) {}
    int parse() {
        std::string str;
        while (!(str = lex_.next()).empty()) {
            printf("%s", str.c_str());
        }
        return 0;
    }

   private:
    Lex lex_;
};
} /* lo */
