/**
  ******************************************************************************
  * @文件     
  * @作者  
  * @版本 V1.00.00
  * @日期 2017-04-11
  * @简述   
  ******************************************************************************
  * @说明
  *
  *
  * 
  ******************************************************************************
  */

#include "stdafx.h"
#include "SerielPortM.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//定义向写线程发送的消息常量
#define   WM_THREADCOMM_WRITE    (WM_USER+110)

//类CSerialPortM构造函数
CSerialPortM::CSerialPortM()
{
	m_hComm = INVALID_HANDLE_VALUE;
    OnSerialRead = NULL;
}

//类CSerialPortM析构函数
CSerialPortM::~CSerialPortM()
{
	
}

/*
*函数介绍：打开串口
*入口参数：pPortOwner	:使用此串口类的窗体句柄
		   portNo		:串口号
		   baud			:波特率
		   parity		:奇偶校验
		   databits		:数据位
		   stopbits		:停止位
*出口参数：(无)
*返回值：TRUE:成功打开串口;FALSE:打开串口失败
*/
BOOL CSerialPortM::OpenPort(void *pPortOwner, stPortConfig_t stPortConifg)
{
	DCB commParam;
	TCHAR szPort[15];	
	
	if (m_hComm != INVALID_HANDLE_VALUE)
	{
		return TRUE;
	}

	ASSERT(stPortConifg.iPortNum > 0);
    assert(OnSerialRead != NULL);
	//设置串口名
	wsprintf(szPort, _T("\\\\.\\COM%d"), stPortConifg.iPortNum);
	//打开串口
	m_hComm = CreateFile(szPort,GENERIC_READ | GENERIC_WRITE,0,	NULL,OPEN_EXISTING,0,NULL);
	
	if (m_hComm == INVALID_HANDLE_VALUE)
	{	
		TRACE(_T("CreateFile 返回无效句柄"));
		return FALSE;
	}
	
	if (!GetCommState(m_hComm,&commParam))
	{		
		return FALSE;
	}
	
	commParam.BaudRate = stPortConifg.dwBaudRate;// 设置波特率 
	commParam.fBinary = TRUE;					// 设置二进制模式，此处必须设置TRUE
	commParam.fParity = TRUE;					// 支持奇偶校验 
	commParam.ByteSize = stPortConifg.DataBits;	// 数据位,范围:4-8 
	commParam.Parity = NOPARITY;				// 校验模式
	commParam.StopBits = stPortConifg.StopBits;	// 停止位 
	
	commParam.fOutxCtsFlow = FALSE;				// No CTS output flow control 
	commParam.fOutxDsrFlow = FALSE;				// No DSR output flow control 
	commParam.fDtrControl = DTR_CONTROL_ENABLE; 
	// DTR flow control type 
	commParam.fDsrSensitivity = FALSE;			// DSR sensitivity 
	commParam.fTXContinueOnXoff = TRUE;			// XOFF continues Tx 
	commParam.fOutX = FALSE;					// No XON/XOFF out flow control 
	commParam.fInX = FALSE;						// No XON/XOFF in flow control 
	commParam.fErrorChar = FALSE;				// Disable error replacement 
	commParam.fNull = FALSE;					// Disable null stripping 
	commParam.fRtsControl = RTS_CONTROL_ENABLE; 
	// RTS flow control 
	commParam.fAbortOnError = FALSE;			// 当串口发生错误，并不终止串口读写
	
	if (!SetCommState(m_hComm, &commParam))
	{
		TRACE(_T("SetCommState error"));		
		return FALSE;
	}
	
    //设置串口读写时间
	COMMTIMEOUTS CommTimeOuts;
	GetCommTimeouts (m_hComm, &CommTimeOuts);
	CommTimeOuts.ReadIntervalTimeout = MAXDWORD;  
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;  
	CommTimeOuts.ReadTotalTimeoutConstant = 0;    
	CommTimeOuts.WriteTotalTimeoutMultiplier = 10;  
	CommTimeOuts.WriteTotalTimeoutConstant = 1000;  
	
	//--if(!SetCommTimeouts( m_hComm, &CommTimeOuts ))
    if(!SetCommTimeouts(m_hComm, &CommTimeOuts))
	{
		TRACE( _T("SetCommTimeouts 返回错误") );
		return FALSE;
	}
	
	m_pPortOwner = pPortOwner;	
	
	//指定端口监测的事件集
	SetCommMask (m_hComm, EV_RXCHAR);
	
	//分配设备缓冲区
	SetupComm(m_hComm,512,512);
	
	//初始化缓冲区中的信息
	PurgeComm(m_hComm,PURGE_TXCLEAR|PURGE_RXCLEAR);
	
	m_hReadExitEvent  = CreateEvent(NULL,TRUE,FALSE,NULL);
	m_hWriteExitEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	//创建读串口线程
	m_hReadThread = CreateThread(NULL,0,ReadThreadFunc,this,0,&m_dwReadThreadID);
	//创建写串口线程
	m_hWriteThread = CreateThread(NULL,0,WriteThreadFunc,this,0,&m_dwWriteThreadID);
	
	TRACE(_T("串口打开成功"));
	
	return TRUE;
}
/*
*函数介绍：关闭串口
*入口参数：(无)
*出口参数：(无)
*返回值：  (无)
*/
void CSerialPortM::ClosePort()
{	
	//表示串口还没有打开
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		return ;
	}
	
	//关闭读线程
	CloseReadThread();
	//关闭写线程
	CloseWriteThread();
	
	//关闭串口
	if (!CloseHandle (m_hComm))
	{
		m_hComm = INVALID_HANDLE_VALUE;
		return ;
	}
    m_hComm = INVALID_HANDLE_VALUE;
}
/*
*函数介绍：向串口发送数据
*入口参数：buf		: 将要往串口写入的数据的缓冲区
		   bufLen	: 将要往串口写入的数据的缓冲区长度
*出口参数：(无)
*返回值：TRUE:表示成功地将要发送的数据传递到写线程消息队列。
		 FALSE:表示将要发送的数据传递到写线程消息队列失败。
		 注视：此处的TRUE,不直接代表数据一定成功写入到串口了。
*/
BOOL CSerialPortM::WritePort(const uint8_t *buf, uint32_t bufLen)
{
	//将要发送的数据传递到写线程消息队列
    /*
	if (PostThreadMessage(m_dwWriteThreadID,WM_THREADCOMM_WRITE,(WPARAM)bufLen, (LPARAM)buf))
	{
		return TRUE;
	}
    */
    DWORD dwNumBytesWritten;
    DWORD dwHaveNumWritten = 0; //

    do
    {
        if (WriteFile(m_hComm,buf + dwHaveNumWritten,bufLen - dwHaveNumWritten,&dwNumBytesWritten,NULL))								//此处必须设置NULL
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
	
	return FALSE;
}
//串口读线程函数
DWORD CSerialPortM::ReadThreadFunc(LPVOID lparam)
{
    CSerialPortM *pSerialPort = (CSerialPortM*)lparam;
	
	DWORD	evtMask;
	BYTE * readBuf = NULL;//读取的字节
	DWORD actualReadLen=0;//实际读取的字节数
	DWORD willReadLen;
	
	DWORD dwReadErrors;
	COMSTAT	cmState;
	
	//--清空缓冲，并检查串口是否打开。
	ASSERT(pSerialPort->m_hComm !=INVALID_HANDLE_VALUE);
	
	//--清空串口
	PurgeComm(pSerialPort->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR );
	//--设置串口事件
	SetCommMask (pSerialPort->m_hComm, EV_RXCHAR | EV_CTS | EV_DSR );
	while (TRUE)
	{   	
		if (WaitCommEvent(pSerialPort->m_hComm,&evtMask,0))
		{			
			SetCommMask (pSerialPort->m_hComm, EV_RXCHAR | EV_CTS | EV_DSR );
			//--表示串口收到字符		
			if (evtMask & EV_RXCHAR) 
			{                     				
				ClearCommError(pSerialPort->m_hComm,&dwReadErrors,&cmState);
				willReadLen = cmState.cbInQue ;
				if (willReadLen <= 0)
				{
					continue;
				}
				
				readBuf = new BYTE[willReadLen];
				ReadFile(pSerialPort->m_hComm, readBuf, willReadLen, &actualReadLen,0);
				
				//--如果读取的数据大于0，
				if (actualReadLen>0)
				{
					//--触发读取回调函数
                    //--if(NULL!= pSerialPort->OnSerialRead)
					    //--pSerialPort->OnSerialRead(pSerialPort->m_pPortOwner,readBuf,actualReadLen);
				}
			}
		}
		//--如果收到读线程退出信号，则退出线程
		if (WaitForSingleObject(pSerialPort->m_hReadExitEvent,500) == WAIT_OBJECT_0)
		{
			break;
		}
	}
	return 0;
}

//串口写线程函数
DWORD CSerialPortM::WriteThreadFunc(LPVOID lparam)
{
    CSerialPortM *pSerialPort = (CSerialPortM*)lparam;
	MSG msg;
	DWORD dwWriteLen = 0;
	BYTE * buf = NULL;
	
	while (TRUE)
	{
		//如果捕捉到线程消息
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.hwnd != 0 )
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				continue;
			}
			if (WM_THREADCOMM_WRITE == msg.message)
			{
				//向串口写
				buf = (BYTE*)msg.lParam;
				dwWriteLen = msg.wParam;
				//向串口写
				WritePort(pSerialPort->m_hComm,buf,dwWriteLen);
                //--WriteFile(ceSeries->m_hComm, buf, dwWriteLen, &dwNumBytesWritten, NULL);
				//删除动态分配的内存
				delete[] buf;
			}
		}
		//如果收到写线程退出信号，则退出线程
		if (WaitForSingleObject(pSerialPort->m_hWriteExitEvent,500) == WAIT_OBJECT_0)
		{
            pSerialPort->m_hWriteThread = NULL;
			break;
		}
        //--pSerialPort->m_hWriteThread = NULL;
	}
	
	return 0;
}

