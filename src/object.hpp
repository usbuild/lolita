#pragma once
#include <memory>
#include "feeder.hpp"
#include "noncopyable.hpp"

namespace lo {

class LuaState;

class GlobalState : NonCopyable {
   public:
    GlobalState(LuaState &main_thread);

    LuaState &mainThread();

   private:
    LuaState &main_thread_;
};

class LuaState : NonCopyable {
   public:
    LuaState();
    ~LuaState();

    int doFile(const std::string &filename);

    int loadFile(const std::string &filename);

    int doString(const std::string &name, const std::string &content);

    int loadString(const std::string &name, const std::string &content);

    int loadFeeder(Feeder &&feeder);

   private:
    GlobalState *g_ = nullptr;
};
} /* lo */
