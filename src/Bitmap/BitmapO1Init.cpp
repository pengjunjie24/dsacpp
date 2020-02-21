
#include <Bitmap/BitmapO1Init.h>

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <Share/Release.hpp>

Bitmap01Init::Bitmap01Init(Rank n)
    :_N(n)
    ,_F(new Rank[_N])
    ,_T(new Rank[_N])
    ,_top(0)
{
}

Bitmap01Init::~Bitmap01Init()
{
    delete[] _F;
    delete[] _T;
}

void Bitmap01Init::reset()
{
    _top = 0;
}

void Bitmap01Init::set(Rank k)
{
    if (!test(k))//忽略已带标记的位
    {
        _T[_top] = k;//创建校验环
        _F[k] = _top++;
    }
}

void Bitmap01Init::clear(Rank k)
{
    if (test(k))//忽略不带标记的位
    {
        if (--_top)//清除校验环，同时回收栈T的空闲单元（留意对空栈的处理）
        {
            _F[_T[_top]] = _F[k];
            _T[_F[k]] = _T[_top];
        }
    }
}

bool Bitmap01Init::test(Rank k)
{
    return (-1 < _F[k]) && (_F[k] < _top) && (k == _T[_F[k]]);
}