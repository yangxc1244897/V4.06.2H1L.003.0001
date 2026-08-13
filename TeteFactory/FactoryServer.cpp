#include "stdafx.h"
#include "FactoryServer.h"
#include "FormWorkControl.h"
#include <vector>

//////////////////////////////////////////////////////////////
/// IFactoryRemoteProc
typedef struct _MsgHead {
	int32_t iType;
	char sHead[256];
}MsgHead_t;
const int g_nMsgCount = 9;
const MsgHead_t MsgHeads[g_nMsgCount] = {
	{ emRMT_BIN,"BIN" },
	{ emRMT_MARK,"START" },
	{ emRMT_AUTOLOT,"LOT2" },
	{ emRMT_CHECKMARK,"STATUS" },
	{ emRMT_POSMATRIX,"@StartMark" },  //取8位
	{ emRMT_2DDATA,"DEVICEID" },
	{ emRMT_ENDLOT,"FINISH" },
	{ emRMT_SETVISION, "@SetVisionProcess" },
	{ emRMT_SETCHARCHECK, "@SetCharCheckProcess"},
};
static int32_t GetMsgType(char *sRecvBuff, int32_t iSize)
{
	int32_t iType = emRMT_UNKONWN;
	for (int i = 0; i < g_nMsgCount; i++)
	{
		char *pMsgBuff = NULL;
		pMsgBuff = strstr(sRecvBuff, MsgHeads[i].sHead);
		if (NULL != pMsgBuff)
		{
			iType = MsgHeads[i].iType;
			break;
		}
	}

	return iType;
}

//////////////////////////////////////////////////////////////
/// CFactoryRemote
CFactoryRemote::CFactoryRemote(CFormWorkControl * pMainDlg, CFactoryServer * pServer)
{
	m_pMainDlg = pMainDlg;
	m_pServer = pServer;
}

CFactoryRemote::~CFactoryRemote()
{
}

