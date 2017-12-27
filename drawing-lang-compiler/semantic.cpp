#include"semantic.hpp"
extern double Parameter,
        Origin_x, Origin_y,
        Scale_x, Scale_y,
        Rot_angle;

static void CalcCoord(struct ExprNode *Hor_Exp, struct ExprNode *Ver_Exp, double &Hor_x, double &Ver_y) {
    double HorCord, VerCord, Hor_tmp;
    HorCord = GetExprValue(Hor_Exp);
    VerCord = GetExprValue(Ver_Exp);
    HorCord *= Scale_x;
    VerCord *= Scale_y;   //进行比例变换
    Hor_tmp = HorCord * cos(Rot_angle) + VerCord * sin(Rot_angle);
    VerCord = VerCord * cos(Rot_angle) - HorCord * sin(Rot_angle);
    HorCord = Hor_tmp;    //进行旋转变换
    HorCord += Origin_x;
    VerCord += Origin_y;    //进行平移变换
    Hor_x = HorCord;
    Ver_y = VerCord;    //返回变换后点的坐标
}

//----------循环绘制点的坐标
void DrawLoop(double Start,double End,double Step, struct ExprNode *HorPtr, struct ExprNode *VerPtr) {
    extern double Parameter;
    double x, y;
    for (Parameter = Start; Parameter <= End; Parameter += Step) {  //把t在范围内的每一个值带入计算
        CalcCoord(HorPtr, VerPtr, x, y);//计算要绘制店的实际坐标
        DrawPixel((unsigned long) x, (unsigned long) y);//绘制这个点
    }
}

//----------计算表达式的值
double GetExprValue(struct ExprNode *root) {//后续遍历语法树  根据不同的节点类型计算当前根节点的值
    if (root == NULL)
        return 0.0;
    switch (root->OpCode) {
        //二元运算符
        case PLUS :
            return GetExprValue(root->Content.CaseOperator.Left) + GetExprValue(root->Content.CaseOperator.Right);
        case MINUS :
            return GetExprValue(root->Content.CaseOperator.Left) - GetExprValue(root->Content.CaseOperator.Right);
        case MUL :
            return GetExprValue(root->Content.CaseOperator.Left) * GetExprValue(root->Content.CaseOperator.Right);
        case DIV :
            return GetExprValue(root->Content.CaseOperator.Left) / GetExprValue(root->Content.CaseOperator.Right);
        case POWER :
            return pow(GetExprValue(root->Content.CaseOperator.Left), GetExprValue(root->Content.CaseOperator.Right));
            // 函数调用
        case FUNC :
            return (*root->Content.CaseFunc.MathFuncPtr)(GetExprValue(root->Content.CaseFunc.Child));
            // 常数
        case CONST_ID :
            return root->Content.CaseConst;
            // 参数
        case T :
            return *(root->Content.CaseParmPtr);
        default :
            return 0.0;
    }//返回值是表达式的值
}

void DelExprTree(struct ExprNode *root) {
    if (root == NULL)
        return;
    switch (root->OpCode) {
        case PLUS :                                                 //二元：：两个孩子的内部节点
        case MINUS :
        case MUL :
        case DIV :
        case POWER :
            DelExprTree(root->Content.CaseOperator.Left);
            DelExprTree(root->Content.CaseOperator.Right);
            break;
        case FUNC :                                                 //一元：：一个孩子的内部节点
            DelExprTree(root->Content.CaseFunc.Child);
            break;
        default :                                                   //叶子节点
            break;
    }
    delete (root);                                               //删除节点
}

void DrawPixel(unsigned long x, unsigned long y) {
    SetPixel(hDC,(int)x,(int)y,red);
}