/**
  ******************************************************************************
  * @文件   
  * @作者 jchq 
  * @版本 V1.00.00
  * @日期 2017-04-11
  * @简述 文件读写,文档管理  
  * @email:jchq0101net@sina.com
  ******************************************************************************
  * @说明
  *
  *
  * 
  ******************************************************************************
  */ 
#include "stdafx.h"
#include "..\stdafx.h"
#include "MarkAction.h"
#include "Common\LogServer.h"
#include <vector>
using namespace std;
#include "MarkPlatform.h"

/*---------------------------------------------------*/
CActionInput::CActionInput():iSign(0),dwPins(0) 
{
    strcpy(m_sCaption,"端口输入");
}
int32_t CActionInput::ActiveAction(void)
{
    IMarkDriver *pTheDriver = CMarkPlatform::Instance()->MarkDriver();
    /*if (pTheDriver->GetDriverType() == emDRIVER_UNKNOWN)
        return 0;*/
    CMarkPlatform::Instance()->Message(emMSG_TYPE_INFO, "%s 开始",m_sCaption);
    uint32_t nPins0,nPins1;
    if(emACTION_SIGN_HIGH==iSign)
    {
        while (1)
        {
            pTheDriver->ReadExtPortPins(nPins0);
            //--延时消抖
            CMarkPlatform::Instance()->Delay(10);
            pTheDriver->ReadExtPortPins(nPins1);
            if ( (nPins0 == nPins1) && (dwPins==(nPins0&dwPins)) )
            {
                break;
            }
            if(!m_IsEnable)return -1;
        }
    }
    else if(emACTION_SIGN_LOW==iSign)
    {
        while (1)
        {
            pTheDriver->ReadExtPortPins(nPins0);
            nPins0 = ~nPins0;
            //--延时消抖
            CMarkPlatform::Instance()->Delay(10);
            pTheDriver->ReadExtPortPins(nPins1);
            nPins1 = ~nPins1;

            if ( (nPins0 == nPins1) && (dwPins==(nPins0&dwPins)) )
            {
                break;
            }
            if(!m_IsEnable)return -1;
        }
    }
    CMarkPlatform::Instance()->Message(emMSG_TYPE_INFO, "%s 完成",m_sCaption);
    
    return 0;
}
/*---------------------------------------------------*/
CActionOutput::CActionOutput():iSign(emACTION_SIGN_LOW),dwPins(0)
{
    strcpy(m_sCaption,"端口输出");
}
int32_t CActionOutput::ActiveAction(void)
{
    IMarkDriver *pTheDriver = CMarkPlatform::Instance()->MarkDriver();
    uint32_t nPins = dwPins;
    if(emACTION_SIGN_HIGH==iSign)
    {
        pTheDriver->WriteExtPortPins(nPins);
    }
    else if(emACTION_SIGN_LOW==iSign)
    {
        pTheDriver->WriteExtPortPins(nPins);
    }
    CMarkPlatform::Instance()->Message(emMSG_TYPE_INFO, "%s 完成",m_sCaption);

    return 0;
}
/*---------------------------------------------------*/
CActionDelay::CActionDelay(): dwmin(0),dwsec(0),dwmsec(500),dwusec(0) 
{
    strcpy(m_sCaption,"延时等待");
}
int32_t CActionDelay::ActiveAction(void)
{
    uint32_t secs = dwmin * 60 + dwsec;
    secs *= 4;
    CMarkPlatform::Instance()->Message(emMSG_TYPE_INFO, "%s 开始",m_sCaption);
	for (uint32_t s = 0; s < secs; s++)
    {   
        CMarkPlatform::Instance()->Delay(250);  //--1/4s
        if(!m_IsEnable)
            return -1;
    }
    CMarkPlatform::Instance()->Delay(dwmsec);
    if(!m_IsEnable)
        return -1;
    CMarkPlatform::Instance()->Message(emMSG_TYPE_INFO, "%s 完成",m_sCaption);
    
    return 0;
}
/*---------------------------------------------------*/
CActionJump::CActionJump(): dwJumpID(0)
{
    strcpy(m_sCaption,"跳转");
}
int32_t CActionJump::ActiveAction(void)
{
    return 0;
}
/*---------------------------------------------------*/
CActionMarkDoc::CActionMarkDoc():iDocID(0),iMarkTimes(1) 
{
    strcpy(m_sCaption,"标记文档");
}
void CActionMarkDoc::EnableAction(bool IsEnable)
{
    m_IsEnable = IsEnable;
    if(!IsEnable)
    {
        CMarkPlatform::Instance()->MarkEngine()->StopMark();
    }
}
int32_t CActionMarkDoc::ActiveAction(void)
{
    /*IGraphEngine *pTheFile = CMarkPlatform::Instance()->GraphEngine();
    IMarkEngine *pTheEngine = CMarkPlatform::Instance()->MarkEngine();
    CMarkPlatform::Instance()->Message(emMSG_TYPE_INFO, "%s 开始",m_sCaption);
    pTheEngine->MarkBegin();
    for (int i = 0; i < iMarkTimes; i++)
    {
        if (0!=pTheFile->MarkDoc(iDocID))
			return -1;
        if(!m_IsEnable)
			return -1;
    }
    pTheEngine->MarkEnd();
    CMarkPlatform::Instance()->Message(emMSG_TYPE_INFO, "%s 完成",m_sCaption);*/
    return 0;
}
/*---------------------------------------------------*/
CActionMotion::CActionMotion() 
{
    for (int i = 0; i < 8; i++)
        motionpos[i] = 0.0;
    strcpy(m_sCaption,"运动控制");
}
int32_t CActionMotion::ActiveAction(void)
{
    return 0;
}
/*---------------------------------------------------*/
CActionVision::CActionVision() 
{
    strcpy(m_sCaption,"机器视觉");
    dbOffsetX = 0.0;
    dbOffsetY = 0.0;
    dbRotateA = 0.0;
}
int32_t CActionVision::ActiveAction(void)
{
    return 0;
}




