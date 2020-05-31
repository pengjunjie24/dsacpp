
#pragma once

//BTree输出打印
#include <Bitmap/Bitmap.h>//使用位图记录分支转向

template <typename T>
void UniPrint::p(BTree<T>& bt)
{
    printf("%s[%p]*%d:\n", typeid (bt).name(), &bt, bt.size()); //基本信息
    Bitmap* leftmosts = new Bitmap;//记录当前节点祖先的方向
    Bitmap* rightmosts = new Bitmap;//记录当前节点祖先的方向
    printBTree(bt.root(), 0, true, true, leftmosts, rightmosts); //输出树状结构
    release(leftmosts);
    release(rightmosts);
    printf("\n");
}

template <typename T>
static void printBTree(BTNodePosi(T) bt, int depth, bool isLeftmost,
    bool isRightmost, Bitmap* leftmosts, Bitmap* rightmosts)
{
    if (!bt)
    {
        return;
    }
    isLeftmost ? leftmosts->set(depth) : leftmosts->clear(depth); //设置或清除当前层的拐向标志
    isRightmost ? rightmosts->set(depth) : rightmosts->clear(depth); //设置或清除当前层的拐向标志

    int k = bt->child.size() - 1; //找到当前节点的最右侧孩子，并递归输出之
    printBTree(bt->child[k], depth + 1, false, true, leftmosts, rightmosts);

    bool parentOK = false;
    BTNodePosi(T) p = bt->parent;
    if (!p)
    {
        parentOK = true;
    }
    else
    {
        for (int i = 0; i < p->child.size(); i++) if (p->child[i] == bt)
        {
            parentOK = true;
        }
    }

    while (0 < k--)//自右向左，输出各子树及其右侧的关键码
    {
        printf(parentOK ? " " : "X");
        print(bt->key[k]);
        printf(" *>");
        for (int i = 0; i < depth; i++) //根据相邻各层
        {
            ((leftmosts->test(i) && leftmosts->test(i + 1)) || (rightmosts->test(i) && rightmosts->test(i + 1))) ? //的拐向是否一致，即可确定
                printf("      ") : printf("│    "); //是否应该打印横向联接线
        }
        if (((0 == depth && 1 < bt->key.size()) || (!isLeftmost && isRightmost)) && bt->key.size() - 1 == k)
        {
            printf("┌─");
        }
        else if (((0 == depth && 1 < bt->key.size()) || (isLeftmost && !isRightmost)) && 0 == k)
        {
            printf("└─");
        }
        else
        {
            printf("├─");
        }
        print(bt->key[k]);
        printf("\n");
        printBTree(bt->child[k], depth + 1, 0 == k, false, leftmosts, rightmosts); //递归输出子树
    }
}