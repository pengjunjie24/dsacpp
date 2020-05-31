
#include <Vector/Vector.hpp>

//测试:无序向量顺序查找
template <typename T>
void TestFind(Vector<T>& V, int n)
{
    //依次查找向量中元素，当然成功
    for (int i = 0; i < V.size(); ++i)
    {
        T e = V[i];
        print(e);
        Rank r = V.find(e);
        if (r < 0)
        {
            printf(": not found until rank V[%d] <> %d", r, e);
        }
        else
        {
            printf(": found at rank V[%d] = %d", r, V[r]);
        }

        printf("\n");
    }
    //依次查找每对相邻元素的均值，可能成功
    for (int i = 0; i <= V.size(); ++i)
    {
        T a = (0 < i) ? V[i - 1] : V[0] - 4;
        T b = (i < V.size()) ? V[i] : V[V.size() - 1] + 4;
        T e = (a + b) / 2;
        print(e);
        Rank r = V.find(e);
        if (r < 0)
        {
            printf(": not found until rank V[%d] <> %d", r, e);
        }
        else
        {
            printf(": found at rank V[%d] = %d", r, V[r]);
        }
        printf("\n");
    }
}

//测试：有序向量的查找（binSearch或fibSearch）
template<typename T>
void TestSearch(Vector<T>& v)
{
    //依次查找向量中元素，当然成功
    for (int i = 0; i < V.size(); ++i)
    {
        T e = V[i];
        print(e);
        printf(": by ");
        Rank r = v.search();
        if (V[r] == e)
        {
            printf("found at rank V[%d] = %d", r, V[r]);
        }
        else
        {
            printf("found at rank V[%d] = %d <> %d\a\a", r, V[r], e);
        }
        printf("\n\n");
    }

    //依次相邻元素的均值，可能成功
    for (int i = 0; i < V.size(); ++i)
    {
        T a = (0 < i) ? V[i - 1] : V[0] - 4;
        T b = (i < V.size()) ? V[i] : V[V.size() - 1] + 4;
        T e = (a + b) / 2; print(e); printf(": by ");
        Rank r = V.search(e);
        printf("V[%3d] =", r);
        (r < 0) ? print("-INF") : print(V[r]); printf("  ~  ");
        printf("V[%3d] =", r + 1);
        (r + 1 < V.size()) ? print(V[r + 1]) : print("+INF");
        bool ordered = true;
        if ((r >= 0) && (V[r] > e))
        {
            ordered = false;
        }
        if ((r + 1 < V.size()) && (V[r + 1] <= e))
        {
            ordered = false;
        }
        if (!ordered)
        {
            printf("\tincorrect search\a\a");
        }
        printf("\n\n");
    }
}

//测试：有序向量的插入
template <typename T>
void TestOrderedInsertion(Vector<T>& V, int n)
{
    while (n * 2 > V.size())
    {
        T e = dice((T)n * 2);
        printf("Inserting ");
        print(e);
        printf(" by ");
        V.insert(V.search(e) + 1, e);
        print(V);
    }
}