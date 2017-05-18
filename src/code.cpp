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

void FuncState::freeVar(int reg) {
    assert(reg == freevar_);
    freevar_--;
}

int FuncState::reserveVar(int count) {
    freevar_ += count;
    return freevar_ - count;
}

void FuncState::dischargeVars(ExpDesc &e) {
    switch (e.k) {
        case ExpKind::VLOCAL: {
            e.k = ExpKind::VNONRELOC;
            break;
        }
        case ExpKind::VUPVAL: {
            e.info = codeABC(OP_GETUPVAL, 0, e.info, 0);
            e.k = ExpKind::VRELOCABLE;
            break;
        }
        case ExpKind::VGLOBAL: {
            e.info = codeABx(OP_GETGLOBAL, 0, e.info);
            e.k = ExpKind::VRELOCABLE;
        }
        case ExpKind::VINDEXED: {
        }
        default:
            break;
    }
}
} /* lo */
