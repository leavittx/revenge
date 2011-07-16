/*
 * Quicksort for doubles.
 * by Lev Panov, 2010
 * and possibly others
 */

#include "swap.h"

void QuickSort(double *a, int lo, int hi)
{
	// lo is the lower index, hi is the upper index
	// of the region of array a that is to be sorted
    int i = lo, j = hi;

    // comparison element x
    double x = a[(lo + hi) / 2];

    // partition
    do
    {    
        while (a[i] < x)
	    i++; 
        while (a[j] > x)
	    j--;
        if (i <= j)
        {
            swap(&a[i], &a[j]);
            
            i++;
            j--;
        }
    } while (i <= j);

    //  recursion
    if (lo < j)
		QuickSort(a, lo, j);
    if (i < hi)
		QuickSort(a, i, hi);
}
