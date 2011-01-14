//------------------------------------------------------------------------------------------//
///\file TranspWndsHKH.h
///\date 30.04.09 (20.22)
///\author Volodin Oleg
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

///\brief включает/отключает блокировку изменения размера выбранного окна
///\param hTargetWnd - хендл окна, размер которого блокируется
///\param szWnd - размер, который должен быть постоянным у окна
///\param fEnable - флаг включения/выключения блокировки размера
///\note данная функция вспомогательная для инструмента сворачивания окна в заголовок
TRANSP_WNDS_HKH_API void EnableBlockSize(HWND hTargetWnd,SIZE szWnd,BOOL fEnable);