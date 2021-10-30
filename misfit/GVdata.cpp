// GVdata.cpp: implementation of the CGVdata class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "misfit.h"
#include "GVdata.h"
#include <fstream>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static char CLR_CHAR[N_DATA_MAX] = { 'b','r','g','m','s','b','r','g','m','s' };

void GsGpCorr(double& Cc, double& Gc, float f, float Gs, float Gp)
{
	std::complex<float> Y(Gc,Cc);
	Y *= f;
	Y *= 1e-6f;
	Y += Gp;
	Y = Gs*Y/(Gs+Y);
	Gc = Y.real()/f*1e6;
	Cc = Y.imag()/f*1e6;

	/*float Cd = C.real(), Gd = -f*C.imag();
	std::complex<float> Y(Gp + Gd,f*Cd);
	Y = Gs*Y/(Gs+Y);
	Gm = Y.real()/f;
	Cm = Y.imag()/f;*/
}

void GsGpUnCorr(double& Cc, double& Gc, float f, float Gs, float Gp)
{
	std::complex<float> Y(Gc,Cc);
	Y *= f;
	Y *= 1e-6f;
	Y = Y*Gs/(Gs-Y) - Gp;
	Gc = Y.real()/f*1e6;
	if (Gc<0.f) Gc = 0.f;
	Cc = Y.imag()/f*1e6;
}


/////////////////////////////////////////////////////////////////
//                  CData

void CModelData::Plot(CGraphWnd *pWnd, const CPlotOptions& PO, int i)
{
	CString O1(". "),O2("- ");
	O1.SetAt(1,CLR_CHAR[i]);
	O2.SetAt(1,CLR_CHAR[i]);
	
	CArray1D x,y1,y2;

	CPlotOptions::TypeX tx = (CPlotOptions::TypeX)(PO.typex);
	switch(tx)
	{
	case CPlotOptions::x_Vg:
		x = V;
		break;
	case CPlotOptions::x_Vs:
		x = Vs;
		break;
	case CPlotOptions::x_Efs:
		x = Efs;
		break;
	}

	CPlotOptions::TypeY ty = (CPlotOptions::TypeY)(PO.typey);
	switch(ty)
	{
	case CPlotOptions::y_C_G:
		y1 = Cf;
		y2 = Gf;
		break;
	case CPlotOptions::y_Qs_Qit:
		y1 = Qs;
		y2 = Qit;
		break;
	case CPlotOptions::y_Cs_Qs:
		y1 = Cs;
		y2 = Qs;
		break;
	}
	
	pWnd->subplot(1); 
	pWnd->plot(x,y1,O2);
	pWnd->subplot(2);
	pWnd->plot(x,y2,O2);
}

void CModelData::CalcGV(CMISitInterpBase *pMIS, float Gs, float Gp, const CPlotOptions& PO)
{
	float Cox = pMIS->Cox();
	
	int n = V.dim();
	for(int j=0; j<n; j++) {
		cdouble cc = pMIS->Ctot(V[j], freq);
		Cf[j] = cc.real()*1e6;
		Gf[j] = -cc.imag()*1e6;
		if (!PO.GsCorrect) GsGpCorr(Cf[j],Gf[j],freq,Gs,Gp);
		Efs[j] = pMIS->Efs();
		Vs[j] = pMIS->Vs();
		Qit[j] = fabs(pMIS->Qit(Vs[j])); 
		Qs[j] = fabs(pMIS->Qs(Vs[j])); 
		Cs[j] = pMIS->Cs(Vs[j])*1e6;
	}
}

void CModelData::Save(const char *fname, const CPlotOptions& PO)
{
	const char* labels = 
		"      Vg      \t"
		"      Vs      \t"
		"    Efs-Ei    \t"
		"      C       \t"
		"     G/w      \t"
		"      Cs      \t"
		"    |Qs|      \t"
		"    |Qit|     \n";

	const char* units = 
		"     (V)      \t"
		"     (eV)     \t"
		"     (eV)     \t"
		"   (uF/cm2)   \t"
		"   (uF/cm2)   \t"
		"   (uF/cm2)   \t"
		"   (C/cm2)    \t"
		"   (C/cm2)    \n";

	ofstream os(fname);

	long formatmask = os.setf(ios::scientific);

	os << labels;
	os << units;

	int N = V.dim(), i;
	for (i=0; i<N; ++i)
	{
		os << V[i] << '\t';
		os << Vs[i] << '\t';
		os << Efs[i] << '\t';
		os << Cf[i] << '\t';
		os << Gf[i] << '\t';
		os << Cs[i] << '\t';
		os << Qs[i] << '\t';
		os << Qit[i] << '\n';
	}

	os.close();

}

