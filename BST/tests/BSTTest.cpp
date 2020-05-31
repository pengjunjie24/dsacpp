
#pragma once

template <typename T>
void testBST(int n)
{
    BST<T> bst;

    while (bst.size() < n)
    {
        bst.insert(dice((T)n * 3));
        print(bst);
    }
}
