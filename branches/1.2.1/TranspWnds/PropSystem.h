#pragma once
#include "ULLibNS.h"
class CPropSystem :
	public CULPropPage
{
public:
	BOOL m_fAutoRun;
public:
	CPropSystem(void);
	~CPropSystem(void);
protected:
	virtual LRESULT OnInitDialog(WPARAM,LPARAM);
	virtual LRESULT OnApply(BYTE nReturn);
};
