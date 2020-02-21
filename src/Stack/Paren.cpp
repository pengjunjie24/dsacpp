/*************************************************************************
  > File Name: Paren.cpp
  > Author: pengjunjie
  > Mail: 1002398145@qq.com
  > Created Time: 2019年06月25日 星期二 11时36分05秒
 ************************************************************************/

#include <Stack/Stack.hpp>

#include <stdio.h>
#include <string.h>

//匹配括号,具有自相似的问题可以递归描述，但分支位置和嵌套深度不固定
//为什么不使用计数器，因为计数器不能匹配多个括号的情况。如:[(])
bool paren(const char exp[], int lo, int hi)//exp[lo, hi)
{
    Stack<char> s;//使用栈记录已经发现但是尚未匹配的左括号
    for (int i = lo; i < hi; ++i)//逐一检查当前字符
    {
        if ('(' == exp[i] || ')' == exp[i])
        {
            if ('(' == exp[i])
            {
                s.push(exp[i]);//遇到左括号进栈
            }
            else if (!s.empty())//遇到右括号且栈非空
            {
                s.pop();
            }
            else//遇到右括号且栈为空，则必不匹配
            {
                return false;
            }
        }

    }

    return s.empty();//只有栈为空，才能说明表达式匹配
}

int main()
{
    char content1[] = "(asd(sdfds)(sdfsdaf(fsd))dfdsfa)";
    char content2[] = "bsdkfds(sdfsad(dfasdfad(dfasd)klkjo0f)";

    printf("content1 bracket matching: %s\n",
        paren(content1, 0, strlen(content1)) ? "true" : "false");

    printf("content2 bracket matching: %s\n",
        paren(content2, 0, strlen(content2)) ? "true" : "false");
}