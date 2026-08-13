/**
  ******************************************************************************
  * @文件 fMarkFile.h    
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
#ifndef H_CODE_NODE_TABLE_TABLE_H
#define H_CODE_NODE_TABLE_TABLE_H

#include <stdlib.h>

/* Code Data Node */
#define CODED_DATA(node, n)   (uint32_t((node) + ((n) & 0x00FF)))
#define CODED_NODE(CNn)		  (uint32_t((CNn) & 0xFF000000))
#define CODED_CODE(CNn)       (uint32_t((CNn) & 0xFFFFFF00))
#define CODED_ID(CNn)         (uint16_t(((CNn) & 0x00FFFF00) >> 8))
#define CODED_TYPE(CNn)       (uint8_t((CNn) & 0x000000FF))

//#define CODED_VERIFY(uCN, uNode)     (0x0 == (uCN) || 0xFFFF == CODED_ID((uCN)) || CODED_NODE(uCN) != uNode)

#define CODED_VERIFY(uCN)     (0x0 == (uCN) || 0xFFFF == CODED_ID((uCN)))

/* |---   Code   --|- Type -|  */
/* |-Node-|-  ID  -|- Type -|  */
/* |-  8 -|-  16  -|-   8  -|  */
/*
    Type: 所属节点类型
	ID:   节点在所属节点中的编号
	Size: 节点内容所属的数据类型
*/
/*------------   数据类型 ---------------------------------------------------*/
enum
{
	emNODE_NULL = 0,
	emNODE_BOOL,
	emNODE_INT8,
	emNODE_INT16,
	emNODE_INT32,
    emNODE_INT64,
	emNODE_FLOAT,
	emNODE_DOUBLE,
	emNODE_STRING
};

/*------------   类型节点 ---------------------------------------------------*/
#define CN_NULL_BASE       		    ((uint32_t)0x00000000)      //--空节点
#define CN_GRAPH_BASE       		((uint32_t)0x01000000)      //--图形引擎属性[暂不使用]
#define CN_MARK_BASE        		((uint32_t)0x02000000)      //--标记引擎属性[暂不使用]
#define CN_FILE_BASE        		((uint32_t)0x11000000)      //--文件
#define CN_DOC_BASE         		((uint32_t)0x12000000)      //--标记文档
#define CN_LAYER_BASE       		((uint32_t)0x13000000)      //--图层
#define CN_OBJECT_BASE      		((uint32_t)0x14000000)      //--对象基类
#define CN_ACTION_BASE      		((uint32_t)0x15000000)      //--动作类
#define CN_WAFER_DOC_BASE      		((uint32_t)0x16000000)      //--晶元文档
#define CN_WAFERMAP_BASE      		((uint32_t)0x17000000)      //--动作类
#define CN_WAFERGROUP_BASE          ((uint32_t)0x18000000)      //--动作类


#define CN_AUTOMNG_BASE     		((uint32_t)0x20000000)      //--自动管理属性
#define CN_SHAPE_BASE       		((uint32_t)0x21000000)      //--图形对象
#define CN_FIXTEXT_BASE     		((uint32_t)0x22000000)      //--文字对象
#define CN_AUTOTXT_BASE     		((uint32_t)0x23000000)      //--自动文字对象
#define CN_TIMETXT_BASE     		((uint32_t)0x24000000)      //--时间文字对象
#define CN_D1CODE_BASE      		((uint32_t)0x25000000)      //--一维码对象
#define CN_D2CODE_BASE      		((uint32_t)0x26000000)      //--二维码对象
#define CN_IMAGE_BASE       		((uint32_t)0x27000000)      //--图像对象
#define CN_TEXT_BASE                ((uint32_t)0x28000000)      //--文本对象


#define CN_META_BASE                ((uint32_t)0x30000000)      //--图元
#define CN_META_DOT_BASE       		((uint32_t)0x31000000)      //--点图元对象
#define CN_META_LINE_BASE           ((uint32_t)0x32000000)      //--线图元对象
#define CN_META_POLYLINE_BASE       ((uint32_t)0x33000000)      //--折线图元对象
#define CN_META_POLYGON_BASE       	((uint32_t)0x34000000)      //--多边形图元对象
#define CN_META_RECTANGLE_BASE      ((uint32_t)0x35000000)      //--矩形图元对象
#define CN_META_ARC_BASE       		((uint32_t)0x36000000)      //--弧图元对象
#define CN_META_CIRCLE_BASE       	((uint32_t)0x37000000)      //--圆图元对象
#define CN_META_ELLIPSE_BASE       	((uint32_t)0x38000000)      //--椭圆图元对象
#define CN_META_CHORD_BASE       	((uint32_t)0x39000000)      //--弦图元对象
#define CN_META_PIE_BASE       		((uint32_t)0x3A000000)      //--饼图元对象
#define CN_META_BEZIER_CONIC_BASE   ((uint32_t)0x3B000000)      //--二阶贝塞尔图元对象
#define CN_META_BEZIER_CUBIC_BASE   ((uint32_t)0x3C000000)      //--三阶贝塞尔图元对象

#define CN_LASER_BASE       		((uint32_t)0x51000000)      //--激光器
#define CN_PEN_BASE         		((uint32_t)0x52000000)      //--笔

/*--------------  文件节点  -------------------------------------------*/
#define CN_FILE_VER         	   	(CN_FILE_BASE+(0x0001<<8))    //--版本
#define CN_FILE_DOCCNT              (CN_FILE_BASE+(0x0002<<8))    //--文档数量
#define CN_FILE_DOC                 (CN_FILE_BASE+(0x0003<<8))    //--文档对象
#define CN_FILE_WAFERDOC            (CN_FILE_BASE+(0x0004<<8))    //--晶元设计
#define CN_FILE_WAFERMAP            (CN_FILE_BASE+(0x0005<<8))    //--晶元MAP



