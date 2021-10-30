// GE_MIS.cpp: implementation of the CMIS class for Germanium.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MIS.h"
#include "SiLayer.h"
#include "SchrodingerBands.h"

#define DX0 0.1

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClGeMIS::CClGeMIS(double tox, double kox, double Nd, double Na, CQitArray* aQit, double Qf,
			   bool fermi, int quality, double T) : P(T)
{
	assert( fabs(Na-Nd)>0 );
	
	double W = 2*sqrt(2*16*N0/fabs(Na-Nd));
	
	double Nd1=0., Na1=0;
	if (Nd>Na) Nd1=1e20;
	else Na1=1e20;

	int Nox,Nsi;
	switch(quality)
	{
	case QUALITY_LOW:
		{
			if (Qf==0.) Nox = 2;
			else Nox = 20;
			Nsi = 80;
		}
		break;
	case QUALITY_NORMAL:
		{
			if (Qf==0.) Nox = 2;
			else Nox = 40;
			Nsi = 100;
		}
		break;
	case QUALITY_HIGH:
		{
			if (Qf==0.) Nox = 2;
			else Nox = 80;
			Nsi = 150;
		}
		break;
	}
	int N1 = Nsi/10;
	if (N1<2) N1=2;
	

	pBC0 = new CPoissonBC(CPoissonBC::voltage, 0.);
	P.Add( pBC0 );
	P.Add( new CDielectricLayer( new CLinearGrid(Nox, tox) , kox, Qf) );

	if (aQit==0)
		pBCs = new CPoissonBC(CPoissonBC::cont, 0.);
	else
		pBCs = new CPoissonBC_Dit(aQit);

	P.Add( pBCs );

	P.Add( new CClassicalGeLayer(new CLogGrid(Nsi,W,DX0), Nd, Na, fermi) );
	P.Add( new CPoissonBC(CPoissonBC::cont, 0.) );
	P.Add( new CClassicalGeLayer(new CLinearGrid(N1,10.), Na1, Nd1) );

	pBC1 = new CPoissonBC(CPoissonBC::field, 0.);
	P.Add( pBC1 );

	Cox_ = kox*VACUUM_KAPPA/tox/L0;

	isok = P.Init();

	bFermi = fermi;

}
double CClGeMIS::SetVb(double aVb)
{
	if (!isok) return -1.;
	if (fabs(Vb_-aVb)>1e-9) {
		pBC0->SetVb(aVb);
		Vb_ = aVb;
		if (bFermi)
			return P.Solve(1e-6,0.1,1000);
		else
			return P.Solve(1e-6);
	}
	else return 0.;
}
/***********************************************************************/
CQGeMIS::CQGeMIS(double tox, double kox, double Nd, double Na, CQitArray* aQit, double Qf,
			 bool fermi, int quality, double T): P(T) 
{
	assert( fabs(Na-Nd)>0 );
	
	double W = 2*sqrt(2*16*N0/fabs(Na-Nd));
	
	double Nd1=0., Na1=0;
	if (Nd>Na) Nd1=1e20;
	else Na1=1e20;

	int Nox,Nsi;
	switch(quality)
	{
	case QUALITY_LOW:
		{
			if (Qf==0.) Nox = 2;
			else Nox = 20;
			Nsi = 80;
		}
		break;
	case QUALITY_NORMAL:
		{
			if (Qf==0.) Nox = 2;
			else Nox = 40;
			Nsi = 100;
		}
		break;
	case QUALITY_HIGH:
		{
			if (Qf==0.) Nox = 2;
			else Nox = 80;
			Nsi = 150;
		}
		break;
	}
	int N1 = Nsi/10;
	if (N1<2) N1=2;
	

	pBC0 = new CPoissonBC(CPoissonBC::voltage, 0.);
	P.Add( pBC0 );
	P.Add( new CDielectricLayer( new CLinearGrid(Nox, tox) , kox, Qf) );

	if (aQit==0)
		pBCs = new CQuantumPoissonBC(CPoissonBC::cont, 0.);
	else
		pBCs = new CQuantumPoissonBC_Dit(aQit);

	P.Add( pBCs );

	P.Add( new CQuantumGeLayer(new CLogGrid(Nsi,W,DX0), Nd, Na) );
	P.Add( new CQuantumPoissonBC(CPoissonBC::cont, 0.) );
	P.Add( new CClassicalGeLayer(new CLinearGrid(N1,10.), Na1, Nd1) );

	pBC1 = new CPoissonBC(CPoissonBC::field, 0.);
	P.Add( pBC1 );

	CMultiBandSchrodinger1D& E = P.GetElectrons();
	E.AddBand( new CSchrodinger1D(1) );
	
	E.Add( 0, new CSchrodingerBC(CSchrodingerBC::box) );
	
	E.Add( 0, new CGeElectron(new CLogGrid(Nsi,W,DX0)) );
	
	E.Add( 0, new CSchrodingerBC(CSchrodingerBC::box) );
	
	CMultiBandSchrodinger1D& H = P.GetHoles();
	H.AddBand( new CSchrodinger1D(-1) );
	H.AddBand( new CSchrodinger1D(-1) );
	H.AddBand( new CSchrodinger1D(-1) );
	
	H.Add( 0, new CSchrodingerBC(CSchrodingerBC::box, -1) );
	H.Add( 1, new CSchrodingerBC(CSchrodingerBC::box, -1) );
	H.Add( 2, new CSchrodingerBC(CSchrodingerBC::box, -1) );
	
	H.Add( 0, new CGeLightHole(new CLogGrid(Nsi,W,DX0)) );
	H.Add( 1, new CGeHeavyHole(new CLogGrid(Nsi,W,DX0)) );
	H.Add( 2, new CGeSplitoffHole(new CLogGrid(Nsi,W,DX0)) );
	
	H.Add( 0, new CSchrodingerBC(CSchrodingerBC::box, -1) );
	H.Add( 1, new CSchrodingerBC(CSchrodingerBC::box, -1) );
	H.Add( 2, new CSchrodingerBC(CSchrodingerBC::box, -1) );


	Cox_ = kox*VACUUM_KAPPA/tox/L0;

	isok = P.Init();

}
double CQGeMIS::SetVb(double aVb)
{
	if (!isok) return -1.;
	if (fabs(Vb_-aVb)>1e-6) {
		pBC0->SetVb(aVb);
		Vb_ = aVb;
		return P.Solve(1e-4);
	}
	else return 0.;
}

