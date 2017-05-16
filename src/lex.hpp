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
    constexpr static int FIRST_RESERVED = 257;
    enum token_type {
        AND = FIRST_RESERVED,
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
    Lex(Feeder &feeder);

    token_t nextToken();

    static std::string tokenToString(int token);

   private:
    void newline() {}

    std::string readString(char delim);
    std::string readLongString(const std::string &delim);

    std::string readNumber(const std::string &prefix);

   private:
    Feeder &feeder_;
};

} /* lo */
