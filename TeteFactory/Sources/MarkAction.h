/**
  ******************************************************************************
  * @文件   
  * @作者 jchq 
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

#ifndef H_MARK_ACTION_H
#define H_MARK_ACTION_H

#include "tlPlatform.h"
/*----  流程动作类型标识 ---------------------------------------------*/
enum {
    emACTION_INPUT = 0,  /* IO输入       */
    emACTION_OUTPUT,     /* IO输出       */
    emACTION_DELAY,      /* 延时等待       */
    emACTION_JUMP,       /* 跳转动作       */
    emACTION_MARKDOC,    /* 标记文档       */
    emACTION_MOTION,     /* 电机运动       */
    emACTION_VISION      /* CCD视觉      */
};
class IMarkAction
{
protected:
    uint32_t  m_nActionID;    //--动作编号
    uint32_t  m_nActionCode;  //--动作编码
    bool      m_IsActive;     //--动作执行期间为true，否则为false
    bool      m_IsEnable;
    char      m_sCaption[512];
    IPlatform *m_pPlatform;
public:
    //--IMarkAction() {};
    virtual ~IMarkAction() {};

    void Init(IPlatform *pPlatform){ m_pPlatform = pPlatform; }
    void SetID(uint32_t nID) { m_nActionID = nID; }
    uint32_t GetID(void) { return m_nActionID; }    
    /*string_t GetCaption(void) { return m_sCaption;}
    void SetCaption(string_t sValue) { m_sCaption=sValue; }*/
    void SetCaption(const char *sName) { strcpy_s(m_sCaption,512,sName); }
    virtual const char *GetCaption(void) const { return m_sCaption; }
    bool IsActive(void){ return m_IsActive; }
    virtual void EnableAction(bool IsEnable){ m_IsEnable = IsEnable; };

    virtual int32_t GetActionType(void) = 0;      //--获取动作类型
    virtual int32_t ActiveAction(void) = 0;       //--执行动作


};
/*--------------------------------------------------------------*/
/************* 边沿触发动作，等待直到信号有效时方可继续 *************/
/*class CActionTrigger : public IMarkAction
{
public:
    int32_t  iSign;       //--上升沿/下降沿
    uint32_t dwPin;      //--32bit引脚控制位,仅其中的一位有效
public:
    CActionTrigger();
    virtual ~CActionTrigger() {};

    virtual int32_t GetActionType() { return emACTION_TRIGGER; };
    virtual int32_t ActiveAction(void);
};*/
/************* 电平输入动作*************/
/*---------- 输入输出动作信号类型 ------------------------------------*/
enum {
    emACTION_SIGN_HIGH = 0,  //--高电平信号
    emACTION_SIGN_LOW,       //--低电平信号
    emACTION_SIGN_RISE,      //--上升沿信号
    emACTION_SIGN_FALL,      //--下降沿信号
};
class CActionInput : public IMarkAction
{
public:
    int32_t  iSign;       //--[高电平/低电平]/上升沿/下降沿
    uint32_t dwPins;      //--32bit引脚控制位,1:使能,0:禁用；0x0003:bit1,bit0使能
public:
    CActionInput();
    virtual ~CActionInput() {};

    int32_t GetActionType() { return emACTION_INPUT; };
    int32_t ActiveAction(void);
};
/************* 输出动作*************/
enum {
    emACTION_OUT_HIGH = 0,  //--高电平信号
    emACTION_OUT_LOW,       //--低电平信号
    emACTION_OUT_RISE,      //--上升沿信号
    emACTION_OUT_FALL,      //--下降沿信号
};
class CActionOutput : public IMarkAction
{
public:
    int32_t  iSign;       //--高电平/低电平
    uint32_t dwPins;      //--32bit引脚控制位,1:使能,0:禁用；0x0003:bit1,bit0使能
                          //  脉冲信号时仅能使能一位
public:
    CActionOutput();
    virtual ~CActionOutput() {};

    int32_t GetActionType() { return emACTION_OUTPUT; };
    int32_t ActiveAction(void);
private:
    IPlatform *m_pPlatform;
};
/************* 脉冲输出动作*************/
/*class CActionPulse : public IMarkAction
{
public:
    int32_t  iSign;       //--高脉冲/低脉冲
    uint32_t dwPin;      //--32bit引脚控制位,1:使能,0:禁用；0x0003:bit1,bit0使能
                          //  脉冲信号时仅能使能一位
    uint32_t dwPeriod;    //--脉冲有效周期
public:
    CActionPulse();
    virtual ~CActionPulse() {};

    virtual int32_t GetActionType() { return emACTION_PULSE; };
    virtual int32_t ActiveAction(void);
};*/

/************* 延时等待动作*************/
class CActionDelay : public IMarkAction
{
public:
    uint32_t dwmin; //--延时周期
    uint32_t dwsec;   //--延时周期
    uint32_t dwmsec;  //--延时周期
    uint32_t dwusec;  //--延时周期
public:
    CActionDelay();
    virtual ~CActionDelay() {};

    int32_t GetActionType() { return emACTION_DELAY; };
    int32_t ActiveAction(void);
};

/************* 跳转到指定动作*************/
class CActionJump : public IMarkAction
{
public:
    uint32_t dwJumpID; //--待跳转动作的编号
public:
    CActionJump();
    virtual ~CActionJump() {};

    int32_t GetActionType() { return emACTION_JUMP; };
    int32_t ActiveAction(void);
};

/************* 文档标记动作*************/
class CActionMarkDoc : public IMarkAction
{
public:
    int32_t iDocID;  
    int32_t iMarkTimes; //-需标记次数,小于0时持续标记
public:
    CActionMarkDoc();
    virtual ~CActionMarkDoc() {};

    virtual void EnableAction(bool IsEnable);

    int32_t GetActionType() { return emACTION_MARKDOC; };
    int32_t ActiveAction(void);
};
/************* 电机运动动作*************/
class CActionMotion : public IMarkAction
{
public:
    double motionpos[8];  //--运动点位的绝对坐标  
public:
    CActionMotion();
    virtual ~CActionMotion() {};

    int32_t GetActionType() { return emACTION_MOTION; };
    int32_t ActiveAction(void);    
};
/************* 相机检测动作*************/
class CActionVision : public IMarkAction
{
public:
    double dbOffsetX;    //--X坐标偏差
    double dbOffsetY;    //--Y坐标偏差
    double dbRotateA;    //--角度偏差
public:
    CActionVision();
    virtual ~CActionVision() {};

    int32_t GetActionType() { return emACTION_VISION; };
    int32_t ActiveAction(void);
};


#endif
