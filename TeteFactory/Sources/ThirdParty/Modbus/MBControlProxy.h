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

#ifndef MBControlProxyH
#define MBControlProxyH

#include <time.h>

#include "mbMappingTable.h"
#include "fMarkLog.h"

#define TETE_MODBUS_VERSION   "TETE_MODBUS_V1.00.00"

/* 激光器的索引 */
//--#define LASER_I      0
//--#define LASER_II     1
//--#define LASER_III    2
//--#define LASER_IV     3
/* 运动轴的索引 */
#define AXIS_X     0
#define AXIS_Y     1
#define AXIS_Z     2
/*the Offset of Laser Center*/
#define LC_OS_X        1
#define LC_OS_Y        2
#define LC_OS_THETA    3
/* 运动轴目标位置索引 */
#define AXIS_TARGETPOS_1         1
#define AXIS_TARGETPOS_2         2
#define AXIS_TARGETPOS_3         3
#define AXIS_TARGETPOS_4         4
#define AXIS_TARGETPOS_MANUL     5
/*Status list of Laser 31001/32001*/
#define LASER_STAT_READY        ((uint16_t)0x0001) //--1/0
#define LASER_STAT_ALARM        ((uint16_t)0x0002) //--0/1
#define LASER_STAT_RUN          ((uint16_t)0x0004) //--1/0
#define LASER_STAT_ORGINRUN     ((uint16_t)0x0008) //--1/0
#define LASER_STAT_LOOPRUN      ((uint16_t)0x0010) //--1/0
//--
#define LASER_STAT_RTC5         ((uint16_t)0x0020) //--1/0
#define LASER_STAT_CORR         ((uint16_t)0x0040) //--1/0
#define LASER_STAT_SHUTTER      ((uint16_t)0x0080) //--1/0
/*Error Code list of Laser 31002/32002*/
#define LASER_ERR_NORMAL        ((uint16_t)0)
#define LASER_ERR_RTC5          ((uint16_t)1)//--RTC5板卡初始化错误
#define LASER_ERR_COR           ((uint16_t)2)//--RTC5矫正错误
/*Status list of Motor 31251/31501/31751 32251/32501/32751*/
#define MOTOR_STAT_SERVERON               ((uint16_t)0x0001)  //--1/0
#define MOTOR_STAT_DRIVERALARM            ((uint16_t)0x0002)  //--0/1
#define MOTOR_STAT_RUNNING                ((uint16_t)0x0004)  //--1/0
#define MOTOR_STAT_INPOSITION             ((uint16_t)0x0008)  //--1/0
#define MOTOR_STAT_INTERPOLATIONMODE      ((uint16_t)0x0010)  //--1/0
#define MOTOR_STAT_HOMEMODE               ((uint16_t)0x0020)  //--1/0
#define MOTOR_STAT_JOGMODE                ((uint16_t)0x0040)  //--1/0
#define MOTOR_STAT_PAUSE                  ((uint16_t)0x0080)  //--1/0
#define MOTOR_STAT_DIRECTION              ((uint16_t)0x0100)  //--1/0
/*Error Code list of Motor 31252/31502/31752 32252/32502/32752*/
#define MOTOR_EC_NORMAL        ((uint16_t)0x0001)
//---------------------------------------------------------------------------

class CMBControlProxy
{
public :
    MBMapInfoSets_t  m_stMBMapInfoSets;//--Mark System Data Sources
    //--uint8_t      m_uCoilsBuff[10000];
    //--uint16_t    m_wInputRegsBuff[10000];
    //--uint16_t    m_wHoldRegsBuff[10000];
    /*
    uint8_t      *m_uCoilsBits;
    uint16_t    *m_wInputRegs;
    uint16_t    *m_wHoldRegs;
    */
protected :
private :
    CString m_sFileName;
public :
    CMBControlProxy();
    ~CMBControlProxy();
    //----Get MarkSystem Info ,Update Modbus Maping Tables ,Called By CModbus Server---->>
    int OnGetCoilsInfo(int iAddr,int iBitNum, uint8_t *pDataBuff);
    int OnGetDiscreteInputsInfo(int iAddr,int iBitNum, uint8_t *pDataBuff);
    int OnGetInputRegsInfo(int iAddr,int iRegNum, uint8_t *pDataBuff);
    int OnGetHoldRegsInfo(int iAddr,int iRegNu, uint8_t *pDataBuff);
    //----<<
    //----the interface of modbus how to control MarkSystem,Called by CModbus Server---->>    
    int OnSetCoilInfo(int iAddr,bool IsEnable, uint8_t *pAddr); //--设置单个线圈
    int OnSetCoilsInfo(int iAddr,int iBitNum, uint8_t *pAddr);    
    int OnSetHoldRegInfo(int iAddr,int iData, uint8_t *pAddr);
    int OnSetHoldRegsInfo(int iAddr,int iRegNum, uint8_t *pAddr);
    //----<<
protected :
    //----MarkSystem can Read/Write ,Called By MarkSystem---->>
    bool writeUnitInfo(uint32_t baseAddr, uint32_t dwOffset, uint8_t *pUnitBuff, int iSize);
    bool readUnitInfo(uint32_t baseAddr, uint32_t dwOffset, uint8_t *pUnitBuff, int iSize);
    bool writeUnitInfo(uint32_t baseAddr, uint32_t dwOffset, uint16_t *pUnitBuff, int iSize);
    bool readUnitInfo(uint32_t baseAddr, uint32_t dwOffset, uint16_t *pUnitBuff, int iSize);

