#pragma once
#include "ULLibNS.h"
class CWorkWnd :
	public CULWnd
{
protected:
	CULMenu m_Menu;
	NOTIFYICONDATA m_niData;
	enum 
	{
		IDM_ENABLE=1001,
		IDM_DISABLE,
		IDM_RESTORE,
		IDM_AUTORUN,
		IDM_QUIT,	
		NIM_MESSAGE=WM_APP+1,
	};
public:
	CWorkWnd(void);
	~CWorkWnd(void);
protected:
	LRESULT OnCreate(WPARAM,LPARAM);
	LRESULT OnDestroy(WPARAM,LPARAM);
	LRESULT OnNIMessage(WPARAM,LPARAM);

	void OnEnable(WORD,HWND);
	void OnDisable(WORD,HWND);
	void OnRestore(WORD,HWND);
	void OnQuit(WORD,HWND);
};
