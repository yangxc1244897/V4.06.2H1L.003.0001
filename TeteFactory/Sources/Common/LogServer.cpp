/**
  ******************************************************************************
  * @文件     
  * @作者  
  * @版本 V1.00.00
  * @日期 2017-04-11
  * @简述   
  * @email:jchq0101net@sina.com
  ******************************************************************************
  * @说明
  *
  *
  * 
  ******************************************************************************
  */
#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdio.h>
#include <time.h>
#include "LogServer.h"

class CDbgPrinter {
public:
    CDbgPrinter() {}
    virtual ~CDbgPrinter() {}
public:
    virtual void printfs(const char * sOutputString) { };
};
class CStdPrinter : public CDbgPrinter{
public:
    CStdPrinter() {}
    virtual ~CStdPrinter() {}
public:
    virtual void printfs(const char * sOutputString) { OutputDebugStringA(sOutputString); };
};
class CLogPrinter : public CDbgPrinter{
public:
    CLogPrinter() {
        char sFileName[255];
        time_t rawtime;
        struct tm* timeinfo;
        time(&rawtime);
        timeinfo=localtime(&rawtime);
        //--strftime(sFileName,255,".\\Log\\TLLog_%Y%m%d%H%M%S.log",timeinfo);
        strftime(sFileName,255,".\\Log\\TLLog_%Y%m%d%H%M%S.log",timeinfo);
        m_pTheFile = fopen(sFileName, "a");        
    }
    virtual ~CLogPrinter() { fclose(m_pTheFile); }
public:
    virtual void printfs(const char * sOutputString) { fputs(sOutputString,m_pTheFile);fflush(m_pTheFile); };
private:
    FILE *m_pTheFile;
};
//---------------------------------------------------------------------------
CLogServer* CLogServer::_instance = NULL;
CLogServer *CLogServer::Instance()
{
    if(NULL==_instance)
    {
        _instance = new CLogServer();
    }
    return _instance;
}
CLogServer::CLogServer(void)
{
    m_iLevel = emLOG_LEVEL_DEBUG;
    m_pThePrinter = new CStdPrinter;
}
CLogServer::~CLogServer(void)
{
    delete m_pThePrinter;
}
void CLogServer::SetPrinter(emLogPrinter_t emPrinter) 
{ 
    delete m_pThePrinter;
    switch (emPrinter)
    {
    case emLOG_PRINT_VIEW:        
        m_pThePrinter = new CStdPrinter;
        info("LOG output:std\n");
        break;
    case emLOG_PRINT_FILE:        
        m_pThePrinter = new CLogPrinter;
        info("LOG output:log\n");
        break;
    default:        
        m_pThePrinter = new CDbgPrinter;
        info("LOG output:null\n");
        break;
    }
}
void CLogServer::SetLevel(emLogLevel_t emLevel)
{ 
    m_emLevel = emLevel;
    m_iLevel = emLevel; 
    switch (emLevel)
    {
    case emLOG_LEVEL_INFO:
        info("LOG level: INFO\n");
        break;
    case emLOG_LEVEL_WARN:
        info("LOG level: WARN\n");
        break;
    case emLOG_LEVEL_ERROR:
        info("LOG level: ERROR\n");
        break;
    case emLOG_LEVEL_FATAL:
        info("LOG level: FATAL\n");
        break;
    case emLOG_LEVEL_NULL:
        info("LOG level: NULL\n");
        break;
    default:break;
    }
}
void CLogServer::debug(const char * strOutputString)
{
    if (m_iLevel > emLOG_LEVEL_INFO)
        return;
    /*char strBuffer[1024] = {'T','E','T','E','-','D',':',' '};
    va_list vlArgs;
    va_start(vlArgs,strOutputString);
    _vsnprintf(strBuffer+8,sizeof(strBuffer),strOutputString,vlArgs);
    va_end(vlArgs);*/
    /*int iLen = strlen(strBuffer);
    if('\n'!=strBuffer[iLen-1])
    strcat(strBuffer, "\n");
    m_pThePrinter->printfs(&strBuffer[0]);*/
    m_pThePrinter->printfs(strOutputString);
}
void CLogServer::info(const char * strOutputString)
{
    if (m_iLevel > emLOG_LEVEL_INFO)
        return;
    /*char strBuffer[1024] = {'T','E','T','E','-','G',':',' '};
    va_list vlArgs;
    va_start(vlArgs,strOutputString);
    _vsnprintf(strBuffer+8,sizeof(strBuffer),strOutputString,vlArgs);
    va_end(vlArgs);
	/*int iLen = strlen(strBuffer);
	if('\n'!=strBuffer[iLen-1])
	    strcat(strBuffer, "\n");
    m_pThePrinter->printfs(&strBuffer[0]);*/
    m_pThePrinter->printfs(strOutputString);
}
void CLogServer::warn(const char * strOutputString)
{
    if (m_iLevel > emLOG_LEVEL_WARN)
        return;
    /*char strBuffer[1024] = {'T','E','T','E','-','W',':',' '};
    va_list vlArgs;
    va_start(vlArgs,strOutputString);
    _vsnprintf(strBuffer+8,sizeof(strBuffer),strOutputString,vlArgs);
    va_end(vlArgs);
    m_pThePrinter->printfs(&strBuffer[0]);*/
    m_pThePrinter->printfs(strOutputString);
}
void CLogServer::error(const char * strOutputString)
{
    if (m_iLevel > emLOG_LEVEL_ERROR)
        return;
    /*char strBuffer[1024] = {'T','E','T','E','-','E',':',' '};
    va_list vlArgs;
    va_start(vlArgs,strOutputString);
    _vsnprintf(strBuffer+8,sizeof(strBuffer),strOutputString,vlArgs);
    va_end(vlArgs);
    m_pThePrinter->printfs(&strBuffer[0]);*/
    m_pThePrinter->printfs(strOutputString);
}
void CLogServer::fatal(const char * strOutputString)
{
    if (m_iLevel > emLOG_LEVEL_FATAL)
        return;
    /*char strBuffer[1024] = {'T','E','T','E','-','F',':',' '};
    va_list vlArgs;
    va_start(vlArgs,strOutputString);
    _vsnprintf(strBuffer+8,sizeof(strBuffer),strOutputString,vlArgs);
    va_end(vlArgs);
    m_pThePrinter->printfs(&strBuffer[0]);*/
    m_pThePrinter->printfs(strOutputString);
}


