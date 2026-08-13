#pragma once


// CDlgRotateCenter 对话框

class CDlgRotateCenter : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgRotateCenter)

public:
	CDlgRotateCenter(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgRotateCenter();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ROTATE_CENTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
};
