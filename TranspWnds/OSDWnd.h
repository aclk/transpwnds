#pragma once
#include "ULLibNS.h"

class COSDWnd :
	public CULWnd
{
public:
	enum enOSDPos
	{
		osdpTopLeft=0,
		osdpTopRight,
		osdpBotLeft,
		osdpBotRight,
		osdpCenter
	};
protected:
	enOSDPos m_osdPos;

	CULStr m_strText;
	RECT m_rcPadding;
	const int m_constIdTimer;
	UINT m_nCurTimout;
	BYTE m_Alpha;
public:
	COSDWnd(void);
	~COSDWnd(void);
	BOOL Create(HWND hwndParent);
	void ShowText(LPCTSTR pszText);
	void SetPos(enOSDPos osdPos);
	COSDWnd::enOSDPos GetPos();
protected:
	virtual LRESULT OnPaint(WPARAM,LPARAM);
	virtual LRESULT OnTimer(WPARAM,LPARAM);
};
