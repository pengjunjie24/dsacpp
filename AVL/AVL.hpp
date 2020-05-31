
#pragma once

#include<BinTree/BinNodeMacro.h>

/******************************************************************************************
 * 在左、右孩子中取更高者，找到失衡的通路
 * 在AVL平衡调整前，借此确定重构方案
 ******************************************************************************************/
#define tallerChild(x) ( \
   stature( (x)->lc ) > stature( (x)->rc ) ? (x)->lc : ( /*左高*/ \
   stature( (x)->lc ) < stature( (x)->rc ) ? (x)->rc : ( /*右高*/ \
   IsLChild(x) ? (x)->lc : (x)->rc /*等高(删除节点时出现)：与父亲x同侧者（zIg-zIg或zAg-zAg）优先*/ \
   ) \
   ) \
)

//基于BST实现AVL树
#include <BST/BST.hpp>
template <typename T>
class AVL : public BST<T>//由BST派生AVL树模板类
{
public:
    virtual ~AVL()
    {
    }

    BinNodePosi(T) insert(const T& e); //插入（重写）
    bool remove(const T& e); //删除（重写）
};

#include <AVL/AVLImplementation.hpp>