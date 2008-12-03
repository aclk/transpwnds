#pragma once
#include <windows.h>
#include <map>




class CHook
{
public:
	HHOOK m_hMouse;
	///\brief ���������, ���������� ���������� �� ����
	///\param bAlpha - ����������
	///\param fAlpha - TRUE,���� ���� ��� �������� ����� WS_EX_LAYERED 
	///\param fTopMost - TRUE,���� ���� ��� �������� ����� WS_EX_TOPMOST 
	typedef struct tagWndInfo
	{
		BYTE bAlpha;
		BOOL fAlpha;
		BOOL fTopMost;
	}WNDINFO,*LPWNDINFO;
	///\brief ��� ���� - ���������� � ��
	std::map<HWND,WNDINFO> m_mapWndInfo;

public:
	CHook(void);
	~CHook(void);

	void Enable();
	void Disable();
	void Restore();

	static CHook* GetHook();
};
