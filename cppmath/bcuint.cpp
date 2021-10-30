#include "cppmath.h"

template class CBiCuInt<float>;
template class CBiCuInt<double>;

template <class T>
void CBiCuInt<T>::GetCoeff()
/*
Given arrays y[1..4], y1[1..4], y2[1..4], and y12[1..4], containing the function, gradients,
and cross derivative at the four grid points of a rectangular grid cell (numbered counterclockwise
from the lower left), and given d1 and d2, the length of the grid cell in the 1- and
2-directions, this routine returns the table c[1..4][1..4] that is used by routine bcuint
for bicubic interpolation.
*/
{
	static int wt[16][16]=
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
	-3,0,0,3,0,0,0,0,-2,0,0,-1,0,0,0,0,
	2,0,0,-2,0,0,0,0,1,0,0,1,0,0,0,0,
	0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,
	0,0,0,0,-3,0,0,3,0,0,0,0,-2,0,0,-1,
	0,0,0,0,2,0,0,-2,0,0,0,0,1,0,0,1,
	-3,3,0,0,-2,-1,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,-3,3,0,0,-2,-1,0,0,
	9,-9,9,-9,6,3,-3,-6,6,-6,-3,3,4,2,1,2,
	-6,6,-6,6,-4,-2,2,4,-3,3,3,-3,-2,-1,-1,-2,
	2,-2,0,0,1,1,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,2,-2,0,0,1,1,0,0,
	-6,6,-6,6,-3,-3,3,3,-4,4,2,-2,-2,-2,-1,-1,
	4,-4,4,-4,2,2,-2,-2,2,-2,-2,2,1,1,1,1};
	int l,k,j,i;
	T xx,d1d2;
	d1d2=d1*d2;
	for (i=1;i<=4;i++) { //Pack a temporary vector x.
		xl_[i-1]=y_[i-1];
		xl_[i+3]=y1_[i-1]*d1;
		xl_[i+7]=y2_[i-1]*d2;
		xl_[i+11]=y12_[i-1]*d1d2;
	}
	for (i=0;i<=15;i++) { //Matrix multiply by the stored table.
		xx=0.0;
		for (k=0;k<=15;k++) xx += wt[i][k]*xl_[k];
		cl_[i]=xx;
	}
	l=0;
	for (i=0;i<4;i++) //Unpack the result into the output table.
		for (j=0;j<4;j++) c_[i][j]=cl_[l++];
}

template <class T>
void CBiCuInt<T>::GetInterp(T& ansy, T& ansy1, T& ansy2)
/*
Bicubic interpolation within a grid square. Input quantities are y,y1,y2,y12 (as described in
bcucof); x1l and x1u, the lower and upper coordinates of the grid square in the 1-direction;
x2l and x2u likewise for the 2-direction; and x1,x2, the coordinates of the desired point for
the interpolation. The interpolated function value is returned as ansy, and the interpolated
gradient values as ansy1 and ansy2. This routine calls bcucof.
*/
{
	ansy=ansy2=ansy1=0.0;
	for (int i=3;i>=0;i--) { //Equation (3.6.6).
		ansy=t*ansy+((c_[i][3]*u+c_[i][2])*u+c_[i][1])*u+c_[i][0];
		ansy2=t*ansy2+(3.0*c_[i][3]*u+2.0*c_[i][2])*u+c_[i][1];
		ansy1=u*ansy1+(3.0*c_[3][i]*t+2.0*c_[2][i])*t+c_[1][i];
	}
	ansy1 /= d1;
	ansy2 /= d2;
}

template <class T>
void CBiCuInt<T>::Interp(T x1, T x2, T& y, T& y1, T& y2)
{
	int i1l = Locate(x1,X1,d1,t), i1u = i1l + 1;
	int i2l = Locate(x2,X2,d2,u), i2u = i2l + 1;

	y_[0] = Y[i1l][i2l];
	y_[1] = Y[i1u][i2l];
	y_[2] = Y[i1u][i2u];
	y_[3] = Y[i1l][i2u];

	y1_[0] = Y1[i1l][i2l];
	y1_[1] = Y1[i1u][i2l];
	y1_[2] = Y1[i1u][i2u];
	y1_[3] = Y1[i1l][i2u];

	y2_[0] = Y2[i1l][i2l];
	y2_[1] = Y2[i1u][i2l];
	y2_[2] = Y2[i1u][i2u];
	y2_[3] = Y2[i1l][i2u];

	y12_[0] = Y12[i1l][i2l];
	y12_[1] = Y12[i1u][i2l];
	y12_[2] = Y12[i1u][i2u];
	y12_[3] = Y12[i1l][i2u];

	GetCoeff();
	GetInterp(y,y1,y2);
}

template <class T>
int CBiCuInt<T>::Locate(T x, TNT::Array1D<T>& X, T& d, T&u)
{
	unsigned int j, n = X.dim();
	
	
	if ( x <= X[0] ) j = 0;
	else if ( x >= X[n-1] ) j = n-2;
	else {
		unsigned int ju,jl;
		jl=0; //Initialize lower
		ju=n-1; //and upper limits.
		while (ju-jl > 1) { //If we are not yet done,
			j=(ju+jl) >> 1; //compute a midpoint,
			if (x >= X[j])
				jl=j; // and replace either the lower limit
			else
				ju=j; // or the upper limit, as appropriate.
		} // Repeat until the test condition is satisfied.
		j = jl;
	}

	d = X[j+1] - X[j];
	u = (x - X[j])/d;
	return j;
}
