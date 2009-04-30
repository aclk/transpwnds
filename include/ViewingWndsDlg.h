#pragma once
#include "ULLibNS.h"
class CViewingWndsDlg :
	public CULDlg
{
public:
	enum enColumnIds
	{
		ciTransparent=1000,
		ciTransparentLevel,
		ciTopMost,
		ciHideCaption
	};
protected:
	ULListCtrlEx::CULListCtrlEx m_listctrlViewingWnds;
public:
	CViewingWndsDlg(void);
	~CViewingWndsDlg(void);
	void DeleteItem(HWND hwndItem);
	void InsertItem(HWND hwndItem);
protected:
	void InitList();
protected:
	LRESULT OnInitDialog(WPARAM,LPARAM);
	LRESULT OnDestroy(WPARAM,LPARAM);

	void OnTransparent(WORD,HWND);
	void OnTransparentLevelComboSelenOk(WORD,HWND);
	void OnTopMost(WORD,HWND);
	void OnHideCaption(WORD,HWND);

	void OnCancel(WORD,HWND);

};
