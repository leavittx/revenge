/*
 * Swap macro for doubles.
 * by Lev Panov, 2010
 * and possibly others
 */

#pragma once

#define swap(a, b) { \
		register double tmp = *(a); \
		*(a) = *(b); \
		*(b) = tmp; \
}
