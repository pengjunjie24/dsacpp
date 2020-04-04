
#pragma once

#include <BinTree/BinTree.hpp>

//二叉搜索树
template <typename T>
class BST : public BinTree<T>
{
public:
    BST()
        :_hot(NULL)
    {
    }

    virtual ~BST()
    {
    }

    //基本接口：以virtual修饰，强制要求所有派生类（BST变种）根据各自的规则对其重写
    virtual BinNodePosi(T) & search(const T& e); //查找
    virtual BinNodePosi(T) insert(const T& e); //插入
    virtual bool remove(const T& e); //删除

    void stretchToLPath() { stretchByZag(this->_root); } //借助zag旋转，转化为左向单链
    void stretchToRPath() { stretchByZig(this->_root); } //借助zig旋转，转化为右向单链
    void stretch();
    void imitate(const BST<T> &); //临摹

protected:
    //按照“3 + 4”结构，联接3个节点及四棵子树
    BinNodePosi(T) connect34(BinNodePosi(T), BinNodePosi(T), BinNodePosi(T),
        BinNodePosi(T), BinNodePosi(T), BinNodePosi(T), BinNodePosi(T));
    //对x及其父亲、祖父做统一旋转调整
    BinNodePosi(T) rotateAt(BinNodePosi(T) x);
    //BST节点删除算法：删除位置x所指的节点
    BinNodePosi(T) removeBSTAt(BinNodePosi(T)& x);

    BinNodePosi(T) _hot;//“命中”节点的父亲
};

#include <BST/BSTImplementation.hpp>