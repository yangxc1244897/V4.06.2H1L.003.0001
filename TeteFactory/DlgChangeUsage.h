#pragma once
#include "afxwin.h"


// CDlgChangeUsage 对话框

class CDlgChangeUsage : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgChangeUsage)

public:
	CDlgChangeUsage(bool useSign, CString hour, CString minute, CString caveatTime, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgChangeUsage();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CHANGE_USAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
private:
	bool m_sign;  //调用标志  true：使用时间    false：报警时间
	CStatic m_ST_text1;
	CStatic m_ST_text2;
	CStatic m_ST_text3;
	CEdit m_ED_hou;
	CEdit m_ED_min;
	CEdit m_ED_cav;
	CString m_newHou;
	CString m_newMin;
	CString m_newCav;
	bool ValueJudgment();       //输入数值的合法性判断
	afx_msg void OnBnClickedOk();
public:
	void GetLaserUsage(CString &hour,CString &minute);   //外部获取使用时间
	void GetLaserCaveat(CString &caveatTime);            //外部获取报警时间
};
