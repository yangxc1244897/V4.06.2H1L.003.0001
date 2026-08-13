/**
******************************************************************************
* @文件  运动控制卡抽象层(XY+模型)
* @作者  
* @版本 V1.00.00
* @日期 2018-11-09
* @简述 XY联动，其他轴单动
* @email:jchq0101net@sina.com
******************************************************************************
* @说明
*
*
* 
******************************************************************************
*/
#ifndef H_TL_MOTION_DRIVER_H
#define H_TL_MOTION_DRIVER_H

#include <stdint.h>
#include <vector>
using namespace std;
#include "tlTeteDefines.h"

/*
* 本驱动使用的轴索引号、初始化时与控制器轴号建立映射关系
* 调用接口时，仅使用下面的轴索引号即可，用于隔离硬件接口
*/
typedef enum {
    emMOTION_AXIS_X=0,
    emMOTION_AXIS_Y,
    emMOTION_AXIS_Z,
    emMOTION_AXIS_R,
    emMOTION_AXIS_A,
    emMOTION_AXIS_B,
    emMOTION_AXIS_C,
    emMOTION_AXIS_D,
    emMOTION_AXIS_NUM
}emAxisID_t;
/** 轴配置信息 **/
typedef struct {
    bool IsEnable;       //--轴是否使能
    bool IsReverse;      //--是否反向
    int32_t iAxisNo;      //--[1:4/8]控制器上的轴号,并非本驱动使用的轴索引号    
    uint32_t dwFactor;   //--脉冲当量[p/mm]

    double dbLimitP;     //--正限位位置[mm]
    double dbLimitN;     //--负限位位置[mm]
    double dbMaxVel;     //--最大速度[mm/s]
}AxisCfg_t;
/* 轴运动参数 */
typedef struct {    
    double dbPos;     //--位置
    double dbVel;     //--速度
    double dbAcc;     //--加速度  
}AxisPrm_t;
/** 轴状态参数 **/
typedef struct {
    bool IsDrvWarn;       //--驱动告警
    bool IsErrLmt;        //--运动出错
    bool IsLmtP;          //--正向限位
    bool IsLmtN;          //--负向限位
    bool IsSStop;  //--平滑停止
    bool IsEStop;         //--IO紧急停止
    bool IsEnable;        //电机使能
    bool IsPrfMove;       //--规划运动
    bool IsReach;         //--电机到位标识
    bool IsWarn(void) { return (IsDrvWarn||IsErrLmt||IsLmtP||IsLmtN); }
}AxisStat_t;
/** 点到点运动参数 **/
typedef struct{
    double vel; //--目标速度 [mm/s]
    double acc; //--加速度   [mm/s^2]
    double dec; //--减速度   [mm/s^2]
    double smoothTime; //--平滑时间 [0,50],越大越平稳
}P2pPrm_t;
/** Jog运动参数 **/
typedef struct{
    double vel; //--目标速度 [mm/s]
    double acc; //--加速度   [mm/s^2]
    double dec; //--减速度   [mm/s^2]
    double smooth; //--平滑系数[0,1),值越大加减速越平稳
}JogPrm_t;
/** XY联动坐标系配置 **/
typedef struct {
    double maxVel;//--
    double maxAcc;
    int32_t iEvenTime;

    double dbLineVel;
    double dbLineAcc;
    double dbArcVel;
    double dbArcAcc;

    double dbOriginX;   //--原点
    double dbOriginY;
    double dbLtopX;     //--左下角
    double dbLtopY;
    double dbRbottomX;  //--右下角
    double dbRbottomY;
}MotionCoord_t;
/** 直线插补运动坐标点 **/
typedef struct {
	double dbX;
	double dbY;
}LinePoint_t;

/* 单轴运动控制 */
class TL_API IMotionAxis
{
public:
    virtual ~IMotionAxis(){};

    virtual int32_t GetMotionCtrl(int32_t iCmd, void *pArg) = 0;  //--获取运动控制配置信息
    virtual int32_t SetMotionCtrl(int32_t iCmd, void *pArg) = 0;  //--设置运动控制配置信息

    virtual int32_t GetStatus(AxisStat_t &axisStat) = 0;   //--获取指定轴的状态信息
    virtual int32_t GetPrfPrm(AxisPrm_t &axisParam) = 0;   //--获取轴规划器参数，位置、速度、加速度
    virtual int32_t GetEncPrm(AxisPrm_t &axisParam) = 0;   //--获取轴编码器参数，位置、速度、加速度
                                                       
    virtual int32_t GetJogParam(JogPrm_t &jogPrm) = 0;
    virtual int32_t SetJogParam(JogPrm_t jogPrm) = 0;
    virtual int32_t StartJog(void) = 0;
    virtual int32_t StopJog(void) = 0;

    virtual int32_t GetMoveParam(emAxisID_t emAxisID, P2pPrm_t &prm)const = 0;
    virtual int32_t SetMoveParam(emAxisID_t emAxisID, const P2pPrm_t &prm) = 0;
    virtual int32_t GetPos(double &pos) = 0;
    virtual int32_t MoveTo(double pos) = 0;

