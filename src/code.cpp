#include "code.hpp"
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

void FuncState::dischargeVars(ExpDesc &e) {
    switch (e.k) {
        case ExpKind::VLOCAL: {
            e.k = ExpKind::VNONRELOC;
            break;
        }
        case ExpKind::VUPVAL: {
            e.info = codeABC(OP_GETUPVAL, 0, e.info, 0);
            break;
        }
        default:
            break;
    }
}
} /* lo */