void CFactoryRemote::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类  
	const int iRecvLen = 1024 * 10;
	char szBuffer[iRecvLen] = {0};
	int nLength = Receive(szBuffer, iRecvLen - 1, 0);
	if (!m_bBigData)   // 如果不是大数据模式，直接清空数据
		m_sRecvBuff.clear();

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
	default: {
		szBuffer[nLength] = '\0';
#if 1
		if (nLength == iRecvLen - 1) {   // 当接收数据等于接收最大尺寸时，判断为大尺寸数据
			m_sRecvBuff += szBuffer;
			m_bBigData = true;
			break;
		}
		else {
			m_sRecvBuff += szBuffer;
			m_bBigData = false;
		}
#else
		// 通过回车/r/n判断是不是达到数据的结尾
		if (szBuffer[nLength - 1] == '\r' || szBuffer[nLength - 1] == '\n') {
			m_sRecvBuff += szBuffer;
			m_bBigData = false;  // 到达结尾，后面可以清除记录的数据
		}
		else {
			m_sRecvBuff += szBuffer;
			m_bBigData = true;   // 数据不能被清空，需要继续记录
			break;
		}
#endif
		char *sSendMsg = NULL;
		if (NULL != m_pMainDlg)
		{
			int32_t iError(0);
			int32_t iType = GetMsgType((char*)m_sRecvBuff.c_str(), m_sRecvBuff.size());
			switch (iType)
			{
			case emRMT_BIN:
				iError = m_pMainDlg->doBin((char*)m_sRecvBuff.c_str(), m_sRecvBuff.size());
				if (emREC_OK == iError)
				{
					sSendMsg = "AK";
				}
				else
					sSendMsg = "NK";
				Send(sSendMsg, strlen(sSendMsg));
				m_pMainDlg->PrintMessage(emMsgType_Normal, _T("发送指令：") + CString(sSendMsg));
				break;
			case emRMT_MARK:
				m_pMainDlg->PrintMessage(emMsgType_Normal, _T("接收指令：") + CString(szBuffer));
				sSendMsg = "AK";
				Send(sSendMsg, strlen(sSendMsg));
				m_pMainDlg->PrintMessage(emMsgType_Normal, _T("发送指令：") + CString(sSendMsg));
				m_pMainDlg->doMark((char*)m_sRecvBuff.c_str(), m_sRecvBuff.size());
				break;
			case emRMT_AUTOLOT:
				iError = m_pMainDlg->doAutoLot((char*)m_sRecvBuff.c_str(), m_sRecvBuff.size());
				if (emREC_OK == iError)
				{
					sSendMsg = "AK";
				}
				else
					sSendMsg = "NK";
				Send(sSendMsg, strlen(sSendMsg));
				m_pMainDlg->PrintMessage(emMsgType_Normal, _T("发送指令：") + CString(sSendMsg));
				break;
			case emRMT_CHECKMARK:
				iError = m_pMainDlg->doCheckMarkStatus((char*)m_sRecvBuff.c_str(), m_sRecvBuff.size());
				if (emREC_OK == iError)   // 标记结束
				{
					sSendMsg = "MARKINGEND;1";
				}
				else
					sSendMsg = "MARKINGEND;0";
				Send(sSendMsg, strlen(sSendMsg));
				m_pMainDlg->PrintMessage(emMsgType_Normal, _T("发送指令：") + CString(sSendMsg), 0x02);
				break;
			case emRMT_POSMATRIX:    //坐标
				if (!CFactoryConfig::Instance()->m_bCheckVisualSystem)
				{
					m_pMainDlg->PrintMessage(emMsgType_Error, _T("未启用视觉定位，请前往【系统设置-启用视觉定位】") + CString(sSendMsg));
					return;
				}
				iError = m_pMainDlg->doPosMatrix((char*)m_sRecvBuff.c_str(), m_sRecvBuff.size());
				if (emREC_OK == iError)
				{
					sSendMsg = "@SetVLMPositionOK";   
					Send(sSendMsg, strlen(sSendMsg));
					m_pMainDlg->PrintMessage(emMsgType_Normal, _T("发送指令：") + CString(sSendMsg));
				}
				/*else
					sSendMsg = "NK";
				Send(sSendMsg, strlen(sSendMsg));
				m_pMainDlg->PrintMessage(emMsgType_Normal, _T("发送指令：") + CString(sSendMsg));*/
				break;
			case emRMT_2DDATA:
				iError = m_pMainDlg->do2DData((char*)m_sRecvBuff.c_str(), m_sRecvBuff.size());
				if (emREC_OK == iError)
					sSendMsg = "AK";
				else
					sSendMsg = "NK";
				Send(sSendMsg, strlen(sSendMsg));
				m_pMainDlg->PrintMessage(emMsgType_Normal, _T("发送指令：") + CString(sSendMsg));
				break;
			case emRMT_ENDLOT:
				iError = m_pMainDlg->doAutoLot((char*)m_sRecvBuff.c_str(), m_sRecvBuff.size());
				if (emREC_OK == iError)
					sSendMsg = "AK";
				else
					sSendMsg = "NK";
				Send(sSendMsg, strlen(sSendMsg));
				m_pMainDlg->PrintMessage(emMsgType_Normal, _T("发送指令：") + CString(sSendMsg));
				break;
			case emRMT_SETVISION:
				iError = m_pMainDlg->doReciveSetVisionResult((char*)m_sRecvBuff.c_str(), m_sRecvBuff.size());
				break;
			case emRMT_SETCHARCHECK:
				//iError = m_pMainDlg->doReciveSetCharCheckResult((char*)m_sRecvBuff.c_str(), m_sRecvBuff.size());
				break;
			default:
			{
				// PLC切换制程，成功指令OK或NG
				CString sRecv = CString(m_sRecvBuff.c_str());
				if ((-1 != sRecv.Find(_T("OK"))) || (-1 != sRecv.Find(_T("NG"))))
				{
					iError = m_pMainDlg->doRecivePLCSwitchProcessResult((char*)m_sRecvBuff.c_str(), m_sRecvBuff.size());
					break;
				}
				//m_pMainDlg->PrintMessage(emMsgType_Normal, _T("接收指令：") + sRecv);
				//sSendMsg = "NK";
				//Send(sSendMsg, strlen(sSendMsg));
				//m_pMainDlg->PrintMessage(emMsgType_Normal, _T("发送指令：") + CString(sSendMsg));
				break;
			}

			}
		}
	}
	}
	CAsyncSocket::OnReceive(nErrorCode);
}

