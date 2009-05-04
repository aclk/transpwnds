//------------------------------------------------------------------------------------------//
//																							//
// TranspWndsHKH.cpp																		//
// 30.04.09 (20.23)																			//
// Volodin Oleg																				//
//																							//
//------------------------------------------------------------------------------------------//
#define TRANSP_WNDS_HKH_EXPORTS

#include <windows.h>

#include "../Include/TranspWndsHKH.h"

#pragma data_seg("Shared")
HINSTANCE g_hInstance		= NULL;
HWND g_hHostWnd				= NULL;
HHOOK g_hCallWndProcHook	= NULL;
#pragma data_seg()
#pragma comment( linker, "/section:Shared,rws" )

BOOL WINAPI DllMain(
	HINSTANCE hInstanceDLL,
	DWORD fdwReason,
	LPVOID lpvReserved
)
{
	g_hInstance = hInstanceDLL;

	return TRUE;
}

LRESULT CALLBACK CallWndProcHookFunc(
	int nCode,
	WPARAM wParam,
	LPARAM lParam
)
{
	if ( nCode >= 0 )
	{
		PCWPSTRUCT pcws = (PCWPSTRUCT)lParam;

		if ( pcws->message == WM_MOUSEWHEEL )
		{
			PostMessage( g_hHostWnd, NWM_TOUCHPADMOUSEWHEEL, pcws->wParam, pcws->lParam );
		}
	}

	return CallNextHookEx( g_hCallWndProcHook, nCode, wParam, lParam );
}

TRANSP_WNDS_HKH_API
BOOL SetTranspWndsHookForTouchpad(
	HWND hHostWnd
)
{
	if (
		(g_hHostWnd = hHostWnd) != NULL &&
		(g_hCallWndProcHook = SetWindowsHookEx( WH_CALLWNDPROC, CallWndProcHookFunc, g_hInstance, NULL )) != NULL
		)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

TRANSP_WNDS_HKH_API
BOOL UnSetTranspWndsHookForTouchpad()
{
	return UnhookWindowsHookEx( g_hCallWndProcHook );
}
