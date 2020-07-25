

#include <HashTable/HashTable.hpp>
#include <Share/Rand.h>
#include <Print/Print.hpp>

template <typename K, typename V>
void testHashtable(int n)
{
    HashTable<K, V> ht(n);

    while (ht.size() < n)
    {
        printf("\n");
        switch (dice(3))
        {
        case 0: //查找，成功率 <= 33.3%
        {
            K key = dice((K)n * 3); //[0, 3n)范围内的key
            printf("Searching ");
            print(key);
            printf("(%04lu) :\n", hashCode(key));
            V* pValue = ht.get(key);
            if (pValue)
            {
                printf("Found with ");
                print(*pValue);
            }
            else
            {
                printf("Not found");
            }
            printf("\n");
            break;
        }
        case 1: //删除，成功率 <= 33.3%
        {
            K key = dice((K)n * 3); //[0, 3n)范围内的key
            printf("Removing ");
            print(key);
            printf("(%04lu) :\n", hashCode(key));
            ht.remove(key) ? printf("Done\n"), printf("ht.size(): %d\n", ht.size())
                : printf("Entry not exists\n");
            break;
        }
        default://插入，成功率 == 100%
        {
            K key = dice((K)n * 3);
            V v = (V) 'A' + dice(26); //在[0, 2n)*['A'~'Z']范围内的词条
            printf("Inserting <");
            print(key);
            printf("(%04lu)", hashCode(key));
            printf(","); print(v);
            printf(">\n");
            ht.put(key, v) ? printf("Done\n"), printf("ht.size(): %d\n", ht.size())
                : printf("Dup key\n");
            break;
        }
        } //switch
    } //while
    while (ht.size() > 0)
    {
        printf("\n");
        K key = dice((K)n * 3); //[0, 3n)范围内的key
        printf("Removing ");
        print(key);
        printf(" :\n");
        ht.remove(key) ? printf("Done\n"), printf("ht.size(): %d\n", ht.size())
            : printf("Entry not exists\n");
    }
}

int main(int argc, char* argv[])
{
    if (2 > argc)
    {
        printf("Usage: %s <size of test>\a\a\n", argv[0]);
        return 1;
    }
    srand((unsigned int)time(NULL)); //设置随机种子
    testHashtable<int, char>(atoi(argv[1])); //元素类型可以在这里任意选择
    return 0;
}