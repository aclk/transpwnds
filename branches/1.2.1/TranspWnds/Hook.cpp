#define _WIN32_WINNT 0x0500
#include "Hook.h"

#include "ULLibNS.h"


LRESULT CALLBACK MouseProc(int nCode,WPARAM wParam,LPARAM lParam);


bool tagHotKeyInfo::IsHotKey(UINT uMsg, PMSLLHOOKSTRUCT lpMouseHookStruct)
{
	if(m_uMsg==uMsg)
	{
		BOOL fKeyState=HIWORD(GetKeyState(VK_MENU));
		if((m_fAlt&&!fKeyState)||(!m_fAlt&&fKeyState))
				return false;
		fKeyState=HIWORD(GetKeyState(VK_CONTROL));
		if((m_fCtrl&&!fKeyState)||(!m_fCtrl&&fKeyState))
				return false;
		fKeyState=HIWORD(GetKeyState(VK_SHIFT));
		if((m_fShift&&!fKeyState)||(!m_fShift&&fKeyState))
				return false;
		fKeyState=HIWORD(GetKeyState(VK_LWIN))||HIWORD(GetKeyState(VK_RWIN));
		if((m_fWin&&!fKeyState)||(!m_fWin&&fKeyState))
				return false;			
		return true;
	}
	else
		return false;
}

tagHotKeyInfo& tagHotKeyInfo::operator=(tagHotKeyInfo& HotKeyInfo)
{
	m_fAlt=HotKeyInfo.m_fAlt;
	m_fCtrl=HotKeyInfo.m_fCtrl;
	m_fShift=HotKeyInfo.m_fShift;
	m_fWin=HotKeyInfo.m_fWin;
	m_uMsg=HotKeyInfo.m_uMsg;
	return *this;
}

CHook::CHook(void):
	m_hMouse(NULL)
{

}

CHook::~CHook(void)
{
	::UnhookWindowsHookEx(m_hMouse);
}

void CHook::Enable()
{
	m_hMouse=SetWindowsHookEx(WH_MOUSE_LL,MouseProc,ULGetResourceHandle(),0);
}

void CHook::Disable()
{
	::UnhookWindowsHookEx(m_hMouse);
}

