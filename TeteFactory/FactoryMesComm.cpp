#include "stdafx.h"
#include "FactoryMesComm.h"
#include "TeteFactoryDlg.h"
#include "FactoryConfig.h"
#include <vector>

CFactoryMesComm::CFactoryMesComm()
{
	m_SockClient = NULL;

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

CFactoryMesComm::~CFactoryMesComm()
{
	DisConnect();
	WSACleanup();
}

int32_t CFactoryMesComm::Connect(const char* IPAddress, unsigned short portNum)
{
	//创建套接字
	m_SockClient = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN addrSrv;
	inet_pton(AF_INET, IPAddress, &addrSrv.sin_addr);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(portNum);
	//向服务器发出连接请求
	int err = connect(m_SockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	if (err != 0)
	{
		m_SockClient = NULL;
		return -1;
	}
	int nTimeOut = 100;
	setsockopt(m_SockClient, SOL_SOCKET, SO_RCVTIMEO, (char *)&nTimeOut, sizeof(int));

	memset(m_ip, 0, 32);
	strcpy(m_ip, IPAddress);
	m_nPort = portNum;
	return 0;
}

int32_t CFactoryMesComm::DisConnect()
{
	if (NULL != m_SockClient)
	{
		closesocket(m_SockClient);
	}
	return 0;
}

int32_t CFactoryMesComm::SendData(const char *sTxBuff, int32_t nSize)
{
	if (NULL == m_SockClient)
		return -1;
	int nSend = 0;
	nSend = send(m_SockClient, sTxBuff, nSize, 0);
	if (SOCKET_ERROR == nSend)
		return -1;
	if (nSend != nSize)
	{
		return -2;
	}
	return 0;
}
int32_t CFactoryMesComm::RecvData(char *sTxBuff, int32_t &nSize, int nTimeOut)
{
	if (NULL == m_SockClient)
		return -1;

	nSize = recv(m_SockClient, sTxBuff, nSize, 0);
	if (nSize == SOCKET_ERROR)
	{
		return -1;
	}
	sTxBuff[nSize] = '\0';

	return 0;
}

bool CFactoryMesComm::SendCmd(const char * cmd, char * sFind, char * sRet, int nsize, int nTimeOut)
{
	if (NULL == m_SockClient)
		return false;

	if (nullptr == cmd || nullptr == sRet)
		return false;

	memset(sRet, 0, nsize);

	if (nTimeOut < 1000)
		nTimeOut = 1000;

	// 重新连接服务器
	if (0 != Connect(CStringA(CFactoryConfig::Instance()->m_sMesIP).GetString(), CFactoryConfig::Instance()->m_nPort))
		return false;
	
	int nret = SendData(cmd, strlen(cmd));
	
	char recvbuff[1024];
	int nrecvsize = 1024;
	for (int i = 0; i < nTimeOut / 1000; i++)
	{
		Sleep(1000);
		nrecvsize = 1023;
		memset(recvbuff, 0, 1024);
		if (0 != RecvData(recvbuff, nrecvsize))
			continue;

		recvbuff[nrecvsize] = '\0';

		strcat_s(sRet, nsize, recvbuff);
	    
		if (nullptr == sFind || "" == sFind)
			return true;
		
		if (nullptr != strstr(sRet, sFind))
		{
			return true;
		}
		
	}

	return false;
}