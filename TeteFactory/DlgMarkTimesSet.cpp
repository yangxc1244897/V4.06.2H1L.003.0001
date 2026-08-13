// DlgMarkTimesSet.cpp : 实现文件
//

#include "stdafx.h"
#include "TeteFactory.h"
#include "DlgMarkTimesSet.h"
#include "afxdialogex.h"
#include "FactoryConfig.h"


// CDlgMarkTimesSet 对话框

IMPLEMENT_DYNAMIC(CDlgMarkTimesSet, CDialogEx)

CDlgMarkTimesSet::CDlgMarkTimesSet(int & nTimes, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_MARKTIMES_SET, pParent)
	, m_nTimes(nTimes)
{

}

CDlgMarkTimesSet::~CDlgMarkTimesSet()
{
}

void CDlgMarkTimesSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMarkTimesSet, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgMarkTimesSet::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgMarkTimesSet 消息处理程序


BOOL CDlgMarkTimesSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetDlgItemInt(IDC_EDIT_COUNT, m_nTimes);
	SetDlgItemInt(IDC_EDIT_TOTAL_COUNT, CFactoryConfig::Instance()->m_nMaxMarkTimes);
	//((CEdit*)GetDlgItem(IDC_EDIT_COUNT))->SetSel(0, -1);
	//GetDlgItem(IDC_EDIT_COUNT)->SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDlgMarkTimesSet::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nTimes = GetDlgItemInt(IDC_EDIT_COUNT);
	CFactoryConfig::Instance()->m_nMaxMarkTimes = GetDlgItemInt(IDC_EDIT_TOTAL_COUNT);

	CDialogEx::OnOK();
}
