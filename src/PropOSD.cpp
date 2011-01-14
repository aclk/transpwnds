#include "../Include/PropOSD.h"
#include "../res/resource.h"

CPropOSD::CPropOSD(void):
	CULPropPage(),
	m_clrText(0xff0000),
	m_clrTextShadow(0x0000ff)
{
	MessageMap.AddCommand<CPropOSD>(IDC_BUTTON_FONT,&CPropOSD::OnBtnFont);
	MessageMap.AddCommand<CPropOSD>(IDC_BUTTON_TEXTCOLOR,&CPropOSD::OnBtnTextColor);
	MessageMap.AddCommand<CPropOSD>(IDC_BUTTON_TEXTSHADOWCOLOR,&CPropOSD::OnBtnTextShadowColor);

	MessageMap.AddMessage<CPropOSD>(WM_CTLCOLORSTATIC,&CPropOSD::OnCtlColorStatic);
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

	m_brushTextColor.CreateSolidBrush(m_clrText);
	m_brushTextShadowColor.CreateSolidBrush(m_clrTextShadow);

	return CULPropPage::OnInitDialog(w,l);
}

LRESULT CPropOSD::OnCtlColorStatic(WPARAM,LPARAM lParam)
{
	switch(GetDlgCtrlID((HWND)lParam))
	{
	case IDC_STATIC_TEXTCOLOR:
		return (LRESULT)(HBRUSH)m_brushTextColor;
	case IDC_STATIC_TEXTSHADOWCOLOR:
		return (LRESULT)(HBRUSH)m_brushTextShadowColor;
	default:
		return NULL;
	}
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
	return CULPropPage::OnApply(nReturn);
}

void CPropOSD::OnBtnFont(WORD,HWND)
{
	CULFontDlg fd;
	fd.SetLogFont(m_LogFont);
	if(fd.CreateModal(0,*this)==IDOK)
		fd.GetLogFont(&m_LogFont);
}

void CPropOSD::OnBtnTextColor(WORD,HWND)
{
	CULColorDlg cd(m_clrText);
	if(cd.CreateModal(0,*this)==IDOK)
	{
		m_clrText=cd.GetColor();
		m_brushTextColor.DeleteBrush();
		m_brushTextColor.CreateSolidBrush(m_clrText);
		InvalidateRect();
	}
}

void CPropOSD::OnBtnTextShadowColor(WORD,HWND)
{
	CULColorDlg cd(m_clrTextShadow);
	if(cd.CreateModal(0,*this)==IDOK)
	{
		m_clrTextShadow=cd.GetColor();
		m_brushTextShadowColor.DeleteBrush();
		m_brushTextShadowColor.CreateSolidBrush(m_clrTextShadow);
		InvalidateRect();
	}
}


