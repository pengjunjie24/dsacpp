
#pragma once

#include <stdint.h>
#include <Bitmap/Bitmap.h>
#include <Share/Entry.hpp>
#include <Vector/Vector.hpp>

template <typename K, typename V>
class HashTable
{
public:
    HashTable(int c = 5); //创建一个容量不小于c的散列表（为测试暂时选用较小的默认值）
    ~HashTable(); //释放桶数组及其中各（非空）元素所指向的词条
    int size() const { return _wordCount; } // 当前的词条数目
    int capacity() const { return _bucktetCapacity; }
    bool put(K, V); //插入（禁止雷同词条，故可能失败）
    V* get(K k); //读取
    bool remove(K k); //删除

private:
    int probe4Hit(const K& k); //沿关键码k对应的查找链，找到词条匹配的桶
    int probe4Free(const K& k); //沿关键码k对应的查找链，找到首个可用空桶
    void rehash(); //重散列算法：扩充桶数组，保证装填因子在警戒线以下

    bool lazilyRemoved(int x) { return _lazyRemoval->test(x); }
    void markAsRemoved(int x) { _lazyRemoval->set(x); }

    Vector<Entry<K, V>*> _entryPtrVector;//桶数组
    int _bucktetCapacity;//桶数组容量
    int _wordCount;//词条数量
    Bitmap* _lazyRemoval;//懒惰删除标记
    int _primeIndex;//取到质数表的下标

    //质数表，其中质数模4均为3
    const static int32_t s_primeNumer[];
};

template <typename K, typename V>
const int32_t HashTable<K, V>::s_primeNumer[] = {43, 211, 1583, 12491, 33223, 701819, 963227};


#include <HashTable/HashTableImplementation.hpp>