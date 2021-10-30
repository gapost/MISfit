// BasicProp.cpp : implementation file
//

#include "stdafx.h"
#include "misfit.h"
#include "BasicProp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBasicProp property page

IMPLEMENT_DYNCREATE(CBasicProp, CMISProp)

CBasicProp::CBasicProp(CMISData* aMISData) : CMISProp(CBasicProp::IDD, aMISData)
{
	//{{AFX_DATA_INIT(CBasicProp)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBasicProp::~CBasicProp()
{
}

void CBasicProp::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBasicProp)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBasicProp, CMISProp)
	//{{AFX_MSG_MAP(CBasicProp)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBasicProp message handlers
