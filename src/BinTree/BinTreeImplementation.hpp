
#pragma once

#include <Share/Release.hpp>

#include <algorithm>

template <typename T>
int BinTree<T>::updateHeight(BinNodePosi(T) x) //更新节点x的高度,具体规则因树不同而异
{
    return x->height = 1 + std::max(stature(x->lc), stature(x->rc));
}//常规二叉树规则，O(1)

template < typename T>
void BinTree<T>::updateHeightAbove(BinNodePosi(T) x) //更新节点x及其祖先的高度
{
    while (x)//TODO:高度不变时
    {
        updateHeight(x);
        x = x->parent;
    }
}//O(n = depth(x))

template < typename T>
BinTree<T>::BinTree() //构造函数
    : _size(0)
    , _root(NULL)
{
}

template < typename T>
BinTree<T>::~BinTree()  //析构函数
{
    if (0 < _size)
    {
        remove(_root);
    }
}

template < typename T>
BinNodePosi(T) BinTree<T>::insertAsRoot(T const& e) //插入根节点
{
    assert(!_root);
    _size = 1;
    return _root = new BinNode<T>(e);
}

template < typename T>
BinNodePosi(T) BinTree<T>::insertAsLC(BinNodePosi(T) x, T const& e) //e作为x的左孩子（原无）插入
{
    assert(!x->lc);
    _size++;
    x->insertAsLC(e);
    updateHeightAbove(x);//祖先高度可能增加，其余节点必然不变
    return x->lc;
}

template < typename T>
BinNodePosi(T) BinTree<T>::insertAsRC(BinNodePosi(T) x, T const& e) //e作为x的右孩子（原无）插入
{
    assert(!x->rc);
    _size++;
    x->insertAsRC(e);
    updateHeightAbove(x);//祖先高度可能增加，其余节点必然不变
    return x->rc;
}

template < typename T>
BinNodePosi(T) BinTree<T>::attachAsLC(BinNodePosi(T) x, BinTree<T>* &S) //左子树接入算法: 将S当作节点x的左子树接入，S本身置空
{
    assert(!x->lc);
    x->lc = S->_root;//接入
    x->lc->parent = x;
    _size += S->_size;//更新全树规模与x所有祖先的高度
    updateHeightAbove(x);
    S->_root = NULL;//释放原树，返回接入位置
    S->_size = 0;
    release(S);
    S = NULL;
    return x;
}

template < typename T>
BinNodePosi(T) BinTree<T>::attachAsRC(BinNodePosi(T) x, BinTree<T>* &S) //S作为x右子树接入
{
    assert(!x->rc);
    x->rc = S->_root;//接入
    x->rc->parent = x;
    _size += S->_size;//更新全树规模与x所有祖先的高度
    updateHeightAbove(x);
    S->_root = NULL;//释放原树，返回接入位置
    S->_size = 0;
    release(S);
    S = NULL;
    return x;
}

template <typename T>
int BinTree<T>::remove(BinNodePosi(T) x)
{
    FromParentTo(*x) = NULL;
    updateHeightAbove(x->parent);
    int n = removeAt(x);
    _size -= n;
    return n;
}

template <typename T>
static int removeAt(BinNodePosi(T) x)
{
    if (!x)//递归基，空树
    {
        return 0;
    }
    int n = 1 + removeAt(x->lc) + removeAt(x->rc);//递归释放左右子树
    release(x->data);//释放被摘除节点
    release(x);
    return n;//返回删除节点总数
}

template <typename T>//二叉树子树分离算法：将子树x从当前树中摘除，将其封装为一棵独立子树返回
BinTree<T>* BinTree<T>::secede(BinNodePosi(T) x)
{
    FromParentTo(*x) = NULL; //切断来自父节点的指针
    updateHeightAbove(x->parent); //更新原树中所有祖先的高度
    BinTree<T>* S = new BinTree<T>; //新树以x为根
    S->_root = x;
    x->parent = NULL;
    S->_size = x->size(); //更新规模，返回分离出来的子树
    _size -= S->_size;
    return S;
}

template < typename T> template <typename VST>
void BinTree<T>::travLevel(VST visit) //层次遍历
{
    if (_root)
    {
        _root->travLevel(visit);
    }
}

template < typename T> template < typename VST>
void BinTree<T>::travPre(VST visit) //先序遍历
{
    if (_root)
    {
        _root->travPre(visit);
    }
}

template < typename T> template < typename VST>
void BinTree<T>::travIn(VST visit) //中序遍历
{
    if (_root)
    {
        _root->travIn(visit);
    }
}

template < typename T> template < typename VST>
void BinTree<T>::travPost(VST visit) //后序遍历
{
    if (_root)
    {
        _root->travPost(visit);
    }
}
