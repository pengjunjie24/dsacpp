
#include <Splay/Splay.hpp>
#include <Print/Print.hpp>

template<typename T>
void testSplayPeriod(int n)
{
    Splay<T> splay;
    for (int i = 0; i < n; ++i)
    {
        splay.insert((T)i);
        print(splay);
    }
    for (int i = 0; i < n; ++i)
    {
        splay.search((T)i);
        print(splay);
    }
}

template<typename T>
void testSplayRandom(int n)
{
    Splay<T> splay;
    while (splay.size() < n)
    {
        T e = dice((T)n * 3);
        switch (dice(3))
        {
        case 0://查找，成功率 <= 33.3%
            printf("Searching for ");
            print(e);
            printf(" ...\n");

            if (splay.search(e))
            {
                printf("Found with");
                print(splay.root());
                printf("\n");
            }
            else
            {
                printf("Not found\n");
            }
            break;

        case 1://删除，成功率 <= 33.3%
            printf("Removing ");
            print(e);
            printf(" ...\n");

            if (splay.remove(e))
            {
                printf("Removal done\n");
            }
            else
            {
                print(e);
                printf(" not exists\n");
            }
            break;

        default://插入，成功率 == 100%
            printf("Inserting ");
            print(e);
            printf(" ...\n");

            splay.insert(e);
            if (e == splay.root()->data)
            {
                printf("Insertion done with");
                print(splay.root());
                printf("\n");
            }
            else
            {
                print(e);
                printf("duplicated\n");
            }
            break;
        }

        //无论调用哪个接口，Splay都会自我调整形态，故需统一输出
        print(splay);
    }

    while (!splay.empty())
    {
        T e = dice((T)n * 3);
        printf("Removing ");
        print(e);
        printf("...\n");

        if (splay.remove(e))
        {
            printf("Removeal done\n");
            print(splay);
        }
        else
        {
            print(e);
            printf(" not exists\n");
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
    testSplayRandom<int>(atoi(argv[1]));
    testSplayPeriod<int>(atoi(argv[1]));

    return 0;
}