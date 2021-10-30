// Schrodinger1DItem.cpp: implementation of the CSchrodinger1DItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Schrodinger1DItem.h"
#include "Schrodinger1D.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CSchrodinger1DItem::Assign(CSchrodinger1D* pS, int& i)
{
	iequ = i; 

	x = &(pS->x[iequ]);
	h = &(pS->h[iequ]);
	L = &(pS->L[iequ]);
	v = &(pS->v[iequ]);
	q = &(pS->q[iequ]);
	dq = &(pS->dq[iequ]);

	d = &(pS->T.d(iequ));
	s = 0;
	if (iequ) s = &(pS->T.s(iequ));
	d1 = &(pS->d1[iequ]);

	E = pS->E;
	Z = pS->Z;
	W1 = pS->W1;
	W2 = pS->W2;
	ne = &(pS->ne);

	i += get_m();
}

void CSchrodingerBC::MakeMatrix()
{
	double M1,M2,dE1,dE2;
	if (Prev) {
		M1 = ((CSchrodinger1DLayer*)Prev)->Mz;
		dE1 = ((CSchrodinger1DLayer*)Prev)->dE;
	}
	if (Next) {
		M2 = ((CSchrodinger1DLayer*)Next)->Mz;
		dE2 = ((CSchrodinger1DLayer*)Next)->dE;
	}

	switch (btype)
	{
	case box:
		if (s) {
			*s = -1/M1/(*L)/(*(L-1))/(*(h-1));
			*d1 = 2/M1/(*h)/(*(h-1)) + dE1 + 1e100;
		} else {
			*d1 = 2/M2/(*(h+1))/(*h) + dE2 + 1e100;

		}
		break;
	case transmit:
		if (s) {
			*s = -1/M1/(*L)/(*(L-1))/(*(h-1));
			*d1 = 1/M1/(*h)/(*h) + dE1;
		} else {
			*d1 = 1/M2/(*h)/(*h) + dE2;
		}
		break;
	case cont:
		int i=0;
		s[i] = -1/M1/L[i]/L[i-1]/h[i-1];
		d1[i] = 1/L[i]/L[i]*(1/M2/h[i] + 1/M1/h[i-1]) + dE2;
		break;
	}
}

void CSchrodingerBC::CalcQ()
{
	double M, absM;
	if (Prev) {
		M = ((CSchrodinger1DLayer*)Prev)->Md;
	}
	if (Next) {
		M = ((CSchrodinger1DLayer*)Next)->Md;
	}
	absM = fabs(M);
	int k;
	switch (btype)
	{
	case box:
		break;
	case transmit:
		for(k=0; k<*ne; k++) {
			double z = Z[k][iequ];
			*q -= M*z*W1[k];
			*dq += absM*z*W2[k];
		}
		break;
	case cont:
		for(k=0; k<*ne; k++) {
			double z = Z[k][iequ];
			*q -= M*z*W1[k];
			*dq += absM*z*W2[k];
		}
		break;
	}

}
double CSchrodingerBC::get_Md()
{
	double M;
	if (Prev) {
		M = ((CSchrodinger1DLayer*)Prev)->Md;
	}
	if (Next) {
		M = ((CSchrodinger1DLayer*)Next)->Md;
	}
	return M;
}

void CSchrodinger1DLayer::MakeMatrix()
{
	int m = get_m();
	for(int i=0; i<m; i++) {
		s[i] = -1/Mz/L[i]/L[i-1]/h[i-1];
		d1[i] = 2/Mz/h[i]/h[i-1] + dE;
	}
}

void CSchrodinger1DLayer::CalcQ()
{
	double absMd = fabs(Md);
	int m = get_m();
	for(int i =0; i<m; i++) {
		for(int k=0; k<*ne; k++) {
			double z = Z[k][iequ+i];
			q[i] -= Md*z*W1[k];
			dq[i] += absMd*z*W2[k];
		}
	}
}

void CSchrodinger1DLayer::CalcBounds(double& lbound, double& ubound, double eps)
{

	int sign = 1;
	if (Md<0) sign=-1;
	double qriterion = sign*(log(sign*Md) - log(eps));

	int m = get_m();
	if (sign==1) {
		double vmin = *(v-1);
		
		for(int i = 0; i<=m; i++) if (v[i]<vmin) vmin = v[i];
		vmin += dE; 
		
		if (lbound==0. && ubound==0.) { lbound=vmin; ubound=qriterion; }
		else {
			if (vmin < lbound) lbound = vmin;
			if (qriterion > ubound) ubound = qriterion;
		}
	} else {
		double vmax = *(v-1);
		
		for(int i = 0; i<=m; i++) if (v[i]>vmax) vmax = v[i];
		vmax += dE;
		
		if (lbound==0. && ubound==0.) { lbound=qriterion; ubound=vmax; }
		else {
			if (vmax > ubound) ubound = vmax;
			if (qriterion < lbound) lbound = qriterion;
		}
	}
}

void CSchrodinger1DLayer::Dump(ostream& s)
{
	int m = get_m();
	for(int i = -1; i<m; i++) {
		s << x[i] << "\t";
		s << (v[i]+dE)/beta << "\t";
		s << fabs(q[i]/beta*N0) << "\t";
		s << dq[i]/beta*N0 << "\n";
	}
}