//私用方法，用于向串口写数据，被写线程调用
BOOL CSerialPortM::WritePort(HANDLE hComm,const BYTE *buf,DWORD bufLen)
{
	DWORD dwNumBytesWritten;
	DWORD dwHaveNumWritten = 0 ; //
	
	ASSERT(hComm != INVALID_HANDLE_VALUE);
	do
	{
		if (WriteFile (hComm,					//串口句柄 
			buf+dwHaveNumWritten,				//被写数据缓冲区 
			bufLen - dwHaveNumWritten,          //被写数据缓冲区大小
			&dwNumBytesWritten,					//函数执行成功后，返回实际向串口写的个数	
			NULL))								//此处必须设置NULL
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
	}while (TRUE);
	
	return TRUE;	
}

//关闭读线程
void CSerialPortM::CloseReadThread()
{
	SetEvent(m_hReadExitEvent);
	
    //等待10秒，如果读线程没有退出，则强制退出
    if (WaitForSingleObject(m_hReadThread,1000) == WAIT_TIMEOUT)
	{
		TerminateThread(m_hReadThread,0);
	}
	m_hReadThread = NULL;

    //设置所有事件无效无效
    SetCommMask(m_hComm, 0);
    //清空所有将要读的数据
    PurgeComm(m_hComm, PURGE_RXCLEAR);
}

//关闭写线程
void CSerialPortM::CloseWriteThread()
{
	SetEvent(m_hWriteExitEvent);
	
	//清空所有将要写的数据
    PurgeComm( m_hComm,  PURGE_TXCLEAR );
	
    //等待10秒，如果读线程没有退出，则强制退出
    if (WaitForSingleObject(m_hWriteThread,10000) == WAIT_TIMEOUT)
	{
		TerminateThread(m_hWriteThread,0);
	}
	m_hWriteThread = NULL;
}