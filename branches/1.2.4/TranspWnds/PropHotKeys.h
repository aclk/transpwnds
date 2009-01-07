#pragma once

#include "Hook.h"

#include "ULLibNS.h"

class CPropHotKeys :
	public CULPropPage 
{
public:
	HOTKEYINFO m_arHotkey[hkoCount];
public:
	CPropHotKeys(void);
	~CPropHotKeys(void);
protected:
	virtual LRESULT OnInitDialog(WPARAM,LPARAM);
	virtual LRESULT OnCtlColor(WPARAM,LPARAM);

	virtual LRESULT OnApply(BYTE nReturn);
};
