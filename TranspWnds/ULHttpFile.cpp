#include "ULHttpFile.h"
#include "ULStates.h"

#pragma comment(lib,"Wininet.lib")

namespace ULInet
{
	CULHttpFile::CULHttpFile(void):
		m_hInternetSession(NULL),
		m_hFile(NULL),
		m_dwOpenFlags(NULL)
	{
	}

	CULHttpFile::~CULHttpFile(void)
	{
		Close();
	}

	BOOL CULHttpFile::Open(LPCTSTR lpszUrl,DWORD dwFlags)
	{
		m_dwOpenFlags=dwFlags;
		m_hInternetSession=InternetOpen(_T("ULHttpFile"),INTERNET_OPEN_TYPE_PRECONFIG,NULL,0,0);
		if(!m_hInternetSession)
			return FALSE;
		m_hFile=InternetOpenUrl(m_hInternetSession,lpszUrl,NULL,0,dwFlags,NULL);
		if(!m_hFile)
		{
			InternetCloseHandle(m_hInternetSession);
			m_hInternetSession=NULL;
			return FALSE;
		}
		return TRUE;
	}
	BOOL CULHttpFile::Close()
	{
		if(!(m_hInternetSession&&m_hFile))
			return FALSE;
		if(!InternetCloseHandle(m_hInternetSession))
			return FALSE;
		if(!InternetCloseHandle(m_hFile))
			return FALSE;
		return TRUE;
	}
	DWORD CULHttpFile::SetFilePointer(LONG lDistanceToMove,DWORD dwMoveMethod)
	{
		ASSERT(!(m_dwOpenFlags&(INTERNET_FLAG_DONT_CACHE|INTERNET_FLAG_NO_CACHE_WRITE)));
		return InternetSetFilePointer(m_hFile,lDistanceToMove,NULL,dwMoveMethod,NULL);
	}
	BOOL CULHttpFile::ReadFile(LPVOID lpBuffer,DWORD dwNumberOfBytesToRead,LPDWORD lpdwNumberOfBytesRead)
	{
		if((!lpBuffer)||(!lpdwNumberOfBytesRead))
			return FALSE;
		return InternetReadFile(m_hFile,lpBuffer,dwNumberOfBytesToRead,lpdwNumberOfBytesRead);
	}
}