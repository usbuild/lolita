#include "code.hpp"
#include "parser.hpp"
namespace lo {
void FuncState::enterBlock(Block &bl) { bllist_.emplace_front(&bl); }

void FuncState::leaveBlock() { bllist_.pop_front(); }

int FuncState::codeABC(OpCode inst, int a, int b, int c) {
    code_.push_back(inst);
    return code_.size() - 1;
}

int FuncState::codeABx(OpCode inst, int a, int b) {
    code_.push_back(inst);
    return code_.size() - 1;
}

void FuncState::freeReg(int reg) {
    assert(reg == freevar_);
    freevar_--;
}

int FuncState::reserveVar(int count) {
    freevar_ += count;
    return freevar_ - count;
}

void FuncState::dischargeVars(ExpDesc &e) {
    switch (e.k) {
        case ExpKind::LOCAL: {
            e.k = ExpKind::NONRELOC;
            break;
        }
        case ExpKind::UPVAL: {
            e.info = codeABC(OP_GETUPVAL, 0, e.info, 0);
            e.k = ExpKind::RELOCABLE;
            break;
        }
        case ExpKind::GLOBAL: {
            e.info = codeABx(OP_GETGLOBAL, 0, e.info);
            e.k = ExpKind::RELOCABLE;
        }
        case ExpKind::INDEXED: {
            freeReg(e.aux);
            freeReg(e.info);
            e.info = codeABC(OP_GETTABLE, 0, e.info, e.aux);
            e.k = ExpKind::RELOCABLE;
        }
        default:
            break;
    }
}
} /* lo */
