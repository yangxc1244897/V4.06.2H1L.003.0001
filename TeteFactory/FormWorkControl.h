#pragma once
#include "afxcmn.h"
#include "FactoryConfig.h"
#include "afxwin.h"
#include "ViewFactory.h"
#include "tlMarkEngine.h"
#include "Wafer\WaferEngine.h"
#include "MyFtp.h"
#include "FactoryServer.h"
#include "FactoryMesComm.h"
#include "ShowTip.h"
#include "MyButton.h"
#include "ViewAllBin.h"
#include "DlgFactoryPwd.h"
#include "SocketCom.h"
#include "SocketComIO.h"
#include "C7230IOcomm.h"
#include "ThirdParty\InputSignal.h"
#include "Scan2DSignal.h"
#include "StartMarkSingal.h"
#include "C2DContentRepeatCheck.h"
#include "MyNSocketServer.h"


const CString g_sMTNormal = _T("NORMAL");
const CString g_sMTCard = _T("CARD");
const CString g_sMTUnit = _T("UNIT");
const CString g_sMTCardAndUnit = _T("CARDANDUNIT");

// CFormWorkControl 对话框

class CFormWorkControl : public CDialogEx, public IMsgWndInterface
{
	DECLARE_DYNAMIC(CFormWorkControl)

public:
	CFormWorkControl( class CTeteFactoryDlg * p, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFormWorkControl();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORM_WORK_CONTROL };
#endif

public:
	////////////////////////////////////////////////////////////////////////
	/// 远程通讯函数接口

	// 该软件作为服务端，接收来自客户端的标记指令
	int32_t PraserFrame(char *pMsgBuff, int32_t iSize, CStringArray &sArray);
	virtual int32_t doBin(char *pMsgBuff, int32_t iSize) override;  //废弃
	virtual int32_t doMark(char *pMsgBuff, int32_t iSize) override;//废弃
	virtual int32_t doAutoLot(char *sRecvBuff, int32_t iSize) override;    // 自动接收印章图档名称//废弃
	virtual int32_t doCheckMarkStatus(char *sRecvBuff, int32_t iSize) override;    // 检查标记状态//废弃
	virtual int32_t doPosMatrix(char *sRecvBuff, int32_t iSize) override;    // 更改阵列位置
	virtual int32_t do2DData(char *sRecvBuff, int32_t iSize) override;       // 接收2D数据
	virtual int32_t doFinishLot(char *sRecvBuff, int32_t iSize) override;       // 结批
	virtual int32_t doReciveSetVisionResult(char *sRecvBuff, int32_t iSize) override;   // 接收视觉制程设置结果指令
	virtual int32_t doReciveSetCharCheckResult(char *sRecvBuff, int32_t iSize) override;   // 接收标后检测视觉制程设置结果指令
	virtual int32_t doRecivePLCSwitchProcessResult(char *sRecvBuff, int32_t iSize) override; // 接收PLC制程设置结果指令

	virtual void PrintMesMessage(int nType, const TCHAR* sMsg) override;

    // 客户端
	int32_t doLot(char *pMsgBuff, int32_t iSize);         // 获取印章图档
	int32_t doStartLot(char *sRecvBuff, int32_t iSize);   // 开始批次
	int32_t doStopLot(char *sRecvBuff, int32_t iSize);    // 结束批次

public:
	////////////////////////////////////////////////////////////////////////
	/// 类自定义变量

