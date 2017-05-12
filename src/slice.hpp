#pragma once
#include <assert.h>
#include <string.h>
#include <string>
#include <vector>

namespace lo {

class Slice {
   public:
    Slice() : ptr_(""), size_(0) {}
    Slice(const char *data) : ptr_(data), size_(strlen(data)) {}
    Slice(const char *data, size_t size) : ptr_(data), size_(size) {}
    Slice(const void *data, size_t size)
        : ptr_(reinterpret_cast<const char *>(data)), size_(size) {}
    Slice(const unsigned char *data, size_t size)
        : ptr_(reinterpret_cast<const char *>(data)), size_(size) {}

    Slice(const std::string &str) : Slice(str.data(), str.size()) {}
    Slice(const std::vector<char> &v) : Slice(v.data(), v.size()) {}
    Slice(const std::vector<unsigned char> &v) : Slice(v.data(), v.size()) {}

    const char *data() const { return ptr_; }
    size_t size() const { return size_; }

    bool empty() const { return size_ == 0; }

    char operator[](size_t n) const {
        assert(n < size());
        return ptr_[n];
    }

    void clear() {
        ptr_ = "";
        size_ = 0;
    }

    void removePrefix(size_t n) {
        assert(n < size());
        ptr_ += n;
        size_ -= n;
    }

    std::string str() const { return std::string(ptr_, size_); }

    Slice sub(size_t start) const {
        assert(start < size());
        return Slice(ptr_ + start, size_ - start);
    }

    Slice sub(size_t start, size_t end) const {
        assert(start < size());
        assert(end < size());
        assert(start <= end);
        return Slice(ptr_ + start, end - start);
    }

   private:
    const char *ptr_;
    size_t size_ = 0;
};
inline bool operator==(const Slice &lhs, const Slice &rhs) {
    return (lhs.size() == rhs.size()) &&
           (memcmp(lhs.data(), rhs.data(), lhs.size()) == 0);
}

inline bool operator!=(const Slice &lhs, const Slice &rhs) {
    return !(lhs == rhs);
}

template <int T>
class SliceOf : public Slice {
   public:
    using Slice::Slice;
};
}
