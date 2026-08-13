// SeriesCom.cpp: implementation of the CSeriesCom class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommDriver.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommDriver::CCommDriver()
{
    m_hDevHandle = INVALID_HANDLE_VALUE;
    m_nWaitFrmIndex = -1;           //等待的帧索引

    m_ovRead.hEvent=NULL;
    m_ovWrite.hEvent=NULL;

    m_hEventExit=NULL;
    m_hEventSendContral=NULL;

    //--创建对应读写事件
    memset( &m_ovRead, 0, sizeof(OVERLAPPED));
    memset( &m_ovWrite, 0, sizeof(OVERLAPPED));
    m_ovRead.Offset = 0 ;
    m_ovRead.OffsetHigh = 0 ;
    m_ovRead.hEvent =  ::CreateEvent(NULL,TRUE, FALSE,NULL) ; 
    m_ovWrite.Offset    = 0 ;
    m_ovWrite.OffsetHigh = 0 ;
    m_ovWrite.hEvent =    ::CreateEvent(NULL,TRUE,FALSE,NULL); 
    m_hEventExit =        ::CreateEvent(NULL,TRUE,FALSE,NULL);   
    m_hEventExitFlag =    ::CreateEvent(NULL,TRUE,TRUE,NULL);
    m_hEventSendContral = ::CreateEvent(NULL,TRUE,TRUE,NULL);
    //初始化事件数组
    m_hEvent[0]=m_hEventExit;
    m_hEvent[1]=m_ovRead.hEvent;
    //--OnSeriesRead = NULL;
}

