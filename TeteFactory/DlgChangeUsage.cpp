// DlgChangeUsage.cpp : 实现文件
//

#include "stdafx.h"
#include "TeteFactory.h"
#include "DlgChangeUsage.h"
#include "afxdialogex.h"


// CDlgChangeUsage 对话框

IMPLEMENT_DYNAMIC(CDlgChangeUsage, CDialogEx)

CDlgChangeUsage::CDlgChangeUsage(bool useSign, CString hour, CString minute, CString caveatTime, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CHANGE_USAGE, pParent)
	, m_sign(useSign)
	, m_newHou(hour)
	, m_newMin(minute)
	, m_newCav(caveatTime)
{

}

CDlgChangeUsage::~CDlgChangeUsage()
{
}

void CDlgChangeUsage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_ChangeText1, m_ST_text1);
	DDX_Control(pDX, IDC_STATIC_ChangeText2, m_ST_text2);
	DDX_Control(pDX, IDC_STATIC_ChangeText3, m_ST_text3);
	DDX_Control(pDX, IDC_EDIT_CHANGE_HOUR, m_ED_hou);
	DDX_Control(pDX, IDC_EDIT_CHANGE_MINUTE, m_ED_min);
	DDX_Control(pDX, IDC_EDIT_CHANGE_CAVEAT, m_ED_cav);
}


BEGIN_MESSAGE_MAP(CDlgChangeUsage, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgChangeUsage::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgChangeUsage 消息处理程序
BOOL CDlgChangeUsage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (true == m_sign) {
		m_ST_text3.ShowWindow(false);
		m_ED_cav.ShowWindow(false);
		m_ED_hou.SetWindowTextW(m_newHou);
		m_ED_min.SetWindowTextW(m_newMin);
		m_ED_hou.SetFocus();
		m_ED_hou.SetSel(0, -1);
		this->SetWindowTextW(_T("修改激光器已使用时间"));
	} else {
		m_ST_text1.ShowWindow(false);
		m_ST_text2.ShowWindow(false);
		m_ED_hou.ShowWindow(false);
		m_ED_min.ShowWindow(false);
		m_ED_cav.SetWindowTextW(m_newCav);
		m_ED_cav.SetFocus();
		m_ED_cav.SetSel(0, -1);
		this->SetWindowTextW(_T("设置报警时间"));
	}

	return FALSE;
}

bool CDlgChangeUsage::ValueJudgment()     //输入数值的合法性判断
{
	if ( true == m_sign )  //设置使用时间
	{
		GetDlgItemText(IDC_EDIT_CHANGE_HOUR, m_newHou);
		GetDlgItemText(IDC_EDIT_CHANGE_MINUTE, m_newMin);
		if (m_newHou.IsEmpty()|| m_newMin.IsEmpty())
		{
			MessageBox(_T("数值不能为空！"), _T("参数错误"), MB_OK | MB_ICONWARNING);
			m_ED_hou.SetFocus();
			return false;
		}
		if ((_ttoi(m_newHou) > 10000) || ((_ttoi(m_newHou) == 10000)&&(_ttoi(m_newMin) != 0))) {
			MessageBox(_T("已使用必须小于激光器最大使用时间！"), _T("参数错误"), MB_OK | MB_ICONWARNING);
			m_ED_hou.SetFocus();
			m_ED_hou.SetSel(0, -1);
			return false;
		}
		if (_ttoi(m_newMin) >= 60) {
			MessageBox(_T("分钟数必须小于60！"), _T("参数错误"), MB_OK | MB_ICONWARNING);
			m_ED_min.SetFocus();
			m_ED_min.SetSel(0, -1);
			return false;
		}
	} else {  //设置报警时间
		GetDlgItemText(IDC_EDIT_CHANGE_CAVEAT, m_newCav);
		if (m_newCav.IsEmpty())
		{
			MessageBox(_T("报警时间不能为空！"), _T("参数错误"), MB_OK | MB_ICONWARNING);
			m_ED_cav.SetFocus();
			return false;
		}
		if ( _ttoi(m_newCav) > 10000 ) {
			MessageBox(_T("预设报警时间必须小于激光器最大使用时间！"), _T("参数错误"), MB_OK | MB_ICONWARNING);
			m_ED_cav.SetFocus();
			m_ED_cav.SetSel(0, -1);
			return false;
		}
	}
	return true;
}

void CDlgChangeUsage::OnBnClickedOk()
{
	//数值合法性判断
	if (!ValueJudgment()) {
		return;
	}

	CString g_sLaserUsage = _T("config\\LaserUsage.ini");
	if (true == m_sign) {
		WritePrivateProfileString(_T("LASERUSAGE"), _T("UsageHour"), m_newHou, g_sLaserUsage);
		WritePrivateProfileString(_T("LASERUSAGE"), _T("UsageMinute"), m_newMin, g_sLaserUsage);
		WritePrivateProfileString(_T("LASERUSAGE"), _T("UsageSecond"), _T("0"), g_sLaserUsage); //修改后，秒数重置为0
	} else {
		WritePrivateProfileString(_T("LASERUSAGE"), _T("CaveatTime"), m_newCav, g_sLaserUsage);
	}

	CDialogEx::OnOK();
}

void CDlgChangeUsage::GetLaserUsage(CString &hour, CString &minute)  //外部获取使用时间
{
	hour = m_newHou;
	minute = m_newMin;
}

void CDlgChangeUsage::GetLaserCaveat(CString &caveatTime)            //外部获取报警时间
{
	caveatTime = m_newCav;
}