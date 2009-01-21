#pragma once
#include "ULLibNS.h"

class CUpdateProgressDlg :
	public CULDlg
{
protected:
	CULListBox m_listUpdateLog;
public:
	CUpdateProgressDlg(void);
	~CUpdateProgressDlg(void);
	LRESULT OnInitDialog(WPARAM,LPARAM);
	LRESULT OnShowWindow(WPARAM,LPARAM);

	LRESULT OnUpdateNotify(WPARAM,LPARAM);
};
