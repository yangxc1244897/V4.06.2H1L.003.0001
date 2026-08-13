#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDlgFactorySet 对话框

class CDlgFactorySet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFactorySet)

public:
	CDlgFactorySet(class CTeteFactoryDlg * p, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgFactorySet();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FACTORY_SET };
#endif

	class CTeteFactoryDlg * m_pMainDlg;
	CBrush m_BkBrush;   // 背景刷
	CBrush m_EditBkBrush;  // 编辑框背景色
	CFont m_fntBtn;
	class CFactoryConfig* m_pFactoryConfig;
	
	void EnableContrlByWork(int nWorkMode);
	void EnableControlByRights();
	BOOL SaveParam();
	void InitBtns();
	void SetGroupCheckRadioTheme();
	void UpdateUIFromConfig();
	void SaveConfigFromUI();
	bool CreateDefaultFilePath();
	CString m_sOld2DPath;
	void Init2DRepeatMode();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonMarkfile();
	afx_msg void OnBnClickedButtonLogfile();
	CString m_sLogFilePath;
	CString m_sMarkFilePath;
	CString m_sLotIDXMLPath;
	CString m_sURLIP;
	CString m_sScripIDXMLPath;
	int m_nPort;
	int m_nWorkMode;
	virtual BOOL OnInitDialog();
	BOOL m_bOpenRMS;
	afx_msg void OnBnClickedCheckOpenrms();
	BOOL m_bOpenRollTip;
	CString m_sTipText;
	afx_msg void OnBnClickedCheckOpenrolltip();
	afx_msg void OnBnClickedRadioWorkAuto();
	afx_msg void OnBnClickedRadioWorkManul();
	afx_msg void OnBnClickedRadioWorkScan();
	CComboBox m_comboEndMark;
	CComboBox m_comboStartMark;
	afx_msg void OnBnClickedButtonLinktest();
	afx_msg void OnBnClickedButtonAdjustSet();
	afx_msg void OnBnClickedButtonSaveSet();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonCancelSet();
	afx_msg void OnBnClickedButtonMobFile();
	CString m_sMobFile;
	CString m_sDeviceNo;
	afx_msg void OnBnClickedButtonStartserver();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CString m_sMesIP;
	CString m_sRmsIP;
	CString m_sMarkFileHead;
	BOOL m_bMcCheck;
	CString m_sMcCheckTip;
	int m_nMCCheckTimes;
	afx_msg void OnBnClickedCheckMccheck();
	afx_msg void OnBnClickedButtonMarkPoint();
	BOOL m_bCheckStatus;
	CString m_s2DPath;
	afx_msg void OnBnClickedButtonMarkstamp();
	afx_msg void OnBnClickedCheckManulLoadstamp();
	afx_msg void OnBnClickedRadioWorkPonum();
	BOOL m_bMaxMarkTimesLimit;
	int m_nMaxMarkTimes;
	afx_msg void OnBnClickedCheckMaxMarktimes();
	afx_msg void OnBnClickedButtonUserManager();  //用户管理
	bool m_bCheckStripIDMatchLotID;
	CString m_sMesUrl;                            //MES系统URL
	void ChangeTab();                             //视图切换，非“系统设置”页
	afx_msg void OnBnClickedButtonDeviceConfig();
	afx_msg void OnBnClickedButtonLaserSet();
	afx_msg void OnBnClickedButtonCalibration();
	afx_msg void OnBnClickedButtonScannerSet();
	afx_msg void OnBnClickedButtonLotIDXmlPath();
	afx_msg void OnBnClickedButtonLinktest2();
	afx_msg void OnBnClickedButtonStampfile();
	CString m_sStampFile;
	BOOL m_bVisualSystem;
	afx_msg void OnBnClickedButtonNineCalc();
	afx_msg void OnBnClickedButtonScripidXmlPath();
	afx_msg void OnBnClickedButtonLaserUse();
	CString m_sMesUrl2;
	CString m_sMesUrl3;
	CComboBox m_combo2DRepeatMode;
	afx_msg void OnBnClickedButtonStartserver2();
	int m_iPort2;
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonGfh();
	CString m_sGFHPath;
	afx_msg void OnBnClickedButtonLaserParamSavePath();
	CString m_sLaserParamSave;
	afx_msg void OnBnClickedButtonDeleteData();
	BOOL m_bVisionCheck;
	afx_msg void OnBnClickedButtonStartserver3();
	CString m_sMesUrl4;
	CString m_sMesUrlProducttype;
	afx_msg void OnBnClickedButtonFinsConnect();
};
