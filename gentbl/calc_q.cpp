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

void CreateQuantumSi(CSCPS& P, double Nd, double Na)
{
	double W = 2*sqrt(2*11.9*N0/fabs(Na-Nd));
	
	double Nd1=0., Na1=0;
	if (Nd>Na) Nd1=1e20;
	else Na1=1e20;

	int Nsi=100;

	int N1 = Nsi/10;
	if (N1<2) N1=2;
	

	P.Add( new CQuantumPoissonBC(CPoissonBC::voltage, 0.) );
	P.Add( new CQuantumSiLayer(new CLogGrid(Nsi,W,DX0), Nd, Na) );
	P.Add( new CQuantumPoissonBC(CPoissonBC::cont, 0.) );
	P.Add( new CClassicalSiLayer(new CLinearGrid(N1,10.), Na1, Nd1) );
	P.Add( new CPoissonBC(CPoissonBC::field, 0.) );

	CMultiBandSchrodinger1D& E = P.GetElectrons();
	E.AddBand( new CSchrodinger1D(1) );
	E.AddBand( new CSchrodinger1D(1) );
	
	E.Add( 0, new CSchrodingerBC(CSchrodingerBC::box) );
	E.Add( 1, new CSchrodingerBC(CSchrodingerBC::box) );
	
	E.Add( 0, new CSiElectronLow(new CLogGrid(Nsi,W,DX0)) );
	E.Add( 1, new CSiElectronHigh(new CLogGrid(Nsi,W,DX0)) );
	
	E.Add( 0, new CSchrodingerBC(CSchrodingerBC::box) );
	E.Add( 1, new CSchrodingerBC(CSchrodingerBC::box) );
	
	CMultiBandSchrodinger1D& H = P.GetHoles();
	H.AddBand( new CSchrodinger1D(-1) );
	H.AddBand( new CSchrodinger1D(-1) );
	H.AddBand( new CSchrodinger1D(-1) );
	
	H.Add( 0, new CSchrodingerBC(CSchrodingerBC::box, -1) );
	H.Add( 1, new CSchrodingerBC(CSchrodingerBC::box, -1) );
	H.Add( 2, new CSchrodingerBC(CSchrodingerBC::box, -1) );
	
	H.Add( 0, new CSiLightHole(new CLogGrid(Nsi,W,DX0)) );
	H.Add( 1, new CSiHeavyHole(new CLogGrid(Nsi,W,DX0)) );
	H.Add( 2, new CSiSplitoffHole(new CLogGrid(Nsi,W,DX0)) );
	
	H.Add( 0, new CSchrodingerBC(CSchrodingerBC::box, -1) );
	H.Add( 1, new CSchrodingerBC(CSchrodingerBC::box, -1) );
	H.Add( 2, new CSchrodingerBC(CSchrodingerBC::box, -1) );
}

void CalcQNd_Q()
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

		CSCPS P;
		CreateQuantumSi(P,pow(10.,logNd[i]),0.);
		
		P.Init();
		
		CPoissonBC* pBC0 = (CPoissonBC*)P.GetFirst();
		
		for(j = 0; j<NV; j++) {
			cout << "QNdQ : " << i << " " << j << endl;
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
	
	printarray("inc\\QNdQ.h","float Q_Nd_Q",Q,&a);
	
	printarray("inc\\CNdQ.h","float C_Nd_Q",C,&a);
}

void CalcQNa_Q()
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

		CSCPS P;
		CreateQuantumSi(P,0.,pow(10.,logNd[i]));
		
		P.Init();
		
		CPoissonBC* pBC0 = (CPoissonBC*)P.GetFirst();

		
		for(j = 0; j<NV; j++) {
			cout << "QNaQ : " << i << " " << j << endl;
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
	
	printarray("inc\\QNaQ.h","float Q_Na_Q",Q,&a);
	
	printarray("inc\\CNaQ.h","float C_Na_Q",C,&a);
}
