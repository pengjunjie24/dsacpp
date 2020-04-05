
#pragma once
#include <BST/BST.hpp>

//基于BST实现的伸展树模板
template <typename T>
class Splay : public BST<T>
{
public:
    BinNodePosi(T) & search(const T& e); //查找（重写）,动态操作
    BinNodePosi(T) insert(const T& e); //插入（重写）
    bool remove(const T& e); //删除（重写）

protected:
    BinNodePosi(T) splay(BinNodePosi(T) v); //将节点v伸展至根
};

#include <Splay/SplayImplentation.hpp>