    virtual int32_t Stop(void) = 0;    //--急停
    virtual int32_t Home(uint32_t nMode) = 0; //--回零，指定回零模式
};
/* XY二轴联动控制 */
class TL_API IMotionAxisXY
{
public:
    virtual ~IMotionAxisXY(){};

    virtual int32_t GetMotionCtrl(int32_t iCmd, void *pArg) = 0;//--获取运动控制配置信息
    virtual int32_t SetMotionCtrl(int32_t iCmd, void *pArg) = 0;//--设置运动控制配置信息
                                                              //--状态
    virtual int32_t GetStatus(AxisStat_t &axisStat) = 0;//--获取指定轴的状态信息

    virtual int32_t GetPos(double &x,double &y) = 0;
    virtual int32_t MoveTo(double x,double y) = 0;
    virtual int32_t ArcTo(double x,double y,double cx,double cy) = 0;

    virtual int32_t Stop(void) = 0;    //--急停
    virtual int32_t Home(uint32_t nMode) = 0; //--回零，指定回零模式
};
/* XYZ三轴联动控制 */
class TL_API IMotionAxisXYZ
{
public:
    virtual ~IMotionAxisXYZ(){};

    virtual int32_t GetMotionCtrl(int32_t iCmd, void *pArg) = 0;//--获取运动控制配置信息
    virtual int32_t SetMotionCtrl(int32_t iCmd, void *pArg) = 0;//--设置运动控制配置信息
                                                               
    virtual int32_t GetStatus(AxisStat_t &axisStat) = 0;  //--获取指定轴的状态信息                                                                                                         

    virtual int32_t GetPos(double &x,double &y,double &z) = 0;
    virtual int32_t MoveTo(double x,double y,double z) = 0;

    virtual int32_t Stop(void) = 0;    //--急停
    virtual int32_t Home(uint32_t nMode) = 0; //--回零，指定回零模式
};
/* XYZA四轴联动控制 */
class TL_API IMotionAxisXYZA
{
public:
    virtual ~IMotionAxisXYZA(){};

    virtual int32_t GetMotionCtrl(int32_t iCmd, void *pArg) = 0;//--获取运动控制配置信息
    virtual int32_t SetMotionCtrl(int32_t iCmd, void *pArg) = 0;//--设置运动控制配置信息

    virtual int32_t GetStatus(AxisStat_t &axisStat) = 0;  //--获取指定轴的状态信息

    virtual int32_t GetPos(double &x,double &y,double &z,double &r) = 0;
    virtual int32_t MoveTo(double x,double y,double z,double r) = 0;

    virtual int32_t Stop(void) = 0;    //--急停
    virtual int32_t Home(uint32_t nMode) = 0; //--回零，指定回零模式
};
//--运动控制卡抽象层，用于统一各类型板卡
class TL_API IMotionDriver
{
public:
    //--IMotionDriver();
    virtual ~IMotionDriver(){};
  
public:
    virtual int32_t LoadConfig(const char *sFileName) = 0;    //--载入配置文件
    virtual int32_t SaveConfig(const char *sFileName) = 0;    //--配置保存至文件
    virtual int32_t Open(void) = 0;                           //--打开、复位、加载配置、启用轴    
    virtual int32_t Close(void) = 0;                          //--关闭轴、复位，关闭
	virtual int32_t ShowMotionWnd(void) = 0;                  //--显示运动控制主界面
    virtual int32_t ShowAxisWnd(emAxisID_t emAxisID) = 0;     //--显示单个轴信息配置界面
    virtual int32_t GetMotionCtrl(int32_t iCmd, void *pArg) = 0;//--获取运动控制配置信息
    virtual int32_t SetMotionCtrl(int32_t iCmd, void *pArg) = 0;//--设置运动控制配置信息
    virtual IMotionAxis *GetAxis(emAxisID_t emAxisID) = 0;
    virtual IMotionAxisXY *GetAxes(emAxisID_t emAxisX,emAxisID_t emAxisY) = 0;

    virtual int32_t GetStatus(AxisStat_t &axisStat) = 0;                //--获取状态信息
    virtual int32_t EStop(emAxisID_t emAxisID) = 0;                     //--指定轴急停
    virtual int32_t EStop(void) = 0;                                    //--所有轴急停
    virtual int32_t GoHome(emAxisID_t emAxisID,uint32_t nMode = 0) = 0; //--指定轴回零
    virtual int32_t GoHome(uint32_t nMode) = 0;                         //--所有轴回零，指定回零模式

    virtual int32_t GetInputPin(uint32_t iPin, uint32_t &nBit) = 0;     //--获取指定输入引脚的状态
    virtual int32_t GetOutputPin(uint32_t iPin, uint32_t &nBit) = 0;    //--获取指定输出引脚的状态
    virtual int32_t SetOutputPin(uint32_t iPin, uint32_t nBit) = 0;     //--设定指定输出引脚的状态

};

#endif
