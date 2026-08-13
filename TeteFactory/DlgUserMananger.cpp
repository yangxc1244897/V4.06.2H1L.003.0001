// DlgUserMananger.cpp : 实现文件
//

#include "stdafx.h"
#include "TeteFactory.h"
#include "DlgUserMananger.h"
#include "afxdialogex.h"
#include "DlgChangePwd.h"


// CDlgUserMananger 对话框

IMPLEMENT_DYNAMIC(CDlgUserMananger, CDialogEx)

CDlgUserMananger::CDlgUserMananger(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_USER_MANANGER, pParent)
{

}

CDlgUserMananger::~CDlgUserMananger()
{
}

void CDlgUserMananger::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_USER, m_listUser);
	DDX_Control(pDX, IDC_LIST_RIGHTS, m_listRights);
}


BEGIN_MESSAGE_MAP(CDlgUserMananger, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_PWD, &CDlgUserMananger::OnBnClickedButtonChangePwd)
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_LBN_SELCHANGE(IDC_LIST_USER, &CDlgUserMananger::OnLbnSelchangeListUser)
END_MESSAGE_MAP()


// CDlgUserMananger 消息处理程序


void CDlgUserMananger::OnBnClickedButtonChangePwd()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sNewPwd;
	CDlgLoadChangePwd dlg((emUserAuthority_t)m_listUser.GetCurSel());
	dlg.DoModal();
}


BOOL CDlgUserMananger::OnInitDialog()
{

	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	std::vector<CString> vsUserName;
	USERMNG->GetAllUser(vsUserName);
	for (int i = 0; i < MAX_USER_TYPE_COUNT; i++)
	{
		m_listUser.AddString(vsUserName[i]);
	}
	m_listUser.SetCurSel(USERMNG->GetCurUser());

	initRightList(USERMNG->GetCurUser());

	checkChangePwdStatus();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


BOOL CDlgUserMananger::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialogEx::PreTranslateMessage(pMsg);
}


HBRUSH CDlgUserMananger::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CDlgUserMananger::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}

void CDlgUserMananger::initRightList(int iUser)
{
	m_listRights.DeleteAllItems();

	CRect rc;
	m_listRights.GetClientRect(&rc);
	CString str;
	str.Format(_T("权限,%d;详细描述,%d"), 150, 1000);
	m_listRights.SetColumnHeader(str);
	m_listRights.SetGridLines(TRUE);

	std::set<emUserOperatorType_t> vUserAuthority, vAllAuthority;
	USERMNG->GetUserRights(USER_A_ADMIN, vAllAuthority);
	USERMNG->GetUserRights((emUserAuthority_t)iUser, vUserAuthority);
	int index = 0;
	for (auto it = vAllAuthority.begin(); it != vAllAuthority.end(); it++)
	{
		m_listRights.InsertItem(index, _T(""));
		m_listRights.SetItemData(index, (DWORD_PTR)(*it));
		m_listRights.SetItemControl(CHECKBOX, SELECT_RIGHT, index, 0, g_sUserOperatorCaption[(*it)]);
		m_listRights.AttachEvent_HandlerForItemControl(index, 0, BN_CLICKED, pFuncListCheck);
		m_listRights.SetItemText(index, 1, g_sUserOperatorDetails[(*it)]);
		index++;
	}
	m_listRights.LockWindowUpdate();
	// 更新勾选
	index = 0;
	for (auto it = vAllAuthority.begin(); it != vAllAuthority.end(); it++)
	{
		ControlTypeInfo* pControlType = m_listRights.GetItemControlInfo(index, 0);
		// 判断权限是否属于当前选中的用户
		if (vUserAuthority.end() != std::find(vUserAuthority.begin(), vUserAuthority.end(), (*it)))
		{
			((CButton*)pControlType->m_pWnd)->SetCheck(true);
		}



		// 管理员权限的权限，不能被改动
		if (iUser > USER_A_ADMIN && (USERMNG->GetCurUser() == USER_A_ADMIN)) {
			pControlType->m_pWnd->EnableWindow(TRUE);
		}
		else {
			pControlType->m_pWnd->EnableWindow(FALSE);
		}

		index++;

	}
	m_listRights.UnlockWindowUpdate();
	m_listRights.MapControlsForSubItems();
	m_listRights.ShowControls(TRUE);
	CHeaderCtrl* pHeaderCtrl = (CHeaderCtrl*)m_listRights.GetHeaderCtrl();
	pHeaderCtrl->EnableWindow(FALSE);




}

//按钮回调函数
void pFuncListCheck(ControlTypeInfo* pControlTypeInfo) // pointer to window sending message
{
	CDlgUserMananger* pDlg = (CDlgUserMananger*)pControlTypeInfo->m_pParent;
	switch (pControlTypeInfo->m_enControlType)
	{
	case CHECKBOX:
	{
		switch (pControlTypeInfo->m_enMethonType)
		{
		case SELECT_RIGHT:
		{
			pDlg->checkSelectRight((CButton*)pControlTypeInfo->m_pWnd, pControlTypeInfo->m_nItem);
		}
		break;
		default:
			break;
		}
	}
	break;
	default:
		break;
	}

}

void CDlgUserMananger::OnLbnSelchangeListUser()
{
	// TODO: 在此添加控件通知处理程序代码
	int iSel = m_listUser.GetCurSel();
	if (-1 == iSel)
		return;

	initRightList(iSel);
	checkChangePwdStatus();
}

void CDlgUserMananger::checkSelectRight(CButton* pCheck, int item)
{
	if (nullptr == pCheck)
		return;

	ControlTypeInfo* pControlType = m_listRights.GetItemControlInfo(item, 0);
	bool bCheck = ((CButton*)pControlType->m_pWnd)->GetCheck();
	m_listRights.SetItemTextColor(item, 0, bCheck ? RGB(0, 0, 255) : RGB(200, 200, 200));
	emUserOperatorType_t right = (emUserOperatorType_t)m_listRights.GetItemData(item);

	int iSel = m_listUser.GetCurSel();
	if (bCheck)
		USERMNG->AddRight((emUserAuthority_t)iSel, right);
	else
		USERMNG->RemoveRight((emUserAuthority_t)iSel, right);
}

void CDlgUserMananger::checkChangePwdStatus()
{
	int iSel = m_listUser.GetCurSel();
	if (USERMNG->GetCurUser() == USER_A_ADMIN
		|| (USERMNG->GetCurUser() == USER_A_ENGINEER && (iSel == USER_A_ENGINEER || iSel == USER_A_TECH || iSel == USER_A_OPER))
		|| (USERMNG->GetCurUser() == USER_A_TECH && (iSel == USER_A_TECH || iSel == USER_A_OPER))
/*		|| (USERMNG->GetCurUser() == USER_A_OPER && (iSel == USER_A_OPER))*/)
	{
		GetDlgItem(IDC_BUTTON_CHANGE_PWD)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_CHANGE_PWD)->EnableWindow(FALSE);
	}
}