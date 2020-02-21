
#pragma once

void UniPrint::p(int e) { printf(" %04d", e); }
void UniPrint::p(float e) { printf(" %4.3f", e); }
void UniPrint::p(double e) { printf(" %4.3f", e); }
void UniPrint::p(char e) { printf(" %c", (31 < e) && (e < 128) ? e : '$'); }


template<typename T>
void UniPrint::p(BinNode<T>& node)
{
    p(node.data);

    //父子链指针
    printf( ((node.lc && (&node != node.lc->parent))
        || (node.rc && (&node != node.rc->parent)))
        ? "@" : " ");
}


template <typename T>
void UniPrint::p(BinTree<T>& bt) //二叉树
{
    printf("%s[%d]*%d:\n", typeid(bt).name(), (int) &bt, bt.size());//基本信息

}