#pragma once
#include "ULLibNS.h"

//=======================================================
namespace ULWnds
{
	namespace ULControls
	{
		///\class CULSlider
		///\brief ����� ������ ��� slider control
		class CULSlider:
			public CULSubClass
		{
		public:
			///\brief �����������
			CULSlider();
			///\brief ������� ��� �������� ��������
			///\param hParentWnd - ����� ������������� ����
			///\param uID - ID ��������
			///\param szCaption - ��������� ��������
			///\param x,y - ���������� ��������
			///\param cx,cy - ������ ��������
			///\param dwStyle - ����� ��������
			///\param dwStyleEx - ����������� ����� 
			///\return TRUE � ������ ������
			BOOL Create(HWND hParentWnd,UINT uID,LPCTSTR szCaption,int x,int y,int cx,int cy,
				DWORD dwStyle=WS_CHILD|WS_VISIBLE|TBS_AUTOTICKS,DWORD dwStyleEx=NULL);
			///\brief ������� ����� �������� ��� ��������
			///\param nMin - ����������e ��������
			///\param nMax - ������������ ��������
			///\param fRedraw - ���� ����������� �������� ��� ��������� ���������
			void SetRange(int nMin,int nMax,BOOL fRedraw=TRUE);
			///\brief ������� ����� ����������� �������� ��� ��������
			///\param nMin - ����������e ��������
			///\param fRedraw - ���� ����������� �������� ��� ��������� ���������
			void SetRangeMin(int nMin,BOOL fRedraw=TRUE);
			///\brief ������� ����� ������������ �������� ��� ��������
			///\param nMax - ������������ ��������
			///\param fRedraw - ���� ����������� �������� ��� ��������� ���������
			void SetRangeMax(int nMax,BOOL fRedraw=TRUE);
			///\brief ������������� ������� ������� ��������
			///\param nPos - ������� �������
			///\param fRedraw - ���� ����������� �������� ��� ������� �������
			void SetPos(int nPos,BOOL fRedraw=TRUE);
			///\brief ���������� ������� ������� ��������
			///\return ������� �������
			int GetPos();
			///\brief ������������� ����� �������
			///\param nLen - ����� �������
			void SetThumbLenght(UINT nLen);
			///\brief ������������� ������� ��������� ��������
			///\param nFreq - ������� ��������� ��������
			void SetTicFreq(UINT nFreq);
			///\brief ������������� ��� ��������� ������� �������� ����������
			///\param nLineSize - ��� ��������� �������
			///\return ���������� ��� ��������� �������
			UINT SetLineSize(UINT nLineSize);
			///\brief ������������� ��� ��������� ������� �������� PageUp/PageDown \n
			/// � ������ ���� �� ��������� �������
			///\param nLineSize - ��� ��������� �������
			///\return ���������� ��� ��������� �������
			UINT SetPageSize(UINT nPageSize);
		};
	}
}
//=======================================================


class CPropTransparent :
	public CULPropPage
{
protected:
	CULSlider m_sliderMinTranspLevel;
	CULSlider m_sliderTranspLevelStep;
public:
	int m_nMinTransparentLevel;
	int m_nTransparentLevelStep;
public:
	CPropTransparent(void);
	~CPropTransparent(void);
protected:
	virtual LRESULT OnInitDialog(WPARAM,LPARAM);
	virtual LRESULT OnHScroll(WPARAM,LPARAM);
};
