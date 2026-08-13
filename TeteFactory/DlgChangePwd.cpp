// DlgChangePwd.cpp : 实现文件
//

#include "stdafx.h"
#include "TeteFactory.h"
#include "DlgChangePwd.h"
#include "afxdialogex.h"
#include "EncryptPwd.h"

// CDlgLoadChangePwd 对话框

IMPLEMENT_DYNAMIC(CDlgLoadChangePwd, CDialogEx)

CDlgLoadChangePwd::CDlgLoadChangePwd(const emUserAuthority_t& usertype, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CHANGE_PWD, pParent)
{
	m_UserType = usertype;
}

CDlgLoadChangePwd::~CDlgLoadChangePwd()
{
}

void CDlgLoadChangePwd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgLoadChangePwd, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgLoadChangePwd::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgLoadChangePwd 消息处理程序


void CDlgLoadChangePwd::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString oldpwd, newpwd, newpwd2;
	GetDlgItemText(IDC_EDIT_OLD_PWD, oldpwd);
	GetDlgItemText(IDC_EDIT_NEW_PWD, newpwd);
	GetDlgItemText(IDC_EDIT_SURE_PWD, newpwd2);

	if (!USERMNG->VerifyPwd(m_UserType, oldpwd)) {
		AfxMessageBox(_T("原密码错误！"));
		GetDlgItem(IDC_EDIT_OLD_PWD)->SetFocus();
		return;
	}

	if (newpwd != newpwd2) {
		AfxMessageBox(_T("新密码两次输入不一致！"));
		GetDlgItem(IDC_EDIT_NEW_PWD)->SetFocus();
		return;
	}

	USERMNG->ChangeUserPwd(m_UserType, newpwd);

	CDialogEx::OnOK();
}
