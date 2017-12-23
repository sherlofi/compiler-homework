//
// Created by sherlofi on 2017/12/22.
//

#ifndef COMPILER_HOMEWORK_PARSER_HPP
#define COMPILER_HOMEWORK_PARSER_HPP

#include "lexer.hpp"
typedef double (*FuncPtr)(double);
struct ExprNode {
    enum TokenType OpCode;

    union {
        struct {
            ExprNode *Left, *Right;
        } CaseOperator;
        struct {
            ExprNode *Child;
            FuncPtr MathFuncPtr;
        } CaseFunc;
        double CaseConst; //常数：叶子节点  右值
        double *CaseParmPtr; //参数T   左值：存放T的地址
    } Content;
};

extern void Parser(const char *SrcFilePtr);
#endif //COMPILER_HOMEWORK_PARSER_HPP
