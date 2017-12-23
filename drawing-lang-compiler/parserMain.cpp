//
// Created by sherlofi on 2017/12/23.
//

#include "parser.hpp"
extern void Parser(const char *SrcFilePtr);//测试主程序
int main()  {
    Parser("test1.txt");//调用parser进行语法分析，其中被测试的语句在test.txt
    return 0;
}

