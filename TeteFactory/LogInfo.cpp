#include "stdafx.h"
#include "LogInfo.h"



CInfo g_HandleLogModule;
CInfo::CInfo(void)
{
	bExit =FALSE;
	m_listInfo.clear();
	m_thread=::AfxBeginThread(InfoThread,this);
	GetAlarmInfo();

	std::vector<CString> vHead;
	vHead.push_back(_T("Time"));
	vHead.push_back(_T("等级"));
	vHead.push_back(_T("来源"));
	vHead.push_back(_T("信息"));
	m_fileLog.SetCsvFileHeader(vHead);

	vHead.clear();
	vHead.push_back(_T("Time"));
	vHead.push_back(_T("状态"));
	vHead.push_back(_T("等级"));
	vHead.push_back(_T("来源"));
	vHead.push_back(_T("报警ID"));
	vHead.push_back(_T("报警名称"));
	vHead.push_back(_T("报警信息"));
	vHead.push_back(_T("报警描述"));
	m_fileAlarm.SetCsvFileHeader(vHead);
}
UINT CInfo::InfoThread(LPVOID lParam)
{
	CInfo*pThis =(CInfo*)lParam;
	CString strInfo=_T("");
	int nLevel =-1;
	CString strSource=_T("");
	while(true)
	{
		if(pThis->bExit){break;;}
		if(!pThis->GetLog(strInfo,nLevel,strSource))
		{
			
			Sleep(1);
			continue;

		}
		CSingleLock lock(&pThis->m_csLockSave);
		lock.Lock();
		pThis->SaveLog(strInfo,nLevel,strSource);
		 lock.Unlock();
	//	Sleep(1);
	}
	OutputDebugString(_T("InfoThread"));
	return 0;
}

void CInfo::InsertInfo(CString strInfo, int nLevel,CString strSource)
{
	CSingleLock lock(&m_csLock);
	lock.Lock();
	ststrInfo info;
	info.nLevel =nLevel;
	info.strInfo =strInfo;
	info.strSource =strSource;
	m_listInfo.push_back(info);
	lock.Unlock();
}
BOOL  CInfo::GetAlarmInfoByID(int nAlarmID,CString& strName ,CString& strInfo)
{

	for (list<WARNING_INFO>::iterator it =m_listwarnInfo.begin();it!=m_listwarnInfo.end();it++)
	{
		WARNING_INFO Info =*it;
		if (it->nAlarmID ==nAlarmID)
		{
			strName =it->strName;
			strInfo =it->strInfo;
			return TRUE;
		}
	}
	return FALSE;

}

