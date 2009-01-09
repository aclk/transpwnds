#include "PropTransparent.h"
#include "resource.h"


//=======================================================
namespace ULWnds
{
	namespace ULControls
	{
		CULSlider::CULSlider():
			CULSubClass()
		{
		}
		BOOL CULSlider::Create(HWND hParentWnd,UINT uID,LPCTSTR szCaption,int x,int y,int cx,int cy,DWORD dwStyle,DWORD dwStyleEx)
		{
			m_hParentWnd=hParentWnd;
			m_hWnd = CreateWindowEx(dwStyleEx,TRACKBAR_CLASS,
				szCaption,
				dwStyle, 
				x, y, cx, cy, 
				hParentWnd,(HMENU)(LONG_PTR)uID,   
				ULOther::ULGetResourceHandle(), 
				NULL);
			return CULSubClass::Attach(m_hWnd);
		}

		void CULSlider::SetRange(int nMin,int nMax,BOOL fRedraw)
		{
			SendMessage(TBM_SETRANGE,(WPARAM)fRedraw,MAKELPARAM(nMin,nMax));
		}
		void CULSlider::SetRangeMin(int nMin,BOOL fRedraw)
		{
			SendMessage(TBM_SETRANGEMIN,(WPARAM)fRedraw,(LPARAM)nMin);
		}
		void CULSlider::SetRangeMax(int nMax,BOOL fRedraw)
		{
			SendMessage(TBM_SETRANGEMAX,(WPARAM)fRedraw,(LPARAM)nMax);
		}
		void CULSlider::SetPos(int nPos,BOOL fRedraw)
		{
			SendMessage(TBM_SETPOS,(WPARAM)fRedraw,(LPARAM)nPos);
		}
		int CULSlider::GetPos()
		{
			return (int)SendMessage(TBM_GETPOS);
		}
		void CULSlider::SetThumbLenght(UINT nLen)
		{
			ASSERT((GetWindowLong(GWL_STYLE)&TBS_FIXEDLENGTH)==TBS_FIXEDLENGTH);
			SendMessage(TBM_SETTHUMBLENGTH,(WPARAM)nLen,0);
		}
		void CULSlider::SetTicFreq(UINT nFreq)
		{
			ASSERT((GetWindowLong(GWL_STYLE)&TBS_NOTICKS)!=TBS_NOTICKS);
			SendMessage(TBM_SETTICFREQ,(WPARAM)nFreq,0);
		}
		UINT CULSlider::SetLineSize(UINT nLineSize) 
		{
			return (UINT)SendMessage(TBM_SETLINESIZE,0,(LPARAM)nLineSize);
		}
		UINT CULSlider::SetPageSize(UINT nPageSize) 
		{
			return (UINT)SendMessage(TBM_SETPAGESIZE,0,(LPARAM)nPageSize);
		}
		
	}
}
//=======================================================


CPropTransparent::CPropTransparent(void):
	CULPropPage(),
	m_nMinTransparentLevel(0),
	m_nTransparentLevelStep(0)
{
	MessageMap.AddMessage<CPropTransparent>(WM_HSCROLL,&CPropTransparent::OnHScroll);
}

CPropTransparent::~CPropTransparent(void)
{
}

LRESULT CPropTransparent::OnInitDialog(WPARAM w,LPARAM l)
{
	m_sliderMinTranspLevel.Attach(GetDlgItem(IDC_SLIDER_MIN_TRANSP_LEVEL));
	m_sliderMinTranspLevel.ModifyStyle(TBS_NOTICKS,TBS_FIXEDLENGTH|TBS_AUTOTICKS);

	m_sliderTranspLevelStep.Attach(GetDlgItem(IDC_SLIDER_TRANSP_LEVEL_STEP));
	m_sliderTranspLevelStep.ModifyStyle(TBS_NOTICKS,TBS_FIXEDLENGTH|TBS_AUTOTICKS);

	m_sliderMinTranspLevel.SetLineSize(5);
	m_sliderMinTranspLevel.SetPageSize(10);
	m_sliderMinTranspLevel.SetThumbLenght(15);

	m_sliderTranspLevelStep.SetThumbLenght(15);


	m_sliderMinTranspLevel.SetRange(10,90);
	m_sliderMinTranspLevel.SetPos(m_nMinTransparentLevel);

	m_sliderMinTranspLevel.SetTicFreq(10);

	m_sliderTranspLevelStep.SetRange(1,m_nMinTransparentLevel/2);
	m_sliderTranspLevelStep.SetTicFreq(((m_nMinTransparentLevel/2)>20)?2:1);
	m_sliderTranspLevelStep.SetPos(m_nMinTransparentLevel);

	return CULPropPage::OnInitDialog(w,l);
}

LRESULT CPropTransparent::OnHScroll(WPARAM wParam,LPARAM lParam)
{
	if(GetDlgItem(IDC_SLIDER_TRANSP_LEVEL)==(HWND)lParam)
	{
		if(LOWORD(wParam)==TB_ENDTRACK)
		{
			m_nMinTransparentLevel=m_sliderMinTranspLevel.GetPos();
			m_nTransparentLevelStep=m_sliderTranspLevelStep.GetPos();
			m_sliderTranspLevelStep.SetRange(1,m_nMinTransparentLevel/2);
			m_sliderTranspLevelStep.SetTicFreq(((m_nMinTransparentLevel/2)>20)?2:1);
			m_sliderTranspLevelStep.SetPos(m_nTransparentLevelStep);
		}
	}
	if(GetDlgItem(IDC_SLIDER_TRANSP_LEVEL_STEP)==(HWND)lParam)
		if(LOWORD(wParam)==TB_ENDTRACK)
			m_nTransparentLevelStep=m_sliderTranspLevelStep.GetPos();
	return 0;
}