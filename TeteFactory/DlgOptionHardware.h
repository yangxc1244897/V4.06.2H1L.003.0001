#pragma once
#include "afxwin.h"
#include "LaserModule.h"

// CDlgOptionHardware 对话框

class CDlgOptionHardware : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgOptionHardware)

public:
	CDlgOptionHardware(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgOptionHardware();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_OPTION_HARDWARE };
#endif

protected:
	class CConfigure* m_pConfig;
	class CMarkPlatform *m_pPlatform;
	std::vector<tlModule_t> m_vsLaserModules;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    int m_iDeviceType;
    int m_iDriverType;
    int m_iHeaderType;
    int m_iMainCtrlType;
    int m_iPortIn[10];
    int m_iPortOut[10];
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedApply();
    afx_msg void OnBnClickedOk();
    int m_iSignIn[10];
    BOOL m_IsSignInEnable[10];
    int m_iSignOut[10];
    BOOL m_IsSignOutEnable[10];
    afx_msg void OnClickedIdccSignInEnable0();
    afx_msg void OnClickedIdccSignInEnable1();
    afx_msg void OnClickedIdccSignInEnable2();
    afx_msg void OnClickedIdccSignInEnable3();
    afx_msg void OnClickedIdccSignInEnable4();
    afx_msg void OnClickedIdccSignInEnable5();
    afx_msg void OnClickedIdccSignInEnable6();
    afx_msg void OnClickedIdccSignInEnable7();
    afx_msg void OnClickedIdccSignInEnable8();
    afx_msg void OnClickedIdccSignInEnable9();
    afx_msg void OnClickedIdccSignOutEnable0();
    afx_msg void OnClickedIdccSignOutEnable1();
    afx_msg void OnClickedIdccSignOutEnable2();
    afx_msg void OnClickedIdccSignOutEnable3();
    afx_msg void OnClickedIdccSignOutEnable4();
    afx_msg void OnClickedIdccSignOutEnable5();
    afx_msg void OnClickedIdccSignOutEnable6();
    afx_msg void OnClickedIdccSignOutEnable7();
    afx_msg void OnClickedIdccSignOutEnable8();
    afx_msg void OnClickedIdccSignOutEnable9();
    CComboBox m_ctlLaserType;
    CString m_sLaserFamily;
    int m_IsMotionEnable;
    int m_iMotionType;
    int m_IsVisionEnable;
    int m_iVisionType;    
    CComboBox m_ctlDriverType;
    CComboBox m_ctlHeadType;
    CComboBox m_ctlMainCtrl;
    CComboBox m_ctlMotionType;
    CComboBox m_ctlVisionType;
    BOOL m_IsAutoDriver;
    BOOL m_IsGateLowActive;
    afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonRecoverIn();
	afx_msg void OnBnClickedButtonRecoverOut();
};
