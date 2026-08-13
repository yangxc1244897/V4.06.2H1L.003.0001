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
#ifndef H_TL_MARK_ENGINE_H
#define H_TL_MARK_ENGINE_H

#include "tlMarkDefine.h"
#include "tlMarkDriver.h"
#include "tlMarkDevice.h"
#include "tlMarkCalib.h"


/* 标记类型选择 按位控制*/
//--#define ME_NORMAL_MARK     ((uint32_t)0x0001<<0)    /* 使能普通标记 */
#define ME_EN_MARK_SCAN       ((uint32_t)0x0001<<1)    /* 使能扫描标记 */
#define ME_EN_MARK_NETWORK    ((uint32_t)0x0001<<2)    /* 使能网络标记 */
#define ME_EN_MARK_ONLINE     ((uint32_t)0x0001<<3)    /* 使能在线标记 */
#define ME_EN_MARK_PEDAL      ((uint32_t)0x0001<<4)    /* 使能脚踏标记 */
/* 文档标记方式选择 */
#define ME_EN_MARK_SEL    ((uint32_t)0x0001<<5)    /* 标记被选中对象 */
#define ME_EN_MARK_ALL    ((uint32_t)0x0001<<6)    /* 标记所有文档 */

/* 标记方式选择 */
#define ME_SEL_MARK_PREVIEW    ((uint32_t)0x0001)    /* 标记预览 */
#define ME_SEL_MARK_TEST       ((uint32_t)0x0002)    /* 标记测试 */
#define ME_SEL_MARK_NORMAL     ((uint32_t)0x0003)    /* 正常标记 */

//--输入信号,即外部触发信号,驱动实体类返回IO的实际值,在引擎类中再做高低有效电平的处理
typedef enum {
    emSIGN_IN_HARDWORK = 0,    //--硬件开工信号 
    emSIGN_IN_PEDALSIGN = 1 ,     //--脚踏信号有效/全层标记开始信
    emSIGN_IN_SYSWARN = 2,       //--系统告警信号
    emSIGN_IN_LINEWARN = 3,      //--流水线告警信号    
    emSIGN_IN_WATERWARN = 4,     //--水冷机告警信号
    emSIGN_IN_LASERWARN = 5,     //--激光器电路告警
    emSIGN_IN_TEMPWARN = 6,      //--激光器温度告警
    emSIGN_IN_SHUTTER = 7,       //--Shutter状态信号
    emSIGN_IN_LASERLIGNT = 8,    //--激光点灯信号
    emSIGN_IN_TRIGMARK = 9,    //--外部触发图形标记开始信号
    emSIGN_IN_MAX
}emSignIn_t;
//--输出信号,驱动实体类返回IO的实际值,在引擎类中再做高低有效电平的处理
typedef enum {
    emSIGN_OUT_SOFTWORK = 0,    //--软件开工信号
    emSIGN_OUT_LASERON = 1,
    emSIGN_OUT_AMPLIFIER = 2,   //--激光器放大器开光
    emSIGN_OUT_ALLLAYER = 3,    //--全层标记状态输出信号
    emSIGN_OUT_REDLIGHT = 4,    //--红光输出
    emSIGN_OUT_LOADDONE = 5,    //--图层加载完毕
    emSIGN_OUT_MOTORDIR1 = 6,   //--电机方向输出1
    emSIGN_OUT_STOP = 7,        //--激光器紧急停止出光
    emSIGN_OUT_MOTORDIR0 = 8,   //--电机方向输出0
    emSIGN_OUT_MOTORPULSE = 9,  //--电机脉冲方向0
    emSIGN_OUT_MAX
}emSignOut_t;

/* 标记引擎状态信息集 */
#define  SYS_STAT_OK              ((uint32_t)0x0000)  
#define  SYS_STAT_SYS_BUSY        ((uint32_t)0x0001)    /* 系统忙 正在标记或者通信等 */
#define  SYS_STAT_SYS_WARN        ((uint32_t)0x0002)    /* 系统告警 */
#define  SYS_STAT_LINE_WARN       ((uint32_t)0x0004)    /* 流水线告警 */
#define  SYS_STAT_TEMP_WARN       ((uint32_t)0x0008)    /* 温度告警 */
#define  SYS_STAT_ELEC_WARN       ((uint32_t)0x0010)    /* 电路告警 */
#define  SYS_STAT_WATER_WARN      ((uint32_t)0x0020)    /* 水冷机告警 */
#define  SYS_STAT_LASER_WARN      ((uint32_t)0x0040)    /* 激光器告警 */

