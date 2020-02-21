/*************************************************************************
  > File Name: Evaluate.cpp
  > Author: pengjunjie
  > Mail: 1002398145@qq.com
  > Created Time: 2019年06月25日 星期二 15时33分57秒
 ************************************************************************/

#include <Stack/Stack.hpp>

#include <iostream>

#include <math.h>
#include <stdlib.h>

//中缀表达式
enum N_OPTR
{
    ADD = 0,
    SUB,
    MUL,
    DIV,
    POW,
    FAC,
    L_P,
    R_P,
    EOE,
};

//+,-,*,/,^,!,(,),\0
const char pri[9][9] = {//操作运算符优先级[栈顶][当前]
    '>','>','<','<','<','<','<','>','>',
    '>','>','<','<','<','<','<','>','>',
    '>','>','>','>','<','<','<','>','>',
    '>','>','>','>','<','<','<','>','>',
    '>','>','>','>','>','<','<','>','>',
    '>','>','>','>','>','>',' ','>','>',
    '<','<','<','<','<','<','<','=',' ',
    ' ',' ',' ',' ',' ',' ',' ',' ',' ',
    '<','<','<','<','<','<','<',' ','='
};



int optr2rank(char op) { //由运算符转译出编号
    switch (op) {
       case '+': return ADD; //加
       case '-': return SUB; //减
       case '*': return MUL; //乘
       case '/': return DIV; //除
       case '^': return POW; //乘方
       case '!': return FAC; //阶乘
       case '(': return L_P; //左括号
       case ')': return R_P; //右括号
       case '\0': return EOE; //起始符与终止符
       default: exit(-1); //未知运算符

    }
}


char orderBetween(char op1, char op2) //比较两个运算符之间的优先级
{ return pri[optr2rank(op1)][optr2rank(op2)]; }

//将起始于p的子串解析为数值，并存入操作数栈中
void readNumber(char*& p, Stack<float>& stk)
{
    stk.push((float)(*p - '0'));//当前数位对应数值进栈
    while (isdigit(*(++p)))//后续紧跟数字
    {
        stk.push(stk.pop() * 10 + (*p - '0'));//弹出原操作数并追加新数位后重新压入栈中
    }

    if ('.' != *p)//此后非小数点则当前操作数解析完成
    {
        return;
    }
    float fraction = 1;//还有小数部分
    while (isdigit(*(++p)))//解析小数部分并加入
    {
        stk.push(stk.pop() + (*p - '0') * (fraction /= 10));
    }
}

float calcu(char op, float opnd)
{
    float sum = 1;
    while (0 < opnd)
    {
        sum *= opnd--;
    }

    return sum;
}

float calcu(float opnd1, char op, float opnd2)
{
    int rank = optr2rank(op);
    switch (rank)
    {
    case ADD:
        return opnd1 + opnd2;

    case SUB:
        return opnd1 - opnd2;

    case MUL:
        return opnd1 * opnd2;

    case DIV:
        return opnd1 / opnd2;

    case POW:
        return pow(opnd1, opnd2);

    default:
        return 0.0;
    }
}

float evaluate(char* s)
{
    Stack<float> opnd;//运算数栈
    Stack<char> optr;//运算符栈

    optr.push('\0');//作为头哨兵首先入栈
    while (!optr.empty())
    {
        if (isdigit(*s))
        {
            readNumber(s, opnd);
        }
        else
        {
            switch (orderBetween(optr.top(), *s))
            {
            case '<'://栈顶运算符优先级更低
                optr.push(*s);//计算推迟，当前运算符入栈
                ++s;
                break;

            case '='://优先级相等(当前运算符为右括号或者尾哨兵'\0')
                optr.pop();//脱括号并接收下一个字符
                ++s;
                break;

            case '>'://栈顶运算符优先级更高，实施相应的计算，结果入栈
            {
                char op = optr.pop();
                if ('!' == op)//一元运算符
                {
                    //std::cout << "!" << opnd.top() << "=";
                    opnd.push(calcu(op, opnd.pop()));

                    //std::cout << opnd.top() << std::endl;

                }
                else//二元运算符
                {
                    float pOpnd2 = opnd.pop();
                    float pOpnd1 = opnd.pop();
                    //std::cout << pOpnd1 << op << pOpnd2;
                    opnd.push(calcu(pOpnd1, op, pOpnd2));//计算后结果入栈
                    //std::cout << "=" << opnd.top() << std::endl;
                }
            }
                break;
            }
        }
    }

    //std::cout << "opnd :" << opnd.top() << std::endl;
    return opnd.top();
}

int main()
{
    char expression[] = "(0!+1)*2^(3!+4)-(5!-67-(8+9))";
    evaluate(expression);
}