// IThread.cpp: implementation of the CIThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IThread.h"
//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIThread::CIThread() : m_eventPause(FALSE, TRUE), m_eventExit(FALSE, TRUE)
{
	m_hThread = INVALID_HANDLE_VALUE;
	m_nExitTimeOut = 3000;
}

CIThread::~CIThread()
{
	Exit();
}

void CIThread::Run(AFX_THREADPROC pfnThreadProc, LPVOID pParam /* = NULL */, DWORD nExitTimeOut /* = 3000 */)
{
	Exit();
	//
	m_nExitTimeOut = nExitTimeOut;
	CWinThread* pThread = ::AfxBeginThread(pfnThreadProc, pParam, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	::DuplicateHandle (GetCurrentProcess()
		, pThread->m_hThread
		, GetCurrentProcess()
		, &m_hThread,
		0, FALSE, DUPLICATE_SAME_ACCESS);
	pThread->ResumeThread();
}

void CIThread::Pause()
{
	m_eventPause.SetEvent();
}

void CIThread::Resume()
{
	m_eventPause.ResetEvent();
}

void CIThread::Stop()
{
	m_eventExit.SetEvent();
}

void CIThread::Exit()
{
	if (INVALID_HANDLE_VALUE != m_hThread)
	{
		m_eventExit.SetEvent();
		Sleep(1);
		//
		DWORD dwStart = GetTickCount();
		BOOL bWait = FALSE;
		for ( ; ; )
		{
//			DWORD dwRet = MsgWaitForMultipleObjects(1, &m_hThread, TRUE, 0, QS_ALLINPUT);
			DWORD dwRet = MsgWaitForMultipleObjects(1, &m_hThread, FALSE, 0, QS_ALLINPUT);
			switch( dwRet )
			{
			case WAIT_OBJECT_0:
				bWait = TRUE;
				break;
			case WAIT_OBJECT_0 + 1:
				{
					MSG msg;
					if ( ::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
					{
						::TranslateMessage(&msg);
						::DispatchMessage(&msg);
					}
				}
				break;
			case WAIT_TIMEOUT:
				break;
			default:
				goto killThread;
			}
			if (bWait)
			{
				break;
			}
			else
			{
				DWORD dwEnd = GetTickCount();
				if (dwEnd - dwStart > m_nExitTimeOut)
				{
					goto killThread;
				}
			}
		}
		CloseHandle(m_hThread);
		m_hThread = INVALID_HANDLE_VALUE;
		m_eventPause.ResetEvent();
		m_eventExit.ResetEvent();
		return;

killThread:
		TRACE(_T("CIThread::Exit situation is bad!\r\n")); //the normally situation is not run to here
		//ASSERT(0);
		if (INVALID_HANDLE_VALUE != m_hThread)
		{
			TerminateThread(m_hThread, 2);
			CloseHandle(m_hThread);
			m_hThread = INVALID_HANDLE_VALUE;
		}
		m_eventPause.ResetEvent();
		m_eventExit.ResetEvent();
	}
}

BOOL CIThread::BlockWaitEvent()
{
	if (INVALID_HANDLE_VALUE != m_hThread)
	{
		HANDLE szHandle[] = {m_eventExit.m_hObject, m_eventPause.m_hObject};
		DWORD dwWaitNum = sizeof(szHandle)/sizeof(szHandle[0]);
		//
		for ( ; ; )
		{
			DWORD dwRet = MsgWaitForMultipleObjects(dwWaitNum, szHandle, FALSE, 0, QS_ALLINPUT);
			switch( dwRet )
			{
			case WAIT_OBJECT_0:
				return TRUE;
			case WAIT_OBJECT_0 + 1:
				{
					Sleep(20);
					continue;
				}
			case WAIT_OBJECT_0 + 2:	
				{
					MSG msg;
					if ( ::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
					{
						::TranslateMessage(&msg);
						::DispatchMessage(&msg);
					}
				}
				break;
			case WAIT_TIMEOUT:
				return FALSE;
			case WAIT_FAILED:
			default:
				ASSERT(0);
				return TRUE;
			}
		}
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

CIThread::eUnBlock CIThread::UnBlockWaitEvent()
{
	if (INVALID_HANDLE_VALUE != m_hThread)
	{
		HANDLE szHandle[] = {m_eventExit.m_hObject, m_eventPause.m_hObject};
		DWORD dwWaitNum = sizeof(szHandle) / sizeof(szHandle[0]);
		//
		DWORD dwRet = MsgWaitForMultipleObjects(dwWaitNum, szHandle, FALSE, 0, QS_ALLINPUT);
		switch( dwRet )
		{
		case WAIT_OBJECT_0:
			return eUnBlockExit;
		case WAIT_OBJECT_0 + 1:
			return eUnBlockPause;
		case WAIT_OBJECT_0 + 2:	
			{
				MSG msg;
				if ( ::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
				{
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}
			}
			return eUnBlockInput;
		case WAIT_TIMEOUT:
			return eUnBlockContinue;
		case WAIT_FAILED:
		default:
			ASSERT(0);
			return eUnBlockExit;
		}
	}
	return eUnBlockExit;
}

BOOL CIThread::IsRun()
{
	if (INVALID_HANDLE_VALUE != m_hThread)
	{
		return (WAIT_OBJECT_0 != WaitForSingleObject(m_hThread, 0));
	}
	return FALSE;
}

BOOL CIThread::IsExit()
{
	return (WAIT_OBJECT_0 == WaitForSingleObject(m_eventExit.m_hObject, 0));
}

BOOL CIThread::IsPause()
{
	return (WAIT_OBJECT_0 == WaitForSingleObject(m_eventPause.m_hObject, 0));
}

CEvent& CIThread::GetExitEvent()
{
	return m_eventExit;
}

//
BOOL CIThread::BlockWaitTime(DWORD dwWaitTime/*ms*/)
{
	DWORD dwStart = GetTickCount();
	BOOL bWait = FALSE;
	for ( ; ; )
	{
		if ( BlockWaitEvent() )
		{
			return TRUE;
		}
		//
		DWORD dwEnd = GetTickCount();
		if (dwEnd - dwStart > m_nExitTimeOut)
		{
			return FALSE;
		}	
	}
}