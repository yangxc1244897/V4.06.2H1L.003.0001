#pragma once


// CDlgMCCheckWarn 对话框

class CDlgMCCheckWarn : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMCCheckWarn)

public:
	CDlgMCCheckWarn(const CString & s, bool bEnableTimer, bool bVerify = false, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMCCheckWarn();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MCCHECKWARN};
#endif

	CFont m_Font;
	CFont m_fntBtn;
	CString m_sContent;
	bool m_bVerify;
	bool m_bEnableTimer;

	int m_nTimeOut;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
