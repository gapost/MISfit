#include "stdafx.h"
#include <fstream>

#define VMAX 2.5
#define NMIN 13
#define NMAX 18
#define NV 151
#define NN 21

void printarray(const char* fname, const char* declstr, const CArray1D& A, char* code = NULL);

void printarray(const char* fname, const char* declstr, const CArray2D& A, char* code = NULL);

#define DX0 0.1

void CreateClassicalGe(CPoisson1D& P, double Nd, double Na)
{
	double W = 2*sqrt(2*16*N0/fabs(Na-Nd));
	
	double Nd1=0., Na1=0;
	if (Nd>Na) Nd1=1e20;
	else Na1=1e20;

	int Nsi = 100;

	int N1 = Nsi/10;
	if (N1<2) N1=2;
	

	P.Add( new CPoissonBC(CPoissonBC::voltage, 0.) );
	P.Add( new CClassicalGeLayer(new CLogGrid(Nsi,W,DX0), Nd, Na) );
	P.Add( new CPoissonBC(CPoissonBC::cont, 0.) );
	P.Add( new CClassicalGeLayer(new CLinearGrid(N1,10.), Na1, Nd1) );
	P.Add( new CPoissonBC(CPoissonBC::field, 0.) );
}

void CalcQNdGe()
{
	CArray2D Q(NN,NV);
	CArray2D C(NN,NV);
	CArray1D V(NV), logNd(NN);

	double dV = 1e-6;
	double dN = 1e-3;

	int i,j;

	for(i=0; i<NN; i++) {
		logNd[i] = 1.0*i*(NMAX - NMIN)/(NN-1) + NMIN ;
	}

	for(i = 0; i<NV; i++) V[i] = i*2.*VMAX/(NV-1) - VMAX;

	for(i=0; i<NN; i++) 
	{
		cout << i << endl;

		CPoisson1D P;
		CreateClassicalGe(P,pow(10.,logNd[i]),0.);

		P.Init();

		CPoissonBC* pBC0 = (CPoissonBC*)P.GetFirst(); 
		
		for(j = 0; j<NV; j++) {
			pBC0->SetVb(V[j]);
			P.Solve(1e-9);
			Q[i][j] = P.GetTotalQ();
			C[i][j] = P.CalcChf();
		}
	}

	char a;

/*
	CGraphFrame GF("",2,1);
	GF.GetGraphWnd()->plot(V,Q);
	GF.GetGraphWnd()->subplot(2);
	GF.GetGraphWnd()->plot(V,C);
	cin >> a;
*/
	a = 'f';
	printarray("inc\\V_ge.h","float V_Ge",V,&a);
	printarray("inc\\logNd_ge.h","float logNd_Ge",logNd,&a);
	
	printarray("inc\\QNdCl_ge.h","float Q_Nd_Cl_Ge",Q,&a);
	
	printarray("inc\\CNdCl_ge.h","float C_Nd_Cl_Ge",C,&a);
}

void CalcQNaGe()
{
	CArray2D Q(NN,NV);
	CArray2D C(NN,NV);
	CArray1D V(NV), logNa(NN);

	double dV = 1e-6;
	double dN = 1e-3;

	int i,j;

	for(i=0; i<NN; i++) {
		logNa[i] = 1.0*i*(NMAX - NMIN)/(NN-1) + NMIN ;
	}

	for(i = 0; i<NV; i++) V[i] = i*2.*VMAX/(NV-1) - VMAX;

	for(i=0; i<NN; i++) 
	{
		cout << i << endl;

		CPoisson1D P;
		CreateClassicalGe(P,0.,pow(10.,logNa[i]));

		P.Init();

		CPoissonBC* pBC0 = (CPoissonBC*)P.GetFirst(); 
		
		for(j = 0; j<NV; j++) {
			pBC0->SetVb(V[j]);
			P.Solve(1e-9);
			Q[i][j] = P.GetTotalQ();
			C[i][j] = P.CalcChf();
		}
	}

	char a;

/*	CGraphFrame GF("",2,1);
	GF.GetGraphWnd()->plot(V,Q);
	GF.GetGraphWnd()->subplot(2);
	GF.GetGraphWnd()->plot(V,C);

	cin >> a;
*/
	a = 'f';
	printarray("inc\\V_Ge.h","float V_Ge",V,&a);
	printarray("inc\\logNd_Ge.h","float logNd_Ge",logNa,&a);
	
	printarray("inc\\QNaCl_Ge.h","float Q_Na_Cl_Ge",Q,&a);
	
	printarray("inc\\CNaCl_Ge.h","float C_Na_Cl_Ge",C,&a);
}
