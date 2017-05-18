#include "code.hpp"
#include "parser.hpp"
namespace lo {

#define EXPECT(token)                                                 \
    {                                                                 \
        if (token_.first != token) {                                  \
            if (token >= Lex::AND)                                    \
                throw ParserError("expect symbol %s",                 \
                                  Lex::tokenToString(token).c_str()); \
            else                                                      \
                throw ParserError("expect symbol %c", token);         \
        }                                                             \
    }

static bool blockFollow(int token) {
    switch (token) {
        case Lex::ELSE:
        case Lex::ELSEIF:
        case Lex::END:
        case Lex::UNTIL:
        case Lex::EOS:
            return true;
        default:
            return false;
    }
}

static UnOpr getunopr(int op) {
    switch (op) {
        case Lex::NOT:
            return OPR_NOT;
        case '-':
            return OPR_MINUS;
        case '#':
            return OPR_LEN;
        default:
            return OPR_NOUNOPR;
    }
}

static BinOpr getbinopr(int op) {
    switch (op) {
        case '+':
            return OPR_ADD;
        case '-':
            return OPR_SUB;
        case '*':
            return OPR_MUL;
        case '/':
            return OPR_DIV;
        case '%':
            return OPR_MOD;
        case '^':
            return OPR_POW;
        case Lex::CONCAT:
            return OPR_CONCAT;
        case Lex::NE:
            return OPR_NE;
        case Lex::EQ:
            return OPR_EQ;
        case '<':
            return OPR_LT;
        case Lex::LE:
            return OPR_LE;
        case '>':
            return OPR_GT;
        case Lex::GE:
            return OPR_GE;
        case Lex::AND:
            return OPR_AND;
        case Lex::OR:
            return OPR_OR;
        default:
            return OPR_NOBINOPR;
    }
}

void initExp(ExpDesc &e, ExpKind k, int i) {
    e.f = e.t = NO_JUMP;
    e.k = k;
    e.info = i;
}

static const std::pair<int, int> priority[] = {
    /* ORDER OPR */
    {6, 6},  {6, 6}, {7, 7}, {7, 7}, {7, 7}, /* `+' `-' `/' `%' */
    {10, 9}, {5, 4},                 /* power and concat (right associative) */
    {3, 3},  {3, 3},                 /* equality and inequality */
    {3, 3},  {3, 3}, {3, 3}, {3, 3}, /* order */
    {2, 2},  {1, 1}                  /* logical (and/or) */
};

#define UNARY_PRIORITY 8

Parser::Parser(Feeder &feeder) : lex_(feeder) {}

void Parser::chunk() {
    bool last = false;
    while (!last && !blockFollow(token_.first)) {
        last = statement();
    }
}

void Parser::cond() {
    ExpDesc v;
    expr(v);
}

void Parser::expr(ExpDesc &v) { subexpr(v, 0); }

void Parser::prefixexp(ExpDesc &v) {
    switch (token_.first) {
        case '(': {
            next();
            expr(v);
            if (token_.first != ')') throw ParserError("() not match!");
            next();
            break;
        }
        case Lex::NAME: {
            next();
            break;
        }
        default: { throw ParserError("unexpected symbol %c", token_.first); }
    }
}

void Parser::primaryexp(ExpDesc &v) {
    prefixexp(v);
    while (true) {
        switch (token_.first) {
            case '[': {
                next();
                expr(v);
                if (token_.first != ']') throw ParserError("[] not match!");
                break;
            }
            case '.': {
                next();
                if (token_.first != Lex::NAME)
                    throw ParserError("invalid dot symbol");
                next();
            }
            case ':': {
                next();
                if (token_.first != Lex::NAME)
                    throw ParserError("invalid dot symbol");
                next();
                break;
            }
            case '(':
            case Lex::STRING:
            case '{': {
                funcargs();
                break;
            }
            default:
                return;
        }
    }
}

void Parser::funcargs() {}

void Parser::body() {}

void Parser::constructor() {}

void Parser::simpleexp(ExpDesc &v) {
    switch (token_.first) {
        case Lex::NUMBER: {
            initExp(v, ExpKind::VKNUM, 0);
            v.nval = stod(token_.second);
            next();
            break;
        }
        case Lex::STRING:
        case Lex::NIL:
        case Lex::TRUE:
        case Lex::FALSE:
        case Lex::DOTS:
            next();
            break;
        case '{': {
            constructor();
            break;
        }
        case Lex::FUNCTION: {
            body();
            break;
        }
        default: { primaryexp(v); }
    }
}

BinOpr Parser::subexpr(ExpDesc &v, int limit) {
    UnOpr uop = getunopr(token_.first);
    if (uop != OPR_NOUNOPR) {
        next();
        subexpr(v, UNARY_PRIORITY);
        // TODO prefix
        switch (uop) {
            case OPR_MINUS:
                v.nval *= -1;
                break;
            default:
                break;
        }
    } else {
        simpleexp(v);
    }

    BinOpr bop = getbinopr(token_.first);
    while (bop != OPR_NOBINOPR && priority[bop].first > limit) {
        next();
        // TODO infix
        ExpDesc v2;
        auto nextop = subexpr(v2, priority[bop].second);
        switch (bop) {
            case OPR_ADD:
                v.nval += v2.nval;
                break;
            case OPR_MUL:
                v.nval *= v2.nval;
                break;
            case OPR_SUB:
                v.nval -= v2.nval;
                break;
            default:
                break;
        }
        // TODO postfix
        bop = nextop;
    }
    return bop;
}

void Parser::condThen() {
    cond();
    EXPECT(Lex::THEN);
    next();
    block();
}

void Parser::condDo() {
    cond();
    EXPECT(Lex::THEN);
    next();
    block();
}

void Parser::ifstat() {
    next();
    condThen();
    while (token_.first == Lex::ELSEIF) {
        condThen();
    }
    if (token_.first == Lex::ELSE) {
        next();
        block();
    }
    if (token_.first != Lex::END) throw ParserError("if end not match!");
    next();
}

void Parser::block() {
    Block bloc;
    currentFunc()->enterBlock(bloc);
    chunk();
    currentFunc()->leaveBlock();
}

void Parser::whilestat() {
    next();
    condDo();
}

void Parser::forstat() {}

void Parser::repeatstat() {
    next();
    block();
    if (token_.first != Lex::UNTIL) throw ParserError("if end not match!");
    next();
    cond();
}

void Parser::retstat() {
    next();
    // TODO ret
}

void Parser::breakstat() { next(); }

void Parser::funcstat() {
    next();
    funcargs();
    body();
}

void Parser::exprstat() {
    ExpDesc v;
    primaryexp(v);
}

void Parser::localfunc() {
    next();
    next();
    funcargs();
    body();
}

void Parser::localstat() {
    next();
    if (token_.first != Lex::NAME) throw ParserError("name for local expected");
    next();
    // TODO multi
}

bool Parser::statement() {
    switch (token_.first) {
        case Lex::IF: {
            ifstat();
            return false;
        }
        case Lex::WHILE: {
            whilestat();
            return false;
        }
        case Lex::FOR: {
            forstat();
            return false;
        }
        case Lex::REPEAT: {
            repeatstat();
            return false;
        }
        case Lex::FUNCTION: {
            funcstat();
            return false;
        }
        case Lex::RETURN: {
            retstat();
            return true;
        }
        case Lex::BREAK: {
            breakstat();
            return false;
        }
        case Lex::LOCAL: {
            next();
            if (token_.first == Lex::FUNCTION) {
                localfunc();
            } else {
                localstat();
            }
        }
        case Lex::DO: {
            next();
            block();
            if (token_.first != Lex::END)
                throw ParserError("do end not match!");
            next();
        }
        default: {
            exprstat();
            return false;
        }
    }
}

int Parser::parse() {
    next();
    openFunc();
    block();
    EXPECT(Lex::EOS);
    closeFunc();
    return 0;
}

void Parser::next() { token_ = lex_.nextToken(); }

void Parser::matchAndNext(int token) {
    if (token_.first == token) {
        next();
    }
}

void Parser::openFunc() {
    fslist_.emplace_front(std::make_unique<FuncState>());
}

void Parser::closeFunc() { fslist_.pop_front(); }

FuncState *Parser::currentFunc() {
    if (fslist_.empty()) return nullptr;
    return fslist_.front().get();
}

} /* lo */
