#pragma once
#include<vector>
const int BUF_SIZE = 1024;

//typedef void(*CBFun)(void *hWin,int id, char *);
typedef void(*CBFun)(void *hWin, int id, const char* data, int length);
UINT AcceptThdSFins(LPVOID p);
UINT RecvThdSFins(LPVOID p);
UINT RecvThdCFins(LPVOID p);

class NSocketServerFins
{
public:
    NSocketServerFins();
    ~NSocketServerFins();

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

class NSocketClientFins
{
public:
    NSocketClientFins();
    ~NSocketClientFins();

    void Register(void *hWin, int id, CBFun pCallBack);
    bool Open(const char *strAddr, int port);    
	void Close();
    bool IsLink();

    bool Send(CString msg);
	bool Send(const char* pData, int nLength);
	
    bool Recv();
	std::vector<char> mRecvBuffer;  // 累积接收的数据
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

