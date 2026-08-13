#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDlgLaserUsage 对话框
const CString g_sLaserUsage = _T("config\\LaserUsage.ini");

class CDlgLaserUsage : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLaserUsage)

public:
	CDlgLaserUsage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgLaserUsage();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LASER_USAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
public:
	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonClear();          //清零（已使用时间）
	afx_msg void OnBnClickedButtonUsageSettime2();  //设置
private:
};
