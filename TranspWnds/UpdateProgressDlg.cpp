#include "UpdateProgressDlg.h"
#include "Updater.h"
#include "resource.h"
#include "WorkWnd.h"
#include <sstream>

CUpdateProgressDlg::CUpdateProgressDlg(void):
	CULDlg()
{
	MessageMap.AddMessage<CUpdateProgressDlg>(CUpdater::UDM_NOTIFY,&CUpdateProgressDlg::OnUpdateNotify);
	MessageMap.AddMessage<CUpdateProgressDlg>(WM_SHOWWINDOW,&CUpdateProgressDlg::OnShowWindow);
}

CUpdateProgressDlg::~CUpdateProgressDlg(void)
{
}
LRESULT CUpdateProgressDlg::OnInitDialog(WPARAM w,LPARAM l)
{
	m_listUpdateLog.Attach(GetDlgItem(IDC_LIST_UPDATELOG));

	return CULDlg::OnInitDialog(w,l);
}

LRESULT CUpdateProgressDlg::OnShowWindow(WPARAM wParam,LPARAM)
{
	if(wParam)
		CULApp::GetULApp()->m_pMainWnd->PostMessage(CWorkWnd::UDM_CHECKFORUPDATE,(WPARAM)m_hWnd,0);

	return 0;
}

LRESULT CUpdateProgressDlg::OnUpdateNotify(WPARAM wParam,LPARAM lParam)
{
	std::basic_stringstream<TCHAR> ss;
	switch(wParam)
	{
	case CUpdater::unCurrentVersion:
		{
			ss<<(LPTSTR)CULStrTable(IDS_CHECKFORUPDATE_CURRENTVERSION)<<" ";
			int* pVer=(int*)lParam;
			for(int i=0;i<CUpdater::constSectionSize;++i)
			{
				ss<<pVer[i];
				if(i!=(CUpdater::constSectionSize-1))
					ss<<".";
			}
			m_listUpdateLog.AddString(ss.str().c_str());
		}
		break;
	case CUpdater::unConnect:
		if(lParam==-1)
		{
			ss<<(LPTSTR)CULStrTable(IDS_CHECKFORUPDATE_CONNECT)<<"......";
			m_listUpdateLog.AddString(ss.str().c_str());
		}
		else
		{
			ss<<(LPTSTR)CULStrTable(IDS_CHECKFORUPDATE_CONNECT)<<"......";
			if(lParam==TRUE)
				ss<<(LPTSTR)CULStrTable(IDS_CHECKFORUPDATE_OK);
			if(lParam==FALSE)
				ss<<(LPTSTR)CULStrTable(IDS_CHECKFORUPDATE_FAIL);
			if(m_listUpdateLog.GetCount()>CUpdater::unConnect)
				m_listUpdateLog.DeleteString(m_listUpdateLog.GetCount()-1);
			m_listUpdateLog.AddString(ss.str().c_str());
		}
		break;
	case CUpdater::unRecieveData:
		if(lParam==0)
			m_listUpdateLog.AddString(CULStrTable(CULStrTable(IDS_CHECKFORUPDATE_RECEIVEDATA)));
		else
		{
			ss<<(LPTSTR)CULStrTable(IDS_CHECKFORUPDATE_RECEIVEDATA)<<"......"<<(int)lParam;
			ss<<" "<<(LPTSTR)CULStrTable(IDS_CHECKFORUPDATE_DATANAME);
			if(m_listUpdateLog.GetCount()>CUpdater::unRecieveData)
				m_listUpdateLog.DeleteString(m_listUpdateLog.GetCount()-1);
			m_listUpdateLog.AddString(ss.str().c_str());
		}
		break;
	case CUpdater::unParseData:
		ss<<(LPTSTR)CULStrTable(IDS_CHECKFORUPDATE_PARSEDATA)<<"......";
		if(lParam==TRUE)
			ss<<(LPTSTR)CULStrTable(IDS_CHECKFORUPDATE_OK);
		if(lParam==FALSE)
			ss<<(LPTSTR)CULStrTable(IDS_CHECKFORUPDATE_FAIL);
		m_listUpdateLog.AddString(ss.str().c_str());
		break;
	case CUpdater::unLastVersion:
		{
			ss<<(LPTSTR)CULStrTable(IDS_CHECKFORUPDATE_LASTVERSION)<<"...... ";
			int* pVer=(int*)lParam;
			for(int i=0;i<CUpdater::constSectionSize;++i)
			{
				ss<<pVer[i];
				if(i!=(CUpdater::constSectionSize-1))
					ss<<".";
			}
			m_listUpdateLog.AddString(ss.str().c_str());
		}
		break;
	case CUpdater::unNewVersionAvail:
		ss<<(LPTSTR)CULStrTable(IDS_CHECKFORUPDATE_NEWVERSIONAVAIL)<<"......";
		if(lParam==TRUE)
			ss<<(LPTSTR)CULStrTable(IDS_CHECKFORUPDATE_OK);
		if(lParam==FALSE)
			ss<<(LPTSTR)CULStrTable(IDS_CHECKFORUPDATE_FAIL);
		m_listUpdateLog.AddString(ss.str().c_str());
		break;
	case CUpdater::unError:
		ss<<(LPTSTR)CULStrTable(IDS_CHECKFORUPDATE_ERROR)<<"......";
		ss<<(LPTSTR)CULStrTable(IDS_CHECKFORUPDATE_FAIL);
		m_listUpdateLog.AddString(ss.str().c_str());
		break;
	}
	return 0;
}
