
#pragma once

#include <BinTree/BinNodeTraverse.hpp>
#include <BinTree/BinNodeMacro.h>
#include <Stack/Stack.hpp>
#include <Queue/Queue.hpp>

#include<stdlib.h>

//构造函数
template <typename T>
BinNode<T>::BinNode()
    : parent(NULL)
    , lc(NULL)
    , rc(NULL)
    , height(0)
    , npl(1)
    , color(RB_RED)
{
}

template <typename T>
BinNode<T>::BinNode(T e, BinNodePosi(T) p, BinNodePosi(T) lc,
    BinNodePosi(T) rc, int h, int l, RBColor c)
    : data(e)
    , parent(p)
    , lc(lc)
    , rc(rc)
    , height(h)
    , npl(l)
    , color(c)
{
}

template <typename T>
int BinNode<T>::size() //统计当前节点后代总数，亦即以其为根的子树的规模
{
    int s = 1;//计入本身
    if (lc)
    {
        s += lc->size();//递归计入左子树规模
    }
    if (rc)
    {
        s += rc->size();//递归计入右子树规模
    }

    return s;
}//O(n = |size|)

template <typename T>
BinNodePosi(T) BinNode<T>::insertAsLC(T const& e) //作为当前节点的左孩子插入新节点
{
    assert(!lc);
    return lc = new BinNode(e, this);
}

template <typename T>
BinNodePosi(T) BinNode<T>::insertAsRC(T const& e) //作为当前节点的右孩子插入新节点
{
    assert(!rc);
    return rc = new BinNode(e, this);
}

//若右子树不为空，则Suc=右子树最左的叶子节点；
//若右子树为空，且拥有右父亲节点，则Suc = 右父亲节点；
//若右子树为空，且拥有左父亲节点，则Suc = 最近的右祖先节点；
template <typename T>
BinNodePosi(T) BinNode<T>::succ() //取当前节点的直接后继(中序遍历)
{
    //定位节点v的直接后继
    BinNodePosi(T) s = this; //记录后继的临时变量
    if (rc)
    {
        //若有右孩子，则直接后继必在右子树中，
        //具体地就是右子树中最靠左（最小）的节点
        s = rc;
        while (HasLChild(s))
        {
            s = s->lc;
        }
    }
    else
    {
        //否则，直接后继应是“将当前节点包含于其左子树中的最低祖先”，
        //具体地就是逆向地沿右向分支，
        //不断朝左上方移动最后再朝右上方移动一步，即抵达直接后继（如果存在）
        while (IsRChild(s))
        {
            s = s->parent;
        }
        s = s->parent;
    }
    return s;
}

template <typename T> template <typename VST>
void BinNode<T>::travLevel(VST& visit) //子树层次遍历
{
    Queue<BinNodePosi(T)> Q; //辅助队列
    Q.enqueue(this); //根节点入队
    while (!Q.empty())//在队列再次变空之前，反复迭代
    {
        BinNodePosi(T) x = Q.dequeue(); //取出队首节点并访问之
        visit(x->data);
        if (HasLChild(x))//左孩子入队
        {
            Q.enqueue(x->lc);
        }
        if (HasRChild(x))//右孩子入队
        {
            Q.enqueue(x->rc);
        }
    }
}

template <typename T> template <typename VST>
void BinNode<T>::travPre(VST& visit) //子树先序遍历
{
    switch (rand() % 3) //此处暂随机选择以做测试，共三种选择
    {
    case 1: //迭代版#1
        travPreI1(this, visit);
        break;
    case 2: //迭代版#2
        travPreI2(this, visit);
        break;
    default: //递归版
        travPreR(this, visit);
        break;
    }
}

template <typename T> template <typename VST>
void BinNode<T>::travIn(VST& visit) //子树中序遍历
{
    switch (rand() % 5) //此处暂随机选择以做测试，共五种选择
    {
    case 1: //迭代版#1
        travInI1(this, visit);
        break;
    case 2: //迭代版#2
        travInI2(this, visit);
        break;
    case 3: //迭代版#3
        travInI3(this, visit);
        break;
    case 4: //迭代版#4
        travInI4(this, visit);
        break;
    default: //递归版
        travInR(this, visit);
        break;
    }
}

template <typename T> template <typename VST>
void BinNode<T>::travPost(VST& visit) //子树后序遍历
{
    switch (rand() % 2) //此处暂随机选择以做测试，共两种选择
    {
    case 1: //迭代版
        travPostI(this, visit);
        break;
    default: //递归版
        travPostR(this, visit);
        break;
    }
}

template <typename T>
BinNodePosi(T) BinNode<T>::zig()
{

}

template <typename T>
BinNodePosi(T) BinNode<T>::zag()
{

}