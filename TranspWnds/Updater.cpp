#include "Updater.h"

#include <string>
#include <sstream>

CUpdater::CUpdater(void):
	m_hwndNotify(NULL)
{
}

CUpdater::~CUpdater(void)
{
}

BOOL CUpdater::GetLastVersion(int arVer[constSectionSize])
{
	::PostMessage(m_hwndNotify,UDM_NOTIFY,(WPARAM)unConnect,-1);
	ULInet::CULHttpFile file;
	if(!file.Open(_T("http://code.google.com/p/transpwnds/downloads/list")))
	{
		::PostMessage(m_hwndNotify,UDM_NOTIFY,(WPARAM)unConnect,FALSE);
		return FALSE;
	}
	char  szData[101];
	DWORD dwBytesRead;
	bool fEnd=false;
	std::string strContent;
	::PostMessage(m_hwndNotify,UDM_NOTIFY,(WPARAM)unConnect,TRUE);
	::PostMessage(m_hwndNotify,UDM_NOTIFY,(WPARAM)unRecieveData,0);
	while(file.ReadFile(szData,sizeof(szData)-1,&dwBytesRead)&&!fEnd)
	{
		if(dwBytesRead==0)
			break;
		szData[dwBytesRead]=0;
		strContent+=szData;
		::PostMessage(m_hwndNotify,UDM_NOTIFY,(WPARAM)unRecieveData,(LPARAM)strContent.size());
		std::string::size_type nPos=strContent.find("version");
		if(nPos!=std::string::npos)
		{
			std::string::size_type nPosEnd=strContent.find(10,nPos);
			if(nPosEnd!=std::string::npos)
			{
				std::string strVersion;
				strVersion.append(strContent,nPos,nPosEnd-nPos);
				if(strVersion.size()>=(sizeof("version x.x.x")-1))
				{
					nPos=sizeof("version");
					bool fEndParseVersion=true;
					for(int i=0;i<constSectionSize;++i)
					{
						std::string strSec;
						nPosEnd=strVersion.find('.',nPos);
						if(nPosEnd)
						{
							strSec.append(strVersion,nPos,nPosEnd-nPos);
							std::stringstream ss;
							ss<<strSec;
							ss>>m_arLastVer[i];
							nPos=nPosEnd+1;
						}
						else
						{
							::PostMessage(m_hwndNotify,UDM_NOTIFY,(WPARAM)unParseData,(LPARAM)FALSE);
							fEndParseVersion=false;
							break;
						}
					}
					if(fEndParseVersion)
					{
						::PostMessage(m_hwndNotify,UDM_NOTIFY,(WPARAM)unParseData,(LPARAM)TRUE);
						::PostMessage(m_hwndNotify,UDM_NOTIFY,(WPARAM)unLastVersion,(LPARAM)m_arLastVer);
						fEnd=true;
					}
				}
			}
		}
	}

	if(arVer)
		for(int i=0;i<constSectionSize;++i)
			arVer[i]=m_arLastVer[i];
	return TRUE;
}

BOOL CUpdater::GetCurrentVersion(int arVer[constSectionSize])
{
	CULFileVersionInfo FileVersionInfo;
	TCHAR szFileName[0xffff];
	GetModuleFileName(ULOther::ULGetResourceHandle(),szFileName,sizeof(szFileName)/sizeof(szFileName[0]));
	FileVersionInfo.SetFileName(szFileName);
	TCHAR szVer[100];
	if(!FileVersionInfo.GetProductVersion(szVer,sizeof(szVer)/sizeof(szVer[0])))
		return FALSE;

	std::basic_string<TCHAR> strVersion;
	strVersion.assign(szVer);
	std::basic_string<TCHAR>::size_type nPos=0;
	bool fEndParseVersion=true;
	for(int i=0;i<constSectionSize;++i)
	{
		std::basic_string<TCHAR> strSec;
		std::basic_string<TCHAR>::size_type nPosEnd=strVersion.find('.',nPos);
		if(nPosEnd)
		{
			strSec.append(strVersion,nPos,nPosEnd-nPos);
			std::basic_stringstream<TCHAR> ss;
			ss<<strSec;
			ss>>m_arCurrentVer[i];
			nPos=nPosEnd+1;
		}
		else
		{
			fEndParseVersion=false;
			break;
		}
	}
	if(!fEndParseVersion)
		return FALSE;
	if(arVer)
		for(int i=0;i<constSectionSize;++i)
			arVer[i]=m_arCurrentVer[i];

	::PostMessage(m_hwndNotify,UDM_NOTIFY,(WPARAM)unCurrentVersion,(LPARAM)m_arCurrentVer);
	
	return TRUE;
}

BOOL CUpdater::CheckForUpdate(int arVer[constSectionSize])
{
	if(!GetCurrentVersion(NULL))
	{
		::PostMessage(m_hwndNotify,UDM_NOTIFY,(WPARAM)unError,(LPARAM)0);
		return FALSE;
	}
	if(!GetLastVersion(NULL))
	{
		::PostMessage(m_hwndNotify,UDM_NOTIFY,(WPARAM)unError,(LPARAM)0);
		return FALSE;
	}
	BOOL fUpdate=TRUE;
	for(int i=0;i<constSectionSize;++i)
	{
		if(m_arLastVer[i]<m_arCurrentVer[i])
		{
			fUpdate=FALSE;
			break;
		}
	}
	if(arVer)
		for(int i=0;i<constSectionSize;++i)
			arVer[i]=m_arLastVer[i];
	::PostMessage(m_hwndNotify,UDM_NOTIFY,(WPARAM)unNewVersionAvail,(LPARAM)fUpdate);
	return fUpdate;
}

BOOL CUpdater::SetNotifyWnd(HWND hwndNotify)
{
	if(!::IsWindow(hwndNotify))
		return FALSE;
	m_hwndNotify=hwndNotify;
	return TRUE;
}

BOOL CUpdater::Run()
{
	CheckForUpdate(NULL);
	return FALSE;
}