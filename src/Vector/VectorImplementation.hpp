
/******************************************************************************************
 * 将vector各方法的实现部分，简洁地引入Vector.hpp
 * 效果等同于将这些实现直接汇入vector.h
 * 在export尚未被编译器支持前，如此可将定义与实现分离
 ******************************************************************************************/

#pragma once

#include <Vector/VectorSearchBinary.hpp>

#include <assert.h>

template <typename T>
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi) //复制数组区间A[lo, hi)
{
    _elem = new T[_capacity = 2 * (hi - lo)];//分配空间，规模清零
    _size = 0;
    while (lo < hi)//A[lo, hi)内的元素逐一复制至_elem[0, hi - lo)
    {
        _elem[_size++] = A[lo++];
    }
}

template <typename T>
void Vector<T>::expand() //空间不足时扩容,加倍扩容分摊时间成本为O(1)
{
    if (_size < _capacity)//尚未满员时，不必扩容
    {
        return;
    }

    _capacity = max(_capacity, DEFAULT_CAPACITY);//不低于最小容量
    T* oldElem = _elem;
    _elem = new T[_capacity <<= 1];//容量加倍
    for (int i = 0; i < _size; ++i)
    {
        _elem[i] = oldElem[i];//复制原向量内容（T为基本类型，或已重载赋值操作符'='）
    }
    delete[] oldElem;//释放原空间
}

template <typename T>
void Vector<T>::shrink() //装填因子过小时压缩
{
    if (_capacity < DEFAULT_CAPACITY << 1)
    {
        return;//不致收缩到DEFAULT_CAPACITY以下
    }
    if (_size << 2 > _capacity)
    {
        return; //以25%为界
    }
    T* oldElem = _elem;  _elem = new T[_capacity >>= 1]; //容量减半
    for (int i = 0; i < _size; i++)
    {
        _elem[i] = oldElem[i]; //复制原向量内容
    }
    delete[] oldElem; //释放原空间
}

template <typename T>
bool Vector<T>::bubble(Rank lo, Rank hi) //扫描交换，返回整体是否有序
{
    bool sorted = true;//整体默认为有序
    while (++lo < hi)//自左向右，逐一检查相邻元素
    {
        if (_elem[lo - 1] > _elem[lo])
        {
            sorted = false;//如果逆序，则意味着未整体有序，需交换
            swap(_elem[lo - 1], _elem[lo]);
        }
    }
    return sorted;//返回有序标志
}

template <typename T>
void Vector<T>::bubbleSort(Rank lo, Rank hi) //起泡排序算法
{
    while (!bubble(lo, hi--));//每次做扫描循环，直至全部有序,时间复杂度为O(n^(3/2))
}

template <typename T>
Rank Vector<T>::bubbleOptimization(Rank lo, Rank hi)//优化扫描交换，返回值为未有序的秩
{
    Rank last = lo;//默认未有序的秩为lo，则是全部有序
    while (++lo < hi)//自左向右，逐一检查相邻元素
    {
        if (_elem[lo - 1] > _elem[lo])
        {
            last = lo;//如果扫描到逆序，则记录当前逆序的秩
            swap(_elem[lo - 1], _elem[lo]);
        }
    }

    return last;//返回最右侧逆序的秩
}

template <typename T>
void Vector<T>::bubbleOptimizationSort(Rank lo, Rank hi)
{
    while (lo < (hi = bubbleOptimization(lo, hi)));//当前逆序的秩等于lo则说明全部有序,,时间复杂度为O(n)
}

template <typename T>
void Vector<T>::merge(Rank lo, Rank mi, Rank hi) //归并算法,总迭代次数不过O(n)
{
    T* A = _elem + lo;//向量A[0, hi - lo) = _elem[lo, hi)

    int lb = mi - lo;//前子向量B[0, lb) = _elem[lo, mi)
    T* B = new T[lb];
    for (Rank i = 0; i < lb; B[i] = A[i++]);//复制前子向量B

    int lc = hi - mi;//后子向量C[0, lc) = _elem[mi, hi)
    T* C = _elem + mi;

    for (Rank i = 0, j = 0, k = 0; (j < lb) || (k < lc);)//遍历完成退出循环
    {
        if ((j < lb) && (lc <= k || B[j] <= C[k]))//lc是哨兵，C[lc]可以看成正无穷
        {
            A[i++] = B[j++];//B[j] <= C[k]表示出现雷同元素，左侧子项优先，可以保持稳定
        }
        if ((k < lc) && (lb <= j || C[k] < B[j]))//lb是哨兵，B[lb]可以看成正无穷
        {
            A[i++] = C[k++];
        }
    }

    delete[] B;
}

