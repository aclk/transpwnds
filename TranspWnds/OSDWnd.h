#pragma once
#include "ULLibNS.h"

class COSDWnd :
	public CULWnd
{
protected:
	CULStr m_strText;
	RECT m_rcPadding;
public:
	enum enOSDPos
	{
		osdpTopLeft=0,
		osdpTopRight,
		osdpBotLeft,
		osdpBotRight,
		osdpCenter
	};
public:
	COSDWnd(void);
	~COSDWnd(void);
	BOOL Create(HWND hwndParent);
	void ShowText(TCHAR* pszText,enOSDPos osdPos);
protected:
	virtual LRESULT OnPaint(WPARAM,LPARAM);
};
