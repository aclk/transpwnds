#define _WIN32_WINNT 0x0500
#include "Hook.h"

#include "ULLibNS.h"


LRESULT CALLBACK MouseProc(int nCode,WPARAM wParam,LPARAM lParam);


bool tagHotKeyInfo::IsHotKey(PMSLLHOOKSTRUCT lpMouseHookStruct)
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
bool tagHotKeyInfo::IsMsg(int nMsg,UINT uMsg)
{
	return (m_uMsg[nMsg]==uMsg);
}

tagHotKeyInfo& tagHotKeyInfo::operator=(tagHotKeyInfo& HotKeyInfo)
{
	m_fAlt=HotKeyInfo.m_fAlt;
	m_fCtrl=HotKeyInfo.m_fCtrl;
	m_fShift=HotKeyInfo.m_fShift;
	m_fWin=HotKeyInfo.m_fWin;
	m_uMsg[0]=HotKeyInfo.m_uMsg[0];
	m_uMsg[1]=HotKeyInfo.m_uMsg[1];
	m_uMsg[2]=HotKeyInfo.m_uMsg[2];
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
			if(i->second.dwStyle)
			{
				::SetWindowLong(i->first,GWL_STYLE,
					::GetWindowLong(i->first,GWL_STYLE)|i->second.dwStyle);
				::SetWindowPos(i->first,0,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_FRAMECHANGED);
			}
		}
		m_mapWndInfo.clear();
}

static CHook theHook;

CHook* CHook::GetHook()
{
	return &theHook;
}