CCommDriver::~CCommDriver()
{
    if(m_hEventExitFlag!=NULL)  //退出事件
    {
        ::CloseHandle(m_hEventExitFlag);
        m_hEventExitFlag=NULL;
    }
    if(m_ovRead.hEvent!=NULL)   //读异步i/o信息结构体
    {
        ::CloseHandle(m_ovRead.hEvent);
        m_ovRead.hEvent=NULL;
    }
    if(m_ovWrite.hEvent!=NULL)  //写异步i/o信息结构体
    {
        ::CloseHandle(m_ovWrite.hEvent);
        m_ovWrite.hEvent=NULL;
    }
    if(m_hEventExit!=NULL)  //退出事件
    {
        ::CloseHandle(m_hEventExit);
        m_hEventExit=NULL;
    }
    if(m_hEventSendContral!=NULL)   //发送控制事件
    {
        ::CloseHandle(m_hEventSendContral);
        m_hEventSendContral=NULL;
    }
}
//打开设备
BOOL CCommDriver::OpenDevice(void *pOwner,stPortConfig_t stPortConfig)
{
    if(INVALID_HANDLE_VALUE!=m_hDevHandle)
        CloseDevice();

    CString csErrorMsg;
    
    m_pOwnerWnd = pOwner;

    TCHAR szPort[15];
    wsprintf(szPort, _T("\\\\.\\COM%d"), stPortConfig.iPortNum);
    m_hDevHandle = CreateFile(szPort,GENERIC_READ | GENERIC_WRITE,
                          0,NULL,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,
                          NULL);
    if (INVALID_HANDLE_VALUE==m_hDevHandle)
    {       
        DWORD dwError = GetLastError();
        LPVOID lpMsgBuf;

        //格式化系统消息
        if (!FormatMessage( 
            FORMAT_MESSAGE_ALLOCATE_BUFFER | 
            FORMAT_MESSAGE_FROM_SYSTEM | 
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,dwError,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
            (LPTSTR) &lpMsgBuf,0,NULL ))
        {
            csErrorMsg.Format(_T("Error Code: %d"),dwError);
        }
        else
        {
            if(dwError==2)
                csErrorMsg = _T("请检查串口是否存在！");
            else if(dwError==5)
                csErrorMsg = _T("访问串口冲突，当前串口已占用，请选择其他串口！");
            else
                csErrorMsg = (LPCTSTR)lpMsgBuf;
            // Free the buffer.
            LocalFree( lpMsgBuf );
        }
        return FALSE;
    }
    //串口打开成功，继续
   ::SetCommMask(m_hDevHandle, EV_RXCHAR) ;
    // 设置设备缓冲
    ::SetupComm( m_hDevHandle, 1024*11, 1024) ;
    // 清除缓冲区内容
    ::PurgeComm( m_hDevHandle, PURGE_TXABORT | PURGE_RXABORT |
                                      PURGE_TXCLEAR | PURGE_RXCLEAR );
    COMMTIMEOUTS  CommTimeOuts ;
    ::GetCommTimeouts(m_hDevHandle, &CommTimeOuts);
    CommTimeOuts.ReadIntervalTimeout =MAXDWORD ;
    CommTimeOuts.ReadTotalTimeoutMultiplier = 0 ;
    CommTimeOuts.ReadTotalTimeoutConstant = 0 ;
    CommTimeOuts.WriteTotalTimeoutMultiplier = 50 ;
    CommTimeOuts.WriteTotalTimeoutConstant = 2000 ;
    ::SetCommTimeouts(m_hDevHandle, &CommTimeOuts) ;
    DCB dcb;
    dcb.DCBlength = sizeof(DCB) ;
    GetCommState(m_hDevHandle, &dcb);
    dcb.BaudRate = stPortConfig.dwBaudRate; //波特率为
    dcb.ByteSize = stPortConfig.DataBits;   //数据位数
    dcb.Parity = stPortConfig.PorityBit;        //奇偶校验
    dcb.StopBits = stPortConfig.StopBits;   //停止位
    dcb.fBinary = TRUE;
    dcb.fParity = TRUE;

    /*************硬件流控制rts/cts选项（请求发送/清除发送）****************************/
    dcb.fOutxCtsFlow = FALSE ;              //指定CTS不监测发送流
    dcb.fRtsControl = RTS_CONTROL_DISABLE ;

    /*************软件流控制xon/xof选项****************************/    
    dcb.fOutX = FALSE ;
    dcb.fInX = FALSE    ;
    dcb.XonLim = 100 ;
    dcb.XoffLim = 100 ;
    dcb.XonChar = 0xff;//ASCII_XON;
    dcb.XoffChar = 0xff;//ASCII_XOFF ;
    /*************硬件流控制DTR/DSR选项（数据终端就绪/数据设置就绪）****************************/
    dcb.fOutxDsrFlow = FALSE ;//指定DSR不监测发送流
    dcb.fDtrControl = DTR_CONTROL_DISABLE ;

    dcb.fDsrSensitivity = FALSE ;//接受的字节被忽
    dcb.fTXContinueOnXoff = TRUE ;//接受缓冲区满时发送继续进行
    dcb.fErrorChar = TRUE ;
    dcb.fNull = FALSE ;         //是否舍弃0
    dcb.fAbortOnError = TRUE ;  
    //  dcb.ErrorChar = 'M' ;
    //  dcb.EofChar = ASCII_XOFF ;
    //  dcb.EvtChar = 0x00 ;
    DWORD dwErrorFlags;
    COMSTAT ComStat ;
    ClearCommError(m_hDevHandle, &dwErrorFlags, &ComStat);
    PurgeComm (m_hDevHandle, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT |PURGE_TXABORT);
        
    if(!SetCommState(m_hDevHandle, &dcb))
    {       
        DWORD dwError = GetLastError();
        CloseHandle(m_hDevHandle) ;
        m_hDevHandle = INVALID_HANDLE_VALUE;
        csErrorMsg="串口设置出错!";
        return FALSE;
    }  

    csErrorMsg="打开串口成功！";

    return TRUE;
}

BOOL CCommDriver::CloseDevice()
{
    if(INVALID_HANDLE_VALUE!=m_hDevHandle)
    {
        ::CloseHandle(m_hDevHandle);
        m_hDevHandle = INVALID_HANDLE_VALUE;
    }
    return TRUE;
}

