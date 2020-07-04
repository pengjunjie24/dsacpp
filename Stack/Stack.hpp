
#ifndef STACK_HPP
#define STACK_HPP

#include <Vector/Vector.hpp>

template <typename T>
class Stack
{
public:
    void push(const T& e) { _rootVec.insert(_rootVec.size(), e); }//入栈，O(1)
    T pop() { return _rootVec.remove(_rootVec.size() - 1); }//出栈,O(1)
    T& top() { return _rootVec[_rootVec.size() - 1]; }//取顶

    Rank size() { return _rootVec.size(); }
    bool empty() { return _rootVec.empty(); }

private:
    Vector<T> _rootVec;//底层结构
};

#endif