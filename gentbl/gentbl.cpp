// gentbl.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "gentbl.h"

#include "misint.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

void CalcQNd();
void CalcQNa();
void CalcQNd_Q();
void CalcQNa_Q();

void CalcQNdGe();
void CalcQNaGe();
void CalcQNdGe_Q();
void CalcQNaGe_Q();

void CalcQNdGaAs();
void CalcQNaGaAs();
void CalcQNdGaAs_Q();
void CalcQNaGaAs_Q();




int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
		//CalcQNdGe_Q();
		//CalcQNaGe_Q();
		//CalcQNdGe();
		//CalcQNaGe();
		//CalcQNdGaAs();
		//CalcQNaGaAs();
		CalcQNdGaAs_Q();
		CalcQNaGaAs_Q();
	}

	return nRetCode;
}


