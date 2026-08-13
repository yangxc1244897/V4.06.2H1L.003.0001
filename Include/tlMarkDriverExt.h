/**
******************************************************************************
* @文件     
* @作者  
* @版本 V1.00.00
* @日期 2018-02-10
* @简述 TETE激光打标控制板卡驱动类模型
* @email:jchq0101net@sina.com
******************************************************************************
* @说明
*
*
* 
******************************************************************************
*/
#ifndef H_TL_MARK_DRIVER_EXT_H
#define H_TL_MARK_DRIVER_EXT_H

#include "tlMarkDefine.h"
#include "tlMarkDriver.h"

#define SCAN_CHN_MASTER    0x0001    /* 仅主通道输出信号   */
#define SCAN_CHN_SLAVER    0x0002    /* 仅从通道输出信号   */
#define SCAN_CHN_BOTH      0x0003    /* 主从通道均输出信号 */

class IPlatform;

class TL_API IMarkDriverExt
{
public:
	//--IMarkDriverExt() {};
	virtual ~IMarkDriverExt() {};

    virtual int32_t GetType() = 0;
    virtual const char *GetName() = 0;
	virtual int32_t Open(IPlatform *pPlatform) = 0;    //--加载板卡驱动、初始化板卡
	virtual int32_t Close(void) = 0;                    //--复位板卡、卸载板卡驱动
    virtual int32_t GetStatus(void) = 0;                //--获取板卡状态
    
	virtual int32_t GetDriverCtrl(int32_t iCmd, void *pArg) = 0;
	virtual int32_t SetDriverCtrl(int32_t iCmd, void *pArg) = 0;
   
    virtual int32_t SetHeadOutput(int32_t iHeadDrv, int32_t iHeadChn) = 0;   //--扫描头类型及输入通道
    virtual int32_t SetLaserPulse(float dbFreq,float dbWidth) = 0;    //--Q-Freq输出
    virtual int32_t SetStandbyPulse(float dbFreq,float dbWidth) = 0;  //--预激励输出
    virtual int32_t SetScanSpeed(uint32_t dwMarkSpeed, uint32_t dwJumpSpeed) = 0;
	virtual int32_t SetLaserDelay(int32_t iOnDelay, int iOffDelay) = 0;
	virtual int32_t SetScanDelay(int32_t iJumpDelay, int iMarkDelay,uint32_t iPolygonDelay) = 0;
    virtual int32_t SetSkyParam(bool IsSky,double dbSkyLength) = 0;

	virtual int32_t LaserOn(void) = 0;
	virtual int32_t LaserOff(void) = 0;
	virtual int32_t MoveMaster(uint32_t nPosX, uint32_t nPosY) = 0;  //--移动振镜到指定电压位置
    virtual int32_t MoveSlaver(uint32_t nPosX, uint32_t nPosY) = 0;  //--移动振镜到指定电压位置
    virtual int32_t TransferCommand(uint32_t *pMarkData,uint32_t dwSize) = 0;

    virtual int32_t MarkBegin(void) = 0;  //--通知板卡准备开始标记
    virtual int32_t MarkEnd(void) = 0;    //--通知板卡准备结束标记
    virtual int32_t StopMark(void) = 0;

    virtual int32_t PathBegin(uint32_t nScanChn) = 0;
    virtual int32_t PathEnd(void) = 0;
    virtual int32_t MoveTo(double px,double py) = 0;
    virtual int32_t LightTo(double px,double py) = 0;
    virtual int32_t LineTo(double px,double py) = 0;
    virtual int32_t MarkPoint(double px,double py) = 0;    
    virtual int32_t MarkLine(double x1,double y1,double x2,double y2) = 0;
    virtual int32_t MarkPolyline(DPoint_t *lppt,uint32_t dwCount) = 0;    
    virtual int32_t MarkRectangle(double x1,double y1,double x2,double y2) = 0;
    virtual int32_t MarkEllipse(double cx,double cy,double xa,double ya,double xb,double yb) = 0;
    virtual int32_t MarkCircle(double cx,double cy,double radius) = 0;
    virtual int32_t MarkArc(double cx,double cy,double radius,double startAng,double endAng,bool IsClockWise) = 0;
    
public:
    virtual int32_t ReadPortPin(uint32_t iPin, uint32_t &nBit) = 0;    //--读主端口引脚状态
    virtual int32_t WritePortPin(uint32_t iPin, uint32_t nBit) = 0;    //--写主端口引脚状态
    virtual int32_t ReadPortPins(uint32_t &nBits) = 0;                 //--读主端口状态
    virtual int32_t WritePortPins(uint32_t nBits) = 0;                 //--写主端口状态
    
    virtual int32_t ReadExtPortPin(uint32_t iPin, uint32_t &nBit) = 0;    //--读扩展端口引脚状态
    virtual int32_t WriteExtPortPin(uint32_t iPin, uint32_t nBit) = 0;    //--写扩展端口引脚状态
    virtual int32_t ReadExtPortPins(uint32_t &nBits) = 0;                 //--读扩展端口状态       
    virtual int32_t WriteExtPortPins(uint32_t nBits) = 0;                 //--读扩展端口状态

	virtual int32_t ReadCommand(uint8_t cmd, uint32_t *pData) = 0;   //--读命令
	virtual int32_t WriteCommand(uint8_t cmd, uint32_t wData) = 0;   //--写命令
	virtual int32_t ReadRegister(uint8_t reg, uint32_t *pData) = 0;  //--读寄存器状态
	virtual int32_t WriteRegister(uint8_t reg, uint32_t wData) = 0;  //--写寄存器状态
};

#endif
