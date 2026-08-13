/**
******************************************************************************
* @模块 
* @作者 jchq
* @版本 V1.00.00
* @日期 2018-09-21
* @简述 
* @email:jchq0101net@sina.com
******************************************************************************
* @说明 
*    
*    
*    
*      
*        
*    
*    
******************************************************************************
*/
#ifndef H_CONFIGURE_H
#define H_CONFIGURE_H

#include "tlPlatform.h"
class CConfigure : public IConfigure
{
public:
	int8_t m_nSoftwareMode;   // 软件模式，0-工厂模式  1-设计模式

    bool m_IsAutoRun;    //--开机自动运行
    bool m_IsAutoLogin;  //--是否自动登录
    bool m_IsStoreMM;    //--是否记住密码
    int  m_iLogType;     //--日志输出方式选择，emLOG_PRINT_NULL/STD/LOG
    int  m_iLogLevel;    //--日志输出等级，emLOG_LEVEL_INFO/WARN/ERROR/FATAL/NULL

    int    m_iAxisMode;      //--坐标系模型[0:7]
    double m_dbAxisLimitX;   //--坐标轴正向最大值
    double m_dbAxisLimitY;   //--
    double m_dbAxisShiftX;   //--坐标系X方向偏移
    double m_dbAxisShiftY;   //--坐标系Y方向偏移
    double m_dbAxisShiftA;   //--坐标系角度偏移,以偏移后的原点为中心旋转
    
    bool m_IsShowPath;    //--文档视图是否显示矢量方向
    bool m_IsShowGrid;    //--文档视图是否显示网格
    bool m_IsShowRuler;   //--是否显示标尺
    bool m_IsShowAxis;    //--是否显示坐标轴
    bool m_IsShowAcross;  //--是否显示十字线

    bool m_IsSingleDoc;       //--true:单文档; false:所有文档
    bool m_IsNetworkEnable;   //--网络标记使能
    bool m_IsPedalEnable;     //--脚踏标记使能
    bool m_IsScannerEnable;   //--扫描枪扫描标记使能
    bool m_IsOnlineEnable;    //--在线流程化标记使能
    bool m_IsLinearAdjust;    //--线性校正

    bool m_IsAutoDriver;      //--自动检测板卡类型
    int32_t m_iDriverType;    //--手动配置的板卡类型
    int32_t m_iScanerType;    //--扫描头是数字头还是模拟头    
    //--int32_t m_iDeviceType;    //--激光器类型
    int32_t m_iMainCtrlVer;   //--控制箱版本
    bool m_IsMotionEnable;    //--运动控制使能
    int32_t m_iMotionType;    //--运动控制卡类型/供应商
    bool m_IsVisionEnable;    //--CCD使能
    int32_t m_iVisionType;    //--CCD类型/供应商

    double m_dbMoveStep;  //--上下左右键移动精度

	double m_dbNinePRadius;   // 九点标定点半径，标定点以圆形显示
	double m_dbNinePFillPitch;  // 九点标定点填充间距
	double m_dbNinePPitch;    // 九点标定点的间距
	double m_dbNinePCenterX;   // 九点标定点的中心点X坐标
	double m_dbNinePCenterY;   // 九点标定点的中心点Y坐标
	double m_dbNinePCenterX2;   // 九点标定点的中心点X坐标2
	double m_dbNinePCenterY2;   // 九点标定点的中心点Y坐标2


	double m_dbAnchorRadius;   // 标定点半径，标定点以圆形显示
	double m_dbAnchorFillPitch;  // 标定点填充间距

	double m_dbAnchorCalibAX;    // 主头标定点X轴校准值
	double m_dbAnchorCalibAY;    // 主头标定点Y轴校准值
	double m_dbAnchorCalibBX;    // 从头标定点X轴校准值
	double m_dbAnchorCalibBY;    // 从头标定点Y轴校准值

	bool m_bShowWaferInfo;    // MAP组是否显示芯片上的信息

	bool m_bShowTextFrame;    // 显示文字外框

	double m_dbWaferWidth;    // 模板默认宽度
	double m_dbWaferHeight;   // 模板默认高度

	double m_dbAreaOffsetX;   // 分区域标记，X方向偏差
	double m_dbAreaOffsetY;   // 分区域标记，Y方向偏差

public:
    static CConfigure *Instance();
	static void Delete();
    ~CConfigure();

    bool SetDataValue(const char *sSegName, const char *sKeyName, bool value); 
    bool SetDataValue(const char *sSegName, const char *sKeyName, int8_t value);
    bool SetDataValue(const char *sSegName, const char *sKeyName, int32_t value);
    bool SetDataValue(const char *sSegName, const char *sKeyName, uint32_t value);
    bool SetDataValue(const char *sSegName, const char *sKeyName, float value);
    bool SetDataValue(const char *sSegName, const char *sKeyName, double value);
    bool SetDataValue(const char *sSegName, const char *sKeyName, const char *value);

    bool GetBoolValue(const char *sSegName, const char *sKeyName, bool defValue);
    int8_t GetInt8Value(const char *sSegName, const char *sKeyName, int8_t defValue);
    int32_t GetInt32Value(const char *sSegName, const char *sKeyName, int32_t defValue);
    uint32_t GetUnsignedValue(const char *sSegName, const char *sKeyName, uint32_t defValue);
    float GetFloatValue(const char *sSegName, const char *sKeyName, float defValue);
    double GetDoubleValue(const char *sSegName, const char *sKeyName, double defValue);
    char *GetStringValue(const char *sSegName, const char *sKeyName, const char *defValue);
protected:
    CConfigure(void);    
private:
    static CConfigure *_instance;

    char m_sFileName[MAX_FILE_NAME];
    char m_sDataValue[MAX_FILE_NAME];
    void LoadConfig(void);
    void SaveConfig(void);
};

#endif