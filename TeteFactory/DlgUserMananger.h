#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ThirdParty\ReportCtrl.h"
#include "UserMananger.h"

// CDlgUserMananger 对话框

class CDlgUserMananger : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgUserMananger)

public:
	CDlgUserMananger(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgUserMananger();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_USER_MANANGER
	};
#endif

protected:
	friend void pFuncListCheck(ControlTypeInfo* pControlTypeInfo); // pointer to window sending message
	void initRightList(int iUser);
	void checkSelectRight(CButton* pCheck, int item);
	void checkChangePwdStatus();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_listUser;
	CReportCtrl m_listRights;
	afx_msg void OnBnClickedButtonChangePwd();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();
	afx_msg void OnLbnSelchangeListUser();
};
