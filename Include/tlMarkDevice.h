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
#ifndef H_TL_MARK_DEVICE_H
#define H_TL_MARK_DEVICE_H

#include "tlMarkDefine.h"
#include "tlGraphFile.h"
//--#include "tlMarkDriver.h"
/*----  标记笔类模型 ---------------------------------------------*/
class TL_API IMarkPen
{
public:
    char  sName[512];    //--笔名，标识而已
    uint32_t dwColor;    //--颜色

    bool IsUseEngery;         //--是否使用功率[W]
    float dbEngery;           //--功率[W]
    float dbPower;            //--功率[%]，50.6 = 50.6%
    float dbFreq;             //--频率[kHz]
                               /*-- Scan Info --*/
    uint32_t dwMarkSpeed;        //--标记速度[mm/s]
    uint32_t dwJumpSpeed;        //--跳转速度[mm/s]

    int32_t iDelayLaserOn;     //--开光延时[us]
    int32_t iDelayLaserOff;    //--关光延时[us]

    int32_t iDelayJump;        //--起笔/跳转延时[us],等待振镜跳转到位并停稳
    int32_t iDelayMark;        //--末笔/标记延时[us],等待振镜标记到位并停稳    
    uint32_t dwDelayPolygon;     //--拐点/多边形延时[us]         =(1-cos@)
    uint32_t dwDelayDot;         //--点延时[us]
    uint32_t dwDelayStep;        //--中间/步间延时[us]

    bool     IsSkyEnable;        //--空飞使能
    double   dbSkyDistance;      //--空飞距离[mm]

public:
    int32_t iPenType;        //--笔类型，实际值是激光器类型emLaserType_xxx
    int32_t iPenID;          //--笔的编号，用于笔管理
                             //--IMarkPen();
    virtual ~IMarkPen() {};
    virtual bool LoadFile(IFileReader *pTheReader) = 0;
    virtual bool SaveFile(IFileWriter *pTheWriter) = 0;
    double GetJumpStep() { return (double)(dwJumpSpeed*dwDelayStep)*0.001; };//--s=vt,[um]
    double GetMarkStep() { return (double)(dwMarkSpeed*dwDelayStep)*0.001; };//--s=vt,[um]
};
/* 请务必将新增激光器信息插入到到Unknown当前位置之前，不可改变已存在的激光器顺序 */
typedef enum {
    emDEVICE_DPY = 0,
    emDEVICE_CO2,
    emDEVICE_IPG,
    emDEVICE_SPI,
    emDEVICE_UV,
    emDEVICE_PI,
    emDEVICE_UNKNOWN
}emLaser_t;

class IPlatform;
class TL_API IMarkDevice
{
public:
    //--IMarkDevice();
    ~IMarkDevice() {};

    virtual int32_t GetType()const = 0;
    virtual void GetName(char *sName)const = 0;

    virtual int32_t Open(IPlatform *pPlatform) = 0;
    virtual int32_t Close(void) = 0;

    virtual int32_t ShowDevOptionWnd(void) = 0;                //--显示激光器的配置窗口
    virtual int32_t ShowPenOptionWnd(IMarkPen *pThePen) = 0;   //--显示笔参数配置窗口
    virtual int32_t ShowPowerCalibWnd(void) = 0;               //--显示功率标定窗口

    virtual int32_t GetStatus(void) = 0;                   //--获取设备的状态信息 
    virtual int32_t GetDeviceCtrl(int iCmd, void *pArg) = 0;  
    virtual int32_t SetDeviceCtrl(int iCmd, void *pArg) = 0;

    virtual IMarkPen *CreatePen(void) = 0;
    virtual int32_t UpdateLaserPen(IMarkPen *pThePen) = 0;     //--设置标记笔

    virtual int32_t ActionBeforeMark(void) = 0;//--完成标记前的准备工作
    virtual int32_t ActionAfterMark(void) = 0; //--完成标记后的收尾工作
};

#endif
