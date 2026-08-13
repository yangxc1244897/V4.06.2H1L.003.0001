/**
  ******************************************************************************
  * @文件     
  * @作者  
  * @版本 V1.00.00
  * @日期 2017-04-11
  * @简述   
  ******************************************************************************
  * @说明
  *
  *
  * 
  ******************************************************************************
  */

#include "stdafx.h"
#include "ServerSocket.h"


// CServerSocket

CServerSocket::CServerSocket()
{
	m_pTheCommSocket = NULL;
    m_pTheLog = new CCLog;
}

CServerSocket::~CServerSocket()
{
	if (NULL != m_pTheCommSocket)
	{
		delete m_pTheCommSocket;
		m_pTheCommSocket = NULL;
        delete m_pTheLog;
	}
}


// CServerSocket 成员函数


void CServerSocket::OnAccept(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	CCommSocket *pTheCommSocket = new CCommSocket();
	if (Accept(*pTheCommSocket))
	{
		pTheCommSocket->AsyncSelect(FD_READ| FD_WRITE);
		m_pTheCommSocket = pTheCommSocket;
        m_pTheLog->WriteLog("TETE MODBUS:>CServerSocket::OnAccept\n");
	}
	else
	{
		delete m_pTheCommSocket;
	}	

	CAsyncSocket::OnAccept(nErrorCode);
}


void CServerSocket::OnClose(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	AfxMessageBox(_T("CServerSocket::OnClose"));

	CAsyncSocket::OnClose(nErrorCode);
}


void CServerSocket::OnConnect(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	AfxMessageBox(_T("CServerSocket::OnConnect"));

	CAsyncSocket::OnConnect(nErrorCode);
}


void CServerSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	AfxMessageBox(_T("CServerSocket::OnReceive"));

	CAsyncSocket::OnReceive(nErrorCode);
}


void CServerSocket::OnSend(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	AfxMessageBox(_T("CServerSocket::OnSend"));

	CAsyncSocket::OnSend(nErrorCode);
}


int CServerSocket::Receive(void* lpBuf, int nBufLen, int nFlags)
{
	// TODO: 在此添加专用代码和/或调用基类
	AfxMessageBox(_T("CServerSocket::Receive"));

	return CAsyncSocket::Receive(lpBuf, nBufLen, nFlags);
}


int CServerSocket::Send(const void* lpBuf, int nBufLen, int nFlags)
{
	// TODO: 在此添加专用代码和/或调用基类
	AfxMessageBox(_T("CServerSocket::Send"));

	return CAsyncSocket::Send(lpBuf, nBufLen, nFlags);
}