/*--------------  晶元设计文档节点 -------------------------------------------*/
#define CN_WAFERDOC_LAYER				(CN_WAFER_DOC_BASE+(0x0001<<8))    //--图层对象
#define CN_WAFERDOC_DOCCNT				(CN_WAFER_DOC_BASE+(0x0002<<8))    //--文档数量
#define CN_WAFERDOC_DOC					(CN_WAFER_DOC_BASE+(0x0003<<8))    //--文档对象
#define CN_WAFERDOC_ACTIVELAYER_NAME	(CN_WAFER_DOC_BASE+(0x0004<<8))    //--当前图层对象名称
#define CN_WAFERDOC_STRIP_MAP			(CN_WAFER_DOC_BASE+(0x0005<<8))    //--是否为StripMap图档


/*--------------  晶体MAP节点  -------------------------------------------*/
#define CN_WAFERMAP_GROUP                 (CN_WAFERMAP_BASE+(0x0001<<8))     //--芯片组名称
#define CN_WAFERMAP_ANCHOR_P_X            (CN_WAFERMAP_BASE+(0x0002<<8))     //--定位点X坐标
#define CN_WAFERMAP_ANCHOR_P_Y            (CN_WAFERMAP_BASE+(0x0003<<8))     //--定位点Y坐标
#define CN_WAFERMAP_TRACK_VERPITCH		  (CN_WAFERMAP_BASE+(0x0004<<8))  // 轨道垂直间距
#define CN_WAFERMAP_TRACK_HORPITCH        (CN_WAFERMAP_BASE+(0x0005<<8))  // 轨道水平间距
#define CN_WAFERMAP_TRACK_DEG             (CN_WAFERMAP_BASE+(0x0006<<8))  // 轨道偏移角度
#define CN_WAFERMAP_TRACK_COUNT           (CN_WAFERMAP_BASE+(0x0007<<8))  // 轨道数量
#define CN_WAFERMAP_MARK_ORDER            (CN_WAFERMAP_BASE+(0x0008<<8))  // 标记顺序

/*--------------  晶体Map中的Group节点  -------------------------------------------*/

#define CN_WAFERGROUP_NAME                (CN_WAFERGROUP_BASE+(0x0001<<8))     //--芯片组名称
#define CN_WAFERGROUP_WAFERID             (CN_WAFERGROUP_BASE+(0x0002<<8))     //--芯片使用BIN
#define CN_WAFERGROUP_COLOR				  (CN_WAFERGROUP_BASE+(0x0003<<8))     //--芯片组颜色
#define CN_WAFERGROUP_NFLAG               (CN_WAFERGROUP_BASE+(0x0004<<8))     //--标志符
#define CN_WAFERGROUP_POINTX              (CN_WAFERGROUP_BASE+(0x0005<<8))     //--dp0[0].X
#define CN_WAFERGROUP_POINTY              (CN_WAFERGROUP_BASE+(0x0006<<8))     //--dp0[0].Y
#define CN_WAFERGROUP_ROW                 (CN_WAFERGROUP_BASE+(0x0007<<8))     //--dp0[0].Y
#define CN_WAFERGROUP_COL                 (CN_WAFERGROUP_BASE+(0x0008<<8))     //--dp0[0].Y
#define CN_WAFERGROUP_CELL_H              (CN_WAFERGROUP_BASE+(0x0009<<8))     
#define CN_WAFERGROUP_CELL_W              (CN_WAFERGROUP_BASE+(0x000A<<8))   
#define CN_WAFERGROUP_BASE_H              (CN_WAFERGROUP_BASE+(0x000B<<8))     
#define CN_WAFERGROUP_BASE_W              (CN_WAFERGROUP_BASE+(0x000C<<8))     
#define CN_WAFERGROUP_COUNT               (CN_WAFERGROUP_BASE+(0x000D<<8))     
#define CN_WAFERGROUP_DEG                 (CN_WAFERGROUP_BASE+(0x000E<<8))     
#define CN_WAFERGROUP_CX                  (CN_WAFERGROUP_BASE+(0x000F<<8))     
#define CN_WAFERGROUP_CY                  (CN_WAFERGROUP_BASE+(0x0010<<8))     
#define CN_WAFERGROUP_CELL_ARRAYSTYLE     (CN_WAFERGROUP_BASE+(0x0011<<8))  // 阵列方式     
#define CN_WAFERGROUP_CELL_DEG            (CN_WAFERGROUP_BASE+(0x0012<<8))  // 单颗芯片方向    
#define CN_WAFERGROUP_CELL_ROWPITCH       (CN_WAFERGROUP_BASE+(0x0013<<8))  // 行间距 
#define CN_WAFERGROUP_CELL_COLPITCH       (CN_WAFERGROUP_BASE+(0x0014<<8))  // 列间距         
#define CN_WAFERGROUP_CELL_ROWCOUNT       (CN_WAFERGROUP_BASE+(0x0015<<8))  // 阵列行数
#define CN_WAFERGROUP_CELL_COLCOUNT       (CN_WAFERGROUP_BASE+(0x0016<<8))  // 阵列列数
#define CN_WAFERGROUP_GROUP_PITCH         (CN_WAFERGROUP_BASE+(0x0017<<8))  // Group间距 
#define CN_WAFERGROUP_CELL_COLPITCH_CS    (CN_WAFERGROUP_BASE+(0x0018<<8))  // 列间距补偿
#define CN_WAFERGROUP_GROUP_IL_COUNT_X    (CN_WAFERGROUP_BASE+(0x0019<<8))  // X轴方向间隔缩进列的数量
#define CN_WAFERGROUP_GROUP_IL_INDENT_X   (CN_WAFERGROUP_BASE+(0x001A<<8))  // X轴方向间隔缩进距离
#define CN_WAFERGROUP_GROUP_IL_DEG_X      (CN_WAFERGROUP_BASE+(0x001B<<8))  // X轴方向间隔缩进列的印章方向
#define CN_WAFERGROUP_GROUP_IL_COUNT_Y    (CN_WAFERGROUP_BASE+(0x001C<<8))  // Y轴方向间隔缩进行的数量
#define CN_WAFERGROUP_GROUP_IL_INDENT_Y   (CN_WAFERGROUP_BASE+(0x001D<<8))  // Y轴方向间隔缩进距离
#define CN_WAFERGROUP_GROUP_IL_DEG_Y      (CN_WAFERGROUP_BASE+(0x001E<<8))  // Y轴方向间隔缩进行的印章方向
#define CN_WAFERGROUP_GROUP_IL_STYLE      (CN_WAFERGROUP_BASE+(0x001F<<8))  // 间隔缩进类型：水平方向还是垂直方向
#define CN_WAFERGROUP_GROUP_COUNT_VER     (CN_WAFERGROUP_BASE+(0x0020<<8))  // 垂直方向，Group的数量
#define CN_WAFERGROUP_GROUP_PITCH_VER     (CN_WAFERGROUP_BASE+(0x0021<<8))  // 垂直方向，Group间距 
#define CN_WAFERGROUP_BLOCKID             (CN_WAFERGROUP_BASE+(0x0022<<8))  // 水平方向，BlockID
#define CN_WAFERGROUP_BLOCKID_VER         (CN_WAFERGROUP_BASE+(0x0023<<8))  // 垂直方向，BlockID

