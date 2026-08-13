#pragma once


// CDlgMarkTimesSet 对话框

class CDlgMarkTimesSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMarkTimesSet)

public:
	CDlgMarkTimesSet(int & nTimes, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMarkTimesSet();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MARKTIMES_SET };
#endif

protected:
	int & m_nTimes;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
