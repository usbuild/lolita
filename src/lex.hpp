#pragma once
#include "feeder.hpp"
#include "noncopyable.hpp"
namespace lo {

class Lex {
   public:
    enum token_type {
        AND = 256,
        BREAK,
        DO,
        ELSE,
        ELSEIF,
        END,
        FALSE,
        FOR,
        FUNCTION,
        IF,
        IN,
        LOCAL,
        NIL,
        NOT,
        OR,
        REPEAT,
        RETURN,
        THEN,
        TRUE,
        UNTIL,
        WHILE,
        CONCAT,
        DOTS,
        EQ,
        GE,
        LE,
        NE,
        NUMBER,
        NAME,
        STRING,
        EOS
    };

    typedef std::pair<int, std::string> token_t;

   public:
    Lex(Feeder &feeder) : feeder_(feeder) { feeder_.next(); }

    token_t nextToken();

   private:
    void newline() {}

   private:
    Feeder &feeder_;
};

} /* lo */
