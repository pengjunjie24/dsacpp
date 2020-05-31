
#pragma once

//* HailStone序列
//* n = 1时，返回1；
//* n > 1时且为偶数时，{ n } ∪{ n / 2 }
//*n > 1时且为奇数时，{ n } ∪{ 3n + 1 }

//函数对象：按照Hailstone规则转化一个T类对象
template <typename T>
struct Hailstone
{
    virtual void operator() (T& e)
    {
        int step = 0;//转换所需步数
        while (1 != e)//按奇、偶逐步转换，直至为1
        {
            (e % 2) ? (e = 3 * e + 1) : (e /= 2);
            step++;
        }
        e = step;//返回转换所经步数
    }
};