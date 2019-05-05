
//分而治之，将一个大规模问题，分解成若干个子问题，规模大体相当
//分别求解子问题，由子问题的解得到原问题的解
//T(n) = 2 * T(n/2) + O(1)  T(1) = O(1)
//数组求和，二分递归
#include <stdio.h>

int sum(int A[], int low, int high)
{
    printf("low = %d, high = %d\n", low, high);
    if (low == high)
    {
        return A[low];
    }

    int mid = (low + high) >> 1;
    return sum(A, low, mid) + sum(A, mid + 1, high);
}

int main()
{
    int a[] = { 2, 4, 6, 3, 7, 4, 1 };
    int result = sum(a, 0, sizeof(a) / sizeof(int) - 1);

    printf("result = %d\n", result);
    getchar();
    return 0;
}