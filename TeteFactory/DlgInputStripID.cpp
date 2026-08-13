// DlgInputStripID.cpp : 实现文件
//

#include "stdafx.h"
#include "TeteFactory.h"
#include "DlgInputStripID.h"
#include "afxdialogex.h"


// CDlgInputStripID 对话框

IMPLEMENT_DYNAMIC(CDlgInputStripID, CDialogEx)

CDlgInputStripID::CDlgInputStripID(CString& sScan, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_INPUT_STRIPID, pParent)
	,m_sScan(sScan)
{

}

CDlgInputStripID::~CDlgInputStripID()
{
}

void CDlgInputStripID::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgInputStripID, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgInputStripID::OnBnClickedOk)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgInputStripID 消息处理程序


void CDlgInputStripID::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT1, m_sScan);
	if (m_sScan.IsEmpty())
	{
		AfxMessageBox(_T("Strip ID为空！"));
		GetDlgItem(IDC_EDIT1)->SetFocus();
		return;
	}
	CDialogEx::OnOK();
}


BOOL CDlgInputStripID::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	GetDlgItem(IDC_EDIT1)->SetFocus();
	m_nTimeOut = 60;
	SetTimer(1, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDlgInputStripID::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (1 == nIDEvent)
	{
		m_nTimeOut--;
		CString s;
		s.Format(_T("手动输入剩下时间：%ds"), m_nTimeOut);
		SetDlgItemText(IDC_STATIC_TIP, s);
		if (m_nTimeOut <= 0)
		{
			m_nTimeOut = 60;
			KillTimer(1);
			CDialogEx::OnCancel();
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}
