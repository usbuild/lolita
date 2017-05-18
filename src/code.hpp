#pragma once
#include <vector>
#include <forward_list>
#include "opcodes.hpp"
#define NO_JUMP (-1)
namespace lo {
enum class ExpKind {
    VOID, /* no value */
    NIL,
    TRUE,
    FALSE,
    K,         /* info = index of constant in `k' */
    KNUM,      /* nval = numerical value */
    LOCAL,     /* info = local register */
    UPVAL,     /* info = index of upvalue in `upvalues' */
    GLOBAL,    /* info = index of table; aux = index of global name in `k' */
    INDEXED,   /* info = table register; aux = index register (or `k') */
    JMP,       /* info = instruction pc */
    RELOCABLE, /* info = instruction pc */
    NONRELOC,  /* info = result register */
    CALL,      /* info = instruction pc */
    VARARG     /* info = instruction pc */
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

enum class BinOpr {
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    POW,
    CONCAT,
    NE,
    EQ,
    LT,
    LE,
    GT,
    GE,
    AND,
    OR,
    NOBINOPR
};

enum UnOpr { MINUS, NOT, LEN, NOUNOPR };
class Block;
struct FuncState {
    void enterBlock(Block &bl);
    void leaveBlock();
    Block *currentBl() { return bllist_.front(); }

    int codeABC(OpCode inst, int a, int b, int c);

    int codeABx(OpCode inst, int a, int b);

    void dischargeVars(ExpDesc &e);

    void freeReg(int reg);

    int reserveVar(int count);

   public:
    std::vector<Instruction> code_;
    std::forward_list<Block *> bllist_;
    int freevar_ = 0;
};
} /* lo */