	CBrush m_BkBrush;   // 背景刷
	CBrush m_EditBkBrush;  // 编辑框背景色
	int m_bInit;
	CRect m_rcClient;
	CFactoryConfig * m_pConfig;
	CFont m_TipFont;
	CFont m_FileFont;
	CFont m_BtnFont;
	CFont m_TextFont;  //文本内容、字体名称所用
	CViewFactory * m_pMarkContentView;
	CViewAllBin *m_pViewAllBin;
	CStdioFile  m_LogStdioFile;
	BOOL m_bOpenFile;
	CWaferGraphDoc * m_pWaferDoc;
	CWaferMapDoc * m_pMapDoc;
	class CTeteFactoryDlg * m_pMainDlg;
	//int m_nSpeed;
	//double m_dbFreq;
	//double m_dbPower;
	//CString m_sPenName;
	std::vector<int> m_nSpeed;
	std::vector<double> m_dbFreq;
	std::vector<double> m_dbPower;
	std::vector<CString> m_sPenName;
	BOOL m_bSysOK;
	int m_nMarkFlag;   // 标记标志 -1 空闲  1 正在标记   0 板卡数据传输完成，需要检测板卡剩余指令 
	int m_nLotFlag;   // 批次标识，1 已经开批， 0 未开批
	IMarkDriver * m_pMarkDriver;
	IMarkEngine * m_pMarkEngine;
	CString m_sSysStatus;   // 系统错误
	CString m_sHardWareStatus;   // 电源错误
	CString m_sLaserStatus;    // 激光器
	CString m_sShutterStatus;   // Shutter
	double m_dbMarkTime;
	int m_nMarkCount;
	CFactoryMesComm * m_pFactoryMesComm;
	CFactoryServer * m_pServer;   // 视觉
	CFactoryServer * m_pServerPLC;   // SystemMonitor自动化软件
	int m_nPrePort;   // 上一次端口
	int m_nPrePortPLC;   // 上一次端口
	int m_nPreVisionPort;
	int m_nPreFinsPort;
	CString m_nPreFinsIp;
	int m_nPreWorkMode;   // 上一次工作模式
	CString m_sMobFilePath;    // 当前加载的阵列修正文件, 注意更新
	CString m_sMarkFileName;   // 标记图档文件名称，带后缀， 注意更新
	CString m_sMarkStampNameNonSuffix;   // 标记印章图档文件名称，不带后缀， 注意更新
	CString m_sLotID;  // 批次号,前8位
	DWORD m_dwStartTick;

	/**
	* 在“扫入Lot后加入本地图档”模式下，有以下几种情况需要送MC检查板子，
	* 1. 初次加载图档，需要提示送MC检查；
	* 2. 当相同图档加载四次后，第五次要提示送MC检查；
	* 3. 如果两次加载不同的图档，则也要提示送MC检查;
	* 注意：需要考虑中途关闭软件的情况，利用保存到配置档去解决这个问题
	*/
	int m_nRetryTimes;         // 重复次数
	CString m_sCheckMarkFile;  // 检查的标记图档
	BOOL m_bScanBarcode;       // 是否扫描条码

	CString m_sMarkFilePath;
	BOOL m_bVariable[10];      // 印章文本对象是否为变量

	BOOL m_bManualStopMark;   // 手动停止标记标志
	BOOL m_bSaveOriginDoc = FALSE;
	BOOL m_bCCDOffseted = FALSE;
	BOOL m_bManualStartMark;  // 手动开启标记

	vector<int> m_iBinMap;
	vector<CString> m_vs2DData;
	vector<CString> m_vs2DData2;  // 第二组2D数据

	BOOL m_bEnablePLCCheck = FALSE;    //  是否启用PLC检查

	//lrf 
	CString m_LoginUserName;              //当前用户名
	int m_LoginUserType;                  //当前用户类型
	CString m_LaserUsagePath;             //（激光器使用情况）配置文件路径
	CSocketCom m_SocketComClient;         // Socket客户端（扫描枪）
	C7230IOcomm m_IOComm;                 //IO控制模块（7230卡）
	CString m_markType;                   //打标类型（XML数据中读取）
	CString m_varName;                    //变量名（XML数据中读取）
	vector <wstring> m_vsFontName;          //字体名称（XML数据中读取）
	CString m_sLaserParamCur;              //激光参数名（XML数据中读取）
	BOOL m_IfConnectCCD;                  //CCD连接状态
	CString m_sLotIDCur;                     //当前扫入的lotID
	CString m_sStripIDCur;                   //当前扫入的StriptID
	CString m_sLotIDBindStripIDCur;          // 基板自带StripID绑定的华天LotID
	class CMySoap *m_pMySoap;
	BOOL m_bSetVisionOK;
	BOOL m_bPLCSwitchProcessOK;
	BOOL m_bSwitchVisionCheckProcessOK;
	enum{UISIGNAL_MAP, UISIGNAL_VISION, UISIGNAL_2D, UISIGNAL_PWS, UISIGNAL_DUMMY,UISIGNAL_NET};
	CScan2DSignal m_Scan2DSignal;
	CStartMarkSingal m_StartMarkSingal;

	C2DContentRepeatCheck m_2DRepeatCheck;   // 二维码数据的重复，需要检查每一台设备前一段时间

	BOOL m_bFirstLoadDocForDummy;  // 首次加载图档，专为dummy功能

	CVisionCheckServer* m_pVisionCheckServer;    // 视觉检测
	//CFactoryServer* m_pVisionCheckServer;    // 视觉检测
	BOOL m_bVisionCheck;                  //视觉检测连接状态
public:
	////////////////////////////////////////////////////////////////////
	/// 类自定义函数接口