/*--------------  图形引擎节点  -------------------------------------------*/


/*--------------  标记引擎节点  -------------------------------------------*/


/*--------------  文档节点  -------------------------------------------*/
#define CN_DOC_ID                   (CN_DOC_BASE+(0x0001<<8))     //--文档编号
#define CN_DOC_NAME                 (CN_DOC_BASE+(0x0002<<8))     //--文档名称
#define CN_DOC_MARKABLE             (CN_DOC_BASE+(0x0003<<8))     //--是否标记
#define CN_DOC_LOCKED               (CN_DOC_BASE+(0x0004<<8))     //--是否锁定
#define CN_DOC_LAYERCNT             (CN_DOC_BASE+(0x0005<<8))     //--图层数量
#define CN_DOC_LAYER                (CN_DOC_BASE+(0x0006<<8))     //--图层对象
#define CN_DOC_OBJTYPE              (CN_DOC_BASE+(0x0007<<8))     //--对象类型
#define CN_DOC_MARKCORNER           (CN_DOC_BASE+(0x0008<<8))     //--标记起始角落
#define CN_DOC_MARKSORT             (CN_DOC_BASE+(0x0009<<8))     //--标记排序方式
#define CN_DOC_MARKDIRT             (CN_DOC_BASE+(0x000A<<8))     //--标记方向
#define CN_DOC_WIDTH                (CN_DOC_BASE+(0x000B<<8))     //--文档宽度
#define CN_DOC_HEIGHT               (CN_DOC_BASE+(0x000C<<8))     //--文档高度
#define CN_DOC_ALIGN_BASEOBJ        (CN_DOC_BASE+(0x000D<<8))     //--文本对齐，基准对象
#define CN_DOC_ALIGN_CHILDOBJ       (CN_DOC_BASE+(0x000E<<8))     //--文本对象，对齐对象
#define CN_DOC_ALIGN_STYLE          (CN_DOC_BASE+(0x000F<<8))     //--文本对象，对齐方式




/*--------------  图层节点  -------------------------------------------*/
#define CN_LAYER_ID                 (CN_LAYER_BASE+(0x0001<<8))   //--编号
#define CN_LAYER_NAME               (CN_LAYER_BASE+(0x0002<<8))   //--名称
#define CN_LAYER_MARKABLE           (CN_LAYER_BASE+(0x0003<<8))   //--是否标记
#define CN_LAYER_LOCKED             (CN_LAYER_BASE+(0x0004<<8))   //--是否锁定
#define CN_LAYER_VISIBLE            (CN_LAYER_BASE+(0x0005<<8))   //--是否可见
#define CN_LAYER_LINECOLOR          (CN_LAYER_BASE+(0x0006<<8))   //--线的颜色
#define CN_LAYER_OBJCNT             (CN_LAYER_BASE+(0x0007<<8))   //--对象数量
#define CN_LAYER_OBJTYPE            (CN_LAYER_BASE+(0x0008<<8))   //--对象类型
#define CN_LAYER_PEN                (CN_LAYER_BASE+(0x0009<<8))   //--笔类型

/*--------------  激光器节点  -------------------------------------------*/



/*--------------  笔节点  -------------------------------------------*/


