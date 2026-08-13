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
#pragma once
#include <stdint.h>

typedef struct _STRU_PortConfig {
	int iPortNum;
	DWORD dwBaudRate;			//波特率
	BYTE DataBits;				//数据位
	BYTE StopBits;				//停止位
	BYTE PorityBit;				//校验位
}stPortConfig_t;
//定义串口接收数据函数类型
//--typedef void (CALLBACK* OnSeralReadFunc)(void*,BYTE* buf,int bufLen);
typedef void (* OnSeralReadFunc)(void*, uint8_t* buf, uint32_t bufLen);

//CE串口通讯类
class CSerialPortM
{

public:
    CSerialPortM();
	virtual ~CSerialPortM();
public:
	
	BOOL OpenPort(void *pPortOwner,stPortConfig_t stPortConifg);//打开串口
	void ClosePort(); //关闭串口

    OnSeralReadFunc OnSerialRead; //回调函数读取串口接收数据
	BOOL WritePort(const uint8_t *buf,uint32_t dwLen);//向串口写入数据
private:    
    static  DWORD WINAPI ReadThreadFunc(LPVOID lparam);//串口读线程函数
    static  DWORD WINAPI WriteThreadFunc(LPVOID lparam);//串口写线程函数

	static BOOL WritePort(HANDLE hComm,const BYTE *buf,DWORD bufLen); //向串口写入数据

	void CloseReadThread();//关闭读线程
	void CloseWriteThread();//关闭写线程
private:
    //已打开的串口句柄
	HANDLE	m_hComm;
	void*   m_pPortOwner;

	HANDLE m_hReadThread;//读写线程句柄
	HANDLE m_hWriteThread;//读写线程句柄

	DWORD m_dwReadThreadID;//读写线程ID标识
	DWORD m_dwWriteThreadID;//读写线程ID标识

	
	HANDLE m_hReadExitEvent;//--读线程退出事件
	HANDLE m_hWriteExitEvent;//--写线程退出事件
};