	/*! 控件位置调整 */
	void AdjustControl();

	/*! 检查系统是否正在标记 */
	BOOL IsMarking();
	/**
	* @brief 开始标记
	* @param iFlag 标记标志 等于0，表示正常检查状态；等于1，表示不检查CCD、2D、map状态
	*/
	int32_t StartMark(int iFlag = 0);
	/*！ 标记文档对象中的Mark函数调用完成后，调用该函数开启一个定时器，检查板卡是否已经标记完成。*/
	void FinishMark(int nflag);
	/*！brief 标记开始后，设置标记状态，包括停用IO信号检测，控件使能，板卡状态。*/
	void SetMarkStatus();
	/*！brief 标记前系统检测。包括硬件报警、是否正在标记、是否存在图档。*/
	BOOL PreMarkCheck();

	/*! 开启IO信号检测，并设置相关控件的使能状态*/
	void LoadJob();
	/*! 光闭IO信号检测，并设置相关控件的使能状态*/
	void CancelJob();

	/**
	* @brief "调试模式1" -- 手动加载图档
	*/
	void ManualLoadFile();
	/**
	* @brief "扫组装批号模式" -- 通过组装批号加载图档
	* @param sPonum 组装批号，与印章模板名称一致
	* @remark 从打印图档的地址下的本地配置档TWD-TWFB.txt中查找组装批号对应的图档名
	*/
	BOOL LoadStampByPonum(const CString & sPonum);

	/**
	* @brief 根据图档名称或印章名称加载图档，或从RMS服务器上下载与同名的印章模板并加载，替换原图档中的印章模板
	* @param sMarkFilePath 图档名称，包含路径
	* @param sStampFilePath 印章模板名称，包含路径
	* @remark sStampFilePath不为空时，表示用sStampFilePath对应的印章内容，替换sMarkFilePath图档中的印章内容
	*/
	BOOL LoadContent(const CString & sMarkFilePath, const CString & sStampFilePath = NULL);

	void ResetStatus();  // 清空内存中的图档，以及相关对象变量显示和图档名称显示，清空标记计数和激光参数，清空消息显示框
	BOOL EndLog();
	void UpdateShowView();      // 更新显示视图
	void UpdateObjectList(CWaferGraph * pWafer);
	BOOL StartLog(const CString & sName);
	void PrintMessage(int nType, const CString & s, UINT nFlag = 0x3);  // 0x01表示显示在UI中，0x02表示保存在LOG中
	BOOL DownloadFile(const CString & sSrcFile, const CString & sDstFile);
	BOOL DownLoadFontFile();
	void SetStaticText(UINT uCtlID, const CString & s);
	void SwitchWorkMode(int nMode);
	void ParseReturnData(const CString & s, std::map<CString, CString> & maps);
	BOOL StartServer(unsigned short uport);  // 开启服务器
	BOOL StopServer();   // 暂停服务器
	BOOL StartServerPLC(unsigned short uport);  // 开启服务器
	BOOL StartFinsClientPLC(CString strIP, int port);
	BOOL StopServerPLC();   // 暂停服务器
	void ClearDialog();
	void UpdateSet();   // 修改设置后，重新更新
	BOOL ContectTest(const char * ip, unsigned short uport);
	BOOL CheckClose();  // 检查是否可以关闭对话框
	BOOL StartLot(const CString & sOperID, const CString & sLotID);   //开批
	BOOL IsSwitchSetTab();  // 是否可以切换到设置页面
	void SetMarkCount(int n);  // 设置标记数量
	BOOL ResetMapOffsetFile();   // 复位阵列修正，恢复原始数据
	BOOL FindNewMobFile();   // 获取最新的阵列修正文件名，如果不存在，则新创建一个
	BOOL LoadMapOffsetFile(const CString & sFileName);    // 加载阵列修正文件
	BOOL CheckMarkFileNameHead(const CString & sFileName);    // 检查印章图档头部
	BOOL IsMCCheck();   // 产品是否需要送检。通富微电需求
	int VerifyRepeatLotID(const CString & sLotID);   // 判断LotID在本机是否已经扫描过, 返回LotID在本机调用的次数
	BOOL IsMarkFileFirstUse(const CString & sMarkFile);   // 检查标记图档是否初次被调用
	BOOL IsEqualStampSize();  /*! 图档和印章模板分开加载时，判断两者的印章大小是否一致 */
	BOOL IsMaxMarkTimes();    /*! 打印次数是否达到最大打印次数 */
	BOOL DeleteOldMobFile();
	BOOL CopyMobFile(const CString & sOldFile, const CString & sNewFile);
	BOOL SaveOriginDoc();   // 金泰软件使用CCD，发送定位坐标过来；加载图档时旋转，修改阵列位置时旋转
	BOOL ResetOriginDoc();
	void RemoveCR(char *sRecvBuff, int32_t& iSize);
	BOOL IsEqualPenName(const std::vector<string> & vsPenName, std::vector<int>& virefLayerID);  // 查询印章模板与阵列文件中的激光参数是否一致。防止如果印章模板中的参数顺序与阵列文件中不一致，需要将阵列文件中激光参数名称对应的层号返回出来
	void ShowLaserParam();
	void Show2DAndMapInfo();
	void Clear2DData();
	void ClearMapData();
	BOOL Check2DBarcode();
	BOOL CheckStripMap();
	BOOL IsCancelJob();
	void ChangeArrayBin_0();   /*! 如果存在多个BIN，则将阵列中所有的BIN改为BIN0, BIN0为正常印章 */
	void ChangeObjLayerID(const std::vector<int>& viLayerID);
	BOOL VariableSubstitution(const std::map<CString, CString>& mapValue);
	void CheckTextSave();    // 检查是否已经保存文本对象，如果没有保存，则自动保存
	bool CheckCaveatTime();                       //（开始标记前）检查是否已达到报警值
	void UpdateLaserDiodeTime(bool bOpen);        // 更新激光器的上电时间
	void InitDiodeTime();

