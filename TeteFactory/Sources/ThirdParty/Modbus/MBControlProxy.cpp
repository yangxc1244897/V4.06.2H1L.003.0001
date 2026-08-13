/**
  ******************************************************************************
  * @文件     
  * @作者  
  * @版本 V1.00.00
  * @日期 2017-04-17
  * @简述   
  ******************************************************************************
  * @说明
  *
  *
  * 
  ******************************************************************************
  */
#include "stdafx.h"
#include "MBControlProxy.h"
#include "tete_laser.h"


//--#include "scanlabRTC5.h"
//--#include "MarkFile.h"

//---------------------------------------------------------------------------

CMBControlProxy *g_pCMBControlProxy;

//------------------------------------------------------------------------------
CMBControlProxy::CMBControlProxy()
{
    /*
    m_uCoilsBits = NULL;
    m_wInputRegs = NULL;
    m_wHoldRegs = NULL;
    */
    /*--
    if(FileExists("modbusfile.mbf"))
        loadFromFile("modbusfile.mbf");
        */
    m_pTheLog = new CCLog;
}
//------------------------------------------------------------------------------
CMBControlProxy::~CMBControlProxy()
{
    delete m_pTheLog;
    //--saveIntoFile("modbusfile.mbf");
}
//------------------------------------------------------------------------------
bool CMBControlProxy::saveIntoFile(CString sFileName)
{
#if 0
    FILE* pFile;
    pFile = fopen(sFileName.c_str(), "wb");

    // 保存32个字节的文件版本号
    char cVer[32];
    memset(cVer, 0, 32);

    strcpy(cVer, TETE_MODBUS_VERSION);
    //
    fwrite(cVer, 32 * sizeof(char), 1, pFile);

    fwrite(&m_stMBMapInfoSets, sizeof(MBMapInfoSets_t), 1, pFile);
#endif    
    return true;
}
//------------------------------------------------------------------------------
bool CMBControlProxy::loadFromFile(CString sFileName)
{
#if 0
    FILE* pFile;
    pFile = fopen(sFileName.c_str(), "rb");

    char cVer[32];
    memset(cVer, 0, 32);
    fread(cVer, 32* sizeof(char), 1, pFile);
    fread(&m_stMBMapInfoSets, sizeof(MBMapInfoSets_t), 1, pFile);
#endif
    return true;
}
//------------------------------------------------------------------------------
bool CMBControlProxy::writeUnitInfo(uint32_t baseAddr,uint32_t dwOffset,
                                    uint8_t *pUnitBuff,int iSize)
{
    uint32_t dwIndex=dwOffset;
    /*
    switch(baseAddr)
    {
    case 0:
    case 1000:
    case 2000:
        for(i=0;i<iSize;i++)
        {
            m_uCoilsBits[dwIndex++] = pUnitBuff[i];
        }
        break;
    case 30000:
    case 31000:
    case 32000:
        for(i=0;i<iSize;i+=2)
        {
            m_wInputRegs[dwIndex++] = (pUnitBuff[i]<<8)|pUnitBuff[i+1];
        }
        break;  
    case 40000:
    case 41000:
    case 42000:
        for(i=0;i<iSize;i+=2)
        {
            m_wHoldRegs[dwIndex++] = (pUnitBuff[i]<<8)|pUnitBuff[i+1];
        }
        break;
    default:break;
    }
    */
    return true;
}
//------------------------------------------------------------------------------
/*
baseAddr:
dwOffset:
*/
bool CMBControlProxy::readUnitInfo(uint32_t baseAddr,uint32_t dwOffset,
                                     uint8_t *pUnitBuff,int iSize)
{
    uint32_t dwIndex=dwOffset;
    /*
    switch(baseAddr)
    {
    case 0:
    case 1000:
    case 2000:
        for(i=0;i<iSize;i++)
        {
            pUnitBuff[i] = m_uCoilsBits[dwIndex++];
        }
        break;
    case 30000:
    case 31000:
    case 32000:
        for(i=0;i<iSize;i+=2)
        {
            pUnitBuff[i] = m_wInputRegs[dwIndex]>>8;
            pUnitBuff[i+1] = m_wInputRegs[dwIndex];
            dwIndex++;
        }
        break;  
    case 40000:
    case 41000:
    case 42000:
        for(i=0;i<iSize;i+=2)
        {
            pUnitBuff[i] = m_wHoldRegs[dwIndex]>>8;
            pUnitBuff[i+1] = m_wHoldRegs[dwIndex];
            dwIndex++;
        }
        break;
    default:break;
    }
    */
    return true;
}
//------------------------------------------------------------------------------
bool CMBControlProxy::writeUnitInfo(uint32_t baseAddr,uint32_t dwOffset,
                                                uint16_t *pUnitBuff,int iSize)
{
    uint32_t dwIndex=dwOffset;
    /*
    switch(baseAddr)
    {
    case 0:
    case 1000:
    case 2000:
        for(i=0;i<iSize;i++)
        {
            m_uCoilsBits[dwIndex++] = pUnitBuff[i]>>8;
            m_uCoilsBits[dwIndex++] = pUnitBuff[i];
        }
        break;
    case 30000:
    case 31000:
    case 32000:
        for(i=0;i<iSize;i++)
        {
            m_wInputRegs[dwIndex++] = pUnitBuff[i];
        }
        break;
    case 40000:
    case 41000:
    case 42000:
        for(i=0;i<iSize;i++)
        {
            m_wHoldRegs[dwIndex++] = pUnitBuff[i];
        }
        break;
    default:break;
    }
    */
    return true;
}
bool CMBControlProxy::writeCoilsUnitInfo(uint32_t iAddr,uint8_t *pUnitBuff,int iSize)
{
    int i;
    uint32_t dwIndex=iAddr;

    for(i=0;i<iSize;i++)
    {
        //--m_uCoilsBits[dwIndex++] = pUnitBuff[i]>>8;
        //--m_uCoilsBits[dwIndex++] = pUnitBuff[i];
    }

    return true;
}
bool CMBControlProxy::writeInputRegsUnitInfo(uint32_t iAddr,uint16_t *pUnitBuff,int iSize)
{
    int i;
    uint32_t dwIndex=iAddr;

    for(i=0;i<iSize;i++)
    {
        //--m_wInputRegs[dwIndex++] = pUnitBuff[i]>>8;
        //--m_wInputRegs[dwIndex++] = pUnitBuff[i];
    }

    return true;
}
bool CMBControlProxy::writeHoldRegsUnitInfo(uint32_t iAddr,uint16_t *pUnitBuff,int iSize)
{
    int i;
    uint32_t dwIndex=iAddr;

    for(i=0;i<iSize;i++)
    {
        //--m_wHoldRegs[dwIndex++] = pUnitBuff[i]>>8;
        //--m_wHoldRegs[dwIndex++] = pUnitBuff[i];
    }

    return true;
}
//------------------------------------------------------------------------------
bool CMBControlProxy::readUnitInfo(uint32_t baseAddr,uint32_t dwOffset,uint16_t *pUnitBuff,int iSize)
{
    uint32_t dwIndex=dwOffset;
    /*
    switch(baseAddr)
    {
    case 0:
    case 1000:
    case 2000:
        for(i=0;i<iSize;i++)
        {
            pUnitBuff[i] = m_uCoilsBits[dwIndex];
            pUnitBuff[i] <<= 8;
            pUnitBuff[i] |= m_uCoilsBits[dwIndex+1];
            dwIndex += 2;
        }
        break;
    case 30000:
    case 31000:
    case 32000:
        for(i=0;i<iSize;i++)
        {
            pUnitBuff[i] = m_wInputRegs[dwIndex++];
        }
        break;  
    case 40000:
    case 41000:
    case 42000:
        for(i=0;i<iSize;i++)
        {
            pUnitBuff[i] = m_wHoldRegs[dwIndex++];
        }
        break;
    default:break;
    }
    */
    return true;
}
bool CMBControlProxy::readCoilsUnitInfo(uint32_t dwOffset,uint8_t *pUnitBuff,int iSize)
{
    int i;
    uint32_t dwIndex=dwOffset;

    for(i=0;i<iSize;i++)
    {
        //--pUnitBuff[i] = m_uCoilsBits[dwIndex++];
    }

    return true;
}
bool CMBControlProxy::readInputRegsUnitInfo(uint32_t dwOffset,uint16_t *pUnitBuff,int iSize)
{
    int i;
    uint32_t dwIndex=dwOffset;

    for(i=0;i<iSize;i++)
    {
        //--pUnitBuff[i] = m_wInputRegs[dwIndex++];
    }

    return true;
}
bool CMBControlProxy::readHoldRegsUnitInfo(uint32_t dwOffset,uint16_t *pUnitBuff,int iSize)
{
    int i;
    uint32_t dwIndex=dwOffset;

    for(i=0;i<iSize;i++)
    {
        //--pUnitBuff[i] = m_wHoldRegs[dwIndex++];
    }

    return true;
}
//------------------------------------------------------------------------------
/*
uLaserSel:
     TETE_LASER_I  :
     TETE_LASER_II :
     TETE_LASER_III:
     TETE_LASER_IV :
IsAction:  
    true : Action
    false: Donot Action
*/
bool CMBControlProxy::GetSystemLaserAction(uint8_t uLaserSel)
{
    //TODO: Add your source code here
    bool IsAction;    
    switch(uLaserSel)
    {
    case TETE_LASER_I:
        IsAction = m_stMBMapInfoSets.stCoilsInfo.stSystemCoilsInfo.System_Laser_I_Action!=0?TRUE:FALSE;
        break;
    case TETE_LASER_II:
        IsAction = m_stMBMapInfoSets.stCoilsInfo.stSystemCoilsInfo.System_Laser_II_Action!=0?TRUE:FALSE;
        break;
    case TETE_LASER_III:
        IsAction = m_stMBMapInfoSets.stCoilsInfo.stSystemCoilsInfo.System_Laser_III_Action!=0?TRUE:FALSE;
        break;
    case TETE_LASER_IV:
        IsAction = m_stMBMapInfoSets.stCoilsInfo.stSystemCoilsInfo.System_Laser_IV_Action!=0?TRUE:FALSE;
        break;
    default:break;
    }

    m_pTheLog->WriteLog("TETE MODBUS:> Laser%d GetSystemLaserAction = %d\n",uLaserSel,IsAction);
    
    return IsAction;
}
/*
功能描述：激光器自动打标
    1、载入模板文件
    2、下载并载入MAP文件
    3、标记MAP文档
*/
bool CMBControlProxy::SetSystemLaserAction(uint8_t uLaserSel,bool IsAction)
{    
    if(false==m_stMBMapInfoSets.stCoilsInfo.stSystemCoilsInfo.System_AutoFlag)
        return false;
	m_pTheLog->WriteLog("TETE MODBUS:> Laser%d SetSystemLaserAction = %d\n",uLaserSel,IsAction);
    switch(uLaserSel)
    {
    case TETE_LASER_I:
        m_stMBMapInfoSets.stCoilsInfo.stSystemCoilsInfo.System_Laser_I_Action = IsAction;
        if(IsAction)
        {
            //-g_pTheMarkFile->GetDoc(2);
			m_pTheLog->WriteLog("TETE MODBUS:> MarkDoc2");
            //--MainForm->btnMarkClick(NULL);
            //--g_pTheScanlabRTC5->MarkDoc(RTC5_LASER_I,g_pTheMarkFile->GetDoc(2));
            m_stMBMapInfoSets.stCoilsInfo.stSystemCoilsInfo.System_Laser_I_Action = false;
            m_stMBMapInfoSets.stCoilsInfo.stSystemCoilsInfo.System_AutoFlag = false;
        }
        break;
    case TETE_LASER_II:
        m_stMBMapInfoSets.stCoilsInfo.stSystemCoilsInfo.System_Laser_II_Action = IsAction;
        if(IsAction)
        {
            //--g_pTheMarkFile->GetDoc(3);
			m_pTheLog->WriteLog("TETE MODBUS:> MarkDoc3");
            //--MainForm->btnMarkClick(NULL);
            //--g_pTheScanlabRTC5->MarkDoc(RTC5_LASER_II,g_pTheMarkFile->GetDoc(3));
            m_stMBMapInfoSets.stCoilsInfo.stSystemCoilsInfo.System_Laser_II_Action = false;
            m_stMBMapInfoSets.stCoilsInfo.stSystemCoilsInfo.System_AutoFlag = false;
        }
        break;
    case TETE_LASER_III:
        m_stMBMapInfoSets.stCoilsInfo.stSystemCoilsInfo.System_Laser_III_Action = IsAction;
        break;
    case TETE_LASER_IV:
        m_stMBMapInfoSets.stCoilsInfo.stSystemCoilsInfo.System_Laser_IV_Action = IsAction;
        break;
    default:break;
    }

    return true;
}
//------------------------------------------------------------------------------
/*
功能描述：判断激光器是否准备就绪
  判断内容如下：
      激光器工作状态是否正常
      运动轴是否正常
*/
bool CMBControlProxy::GetSystemLaserReady(uint8_t uLaserSel)
{
    //TODO: Add your source code here
    bool IsAction=false;    
    switch(uLaserSel)
    {
    case TETE_LASER_I:
        //--IsAction = m_stMBMapInfoSets.stCoilsInfo.stSystemCoilsInfo.System_Laser_I_Ready!=0?TRUE:FALSE;
        break;
    case TETE_LASER_II:
        //--IsAction = m_stMBMapInfoSets.stCoilsInfo.stSystemCoilsInfo.System_Laser_II_Ready!=0?TRUE:FALSE;
        break;
    case TETE_LASER_III:
        //--IsAction = m_stMBMapInfoSets.stCoilsInfo.stSystemCoilsInfo.System_Laser_III_Ready!=0?TRUE:FALSE;
        break;
    case TETE_LASER_IV:
        //--IsAction = m_stMBMapInfoSets.stCoilsInfo.stSystemCoilsInfo.System_Laser_IV_Ready!=0?TRUE:FALSE;
        break;
    default:break;
    }

	m_pTheLog->WriteLog("TETE MODBUS:> Laser%d GetSystemLaserReady = %d\n",uLaserSel,IsAction);
    return true;
}
//------------------------------------------------------------------------------
/*
功能描述：系统对时
*/
bool CMBControlProxy::SetSystemTimeTick(bool IsAction)
{
    //TODO: Add your source code here
	SYSTEMTIME stSysTime;
    stSysTime.wYear   = m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_RTC_Year;
    stSysTime.wMonth  = m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_RTC_Month;
    stSysTime.wDay    = m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_RTC_Date;
    stSysTime.wHour   = m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_RTC_Hour;
    stSysTime.wMinute = m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_RTC_Min;
    stSysTime.wSecond = m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_RTC_Sec;
	GetLocalTime(&stSysTime);
	//--GetSystemTime(&stSysTime);

	m_pTheLog->WriteLog("TETE MODBUS:> SetSystemRTC = %04d-%02d-%02d %02d:%02d%02d",
                stSysTime.wYear,
                stSysTime.wMonth,
                stSysTime.wDay,
                stSysTime.wHour,
                stSysTime.wMinute,
                stSysTime.wSecond);
    //--SetSystemRTC(&stSysTime);
    return true;
}
/*
功能描述：系统对时
*/
bool CMBControlProxy::GetSystemTimeTick(bool IsAction)
{
    //TODO: Add your source code here
	SYSTEMTIME stSysTime;
    stSysTime.wYear   = m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_RTC_Year;
    stSysTime.wMonth  = m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_RTC_Month;
    stSysTime.wDay    = m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_RTC_Date;
    stSysTime.wHour   = m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_RTC_Hour;
    stSysTime.wMinute = m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_RTC_Min;
    stSysTime.wSecond = m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_RTC_Sec;
	GetLocalTime(&stSysTime);
	//--GetSystemTime(&stSysTime);

	m_pTheLog->WriteLog("TETE MODBUS:> SetSystemRTC = %04d-%02d-%02d %02d:%02d%02d",
                stSysTime.wYear,
                stSysTime.wMonth,
                stSysTime.wDay,
                stSysTime.wHour,
                stSysTime.wMinute,
                stSysTime.wSecond);
    //--SetSystemRTC(&stSysTime);
    return true;
}
//------------------------------------------------------------------------------
/*
功能描述：设置自动打标标识
uLaserSel:
     TETE_LASER_I  :
     TETE_LASER_II :
return:  
    true :  OK
    false:  Failed
*/
bool CMBControlProxy::GetSystemAutoFlagAction(bool IsAction)
{
	m_pTheLog->WriteLog("TETE MODBUS:> SetSystemAutoFlagAction = %d\n",IsAction);
    
    m_stMBMapInfoSets.stCoilsInfo.stSystemCoilsInfo.System_AutoFlag = IsAction;
    //--MainForm->SetAutoMarkMode(IsAction);

    return true;
}
/*
功能描述：设置自动打标标识
uLaserSel:
     TETE_LASER_I  :
     TETE_LASER_II :
return:  
    true :  OK
    false:  Failed
*/
bool CMBControlProxy::SetSystemAutoFlagAction(bool IsAction)
{
	m_pTheLog->WriteLog("TETE MODBUS:> SetSystemAutoFlagAction = %d\n",IsAction);
    
    m_stMBMapInfoSets.stCoilsInfo.stSystemCoilsInfo.System_AutoFlag = IsAction;
    //--MainForm->SetAutoMarkMode(IsAction);

    return true;
}
//------------------------------------------------------------------------------
/*
功能描述：
uLaserSel:
     TETE_LASER_I  :
     TETE_LASER_II :
return:  
    true :  OK
    false:  Failed
*/
bool CMBControlProxy::SetSystemAutoArea1Action(bool IsAction)
{
	m_pTheLog->WriteLog("TETE MODBUS:> SetSystemAutoArea1Action = %d\n",IsAction);
    
    //--m_stMBMapInfoSets.stCoilsInfo.stSystemCoilsInfo.System_AutoArea_1 = IsAction;

    return true;
}
//------------------------------------------------------------------------------
/*
功能描述：
uLaserSel:
     TETE_LASER_I  :
     TETE_LASER_II :
return:  
    true :  OK
    false:  Failed
*/
bool CMBControlProxy::SetSystemAutoArea2Action(bool IsAction)
{
	m_pTheLog->WriteLog("TETE MODBUS:> SetSystemAutoArea2Action = %d\n",IsAction);
    
    //--m_stMBMapInfoSets.stCoilsInfo.stSystemCoilsInfo.System_AutoArea_2 = IsAction;

    return true;
}
//------------------------------------------------------------------------------
/*
功能描述：判断激光器状态自检
uLaserSel:
     TETE_LASER_I  :
     TETE_LASER_II :
return:  
    true : Inservice OK
    false: Inservice Failed
*/
bool CMBControlProxy::GetLaserInservice(uint8_t uLaserSel)
{
    //TODO: Add your source code here
    bool IsAction = false;
    switch(uLaserSel)
    {
    case TETE_LASER_I:
        //--IsAction = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[0].Laser_Inservice!=0?TRUE:FALSE;
        break;
    case TETE_LASER_II:
        //--IsAction = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[1].Laser_Inservice!=0?TRUE:FALSE;
        break;
    default:break;
    }

	m_pTheLog->WriteLog("TETE MODBUS:> Laser%d GetLaserInservice = %d\n",uLaserSel,IsAction);
    return true;
}
/*
功能描述：相當於激光器狀態自檢，更新31002/32002，等待Client查询
  */
