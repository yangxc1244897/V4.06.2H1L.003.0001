#pragma once
#include "stdafx.h"
#include <string>

class CFactoryMesComm
{
public:
	CFactoryMesComm(void);
	~CFactoryMesComm();

public:
	int32_t Connect(const char* IPAddress, unsigned short portNum);
	int32_t DisConnect();

	int32_t SendData(const char *sTxBuff, int32_t nSize);//--发送指令数据	
	int32_t RecvData(char *sTxBuff, int32_t &nSize, int nTimeOut = 500); //--接收数据

	bool SendCmd(const char * cmd, char * sFind, char * sRet, int nsize, int nTimeOut = 30000);

private:
	SOCKET m_SockClient;
	char m_ip[32];
	unsigned short m_nPort;
};