/*--------------  对象基类节点  -------------------------------------------*/
#define CN_OBJECT_ID            	(CN_OBJECT_BASE+(0x0001<<8))     //--编号
#define CN_OBJECT_TYPE              (CN_OBJECT_BASE+(0x0002<<8))     //--对象类型
#define CN_OBJECT_MARKABLE          (CN_OBJECT_BASE+(0x0003<<8))     //--是否标记
#define CN_OBJECT_LAYERID           (CN_OBJECT_BASE+(0x0004<<8))     //--所在图层ID
#define CN_OBJECT_LEFT              (CN_OBJECT_BASE+(0x0005<<8))     //--对象左边界
#define CN_OBJECT_TOP               (CN_OBJECT_BASE+(0x0006<<8))     //--对象上边界
#define CN_OBJECT_RIGHT             (CN_OBJECT_BASE+(0x0007<<8))     //--对象右边界
#define CN_OBJECT_BOTTOM            (CN_OBJECT_BASE+(0x0008<<8))     //--对象下边界
#define CN_OBJECT_MARKFRAME          (CN_OBJECT_BASE+(0x0009<<8))     //--是否标记外框
/*--------------  位置恢复参数节点  -------------------------------------------*/
#define CN_DIMENSION_RP0_X          (CN_OBJECT_BASE+(0x0020<<8))     //--参考定位点0的X坐标
#define CN_DIMENSION_RP0_Y          (CN_OBJECT_BASE+(0x0021<<8))     //--参考定位点0的Y坐标
#define CN_DIMENSION_RP1_X          (CN_OBJECT_BASE+(0x0022<<8))     //--参考定位点1的X坐标
#define CN_DIMENSION_RP1_Y          (CN_OBJECT_BASE+(0x0023<<8))     //--参考定位点1的Y坐标
#define CN_DIMENSION_RP2_X          (CN_OBJECT_BASE+(0x0024<<8))     //--参考定位点2的X坐标
#define CN_DIMENSION_RP2_Y          (CN_OBJECT_BASE+(0x0025<<8))     //--参考定位点2的Y坐标
#define CN_DIMENSION_AP0_X          (CN_OBJECT_BASE+(0x0026<<8))     //--实际定位点0的X坐标
#define CN_DIMENSION_AP0_Y          (CN_OBJECT_BASE+(0x0027<<8))     //--实际定位点0的Y坐标
#define CN_DIMENSION_AP1_X          (CN_OBJECT_BASE+(0x0028<<8))     //--实际定位点1的X坐标
#define CN_DIMENSION_AP1_Y          (CN_OBJECT_BASE+(0x0029<<8))     //--实际定位点1的Y坐标
#define CN_DIMENSION_AP2_X          (CN_OBJECT_BASE+(0x002A<<8))     //--实际定位点2的X坐标
#define CN_DIMENSION_AP2_Y          (CN_OBJECT_BASE+(0x002B<<8))     //--实际定位点2的Y坐标
/*--------------  填充节点  -------------------------------------------*/
#define CN_OBJECT_HATCHCNT          (CN_OBJECT_BASE+(0x0040<<8))      //--填充数量
#define CN_HATCH_ID                 (CN_OBJECT_BASE+(0x0041<<8))      //--填充编号,填充属性的第一个节点
#define CN_HATCH_ENABLE             (CN_OBJECT_BASE+(0x0042<<8))      //--是否填充
#define CN_HATCH_COLOR              (CN_OBJECT_BASE+(0x0043<<8))      //--颜色
#define CN_HATCH_BORDERPITCH        (CN_OBJECT_BASE+(0x0044<<8))      //--填充线与边框的间距
#define CN_HATCH_FILLPITCH          (CN_OBJECT_BASE+(0x0045<<8))      //--填充线间的距离
#define CN_HATCH_FILLAVGDIS         (CN_OBJECT_BASE+(0x0046<<8))      //--是否平均分配 
#define CN_HATCH_FILLTIMES          (CN_OBJECT_BASE+(0x0047<<8))      //--設定填滿的次數
#define CN_HATCH_FILLANGLESTART     (CN_OBJECT_BASE+(0x0048<<8))      //--設定填滿的起始角度
#define CN_HATCH_FILLANGLESTEP      (CN_OBJECT_BASE+(0x0049<<8))      //--設定填滿的累进角度
#define CN_HATCH_FILLSTYLE          (CN_OBJECT_BASE+(0x004A<<8))      //--填充模式
#define CN_HATCH_FILLLOOPS          (CN_OBJECT_BASE+(0x004B<<8))      //--填滿雕刻模式若是環狀,則在外框向內畫幾環後再做其他的填滿
#define CN_HATCH_INSIDEFIRST        (CN_OBJECT_BASE+(0x004C<<8))      //--設定島型填滿是否由內向外填滿
#define CN_HATCH_MARKINVERSELY      (CN_OBJECT_BASE+(0x004D<<8))      //--設定反相雕刻
#define CN_HATCH_FILLDIRECTION      (CN_OBJECT_BASE+(0x004E<<8))      //--填充方向
#define CN_HATCH_END                (CN_OBJECT_BASE+(0x004F<<8))      //--结束节点,填充属性的最后一个节点


/*--------------  图形节点  -------------------------------------------*/
#define CN_SHAPE_METACNT		     (CN_SHAPE_BASE+(0x0001<<8))     //图元数量
#define CN_SHAPE_GRAPHCODE		     (CN_SHAPE_BASE+(0x0002<<8))     //图元类型
#define CN_SHAPE_OBJECT		         (CN_SHAPE_BASE+(0x0003<<8))     //对象基类

