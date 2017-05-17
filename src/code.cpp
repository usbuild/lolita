#include "code.hpp"
namespace lo {
void FuncState::enterBlock(Block &bl) { bllist_.emplace_front(&bl); }

void FuncState::leaveBlock() { bllist_.pop_front(); }
} /* lo */
