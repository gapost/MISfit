// Poisson1D.cpp: implementation of the CPoisson1D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include <cassert>
#include <fstream>

#include "Poisson1D.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPoisson1D::~CPoisson1D()
{
	CPoisson1DItem *P1, *P = First;
	while (P!=0) {
		P1 = P->GetNext();
		delete P;
		P = P1;
	}
}
void CPoisson1D::Add(CPoisson1DItem* P)
{
	if (nItem==0) {
		First = P;
		P->SetNext(0);
		P->SetPrev(0);
	} else {
		Last->SetNext(P);
		P->SetPrev(Last);
		P->SetNext(0);
	}
	Last = P;
	nItem++;
}


int CPoisson1D::Init(double xo)
{
	assert(nItem>2 && nItem%2==1);

	CPoisson1DItem* P;

	M = 0; 
	P = First;
	while(P!=0) {
		M += P->get_m();
		P->SetBeta(beta);
		P = P->GetNext();
	}

	x  = CArray1D(M);
	h  = CArray1D(M);
	V  = CArray1D(M,0.);
	CP = CArray1D(M,0.);
	F  = CArray1D(M);

	Q  = CArray2D(M,N_CHARGE_TYPES);
	dQ = CArray2D(M,N_CHARGE_TYPES);

	T = CTridiagSym2D(M);
	A = CTridiagSym2D(M);
	C = CTridiag2D(M);
	b = CArray1D(M);
	cC = CComplexTridiag2D(M);
	cb = CComplexArray1D(M);

	int i = 0;
	P = First;
	while (P!=0) {
		P->Assign(this, i);
		P = P->GetNext();
	}
	assert(i==M);


	x[0] = xo;
	P = First;
	while (P!=0) { P->MakeGrid(); P = P->GetNext();	}
	for(i=0; i<M-1; i++) h[i]=x[i+1]-x[i];
	h[M-1]=h[M-2];

	P = First;
	while (P!=0) {
		P->MakeMatrix();
		P = P->GetNext();
	}

	P = First;
	while (P!=0) {
		P->CalcCP();
		P = P->GetNext();
	}

	CPoisson1DItem* pBC0 = First;
	CPoisson1DItem* pBCM = Last;

	CPoissonBC BC0(CPoissonBC::field,0.);
	CPoissonBC BCM(CPoissonBC::field,0.);
	BC0.SetBeta(beta);
	BCM.SetBeta(beta);
	BC0.SetNext(pBC0->GetNext());
	BCM.SetPrev(pBCM->GetPrev());
	First = &BC0;
	Last = &BCM;

	i=0;
	BC0.Assign(this, i);
	BC0.MakeMatrix();
	i=M-1;
	BCM.Assign(this, i);
	BCM.MakeMatrix();


	double ret = Solve();
	CP.inject(V);

	First = pBC0;
	Last  = pBCM;
	pBC0->MakeMatrix();
	pBCM->MakeMatrix();
	
	return (ret>0.);
}

void CPoisson1D::Dump(ostream &s)
{
	s << "x\tCP\tV\tF\tQe\tQh\tQf\tQtot\tdQe\tdQh\tdQf\tdQtot\n";
	CPoisson1DItem* P = First;
	while(P!=0) { P->Dump(s); P = P->GetNext(); }
}
void CPoisson1D::Dump(const char* s)
{
	ofstream foo(s);
	foo.setf(ios::scientific);
	Dump(foo);
	foo.close();
}

double CPoisson1D::Iter(int hf)
{
	CPoisson1DItem* P = First;
	while(P!=0) { P->CalcQ(hf); P = P->GetNext(); }
	P = First;
	while(P!=0) { P->FixBoundaryCharge(); P = P->GetNext(); }
	P = First;
	while(P!=0) { P->PrepareForSolve(); P = P->GetNext(); }

	Dgtsl(C, b);

	err = 0.;
	int i;
	for(i=0; i<M; i++) err += fabs(b[i]);
	err /= M;

	//double cc = conv;
	//if (err>1.) cc /= err;
	for(i=0; i<M; i++) V[i] = V[i] + conv*b[i];

	itcount++;
	return err;
}
double CPoisson1D::CalcC_(int hf)
{
	CPoisson1DItem* P = First;
	while(P!=0) { P->PrepareForCalcC(hf); P = P->GetNext(); }

	Dgtsl(C, b);

	int N = M-1;
	double c = T.s(1)*(b[1]-b[0]);
	c = T.s(N)*(b[N]-b[N-1]) - c;
	return fabs(c)*VACUUM_KAPPA/L0; 
}

