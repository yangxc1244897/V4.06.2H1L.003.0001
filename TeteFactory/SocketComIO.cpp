#include "stdafx.h"
#include "SocketComIO.h"
#include <winsock2.h>
#include <Ws2tcpip.h>

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 1024
#define RECV_TIMEOUT_BASE 100

CSocketComIO::CSocketComIO()
{
	m_SockClient = INVALID_SOCKET;
	m_iRevTimeO = RECV_TIMEOUT_BASE;   // 至少是RECV_TIMEOUT_BASE + 1

	//加载套接字库
	WORD wVersionRequested;
	WSADATA wsaData;
	int err = 0;

	wVersionRequested = MAKEWORD(1, 1);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		return;
	}
	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return;
	}
}

CSocketComIO::~CSocketComIO()
{
	Close();
	WSACleanup();
}

int CSocketComIO::Connect(const TString& IPAddress, unsigned short portNum)
{
	Close();

	//创建套接字
	m_SockClient = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == m_SockClient) {
		TCHAR errbuf[1024] = { 0 };
		swprintf(errbuf, _T("socket create failed：%d."), WSAGetLastError());
		PrintErrorMsg(errbuf);
		return -1;
	}
	SOCKADDR_IN addrSrv;
	InetPton(AF_INET, IPAddress.c_str(), &addrSrv.sin_addr);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(portNum);
	//向服务器发出连接请求
	int err = connect(m_SockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	if (err == SOCKET_ERROR)
	{
		TCHAR errbuf[1024] = { 0 };
		swprintf(errbuf, _T("socket connect failed：%d."), WSAGetLastError());
		PrintErrorMsg(errbuf);
		return -1;
	}

	setsockopt(m_SockClient, SOL_SOCKET, SO_RCVTIMEO, (char *)&m_iRevTimeO, sizeof(int));

	m_ip = IPAddress;
	m_nPort = portNum;
	return 0;
}

int CSocketComIO::Close()
{
	if (INVALID_SOCKET != m_SockClient)
	{
		closesocket(m_SockClient);
		m_SockClient = INVALID_SOCKET;
	}
	return 0;
}

int CSocketComIO::SendData(const char* sendbuf, int sendbuflen)
{
	if (INVALID_SOCKET == m_SockClient) {
		PrintErrorMsg(_T("socket未连接!"));
		return -1;
	}
	int iResult = 0;
	iResult = send(m_SockClient, sendbuf, sendbuflen, 0);
	if (SOCKET_ERROR == iResult) {
		TCHAR errbuf[1024] = { 0 };
		swprintf(errbuf, _T("send failed：%d."), WSAGetLastError());
		PrintErrorMsg(errbuf);
		return -1;
	}
	
	return 0;
}

int CSocketComIO::RecvData(char* recvbuf, int recvbuflen, int nTimeOut)
{
	if (INVALID_SOCKET == m_SockClient) {
		PrintErrorMsg(_T("socket未连接!"));
		return -1;
	}

	if (nTimeOut < m_iRevTimeO) {
		nTimeOut = m_iRevTimeO;
	}
	int iTimes = nTimeOut / m_iRevTimeO;

	int iResult = 0;
	int iTotalLen = 0;
	char buff[12] = { 0 };
	int bufflen = 12;
	while(iTimes > 0){
		memset(buff, 0, 12);
		iResult = recv(m_SockClient, buff, bufflen, 0);
		if (iResult > 0) {
			memcpy(recvbuf, buff, iResult);
			return iResult;
		}
		else if (0 == iResult) {
			PrintErrorMsg(_T("Socket连接已经关闭！"));
			return -1;
		}
		else {
			int iErrorCode = WSAGetLastError();
			if (WSAETIMEDOUT == iErrorCode) {
				iTimes--;
			}
			else {
				TCHAR errbuf[1024] = { 0 };
				swprintf(errbuf, _T("recv failed：%d."), WSAGetLastError());
				PrintErrorMsg(errbuf);
				return -1;
			}
		}
		Sleep(m_iRevTimeO);
	} 

	return 0;
}

bool CSocketComIO::SendCmd(const char* sendbuf, int sendbuflen
	, char* recvbuf, int recvbuflen
	, const char* findbuf
	, int nTimeOut)
{
	if (INVALID_SOCKET == m_SockClient) {
		PrintErrorMsg(_T("socket未连接!"));
		return -1;
	}

	if (nTimeOut < (m_iRevTimeO + RECV_TIMEOUT_BASE)) {
		nTimeOut = m_iRevTimeO + RECV_TIMEOUT_BASE;
	}
	int iTimes = nTimeOut / ((m_iRevTimeO + RECV_TIMEOUT_BASE) * 2);

	int iResult;
	iResult = SendData(sendbuf, sendbuflen);
	if (0 != iResult) {
		return false;
	}
	
	int iTotalLen = 0;
	char buff[DEFAULT_BUFLEN];
	int bufflen = DEFAULT_BUFLEN;
	for (int i = 0; i < iTimes; i++)
	{
		memset(buff, 0, DEFAULT_BUFLEN);
		Sleep(m_iRevTimeO + RECV_TIMEOUT_BASE);
		iResult = recv(m_SockClient, buff, bufflen, 0);  // 没有内容100ms返回
		if (0 < iResult) {
			buff[iResult] = '\0';
			iTotalLen += iResult;
			if (iTotalLen >= recvbuflen) {
				PrintErrorMsg(_T("输出缓冲区溢出1！"));
				return -1;
			}
			strcat_s(recvbuf, recvbuflen, buff);

			if (nullptr != strstr(recvbuf, findbuf)){
				return true;
			}
		}
		else if (0 == iResult) {
			PrintErrorMsg(_T("Socket连接已经关闭！"));
			return false;
		}
		else {
			int iErrorCode = WSAGetLastError();
			if (WSAETIMEDOUT == iErrorCode) {
				continue;
			}
			else {
				TCHAR errbuf[1024] = { 0 };
				swprintf(errbuf, _T("recv failed：%d."), WSAGetLastError());
				PrintErrorMsg(errbuf);
				return false;
			}
		}
	}
	PrintErrorMsg(_T("未找到关键字！"));
	return false;
}

void CSocketComIO::PrintErrorMsg(const TCHAR* sErr)
{
	m_sErr = sErr;
}