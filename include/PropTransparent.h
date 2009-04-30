#pragma once
#include "ULLibNS.h"

class CPropTransparent :
	public CULPropPage
{
protected:
	CULSlider m_sliderMinTranspLevel;
	CULSlider m_sliderTranspLevelStep;
public:
	int m_nMinTransparentLevel;
	int m_nTransparentLevelStep;
public:
	CPropTransparent(void);
	~CPropTransparent(void);
protected:
	virtual LRESULT OnInitDialog(WPARAM,LPARAM);
	virtual LRESULT OnHScroll(WPARAM,LPARAM);
};
