// DataProp.cpp : implementation file
//

#include "stdafx.h"
#include "misfit.h"
#include "DataProp.h"
#include "FreqDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataProp property page

IMPLEMENT_DYNCREATE(CDataProp, CMISProp)

CDataProp::CDataProp(CMISData* aMISData) : 
CMISProp(CDataProp::IDD,aMISData)
{
	//{{AFX_DATA_INIT(CDataProp)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDataProp::~CDataProp()
{
}

void CDataProp::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataProp)
	DDX_Control(pDX, IDC_DATA_LIST, lbData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataProp, CMISProp)
	//{{AFX_MSG_MAP(CDataProp)
	ON_BN_CLICKED(IDC_LOAD, OnLoad)
	ON_BN_CLICKED(IDC_UNLOAD, OnUnload)
	ON_BN_CLICKED(IDC_NEW_FREQ, OnNewFreq)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataProp message handlers

void CDataProp::OnLoad() 
{
	if (pMISData->IsFull()) {
		MessageBox("Cannot load any more frequencies!");
		return;
	}

	static char BASED_CODE szFilter[] = 
		"Data files (*.dat)|*.dat|Text files (*.txt)|*.txt|All Files (*.*)|*.*||";

	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY,szFilter);

	if (!(dlg.DoModal()==IDOK)) return;

	CFreqDlg fdlg;
	if (!(fdlg.DoModal()==IDOK)) return;

	if (pMISData->Load(dlg.GetPathName(),fdlg.Freq)) {
		pMISData->DoCalc();
		CString S;
		S.Format("f=%.0gHz, %s",fdlg.Freq,dlg.GetPathName());
		lbData.AddString(S);
	}
}

void CDataProp::OnUnload() 
{
	int i = lbData.GetCurSel();
	if (i>=0) {
		if (pMISData->Unload(i)) {
			pMISData->UpdatePlot();
			lbData.DeleteString(i);
		}
	}
}


void CDataProp::OnNewFreq() 
{
	CFreqDlg fdlg;
	if (!(fdlg.DoModal()==IDOK)) return;

	if (pMISData->Load(201,-3.f,3.f,fdlg.Freq)) {
		pMISData->DoCalc();
		//pMISData->UpdatePlot();
		CString S;
		S.Format("f=%.0gHz, Modell Curve",fdlg.Freq);
		lbData.AddString(S);
	}
}

void CDataProp::OnSave() 
{
	int i = lbData.GetCurSel();
	if (i<0) 
	{
		MessageBox("No data/model curve selected");
		return;
	}

	static char BASED_CODE szFilter[] = 
		"Data files (*.dat)|*.dat|Text files (*.txt)|*.txt|All Files (*.*)|*.*||";

	CFileDialog dlg(FALSE,".dat",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter);

	if (!(dlg.DoModal()==IDOK)) return;

	CModelData* pData = pMISData->Data[i];

	pData->Save(dlg.GetPathName(),pMISData->PlotOptions);
}