	bool CheckCCDStatus();                        //检测CCD是否连接
	bool GetAndCheckLotID(CString &sLotID);       //LotID检测与转化
	bool VerifyStripID(const CString &sStripID);         //检查StripID批次及重复情况  (即“检查2D图档”)
	bool LoadSeal(CString lotID);                 //加载印章
	bool GetStampInfoFromXml(CString lotID, CString &sFilePath, CString &sStampFilePath, map<CString, CString>& textValue);          //从XML中解析印章数据
	void GetFontNameInfo(CString fontname);       //获取字体名称信息
	bool VerifyFontName();                        //对校验xml中的字体名称是否与图档中一致
	bool GetDataFromScripIDXml(const CString& xmlName, CStringArray &binInfo, int &column, CStringArray &barcordInfo, CStringArray &barcordInfo2);  //从ScriptID.XML中解析相关数据
	int GetNumberFromCString();       //从CString中取出末尾包含的数字值
	bool GetMesInfoFromXml(const CString& xmlInfo, std::map<CString, CString> &varValue);      //从MES返回的xml中解析所需变量
	bool GetMesInfoFromXmlAndGFH(const CString& xmlInfo, std::map<CString, CString> &varValue);
	bool GetVarRelationFromGFH(const CString& sGFHName, std::map<CString,CString> &varValue);      //从规范号文件中获取变量
	BOOL ReplaceVarByWebService();                //根据WebService通讯内容替换变量
	BOOL CheckStripIDMatchLotIDByWebService();
	BOOL ConnectMes();                            //连接MES系统（WebServices）
	int32_t ChangeBin(CStringArray &binInfo, int column);                             //2D Mapping 修改BIN
	int32_t ResolveBinInfo(CStringArray &binInfo, int column, CStringArray &sArray);  //解析BIN数据
	int32_t Write2DBarcodeInfo(const CStringArray &barcordInfo, CStringArray &barcordInfo2);                      //写入二维码数据
	bool Verify2DData();
	bool SetCommIOStatus(int port, int sigle);            //设置IO口信号
	bool GetCommIOStatus(int port, unsigned int &single); //获取IO口信号
	void UpdateIOStatus(int type);   //更新状态到界面控件

	int ScannerTimerProcess();                            //扫描计时器--处理主函数
	int Scanned2DProcess(int nScanNo = 1, int nScanTimes = 1);     // 2D扫描成功后，处理流程
	BOOL GetScannerContent(int nScanNo, CString &sScan);  //获取自动扫描内容（StripID）
	BOOL ScannerContentProcess(CString scanstr);          //根据扫描内容进行处理

