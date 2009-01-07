#pragma once

#include <windows.h>
#include <map>

typedef struct tagHotKeyInfo
{
	bool IsHotKey(PMSLLHOOKSTRUCT lpMouseHookStruct);
	bool IsMsg(int nMsg,UINT uMsg);
	BOOL m_fAlt;
	BOOL m_fCtrl;
	BOOL m_fShift;
	BOOL m_fWin;
	UINT m_uMsg[3];
	tagHotKeyInfo& operator=(tagHotKeyInfo& HotKeyInfo);
}HOTKEYINFO,*LPHOTKEYINFO;

enum enHotKeyOperations
{
	hkoTransp,
	hkoTopMost,
	hkoMoveWnd,
	hkoSizeWnd,
	hkoToggleCaption,
	hkoCount
};

class CHook
{
public:
	HHOOK m_hMouse;
	///\brief структура, содержащая информацию об окне
	///\param bAlpha - альфаканал
	///\param fAlpha - TRUE,если окну был присвоен стиль WS_EX_LAYERED 
	///\param fTopMost - TRUE,если окну был присвоен стиль WS_EX_TOPMOST 
	///\param dwStyle - стиль окна,если у окна был убран заголовок, иначе 0
	typedef struct tagWndInfo
	{
		BYTE bAlpha;
		BOOL fAlpha;
		BOOL fTopMost;
		DWORD dwStyle;
	}WNDINFO,*LPWNDINFO;
	///\brief мап окно - информация о нём
	std::map<HWND,WNDINFO> m_mapWndInfo;


	HOTKEYINFO m_arHotKeyInfo[hkoCount];

	BYTE m_bMinTranspVal;
	BYTE m_bTranspStep;
public:
	CHook(void);
	~CHook(void);

	void Enable();
	void Disable();
	void Restore();

	static CHook* GetHook();
public:
	LRESULT ProcessTransp(UINT uMsg, PMSLLHOOKSTRUCT lpMouseHookStruct);
	LRESULT ProcessTopMost(UINT uMsg, PMSLLHOOKSTRUCT lpMouseHookStruct);
	LRESULT ProcessMoveWnd(UINT uMsg, PMSLLHOOKSTRUCT lpMouseHookStruct);
	LRESULT ProcessSizeWnd(UINT uMsg, PMSLLHOOKSTRUCT lpMouseHookStruct);
	LRESULT ProcessToggleCaption(UINT uMsg, PMSLLHOOKSTRUCT lpMouseHookStruct);
};
