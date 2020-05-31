
#pragma once

//在B-树中查找关键码e
template <typename T>
BTNodePosi(T) BTree<T>::search(const T& e)
{
    BTNodePosi(T) v = _root;//从根节点出发
    _hot = NULL;
    while (v)//逐层查找
    {
        Rank r = v->key.search(e);//在当前节点中，找到不大于e的最大关键码
        if ((0 <= r) && (e == v->key[r]))
        {
            return v;//成功：在当前节点中命中目标关键码
        }

        _hot = v;//否则，转入对应子树（_hot指向其父）——需做I/O，最费时间
        v = v->child[r + 1];
    }//这里在向量内是二分查找，但对通常的_order可直接顺序查找
    return NULL;//失败：最终抵达外部节点
}

//将关键码e插入B树中
template <typename T>
bool BTree<T>::insert(const T& e)
{
    //在B-树中查找关键码e，若存在则不插入直接返回
    BTNodePosi(T) v = search(e);
    if (v)
    {
        return false;
    }
    Rank r = _hot->key.search(e);//在有序关键码向量中查找合适位置插入
    _hot->key.insert(r + 1, e);//将新节点插入
    _hot->child.insert(r + 2, NULL);//创建空的子树指针
    ++_size;//更新规模
    sloveOverflow(_hot);//如有必要，需做分裂
    return true;
}

//从BTree树中删除关键码e
template <typename T>
bool BTree<T>::remove(const T& e)
{
    BTNodePosi(T) v = search(e);
    //确认目标关键码存在
    if (!v)
    {
        return false;
    }

    Rank r = v->key.search(e);//确定目标关键码在节点v中的秩
    //若v非叶子，则e的后继必属于某叶节点
    if (v->child[0])
    {
        //在右子树中一直向左，即可找出e的后继
        BTNodePosi(T) u = v->child[r + 1];
        while (u->child[0])
        {
            u = u->child[0];
        }

        //并与之交换位置
        v->key[r] = u->key[0];
        v = u;
        r = 0;
    }//至此，v必然位于最底层，且其中第r个关键码就是待删除者

    //删除e，以及其下两个外部节点之一
    v->key.remove(r);
    v->child.remove(r + 1);
    --_size;

    sloveUnderflow(v);//如有必要，需做旋转或合并
    return true;
}

//关键码插入后若节点上溢，则做节点分裂处理
template <typename T>
void BTree<T>::sloveOverflow(BTNodePosi(T) v)
{
    //递归基：当前节点并未上溢
    if (_order >= v->child.size())
    {
        return;
    }

    Rank s = _order / 2;//轴点（此时应有_order = key.size() = child.size() - 1）
    BTNodePosi(T) u = new BTNode<T>();//新节点已有一个空孩子
    //v右侧_order-s-1个孩子及关键码分裂为右侧节点u
    for (Rank j = 0; j < _order - (s + 1); ++j)
    {
        //TODO:逐个移动效率低
        u->child.insert(j, v->child.remove(s + 1));
        u->key.insert(j, v->key.remove(s + 1));
    }
    u->child[_order - (s + 1)] = v->child.remove(s + 1);//移动v最靠右的孩子
    //如果u的孩子非空，则令它们的父节点指向u
    if (u->child[0])
    {
        for (Rank j = 0; j < _order - s; ++j)
        {
            u->child[j]->parent = u;
        }
    }

    BTNodePosi(T) p = v->parent;//v的当前父节点p
    if (!p)//若p为空，则v为根节点
    {
        _root = p = new BTNode<T>();
        p->child[0] = v;
        v->parent = p;
    }

    Rank r = 1 + p->key.search(v->key[0]);//p中指向u的指针的秩，v->key[0]是一定存在的
    p->key.insert(r, v->key.remove(s));//轴点关键码上升
    p->child.insert(r + 1, u);//新节点u与父节点p互联
    u->parent = p;

    sloveOverflow(p);//上升一层，如有必要则继续分裂——至多递归O(logn)层
}