void CHook::Restore()
{
	for(std::map<HWND,WNDINFO>::iterator i=m_mapWndInfo.begin();i!=m_mapWndInfo.end();i++)
		if(IsWindow(i->first))
		{
			if(i->second.fAlpha)
			{
				SetLayeredWindowAttributes(i->first,0,255,LWA_ALPHA);
				::SetWindowLong(i->first,GWL_EXSTYLE,GetWindowLong(i->first,GWL_EXSTYLE)&~WS_EX_LAYERED);
				RedrawWindow(i->first,NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_FRAME|RDW_ALLCHILDREN);
			}

			if(i->second.fTopMost)
				::SetWindowPos(i->first,HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		}
		m_mapWndInfo.clear();
}

static CHook theHook;

CHook* CHook::GetHook()
{
	return &theHook;
}

///\brief функция преднозначена для popup родителя
///\param hWnd - хендл окна на родителе, или определенной иерархии дочерних окон или самого popup
///\return хендл popup в случае успеха, если popup оказывается рабочий стол то вернет NULL
HWND GetPopup(HWND hWnd)
{
	while(((GetWindowLong(hWnd,GWL_STYLE)&WS_CHILD))&&IsWindow(hWnd))
		hWnd=GetParent(hWnd);
	if(!hWnd)
		return NULL;		
	static TCHAR szText[255];
	GetClassName(hWnd,szText,255);
	//_tcslwr_s(szText,255);
	if(_tcsicmp(szText,_T("progman"))==0)
		return NULL;
	return hWnd;
}

///\brief процедура мышинного хука
LRESULT CALLBACK MouseProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	LRESULT lRes=0;
	lRes=theHook.ProcessTransp((UINT)wParam,(PMSLLHOOKSTRUCT)lParam);
	if(lRes)
		return lRes;
	lRes=theHook.ProcessTopMost((UINT)wParam,(PMSLLHOOKSTRUCT)lParam);
	if(lRes)
		return lRes;

	if((wParam==WM_LBUTTONDOWN)&&(HIWORD(GetKeyState(VK_MENU)))&&(HIWORD(GetKeyState(VK_CONTROL))))
	{

	}
	return CallNextHookEx(theHook.m_hMouse,nCode,wParam,lParam);
}
LRESULT CHook::ProcessTransp(UINT uMsg,PMSLLHOOKSTRUCT lpMouseHookStruct)
{
	if(!m_arHotKeyInfo[hkoTransp].IsHotKey(uMsg,lpMouseHookStruct))
		return 0;

	HWND hWnd=::WindowFromPoint(lpMouseHookStruct->pt);
	if((hWnd=GetPopup(hWnd))==0)
		return 0;

	if((GetWindowLong(hWnd,GWL_EXSTYLE)&WS_EX_LAYERED)==0)
	{
		if(((short)HIWORD(lpMouseHookStruct->mouseData))>0)
			return 0;
		::SetWindowLong(hWnd,GWL_EXSTYLE,GetWindowLong(hWnd,GWL_EXSTYLE)|WS_EX_LAYERED);
		CHook::WNDINFO wi={0};
		wi.bAlpha=255;
		wi.fAlpha=TRUE;
		m_mapWndInfo[hWnd]=wi;
		SetLayeredWindowAttributes(hWnd,0,255,LWA_ALPHA);
		RedrawWindow(hWnd,NULL,NULL,RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
		return 1;
	}
	else
	{
		std::map<HWND,CHook::WNDINFO>::const_iterator iterItem=m_mapWndInfo.find(hWnd);
		if((iterItem._Mynode()->_Myval.first!=hWnd)||(!iterItem._Mynode()->_Myval.second.fAlpha))
			return 1;                               
	}
	BYTE bAlpha=m_mapWndInfo[hWnd].bAlpha;
	if(((short)HIWORD(lpMouseHookStruct->mouseData))<0)
	{
		if(bAlpha>m_bMinTranspVal)
			bAlpha-=m_bTranspStep;
	}
	else
	{
		if(bAlpha<=245)
			bAlpha+=m_bTranspStep;
	}                       
	SetLayeredWindowAttributes(hWnd,0,bAlpha,LWA_ALPHA);            
	m_mapWndInfo[hWnd].bAlpha=bAlpha;
	return 1;
}

LRESULT CHook::ProcessTopMost(UINT uMsg,PMSLLHOOKSTRUCT lpMouseHookStruct)
{
	if(!m_arHotKeyInfo[hkoTopMost].IsHotKey(uMsg,lpMouseHookStruct))
		return 0;

	HWND hWnd=::WindowFromPoint(lpMouseHookStruct->pt);
	if((hWnd=GetPopup(hWnd))==0)
		return 0;
	if((GetWindowLong(hWnd,GWL_EXSTYLE)&WS_EX_TOPMOST)==0)
	{
		std::map<HWND,CHook::WNDINFO>::const_iterator iterItem=m_mapWndInfo.find(hWnd);
		if((iterItem._Mynode()->_Myval.first==hWnd))
			iterItem._Mynode()->_Myval.second.fTopMost=TRUE;
		else
		{
			CHook::WNDINFO wi={0};
			wi.fTopMost=TRUE;
			m_mapWndInfo[hWnd]=wi;
		}
		::SetWindowPos(hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		return 1;						
	}
	else
	{
		std::map<HWND,CHook::WNDINFO>::const_iterator iterItem=m_mapWndInfo.find(hWnd);
		if((iterItem._Mynode()->_Myval.first==hWnd))
		{
			iterItem._Mynode()->_Myval.second.fTopMost=FALSE;
			::SetWindowPos(hWnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		}
	}
	return 1;
}
