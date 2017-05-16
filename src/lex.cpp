#include "lex.hpp"
namespace lo {
Lex::token_t Lex::nextToken() {
    std::string value;
    while (true) {
        char current = feeder_.current();
        if (!current) {
            return token_t(EOS, "");
        }
        switch (feeder_.current()) {
            case '\r':
            case '\n':
                newline();
            case ' ':
            case '\t': {
                feeder_.next();
                if (value.size() > 0) {
                    return token_t(STRING, value);
                }
                break;
            }
                       /*
 case '[': {
     break;
 }
 case '.': {
     break;
 }
 case '-': {
     break;
 }
 */
            default: {
                value.push_back(feeder_.current());
                feeder_.next();
            }
        }
    }
}

} /* lo */