void CExpData::Plot(CGraphWnd *pWnd, const CPlotOptions& PO, int i)
{
	if (PO.typey > (int)CPlotOptions::y_C_G)
	{
		CModelData::Plot(pWnd, PO, i);
		return;
	}

	CString O1(". "),O2("- ");
	O1.SetAt(1,CLR_CHAR[i]);
	O2.SetAt(1,CLR_CHAR[i]);

	CArray1D x,y1,y2;
	CPlotOptions::TypeX tx = (CPlotOptions::TypeX)(PO.typex);
	CPlotOptions::TypeY ty = (CPlotOptions::TypeY)(PO.typey);

	switch(tx)
	{
	case CPlotOptions::x_Vg:
		x = V;
		break;
	case CPlotOptions::x_Vs:
		x = Vs;
		break;
	case CPlotOptions::x_Efs:
		x = Efs;
		break;
	}

	y1 = Cm;
	y2 = Gm;
	if (PO.GsCorrect) {
		y1 = Cc;
		y2 = Gc;
	}
	
	pWnd->subplot(1); 
	pWnd->plot(x,y1,O1);
	pWnd->plot(x,Cf,O2);
			 
	pWnd->subplot(2);
	pWnd->plot(x,y2,O1);
	pWnd->plot(x,Gf,O2);
}

void CExpData::CalcGV(CMISitInterpBase *pMIS, float Gs, float Gp, const CPlotOptions& PO)
{
	CModelData::CalcGV(pMIS,Gs,Gp,PO);
	int n = V.dim();
	for(int j=0; j<n; j++) {
		Cc[j] = Cm[j];
		Gc[j] = Gm[j];
		GsGpUnCorr(Cc[j],Gc[j],freq,Gs,Gp);
	}
}

