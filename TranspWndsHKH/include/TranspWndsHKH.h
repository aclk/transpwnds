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

///\brief ��������/��������� ���������� ��������� ������� ���������� ����
///\param hTargetWnd - ����� ����, ������ �������� �����������
///\param szWnd - ������, ������� ������ ���� ���������� � ����
///\param fEnable - ���� ���������/���������� ���������� �������
///\note ������ ������� ��������������� ��� ����������� ������������ ���� � ���������
TRANSP_WNDS_HKH_API void EnableBlockSize(HWND hTargetWnd,SIZE szWnd,BOOL fEnable);