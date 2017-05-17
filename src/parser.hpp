#pragma once
#include <stdio.h>
#include <forward_list>
#include <memory>
#include "feeder.hpp"
#include "lex.hpp"
#include "noncopyable.hpp"
namespace lo {
class ParserError : public Error {
   public:
    DEFINE_EXCEPTION_CTOR(ParserError);
};

enum class ExpKind {
    VVOID, /* no value */
    VNIL,
    VTRUE,
    VFALSE,
    VK,         /* info = index of constant in `k' */
    VKNUM,      /* nval = numerical value */
    VLOCAL,     /* info = local register */
    VUPVAL,     /* info = index of upvalue in `upvalues' */
    VGLOBAL,    /* info = index of table; aux = index of global name in `k' */
    VINDEXED,   /* info = table register; aux = index register (or `k') */
    VJMP,       /* info = instruction pc */
    VRELOCABLE, /* info = instruction pc */
    VNONRELOC,  /* info = result register */
    VCALL,      /* info = instruction pc */
    VVARARG     /* info = instruction pc */
};

struct ExpDesc {
    ExpKind k;
    union {
        struct {
            int info, aux;
        } s;
        double nval;
    } u;
    int t; /* patch list of `exit when true' */
    int f; /* patch list of `exit when false' */
};

enum BinOpr {
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
};

enum UnOpr { OPR_MINUS, OPR_NOT, OPR_LEN, OPR_NOUNOPR };

struct Block {
    Block *parent = nullptr;
};

struct FuncState {
    void enterBlock(Block &bl);
    void leaveBlock();
    Block *currentBl() { return bllist_.front(); }
    std::forward_list<Block *> bllist_;
};

class Parser {
   public:
    Parser(Feeder &feeder);
    int parse();

   private:
    void block();
    void chunk();
    void next();
    bool statement();
    void condThen();
    void condDo();
    void cond();

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
