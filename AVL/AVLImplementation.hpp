
#pragma once

template <typename T>
BinNodePosi(T) AVL<T>::insert(const T& e)
{
    //确认目标节点是否存在
    BinNodePosi(T)& hitPosi = this->search(e);
    if (hitPosi)
    {
        return hitPosi;
    }

    //创建新节点
    BinNodePosi(T) newPosi = NULL;
    if (this->empty())
    {
        newPosi = this->insertAsRoot(e);
    }
    else
    {
        newPosi = (e < this->_hot->data) ?
            this->insertAsLC(this->_hot, e) : this->insertAsRC(this->_hot, e);
    }
    this->updateHeightAbove(newPosi);

    //从newPosi之父出发向上，逐层检查各代祖先oldPosi
    for (BinNodePosi(T) oldPosi = this->_hot; oldPosi; oldPosi = oldPosi->parent)
    {
        //一旦发现oldPosi失衡，则（采用“3 + 4”算法）使之复衡，并将子树重新接入原树
        //oldPosi复衡后，局部子树高度必然复原；其祖先亦必如此，故调整随即结束
        if (!AvlBalanced(oldPosi))
        {
            //传入参数为g,p,v中的v节点
            BinNodePosi(T)& parentToRef = this->fromParentTo(oldPosi);
            parentToRef = this->rotateAt(tallerChild(tallerChild(oldPosi)));
            break;
        }
        else
        {
            //否则（g依然平衡），只需简单地更新其高度（注意：即便g未失衡，高度亦可能增加）
            this->updateHeight(oldPosi);
        }
    }//至多只需一次调整；若果真做过调整，则全树高度必然复原

    return newPosi;//返回新节点位置
}//无论e是否存在于原树中，总有AVL::insert(e)->data == e

template <typename T>
bool AVL<T>::remove(const T& e)
{
    BinNodePosi(T)& x = this->search(e);
    if (!x)//确认目标存在
    {
        return false;
    }

    //先按BST规则删除之（此后，原节点之父_hot及其祖先均可能失衡）
    this->removeBSTAt(x);
    --this->_size;

    //从_hot出发向上，逐层检查各代祖先g
    for (BinNodePosi(T) g = this->_hot; g; g = g->parent)
    {
        //一旦发现g失衡，则（采用“3 + 4”算法）使之复衡，并将该子树联至原父亲
        if (!AvlBalanced(g))
        {
            BinNodePosi(T)& gParentRef = this->fromParentTo(g);
            gParentRef = this->rotateAt(tallerChild(tallerChild(g)));
            g = gParentRef;
        }
        this->updateHeight(g);//并更新其高度（注意：即便g未失衡，高度亦可能降低）
    }//可能需做(logn)次调整——无论是否做过调整，全树高度均可能降低
    return true;
}//若目标节点存在且被删除，返回true；否则返回false