#include <sstream>
#include "feeder.hpp"
namespace lo {
std::pair<bool, std::string> Feeder::readUntil(const std::string &delim) {
    bool found = false;
    std::string result;
    while (next()) {
        result.push_back(current());
        if (result.size() >= delim.size()) {
            if (!result.compare(result.size() - delim.size(), delim.size(),
                                delim)) {
                found = true;
                result.erase(result.size() - delim.size());
                return std::make_pair(found, std::move(result));
            }
        }
    }
    return std::make_pair(found, result);
}
} /* lo */
