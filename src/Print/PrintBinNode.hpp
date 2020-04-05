
#pragma once

//BinNode节点
template<typename T>
void UniPrint::p(BinNode<T>& node)
{
    p(node.data);

    //节点高度
#if defined(DSA_BST)
    printf("(%-2d)", node.height); //高度
#elif defined(DSA_AVL)
    printf("(%-2d)", node.height); //高度
#elif defined(DSA_REDBLACK)
    printf("(%-2d)", node.height); //高度
#elif defined(DSA_SPLAY)
    printf("(%-2d)", node.height); //高度
#endif

    //父子链指针
    printf(((node.lc && (&node != node.lc->parent))
        || (node.rc && (&node != node.rc->parent)))
        ? "@" : " ");

    //左右平衡
#if defined(DSA_AVL)
    if (!AvlBalanced(node))
    {
        printf("X"); //AVL平衡
    }
    else if (0 < BalFac(node))
    {
        printf("\\"); //AVL平衡
    }
    else if (BalFac(node) < 0)
    {
        printf("/"); //AVL平衡
    }
    else
    {
        printf("-"); //AVL平衡
    }
#elif defined(DSA_REDBLACK)
    if (!Balanced(node))
    {
        printf("X"); //RB平衡
    }
    else if (0 < BalFac(node))
    {
        printf("\\"); //RB平衡
    }
    else if (BalFac(node) < 0)
    {
        printf("/"); //RB平衡
    }
    else
    {
        printf("-"); //RB平衡
    }
#else
   //平衡无所谓
#endif
}
