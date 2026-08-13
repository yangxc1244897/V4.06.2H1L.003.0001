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

#include "stdafx.h"
//--#include "SerialPortMonitor.h"
#include "CommSocket.h"



// CCommSocket

CCommSocket::CCommSocket()
{
    m_pTheModbusExt = new CModbusExt();

    m_pTheLog = new CCLog;
}

CCommSocket::~CCommSocket()
{
    delete m_pTheModbusExt;

    delete m_pTheLog;
}


// CCommSocket 成员函数


void CCommSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_iRxDataLen = Receive(m_pRxDataBuff, MAX_RX_BUFF_SIZE,0);
    m_pTheLog->WriteLog("TETE MODBUS:>CCommSocket::OnReceive = %d\n", m_iRxDataLen);
    if(m_iRxDataLen>6)
    {
        m_pTheModbusExt->dealRequest(m_pRxDataBuff, m_iRxDataLen, m_pTxDataBuff, m_iTxDataLen);         
        Send(m_pTxDataBuff, m_iTxDataLen, 0);
        m_pTheLog->WriteLog("TETE MODBUS:>CCommSocket::Send = %d\n", m_iTxDataLen);
    }

	AsyncSelect(FD_WRITE| FD_READ);

	CAsyncSocket::OnReceive(nErrorCode);
}

void CCommSocket::OnSend(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	//--Send(m_pDataBuff, m_iDataLen, 0);

	//--AsyncSelect(FD_READ| FD_READ);
	CAsyncSocket::OnSend(nErrorCode);
}

void CCommSocket::OnConnect(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类

	CAsyncSocket::OnConnect(nErrorCode);
}


void CCommSocket::OnClose(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	AfxMessageBox(_T("CCommSocket::OnClose"));

	CAsyncSocket::OnClose(nErrorCode);
}

