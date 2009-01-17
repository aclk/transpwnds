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
public:
	CPropOSD(void);
	~CPropOSD(void);
protected:
	virtual LRESULT OnInitDialog(WPARAM,LPARAM);

	virtual LRESULT OnApply(BYTE nReturn);
};
