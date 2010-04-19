/*
 * Sort test.
 * by Lev Panov, 2010
 * and possibly others
 */

#include <stdio.h>

#include "../common/common.h"
#include "introsort.h"

#define N 8

void print_array(double *a, int n)
{
	int i;
	
	for (i = 0; i < n; i ++)
		printf("%.2lf ", a[i]);
	if (n)
		printf("\n");
}

int main( void )
{
	double a[N] = {22.2, 23.5, 49.3, 0.1, -2.5, 44.5, -77.5, 200.0};
	
	print_array(a, N);
	
	IntroSort(a, N);
	
	print_array(a, N);
	
	return 0;
}
