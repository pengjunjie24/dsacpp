
#pragma once

#include <BTree/BTNode.hpp> //引入B-树节点类

template <typename T>
class BTree
{
public:
    BTree(int order = 3)
        :_order(order)
        , _size(0)
    {
        _root = new BTNode<T>();
    }

    ~BTree()
    {
        if (_root)
        {
            release(_root);
        }
    }

    //阶次
    const int order()
    {
        return _order;
    }

    //规模
    const int size()
    {
        return _size;
    }

    //树根
    BTNodePosi(T)& root()
    {
        return _root;
    }

    //判空
    bool empty() const
    {
        return _root;
    }

    BTNodePosi(T) search(const T& e);//查找
    bool insert(const T& e);//插入
    bool remove(const T& e);//删除

protected:
    void sloveOverflow(BTNodePosi(T));//因插入而上溢之后的分裂处理
    void sloveUnderflow(BTNodePosi(T));//因删除而下溢之后的合并处理

    int _order;//B-树阶次，至少为3。创建时指定，一般不能修改
    int _size;//存放关键码总数
    BTNodePosi(T) _root;//根节点
    BTNodePosi(T) _hot;//BTree::search()最后访问的非空（除非树空）的节点位置
};

#include <BTree/BTreeImplementation.hpp>