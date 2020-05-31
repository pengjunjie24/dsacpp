
#pragma once

//函数对象：递增一个T类对象
template <typename T>
struct Increase
{
    virtual void operator() (T& e)
    {
        e++;//假设T可直接递增或已重载++
    }
};