bool CMBControlProxy::SetLaserInservice(uint8_t uLaserSel,bool IsInservice)
{
    //TODO: Add your source code here
    CString sFlag;
	m_pTheLog->WriteLog("TETE MODBUS:> Laser%d SetLaserInservice = %d\n",uLaserSel,IsInservice);
    switch(uLaserSel)
    {
    case TETE_LASER_I:
        m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[0].Laser_Inservice = IsInservice;
        break;
    case TETE_LASER_II:
        m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[1].Laser_Inservice = IsInservice;
        break;
    default:break;
    }

    return true;
}
//------------------------------------------------------------------------------
/*
uLaserSel:
     TETE_LASER_I  :
     TETE_LASER_II :
return:
    true : Initial
    false: Donot Initial
*/
bool CMBControlProxy::GetLaserInitial(uint8_t uLaserSel)
{
    //TODO: Add your source code here
    bool IsAction = false;
    switch(uLaserSel)
    {
    case TETE_LASER_I:
        IsAction = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[0].Laser_Initial!=0?TRUE:FALSE;
        break;
    case TETE_LASER_II:
        IsAction = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[1].Laser_Initial!=0?TRUE:FALSE;
        break;
    default:break;
    }

	m_pTheLog->WriteLog("TETE MODBUS:> Laser%d GetLaserInitial = %d\n",uLaserSel,IsAction);
    
    return true;
}
/*
功能描述：激光器初始化，相当于恢复初始状态
*/
bool CMBControlProxy::SetLaserInitial(uint8_t uLaserSel,bool IsAction)
{
    //TODO: Add your source code here
	m_pTheLog->WriteLog("TETE MODBUS:> Laser%d SetLaserInitial = %d\n",uLaserSel,IsAction);
    switch(uLaserSel)
    {
    case TETE_LASER_I:
        m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[0].Laser_Initial = IsAction;
        break;
    case TETE_LASER_II:
        m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[1].Laser_Initial = IsAction;
        break;
    default:break;
    }
    
    return true;
}
//------------------------------------------------------------------------------
/*
uLaserSel:
     TETE_LASER_I  :
     TETE_LASER_II :
IsAction:
    true : LaserProgRunOrgin
    false: Donot LaserProgRunOrgin
*/
bool CMBControlProxy::GetLaserProgRunOrgin(uint8_t uLaserSel)
{
    //TODO: Add your source code here
    bool IsAction = false;
    switch(uLaserSel)
    {
    case TETE_LASER_I:
        IsAction = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[0].Laser_ProgRun_Orgin!=0?TRUE:FALSE;
        break;
    case TETE_LASER_II:
        IsAction = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[1].Laser_ProgRun_Orgin!=0?TRUE:FALSE;
        break;
    default:break;
    }

	m_pTheLog->WriteLog("TETE MODBUS:> Laser%d GetLaserProgRunOrgin = %d\n",uLaserSel,IsAction);
    
    return true;
}
/*
功能描述：激光器原点位置出光，用于校正
*/
bool CMBControlProxy::SetLaserProgRunOrgin(uint8_t uLaserSel,bool IsAction)
{
    //TODO: Add your source code here
    /*
    debugf("TETE MODBUS:> Laser%d SetLaserProgRunOrgin = %d\n",uLaserSel,IsAction);
    LaserPoint_t LaserPoints;
    LaserPoints.dbPointX = 0;
    LaserPoints.dbPointY = 0;
    switch(uLaserSel)
    {
    case TETE_LASER_I:
        m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[0].Laser_ProgRun_Orgin = IsAction;        
        if(IsAction)
        {
            g_pTheScanlabRTC5->MarkPoint(TETE_LASER_I, &LaserPoints, 1);
            m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[0].Laser_ProgRun_Orgin = false;
        }
        break;
    case TETE_LASER_II:
        m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[1].Laser_ProgRun_Orgin = IsAction;
        if(IsAction)
        {
            g_pTheScanlabRTC5->MarkPoint(TETE_LASER_II, &LaserPoints, 1);
            m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[1].Laser_ProgRun_Orgin = false;
        }
        break;
    default:break;
    }
    */
    return true;
}
//------------------------------------------------------------------------------
/*
uLaserSel:
     TETE_LASER_I  :
     TETE_LASER_II :
IsAction:
    true : LaserProgRunLoop
    false: Donot LaserProgRunLoop
*/
bool CMBControlProxy::GetLaserProgRunLoop(uint8_t uLaserSel)
{
    //TODO: Add your source code here
    bool IsAction = false;
    switch(uLaserSel)
    {
    case TETE_LASER_I:
        IsAction = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[0].Laser_ProgRun_Loop!=0?TRUE:FALSE;
        break;
    case TETE_LASER_II:
        IsAction = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[1].Laser_ProgRun_Loop!=0?TRUE:FALSE;
        break;
    default:break;
    }

	m_pTheLog->WriteLog("TETE MODBUS:> Laser%d GetLaserProgRunLoop = %d\n",uLaserSel,IsAction);
    
    return true;
}
/*
功能描述：激光器四点位置出光，用于校正
*/
bool CMBControlProxy::SetLaserProgRunLoop(uint8_t uLaserSel,bool IsAction)
{
    //TODO: Add your source code here
	m_pTheLog->WriteLog("TETE MODBUS:> Laser%d SetLaserProgRunLoop = %d\n",uLaserSel,IsAction);
    /*
    LaserPoint_t LaserPoints[4];
    int i;
    switch(uLaserSel)
    {
    case TETE_LASER_I:
        m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[0].Laser_ProgRun_Loop = IsAction;
        if(0!=IsAction)
        {
            for(i=0;i<4;i++)
            {
                LaserPoints[i].dbPointX = m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[0].stLaserAxisHoldRegsInfo[0].Axis_TargetPos[i];
                LaserPoints[i].dbPointY = m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[0].stLaserAxisHoldRegsInfo[1].Axis_TargetPos[i];
            }
            g_pTheScanlabRTC5->MarkPoint(TETE_LASER_I, LaserPoints, 4);
        }
        break;
    case TETE_LASER_II:
        m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[1].Laser_ProgRun_Loop = IsAction;
        if(0!=IsAction)
        {
            for(i=0;i<4;i++)
            {
                LaserPoints[i].dbPointX = m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[1].stLaserAxisHoldRegsInfo[0].Axis_TargetPos[i];
                LaserPoints[i].dbPointY = m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[1].stLaserAxisHoldRegsInfo[1].Axis_TargetPos[i];
            }
            g_pTheScanlabRTC5->MarkPoint(TETE_LASER_II, LaserPoints, 4);
        }
        break;
    default:break;
    }
    */
    return true;
}
//------------------------------------------------------------------------------
/*
uLaserSel:
     TETE_LASER_I  :
     TETE_LASER_II :
IsAction:
    true : LaserAction
    false: Donot LaserAction
*/
bool CMBControlProxy::GetLaserAction(uint8_t uLaserSel)
{
    //TODO: Add your source code here
    bool IsAction = false;
    switch(uLaserSel)
    {
    case TETE_LASER_I:
        //--IsAction = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].Laser_Action!=0?TRUE:FALSE;
        break;
    case TETE_LASER_II:
        //--IsAction = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].Laser_Action!=0?TRUE:FALSE;
        break;
    default:break;
    }

	m_pTheLog->WriteLog("TETE MODBUS:> Laser%d GetLaserAction = %d\n",uLaserSel,IsAction);
    
    return true;
}
/*
功能描述：激光器打标使能，用于手动打标
*/
bool CMBControlProxy::SetLaserAction(uint8_t uLaserSel,bool IsAction)
{
    //TODO: Add your source code here
	m_pTheLog->WriteLog("TETE MODBUS:> Laser%d SetLaserAction = %d\n",uLaserSel,IsAction);
    /*
    //--自动打标标识使能时，禁止手动打标
    if(m_stMBMapInfoSets.stCoilsInfo.stSystemCoilsInfo.System_AutoFlag)
        return false;
    
    switch(uLaserSel)
    {
    case TETE_LASER_I:
        m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[0].Laser_Action=IsAction;
        if(IsAction)
        {
            g_pTheMarkFile->GetDoc(2);
            MainForm->btnMarkClick(NULL);
            //--g_pTheScanlabRTC5->MarkDoc(RTC5_LASER_I,g_pTheMarkFile->GetDoc(2));
            m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[0].Laser_Action=false;
        }
        break;
    case TETE_LASER_II:
        m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[1].Laser_Action=IsAction;
        if(IsAction)
        {
            g_pTheMarkFile->GetDoc(3);
            MainForm->btnMarkClick(NULL);
            //--g_pTheScanlabRTC5->MarkDoc(RTC5_LASER_II,g_pTheMarkFile->GetDoc(3));
            m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[1].Laser_Action=false;
        }
        break;
    default:break;
    }
    */
    return true;
}
//------------------------------------------------------------------------------
/*
uLaserSel:
     TETE_LASER_I  :
     TETE_LASER_II :
IsAction:
    true : LaserParameterLoad
    false: Donot LaserParameterLoad
*/
bool CMBControlProxy::GetLaserParameterLoad(uint8_t uLaserSel)
{
    //TODO: Add your source code here
    bool IsAction = false;
    //--IsAction = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].Laser_ParameterLoad!=0?TRUE:FALSE;

	m_pTheLog->WriteLog("TETE MODBUS:> Laser%d GetLaserParameterLoad = %d\n",uLaserSel,IsAction);
    
    return true;
}
bool CMBControlProxy::SetLaserParameterLoad(uint8_t uLaserSel,bool IsAction)
{
    //TODO: Add your source code here
	m_pTheLog->WriteLog("TETE MODBUS:> Laser%d SetLaserParameterLoad = %d\n",uLaserSel,IsAction);
    //--m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].Laser_ParameterLoad = IsAction;
    switch(uLaserSel)
    {
    case TETE_LASER_I:
        //--m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].Laser_ParameterLoad = IsAction;
        break;
    case TETE_LASER_II:
        //--m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].Laser_ParameterLoad = IsAction;
        break;
    default:break;
    }

    return true;
}
/**/
bool CMBControlProxy::SetLaserCalibrationFileActive(uint8_t uLaserSel,bool IsAction)
{
    //TODO: Add your source code here
	m_pTheLog->WriteLog("TETE MODBUS:> Laser%d SetLaserCalibrationFileActive = %d\n",uLaserSel,IsAction);
    m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].Laser_Write_Cal = IsAction;
    switch(uLaserSel)
    {
    case TETE_LASER_I:
        m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].Laser_Write_Cal= IsAction;
        break;
    case TETE_LASER_II:
        m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].Laser_Write_Cal = IsAction;
        break;
    default:break;
    }

    return true;
}
/**/
bool CMBControlProxy::SetLaserTemplateFileActive(uint8_t uLaserSel,bool IsAction)
{
    //TODO: Add your source code here
	m_pTheLog->WriteLog("TETE MODBUS:> Laser%d SetLaserTemplateFileActive = %d\n",uLaserSel,IsAction);
    //--m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].Laser_Write_Template= IsAction;
    switch(uLaserSel)
    {
    case TETE_LASER_I:
        //--m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].Laser_Write_Template = IsAction;
        break;
    case TETE_LASER_II:
        //--m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].Laser_Write_Template = IsAction;
        break;
    default:break;
    }

    return true;
}
/**/
bool CMBControlProxy::SetLaserMapFileActive(uint8_t uLaserSel,bool IsAction)
{
    //TODO: Add your source code here
	m_pTheLog->WriteLog("TETE MODBUS:> Laser%d SetLaserMapFileActive = %d\n",uLaserSel,IsAction);
    //--m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].Laser_Write_Map= IsAction;
    switch(uLaserSel)
    {
    case TETE_LASER_I:
        //--m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].Laser_Write_Map = IsAction;
        break;
    case TETE_LASER_II:
        //--m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].Laser_Write_Map = IsAction;
        break;
    default:break;
    }

    return true;
}
/**/
bool CMBControlProxy::SetLaserEmgActive(uint8_t uLaserSel,bool IsAction)
{
    //TODO: Add your source code here
	m_pTheLog->WriteLog("TETE MODBUS:> Laser%d SetLaserEmgActive = %d\n",uLaserSel,IsAction);
    /*
    m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].Laser_Emg = IsAction;
    if(IsAction)
    {
        g_pTheScanlabRTC5->SetlaserOnOff(false);
        m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].Laser_Emg = false;
    }
    switch(uLaserSel)
    {
    case TETE_LASER_I:
        m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].Laser_Emg = IsAction;
        if(IsAction)
        {
            g_pTheScanlabRTC5->SetlaserOnOff(false);
            m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].Laser_Emg = false;
        }
        break;
    case TETE_LASER_II:
        m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].Laser_Emg = IsAction;
        if(IsAction)
        {
            g_pTheScanlabRTC5->SetlaserOnOff(false);
            m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].Laser_Emg = false;
        }
        break;
    default:break;
    }*/

    return true;
}
//------------------------------------------------------------------------------
bool CMBControlProxy::GetLaserStatusBit(uint8_t uLaserSel,int iIndex)
{
    //TODO: Add your source code here
    bool IsAction = false;
    //--IsAction = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].Laser_Status_Bit2!=0?TRUE:FALSE;

	m_pTheLog->WriteLog("TETE MODBUS:> Laser%d GetLaserStatusBit = %d\n",uLaserSel,IsAction);
    
    return true;
}
//------------------------------------------------------------------------------
/*
uLaserSel:
    LASER_I/LASER_II :
uAxisSel:
    AXIS_X/AXIS_Y/AXIS_Z:
IsAction:
    true/false:
*/
/*
bool CMBControlProxy::GetAxisInservice(uint8_t uLaserSel,uint8_t uAxisSel)
{
    //TODO: Add your source code here
    bool IsAction;    
    IsAction = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].stAxisCoilsInfo[uAxisSel].Axis_Inservice!=0?TRUE:FALSE;
    printd("TETE MODBUS:> Laser%d Axis%d,GetAxisInservice = %d\n",uLaserSel,uAxisSel,IsAction);
    
    return true;
}
bool CMBControlProxy::SetAxisInservice(uint8_t uLaserSel,uint8_t uAxisSel,bool IsAction)
{
    //TODO: Add your source code here
    printd("TETE MODBUS:> Laser%d Axis%d,SetAxisInservice = %d\n",uLaserSel,uAxisSel,IsAction);
    m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].stAxisCoilsInfo[uAxisSel].Axis_Inservice = IsAction;
    switch(uLaserSel)
    {
    case LASER_I:
        break;
    case LASER_II:
        break;
    default:break;
    }
    
    return true;
}
*/
//------------------------------------------------------------------------------
/*
uLaserSel:
    LASER_I/LASER_II :
uAxisSel:
    AXIS_X/AXIS_Y/AXIS_Z:
IsAction:
    true/false:
*/
/*
bool CMBControlProxy::GetAxisInitial(uint8_t uLaserSel,uint8_t uAxisSel)
{
    //TODO: Add your source code here
    bool IsAction;
    IsAction = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].stAxisCoilsInfo[uAxisSel].Axis_Initial!=0?TRUE:FALSE;

    printd("TETE MODBUS:> Laser%d Axis%d,GetAxisInitial = %d\n",uLaserSel,uAxisSel,IsAction);
    
    return true;
}
bool CMBControlProxy::SetAxisInitial(uint8_t uLaserSel,uint8_t uAxisSel,bool IsAction)
{
    //TODO: Add your source code here
    printd("TETE MODBUS:> Laser%d Axis%d,SetAxisInitial = %d\n",uLaserSel,uAxisSel,IsAction);
    m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].stAxisCoilsInfo[uAxisSel].Axis_Initial = IsAction;
    switch(uLaserSel)
    {
    case LASER_I:
        break;
    case LASER_II:
        break;
    default:break;
    }

    return true;
}
//------------------------------------------------------------------------------
bool CMBControlProxy::GetAxisReady(uint8_t uLaserSel,uint8_t uAxisSel)
{
    //TODO: Add your source code here
    bool IsAction;    
    IsAction = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].stAxisCoilsInfo[uAxisSel].Axis_Ready!=0?TRUE:FALSE;

    printd("TETE MODBUS:> Laser%d Axis%d,GetAxisReady = %d\n",uLaserSel,uAxisSel,IsAction);
    
    return true;
}
*/
//------------------------------------------------------------------------------
/*
uLaserSel:
    LASER_I/LASER_II :
uAxisSel:
    AXIS_X/AXIS_Y/AXIS_Z:
IsAction:
    true/false:
*/
/*
bool CMBControlProxy::GetAxisHome(uint8_t uLaserSel,uint8_t uAxisSel)
{
    //TODO: Add your source code here
    bool IsAction;    
    IsAction = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].stAxisCoilsInfo[uAxisSel].Axis_Home!=0?TRUE:FALSE;

    printd("TETE MODBUS:> Laser%d Axis%d,GetAxisHome = %d\n",uLaserSel,uAxisSel,IsAction);

    return true;
}
bool CMBControlProxy::SetAxisHome(uint8_t uLaserSel,uint8_t uAxisSel,bool IsAction)
{
    //TODO: Add your source code here
    printd("TETE MODBUS:> Laser%d Axis%d,SetAxisHome = %d\n",uLaserSel,uAxisSel,IsAction);
    m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].stAxisCoilsInfo[uAxisSel].Axis_Home = IsAction;
    switch(uLaserSel)
    {
    case LASER_I:
        break;
    case LASER_II:
        break;
    default:break;
    }

    return true;
}
*/
//------------------------------------------------------------------------------
/*
uLaserSel:
    LASER_I/LASER_II :
uAxisSel:
    AXIS_X/AXIS_Y/AXIS_Z:
IsAction:
    true/false:
*/
/*
bool CMBControlProxy::GetAxisAbsMove(uint8_t uLaserSel,uint8_t uAxisSel)
{
    //TODO: Add your source code here
    bool IsAction;    
    IsAction = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].stAxisCoilsInfo[uAxisSel].Axis_AbsMove!=0?TRUE:FALSE;

    printd("TETE MODBUS:> Laser%d Axis%d,GetAxisAbsMove = %d\n",uLaserSel,uAxisSel,IsAction);
    
    return true;
}
bool CMBControlProxy::SetAxisAbsMove(uint8_t uLaserSel,uint8_t uAxisSel,bool IsAction)
{
    //TODO: Add your source code here
    printd("TETE MODBUS:> Laser%d Axis%d,SetAxisAbsMove = %d\n",uLaserSel,uAxisSel,IsAction);
    m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].stAxisCoilsInfo[uAxisSel].Axis_AbsMove = IsAction;
    switch(uLaserSel)
    {
    case LASER_I:
        break;
    case LASER_II:
        break;
    default:break;
    }

    return true;
}
*/
//------------------------------------------------------------------------------
/*
uLaserSel:
    LASER_I/LASER_II :
uAxisSel:
    AXIS_X/AXIS_Y/AXIS_Z:
IsAction:
    true/false:
*/
/*
bool CMBControlProxy::GetAxisRelMove(uint8_t uLaserSel,uint8_t uAxisSel)
{
    //TODO: Add your source code here
    bool IsAction;    
    IsAction = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uAxisSel].stAxisCoilsInfo[uAxisSel].Axis_RelMove!=0?TRUE:FALSE;
   
    printd("TETE MODBUS:> Laser%d Axis%d,GetAxisRelMove = %d\n",uLaserSel,uAxisSel,IsAction);
    
    return true;
}
bool CMBControlProxy::SetAxisRelMove(uint8_t uLaserSel,uint8_t uAxisSel,bool IsAction)
{
    //TODO: Add your source code here
    printd("TETE MODBUS:> Laser%d Axis%d,SetAxisRelMove = %d\n",uLaserSel,uAxisSel,IsAction);
    m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].stAxisCoilsInfo[uAxisSel].Axis_RelMove = IsAction;
    switch(uLaserSel)
    {
    case LASER_I:
        break;
    case LASER_II:
        break;
    default:break;
    }

    return true;
}
*/
//------------------------------------------------------------------------------
/*
uLaserSel:
    LASER_I/LASER_II :
uAxisSel:
    AXIS_X/AXIS_Y/AXIS_Z:
IsAction:
    true/false:
*/
/*
bool CMBControlProxy::GetAxisEmgStop(uint8_t uLaserSel,uint8_t uAxisSel)
{
    //TODO: Add your source code here
    bool IsAction;
    IsAction = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].stAxisCoilsInfo[uAxisSel].Axis_EmgStop!=0?TRUE:FALSE;

    printd("TETE MODBUS:> Laser%d Axis%d,GetAxisEmgStop = %d\n",uLaserSel,uAxisSel,IsAction);
    
    return true;
}
bool CMBControlProxy::SetAxisEmgStop(uint8_t uLaserSel,uint8_t uAxisSel,bool IsAction)
{
    //TODO: Add your source code here
    printd("TETE MODBUS:> Laser%d Axis%d,SetAxisEmgStop = %d\n",uLaserSel,uAxisSel,IsAction);
    m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].stAxisCoilsInfo[uAxisSel].Axis_EmgStop = IsAction;
    switch(uLaserSel)
    {
    case LASER_I:
        break;
    case LASER_II:
        break;
    default:break;
    }

    return true;
}
*/
//------------------------------------------------------------------------------
/*
uLaserSel:
    LASER_I/LASER_II :
uAxisSel:
    AXIS_X/AXIS_Y/AXIS_Z:
IsAction:
    true/false:
*/
/*
bool CMBControlProxy::GetAxisPauseStop(uint8_t uLaserSel,uint8_t uAxisSel)
{
    //TODO: Add your source code here
    bool IsAction;    
    IsAction = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].stAxisCoilsInfo[uAxisSel].Axis_PauseStop!=0?TRUE:FALSE;

    printd("TETE MODBUS:> Laser%d Axis%d,GetAxisPauseStop = %d\n",uLaserSel,uAxisSel,IsAction);
    
    return true;
}
bool CMBControlProxy::SetAxisPauseStop(uint8_t uLaserSel,uint8_t uAxisSel,bool IsAction)
{
    //TODO: Add your source code here
    printd("TETE MODBUS:> Laser%d Axis%d,SetAxisPauseStop = %d\n",uLaserSel,uAxisSel,IsAction);
    m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].stAxisCoilsInfo[uAxisSel].Axis_PauseStop = IsAction;
    switch(uLaserSel)
    {
    case LASER_I:
        break;
    case LASER_II:
        break;
    default:break;
    }

    return true;
}
*/
//------------------------------------------------------------------------------
/*
uLaserSel:
    LASER_I/LASER_II :
uAxisSel:
    AXIS_X/AXIS_Y/AXIS_Z:
IsAction:
    true/false:
*/
/*
bool CMBControlProxy::GetAxisParaLoad(uint8_t uLaserSel,uint8_t uAxisSel)
{
    //TODO: Add your source code here
    bool IsAction; 
    IsAction = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].stAxisCoilsInfo[uAxisSel].Axis_ParaLoad!=0?TRUE:FALSE;
    printd("TETE MODBUS:> Laser%d Axis%d,GetAxisParaLoad = %d\n",uLaserSel,uAxisSel,IsAction);
    
    return true;
}
bool CMBControlProxy::SetAxisParaLoad(uint8_t uLaserSel,uint8_t uAxisSel,bool IsAction)
{
    //TODO: Add your source code here
    printd("TETE MODBUS:> Laser%d Axis%d,SetAxisParaLoad = %d\n",uLaserSel,uAxisSel,IsAction);
    m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].stAxisCoilsInfo[uAxisSel].Axis_ParaLoad = IsAction;
    switch(uLaserSel)
    {
    case LASER_I:
        break;
    case LASER_II:
        break;
    default:break;
    }

    return true;
}

//------------------------------------------------------------------------------
bool CMBControlProxy::GetAxisStatusBit(uint8_t uLaserSel,uint8_t uAxisSel,int iIndex)
{
    //TODO: Add your source code here
    bool IsAction;

    printd("TETE MODBUS:> Laser%d Axis%d,Index%d,GetAxisStatusBit = %d\n",uLaserSel,uAxisSel,iIndex,IsAction);
    
    return true;
}
*/
//------------------------------------------------------------------------------
/*
return:
    use macro LASER_STAT_XXXX    
*/
uint16_t CMBControlProxy::GetLaserMCReadStatus(uint8_t uLaserSel)
{
    //TODO: Add your source code here  
    uint16_t wStatus = 0;
    //--wStatus = m_stMBMapInfoSets.stInputRegsInfo.stLaserInputRegsInfo[uLaserSel].Laser_MC_ReadStatus;
	m_pTheLog->WriteLog("TETE MODBUS:> Laser%d GetLaserMCReadStatus = %d\n",uLaserSel,wStatus);
    return wStatus;
}
//------------------------------------------------------------------------------
/*
return:
    use macro LASER_EC_XXXX    
*/
uint16_t CMBControlProxy::GetLaserMCReadError(uint8_t uLaserSel)
{
    //TODO: Add your source code here
    uint16_t wStatus = 0;
    //--wStatus = m_stMBMapInfoSets.stInputRegsInfo.stLaserInputRegsInfo[uLaserSel].Laser_MC_ReadError;
	m_pTheLog->WriteLog("TETE MODBUS:> Laser%d GetLaserMCReadError = %d\n",uLaserSel,wStatus);
    return wStatus;
}
//------------------------------------------------------------------------------
/*
iIndex:
    0~15
*/
uint16_t CMBControlProxy::GetLaserMCReadDataIn(uint8_t uLaserSel,int iIndex)
{
    //TODO: Add your source code here
    uint16_t wStatus = 0;
	m_pTheLog->WriteLog("TETE MODBUS:> Laser%d GetLaserMCReadDataIn = %d\n",uLaserSel,wStatus);
    return wStatus;
}
//------------------------------------------------------------------------------
/*
return:
    use macro MOTOR_STAT_XXXX    
*/
uint16_t CMBControlProxy::GetAxisReadStatus(uint8_t uLaserSel,uint8_t uAxisSel)
{
    //TODO: Add your source code here
    uint16_t wStatus = 0;

    //--wStatus = m_stMBMapInfoSets.stInputRegsInfo.stAxisInputRegsInfo[uLaserSel][uAxisSel].Axis_ReadStatus;
    
	m_pTheLog->WriteLog("TETE MODBUS:> Laser%d,Axis%d, GetLaserMCReadStatus = %d\n",uLaserSel,uAxisSel,wStatus);
    return wStatus;
}
//------------------------------------------------------------------------------
/*
return:
    use macro MOTOR_EC_XXXX    
*/
uint16_t CMBControlProxy::GetAxisReadError(uint8_t uLaserSel,uint8_t uAxisSel)
{
    //TODO: Add your source code here
    uint16_t wStatus = 0;

    //--m_stMBMapInfoSets.stInputRegsInfo.stAxisInputRegsInfo[uLaserSel][uAxisSel].Axis_ReadEncoder = wStatus;
       
	m_pTheLog->WriteLog("TETE MODBUS:> Laser%d,Axis%d, GetAxisReadError = %d\n",uLaserSel,uAxisSel,wStatus);
    return wStatus;
}
//------------------------------------------------------------------------------
uint16_t CMBControlProxy::GetAxisReadEncoder(uint8_t uLaserSel,uint8_t uAxisSel)
{
    //TODO: Add your source code here
    uint16_t wStatus = 0;
    
    //--wStatus = m_stMBMapInfoSets.stInputRegsInfo.stAxisInputRegsInfo[uLaserSel][uAxisSel].Axis_ReadEncoder;
        
	m_pTheLog->WriteLog("TETE MODBUS:> Laser%d,Axis%d, GetAxisReadEncoder = %d\n",uLaserSel,uAxisSel,wStatus);
    return wStatus;
}
//------------------------------------------------------------------------------
/*
iIndex:
    0~7
*/
uint16_t CMBControlProxy::GetAxisReadDataIn(uint8_t uLaserSel,uint8_t uAxisSel,int iIndex)
{
    //TODO: Add your source code here
    uint16_t wStatus = 0;
    
    //--wStatus = m_stMBMapInfoSets.stInputRegsInfo.stAxisInputRegsInfo[uLaserSel][uAxisSel].Axis_ReadDataIn[iIndex];
        
	m_pTheLog->WriteLog("TETE MODBUS:> Laser%d,Axis%d,iIndex%d GetAxisReadDataIn = %d\n",uLaserSel,uAxisSel,iIndex,wStatus);
    return wStatus;
}
//------------------------------------------------------------------------------
bool CMBControlProxy::GetSystemRTC(SYSTEMTIME *stSysTime)
{
    //TODO: Add your source code here
    GetSystemTime(stSysTime);

    return true;
}
//------------------------------------------------------------------------------
bool CMBControlProxy::SetSystemRTC(SYSTEMTIME *pstSysTime)
{
    //TODO: Add your source code here
    
    SetSystemTime(pstSysTime);
    
    return true;
}
//------------------------------------------------------------------------------
/*
uLaserSel:
    LASER_I/LASER_II/LASER_III/LASER_IV :
uLCOSSel:
    LC_OS_X/LC_OS_Y/LC_OS_THETA:
fValue:
    the Value Of Offset:
*/
/*
float CMBControlProxy::GetSystemLaserCenterOffset(uint8_t uLaserSel,uint8_t uLCOSSel)
{
    float fValue;
    
    switch(uLaserSel)
    {
    case LASER_I:
        if(uLCOSSel==LC_OS_X)
            fValue = m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_LC_I_OS_X; 
        else if(uLCOSSel==LC_OS_Y)
            fValue = m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_LC_I_OS_Y;
        else if(uLCOSSel==LC_OS_THETA)
            fValue = m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_LC_I_OS_Theta;
        
        break;
    case LASER_II:
        if(uLCOSSel==LC_OS_X)
            fValue = m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_LC_II_OS_X; 
        else if(uLCOSSel==LC_OS_Y)
            fValue = m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_LC_II_OS_Y;
        else if(uLCOSSel==LC_OS_THETA)
            fValue = m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_LC_II_OS_Theta;
        
        break;
    case LASER_III:
        if(uLCOSSel==LC_OS_X)
            fValue = m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_LC_III_OS_X; 
        else if(uLCOSSel==LC_OS_Y)
            fValue = m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_LC_III_OS_Y;
        else if(uLCOSSel==LC_OS_THETA)
            fValue = m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_LC_III_OS_Theta;
        
        break;
    case LASER_IV:
        if(uLCOSSel==LC_OS_X)
            fValue = m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_LC_IV_OS_X; 
        else if(uLCOSSel==LC_OS_Y)
            fValue = m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_LC_IV_OS_Y;
        else if(uLCOSSel==LC_OS_THETA)
            fValue = m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_LC_IV_OS_Theta;
        
        break;
    default:break;
    }
    printd("TETE MODBUS:> Laser%d LC_OS_%d,GetSystemLaserCenterOffset = %f",uLaserSel,uLCOSSel,fValue);
    
    return fValue;
}
bool CMBControlProxy::SetSystemLaserCenterOffset(uint8_t uLaserSel,uint8_t uLCOSSel,float fValue)
{
    //TODO: Add your source code here
    printd("TETE MODBUS:> Laser%d LC_OS_%d,SetSystemLaserCenterOffset = %f",uLaserSel,uLCOSSel,fValue);
    switch(uLaserSel)
    {
    case LASER_I:
        if(uLCOSSel==LC_OS_X)
            m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_LC_I_OS_X = fValue; 
        else if(uLCOSSel==LC_OS_Y)
            m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_LC_I_OS_Y = fValue;
        else if(uLCOSSel==LC_OS_THETA)
            m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_LC_I_OS_Theta = fValue;
        
        break;
    case LASER_II:
        if(uLCOSSel==LC_OS_X)
            m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_LC_II_OS_X = fValue; 
        else if(uLCOSSel==LC_OS_Y)
            m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_LC_II_OS_Y = fValue;
        else if(uLCOSSel==LC_OS_THETA)
            m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_LC_II_OS_Theta = fValue;
        
        break;
    case LASER_III:
        if(uLCOSSel==LC_OS_X)
            m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_LC_III_OS_X = fValue; 
        else if(uLCOSSel==LC_OS_Y)
            m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_LC_III_OS_Y = fValue;
        else if(uLCOSSel==LC_OS_THETA)
            m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_LC_III_OS_Theta = fValue;
        
        break;
    case LASER_IV:
        if(uLCOSSel==LC_OS_X)
            m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_LC_IV_OS_X = fValue; 
        else if(uLCOSSel==LC_OS_Y)
            m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_LC_IV_OS_Y = fValue;
        else if(uLCOSSel==LC_OS_THETA)
            m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_LC_IV_OS_Theta = fValue;
        
        break;
    default:break;
    }

    return true;
}
*/
//------------------------------------------------------------------------------
/*
uLaserSel:
    LASER_I/LASER_II/LASER_III/LASER_IV :
FileName:
    File Path is included,less then 100 ASCII:
*/
bool CMBControlProxy::GetLaserTemplateFile(uint8_t uLaserSel,char *sFileName)
{
    switch(uLaserSel)
    {
    case TETE_LASER_I:
        strncpy(sFileName,m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[0].Laser_Template_Filename,100);
        break;
    case TETE_LASER_II:
        strncpy(sFileName,m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[1].Laser_Template_Filename,100);
        break;
    default:break;
    }
	m_pTheLog->WriteLog("TETE MODBUS:>Laser%d,GetActionTemplateFileName=%s\n",uLaserSel,sFileName);
    return true;
}
bool CMBControlProxy::SetLaserTemplateFile(uint8_t uLaserSel,char *sFileName)
{
    //TODO: Add your source code here
    bool uResult = 0;
    /*
    if(TETE_LASER_I==uLaserSel)
    {
        if(0==m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[0].Laser_Write_Template)
            return false;
        strncpy(m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[0].Laser_Template_Filename,sFileName,99);
        m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[0].Laser_Template_Filename[99] = '\0';
        debugf("TETE MODBUS:>Laser%d,SetLaserTemplateFileName=%s",uLaserSel,sFileName);
        MainForm->LoadTemplateFile(TETE_LASER_I,m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[0].Laser_Template_Filename);
        m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[0].Laser_Write_Template = 0;
    }
    else if(TETE_LASER_II==uLaserSel)
    {
        if(0==m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[1].Laser_Write_Template)
            return false;
        strncpy(m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[1].Laser_Template_Filename,sFileName,99);
        m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[1].Laser_Template_Filename[99] = '\0';
        debugf("TETE MODBUS:>Laser%d,SetLaserTemplateFileName=%s",uLaserSel,sFileName);
        MainForm->LoadTemplateFile(TETE_LASER_II,m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[1].Laser_Template_Filename);
        m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[1].Laser_Write_Template = 0;
    }
    else
        return false;
    */
    return uResult;
}

