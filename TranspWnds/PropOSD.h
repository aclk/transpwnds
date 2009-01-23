#pragma once
#include"ULLibNS.h"
class CPropOSD :
	public CULPropPage
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
public:
	enOSDPos m_osdPos;
	LOGFONT m_LogFont;
	COLORREF m_clrText;
	COLORREF m_clrTextShadow;

	CULBrush m_brushTextColor;
	CULBrush m_brushTextShadowColor;
public:
	CPropOSD(void);
	~CPropOSD(void);
protected:
	virtual LRESULT OnInitDialog(WPARAM,LPARAM);
	virtual LRESULT OnCtlColorStatic(WPARAM,LPARAM);

	virtual LRESULT OnApply(BYTE nReturn);

	void OnBtnFont(WORD,HWND);
	void OnBtnTextColor(WORD,HWND);
	void OnBtnTextShadowColor(WORD,HWND);
};
