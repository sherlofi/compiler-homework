//
// Created by sherlofi on 2017/12/12.
//
#include "lexer.hpp"
#include <cstdio>

using namespace std;

int main() {
    Token token;
    if (InitScanner("test1.txt"))
        return 1;       //打开失败，退出程序
    printf("\nTokenType   Lexeme      Value       Pointer\n");
    printf("------------------------------------------------\n");
    while (1) {
        token = GetToken();
        if (token.type != NONTOKEN) {
            printf("%4d %12s %12f %12x\n", token.type, token.lexeme.c_str(), token.value, token.FuncPtr);
        } else {
            break;
        }
    }
    CloseScanner();
    return 0;
}
