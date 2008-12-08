#define _WIN32_WINNT 0x0500
#include "App.h"
#include "ULLibLink.h"
#include "WorkWnd.h"
CApp::CApp(void)
{
}

CApp::~CApp(void)
{
	
}

BOOL CApp::InitInstance()
{
	//синглтон
	if(CreateMutex(NULL,FALSE,_T("TranspWndSinletone")))
		if(GetLastError()==ERROR_ALREADY_EXISTS)
			return FALSE;

	SetThreadLocale(MAKELCID(0x040c, SORT_DEFAULT));


	m_pMainWnd=new CWorkWnd;
	return m_pMainWnd->Create(NULL,NULL,WS_POPUP,GetSystemMetrics(SM_CXFULLSCREEN)/2-100,
		GetSystemMetrics(SM_CYFULLSCREEN)/2-100,0,0,NULL,NULL);
}

CApp theApp;