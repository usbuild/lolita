#pragma once
#include <stdio.h>
#include <forward_list>
#include <memory>
#include "code.hpp"
#include "opcodes.hpp"
#include "feeder.hpp"
#include "lex.hpp"
#include "noncopyable.hpp"
namespace lo {
class ParserError : public Error {
   public:
    DEFINE_EXCEPTION_CTOR(ParserError);
};

struct Block {
    Block *parent = nullptr;
};

class Parser {
   public:
    Parser(Feeder &feeder);
    int parse();

   private:
    void block();
    void chunk();
    void next();
    bool matchAndNext(int token);
    void assertToken(int token);
    bool statement();
    void condThen();
    void condDo();
    void cond();

    void newLocalVar();

    void expr(ExpDesc &v);
    BinOpr subexpr(ExpDesc &v, int limit);
    void simpleexp(ExpDesc &v);
    void primaryexp(ExpDesc &v);
    void prefixexp(ExpDesc &v);
    void funcargs();
    void body();
    void constructor();

    // stats
    void ifstat();
    void whilestat();
    void forstat();
    void repeatstat();
    void retstat();
    void breakstat();
    void funcstat();
    void exprstat();
    void localfunc();
    void localstat();

    void openFunc();
    void closeFunc();
    FuncState *currentFunc();

   private:
    Lex lex_;
    Lex::token_t token_;
    Block block_;
    Block *current_block_ = &block_;
    std::forward_list<std::unique_ptr<FuncState>> fslist_;
};
} /* lo */
