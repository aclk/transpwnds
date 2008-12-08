#include "PropSystem.h"
#include "resource.h"

CPropSystem::CPropSystem(void):
	CULPropPage()
{
}

CPropSystem::~CPropSystem(void)
{
}

LRESULT CPropSystem::OnInitDialog(WPARAM w,LPARAM l)
{
	SendDlgItemMessage(IDC_CHECK_AUTORUN,BM_SETCHECK,(WPARAM)m_fAutoRun,0);	
	return CULPropPage::OnInitDialog(w,l);
}

LRESULT CPropSystem::OnApply(BYTE nReturn)
{
	m_fAutoRun=(BOOL)SendDlgItemMessage(IDC_CHECK_AUTORUN,BM_GETCHECK,0,0);	

	return CULPropPage::OnApply(nReturn);
}
