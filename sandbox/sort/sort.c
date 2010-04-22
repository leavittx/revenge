/*
 * Sort for doubles.
 * by Lev Panov, 2010
 * and possibly others
 */

#include "introsort.h"
#include "quicksort.h"

void Sort(double *a, int n)
{
    IntroSort(a, n);
    //QuickSort(a, 0, n);
}
