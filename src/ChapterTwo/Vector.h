
#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <assert.h>

typedef int Rank; //秩
#define DEFAULT_CAPACITY  3 //默认的初始容量（实际应用中可设置为更大）

template <typename T>
class Vector//向量模板类
{
protected:
    Rank _size;
    int _capacity;
    T* _elem; //规模、容量、数据区

    void copyFrom(T const* A, Rank lo, Rank hi) //复制数组区间A[lo, hi)
    {
        _elem = new T[_capacity = 2 * (hi - lo)];
        _size = 0;
        while (lo < hi)
        {
            _elem[_size++] = A[lo++];
        }
    }

    void expand() //空间不足时扩容,加倍扩容分摊时间成本为O(1)
    {
        if (_size < _capacity)
        {
            return;
        }

        _capacity = max(_capacity, DEFAULT_CAPACITY);
        T* oldElem = _elem;
        _elem = new T[_capacity <<= 1];
        for (int i = 0; i < _size; ++i)
        {
            _elem[i] = oldElem[i];
        }
        delete[] oldElem;
    }

    void shrink() //装填因子过小时压缩
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

    Rank bubbleOptimization(Rank lo, Rank hi)//优化扫描交换，返回值为未有序的秩
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

    void bubbleOptimizationSort(Rank lo, Rank hi)
    {
        while (lo < (hi = bubbleOptimization(lo, hi)));//当前逆序的秩等于lo则说明全部有序,,时间复杂度为O(n)
    }

    bool bubble(Rank lo, Rank hi) //扫描交换，返回整体是否有序
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

    void bubbleSort(Rank lo, Rank hi) //起泡排序算法
    {
        while (!bubble(lo, hi--));//每次做扫描循环，直至全部有序,时间复杂度为O(n^(3/2))
    }

    Rank max(Rank lo, Rank hi) { return (lo < hi) ? hi : lo; } //选取最大元素
    void selectionSort(Rank lo, Rank hi); //选择排序算法

    void merge(Rank lo, Rank mi, Rank hi) //归并算法,总迭代次数不过O(n)
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

    void mergeSort(Rank lo, Rank hi)//归并排序算法,时间复杂度为O(n*logn)
    {
        if (hi - lo < 2){ return; }
        int mi = (lo + hi) >> 1;

        mergeSort(lo, mi);//对前半段排序
        mergeSort(mi, hi);//对后半段排序
        merge(lo, mi, hi);//归并
    }
    Rank partition(Rank lo, Rank hi); //轴点构造算法
    void quickSort(Rank lo, Rank hi); //快速排序算法
    void heapSort(Rank lo, Rank hi); //堆排序（稍后结合完全堆讲解）

public:
    // 构造函数
    Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0) //容量为c、规模为s、所有元素初始为v
    {
        _elem = new T[_capacity = c];
        for (_size = 0; _size < s; _elem[_size++] = v);
    } //s<=c

    Vector(T const* A, Rank n) { copyFrom(A, 0, n); } //数组整体复制
    Vector(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); } //区间
    Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); } //向量整体复制
    Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); } //区间

    // 析构函数
    ~Vector() { delete[] _elem; } //释放内部空间

    // 只读访问接口
    Rank size() const { return _size; } //规模
    bool empty() const { return !_size; } //判空
    int disordered() const; //判断向量是否已排序
    Rank find(T const& e) const { return find(e, 0, _size); } //无序向量整体查找
    Rank find(T const& e, Rank lo, Rank hi) const //无序向量区间查找
    {
        while ((lo < hi--) && (e != _elem[hi]));//从后向前，顺序查找
        return hi;//若hi < lo，则意味着失败；否则hi即命中元素的秩
    }
    Rank search(T const& e) const //有序向量整体查找
    {
        return (0 >= _size) ? -1 : search(e, 0, _size);
    }
    Rank search(T const& e, Rank lo, Rank hi) const; //有序向量区间查找

    // 可写访问接口
    T& operator[] (Rank r) //重载下标操作符，可以类似于数组形式引用各元素
    {
        assert(r >= 0 || r <= _size);
        return _elem[r];
    }
    const T& operator[] (Rank r) const //仅限于做右值的重载版本
    {
        assert(r >= 0 || r <= _size);
        return _elem[r];
    }
    Vector<T> & operator= (Vector<T> const&); //重载赋值操作符，以便直接克隆向量

    //不基于remove(r)接口，实现remove(lo, hi),是因为会导致后者复杂度到O(n^2)
    //每次调用remove(r) = n - hi = O(n),可能实现remove(lo, hi) = O(n^2)
    T remove(Rank r) //删除秩为r的元素
    {
        T e = _elem[r];//备份被删除元素
        remove(r, r + 1);//调用区间删除算法，等效于对区间[r, r + 1)的删除
        return e;//返回被删除元素
    }
    int remove(Rank lo, Rank hi) //删除秩在区间[lo, hi)之内的元素
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

    Rank insert(Rank r, T const& e) //插入元素
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
    Rank insert(T const& e) { return insert(_size, e); } //默认作为末元素插入
    void sort(Rank lo, Rank hi); //对[lo, hi)排序
    void sort() { sort(0, _size); } //整体排序
    void unsort(Rank lo, Rank hi); //对[lo, hi)置乱
    void unsort() { unsort(0, _size); } //整体置乱

    //无序元素删除，能够保证正确性，但是总的时间效率为O(n^2)
    //while循环内部find + remove总的时间复杂度为O(n),while时间复杂度为O(n)
    int deduplicate() //无序去重
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
    int uniquify() //有序去重
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
    void traverse(void(*visit) (T&)) //遍历（使用函数指针，只读或局部性修改）
    {
        for (int i = 0; i < _size; ++i)
        {
            visit(_elem[i]);
        }
    }
    template <typename VST>
    void traverse(VST& visit) //遍历（使用函数对象，可全局性修改）
    {
        for (int i = 0; i < _size; ++i)
        {
            visit(_elem[i]);
        }
    }

    //线性递归: T(n) = T(n/2) + O(1) = O(logn)
    //递归跟踪: 轴点总取中点，递归深度O(logn),各递归实例均耗时O(1)
    Rank binSearch(T const& e, Rank low, Rank high)// 二分查找算法（版本A）：
    //在有序向量的区间[lo, hi)内查找元素e，0 <= lo <= hi <= _size
    {
        while (low < high)
        {
            Rank mid = (low + high) >> 1;//每步迭代可能要做两次比较判断，有三个分支
            if (e < _elem[mid])//以中点为轴点（区间宽度的折半，等效于宽度之数值表示的右移）
            {
                high = mid;//深入前半段[lo, mid)继续查找
                binSearch(e, low, high);
            }
            else if (_elem[mid] < e)
            {
                low = mid + 1;//深入后半段(mid, hi)继续查找
                binSearch(e, low, high);
            }
            else
            {
                return mid;//在mi处命中
            }
        }
        return -1;//查找失败
        //有多个命中元素时，不能保证返回秩最大者；查找失败时，简单地返回-1，而不能指示失败的位置
    }

}; //Vector



#endif