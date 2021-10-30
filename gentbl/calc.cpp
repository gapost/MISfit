#include "stdafx.h"
#include <fstream>

#define VMAX 2.5
#define NMIN 13
#define NMAX 18
#define NV 151
#define NN 21

void printarray(const char* fname, const char* declstr, const CArray1D& A, char* code = NULL)
{
	ofstream foo(fname);
	foo.flags(foo.flags() | ios::showpoint);   

	int i,k=1,n=A.dim();
	foo << declstr << "[" << n << "] =\n{\n\t" << A[0];
	if (code) foo << *code;

	for(i=1; i<n; i++)
	{
		foo << ", ";
		k++;
		if (k==5) {
			foo << "\n\t";
			k=0;
		}
		foo << A[i];
		if (code) foo << *code;
	}

	foo << "\n};\n";

	foo.close();
}

void printarray(const char* fname, const char* declstr, const CArray2D& A, char* code = NULL)
{
	ofstream foo(fname);
	foo.flags(foo.flags() | ios::showpoint);   

	int i,j,k,n=A.dim1(),m=A.dim2();

	foo << declstr << "[" << n << "][" << m << "] =\n{\n";

	for(i=0; i<n; i++)
	{
		foo << "\t{ " << A[i][0];
		if (code) foo << *code;
		k=1;
		for(j=1; j<m; j++) {
			foo << ", ";
			k++;
			if (k==5) {
				foo << "\n\t";
				k=0;
			}
			foo << A[i][j];
			if (code) foo << *code;
		}
		if (i<n-1) foo << " },\n";
		else foo << " }\n";
	}

	foo << "};\n";

	foo.close();
}

#define DX0 0.1

void CreateClassicalSi(CPoisson1D& P, double Nd, double Na)
{
	double W = 2*sqrt(2*11.9*N0/fabs(Na-Nd));
	
	double Nd1=0., Na1=0;
	if (Nd>Na) Nd1=1e20;
	else Na1=1e20;

	int Nsi = 100;

	int N1 = Nsi/10;
	if (N1<2) N1=2;
	

	P.Add( new CPoissonBC(CPoissonBC::voltage, 0.) );
	P.Add( new CClassicalSiLayer(new CLogGrid(Nsi,W,DX0), Nd, Na) );
	P.Add( new CPoissonBC(CPoissonBC::cont, 0.) );
	P.Add( new CClassicalSiLayer(new CLinearGrid(N1,10.), Na1, Nd1) );
	P.Add( new CPoissonBC(CPoissonBC::field, 0.) );
}

void CalcQNd()
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
		CreateClassicalSi(P,pow(10.,logNd[i]),0.);

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


	CGraphFrame GF("",2,1);
	GF.GetGraphWnd()->plot(V,Q);
	GF.GetGraphWnd()->subplot(2);
	GF.GetGraphWnd()->plot(V,C);
	cin >> a;

	a = 'f';
	printarray("inc\\V.h","float V",V,&a);
	printarray("inc\\logNd.h","float logNd",logNd,&a);
	
	printarray("inc\\QNdCl.h","float Q_Nd_Cl",Q,&a);
	
	printarray("inc\\CNdCl.h","float C_Nd_Cl",C,&a);
}

void CalcQNa()
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
		CreateClassicalSi(P,0.,pow(10.,logNd[i]));

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

	/*CGraphFrame GF("",2,1);
	GF.GetGraphWnd()->plot(V,Q);
	GF.GetGraphWnd()->subplot(2);
	GF.GetGraphWnd()->plot(V,C);

	cin >> a;*/

	a = 'f';
	printarray("inc\\V.h","float V",V,&a);
	printarray("inc\\logNd.h","float logNd",logNd,&a);
	
	printarray("inc\\QNaCl.h","float Q_Na_Cl",Q,&a);
	
	printarray("inc\\CNaCl.h","float C_Na_Cl",C,&a);
}
