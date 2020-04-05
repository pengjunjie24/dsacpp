
#pragma once

#include <assert.h>

template<typename T>
BinNodePosi(T)& Splay<T>::search(const T& e)
{
    BinNodePosi(T)p = searchInIterative(this->_root, e, this->_hot);
    this->_root = splay(p ? p : this->_hot);//将最后一个被访问的节点伸展至根
    return this->_root;
}//与其它BST不同，无论查找成功与否，_root都指向最后被访问的节点

template<typename T>
BinNodePosi(T) Splay<T>::insert(const T& e)
{
    //空树
    if (!this->_root)
    {
        ++this->_size;
        return this->_root = new BinNode<T>(e);
    }

    if (e == search(e)->data)
    {
        return this->_root;
    }

    //创建新节点
    ++this->_size;
    BinNodePosi(T) t = this->_root;
    //插入新根，以t和t->rc为左右孩子
    //当root->data < e时
    if (this->_root->data < e)
    {
        t->parent = this->_root = new BinNode<T>(e, NULL, t, t->rc);
        if (HasRChild(t))
        {
            t->rc->parent = this->_root;
            t->rc = NULL;
        }
    }
    else//插入新根，以t->lc和t为左、右孩子
    {
        t->parent = this->_root = new BinNode<T>(e, NULL, t->lc, t);
        if (HasLChild(t))
        {
            t->lc->parent = this->_root;
            t->lc = NULL;
        }
    }
    this->updateHeightAbove(t);//更新t及其祖先（实际上只有_root一个）的高度
    return this->_root;//新节点必然置于树根，无论e是否存在于原树中，返回时总有_root->data == e
}

template<typename T>
bool Splay<T>::remove(const T& e)
{
    //若树空或目标不存在，则无法删除
    if (!this->_root || (e != search(e)->data))
    {
        return false;
    }

    //经search()后节点e已被伸展至树根
    assert(e == this->_root->data);
    BinNodePosi(T) w = this->_root;

    if (!HasLChild(this->_root))//没有左子树
    {
        this->_root = this->_root->rc;
        if (this->_root)
        {
            this->_root->parent = NULL;
        }
    }
    else if (!HasRChild(this->_root))//没有右子树
    {
        this->_root = this->_root->lc;
        if (this->_root)
        {
            this->_root->parent = NULL;
        }
    }
    else//左右子树同时存在
    {
        //暂时将左子树切除
        BinNodePosi(T) lTree = this->_root->lc;
        lTree->parent = NULL;
        this->_root->lc = NULL;

        //保留右子树作为树根
        this->_root = this->_root->rc;
        this->_root->parent = NULL;

        //以原树根为目标，做一次（必定失败的）查找，右子树中最小节点必伸展至根
        //因右子树中最小节点必大于w->data,所以其左子树必为空
        search(w->data);

        //只需将原左子树接回原位即可
        this->_root->lc = lTree;
        lTree->parent = this->_root;
    }

    //删除节点，更新规模
    release(w->data);
    release(w);
    --this->_size;

    //此树非空，则更新高度
    if (this->_root)
    {
        this->updateHeight(this->_root);
    }

    return true;
}

//在节点*p与*lc（可能为空）之间建立父（左）子关系
template <typename NodePosi>
void attachAsLChild(NodePosi p, NodePosi lc)
{
    p->lc = lc;
    if (lc)
    {
        lc->parent = p;
    }
}

//在节点*p与*rc（可能为空）之间建立父（右）子关系
template <typename NodePosi>
void attachAsRChild(NodePosi p, NodePosi rc)
{
    p->rc = rc;
    if (rc)
    {
        rc->parent = p;
    }
}

//Splay树伸展算法：从节点v出发逐层伸展，v为因最近访问而需伸展的节点位置
template<typename T>
BinNodePosi(T) Splay<T>::splay(BinNodePosi(T) v)
{
    if (!v)
    {
        return NULL;
    }

    BinNodePosi(T) p = NULL;//p为v的父亲
    BinNodePosi(T) g = NULL;//g为v的祖父

    //自下而上，反复对v做双层伸展
    while ((p = v->parent) && (g = p->parent))
    {
        BinNodePosi(T) gg = g->parent;
        if (IsLChild(v))
        {
            if (IsLChild(p))//zig-zig
            {
                attachAsLChild(g, p->rc);
                attachAsLChild(p, v->rc);
                attachAsRChild(p, g);
                attachAsRChild(v, p);
            }
            else//zig-zag
            {
                attachAsLChild(p, v->rc);
                attachAsRChild(g, v->lc);
                attachAsLChild(v, g);
                attachAsRChild(v, p);
            }
        }

        if (IsRChild(v))
        {
            if (IsRChild(p))//zag-zag
            {
                attachAsRChild(g, p->lc);
                attachAsRChild(p, v->lc);
                attachAsLChild(p, g);
                attachAsLChild(v, p);
            }
            else//zag-zig
            {
                attachAsRChild(p, v->lc);
                attachAsLChild(g, v->rc);
                attachAsRChild(v, g);
                attachAsLChild(v, p);
            }
        }

        //若v原先的曾祖父gg不存在，则v现在应为树根
        if (!gg)
        {
            v->parent = NULL;
        }
        else
        {
            (g == gg->lc) ? attachAsLChild(gg, v) : attachAsRChild(gg, v);
        }

        //更新子树高度
        this->updateHeight(g);
        this->updateHeight(p);
        this->updateHeight(v);
    }//双层伸展结束时，必有g == NULL，但p可能非空

    //若p果真非空，则额外再做一次单旋
    if (p = v->parent)
    {
        if (IsLChild(v))//zig
        {
            attachAsLChild(p, v->rc);
            attachAsRChild(v, p);
        }
        else
        {
            attachAsRChild(p, v->lc);
            attachAsLChild(v, p);
        }
        this->updateHeight(p);
        this->updateHeight(v);
    }

    //调整之后新树根应为被伸展的节点，故返回该节点的位置以便上层函数更新树根
    v->parent = NULL;
    return v;
}
