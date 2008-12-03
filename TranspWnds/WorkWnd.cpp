#include "WorkWnd.h"
#include "resource.h"
#include "Hook.h"
CWorkWnd::CWorkWnd(void):
	CULWnd()
{
	MessageMap.AddMessage<CWorkWnd>(WM_CREATE,&CWorkWnd::OnCreate);
	MessageMap.AddMessage<CWorkWnd>(WM_DESTROY,&CWorkWnd::OnDestroy);
	MessageMap.AddMessage<CWorkWnd>(NIM_MESSAGE,&CWorkWnd::OnNIMessage);

	MessageMap.AddCommand<CWorkWnd>(IDM_ENABLE,&CWorkWnd::OnEnable);
	MessageMap.AddCommand<CWorkWnd>(IDM_DISABLE,&CWorkWnd::OnDisable);
	MessageMap.AddCommand<CWorkWnd>(IDM_RESTORE,&CWorkWnd::OnRestore);
	MessageMap.AddCommand<CWorkWnd>(IDM_QUIT,&CWorkWnd::OnQuit);
}

CWorkWnd::~CWorkWnd(void)
{
}

LRESULT CWorkWnd::OnCreate(WPARAM,LPARAM)
{
	m_Menu.CreatePopupMenu();
	m_Menu.AppendMenu(MF_GRAYED|MF_DISABLED|MF_BYCOMMAND,IDM_ENABLE,CULStrTable(IDS_MENU_ENABLE));
	m_Menu.AppendMenu(MF_BYCOMMAND,IDM_DISABLE,CULStrTable(IDS_MENU_DISABLE));
	m_Menu.AppendMenu(MF_BYCOMMAND,IDM_RESTORE,CULStrTable(IDS_MENU_RESTORE));			
//	m_Menu.AppendMenu((IsAutoRun())?MF_CHECKED:0)|MF_BYCOMMAND, IDM_AUTORUN, _T("AutoRun"));			
	m_Menu.AppendMenu(MF_BYCOMMAND,IDM_QUIT,CULStrTable(IDS_MENU_QUIT));

	m_niData.cbSize=sizeof(m_niData);
	m_niData.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP;
	m_niData.hWnd=*this;
	m_niData.hIcon=(HICON)LoadImage(ULGetResourceHandle(),MAKEINTRESOURCE(IDI_MAINICON),IMAGE_ICON,0,0,LR_DEFAULTSIZE|LR_DEFAULTCOLOR);
	if(m_niData.hIcon==NULL)
		DestroyWindow();
	_tcscpy_s(m_niData.szTip,64,CULStrTable(IDS_APP_NAME));
	m_niData.uCallbackMessage=NIM_MESSAGE;
	m_niData.uID=NIM_MESSAGE;
	if(!Shell_NotifyIcon(NIM_ADD,&m_niData))
		DestroyWindow();

	CHook::GetHook()->Enable();


	return 0;
}

LRESULT CWorkWnd::OnDestroy(WPARAM,LPARAM)
{
	Shell_NotifyIcon(NIM_DELETE,&m_niData);
	::PostQuitMessage(0);
	return 0;
}

LRESULT CWorkWnd::OnNIMessage(WPARAM,LPARAM lParam)
{
	if((lParam==WM_LBUTTONDOWN)||(lParam==WM_RBUTTONDOWN))
	{
		POINT pt={0};
		GetCursorPos(&pt);
		SetForegroundWindow();
		m_Menu.TrackPopupMenu(TPM_RIGHTBUTTON,pt,*this);
		PostMessage(WM_NULL);
	}
	return 1;
}

void CWorkWnd::OnEnable(WORD,HWND)
{
	CHook::GetHook()->Enable();
	m_Menu.EnableMenuItem(IDM_ENABLE,MFS_GRAYED|MFS_DISABLED);
	m_Menu.EnableMenuItem(IDM_DISABLE,MFS_ENABLED);
}

void CWorkWnd::OnDisable(WORD,HWND)
{
	CHook::GetHook()->Disable();
	m_Menu.EnableMenuItem(IDM_DISABLE,MFS_GRAYED|MFS_DISABLED);
	m_Menu.EnableMenuItem(IDM_ENABLE,MFS_ENABLED);
}

void CWorkWnd::OnRestore(WORD,HWND)
{
	CHook::GetHook()->Restore();
}


void CWorkWnd::OnQuit(WORD,HWND)
{
	CHook::GetHook()->Restore();
	DestroyWindow();
}
