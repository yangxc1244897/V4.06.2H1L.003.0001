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
  * @Modbus激光器信息映射表
  * 1,
  *   COIL_BASE_ADDR          ----MODBUS中实际的基地址
  *   INPUTREG_BASE_ADDR      ----MODBUS中实际的基地址
  *   HOLDREG_BASE_ADDR       ----MODBUS中实际的基地址
  * 2,
  *   SYS_xxx_MAP_ADDR    ----映射表中实际的基地址,
  *   LASERn_xxx_MAP_ADDR ----映射表中实际的基地址
  *----xxx:COILS/INPUTREG/HOLDREG

  *关于Laser1/2 XXXX*_ADDR为映射表中Laser1/2的偏移地址，其在映射表中的
  *实际地址为:
  *(LASER1_xxx_MAP_ADDR + xxxx_COIL)
  *(LASER2_xxx_MAP_ADDR + xxxx_COIL)
  *----xxx为COILS/INPUTREG/HOLDREG
  *----xxxx为AXIS****或其他
  ******************************************************************************
  */
//---------------------------------------------------------------------------
#ifndef MB_MAPPING_TABLE_H
#define MB_MAPPING_TABLE_H
//---------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <limits.h>
#include <time.h>
#include <stdint.h>
//-------------------------------------------------------------------------------------
/** ModBus COILS信息 **/
//----------------------------------------------------------------->>>>>
#define COILS_BASE_ADDR                0      //--Coils的基地址
#define SYS_COILS_MAP_ADDR             0      //--System Coils的基地址
#define LASER1_COILS_MAP_ADDR       1000      //--Laser1 Coils的基地址
#define LASER2_COILS_MAP_ADDR       2000      //--Laser2 Coils的基地址
#define LASER3_COILS_MAP_ADDR       3000      //--Laser3 Coils的基地址
#define LASER4_COILS_MAP_ADDR       4000      //--Laser4 Coils的基地址
//----------------------------------------------------------------->>>>>
#define LASER1_COILS_AXIS_X_MAP_ADDR    (LASER1_COILS_MAP_ADDR+250)      //--Laser1 COILS Axis X的基地址
#define LASER1_COILS_AXIS_Y_MAP_ADDR    (LASER1_COILS_MAP_ADDR+500)      //--Laser1 COILS Axis X的基地址
#define LASER1_COILS_AXIS_Z_MAP_ADDR    (LASER1_COILS_MAP_ADDR+750)      //--Laser1 COILS Axis X的基地址

#define LASER2_COILS_AXIS_X_MAP_ADDR    (LASER2_COILS_MAP_ADDR+250)      //--Laser2 COILS Axis X的基地址
#define LASER2_COILS_AXIS_Y_MAP_ADDR    (LASER2_COILS_MAP_ADDR+500)      //--Laser2 COILS Axis X的基地址
#define LASER2_COILS_AXIS_Z_MAP_ADDR    (LASER2_COILS_MAP_ADDR+750)      //--Laser2 COILS Axis X的基地址
//---------------------------------------------------------------->>>>>
#define SYSTEM_LASER_I_ACTION_COIL       ((int16_t)1)
#define SYSTEM_LASER_II_ACTION_COIL      ((int16_t)2)
#define SYSTEM_LASER_III_ACTION_COIL     ((int16_t)3)
#define SYSTEM_LASER_IV_ACTION_COIL      ((int16_t)4)

#define SYSTEM_TIME_TICK_COIL            ((int16_t)9)
#define SYSTEM_AUTO_FLAG_COIL            ((int16_t)10)  //--雷射自動旗標(生產用)

typedef struct ST_SystemCoilsInfo{
    uint8_t System_Laser_I_Action;
    uint8_t System_Laser_II_Action;
    uint8_t System_Laser_III_Action;
    uint8_t System_Laser_IV_Action;
	uint8_t System_Time_Tick;
    uint8_t System_AutoFlag;
}SystemCoilsInfo_t;

