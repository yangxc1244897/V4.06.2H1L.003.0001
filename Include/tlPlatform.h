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
#ifndef H_TL_PLATFORM_H
#define H_TL_PLATFORM_H

#include "tlTeteDefines.h"
/*#include "tlGraphEngine.h"
#include "tlMarkEngine.h"*/
#include "tlLogServer.h"

typedef enum {
    emMSG_TYPE_INFO,        /* 仅输出普通及以上信息 */
    emMSG_TYPE_WARN,        /* 仅输出告警及以上信息 */
    emMSG_TYPE_ERROR,       /* 仅输出错误及以上信息 */
    emMSG_TYPE_FATAL,       /* 仅输出致命错误信息   */
}emMsgType_t;
class ILogServer;
class IMarkCalib;
class IMarkDriver;
class IMarkDevice;
class IMarkEngine;
class IRenderEngine;
class IObjectEngine;
class IVisionDriver;
class IMotionDriver;
class TL_API IConfigure {
public:
    virtual ~IConfigure() {};

    virtual bool SetDataValue(const char *sSegName, const char *sKeyName, bool value) = 0; 
    virtual bool SetDataValue(const char *sSegName, const char *sKeyName, int8_t value) = 0;
    virtual bool SetDataValue(const char *sSegName, const char *sKeyName, int32_t value) = 0;
    virtual bool SetDataValue(const char *sSegName, const char *sKeyName, uint32_t value) = 0;
    virtual bool SetDataValue(const char *sSegName, const char *sKeyName, float value) = 0;
    virtual bool SetDataValue(const char *sSegName, const char *sKeyName, double value) = 0;
    virtual bool SetDataValue(const char *sSegName, const char *sKeyName, const char *value) = 0;

    virtual bool GetBoolValue(const char *sSegName, const char *sKeyName, bool defValue) = 0;
    virtual int8_t GetInt8Value(const char *sSegName, const char *sKeyName, int8_t defValue) = 0;
    virtual int32_t GetInt32Value(const char *sSegName, const char *sKeyName, int32_t defValue) = 0;
    virtual uint32_t GetUnsignedValue(const char *sSegName, const char *sKeyName, uint32_t defValue) = 0;
    virtual float GetFloatValue(const char *sSegName, const char *sKeyName, float defValue) = 0;
    virtual double GetDoubleValue(const char *sSegName, const char *sKeyName, double defValue) = 0;
    virtual char *GetStringValue(const char *sSegName, const char *sKeyName, const char *defValue) = 0;
};
class TL_API IPlatform {
public:
    virtual ~IPlatform() {};

    virtual int32_t Open(void) = 0;
    virtual int32_t Close(void) = 0;
    virtual const char *GetVersion(void) = 0;    //--版本信息
    virtual const char *GetSystemPath(void) = 0;
    virtual const char *GetSystemPath(const char *sSubPath) = 0;
    virtual int32_t Error(int32_t iUser,int32_t iCmd,void *pMsg) = 0;   //--向平台报告出错信息
    virtual int32_t Notify(int32_t iUser,int32_t iCmd,void *pMsg) = 0;  //--向平台发送通知指令
    virtual int32_t Message(emMsgType_t emMsgType,const char * strOutputString,...) = 0;//--向平台发送指示类消息
    virtual void PrintLog(emLogLevel_t emLevel,const char * strOutputString,...) = 0;//--向平台输出日志信息    
    virtual void SystemIdle(void) = 0;                             //--空闲等待
    virtual void Delay(uint32_t ms) = 0;                           //--延时xx毫秒
    virtual void Delay(uint32_t min,uint32_t sec,uint32_t ms) = 0; //--延时min分sec秒ms毫秒
    
    virtual IConfigure *Configure(void) = 0;            //--配置信息
    virtual IMarkCalib *MasterCalib(void) = 0;            //--标记校正
    virtual IMarkCalib *SlaverCalib(void) = 0;            //--标记校正
    virtual IMarkDriver *MarkDriver(void) = 0;          //--标记板卡
    virtual IMarkDevice *MarkDevice(void) = 0;          //--标记设备(激光器)    
    virtual IMarkEngine *MarkEngine(void) = 0;          //--标记引擎
    virtual IVisionDriver *VisionDriver(void) = 0;      //--CCD视觉
    virtual IMotionDriver *MotionDriver(void) = 0;      //--运动控制
    virtual IRenderEngine *RenderEngine(void) = 0;      //--渲染引擎
    virtual IObjectEngine *ObjectEngine(void) = 0;        //--图形引擎

    virtual int32_t GetPlatformCtrl(int32_t iCmd, void *pArg) = 0;
    virtual int32_t SetPlatformCtrl(int32_t iCmd, void *pArg) = 0;
};

#endif
 