template <typename T>
void Vector<T>::mergeSort(Rank lo, Rank hi)//归并排序算法,时间复杂度为O(n*logn)
{
    if (hi - lo < 2) { return; }
    int mi = (lo + hi) >> 1;

    mergeSort(lo, mi);//对前半段排序
    mergeSort(mi, hi);//对后半段排序
    merge(lo, mi, hi);//归并
}

template <typename T>
Rank Vector<T>::find(T const& e, Rank lo, Rank hi) const //无序向量区间查找
{
    while ((lo < hi--) && (e != _elem[hi]));//从后向前，顺序查找
    return hi;//若hi < lo，则意味着失败；否则hi即命中元素的秩
}

template <typename T>
Rank Vector<T>::search(T const& e) const //有序向量整体查找
{
    return (0 >= _size) ? -1 : search(e, 0, _size);
}

template <typename T>
T& Vector<T>::operator[] (Rank r) //重载下标操作符，可以类似于数组形式引用各元素
{
    assert(r >= 0 || r <= _size);
    return _elem[r];
}

template <typename T>
const T& Vector<T>::operator[] (Rank r) const //仅限于做右值的重载版本
{
    assert(r >= 0 || r <= _size);
    return _elem[r];
}


//不基于remove(r)接口，实现remove(lo, hi),是因为会导致后者复杂度到O(n^2)
//每次调用remove(r) = n - hi = O(n),可能实现remove(lo, hi) = O(n^2)
template <typename T>
T Vector<T>::remove(Rank r) //删除秩为r的元素
{
    T e = _elem[r];//备份被删除元素
    remove(r, r + 1);//调用区间删除算法，等效于对区间[r, r + 1)的删除
    return e;//返回被删除元素
}

template <typename T>
int Vector<T>::remove(Rank lo, Rank hi) //删除秩在区间[lo, hi)之内的元素
{
    if (lo == hi)//出于效率考虑，单独处理退化情况，比如remove(0, 0)
    {
        return 0;
    }

    while (hi < _size)//[hi, _size)顺次前移hi - lo个单元
    {
        _elem[lo++] = _elem[hi++];
    }
    _size = lo;//更新规模，直接丢弃尾部[lo, _size = hi)区间
    shrink();//若有必要，则缩容
    return hi - lo;//返回被删除元素的数目
}

template <typename T>
Rank Vector<T>::insert(Rank r, T const& e) //插入元素
{
    expand();//如果有必要，扩容
    for (int i = _size; i > r; --i)
    {
        _elem[i] = _elem[i - 1];
    }
    _elem[r] = e;
    ++_size;

    return r;
}

//无序元素删除，能够保证正确性，但是总的时间效率为O(n^2)
//while循环内部find + remove总的时间复杂度为O(n),while时间复杂度为O(n)
template <typename T>
int Vector<T>::deduplicate() //无序去重
{
    int oldSize = _size;//保留原始规模
    Rank i = 1;//从_elem[1]开始
    while (i < _size)//从前往后寻找_elem[i]
    {
        (find(_elem[i], 0, i) < 0)//在前缀中寻找雷同者
            ? ++i//无雷同继续考察后继
            : remove(i);//否则删除雷同者
    }

    return oldSize - _size;//返回删除元素总数
}

//有序去重时间复杂度为O(n)
template <typename T>
int Vector<T>::uniquify() //有序去重
{
    Rank i = 0, j = 0;
    while (++j < _size)//逐一扫描，直至末元素
    {
        //跳过雷同者，发现不同元素时，向前移至紧邻前者右侧
        if (_elem[i] != _elem[j])
        {
            _elem[++i] = _elem[j];//O(1)
        }
    }

    _size = ++i;//截取尾部多余元素
    shrink();
    return j - i;//删除元素个数
}

// 遍历
template <typename T>
void Vector<T>::traverse(void(*visit) (T&)) //遍历（使用函数指针，只读或局部性修改）
{
    for (int i = 0; i < _size; ++i)
    {
        visit(_elem[i]);
    }
}

template <typename T> template <typename VST>
void Vector<T>::traverse(VST& visit) //遍历（使用函数对象，可全局性修改）
{
    for (int i = 0; i < _size; ++i)
    {
        visit(_elem[i]);
    }
}