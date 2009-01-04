#define _WIN32_WINNT 0x0400
#include "PropHotKeys.h"
#include "resource.h"
CPropHotKeys::CPropHotKeys(void):
	CULPropPage()
{
//	MessageMap.AddMessage<CPropHotKeys>(WM_CTLCOLORSTATIC,&CPropHotKeys::OnCtlColor);
//	MessageMap.AddMessage<CPropHotKeys>(WM_CTLCOLORBTN,&CPropHotKeys::OnCtlColor);
}

CPropHotKeys::~CPropHotKeys(void)
{
}

LRESULT CPropHotKeys::OnInitDialog(WPARAM w,LPARAM l)
{
	//инициализируем горячие клавиши изменения прозрачности
	SendDlgItemMessage(IDC_CHECK_TRANSP_ALT,BM_SETCHECK,
		m_arHotkey[hkoTransp].m_fAlt,0);
	SendDlgItemMessage(IDC_CHECK_TRANSP_CTRL,BM_SETCHECK,
		m_arHotkey[hkoTransp].m_fCtrl,0);
	SendDlgItemMessage(IDC_CHECK_TRANSP_SHIFT,BM_SETCHECK,
		m_arHotkey[hkoTransp].m_fShift,0);
	SendDlgItemMessage(IDC_CHECK_TRANSP_WIN,BM_SETCHECK,
		m_arHotkey[hkoTransp].m_fWin,0);
	//инициализируем горячие клавиши для топмост
	SendDlgItemMessage(IDC_CHECK_TOPMOST_ALT,BM_SETCHECK,
		m_arHotkey[hkoTopMost].m_fAlt,0);
	SendDlgItemMessage(IDC_CHECK_TOPMOST_CTRL,BM_SETCHECK,
		m_arHotkey[hkoTopMost].m_fCtrl,0);
	SendDlgItemMessage(IDC_CHECK_TOPMOST_SHIFT,BM_SETCHECK,
		m_arHotkey[hkoTopMost].m_fShift,0);
	SendDlgItemMessage(IDC_CHECK_TOPMOST_WIN,BM_SETCHECK,
		m_arHotkey[hkoTopMost].m_fWin,0);

	CULComboBox cbTopMost;
	cbTopMost.Attach(GetDlgItem(IDC_COMBO_TOPMOST_MSG));
	int nItem=cbTopMost.AddString(CULStrTable(IDS_OPTOINS_TOPMOST_COMBO_LBTN));
	cbTopMost.SetItemData(nItem,WM_LBUTTONDOWN);
	nItem=cbTopMost.AddString(CULStrTable(IDS_OPTOINS_TOPMOST_COMBO_RBTN));
	cbTopMost.SetItemData(nItem,WM_RBUTTONDOWN);
	nItem=cbTopMost.AddString(CULStrTable(IDS_OPTOINS_TOPMOST_COMBO_MBTN));
	cbTopMost.SetItemData(nItem,WM_MBUTTONDOWN);
	for(int i=0;i<cbTopMost.GetCount();++i)
		if(m_arHotkey[hkoTopMost].m_uMsg==cbTopMost.GetItemData(i))
		{
			cbTopMost.SetCurSel(i);
			break;
		}
	cbTopMost.Detach();
	//инициализируем горячие клавиши для move
	SendDlgItemMessage(IDC_CHECK_MOVE_ALT,BM_SETCHECK,
		m_arHotkey[hkoMoveWnd].m_fAlt,0);
	SendDlgItemMessage(IDC_CHECK_MOVE_CTRL,BM_SETCHECK,
		m_arHotkey[hkoMoveWnd].m_fCtrl,0);
	SendDlgItemMessage(IDC_CHECK_MOVE_SHIFT,BM_SETCHECK,
		m_arHotkey[hkoMoveWnd].m_fShift,0);
	SendDlgItemMessage(IDC_CHECK_MOVE_WIN,BM_SETCHECK,
		m_arHotkey[hkoMoveWnd].m_fWin,0);

	CULComboBox cbMove;
	cbMove.Attach(GetDlgItem(IDC_COMBO_MOVE_MSG));
	nItem=cbMove.AddString(CULStrTable(IDS_OPTOINS_TOPMOST_COMBO_LBTN));
	cbMove.SetItemData(nItem,WM_LBUTTONDOWN);
	nItem=cbMove.AddString(CULStrTable(IDS_OPTOINS_TOPMOST_COMBO_RBTN));
	cbMove.SetItemData(nItem,WM_RBUTTONDOWN);
	nItem=cbMove.AddString(CULStrTable(IDS_OPTOINS_TOPMOST_COMBO_MBTN));
	cbMove.SetItemData(nItem,WM_MBUTTONDOWN);
	for(int i=0;i<cbMove.GetCount();++i)
		if(m_arHotkey[hkoMoveWnd].m_uMsg==cbMove.GetItemData(i))
		{
			cbMove.SetCurSel(i);
			break;
		}
	cbMove.Detach();
	
	return CULPropPage::OnInitDialog(w,l);
}

