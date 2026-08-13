#pragma once
#include "afxwin.h"
#include "UserMananger.h"

// CDlgFactoryPwd 对话框
class CDlgFactoryPwd : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFactoryPwd)

public:
	CDlgFactoryPwd(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgFactoryPwd();
	bool CheckPwd(const emUserAuthority_t& usertype);

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FACTORY_PWD };
#endif
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog(); 
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CComboBox m_comboUserName;
};
