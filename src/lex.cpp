#include <limits>
#include "lex.hpp"
namespace lo {
#define TK(x) token_t((x), "")

Lex::token_t Lex::nextToken() {
    std::string value;
    while (true) {
        switch (feeder_.current()) {
            case 0: return TK(EOS);
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
            case '"':
            case '\'': {
                feeder_.next();
                auto str = readString(feeder_.current());
                return token_t(STRING, str);
            }
            case '[': {
                feeder_.next();
                if (feeder_.current() != '=' && feeder_.current() != '[') {
                    return TK('[');
                }
                std::string delim("]");
                while (true) {
                    switch (feeder_.current()) {
                        case 0:
                            throw LexError("expect end of string");
                        case '[':
                            delim.push_back(']');
                            feeder_.next();
                            return token_t(STRING, readLongString(delim));
                        case '=':
                            delim.push_back('=');
                            feeder_.next();
                            continue;
                        default:
                            throw LexError("unexpected symbol %c",
                                           feeder_.current());
                    }
                }
                break;
            }
            case '-': {
                feeder_.next();
                if (feeder_.current() != '-') {
                    return TK('-');
                }

                feeder_.next();
                if (feeder_.current() == '[') {
                    std::string delim("--]");
                    feeder_.next();
                    while (feeder_.current() == '=') {
                        delim.push_back('=');
                        feeder_.next();
                    }
                    switch (feeder_.current()) {
                        case '[':
                            delim.push_back(']');
                            feeder_.next();
                            readLongString(delim);
                            continue;
                        default:
                            break;
                    }
                }
                {  // short comment
                    while (feeder_.current() != '\n' &&
                           feeder_.current() != '\r' && feeder_.current() != 0)
                        feeder_.next();
                }
                break;
            }

            case '>': {
                feeder_.next();
                if (feeder_.current() == '=')
                    return TK(GE);
                else
                    return TK('>');
            }
            case '<': {
                feeder_.next();
                if (feeder_.current() == '=')
                    return TK(LE);
                else
                    return TK('<');
            }
            case '=': {
                feeder_.next();
                if (feeder_.current() == '=')
                    return TK(EQ);
                else
                    return TK('=');
            }
            case '~': {
                feeder_.next();
                if (feeder_.current() == '=')
                    return TK(NE);
                else
                    return TK('~');
            }

            case '.': {
                feeder_.next();
                if (feeder_.current() == '.') {
                    feeder_.next();
                    if (feeder_.current() == '.') {
                        return TK(DOTS);
                    } else {
                        return TK(CONCAT);
                    }
                } else if (!isdigit(feeder_.current())) {
                    return TK('.');
                } else {
                    // handle number
                    return TK(NUMBER);
                }
            }

            default: {
                value.push_back(feeder_.current());
                feeder_.next();
            }
        }
    }
}

std::string Lex::readString(char delim) {
    std::string result;
    while (true) {
        switch (feeder_.current()) {
            case 0: {
                throw LexError("expect end of string");
                break;
            }
            case '\\': {
                int c = 0;
                feeder_.next();
                switch (feeder_.current()) {
                    case 0:
                        continue;
                    case 'a':
                        c = '\a';
                        break;
                    case 'b':
                        c = '\b';
                        break;
                    case 'f':
                        c = '\f';
                        break;
                    case 'n':
                        c = '\n';
                        break;
                    case 'r':
                        c = '\r';
                        break;
                    case 't':
                        c = '\t';
                        break;
                    case 'v':
                        c = '\v';
                        break;
                    case '\n':
                    case '\r':
                        newline();
                        result.push_back('\n');
                        feeder_.next();
                        continue;
                    default: {
                        if (!isdigit(feeder_.current())) {
                            c = feeder_.current();
                        } else {
                            int i = 0;
                            do {
                                c = 10 * c + feeder_.current() - '0';
                                feeder_.next();
                            } while (++i < 3 && isdigit(feeder_.current()));
                            if (c > std::numeric_limits<unsigned char>::max()) {
                                throw LexError("sequence too large %d", c);
                            }
                            result.push_back(c);
                            continue;
                        }
                    }
                }
                result.push_back(c);
                feeder_.next();
                break;
            }
            default: {
                if (feeder_.current() == delim) {
                    feeder_.next();
                    return result;
                }
                result.push_back(feeder_.current());
                feeder_.next();
                break;
            }
        }
    }
}

std::string Lex::readLongString(const std::string& delim) {
    std::string result;
    while (true) {
        switch (feeder_.current()) {
            case 0:
                throw LexError("expect end of string");
                break;
            case '\r':
            case '\n':
                newline();
            default: {
                result.push_back(feeder_.current());
                feeder_.next();
                if (result.size() >= delim.size()) {
                    if (!result.compare(result.size() - delim.size(),
                                        delim.size(), delim)) {
                        result.erase(result.size() - delim.size());
                        return result;
                    }
                }
            }
        }
    }
    throw LexError("expect end of string");
}
} /* lo */
