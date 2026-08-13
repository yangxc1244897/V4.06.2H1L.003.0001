/**
******************************************************************************
* @文件     
* @作者  
* @版本 V1.00.00
* @日期 2018-02-10
* @简述 TETE激光打标校正模型
* @email:jchq0101net@sina.com
******************************************************************************
* @说明
*
*
* 
******************************************************************************
*/
#ifndef H_TL_MARK_CALIB_H
#define H_TL_MARK_CALIB_H

#include "tlMarkDefine.h"
#include "tlMarkEngine.h"
/*----  标记线性校正控制命令 -------------------------------------------*/
#define MC_CALIB_HEAD_SEL     ((int32_t)0x0000)
#define MC_CALIB_LIMIT_X      ((int32_t)0x0001)
#define MC_CALIB_LIMIT_Y      ((int32_t)0x0002)
#define MC_CALIB_GRID_X       ((int32_t)0x0003)
#define MC_CALIB_GRID_Y       ((int32_t)0x0004)
/*----  标记线性校正类模型 ---------------------------------------------*/
class IPlatform;
class TL_API IMarkCalib
{
public:
    virtual ~IMarkCalib() {};

    virtual int32_t Open(IPlatform *pPlatform) = 0;
    virtual int32_t Close(void) = 0;
    virtual int32_t ShowWnd(void) = 0;  //--校正窗体
	virtual int32_t GetCalibCtrl(int32_t iCmd, void *pArg) = 0; //--获取控制信息
	virtual int32_t SetCalibCtrl(int32_t iCmd, void *pArg) = 0; //--设置控制信息

    virtual int32_t LoadGrid(DPoint_t *pGridPoints,uint32_t nGridX,uint32_t nGridY) = 0;
    virtual int32_t LoadGridFile(const char *sFileName) = 0;//--载入测量网格文件*.txt
    virtual int32_t LoadCalibFile(const char *sFileName) = 0;/* xxx.tlaf */
    virtual int32_t SaveCalibFile(const char *sFileName) = 0;/* xxx.tlaf */

    virtual void SetTransform(int32_t iCoordFlag,double dbOffsetX,double dbOffsetY,double dbAngle) = 0;
    virtual void LaserCalib(double &px, double &py) = 0;  //--激光标记校正
    virtual void LightCalib(double &px, double &py) = 0;  //--红光预览校正
    virtual void LaserCalib(double px, double py,uint32_t &mx,uint32_t &my) = 0;  //--激光标记校正
    virtual void LightCalib(double px, double py,uint32_t &mx,uint32_t &my) = 0;  //--红光预览校正

public:
    enum CoordFlags {
        emReverseX =        (int32_t) 0x00001,
        emReverseY =        (int32_t) 0x00002,
        emExchangeXY =      (int32_t) 0x00004
    };
};

#endif
