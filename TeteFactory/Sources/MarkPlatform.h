/**
******************************************************************************
* @模块 
* @作者 jchq
* @版本 V1.00.00
* @日期 2018-09-21
* @简述 
* @email:jchq0101net@sina.com
******************************************************************************
* @说明 
*    
*    
*    
*      
*        
*    
*    
******************************************************************************
*/
#ifndef H_MARK_PLATFORM_H
#define H_MARK_PLATFORM_H
#include <stdint.h>
#include "LoadMarkCalib.h"
#include "LoadMarkDriver.h"
#include "LoadMarkDevice.h"
#include "LoadMarkEngine.h"
#include "LoadGraphEngine.h"
#include "tlPlatform.h"

class IMsgWndInterface
{
public:
	virtual ~IMsgWndInterface() {};

	virtual int32_t doBin(char *sRecvBuff, int32_t iSize) = 0;
	virtual int32_t doMark(char *sRecvBuff, int32_t iSize) = 0;
	virtual int32_t doAutoLot(char *sRecvBuff, int32_t iSize) = 0;    // 自动接收印章图档名称
	virtual int32_t doCheckMarkStatus(char *sRecvBuff, int32_t iSize) = 0;    // 检查标记状态
	virtual int32_t doPosMatrix(char *sRecvBuff, int32_t iSize) = 0;    // 更改阵列位置
	virtual int32_t do2DData(char *sRecvBuff, int32_t iSize) = 0;
	virtual int32_t doFinishLot(char *sRecvBuff, int32_t iSize) = 0;       // 结批
	virtual int32_t doReciveSetVisionResult(char *sRecvBuff, int32_t iSize) = 0;
	virtual int32_t doRecivePLCSwitchProcessResult(char *sRecvBuff, int32_t iSize) = 0;
	virtual int32_t doReciveSetCharCheckResult(char *sRecvBuff, int32_t iSize) = 0;
	virtual void PrintMesMessage(int nType, const TCHAR* sMsg) = 0;
};

class CMarkPlatform : public IPlatform
{        
public:
    static CMarkPlatform *Instance();
    ~CMarkPlatform();

public:      
    virtual int32_t Open(void);
    virtual int32_t Close(void);
    virtual const char *GetVersion(void) { return NULL; };  //--版本信息
    virtual int32_t Error(int32_t iUser, int32_t iCmd, void *pMsg) { return 0; };   //--向平台报告出错信息
    virtual int32_t Notify(int32_t iUser,int32_t iCmd,void *pMsg);  //--向平台发送通知指令
    virtual int32_t Message(emMsgType_t emMsgType,const char * strOutputString,...);//--向平台发送消息
    virtual void PrintLog(emLogLevel_t emLevel, const char * strOutputString, ...);
    virtual const char *GetSystemPath(void);
    virtual const char *GetSystemPath(const char *sSubPath);
    virtual void SystemIdle(void);  //--空闲等待
    virtual void Delay(uint32_t ms); //--延时毫秒
    virtual void Delay(uint32_t min,uint32_t sec,uint32_t ms);//--延时分秒毫秒

    virtual IConfigure *Configure(void);         //--配置信息
    virtual IMarkCalib *MasterCalib(void);         //--标记校正
    virtual IMarkCalib *SlaverCalib(void);
    virtual IMarkDriver *MarkDriver(void);       //--标记板卡
    virtual IMarkDevice *MarkDevice(void);       //--标记设备(激光器)    
    virtual IMarkEngine *MarkEngine(void);       //--标记引擎    
    virtual IMotionDriver *MotionDriver(void);   //--运动控制
    virtual IVisionDriver *VisionDriver(void);   //--CCD视觉
    virtual IObjectEngine *ObjectEngine(void);
    virtual IRenderEngine *RenderEngine(void);

    virtual int32_t GetPlatformCtrl(int32_t iCmd, void *pArg);
    virtual int32_t SetPlatformCtrl(int32_t iCmd, void *pArg);
public:
    void RegisterMsgWnd(IMsgWndInterface *pTheWnd) { m_pTheMsgWnd = pTheWnd; }
	void MsgWaitForThread(HANDLE * p);
protected:
    CMarkPlatform(void);    
private:
    static CMarkPlatform *_instance;

    CLoadMarkCalib  *m_pLoadMarkCalib;
    CLoadMarkDriver *m_pLoadMarkDriver;
    CLoadMarkDevice *m_pLoadMarkDevice;
    CLoadMarkEngine *m_pLoadMarkEngine;
    CLoadGraphEngine *m_pLoadGraphEngine;
    CLoadRenderEngine *m_pLoadRenderEngine;
    CLoadMotionDriver *m_pLoadMotionDriver;
    CLoadVisionDriver *m_pLoadVisionDriver;

	IMsgWndInterface *m_pTheMsgWnd;
    void LoadConfig(void);
    void SaveConfig(void);
	emLogLevel_t switchMsgTypeToLogType(const emMsgType_t& emMsgType) const;
	emFactoryMsgType_t switchMsgTypeToFactoryMsgType(const emMsgType_t& emMsgType) const;
};

#endif