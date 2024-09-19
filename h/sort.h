#ifndef SORT_H
#define SORT_H

typedef int (*voidcmp_f)(const void*, const void*);

void swapElem(char* lft, char* rgt, size_t size);

void bubbleSort(void* ptr, size_t count, size_t size, voidcmp_f cmp);
void myQsort(void* ptr, size_t count, size_t size, voidcmp_f cmp);

#endif //SORT_H