#pragma once
#include <string>

class CVisionCheckServer; // 前置声明
class CFormWorkControl;
/**
* @brief Server与客户端通讯类
*/
class CVisionCheckClient : public CAsyncSocket
{
public:
	CVisionCheckClient(CFormWorkControl * pMainDlg, CVisionCheckServer * pServer);
	virtual ~CVisionCheckClient();
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnClose(int nErrorCode);

private:
	CFormWorkControl * m_pMainDlg;  
	CVisionCheckServer * m_pServer;   // 服务器对象指针
	std::string m_sRecvBuff;
};


/**
* @brief Server监听类
*/
class CVisionCheckServer : public CAsyncSocket
{
public:
	CVisionCheckServer(CFormWorkControl * pMainDlg);
	virtual ~CVisionCheckServer();

	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);

	bool SendRemote(const char * sendbuff, int sendlen);

	void OnClientClose(CVisionCheckClient * pClient);
private:
	CFormWorkControl * m_pMainDlg;
	CVisionCheckClient * m_pRemote;  // 指向一个连接的socket对象
};

