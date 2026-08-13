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
#ifndef H_TL_MARK_DRIVER_H
#define H_TL_MARK_DRIVER_H

#include "tlMarkDefine.h"
//--#include "tlMarkCalib.h"
/* 扫描头输出模式 */
enum {
    emHEAD_M = 0x0001,  /* 仅主头输出 */
    emHEAD_S = 0x0002,  /* 仅从头输出*/
    emHEAD_MS = 0x0003  /* 双头输出 */
};
/* 扫描头信号类型 */
enum {
    emHEAD_DA=0,
    emHEAD_XY
};
/* 板卡状态类型 */
typedef enum{   
    emDRIVER_STAT_IDLE = 0,  /* 空闲，数据缓存为空 */
    emDRIVER_STAT_BUSY,      /* 正在传送数据 */
    emDRIVER_STAT_STOP,      /* 停止标记 */
    emDRIVER_STAT_ERROR      /* 板卡出错 */
}emDrvStat_t;

typedef struct _MarkPrm_t
{
    uint32_t nJumpSpeed;    //--空跳速度
    uint32_t nMarkSpeed;    //--标记速度

    int32_t iOnDelay;      //--开光延时
    int32_t iOffDelay;     //--关光延时

    int32_t iJumpDelay;    //--起笔延时
    int32_t iMarkDelay;    //--末笔延时
    uint32_t nPolyDelay;   //--拐点延时
    uint32_t nDotPeriod;   //--点持续时间

    bool IsSky;            //--是否空飞
    double dbSkyLength;    //-空飞段长度
}xMarkPrm_t;

/**
* @breif 首脉冲抑制结构
*/
typedef struct _FPKPrm_t
{
	bool IsFPKEnable;              // 是否启用首脉冲抑制
	uint32_t dwFPKLength;          // 首脉冲抑制时长[us] - 采用数字信号进行首脉冲抑制时设置，暂未启用
	uint16_t wVol0, wVol1;       // 首脉冲抑制起止电压 - 采用模拟电压进行首脉冲抑制时设置
	uint32_t dwT1, dwT2;         // T1:首脉冲抑制时长，T2:最小关开光间隔，小于该值时不抑制， 采用模拟电压进行首脉冲抑制时设置
}xFPKPrm_t;

class IPlatform;
/* 板卡控制命令字 */
#define CMD_MDRV_VER        ((int32_t)0x0000)      /* 版本信息     */
#define CMD_GATE_LEVEL      ((int32_t)0x0001)      /* GATE信号电平 */
//--#define CMD_MARK_HEAD       ((int32_t)0x0002)  /* 扫描头 1:主,2:从，0/3：主从*/
#define CMD_HEAD_TYPE  ((int32_t)0x0003)      /* 扫描头型号，模拟还是数字 */

#define CMD_MASTER_SAFE     ((int32_t)0x0101)      /* 主头安全位置 */
#define CMD_MASTER_TURN     ((int32_t)0x0102)      /* 主头变换 */

#define CMD_SLAVER_SAFE     ((int32_t)0x0201)      /* 从头安全位置 */
#define CMD_SLAVER_TURN     ((int32_t)0x0202)      /* 从头变换 */

#define CMD_FPKPRM  ((int32_t)0x0301)      /* 首脉冲抑制参数，对应xFPKPrm_t参数*/

#define CMD_MARKREAL_INDEX  ((int32_t)0x0401)      /* 实时标记序号，对应uint16_t类型，存储在寄存器0x18的低16位，也就是XY2-100的Z地址*/
/* 板卡接口返回值 */
#define MDRV_OK    0

class TL_API IMarkDriver
{
public:
	//--IMarkDriver() {};
	virtual ~IMarkDriver() {};

    virtual int32_t GetType() = 0;
    virtual const char *GetName() = 0;
    virtual int32_t LoadConfig(const char *sFile) = 0;
    virtual int32_t SaveConfig(const char *sFile) = 0;
	virtual int32_t Open(IPlatform *pPlatform) = 0;     //--加载板卡驱动、初始化板卡
	virtual int32_t Close(void) = 0;                    //--复位板卡、卸载板卡驱动
    virtual int32_t GetStatus(void) = 0;                //--获取板卡状态
    
	virtual int32_t GetDriverCtrl(int32_t iCmd, void *pArg) = 0;
	virtual int32_t SetDriverCtrl(int32_t iCmd, void *pArg) = 0;
   
    virtual int32_t SetScanHead(int32_t iMaster, int32_t iSlaver=0) = 0;   //--0禁用，1启用
    virtual int32_t SetLaserPulse(float dbFreq,float dbWidth) = 0;               //--Q-Freq输出
    virtual int32_t SetStandbyPulse(float dbFreq,float dbWidth) = 0;             //--预激励输出
    virtual int32_t GetMarkParam(xMarkPrm_t *pMarkPrm) = 0;
    virtual int32_t SetMarkParam(const xMarkPrm_t *pMarkPrm) = 0;
    /*virtual int32_t SetScanSpeed(uint32_t dwMarkSpeed, uint32_t dwJumpSpeed) = 0;
	virtual int32_t SetLaserDelay(int32_t iOnDelay, int iOffDelay) = 0;
	virtual int32_t SetScanDelay(int32_t iJumpDelay, int iMarkDelay,uint32_t iPolygonDelay) = 0;
    virtual int32_t SetSkyParam(bool IsSky,double dbSkyLength) = 0;*/

	virtual int32_t LaserOn(void) = 0;
	virtual int32_t LaserOff(void) = 0;
	virtual int32_t MoveMaster(uint32_t nPosX, uint32_t nPosY) = 0;  //--移动振镜到指定电压位置
    virtual int32_t MoveSlaver(uint32_t nPosX, uint32_t nPosY) = 0; 
    virtual int32_t TransferCommand(uint32_t *pMarkData,uint32_t dwSize) = 0;

    virtual int32_t MarkBegin(void) = 0;  //--1：主，2：从，0/3：主从
    virtual int32_t MarkEnd(void) = 0;    //--通知板卡准备结束标记
    virtual int32_t StopMark(void) = 0;

    virtual int32_t PathBegin(void) = 0;
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

    //--virtual void SetOffset(int32_t iHead,double dbOffsetX, double dbOffsetY) = 0;
    //--virtual void SetAngle(int32_t iHead,double dbAngle) = 0;
    
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
