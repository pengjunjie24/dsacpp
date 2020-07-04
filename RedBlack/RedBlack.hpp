
#pragma once

#include <BST/BST.hpp>

//基于BST实现的红黑树
template <typename T>
class RedBlack : public BST<T>
{
public:
    BinNodePosi(T) insert(const T& e);//插入(重写)
    bool remove(const T& e);//删除(重新)

protected:
    void sloveDoubleRed(BinNodePosi(T) x);//双红修正
    void solveDoubleBlack(BinNodePosi(T) x);//双黑修正
    int updateHeight(BinNodePosi(T) x);//更新节点x的高度

    static bool isBlack(BinNodePosi(T) x) { return !x || RB_BLACK == x->color; }
    static bool isRed(BinNodePosi(T) x) { return !isBlack(x); }
    static bool blackHeightUpdated(BinNodePosi(T) x)//RedBlack高度更新条件
    {
        return (
            ( stature(x->lc) == stature(x->rc) ) &&
            ( x->height == (isRed(x) ? stature(x->lc) : stature(x->lc) + 1) )
            );
    }
};

#include <RedBlack/RedBlackImplementation.hpp>