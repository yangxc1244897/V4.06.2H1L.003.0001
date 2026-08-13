#pragma once
#include "UserMananger.h"


// CDlgLoadChangePwd 对话框

class CDlgLoadChangePwd : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLoadChangePwd)

public:
	CDlgLoadChangePwd(const emUserAuthority_t& usertype, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgLoadChangePwd();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CHANGE_PWD };
#endif

protected:
	emUserAuthority_t m_UserType;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
