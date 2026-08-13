#pragma once

#include "ColorLed.h"
#include "afxcmn.h"
#include "afxwin.h"
// CDlgStartup 对话框

class CDlgStartup : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgStartup)

public:
	CDlgStartup(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgStartup();

    UINT_PTR m_nIDEvent;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_STARTUP };
#endif
    void UpdateStartupInfo(void);
    void SystemInit(void);
    bool IsStarted;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedCancel();
    CListBox  m_ctlSysInfoList;
    CProgressCtrl m_ctlProgressCtrl;
    CColorLed m_ctlSysStat;
    CColorLed m_ctlSysOff;
    CColorLed m_ctlLaserOff;
    CColorLed m_ctlShutterOff;
    virtual BOOL OnInitDialog();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};
