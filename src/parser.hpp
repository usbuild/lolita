#pragma once
#include <stdio.h>
#include "feeder.hpp"
#include "lex.hpp"
#include "noncopyable.hpp"
namespace lo {
class ParserError : public Error {
   public:
    DEFINE_EXCEPTION_CTOR(ParserError);
};

typedef enum BinOpr {
    OPR_ADD,
    OPR_SUB,
    OPR_MUL,
    OPR_DIV,
    OPR_MOD,
    OPR_POW,
    OPR_CONCAT,
    OPR_NE,
    OPR_EQ,
    OPR_LT,
    OPR_LE,
    OPR_GT,
    OPR_GE,
    OPR_AND,
    OPR_OR,
    OPR_NOBINOPR
} BinOpr;

typedef enum UnOpr { OPR_MINUS, OPR_NOT, OPR_LEN, OPR_NOUNOPR } UnOpr;

class Parser {
   public:
    Parser(Feeder& feeder);
    int parse();

   private:
    void chunk();
    void next();
    void statement();
    void condThen();
    void cond();

    void expr();
    BinOpr subexpr(int limit);
    void simpleexp();
    void primaryexp();
    void prefixexp();
    void funcargs();
    void body();
    void constructor();

   private:
    Lex lex_;
    Lex::token_t token_;
};
} /* lo */
