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
#ifndef H_LOG_SERVER_H
#define H_LOG_SERVER_H

#include "tlTeteDefines.h"
#include "tlLogServer.h"
//---------------------------------------------------------------------------
#define  LOG(a) CLogServer::Instance()->SetPrintType(a)
#define  LOG_LEVEL(a) CLogServer::Instance()->SetLevel(a)
#define  LOG_INFO     CLogServer::Instance()->info
#define  LOG_WARN     CLogServer::Instance()->warn
#define  LOG_ERROR    CLogServer::Instance()->error
#define  LOG_FATAL    CLogServer::Instance()->fatal

class CDbgPrinter;
class CLogServer : public ILogServer
{
public:
    static CLogServer *Instance();
    ~CLogServer();

public:
    void SetPrinter(emLogPrinter_t emPrinter);
    void SetLevel(emLogLevel_t emLevel);
    emLogLevel_t GetLevel(void)const { return m_emLevel; };
    void debug(const char * strOutputString);
    void info(const char * strOutputString);
    void warn(const char * strOutputString);
    void error(const char * strOutputString);
    void fatal(const char * strOutputString);
private:
    CLogServer(void);
    static CLogServer *_instance;
    CDbgPrinter  *m_pThePrinter;
    emLogLevel_t m_emLevel;  /* 需要记录的日志最大等级 */
    int m_iLevel;
};

#endif
