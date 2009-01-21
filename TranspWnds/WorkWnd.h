#pragma once
#include "ULLibNS.h"
#include "ViewingWndsDlg.h"
#include "PropSheetOptions.h"
#include "OSDWnd.h"
#include "Updater.h"

class CWorkWnd :
	public CULWnd
{
public:
	enum 
	{
		NIM_MESSAGE=WM_APP+1,
		OSDM_MESSAGE,
		UDM_CHECKFORUPDATE
	};
	COSDWnd m_osdWnd;
protected:
	CULMenu m_Menu;
	NOTIFYICONDATA m_niData;

	CUpdater m_Updater;

	CULProfileReg m_ProfileReg;

	CViewingWndsDlg m_ViewingWndsDlg;
	CPropSheetOptions m_propsheetOptions;
public:
	CWorkWnd(void);
	~CWorkWnd(void);
	void LoadSettings();
	void SaveSettings();
protected:
	LRESULT OnCreate(WPARAM,LPARAM);
	LRESULT OnTimer(WPARAM,LPARAM);
	LRESULT OnDestroy(WPARAM,LPARAM);
	LRESULT OnNIMessage(WPARAM,LPARAM);
	LRESULT OnOSDMessage(WPARAM,LPARAM);
	LRESULT OnCheckForUpdate(WPARAM,LPARAM);

	void OnEnable(WORD,HWND);
	void OnDisable(WORD,HWND);
	void OnRestore(WORD,HWND);

	void OnViewingWnds(WORD,HWND);

	void OnOptions(WORD,HWND);
	
	void OnAbout(WORD,HWND);
	
	void OnQuit(WORD,HWND);
};