LRESULT CPropHotKeys::OnCtlColor(WPARAM w,LPARAM l)
{
	if(::GetDlgCtrlID((HWND)l)==(short)-1)
	{
		TCHAR szClass[50];
		if(::GetClassName((HWND)l,szClass,50))
			if(_tcsicmp(szClass,_T("BUTTON"))==0)
				return 0;
	}
	HDC hDC=(HDC)w;
	::SetBkMode(hDC,TRANSPARENT);
	return (LRESULT)::GetStockObject(NULL_BRUSH);
}

LRESULT CPropHotKeys::OnApply(BYTE nReturn)
{
	//возвращаем значение горячих клавиш изменения прозрачности
	m_arHotkey[hkoTransp].m_fAlt=(SendDlgItemMessage(IDC_CHECK_TRANSP_ALT,
		BM_GETCHECK,0,0)!=0);
	m_arHotkey[hkoTransp].m_fCtrl=(SendDlgItemMessage(IDC_CHECK_TRANSP_CTRL,
		BM_GETCHECK,0,0)!=0);
	m_arHotkey[hkoTransp].m_fShift=(SendDlgItemMessage(IDC_CHECK_TRANSP_SHIFT,
		BM_GETCHECK,0,0)!=0);
	m_arHotkey[hkoTransp].m_fWin=(SendDlgItemMessage(IDC_CHECK_TRANSP_WIN,
		BM_GETCHECK,0,0)!=0);
	//возвращаем значение горячих клавиш для топмост
	m_arHotkey[hkoTopMost].m_fAlt=(SendDlgItemMessage(IDC_CHECK_TOPMOST_ALT,
		BM_GETCHECK,0,0)!=0);
	m_arHotkey[hkoTopMost].m_fCtrl=(SendDlgItemMessage(IDC_CHECK_TOPMOST_CTRL,
		BM_GETCHECK,0,0)!=0);
	m_arHotkey[hkoTopMost].m_fShift=(SendDlgItemMessage(IDC_CHECK_TOPMOST_SHIFT,
		BM_GETCHECK,0,0)!=0);
	m_arHotkey[hkoTopMost].m_fWin=(SendDlgItemMessage(IDC_CHECK_TOPMOST_WIN,
		BM_GETCHECK,0,0)!=0);
	CULComboBox cbTopMost;
	cbTopMost.Attach(GetDlgItem(IDC_COMBO_TOPMOST_MSG));
	m_arHotkey[hkoTopMost].m_uMsg=cbTopMost.GetItemData(cbTopMost.GetCurSel());
	cbTopMost.Detach();
	//возвращаем значение горячих клавиш для move
	m_arHotkey[hkoMoveWnd].m_fAlt=(SendDlgItemMessage(IDC_CHECK_MOVE_ALT,
		BM_GETCHECK,0,0)!=0);
	m_arHotkey[hkoMoveWnd].m_fCtrl=(SendDlgItemMessage(IDC_CHECK_MOVE_CTRL,
		BM_GETCHECK,0,0)!=0);
	m_arHotkey[hkoMoveWnd].m_fShift=(SendDlgItemMessage(IDC_CHECK_MOVE_SHIFT,
		BM_GETCHECK,0,0)!=0);
	m_arHotkey[hkoMoveWnd].m_fWin=(SendDlgItemMessage(IDC_CHECK_MOVE_WIN,
		BM_GETCHECK,0,0)!=0);
	CULComboBox cbMove;
	cbMove.Attach(GetDlgItem(IDC_COMBO_MOVE_MSG));
	m_arHotkey[hkoMoveWnd].m_uMsg=cbMove.GetItemData(cbMove.GetCurSel());
	switch(m_arHotkey[hkoMoveWnd].m_uMsg)
	{
	case WM_LBUTTONDOWN:
		m_arHotkey[hkoMoveWnd].m_uMsg2=WM_LBUTTONUP;
		break;
	case WM_MBUTTONDOWN:
		m_arHotkey[hkoMoveWnd].m_uMsg2=WM_MBUTTONUP;
		break;
	case WM_RBUTTONDOWN:
		m_arHotkey[hkoMoveWnd].m_uMsg2=WM_RBUTTONUP;
		break;
	}
	cbMove.Detach();
	return CULPropPage::OnApply(nReturn);
}
