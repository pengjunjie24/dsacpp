
#ifndef BIN_NODE_MACRO_H
#define BIN_NODE_MACRO_H

//basic
/******************************************************************************************
 * BinNode状态与性质的判断
 ******************************************************************************************/
#define IsRoot(x) (!((x)->parent))
#define IsLChild(x) (!IsRoot(x) && ((x) == (x)->parent->lc))
#define IsRChild(x) (!IsRoot(x) && ((x) == (x)->parent->rc))
#define HasParent(x) (!IsRoot(x))
#define HasLChild(x) ((x)->lc)
#define HasRChild(x) ((x)->rc)
#define HasChild(x) (HasLChild(x) || HasRChild(x)) //至少拥有一个孩子
#define HasBothChild(x) (HasLChild(x) && HasRChild(x)) //同时拥有两个孩子
#define IsLeaf(x) (!HasChild(x))

 /******************************************************************************************
  * 与BinNode具有特定关系的节点及指针
  ******************************************************************************************/
#define sibling(p) /*兄弟*/ \
    ( IsLChild(p) ? ((p)->parent->rc) : ((p)->parent->lc) )

#define uncle(x) /*叔叔*/ \
    ( IsLChild(x->parent) ? ((x)->parent->parent->rc) : ((x)->parent->parent->lc ))

//AVL
#define HeightUpdated(x) /*高度更新常规条件*/ \
        ( x->height == 1 + std::max( stature( x->lc ), stature( x->rc ) ) )
#define Balanced(x) ( stature( x->lc ) == stature( x->rc ) ) //理想平衡条件
#define BalFac(x) ( stature( x->lc ) - stature( x->rc ) ) //平衡因子
#define AvlBalanced(x) ( ( -2 < BalFac(x) ) && ( BalFac(x) < 2 ) ) //AVL平衡条件

#endif