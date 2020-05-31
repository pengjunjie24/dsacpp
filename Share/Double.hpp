
#pragma once

//函数对象：倍增一个T类对象
template <typename T>
struct Double
{
    virtual void operator() (T& e)
    {
        e *= 2;//假设T可直接倍增
    }
};