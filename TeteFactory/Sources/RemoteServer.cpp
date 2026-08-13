#include "stdafx.h"
#include "RemoteServer.h"
#include "MarkPlatform.h"
#include <vector>
//-----------------------------------------------------------------------------------------------------------
typedef unsigned (WINAPI *PBEGINTHREADEX_THREADFUNC)(LPVOID lpThreadFunc);
typedef unsigned *PBEGINTHREADEX_THREADID;
//---------------------------------------------------------------------------
typedef struct _MsgHead {
    int32_t iType;
    char sHead[256];
}MsgHead_t;
const MsgHead_t MsgHeads[3] = {
    {emMT_LOT,"LOT2"},
    {emMT_BIN,"BIN"},
    {emMT_MARK,"START"}
};
int32_t IRemoteProc::GetMsgType(char *sRecvBuff, int32_t iSize)
{
    int32_t iType = emMT_UNKONWN;
    for (int i = 0; i < 3; i++)
    {
        char *pMsgBuff = NULL;
        pMsgBuff = strstr(sRecvBuff, MsgHeads[i].sHead);
        if (NULL != pMsgBuff)
        {
            iType = MsgHeads[i].iType;
            break;
        }        
    }

    return iType;
}
/*class CRemoteMessage : public IRemoteMessage
{
public:
    CRemoteMessage();
    ~CRemoteMessage();

    virtual int32_t Praser(char *pMsgBuff, int32_t iSize);//--解析消极

    virtual int32_t GetType(void);//--获取消息类型
    virtual int32_t GetDataCount(void);//--获取消息中数据个数
    virtual int32_t GetDataValue(int32_t iIndex,char *pDataBuff, int32_t &iSize);//--获取指定索引的数据内容
private:
    //--std::vector<CStringA> m_vMsgDatas;
    CStringArray m_vMsgDatas;
};
CRemoteMessage::CRemoteMessage()
{}
CRemoteMessage::~CRemoteMessage()
{}
int32_t CRemoteMessage::Praser(char *pRecvBuff, int32_t iSize)
{
    int32_t iError(0);

    char *sMsgBuff = NULL;
    char *sContext = NULL;
    sMsgBuff = strtok_s(pRecvBuff, ";", &sContext);  //-- "LOT2"
    while(NULL != sMsgBuff)
    {            
        int iLen = strlen(sMsgBuff);
     
        sMsgBuff = strtok_s(NULL, ",",&sContext);
    }
    return iError;
}
int32_t CRemoteMessage::GetType(void)
{
    return 0;
}
int32_t CRemoteMessage::GetDataCount(void)
{
    return 0;
}
int32_t CRemoteMessage::GetDataValue(int32_t iIndex, char *pDataBuff, int32_t &iSize)
{
    return 0;
}*/
//---------------------------------------------------------------------------
CServerSocket::CServerSocket(IRemoteProc *pTheProc)
{
    m_pTheProc = pTheProc;
}
CServerSocket::~CServerSocket()
{
}
void CServerSocket::OnAccept(int nErrorCode)
{
    // TODO: 在此添加专用代码和/或调用基类
    CRemoteSocket *pRemoteSocket = new CRemoteSocket(m_pTheProc);
    if (Accept(*pRemoteSocket))
    {
        pRemoteSocket->AsyncSelect(FD_READ);
    }
    else
        delete pRemoteSocket;

    CAsyncSocket::OnAccept(nErrorCode);
}
void CServerSocket::OnClose(int nErrorCode)
{
    // TODO: 在此添加专用代码和/或调用基类

    CAsyncSocket::OnClose(nErrorCode);
}
//---------------------------------------------------------------------------
CRemoteSocket::CRemoteSocket(IRemoteProc *pTheProc)
{
    m_pTheProc = pTheProc;
}
CRemoteSocket::~CRemoteSocket()
{
}
// CRemoteSocket 成员函数
void CRemoteSocket::OnReceive(int nErrorCode)
{
    // TODO: 在此添加专用代码和/或调用基类    
    char szBuffer[MAX_DATA_SIZE];
    int nLength =Receive(szBuffer,sizeof(szBuffer),0); //接收数据  
    if (nLength > (MAX_DATA_SIZE - 2))
    {
        CAsyncSocket::OnReceive(nErrorCode);
        return;
    }
    
    szBuffer[nLength++] = ';';
    szBuffer[nLength] ='\0';

    char *sSendMsg = NULL;
    if (NULL != m_pTheProc)
    {
        int32_t iError(0);
        int32_t iType = m_pTheProc->GetMsgType(szBuffer,nLength);
        switch (iType)
        {
        case emMT_LOT:
            iError = m_pTheProc->doLot(szBuffer,nLength);  
            if (emEC_OK == iError)
            {
                sSendMsg = "AK";
            }
            else
                sSendMsg = "NK";
            Send(sSendMsg, strlen(sSendMsg));
            /*if(emEC_OK==iError)
            m_emWorkStat = emWS_LOT;
            else
            m_emWorkStat = emWS_NULL;*/
            break;
        case emMT_BIN:
            iError = m_pTheProc->doBin(szBuffer,nLength);
            if (emEC_OK == iError)
            {
                sSendMsg = "AK";
            }
            else
                sSendMsg = "NK";
            Send(sSendMsg, strlen(sSendMsg));
            /*if (emWS_NULL != m_emWorkStat)
            {
            iError = doBin(pMsgInfo->sData, pMsgInfo->nSize);
            if(emEC_OK==iError)
            m_emWorkStat = emWS_BIN;
            else
            m_emWorkStat = emWS_LOT;
            }*/
            break;
        case emMT_MARK:
            iError = m_pTheProc->doMark(szBuffer,nLength);
            if (emEC_OK == iError)
            {
                sSendMsg = "MARKEND;1";
            }
            else
                sSendMsg = "NK";
            Send(sSendMsg, strlen(sSendMsg));
            /*if (emWS_BIN == m_emWorkStat)
            iError = doMark(pMsgInfo->sData,pMsgInfo->nSize);*/
            break;
        default:break;
        }
    }

    CAsyncSocket::OnReceive(nErrorCode);
}
void CRemoteSocket::OnSend(int nErrorCode)
{
    // TODO: 在此添加专用代码和/或调用基类

    CAsyncSocket::OnSend(nErrorCode);
}
