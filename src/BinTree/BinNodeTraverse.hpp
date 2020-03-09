
#pragma once

#include <BinTree/BinNodeDefine.h>

#include <Stack/Stack.hpp>
#include <Queue/Queue.hpp>

//二叉树先序遍历
template < typename T, typename VST> //元素类型、操作器
void travPreR(BinNodePosi(T) x, VST& visit) //二叉树先序遍历算法（递归版）
{
    if (x)
    {
        visit(x->data);
        travPreR(x->lc, visit);
        travPreR(x->rc, visit);
    }
}//T(n)=O(1)+T(a)+T(n-a-1)=O(n)

template < typename T, typename VST> //元素类型、操作器
void travPreI1(BinNodePosi(T) x, VST& visit) //二叉树先序遍历算法（迭代版#1）
{
    Stack<BinNodePosi(T)> S; //辅助栈
    if (x)
    {
        S.push(x); //根节点入栈
    }
    while (!S.empty())//在栈变空之前反复循环
    {
        x = S.pop();
        visit(x->data); //弹出并访问当前节点，其非空孩子的入栈次序为先右后左
        if (HasRChild(*x))
        {
            S.push(x->rc);//右孩子先入后出
        }
        if (HasLChild(*x))
        {
            S.push(x->lc);//左孩子后入先出
        }
    }
}

//从当前节点出发，沿左分支不断深入，直至没有左分支的节点；沿途节点遇到后立即访问
template < typename T, typename VST> //元素类型、操作器
static void visitAlongVine(BinNodePosi(T) x, VST& visit, Stack<BinNodePosi(T)>& S)
{
    while (x)
    {
        visit(x->data); //访问当前节点
        if (x->rc)
        {
            S.push(x->rc); //右孩子入栈暂存
        }
        x = x->lc;  //沿左分支深入一层
    }
}

template < typename T, typename VST> //元素类型、操作器
void travPreI2(BinNodePosi(T) x, VST& visit)//二叉树先序遍历算法（迭代版#2）
{
    Stack<BinNodePosi(T)> S; //辅助栈
    while (true)
    {
        visitAlongVine(x, visit, S); //从当前节点出发，逐批访问
        if (S.empty())
        {
            break; //直到栈空
        }
        x = S.pop(); //弹出下一批的起点
    }
}


//二叉树中序遍历
template < typename T, typename VST> //元素类型、操作器
void travInR(BinNodePosi(T) x, VST& visit) //二叉树中序遍历算法（递归版）
{
    if (x)
    {
        travInR(x->lc, visit);
        visit(x->data);
        travInR(x->rc, visit);
    }
}

template<typename T>
static void goAlongVine(BinNodePosi(T) x, Stack<BinNodePosi(T)>& S)
{
    while (x)
    {
        S.push(x);
        x = x->lc;
    }
}

template < typename T, typename VST> //元素类型、操作器
void travInI1(BinNodePosi(T) x, VST& visit)//二叉树中序遍历算法（迭代版#1）
{
    Stack<BinNodePosi(T)> S;//辅助栈
    while (true)
    {
        goAlongVine(x, S);//从当前节点出发，逐批入栈
        if (S.empty())//直至所有节点处理完毕
        {
            break;
        }
        x = S.pop();//弹出栈顶节点并访问之
        visit(x->data);
        x = x->rc;//转向右子树
    }
}//O(n)

template < typename T, typename VST> //元素类型、操作器
void travInI2(BinNodePosi(T) x, VST& visit)//二叉树中序遍历算法（迭代版#2）
{
    Stack<BinNodePosi(T)> S; //辅助栈
    while (true)
    {
        if (x)
        {
            S.push(x); //根节点进栈
            x = x->lc; //深入遍历左子树
        }
        else if (!S.empty())
        {
            x = S.pop(); //尚未访问的最低祖先节点退栈
            visit(x->data); //访问该祖先节点
            x = x->rc; //遍历祖先的右子树
        }
        else
        {
            break; //遍历完成
        }
    }
}

template < typename T, typename VST> //元素类型、操作器
void travInI3(BinNodePosi(T) x, VST& visit)//二叉树中序遍历算法（迭代版#3，无需辅助栈）
{
    bool backtrack = false; //前一步是否刚从右子树回溯——省去栈，仅O(1)辅助空间
    while (true)
    {
        if (!backtrack && HasLChild(*x)) //若有左子树且不是刚刚回溯，则
        {
            x = x->lc; //深入遍历左子树
        }
        else//否则——无左子树或刚刚回溯（相当于无左子树）
        {
            visit(x->data); //访问该节点
            if (HasRChild(*x))//若其右子树非空，则深入右子树继续遍历
            {
                x = x->rc;
                backtrack = false; //并关闭回溯标志
            }
            else//若右子树空，则回溯
            {
                if (!(x = x->succ()))//含抵达末节点时的退出返回
                {
                    break;
                }
                backtrack = true; //并设置回溯标志
            }
        }
    }
}

template < typename T, typename VST> //元素类型、操作器
void travInI4(BinNodePosi(T) x, VST& visit)//二叉树中序遍历（迭代版#4，无需栈或标志位）
{
    while (true)
    {
        if (HasLChild(*x)) //若有左子树，则深入遍历左子树
        {
            x = x->lc;
        }
        else
        {
            visit(x->data);//否则访问当前节点
            while (!HasRChild(*x)) //不断地在无右分支处回溯至直接后继
            {
                if (!(x = x->succ()))//在没有后继的末节点处，直接退出
                {
                    return;
                }
                else//否则访问新的当前节点
                {
                    visit(x->data);
                }
            }
            x = x->rc; //（直至有右分支处）转向非空的右子树
        }
    }
}

//二叉树后序遍历
template < typename T, typename VST> //元素类型、操作器
void travPostR(BinNodePosi(T) x, VST& visit) //二叉树后序遍历算法（递归版）
{
    if (x)
    {
        travPostR(x->lc, visit);
        travPostR(x->rc, visit);
        visit(x->data);
    }
}

template < typename T> //在以S栈顶节点为根的子树中，找到最高左侧可见叶节点
static void gotoHLVFL(Stack<BinNodePosi(T)>& S)//沿途所遇节点依次入栈
{
    while (BinNodePosi(T) x = S.top()) //自顶而下，反复检查当前节点（即栈顶）
    {
        if (HasLChild(*x))//尽可能向左
        {
            if (HasRChild(*x))
            {
                S.push(x->rc); //若有右孩子，优先入栈
            }
            S.push(x->lc); //然后才转至左孩子
        }
        else //实不得已
        {
            S.push(x->rc); //才向右
        }
    }
    S.pop(); //返回之前，弹出栈顶的空节点
}

template < typename T, typename VST>
void travPostI(BinNodePosi(T) x, VST& visit)//二叉树的后序遍历（迭代版）
{
    Stack<BinNodePosi(T)> S; //辅助栈
    if (x)
    {
        S.push(x); //根节点入栈
    }
    while (!S.empty())
    {
        if (S.top() != x->parent) //若栈顶非当前节点之父（则必为其右兄）
        {
            gotoHLVFL(S); //此时需在以其右兄为根之子树中，找到HLVFL（相当于递归深入其中）
        }
        x = S.pop(); //弹出栈顶（即前一节点之后继），并访问之
        visit(x->data);
    }
}