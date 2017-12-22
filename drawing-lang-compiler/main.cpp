//
// Created by sherlofi on 2017/12/12.
//
#include "lexer.hpp"
#include <cstdio>

using namespace std;

int main(int argc, char *argv[]) {
    Token token;
    if (InitScanner(argv[1]))
        return 1;       //打开失败，退出程序
    printf("\n记号类别   字符串      常数值       函数指针\n");
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
