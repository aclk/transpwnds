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
	COLORREF m_clrText;
	COLORREF m_clrTextShadow;
	LOGFONT m_lf;
public:
	COSDWnd(void);
	~COSDWnd(void);
	BOOL Create(HWND hwndParent);
	void ShowText(LPCTSTR pszText);
	void SetPos(enOSDPos osdPos);
	COSDWnd::enOSDPos GetPos();
	void SetTextColor(COLORREF clrText);
	COLORREF GetTextColor();
	void SetTextShadowColor(COLORREF clrTextShadow);
	COLORREF GetTextShadowColor();
	void SetFont(LOGFONT& lf);
	BOOL GetFont(LOGFONT* plf);
protected:
	virtual LRESULT OnPaint(WPARAM,LPARAM);
	virtual LRESULT OnTimer(WPARAM,LPARAM);
};