//------------------------------------------------------------------------------

/*
uLaserSel:
    LASER_I/LASER_II/LASER_III/LASER_IV :
FileName:
    File Path is included,less then 100 ASCII:
*/
bool CMBControlProxy::GetLaserMapFile(uint8_t uLaserSel,char *sFileName)
{
    switch(uLaserSel)
    {
    case TETE_LASER_I:
        strncpy(sFileName,m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[0].Laser_Map_Filename,100);
        break;
    case TETE_LASER_II:
        strncpy(sFileName,m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[1].Laser_Map_Filename,100);
        break;
    default:break;
    }
	m_pTheLog->WriteLog("TETE MODBUS:>Laser%d,GetActionMapFileName=%s\n",uLaserSel,sFileName);
    return true;
}
bool CMBControlProxy::SetLaserMapFile(uint8_t uLaserSel,char *sFileName)
{
    //TODO: Add your source code here
    uint8_t uResult;
	m_pTheLog->WriteLog("TETE MODBUS:>Laser%d,SetActionMapFileName=%s\n",uLaserSel,sFileName);
    /*
    if(TETE_LASER_I==uLaserSel)
    {
        if(0==m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[0].Laser_Write_Map)
            return false;
        strncpy(m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[0].Laser_Map_Filename,sFileName,99);
        m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[0].Laser_Map_Filename[99] = '\0';
        g_pCMapFileParser->m_sMapFileName = m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[0].Laser_Map_Filename;
        MainForm->LoadMapFile(TETE_LASER_I,sFileName);
        m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[0].Laser_Write_Map = 0;
    }
    else if(TETE_LASER_II==uLaserSel)
    {
        if(0==m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[1].Laser_Write_Map)
            return false;
        strncpy(m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[1].Laser_Map_Filename,sFileName,99);
        m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[1].Laser_Map_Filename[99] = '\0';
        g_pCMapFileParser->m_sMapFileName = m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[1].Laser_Map_Filename;
        MainForm->LoadMapFile(TETE_LASER_II,sFileName);
        m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[1].Laser_Write_Map = 0;
    }
    else
        return false;
    */
    return true;
}
bool CMBControlProxy::GetLaserCalibrateFile(uint8_t uLaserSel,int iIndex,char *sFileName)
{
    switch(uLaserSel)
    {
    case TETE_LASER_I:
        if(iIndex==1)
            strncpy(sFileName,m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[0].Laser_Calibrate_Filename_I,100);
        else
            strncpy(sFileName,m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[0].Laser_Calibrate_Filename_II,100);
        break;
    case TETE_LASER_II:
        if(iIndex==1)
            strncpy(sFileName,m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[1].Laser_Calibrate_Filename_I,100);
        else
            strncpy(sFileName,m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[1].Laser_Calibrate_Filename_II,100);
        break;
    default:break;
    }
	m_pTheLog->WriteLog("TETE MODBUS:>Laser%d,GetLaserCalibrateFileName=%s\n",uLaserSel,sFileName);
    return true;
}
//------------------------------------------------------------------------------
/*
uLaserSel:
    LASER_I/LASER_II/LASER_III/LASER_IV :
FileName:
    File Path is included,less then 100 ASCII:
*/
bool CMBControlProxy::SetLaserCalibrateFile(uint8_t uLaserSel,int iIndex,char *sFileName)
{
    //TODO: Add your source code here
    bool uResult = 0;
	m_pTheLog->WriteLog("TETE MODBUS:>Laser%d,SetLaserCalibrateFileName%d=%s\n",uLaserSel,iIndex,sFileName);
    /*
    if(TETE_LASER_I==uLaserSel)
    {
        if(0==m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[0].Laser_Write_Cal)
            return false;
        if(1==iIndex)
        {
            strncpy(m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[0].Laser_Calibrate_Filename_I,sFileName,99);
            m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[0].Laser_Calibrate_Filename_I[99] = '\0';
        }
        else
        {
            strncpy(m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[0].Laser_Calibrate_Filename_II,sFileName,99);
            m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[0].Laser_Calibrate_Filename_II[99] = '\0';
        }
        MainForm->LoadCorFile(TETE_LASER_I,iIndex,sFileName);
        
        m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[0].Laser_Write_Cal = 0;
    }
    else if(TETE_LASER_II==uLaserSel)
    {
        if(0==m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[1].Laser_Write_Cal)
            return false;
        if(1==iIndex)
        {
            strncpy(m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[1].Laser_Calibrate_Filename_I,sFileName,99);
            m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[1].Laser_Calibrate_Filename_I[99] = '\0';
        }
        else
        {
            strncpy(m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[1].Laser_Calibrate_Filename_II,sFileName,99);
            m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[1].Laser_Calibrate_Filename_II[99] = '\0';
        }
        MainForm->LoadCorFile(TETE_LASER_II,iIndex,sFileName);
        m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[1].Laser_Write_Cal = 0;
    }
    else
        return false;
    */
    return uResult;
}
uint16_t CMBControlProxy::GetLaserMCControlData(uint8_t uLaserSel,int iIndex)
{
    uint16_t wValue = 0;
    
    //--m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[uLaserSel].Laser_MC_ControlData[iIndex] = wValue;
    
	m_pTheLog->WriteLog("TETE MODBUS:> Laser%d index=%d,GetLaserMCControlData = %d\n",uLaserSel,iIndex,wValue);

    return wValue;
}

