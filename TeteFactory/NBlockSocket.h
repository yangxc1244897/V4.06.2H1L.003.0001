#pragma once

const int BUF_SIZE = 1024;

typedef void(*CBFun)(void *hWin,int id, char *);
UINT AcceptThdS(LPVOID p);
UINT RecvThdS(LPVOID p);
UINT RecvThdC(LPVOID p);

class NSocketServer
{
public:
    NSocketServer();
    ~NSocketServer();

    bool Init(int port);
    void Register(void *hWin, int id, CBFun pCallBack);
    bool Send(CString msg);
	void Close();

	bool Accept();
	bool Recv();

	bool IsLink();
private:
    CWinThread* mpThreadRecv;
    SOCKET mSocketServer;
    SOCKET mSocketLink;
	sockaddr_in mAddrClient;
	int mAddrClientlen;
	
	bool m_IsLink;
    bool m_IsClose;
	char mRecvBuf[BUF_SIZE];
	int mRecvSize;
	
    void *mhWin;
    int mId;
    CBFun mpCallBack;	
};

class NSocketClient
{
public:
    NSocketClient();
    ~NSocketClient();

    void Register(void *hWin, int id, CBFun pCallBack);
    bool Open(const char *strAddr, int port);    
	void Close();
    bool IsLink();

    bool Send(CString msg);
    bool Recv();
private:
    CWinThread* mpthreadRecv;
    SOCKET mSocketClient;
	
	bool m_IsLink;
	bool m_IsClose;
	char mRecvBuf[BUF_SIZE];

	void *mhWin;
	int mId;
	CBFun mpCallBack;	
};