    bool writeCoilsUnitInfo(uint32_t dwOffset, uint8_t *pUnitBuff, int iSize);
    bool writeInputRegsUnitInfo(uint32_t dwOffset, uint16_t *pUnitBuff, int iSize);
    bool writeHoldRegsUnitInfo(uint32_t dwOffset, uint16_t *pUnitBuff, int iSize);

    bool readCoilsUnitInfo(uint32_t dwOffset, uint8_t *pUnitBuff, int iSize);
    bool readInputRegsUnitInfo(uint32_t dwOffset, uint16_t *pUnitBuff, int iSize);
    bool readHoldRegsUnitInfo(uint32_t dwOffset, uint16_t *pUnitBuff, int iSize);
    //----<<
private :
    bool saveIntoFile(CString sFileName);
    bool loadFromFile(CString sFileName);
    //---- ---->>
    int OnHandleSystemCoilsChange(int iAddr,bool IsAction, uint8_t *pAddr);
    int OnHandleLasersCoilsChange(int iAddr,bool IsAction, uint8_t *pAddr);

    int OnHandleSystemHoldRegsChange(int iAddr,int iData, uint8_t *pAddr);
    int OnHandleLasersHoldRegsChange(int iAddr,int iData, uint8_t *pAddr);
    //---- ----<<
    //----the InputRegister interface of modbus how to Get MarkSystem Info,Called by Proxy  ---->>
    uint16_t GetLaserMCReadStatus(uint8_t uLaserSel);
    uint16_t GetLaserMCReadError(uint8_t uLaserSel);
    uint16_t GetLaserMCReadDataIn(uint8_t uLaserSel,int iIndex);
    uint16_t GetAxisReadStatus(uint8_t uLaserSel,uint8_t uAxisSel);
    uint16_t GetAxisReadError(uint8_t uLaserSel,uint8_t uAxisSel);
    uint16_t GetAxisReadEncoder(uint8_t uLaserSel,uint8_t uAxisSel);
    uint16_t GetAxisReadDataIn(uint8_t uLaserSel,uint8_t uAxisSel,int iIndex);
    //---- ----<<
    //----the Coils interface of modbus how to control MarkSystem,Called by Proxy >>
    bool GetSystemLaserAction(uint8_t uLaserSel);
    bool SetSystemLaserAction(uint8_t uLaserSel,bool IsAction);
    bool GetSystemLaserReady(uint8_t uLaserSel);
    bool SetSystemTimeTick(bool IsAction);  //--2016.11.08
    bool GetSystemTimeTick(bool IsAction);
    bool SetSystemAutoFlagAction(bool IsAction);//--手动/自动标记模式标识
    bool GetSystemAutoFlagAction(bool IsAction);//--手动/自动标记模式标识
    bool SetSystemAutoArea1Action(bool IsAction);//--2016.12.01
    bool SetSystemAutoArea2Action(bool IsAction);//--2016.12.01
    bool GetLaserInservice(uint8_t uLaserSel);
    bool SetLaserInservice(uint8_t uLaserSel,bool IsAction);
    bool GetLaserInitial(uint8_t uLaserSel);
    bool SetLaserInitial(uint8_t uLaserSel,bool IsAction);
    bool GetLaserProgRunOrgin(uint8_t uLaserSel);
    bool SetLaserProgRunOrgin(uint8_t uLaserSel,bool IsAction);
    bool GetLaserProgRunLoop(uint8_t uLaserSel);
    bool SetLaserProgRunLoop(uint8_t uLaserSel,bool IsAction);
    bool GetLaserAction(uint8_t uLaserSel);
    bool SetLaserAction(uint8_t uLaserSel,bool IsAction);
    bool GetLaserParameterLoad(uint8_t uLaserSel);
    bool SetLaserParameterLoad(uint8_t uLaserSel,bool IsAction);
    bool SetLaserCalibrationFileActive(uint8_t uLaserSel,bool IsAction);
    bool SetLaserTemplateFileActive(uint8_t uLaserSel,bool IsAction);
    bool SetLaserMapFileActive(uint8_t uLaserSel,bool IsAction);
    bool SetLaserEmgActive(uint8_t uLaserSel,bool IsAction);
    bool GetLaserStatusBit(uint8_t uLaserSel,int iIndex);
    /*
    bool GetAxisInservice(uint8_t uLaserSel,uint8_t uAxisSel);
    bool SetAxisInservice(uint8_t uLaserSel,uint8_t uAxisSel,bool IsAction);
    bool GetAxisInitial(uint8_t uLaserSel,uint8_t uAxisSel);
    bool SetAxisInitial(uint8_t uLaserSel,uint8_t uAxisSel,bool IsAction);
    bool GetAxisReady(uint8_t uLaserSel,uint8_t uAxisSel);
    bool GetAxisHome(uint8_t uLaserSel,uint8_t uAxisSel);
    bool SetAxisHome(uint8_t uLaserSel,uint8_t uAxisSel,bool IsAction);
    bool GetAxisAbsMove(uint8_t uLaserSel,uint8_t uAxisSel);
    bool SetAxisAbsMove(uint8_t uLaserSel,uint8_t uAxisSel,bool IsAction);
    bool GetAxisRelMove(uint8_t uLaserSel,uint8_t uAxisSel);
    bool SetAxisRelMove(uint8_t uLaserSel,uint8_t uAxisSel,bool IsAction);
    bool GetAxisEmgStop(uint8_t uLaserSel,uint8_t uAxisSel);
    bool SetAxisEmgStop(uint8_t uLaserSel,uint8_t uAxisSel,bool IsAction);
    bool GetAxisPauseStop(uint8_t uLaserSel,uint8_t uAxisSel);
    bool SetAxisPauseStop(uint8_t uLaserSel,uint8_t uAxisSel,bool IsAction);
    bool GetAxisParaLoad(uint8_t uLaserSel,uint8_t uAxisSel);
    bool SetAxisParaLoad(uint8_t uLaserSel,uint8_t uAxisSel,bool IsAction);
    bool GetAxisStatusBit(uint8_t uLaserSel,uint8_t uAxisSel,int iIndex);
    */
    //---- ----<<
    //----the HoldRegister interface of modbus how to control MarkSystem,Called by Proxy >>
    bool GetSystemRTC(SYSTEMTIME *stSysTime);
    bool SetSystemRTC(SYSTEMTIME *stSysTime);
    /*
    float GetSystemLaserCenterOffset(uint8_t uLaserSel,uint8_t uOffsetSel);
    bool SetSystemLaserCenterOffset(uint8_t uLaserSel,uint8_t uOffsetSel,float fValue);
    */
    bool GetLaserTemplateFile(uint8_t uLaserSel,char *FileName);
    bool SetLaserTemplateFile(uint8_t uLaserSel,char *FileName);
    bool GetLaserMapFile(uint8_t uLaserSel,char *FileName);
    bool SetLaserMapFile(uint8_t uLaserSel,char *FileName);
    bool GetLaserCalibrateFile(uint8_t uLaserSel,int iIndex,char *FileName);
    bool SetLaserCalibrateFile(uint8_t uLaserSel,int iIndex,char *FileName);
    uint16_t GetLaserMCControlData(uint8_t uLaserSel,int iIndex);
    bool SetLaserMCControlData(uint8_t uLaserSel,int iIndex,uint16_t wValue);  
    float GetAxisTargetPos(uint8_t uLaserSel,uint8_t AxisSel,uint8_t uPosSel);
    bool SetAxisTargetPos(uint8_t uLaserSel,uint8_t AxisSel,uint8_t uPosSel,float fTargetPos);
    uint16_t GetAxisControlData(uint8_t uLaserSel,uint8_t AxisSel,int iIndex);
    bool SetAxisControlData(uint8_t uLaserSel,uint8_t AxisSel,int iIndex,uint16_t wValue);
    //----<<
private:
    CCLog *m_pTheLog;
};

extern CMBControlProxy *g_pCMBControlProxy;

#endif
