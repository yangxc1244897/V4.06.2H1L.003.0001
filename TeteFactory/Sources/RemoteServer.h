#pragma once
#include "stdafx.h"
#include <stdint.h>
#include "tlPlatform.h"

#define MAX_DATA_SIZE   1024

enum {
    emMT_UNKONWN=0,
    emMT_LOT,  /* 载入模板，并更新对象内容 */
    emMT_BIN,  /* Mapping */
    emMT_MARK  /* Start Mark */
};
enum {
    emEC_OK = 0,       //--正常
    emEC_RXCMD,    //--指令解析错误
    emEC_PCCMD     //--指令执行错误        
};  /* 执行动作的返回值 */
class IRemoteProc
{       
public:
    virtual ~IRemoteProc(){};

    int32_t GetMsgType(char *sRecvBuff, int32_t iSize);

    virtual int32_t doLot(char *sRecvBuff, int32_t iSize) = 0;
    virtual int32_t doBin(char *sRecvBuff, int32_t iSize) = 0;
    virtual int32_t doMark(char *sRecvBuff, int32_t iSize) = 0;
};
/***************************************************************/
class CServerSocket : public CAsyncSocket
{
public:
    CServerSocket(IRemoteProc *pTheProc);
    virtual ~CServerSocket();
    virtual void OnAccept(int nErrorCode);
    virtual void OnClose(int nErrorCode);
private:
    IRemoteProc *m_pTheProc;
};
// CRemoteServer 命令目标

class CRemoteSocket : public CAsyncSocket
{
public:
    CRemoteSocket(IRemoteProc *pTheProc);
    virtual ~CRemoteSocket();
    virtual void OnReceive(int nErrorCode);
    virtual void OnSend(int nErrorCode);
private:
    IRemoteProc *m_pTheProc;
};








