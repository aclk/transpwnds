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

	m_pMainWnd=new CWorkWnd;
	return m_pMainWnd->Create(NULL,NULL,WS_POPUP,0,0,0,0,NULL,NULL);
}

CApp theApp;