BOOL CInfo::InsertWarning(CString strinfo,int nLevel,CString strSource,int nAlarmID,CString strWaringDestription)
{

	
	CSingleLock lock(&m_csAlarm);
	try
	{
		
		lock.Lock();
	CTime timeNow = CTime::GetCurrentTime();
	CString strFile,strFilePath;
	strFile.Format(_T("\\%.4d%.2d%.2d.csv"), timeNow.GetYear(), timeNow.GetMonth(), timeNow.GetDay());
	strFilePath.Format(_T("\\激光作业日志文件\\报警日志\\%.4d%.2d\\"),timeNow.GetYear(),timeNow.GetMonth());
	TCHAR* pszPath = new TCHAR[1024];
	GetLocalPath(pszPath, 1024, TRUE);
	CString strFileName;
	strFileName.Format(_T("%s"), pszPath);
	delete[] pszPath;
	pszPath = NULL;

	
	
	strFileName = strFileName.Left(strFileName.Find(_T("\\")));    // 更改保存路径;
	strFileName += strFilePath;
	strFileName =_T("D:\\")+strFilePath+_T("\\");
	if (!vPathIsExist(strFileName))
	{
		MakeDirectory(strFileName);
	}
	strFileName += strFile;
	CString strWriteLog;
	CString strLevel;
	if (nLevel ==E_LOG_WARN_EVENT)
	{
		strLevel =_T("WARNING");
	}
	else if (nLevel==E_LOG_ERROR_EVENT)
	{
		strLevel =_T("ERROR");
	}
	CString strAlarmName=_T("NULL");
	CString strAlarmInfo=_T("NULL");

	if (GetAlarmInfoByID(nAlarmID,strAlarmName,strAlarmInfo))
	{
		//return FALSE;
	}
	strWriteLog.Format(_T("%s,发生,%s,%s,%d,%s,%s,%s"), timeNow.Format("%H:%M:%S"),strLevel,strSource,nAlarmID,strAlarmName,strAlarmInfo,strWaringDestription);//%Y-%m-%d 
	if (!m_fileAlarm.WriteString(strFileName,strWriteLog))
	{
		lock.Unlock();
		return FALSE;
	}
	////
	}
	catch(...)
	{
		lock.Unlock();
		return TRUE;
	}
	lock.Unlock();
	return TRUE;
}
BOOL CInfo::GetLog(CString& strinfo,int& nLevel,CString& strSource)
{
	if(m_listInfo.size() ==0)
	{
		return FALSE;
	}
	int nSize =m_listInfo.size();
	CSingleLock lock(&m_csLock);
	lock.Lock();
	ststrInfo info;
	info = m_listInfo.front();
	strinfo =info.strInfo;
	nLevel =info.nLevel;
	strSource =info.strSource;
	m_listInfo.pop_front();
	lock.Unlock();
	return TRUE;

}
CInfo::~CInfo(void)
{
	m_thread->PostThreadMessage(WM_QUIT,NULL,NULL);
	delete m_thread;
	m_thread =NULL;
	//m_thread->Eixt();//
}
 BOOL CInfo::vPathIsExist(LPCTSTR lpszPath)
{
	DWORD attr=::GetFileAttributes(lpszPath);
	if( -1==attr || (FILE_ATTRIBUTE_DIRECTORY&attr)!=FILE_ATTRIBUTE_DIRECTORY )
		return FALSE;
	return TRUE;
}

 BOOL CInfo::GetLocalPath(LPTSTR lpszRetBuff,long nBuffSize,BOOL bCorrect)
{
	int len;
	TCHAR* pCh;
	TCHAR* pszPath=new TCHAR[1024];
	////
	::GetModuleFileName(NULL,pszPath,1024);
	pCh=::_tcsrchr(pszPath,TCHAR('\\'));
	if( bCorrect ){
		if( NULL==pCh ){
			len=::_tcslen(pszPath);
			pszPath[len]=TCHAR('\\');
			len++;
			pszPath[len]=0;
		}
		else{
			pCh++;
			*pCh=0;
		}
	}
	else{
		if( NULL!=pCh ){
			*pCh=0;
		}
	}

	////
	len=::_tcslen(pszPath);
	if( NULL==lpszRetBuff || len<1 || nBuffSize<(len+1) )
	{
		if( NULL!=lpszRetBuff )
			lpszRetBuff[0]=0;
		delete[] pszPath;
		pszPath=NULL;
		pCh =NULL;
		return FALSE;
	}
	else
	{
		memcpy(lpszRetBuff,pszPath,sizeof(TCHAR)*(len+1));
		delete[] pszPath;
		pszPath =NULL;
		pCh =NULL;
		return TRUE;
	}
	
}
BOOL CInfo::SaveLog(CString strinfo,int nLevel,CString strSource)
{
	CSingleLock lock(&m_csAlarm);
	lock.Lock();
	
	CTime timeNow = CTime::GetCurrentTime();
	CString strFile,strFilePath;
	SYSTEMTIME sm;
	GetLocalTime(&sm);
	TCHAR* pszPath = new TCHAR[1024];
	GetLocalPath(pszPath, 1024, TRUE);
	CString strFileName;
	strFileName.Format(_T("%s"), pszPath);
	delete[] pszPath;
	pszPath = NULL;
	CString strUnit ;
	strUnit=_T("");
	strFileName = strFileName.Left(strFileName.Find(_T("\\")));    // 更改保存路径;


	strFileName += strFilePath;

	strFileName .Format(_T("D:\\log\\加工日志\\%.4d年%.2d月\\"),sm.wYear,sm.wMonth);
	if (!vPathIsExist(strFileName))
	{
		MakeDirectory(strFileName);
	}
	strFile.Format(_T("%.4d年%.2d月%.2d日.csv"),sm.wYear, sm.wMonth,sm.wDay);
	strFileName += strFile;
	CString strWriteLog;
	//时间 等级 来源  信息
	       // 信息事件 
		//E_LOG_INFO_TITLE,	       // 信息事件 
		//E_LOG_WARN_EVENT, 	       // 警告事件 
		//E_LOG_ERROR_EVENT,
		//E_LOG_SUCESS_EVENT,
		//E_LOG_NO_SHOW_EVENT,	   // 不显示信息事件
		//E_LOG_UNKONW_EVENT = 1024  // 未定义事件 
	if (E_LOG_INFO_EVENT ==nLevel || E_LOG_SUCESS_EVENT ==nLevel )
	{
		strWriteLog.Format(_T("%s.%.3d,INFO,%s,%s"), timeNow.Format("%H.%M.%S"), sm.wMilliseconds, strSource, strinfo);//%Y-%m-%d 

	}
	else if (E_LOG_ERROR_EVENT == nLevel )
	{
		strWriteLog.Format(_T("%s.%.3d,ERROR,%s,%s"), timeNow.Format("%H.%M.%S"), sm.wMilliseconds, strSource, strinfo);//%Y-%m-%d 

	}
	else if (E_LOG_WARN_EVENT == nLevel)
	{
		strWriteLog.Format(_T("%s.%.3d,WARN,%s,%s"), timeNow.Format("%H.%M.%S"), sm.wMilliseconds, strSource, strinfo);//%Y-%m-%d 

	}
	else
	{
		strWriteLog.Format(_T("%s.%.3d,OTHER,%s,%s"), timeNow.Format("%H.%M.%S"),sm.wMilliseconds,strSource,strinfo);//%Y-%m-%d 
		
	}


	if (!m_fileLog.WriteString(strFileName,strWriteLog))
	{
		lock.Unlock();
		return FALSE;

	}

	lock.Unlock();
	return TRUE;
}
BOOL CInfo::AddAlarmIdEvent(int nAlarmCode)
{

	return TRUE;
}
BOOL CInfo::GetAlarmInfo()
{
	TCHAR* pszPath = new TCHAR[1024];
	GetLocalPath(pszPath, 1024, TRUE);
	CString strFileName;
	strFileName.Format(_T("%s"), pszPath);
	delete[] pszPath;
	pszPath = NULL;
	strFileName+=_T("ALARMINFO.csv");
	CCsvFile m_file;
	CString strRead;
	if(!m_file.ReadString(strFileName, strRead))
	{
		return FALSE;
	}
	vector<CString>vLines;
	m_file.SpliteLine(strRead,vLines);
	if (vLines.size()<2)
	{
		return FALSE;
	}
	m_listwarnInfo.clear();
	for (int i=0;i<vLines.size();i++)
	{
		if ( 0==i)
		{
			continue;
		}
		CString strLine =vLines[i];
		vector<CString>vItems;
		vItems.clear();
		m_file.SpliteItem(strLine,vItems);
		if (vItems.size()<3)
		{
			continue;
		}
		
			
			int nAlarmId=_ttoi(vItems[0]);
			CString nAlarmName =vItems[1];
			CString AlarmInfo =vItems[2];
			WARNING_INFO info;
			info.nAlarmID = nAlarmId;
			info.strName =nAlarmName;
			info.strInfo =AlarmInfo;
			m_listwarnInfo.push_back(info);


		
		
		
	}
	return TRUE;

}
BOOL CInfo::MakeDirectory(CString strPath)
{
	strPath += _T("\\");

	// 创建文件路径
	int nStart = 0;
	int nEnd = 0;
	CString strTemp = _T("");
	nEnd = strPath.Find(_T("\\"), nEnd);
	while (nEnd != -1)
	{
		strTemp += strPath.Mid(nStart, nEnd-nStart);
		CreateDirectory(strTemp, NULL);
		strTemp += _T("\\");

		nStart = nEnd + 1;	
		nEnd = strPath.Find(_T("\\"), nEnd + 1);
	}

	return TRUE;
}
void CInfo::RecodeAndDisplay(CString strinfo, CString  strSource, emLogEvent logtype,int nAlarmID)
{
	// 
	InsertInfo(strinfo, logtype, strSource);
	return;
	//Time  level  Source Info
	switch(logtype)
	{
	case  E_LOG_INFO_EVENT:
		InsertInfo(strinfo,E_LOG_INFO_EVENT,strSource);
		break;
	case  E_LOG_WARN_EVENT:
	//	g_MarkFlowControl.SetAlarmBuzzer(TRUE);
		InsertWarning(strinfo,E_LOG_WARN_EVENT,strSource,nAlarmID,strinfo);
		break;
	case  E_LOG_ERROR_EVENT:
	//	g_MarkFlowControl.SetAlarmBuzzer(TRUE);
		InsertWarning(strinfo,E_LOG_ERROR_EVENT,strSource,nAlarmID,strinfo);
		break;
	}
}