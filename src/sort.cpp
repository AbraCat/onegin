#include <assert.h>
#include <stddef.h>

#include <sort.h>

void swapElem(char* lft, char* rgt, size_t size)
{
    char tmp = 0;
    for (int i = 0; i < size; ++i)
    {
        tmp = lft[i];
        lft[i] = rgt[i];
        rgt[i] = tmp;
    }
}

void bubbleSort(void* ptr, size_t count, size_t size, voidcmp_f cmp)
{
    assert(ptr != NULL);

    char *arr = (char*)ptr, tmp = 0;
    for (int i = count - 2; i >= 0; --i)
    {
        for (int j = 0; j <= i; ++j)
        {
            if (cmp(arr + j * size, arr + (j + 1) * size) > 0)
            {
                swapElem(arr + j * size, arr + (j + 1) * size, size);
            }
        }
    }
}

void myQsort(void* ptr, size_t count, size_t size, voidcmp_f cmp)
{
    assert(ptr != NULL);

    if (count == 1 || count == 0) return;

    char *arr = (char*)ptr;
    char *lft = (char*)ptr, *rgt = (char*)arr + (count - 1) * size, *piv = arr + (count - 1) * size;

    while (lft < rgt && cmp(lft, piv) < 0)
        lft += size;
    while (lft < rgt && cmp(rgt, piv) > 0)
        rgt -= size;

    while (lft < rgt)
    {
        swapElem(lft, rgt, size);

        lft += size;
        rgt -= size;

        while (lft < rgt && cmp(lft, piv) < 0)
            lft += size;
        while (lft < rgt && cmp(rgt, piv) > 0)
            rgt -= size;
    }
    if (lft == arr + (count - 1) * size)
    {
        myQsort(arr, count - 1, size, cmp);
        myQsort(arr + (count - 1) * size, 1, size, cmp);
        return;
    }
    if (lft > rgt)
    {
        myQsort(arr, (lft - arr) / size, size, cmp);
        myQsort(lft, count - (lft - arr) / size, size, cmp);
        return;
    }
    int inc_l = cmp(lft, piv) <= 0 ? 1 : 0;
    myQsort(arr, (lft - arr) / size + inc_l, size, cmp);
    myQsort(rgt + inc_l * size, count - (rgt - arr) / size - inc_l, size, cmp);
}