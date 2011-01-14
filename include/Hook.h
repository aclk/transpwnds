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
	hkoThroughClick,
	hkoCollapseWnd,
	hkoCount
};

enum enMoveSizeMethod
{
	msmProgram,
	msmSystem
};

enum enSizeMethodBy
{
	smbCorner,
	msmBorder
};

class CHook
{
public:
	HHOOK m_hMouse;
public:
	///\brief ���������, ���������� ���������� �� ����
	///\param bAlpha - ����������
	///\param fAlpha - TRUE,���� ���� ��� �������� ����� WS_EX_LAYERED 
	///\param fTopMost - TRUE,���� ���� ��� �������� ����� WS_EX_TOPMOST 
	///\param dwStyle - ����� ����,���� � ���� ��� ����� ���������, ����� 0
	///\param dwNonCollapseHeight - ������ ���� �� ���������� ������
	typedef struct tagWndInfo
	{
		BYTE bAlpha;
		BOOL fAlpha;
		BOOL fTopMost;
		DWORD dwStyle;
		DWORD dwNonCollapseHeight;
	}WNDINFO,*LPWNDINFO;
	///\brief ��� ���� - ���������� � ��
	std::map<HWND,WNDINFO> m_mapWndInfo;

	HOTKEYINFO m_arHotKeyInfo[hkoCount];

	BYTE m_bMinTranspVal;
	BYTE m_bTranspStep;

	enMoveSizeMethod m_MoveMethod;
	enMoveSizeMethod m_SizeMethod;
	enSizeMethodBy m_SizeMethodBy;
public:
	CHook(void);
	~CHook(void);

	void Enable();
	void Disable();
	void Restore();

	static CHook* GetHook();
public:
	LRESULT ProcessTransp( UINT uMsg, PMSLLHOOKSTRUCT lpMouseHookStruct );
	LRESULT ProcessTransp( UINT uMsg, POINT ptMouseAction, DWORD dwMouseData );
	LRESULT ProcessTopMost(UINT uMsg, PMSLLHOOKSTRUCT lpMouseHookStruct);
	LRESULT ProcessMoveWnd(UINT uMsg, PMSLLHOOKSTRUCT lpMouseHookStruct);
	LRESULT ProcessSizeWnd(UINT uMsg, PMSLLHOOKSTRUCT lpMouseHookStruct);
	LRESULT ProcessToggleCaption(UINT uMsg, PMSLLHOOKSTRUCT lpMouseHookStruct);
	LRESULT ProcessThroughClick(UINT uMsg, PMSLLHOOKSTRUCT lpMouseHookStruct);
	///\brief ������� ��������� ���������� ����
	LRESULT ProcessCollapseWnd(UINT uMsg, PMSLLHOOKSTRUCT lpMouseHookStruct);
};
