
#pragma once

#include <BST/BSTHelperFunc.hpp>
#include <Share/Rand.h>

#include <stdio.h>

template<typename T>
BinNodePosi(T)& BST<T>::search(const T& e)
{
    if (dice(2))
    {
        return searchInRecursive(this->_root, e, _hot);
    }
    else
    {
        searchInIterative(this->_root, e, _hot);
    }
}

//将关键码e插入BST树中
template<typename T>
BinNodePosi(T) BST<T>::insert(const T& e)
{
    BinNodePosi(T)& x = search(e);//查找目标
    if (!x)//原树中已有目标元素则不插入
    {
        //以_hot为父节点插入元素
        x = (e < _hot->data) ? _hot->insertAsLC(e) : _hot->insertAsRC(e);
        ++this->_size;
        this->updateHeightAbove(x);//更新全树规模
    }
    return x;//无论e是否在原树中，至此总有x->data == e
}//算法复杂度为O(h)

template<typename T>
bool BST<T>::remove(const T& e)
{
    BinNodePosi(T)& x = search(e);//定位目标节点
    if (!x)
    {
        return false;
    }
    removeBSTAt(x);
    this->_size--;//更新全树规模
    this->updateHeightAbove(_hot);//更新_hot及历代祖先高度
    return true;
}//删除结果由返回值决定,算法复杂度O(h)

template<typename T>
void BST<T>::stretch()
{

}

template<typename T>
void BST<T>::imitate(const BST<T> &)
{

}

//按照“3 + 4”结构联接3个节点及其四棵子树，返回重组之后的局部子树根节点位置（即b）
//子树根节点与上层节点之间的双向联接，均须由上层调用者完成
//传入顺序按照中序遍历顺序传入T0->a->T1->b->T2->c->T3,最终组成高度最低的树
//可用于AVL和RedBlack的局部平衡调整
template <typename T>
BinNodePosi(T) BST<T>::connect34(
    BinNodePosi(T) a, BinNodePosi(T)b, BinNodePosi(T) c,
    BinNodePosi(T) T0, BinNodePosi(T) T1,
    BinNodePosi(T) T2, BinNodePosi(T) T3)
{
    a->lc = T0;
    if (T0)
    {
        T0->parent = a;
    }
    a->rc = T1;
    if (T1)
    {
        T1->parent = a;
    }
    this->updateHeight(a);

    c->lc = T2;
    if (T2)
    {
        T2->parent = c;
    }
    c->rc = T3;
    if (T3)
    {
        T3->parent = c;
    }
    this->updateHeight(c);

    b->lc = a;
    a->parent = b;
    b->rc = c;
    c->parent = b;
    this->updateHeight(b);
    return b; //该子树新的根节点
}


//BST节点旋转变换统一算法（3节点 + 4子树），返回调整之后局部子树根节点的位置
//注意：尽管子树根会正确指向上层节点（如果存在），但反向的联接须由上层函数完成
template<typename T>
BinNodePosi(T) BST<T>::rotateAt(BinNodePosi(T) curPosi)
{
    //curPosi为非空孙辈节点
    if (!curPosi)
    {
        printf("\a\nFail to rotate a null node\n");
        exit(-1);
    }

    BinNodePosi(T) parentPosi = curPosi->parent;
    BinNodePosi(T) grandPosi = parentPosi->parent;

    //curPosi, parentPosi, grandPosi相对位置分为4种情况
    if (IsLChild(parentPosi))//zig
    {
        if (IsLChild(curPosi))//zig-zig
        {
            parentPosi->parent = grandPosi->parent;//向上联接
            return connect34(curPosi, parentPosi, grandPosi,
                curPosi->lc, curPosi->rc, parentPosi->rc, grandPosi->rc);
        }
        else//zig-zag
        {
            curPosi->parent = grandPosi->parent;
            return connect34(parentPosi, curPosi, grandPosi,
                parentPosi->lc, curPosi->lc, curPosi->rc, grandPosi->rc);
        }
    }
    else//zag
    {
        if (IsRChild(curPosi))//zag-zag
        {
            parentPosi->parent = grandPosi->parent;
            return connect34(grandPosi, parentPosi, curPosi,
                grandPosi->lc, parentPosi->lc, curPosi->lc, curPosi->rc);
        }
        else//zag-zig
        {
            curPosi->parent = grandPosi->parent;
            return connect34(grandPosi, curPosi, parentPosi,
                grandPosi->lc, curPosi->lc, curPosi->rc, parentPosi->rc);
        }
    }
}

//BST节点删除算法：删除位置x所指的节点（全局静态模板函数，适用于AVL、Splay、RedBlack等各种BST）
//目标x在此前经查找定位，并确认非NULL，故必删除成功；与searchIn不同，调用之前不必将_hot置空
//返回值和传入的x指向实际被删除节点的接替者，_hot指向实际被删除节点的父亲——二者均有可能是NULL
template<typename T>
BinNodePosi(T) BST<T>::removeBSTAt(BinNodePosi(T)& x)
{
    BinNodePosi(T) w = x;
    BinNodePosi(T) succ = NULL;

    if (!HasLChild(x))//左子树为空,直接将*x替换为右子树
    {
        succ = x->rc;
        this->fromParentTo(w) = succ;//将父节点中指向w的指针指向后继节点
        x = succ;//x赋值为后继节点
    }
    else if (!HasRChild(x))//右子树为空，直接将*x替换为左子树，此时succ != NULL
    {
        succ = x->lc;
        this->fromParentTo(w) = succ;
        x = succ;
    }
    else//左右子树都存在，则选择x的直接后继作为实际被摘除节点
    {
        w = w->succ();//(在右子树中)找到*x直接后继*w
        std::swap(x->data, w->data);//交换元素，待删除节点为w
        BinNodePosi(T) u = w->parent;//u为待删除节点的父节点
        ((u == x) ? u->rc : u->lc) = succ = w->rc;//隔离节点w
        //因为x中元素被交换，所以x已经变为后继节点，不能对x进行操作
    }

    _hot = w->parent;//记录实际被删除节点的父亲
    if (succ)
    {
        succ->parent = _hot;//将被删除节点的接替者与hot相关联
    }
    release(w->data);//释放被删除节点
    release(w);

    return succ;//返回接替者
}