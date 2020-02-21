
#include <Bitmap/Bitmap.h>

#include <Share/Release.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>

Bitmap::Bitmap(int n)
{
    init(n);
}

Bitmap::Bitmap(char* file, int n)
{
    init(n);
    FILE* fp = fopen(file, "r");
    fread(_M, sizeof(char), _N, fp);
    fclose(fp);
}

Bitmap::~Bitmap()
{
    delete[] _M;
    _M = NULL;
}

void Bitmap::set(int k)
{
    expand(k);
    _M[k >> 3] |= (0x80 >> (k & 0x07));
}
void Bitmap::clear(int k)
{
    expand(k);
    _M[k >> 3] &= ~(0x80 >> (k & 0x07));
}

bool Bitmap::test(int k)
{
    expand(k);
    return _M[k >> 3] & (0x80 >> (k & 0x07));
}

void Bitmap::dump(char* file)
{
    FILE* fp = fopen(file, "w");
    fwrite(_M, sizeof(char), _N, fp);
    fclose(fp);
}

char* Bitmap::bits2string(int n)
{
    expand(n - 1);//此时可能被访问的最高位为bitmap[n - 1]
    char* s = new char[n + 1];//字符串所占空间，由上层调用者负责释放
    s[n] = '\0';
    for (int i = 0; i < n; ++i)
    {
        s[i] = test(i) ? '1' : '0';
    }
    return s;//返回字符串位置
}

void Bitmap::expand(int k)
{
    if (k < 8 * _N)//仍在界内，无需扩容
    {
        return;
    }

    int oldN = _N;
    char* oldM = _M;
    init(2 * k);//与向量类型，加倍策略
    memcpy(_M, oldM, oldN);//原数据转移至新空间
    delete[] oldM;
}

void Bitmap::print(int n)
{
    expand(n);
    for (int i = 0; i < n; ++i)
    {
        printf(test(i) ? "1" : "0");
    }
}

void Bitmap::init(int n)
{
    _M = new char[_N = (n + 7) / 8];
    memset(_M, 0, _N);
}