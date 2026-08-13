#include "stdafx.h"
#include "Scan2DSignal.h"
#include "FormWorkControl.h"

CScan2DSignal::CScan2DSignal(CFormWorkControl* pFormWorkControl)
{
	m_pFormWorkControl = pFormWorkControl;
	m_bExitThread = false;
	m_hThread = INVALID_HANDLE_VALUE;
	m_bStartDetect = false;
}

CScan2DSignal::~CScan2DSignal()
{
	if (INVALID_HANDLE_VALUE != m_hThread)
	{
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
	}
}

void CScan2DSignal::Init()
{
	m_bExitThread = false;
	UINT uThreadID = 0;
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, &s_fncDetectThread, (LPVOID)this, 0, &uThreadID);
}

void CScan2DSignal::StartDetect()
{
	m_bStartDetect = true;
}

void CScan2DSignal::EndDetect()
{
	m_bStartDetect = false;
}

UINT __stdcall CScan2DSignal::s_fncDetectThread(LPVOID lpVoid)
{
	CScan2DSignal* pDlg = (CScan2DSignal*)lpVoid;
	while (true)
	{
		// 退出线程
		if (pDlg->m_bExitThread)
			return 0;

		if (pDlg->m_bStartDetect)
		{
			if (pDlg->m_pFormWorkControl->Scan1SignalEffective(0))
			{
				pDlg->m_bStartDetect = false;
				// 检测到2D信号
				pDlg->m_pFormWorkControl->PostMessage(WM_USER_SCAN_PROCESS);	
			}
		}

		Sleep(10);
	}
	return 0;
}