//------------------------------------------------------------------------------
/*
uLaserSel:
    LASER_I/LASER_II;
iIndex:
    0~15;
wValue:
    the Value of Control Data;
*/
bool CMBControlProxy::SetLaserMCControlData(uint8_t uLaserSel,int iIndex,uint16_t wValue)
{
    //TODO: Add your source code here
	m_pTheLog->WriteLog("TETE MODBUS:> Laser%d index=%d,SetLaserMCControlData = %d\n",uLaserSel,iIndex,wValue);
    
    m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[uLaserSel].Laser_MC_ControlData[iIndex] = wValue;

    return true;
}
float CMBControlProxy::GetAxisTargetPos(uint8_t uLaserSel,uint8_t uAxisSel,uint8_t uPosSel)
{    
    float fTargetPos = 0.0f;
    /*
    switch(uPosSel)
    {
    case AXIS_TARGETPOS_1:
        fTargetPos = m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[uLaserSel].stLaserAxisHoldRegsInfo[uAxisSel].Axis_TargetPos[0];
        break;
    case AXIS_TARGETPOS_2:
        fTargetPos = m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[uLaserSel].stLaserAxisHoldRegsInfo[uAxisSel].Axis_TargetPos[1];
        break;
    case AXIS_TARGETPOS_3:
        fTargetPos = m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[uLaserSel].stLaserAxisHoldRegsInfo[uAxisSel].Axis_TargetPos[2];
        break;
    case AXIS_TARGETPOS_4:
        fTargetPos = m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[uLaserSel].stLaserAxisHoldRegsInfo[uAxisSel].Axis_TargetPos[3];
        break;
    case AXIS_TARGETPOS_MANUL:
        fTargetPos = m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[uLaserSel].stLaserAxisHoldRegsInfo[uAxisSel].Axis_TargetPos_Manul;
        break;
    default:break;
    }
    */  
	m_pTheLog->WriteLog("TETE MODBUS:> Laser%d AXIS_TARGETPOS_%d,GetAxisTargetPos = %f\n",uLaserSel,uPosSel,fTargetPos);
    return fTargetPos;
}
//------------------------------------------------------------------------------
/*
uLaserSel:
    LASER_I/LASER_II:
uPosSel:
    AXIS_TARGETPOS_1/AXIS_TARGETPOS_2/AXIS_TARGETPOS_3/AXIS_TARGETPOS_4/AXIS_TARGETPOS_MANUL:
fTargetPos:
    the Value Of TargetPos:
*/
bool CMBControlProxy::SetAxisTargetPos(uint8_t uLaserSel,uint8_t uAxisSel,uint8_t uPosSel,float fTargetPos)
{
    //TODO: Add your source code here
	m_pTheLog->WriteLog("TETE MODBUS:> Laser%d AXIS_TARGETPOS_%d,SetAxisTargetPos = %f\n",uLaserSel,uPosSel,fTargetPos);
    /*
    switch(uPosSel)
    {
    case AXIS_TARGETPOS_1:
        m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[uLaserSel].stLaserAxisHoldRegsInfo[uAxisSel].Axis_TargetPos[0] = fTargetPos;
        break;
    case AXIS_TARGETPOS_2:
        m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[uLaserSel].stLaserAxisHoldRegsInfo[uAxisSel].Axis_TargetPos[1] = fTargetPos;
        break;
    case AXIS_TARGETPOS_3:
        m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[uLaserSel].stLaserAxisHoldRegsInfo[uAxisSel].Axis_TargetPos[2] = fTargetPos;
        break;
    case AXIS_TARGETPOS_4:
        m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[uLaserSel].stLaserAxisHoldRegsInfo[uAxisSel].Axis_TargetPos[3] = fTargetPos;
        break;
    case AXIS_TARGETPOS_MANUL:
        m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[uLaserSel].stLaserAxisHoldRegsInfo[uAxisSel].Axis_TargetPos_Manul = fTargetPos;
        break;
    default:break;
    }   
    */
    return true;
}
/*
uLaserSel:
    LASER_I/LASER_II;
uAxisSel:
    AXIS_X/AXIS_Y/AXIS_Z;
iIndex:
    2~15;
return:
    the Value of Control Data;
*/
uint16_t CMBControlProxy::GetAxisControlData(uint8_t uLaserSel,uint8_t uAxisSel,int iIndex)
{
    uint16_t wValue = 0;
    
    //--wValue = m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[uLaserSel].stLaserAxisHoldRegsInfo[uAxisSel].Axis_ControlData[iIndex];
    
	m_pTheLog->WriteLog("TETE MODBUS:> axis%d index=%d,SetAxisControlData = %d\n",uAxisSel,iIndex,wValue);
    return true;
}
//------------------------------------------------------------------------------
/*
uLaserSel:
    LASER_I/LASER_II;
uAxisSel:
    AXIS_X/AXIS_Y/AXIS_Z;
iIndex:
    2~15;
wValue:
    the Value of Control Data;
*/
bool CMBControlProxy::SetAxisControlData(uint8_t uLaserSel,uint8_t uAxisSel,int iIndex,uint16_t wValue)
{
    //TODO: Add your source code here
	m_pTheLog->WriteLog("TETE MODBUS:> axis%d index=%d,SetAxisControlData = %d\n",uAxisSel,iIndex,wValue);
    
    //--m_stMBMapInfoSets.stHoldRegsInfo.stLaserHoldRegsInfo[uLaserSel].stLaserAxisHoldRegsInfo[uAxisSel].Axis_ControlData[iIndex] = wValue;

    return true;
}

