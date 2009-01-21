#pragma once
#include "ULHttpFile.h"
#include "ULLibNS.h"

class CUpdater:
	public CULThread
{
public:
	enum 
	{
		UDM_NOTIFY=WM_APP+1,
	};
	enum enUpdateNotify
	{
		unCurrentVersion,
		unConnect,
		unRecieveData,
		unParseData,
		unLastVersion,
		unNewVersionAvail,
		unError
	};

	enum{constSectionSize=3};
	enum enUpdateType
	{
		utNever,
		utAtStart,
		utEvery24,
		utAtStartEvery24
	};
	enUpdateType m_UpdateType;
	HWND m_hwndNotify;
protected:
	int m_arLastVer[constSectionSize];
	int m_arCurrentVer[constSectionSize];
public:
	CUpdater(void);
	~CUpdater(void);
	BOOL GetLastVersion(int arVer[constSectionSize]);
	BOOL GetCurrentVersion(int arVer[constSectionSize]);
	BOOL CheckForUpdate(int arVer[constSectionSize]);
	BOOL SetNotifyWnd(HWND hwndNotify);
protected:
	BOOL Run();

};
