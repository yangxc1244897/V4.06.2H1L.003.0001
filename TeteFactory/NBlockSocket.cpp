#include "stdafx.h"

#include <Ws2tcpip.h>
#include "NBlockSocket.h"


void ProcessMessage()
{
    MSG Curmsg;
    while (PeekMessage(&Curmsg, (HWND)NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&Curmsg);
        DispatchMessage(&Curmsg);
    }
}
/********** NSocketServer *******************************************************************/
NSocketServer::NSocketServer()
{
    m_IsLink = false;
    m_IsClose = false;
    mpCallBack = NULL;
	mAddrClientlen = sizeof(mAddrClient);
}
NSocketServer::~NSocketServer()
{
}
void NSocketServer::Register(void *hWin, int id, CBFun pCallBack)
{
    mhWin = hWin;
    mId = id;
    mpCallBack = pCallBack;
}
bool NSocketServer::Init(int port)
{
    m_IsLink = false;
    WSADATA wsd;
    bool rVal = true;
    
    do {
        //初始化Socket
        if (0 != WSAStartup(MAKEWORD(2, 2), &wsd))
        {
            rVal = false;
            break;
        }

        //创建用于监听的Socket
        mSocketServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (INVALID_SOCKET == mSocketServer)
        {
            WSACleanup();
            rVal = false;
            break;
        }

        //设置Socket为非阻塞模式
        int iMode = 1;
        if (SOCKET_ERROR == ioctlsocket(mSocketServer, FIONBIO, (u_long FAR*)&iMode))
        {
            WSACleanup();
            rVal = false;
            break;
        }

        //设置服务器Socket地址
        sockaddr_in addrServ;
        addrServ.sin_family = AF_INET;
        addrServ.sin_port = htons(port);
        addrServ.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
        //绑定Socket Server到本地地址
        if (SOCKET_ERROR == bind(mSocketServer, (const struct sockaddr*)&addrServ, sizeof(sockaddr_in)))
        {
            closesocket(mSocketServer);
            WSACleanup();
            rVal = false;
            break;
        }

        //监听
        if (SOCKET_ERROR == listen(mSocketServer, 1))
        {
            closesocket(mSocketServer);
            WSACleanup();
            rVal = false;
            break;
        }

		AfxBeginThread(&AcceptThdS, this);
    } while (0);

    return rVal;
}
bool NSocketServer::Send(CString msg)
{
    if (!m_IsLink || m_IsClose)
    {
        return false;
    }

    while (!m_IsClose)
    {
        ProcessMessage();
        USES_CONVERSION;
        char *pMsg = T2A(msg);
        int retVal = send(mSocketLink, pMsg, msg.GetLength(), 0);
        if (SOCKET_ERROR == retVal)
        {
            int err = WSAGetLastError();
            if (WSAEWOULDBLOCK == err)
            {
                Sleep(100);
                continue;
            }
            else
            {
                Close();
                return false;
            }
        }
        return true;
    }

    return false;
}
bool NSocketServer::Accept()
{
	if (m_IsClose)
	{
		return false;
	}

	if (m_IsLink)
	{
		Sleep(100);
		return true;
	}

	mSocketLink = accept(mSocketServer, (sockaddr FAR*)&mAddrClient, &mAddrClientlen);
	if (INVALID_SOCKET == mSocketLink)
	{
		if (WSAEWOULDBLOCK == WSAGetLastError())
		{
			Sleep(100);
			return true;
		}
		else
		{
			closesocket(mSocketServer);
			WSACleanup();
			return false;
		}
	}

    ZeroMemory(mRecvBuf, BUF_SIZE);
    if (SOCKET_ERROR == recv(mSocketLink, mRecvBuf, BUF_SIZE, 0))
	{
		if (WSAENOTCONN == WSAGetLastError())
		{
			return true;
		}
		else if (NULL != mpCallBack)
		{
			mpCallBack(mhWin, mId, "C Link OK");
			m_IsLink = true;
			m_IsClose = false;
			AfxBeginThread(&RecvThdS, this);
		}
	}
	else
	{
		mpCallBack(mhWin, mId, "C Link OK");
		m_IsLink = true;
		m_IsClose = false;
        mpCallBack(mhWin, mId, mRecvBuf);
		AfxBeginThread(&RecvThdS, this);
	}
	

	return true;
}
bool NSocketServer::Recv()
{
    if (m_IsClose)
    {
        return false;
    }

    ZeroMemory(mRecvBuf, BUF_SIZE);
    mRecvSize = recv(mSocketLink, mRecvBuf, BUF_SIZE, 0);
    if (SOCKET_ERROR == mRecvSize)
    {
        int err = WSAGetLastError();
        if (WSAEWOULDBLOCK == err)
        {
            Sleep(100);
			return true;
        }
        else// if (WSAETIMEDOUT == err || WSAECONNRESET == err || WSAENETDOWN == err || WSAENOTCONN == err)
        //else if (WSAETIMEDOUT == err || WSAENETDOWN == err || WSAENOTCONN == err)
        {
            Close();
			return false;
		}
    }

    if (0 == mRecvSize)//客户端断开连接
    {
		mpCallBack(mhWin, mId, "Link ERROR");
		closesocket(mSocketLink);
		m_IsLink = false;
		return false;
    }
    else if (NULL != mpCallBack)
    {
        mpCallBack(mhWin, mId, mRecvBuf);
    }
   
    return true;
}
void NSocketServer::Close()
{
    m_IsClose = true;
    if (m_IsLink)
    {
        m_IsLink = false;
        closesocket(mSocketServer);
        closesocket(mSocketLink);
        WSACleanup();
    }
}

