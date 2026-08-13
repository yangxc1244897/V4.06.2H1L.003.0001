#pragma once
#include "stdafx.h"
#include <string>
#include "Sources\MarkPlatform.h"

enum {
	emRMT_UNKONWN = 0,
	emRMT_BIN,       /* Mapping */
	emRMT_MARK,      /* Start Mark */
	emRMT_AUTOLOT,   /* 自动接收印章图档名称 */
	emRMT_CHECKMARK, /* 检查标记状态 */
	emRMT_POSMATRIX, /* 更改阵列位置 */
	emRMT_2DDATA, /*! 2DMAP */
	emRMT_ENDLOT, /*! 结批 */
	emRMT_SETVISION, /*! 切换CCD方案 */
	emRMT_SETCHARCHECK, /*! 切换标后检测方案 */
};

enum {
	emREC_OK = 0,       //--正常
	emREC_RXCMD,    //--指令解析错误
	emREC_PCCMD     //--指令执行错误        
};  /* 执行动作的返回值 */




class CFormWorkControl;   // 前置声明
class CFactoryServer; // 前置声明
/**
* @brief Server与客户端通讯类
*/
class CFactoryRemote : public CAsyncSocket
{
public:
	CFactoryRemote(CFormWorkControl * pMainDlg, CFactoryServer * pServer);
	virtual ~CFactoryRemote();
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnClose(int nErrorCode);

private:
	CFormWorkControl * m_pMainDlg;   // 窗口
	CFactoryServer * m_pServer;   // 服务器对象指针
	std::string m_sRecvBuff;
	bool m_bBigData = false;
};


/**
* @brief Server监听类
*/
class CFactoryServer : public CAsyncSocket
{
public:
	CFactoryServer(CFormWorkControl * pMainDlg);
    virtual ~CFactoryServer();

	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);

	bool SendRemote(const char * sendbuff, int sendlen);

	void OnClientClose(CFactoryRemote * pClient);
private:
	CFormWorkControl * m_pMainDlg;   // 窗口
	CFactoryRemote * m_pRemote;  // 指向一个连接的socket对象
};






