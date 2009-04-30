#pragma once
#ifndef _ULHTTPFILE_H__
#define _ULHTTPFILE_H__
#include <windows.h>
#include <Wininet.h>

namespace ULInet
{
	class CULHttpFile
	{
	protected:
		HINTERNET m_hInternetSession;
		HINTERNET m_hFile;
		DWORD m_dwOpenFlags;
	public:
		CULHttpFile(void);
		~CULHttpFile(void);
		BOOL Open(LPCTSTR lpszUrl,DWORD dwFlags=INTERNET_FLAG_NO_CACHE_WRITE);
		BOOL Close();
		DWORD SetFilePointer(LONG lDistanceToMove,DWORD dwMoveMethod=FILE_BEGIN);
		BOOL ReadFile(LPVOID lpBuffer,DWORD dwNumberOfBytesToRead,LPDWORD lpdwNumberOfBytesRead);

	};
}
#endif //_ULHTTPFILE_H__
