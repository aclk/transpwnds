#include "PropSheetOptions.h"
#include "resource.h"

CPropSheetOptions::CPropSheetOptions(void)
{
	MessageMap.AddMessage<CPropSheetOptions>(WM_INITDIALOG,&CPropSheetOptions::OnInitDialog);
}

CPropSheetOptions::~CPropSheetOptions(void)
{
}

LRESULT CPropSheetOptions::OnInitDialog(WPARAM,LPARAM)
{
	HICON hIcon=(HICON)::LoadImage(ULGetResourceHandle(),MAKEINTRESOURCE(IDI_MAINICON),
		IMAGE_ICON,0,0,LR_DEFAULTSIZE);
	SetIcon(hIcon,FALSE);
	//#define CS_DROPSHADOW       0x00020000
	SetClassLong(GCL_STYLE,GetClassLong(*this,GCL_STYLE)|0x00020000);
	return 0;
}