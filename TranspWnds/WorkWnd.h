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
		NIM_MESSAGE=WM_APP+1,
	};

	CULProfileReg m_ProfileReg;
public:
	CWorkWnd(void);
	~CWorkWnd(void);
	void LoadSettings();
	void SaveSettings();
protected:
	LRESULT OnCreate(WPARAM,LPARAM);
	LRESULT OnDestroy(WPARAM,LPARAM);
	LRESULT OnNIMessage(WPARAM,LPARAM);

	void OnEnable(WORD,HWND);
	void OnDisable(WORD,HWND);
	void OnRestore(WORD,HWND);

	void OnViewingWnds(WORD,HWND);

	void OnOptions(WORD,HWND);
	
	void OnAbout(WORD,HWND);
	
	void OnQuit(WORD,HWND);
};