/*--------------  图元节点  -------------------------------------------*/
/*--------------  点图元节点  -------------------------------------------*/
#define CN_META_POINT_X               (CN_META_BASE+(0x0001<<8))     //--X坐标
#define CN_META_POINT_Y               (CN_META_BASE+(0x0002<<8))     //--Y坐标
/*--------------  线图元节点  -------------------------------------------*/
#define CN_META_LINE_SX             (CN_META_BASE+(0x0011<<8))     //--起始点X坐标
#define CN_META_LINE_SY             (CN_META_BASE+(0x0012<<8))     //--起始点Y坐标
#define CN_META_LINE_EX             (CN_META_BASE+(0x0013<<8))     //--结束点X坐标
#define CN_META_LINE_EY             (CN_META_BASE+(0x0014<<8))     //--结束点Y坐标
/*--------------  折线图元节点  -------------------------------------------*/
#define CN_META_POLYLINE_DOTCNT     (CN_META_BASE+(0x0021<<8))     //--折线的点数
#define CN_META_POLYLINE_DOT_X      (CN_META_BASE+(0x0022<<8))     //--折线的点的X坐标
#define CN_META_POLYLINE_DOT_Y      (CN_META_BASE+(0x0023<<8))     //--折线的点的Y坐标
/*--------------  多边形图元节点  -------------------------------------------*/
#define CN_META_POLYGON_DOTCNT      (CN_META_BASE+(0x0031<<8))     //--多边形的点数
#define CN_META_POLYGON_DOT_X       (CN_META_BASE+(0x0032<<8))     //--多边的点的X坐标
#define CN_META_POLYGON_DOT_Y       (CN_META_BASE+(0x0033<<8))      //--多边的点的Y坐标
/*--------------  矩形图元节点  -------------------------------------------*/
#define CN_META_RECTANGLE_P0_X      (CN_META_BASE+(0x0041<<8))     //--矩形第一点的X坐标
#define CN_META_RECTANGLE_P0_Y      (CN_META_BASE+(0x0042<<8))     //--矩形第一点的Y坐标
#define CN_META_RECTANGLE_P1_X      (CN_META_BASE+(0x0043<<8))     //--矩形第二点的X坐标
#define CN_META_RECTANGLE_P1_Y      (CN_META_BASE+(0x0044<<8))     //--矩形第二点的Y坐标
#define CN_META_RECTANGLE_P2_X      (CN_META_BASE+(0x0045<<8))     //--矩形第三点的X坐标
#define CN_META_RECTANGLE_P2_Y      (CN_META_BASE+(0x0046<<8))     //--矩形第三点的Y坐标
#define CN_META_RECTANGLE_P3_X      (CN_META_BASE+(0x0047<<8))     //--矩形第四点的X坐标
#define CN_META_RECTANGLE_P3_Y      (CN_META_BASE+(0x0048<<8))     //--矩形第四点的Y坐标

/*--------------  弧形图元节点  -------------------------------------------*/
#define CN_META_ARC_P0_X            (CN_META_BASE+(0x0051<<8))     //--弧形第一点的X坐标
#define CN_META_ARC_P0_Y            (CN_META_BASE+(0x0052<<8))     //--弧形第一点的Y坐标
#define CN_META_ARC_P1_X            (CN_META_BASE+(0x0053<<8))     //--弧形第二点的X坐标
#define CN_META_ARC_P1_Y            (CN_META_BASE+(0x0054<<8))     //--弧形第二点的Y坐标
#define CN_META_ARC_P2_X            (CN_META_BASE+(0x0055<<8))     //--弧形第三点的X坐标
#define CN_META_ARC_P2_Y            (CN_META_BASE+(0x0056<<8))     //--弧形第三点的Y坐标
#define CN_META_ARC_P3_X            (CN_META_BASE+(0x0057<<8))     //--弧形第四点的X坐标
#define CN_META_ARC_P3_Y            (CN_META_BASE+(0x0058<<8))     //--弧形第四点的Y坐标
/*--------------  圆图元节点  -------------------------------------------*/
#define CN_META_CIRCLE_CX		     (CN_META_BASE+(0x0061<<8))     //--圆形中点的X坐标
#define CN_META_CIRCLE_CY		     (CN_META_BASE+(0x0062<<8))     //--圆形中点的Y坐标
#define CN_META_CIRCLE_RADIUS	     (CN_META_BASE+(0x0063<<8))     //--圆形的半径
/*--------------  椭圆图元节点  -------------------------------------------*/
#define CN_META_ELLIPSE_CX          (CN_META_BASE+(0x0071<<8))     //--椭圆中心点的X坐标
#define CN_META_ELLIPSE_CY          (CN_META_BASE+(0x0072<<8))     //--椭圆中心点的Y坐标
#define CN_META_ELLIPSE_RX          (CN_META_BASE+(0x0073<<8))     //--椭圆的X轴的右顶点的X坐标
#define CN_META_ELLIPSE_RY          (CN_META_BASE+(0x0074<<8))     //--椭圆的X轴的右顶点的Y坐标
#define CN_META_ELLIPSE_TX          (CN_META_BASE+(0x0075<<8))     //--椭圆的X轴的上顶点的X坐标
#define CN_META_ELLIPSE_TY          (CN_META_BASE+(0x0076<<8))     //--椭圆的X轴的上顶点的Y坐标

/*--------------  二阶贝塞尔图元节点  -------------------------------------------*/
/*--------------  三阶贝塞尔图元节点  -------------------------------------------*/
/*--------------  弦图元节点  -------------------------------------------*/
/*--------------  饼图元节点  -------------------------------------------*/

