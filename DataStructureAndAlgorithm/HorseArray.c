
#include <stdio.h>


//减而治之，将一个规模较大的问题分解成两个子问题
//其中一个问题平凡，另一个规模缩减
//将数组翻转
void swap(int* leftValue, int* rightValue)
{
    int tempValue = *leftValue;
    *leftValue = *rightValue;
    *rightValue = tempValue;
}

void reverse(int* srcArray, int lowIndex, int highIndex)
{
    printf("lowIndex = %d, highIndex = %d\n", lowIndex, highIndex);
    if (lowIndex < highIndex)
    {
        swap(&srcArray[lowIndex], &srcArray[highIndex]);
        reverse(srcArray, lowIndex + 1, highIndex - 1);
    }
}

int main()
{
    int reverseArray[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
    int reverseLen = sizeof(reverseArray) / sizeof(int);
    for (int i = 0; i < reverseLen; ++i)
    {
        printf("%d ", reverseArray[i]);
    }
    printf("\n");
    getchar();

    reverse(reverseArray, 0, reverseLen - 1);
    for (int i = 0; i < reverseLen; ++i)
    {
        printf("%d ", reverseArray[i]);
    }
    printf("\n");
    getchar();
}