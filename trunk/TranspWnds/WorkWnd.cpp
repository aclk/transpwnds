#define _WIN32_WINNT 0x0500
#include "Hook.h"
#include "WorkWnd.h"
#include "resource.h"
#include "PropHotKeys.h"
#include "PropSystem.h"
#include "PropSheetOptions.h"
CWorkWnd::CWorkWnd(void):
	CULWnd()
{
	MessageMap.AddMessage<CWorkWnd>(WM_CREATE,&CWorkWnd::OnCreate);
	MessageMap.AddMessage<CWorkWnd>(WM_DESTROY,&CWorkWnd::OnDestroy);
	MessageMap.AddMessage<CWorkWnd>(NIM_MESSAGE,&CWorkWnd::OnNIMessage);

	MessageMap.AddCommand<CWorkWnd>(IDM_ENABLE,&CWorkWnd::OnEnable);
	MessageMap.AddCommand<CWorkWnd>(IDM_DISABLE,&CWorkWnd::OnDisable);
	MessageMap.AddCommand<CWorkWnd>(IDM_RESTORE,&CWorkWnd::OnRestore);
	MessageMap.AddCommand<CWorkWnd>(IDM_OPTIONS,&CWorkWnd::OnOptions);
	
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
	m_Menu.AppendMenu(MF_BYPOSITION|MF_SEPARATOR,3,NULL);			
	m_Menu.AppendMenu(MF_BYCOMMAND,IDM_OPTIONS,CULStrTable(IDS_MENU_OPTIONS));			
//	m_Menu.AppendMenu((IsAutoRun())?MF_CHECKED:0)|MF_BYCOMMAND, IDM_AUTORUN, _T("AutoRun"));			
	m_Menu.AppendMenu(MF_BYPOSITION|MF_SEPARATOR,5,NULL);			
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


	m_ProfileReg.SetRegistryKey(CULStrTable(IDS_COMPANY_NAME),CULStrTable(IDS_APP_NAME));
	LoadSettings();

	CHook::GetHook()->Enable();

	return 0;
}

void CWorkWnd::LoadSettings()
{
	DWORD dwVal=0;
	//загрузка гор€чих клавишь дл€ тулзы прозрачности
	if(m_ProfileReg.GetProfileInt(_T("HotKeys"),_T("TranspAlt"),&dwVal))
		CHook::GetHook()->m_arHotKeyInfo[hkoTransp].m_fAlt=(BOOL)dwVal;
	else
		CHook::GetHook()->m_arHotKeyInfo[hkoTransp].m_fAlt=TRUE;
	if(m_ProfileReg.GetProfileInt(_T("HotKeys"),_T("TranspCtrl"),&dwVal))
		CHook::GetHook()->m_arHotKeyInfo[hkoTransp].m_fCtrl=(BOOL)dwVal;
	else
		CHook::GetHook()->m_arHotKeyInfo[hkoTransp].m_fCtrl=FALSE;
	if(m_ProfileReg.GetProfileInt(_T("HotKeys"),_T("TranspShift"),&dwVal))
		CHook::GetHook()->m_arHotKeyInfo[hkoTransp].m_fShift=(BOOL)dwVal;
	else
		CHook::GetHook()->m_arHotKeyInfo[hkoTransp].m_fShift=FALSE;
	if(m_ProfileReg.GetProfileInt(_T("HotKeys"),_T("TranspWin"),&dwVal))
		CHook::GetHook()->m_arHotKeyInfo[hkoTransp].m_fWin=(BOOL)dwVal;
	else
		CHook::GetHook()->m_arHotKeyInfo[hkoTransp].m_fWin=FALSE;
	CHook::GetHook()->m_arHotKeyInfo[hkoTransp].m_uMsg=WM_MOUSEWHEEL;
	//загрузка гор€чих клавишь дл€ тулзы топмост
	if(m_ProfileReg.GetProfileInt(_T("HotKeys"),_T("TopMostAlt"),&dwVal))
		CHook::GetHook()->m_arHotKeyInfo[hkoTopMost].m_fAlt=(BOOL)dwVal;
	else
		CHook::GetHook()->m_arHotKeyInfo[hkoTopMost].m_fAlt=TRUE;
	if(m_ProfileReg.GetProfileInt(_T("HotKeys"),_T("TopMostCtrl"),&dwVal))
		CHook::GetHook()->m_arHotKeyInfo[hkoTopMost].m_fCtrl=(BOOL)dwVal;
	else
		CHook::GetHook()->m_arHotKeyInfo[hkoTopMost].m_fCtrl=TRUE;
	if(m_ProfileReg.GetProfileInt(_T("HotKeys"),_T("TopMostShift"),&dwVal))
		CHook::GetHook()->m_arHotKeyInfo[hkoTopMost].m_fShift=(BOOL)dwVal;
	else
		CHook::GetHook()->m_arHotKeyInfo[hkoTopMost].m_fShift=FALSE;
	if(m_ProfileReg.GetProfileInt(_T("HotKeys"),_T("TopMostWin"),&dwVal))
		CHook::GetHook()->m_arHotKeyInfo[hkoTopMost].m_fWin=(BOOL)dwVal;
	else
		CHook::GetHook()->m_arHotKeyInfo[hkoTopMost].m_fWin=FALSE;
	if(m_ProfileReg.GetProfileInt(_T("HotKeys"),_T("TopMostMsg"),&dwVal))
		CHook::GetHook()->m_arHotKeyInfo[hkoTopMost].m_uMsg=(BOOL)dwVal;
	else
		CHook::GetHook()->m_arHotKeyInfo[hkoTopMost].m_uMsg=WM_LBUTTONDOWN;

	//загрузка параметров изменени€ прозрачности
	if(m_ProfileReg.GetProfileInt(_T("TranspVals"),_T("MinTranspVal"),&dwVal))
		CHook::GetHook()->m_bMinTranspVal=(BOOL)dwVal;
	else
		CHook::GetHook()->m_bMinTranspVal=100;
	if(m_ProfileReg.GetProfileInt(_T("TranspVals"),_T("TranspStep"),&dwVal))
		CHook::GetHook()->m_bTranspStep=(BYTE)dwVal;
	else
		CHook::GetHook()->m_bTranspStep=10;
}