//------------------------------------------------------------------------------
int CMBControlProxy::OnHandleSystemCoilsChange(int iAddr,bool IsAction, uint8_t *pAddr)
{
    //TODO: Add your source code here
    
    switch(iAddr)
    {
    case SYSTEM_LASER_I_ACTION_COIL:
        SetSystemLaserAction(TETE_LASER_I,IsAction ); 
        break;
    case SYSTEM_LASER_II_ACTION_COIL:
        SetSystemLaserAction(TETE_LASER_II,IsAction );
        break;
    case SYSTEM_LASER_III_ACTION_COIL:
        SetSystemLaserAction(TETE_LASER_III,IsAction );
        break;
    case SYSTEM_LASER_IV_ACTION_COIL:
        SetSystemLaserAction(TETE_LASER_IV,IsAction );
        break;
    case SYSTEM_TIME_TICK_COIL:
        SetSystemTimeTick(IsAction);
        break;
    case SYSTEM_AUTO_FLAG_COIL:
        SetSystemAutoFlagAction(IsAction);
        break;
    default:break;
    }
    return true;
}
int CMBControlProxy::OnHandleLasersCoilsChange(int iAddress,bool IsAction, uint8_t *pAddr)
{
    //TODO: Add your source code here
    uint8_t uLaserSel;
    
    if(iAddress<LASER2_COILS_MAP_ADDR)
    {
        uLaserSel = TETE_LASER_I;
        iAddress -= LASER1_COILS_MAP_ADDR;
    }
    else
    {
        uLaserSel = TETE_LASER_II;
        iAddress -= LASER2_COILS_MAP_ADDR;
    }

    switch(iAddress)
    {
    case LASER_INSERVICE_COIL:
        SetLaserInservice(uLaserSel,IsAction );
        break;
    case LASER_INITIAL_COIL:
        SetLaserInitial(uLaserSel,IsAction );
        break;
    case LASER_ON_COIL:
        //--LaserOn(uLaserSel,IsAction );
        break;
    case LASER_OFF_COIL:
        //--LaserOff(uLaserSel,IsAction );
        break;
    case LASER_PROGRUN_ORGIN_COIL:
        SetLaserProgRunOrgin(uLaserSel,IsAction );
        break;
    case LASER_PROGRUN_LOOP_COIL:
        SetLaserProgRunLoop(uLaserSel,IsAction );
        break;
    case LASER_MANUAL_ACTION_COIL:
        //--SetLaserAction(uLaserSel,IsAction );
        break;
    case LASER_WRITE_CAL_COIL:
        //--SetLaserParameterLoad(uLaserSel,IsAction );
        break;
    case WLASER_AUTO_RUN_COIL:
        break;
    case WLASER_PRM_LOAD_COIL:
        break;
    case WLASER_WRITE_MAP_COIL:
        break;
    case WLASER_WRITE_DRAW_COIL:
        break;
    case WLASER_DRAW_ACTION_COIL:
        break;
    case WLASER_TEMPLATE_ACTION_COIL:
        break;
    case WLASER_WRITE_CAL_COIL:
        break;
    case PLASER_AUTO_RUN_COIL:
        break;
    case PLASER_WRITE_MARKFILE_COIL:
        break;
    case PLASER_DRAW_ACTION_COIL:
        break;
    case PLASER_TEMPLATE_ACTION_COIL:
        break;
    default:break;
    }
    return true;
}
//------------------------------------------------------------------------------
int CMBControlProxy::OnSetCoilInfo(int iAddr,bool IsAction, uint8_t *pAddr)
{
    //--
    bool bFlag = false;
	m_pTheLog->WriteLog("TETE MODBUS:>WriteCoil=%05d,value=%d",iAddr,IsAction);
    /*
    //--将modbus实际地址转换为映射表地址
    int iAddress = iAddr - COILS_BASE_ADDR;

    if(iAddress<LASER1_COILS_MAP_ADDR)
        bFlag = OnHandleSystemCoilsChange(iAddress,IsAction,*pAddr);
    else
        bFlag = OnHandleLasersCoilsChange(iAddress,IsAction,*pAddr);
    */
    return bFlag;
}
//------------------------------------------------------------------------------
int CMBControlProxy::OnSetCoilsInfo(int iAddr,int iBitNum, uint8_t *pAddr)
{
    //--
    bool IsAction = false;
    int iAddress;
	m_pTheLog->WriteLog("TETE MODBUS:> WriteCoils addr=%05d,num=%d",iAddr,iBitNum);
    for(int i=0;i<iBitNum;i++)
    {
        iAddress = iAddr+i;
       
        return OnSetCoilInfo(iAddress,IsAction,pAddr);
    }

    return true;
}
//------------------------------------------------------------------------------
int CMBControlProxy::OnHandleSystemHoldRegsChange(int iAddress,int iRegValue, uint8_t *pAddr)
{
    //TODO: Add your source code here
	//--SYSTEMTIME stSysTime;
    union {
        float fValue;
        uint8_t uValue[4];
        uint16_t wValue[2];
        int iValue;
    }unValue;
    //--uint8_t cFileName[100];
    unValue.iValue = iRegValue;

    if((iAddress>SYS_HOLDREG_MAP_ADDR)&&(iAddress<LASER1_HOLDREG_MAP_ADDR))
    {
        iAddress -= SYS_HOLDREG_MAP_ADDR;
        switch(iAddress)
        {
        case SYSTEM_RTC_YEAR_REG:
            //--GetSystemRTC(&stSysTime);            
            //--stSysTime.wYear = iRegValue;
            //--SetSystemRTC(&stSysTime);
            m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_RTC_Year = iRegValue;
            break;
        case SYSTEM_RTC_MONTH_REG:
            //--GetSystemRTC(&stSysTime);
            //--stSysTime.wMonth = iRegValue;
            //--SetSystemRTC(&stSysTime);
            m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_RTC_Month = iRegValue;
            break;
        case SYSTEM_RTC_DATE_REG:
            //--GetSystemRTC(&stSysTime);
            //--stSysTime.wDay = iRegValue;
            //--SetSystemRTC(&stSysTime);
            m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_RTC_Date = iRegValue;
            break;
        case SYSTEM_RTC_HOUR_REG:
            //--GetSystemRTC(&stSysTime);
            //--stSysTime.wHour = iRegValue;
            //--SetSystemRTC(&stSysTime);
            m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_RTC_Hour = iRegValue;
            break;
        case SYSTEM_RTC_MIN_REG:
            //--GetSystemRTC(&stSysTime);
            //--stSysTime.wMinute = iRegValue;
            //--SetSystemRTC(&stSysTime);
            m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_RTC_Min = iRegValue;
            break;
        case SYSTEM_RTC_SEC_REG:
            //--GetSystemRTC(&stSysTime);
            //--stSysTime.wSecond = iRegValue;
            //--SetSystemRTC(&stSysTime);
            m_stMBMapInfoSets.stHoldRegsInfo.stSystemHoldRegsInfo.System_RTC_Sec = iRegValue;
            break;
            /*
        case SYSTEM_LC_I_OS_X_REG:
            readHoldRegsUnitInfo(SYSTEM_LC_I_OS_X_REG,unValue.wValue, 2);
            SetSystemLaserCenterOffset(LASER_I,LC_OS_X, unValue.fValue);
            break;
        case SYSTEM_LC_I_OS_Y_REG:
            readHoldRegsUnitInfo(SYSTEM_LC_I_OS_Y_REG,unValue.wValue, 2);
            SetSystemLaserCenterOffset(LASER_I,LC_OS_Y, unValue.fValue);
            break;
        case SYSTEM_LC_I_OS_THETA_REG:
            readHoldRegsUnitInfo(SYSTEM_LC_I_OS_Y_REG,unValue.wValue, 2);
            SetSystemLaserCenterOffset(LASER_I,LC_OS_THETA, unValue.fValue);
            break;
        case SYSTEM_LC_II_OS_X_REG:
            readHoldRegsUnitInfo(SYSTEM_LC_II_OS_X_REG,unValue.wValue, 2);
            SetSystemLaserCenterOffset(LASER_II,LC_OS_X, unValue.fValue);
            break;
        case SYSTEM_LC_II_OS_Y_REG:
            readHoldRegsUnitInfo(SYSTEM_LC_II_OS_Y_REG,unValue.wValue, 2);
            SetSystemLaserCenterOffset(LASER_II,LC_OS_Y, unValue.fValue);
            break;
        case SYSTEM_LC_II_OS_THETA_REG:
            readHoldRegsUnitInfo(SYSTEM_LC_II_OS_Y_REG,unValue.wValue, 2);
            SetSystemLaserCenterOffset(LASER_II,LC_OS_THETA, unValue.fValue);
            break;
        case SYSTEM_LC_III_OS_X_REG:
            readHoldRegsUnitInfo(SYSTEM_LC_III_OS_X_REG,unValue.wValue, 2);
            SetSystemLaserCenterOffset(LASER_III,LC_OS_X, unValue.fValue);
            break;
        case SYSTEM_LC_III_OS_Y_REG:
            readHoldRegsUnitInfo(SYSTEM_LC_III_OS_Y_REG,unValue.wValue, 2);
            SetSystemLaserCenterOffset(LASER_III,LC_OS_Y, unValue.fValue);
            break;
        case SYSTEM_LC_III_OS_THETA_REG:
            readHoldRegsUnitInfo(SYSTEM_LC_III_OS_Y_REG,unValue.wValue, 2);
            SetSystemLaserCenterOffset(LASER_III,LC_OS_THETA, unValue.fValue);
            break;
        case SYSTEM_LC_IV_OS_X_REG:
            readHoldRegsUnitInfo(SYSTEM_LC_IV_OS_X_REG,unValue.wValue, 2);
            SetSystemLaserCenterOffset(LASER_IV,LC_OS_X, unValue.fValue);
            break;
        case SYSTEM_LC_IV_OS_Y_REG:
            readHoldRegsUnitInfo(SYSTEM_LC_IV_OS_Y_REG,unValue.wValue, 2);
            SetSystemLaserCenterOffset(LASER_IV,LC_OS_Y, unValue.fValue);
            break;
        case SYSTEM_LC_IV_OS_THETA_REG:
            readHoldRegsUnitInfo(SYSTEM_LC_IV_OS_Y_REG,unValue.wValue, 2);
            SetSystemLaserCenterOffset(LASER_IV,LC_OS_THETA, unValue.fValue);
            break;
        case ACTION_TEMPLATE_FILENAME_I_REG:
            readHoldRegsUnitInfo(ACTION_TEMPLATE_FILENAME_I_REG,(uint16_t *)cFileName,50);
            SetActionTemplateFileName(LASER_I, cFileName);
            break;
        case ACTION_TEMPLATE_FILENAME_II_REG:
            readHoldRegsUnitInfo(ACTION_TEMPLATE_FILENAME_II_REG,(uint16_t *)cFileName,50);
            SetActionTemplateFileName(LASER_II, cFileName);
            break;
        case ACTION_TEMPLATE_FILENAME_III_REG:
            readHoldRegsUnitInfo(ACTION_TEMPLATE_FILENAME_III_REG,(uint16_t *)cFileName,50);
            SetActionTemplateFileName(LASER_III, cFileName);
            break;
        case ACTION_TEMPLATE_FILENAME_IV_REG:
            readHoldRegsUnitInfo(ACTION_TEMPLATE_FILENAME_IV_REG,(uint16_t *)cFileName,50);
            SetActionTemplateFileName(LASER_IV, cFileName);
            break;
        case ACTION_MAP_FILENAME_I_REG:
            readHoldRegsUnitInfo(ACTION_MAP_FILENAME_I_REG,(uint16_t *)cFileName,50);
            SetActionMapFileName(LASER_I, cFileName);
            break;
        case ACTION_MAP_FILENAME_II_REG:
            readHoldRegsUnitInfo(ACTION_MAP_FILENAME_II_REG,(uint16_t *)cFileName,50);
            SetActionMapFileName(LASER_II, cFileName);
            break;
        case ACTION_MAP_FILENAME_III_REG:
            readHoldRegsUnitInfo(ACTION_MAP_FILENAME_III_REG,(uint16_t *)cFileName,50);
            SetActionMapFileName(LASER_III, cFileName);
            break;
        case ACTION_MAP_FILENAME_IV_REG:
            readHoldRegsUnitInfo(ACTION_MAP_FILENAME_IV_REG,(uint16_t *)cFileName,50);
            SetActionMapFileName(LASER_IV, cFileName);
            break;
        case LASER_CALIBRATE_FILENAME_I_REG:
            readHoldRegsUnitInfo(LASER_CALIBRATE_FILENAME_I_REG,(uint16_t *)cFileName,50);
            SetLaserCalibrateFileName(LASER_I, cFileName);
            break;
        case LASER_CALIBRATE_FILENAME_II_REG:
            readHoldRegsUnitInfo(LASER_CALIBRATE_FILENAME_II_REG,(uint16_t *)cFileName,50);
            SetLaserCalibrateFileName(LASER_II, cFileName);
            break;
        case LASER_CALIBRATE_FILENAME_III_REG:
            readHoldRegsUnitInfo(LASER_CALIBRATE_FILENAME_III_REG,(uint16_t *)cFileName,50);
            SetLaserCalibrateFileName(LASER_III, cFileName);
            break;
        case LASER_CALIBRATE_FILENAME_IV_REG:
            readHoldRegsUnitInfo(LASER_CALIBRATE_FILENAME_IV_REG,(uint16_t *)cFileName,50);
            SetLaserCalibrateFileName(LASER_IV, cFileName);
            break;//--------<<
            */
        default:break;
        }
    }
    
    return true;
}

