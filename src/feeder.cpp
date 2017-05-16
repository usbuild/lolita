#include <sstream>
#include "feeder.hpp"
namespace lo {
std::pair<bool, std::string> Feeder::readString(const std::string &delim) {
    bool found = false;
    std::string result;
    bool in_escape = false;
    while (current()) {
        if (!in_escape && current() == '\\') {
            in_escape = true;
            continue;
        }
        if (in_escape) {
        }
        result.push_back(current());
        if (result.size() >= delim.size()) {
            if (!result.compare(result.size() - delim.size(), delim.size(),
                                delim)) {
                found = true;
                result.erase(result.size() - delim.size());
                return std::make_pair(found, std::move(result));
            }
        }
        next();
    }
    return std::make_pair(found, result);
}
} /* lo */
