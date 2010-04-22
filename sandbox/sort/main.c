/*
 * Sort test.
 * by Lev Panov, 2010
 * and possibly others
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../common/common.h"
#include "sort.h"

#define N 10000000LL

void fill_rand(double *a, long long n)
{
	srand(time(NULL));

	while (--n > 0) {
		a[n] = (rand() % 10000) * ((rand() % 2) ? 1 : -1) + 
			0.01 * (rand() % 100);
	}

	a[0] = (rand() % 10000) * ((rand() % 2) ? 1 : -1) +
		0.01 * (rand() % 100);
}

void print_array(double *a, long long n)
{
	long long i;
	
	for (i = 0; i < n; i++)
		printf("%.2lf ", a[i]);
	if (n)
		printf("\n");
}

int main( void )
{
//	double a[] = {22.2, 23.5, 49.3, 0.1, -2.5, 44.5, -77.5, 200.0, 99, 28, 91.2, 49.3};
//	int N = NELEMS(a);
	static double a[N];
	clock_t t;

	fill_rand(a, N);

	//print_array(a, N);
		
	t = clock();
	
	Sort(a, N);

	printf("N: %lli, time: %f\n", N,  (double)(clock() - t) / CLOCKS_PER_SEC);
	
	//print_array(a, N);

	return 0;
}
