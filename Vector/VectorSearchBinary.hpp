

#pragma once

//线性递归: T(n) = T(n/2) + O(1) = O(logn)
//递归跟踪: 轴点总取中点，递归深度O(logn),各递归实例均耗时O(1)
template<typename T>
Rank binSearch(T* S, T const& e, Rank low, Rank high)// 二分查找算法（版本A）：
//在有序向量的区间[lo, hi)内查找元素e，0 <= lo <= hi <= _size
{
    while (low < high)
    {
        Rank mid = (low + high) >> 1;//每步迭代可能要做两次比较判断，有三个分支
        if (e < S[mid])//以中点为轴点（区间宽度的折半，等效于宽度之数值表示的右移）
        {
            high = mid;//深入前半段[lo, mid)继续查找
        }
        else if (S[mid] < e)
        {
            low = mid + 1;//深入后半段(mid, hi)继续查找
        }
        else
        {
            return mid;//在mi处命中
        }
    }
    return -1;//查找失败
    //有多个命中元素时，不能保证返回秩最大者；查找失败时，简单地返回-1，而不能指示失败的位置
}