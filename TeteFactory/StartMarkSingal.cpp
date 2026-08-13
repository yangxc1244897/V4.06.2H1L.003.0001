#include "stdafx.h"
#include "StartMarkSingal.h"
#include "FormWorkControl.h"

CStartMarkSingal::CStartMarkSingal(CFormWorkControl* pFormWorkControl)
{
	m_pFormWorkControl = pFormWorkControl;
	m_bExitThread = false;
	m_hThread = INVALID_HANDLE_VALUE;
	m_bStartMonitor = false;
}


CStartMarkSingal::~CStartMarkSingal()
{
	if (INVALID_HANDLE_VALUE != m_hThread) {
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
	}
}

void CStartMarkSingal::Init()
{
	m_bStartMonitor = false;
	UINT uThreadID = 0;
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, &FuncMonitorThread, (LPVOID)this, 0, &uThreadID);
}

void CStartMarkSingal::StartMonitor()
{
	m_bStartMonitor = true;
}

void CStartMarkSingal::StopMonitor()
{
	m_bStartMonitor = false;
}

UINT __stdcall CStartMarkSingal::FuncMonitorThread(LPVOID lpVoid)
{
	CStartMarkSingal *pWnd = (CStartMarkSingal*)lpVoid;
	while (true)
	{
		if (pWnd->m_bExitThread == true) {  //判断是否退出
			return 0;
		}
		if (pWnd->m_bStartMonitor == true) {  //启用监视才检测信号
			uint32_t nBit = 0x00;
			pWnd->m_pFormWorkControl->m_pMarkDriver->ReadExtPortPin(pWnd->m_pFormWorkControl->m_pConfig->m_nStartPort + 16, nBit);
			if (nBit == IO_IN_VALID) {
				pWnd->m_bStartMonitor = false;
				pWnd->m_pFormWorkControl->PostMessage(WM_USER_START_PROCESS, NULL, nBit);
			}
		}

		Sleep(10);
	}
}
