

#include <BTree/BTree.hpp>
#include <Print/Print.hpp>
#include <Share/Rand.h>

template <typename T>
void testBTree(int order, int n)
{
    BTree<T> bt(order);
    while (bt.size() < n)
    {
        T e = dice((T)n * 3); //[0, 3n)范围内的e
        switch (dice(3))
        {
        case 0://查找，成功率 <= 33.3%
        {
            printf("Searching for ");
            print(e);
            printf(" ... ");
            BTNodePosi(T) p = bt.search(e);
            printf(p ? "Found\n" : "Not found\n");
            break;
        }
        case 1://删除，成功率 <= 33.3%
        {
            printf("Removing ");
            print(e);
            printf(" ... ");
            if (bt.remove(e))
            {
                printf("Done\n");
                print(bt);
            }
            else
            {
                printf("Not exists\n");
            }
            break;
        }
        default://插入，成功率 == 100%
        {
            printf("Inserting ");
            print(e);
            printf(" ... ");
            if (bt.insert(e))
            {
                printf("Done\n");
                print(bt);
            }
            else
            {
                printf("Dup key\n");
            }
            break;
        }
        }
    }

    while (bt.size() > 0)
    {
        T e = dice((T)n * 3); //[0, 3n)范围内的e
        printf("Removing ");
        print(e);
        printf(" ... ");

        if (bt.remove(e))
        {
            printf("Done\n");
            print(bt);
        }
        else
        {
            printf("not exists\n");
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        printf("Usage: %s <order of B-tree> <size of test>\a\a\n", argv[0]);
        return 1;
    }

    int order = atoi(argv[1]);
    if (order < 3)
    {
        printf("Make sure the order (%d) is no less than 3.\a\a\n", order);
        return 1;
    }

    int size = atoi(argv[2]);
    if (size < 0)
    {
        printf("Make sure the size (%d) is no less than 0.\a\a\n", size);
        return 1;
    }

    srand((unsigned int)time(NULL));
    testBTree<int>(order, size);//元素类型、比较器可以在这里任意选择
    return 0;
}