/* 相对于Laser1/2基地址的地址 */
#define LASER_INSERVICE_COIL            ((int16_t)1)
#define LASER_INITIAL_COIL              ((int16_t)2)
#define LASER_ON_COIL                   ((int16_t)3)
#define LASER_OFF_COIL                  ((int16_t)4)
/* Calibration Coils*/
#define LASER_PROGRUN_ORGIN_COIL        ((int16_t)101)    //--通知雷射出光Origin Position
#define LASER_PROGRUN_LOOP_COIL         ((int16_t)102)    //--通知雷射出光Loop1-Loop4 Position
#define LASER_MANUAL_ACTION_COIL        ((int16_t)103)    //--通知雷射出光Manual Position
#define LASER_WRITE_CAL_COIL            ((int16_t)104)    //--通知雷射進行校正檔讀取
/* Wafer Coils*/
#define WLASER_AUTO_RUN_COIL            ((int16_t)201)    //--Auto Run(Wafer) 
#define WLASER_PRM_LOAD_COIL            ((int16_t)203)    //-通知雷射進行雷參讀取
#define WLASER_WRITE_MAP_COIL           ((int16_t)204)    //--通知雷射進行Map檔讀取(Wafer)
#define WLASER_WRITE_DRAW_COIL          ((int16_t)205)    //--通知雷射進行Draw檔讀取(Wafer)
#define WLASER_DRAW_ACTION_COIL         ((int16_t)206)    //--通知雷射進行單一內容打標(Draw)
#define WLASER_TEMPLATE_ACTION_COIL     ((int16_t)207)    //--通知雷射進行單一內容打標(Draw)
#define WLASER_WRITE_CAL_COIL           ((int16_t)208)    //--通知雷射載入Wafer下半部校正檔

/* Package Coils*/
#define PLASER_AUTO_RUN_COIL            ((int16_t)301)    //--Auto Run(Package)
#define PLASER_WRITE_MARKFILE_COIL      ((int16_t)302)    //--通知雷射進行模板讀取(MarkFile.xml)
#define PLASER_DRAW_ACTION_COIL         ((int16_t)303)    //--通知雷射進行單一內容打標(Draw)
#define PLASER_TEMPLATE_ACTION_COIL     ((int16_t)304)    //--通知雷射進行單一樣板打標(Template)
typedef struct ST_LaserCoilsInfo{
    uint8_t Laser_Inservice;
    uint8_t Laser_Initial;
    uint8_t Laser_On;
    uint8_t Laser_Off;
    uint8_t Laser_ProgRun_Orgin;
    uint8_t Laser_ProgRun_Loop;
    uint8_t Laser_Manual_Action;
    uint8_t Laser_Write_Cal;
    
    uint8_t WLaser_AutoRun;
    uint8_t WLaser_ParameterLoad;
    uint8_t WLaser_Write_Map;
    uint8_t WLaser_Write_Draw;
    uint8_t WLaser_Draw_Action;
    uint8_t WLaser_Template_Action;
    uint8_t WLaser_WriteCal_Area2;


    uint8_t PLaser_AutoRun;    
    uint8_t PLaser_Write_MarkFile;
    uint8_t PLaser_Draw_Action;
    uint8_t PLaser_Template_Action;
}LaserCoilsInfo_t;


typedef struct ST_CoilsInfo{
    SystemCoilsInfo_t   stSystemCoilsInfo;
    LaserCoilsInfo_t     stLaserCoilsInfo[2];
}CoilsInfo_t;

//-------------------------------------------------------------------------------------
/** ModBus Descrete In信息 **/
//-------------------------------------------------------------------------------------
#define DESCRETE_BASE_ADDR    10000   //--Descrete In的基地址
#define SYS_DESCRETE_BASE_ADDR    10000   //--System Descrete In的基地址
#define LASER1_DESCRETE_ADDR      11000   //--Laser1 Descrete In的基地址
#define LASER2_DESCRETE_ADDR      12000   //--Laser2 Descrete In的基地址

