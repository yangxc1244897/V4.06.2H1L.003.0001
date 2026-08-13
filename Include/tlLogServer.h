/**
  ******************************************************************************
  * @文件     
  * @作者  
  * @版本 V1.00.00
  * @日期 2018-04-11
  * @简述   
  * @email:jchq0101net@sina.com
  ******************************************************************************
  * @说明
  *
  *
  * 
  ******************************************************************************
  */
#ifndef H_TL_LOG_SERVER_H
#define H_TL_LOG_SERVER_H

#include "tlTeteDefines.h"
typedef enum {
    emLOG_LEVEL_DEBUG=0 ,    /* 输出所有调试信息     */
    emLOG_LEVEL_INFO,        /* 仅输出普通及以上信息 */
    emLOG_LEVEL_WARN,        /* 仅输出告警及以上信息 */
    emLOG_LEVEL_ERROR,       /* 仅输出错误及以上信息 */
    emLOG_LEVEL_FATAL,       /* 仅输出致命错误信息   */
    emLOG_LEVEL_NULL         /* 不输出任何信息       */
}emLogLevel_t;
typedef enum {
    emLOG_PRINT_NULL = 0,    /* 无输出 */
    emLOG_PRINT_VIEW,        /* 输出至查看器中  */
    emLOG_PRINT_FILE         /* 输出至log文件中 */
}emLogPrinter_t;

class TL_API ILogServer
{
public:
    virtual ~ILogServer() {};

    virtual void SetPrinter(emLogPrinter_t emType) = 0;
    virtual void SetLevel(emLogLevel_t emLevel) = 0;
    virtual emLogLevel_t GetLevel(void)const = 0;

    virtual void debug(const char * strOutputString) = 0;
    virtual void info(const char * strOutputString) = 0;
    virtual void warn(const char * strOutputString) = 0;
    virtual void error(const char * strOutputString) = 0;
    virtual void fatal(const char * strOutputString) = 0;
};

#endif
 