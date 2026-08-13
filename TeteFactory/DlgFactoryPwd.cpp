// DlgFactoryPwd.cpp : 实现文件
//

#include "stdafx.h"
#include "TeteFactory.h"
#include "DlgFactoryPwd.h"
#include "afxdialogex.h"
#include "DlgChangePwd.h"


// CDlgFactoryPwd 对话框

IMPLEMENT_DYNAMIC(CDlgFactoryPwd, CDialogEx)

CDlgFactoryPwd::CDlgFactoryPwd( CWnd* pParent /*= NULL*/)
	: CDialogEx(IDD_DIALOG_FACTORY_PWD, pParent)
{
}

CDlgFactoryPwd::~CDlgFactoryPwd()
{
}

void CDlgFactoryPwd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CB_USERNAME, m_comboUserName);
}


BEGIN_MESSAGE_MAP(CDlgFactoryPwd, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgFactoryPwd::OnBnClickedOk)
END_MESSAGE_MAP()

// CDlgFactoryPwd 消息处理程序

BOOL CDlgFactoryPwd::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 获取所有的用户
	std::vector<CString> vsUser;
	USERMNG->GetAllUser(vsUser);
	for (const auto& item : vsUser)
	{
		m_comboUserName.AddString(item);
	}

	// 设置当前用户
	m_comboUserName.SetCurSel(USERMNG->GetCurUser());

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


bool CDlgFactoryPwd::CheckPwd(const emUserAuthority_t& usertype)
{
	CString inputPwd;
	GetDlgItemText(IDC_EDIT_PWD, inputPwd);

	if (!USERMNG->VerifyPwd(usertype, inputPwd))
	{
		AfxMessageBox(_T("密码错误！"));
		GetDlgItem(IDC_EDIT_PWD)->SetWindowText(_T(""));
		GetDlgItem(IDC_EDIT_PWD)->SetFocus();
		return false;
	}

	return true;
}

void CDlgFactoryPwd::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	int iSel = m_comboUserName.GetCurSel();
	if (-1 == iSel)
	{
		AfxMessageBox(_T("请先选择一个用户！"));
		return;
	}
	if (!CheckPwd(emUserAuthority_t(iSel))) {
		return;
	}

	USERMNG->ChangeUser(emUserAuthority_t(iSel));

	CDialogEx::OnOK();
}
