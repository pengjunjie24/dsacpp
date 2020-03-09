
#pragma once

#include <Bitmap/BitmapO1Init.h>
#include <Share/Release.hpp>

#define ROOT 0
#define L_CHILD 1
#define R_CHILD -1*L_CHILD

//二叉树输出打印
template <typename T>
void UniPrint::p(BinTree<T>& bt) //二叉树
{
    printf("BinTree\tname:%s\tpoint:%p\tsize:%d:\n", typeid(bt).name(), &bt, bt.size());//基本信息
    Bitmap01Init* branchType = new Bitmap01Init;//记录当前节点祖先的方向
    printBinTree(bt.root(), -1, ROOT, branchType);
    release(branchType);
    printf("\n");
}

//二叉树各种派生类统一打印
template <typename T>
static void printBinTree(BinNodePosi(T) bt, int depth, int type, Bitmap01Init* bType)
{
    if (!bt)//递归基
    {
        return;
    }

    if (-1 < depth)//设置当前层的拐向标志
    {
        R_CHILD == type ? bType->set(depth) : bType->clear(depth);
    }

    printBinTree(bt->rc, depth + 1, R_CHILD, bType);//右子树（在上）
    print(bt);
    printf(" *");
    for (int i = -1; i < depth; ++i)//根据相邻各层的拐向是否一致，即可确定是否应该打印横线
    {
        if ((0 > i) || bType->test(i) == bType->test(i + 1))
        {
            printf("         ");
        }
        else
        {
            printf("|        ");
        }
    }
    switch (type)
    {
    case R_CHILD:
        printf("┌─");
        break;
    case  L_CHILD:
        printf("└─");
        break;
    default:
        printf("──"); //root
        break;
    }
    print(bt);
    printf("\n");
    printBinTree(bt->lc, depth + 1, L_CHILD, bType); //左子树（在下）
}