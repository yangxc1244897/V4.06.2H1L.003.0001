#include "stdafx.h"
#include "MyNSocketServer.h"
#include "FormWorkControl.h"
#include <vector>


//////////////////////////////////////////////////////////////
/// CFactoryRemote
CVisionCheckClient::CVisionCheckClient(CFormWorkControl * pMainDlg, CVisionCheckServer * pServer)
{
	m_pMainDlg = pMainDlg;
	m_pServer = pServer;
}

CVisionCheckClient::~CVisionCheckClient()
{
}

void CVisionCheckClient::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类  
	const int iRecvLen = 1024 * 10;
	char szBuffer[iRecvLen] = { 0 };
	int nLength = Receive(szBuffer, iRecvLen - 1, 0);
	
	switch (nLength)
	{
	case 0:
		Close();
		m_pMainDlg->PrintMessage(emMsgType_Error, _T("socket通讯异常！"));
		break;
	case SOCKET_ERROR: {
		int errid = GetLastError();
		if (WSAEWOULDBLOCK != errid) {
			m_pMainDlg->PrintMessage(emMsgType_Error, _T("socket通讯异常"));
			Close();
		}
		break;
	}

	default:
		szBuffer[nLength] = '\0';

		m_sRecvBuff = szBuffer;
	}


	// 切换制程，成功指令OK或NG
	CString sRecv = CString(m_sRecvBuff.c_str());
	if ((-1 != sRecv.Find(_T("OK"))) || (-1 != sRecv.Find(_T("NG"))))
	{
		m_pMainDlg->doReciveSetCharCheckResult((char*)m_sRecvBuff.c_str(), m_sRecvBuff.size());
	}

	
	CAsyncSocket::OnReceive(nErrorCode);
}

void CVisionCheckClient::OnSend(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类

	CAsyncSocket::OnSend(nErrorCode);
}


void CVisionCheckClient::OnClose(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (NULL == m_pMainDlg)
		return;

	if (m_hSocket != INVALID_SOCKET)
	{
		Close();
	}
	m_pServer->OnClientClose(this);

	CAsyncSocket::OnClose(nErrorCode);
}


//////////////////////////////////////////////////////////////
/// CCVisionCheckServer
CVisionCheckServer::CVisionCheckServer(CFormWorkControl * pMainDlg)
{
	m_pMainDlg = pMainDlg;
	m_pRemote = NULL;
}
CVisionCheckServer::~CVisionCheckServer()
{
	if (NULL != m_pRemote)
	{
		m_pRemote->Close();
		delete m_pRemote;
		m_pRemote = NULL;
	}
}
void CVisionCheckServer::OnAccept(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (NULL != m_pRemote)
	{
		m_pRemote->Close();
		delete m_pRemote;
		m_pRemote = NULL;
	}

	SOCKADDR_IN client;
	int addrlen = sizeof(client);

	m_pRemote = new CVisionCheckClient(m_pMainDlg, this);
	if (Accept(*m_pRemote, (SOCKADDR*)&client, &addrlen))
	{
		m_pRemote->AsyncSelect(FD_READ | FD_CLOSE);  // //触发通信socket的Read

		if (NULL != m_pMainDlg)
		{
			char ip[20];
			inet_ntop(AF_INET, (void*)&client.sin_addr, ip, 20);

			CString s;
			s.Format(_T("视觉检测[IP=%s,Port=%d]已连接！"), CString(ip), client.sin_port);
			m_pMainDlg->PrintMessage(emMsgType_Success, s);
			m_pMainDlg->m_bVisionCheck = TRUE;
			m_pMainDlg->UpdateIOStatus(CFormWorkControl::UISIGNAL_NET);
		}
	}
	else
	{
		delete m_pRemote;
	}


	CAsyncSocket::OnAccept(nErrorCode);
}

bool CVisionCheckServer::SendRemote(const char * sendbuff, int sendlen)
{
	if (NULL == sendbuff)
		return false;

	if (NULL == m_pRemote && NULL != m_pMainDlg)
	{
		m_pMainDlg->PrintMessage(emMsgType_Error, _T("视觉检测端已经断开！"));
		return false;
	}
	int nret = m_pRemote->Send(sendbuff, sendlen, 0);
	if (-1 == nret)
	{
		m_pMainDlg->PrintMessage(emMsgType_Error, _T("视觉检测端已经断开！"));
		return false;
	}

	return true;
}

void CVisionCheckServer::OnClose(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_pMainDlg->PrintMessage(emMsgType_Error, _T("视觉检测服务器关闭"));
	if (NULL != m_pRemote &&  INVALID_SOCKET != m_pRemote->m_hSocket)
	{
		m_pRemote->Close();
		delete m_pRemote;
		m_pRemote = NULL;

		m_pMainDlg->m_bVisionCheck = FALSE;
		m_pMainDlg->UpdateIOStatus(CFormWorkControl::UISIGNAL_NET);
	}

	CAsyncSocket::OnClose(nErrorCode);
}

void CVisionCheckServer::OnClientClose(CVisionCheckClient * pClient)
{
	if (m_pRemote == pClient)
	{
		if (INVALID_SOCKET != m_pRemote->m_hSocket)
			m_pRemote->Close();
		delete m_pRemote;
		m_pRemote = NULL;
		m_pMainDlg->PrintMessage(emMsgType_Error, _T("视觉检测端断开连接！"));
		m_pMainDlg->m_bVisionCheck = FALSE;
		m_pMainDlg->UpdateIOStatus(CFormWorkControl::UISIGNAL_NET);
	}
}