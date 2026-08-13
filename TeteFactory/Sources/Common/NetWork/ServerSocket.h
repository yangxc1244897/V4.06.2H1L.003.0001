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
#ifndef H_SERVER_SOCKET
#define H_SERVER_SOCKET


#include "fMarkLog.h"
#include "CommSocket.h"
// CServerSocket 命令目标

class CServerSocket : public CAsyncSocket
{
public:
	CCommSocket *m_pTheCommSocket;
public:
	CServerSocket();
	virtual ~CServerSocket();
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual int Receive(void* lpBuf, int nBufLen, int nFlags = 0);
	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);
private:
    CCLog *m_pTheLog;
};
#endif


