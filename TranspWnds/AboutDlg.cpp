#include "AboutDlg.h"
#include "resource.h"
CAboutDlg::CAboutDlg(void):
	CULDlg()
{
}

CAboutDlg::~CAboutDlg(void)
{
}

LRESULT CAboutDlg::OnInitDialog(WPARAM w,LPARAM l)
{
	HICON hIcon=(HICON)::LoadImage(ULGetResourceHandle(),MAKEINTRESOURCE(IDI_MAINICON),
		IMAGE_ICON,0,0,LR_DEFAULTSIZE);
	SetIcon(hIcon,FALSE);

	m_hlHomePage.CreateHyperLink(*this,115,47,_T("http://transpwnds.googlecode.com"),
		_T("http://transpwnds.googlecode.com"),0);
	m_hlHomePage.SetStyles(ULWnds::ULControls::ULButtons::CULHyperLink::tDefault,
		ULWnds::ULControls::ULButtons::CULHyperLink::sNone,RGB(0,0,255));
	m_hlHomePage.SetStyles(ULWnds::ULControls::ULButtons::CULHyperLink::tHiLite,
		ULWnds::ULControls::ULButtons::CULHyperLink::sUnderLine,RGB(0,0,255));
	m_hlHomePage.SetStyles(ULWnds::ULControls::ULButtons::CULHyperLink::tDown,
		ULWnds::ULControls::ULButtons::CULHyperLink::enStyles
		((int)ULWnds::ULControls::ULButtons::CULHyperLink::sUnderLine),RGB(64,0,255));
	m_hlHomePage.SetStyles(ULWnds::ULControls::ULButtons::CULHyperLink::tVisited,
		ULWnds::ULControls::ULButtons::CULHyperLink::enStyles
		((int)ULWnds::ULControls::ULButtons::CULHyperLink::sUnderLine),RGB(128,0,255));
	return CULDlg::OnInitDialog(w,l);
}