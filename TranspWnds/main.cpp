#define _WIN32_WINNT 0x0501
#include <windows.h>
#include <tchar.h>
#include <assert.h>
#include "resource.h"

LRESULT CALLBACK WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

HINSTANCE hInst;
int __stdcall WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	if(CreateMutex(NULL,FALSE,_T("TranspWndSinletone"))==NULL)
		if(GetLastError()==ERROR_ALREADY_EXISTS)
			return 0;

	hInst=hInstance;

	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW; 
	wc.lpfnWndProc = (WNDPROC) WindowProc; 
	wc.cbClsExtra = 0; 
	wc.cbWndExtra = 0; 
	wc.hInstance = hInstance; 
	wc.hIcon=LoadIcon(NULL, IDI_APPLICATION); 
	wc.hCursor=LoadCursor(NULL, IDC_ARROW); 
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); 
	wc.lpszMenuName=NULL;
	wc.lpszClassName=_T("TranspWndClass");

	if (!RegisterClass(&wc)) 
		return FALSE; 

	HWND hWnd=CreateWindowEx(0,_T("TranspWndClass"),_T("Main Window"),
    WS_POPUP,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		(HWND) NULL,(HMENU) NULL,hInstance,NULL);
 
	if (!hWnd) 
		return FALSE; 

	MSG msg;
	while(GetMessage(&msg,0,0,0)&&IsWindow(hWnd))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
	return 0;
}

HHOOK hhMouse;

LRESULT CALLBACK MouseProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	if((wParam==WM_MOUSEWHEEL)&&(HIWORD(GetKeyState(VK_MENU))))
	{
		PMSLLHOOKSTRUCT phs=(PMSLLHOOKSTRUCT)lParam;
		HWND hWnd=::WindowFromPoint(phs->pt);
		while((GetWindowLong(hWnd,GWL_STYLE)&WS_CHILD)&&IsWindow(hWnd))
			hWnd=GetParent(hWnd);
		if(IsWindow(hWnd)&&((GetWindowLong(hWnd,GWL_STYLE)&WS_CHILD)==0))
		{
			if((GetWindowLong(hWnd,GWL_EXSTYLE)&WS_EX_LAYERED)==0)
			{
				if(((short)HIWORD(phs->mouseData))>0)
					return 1;
				::SetWindowLong(hWnd,GWL_EXSTYLE,GetWindowLong(hWnd,GWL_EXSTYLE)|WS_EX_LAYERED);
				BYTE bAlpha=0;
				if(GetLayeredWindowAttributes(hWnd,0,&bAlpha,0))
				{
					if(bAlpha!=255)
						SetLayeredWindowAttributes(hWnd,0,255,LWA_ALPHA);
				}
				else
					SetLayeredWindowAttributes(hWnd,0,255,LWA_ALPHA);
				RedrawWindow(hWnd,NULL,NULL,RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
			}
			BYTE bAlpha=0;
			if(GetLayeredWindowAttributes(hWnd,0,&bAlpha,0))
			{
				if(((short)HIWORD(phs->mouseData))<0)
				{
					if(bAlpha>100)
						bAlpha-=10;
				}
				else
				{
					if(bAlpha<245)
						bAlpha+=10;
					else
					{
						SetLayeredWindowAttributes(hWnd,0,255,LWA_ALPHA);
//						::SetWindowLong(hWnd,GWL_EXSTYLE,GetWindowLong(hWnd,GWL_EXSTYLE)&~WS_EX_LAYERED);
//						RedrawWindow(hWnd,NULL,NULL,RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
						return 1;
					}
				}
				
			}
			else
				SetLayeredWindowAttributes(hWnd,0,255,LWA_ALPHA);
			
			SetLayeredWindowAttributes(hWnd,0,bAlpha,LWA_ALPHA);
			return 1;
		}
	}
	return CallNextHookEx(hhMouse,nCode,wParam,lParam);
}

enum 
{
	IDM_ENABLE=1001,
	IDM_DISABLE,
	IDM_AUTORUN,
	IDM_QUIT,
	
	NIM_MESSAGE=WM_APP+1,
};

void AutoRun(BOOL fAdd)
{
	HKEY hKey;
	LPCTSTR szValName=_T("TranspWnds");
	TCHAR szFilePath[0xffff];
	::GetModuleFileName(hInst,szFilePath,0xffff);
	TCHAR szKeyName[]=_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
	LONG lResult=RegOpenKeyEx(HKEY_CURRENT_USER,szKeyName,0,(fAdd)?KEY_WRITE:KEY_SET_VALUE,&hKey);
	if(lResult==ERROR_SUCCESS)
		if(!fAdd)
		{
			lResult = ::RegDeleteValue(hKey,(LPTSTR)szValName);
		}
		else
			lResult=RegSetValueEx(hKey,szValName,0,REG_SZ,
				(BYTE*)szFilePath,(WORD)_tcslen(szFilePath)*sizeof(szFilePath[0]));
}
bool IsAutoRun()
{
	HKEY hKey;
	LPCTSTR szValName=_T("TranspWnds");
	TCHAR szKeyName[]=_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
	LONG lResult=RegOpenKeyEx(HKEY_CURRENT_USER,szKeyName,0,KEY_QUERY_VALUE,&hKey);
	TCHAR szValNameRet[0xffff]={0};
	bool fFind=false;
	for(DWORD i=0;;++i)
	{
		DWORD dwSize=0xffff;
		lResult=RegEnumValue(hKey,i++,szValNameRet,&dwSize,0,0,0,0);
		if(lResult==ERROR_SUCCESS)
		{
			if(_tcscmp(szValName,szValNameRet)==0)
			{
				fFind=true;
				break;
			}
		}
		else
			break;
	}
	::RegCloseKey(hKey);	
	return fFind;
}

