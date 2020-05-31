

#pragma once

//线性递归: T(n) = T(n/2) + O(1) = O(logn)
//递归跟踪: 轴点总取中点，递归深度O(logn),各递归实例均耗时O(1)
template<typename T>
Rank binSearch(T* S, T const& e, Rank low, Rank high)// 二分查找算法
//在有序向量的区间[lo, hi)内查找元素e，0 <= lo <= hi <= _size
{
    while (low < high)
    {
        Rank mid = (low + high) >> 1;//以中点为轴点（区间宽度的折半，等效于宽度之数值表示的右移）
        (e < S[mid]) ? high = mid : low = mid + 1;//经比较后确定深入[lo, mi)或(mi, hi)
    }//成功查找不能提前终止

    return low - 1;//循环结束时，lo为大于e的元素的最小秩，故lo - 1即不大于e的元素的最大秩
}//有多个命中元素时，总能保证返回秩最大者；查找失败时，能够返回失败的位置