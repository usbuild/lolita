#pragma once
#include <vector>
#include <forward_list>
#include "opcodes.hpp"
#define NO_JUMP (-1)
namespace lo {
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
        };
        double nval;
    };
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
class Block;
struct FuncState {
    void enterBlock(Block &bl);
    void leaveBlock();
    Block *currentBl() { return bllist_.front(); }

    int codeABC(OpCode inst, int a, int b, int c);

    int codeABx(OpCode inst, int a, int b);

    void dischargeVars(ExpDesc &e);

    void freeVar(int reg);

    int reserveVar(int count);

   public:
    std::vector<Instruction> code_;
    std::forward_list<Block *> bllist_;
    int freevar_ = 0;
};
} /* lo */
