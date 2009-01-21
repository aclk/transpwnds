#include "PropSystem.h"
#include "resource.h"
#include "UpdateProgressDlg.h"

CPropSystem::CPropSystem(void):
	CULPropPage()
{
	MessageMap.AddCommand<CPropSystem>(IDC_BUTTON_CHECKNOW,&CPropSystem::OnBtnCheckNow);
}

CPropSystem::~CPropSystem(void)
{
}

LRESULT CPropSystem::OnInitDialog(WPARAM w,LPARAM l)
{
	SendDlgItemMessage(IDC_CHECK_AUTORUN,BM_SETCHECK,(WPARAM)m_fAutoRun,0);	

	switch(m_UpdateType)
	{
	case utNever:
		SendDlgItemMessage(IDC_RADIO_NEVER,BM_SETCHECK,(WPARAM)1,0);	
		break;
	case utAtStart:
		SendDlgItemMessage(IDC_RADIO_ATSTART,BM_SETCHECK,(WPARAM)1,0);	
		break;
	case utEvery24:
		SendDlgItemMessage(IDC_RADIO_EVERY24HOURS,BM_SETCHECK,(WPARAM)1,0);	
		break;
	case utAtStartEvery24:
		SendDlgItemMessage(IDC_RADIO_ATSTARTANDEVERY24HOURS,BM_SETCHECK,(WPARAM)1,0);	
		break;
	}
	
	return CULPropPage::OnInitDialog(w,l);
}

LRESULT CPropSystem::OnApply(BYTE nReturn)
{
	m_fAutoRun=(BOOL)SendDlgItemMessage(IDC_CHECK_AUTORUN,BM_GETCHECK,0,0);	
	if(SendDlgItemMessage(IDC_RADIO_NEVER,BM_GETCHECK,0,0))
		m_UpdateType=utNever;
	else
		if(SendDlgItemMessage(IDC_RADIO_ATSTART,BM_GETCHECK,0,0))
			m_UpdateType=utAtStart;
		else
			if(SendDlgItemMessage(IDC_RADIO_EVERY24HOURS,BM_GETCHECK,0,0))
				m_UpdateType=utEvery24;
			else
				if(SendDlgItemMessage(IDC_RADIO_ATSTARTANDEVERY24HOURS,BM_GETCHECK,0,0))
					m_UpdateType=utAtStartEvery24;
	return CULPropPage::OnApply(nReturn);
}

void CPropSystem::OnBtnCheckNow(WORD,HWND)
{
	CUpdateProgressDlg dlg;
	dlg.CreateModal(IDD_DIALOG_UPDATEPROGRESS,*this);
}
