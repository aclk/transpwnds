//------------------------------------------------------------------------------------------//
//																							//
// TranspWndsHKH.h																			//
// 30.04.09 (20.22)																			//
// Volodin Oleg																				//
//																							//
//------------------------------------------------------------------------------------------//
#pragma once

#ifdef TRANSP_WNDS_HKH_EXPORTS
	#define TRANSP_WNDS_HKH_API __declspec(dllexport)
#else
	#define TRANSP_WNDS_HKH_API __declspec(dllimport)
#endif

#define NWM_TOUCHPADMOUSEWHEEL WM_USER + 0x10

TRANSP_WNDS_HKH_API
BOOL SetTranspWndsHookForTouchpad(HWND hHostWnd);

TRANSP_WNDS_HKH_API
BOOL UnSetTranspWndsHookForTouchpad();