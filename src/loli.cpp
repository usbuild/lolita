#include <stdio.h>
#include "object.hpp"

void interact() {}

int main(int argc, const char *argv[]) {
    if (argc <= 1) {
        interact();
    } else {
        lo::LuaState state;
        state.doFile(argv[1]);
    }
    return 0;
}
