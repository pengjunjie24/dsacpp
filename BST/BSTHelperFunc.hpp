
#pragma once

#include <Share/Release.hpp>

#include <algorithm>

//在以v为根的（AVL、SPLAY、rbTree等）BST子树中查找关键码e
//返回时，返回值指向命中节点（或假想的通配哨兵），hot指向其父亲（退化时为初始值NULL）
//递归版
template<typename T>
static BinNodePosi(T)& searchInRecursive(BinNodePosi(T)& v, const T& e, BinNodePosi(T)& hot)
{
    if (!v || (e == v->data))//递归基：在节点v（或假想的通配节点）处命中
    {
        return v;
    }
    hot = v;//一般情况：先记下当前节点，然后再深入一层，递归查找
    return searchInRecursive(((e < v->data) ? v->lc : v->rc), e, hot);

}//运行时间正比于返回节点v的深度，不超过树高O(h)

//迭代版
template<typename T>
static BinNodePosi(T)& searchInIterative(BinNodePosi(T)& v, const T& e, BinNodePosi(T)& hot)
{
    //退化情况：在子树根节点v处命中
    if (!v || (e == v->data))
    {
        return v;
    }
    hot = v;

    //一般地，反复不断地确定深入方向
    while (true)
    {
        BinNodePosi(T)& c = (e < hot->data) ? hot->lc : hot->rc;
        //命中返回，或者深入一层
        if (!c || (e == c->data))
        {
            return c;
        }
        hot = c;//hot始终指向最后一个失败节点
    }//返回时，返回值指向命中节点（或假想的通配哨兵），hot指向其父亲（退化时为初始值NULL）
}

//通过zig旋转调整，将BST子树x拉伸成最右侧通路
template <typename T>
void stretchByZig(BinNodePosi(T)& x)
{
    int c = 0;//记录旋转次数
    BinNodePosi(T) v = x;//v为子树根部
    while (x->lc)
    {
        x = x->lc;//x为最右侧的叶子节点
    }

    for (; v; v = v->rc)
    {
        while (v->lc)
        {
            v = v->zig();
            ++c;
        }
        v = v->rc;
    }

    printf("\nsize = %d, height = %d, #zig = %d\n\n", x->size(), x->height, c);
}

//通过zag旋转调整，将BST子树x拉伸成最左侧通路
template<typename T>
void stretchByZag(BinNodePosi(T)& x)
{

}