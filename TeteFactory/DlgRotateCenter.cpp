// DlgRotateCenter.cpp : 实现文件
//

#include "stdafx.h"
#include "TeteFactory.h"
#include "DlgRotateCenter.h"
#include "afxdialogex.h"
#include "FactoryConfig.h"

// CDlgRotateCenter 对话框

IMPLEMENT_DYNAMIC(CDlgRotateCenter, CDialogEx)

CDlgRotateCenter::CDlgRotateCenter(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_ROTATE_CENTER, pParent)
{

}

CDlgRotateCenter::~CDlgRotateCenter()
{
}

void CDlgRotateCenter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgRotateCenter, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgRotateCenter::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlgRotateCenter 消息处理程序


void CDlgRotateCenter::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strx, stry;
	GetDlgItemText(IDC_EDIT1, strx);
	GetDlgItemText(IDC_EDIT3, stry);

	CFactoryConfig::Instance()->m_dpRotateCenter.x = _ttof(strx);
	CFactoryConfig::Instance()->m_dpRotateCenter.y = _ttof(stry);

	CDialogEx::OnOK();
}


BOOL CDlgRotateCenter::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString strx, stry;
	strx.Format(_T("%.3f"), CFactoryConfig::Instance()->m_dpRotateCenter.x);
	stry.Format(_T("%.3f"), CFactoryConfig::Instance()->m_dpRotateCenter.y);
	SetDlgItemText(IDC_EDIT1, strx);
	SetDlgItemText(IDC_EDIT3, stry);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
