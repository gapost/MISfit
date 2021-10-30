// GVdata.h: interface for the CGVdata class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GVDATA_H__B2BE4DA9_CBF4_4EFB_B3E2_A44DC58D951E__INCLUDED_)
#define AFX_GVDATA_H__B2BE4DA9_CBF4_4EFB_B3E2_A44DC58D951E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define N_DATA_MAX 10

class CPlotOptions {
public:
	enum TypeX {x_Vg = 0, x_Vs = 1, x_Efs = 2};
	enum TypeY {y_C_G = 0, y_Qs_Qit, y_Cs_Qs};
public:
	CPlotOptions() :
	typey(0), typex(0), logC(FALSE), logG(FALSE), GsCorrect(FALSE)
	{}
	int typey;
	int typex;
	BOOL logC;
	BOOL logG;
	BOOL GsCorrect;
};

class CModelData{
public:
	CModelData(int n, float afreq) :
		V(n,0.),Vs(n,0.),Efs(n,0.),Cf(n,0.),Gf(n,0.),
		Cs(n,0.), Qs(n,0.), Qit(n,0.),
		freq(afreq)
	{}
	virtual ~CModelData() {}
	CArray1D V,Vs,Efs,Cf,Gf,Cs,Qs,Qit;
	float freq;
	virtual void Plot(CGraphWnd *pWnd, const CPlotOptions& PO, int i);
	virtual void CalcGV(CMISitInterpBase *pMIS, float Gs, float Gp, const CPlotOptions& PO);
	virtual int nCol() { return 3; }
	virtual void Save(const char* fname, const CPlotOptions& PO);
};

class CExpData : public CModelData {
public:
	CExpData(int n, float afreq) : CModelData(n, afreq)
	,Cm(n,0.),Gm(n,0.),Cc(n,0.),Gc(n,0.)
	{}
	CArray1D Cm,Gm,Cc,Gc;
	virtual void Plot(CGraphWnd *pWnd, const CPlotOptions& PO, int i);
	virtual void CalcGV(CMISitInterpBase *pMIS, float Gs, float Gp, const CPlotOptions& PO);
	virtual int nCol() { return 3; }
	virtual void Save(const char* fname, const CPlotOptions& PO);
};


class CDataArray  
{
public:
	void Plot(CGraphWnd* pWnd, const CPlotOptions& PO);
	bool Unload(int i);
	bool Unload();
	bool Load(const char* fname, float freq);
	bool Load(int aN, float aV1, float aV2, float afreq);
	void CalcGV(CMISitInterpBase *pMIS, float Gs, float Gp, const CPlotOptions& PO) {
		for(int i=0; i<nData; i++) Data[i]->CalcGV(pMIS,Gs,Gp,PO);
	}
	CModelData* operator [](int i) {
		if (i<0 || nData==0 || i>=nData) return NULL;
		else return Data[i];
	}
	CDataArray();
	virtual ~CDataArray();

	bool IsFull() { return nData == N_DATA_MAX; }
	bool IsEmpty() { return nData == 0; }

	CModelData* Data[N_DATA_MAX];
	int nData;

};

#define N_TRAP_PTS 5

class CTrapData {
public:
	void SortArrays();
	CTrapData(float gap = 1.12) : Dit(1e12f),sigma(1e-16f),nTraptype(0),nTrapMode(0),Eo(0.3f),DE(0.2f) 
	{
		for(int i=0; i<N_TRAP_PTS; i++) {
			Dit_[i]=1e12f; E_[i] = gap*i/(N_TRAP_PTS-1)/2;
		}
	}

	void AddTrap(CQitArray& Q, bool isn, int nMaterial)
	{
		float Eg,Nc,Nv,vth;

		bool is_el_trap = nTraptype==0;

		switch(nMaterial)
		{
		case 0: // Si
			Eg = (float)SI_EG; Nc = (float)SI_NC; Nv = (float)SI_NV; 
			vth = (float)(is_el_trap ? SI_VTH_E : SI_VTH_H);
			break;
		case 1: // Ge
			Eg = (float)GE_EG; Nc = (float)GE_NC; Nv = (float)GE_NV;
			vth = (float)(is_el_trap ? GE_VTH_E : GE_VTH_H);
			break;
		case 2: // GaAs
			Eg = (float)GAAS_EG; Nc = (float)GAAS_NC; Nv = (float)GAAS_NV;
			vth = (float)(is_el_trap ? GAAS_VTH_E : GAAS_VTH_H);
			break;
		}

		switch(nTrapMode)
		{
		case 0:
			Q.Add( new CQit(Dit,sigma,is_el_trap,isn,Eg,Nc,Nc,vth) );
			break;
		case 1:
			Q.Add( new CQitGauss(Dit,Eo,DE,sigma,is_el_trap,isn,Eg,Nc,Nc,vth) );
			break;
		case 2:
			Q.Add( new CQitLorentz(Dit,Eo,DE,sigma,is_el_trap,isn,Eg,Nc,Nc,vth) );
			break;
		case 3:
			Q.Add( new CQitMatrix(N_TRAP_PTS,Dit_,E_,sigma,nTraptype==0,isn,Eg,Nc,Nc,vth) );
			break;
		}
	}

	CString MakeTitle() {
		CString S("");
		if (nTrapMode==0) S = "Uniform ";
		else if (nTrapMode==1) S = "Gaussian ";
		else if (nTrapMode==2) S = "Lorentz ";
		else S = "Matrix ";
		if (nTraptype==0) S += "e-trap";
		else S += "h-trap";
		return S;
	}

	float Dit,sigma,Eo,DE;
	int nTraptype,nTrapMode;
	float Dit_[N_TRAP_PTS],E_[N_TRAP_PTS];
	void dump(ostream& os) {
		CString stype;
		if (nTraptype==0) stype = "e-trap";
		else stype = "h-trap";
		if (nTrapMode==0) {
			os << "Uniform " << stype << " trap" << endl;
			os << "Dit (cm^-3 eV^-1) = " << Dit << endl;
			os << "sigma (cm^2) = " << sigma << endl;
		}
		else if (nTrapMode==1) {
			os << "Gaussian " << stype << " trap" << endl;
			os << "Dit (cm^-3 eV^-1) = " << Dit << endl;
			os << "sigma (cm^2) = " << sigma << endl;
			os << "Eo (eV) = " << Eo << endl;
			os << "DE (eV) = " << DE << endl;
		}
		else if (nTrapMode==2) {
			os << "Lorenzian " << stype << " trap" << endl;
			os << "Dit (cm^-3 eV^-1) = " << Dit << endl;
			os << "sigma (cm^2) = " << sigma << endl;
			os << "Eo (eV) = " << Eo << endl;
			os << "DE (eV) = " << DE << endl;
		}
		else {
			os << "Matrix " << stype << " trap" << endl;
			os << "sigma (cm^2) = " << sigma << endl;
			os << "E(eV) \tDit (cm^-3 eV^-1)" << endl;
			for(int i =0; i<N_TRAP_PTS; i++) 
				os << E_[i] << "\t" << Dit_[i] << endl;
		}
	}
};

#endif // !defined(AFX_GVDATA_H__B2BE4DA9_CBF4_4EFB_B3E2_A44DC58D951E__INCLUDED_)