void CFactoryRemote::OnSend(int nErrorCode)
{
    // TODO: 在此添加专用代码和/或调用基类

    CAsyncSocket::OnSend(nErrorCode);
}


void CFactoryRemote::OnClose(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (NULL == m_pMainDlg)
		return;

	if (m_hSocket != INVALID_SOCKET)
	{
		Close();
	}
	//m_pMainDlg->PrintMessage(emMsgType_Warn, _T("客户端断开连接"));
	m_pServer->OnClientClose(this);

	CAsyncSocket::OnClose(nErrorCode);
}


//////////////////////////////////////////////////////////////
/// CFactoryServer
CFactoryServer::CFactoryServer(CFormWorkControl * pMainDlg)
{
	m_pMainDlg = pMainDlg;
	m_pRemote = NULL;
}
CFactoryServer::~CFactoryServer()
{
	if (NULL != m_pRemote)
	{
		m_pRemote->Close();
		delete m_pRemote;
		m_pRemote = NULL;
	}
}
void CFactoryServer::OnAccept(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	SOCKADDR_IN client;
	int addrlen = sizeof(client);

	CFactoryRemote * pRemote = new CFactoryRemote(m_pMainDlg, this);
	if (Accept(*pRemote, (SOCKADDR*)&client, &addrlen))
	{
		pRemote->AsyncSelect(FD_READ | FD_CLOSE);  // //触发通信socket的Read
		m_pRemote = pRemote;

		if (NULL != m_pMainDlg)
		{
			char ip[20];
			inet_ntop(AF_INET, (void*)&client.sin_addr, ip, 20);

			CString s;
			s.Format(_T("客户端[IP=%s,Port=%d]已连接！"), CString(ip), client.sin_port);
			m_pMainDlg->PrintMessage(emMsgType_Success, s);
			m_pMainDlg->m_IfConnectCCD = TRUE;
			m_pMainDlg->UpdateIOStatus(CFormWorkControl::UISIGNAL_VISION);  
		}
	}
	else
	{
		delete pRemote;
	}
		

	CAsyncSocket::OnAccept(nErrorCode);
}

bool CFactoryServer::SendRemote(const char * sendbuff, int sendlen)
{
	if (NULL == sendbuff)
		return false;

	if (NULL == m_pRemote && NULL != m_pMainDlg)
	{
		m_pMainDlg->PrintMessage(emMsgType_Error, _T("客户端已经断开！"));
		return false;
	}
	int nret = m_pRemote->Send(sendbuff, sendlen, 0);
	if (-1 == nret)
	{
		m_pMainDlg->PrintMessage(emMsgType_Error, _T("客户端已经断开！"));
		return false;
	}

	return true;
}

void CFactoryServer::OnClose(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_pMainDlg->PrintMessage(emMsgType_Warn, _T("服务器关闭"));
	if (NULL != m_pRemote &&  INVALID_SOCKET != m_pRemote->m_hSocket)
	{
		m_pRemote->Close();
		delete m_pRemote;
		m_pRemote = NULL;
	}

	CAsyncSocket::OnClose(nErrorCode);
}

void CFactoryServer::OnClientClose(CFactoryRemote * pClient)
{
	if (m_pRemote == pClient)
	{
		if (INVALID_SOCKET != m_pRemote->m_hSocket)
			m_pRemote->Close();
		delete m_pRemote;
		m_pRemote = NULL;
		m_pMainDlg->PrintMessage(emMsgType_Error, _T("客户端断开连接！"));
		m_pMainDlg->m_IfConnectCCD = FALSE;
		m_pMainDlg->UpdateIOStatus(CFormWorkControl::UISIGNAL_VISION);
	}
}