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
#ifndef H_TL_TETE_DEFINES_H
#define H_TL_TETE_DEFINES_H

#ifndef TL_API
# define TL_API
#endif

#include "tlAttributeType.h"

#define TETE_VERSION  406000     /* 4.06.000 */

#define MAX_TEXT_SIZE   256     /* 字符变量的最大字符数 */
#define MAX_FILE_NAME   512     /* 文件名的最大字符数 */
#define MAX_DOC_NAME     256
#define MAX_LAYER_NAME   256
#define MAX_TXT_SIZE     512    /* 字符串保存到文件中的最大字符数 */

#define TETE_PI     3.14159265358979323846264338327950
#define TETE_2PI    3.14159265358979323846264338327950*2
#define DEG2RAD(d)              ((TETE_PI*(d))/180.0)   /*角度转换为弧度*/
#define RAD2DEG(d)              ((180*(d))/TETE_PI)     /*弧度转换为角度*/

#define dim(x)  sizeof(x)/sizeof(x[0])

const float EPSINON = 0.000001f;
#define D_EQUAL(a, b)  (fabs((a) - (b)) < EPSINON)
#define D_LESS(a, b)   ((a) + EPSINON < (b))
#define D_GREATER(a, b)   ((a) - EPSINON > (b))

#define D_EQUAL_X(a, b, E)  (fabs((a) - (b)) < (E))   // 指定比较精度

#define clBlack     (RGB(0,0,0))
#define clWhite     (RGB(255,255,255))
#define clRed       (RGB(255,0,0))
#define clGreen     (RGB(0,255,0))
#define clBlue      (RGB(0,0,255))
#define clSilver    (RGB(205,205,205))
#define clGray      (RGB(125,125,125))

  /* 数据类型重定义 */
typedef signed char        int8_t;
typedef signed char        char_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;
typedef float              float32_t;
typedef double             float64_t;
//--typedef char*              string_t;
#include <math.h>
/*  二维图形点位描述 [mm]  */
class DPoint_t {
public:
    double x;
    double y;

    inline DPoint_t() : x(0), y(0) { }
    inline DPoint_t(double xpos, double ypos) : x(xpos), y(ypos) { }
    inline DPoint_t(const DPoint_t &p) : x(p.x), y(p.y) { }

    inline void setX(double xpos) { x = xpos; }
    inline void setY(double ypos) { y = ypos; }
    inline DPoint_t &operator+=(const DPoint_t &p) { x += p.x; y += p.y; return *this; }
    inline DPoint_t &operator-=(const DPoint_t &p) { x -= p.x; y -= p.y; return *this; }
    inline DPoint_t &operator*=(double c) { x *= c; y *= c; return *this; }
    inline bool operator==(const DPoint_t &p) { return (D_EQUAL(p.x, x) && D_EQUAL(p.y, y)); }
	inline bool operator==(const DPoint_t &p) const { return (D_EQUAL(p.x, x) && D_EQUAL(p.y, y)); }
    inline void repos(double dx, double dy) { x += dx; y += dy; }
    inline void resize(double fx, double fy) { x *= fx; y *= fy; }
    inline void rotate(double cx, double cy,double angle) 
    { 
        double dx = x - cx;
        double dy = y - cy;
        x = dx*cos(angle) - dy*sin(angle) + cx;
        y = dx*sin(angle) + dy*cos(angle) + cy;
    }
    inline void affine(double ax, double ay) 
    { 
        double xx = x + ay*y;
        double yy = ax*x + y;
        x = xx;
        y = yy;
    }
};

/*
typedef struct {
    char sName[MAX_TEXT_SIZE];    //--名称
    char sVersion[MAX_TEXT_SIZE]; //--版本
    char sID[MAX_TEXT_SIZE];      //--标识码
    char sVender[MAX_TEXT_SIZE];  //--提供者
    char sNote[MAX_TEXT_SIZE];    //--备注说明
}stModuleInfo_t;
*/
#define TETE_INFO_NAME     0
#define TETE_INFO_VERSION  1
#define TETE_INFO_ID       2
#define TETE_INFO_VENDER   3
#define TETE_INFO_NOTE     4

#define TETE_FLAG_OPEN    1   /* 创建实例 */
#define TETE_FLAG_CLOSE   2   /* 释放实例 */

#endif
 