//
// Created by sherlofi on 2017/12/23.
//
#include "parser.hpp"
#include <cstdarg>
#include <cstdio>
#include <cstdlib>

//#define PARSER_DEBUG

#ifndef PARSER_DEBUG

#include "semantic.hpp"

#endif

#ifdef PARSER_DEBUG
    #define enter(x) printf("enter in   ");printf(x);printf("\n")
#else
    #define enter(x)
#endif

#ifdef  PARSER_DEBUG
    #define back(x) printf("exit from   ");printf(x);printf("\n")
#else
    #define back(x)
#endif

#ifdef  PARSER_DEBUG
#define call_match(x) printf("match token  ");printf(x);printf("\n")
#else
#define call_match(x)
#endif

#ifdef  PARSER_DEBUG
    #define Tree_trace(x) PrintSyntaxTree(x,1);
#else
    #define Tree_trace
#endif

#ifdef PARSER_DEBUG
    double Parameter = 0;
#else
    double Parameter = 0,
    Origin_x = 0, Origin_y = 0,
    Scale_x = 1, Scale_y = 1,
    Rot_angle = 0;
#endif

static Token token;
using std::string;

//------------------ 辅助函数
static void FetchToken();
static void MatchToken(enum TokenType TheToken);
static void SyntaxError(int CaseOf);
static void ErrMsg(unsigned LineNo, string description,string lexeme);
static void PrintSyntaxTree(struct ExprNode *root, int indent);

//------------------ 非终结符的递归下降子程序
static void Program();
static void Statement();
static void OriginStatement();
static void RotStatement();
static void ScaleStatement();
static void ForStatement();
static struct ExprNode *Expression();
static struct ExprNode *Term();
static struct ExprNode *Factor();
static struct ExprNode *Component();
static struct ExprNode *Atom();

//------------------- 外部接口与语法书构造函数
extern void Parser(const char *SrcFilePtr);
static struct ExprNode *MakeExprNode(enum TokenType opcode, ...);

//------------- 获取一个记号
static void FetchToken() {
    token = GetToken();
    if(token.type == ERRTOKEN)
        SyntaxError(1);
}

//------------- 匹配记号
static void MatchToken(enum TokenType TheToken) {
    if(token.type != TheToken)
        SyntaxError(2);
    FetchToken();
}

//-------------- 语法错误处理
static void SyntaxError(int CaseOf) {
    switch (CaseOf){
        case 1:
            ErrMsg(LineNo, "错误记号", token.lexeme);
            break;
        case 2:
            ErrMsg(LineNo, "不是预期记号", token.lexeme);
            break;
    }
}

//--------------- 打印错误信息
void ErrMsg(unsigned LineNo, string description, string lexeme) {
#ifdef PARSER_DEBUG
    printf("Line No %2d : %s %s !\n", LineNo, description.c_str(), lexeme.c_str());
#else
    char msg[256];
    sprintf(msg, "Line No %2d : %s %s !", LineNo, description.c_str(), lexeme.c_str());
#endif

#ifdef _VC_COMPILER
    MessageBox(NULL, msg, "error!", MB_OK);
#endif

#ifdef _BC_COMPILER
    printf("%s\n", msg);
#endif

    CloseScanner();
    exit(1);
}

//-------------- 先序遍历打印语法树
void PrintSyntaxTree(struct ExprNode *root, int indent) {
    int temp;
    for(temp = 1; temp <= indent; temp++)
        printf("\t");
    switch(root -> OpCode) {
        case PLUS:
            printf("%s\n", "+");
            break;
        case MINUS:
            printf("%s\n", "-");
            break;
        case MUL:
            printf("%s\n", "*");
            break;
        case DIV:
            printf("%s\n", "/");
            break;
        case POWER:
            printf("%s\n", "**");
            break;
        case FUNC:
            printf("%x\n", root->Content.CaseFunc.MathFuncPtr);
            break;
        case CONST_ID:
            printf("%f\n", root->Content.CaseConst);
            break;
        case T:
            printf("%s\n", "T");
            break;
        default:
            printf("Error Tree Node !\n");
            exit(0);
    }
    if(root->OpCode == CONST_ID || root->OpCode == T)
        return ;
    if(root->OpCode == FUNC)
        PrintSyntaxTree(root->Content.CaseFunc.Child, indent+1);
    else {
        PrintSyntaxTree(root->Content.CaseOperator.Left, indent+1);
        PrintSyntaxTree(root->Content.CaseOperator.Right, indent+1);
    }
}

void Parser(const char *SrcFilePtr) {
    enter("Parser");
    if(InitScanner(SrcFilePtr))
        return;
    FetchToken();
    Program();
    CloseScanner();
    back("Parser");
    return;
}

