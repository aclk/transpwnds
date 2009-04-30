#pragma once
#include "ULLibNS.h"

class CPropSheetOptions :
	public CULPropSheet
{
public:
	CPropSheetOptions(void);
	~CPropSheetOptions(void);
protected:
	virtual LRESULT OnInitDialog(WPARAM,LPARAM);
};
