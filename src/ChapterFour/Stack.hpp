

#ifndef STACK_HPP
#define STACK_HPP

#include "../ChapterTwo/Vector.h"

template <typename T>
class Stack : public Vector<T>//由向量派生
{
public://size(), empty()以及其他开发接口均可直接沿用
    void push(const T& e) { this->insert(this->size(), e); }//入栈，O(1)
    T pop() { return this->remove(this->size() - 1); }//出栈,O(1)
    T& top() { return (*this)[this->size() - 1]; }//取顶
};

#endif