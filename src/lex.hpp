#pragma once
#include "feeder.hpp"
#include "noncopyable.hpp"
namespace lo {
class Lex {
   public:
    Lex(Feeder &feeder) : feeder_(feeder) {}

    std::string next() {
        char c = feeder_.next();
        if (c) {
            return std::string(&c, 1);
        } else {
            return "";
        }
    }

   private:
    Feeder &feeder_;
};

} /* lo */
