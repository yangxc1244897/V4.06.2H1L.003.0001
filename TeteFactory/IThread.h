// IThread.h: interface for the CIThread class.
//
//////////////////////////////////////////////////////////////////////
//
#include <afxwin.h>

#pragma once

class CIThread  
{
public:
	CIThread();
	virtual ~CIThread();

public:
	enum eUnBlock
	{
		eUnBlockExit,		//线程退出
		eUnBlockPause,		//线程暂停
		eUnBlockInput,		//消息到达
		eUnBlockContinue,	//线程继续
	};

public:
	void Run(AFX_THREADPROC pfnThreadProc, LPVOID pParam = NULL, DWORD nExitTimeOut = 30000/*ms*/);
	void Pause();
	void Resume();
	void Stop();
	void Exit();
	//if return true function must quit. (thread function call) 阻塞
	BOOL BlockWaitEvent();
	//return eUnBlockExit 线程退出;  return eUnBlockPause 线程暂停; eUnBlockInput 不处理; eUnBlockContinue 继续
	eUnBlock UnBlockWaitEvent();
	//if thread is run return true, else return false
	BOOL IsRun();
	//if return true m_eventExit states is triggered
	BOOL IsExit();
	//if return true m_eventPause states is triggered
	BOOL IsPause();

public:
	CEvent& GetExitEvent();
	//if return true function must quit. (thread function call) 阻塞
	BOOL BlockWaitTime(DWORD dwWaitTime/*ms*/);

protected:
	CEvent	m_eventExit;
	CEvent	m_eventPause;
	HANDLE	m_hThread;
	DWORD	m_nExitTimeOut;
};
