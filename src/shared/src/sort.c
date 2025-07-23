
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/sort.h>
#include <stdlib.h>

int _fSortIntDescendingImpl(const void *_a, const void *_b) {
    const int *a = (const int *)_a;
    const int *b = (const int *)_b;

    if (*a < *b) return 1;
    if (*a > *b) return -1;
    return 0;
}
int _fSortIntAscendingImpl(const void *_a, const void *_b) {
    return _fSortIntDescendingImpl(_b, _a);
}

void _fSortIntDescending(int *array, unsigned int array_size) {
    qsort(array, array_size, sizeof(int), _fSortIntDescendingImpl);
}
void _fSortIntAscending(int *array, unsigned int array_size) {
    qsort(array, array_size, sizeof(int), _fSortIntAscendingImpl);
}
