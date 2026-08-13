/**
******************************************************************************
* @文件     
* @作者  
* @版本 V1.00.00
* @日期 2018-10-21
* @简述 
* @email:jchq0101net@sina.com
******************************************************************************
* @说明
*
*
* 
******************************************************************************
*/
#include "stdafx.h"
#include <Windows.h>
#include <process.h>
#include "tlTeteDefines.h"
#include "MarkThread.h"
typedef unsigned (WINAPI *PBEGINTHREADEX_THREADFUNC)(LPVOID lpThreadFunc);
typedef unsigned *PBEGINTHREADEX_THREADID;
CMarkThread* CMarkThread::_instance = NULL;
//---------------------------------------------------------------------------
CMarkThread *CMarkThread::Instance()
{
    if(NULL==_instance)
    {
        _instance = new CMarkThread();
    }
    return _instance;
}
CMarkThread::CMarkThread()
{
    ThreadProc = NULL;
    m_hThread = NULL;
    m_dwThreadID = 0;
}
CMarkThread::~CMarkThread()
{
    WaitForExit();
    _instance = NULL;
}
void CMarkThread::StartThread(LPVOID *pParam,MTFC_THREADPROC fcProc)
{
    m_pParam = pParam;
    ThreadProc = fcProc;
    m_hStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    m_hThread = (HANDLE)_beginthreadex(NULL, 0, (PBEGINTHREADEX_THREADFUNC)CMarkThread::ThreadFunc, (LPVOID)this, 0, (PBEGINTHREADEX_THREADID)&m_dwThreadID);
}
void CMarkThread::StopThread(void)
{
    SetEvent(m_hStopEvent);
    WaitForExit();
    ThreadProc = NULL;
}
bool CMarkThread::IsMarking(void) 
{ 
    if (NULL == m_hThread)
        return false;
    DWORD rtn = WaitForSingleObject(m_hThread, 0); 
    if (WAIT_OBJECT_0 == rtn)
        return true;
    else
        return false;
}
void CMarkThread::WaitForExit(void)
{   
    if (NULL == m_hThread)
        return;
    WaitForSingleObject(m_hThread, INFINITE);
    CloseHandle(m_hThread);
    m_hThread = NULL;
}
DWORD WINAPI CMarkThread::ThreadFunc(LPVOID param)
{
    CMarkThread *pct = (CMarkThread *)param;
    return pct->ThreadWorkFunc();
}
//--测试验证该函数
DWORD CMarkThread::ThreadWorkFunc(void)
{
    uint32_t iCount = 0;
    while(1)
    {
        if (NULL != ThreadProc)
            if(ThreadProc(m_pParam)<0) break;
        if (WaitForSingleObject(m_hStopEvent,0) != WAIT_TIMEOUT )
        {
            break;
        }
    }
    return 0;
}