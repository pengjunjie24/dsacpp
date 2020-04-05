
/******************************************************************************************
 * 数据元素、数据结构通用输出接口
 ******************************************************************************************/
#pragma once

#include <BinTree/BinTree.hpp>
#include <BinTree/BinNode.hpp>
#include <AVL/AVL.hpp>
#include <Splay/Splay.hpp>

#include <stdio.h>


class UniPrint
{
public:
    static void p(int);
    static void p(float);
    static void p(double);
    static void p(char);

    template <typename T>
    static void p(BinNode<T>&); //BinTree节点
    template <typename T>
    static void p(BinTree<T>&); //二叉树
    template <typename T>
    static void p(AVL<T>&); //AVL
    template <typename T>
    static void p(Splay<T>&); //Splay
};

template <typename T> static void print(T& x) { UniPrint::p(x); }
template <typename T> static void print(const T& x) { UniPrint::p(x); } //for Stack
static void print(char* x) { printf(" %s", x ? x : "<NULL>"); } //字符串特别处理
static void print(const char* x) { printf(" %s", x ? x : "<NULL>"); } //字符串特别处理
template <typename T> static void print(T* x) { x ? print(*x) : print(" <NULL>"); }

#include <Print/PrintImplementation.hpp>