#pragma once
#include "stdafx.h"
#include <list>
#include "CsvFile.h"
using namespace std;
//emMsgType_Normal = 0,
//emMsgType_Title,
//emMsgType_Warn,
//emMsgType_Error,
//emMsgType_Success
enum emLogEvent
{

	E_LOG_INFO_EVENT,	       // 信息事件 
	E_LOG_INFO_TITLE,	       // 信息事件 
	E_LOG_WARN_EVENT, 	       // 警告事件 
	E_LOG_ERROR_EVENT,
	E_LOG_SUCESS_EVENT,
	E_LOG_NO_SHOW_EVENT,	   // 不显示信息事件
	E_LOG_UNKONW_EVENT = 1024  // 未定义事件 
};


typedef struct WARNING_INFO
{
	int nAlarmID ;
	int nLevel;
	CString strInfo;//信息
	CString strSource;
	CString strName;//英文名称
	WARNING_INFO()
	{
		nAlarmID =-1;
		nLevel =-1;
		strInfo=_T("Unknow err");
		strSource=_T("NULL");
	}

}SWARN_INFO;
class CInfo
{
	struct ststrInfo
	{
		int nLevel ;
		CString strInfo;
		CString strSource;
		ststrInfo()
		{
			nLevel =-1;
			strInfo=_T("NULL");
			strSource=_T("NULL");
		}
	};
public:
	CInfo(void);
	~CInfo(void);
	list<ststrInfo> m_listInfo;

	BOOL InsertWarning(CString strinfo,int nLevel,CString strSource,int nAlarmID,CString strWaringDestription);
	BOOL GetLog(CString& strinfo,int& nLevel,CString& strSource);
	BOOL SaveLog(CString strinfo,int nLevel,CString strSource);
	BOOL GetLocalPath(LPTSTR lpszRetBuff,long nBuffSize,BOOL bCorrect=TRUE);
	BOOL GetAlarmInfoByID(int nAlarmID,CString& strName ,CString& strInfo);
	 
	CCriticalSection m_csLock;
	CCriticalSection m_csLockSave;
	CWinThread*m_thread;
	static UINT InfoThread(LPVOID lParam);
	void RecodeAndDisplay(CString strinfo, CString  strSource=_T(""), emLogEvent logtype=emLogEvent::E_LOG_INFO_EVENT,int nAlarmID =0);

	//报警日志
	list<WARNING_INFO> m_listwarnInfo;
	BOOL GetAlarmInfo();
	BOOL AddAlarmIdEvent(int nAlarmCode);
	BOOL vPathIsExist(LPCTSTR lpszPath);
	BOOL MakeDirectory(CString strPath);

	CCriticalSection m_csAlarm;
	
//	CStdioFile m_file;
	BOOL bExit ;
private:
	void InsertInfo(CString strInfo, int nLevel,CString strSource);
	//
	CCsvFile m_fileAlarm;
	CCsvFile m_fileLog;
	

};

extern CInfo g_HandleLogModule;

