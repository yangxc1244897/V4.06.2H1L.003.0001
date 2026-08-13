// SeriesCom.h: interface for the CSeriesCom class
//////////////////////////////////////////////////////////////////////

#pragma once
#include <stdint.h>


#define USE_MULTI_THREAD  1
#define WM_COMM_RXCHAR              (WM_USER+7) // A character was received and placed in the input buffer. 

typedef struct _STRU_ComParam {
    //--CString strPortNum;         //串口号，“\\\\.\\COM%d”
    int iPortNum;
    uint32_t dwBaudRate;            //波特率
    uint8_t DataBits;               //数据位
    uint8_t StopBits;               //停止位
    uint8_t PorityBit;              //校验位
}stPortConfig_t;

//定义串口接收数据函数类型  CALLBACK
typedef void (* ONSERIESREAD)(void *, uint8_t* , uint32_t );

class CCommDriver  
{
public:
    CCommDriver();
    virtual ~CCommDriver();
    
    uint8_t    m_uRxDataBuf[10240]; //--缓冲区
    uint32_t   m_uRxDataLen;      //--缓冲区长度
    
    HANDLE      m_hDevHandle;   //--设备句柄
    OVERLAPPED  m_ovRead;       //--读异步i/o信息结构体
    OVERLAPPED  m_ovWrite;      //--写异步i/o信息结构体
    
    
    HANDLE  m_hEvent[2];
    HANDLE  m_hEventSendContral;    //--发送控制事件
    int     m_nWaitFrmIndex;        //--等待的帧索引
    
    CWinThread *m_pThComWatch;      //--串口监测线程句柄

    HANDLE  m_hEventExit;           //--退出事件
    HANDLE  m_hEventExitFlag;       //--是否已经退出监视的标志事件
                                
    /*HANDLE m_hRxThread;     //--读线程句柄
    HANDLE m_hTxThread;     //--写线程句柄 
    DWORD m_dwRxThreadID;   //--读线程ID标识
    DWORD m_dwTxThreadID;   //--写线程ID标识
    HANDLE m_hRxExitEvent;  //--读线程退出事件
    HANDLE m_hTxExitEvent;  //--写线程退出事件
    */
    void   *m_pOwnerWnd;  //--所属窗口句柄  

public:
    BOOL OpenDevice(void *pOwner,stPortConfig_t stPortConfig);
    BOOL CloseDevice();
    
    BOOL WriteDataToDevice(uint8_t *lpByte, uint32_t dwBytesToWrite);
    BOOL WriteDataToDevice(CString swdata);    

    virtual void RespMessage(uint8_t *pMsgBuf, uint32_t iSize)=0;//--对接收到的数据进行响应处理
    
    void NotifyReturnMsg(int nReturnFrmIndex);
    
    BOOL StartMonitor();
    void StopMonitor();
    VOID doMonitor();
    
    //--DWORD ReadThreadFunc(LPVOID lparam);
    //--DWORD WriteThreadFunc(LPVOID lparam);
    
private:
    static UINT CommThreadProc(LPVOID pVoid);
    static BOOL WriteDataToPort(HANDLE hComm, const BYTE *buf, DWORD bufLen);    
    void ReadDataFromPort(DWORD dwEvent);
};

