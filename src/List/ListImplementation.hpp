
#pragma once

template <typename T>
List<T>::~List()
{
    clear();
    delete _header;
    delete _trailer;
}

template <typename T>
T& List<T>::operator[] (Rank r) const //重载，支持循秩访问（效率低）O(r)
{
    ListNodePosi(T) p = first();//从首节点出发
    while (0 < r--)
    {
        p = p->succ;//顺数第r个节点即是目标节点
    }
    return p->data;//返回目标节点
}

template <typename T>
ListNodePosi(T) List<T>::find(T const& e, int n,
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

template <typename T>
ListNodePosi(T) List<T>::search(T const& e, int n, ListNodePosi(T) p) const //有序区间查找,
//在有序列表内节点p的n个(真)前驱中，找到不大于e的最后者，O(n),时间复杂度与无序一致
{
    while (0 <= n--)//对于p的最近几个n前驱，从右向左
    {
        if (((p = p->pred)->data) <= e)//一个个比较
        {
            break;
        }
    }
    return p;//直至命中，数值越界或者范围越界后，返回查找终止位置
}

template <typename T>
ListNodePosi(T) List<T>::selectMax(ListNodePosi(T) p, int n) //在p及其n-1个后继中选出最大者
{
    ListNodePosi(T) pMax = p;//暂定最大元素为首节点p
    ListNodePosi(T) pCur = p;//当前元素为p
    for (int i = 1; i < n; ++i)
    {
        pMax = ((pMax->data) < ((pCur = pCur->succ)->data)) ?
            pCur : pMax;//当前最大元素小于当前元素，更新当前最大元素
            //对多个相同的pMax做处理，选取最靠后的的pMax
    }

    return pMax;//返回最大节点位置
}

template <typename T>
ListNodePosi(T) List<T>::insertA(ListNodePosi(T) p, T const& e) //将e当作p的后继插入（After）
{
    _size++;
    return p->insertAsSucc(e);
}

template <typename T>
ListNodePosi(T) List<T>::insertB(ListNodePosi(T) p, T const& e) //将e当作p的前驱插入（Before）
{
    _size++;
    return p->insertAsPred(e);
}

template <typename T>
T List<T>::remove(ListNodePosi(T) p) //删除合法位置p处的节点,返回被删除节点
{
    T e = p->data;//备份待删除节点数值
    p->pred->succ = p->succ;
    p->succ->pred = p->pred;
    delete p;
    _size--;
    return e;//返回备份数据
}

template <typename T>
int List<T>::deduplicate() //无序去重, O(n^2)
{
    if (_size < 2)//规模小于2不可能有重复元素
    {
        return 0;
    }
    int oldSize = _size;
    ListNodePosi(T) p = first();
    Rank r = 1;//从首节点起
    while (_trailer != (p = p->succ))//O(n)
    {
        ListNodePosi(T) q = find(p->data, r, p);//O(n)
        q ? remove(q) : ++r;//删除q不删除p的原因:防止p指针失效
    }
    return oldSize - _size;//被删除元素总数
}

template <typename T>
int List<T>::uniquify() //有序去重，O(n)
{
    if (_size < 2)//平凡列表自然无重复
    {
        return 0;
    }

    int oldSize = _size;//记录原规模
    ListNodePosi(T) p = first();
    ListNodePosi(T) q;

    while (_trailer != (q = p->succ))//没有到尾哨兵节点，O(n)
    {
        if (p->data != q->data)//反复考察相邻节点，互异转向下一个节点
        {
            p = q;
        }
        else
        {
            remove(q);//雷同则删除后者,O(1)
        }
    }

    return oldSize - _size;//被删除元素总数
}

template <typename T>
void List<T>::init()//列表创建时初始化
{
    _header = new ListNode<T>;//创建头哨兵节点
    _trailer = new ListNode<T>;//创建尾哨兵节点

    _header->succ = _trailer;
    _header->pred = NULL;//互联

    _trailer->pred = _header;
    _trailer->succ = NULL;//互联

    _size = 0;
}

template <typename T>
int List<T>::clear()//清除所有节点,O(n)
{
    int oldSize = _size;
    while (0 < _size)
    {
        remove(_header->succ);
    }

    return oldSize;
}

template <typename T>
void List<T>::copyNodes(ListNodePosi(T) p, int n)//复制列表中自位置p起的n项
{
    init();//创建首尾哨兵节点并且初始化
    while (n--)//将起自p的n项依次作为末节点插入
    {
        insertAsLast(p->data);
        p = p->succ;
    }
}

template <typename T>
void List<T>::selectionSort(ListNodePosi(T) p, int n)//对从p开始连续的n个节点选择排序,最好和最坏的情况都是O(n^2)
{
    ListNodePosi(T) head = p->pred;
    ListNodePosi(T) tail = p;
    for (int i = 0; i < n; ++i)//head和tail是头/尾哨兵节点
    {
        tail = tail->succ;
    }

    while (1 < n)//从待排区间找出最大者，并移至有序区间前端
    {
        //new和delete操作是常规操作时间的100倍左右,下面这个步骤效率很低
        //insertB(tail, remove(selectMax(head->succ, n)));//将当前最大元素插入尾哨兵之前

        //交换数据域即可
        ListNodePosi(T) pCurMax = selectMax(head->succ, n);//选择出最靠后的pMax,插入到尾部，保证了算法稳定性,O(n)
        ListNodePosi(T) pLastNode = tail->pred;
        T tmpData = pCurMax->data;
        pCurMax->data = pLastNode->data;
        pLastNode->data = tmpData;

        tail = tail->pred;//待排区间和有序区间范围不断更新
        --n;
    }
}

//insertSort可以用逆序对来衡量性能，它是输入敏感的排序
template <typename T>
void List<T>::insertionSort(ListNodePosi(T) p, int n)//对从p开始连续的n个节点插入排序
{
    for (int r = 0; r < n; ++r)//n次迭代,每次O(r+1)
    {
        insertA(search(p->data, r, p), p->data);//查询+插入
        p = p->succ;//转向下一个节点
        remove(p->pred);
    }
}//仅用O(1)的辅助空间，属于就地算法