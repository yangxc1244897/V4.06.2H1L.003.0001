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
#pragma once
#include <Windows.h>
#include <process.h>
#include "tlTeteDefines.h"

//定义线程操作函数类型
typedef int32_t (* MTFC_THREADPROC)(LPVOID *pParam);
class CMarkThread
{
public:
    static CMarkThread *Instance();
    ~CMarkThread();
protected:
    CMarkThread();
    static CMarkThread *_instance;
public:
    void StartThread(LPVOID *pParam,MTFC_THREADPROC fcProc);
    void StopThread(void);
    bool IsMarking(void);
    void WaitForExit(void);

    static DWORD WINAPI ThreadFunc(LPVOID param);

protected:
    virtual DWORD ThreadWorkFunc();
    HANDLE m_hStopEvent;
    HANDLE m_hThread;
    DWORD m_dwThreadID;
    LPVOID *m_pParam;
    MTFC_THREADPROC ThreadProc;
};