//------------------------------------------------------------------------------
int CMBControlProxy::OnHandleLasersHoldRegsChange(int iAddress,int iRegValue, uint8_t *pAddr)
{
    //TODO: Add your source code here
	//--SYSTEMTIME stSysTime;
    union {
        float fValue;
        uint8_t uValue[4];
        uint16_t wValue[2];
        int iValue;
    }unValue;
    //--uint8_t cFileName[100];
    unValue.iValue = iRegValue;
    uint8_t uLaserSel;

    bool bResult = false;

    if(iAddress<LASER2_HOLDREG_MAP_ADDR)
    {
        iAddress -= LASER1_HOLDREG_MAP_ADDR;
        uLaserSel = TETE_LASER_I;
    }
    else
    {
        iAddress -= LASER2_HOLDREG_MAP_ADDR;
        uLaserSel = TETE_LASER_II;
    }
    /*
    switch(iAddress)
    {
    case LASER_TEMPLATE_FILENAME_REG:        
        if(TETE_LASER_I==uLaserSel)
            readHoldRegsUnitInfo(LASER_TEMPLATE_FILENAME_REG+LASER1_HOLDREG_MAP_ADDR,(uint16_t *)cFileName,50);
        else
            readHoldRegsUnitInfo(LASER_TEMPLATE_FILENAME_REG+LASER2_HOLDREG_MAP_ADDR,(uint16_t *)cFileName,50);
        bResult = SetLaserTemplateFile(uLaserSel, cFileName);
        break;
    case LASER_MAP_FILENAME_REG:
        if(TETE_LASER_I==uLaserSel)
            readHoldRegsUnitInfo(LASER_MAP_FILENAME_REG+LASER1_HOLDREG_MAP_ADDR,(uint16_t *)cFileName,50);
        else
            readHoldRegsUnitInfo(LASER_MAP_FILENAME_REG+LASER2_HOLDREG_MAP_ADDR,(uint16_t *)cFileName,50);
        bResult = SetLaserMapFile(uLaserSel, cFileName);
        break;
    case LASER_CALIBRATE_FILENAME_I_REG:
        if(TETE_LASER_I==uLaserSel)
            readHoldRegsUnitInfo(LASER_CALIBRATE_FILENAME_I_REG+LASER1_HOLDREG_MAP_ADDR,(uint16_t *)cFileName,50);
        else
            readHoldRegsUnitInfo(LASER_CALIBRATE_FILENAME_I_REG+LASER2_HOLDREG_MAP_ADDR,(uint16_t *)cFileName,50);
        bResult = SetLaserCalibrateFile(uLaserSel,1, cFileName);
        break;
    case LASER_CALIBRATE_FILENAME_II_REG:
        if(TETE_LASER_I==uLaserSel)
            readHoldRegsUnitInfo(LASER_CALIBRATE_FILENAME_II_REG+LASER1_HOLDREG_MAP_ADDR,(uint16_t *)cFileName,50);
        else
            readHoldRegsUnitInfo(LASER_CALIBRATE_FILENAME_II_REG+LASER2_HOLDREG_MAP_ADDR,(uint16_t *)cFileName,50);
        bResult = SetLaserCalibrateFile(uLaserSel,2, cFileName);
        break;
    case LASER_MC_CONTROLDATA_0_REG:
        bResult = SetLaserMCControlData(uLaserSel,0,iRegValue);
        break;
    case LASER_MC_CONTROLDATA_1_REG:
        bResult = SetLaserMCControlData(uLaserSel,1,iRegValue);
        break;
    case LASER_MC_CONTROLDATA_2_REG:
        bResult = SetLaserMCControlData(uLaserSel,2,iRegValue);
        break;
    case LASER_MC_CONTROLDATA_3_REG:
        bResult = SetLaserMCControlData(uLaserSel,3,iRegValue);
        break;
    case LASER_MC_CONTROLDATA_4_REG:
        bResult = SetLaserMCControlData(uLaserSel,4,iRegValue);
        break;
    case LASER_MC_CONTROLDATA_5_REG:
        bResult = SetLaserMCControlData(uLaserSel,5,iRegValue);
        break;
    case LASER_MC_CONTROLDATA_6_REG:
        bResult = SetLaserMCControlData(uLaserSel,6,iRegValue);
        break;
    case LASER_MC_CONTROLDATA_7_REG:
        bResult = SetLaserMCControlData(uLaserSel,7,iRegValue);
        break;
    case LASER_MC_CONTROLDATA_8_REG:
        bResult = SetLaserMCControlData(uLaserSel,8,iRegValue);
        break;
    case LASER_MC_CONTROLDATA_9_REG:
        bResult = SetLaserMCControlData(uLaserSel,9,iRegValue);
        break;
    case LASER_MC_CONTROLDATA_10_REG:
        bResult = SetLaserMCControlData(uLaserSel,10,iRegValue);
        break;
    case LASER_MC_CONTROLDATA_11_REG:
        bResult = SetLaserMCControlData(uLaserSel,11,iRegValue);
        break;
    case LASER_MC_CONTROLDATA_12_REG:
        bResult = SetLaserMCControlData(uLaserSel,12,iRegValue);
        break;
    case LASER_MC_CONTROLDATA_13_REG:
        bResult = SetLaserMCControlData(uLaserSel,13,iRegValue);
        break;
    case LASER_MC_CONTROLDATA_14_REG:
        bResult = SetLaserMCControlData(uLaserSel,14,iRegValue);
        break;
    case LASER_MC_CONTROLDATA_15_REG:
        bResult = SetLaserMCControlData(uLaserSel,15,iRegValue);
        break;//----<<
    case AXIS_X_TARGETPOS_1_REG:
        readHoldRegsUnitInfo(AXIS_X_TARGETPOS_1_REG,unValue.wValue, 2);
        bResult = SetAxisTargetPos(uLaserSel,AXIS_X,AXIS_TARGETPOS_1, unValue.fValue);
        break;
    case AXIS_X_TARGETPOS_2_REG:
        readHoldRegsUnitInfo( AXIS_X_TARGETPOS_2_REG,unValue.wValue, 2);
        bResult = SetAxisTargetPos(uLaserSel,AXIS_X,AXIS_TARGETPOS_2, unValue.fValue);
        break;
    case AXIS_X_TARGETPOS_3_REG:
        readHoldRegsUnitInfo(AXIS_X_TARGETPOS_3_REG,unValue.wValue, 2);
        bResult = SetAxisTargetPos(uLaserSel,AXIS_X,AXIS_TARGETPOS_3, unValue.fValue);
        break;
    case AXIS_X_TARGETPOS_4_REG:
        readHoldRegsUnitInfo(AXIS_X_TARGETPOS_4_REG,unValue.wValue, 2);
        bResult = SetAxisTargetPos(uLaserSel,AXIS_X,AXIS_TARGETPOS_4, unValue.fValue);
        break;
    case AXIS_X_TARGETPOS_MANUL_REG:
        readHoldRegsUnitInfo(AXIS_X_TARGETPOS_MANUL_REG,unValue.wValue, 2);
        bResult = SetAxisTargetPos(uLaserSel,AXIS_X,AXIS_TARGETPOS_MANUL, unValue.fValue);
        break;
    case AXIS_X_CONTROLDATA_2_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_X,2,iRegValue);
        break;
    case AXIS_X_CONTROLDATA_3_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_X,3,iRegValue);
        break;
    case AXIS_X_CONTROLDATA_4_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_X,4,iRegValue);
        break;
    case AXIS_X_CONTROLDATA_5_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_X,5,iRegValue);
        break;
    case AXIS_X_CONTROLDATA_6_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_X,6,iRegValue);
        break;
    case AXIS_X_CONTROLDATA_7_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_X,7,iRegValue);
        break;
    case AXIS_X_CONTROLDATA_8_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_X,8,iRegValue);
        break;
    case AXIS_X_CONTROLDATA_9_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_X,9,iRegValue);
        break;
    case AXIS_X_CONTROLDATA_10_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_X,10,iRegValue);
        break;
    case AXIS_X_CONTROLDATA_11_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_X,11,iRegValue);
        break;
    case AXIS_X_CONTROLDATA_12_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_X,12,iRegValue);
        break;
    case AXIS_X_CONTROLDATA_13_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_X,13,iRegValue);
        break;
    case AXIS_X_CONTROLDATA_14_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_X,14,iRegValue);
        break;
    case AXIS_X_CONTROLDATA_15_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_X,15,iRegValue);
        break;//----Axis X----<<
    case AXIS_Y_TARGETPOS_1_REG:
        readHoldRegsUnitInfo(AXIS_Y_TARGETPOS_1_REG,unValue.wValue, 2);
        bResult = SetAxisTargetPos(uLaserSel,AXIS_Y,AXIS_TARGETPOS_1, unValue.fValue);
        break;
    case AXIS_Y_TARGETPOS_2_REG:
        readHoldRegsUnitInfo(AXIS_Y_TARGETPOS_2_REG,unValue.wValue, 2);
        bResult = SetAxisTargetPos(uLaserSel,AXIS_Y,AXIS_TARGETPOS_2, unValue.fValue);
        break;
    case AXIS_Y_TARGETPOS_3_REG:
        readHoldRegsUnitInfo(AXIS_Y_TARGETPOS_3_REG,unValue.wValue, 2);
        bResult = SetAxisTargetPos(uLaserSel,AXIS_Y,AXIS_TARGETPOS_3, unValue.fValue);
        break;
    case AXIS_Y_TARGETPOS_4_REG:
        readHoldRegsUnitInfo(AXIS_Y_TARGETPOS_4_REG,unValue.wValue, 2);
        bResult = SetAxisTargetPos(uLaserSel,AXIS_Y,AXIS_TARGETPOS_4, unValue.fValue);
        break;
    case AXIS_Y_TARGETPOS_MANUL_REG:
        readHoldRegsUnitInfo(AXIS_Y_TARGETPOS_MANUL_REG,unValue.wValue, 2);
        bResult = SetAxisTargetPos(uLaserSel,AXIS_Y,AXIS_TARGETPOS_MANUL, unValue.fValue);
        break;
    case AXIS_Y_CONTROLDATA_2_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_Y,2,iRegValue);
        break;
    case AXIS_Y_CONTROLDATA_3_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_Y,3,iRegValue);
        break;
    case AXIS_Y_CONTROLDATA_4_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_Y,4,iRegValue);
        break;
    case AXIS_Y_CONTROLDATA_5_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_Y,5,iRegValue);
        break;
    case AXIS_Y_CONTROLDATA_6_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_Y,6,iRegValue);
        break;
    case AXIS_Y_CONTROLDATA_7_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_Y,7,iRegValue);
        break;
    case AXIS_Y_CONTROLDATA_8_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_Y,8,iRegValue);
        break;
    case AXIS_Y_CONTROLDATA_9_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_Y,9,iRegValue);
        break;
    case AXIS_Y_CONTROLDATA_10_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_Y,10,iRegValue);
        break;
    case AXIS_Y_CONTROLDATA_11_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_Y,11,iRegValue);
        break;
    case AXIS_Y_CONTROLDATA_12_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_Y,12,iRegValue);
        break;
    case AXIS_Y_CONTROLDATA_13_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_Y,13,iRegValue);
        break;
    case AXIS_Y_CONTROLDATA_14_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_Y,14,iRegValue);
        break;
    case AXIS_Y_CONTROLDATA_15_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_Y,15,iRegValue);
        break;//----Axis Y----<<
    case AXIS_Z_TARGETPOS_1_REG:
        readHoldRegsUnitInfo(AXIS_Z_TARGETPOS_1_REG,unValue.wValue, 2);
        bResult = SetAxisTargetPos(uLaserSel,AXIS_Z,AXIS_TARGETPOS_1, unValue.fValue);
        break;
    case AXIS_Z_TARGETPOS_2_REG:
        readHoldRegsUnitInfo(AXIS_Z_TARGETPOS_2_REG,unValue.wValue, 2);
        bResult = SetAxisTargetPos(uLaserSel,AXIS_Z,AXIS_TARGETPOS_2, unValue.fValue);
        break;
    case AXIS_Z_TARGETPOS_3_REG:
        readHoldRegsUnitInfo(AXIS_Z_TARGETPOS_3_REG,unValue.wValue, 2);
        bResult = SetAxisTargetPos(uLaserSel,AXIS_Z,AXIS_TARGETPOS_3, unValue.fValue);
        break;
    case AXIS_Z_TARGETPOS_4_REG:
        readHoldRegsUnitInfo(AXIS_Z_TARGETPOS_4_REG,unValue.wValue, 2);
        SetAxisTargetPos(uLaserSel,AXIS_Z,AXIS_TARGETPOS_4, unValue.fValue);
        break;
    case AXIS_Z_TARGETPOS_MANUL_REG:
        readHoldRegsUnitInfo(AXIS_Z_TARGETPOS_MANUL_REG,unValue.wValue, 2);
        bResult = SetAxisTargetPos(uLaserSel,AXIS_Z,AXIS_TARGETPOS_MANUL, unValue.fValue);
        break;
    case AXIS_Z_CONTROLDATA_2_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_Z,2,iRegValue);
        break;
    case AXIS_Z_CONTROLDATA_3_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_Z,3,iRegValue);
        break;
    case AXIS_Z_CONTROLDATA_4_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_Z,4,iRegValue);
        break;
    case AXIS_Z_CONTROLDATA_5_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_Z,5,iRegValue);
        break;
    case AXIS_Z_CONTROLDATA_6_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_Z,6,iRegValue);
        break;
    case AXIS_Z_CONTROLDATA_7_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_Z,7,iRegValue);
        break;
    case AXIS_Z_CONTROLDATA_8_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_Z,8,iRegValue);
        break;
    case AXIS_Z_CONTROLDATA_9_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_Z,9,iRegValue);
        break;
    case AXIS_Z_CONTROLDATA_10_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_Z,10,iRegValue);
        break;
    case AXIS_Z_CONTROLDATA_11_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_Z,11,iRegValue);
        break;
    case AXIS_Z_CONTROLDATA_12_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_Z,12,iRegValue);
        break;
    case AXIS_Z_CONTROLDATA_13_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_Z,13,iRegValue);
        break;
    case AXIS_Z_CONTROLDATA_14_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_Z,14,iRegValue);
        break;
    case AXIS_Z_CONTROLDATA_15_REG:
        bResult = SetAxisControlData(uLaserSel,AXIS_Z,15,iRegValue);
        break;//----Axis Z----<<    ---------LASER I----<<
    default:break;
    }
    */
    return bResult;
}
//------------------------------------------------------------------------------
int CMBControlProxy::OnSetHoldRegInfo(int iAddr,int iRegValue, uint8_t *pAddr)
{
    //--
    bool bFlag;
    //--将modbus实际地址转换为映射表地址
    int iAddress = iAddr - HOLDREG_BASE_ADDR;
    
    if(iAddress<LASER1_HOLDREG_MAP_ADDR)
        bFlag = OnHandleSystemHoldRegsChange(iAddress,iRegValue,pAddr);
    else
        bFlag = OnHandleLasersHoldRegsChange(iAddress,iRegValue,pAddr);

    return bFlag;
}
//------------------------------------------------------------------------------
int CMBControlProxy::OnSetHoldRegsInfo(int iAddress,int iRegNum, uint8_t *pAddr)
{
    //--
    bool bFlag = false;
    //--uint16_t wValue;
	m_pTheLog->WriteLog("TETE MODBUS:> WriteHoldRegs addr=%05d,num=%d",iAddress,iRegNum);
    /*
    for(int i=0;i<iRegNum;i++)
    {
        //--iAddress += i;
        readHoldRegsUnitInfo(iAddress+i,&wValue, 1);
        bFlag = OnHandleHoldRegChange(iAddress+i,wValue);
        if(!bFlag)
            break;
    }
    */
    return bFlag;
}
//------------------------------------------------------------------------------
/*
iAddr:
    the First Coils Addr
iRegnum:
    the Num of Coils
*/
int CMBControlProxy::OnGetCoilsInfo(int iAddr,int iBitNum, uint8_t *pDataBuff)
{
	m_pTheLog->WriteLog("TETE MODBUS:> ReadCoils addr=%05d,num=%d\n",iAddr,iBitNum);
    //--将modbus实际地址转换为映射表地址
    int iStartAddr = iAddr-COILS_BASE_ADDR;
#if 1    
    int iCoilCnt=0;
    uint8_t uLaserSel;
    for(int i=0;i<iBitNum;i++)
    {
        int iCoilAddr = iStartAddr+i;
        
        if((iCoilAddr>SYS_COILS_MAP_ADDR)&&(iCoilAddr<LASER1_COILS_MAP_ADDR))
        {
            switch(iCoilAddr)
            {
            case SYSTEM_LASER_I_ACTION_COIL:
                pDataBuff[iCoilCnt++] = GetSystemLaserAction(TETE_LASER_I);
                break;
            case SYSTEM_LASER_II_ACTION_COIL:
                pDataBuff[iCoilCnt++] = GetSystemLaserAction(TETE_LASER_II);
                break;
            case SYSTEM_LASER_III_ACTION_COIL:
                pDataBuff[iCoilCnt++] = GetSystemLaserAction(TETE_LASER_III);
                break;
            case SYSTEM_LASER_IV_ACTION_COIL:
                pDataBuff[iCoilCnt++] = GetSystemLaserAction(TETE_LASER_IV);
                break;
            case SYSTEM_TIME_TICK_COIL:
                pDataBuff[iCoilCnt++] = GetSystemTimeTick(true);
                break;
            case SYSTEM_AUTO_FLAG_COIL:
                pDataBuff[iCoilCnt++] = GetSystemAutoFlagAction(true);
                break;
            default:break;
            }
        }
        else
        {
            if(iCoilAddr<LASER2_COILS_MAP_ADDR)
            {
                uLaserSel = TETE_LASER_I;
                iCoilAddr -= LASER1_COILS_MAP_ADDR;
            }
            else
            {
                uLaserSel = TETE_LASER_II;
                iCoilAddr -= LASER2_COILS_MAP_ADDR;
            }
            
            switch(iCoilAddr)
            {
            case LASER_INSERVICE_COIL:
                pDataBuff[iCoilCnt++] = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].Laser_Inservice;
                break;
            case LASER_INITIAL_COIL:
                pDataBuff[iCoilCnt++] = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].Laser_Initial;
                break;
            case LASER_ON_COIL:
                pDataBuff[iCoilCnt++] = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].Laser_On;
                break;
            case LASER_OFF_COIL:
                pDataBuff[iCoilCnt++] = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].Laser_Off;
                break;
            case LASER_PROGRUN_ORGIN_COIL:
                pDataBuff[iCoilCnt++] = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].Laser_ProgRun_Orgin;
                break;
            case LASER_PROGRUN_LOOP_COIL:
                pDataBuff[iCoilCnt++] = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].Laser_ProgRun_Loop;
                break;
            case LASER_MANUAL_ACTION_COIL:
                pDataBuff[iCoilCnt++] = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].Laser_Manual_Action;
                break;
            case LASER_WRITE_CAL_COIL:
                pDataBuff[iCoilCnt++] = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].Laser_Write_Cal;
                break;
            case WLASER_AUTO_RUN_COIL:
                pDataBuff[iCoilCnt++] = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].WLaser_AutoRun;
                break;
            case WLASER_PRM_LOAD_COIL:
                pDataBuff[iCoilCnt++] = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].WLaser_ParameterLoad;
                break;
            case WLASER_WRITE_MAP_COIL:
                pDataBuff[iCoilCnt++] = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].WLaser_Write_Map;
                break;
            case WLASER_WRITE_DRAW_COIL:
                pDataBuff[iCoilCnt++] = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].WLaser_Write_Draw;
                break;
            case WLASER_DRAW_ACTION_COIL:
                pDataBuff[iCoilCnt++] = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].WLaser_Draw_Action;
                break;
            case WLASER_TEMPLATE_ACTION_COIL:
                pDataBuff[iCoilCnt++] = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].WLaser_Template_Action;
                break;
            case WLASER_WRITE_CAL_COIL:
                pDataBuff[iCoilCnt++] = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].WLaser_WriteCal_Area2;
                break;
            case PLASER_AUTO_RUN_COIL:
                pDataBuff[iCoilCnt++] = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].PLaser_AutoRun;
                break;
            case PLASER_WRITE_MARKFILE_COIL:
                pDataBuff[iCoilCnt++] = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].PLaser_Write_MarkFile;
                break;
            case PLASER_DRAW_ACTION_COIL:
                pDataBuff[iCoilCnt++] = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].PLaser_Draw_Action;
                break;
            case PLASER_TEMPLATE_ACTION_COIL:
                pDataBuff[iCoilCnt++] = m_stMBMapInfoSets.stCoilsInfo.stLaserCoilsInfo[uLaserSel].PLaser_Template_Action;
                break;
            default:break;
            }
        }
    }
