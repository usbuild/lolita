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
        Lex::token_t token;
        while (true) {
            token = lex_.nextToken();
            if (token.first == Lex::EOS) {
                return 0;
            } else {
                printf("%d %s \n", token.first, token.second.c_str());
            }
        }
        return 0;
    }

   private:
    Lex lex_;
};
} /* lo */