//--------------- 递归子程序部分
static void Program() {
    enter("Program");
    while (token.type != NONTOKEN) {
        Statement();
        MatchToken(SEMICO);
    }
    back("Program");
}

static void Statement() {
    enter("Statement");
    switch (token.type) {
        case ORIGIN :
            OriginStatement();
            break;
        case SCALE :
            ScaleStatement();
            break;
        case ROT :
            RotStatement();
            break;
        case FOR :
            ForStatement();
            break;
        default:
            SyntaxError(2);
    }
    back("Statement");
}

static void OriginStatement() {
    ExprNode *tmp;
    enter("OriginStatement");
    MatchToken(ORIGIN);
    MatchToken(IS);
    MatchToken(L_BRACKET);
    tmp = Expression();
#ifndef PARSER_DEBUG
    Origin_x = GetExprValue(tmp);//获取横坐标点平移距离
    DelExprTree(tmp);//删除一棵树
#endif
    MatchToken(COMMA);//eg:,
    tmp = Expression();
#ifndef PARSER_DEBUG
    Origin_y = GetExprValue(tmp);                     //获取纵坐标的平移距离
    DelExprTree(tmp);
#endif
    MatchToken(R_BRACKET);//eg:)
    back("OriginStatement");
}

static void ScaleStatement(void)
{
    struct ExprNode *tmp;
    enter("ScaleStatement");
    MatchToken(SCALE);
    MatchToken(IS);
    MatchToken(L_BRACKET);//eg:scale is (
    tmp = Expression();
#ifndef PARSER_DEBUG
    Scale_x = GetExprValue(tmp);//获取横坐标的比例因子
    DelExprTree(tmp);
#endif
    MatchToken(COMMA);//eg:,
    tmp = Expression();
#ifndef PARSER_DEBUG
    Scale_y = GetExprValue(tmp);//获取纵坐标的比例因子
    DelExprTree(tmp);
#endif
    MatchToken(R_BRACKET);//eg:)
    back("ScaleStatement");
}

static void RotStatement(void)
{
    struct ExprNode *tmp;
    enter("RotStatement");
    MatchToken(ROT);
    MatchToken(IS);//eg:rot is
    tmp = Expression();
#ifndef PARSER_DEBUG
    Rot_angle = GetExprValue(tmp);//获取旋转角度
    DelExprTree(tmp);
#endif
    back("RotStatement");
}

static void ForStatement()
{
    //eg:for T from 0 to 2*pi step pi/50 draw (t, -sin(t));
#ifndef PARSER_DEBUG
    double Start, End, Step;//绘图起点、终点、步长
#endif
    struct ExprNode *start_ptr, *end_ptr,
            *step_ptr, *x_ptr, *y_ptr;//指向各表达式语法树根节点
    enter("ForStatement");
    //遇到非终结符就调用相应子程序
    MatchToken(FOR); call_match("FOR");
    MatchToken(T); call_match("T");
    MatchToken(FROM); call_match("FROM");//                         eg:for T from
    start_ptr = Expression();//获得参数起点表达式的语法树
#ifndef PARSER_DEBUG
    Start = GetExprValue(start_ptr);//计算参数起点表达式的值
    DelExprTree(start_ptr);//释放参数起点语法树所占空间            eg:0
#endif
    MatchToken(TO); call_match("TO");//                            eg:to
    end_ptr = Expression();//构造参数终点表达式语法树
#ifndef PARSER_DEBUG
    End = GetExprValue(end_ptr);//计算参数终点表达式的值              eg:2*pi
    DelExprTree(end_ptr);//释放参数终点语法树所占空间
#endif
    MatchToken(STEP); call_match("STEP");//                        eg:step
    step_ptr = Expression();//构造参数步长表达式语法树
#ifndef PARSER_DEBUG
    Step = GetExprValue(step_ptr);//计算参数步长表达式的值         eg:pi/50
    DelExprTree(step_ptr);//释放参数步长语法树所占空间
#endif
    MatchToken(DRAW);
    call_match("DRAW");
    MatchToken(L_BRACKET);
    call_match("(");//                                                eg:draw（
    x_ptr = Expression();//跟节点                                  eg:t
    MatchToken(COMMA);
    call_match(",");//                                                eg:,
    y_ptr = Expression();//根节点
    MatchToken(R_BRACKET);
    call_match(")");
#ifndef PARSER_DEBUG
    DrawLoop(Start, End, Step, x_ptr, y_ptr);          //绘制图形
    DelExprTree(x_ptr);                             //释放横坐标语法树所占空间
    DelExprTree(y_ptr);                             //释放纵坐标语法树所占空间
#endif
    back("ForStatement");
}