bool NSocketServer::IsLink()
{
	return m_IsLink;
}
/********** NSocketClient *******************************************************************/
NSocketClient::NSocketClient()
{
    m_IsLink = false;
    m_IsClose = false;
    mpCallBack = NULL;
}
NSocketClient::~NSocketClient()
{
    if (m_IsLink)
    {
        closesocket(mSocketClient);
        WSACleanup();
    }
} 
void NSocketClient::Register(void *hWin, int id, CBFun pCallBack)
{
    mhWin = hWin;
    mId = id;
    mpCallBack = pCallBack;
} 
bool NSocketClient::Open(const char *strAddr, int port)
{
    if (m_IsLink)
    {
        return true;
    }
    WSADATA wsd;
    SOCKADDR_IN servAddr;
    char buf[BUF_SIZE];
    int retVal;
    if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
    {
        return false;
    }
    
    mSocketClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == mSocketClient)
    {
        WSACleanup();
        return false;
    }
    
    int iMode = 1;
    retVal = ioctlsocket(mSocketClient, FIONBIO, (u_long FAR*)&iMode);
    if (retVal == SOCKET_ERROR)
    {
        WSACleanup();
        return false;
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(port);
    USES_CONVERSION;
    //--char *pIp = T2A(ip);
    inet_pton(AF_INET, strAddr, (PVOID)&(servAddr.sin_addr.S_un.S_addr));// htonl(INADDR_ANY);
	//servAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    int sServerAddlen = sizeof(servAddr);

    int i = 5;
    while (i--)
    {
        retVal = connect(mSocketClient, (LPSOCKADDR)&servAddr, sizeof(servAddr));
        if (SOCKET_ERROR == retVal)
        {
            int err = WSAGetLastError();
            if (err == WSAEWOULDBLOCK || err == WSAEINVAL)
            {
                Sleep(500);
                continue;
            }
            else if (err == WSAEISCONN)
            {
                break;
            }
            else
            {
                closesocket(mSocketClient);
                WSACleanup();
                return false;
            }
        }
        ProcessMessage();
    }

    retVal = recv(mSocketClient, buf, sizeof(buf) + 1, 0);
    if (SOCKET_ERROR == retVal)
    {
        int err = WSAGetLastError();
        if (WSAENOTCONN == err)
        {
            Close();
            return false;
        }
    }

    m_IsLink = true;
    m_IsClose = false;
    mpthreadRecv = AfxBeginThread(&RecvThdC, this);

    return true;
}
bool NSocketClient::Send(CString msg)
{
    if (!m_IsLink || m_IsClose)
    {
        return false;
    }
    int i = 5;
    while (!m_IsClose && i-->0)
    {
        ProcessMessage();
        USES_CONVERSION;
        char *pMsg = T2A(msg);
        int retVal = send(mSocketClient, pMsg, msg.GetLength(), 0);
        if (SOCKET_ERROR == retVal)
        {
            int err = WSAGetLastError();
            if (err == WSAEWOULDBLOCK)
            {
                Sleep(500);
                continue;
            }
            else
            {
                m_IsLink = false;
                closesocket(mSocketClient);
                WSACleanup();
                return false;
            }
        }
        return true;
    }

    return false;
}
bool NSocketClient::Recv()
{
	bool rVal = true;

	do {
	    if (m_IsClose)
	    {
            rVal = false;
            break;
	    }
	
	    ZeroMemory(mRecvBuf, BUF_SIZE);
        int retVal = recv(mSocketClient, mRecvBuf, BUF_SIZE, 0);
        if (SOCKET_ERROR == retVal)
        {
            int err = WSAGetLastError();
            if (err == WSAEWOULDBLOCK)
			{
				Sleep(50);
                break;
            }
            else// if (err == WSAETIMEDOUT || err == WSAENETDOWN || WSAENOTCONN == err)
            {
                mpCallBack(mhWin, mId, "Link ERROR");
                Close();
                rVal = false;
                break;
            }
        }

        if (retVal == 0)
        {
            mpCallBack(mhWin, mId, "Link ERROR");
            Close();
            rVal = false;
            break;
        }
        else if (NULL != mpCallBack)
        {
            mpCallBack(mhWin, mId, mRecvBuf);
			Sleep(50);
            break;
        }
	}while(0);

	return rVal;
}
bool NSocketClient::IsLink()
{
	return m_IsLink;
}
void NSocketClient::Close()
{
    m_IsClose = true;
	Sleep(200);
    if (m_IsLink)
    {
        m_IsLink = false;
        closesocket(mSocketClient);
        WSACleanup();
    }
}
UINT AcceptThdS(LPVOID p)
{
	NSocketServer *socketS = (NSocketServer *)p;
	while (1)
	{
		ProcessMessage();
		if (!socketS->Accept())
		{
			break;
		}
	}

	return 0;
}
UINT RecvThdS(LPVOID p)
{
    NSocketServer *socketS = (NSocketServer *)p;
    while (1)
    {
        ProcessMessage();
        if (!socketS->Recv())
        {
        	break;
        }
    }

    return 0;
}
UINT RecvThdC(LPVOID p)
{
    NSocketClient *socketC = (NSocketClient *)p;
    while (1)
    {
        if (!socketC->Recv())
        {
        	break;
        }
    }

    return 0;
}




