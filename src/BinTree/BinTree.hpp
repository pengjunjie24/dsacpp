
#pragma once

#include <BinTree/BinNode.hpp> //引入二叉树节点类

//二叉树模板类
template <typename T>
class BinTree
{
protected:
    int _size; //规模
    BinNodePosi(T) _root; //根节点

    virtual int updateHeight(BinNodePosi(T) x); //更新节点x的高度,具体规则因树不同而异
    void updateHeightAbove(BinNodePosi(T) x); //更新节点x及其祖先的高度

public:
    BinTree(); //构造函数
    ~BinTree(); //析构函数
    int size() const { return _size; } //规模
    bool empty() const { return !_root; } //判空
    BinNodePosi(T) root() const { return _root; } //树根
    BinNodePosi(T) insertAsRoot(T const& e); //插入根节点
    BinNodePosi(T) insertAsLC(BinNodePosi(T) x, T const& e); //e作为x的左孩子（原无）插入
    BinNodePosi(T) insertAsRC(BinNodePosi(T) x, T const& e); //e作为x的右孩子（原无）插入
    BinNodePosi(T) attachAsLC(BinNodePosi(T) x, BinTree<T>* &S); //S作为x左子树接入
    BinNodePosi(T) attachAsRC(BinNodePosi(T) x, BinTree<T>* &S); //S作为x右子树接入
    int remove(BinNodePosi(T) x); //删除以位置x处节点为根的子树，返回该子树原先的规模
    BinTree<T>* secede(BinNodePosi(T) x); //将子树x从当前树中摘除，并将其转换为一棵独立子树
    template <typename VST>
    void travLevel(VST visit); //层次遍历
    template < typename VST>
    void travPre(VST visit); //先序遍历
    template < typename VST>
    void travIn(VST visit); //中序遍历
    template < typename VST>
    void travPost(VST visit); //后序遍历
    bool operator< (BinTree<T> const& t) //比较器（其余自行补充）
    { return _root && t._root && lt(_root, t._root); }
    bool operator== (BinTree<T> const& t) //判等器
    { return _root && t._root && (_root == t._root); }

};

#include <BinTree/BinTreeImplementation.hpp>