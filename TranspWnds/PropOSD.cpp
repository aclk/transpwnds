#include "PropOSD.h"
#include "resource.h"

CPropOSD::CPropOSD(void):
	CULPropPage()
{
}

CPropOSD::~CPropOSD(void)
{
}

LRESULT CPropOSD::OnInitDialog(WPARAM w,LPARAM l)
{
	CULButton btnRadio;
	switch(m_osdPos)
	{
	case osdpTopLeft:
		btnRadio.Attach(GetDlgItem(IDC_RADIO_TOPLEFT));
		break;
	case osdpTopRight:
		btnRadio.Attach(GetDlgItem(IDC_RADIO_TOPRIGHT));
		break;
	case osdpBotLeft:
		btnRadio.Attach(GetDlgItem(IDC_RADIO_BOTLEFT));
		break;
	case osdpBotRight:
		btnRadio.Attach(GetDlgItem(IDC_RADIO_BOTRIGHT));
		break;
	case osdpCenter:
		btnRadio.Attach(GetDlgItem(IDC_RADIO_CENTER));
		break;
	}
	btnRadio.SetCheck(CULButton::csChecked);
	btnRadio.Detach();

	return CULPropPage::OnInitDialog(w,l);
}
LRESULT CPropOSD::OnApply(BYTE nReturn)
{
	if(SendDlgItemMessage(IDC_RADIO_TOPLEFT,BM_GETCHECK,0,0))
		m_osdPos=osdpTopLeft;
	else
		if(SendDlgItemMessage(IDC_RADIO_TOPRIGHT,BM_GETCHECK,0,0))
			m_osdPos=osdpTopRight;
		else
			if(SendDlgItemMessage(IDC_RADIO_BOTLEFT,BM_GETCHECK,0,0))
				m_osdPos=osdpBotLeft;
			else
				if(SendDlgItemMessage(IDC_RADIO_BOTRIGHT,BM_GETCHECK,0,0))
					m_osdPos=osdpBotRight;
				else
					if(SendDlgItemMessage(IDC_RADIO_CENTER,BM_GETCHECK,0,0))
						m_osdPos=osdpCenter;
					else
	return CULPropPage::OnApply(nReturn);
}