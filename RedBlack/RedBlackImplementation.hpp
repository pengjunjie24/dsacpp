

#pragma once

template <typename T>
BinNodePosi(T) RedBlack<T>::insert(const T& e)
{
    BinNodePosi(T)& x = this->search(e);
    if (x)
    {
        return x;
    }

    //TODO: why
    x = new BinNode<T>(e, this->_hot, NULL, NULL, -1);
    ++this->_size;

    BinNodePosi(T)xOld = x;
    sloveDoubleRed(x);
    return xOld;
}

template <typename T>
bool RedBlack<T>::remove(const T& e)
{
    //确认目标存在（留意_hot的设置）
    BinNodePosi(T)& x = this->search(e);
    if (!x)
    {
        return false;
    }

    BinNodePosi(T) r = this->removeBSTAt(x);//实施删除
    if (!(--this->_size))
    {
        return true;
    }

    // assert: _hot某一孩子刚被删除，且被r所指节点（可能是NULL）接替。以下检查是否失衡，并做必要调整
    if (!this->_hot)//若刚被删除的是根节点，则将其置黑，并更新黑高度
    {
        this->_root->color = RB_BLACK;
        updateHeight(this->_root);
        return true;
    }

    // assert: 以下，原x（现r）必非根，_hot必非空

    //若所有祖先的黑深度依然平衡，则无需调整
    if (RedBlack<T>::blackHeightUpdated(this->_hot))
    {
        return true;
    }
    if (RedBlack<T>::isRed(r))//否则，若r为红，则只需令其转黑
    {
        r->color = RB_BLACK;
        ++r->height;
        return true;
    }

    // assert: 以下，原x（现r）均为黑色
    solveDoubleBlack(r);//经双黑调整后返回
    return true;
}

//RedBlack双红调整算法：解决节点x与其父均为红色的问题。分为两大类情况：
//    RR - 1：2次颜色翻转，2次黑高度更新，1~2次旋转，不再递归
//    RR - 2：3次颜色翻转，3次黑高度更新，0次旋转，需要递归
template <typename T>
void RedBlack<T>::sloveDoubleRed(BinNodePosi(T) x)
{
    //若已（递归）转至树根，则将其转黑，整树黑高度也随之递增
    if (IsRoot(x))
    {
        this->_root->color = RB_BLACK;
        ++this->_root->height;
        return;
    }

    //否则，x的父亲p必存在,若p为黑，则可终止调整
    BinNodePosi(T) p = x->parent;
    if (isBlack(p))
    {
        return;
    }

    //既然p为红，则x的祖父必存在，且必为黑色
    BinNodePosi(T) g = p->parent;
    BinNodePosi(T) u = uncle(x);

    //根据x叔父u的颜色分别处理
    if (isBlack(u))//u为黑色(包含NULL)
    {
        BinNodePosi(T) gg = g->parent;//曾祖父
        BinNodePosi(T) r = this->fromParentTo(g) = this->rotateAt(x);//调整后的子树根节点
        r->parent = gg;//与原曾祖父联接

        //将转换后子树的根置黑，孩子置红
        r->color = RB_BLACK;
        r->lc->color = r->rc->color = RB_RED;
    }
    else//u为红色
    {
        p->color = RB_BLACK;//p由红转黑
        ++p->height;
        u->color = RB_BLACK;//u由红转黑
        ++u->height;

        if (!IsRoot(x))//g若非根，则转红
        {
            g->color = RB_RED;
        }

        sloveDoubleRed(g);//继续调整g（类似于尾递归，可优化为迭代形式）
    }
}


//RedBlack双黑调整算法：解决节点x与被其替代的节点均为黑色的问题
//为三大类共四种情况：
//---------------------------------------------------------------------
//|         情况            | 旋转  | 染色  | 单轮修正之后             |
//|BB - 1  | 黑s有红子t     | 1 - 2 | 3     | 调整随即完成             |
//|BB - 2R | 黑s无红子, p红 | 0     | 2     | 调整随即完成             |
//|BB - 2B | 黑s无红子, p黑 | 0     | 1     | 必然再次双黑, 但将上升1层|
//|BB - 3  | 红s            | 1     | 2     | 转为(BB - 1)或(BB - 2R)  |
//---------------------------------------------------------------------
template <typename T>
void RedBlack<T>::solveDoubleBlack(BinNodePosi(T) r)
{
    //r的父亲
    BinNodePosi(T) p = r ? r->parent : this->_hot;
    if (!p)
    {
        return;
    }

    //r的兄弟
    BinNodePosi(T) s = (r == p->lc) ? p->rc : p->lc;

    //兄弟s为黑
    if (RedBlack<T>::isBlack(s))
    {
        BinNodePosi(T) t = NULL;//s的红孩子（若左、右孩子皆红，左者优先；皆黑时为NULL）
        if (RedBlack<T>::isRed(s->lc))
        {
            t = s->lc;
        }
        else if (RedBlack<T>::isRed(s->rc))
        {
            t = s->rc;
        }

        if (t)//BB-1:黑s有红孩子
        {
            //备份原子树根节点p颜色，并对t及其父亲、祖父以下，
            //通过旋转重平衡，并将新子树的左、右孩子染黑
            RBColor oldColor = p->color;
            BinNodePosi(T) b = this->fromParentTo(p) = this->rotateAt(t);
            if (HasLChild(b))
            {
                b->lc->color = RB_BLACK;
                updateHeight(b->lc);
            }
            if (HasRChild(b))
            {
                b->rc->color = RB_BLACK;
                updateHeight(b->rc);
            }
            b->color = oldColor;//新子树根节点继承原根节点的颜色
            updateHeight(b);
        }
        else//BB-2R:黑s无红孩子
        {
            s->color = RB_RED;//s转红
            --s->height;
            if (RedBlack<T>::isRed(p))//BB-2R
            {
                p->color = RB_BLACK;//p转黑，但黑高度不变
            }
            else//BB-2B
            {
                --p->height;//p保持黑，但是黑高度下降
                solveDoubleBlack(p);//递归上溯
            }
        }
    }
    else//兄弟s为红: BB-3
    {
        //s转黑，p转红
        s->color = RB_BLACK;
        p->color = RB_RED;

        BinNodePosi(T) t = IsLChild(s) ? s->lc : s->rc;//取t与其父s同侧
        this->_hot = p;
        this->fromParentTo(p) = this->rotateAt(t);//对t及其父亲、祖父做平衡调整
        solveDoubleBlack(r);//继续修正r处双黑——此时的p已转红，故后续只能是BB-1或BB-2R
    }
}

template <typename T>
int RedBlack<T>::updateHeight(BinNodePosi(T) x)
{
    x->height = std::max(stature(x->lc), stature(x->rc));
    return (RedBlack<T>::isBlack(x) ? x->height++ : x->height);
}