#define SYS_LASER_I_READY                ((int16_t)1)
#define SYS_LASER_II_READY               ((int16_t)2)
#define SYS_LASER_III_READY              ((int16_t)3)
#define SYS_LASER_IV_READY               ((int16_t)4)
#define SYS_LASER_AUTO_FLAG              ((int16_t)5)

#define LASER_STATUS_READY               ((int16_t)1)
#define LASER_STATUS_XXXXX               ((int16_t)2)
//-------------------------------------------------------------------------------------
/** ModBus Input Registers信息 **/
//-------------------------------------------------------------------------------------
#define INPUTREG_BASE_ADDR    30000   //--Input Registers的基地址
#define LASER1_INPUTREG_MAP_ADDR    31000   //--Laser1 Input Registers的基地址
#define LASER2_INPUTREG_MAP_ADDR    32000   //--Laser2 Input Registers的基地址

#define LASER_MC_READ_DATAIN_0_INPUT      ((int16_t)1)
#define LASER_MC_READ_DATAIN_1_INPUT      ((int16_t)2)       
#define LASER_MC_READ_DATAIN_2_INPUT      ((int16_t)3)
#define LASER_MC_READ_DATAIN_3_INPUT      ((int16_t)4)
#define LASER_MC_READ_DATAIN_4_INPUT      ((int16_t)5)

#define WLASER_MC_READ_STATUS_INPUT       ((int16_t)200)
#define WLASER_MC_READ_ERROR_INPUT        ((int16_t)205)

#define PLASER_MC_READ_STATUS_INPUT       ((int16_t)220)
#define PLASER_MC_READ_ERROR_INPUT        ((int16_t)225)

typedef struct ST_LaserInputRegsInfo{
    uint16_t WLaser_MC_ReadStatus;
    uint16_t WLaser_MC_ReadError;
    uint16_t PLaser_MC_ReadStatus;
    uint16_t PLaser_MC_ReadError;
    uint16_t Laser_MC_ReadDataIn_0;
    uint16_t Laser_MC_ReadDataIn_1;
    uint16_t Laser_MC_ReadDataIn_2;
    uint16_t Laser_MC_ReadDataIn_3;
    uint16_t Laser_MC_ReadDataIn_4;
    uint16_t Laser_MC_ReadDataIn_5;
    uint16_t Laser_MC_ReadDataIn_6;
    uint16_t Laser_MC_ReadDataIn_7;
    uint16_t Laser_MC_ReadDataIn_8;
    uint16_t Laser_MC_ReadDataIn_9;
    uint16_t Laser_MC_ReadDataIn_10;
    uint16_t Laser_MC_ReadDataIn_11;
    uint16_t Laser_MC_ReadDataIn_12;
    uint16_t Laser_MC_ReadDataIn_13;
    uint16_t Laser_MC_ReadDataIn_14;
    uint16_t Laser_MC_ReadDataIn_15; 
}LaserInputRegsInfo_t;

typedef struct ST_InputRegsInfo{
    LaserInputRegsInfo_t   stLaserInputRegsInfo[2];
    //--AxisInputRegsInfo_t    stAxisInputRegsInfo[2][3];
}InputRegsInfo_t;
//-------------------------------------------------------------------------------------
/** ModBus Hold Registers信息 **/
//-------------------------------------------------------------------------------------
#define HOLDREG_BASE_ADDR       40000       //--Hold Registers的基地址
#define SYS_HOLDREG_MAP_ADDR       40000        //--System Hold Registers的基地址
#define LASER1_HOLDREG_MAP_ADDR    41000     //--Laser1 Hold Registers的基地址
#define LASER2_HOLDREG_MAP_ADDR    42000     //--Laser2 Hold Registers的基地址

