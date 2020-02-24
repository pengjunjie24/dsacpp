
#pragma once

#include <stddef.h>

#define BinNodePosi(T) BinNode<T>* //节点位置
#define stature(p) ((p) ? (p)->height : -1) //节点高度（与“空树高度为-1”的约定相统一）
enum RBColor //节点颜色
{
    RB_RED,
    RB_BLACK,
};

//二叉树节点模板类
template <typename T>
struct BinNode
{
    //成员（为简化描述起见统一开放，读者可根据需要进一步封装）
    T data; //数值
    BinNodePosi(T) parent;//父节点
    BinNodePosi(T) lc;//左孩子
    BinNodePosi(T) rc; //右孩子
    int height; //高度（通用）
    int npl; //Null Path Length（左式堆，也可直接用height代替）
    RBColor color; //颜色（红黑树）

    //构造函数
    BinNode();
    BinNode(T e, BinNodePosi(T) p = NULL, BinNodePosi(T) lc = NULL, BinNodePosi(T) rc = NULL,
        int h = 0, int l = 1, RBColor c = RB_RED);

    // 操作接口
    int size(); //统计当前节点后代总数，亦即以其为根的子树的规模
    BinNodePosi(T) insertAsLC(T const& e);//作为当前节点的左孩子插入新节点
    BinNodePosi(T) insertAsRC(T const& e);//作为当前节点的右孩子插入新节点
    BinNodePosi(T) succ(); //取当前节点的直接后继(中序遍历),在当前节点下一个访问的节点
    template <typename VST>
    void travLevel(VST& visit); //子树层次遍历
    template <typename VST>
    void travPre(VST& visit); //子树先序遍历
    template <typename VST>
    void travIn(VST& visit); //子树中序遍历
    template <typename VST>
    void travPost(VST& visit); //子树后序遍历

 // 比较器、判等器（各列其一，其余自行补充）
    bool operator< (BinNode const& bn) { return data < bn.data; } //小于
    bool operator== (BinNode const& bn) { return data == bn.data; } //等于
};

#include <BinTree/BinNodeImplementation.hpp>