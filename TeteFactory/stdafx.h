
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的
#define  _CRT_NON_CONFORMING_SWPRINTFS

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类

#include <afxsock.h>            // MFC 套接字扩展

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持
#include "assert.h"

#include "Wafer\WaferUserMsg.h"
#include "tlGraphDefines.h"

#include <Shlwapi.h>
#include <imagehlp.h>
#pragma comment(lib,"Shlwapi.lib")
#pragma comment(lib,"imagehlp.lib")

extern const char *GetSystemDir(void);
extern const char *GetSystemDirFile(const char *sFile);
extern double CalAngle(double da, double db);
extern void GetArcInfo(DPoint_t dp1, DPoint_t dp2, DPoint_t dp3, stArc_t &stArc);
extern void DoEvent(void);

#define BKCOLOR RGB(60, 60, 60)
#define STATIC_TITLE_COLOR RGB(255, 97, 0)
#define EDIT_BKCOLOR RGB(193, 210, 240)   // 编辑框背景色
#define EDIT_TEXT_COLOR RGB(45, 45, 45)   // 编辑框字体颜色
#define STATIC_COLOR RGB(200, 200, 200)
#define BUTTON_NOR_COLOR RGB(0, 0, 200)
#define BUTTON_FOCUS_COLOR RGB(0, 0, 250)
#define BUTTON_TEXT_COLOR RGB(192, 192, 192)
#define BUTTON_TEXT_DISABLE_COLOR RGB(60, 60, 60)
#define STATIC_TITLE_COLOR_NAME  RGB(50,205,50)

#include "StdioFileCodePage.h"
const CString g_sMarkDriverConfigFile = _T(".\\config\\markDriver.ini");

enum emFactoryMsgType_t
{
	emMsgType_Normal = 0,
	emMsgType_Title,
	emMsgType_Warn,
	emMsgType_Error,
	emMsgType_Success
};

// PropertyBuilderByName 用于生成类的成员变量
// 并生成set和get方法
// type 为变量类型
// typeflag, 类型标识符，bool=b, int=i, string=s
// value 为初始化值
// access_permission 为变量的访问权限(public, priavte, protected)
#define PropertyBuilderByName(type, typeflag, name, value, access_permission)\
    access_permission:\
        type m_##typeflag##name = value;\
    public:\
    inline void Set##name(type v) {\
        m_##typeflag##name = v;\
    }\
    inline type Get##name() const{\
        return m_##typeflag##name;\
    }\

#define PointerPropertyBuilderByName(type, name, value, access_permission)\
    access_permission:\
        type* m_p##name = nullptr;\
    public:\
    inline void Set##name(type* v) {\
        m_p##name = v;\
    }\
    inline type* Get##name() const{\
        return m_p##name;\
    }\

/*
* @brief 获取本机设备DPI，每英寸像素值
*/
//long g_GetDeviceDPI()
//{
//	GetDeviceCaps(NULL, LOGPIXELSX);
//}

#define IO_OUT_VALID   0  //有效
#define IO_OUT_INVALID 1 //无效
#define IO_IN_VALID 0
#define IO_IN_INVALID 1

#define IO_2D_OUT_VALID   0  //有效
#define IO_2D_OUT_INVALID 1 //无效
#define IO_2D_IN_VALID 0
#define IO_2D_IN_INVALID 1

#define WM_USER_SCAN_PROCESS WM_USER+1
#define WM_USER_START_PROCESS WM_USER+2  //开始标记（检测到开始信号）


// 编辑框控件设置浮点数或整形
inline void s_SetEditItemValue(CWnd* pWnd, UINT nID, double d, int nPrecise = 4)
{
	assert(nullptr != pWnd);
	CString sf, s;
	sf.Format(_T("%%.%df"), nPrecise);
	s.Format(sf, d);
	pWnd->SetDlgItemText(nID, s);
}

inline double s_GetEditItemValue(CWnd* pWnd, UINT nID)
{
	assert(nullptr != pWnd);
	CString s;
	pWnd->GetDlgItemText(nID, s);
	return _ttof(s);
}

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

enum { SELECT_TRACK1 = 0, SELECT_TRACK2 };