/*--------------  文本节点  -------------------------------------------*/
#define CN_TEXT_OBJECT           (CN_TEXT_BASE+(0x0001<<8))        // 对象基类   
#define CN_TEXT_TYPE             (CN_TEXT_BASE+(0x0002<<8))        // 文本类型
#define CN_TEXT_FONTTYPE         (CN_TEXT_BASE+(0x0003<<8))        // 字体样式
/*--------------  字体信息节点  -------------------------------------------*/
#define CN_FONTINFO_NAME            (CN_TEXT_BASE+(0x0010<<8))    //--字体样式名称
#define CN_FONTINFO_CODEPAGE        (CN_TEXT_BASE+(0x0011<<8))    //--字体样式名称
#define CN_FONTINFO_BOLD            (CN_TEXT_BASE+(0x0012<<8))    //--是否粗体
#define CN_FONTINFO_ITALIC          (CN_TEXT_BASE+(0x0013<<8))    //--是否斜体
#define CN_FONTINFO_ZOOMSTYLE       (CN_TEXT_BASE+(0x0014<<8))    //--缩放样式
#define CN_FONTINFO_SIZE            (CN_TEXT_BASE+(0x0015<<8))    //--字体大小
#define CN_FONTINFO_HEIGHT          (CN_TEXT_BASE+(0x0016<<8))    //--字体高度
#define CN_FONTINFO_WIDTH           (CN_TEXT_BASE+(0x0017<<8))    //--字体宽度
/*--------------  文字排版  -------------------------------------------*/
#define CN_TYPESET_STYLE           (CN_TEXT_BASE+(0x0030<<8))    //--排版样式
#define CN_TYPESET_ROWCHARPITCH    (CN_TEXT_BASE+(0x0031<<8))    //--水平排版字符间距
#define CN_TYPESET_ROWPITCH        (CN_TEXT_BASE+(0x0032<<8))    //--水平排版的行间距
#define CN_TYPESET_ROWALIGN        (CN_TEXT_BASE+(0x0033<<8))    //--水平排版对齐方式
#define CN_TYPESET_COLCHARPITCH    (CN_TEXT_BASE+(0x0034<<8))    //--垂直排版字符间距
#define CN_TYPESET_COLPITCH        (CN_TEXT_BASE+(0x0035<<8))    //--垂直排版的列间距
#define CN_TYPESET_COLALIGN        (CN_TEXT_BASE+(0x0036<<8))    //--垂直排版对齐方式
#define CN_TYPESET_CX         	   (CN_TEXT_BASE+(0x0037<<8))    //--弧形排版的圆心X坐标
#define CN_TYPESET_CY              (CN_TEXT_BASE+(0x0038<<8))    //--弧形排版的圆心Y坐标
#define CN_TYPESET_RADIUS          (CN_TEXT_BASE+(0x0039<<8))    //--弧形排版的半径
#define CN_TYPESET_SANG            (CN_TEXT_BASE+(0x003A<<8))    //--弧形排版的圆弧起始角度
#define CN_TYPESET_EANG            (CN_TEXT_BASE+(0x003B<<8))    //--弧形排版的圆弧结束角度
#define CN_TYPESET_ARCALIGN        (CN_TEXT_BASE+(0x003C<<8))    //--弧形排版对齐方式

/*--------------  字体节点  -------------------------------------------*/
#define CN_TEXTFONT_NAME            (CN_FIXTEXT_BASE+(0x0010<<8))    //--字体样式名称
#define CN_TEXTFONT_BOLD            (CN_FIXTEXT_BASE+(0x0011<<8))    //--是否粗体
#define CN_TEXTFONT_ITALIC          (CN_FIXTEXT_BASE+(0x0012<<8))    //--是否斜体
#define CN_TEXTFONT_SIZE            (CN_FIXTEXT_BASE+(0x0013<<8))    //--字体大小
#define CN_TEXTFONT_HEIGHT          (CN_FIXTEXT_BASE+(0x0014<<8))    //--字体高度
#define CN_TEXTFONT_WIDTH           (CN_FIXTEXT_BASE+(0x0015<<8))    //--字体宽度
/*--------------  样式节点  -------------------------------------------*/
#define CN_TEXTTYPE_STYLE           (CN_FIXTEXT_BASE+(0x0030<<8))    //--排版样式
#define CN_TEXTTYPE_ROWCHARPITCH    (CN_FIXTEXT_BASE+(0x0031<<8))    //--水平排版字符间距
#define CN_TEXTTYPE_ROWPITCH        (CN_FIXTEXT_BASE+(0x0032<<8))    //--水平排版的行间距
#define CN_TEXTTYPE_ROWALIGN        (CN_FIXTEXT_BASE+(0x0033<<8))    //--水平排版对齐方式
#define CN_TEXTTYPE_COLCHARPITCH    (CN_FIXTEXT_BASE+(0x0034<<8))    //--垂直排版字符间距
#define CN_TEXTTYPE_COLPITCH        (CN_FIXTEXT_BASE+(0x0035<<8))    //--垂直排版的列间距
#define CN_TEXTTYPE_COLALIGN        (CN_FIXTEXT_BASE+(0x0036<<8))    //--垂直排版对齐方式
#define CN_TEXTTYPE_CX         	    (CN_FIXTEXT_BASE+(0x0037<<8))    //--弧形排版的圆心X坐标
#define CN_TEXTTYPE_CY              (CN_FIXTEXT_BASE+(0x0038<<8))    //--弧形排版的圆心Y坐标
#define CN_TEXTTYPE_RADIUS          (CN_FIXTEXT_BASE+(0x0039<<8))    //--弧形排版的半径
#define CN_TEXTTYPE_SANG            (CN_FIXTEXT_BASE+(0x003A<<8))    //--弧形排版的圆弧起始角度
#define CN_TEXTTYPE_EANG            (CN_FIXTEXT_BASE+(0x003B<<8))    //--弧形排版的圆弧结束角度
#define CN_TEXTTYPE_ARCALIGN        (CN_FIXTEXT_BASE+(0x003C<<8))    //--弧形排版对齐方式