/* System */
#define SYSTEM_RTC_YEAR_REG            ((uint16_t)1)
#define SYSTEM_RTC_MONTH_REG           ((uint16_t)2)
#define SYSTEM_RTC_DATE_REG            ((uint16_t)3)
#define SYSTEM_RTC_HOUR_REG            ((uint16_t)4)//--24 hour
#define SYSTEM_RTC_MIN_REG             ((uint16_t)5)
#define SYSTEM_RTC_SEC_REG             ((uint16_t)6)

#define SYSTEM_TYPE_SEL_REG            ((uint16_t)7)//--0:Wafer 1:Package
#define SYSTEM_LASER_SEL_REG           ((uint16_t)8)//--Select Num of Laser

typedef struct ST_SystemHoldRegsInfo{
    uint16_t System_RTC_Year;
    uint16_t System_RTC_Month;
    uint16_t System_RTC_Date;
    uint16_t System_RTC_Hour;
    uint16_t System_RTC_Min;
    uint16_t System_RTC_Sec;

    uint16_t System_TypeSel;
    uint16_t System_LaserSel;
}SystemHoldRegsInfo_t;

#define LASER_MC_CONTROLDATA_0_REG       ((uint16_t)1)
#define LASER_MC_CONTROLDATA_1_REG       ((uint16_t)2)
#define LASER_MC_CONTROLDATA_2_REG       ((uint16_t)3)
#define LASER_MC_CONTROLDATA_3_REG       ((uint16_t)4)
#define LASER_MC_CONTROLDATA_4_REG       ((uint16_t)5)
#define LASER_MC_CONTROLDATA_5_REG       ((uint16_t)6)

/* Calibration */
#define LASER_CALIBRATE_FILENAME_I_REG     ((uint16_t)100)
#define LASER_CALIBRATE_FILENAME_II_REG    ((uint16_t)105)

//--2016.12.01-->>
//--#define LASER_TEMPLATE_FILENAME_REG        ((uint16_t)400)
//--#define LASER_MAP_FILENAME_REG             ((uint16_t)600)
//--#define LASER_CALIBRATE_FILENAME_I_REG     ((uint16_t)800)
//--#define LASER_CALIBRATE_FILENAME_II_REG    ((uint16_t)850)
//--<<

#define AXIS_X_TARGETPOS_1_REG         ((uint16_t)110)
#define AXIS_X_TARGETPOS_2_REG         ((uint16_t)111)
#define AXIS_X_TARGETPOS_3_REG         ((uint16_t)112)
#define AXIS_X_TARGETPOS_4_REG         ((uint16_t)113)
#define AXIS_X_TARGETPOS_M_REG         ((uint16_t)114)
#define AXIS_X_CONTROLDATA_1_REG       ((uint16_t)115)
#define AXIS_X_CONTROLDATA_2_REG       ((uint16_t)116)
#define AXIS_X_CONTROLDATA_3_REG       ((uint16_t)117)
#define AXIS_X_CONTROLDATA_4_REG       ((uint16_t)118)
#define AXIS_X_CONTROLDATA_5_REG       ((uint16_t)119)

#define AXIS_Y_TARGETPOS_1_REG         ((uint16_t)130)
#define AXIS_Y_TARGETPOS_2_REG         ((uint16_t)131)
#define AXIS_Y_TARGETPOS_3_REG         ((uint16_t)132)
#define AXIS_Y_TARGETPOS_4_REG         ((uint16_t)133)
#define AXIS_Y_TARGETPOS_M_REG         ((uint16_t)134)
#define AXIS_Y_CONTROLDATA_1_REG       ((uint16_t)135)
#define AXIS_Y_CONTROLDATA_2_REG       ((uint16_t)136)
#define AXIS_Y_CONTROLDATA_3_REG       ((uint16_t)137)
#define AXIS_Y_CONTROLDATA_4_REG       ((uint16_t)138)
#define AXIS_Y_CONTROLDATA_5_REG       ((uint16_t)139)

