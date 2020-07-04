

#include <RedBlack/RedBlack.hpp>
#include <Print/Print.hpp>

template<typename T>
void testRedBlack(int n)
{
    RedBlack<T> rb;
    while (rb.size() < n)
    {
        T e = dice((T)n * 3);//[0, 3n)范围内的e
        switch (dice(6))
        {
        case 0://查找（概率 = 1/6）
        {
            printf("Searching for ");
            print(e);
            printf("  \n");
            BinNodePosi(T) p = rb.search(e);
            if (p)
            {
                printf("Found with");
                print(p);
                printf("\n");
            }
            else
            {
                printf("Not found\n");
            }
            break;
        }

        case 1:
        case 2://删除（概率 = 2/6)
        {
            printf("Removing ");
            print(e);
            printf(" ...\n");

            if (rb.remove(e))
            {
                printf("remove ");
                print(rb);
                printf(" Done\n");
            }
            else
            {
                printf("Not exists\n");
            }
            break;
        }

        default://插入（概率 = 3/6)
        {
            printf("Inserting ");
            print(e);
            printf(" ...\n");

            BinNodePosi(T) p = rb.insert(e);
            if (p->data != e)
            {
                print(p->data);
                printf(" <> ");
                print(e);
                printf("\n");
            }

            printf("Done with");
            print(p);
            printf("\n");
            print(rb);
            break;
        }

        }
    }

    while (rb.size() > 0)
    {
        T e = dice((T)n * 3);
        printf("Removing ");
        print(e);
        printf(" ...\n");

        if (rb.remove(e))
        {
            printf("Done\n");
            print(rb);
        }
        else
        {
            printf("Not exists\n");
        }
    }
}

int main(int argc, char* argv[])
{
    if (2 > argc)
    {
        printf("Usage: %s <size of test>\a\a\n", argv[0]);
        return 1;
    }
    srand((unsigned int)time(NULL));
    testRedBlack<int>(atoi(argv[1])); //元素类型可以在这里任意选择
    return 0;
}