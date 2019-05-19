
//LCS: 求解最长公共子序列
//递归求解
//m == -1 || n == -1,则取空序列，递归基
//若A[n] = B[m] = X, 则取作LCS(A[0, n), B[0, m) + X)//减而治之
//A[n] ！= B[m],则取LCS(A[0, n], B[0, m))与LCS(A[0, n), B[0, m])中取更长者;
//大量重复的递归实例，时间复杂度为O(2^n)
#include <stdio.h>
#include <string.h>

char* LCS(char* sequenceA, char* sequenceB, int sequenceALen, int sequenceBLen, char* lcs, int lcsLen)
{
    if (sequenceALen == -1 || sequenceBLen == -1)
    {
        return lcs;//递归基
    }
    if (sequenceA[sequenceALen] == sequenceB[sequenceBLen])//第一种情况，A与B末尾元素相同
    {
        lcs[lcsLen++] = sequenceA[sequenceALen];
        return LCS(sequenceA, sequenceB, sequenceALen - 1, sequenceBLen - 1, lcs, lcsLen);
    }
    else//第二种情况，A与B不相等
    {
        return strlen(LCS(sequenceA, sequenceB, sequenceALen, sequenceBLen - 1, lcs, lcsLen)) >
            strlen(LCS(sequenceA, sequenceB, sequenceALen - 1, sequenceBLen, lcs, lcsLen)) ?
            LCS(sequenceA, sequenceB, sequenceALen, sequenceBLen - 1, lcs, lcsLen) :
            LCS(sequenceA, sequenceB, sequenceALen - 1, sequenceBLen, lcs, lcsLen);
    }
}

int main()
{
    char a[] = "adbbqs";//单词太长会特别慢
    char b[] = "diobdads";

    int lcsMinLen = strlen(a) > strlen(b) ? strlen(b) : strlen(a);
    char lcs[20] = { 0 };

    char* lcsResult = LCS(a, b, strlen(a) - 1, strlen(b) - 1, lcs, 0);

    printf("a = %s, b = %s, lcs = %s\n", a, b, lcs);

    getchar();
    return 0;
}