cdouble CPoisson1D::CalcC(double omega)
{
	CPoisson1DItem* P = First;

	P = First;
	while(P!=0) { P->PrepareForCalcC(omega); P = P->GetNext(); }

	Dgtsl(cC, cb);

	int N = M-1;
	cdouble c = T.s(1)*(cb[1]-cb[0]);
	c = T.s(N)*(cb[N]-cb[N-1]) - c;
	return c*VACUUM_KAPPA/L0; 
}

bool CPoisson1D::Continue() 
{ 
	bool fl = (err>eps) && (itcount<itmax); 
	if (!fl && err<eps) {
		CPoisson1DItem* P = First;
		while(P!=0) { P->CalcQ(); P = P->GetNext(); }
	}
	return fl;
}

double CPoisson1D::Solve(double aeps, double aconv, int aitmax, int hf)
{
	InitSolve(aeps,aconv,aitmax);

	while (Continue()) {
		Iter();
	}

	if (itcount>=itmax) return -1;
	else return err;
}
double CPoisson1D::GetTotalQ()
{
	int N = M-1;
	double c = T.s(1)*(V[1]-V[0]) - A.s(1)*(3*Q[0][0] + Q[1][0]);
	c = T.s(N)*(V[N]-V[N-1]) + A.s(N)* (3*Q[N][0] + Q[N-1][0]) - c;
	return c*VACUUM_KAPPA/L0/beta;
}
double CPoisson1D::CalcTotalQ()
{
	int i=0;
	double qt = 0;
	int N=M-1;

	qt += Q[i][0]*h[i]/2;

	for(i=1; i<N; i++)
	{
		qt += Q[i][0]*(h[i]+h[i-1])/2;
	}

	qt += Q[i][0]*h[i-1]/2;

	return qt*(N0*L0/beta*ELECTRON_CHARGE);

}
/*
double CPoisson1D::SolveRefine(double delta, double eps, double aconv, int itmax)
{
	int it = 0;
	conv = aconv;
	assert(delta<1.0);

	// Fisrt Solve
	do {
		Iter();
		it++;
#ifdef _DEBUG
		cout << it << "\t" << err << endl;
		cout.flush();
#endif

	} while(err>eps && it<itmax);

	CPoisson1DItem* P;

	// Then refine
	do {
		P = First;
		while(P!=0) { P->RefineGrid(delta); P = P->GetNext(); }
		for(int i=0; i<M-1; i++) h[i]=x[i+1]-x[i];
		h[M-1]=h[M-2];

		P = First;
		while (P!=0) { P->MakeMatrix(); P = P->GetNext(); }

		it++;
		Iter();

#ifdef _DEBUG
		cout << it << "\t" << err << endl;
		cout.flush();
#endif

	} while(err>eps && it<itmax);

	
	P = First;
	while(P!=0) { P->CalcQ(); P = P->GetNext(); }

	if (it>=itmax) return -1;
	else return err;
}
*/
////////////////////////////////////////////////////////////////////////////////

