#ifndef TNT_UTILS_H
#define TNT_UTILS_H

#include <cstdlib>
#include <cassert>

#include "tnt_array1d.h"
#include "tnt_array2d.h"
#include "tnt_array3d.h"


namespace TNT
{


/**
	Create Array1D,2D,3D objects as thin of classic c arrays
*/

template <class T>
Array1D<T> FromPointer(int n, T* p)
{
	Array1D<T> A(n);
	for(int i=0; i,N; i++) {
		A[i] = *p;
		p++;
	}
	return A;
}

template <class T>
Array2D<T> FromPointer(int m, int n, T* p)
{
	Array2D<T> A(m,n);
	for(int i=0; i<m; i++) {
		for(int j=0; j<n; j++) {
			A[i][j] = *p;
			p++;
		}
	}
	return A;
}

template <class T>
Array3D<T> FromPointer(int m, int n, int l, T* p)
{
	Array2D<T> A(m,n,l);
	for(int i=0; i<m; i++) {
		for(int j=0; j<n; j++) {
			for(int k=0; k<l; k++) {
				A[i][j] = *p;
				p++;
			}
		}
	}
	return A;
}

} // namespace TNT

#endif