//----------Expression的递归子程序
//把函数设计为语法树节点的指针，在函数内引进2个语法树节点的指针变量，分别作为Expression左右操作数（Term）的语法树节点指针
//表达式应该是由正负号或无符号开头、由若干个项以加减号连接而成。
static struct ExprNode* Expression()//展开右部，并且构造语法树
{
    struct ExprNode *left, *right;//左右子树节点的指针
    enum TokenType token_tmp;//当前记号

    enter("Expression");
    left = Term();//分析左操作数且得到其语法树
    while (token.type == PLUS || token.type == MINUS)
    {
        token_tmp = token.type;
        MatchToken(token_tmp);
        right = Term();//分析右操作数且得到其语法树
        left = MakeExprNode(token_tmp, left, right);//构造运算的语法树，结果为左子树
    }
    Tree_trace(left);//打印表达式的语法树
    back("Expression");
    return left;//返回最终表达式的语法树
}

//----------Term的递归子程序
//项是由若干个因子以乘除号连接而成
static struct ExprNode* Term()
{
    struct ExprNode *left, *right;
    enum TokenType token_tmp;
    left = Factor();
    while (token.type == MUL || token.type == DIV)
    {
        token_tmp = token.type;
        MatchToken(token_tmp);
        right = Factor();
        left = MakeExprNode(token_tmp, left, right);
    }
    return left;
}

//----------Factor的递归子程序
//因子则可能是一个标识符或一个数字，或是一个以括号括起来的子表达式
static struct ExprNode *Factor()
{
    struct ExprNode *left, *right;
    if (token.type == PLUS)                            //匹配一元加运算
    {
        MatchToken(PLUS);
        right = Factor();                             //表达式退化为仅有右操作数的表达式
    }
    else if (token.type == MINUS)
    {
        MatchToken(MINUS);
        right = Factor();
        left = new ExprNode;
        left->OpCode = CONST_ID;
        left->Content.CaseConst = 0.0;
        right = MakeExprNode(MINUS, left, right);
    }
    else
        right = Component();                          //匹配非终结符Component
    return right;
}

//----------Component的递归子程序
//幂
static struct ExprNode* Component()//右结合
{
    struct ExprNode *left, *right;
    left = Atom();
    if (token.type == POWER)
    {
        MatchToken(POWER);
        right = Component();                          //递归调用Component以实现POWER的右结合
        left = MakeExprNode(POWER, left, right);
    }
    return left;
}

//----------Atom的递归子程序
//包括分隔符 函数 常数 参数
static struct ExprNode* Atom()
{
    struct Token t = token;
    struct ExprNode *address = nullptr, *tmp;
    switch (token.type)
    {
        case CONST_ID:
            MatchToken(CONST_ID);
            address = MakeExprNode(CONST_ID, t.value);
            break;
        case T:
            MatchToken(T);
            address = MakeExprNode(T);
            break;
        case FUNC:
            MatchToken(FUNC);
            MatchToken(L_BRACKET);
            tmp = Expression();
            address = MakeExprNode(FUNC, t.FuncPtr, tmp);
            MatchToken(R_BRACKET);
            break;
        case L_BRACKET:
            MatchToken(L_BRACKET);
            address = Expression();
            MatchToken(R_BRACKET);
            break;
        default:
            SyntaxError(2);
    }
    return address;
}

static struct ExprNode *MakeExprNode(enum TokenType opcode, ...)
{
    struct ExprNode *ExprPtr = new(struct ExprNode);
    ExprPtr->OpCode = opcode;//接收记号的类别
    va_list ArgPtr;//声明一个转换参数的变量
    va_start(ArgPtr, opcode);   //初始化变量
    switch (opcode)//根据记号的类别构造不同的节点
    {
        case CONST_ID://常数
            ExprPtr->Content.CaseConst = (double)va_arg(ArgPtr, double);//右值
            break;
        case T://参数
            ExprPtr->Content.CaseParmPtr = &Parameter;//左值：地址
            break;
        case FUNC://函数调用
            ExprPtr->Content.CaseFunc.MathFuncPtr = (FuncPtr)va_arg(ArgPtr, FuncPtr);//指向对应函数名的指针 MathFuncPtr
            ExprPtr->Content.CaseFunc.Child = (struct ExprNode*)va_arg(ArgPtr, struct ExprNode *);//孩子的内部节点
            break;
        default://二元运算节点
            ExprPtr->Content.CaseOperator.Left = (struct ExprNode *)va_arg(ArgPtr, struct ExprNode *);
            ExprPtr->Content.CaseOperator.Right = (struct ExprNode *)va_arg(ArgPtr, struct ExprNode *);
            break;
    }
    va_end(ArgPtr);//结束变量列表,和va_start成对使用
    return ExprPtr;
}