LRESULT CALLBACK WindowProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	static HMENU hPupupMenu=NULL;
	static NOTIFYICONDATA niData={};

	switch(uMsg)
	{
	case WM_CREATE:
		{
			hPupupMenu=::CreatePopupMenu();
			::AppendMenu(hPupupMenu, MF_GRAYED|MF_DISABLED|MF_BYCOMMAND, IDM_ENABLE, _T("Enable"));
			::AppendMenu(hPupupMenu, MF_BYCOMMAND, IDM_DISABLE, _T("Disable"));
			::AppendMenu(hPupupMenu, ((IsAutoRun())?MF_CHECKED:0)|MF_BYCOMMAND, IDM_AUTORUN, _T("AutoRun"));			
			::AppendMenu(hPupupMenu, MF_BYCOMMAND, IDM_QUIT, _T("Quit"));
			
			niData.cbSize=sizeof(niData);
			niData.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP;
			niData.hWnd=hWnd;
			niData.hIcon=(HICON)LoadImage(hInst,MAKEINTRESOURCE(IDI_MAINICON),IMAGE_ICON,0,0,LR_DEFAULTSIZE|LR_DEFAULTCOLOR);
			if(niData.hIcon==NULL)
				::DestroyWindow(hWnd);
			_tcscpy_s(niData.szTip,64,_T("TranspWnds"));
			niData.uCallbackMessage=NIM_MESSAGE;
			niData.uID=NIM_MESSAGE;
			Shell_NotifyIcon(NIM_ADD,&niData);
			hhMouse=SetWindowsHookEx(WH_MOUSE_LL,MouseProc,hInst,0);
			break;
		}
	case NIM_MESSAGE:
		{
			if((lParam==WM_LBUTTONDOWN)||(lParam==WM_RBUTTONDOWN))
			{
				POINT pt={0};
				GetCursorPos(&pt);
				SetForegroundWindow(hWnd);
				TrackPopupMenu(hPupupMenu,TPM_RIGHTBUTTON,pt.x,pt.y,0,hWnd,NULL);
				PostMessage(hWnd,WM_NULL,0,0);
			}
			return 1;
		}
	case WM_COMMAND:
		{
			static MENUITEMINFO mii={0};
			switch(LOWORD(wParam))
			{
			case IDM_ENABLE:
				{
					hhMouse=SetWindowsHookEx(WH_MOUSE_LL,MouseProc,hInst,0);
					mii.cbSize=sizeof(MENUITEMINFO);
					mii.fMask=MIIM_STATE;
					mii.fState=MFS_GRAYED|MFS_DISABLED;
					SetMenuItemInfo(hPupupMenu,IDM_ENABLE,FALSE,&mii);
					mii.fState=MFS_ENABLED;
					SetMenuItemInfo(hPupupMenu,IDM_DISABLE,FALSE,&mii);
					break;
				}
			case IDM_DISABLE:
				UnhookWindowsHookEx(hhMouse);
				mii.cbSize=sizeof(MENUITEMINFO);
				mii.fMask=MIIM_STATE;
				mii.fState=MFS_GRAYED|MFS_DISABLED;
				SetMenuItemInfo(hPupupMenu,IDM_DISABLE,FALSE,&mii);
				mii.fState=MFS_ENABLED;
				SetMenuItemInfo(hPupupMenu,IDM_ENABLE,FALSE,&mii);
				hhMouse=NULL;
				break;
			case IDM_AUTORUN:
				{
					bool fAutoRun=IsAutoRun();
					AutoRun(!fAutoRun);
					mii.cbSize=sizeof(MENUITEMINFO);
					mii.fMask=MIIM_STATE;
					mii.fState=((IsAutoRun())?MF_CHECKED:0);
					SetMenuItemInfo(hPupupMenu,IDM_AUTORUN,FALSE,&mii);
					break;
				}
			case IDM_QUIT:
				DestroyWindow(hWnd);
				break;
			}
			break;
		}
	case WM_DESTROY:
		if(hhMouse)
			UnhookWindowsHookEx(hhMouse);
		Shell_NotifyIcon(NIM_DELETE,&niData);
		::PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}