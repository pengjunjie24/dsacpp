
#pragma once

#include <Vector/Vector.hpp>

#include <stddef.h>

#define BTNodePosi(T) BTNode<T>* //B树节点位置

template <typename T>
struct BTNode
{
    BTNodePosi(T) parent; //父节点
    Vector<T> key; //关键码向量
    Vector<BTNodePosi(T)> child; //孩子向量(长度比key多1)

    // 构造函数（注意：BTNode只能作为根节点创建，而且初始时有0个关键码和1个空孩子指针）
    BTNode()
        :parent(NULL)
    {
        child.insert(0, NULL);
    }

    BTNode(T e, BTNodePosi(T) lc = NULL, BTNodePosi(T) rc = NULL)
        :parent(NULL)//作为根节点
    {
        key.insert(0, e);//初始时只有一个关键码
        child.insert(0, lc);//两个孩子
        child.insert(1, rc);

        if (lc)
        {
            lc->parent = this;
        }
        if (rc)
        {
            rc->parent = this;
        }
    }
};