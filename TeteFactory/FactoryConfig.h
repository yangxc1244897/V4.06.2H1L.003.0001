#pragma once

#include <vector>

enum emWorkMode_t
{
	emWorkMode_Auto = 0,  // 与金泰软件通讯模式  (目前作为华天“扫码后自动加载图档模式”)
	emWorkMode_Scan,      // 扫码后自动加载图档模式，开批
	emWorkMode_Manual,    // 调试模式
	emWorkMode_Ponum,     // 组装批号模式
};

enum emScannerType_t
{
	emScannerType_None = 0,   // 不启用扫描枪
	emScannerType_Client1,    // 1号网口扫描枪
	emScannerType_Client2,    // 2号网口扫描枪
	emScannerType_Client_All, // 启用两个扫描枪
};

class CFactoryConfig
{
public:
	int m_nWorkMode;        // 作业模式
	bool m_bAutoMark;       // 自动开始标记
	bool m_bAutoStartJob;   // 自动批处理
	bool m_bOpenRMS;        // 启动RMS功能
	bool m_bOpenRollTip;    // 打开滚动提示
	int m_nPort;            // 服务器端口
	int m_nPortPLC;            // 服务器端口
	//int m_nStartMarkSignalType;   // 开始标记触发信号类型
	//int m_nFinishMarkSignalType;  // 结束标记触发信号类型
	bool m_bMCCheckWarn;      // 是否送MC检查提醒
	int m_nRetryTimes;        // 相同图档加载次数上限
	bool m_bManulStamp;       // 手动加载印章
	bool m_bCheckOrderStatus;  // 检查工序状态5140
	bool m_bMaxMarkTimesLimit;   // 条数限制
	int m_nMaxMarkTimes;        // 最大条数
	bool m_bPenOrder;         // 按激光参数顺序标记
	bool m_bCheckStripMap;    // 启用Mapping功能
	bool m_bCheck2DBarcode;   // 启用2D读码器（仅限Normal使用）
	bool m_bCheckVisualSystem; //是否启用视觉定位系统（CCD）连接
	bool m_bVisionCheck;		//是否启用视觉检测连接
	bool m_bAddFilePrefix;  
	bool m_bCheckEQPStripIdIsMatchLotId;   // 检查Strip ID和Lot ID是否匹配
	bool m_bCheckVisionSwitchProcess;
	bool m_bCheckPLCSwitchProcess;
	bool m_bCheckFinsPLC;
	bool m_bCheckProductType;  // 检查产品类型
    
	CString m_sFtpIP;          // FTP服务器IP
	CString m_sMarkFilePath;   // 标记文件路径
	CString m_sLogFilePath;    // Log文件路径
	CString m_sWarnTips;       // 警告提示语
	CString m_sMesIP;          // MES系统IP
	CString m_sMobFilePath;    // 阵列位置修正文件
	CString m_sDeviceNo;       // 设备编号
	CString m_sBarcodeHead;    // 条码头部
	CString m_sMCCheckTip;     // MC检查提示语
	CString m_sMesUrl;        // Mes系统URL
	CString m_sStampPath;     // 扫LotID加载印章图档
	CString m_sGFHPath;     // 扫LotID加载印章图档
	CString m_sMesCheckUrl;   // Mes系统URL，检查基板条码和工单是否匹配的WebService地址
	CString m_sMesTrackInUrl;   // Mes系统URL，检查过站信息的WebService地址
	CString m_sMesThickUrl;  // Mes系统URL，获取晶圆厚度地址
	CString m_sMesProductType;  // Mes系统URL，产品类型
	CString m_sLaserParamSave;  // 激光参数保存路径

	CString m_sLotIDXMLPath;       //LotID xml信息保存路径
	CString m_sScripIDXMLPath;     //ScripID xml信息保存路径
	emScannerType_t m_emUseScan;//启用扫描枪类型
	CString m_sScannerIP[2];     //网口扫描枪IP地址
	int m_nScannerPort[2];    //网口扫描枪端口
	int m_nScannStartSignal[2]; //扫描触发端口
	int m_nScannStartSignal2[2];//第二个扫描枪扫描触发端口
	int m_nScannReadySingle; //准备就绪
	int m_nScannOKSignal;    //扫描OK端口
	int m_nScannNGSignal;    //扫描NG端口
	int m_nScanningSignal;   //正在扫描端口
	int m_nScanWarnSignal;   //扫描报警端口
	int m_nBadSignal;        //不良品端口
	int m_nScannTime;        //扫描结果延时时间
	int m_ExitMarkSignal;          // 退出标记端口
	int m_CleanMarkFileSignal;     // 清空文档标志信号
	int m_StopScannerSignal;       // 完成工单中的一个小批次，停止扫描
	int m_nScanUse2DPort;  // 使用2D信号
	int m_nScanInputTimeOutPort;  // 手动输入超时
	int m_nScanEnablePWS;  // 启用PWS
	int m_nScanPWSOK;  // PWS OK
	int m_nScanPWSNG;  // PWS NG

	int m_nStartPort;   // start
	int m_nBusyPort;    // Busy
	int m_nReadyPort;   // Ready
	int m_nEndPort;     // End
	CString m_nCommIP;  //网络IO的IP
	int m_nCommPort;    //网络IO的端口

	CString m_s2DPath;   // UINT模式下，保存二维码信息
	// UINT模式下，检查二维码是否重复。默认等于2
	// 等于0，不检查二维码重复；
	// 等于1，不检查当前板的二维码重复（有些客户，当前板内容一样），但是需要与之前的二维码数据比较
	// 等于2，所有二维码都要检查是否重复。
	int m_nCheck2DBarcodeRepeat;    
	int m_nSavePeriod;   // 保存二维码数据的时间（单位天数），默认保存一个月

	bool m_bEnableDummy;
	bool m_bEnableFirstPrint;
	int m_iMappingDebugSet;   // Mapping调试设置(m_bEnableDummy=true时有效)：0=无；1=不良品数据对比；2=全部打印BIN1；3=打印设置BIN
	std::vector<int> m_iManualBinMap;   // m_iMappingDebugSet = 1 和 3时，手动设置的不良品信息记录

	int m_iVisionCheckPort;
	int m_iFinsPort;
	CString m_sFinsIP;

	DPoint_t m_dpRotateCenter{0, 0};  // 旋转中心

	//
	CString  m_FinishedLotIP;
	bool     m_EnableFinishedLot;
	CString  m_RemotLotURL;
	bool     m_EnableRemoteLotURL;
public:
    static CFactoryConfig *Instance();
    ~CFactoryConfig();

	void LoadConfig(void);
	void SaveConfig(void);

	// 立即保存函数，运行此函数，立刻保存到配置档
	void WriteData(const CString & section, const CString & variable, const CString & s);
	void WriteData(const CString & section, const CString & variable, int n);
	void WriteData(const CString & section, const CString & variable, double d);
	void WriteData(const CString & section, const CString & variable, bool b);

	// 立即读取函数，运行此函数，立刻读取配置档的内容
	CString GetData(const CString & section, const CString & variable, const CString & sD);
	int GetData(const CString & section, const CString & variable, int nD);
	double GetData(const CString & section, const CString & variable, double dD);
	bool GetData(const CString & section, const CString & variable, bool bD);

protected:
	CFactoryConfig(void);
private:
    static CFactoryConfig *_instance;
};
