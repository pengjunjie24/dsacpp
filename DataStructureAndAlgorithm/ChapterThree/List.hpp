

#ifndef LIST_HPP
#define LIST_HPP

#include "ListNode.hpp"

template<typename T>
class List
{
public:
    //构造函数
    List(){ init(); }//默认
    List(List<T> const& L);//整体复制列表L
    List(List<T> const& L, Rank r, int n);//复制列表L中自第r项起的第n项
    List(ListNodePosi(T)p, int n);//复制列表中自位置p起的第n项

    //析构函数
    ~List()
    {
        clear();
        delete _header;
        delete _trailer;
    }

    // 只读访问接口
    Rank size() const { return _size; } //规模
    bool empty() const { return _size <= 0; } //判空

    T& operator[] (Rank r) const //重载，支持循秩访问（效率低）O(r)
    {
        ListNodePosi(T) p = first();//从首节点出发
        while (0 < r--)
        {
            p = p->succ;//顺数第r个节点即是目标节点
        }
        return p->data;//返回目标节点
    }

    ListNodePosi(T) first() const { return _header->succ; } //首节点位置
    ListNodePosi(T) last() const { return _trailer->pred; } //末节点位置
    bool valid(ListNodePosi(T) p) //判断位置p是否对外合法
    {
        return p && (_trailer != p) && (_header != p);
    } //将头、尾节点等同于NULL
    int disordered() const; //判断列表是否已排序
    ListNodePosi(T) find(T const& e) const //无序列表查找
    {
        return find(e, _size, _trailer);
    }

    ListNodePosi(T) find(T const& e, int n,
        ListNodePosi(T) p) const //无序区间查找，从节点p的n个前驱中找到等于e的最后者,0(n)
    {
        while (0 < n--)//从右向左，逐个将p的前驱与e比较
        {
            if (e == (p = p->pred)->data)
            {
                return p;//直到命中返回
            }
        }

        return NULL;//越界返回null
    }

    ListNodePosi(T) search(T const& e) const //有序列表查找
    {
        return search(e, _size, trailer);
    }
    ListNodePosi(T) search(T const& e, int n, ListNodePosi(T) p) const; //有序区间查找
    ListNodePosi(T) selectMax(ListNodePosi(T) p, int n); //在p及其n-1个后继中选出最大者
    ListNodePosi(T) selectMax() { return selectMax(header->succ, _size);} //整体最大者

    // 可写访问接口
    ListNodePosi(T) insertAsFirst(T const& e) //将e当作首节点插入
    {
        insertA(first(), e);//当作首哨兵的后驱插入
    }

    ListNodePosi(T) insertAsLast(T const& e) //将e当作末节点插入
    {
        insertB(last(), e);//当作尾哨兵的前驱插入
    }

    ListNodePosi(T) insertA(ListNodePosi(T) p, T const& e) //将e当作p的后继插入（After）
    {
        _size++;
        return p->insertAsSucc(e);
    }

    ListNodePosi(T) insertB(ListNodePosi(T) p, T const& e) //将e当作p的前驱插入（Before）
    {
        _size++;
        return p->insertAsPred(e);
    }

    T remove(ListNodePosi(T) p) //删除合法位置p处的节点,返回被删除节点
    {
        T e = p->data;//备份待删除节点数值
        p->pred->succ = p->succ;
        p->succ->pred = p->pred;
        delete p;
        _size--;
        return e;//返回备份数据
    }

    void merge(List<T>& L) { merge(first(), size, L, L.first(), L._size); } //全列表归并
    void sort(ListNodePosi(T) p, int n); //列表区间排序
    void sort() { sort(first(), _size); } //列表整体排序

    int deduplicate() //无序去重
    {
        if (_size < 2)//规模小于2不可能有重复元素
        {
            return 0;
        }
        int oldSize = _size;
        ListNodePosi(T) p = first();
        Rank r = 1;//从首节点起
        while (_trailer != (p = p->succ))
        {
            ListNodePosi(T) q = find(p->data, r, p);
            q ? remove(q) : ++r;//删除q不删除p的原因:防止p指针失效
        }
        return oldSize - _size;//被删除元素总数
    }
    int uniquify(); //有序去重
    void reverse(); //前后倒置（习题）

    // 遍历
    void traverse(void(*) (T&)); //遍历，依次实施visit操作（函数指针，只读或局部性修改）
    template <typename VST> //操作器
    void traverse(VST&); //遍历，依次实施visit操作（函数对象，可全局性修改）

protected:
    void init()//列表创建时初始化
    {
        _header = new ListNode<T>;//创建头哨兵节点
        _trailer = new ListNode<T>;//创建尾哨兵节点

        _header->succ = _trailer;
        _header->pred = NULL;//互联

        _trailer->pred = _header;
        _trailer->succ = NULL;//互联

        _size = 0;
    }

    int clear()//清除所有节点,O(n)
    {
        int oldSize = _size;
        while (0 < _size)
        {
            remove(_header->succ);
        }

        return oldSize;
    }

    void copyNodes(ListNodePosi(T) p, int n)//复制列表中自位置p起的n项
    {
        init();//创建首尾哨兵节点并且初始化
        while (n--)//将起自p的n项依次作为末节点插入
        {
            insertAsLast(p->data);
            p = p->succ;
        }
    }
    void merge(ListNodePosi(T)&, int, List<T>&, ListNodePosi(T), int);//归并
    void mergeSort(ListNodePosi(T)& p, int n);//对从p开始连续的n个节点归并排序
    void selectionSort(ListNodePosi(T) p, int n);//对从p开始连续的n个节点选择排序
    void insertionSort(ListNodePosi(T) p, int n);//对从p开始连续的n个节点插入排序

private:
    int _size;//大小
    ListNodePosi(T) _header;//头哨兵
    ListNodePosi(T) _trailer;//尾哨兵
};

#endif