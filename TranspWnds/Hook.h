#pragma once
#include <windows.h>
#include <map>




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

public:
	CHook(void);
	~CHook(void);

	void Enable();
	void Disable();
	void Restore();

	static CHook* GetHook();
};