///\brief функция предназначена для поиска popup родителя
///\param hWnd - хендл окна на родителе, или определенной иерархии дочерних окон или самого popup
///\return хендл popup в случае успеха, если popup оказывается рабочий стол то вернет NULL
HWND GetPopup(HWND hWnd)
{
	while(((GetWindowLong(hWnd,GWL_STYLE)&WS_CHILD))&&IsWindow(hWnd))
		hWnd=GetParent(hWnd);
	//hWnd=GetAncestor(hWnd,GA_ROOT);
	if(!hWnd)
		return NULL;		
	static TCHAR szText[255];
	GetClassName(hWnd,szText,255);
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
	lRes=theHook.ProcessMoveWnd((UINT)wParam,(PMSLLHOOKSTRUCT)lParam);
	if(lRes)
		return lRes;
	lRes=theHook.ProcessSizeWnd((UINT)wParam,(PMSLLHOOKSTRUCT)lParam);
	if(lRes)
		return lRes;
	lRes=theHook.ProcessToggleCaption((UINT)wParam,(PMSLLHOOKSTRUCT)lParam);
	if(lRes)
		return lRes;
	
	return CallNextHookEx(theHook.m_hMouse,nCode,wParam,lParam);
}
LRESULT CHook::ProcessTransp(UINT uMsg,PMSLLHOOKSTRUCT lpMouseHookStruct)
{
	if(!m_arHotKeyInfo[hkoTransp].IsHotKey(lpMouseHookStruct))
		return 0;
	if(!m_arHotKeyInfo[hkoTransp].IsMsg(0,uMsg))
		return 0;

	HWND hWnd=::WindowFromPoint(lpMouseHookStruct->pt);
	if((hWnd=GetPopup(hWnd))==0)
		return 0;

	if((GetWindowLong(hWnd,GWL_EXSTYLE)&WS_EX_LAYERED)==0)
	{
		if(((short)HIWORD(lpMouseHookStruct->mouseData))>0)
			return 0;

		std::map<HWND,CHook::WNDINFO>::const_iterator iterItem=m_mapWndInfo.find(hWnd);
		if(iterItem!=m_mapWndInfo.end())
		{
			iterItem._Mynode()->_Myval.second.bAlpha=255;
			iterItem._Mynode()->_Myval.second.fAlpha=TRUE;
		}
		else
		{
			CHook::WNDINFO wi={0};
			wi.bAlpha=255;
			wi.fAlpha=TRUE;
			m_mapWndInfo[hWnd]=wi;
		}
		::SetWindowLong(hWnd,GWL_EXSTYLE,GetWindowLong(hWnd,GWL_EXSTYLE)|WS_EX_LAYERED);
		SetLayeredWindowAttributes(hWnd,0,255,LWA_ALPHA);
		RedrawWindow(hWnd,NULL,NULL,RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
		return 1;
	}
	else
	{
		std::map<HWND,CHook::WNDINFO>::const_iterator iterItem=m_mapWndInfo.find(hWnd);
		if((iterItem==m_mapWndInfo.end())||(!iterItem._Mynode()->_Myval.second.fAlpha))
			return 0;                               
	}
	BYTE bAlpha=m_mapWndInfo[hWnd].bAlpha;
	if(((short)HIWORD(lpMouseHookStruct->mouseData))<0)
	{
		if(bAlpha>=(m_bMinTranspVal+m_bTranspStep))
			bAlpha-=m_bTranspStep;
	}
	else
	{
		if(bAlpha<=(255-m_bTranspStep))
			bAlpha+=m_bTranspStep;
	}                       
	SetLayeredWindowAttributes(hWnd,0,bAlpha,LWA_ALPHA);            
	m_mapWndInfo[hWnd].bAlpha=bAlpha;
/*	if(m_mapWndInfo[hWnd].bAlpha==255)
	{
		::SetWindowLong(hWnd,GWL_EXSTYLE,GetWindowLong(hWnd,GWL_EXSTYLE)&~WS_EX_LAYERED);
		m_mapWndInfo[hWnd].fAlpha=FALSE;
	}
	*/
	return 1;
}

LRESULT CHook::ProcessTopMost(UINT uMsg,PMSLLHOOKSTRUCT lpMouseHookStruct)
{
	if(!m_arHotKeyInfo[hkoTopMost].IsHotKey(lpMouseHookStruct))
		return 0;
	if(!m_arHotKeyInfo[hkoTopMost].IsMsg(0,uMsg))
		return 0;
	HWND hWnd=::WindowFromPoint(lpMouseHookStruct->pt);
	if((hWnd=GetPopup(hWnd))==0)
		return 0;
	if((GetWindowLong(hWnd,GWL_EXSTYLE)&WS_EX_TOPMOST)==0)
	{
		std::map<HWND,CHook::WNDINFO>::const_iterator iterItem=m_mapWndInfo.find(hWnd);
		if(iterItem!=m_mapWndInfo.end())
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
		if(iterItem!=m_mapWndInfo.end())
		{
			iterItem._Mynode()->_Myval.second.fTopMost=FALSE;
			::SetWindowPos(hWnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		}
	}
	return 1;
}

LRESULT CHook::ProcessMoveWnd(UINT uMsg, PMSLLHOOKSTRUCT lpMouseHookStruct)
{
	static bool fStartMoveWnd=false;
	static POINT ptStart={0,0};
	static HWND hWnd=NULL;

	if(fStartMoveWnd&&m_arHotKeyInfo[hkoMoveWnd].IsMsg(1,uMsg))
	{
		RECT rc;
		::GetWindowRect(hWnd,&rc);
		int nWidth=rc.right-rc.left;
		int nHeight=rc.bottom-rc.top;
		rc.left+=lpMouseHookStruct->pt.x-ptStart.x;
		rc.top+=lpMouseHookStruct->pt.y-ptStart.y;
		::MoveWindow(hWnd,rc.left,rc.top,nWidth,nHeight,TRUE);
		ptStart.x=lpMouseHookStruct->pt.x;
		ptStart.y=lpMouseHookStruct->pt.y;
	}
	else
		fStartMoveWnd=false;
	
	if(m_arHotKeyInfo[hkoMoveWnd].IsMsg(2,uMsg)&&fStartMoveWnd)
	{
		fStartMoveWnd=false;
		RedrawWindow(hWnd,NULL,NULL,RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
		return 0;
	}

	if(!m_arHotKeyInfo[hkoMoveWnd].IsHotKey(lpMouseHookStruct))
		return 0;
	if(!m_arHotKeyInfo[hkoMoveWnd].IsMsg(0,uMsg))
		return 0;
	hWnd=::WindowFromPoint(lpMouseHookStruct->pt);
	if((hWnd=GetPopup(hWnd))==0)
		return 0;
	fStartMoveWnd=true;
	ptStart=lpMouseHookStruct->pt;
	return 1;
}

LRESULT CHook::ProcessSizeWnd(UINT uMsg, PMSLLHOOKSTRUCT lpMouseHookStruct)
{
	static bool fStartSizeWnd=false;
	static POINT ptStart={0,0};
	static HWND hWnd=NULL;

	if(fStartSizeWnd&&m_arHotKeyInfo[hkoSizeWnd].IsMsg(1,uMsg))
	{
		RECT rc;
		::GetWindowRect(hWnd,&rc);
		int nWidth=rc.right-rc.left;
		int nHeight=rc.bottom-rc.top;
		nWidth+=lpMouseHookStruct->pt.x-ptStart.x;
		nHeight+=lpMouseHookStruct->pt.y-ptStart.y;
		::MoveWindow(hWnd,rc.left,rc.top,nWidth,nHeight,TRUE);
		ptStart.x=lpMouseHookStruct->pt.x;
		ptStart.y=lpMouseHookStruct->pt.y;
	}
	else
		fStartSizeWnd=false;
	
	if(m_arHotKeyInfo[hkoSizeWnd].IsMsg(2,uMsg)&&fStartSizeWnd)
	{
		RedrawWindow(hWnd,NULL,NULL,RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
		fStartSizeWnd=false;
		return 0;
	}

	if(!m_arHotKeyInfo[hkoSizeWnd].IsHotKey(lpMouseHookStruct))
		return 0;
	if(!m_arHotKeyInfo[hkoSizeWnd].IsMsg(0,uMsg))
		return 0;
	hWnd=::WindowFromPoint(lpMouseHookStruct->pt);
	if((hWnd=GetPopup(hWnd))==0)
		return 0;
	fStartSizeWnd=true;
	ptStart=lpMouseHookStruct->pt;
	return 1;
}

LRESULT CHook::ProcessToggleCaption(UINT uMsg, PMSLLHOOKSTRUCT lpMouseHookStruct)
{
	if(!m_arHotKeyInfo[hkoToggleCaption].IsHotKey(lpMouseHookStruct))
		return 0;
	if(!m_arHotKeyInfo[hkoToggleCaption].IsMsg(0,uMsg))
		return 0;
	HWND hWnd=::WindowFromPoint(lpMouseHookStruct->pt);
	if((hWnd=GetPopup(hWnd))==0)
		return 0;
	if((GetWindowLong(hWnd,GWL_STYLE)&WS_CAPTION)==WS_CAPTION)
	{
		std::map<HWND,CHook::WNDINFO>::const_iterator iterItem=m_mapWndInfo.find(hWnd);
		if(iterItem!=m_mapWndInfo.end())
			iterItem._Mynode()->_Myval.second.dwStyle=(DWORD)::GetWindowLong(hWnd,GWL_STYLE);
		else
		{
			CHook::WNDINFO wi={0};
			wi.dwStyle=(DWORD)::GetWindowLong(hWnd,GWL_STYLE);
			m_mapWndInfo[hWnd]=wi;
		}
		::SetWindowLong(hWnd,GWL_STYLE,
			::GetWindowLong(hWnd,GWL_STYLE)&~WS_CAPTION);
		::SetWindowPos(hWnd,0,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_FRAMECHANGED);

		return 1;						
	}
	else
	{
		std::map<HWND,CHook::WNDINFO>::const_iterator iterItem=m_mapWndInfo.find(hWnd);
		if(iterItem!=m_mapWndInfo.end())
		{
			::SetWindowLong(hWnd,GWL_STYLE,
				::GetWindowLong(hWnd,GWL_STYLE)|iterItem._Mynode()->_Myval.second.dwStyle);
			::SetWindowPos(hWnd,0,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_FRAMECHANGED);
			iterItem._Mynode()->_Myval.second.dwStyle=0;
		}
	}
	RedrawWindow(hWnd,NULL,NULL,RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
	return 1;
}

