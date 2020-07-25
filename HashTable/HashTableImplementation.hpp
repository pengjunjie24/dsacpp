
#pragma once

#include <HashTable/HashCode.h>

#include <Share/Release.hpp>

template<typename K, typename V>
HashTable<K, V>::HashTable(int c)
{
    _wordCount = 0;
    _primeIndex = 0;

    while (s_primeNumer[_primeIndex] < c)
    {
        ++_primeIndex;
    }
    _bucktetCapacity = s_primeNumer[_primeIndex];
    _entryPtrVector.resize(_bucktetCapacity);//开辟桶数组（还需核对申请成功），初始装填因子为N/M = 0%
    _lazyRemoval = new Bitmap(_bucktetCapacity);//懒惰删除标记比特图
}

template<typename K, typename V>
HashTable<K, V>::~HashTable()
{
    //析构前释放桶数组及非空词条
    for (int i = 0; i < _bucktetCapacity; ++i)
    {
        if (_entryPtrVector[i])
        {
            release(_entryPtrVector[i]);//释放非空的桶
        }
    }
    release(_entryPtrVector);//释放桶数组
    release(_lazyRemoval);//释放懒惰删除标记
}

template<typename K, typename V>
bool HashTable<K, V>::put(K key, V value)
{
    //雷同元素不必重复插入
    if (_entryPtrVector[probe4Hit(key)])
    {
        return false;
    }

    int freeLocation = probe4Free(key);//为新词条找个空桶（只要装填因子控制得当，必然成功）
    _entryPtrVector[freeLocation] = new Entry<K, V>(key, value);
    ++_wordCount;//插入（注意：懒惰删除标记无需复位）

    if (_wordCount * 2 > _bucktetCapacity)//装填因子高于50%后重散列
    {
        rehash();
    }

    return true;
}

template<typename K, typename V>
V* HashTable<K, V>::get(K key)
{
    int matchKey = probe4Hit(key);
    return _entryPtrVector[matchKey] ? (&(_entryPtrVector[matchKey]->value)) : NULL;
}

template<typename K, typename V>
bool HashTable<K, V>::remove(K key)
{
    int matchKey = probe4Hit(key);
    if (!_entryPtrVector[matchKey])//对应词条不存在时，无法删除
    {
        return false;
    }
    release(_entryPtrVector[matchKey]);//否则释放桶中词条
    _entryPtrVector[matchKey] = NULL;
    markAsRemoved(matchKey);//设置懒惰删除标记
    --_wordCount;//并更新词条总数
    return true;
}

//沿关键码k对应的查找链，找到与之匹配的桶（供查找和删除词条时调用）
//试探策略多种多样，可灵活选取；这里仅以线性试探策略为例
template<typename K, typename V>
int HashTable<K, V>::probe4Hit(const K& key)
{
    int hashkey = hashCode(key) % _bucktetCapacity;
    //判断条件:(1)hashkey位置上有数据，但是key值不相等
    //         (2)hashkey位置上无数据，但是该位置上有删除标记
    while ( (_entryPtrVector[hashkey] && (key != _entryPtrVector[hashkey]->key))
        || (!_entryPtrVector[hashkey] && lazilyRemoved(hashkey)) )
    {
        hashkey = (hashkey + 1) % _bucktetCapacity;
    }

    return hashkey;//调用者根据_entryPtrVector[hashkey]是否为空，即可判断查找是否成功
}

//沿关键码k对应的查找链，找到首个可用空桶（仅供插入词条时调用）
//试探策略多种多样，可灵活选取；这里仅以线性试探策略为例
template<typename K, typename V>
int HashTable<K, V>::probe4Free(const K& key)
{
    int hashKey = hashCode(key) % _bucktetCapacity;//从起始桶（按除余法确定）出发
    while (_entryPtrVector[hashKey])//首个试探的桶单元地址,沿查找链逐桶试探，直到首个空桶（无论是否带有懒惰删除标记）
    {
        hashKey = (hashKey + 1) % _bucktetCapacity;
    }

    return hashKey;//为保证空桶总能找到，装填因子及散列表长需要合理设置
}

//重散列算法：装填因子过大时，采取“逐一取出再插入”的朴素策略，对桶数组扩容
//因散列函数的定址与表长M直接相关，既然M已改变，就不可简单地批量复制原桶数组
template<typename K, typename V>
void HashTable<K, V>::rehash()
{
    //获取新的桶数量
    int oldCapacity = _bucktetCapacity;
    _bucktetCapacity = s_primeNumer[++_primeIndex];

    Vector<Entry<K, V>*> oldHashTable;
    oldHashTable.swap(_entryPtrVector);//交换桶数据存放区
    _entryPtrVector.resize(_bucktetCapacity);

    release(_lazyRemoval);
    _lazyRemoval = new Bitmap(_bucktetCapacity);//新开懒惰删除标记比特图

    for (int i = 0; i < oldCapacity; ++i)//扫描原桶数组
    {
        //将非空桶中的词条逐一插入至新的桶数组
        if (oldHashTable[i])
        {
            put(oldHashTable[i]->key, oldHashTable[i]->value);
        }
    }
}
