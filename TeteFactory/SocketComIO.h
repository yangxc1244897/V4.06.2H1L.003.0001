#pragma once
#include "tlString.h"

class CSocketComIO
{
public:
	CSocketComIO(void);
	~CSocketComIO();

public:
	int Connect(const TString& IPAddress, unsigned short portNum);
	int Close();
	int SendData(const char* sendbuf, int sendbuflen);
	int RecvData(char* recvbuf, int recvbuflen, int nTimeOut = 500);
	bool SendCmd(const char* sendbuf, int sendbuflen
		,char* recvbuf, int recvbuflen
		,const char* findbuf
		,int nTimeOut = 5000);

	void PrintErrorMsg(const TCHAR* sErr);
	const TCHAR* GetLastError() { return m_sErr.c_str();  }
private:
	SOCKET m_SockClient;
	TString m_ip;
	unsigned short m_nPort;
	TString m_sErr;
	int m_iRevTimeO;
};




