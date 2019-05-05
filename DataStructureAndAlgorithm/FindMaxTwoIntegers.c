
//分而治之
//从数组A[low, high)中找出最大两个整数A[X1]和A[X2]
//元素比较次数尽可能少
//T(n) = 2 * T(n/2) + 2 <= 5n / 3 - 2

#include <stdio.h>

void max2(int A[], int low, int high, int* x1, int* x2)
{
    if (low + 1 == high)
    {
        *x1 = (A[low] > A[high]) ? low : high;
        *x2 = (*x1 == low) ? high : low;
        return;
    }
    if (low + 2 == high)
    {
        if (A[low] > A[low + 1])
        {
            *x1 = low;
            *x2 = low + 1;
        }
        else
        {
            *x1 = low + 1;
            *x2 = low;
        }

        if (A[high] > A[*x1])
        {
            *x2 = *x1;
            *x1 = high;
        }
        else if (A[high] > A[*x2])
        {
            *x2 = high;
        }
        return;
    }

    int mid = (low + high) / 2;
    int x1L, x2L;
    max2(A, low, mid, &x1L, &x2L);

    int x1R, x2R;
    max2(A, mid + 1, high, &x1R, &x2R);

    if (A[x1L] > A[x1R])
    {
        *x1 = x1L;
        *x2 = (A[x1R] > A[x2L]) ? x1R : x2L;
    }
    else
    {
        *x1 = x1R;
        *x2 = (A[x1L] > A[x2R]) ? x1L : x2R;
    }
}

int main()
{

    int a[] = { 2, 4, 6, 3, 4, 1, 5, 9};
    int x1 = 0;
    int x2 = 0;
    max2(a, 0, sizeof(a) / sizeof(int)-1, &x1, &x2);
    printf("x1 = %d, x2 = %d\n", x1, x2);

    getchar();
    return 0;
}