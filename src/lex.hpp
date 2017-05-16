#pragma once
#include "except.hpp"
#include "feeder.hpp"
#include "noncopyable.hpp"
namespace lo {

class LexError : public Error {
   public:
    DEFINE_EXCEPTION_CTOR(LexError);
};

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

    void pushAndNext(std::string &value) {
        value.push_back(feeder_.current());
        feeder_.next();
    }

    std::string readString(char delim);
    std::string readLongString(const std::string &delim);

   private:
    Feeder &feeder_;
};

} /* lo */
