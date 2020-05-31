
#pragma once

class Fib
{
public:
    Fib(int maxN)//初始化为不小于n的最小Fibonacci项
        :f(1)//fib(-1)
        , g(0)//fib(0)
        , n(0)
        , maxItem(maxN)
    {
        while (n < maxItem)
        {
            next();
        }
    }

    int get() { return g; }//获取当前Fibonacci项，O(1)时间
    int getItem() { return n; }//获取当前项数，O(1)时间
    int next()//转至下一Fibonacci项，O(1)时间
    {
        g += f;
        f = g - f;
        ++n;
        return g;
    }
    int prev()//转至上一Fibonacci项，O(1)时间
    {
        f = g - f;
        g -= f;
        --n;
        return g;
}

private:
    int f;//f = fib(k - 1), g = fib(k),均为int型，很快就会数值溢出
    int g;
    int n;//当前项数
    int maxItem;
};