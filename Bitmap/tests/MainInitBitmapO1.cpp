

#include <Share/Rand.h>
#include <Bitmap/BitmapO1Init.h>
#include <Bitmap/Bitmap.h>

#include <typeinfo>

#include <string.h>
#include <stdio.h>

template <typename BIT_MAP>
int testBitmap(int n)
{
    printf("\n%s start test\n", typeid(BIT_MAP).name());

    bool* B = new bool[n];//常规位图
    BIT_MAP M(n);//高效位图
    for (int t = 0; t < 50; ++t)//多次重复使用位图测试
    {
        memset(B, 0, n * sizeof(bool));//逐位清零(O(n))
        M.reset();//逻辑清零O(1)

        for (int i = 0; i < 3 * n; ++i)
        {
            Rank k = dice(n);//随机赋值的位置，小于n的位置上
            if (dice(2))//随机赋值
            {
                B[k] = true;
                M.set(k);
            }
            else
            {
                B[k] = false;
                M.clear(k);
            }
        }

        int k;
        for (k = 0; k < n; ++k) //逐位将原始位图与高效位图进行对比
        {
            if (B[k] != M.test(k))//结果不相等
            {
                break;
            }
        }
        if (k < n)//结果不相等的处理
        {
            printf("\nB[]: ");
            for (int j = 0; j < k; ++j)
            {
                printf("%6c", B[j] ? 'x' : ' ');
            }
            printf("\n M[]: ");
            for (int j = 0; j <= k; j++)
            {
                printf("%6c", M.test(j) ? 'x' : ' ');
            }
            printf("\n");
        }
        else
        {
            printf("Test %4d OK\n", t);
        }
    }
    delete[] B;
    return 0;
}

int main(int argc, char* argv[])
{
    if (2 > argc)
    {
        printf("Usage: %s <size of test>\a\a\n", argv[0]);
        return 1;
    }
    srand((unsigned int)time(NULL)); //设置随机种子
    testBitmap<Bitmap01Init>(atoi(argv[1])); //启动测试
    testBitmap<Bitmap>(atoi(argv[1]));

    return 0; 
}