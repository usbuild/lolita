#include "parser.hpp"
namespace lo {

#define EXPECT(token)                                     \
    {                                                     \
        if (token_.first != token) {                      \
            throw ParserError("expect symbol %d", token); \
        }                                                 \
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

static const std::pair<int, int> priority[] = {
    /* ORDER OPR */
    {6, 6},  {6, 6}, {7, 7}, {7, 7}, {7, 7}, /* `+' `-' `/' `%' */
    {10, 9}, {5, 4},                 /* power and concat (right associative) */
    {3, 3},  {3, 3},                 /* equality and inequality */
    {3, 3},  {3, 3}, {3, 3}, {3, 3}, /* order */
    {2, 2},  {1, 1}                  /* logical (and/or) */
};

#define UNARY_PRIORITY 8

Parser::Parser(Feeder& feeder) : lex_(feeder) {}

void Parser::chunk() {
    while (!blockFollow(token_.first)) {
        statement();
    }
}

void Parser::cond() { expr(); }

void Parser::expr() { subexpr(0); }

void Parser::simpleexp() { next(); }

BinOpr Parser::subexpr(int limit) {
    UnOpr uop = getunopr(token_.first);
    if (uop != OPR_NOUNOPR) {
        next();
        subexpr(UNARY_PRIORITY);
        // TODO prefix
    } else {
        simpleexp();
    }

    BinOpr bop = getbinopr(token_.first);
    while (bop != OPR_NOBINOPR && priority[bop].first > limit) {
        next();
        // TODO infix
        auto nextop = subexpr(priority[bop].second);
        // TODO postfix
        bop = nextop;
    }
    return bop;
}

void Parser::condThen() {
    cond();
    EXPECT(Lex::THEN);
}

void Parser::statement() {
    switch (token_.first) {
        case Lex::IF: {
            next();
            condThen();
            break;
        }
        default:
            next();
            break;
    }
}

int Parser::parse() {
    next();
    chunk();
    return 0;
}

void Parser::next() { token_ = lex_.nextToken(); }
} /* lo */
