#pragma once
namespace lo {
class NonCopyable {
   protected:
    NonCopyable() = default;
    ~NonCopyable() = default;

   private:
    NonCopyable(const NonCopyable &);
    NonCopyable &operator=(const NonCopyable &);
};
}