//关键码删除后若节点下溢，则做节点旋转或合并处理
template <typename T>
void BTree<T>::sloveUnderflow(BTNodePosi(T) v)
{
    //递归基：当前节点并未下溢
    if ((_order + 1) / 2 <= v->child.size())
    {
        return;
    }

    BTNodePosi(T) p = v->parent;
    //递归基：已到根节点，没有孩子的下限
    if (!p)
    {
        //但倘若作为树根的v已不含关键码，却有（唯一的）非空孩子
        if (!v->key.size() && v->child[0])
        {
            //这个节点可被跳过,并因不再有用而被销毁
            _root = v->child[0];
            _root->parent = NULL;
            v->child[0] = NULL;
            release(v);
        }//整树高度降低一层
        return;
    }

    //确定v是p的第r个孩子——此时v可能不含关键码，故不能通过关键码查找
    Rank r = 0;
    while (p->child[r] != v)
    {
        ++r;
    }

    // 情况1：向左兄弟借关键码
    if (0 < r)//若v不是p的第一个孩子，则左兄弟必存在
    {
        BTNodePosi(T) ls = p->child[r - 1];
        //若该兄弟足够“胖”
        if ((_order + 1) / 2 < ls->child.size())
        {
            v->key.insert(0, p->key[r - 1]);//p借出一个关键码给v（作为最小关键码）
            p->key[r - 1] = ls->key.remove(ls->key.size() - 1);//ls的最大关键码转入p
            v->child.insert(0, ls->child.remove(ls->child.size() - 1));//同时ls的最右侧孩子过继给v

            //作为v的最左侧孩子
            if (v->child[0])
            {
                v->child[0]->parent = v;
            }
            //至此，通过右旋已完成当前层（以及所有层）的下溢处理
            return;
        }//至此，左兄弟要么为空，要么太“瘦”
    }

    // 情况2：向右兄弟借关键码
    if (p->child.size() - 1 > r)//若v不是p的最后一个孩子,则右兄弟必存在
    {
        BTNodePosi(T) rs = p->child[r + 1];
        //若该兄弟足够胖
        if ((_order + 1) / 2 < rs->child.size())
        {
            v->key.insert(v->key.size(), p->key[r]);//p借出一个关键码给v
            p->key[r] = rs->key.remove(0);//rs最小的关键码转入p
            //rs最左侧孩子过继给v
            v->child.insert(rs->child.remove(0));

            //作为v的最右侧孩子
            int rightMost = v->child.size() - 1;
            if (v->child[rightMost])
            {
                v->child[rightMost]->parent = v;
            }
            return;//通过左旋完成当前层(以及所有层)的下溢处理
        }
    }

    //情况3: 左右兄弟要么为不同时空，要么太瘦--合并
    if (0 < r)//与左兄弟合并
    {
        BTNodePosi(T) ls = p->child[r - 1];
        //p的第r-1个关键码转入ls, v不再是p的第r个孩子
        ls->key.insert(ls->key.size(), p->key.remove(r - 1));
        p->child.remove(r);

        //v最左侧的孩子过继给ls最右侧孩子
        ls->child.insert(ls->child.size(), v->child.remove(0));
        if (ls->child[ls->child.size() - 1])
        {
            ls->child[ls->child.size() - 1]->parent = ls;
        }

        //v剩余的关键码和孩子依次转入ls
        while (!v->key.empty())
        {
            ls->key.insert(ls->key.size(), v->key.remove(0));
            ls->child.insert(ls->child.size(), v->child.remove(0));
            if (ls->child[ls->child.size() - 1])
            {
                ls->child[ls->child.size() - 1]->parent = ls;
            }
        }
        release(v);
    }
    else
    {
        BTNodePosi(T) rs = p->child[r + 1];
        //p的第r个关键码转入rs，v不再是p的第r个孩子
        rs->key.insert(0, p->key.remove(r));
        p->child.remove(r);
        //v最左侧的孩子过继给rs
        rs->child.insert(0, v->child.remove(v->child.size() - 1));
        if (rs->child[0])
        {
            rs->child[0]->parent = rs;
        }

        //依次将v中剩余关键码和孩子转入rs中
        while (!v->key.empty())
        {
            rs->key.insert(0, v->key.remove(v->key.size() - 1));
            rs->child.insert(0, v->child.remove(v->child.size() - 1));
            if (rs->child[0])
            {
                rs->child[0]->parent = rs;
            }
        }

        release(v);
    }

    sloveUnderflow(p);//上升一层，如有必要则继续分裂——至多递归O(logn)
    return;
}