#define AXIS_Z_TARGETPOS_1_REG         ((uint16_t)150)
#define AXIS_Z_TARGETPOS_2_REG         ((uint16_t)151)
#define AXIS_Z_TARGETPOS_3_REG         ((uint16_t)152)
#define AXIS_Z_TARGETPOS_4_REG         ((uint16_t)153)
#define AXIS_Z_TARGETPOS_M_REG         ((uint16_t)154)
#define AXIS_Z_CONTROLDATA_1_REG       ((uint16_t)155)
#define AXIS_Z_CONTROLDATA_2_REG       ((uint16_t)156)
#define AXIS_Z_CONTROLDATA_3_REG       ((uint16_t)157)
#define AXIS_Z_CONTROLDATA_4_REG       ((uint16_t)158)
#define AXIS_Z_CONTROLDATA_5_REG       ((uint16_t)159)

typedef struct ST_LaserAxisHoldRegsInfo{
    float Axis_TargetPos[5];
	uint16_t Axis_ControlData[5];
}LaserAxisHoldRegsInfo_t;
/* Wafer */
#define WLASER_ACTION_DRAW_FILENAME             ((uint16_t)210)
#define WLASER_ACTION_MAP_FILENAME              ((uint16_t)215)
#define WLASER_ACTION_LASERPARAMETER_FILENAME   ((uint16_t)220)

/* Package */
#define PLASER_ACTION_MARKFILE_FILENAME         ((uint16_t)310)
#define PLASER_ACTION_TEMPLATE_FILENAME         ((uint16_t)315)
#define PLASER_ACTION_LAYOUT_FILENAME           ((uint16_t)320)
#define PLASER_ACTION_DRAW_FILENAME             ((uint16_t)325)
#define PLASER_ACTION_LASERPRM_FILENAME         ((uint16_t)330)

#define PLASER_SYS_LC_I_OS_X             ((uint16_t)350)
#define PLASER_SYS_LC_I_OS_Y             ((uint16_t)351)
#define PLASER_SYS_LC_I_OS_Z             ((uint16_t)352)
#define PLASER_SYS_LC_I_OS_T             ((uint16_t)353)

#define PLASER_SYS_LC_II_OS_X             ((uint16_t)354)
#define PLASER_SYS_LC_II_OS_Y             ((uint16_t)355)
#define PLASER_SYS_LC_II_OS_Z             ((uint16_t)356)
#define PLASER_SYS_LC_II_OS_T             ((uint16_t)357)

#define PLASER_SYS_LC_III_OS_X             ((uint16_t)358)
#define PLASER_SYS_LC_III_OS_Y             ((uint16_t)359)
#define PLASER_SYS_LC_III_OS_Z             ((uint16_t)360)
#define PLASER_SYS_LC_III_OS_T             ((uint16_t)361)

#define PLASER_SYS_LC_IV_OS_X             ((uint16_t)362)
#define PLASER_SYS_LC_IV_OS_Y             ((uint16_t)363)
#define PLASER_SYS_LC_IV_OS_Z             ((uint16_t)364)
#define PLASER_SYS_LC_IV_OS_T             ((uint16_t)365)

typedef struct ST_LaserHoldRegsInfo{
    char Laser_Template_Filename[256];
    char Laser_Map_Filename[256];
    char Laser_Calibrate_Filename_I[256];
    char Laser_Calibrate_Filename_II[256];
    uint16_t Laser_MC_ControlData[16];
 
    LaserAxisHoldRegsInfo_t  stLaserAxisHoldRegsInfo[3];
}LaserHoldRegsInfo_t;
typedef struct ST_HoldRegsInfo{
    SystemHoldRegsInfo_t    stSystemHoldRegsInfo;
    LaserHoldRegsInfo_t    stLaserHoldRegsInfo[2];
}HoldRegsInfo_t;

typedef struct ST_MBMapInfoSets{
    CoilsInfo_t     stCoilsInfo;
    InputRegsInfo_t     stInputRegsInfo;
    HoldRegsInfo_t     stHoldRegsInfo;
}MBMapInfoSets_t;

#endif
 
 