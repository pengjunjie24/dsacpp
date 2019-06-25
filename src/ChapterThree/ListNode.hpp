/*************************************************************************
  > File Name: ListNode.hpp
  > Author: pengjunjie
  > Mail: 1002398145@qq.com
  > Created Time: 2019年06月09日 星期日 10时26分31秒
 ************************************************************************/

#ifndef LISTNODE_HPP
#define LISTNODE_HPP

typedef int Rank;//秩
#define ListNodePosi(T) ListNode<T>* //列表节点位置

template <typename T>
struct ListNode//列表节点模板类
{
    //成员
    T data;//数值
    ListNodePosi(T) pred;//前驱
    ListNodePosi(T) succ;//后继

    //构造函数
    ListNode(){}//针对header和trailer的构造
    ListNode(T e, ListNodePosi(T) p = NULL,
        ListNodePosi(T) s = NULL)//默认构造器
        :data(e)
        , pred(p)
        , succ(s)
    {}

    //操作接口
    ListNodePosi(T) insertAsPred(T const& e)//紧靠当前节点之前插入新节点
    {
        ListNodePosi(T) x = new ListNode(e, pred, this);
        pred->succ = x;
        pred = x;
        return x;//返回新节点的位置
    }

    ListNodePosi(T) insertAsSucc(T const& e)//紧随当前节点之后插入新节点
    {
        ListNodePosi(T) x = new ListNode(e, this, succ);
        succ->pred = x;//当前后继的前驱为新插入元素
        succ = x;//当前后继变为新插入元素
        return x;
    }
};

#endif
