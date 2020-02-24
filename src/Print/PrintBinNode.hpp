
#pragma once

//BinNode节点
template<typename T>
void UniPrint::p(BinNode<T>& node)
{
    p(node.data);

    //父子链指针
    printf(((node.lc && (&node != node.lc->parent))
        || (node.rc && (&node != node.rc->parent)))
        ? "@" : " ");
}
