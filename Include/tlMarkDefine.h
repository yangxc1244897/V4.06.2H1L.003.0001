/**
  ******************************************************************************
  * @文件     
  * @作者  
  * @版本 V1.00.00
  * @日期 2017-04-11
  * @简述   
  * @email:jchq0101net@sina.com
  ******************************************************************************
  * @说明
  *
  *
  * 
  ******************************************************************************
  */
#ifndef H_TL_MARK_DEFINES_H
#define H_TL_MARK_DEFINES_H

#include "tlTeteDefines.h"

typedef struct _HeadTurn_t {
	double dx, dy;  //-偏移量
	double cx, cy, angle;//--旋转中心及角度
	double fx, fy;  //--缩放因子
}xHeadTurn_t;

//-----------------------------------------------------------------------------------------------------

/**
* @brief 板卡型号
*/
enum emDriverType_t
{
	emDRIVER_UNKNOWN = 0,
	emDRIVER_TE9608_OLD,    /*! DMA9608 旧版*/
	emDRIVER_TE9608_NEW,    /*! DMA9608 新版*/
	emDRIVER_TE9632_OLD,    /*! DMA9632 旧版*/
	emDRIVER_TE9632_NEW,    /*! DMA9632 新版*/
};

#endif
 