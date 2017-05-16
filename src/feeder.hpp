#pragma once
#include <fstream>
#include "slice.hpp"
namespace lo {
class Feeder {
   public:
    virtual ~Feeder() {}
    char next() {
        current_ = nextImpl();
        return current_;
    }
    virtual char nextImpl() = 0;
    virtual const std::string &name() const = 0;
    char current() const { return current_; }

    std::pair<bool, std::string> readString(const std::string &delim);

   protected:
    char current_ = 0;
};

class FileFeeder : public Feeder {
   public:
    FileFeeder(const lo::Slice &filename)
        : filename_(filename.str()), file_(filename_) {}

    const std::string &name() const { return filename_; }

    char nextImpl() {
        char c = file_.get();
        if (file_.eof()) {
            return 0;
        } else {
            return c;
        }
    }

   private:
    const std::string filename_;
    std::ifstream file_;
};

class MemoryFeeder : public Feeder {
   public:
    MemoryFeeder(const lo::Slice &input, const lo::Slice &source)
        : data_(input.str()), source_(source.str()) {}

    char nextImpl() {
        if (pos_ == data_.size()) {
            return 0;
        } else {
            return data_[pos_++];
        }
    }

    const std::string &name() const { return source_; }

   private:
    size_t pos_ = 0;
    std::string data_;
    std::string source_;
};

} /* lo */