/*--------------  自动文字节点  -------------------------------------------*/
/*--------------  自动管理对象节点  -------------------------------------------*/
#define CN_AUTOMNG_ID			    (CN_AUTOMNG_BASE+(0x0001<<8))     //--编号
#define CN_AUTOMNG_TYPE			    (CN_AUTOMNG_BASE+(0x0002<<8))     //--类别
#define CN_AUTOMNG_PREFIX		    (CN_AUTOMNG_BASE+(0x0003<<8))     //--字首
#define CN_AUTOMNG_SUFFIX		    (CN_AUTOMNG_BASE+(0x0004<<8))     //--字尾
#define CN_AUTOMNG_VALUE	        (CN_AUTOMNG_BASE+(0x0005<<8))     //--当前值
#define CN_AUTOMNG_START		    (CN_AUTOMNG_BASE+(0x0006<<8))     //--起始值
#define CN_AUTOMNG_STEP			    (CN_AUTOMNG_BASE+(0x0007<<8))     //--步进值
#define CN_AUTOMNG_END			    (CN_AUTOMNG_BASE+(0x0008<<8))     //--结束值
#define CN_AUTOMNG_DIGIT		    (CN_AUTOMNG_BASE+(0x0009<<8))     //--自动数值位数
#define CN_AUTOMNG_LEAD			    (CN_AUTOMNG_BASE+(0x000A<<8))     //--填补字符
#define CN_AUTOMNG_REPEAT		    (CN_AUTOMNG_BASE+(0x000B<<8))     //--循环次数
#define CN_AUTOMNG_RECYCLE		    (CN_AUTOMNG_BASE+(0x000C<<8))     //--是否自动循环
#define CN_AUTOMNG_BASETYPE		    (CN_AUTOMNG_BASE+(0x000D<<8))     //--进制类型
#define CN_AUTOMNG_REPEATED		    (CN_AUTOMNG_BASE+(0x000E<<8))     //--已重复次数 

/*--------------  固定文字节点  -------------------------------------------*/
#define CN_FIXTEXT_CONTENT          (CN_FIXTEXT_BASE+(0x0001<<8))     //--固定字体内容


/*--------------  时间文字节点  -------------------------------------------*/
#define CN_TIMETXT_STYLE            (CN_TIMETXT_BASE+(0x0001<<8))     //--时间样式


/*--------------  一维码节点  -------------------------------------------*/
#define CN_D1CODE_TYPE              (CN_D1CODE_BASE+(0x0001<<8))      //--条码类型
#define CN_D1CODE_CONTENT           (CN_D1CODE_BASE+(0x0002<<8))      //--条码内容
#define CN_D1CODE_WIDTHRADIO        (CN_D1CODE_BASE+(0x0003<<8))      //--宽窄比例
#define CN_D1CODE_SHOWCODE          (CN_D1CODE_BASE+(0x0004<<8))      //--是否显示文字
#define CN_D1CODE_CHECKDIGIT        (CN_D1CODE_BASE+(0x0005<<8))      //--是否有校验码
#define CN_D1CODE_AUTOENABLE        (CN_D1CODE_BASE+(0x0006<<8))      //--是否是自动变量
#define CN_D1CODE_OPTION1           (CN_D1CODE_BASE+(0x0007<<8))      //--条形码的配置参数1
#define CN_D1CODE_OPTION2           (CN_D1CODE_BASE+(0x0008<<8))      //--条形码的配置参数2
#define CN_D1CODE_OPTION3           (CN_D1CODE_BASE+(0x0009<<8))      //--条形码的配置参数3
#define CN_D1CODE_WIDTH             (CN_D1CODE_BASE+(0x000A<<8))      //--条形码的宽度
#define CN_D1CODE_HEIGHT            (CN_D1CODE_BASE+(0x000B<<8))      //--条形码的高度
#define CN_D1CODE_BARWIDTH          (CN_D1CODE_BASE+(0x000C<<8))      //--条形码的单位条宽度
#define CN_D1CODE_BARHEIGHT         (CN_D1CODE_BASE+(0x000D<<8))      //--条形码的单位条高度
#define CN_D1CODE_WIDTHFIXTYPE      (CN_D1CODE_BASE+(0x000E<<8))      //--条码尺寸的固定方式，固定码宽或者固定条宽
#define CN_D1CODE_BORDERSPACE       (CN_D1CODE_BASE+(0x000F<<8))      //--条形码的边距
#define CN_D1CODE_ADDFRAME          (CN_D1CODE_BASE+(0x0010<<8))      //--是否添加外框
#define CN_D1CODE_INVENABLE         (CN_D1CODE_BASE+(0x0011<<8))      //--是否反向标记
#define CN_D1CODE_FILLDIRECTION     (CN_D1CODE_BASE+(0x0012<<8))      //--填充方向
#define CN_D1CODE_PENWIDTH          (CN_D1CODE_BASE+(0x0013<<8))      //--笔宽度
#define CN_D1CODE_FILLPITCH         (CN_D1CODE_BASE+(0x0014<<8))      //--填充间距
#define CN_D1CODE_FILLSTYLE         (CN_D1CODE_BASE+(0x0015<<8))      //--填充样式
#define CN_D1CODE_FILLTYPE          (CN_D1CODE_BASE+(0x0016<<8))      //--填充类型
#define CN_D1CODE_FILLNUM           (CN_D1CODE_BASE+(0x0017<<8))      //--填充线数
#define CN_D1CODE_FRAMEWIDTH        (CN_D1CODE_BASE+(0x0018<<8))      //--外框宽度
#define CN_D1CODE_OBJECT            (CN_D1CODE_BASE+(0x0019<<8))      // 对象基类
#define CN_D1CODE_TEXTTYPE              (CN_D1CODE_BASE+(0x001A<<8))      // 文本类型