#endif
    if(iCoilCnt!=iBitNum)
        return -1;
	return iCoilCnt;
}
/*
iAddr:
    the First Coils Addr
iRegnum:
    the Num of Coils
*/
int CMBControlProxy::OnGetDiscreteInputsInfo(int iAddr,int iBitNum, uint8_t *pDataBuff)
{
    /*union {
        float fValue;
        uint8_t uValue[4];
        uint16_t wValue[2];
        uint32_t dwValue;
    }unValue;*/
	m_pTheLog->WriteLog("TETE MODBUS:> ReadCoils addr=%05d,num=%d\n",iAddr,iBitNum);
    //--将modbus实际地址转换为映射表地址
    int iStartAddr = iAddr-COILS_BASE_ADDR;
#if 1    
    int iCoilCnt=0;
    uint8_t uLaserSel;
    for(int i=0;i<iBitNum;i++)
    {
        int iCoilAddr = iStartAddr+i;
        
        if((iCoilAddr>SYS_COILS_MAP_ADDR)&&(iCoilAddr<LASER1_COILS_MAP_ADDR))
        {
            switch(iCoilAddr)
            {
            case SYSTEM_LASER_I_ACTION_COIL:
                pDataBuff[iCoilCnt++] = GetSystemLaserAction(TETE_LASER_I);
                break;
            case SYSTEM_LASER_II_ACTION_COIL:
                pDataBuff[iCoilCnt++] = GetSystemLaserAction(TETE_LASER_II);
                break;
            case SYSTEM_LASER_III_ACTION_COIL:
                pDataBuff[iCoilCnt++] = GetSystemLaserAction(TETE_LASER_III);
                break;
            case SYSTEM_LASER_IV_ACTION_COIL:
                pDataBuff[iCoilCnt++] = GetSystemLaserAction(TETE_LASER_IV);
                break;
            case SYSTEM_TIME_TICK_COIL:
                pDataBuff[iCoilCnt++] = GetSystemTimeTick(true);
                break;
            case SYSTEM_AUTO_FLAG_COIL:
                pDataBuff[iCoilCnt++] = GetSystemAutoFlagAction(true);
                break;
            default:break;
            }
        }
        else
        {
            if(iCoilAddr<LASER2_COILS_MAP_ADDR)
            {
                uLaserSel = TETE_LASER_I;
                iCoilAddr -= LASER1_COILS_MAP_ADDR;
            }
            else
            {
                uLaserSel = TETE_LASER_II;
                iCoilAddr -= LASER2_COILS_MAP_ADDR;
            }
            
            switch(iCoilAddr)
            {
            case LASER_INSERVICE_COIL:
                pDataBuff[iCoilCnt++] = GetLaserInservice(uLaserSel);
                break;
            case LASER_INITIAL_COIL:
                pDataBuff[iCoilCnt++] = GetLaserInitial(uLaserSel);
                break;
            case LASER_PROGRUN_ORGIN_COIL:
                pDataBuff[iCoilCnt++] = GetLaserProgRunOrgin(uLaserSel);
                break;
            case LASER_PROGRUN_LOOP_COIL:
                pDataBuff[iCoilCnt++] = GetLaserProgRunLoop(uLaserSel);
                break;
            default:break;
            }
        }
    }
#endif
    if(iCoilCnt!=iBitNum)
        return -1;
	return iCoilCnt;
}
//------------------------------------------------------------------------------
/*
iAddr:
    the First Register Addr
iRegnum:
    the Num of Register
*/
int CMBControlProxy::OnGetInputRegsInfo(int iAddr,int iRegNum, uint8_t *pAddr)
{
    /*
    union {
        float fValue;
        uint8_t uValue[4];
        uint16_t wValue[2];
        uint32_t dwValue;
    }unValue;
    */
    int iError=0;
    //--uint16_t wValue;
	m_pTheLog->WriteLog("TETE MODBUS:> ReadInputs addr=%05d,num=%d\n",iAddr,iRegNum);
    //--将modbus实际地址转换为映射表地址
    int iStartAddr = iAddr-INPUTREG_BASE_ADDR;
    
    int iAddress;
    uint8_t uLaserSel;
    for(int i=0;i<iRegNum;i++)
    {
        iAddress = iStartAddr+i;
        if(iAddress<LASER2_INPUTREG_MAP_ADDR)
        {
            uLaserSel = TETE_LASER_I;
            iAddress -= LASER1_INPUTREG_MAP_ADDR;
        }
        else
        {
            uLaserSel = TETE_LASER_II;
            iAddress -= LASER2_INPUTREG_MAP_ADDR;
        }
        switch(iAddress)
        {        
        case LASER_MC_READ_DATAIN_0_INPUT:
            iError = GetLaserMCReadDataIn(uLaserSel,0);
            break;
        case LASER_MC_READ_DATAIN_1_INPUT:
            iError = GetLaserMCReadDataIn(uLaserSel,1);
            break;
        case LASER_MC_READ_DATAIN_2_INPUT:
            iError = GetLaserMCReadDataIn(uLaserSel,2);
            break;
        case LASER_MC_READ_DATAIN_3_INPUT:
            iError = GetLaserMCReadDataIn(uLaserSel,3);
            break;
        case LASER_MC_READ_DATAIN_4_INPUT:
            iError = GetLaserMCReadDataIn(uLaserSel,4);
            break;
        case WLASER_MC_READ_STATUS_INPUT:
            iError = GetLaserMCReadStatus(uLaserSel);
            break;
        case WLASER_MC_READ_ERROR_INPUT:
            iError = GetLaserMCReadError(uLaserSel);
            break;
        case PLASER_MC_READ_STATUS_INPUT:
            iError = GetLaserMCReadStatus(uLaserSel);
            break;
        case PLASER_MC_READ_ERROR_INPUT:
            iError = GetLaserMCReadError(uLaserSel);
            break;
        default:break;
        }
    }
	
	return 0;
}
/*
iAddr:
    the First Register Addr
iRegnum:
    the Num of Register
*/
//------------------------------------------------------------------------------
int CMBControlProxy::OnGetHoldRegsInfo(int iAddr,int iRegNum, uint8_t *pAddr)
{
    //--uint8_t cFileName[100];
	m_pTheLog->WriteLog("TETE MODBUS:> ReadHoldRegs addr=%05d,num=%d\n",iAddr,iRegNum);
    //--将modbus实际地址转换为映射表地址
    int iStartAddr = iAddr-HOLDREG_BASE_ADDR;
    
    int iAddress;
    int iLaserSel;
    for(int i=0;i<iRegNum;i++)
    {
        iAddress = iStartAddr+i;
        if((iAddress>SYS_HOLDREG_MAP_ADDR)&&(iAddress<LASER1_HOLDREG_MAP_ADDR))
        {
			SYSTEMTIME  stSysTime;
            switch(iAddress)
            {
            case (SYS_HOLDREG_MAP_ADDR+ SYSTEM_RTC_YEAR_REG):
                GetSystemRTC(&stSysTime);
                //--m_wHoldRegs[iAddress] = stSysTime.wYear;
                break;
            case (SYS_HOLDREG_MAP_ADDR+ SYSTEM_RTC_MONTH_REG):
                GetSystemRTC(&stSysTime);
                //--m_wHoldRegs[iAddress] = stSysTime.wMonth;
                break;
            case (SYS_HOLDREG_MAP_ADDR+ SYSTEM_RTC_DATE_REG):
                GetSystemRTC(&stSysTime);
                //--m_wHoldRegs[iAddress] = stSysTime.wDay;
                break;
            case (SYS_HOLDREG_MAP_ADDR+ SYSTEM_RTC_HOUR_REG):
                GetSystemRTC(&stSysTime);
                //--m_wHoldRegs[iAddress] = stSysTime.wHour;
                break;
            case (SYS_HOLDREG_MAP_ADDR+ SYSTEM_RTC_MIN_REG):
                GetSystemRTC(&stSysTime);
                //--m_wHoldRegs[iAddress] = stSysTime.wMinute;
                break;
            case (SYS_HOLDREG_MAP_ADDR+ SYSTEM_RTC_SEC_REG):
                GetSystemRTC(&stSysTime);
                //--m_wHoldRegs[iAddress] = stSysTime.wSecond;
                break;
            case (SYS_HOLDREG_MAP_ADDR+ SYSTEM_TYPE_SEL_REG):
                break;
            case (SYS_HOLDREG_MAP_ADDR+ SYSTEM_LASER_SEL_REG):
                break;
            default:break;
            }
        }
        else if((iAddress>LASER1_HOLDREG_MAP_ADDR)&&(iAddress<LASER2_HOLDREG_MAP_ADDR))
        {
            //--iAddress -= LASER1_HOLDREG_BASE_ADDR;
            iLaserSel = TETE_LASER_I;
            /*
            switch(iAddress)
            {
            case (LASER1_HOLDREG_MAP_ADDR+ LASER_TEMPLATE_FILENAME_REG):
                GetLaserTemplateFile(iLaserSel, cFileName);
                writeHoldRegsUnitInfo(iAddress, (uint16_t *)cFileName,50);
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ LASER_MAP_FILENAME_REG):
                GetLaserMapFile(iLaserSel, cFileName);
                writeHoldRegsUnitInfo(iAddress, (uint16_t *)cFileName,50);
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ LASER_CALIBRATE_FILENAME_I_REG):
                GetLaserCalibrateFile(iLaserSel,1, cFileName);
                writeHoldRegsUnitInfo(iAddress, (uint16_t *)cFileName,50);
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ LASER_CALIBRATE_FILENAME_II_REG):
                GetLaserCalibrateFile(iLaserSel,2, cFileName);
                writeHoldRegsUnitInfo(iAddress, (uint16_t *)cFileName,50);
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ LASER_MC_CONTROLDATA_0_REG):
                unValue.wValue[0] = GetLaserMCControlData(iLaserSel,0);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ LASER_MC_CONTROLDATA_1_REG):
                unValue.wValue[0] = GetLaserMCControlData(iLaserSel,1);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ LASER_MC_CONTROLDATA_2_REG):
                unValue.wValue[0] = GetLaserMCControlData(iLaserSel,2);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ LASER_MC_CONTROLDATA_3_REG):
                unValue.wValue[0] = GetLaserMCControlData(iLaserSel,3);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ LASER_MC_CONTROLDATA_4_REG):
                unValue.wValue[0] = GetLaserMCControlData(iLaserSel,4);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ LASER_MC_CONTROLDATA_5_REG):
                unValue.wValue[0] = GetLaserMCControlData(iLaserSel,5);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ LASER_MC_CONTROLDATA_6_REG):
                unValue.wValue[0] = GetLaserMCControlData(iLaserSel,6);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ LASER_MC_CONTROLDATA_7_REG):
                unValue.wValue[0] = GetLaserMCControlData(iLaserSel,7);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ LASER_MC_CONTROLDATA_8_REG):
                unValue.wValue[0] = GetLaserMCControlData(iLaserSel,8);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ LASER_MC_CONTROLDATA_9_REG):
                unValue.wValue[0] = GetLaserMCControlData(iLaserSel,9);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ LASER_MC_CONTROLDATA_10_REG):
                unValue.wValue[0] = GetLaserMCControlData(iLaserSel,10);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ LASER_MC_CONTROLDATA_11_REG):
                unValue.wValue[0] = GetLaserMCControlData(iLaserSel,11);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ LASER_MC_CONTROLDATA_12_REG):
                unValue.wValue[0] = GetLaserMCControlData(iLaserSel,12);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ LASER_MC_CONTROLDATA_13_REG):
                unValue.wValue[0] = GetLaserMCControlData(iLaserSel,13);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ LASER_MC_CONTROLDATA_14_REG):
                unValue.wValue[0] = GetLaserMCControlData(iLaserSel,14);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+LASER_MC_CONTROLDATA_15_REG):
                unValue.wValue[0] = GetLaserMCControlData(iLaserSel,15);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;//----<<
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_X_TARGETPOS_1_REG):
                unValue.fValue = GetAxisTargetPos(iLaserSel,AXIS_X,AXIS_TARGETPOS_1);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                m_wHoldRegs[iAddress+1] = unValue.wValue[1];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_X_TARGETPOS_2_REG):
                unValue.fValue = GetAxisTargetPos(iLaserSel,AXIS_X,AXIS_TARGETPOS_2);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                m_wHoldRegs[iAddress+1] = unValue.wValue[1];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_X_TARGETPOS_3_REG):
                unValue.fValue = GetAxisTargetPos(iLaserSel,AXIS_X,AXIS_TARGETPOS_3);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                m_wHoldRegs[iAddress+1] = unValue.wValue[1];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_X_TARGETPOS_4_REG):
                unValue.fValue = GetAxisTargetPos(iLaserSel,AXIS_X,AXIS_TARGETPOS_4);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                m_wHoldRegs[iAddress+1] = unValue.wValue[1];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_X_TARGETPOS_MANUL_REG):
                unValue.fValue = GetAxisTargetPos(iLaserSel,AXIS_X,AXIS_TARGETPOS_MANUL);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                m_wHoldRegs[iAddress+1] = unValue.wValue[1];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_X_CONTROLDATA_2_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_X,2);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_X_CONTROLDATA_3_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_X,3);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_X_CONTROLDATA_4_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_X,4);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_X_CONTROLDATA_5_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_X,5);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_X_CONTROLDATA_6_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_X,6);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_X_CONTROLDATA_7_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_X,7);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_X_CONTROLDATA_8_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_X,8);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_X_CONTROLDATA_9_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_X,9);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_X_CONTROLDATA_10_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_X,10);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_X_CONTROLDATA_11_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_X,11);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_X_CONTROLDATA_12_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_X,12);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_X_CONTROLDATA_13_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_X,13);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_X_CONTROLDATA_14_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_X,14);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_X_CONTROLDATA_15_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_X,15);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;//----Axis X----<<
            case (LASER1_HOLDREG_MAP_ADDR + AXIS_Y_TARGETPOS_1_REG):
                unValue.fValue = GetAxisTargetPos(iLaserSel,AXIS_Y,AXIS_TARGETPOS_1);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                m_wHoldRegs[iAddress+1] = unValue.wValue[1];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_Y_TARGETPOS_2_REG):
                unValue.fValue = GetAxisTargetPos(iLaserSel,AXIS_Y,AXIS_TARGETPOS_2);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                m_wHoldRegs[iAddress+1] = unValue.wValue[1];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_Y_TARGETPOS_3_REG):
                unValue.fValue = GetAxisTargetPos(iLaserSel,AXIS_Y,AXIS_TARGETPOS_3);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                m_wHoldRegs[iAddress+1] = unValue.wValue[1];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_Y_TARGETPOS_4_REG):
                unValue.fValue = GetAxisTargetPos(iLaserSel,AXIS_Y,AXIS_TARGETPOS_4);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                m_wHoldRegs[iAddress+1] = unValue.wValue[1];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_Y_TARGETPOS_MANUL_REG):
                unValue.fValue = GetAxisTargetPos(iLaserSel,AXIS_Y,AXIS_TARGETPOS_MANUL);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                m_wHoldRegs[iAddress+1] = unValue.wValue[1];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_Y_CONTROLDATA_2_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Y,2);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_Y_CONTROLDATA_3_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Y,3);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_Y_CONTROLDATA_4_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Y,4);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_Y_CONTROLDATA_5_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Y,5);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_Y_CONTROLDATA_6_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Y,6);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_Y_CONTROLDATA_7_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Y,7);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_Y_CONTROLDATA_8_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Y,8);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_Y_CONTROLDATA_9_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Y,9);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_Y_CONTROLDATA_10_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Y,10);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_Y_CONTROLDATA_11_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Y,11);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_Y_CONTROLDATA_12_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Y,12);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_Y_CONTROLDATA_13_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Y,13);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_Y_CONTROLDATA_14_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Y,14);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_Y_CONTROLDATA_15_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Y,15);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;//----Axis Y----<<
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_Z_TARGETPOS_1_REG):
                unValue.fValue = GetAxisTargetPos(iLaserSel,AXIS_Z,AXIS_TARGETPOS_1);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                m_wHoldRegs[iAddress+1] = unValue.wValue[1];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_Z_TARGETPOS_2_REG):
                unValue.fValue = GetAxisTargetPos(iLaserSel,AXIS_Z,AXIS_TARGETPOS_2);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                m_wHoldRegs[iAddress+1] = unValue.wValue[1];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_Z_TARGETPOS_3_REG):
                unValue.fValue = GetAxisTargetPos(iLaserSel,AXIS_Z,AXIS_TARGETPOS_3);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                m_wHoldRegs[iAddress+1] = unValue.wValue[1];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_Z_TARGETPOS_4_REG):
                unValue.fValue = GetAxisTargetPos(iLaserSel,AXIS_Z,AXIS_TARGETPOS_4);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                m_wHoldRegs[iAddress+1] = unValue.wValue[1];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_Z_TARGETPOS_MANUL_REG):
                unValue.fValue = GetAxisTargetPos(iLaserSel,AXIS_Z,AXIS_TARGETPOS_MANUL);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                m_wHoldRegs[iAddress+1] = unValue.wValue[1];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_Z_CONTROLDATA_2_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Z,2);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_Z_CONTROLDATA_3_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Z,3);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_Z_CONTROLDATA_4_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Z,4);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_Z_CONTROLDATA_5_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Z,5);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_Z_CONTROLDATA_6_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Z,6);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_Z_CONTROLDATA_7_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Z,7);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_Z_CONTROLDATA_8_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Z,8);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_Z_CONTROLDATA_9_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Z,9);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_Z_CONTROLDATA_10_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Z,10);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_Z_CONTROLDATA_11_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Z,11);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_Z_CONTROLDATA_12_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Z,12);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_Z_CONTROLDATA_13_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Z,13);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_Z_CONTROLDATA_14_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Z,14);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER1_HOLDREG_MAP_ADDR+ AXIS_Z_CONTROLDATA_15_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Z,15);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;//----Axis Z----<<    ---------LASER I----<<
            default:break;
            }
            */
        }
        else if(iAddress>LASER2_HOLDREG_MAP_ADDR)
        {
            //--iAddress -= LASER2_HOLDREG_BASE_ADDR;
            iLaserSel = TETE_LASER_II;
            /*
            switch(iAddress)
            {
            case (LASER2_HOLDREG_MAP_ADDR+ LASER_TEMPLATE_FILENAME_REG):
                GetLaserTemplateFile(iLaserSel, cFileName);
                writeHoldRegsUnitInfo(iAddress, (uint16_t *)cFileName,50);
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ LASER_MAP_FILENAME_REG):
                GetLaserMapFile(iLaserSel, cFileName);
                writeHoldRegsUnitInfo(iAddress, (uint16_t *)cFileName,50);
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ LASER_CALIBRATE_FILENAME_I_REG):
                GetLaserCalibrateFile(iLaserSel,1, cFileName);
                writeHoldRegsUnitInfo(iAddress, (uint16_t *)cFileName,50);
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ LASER_CALIBRATE_FILENAME_II_REG):
                GetLaserCalibrateFile(iLaserSel,2, cFileName);
                writeHoldRegsUnitInfo(iAddress, (uint16_t *)cFileName,50);
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ LASER_MC_CONTROLDATA_0_REG):
                unValue.wValue[0] = GetLaserMCControlData(iLaserSel,0);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ LASER_MC_CONTROLDATA_1_REG):
                unValue.wValue[0] = GetLaserMCControlData(iLaserSel,1);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ LASER_MC_CONTROLDATA_2_REG):
                unValue.wValue[0] = GetLaserMCControlData(iLaserSel,2);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ LASER_MC_CONTROLDATA_3_REG):
                unValue.wValue[0] = GetLaserMCControlData(iLaserSel,3);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ LASER_MC_CONTROLDATA_4_REG):
                unValue.wValue[0] = GetLaserMCControlData(iLaserSel,4);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ LASER_MC_CONTROLDATA_5_REG):
                unValue.wValue[0] = GetLaserMCControlData(iLaserSel,5);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ LASER_MC_CONTROLDATA_6_REG):
                unValue.wValue[0] = GetLaserMCControlData(iLaserSel,6);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ LASER_MC_CONTROLDATA_7_REG):
                unValue.wValue[0] = GetLaserMCControlData(iLaserSel,7);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ LASER_MC_CONTROLDATA_8_REG):
                unValue.wValue[0] = GetLaserMCControlData(iLaserSel,8);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ LASER_MC_CONTROLDATA_9_REG):
                unValue.wValue[0] = GetLaserMCControlData(iLaserSel,9);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ LASER_MC_CONTROLDATA_10_REG):
                unValue.wValue[0] = GetLaserMCControlData(iLaserSel,10);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ LASER_MC_CONTROLDATA_11_REG):
                unValue.wValue[0] = GetLaserMCControlData(iLaserSel,11);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ LASER_MC_CONTROLDATA_12_REG):
                unValue.wValue[0] = GetLaserMCControlData(iLaserSel,12);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ LASER_MC_CONTROLDATA_13_REG):
                unValue.wValue[0] = GetLaserMCControlData(iLaserSel,13);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ LASER_MC_CONTROLDATA_14_REG):
                unValue.wValue[0] = GetLaserMCControlData(iLaserSel,14);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+LASER_MC_CONTROLDATA_15_REG):
                unValue.wValue[0] = GetLaserMCControlData(iLaserSel,15);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;//----<<
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_X_TARGETPOS_1_REG):
                unValue.fValue = GetAxisTargetPos(iLaserSel,AXIS_X,AXIS_TARGETPOS_1);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                m_wHoldRegs[iAddress+1] = unValue.wValue[1];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_X_TARGETPOS_2_REG):
                unValue.fValue = GetAxisTargetPos(iLaserSel,AXIS_X,AXIS_TARGETPOS_2);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                m_wHoldRegs[iAddress+1] = unValue.wValue[1];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_X_TARGETPOS_3_REG):
                unValue.fValue = GetAxisTargetPos(iLaserSel,AXIS_X,AXIS_TARGETPOS_3);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                m_wHoldRegs[iAddress+1] = unValue.wValue[1];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_X_TARGETPOS_4_REG):
                unValue.fValue = GetAxisTargetPos(iLaserSel,AXIS_X,AXIS_TARGETPOS_4);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                m_wHoldRegs[iAddress+1] = unValue.wValue[1];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_X_TARGETPOS_MANUL_REG):
                unValue.fValue = GetAxisTargetPos(iLaserSel,AXIS_X,AXIS_TARGETPOS_MANUL);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                m_wHoldRegs[iAddress+1] = unValue.wValue[1];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_X_CONTROLDATA_2_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_X,2);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_X_CONTROLDATA_3_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_X,3);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_X_CONTROLDATA_4_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_X,4);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_X_CONTROLDATA_5_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_X,5);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_X_CONTROLDATA_6_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_X,6);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_X_CONTROLDATA_7_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_X,7);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_X_CONTROLDATA_8_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_X,8);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_X_CONTROLDATA_9_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_X,9);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_X_CONTROLDATA_10_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_X,10);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_X_CONTROLDATA_11_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_X,11);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_X_CONTROLDATA_12_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_X,12);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_X_CONTROLDATA_13_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_X,13);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_X_CONTROLDATA_14_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_X,14);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_X_CONTROLDATA_15_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_X,15);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;//----Axis X----<<
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_Y_TARGETPOS_1_REG):
                unValue.fValue = GetAxisTargetPos(iLaserSel,AXIS_Y,AXIS_TARGETPOS_1);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                m_wHoldRegs[iAddress+1] = unValue.wValue[1];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_Y_TARGETPOS_2_REG):
                unValue.fValue = GetAxisTargetPos(iLaserSel,AXIS_Y,AXIS_TARGETPOS_2);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                m_wHoldRegs[iAddress+1] = unValue.wValue[1];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_Y_TARGETPOS_3_REG):
                unValue.fValue = GetAxisTargetPos(iLaserSel,AXIS_Y,AXIS_TARGETPOS_3);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                m_wHoldRegs[iAddress+1] = unValue.wValue[1];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_Y_TARGETPOS_4_REG):
                unValue.fValue = GetAxisTargetPos(iLaserSel,AXIS_Y,AXIS_TARGETPOS_4);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                m_wHoldRegs[iAddress+1] = unValue.wValue[1];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_Y_TARGETPOS_MANUL_REG):
                unValue.fValue = GetAxisTargetPos(iLaserSel,AXIS_Y,AXIS_TARGETPOS_MANUL);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                m_wHoldRegs[iAddress+1] = unValue.wValue[1];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_Y_CONTROLDATA_2_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Y,2);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_Y_CONTROLDATA_3_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Y,3);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_Y_CONTROLDATA_4_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Y,4);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_Y_CONTROLDATA_5_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Y,5);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_Y_CONTROLDATA_6_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Y,6);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_Y_CONTROLDATA_7_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Y,7);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_Y_CONTROLDATA_8_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Y,8);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_Y_CONTROLDATA_9_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Y,9);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_Y_CONTROLDATA_10_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Y,10);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_Y_CONTROLDATA_11_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Y,11);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_Y_CONTROLDATA_12_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Y,12);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_Y_CONTROLDATA_13_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Y,13);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_Y_CONTROLDATA_14_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Y,14);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_Y_CONTROLDATA_15_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Y,15);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;//----Axis Y----<<
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_Z_TARGETPOS_1_REG):
                unValue.fValue = GetAxisTargetPos(iLaserSel,AXIS_Z,AXIS_TARGETPOS_1);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                m_wHoldRegs[iAddress+1] = unValue.wValue[1];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_Z_TARGETPOS_2_REG):
                unValue.fValue = GetAxisTargetPos(iLaserSel,AXIS_Z,AXIS_TARGETPOS_2);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                m_wHoldRegs[iAddress+1] = unValue.wValue[1];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_Z_TARGETPOS_3_REG):
                unValue.fValue = GetAxisTargetPos(iLaserSel,AXIS_Z,AXIS_TARGETPOS_3);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                m_wHoldRegs[iAddress+1] = unValue.wValue[1];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_Z_TARGETPOS_4_REG):
                unValue.fValue = GetAxisTargetPos(iLaserSel,AXIS_Z,AXIS_TARGETPOS_4);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                m_wHoldRegs[iAddress+1] = unValue.wValue[1];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_Z_TARGETPOS_MANUL_REG):
                unValue.fValue = GetAxisTargetPos(iLaserSel,AXIS_Z,AXIS_TARGETPOS_MANUL);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                m_wHoldRegs[iAddress+1] = unValue.wValue[1];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_Z_CONTROLDATA_2_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Z,2);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_Z_CONTROLDATA_3_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Z,3);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_Z_CONTROLDATA_4_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Z,4);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_Z_CONTROLDATA_5_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Z,5);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_Z_CONTROLDATA_6_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Z,6);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_Z_CONTROLDATA_7_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Z,7);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_Z_CONTROLDATA_8_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Z,8);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_Z_CONTROLDATA_9_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Z,9);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_Z_CONTROLDATA_10_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Z,10);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_Z_CONTROLDATA_11_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Z,11);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_Z_CONTROLDATA_12_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Z,12);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_Z_CONTROLDATA_13_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Z,13);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_Z_CONTROLDATA_14_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Z,14);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;
            case (LASER2_HOLDREG_MAP_ADDR+ AXIS_Z_CONTROLDATA_15_REG):
                unValue.wValue[0] = GetAxisControlData(iLaserSel,AXIS_Z,15);
                m_wHoldRegs[iAddress] = unValue.wValue[0];
                break;//----Axis Z----<<    ---------LASER I----<<
            default:break;
            }
            */
        }
    }
	return 0;
}
