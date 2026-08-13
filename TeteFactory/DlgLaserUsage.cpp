// LaserUsage.cpp : 实现文件
//

#include "stdafx.h"
#include "TeteFactory.h"
#include "DlgLaserUsage.h"
#include "afxdialogex.h"
#include "DlgChangeUsage.h"
#include "DlgFactoryPwd.h"
#include "SettingChangeDetected.h"

// CDlgLaserUsage 对话框

IMPLEMENT_DYNAMIC(CDlgLaserUsage, CDialogEx)

CDlgLaserUsage::CDlgLaserUsage(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_LASER_USAGE, pParent)
{

}

CDlgLaserUsage::~CDlgLaserUsage()
{
}

void CDlgLaserUsage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgLaserUsage, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_USAGE_CLEAR, &CDlgLaserUsage::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_USAGE_SETTIME2, &CDlgLaserUsage::OnBnClickedButtonUsageSettime2)
END_MESSAGE_MAP()

BOOL CDlgLaserUsage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString caveatTime, sUsedTime;
	GetPrivateProfileString(_T("LaserDiodeTime"), _T("DiodeTime"), _T("0"), sUsedTime.GetBuffer(100), 100, g_sLaserUsage);
	sUsedTime.ReleaseBuffer();
	GetPrivateProfileString(_T("LaserDiodeTime"), _T("CaveatTime"), _T("20000"), caveatTime.GetBuffer(100), 100, g_sLaserUsage);
	caveatTime.ReleaseBuffer();

	SetDlgItemText(IDC_EDIT_USED, sUsedTime);
	SetDlgItemText(IDC_EDIT_ALARM, caveatTime);
	return TRUE;
}


HBRUSH CDlgLaserUsage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性


	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CDlgLaserUsage::OnBnClickedButtonClear()   //清零（已使用时间）
{
	USER_VERIFY_RET(USER_O_DEVICE_CLEAR_ZERO);

	if (IDYES != MessageBox(_T("此动作不可撤销，确定是否清零？"), _T("警告"), MB_YESNO)) {
		return;
	}	
	WritePrivateProfileString(_T("LaserDiodeTime"), _T("DiodeTime"), _T("0"), g_sLaserUsage);
	SETCHANGEDETECT->RecordVarChange(_T("DiodeTime"));
	SetDlgItemText(IDC_EDIT_USED, _T("0"));
}
void CDlgLaserUsage::OnBnClickedButtonUsageSettime2()  //设置
{
	CString caveatTime;
	GetDlgItemText(IDC_EDIT_ALARM, caveatTime);
	WritePrivateProfileString(_T("LaserDiodeTime"), _T("CaveatTime"), caveatTime, g_sLaserUsage);
	SETCHANGEDETECT->CompareAndChangeStringVar(_T("CaveatTime"), caveatTime);
	CDialogEx::OnOK();
} 