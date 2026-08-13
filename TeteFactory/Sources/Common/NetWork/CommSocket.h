/**
  ******************************************************************************
  * @文件     
  * @作者  
  * @版本 V1.00.00
  * @日期 2017-04-11
  * @简述   
  * @email:jchq0101net@sina.com
  ******************************************************************************
  * @说明
  *
  *
  * 
  ******************************************************************************
  */
#ifndef H_COMM_SOCKET
#define H_COMM_SOCKET


#include "modbusExt.h"
#include "fMarkLog.h"
// CCommSocket 命令目标

#define MAX_RX_BUFF_SIZE    512
#define MAX_TX_BUFF_SIZE    512

class CCommSocket : public CAsyncSocket
{
public:
	uint8_t m_pRxDataBuff[MAX_RX_BUFF_SIZE];
	int m_iRxDataLen;
	uint8_t m_pTxDataBuff[MAX_TX_BUFF_SIZE];
	int m_iTxDataLen;
    CModbusExt  *m_pTheModbusExt;
public:
	CCommSocket();
	virtual ~CCommSocket();
	virtual void OnReceive(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnSend(int nErrorCode);
private:
    CCLog *m_pTheLog;
};

#endif

