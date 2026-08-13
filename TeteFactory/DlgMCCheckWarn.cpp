// DlgMCCheckWarn.cpp : 实现文件
//

#include "stdafx.h"
#include "TeteFactory.h"
#include "DlgMCCheckWarn.h"
#include "afxdialogex.h"


// CDlgMCCheckWarn 对话框

IMPLEMENT_DYNAMIC(CDlgMCCheckWarn, CDialogEx)

CDlgMCCheckWarn::CDlgMCCheckWarn(const CString & s, bool bEnableTimer, bool bVerify, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_MCCHECKWARN, pParent)
{
	m_sContent = s;
	m_bVerify = bVerify;
	m_bEnableTimer = bEnableTimer;
}

CDlgMCCheckWarn::~CDlgMCCheckWarn()
{
}

void CDlgMCCheckWarn::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMCCheckWarn, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgMCCheckWarn::OnBnClickedOk)
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CDlgMCCheckWarn::OnBnClickedButtonCancel)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgMCCheckWarn 消息处理程序


void CDlgMCCheckWarn::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


HBRUSH CDlgMCCheckWarn::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_TIP
		|| pWnd->GetDlgCtrlID() == IDC_STATIC_TIMEOUT_TIP)
	{
		pDC->SetTextColor(STATIC_TITLE_COLOR);
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


BOOL CDlgMCCheckWarn::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_Font.CreatePointFont(250, _T("宋体"));
	m_fntBtn.CreatePointFont(200, _T("宋体"));
	SetDlgItemText(IDC_STATIC_TIP, m_sContent);   
	GetDlgItem(IDC_STATIC_TIP)->SetFont(&m_Font);
	GetDlgItem(IDOK)->SetFont(&m_fntBtn);
	GetDlgItem(IDC_BUTTON_CANCEL)->SetFont(&m_fntBtn);

	GetDlgItem(IDC_BUTTON_CANCEL)->ShowWindow(m_bVerify);

	if (m_bEnableTimer)
	{
		GetDlgItem(IDC_STATIC_TIMEOUT_TIP)->SetFont(&m_fntBtn);
		m_nTimeOut = 60;
		SetTimer(1, 1000, NULL);
	}
	else
	{
		GetDlgItem(IDC_STATIC_TIMEOUT_TIP)->ShowWindow(FALSE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDlgMCCheckWarn::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_Font.DeleteObject();
	m_fntBtn.DeleteObject();

	CDialogEx::OnClose();
}


void CDlgMCCheckWarn::OnBnClickedButtonCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CDlgMCCheckWarn::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (1 == nIDEvent)
	{
		m_nTimeOut--;
		CString s;
		s.Format(_T("剩余确认时间：%ds"), m_nTimeOut);
		SetDlgItemText(IDC_STATIC_TIMEOUT_TIP, s);
		if (m_nTimeOut <= 0)
		{
			m_nTimeOut = 60;
			KillTimer(1);
			CDialogEx::OnCancel();
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}