#define  SYS_MARK_IDLE       ((uint32_t)0x0040)    /* 标记空闲 */
#define  SYS_MARK_BUSY       ((uint32_t)0x0040)    /* 正在标记 */
#define  SYS_MARK_STOP       ((uint32_t)0x0040)    /* 标记结束 */

/* 标记状态类型 */
enum{
    emMARK_STAT_IDLE = 0,  /* 空闲，数据缓存为空 */
    emMARK_STAT_BUSY,      /* 正在标记 */
    emMARK_STAT_STOP,      /* 停止标记 */
    emMARK_STAT_ERROR      /* 板卡出错 */
};
/* 标记引擎控制命令集 */
enum emMarkEngineCmd{
    emMECMD_DRIVER_TYPE=0,   /* 控制卡 */
    emMECMD_HEADER_TYPE,     /* 扫描头 */
    emMECMD_LASER_TYPE,      /* 激光器 */
    emMECMD_MCTRL_TYPE,    /* 主控箱 */
    emMECMD_DIGIT_IN,      /* 读取16bit数据输入端口命令 */
    emMECMD_DIGIT_OUT,      /* 设置16bit数据输出端口命令 */
	emMECMD_LIMIT_X,
	emMECMD_LIMIT_Y,
    emMECMD_MARK_TURN
};

enum {
    emSIGN_HIGH = 0,  //--高电平信号
    emSIGN_LOW,       //--低电平信号
    emSIGN_RISE,      //--上升沿信号
    emSIGN_FALL,      //--下降沿信号
};
typedef struct ST_PortSign {
    bool  IsEnable;    //--是否启用
    int   iID;         //--端口编号,跟具体功能关联
    int   iPin;        //--端口号
    int   iSign;       //--信号类型    
    int   iPeriod;     //--信号为脉冲时的脉宽[ms]
}TPortSign_t;

typedef struct _MarkTurn_t {
    double dx, dy;  //-偏移量
    double cx, cy, angle;//--旋转中心及角度
    double fx, fy;  //--缩放因子
}xMarkTurn_t;

class TL_API IMarkEngine
{
public:
    //--IMarkEngine();
    virtual ~IMarkEngine(){};
    virtual int32_t LoadConfig(const char *sFile) = 0;
    virtual int32_t SaveConfig(const char *sFile) = 0;
    virtual int32_t Open(IPlatform *pPlatform)=0;
    virtual int32_t Close(void)=0;
    virtual int32_t GetStatus(void)=0;
    virtual int32_t GetMarkStat(void) = 0;
    virtual int32_t GetEngineCtrl(int iCmd, void *pArg)=0;
    virtual int32_t SetEngineCtrl(int iCmd, void *pArg)=0;
    virtual bool IsActive(void) = 0;

    virtual void LaserOn(double px,double py) = 0;
    virtual void LaserOff(void) = 0;
    
    virtual int32_t MarkBegin(void)=0;
    virtual int32_t MarkEnd(void) = 0;
    virtual int32_t StopMark(void) = 0;

    virtual bool UpdatePen(IMarkPen *pThePen)=0;
    virtual int32_t PathBegin(double x,double y) = 0;
    virtual int32_t PathBegin(void) = 0;
    virtual int32_t PathEnd(void) = 0;   
    virtual int32_t LightTo(double px, double py) = 0;
    virtual int32_t MoveTo(double px,double py) = 0;
    virtual int32_t LineTo(double px,double py) = 0;
    virtual int32_t PointOn(double px,double py) = 0;    
    virtual int32_t MarkLine(double x1,double y1,double x2,double y2) = 0;
    virtual int32_t MarkPolyline(DPoint_t *lppt,uint32_t dwCount) = 0;    
    virtual int32_t MarkRectangle(double x1,double y1,double x2,double y2) = 0;
    virtual int32_t MarkCircle(double cx,double cy,double radius) = 0;
    virtual int32_t MarkArc(double cx,double cy,double radius,double startAng,double endAng,bool IsClockWise) = 0;
public:    
    virtual bool IsSignInActive(emSignIn_t emSign) = 0;//--判断输入信号是否处于有效状态
    virtual void ActiveOutputSign(emSignOut_t emSign,bool IsActive) = 0;//--设置输出信号是否有效
};

#endif
