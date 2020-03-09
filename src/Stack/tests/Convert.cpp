/*************************************************************************
  > File Name: Convert.cpp
  > Author: pengjunjie
  > Mail: 1002398145@qq.com
  > Created Time: 2019年06月25日 星期二 10时42分36秒
 ************************************************************************/

#include <Stack/Stack.hpp>

#include <stdio.h>
#include <stdint.h>

//进制转换，输出次序与处理过程颠倒，递归深度和输出长度不易预知
//s:按数位存储数据
//n:待转换的十进制数据
//base:转换的进制数
void convert(Stack<char>& s, int64_t n, int base)
{
    static char digit[] = { '0', '1', '2', '3', '4', '5',
        '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

    while (n > 0)
    {
        s.push(digit[n % base]);//余数(对应的数位进栈)
        n /= base;//n更新为其对base的余商
    }
}

int main()
{
    Stack<char> s;
    convert(s, 1000, 2);//用栈记录转换得到各数位
    while (!s.empty())
    {
        printf("%c", s.pop());//逆序输出
    }

    printf("\n");
}