void CWorkWnd::SaveSettings()
{
	//сохранение гор€чих клавишь дл€ тулзы прозрачности
	m_ProfileReg.WriteProfileInt(_T("HotKeys"),_T("TranspAlt"),
		CHook::GetHook()->m_arHotKeyInfo[hkoTransp].m_fAlt);
	m_ProfileReg.WriteProfileInt(_T("HotKeys"),_T("TranspCtrl"),
		CHook::GetHook()->m_arHotKeyInfo[hkoTransp].m_fCtrl);
	m_ProfileReg.WriteProfileInt(_T("HotKeys"),_T("TranspShift"),
		CHook::GetHook()->m_arHotKeyInfo[hkoTransp].m_fShift);
	m_ProfileReg.WriteProfileInt(_T("HotKeys"),_T("TranspWin"),
		CHook::GetHook()->m_arHotKeyInfo[hkoTransp].m_fWin);
	//сохранение гор€чих клавишь дл€ тулзы топмост
	m_ProfileReg.WriteProfileInt(_T("HotKeys"),_T("TopMostAlt"),
		CHook::GetHook()->m_arHotKeyInfo[hkoTopMost].m_fAlt);
	m_ProfileReg.WriteProfileInt(_T("HotKeys"),_T("TopMostCtrl"),
		CHook::GetHook()->m_arHotKeyInfo[hkoTopMost].m_fCtrl);
	m_ProfileReg.WriteProfileInt(_T("HotKeys"),_T("TopMostShift"),
		CHook::GetHook()->m_arHotKeyInfo[hkoTopMost].m_fShift);
	m_ProfileReg.WriteProfileInt(_T("HotKeys"),_T("TopMostWin"),
		CHook::GetHook()->m_arHotKeyInfo[hkoTopMost].m_fWin);
	m_ProfileReg.WriteProfileInt(_T("HotKeys"),_T("TopMostMsg"),
		CHook::GetHook()->m_arHotKeyInfo[hkoTopMost].m_uMsg);}

LRESULT CWorkWnd::OnDestroy(WPARAM,LPARAM)
{
	Shell_NotifyIcon(NIM_DELETE,&m_niData);
	::PostQuitMessage(0);
	return 0;
}

LRESULT CWorkWnd::OnNIMessage(WPARAM,LPARAM lParam)
{
	SetForegroundWindow();
	if((lParam==WM_LBUTTONDOWN)||(lParam==WM_RBUTTONDOWN))
	{
		POINT pt={0};
		GetCursorPos(&pt);
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

void CWorkWnd::OnOptions(WORD,HWND)
{
	CPropSheetOptions ps;
	CPropHotKeys propHotKeys;
	for(int i=0;i<hkoCount;++i)
		propHotKeys.m_arHotkey[i]=CHook::GetHook()->m_arHotKeyInfo[i];
	ps.AddPage(propHotKeys.Create(IDD_PROPPAGE_HOTKEYS));	

	CPropSystem propSystem;
	propSystem.m_fAutoRun=m_ProfileReg.IsAutoRun(CULStrTable(IDS_APP_NAME));
	ps.AddPage(propSystem.Create(IDD_PROPPAGE_SYSTEM));	

	ps.m_psh.dwFlags=PSH_NOAPPLYNOW|PSH_NOCONTEXTHELP;
	if(ps.Create(*this,CULStrTable(IDS_OPTIONSDLG_CAPTION),0)==IDOK)
	{
		for(int i=0;i<hkoCount;++i)
			CHook::GetHook()->m_arHotKeyInfo[i]=propHotKeys.m_arHotkey[i];
		SaveSettings();
		if(propSystem.m_fAutoRun)
		{
			TCHAR szFileName[0xffff];
			::GetModuleFileName(NULL,szFileName,0xffff);
			m_ProfileReg.AddToAutoRun(CULStrTable(IDS_APP_NAME),szFileName);
		}
		else
			m_ProfileReg.AddToAutoRun(CULStrTable(IDS_APP_NAME),NULL);
	}
}

void CWorkWnd::OnQuit(WORD,HWND)
{
	CHook::GetHook()->Restore();
	DestroyWindow();
}