int CSCPS::Init(double xo)
{
	assert(nItem>2 && nItem%2==1);

	CPoisson1DItem* P;

	M = 0;
	Ms = 0;
	P = First;
	while(P!=0) {
		M += P->get_m();
		if (P->is_quant()) {
			if (Ms==0) iQstart = M-1;
			Ms += P->get_m();
		}
		P->SetBeta(beta);
		P = P->GetNext();
	}

	x  = CArray1D(M);
	h  = CArray1D(M);
	V  = CArray1D(M,0.);
	CP = CArray1D(M,0.);
	F  = CArray1D(M);
	DC = CArray2D(M,M);
	cDC = CComplexArray2D(M,M);
	Idx = CIntArray1D(M);

	Q  = CArray2D(M,N_CHARGE_TYPES);
	dQ = CArray2D(M,N_CHARGE_TYPES);

	Qe  = CArray1D(Ms);
	dQe = CArray1D(Ms);
	Qh  = CArray1D(Ms);
	dQh = CArray1D(Ms);
	L = CArray1D(Ms);
	DeltaE = CArray2D(Ms,Ms);
	DeltaH = CArray2D(Ms,Ms);
	Delta = CArray2D(Ms,Ms);

	T = CTridiagSym2D(M);
	A = CTridiagSym2D(M);
	C = CTridiag2D(M);
	b = CArray1D(M);
	cC = CComplexTridiag2D(M);
	cb = CComplexArray1D(M);

	int ip = 0, is = 0;
	P = First;
	while (P!=0) {
		P->Assign(this, ip, is);
		P = P->GetNext();
	}
	assert(ip==M && is==Ms);


	x[0] = xo;
	P = First;
	while (P!=0) { P->MakeGrid(); P = P->GetNext();	}
	for(int i=0; i<M-1; i++) h[i]=x[i+1]-x[i];
	h[M-1]=h[M-2];
	for(i=1; i<Ms; i++) {
		int j = i + iQstart;
		L[i]=sqrt((h[j]+h[j-1])/2);
	}
	L[0] = sqrt(h[iQstart]);

	Electrons.Init(Ms,&x[iQstart],&h[iQstart],&L[0],&V[iQstart],&Qe[0],&dQe[0],&DeltaE[0][0]);
	Holes.Init(Ms,&x[iQstart],&h[iQstart],&L[0],&V[iQstart],&Qh[0],&dQh[0],&DeltaH[0][0]);

	P = First;
	while (P!=0) {
		P->MakeMatrix();
		P = P->GetNext();
	}

	P = First;
	while (P!=0) {
		P->CalcCP();
		P = P->GetNext();
	}

	CPoisson1DItem* pBC0 = First;
	CPoisson1DItem* pBCM = Last;

	CPoissonBC BC0(CPoissonBC::field,0.);
	CPoissonBC BCM(CPoissonBC::field,0.);
	BC0.SetBeta(beta);
	BCM.SetBeta(beta);
	BC0.SetNext(pBC0->GetNext());
	BCM.SetPrev(pBCM->GetPrev());
	First = &BC0;
	Last = &BCM;

	i=0;
	BC0.Assign(this, i);
	BC0.MakeMatrix();
	i=M-1;
	BCM.Assign(this, i);
	BCM.MakeMatrix();


	double ret = Solve();
	CP.inject(V);

	First = pBC0;
	Last  = pBCM;
	pBC0->MakeMatrix();
	pBCM->MakeMatrix();
	
	return (err<eps);
}

double CSCPS::Iter(int hf)
{
	//int iQend = iQstart + Ms - 1;
	//double dV = V[iQstart] - V[iQend];
	//Holes.Ns = 0;
	//Electrons.Ns = 0;
	//if (fabs(dV)>=0) {  // (fabs(dV)>0.2/KT300) {
	//if (V[iQstart]<V[iQend]) {
	Electrons.Solve(1e-15); //(eps);
	Electrons.CalcQ();
	Electrons.CalcNs();
	//Electrons.FixTail2();
	//}
	//else if (V[iQstart]>V[iQend]) {
	Holes.Solve(1e-15); //(eps);
	Holes.CalcQ();
	Holes.CalcNs();
	//Holes.FixTail2();
	//}
	//else {
	//}
	return CPoisson1D::Iter(hf);
}