BOOL CCommDriver::StartMonitor()
{
    if(INVALID_HANDLE_VALUE==m_hDevHandle)
    {
        return FALSE;
    }
    m_uRxDataLen = 0;           //接收缓冲区长度
    ResetEvent(m_hEventExit);
    ::SetCommMask(m_hDevHandle, EV_RXCHAR);
    m_pThComWatch = AfxBeginThread(CommThreadProc,(LPVOID)this,THREAD_PRIORITY_HIGHEST);

    if(NULL==m_pThComWatch)
        return FALSE;
    else 
        return TRUE;
}
void CCommDriver::StopMonitor()
{
    //关闭监视线程
    SetEvent(m_hEventExit);
    ::SetCommMask(m_hDevHandle, NULL);

    //等待退出
    WaitForSingleObject(m_hEventExitFlag,INFINITE);
}
VOID CCommDriver::doMonitor()
{
    BOOL bResult;
    DWORD dwEventMask;
    int nEvent;
    BOOL bContinue=TRUE;
    DWORD dwErrorFlags;
    COMSTAT ComStat ;    
    
    ClearCommError(m_hDevHandle, &dwErrorFlags, &ComStat);
    PurgeComm (m_hDevHandle, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT |PURGE_TXABORT);

    m_uRxDataLen = 0;
    while(bContinue) //只要所要监控的串口有线程在运行，就一直处于监视端口行为的无限循环     
    {       
        bResult = WaitCommEvent(m_hDevHandle,&dwEventMask,&m_ovRead); // m_ovRead是OVERLAPPED类型的变量
        if(!bResult)//进行相关的出错处理的具体函数调用过程            
        {   
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //等待足够的数据，特殊处理
                Sleep(5);
                DWORD dwRet=WaitForSingleObject(m_ovRead.hEvent,INFINITE);
                if(dwRet!=WAIT_OBJECT_0)        //出现错误
                {
                    ClearCommError(m_hDevHandle, &dwErrorFlags, &ComStat);
                    continue;
                }
            }
        }
        nEvent= WaitForMultipleObjects(2, m_hEvent, FALSE, 100);
        if(nEvent==WAIT_TIMEOUT)
        {
            ClearCommError(m_hDevHandle, &dwErrorFlags, &ComStat);
            PurgeComm (m_hDevHandle, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT |PURGE_TXABORT);
            continue;
        }
        else if(nEvent==WAIT_FAILED)
        {
            if(INVALID_HANDLE_VALUE==m_hDevHandle)
                nEvent=0;           //监听失败，说明串口已经关闭，退出
            else
            {
                ClearCommError(m_hDevHandle, &dwErrorFlags, &ComStat);
                PurgeComm (m_hDevHandle, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT |PURGE_TXABORT);
                continue;
            }
        }
        switch(nEvent)
        {
        case 0:     //退出
            bContinue=FALSE;
            break;
        case 1:                         //串口进行数据读事件的响应处理过程
            ReadDataFromPort(dwEventMask);
            ResetEvent(m_ovRead.hEvent);        
            break;
        }           
    }

    SetEvent(m_hEventExitFlag);
}
BOOL CCommDriver::WriteDataToDevice(uint8_t *lpByte, uint32_t dwBytesToWrite)//等待帧索引
{
    if(INVALID_HANDLE_VALUE==m_hDevHandle)
    {
        return FALSE;
    }
    //置等待帧索引
    ResetEvent(m_hEventSendContral);

    BOOL        bWriteStat ;
    DWORD       dwBytesWritten ;
    DWORD       dwErrorFlags;
    DWORD       dwError;
    COMSTAT     ComStat;

    bWriteStat = WriteFile(m_hDevHandle, lpByte, dwBytesToWrite,&dwBytesWritten,&m_ovWrite) ;

    if (bWriteStat)
    {
        if(dwBytesToWrite==dwBytesWritten)
            return  TRUE;
        else
            return FALSE;
    }//if (bWriteStat)
    dwError = GetLastError();
    if(dwError != ERROR_IO_PENDING)
    {   
        ClearCommError(m_hDevHandle, &dwErrorFlags, &ComStat ) ;
        return FALSE;
    }
    return TRUE;
}
BOOL CCommDriver::WriteDataToDevice(CString swdata)//等待帧索引
{
    if(INVALID_HANDLE_VALUE==m_hDevHandle)
    {
        return FALSE;
    }
    //置等待帧索引
    //--m_nWaitFrmIndex=nWaitFrmIndex;
    ResetEvent(m_hEventSendContral);
    
    BOOL        bWriteStat ;
    DWORD       dwBytesWritten ;
    DWORD       dwErrorFlags;
    DWORD       dwError;
    COMSTAT     ComStat;
    
    bWriteStat = WriteFile(m_hDevHandle, swdata.GetBuffer(swdata.GetLength()), swdata.GetLength(),&dwBytesWritten,&m_ovWrite) ;
    
    if (bWriteStat)
    {
        if(swdata.GetLength()==dwBytesWritten)
            return  TRUE;
        else
            return FALSE;
    }//if (bWriteStat)
    dwError = GetLastError();
    if(dwError != ERROR_IO_PENDING)
    {   
        ClearCommError(m_hDevHandle, &dwErrorFlags, &ComStat ) ;
        return FALSE;
    }
    return TRUE;
}
UINT CCommDriver::CommThreadProc(LPVOID pVoid)
{
    CCommDriver * pTheDriver = (CCommDriver *)pVoid;
    pTheDriver->doMonitor();
    return 0;
}
//通知接收消息
void CCommDriver::NotifyReturnMsg(int nReturnFrmIndex)
{
    if(nReturnFrmIndex==m_nWaitFrmIndex)
        SetEvent(m_hEventSendContral);
}
//串口读线程函数
#if 0
DWORD CCommDriver::ReadThreadFunc(LPVOID lparam)
{
    CCommDriver *pTheDriver = (CCommDriver*)lparam;

    DWORD   evtMask;
    BYTE * readBuf = NULL;//读取的字节
    DWORD actualReadLen = 0;//实际读取的字节数
    DWORD willReadLen;

    DWORD dwReadErrors;
    COMSTAT cmState;

    // 清空缓冲，并检查串口是否打开。
    ASSERT(pTheDriver->m_hDevHandle != INVALID_HANDLE_VALUE);


    //清空串口
    PurgeComm(pTheDriver->m_hDevHandle, PURGE_RXCLEAR | PURGE_TXCLEAR);

    SetCommMask(pTheDriver->m_hDevHandle, EV_RXCHAR | EV_CTS | EV_DSR);
    while (TRUE)
    {
        if (WaitCommEvent(pTheDriver->m_hDevHandle, &evtMask, 0))
        {
            SetCommMask(pTheDriver->m_hDevHandle, EV_RXCHAR | EV_CTS | EV_DSR);
            //表示串口收到字符      
            if (evtMask & EV_RXCHAR)
            {

                ClearCommError(pTheDriver->m_hDevHandle, &dwReadErrors, &cmState);
                willReadLen = cmState.cbInQue;
                if (willReadLen <= 0)
                {
                    continue;
                }

                readBuf = new BYTE[willReadLen];
                ReadFile(pTheDriver->m_hDevHandle, readBuf, willReadLen, &actualReadLen, 0);

                //如果读取的数据大于0，
                if (actualReadLen>0)
                {
                    RespMessage(m_ComRecvBuf, m_dwRecBufHead);
                }
            }
        }
        //如果收到读线程退出信号，则退出线程
        if (WaitForSingleObject(pTheDriver->m_hRxExitEvent, 500) == WAIT_OBJECT_0)
        {
            break;
        }
    }
    return 0;
}
//串口写线程函数
DWORD CCommDriver::WriteThreadFunc(LPVOID lparam)
{
    CCommDriver *pTheDriver = (CCommDriver*)lparam;
    MSG msg;
    DWORD dwWriteLen = 0;
    BYTE * buf = NULL;

    while (TRUE)
    {
        //如果捕捉到线程消息
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            if (msg.hwnd != 0)
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
                continue;
            }
            //--if (msg.message == CM_THREADCOMMWRITE)
            {
                //向串口写
                buf = (BYTE*)msg.lParam;
                dwWriteLen = msg.wParam;
                //向串口写
                WriteDataToPort(pTheDriver->m_hDevHandle, buf, dwWriteLen);
                //删除动态分配的内存
                delete[] buf;
            }
        }
        //如果收到写线程退出信号，则退出线程
        if (WaitForSingleObject(pTheDriver->m_hTxExitEvent, 500) == WAIT_OBJECT_0)
        {
            break;
        }
        pTheDriver->m_hTxThread = NULL;
    }

    return 0;
}
#endif
//私用方法，用于向串口写数据，被写线程调用
BOOL CCommDriver::WriteDataToPort(HANDLE hComm, const BYTE *buf, DWORD bufLen)
{
    DWORD dwNumBytesWritten;
    DWORD dwHaveNumWritten = 0; //已经写入多少

    ASSERT(hComm != INVALID_HANDLE_VALUE);
    do
    {
        if (WriteFile(hComm,                    //串口句柄 
            buf + dwHaveNumWritten,             //被写数据缓冲区 
            bufLen - dwHaveNumWritten,          //被写数据缓冲区大小
            &dwNumBytesWritten,                 //函数执行成功后，返回实际向串口写的个数 
            NULL))                              //此处必须设置NULL
        {
            dwHaveNumWritten = dwHaveNumWritten + dwNumBytesWritten;
            //写入完成
            if (dwHaveNumWritten == bufLen)
            {
                break;
            }
            Sleep(10);
        }
        else
        {
            return FALSE;
        }
    } while (TRUE);

    return TRUE;
}
void CCommDriver::ReadDataFromPort(DWORD dwEvent)
{
    COMSTAT ComStat ;
    DWORD dwLength;         //缓冲区数据长度
    DWORD dwBytesRead=0;    //读取的数据个数
    DWORD dwErrorFlags;     //com操作返回标志
    DWORD dwError;
    BOOL fReadStat;
    if((dwEvent & EV_RXCHAR) != EV_RXCHAR) // 缓冲区中没有数据到达
        return;
    //输入缓冲区有多少数据？
    ClearCommError(m_hDevHandle, &dwErrorFlags, &ComStat);
    dwLength = ComStat.cbInQue;             
    if(dwLength < 1)
        return;
    //如果缓冲区积累太多，清空，防止出现错误。
    //数据量较大时，注意会丢掉数据
    if(m_uRxDataLen + dwLength > 10240)
    {
        m_uRxDataLen = 0;
    }
    //读数据
    fReadStat = ReadFile( m_hDevHandle,m_uRxDataBuf+m_uRxDataLen,dwLength,&dwBytesRead,&m_ovRead);
    if(!fReadStat)  //重叠io处理
    {
        if(GetLastError() != ERROR_IO_PENDING)
        {
            ClearCommError(m_hDevHandle, &dwErrorFlags, &ComStat ) ;
        //  ComStat.cbInQue=0;//
            return ;
        }
        int nTimeoutCon = 0;    //超时控制
        while(!GetOverlappedResult(m_hDevHandle, &m_ovRead, &dwBytesRead, FALSE ))
        {
            dwError = GetLastError();
            if(dwError == ERROR_IO_INCOMPLETE)
            {
                if(nTimeoutCon < 100)
                {
                    Sleep(2);
                    nTimeoutCon++;
                    continue;
                }
                else        //超时
                {
                    ClearCommError(m_hDevHandle, &dwErrorFlags, &ComStat ) ;                
                    return;
                }
            }
            else
            {               
                ClearCommError(m_hDevHandle, &dwErrorFlags, &ComStat ) ;                    
                return;
            }
        }//while(!GetOverlappedResult(m_hDevHandle, &m_ovRead, &dwBytesRead, FALSE ))

        //判断结束事件
        ClearCommError(m_hDevHandle, &dwErrorFlags, &ComStat );
        switch ( dwErrorFlags )
        {
        case CE_BREAK:
            TRACE("<CE-%s>\n\r", "CE_BREAK" ) ;
            return ;
        case CE_FRAME:
            TRACE("<CE-%s>\n\r", "CE_FRAME" ) ;
            return;
        case CE_IOE:
            TRACE("<CE-%s>\n\r", "CE_IEO" ) ;
            return ;
        case CE_MODE:
            TRACE("<CE-%s>\n\r", "CE_MODE" ) ;
            return ;
        case CE_OVERRUN:
            TRACE("<CE-%s>\n\r", "CE_RXOVER" ) ;
            return ;
        case CE_RXOVER:
            TRACE("<CE-%s>\n\r", "CE_RXOVER" ) ;
            return ;
        case CE_RXPARITY:
            TRACE("<CE-%s>\n\r", "CE_RXPARITY" ) ;
            return ;
        default:
            break;
        }
    }   //重叠io处理
    DWORD dwRecBufHeadLast = m_uRxDataLen;
    m_uRxDataLen += dwBytesRead;
    // 通知父窗口收到数据
    //--//--::SendMessage(m_pOwnerWnd->m_hWnd, WM_COMM_RXCHAR,0,0);
    /*if(NULL!= OnSeriesRead)
        OnSeriesRead(m_pOwnerWnd,m_ComRecvBuf, m_dwRecBufHead);*/
    RespMessage(m_uRxDataBuf, m_uRxDataLen);
    m_uRxDataLen = 0;
}

