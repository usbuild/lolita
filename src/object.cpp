#include "object.hpp"
#include "parser.hpp"

namespace lo {

GlobalState::GlobalState(LuaState &main_thread) : main_thread_(main_thread) {}

LuaState &GlobalState::mainThread() { return main_thread_; }

LuaState::LuaState() : g_(new GlobalState(*this)) {}
LuaState::~LuaState() {
    if (&g_->mainThread() == this) {
        delete g_;
    }
}

int LuaState::doFile(const std::string &filename) { return loadFile(filename); }

int LuaState::loadFile(const std::string &filename) {
    return loadFeeder(FileFeeder(filename));
}

int LuaState::loadFeeder(Feeder &&feeder) {
    try {
        Parser parser(feeder);
        return parser.parse();
    } catch (const Error &err) {
        fprintf(stderr, "%s\n", err.what());
        return 1;
    }
}

} /* lo */
