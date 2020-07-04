
#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <List/List.hpp>

template <typename T>
class Queue
{
public:
    void enqueue(T const& e) { _rootList.insertAsLast(e); } //入队：尾部插入
    T dequeue() { return _rootList.remove(_rootList.first()); } //出队：首部删除
    T& front() { return _rootList.first()->data; } //队首

    Rank size() { return _rootList.size(); }
    bool empty() { return _rootList.empty(); }

private:
    List<T> _rootList;
};

#endif