void CExpData::Save(const char *fname, const CPlotOptions& PO)
{
	const char* labels = 
		"      Vg      \t"
		"      Vs      \t"
		"    Efs-Ei    \t"
		"     Cexp     \t"
		"    Gexp/w    \t"
		"      Cf      \t"
		"     Gf/w     \t"
		"      Cs      \t"
		"    |Qs|      \t"
		"    |Qit|     \n";

	const char* units = 
		"     (V)      \t"
		"     (eV)     \t"
		"     (eV)     \t"
		"   (uF/cm2)   \t"
		"   (uF/cm2)   \t"
		"   (uF/cm2)   \t"
		"   (uF/cm2)   \t"
		"   (uF/cm2)   \t"
		"   (C/cm2)    \t"
		"   (C/cm2)    \n";

	CArray1D y1, y2;
	y1 = Cm;
	y2 = Gm;
	if (PO.GsCorrect) {
		y1 = Cc;
		y2 = Gc;
	}

	ofstream os(fname);

	long formatmask = os.setf(ios::scientific);

	os << labels;
	os << units;

	int N = V.dim(), i;
	for (i=0; i<N; ++i)
	{
		os << V[i] << '\t';
		os << Vs[i] << '\t';
		os << Efs[i] << '\t';
		os << y1[i] << '\t';
		os << y2[i] << '\t';
		os << Cf[i] << '\t';
		os << Gf[i] << '\t';
		os << Cs[i] << '\t';
		os << Qs[i] << '\t';
		os << Qit[i] << '\n';
	}

	os.close();
	
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDataArray::CDataArray() : nData(0)
{
	for(int i=0; i<N_DATA_MAX; i++) Data[i] = NULL;

}

CDataArray::~CDataArray()
{
	for(int i=0; i<nData; i++) 
		if (Data[i]!=NULL) {
			delete Data[i];
			Data[i] = NULL;
		}
	nData=0;
}

bool CDataArray::Load(const char *fname, float afreq)
{
	if (nData==N_DATA_MAX) return false;

	CArray2D A;
	int nRow = textload(fname,3,A);
	if (nRow > 0) {
		CExpData* pData = new CExpData(nRow,2*PI*afreq);
		for(int i = 0; i<nRow; i++) {
			pData->V[i] = A[i][0];
			pData->Cm[i] = A[i][1];
			pData->Gm[i] = A[i][2];
		}
		Data[nData]=pData;
		nData++;
		return true;
	} else return false;
}

bool CDataArray::Load(int aN, float aV1, float aV2, float afreq)
{
	if (nData==N_DATA_MAX) return false;

	CModelData* pData = new CModelData(aN,2*PI*afreq);

	float bb = (aV2-aV1)/(aN-1);
	for(int i=0; i<aN; i++) pData->V[i] = bb*i + aV1;

	Data[nData] = pData;
	nData++;
	return true;
}


bool CDataArray::Unload(int i)
{
	if (nData==0 || i>=nData) return false;
	else {
		delete Data[i];
		Data[i]=NULL;

		nData--;
		if (nData) {
			for(int j=i; j<nData; j++) {
				Data[j] = Data[j+1];
				Data[j+1] = NULL;
			}
		}

		return true;
	}
}

bool CDataArray::Unload()
{
	if (nData==0) return false;
	else {
		nData--;
		delete Data[nData];
		Data[nData]=NULL;
		return true;
	}
}



void CDataArray::Plot(CGraphWnd* pWnd, const CPlotOptions& PO)
{
	if (nData==0) {
		CArray1D x(2); x[1]= 1.; x[0] = 0;
		CArray1D y(2,0.); 
		pWnd->subplot(1);
		pWnd->Clear();
		pWnd->plot(x,y);
		pWnd->subplot(2);
		pWnd->Clear();
		pWnd->plot(x,y);
		return;
	}

	pWnd->subplot(1);
	pWnd->Clear();
	pWnd->subplot(2);
	pWnd->Clear();

	for(int i=0; i<nData; i++) Data[i]->Plot(pWnd,PO,i);

	CString TitleX, TitleY1, TitleY2;

	CPlotOptions::TypeX tx = (CPlotOptions::TypeX)(PO.typex);
	switch(tx)
	{
	case CPlotOptions::x_Vg:
		TitleX = "Vg (Volts)";
		break;
	case CPlotOptions::x_Vs:
		TitleX = "Vs (eV)";
		break;
	case CPlotOptions::x_Efs:
		TitleX = "Efs - Ei (eV)";
		break;
	}

	CPlotOptions::TypeY ty = (CPlotOptions::TypeY)(PO.typey);
	switch(ty)
	{
	case CPlotOptions::y_C_G:
		TitleY1 = "C (uF/cm2)";
		TitleY2 = "G/w (uF/cm2)";
		break;
	case CPlotOptions::y_Qs_Qit:
		TitleY1 = "|Qs| (C/cm2)";
		TitleY2 = "|Qit| (C/cm2)";
		break;
	case CPlotOptions::y_Cs_Qs:
		TitleY1 = "Cs (uF/cm2)";
		TitleY2 = "|Qs| (C/cm2)";
		break;
	}


	pWnd->subplot(1);
	pWnd->xlabel(TitleX);
	pWnd->ylabel(TitleY1);
	pWnd->grid();

	pWnd->subplot(2);
	pWnd->xlabel(TitleX);
	pWnd->ylabel(TitleY2);
	pWnd->grid();

	//if (PO.logG) pWnd->logy();

}




/*
void CGVdata::CalcGV(CMIS *pMIS, float Vfb, float Gs, float Gp)
{
	if (nfreq==0) return;

	int i,j;

	for(i=0; i<nfreq; i++) {
		TFreqData& Data = *(DataArray[i]);
		int n = Data.V.dim();
		if (n!=Data.Cf.dim()) { 
			Data.Cf = CArray1D(n); 
			Data.Gf = CArray1D(n); 
			Data.Vs = CArray1D(n);
			Data.Efs = CArray1D(n);
		}
	}

	int n = DataArray[0]->V.dim();
	//double vfbo = pMIS->CPs();
	for(j=0; j<n; j++) 
	{
		pMIS->SetVb(-DataArray[0]->V[j]+Vfb);
		for(i=0; i<nfreq; i++) {
			TFreqData& Data = *(DataArray[i]);
			cdouble cc = pMIS->CalcC(freq[i]);
			GsGpCorr(cc,freq[i],Gs,Gp,Data.Cf[j],Data.Gf[j]);
			Data.Vs[j] = pMIS->Vs();
			Data.Efs[j] = pMIS->Efs();
			Data.Cf[j] *= 1e6;
			Data.Gf[j] *= 1e6;
		}
	}
	
}
*/





void CTrapData::SortArrays()
{
	float b;
	int exchange = 1;

	while (exchange) {
		exchange = 0;
		for(int i=0; i<N_TRAP_PTS-1; i++)
		{
			if (E_[i+1]<E_[i]) {
				b = E_[i];
				E_[i] = E_[i+1];
				E_[i+1] = b;
				b = Dit_[i];
				Dit_[i] = Dit_[i+1];
				Dit_[i+1] = b;
				exchange++;
			}
		}
	}

}