double CSCPS::CalcC_(int hf)
{
	int i,j,k;

	Electrons.CalcDelta();
	Holes.CalcDelta();
	Delta = 0.;
	CPoisson1DItem* P = First;
	while(P!=0) { P->PrepareForCalcC(hf,DeltaE,DeltaH,Delta); P = P->GetNext(); }


	// init DC matrix
	DC = 0.;

	// copy triangular part
	i = 0;
	DC[i][i] += C.d(i);
	DC[i][i+1] += C.e(i);
	i++;
	
	for(; i<M-1; i++) {
		DC[i][i] += C.d(i);
		DC[i][i+1] += C.e(i);
		DC[i][i-1] += C.s(i);
	}
	
	DC[i][i] += C.d(i);
	DC[i][i-1] += C.s(i);

	//copy quantum diff

	// complete symmetric Delta matrix
	for(i=0; i<Ms; i++) {
		for(j=i; j>=0; j--) {			
			Delta[i][j] = Delta[j][i];
		}
	}

	// multiply by L
	for(j=0; j<Ms; j++) {
		double l = L[j];
		l *= l;
		for(i=0; i<Ms; i++) {
			Delta[i][j] *= l;
		}
	}

	i = iQstart - 1;
	k = i-iQstart;

	
	if (i>=0) for(j=0; j<Ms; j++) DC[i][j+iQstart] -= A.e(i)*Delta[k+1][j];
	i++; k++;
	
	for(j=0; j<Ms; j++) DC[i][j+iQstart] -= (A.d(i)*Delta[k][j] + A.e(i)*Delta[k+1][j]);
	i++; k++;
	
	int iQend = iQstart + Ms;
	for(; i<iQend-1; i++) {
		for(j=0; j<Ms; j++) {
			DC[i][j+iQstart] -= (
				A.e(i)*Delta[k+1][j] +
				A.d(i)*Delta[k][j] +
				A.s(i)*Delta[k-1][j]);
		}
		k++;
	}
	
	for(j=0; j<Ms; j++) DC[i][j+iQstart] -= (A.d(i)*Delta[k][j] + A.s(i)*Delta[k-1][j]);
	i++; k++;

	for(j=0; j<Ms; j++)	DC[i][j+iQstart] -= A.s(i)*Delta[k-1][j];
	
	int ret = Ludcmp(DC , Idx, i);
	Lubksb(DC, Idx, b);

	int N = M-1;
	double c = T.s(1)*(b[1]-b[0]);
	c = T.s(N)*(b[N]-b[N-1]) - c;
	return fabs(c)*VACUUM_KAPPA/L0; 

}

cdouble CSCPS::CalcC(double omega)
{
	int i,j,k;

	Electrons.CalcDelta();
	Holes.CalcDelta();
	Delta = 0.;
	CPoisson1DItem* P = First;
	while(P!=0) { P->PrepareForCalcC(omega,DeltaE,DeltaH,Delta); P = P->GetNext(); }


	// 1. init complex MxM matrix to zero
	cDC = 0.;

	// 2. copy tridiagonal part
	i = 0;
	cDC[i][i] += cC.d(i);
	cDC[i][i+1] += cC.e(i);
	i++;
	
	for(; i<M-1; i++) {
		cDC[i][i] += cC.d(i);
		cDC[i][i+1] += cC.e(i);
		cDC[i][i-1] += cC.s(i);
	}
	
	cDC[i][i] += cC.d(i);
	cDC[i][i-1] += cC.s(i);

	// 3. copy quantum diff
	// 3a. complete symmetric Delta matrix
	for(i=0; i<Ms; i++) {
		for(j=i; j>=0; j--) {			
			Delta[i][j] = Delta[j][i];
		}
	}

	// 3b. multiply by L
	for(j=0; j<Ms; j++) {
		double l = L[j];
		l *= l;
		for(i=0; i<Ms; i++) {
			Delta[i][j] *= l;
		}
	}

	// 3c. do the copy
	i = iQstart - 1;
	k = i-iQstart;

	for(j=0; j<Ms; j++) cDC[i][j+iQstart] -= A.e(i)*Delta[k+1][j];
	i++; k++;
	
	for(j=0; j<Ms; j++) cDC[i][j+iQstart] -= (A.d(i)*Delta[k][j] + A.e(i)*Delta[k+1][j]);
	i++; k++;
	
	int iQend = iQstart + Ms;
	for(; i<iQend-1; i++) {
		for(j=0; j<Ms; j++) {
			cDC[i][j+iQstart] -= (
				A.e(i)*Delta[k+1][j] +
				A.d(i)*Delta[k][j] +
				A.s(i)*Delta[k-1][j]);
		}
		k++;
	}
	
	for(j=0; j<Ms; j++) cDC[i][j+iQstart] -= (A.d(i)*Delta[k][j] + A.s(i)*Delta[k-1][j]);
	i++; k++;

	for(j=0; j<Ms; j++)	cDC[i][j+iQstart] -= A.s(i)*Delta[k-1][j];

	// 4. solve complex system by LU decomp.
	int ret = Ludcmp(cDC , Idx, i);
	Lubksb(cDC, Idx, cb);

	// 5. return the complex capacitance
	int N = M-1;
	cdouble c = T.s(1)*(cb[1]-cb[0]);
	c = T.s(N)*(cb[N]-cb[N-1]) - c;
	return c*VACUUM_KAPPA/L0; 

}