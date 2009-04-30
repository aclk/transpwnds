#pragma once
#include "ULLibNS.h"
class CAboutDlg :
	public CULDlg
{
protected:
	CULHyperLink m_hlHomePage;
public:
	CAboutDlg(void);
	~CAboutDlg(void);
protected:
	virtual LRESULT OnInitDialog(WPARAM,LPARAM);
};
