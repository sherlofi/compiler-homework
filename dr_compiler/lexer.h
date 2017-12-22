//
// Created by sherlofi on 2017/12/11.
//

#ifndef COMPILER_HOMEWORK_DR_LEXICAL_ANALYSIS_H
#define COMPILER_HOMEWORK_DR_LEXICAL_ANALYSIS_H

#include <cmath>
#include <string>

enum TokenType {
    ORIGIN, SCALE, ROT, IS,
    TO, STEP, DRAW, FOR, FROM,
    T,
    SEMICO, L_BRACKET, R_BRACKET, COMMA,
    PLUS, MINUS, MUL, DIV, POWER,
    FUNC,
    CONST_ID,
    NONTOKEN,
    ERRTOKEN
};

struct Token {
    TokenType type;
    std::string lexeme;
    double value;
    double (*FuncPtr)(double);
};

static Token TokenTable[] = {
        {CONST_ID, "PI",     3.1415926, nullptr},
        {CONST_ID, "E",      2.71828,   nullptr},
        {T,        "T",      0.0,       nullptr},
        {FUNC,     "SIN",    0.0,       sin},
        {FUNC,     "COS",    0.0,       cos},
        {FUNC,     "TAN",    0.0,       tan},
        {FUNC,     "LN",     0.0,       log},
        {FUNC,     "EXP",    0.0,       exp},
        {FUNC,     "SQRT",   0.0,       sqrt},
        {ORIGIN,   "ORIGIN", 0.0,       nullptr},
        {SCALE,    "SCALE",  0.0,       nullptr},
        {ROT,      "ROT",    0.0,       nullptr},
        {IS,       "IS",     0.0,       nullptr},
        {FOR,      "FOR",    0.0,       nullptr},
        {FROM,     "FROM",   0.0,       nullptr},
        {TO,       "TO",     0.0,       nullptr},
        {STEP,     "STEP",   0.0,       nullptr},
        {DRAW,     "DRAW",   0.0,       nullptr}
};

extern int InitScanner(const char *);
extern Token GetToken(void);
extern void CloseScanner(void);

#endif //COMPILER_HOMEWORK_DR_LEXICAL_ANALYSIS_H
