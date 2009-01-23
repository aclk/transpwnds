#pragma once
#include "ULLibNS.h"

class CUpdateProgressDlg :
	public CULDlg
{
	enum{UDM_NOTIFY=WM_APP+1};
protected:
	CULListBox m_listUpdateLog;
	CULHyperLink m_hyperlinkDownloadPage;
public:
	CUpdateProgressDlg(void);
	~CUpdateProgressDlg(void);
	LRESULT OnInitDialog(WPARAM,LPARAM);
	LRESULT OnShowWindow(WPARAM,LPARAM);

	LRESULT OnUpdateNotify(WPARAM,LPARAM);
};
