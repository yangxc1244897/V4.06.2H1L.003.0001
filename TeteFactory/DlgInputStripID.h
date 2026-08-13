#pragma once


// CDlgInputStripID 对话框

class CDlgInputStripID : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgInputStripID)

public:
	CDlgInputStripID(CString& sScan, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgInputStripID();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_INPUT_STRIPID };
#endif
	CString& m_sScan;
	int m_nTimeOut;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
