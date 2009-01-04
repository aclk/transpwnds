#pragma once

#include <windows.h>
#include <map>

typedef struct tagHotKeyInfo
{
	bool IsHotKey(UINT uMsg,PMSLLHOOKSTRUCT lpMouseHookStruct);
	BOOL m_fAlt;
	BOOL m_fCtrl;
	BOOL m_fShift;
	BOOL m_fWin;
	UINT m_uMsg;
	UINT m_uMsg1;
	UINT m_uMsg2;
	tagHotKeyInfo& operator=(tagHotKeyInfo& HotKeyInfo);
}HOTKEYINFO,*LPHOTKEYINFO;

enum enHotKeyOperations
{
	hkoTransp,
	hkoTopMost,
	hkoMoveWnd,
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
	typedef struct tagWndInfo
	{
		BYTE bAlpha;
		BOOL fAlpha;
		BOOL fTopMost;
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
};