/*--------------  二维码节点  -------------------------------------------*/
#define CN_D2CODE_TYPE              (CN_D2CODE_BASE+(0x0001<<8))      //--条码类型
#define CN_D2CODE_CONTENT           (CN_D2CODE_BASE+(0x0002<<8))      //--条码内容
#define CN_D2CODE_AUTOENABLE        (CN_D2CODE_BASE+(0x0003<<8))      //--是否是自动变量
#define CN_D2CODE_OPTION1           (CN_D2CODE_BASE+(0x0004<<8))      //--条形码的配置参数1
#define CN_D2CODE_OPTION2           (CN_D2CODE_BASE+(0x0005<<8))      //--条形码的配置参数2
#define CN_D2CODE_OPTION3           (CN_D2CODE_BASE+(0x0006<<8))      //--条形码的配置参数3
#define CN_D2CODE_WIDTH             (CN_D2CODE_BASE+(0x0007<<8))      //--条形码的宽度
#define CN_D2CODE_HEIGHT            (CN_D2CODE_BASE+(0x0008<<8))      //--条形码的高度
#define CN_D2CODE_CELLWIDTH         (CN_D2CODE_BASE+(0x0009<<8))      //--条形码的单位条宽度
#define CN_D2CODE_CELLHEIGHT        (CN_D2CODE_BASE+(0x000A<<8))      //--条形码的单位条高度
#define CN_D2CODE_WIDTHFIXTYPE      (CN_D2CODE_BASE+(0x000B<<8))      //--条码尺寸的固定方式，固定码宽或者固定条宽
#define CN_D2CODE_ADDFRAME          (CN_D2CODE_BASE+(0x000C<<8))      //--是否添加外框
#define CN_D2CODE_INVENABLE         (CN_D2CODE_BASE+(0x000D<<8))      //--是否反向标记
#define CN_D2CODE_FILLDIRECTION     (CN_D2CODE_BASE+(0x000E<<8))      //--填充方向
#define CN_D2CODE_CELLMERGE         (CN_D2CODE_BASE+(0x000F<<8))      //--是否合并单元格
#define CN_D2CODE_CELLSTYLE         (CN_D2CODE_BASE+(0x0010<<8))      //--填充样式
#define CN_D2CODE_PENTYPE           (CN_D2CODE_BASE+(0x0011<<8))      //--笔类型
#define CN_D2CODE_LINEWIDTH         (CN_D2CODE_BASE+(0x0012<<8))      //--线宽
#define CN_D2CODE_LINEPITCH         (CN_D2CODE_BASE+(0x0013<<8))      //--线间距
#define CN_D2CODE_LINECOUNT         (CN_D2CODE_BASE+(0x0014<<8))      //--填充线数
#define CN_D2CODE_FRAMEWIDTH        (CN_D2CODE_BASE+(0x0015<<8))      //--外框宽度
#define CN_D2CODE_OBJECT            (CN_D2CODE_BASE+(0x0016<<8))      // 对象基类
#define CN_D2CODE_TEXTTYPE              (CN_D2CODE_BASE+(0x0017<<8))      // 文本类型

/*--------------  图形节点  -------------------------------------------*/

/*-------- 流程动作节点 --------*/
#define CN_ACTION_ID            	(CN_ACTION_BASE+(0x0001<<8))     //--编号
#define CN_ACTION_TYPE            	(CN_ACTION_BASE+(0x0002<<8))
#define CN_ACTION_CAPTION         	(CN_ACTION_BASE+(0x0003<<8))

#define CN_ACTION_SIGN            	(CN_ACTION_BASE+(0x0004<<8))
#define CN_ACTION_PINS            	(CN_ACTION_BASE+(0x0005<<8))
#define CN_ACTION_DMIN            	(CN_ACTION_BASE+(0x0006<<8))
#define CN_ACTION_DSEC            	(CN_ACTION_BASE+(0x0007<<8))
#define CN_ACTION_MSEC            	(CN_ACTION_BASE+(0x0008<<8))
#define CN_ACTION_JUMP            	(CN_ACTION_BASE+(0x0009<<8))
#define CN_ACTION_DOCID            	(CN_ACTION_BASE+(0x000A<<8))
#define CN_ACTION_TIMES            	(CN_ACTION_BASE+(0x000B<<8))
#define CN_ACTION_MX                (CN_ACTION_BASE+(0x000C<<8))
#define CN_ACTION_MY                (CN_ACTION_BASE+(0x000D<<8))
#define CN_ACTION_MZ                (CN_ACTION_BASE+(0x000E<<8))
#define CN_ACTION_MA                (CN_ACTION_BASE+(0x000F<<8))
#define CN_ACTION_MB                (CN_ACTION_BASE+(0x0010<<8))
#define CN_ACTION_OX                (CN_ACTION_BASE+(0x0011<<8))
#define CN_ACTION_OY                (CN_ACTION_BASE+(0x0012<<8))
#define CN_ACTION_RA                (CN_ACTION_BASE+(0x0013<<8))




#endif