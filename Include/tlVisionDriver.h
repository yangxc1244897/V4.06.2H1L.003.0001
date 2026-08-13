/**
******************************************************************************
* @文件  机器视觉抽象层
* @作者  
* @版本 V1.00.00
* @日期 2018-11-09
* @简述 
* @email:jchq0101net@sina.com
******************************************************************************
* @说明
*
*
* 
******************************************************************************
*/
#ifndef H_TL_VISION_DRIVER_H
#define H_TL_VISION_DRIVER_H

#include <stdint.h>
#include <vector>
using namespace std;
#include "tlTeteDefines.h"


/* 轴运动参数 */
typedef struct {    
    double x;     //--CCD视野下的X坐标
    double y;     //--CCD视野下的Y坐标
    double a;     //--CCD视野下的角度值
}stPosition_t;

//--机器视觉抽象层
class IPlatform;
class TL_API IVisionDriver
{
public:
    //--IVisionDriver();
    virtual ~IVisionDriver(){};
  
public:
    virtual int32_t Open(IPlatform *pPlatform) = 0;       //--打开、复位、加载配置、启用  
    virtual int32_t Close(void) = 0;      //--关闭

	virtual void ShowVisionWnd(void) = 0;  //--显示控制主界面
    virtual int32_t GetVisionCtrl(int32_t iCmd, void *pArg) = 0;//--获取控制配置信息
    virtual int32_t SetVisionCtrl(int32_t iCmd, void *pArg) = 0;//--设置控制配置信息
    
    virtual int32_t GetStatus(void) = 0;  //--获取当前状态
    virtual int32_t SinglePosition(stPosition_t &posData) = 0;
	virtual int32_t MultiPosition(stPosition_t *pPosData, int32_t &nCount) = 0;
	virtual int32_t ExecNineMark(int no, double cx, double cy, double pitch) = 0;

};

#endif