	BOOL Scan1SignalEffective(int times = 0);             // 扫描枪1触发信号是否有效
	BOOL Scan2SignalEffective(int times = 0);             // 扫描枪2触发信号是否有效
	BOOL ManualInputScanStripID(CString& sScan);          //扫描失败，手动输入StripID
	void SetPulseSignal(int port, bool status, int delaytime_ms);  //设置脉冲信号？
	BOOL CleanMarkFileSignalEffective();                  // 清空文档信号是否有效
	BOOL StopScannerSignalEffective();                    //检查是否需要停止标记   （暂时弃用）
	void DetectionProcessSignal();                        // 侦测流程结束信号
	void VerifyMutilTrack();                              // 判断是否为多轨道，如果是则显示菜单按钮，否则显示普通按钮  （暂时弃用）
	int VerifyRepeatStripID(const CString & sStripID);   // 判断StripID在本机是否已经扫描过, 返回StripID在本机调用的次数
	void RecordMarkedStripID();                          // 记录已经扫描过的StripID
	BOOL SwitchVisionProcess();
	BOOL IsIgnoreCCDCheckAndSwitchVisionProcess();
	void EnableDummy(bool b);
	bool IsDummy();
	bool IsFirstPrint();  // 首条打印，属于正式打印，但是不会检查二维码和保存二维码信息
	void EnableFirstPrint(bool b);
	bool RightConfirmBox(const emUserOperatorType_t& emUserRight, bool bChangeUserToOper = false);
	CString GetLotIDCur();   // 获取当前卡的卡号，主要用于厂内基板号和厂外基板号的兼容
	/**
	* @brief 判断当前批次是否启用了MAPPING熟悉
	*/
	bool IsMapping();
	/**
	* @brief 单独生成参数文件，以供客户调取
	* @param sLotID 批次号
	*/
	bool SaveLaserParamFile(const CString& sLotID);
	/**
	* @brief 清空二维码数据库内容
	*/

	void Delete2DBarcodeContent();

	BOOL StartVisionCheckServer(unsigned short uport);  // 开启视觉检测服务器
	BOOL StopVisionCheckServer();   // 暂停视觉检测服务器

	/**
	* @读取任意编码格式文件的内容
	*/
	void ReadFileContent(const CString&filepath);

	// 复位Mapping调试设置
	void ResetMappingDebugSet();

	// 设置Mapping调试
	bool SetMappingDebug();

	/**
	* @brief 刷新某个背景透明的控件的背景。比如，静态文本框、List列表
	*/
	void UpdateTransparentControlBK(UINT uID);

	/**
	* @brief 刷新整个窗口的背景
	*/
	void UpdateBK();

	/**
	* @brief 激光参数显示在面板上
	* @param index 显示第几个参数
	*/
	void ShowLaserParamLeft(int index);
	/**
	* @brief 清空激光参数显示
	*/
	void ClearLaserParamShow();

private:
		CString m_stripIDInfo;
		CString m_stripID;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonMapAdjust();
	afx_msg void OnBnClickedButtonLoadSet();
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedButtonCell();
	afx_msg void OnBnClickedButtonMatrix();
	afx_msg void OnBnClickedButtonArea();
	afx_msg void OnBnClickedButtonStartlot();
	afx_msg void OnBnClickedButtonStoplot();
	afx_msg void OnBnClickedButtonLoadJob();
	afx_msg void OnBnClickedButtonCanceljob();
	afx_msg void OnBnClickedButtonStartmark();
	afx_msg void OnBnClickedButtonStopmark();
	virtual void OnOK();
	CStatic m_ViewFrame;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	CShowTip m_ctrlTips;
	CStatic m_LeftFrame;
	CStatic m_ctrlViewFrame_Stamp;
	afx_msg void OnBnClickedButtonLaserParam();
	afx_msg void OnBnClickedButtonEditTextContont();
	afx_msg void OnBnClickedButtonFinishEditContent();
	CInputStatic m_sStatusVision;
	CInputStatic m_sStatusStartMark;
	CInputStatic m_sStatusMapping;
	CInputStatic m_sStatus2DBarcode;
	CInputStatic m_sStatus2DBarcodeBegin;
	CInputStatic m_sStatusPWS;
	CInputStatic m_sNetConnect;
	afx_msg LRESULT OnScanProcess(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnStartProcess(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedCheckTryDummy();
	afx_msg void OnBnClickedCheckFirstPrint();
	


	afx_msg void OnBnClickedButton1();
	CListBox m_listLaserParam;
	afx_msg void OnLbnSelchangeListLaserParam();
	// 校验工单号 http协议处理
	BOOL CheckMesInfo();
	BOOL QueryEqpInfo();
	int m_nMapOKCount;
	int m_nMapNGCount;
};
