// DlgHardwareOption.cpp : 实现文件
//

#include "stdafx.h"
#include "TeteFactory.h"
#include "DlgOptionHardware.h"
#include "afxdialogex.h"
#include "tlMarkEngine.h"
#include "Configure.h"
#include "MarkPlatform.h"

// CDlgOptionHardware 对话框

IMPLEMENT_DYNAMIC(CDlgOptionHardware, CDialogEx)

CDlgOptionHardware::CDlgOptionHardware(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_OPTION_HARDWARE, pParent)
	, m_iDriverType(0)
	, m_iHeaderType(0)
	, m_iMainCtrlType(0)
	, m_sLaserFamily(_T(""))
	, m_IsMotionEnable(0)
	, m_IsVisionEnable(0)
	, m_iVisionType(0)
	, m_iMotionType(0)
	, m_IsAutoDriver(FALSE)
	, m_IsGateLowActive(TRUE)
{
	m_pConfig = CConfigure::Instance();
	m_pPlatform = CMarkPlatform::Instance();
}

CDlgOptionHardware::~CDlgOptionHardware()
{

}
void CDlgOptionHardware::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDCC_AUTO_DRIVER, m_IsAutoDriver);
	DDX_Check(pDX, IDCC_GATE_LEVEL, m_IsGateLowActive);
	DDX_CBIndex(pDX, IDCC_CARD_TYPE, m_iDriverType);
	DDX_CBIndex(pDX, IDCC_HEAD_TYPE, m_iHeaderType);
	DDX_CBIndex(pDX, IDCC_MAINCTRL_TYPE, m_iMainCtrlType);
	DDX_CBIndex(pDX, IDCC_PORT_IN0, m_iPortIn[0]);
	DDX_CBIndex(pDX, IDCC_PORT_IN1, m_iPortIn[1]);
	DDX_CBIndex(pDX, IDCC_PORT_IN2, m_iPortIn[2]);
	DDX_CBIndex(pDX, IDCC_PORT_IN3, m_iPortIn[3]);
	DDX_CBIndex(pDX, IDCC_PORT_IN4, m_iPortIn[4]);
	DDX_CBIndex(pDX, IDCC_PORT_IN5, m_iPortIn[5]);
	DDX_CBIndex(pDX, IDCC_PORT_IN6, m_iPortIn[6]);
	DDX_CBIndex(pDX, IDCC_PORT_IN7, m_iPortIn[7]);
	DDX_CBIndex(pDX, IDCC_PORT_IN8, m_iPortIn[8]);
	DDX_CBIndex(pDX, IDCC_PORT_IN9, m_iPortIn[9]);
	DDX_CBIndex(pDX, IDCC_PORT_OUT0, m_iPortOut[0]);
	DDX_CBIndex(pDX, IDCC_PORT_OUT1, m_iPortOut[1]);
	DDX_CBIndex(pDX, IDCC_PORT_OUT2, m_iPortOut[2]);
	DDX_CBIndex(pDX, IDCC_PORT_OUT3, m_iPortOut[3]);
	DDX_CBIndex(pDX, IDCC_PORT_OUT4, m_iPortOut[4]);
	DDX_CBIndex(pDX, IDCC_PORT_OUT5, m_iPortOut[5]);
	DDX_CBIndex(pDX, IDCC_PORT_OUT6, m_iPortOut[6]);
	DDX_CBIndex(pDX, IDCC_PORT_OUT7, m_iPortOut[7]);
	DDX_CBIndex(pDX, IDCC_PORT_OUT8, m_iPortOut[8]);
	DDX_CBIndex(pDX, IDCC_PORT_OUT9, m_iPortOut[9]);
	DDX_CBIndex(pDX, IDCC_SIGN_IN0, m_iSignIn[0]);
	DDX_CBIndex(pDX, IDCC_SIGN_IN1, m_iSignIn[1]);
	DDX_CBIndex(pDX, IDCC_SIGN_IN2, m_iSignIn[2]);
	DDX_CBIndex(pDX, IDCC_SIGN_IN3, m_iSignIn[3]);
	DDX_CBIndex(pDX, IDCC_SIGN_IN4, m_iSignIn[4]);
	DDX_CBIndex(pDX, IDCC_SIGN_IN5, m_iSignIn[5]);
	DDX_CBIndex(pDX, IDCC_SIGN_IN6, m_iSignIn[6]);
	DDX_CBIndex(pDX, IDCC_SIGN_IN7, m_iSignIn[7]);
	DDX_CBIndex(pDX, IDCC_SIGN_IN8, m_iSignIn[8]);
	DDX_CBIndex(pDX, IDCC_SIGN_IN9, m_iSignIn[9]);
	DDX_Check(pDX, IDCC_SIGN_IN_ENABLE0, m_IsSignInEnable[0]);
	DDX_Check(pDX, IDCC_SIGN_IN_ENABLE1, m_IsSignInEnable[1]);
	DDX_Check(pDX, IDCC_SIGN_IN_ENABLE2, m_IsSignInEnable[2]);
	DDX_Check(pDX, IDCC_SIGN_IN_ENABLE3, m_IsSignInEnable[3]);
	DDX_Check(pDX, IDCC_SIGN_IN_ENABLE4, m_IsSignInEnable[4]);
	DDX_Check(pDX, IDCC_SIGN_IN_ENABLE5, m_IsSignInEnable[5]);
	DDX_Check(pDX, IDCC_SIGN_IN_ENABLE6, m_IsSignInEnable[6]);
	DDX_Check(pDX, IDCC_SIGN_IN_ENABLE7, m_IsSignInEnable[7]);
	DDX_Check(pDX, IDCC_SIGN_IN_ENABLE8, m_IsSignInEnable[8]);
	DDX_Check(pDX, IDCC_SIGN_IN_ENABLE9, m_IsSignInEnable[9]);
	DDX_Check(pDX, IDCC_SIGN_OUT_ENABLE0, m_IsSignOutEnable[0]);
	DDX_Check(pDX, IDCC_SIGN_OUT_ENABLE1, m_IsSignOutEnable[1]);
	DDX_Check(pDX, IDCC_SIGN_OUT_ENABLE2, m_IsSignOutEnable[2]);
	DDX_Check(pDX, IDCC_SIGN_OUT_ENABLE3, m_IsSignOutEnable[3]);
	DDX_Check(pDX, IDCC_SIGN_OUT_ENABLE4, m_IsSignOutEnable[4]);
	DDX_Check(pDX, IDCC_SIGN_OUT_ENABLE5, m_IsSignOutEnable[5]);
	DDX_Check(pDX, IDCC_SIGN_OUT_ENABLE6, m_IsSignOutEnable[6]);
	DDX_Check(pDX, IDCC_SIGN_OUT_ENABLE7, m_IsSignOutEnable[7]);
	DDX_Check(pDX, IDCC_SIGN_OUT_ENABLE8, m_IsSignOutEnable[8]);
	DDX_Check(pDX, IDCC_SIGN_OUT_ENABLE9, m_IsSignOutEnable[9]);
	DDX_Control(pDX, IDCC_LASER_TYPE, m_ctlLaserType);
	DDX_CBString(pDX, IDCC_LASER_TYPE, m_sLaserFamily);
	DDX_Check(pDX, IDCC_MOTION_ENABLE, m_IsMotionEnable);
	DDX_Check(pDX, IDCC_VISION_ENABLE, m_IsVisionEnable);
	DDX_CBIndex(pDX, IDCC_VISION_TYPE, m_iVisionType);
	DDX_CBIndex(pDX, IDCC_MOTION_TYPE, m_iMotionType);
	DDX_Control(pDX, IDCC_CARD_TYPE, m_ctlDriverType);
	DDX_Control(pDX, IDCC_HEAD_TYPE, m_ctlHeadType);
	DDX_Control(pDX, IDCC_MAINCTRL_TYPE, m_ctlMainCtrl);
	DDX_Control(pDX, IDCC_MOTION_TYPE, m_ctlMotionType);
	DDX_Control(pDX, IDCC_VISION_TYPE, m_ctlVisionType);
}

BEGIN_MESSAGE_MAP(CDlgOptionHardware, CDialogEx)
	ON_BN_CLICKED(IDCB_APPLY, &CDlgOptionHardware::OnBnClickedApply)
	ON_BN_CLICKED(IDOK, &CDlgOptionHardware::OnBnClickedOk)
	ON_BN_CLICKED(IDCC_SIGN_IN_ENABLE0, &CDlgOptionHardware::OnClickedIdccSignInEnable0)
	ON_BN_CLICKED(IDCC_SIGN_IN_ENABLE1, &CDlgOptionHardware::OnClickedIdccSignInEnable1)
	ON_BN_CLICKED(IDCC_SIGN_IN_ENABLE2, &CDlgOptionHardware::OnClickedIdccSignInEnable2)
	ON_BN_CLICKED(IDCC_SIGN_IN_ENABLE3, &CDlgOptionHardware::OnClickedIdccSignInEnable3)
	ON_BN_CLICKED(IDCC_SIGN_IN_ENABLE4, &CDlgOptionHardware::OnClickedIdccSignInEnable4)
	ON_BN_CLICKED(IDCC_SIGN_IN_ENABLE5, &CDlgOptionHardware::OnClickedIdccSignInEnable5)
	ON_BN_CLICKED(IDCC_SIGN_IN_ENABLE6, &CDlgOptionHardware::OnClickedIdccSignInEnable6)
	ON_BN_CLICKED(IDCC_SIGN_IN_ENABLE7, &CDlgOptionHardware::OnClickedIdccSignInEnable7)
	ON_BN_CLICKED(IDCC_SIGN_IN_ENABLE8, &CDlgOptionHardware::OnClickedIdccSignInEnable8)
	ON_BN_CLICKED(IDCC_SIGN_IN_ENABLE9, &CDlgOptionHardware::OnClickedIdccSignInEnable9)
	ON_BN_CLICKED(IDCC_SIGN_OUT_ENABLE0, &CDlgOptionHardware::OnClickedIdccSignOutEnable0)
	ON_BN_CLICKED(IDCC_SIGN_OUT_ENABLE1, &CDlgOptionHardware::OnClickedIdccSignOutEnable1)
	ON_BN_CLICKED(IDCC_SIGN_OUT_ENABLE2, &CDlgOptionHardware::OnClickedIdccSignOutEnable2)
	ON_BN_CLICKED(IDCC_SIGN_OUT_ENABLE3, &CDlgOptionHardware::OnClickedIdccSignOutEnable3)
	ON_BN_CLICKED(IDCC_SIGN_OUT_ENABLE4, &CDlgOptionHardware::OnClickedIdccSignOutEnable4)
	ON_BN_CLICKED(IDCC_SIGN_OUT_ENABLE5, &CDlgOptionHardware::OnClickedIdccSignOutEnable5)
	ON_BN_CLICKED(IDCC_SIGN_OUT_ENABLE6, &CDlgOptionHardware::OnClickedIdccSignOutEnable6)
	ON_BN_CLICKED(IDCC_SIGN_OUT_ENABLE7, &CDlgOptionHardware::OnClickedIdccSignOutEnable7)
	ON_BN_CLICKED(IDCC_SIGN_OUT_ENABLE8, &CDlgOptionHardware::OnClickedIdccSignOutEnable8)
	ON_BN_CLICKED(IDCC_SIGN_OUT_ENABLE9, &CDlgOptionHardware::OnClickedIdccSignOutEnable9)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_RECOVER_IN, &CDlgOptionHardware::OnBnClickedButtonRecoverIn)
	ON_BN_CLICKED(IDC_BUTTON_RECOVER_OUT, &CDlgOptionHardware::OnBnClickedButtonRecoverOut)
END_MESSAGE_MAP()


// CDlgOptionHardware 消息处理程序
BOOL CDlgOptionHardware::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化  
	m_IsAutoDriver = m_pConfig->m_IsAutoDriver ? TRUE : FALSE;
	m_ctlDriverType.AddString(L"未识别");
	m_ctlDriverType.AddString(L"旧 9608");
	m_ctlDriverType.AddString(L"新 9608");
	m_ctlDriverType.AddString(L"旧 9632");
	m_ctlDriverType.AddString(L"新 9632");
	if (m_IsAutoDriver)
		m_iDriverType = m_pPlatform->MarkDriver()->GetType();
	else
		m_iDriverType = m_pConfig->m_iDriverType;

	m_ctlHeadType.AddString(L"模拟振镜");
	m_ctlHeadType.AddString(L"数字振镜");
	m_iHeaderType = m_pConfig->m_iScanerType;

	m_ctlMainCtrl.AddString(L"老主控箱");
	m_ctlMainCtrl.AddString(L"新主控箱");
	m_iMainCtrlType = m_pConfig->m_iMainCtrlVer;

	m_ctlMotionType.AddString(L"TETE9206");
	m_ctlMotionType.AddString(L"GOOGOL");
	m_IsMotionEnable = m_pConfig->m_IsMotionEnable ? 1 : 0;
	m_iMotionType = m_pConfig->m_iMotionType;


	m_ctlVisionType.AddString(L"OPT");
	m_IsVisionEnable = m_pConfig->m_IsVisionEnable ? 1 : 0;
	m_iVisionType = m_pConfig->m_iVisionType;

	m_ctlLaserType.Clear();
	CLaserModule laserModules;
	laserModules.GetAllModule(m_vsLaserModules);
	int iCnt = m_vsLaserModules.size();
	CString sdata; CStringA strdat;
	for (int i = 0; i < iCnt; i++)
	{
		strdat = m_vsLaserModules[i].sName;
		sdata = strdat;
		m_ctlLaserType.AddString(sdata);
	}
	tlModule_t mod = laserModules.GetDefault();
	strdat = mod.sName;
	sdata = strdat;
	int iIndex = m_ctlLaserType.FindString(-1, sdata);
	m_ctlLaserType.SetCurSel(iIndex);
	m_iDeviceType = iIndex;

	IMarkEngine *pTheEngine = m_pPlatform->MarkEngine();
	TPortSign_t portSignIN;
	for (int i = 0; i < 10; i++)
	{
		portSignIN.iID = i;
		pTheEngine->GetEngineCtrl(emMECMD_DIGIT_IN, &portSignIN);
		m_iPortIn[i] = portSignIN.iPin;
		m_iSignIn[i] = portSignIN.iSign;
		m_IsSignInEnable[i] = portSignIN.IsEnable;
	}

	TPortSign_t portSignOut;
	for (int i = 0; i < 10; i++)
	{
		portSignOut.iID = i;
		pTheEngine->GetEngineCtrl(emMECMD_DIGIT_OUT, &portSignOut);
		m_iPortOut[i] = portSignOut.iPin;
		m_iSignOut[i] = portSignOut.iSign;
		m_IsSignOutEnable[i] = portSignOut.IsEnable;
	}

	m_IsGateLowActive = !m_pConfig->GetBoolValue("MarkDriver", "GateLevel", 0);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
void CDlgOptionHardware::OnBnClickedApply()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	BOOL bNeedRestartSoftware = FALSE;

	if ((m_pConfig->m_iDriverType != m_iDriverType)
		|| (m_pConfig->m_iScanerType != m_iHeaderType)
		|| (m_pConfig->m_iMainCtrlVer != m_iMainCtrlType)
		|| (m_iDeviceType != m_ctlLaserType.GetCurSel()))
	{
		bNeedRestartSoftware = TRUE;
	}
	int iGateLevel = !m_IsGateLowActive;
	m_pConfig->SetDataValue("MarkDriver", "GateLevel", iGateLevel);
	m_pPlatform->MarkDriver()->SetDriverCtrl(CMD_GATE_LEVEL, &iGateLevel);

	m_pConfig->m_IsAutoDriver = (m_IsAutoDriver == TRUE) ? true : false;
	m_pConfig->m_iDriverType = m_iDriverType;
	m_pConfig->m_iScanerType = m_iHeaderType;
	m_pConfig->m_iMainCtrlVer = m_iMainCtrlType;
	m_pConfig->m_IsMotionEnable = m_IsMotionEnable != 0 ? true : false;
	m_pConfig->m_iMotionType = m_iMotionType;
	m_pConfig->m_IsVisionEnable = m_IsVisionEnable != 0 ? true : false;
	m_pConfig->m_iVisionType = m_iVisionType;
	CLaserModule laserModules;
	int iLaserIndex = m_ctlLaserType.GetCurSel();
	laserModules.SetDefault(&m_vsLaserModules[iLaserIndex]);

	IMarkEngine *pTheEngine = m_pPlatform->MarkEngine();
	TPortSign_t portSignIN;
	for (int i = 0; i < 10; i++)
	{
		portSignIN.iID = i;
		portSignIN.iPin = m_iPortIn[i];
		portSignIN.iSign = m_iSignIn[i];
		portSignIN.IsEnable = (m_IsSignInEnable[i] == TRUE) ? true : false;
		portSignIN.iPeriod = 0;
		pTheEngine->SetEngineCtrl(emMECMD_DIGIT_IN, &portSignIN);
	}

	TPortSign_t portSignOut;
	for (int i = 0; i < 10; i++)
	{
		portSignOut.iID = i;
		portSignOut.iPin = m_iPortOut[i];
		portSignOut.iSign = m_iSignOut[i];
		portSignOut.IsEnable = (m_IsSignOutEnable[i] == TRUE) ? true : false;
		portSignOut.iPeriod = 0;
		pTheEngine->SetEngineCtrl(emMECMD_DIGIT_OUT, &portSignOut);
	}

	if (0 != pTheEngine->SaveConfig(m_pPlatform->GetSystemPath("config\\mark.xml"))) {
		AfxMessageBox(_T("保存标记引擎配置失败！"));
	}

	// CCD模块处理
	IVisionDriver* pVisionDriver = CMarkPlatform::Instance()->VisionDriver();
	if (m_IsVisionEnable)
	{
		// 打开CCD
		if (0 != pVisionDriver->Open(m_pPlatform))
		{
			AfxMessageBox(_T("CCD视觉模块打开失败！\
			\r\n1、请先确认CCD是否已经开启；\
			\r\n2、在工具栏“CCD设置”中确认CCD客户端的IP和端口是否设置正常。"));
		}
	}
	else
	{
		pVisionDriver->Close();
	}
	
	if (bNeedRestartSoftware)
	{
		if (IDYES == MessageBox(_T("硬件设置已经更改，需要重启软件才能生效，是否立刻重启？"),
			_T("重启软件提示"), MB_YESNO | MB_ICONWARNING))
		{
			m_pConfig->Delete();
			PostQuitMessage(0);
			ShellExecuteA(nullptr, "open", "TeteFactory.exe", NULL, GetSystemDir(), SW_SHOWNORMAL);
		}
	}
}
void CDlgOptionHardware::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedApply();
	m_pConfig->Delete();
	CDialogEx::OnOK();
}
void CDlgOptionHardware::OnClickedIdccSignInEnable0()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (TRUE == m_IsSignInEnable[0])
	{
		GetDlgItem(IDCC_PORT_IN0)->EnableWindow(TRUE);
		GetDlgItem(IDCC_SIGN_IN0)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDCC_PORT_IN0)->EnableWindow(FALSE);
		GetDlgItem(IDCC_SIGN_IN0)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}
void CDlgOptionHardware::OnClickedIdccSignInEnable1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (TRUE == m_IsSignInEnable[1])
	{
		GetDlgItem(IDCC_PORT_IN1)->EnableWindow(TRUE);
		GetDlgItem(IDCC_SIGN_IN1)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDCC_PORT_IN1)->EnableWindow(FALSE);
		GetDlgItem(IDCC_SIGN_IN1)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}
void CDlgOptionHardware::OnClickedIdccSignInEnable2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (TRUE == m_IsSignInEnable[2])
	{
		GetDlgItem(IDCC_PORT_IN2)->EnableWindow(TRUE);
		GetDlgItem(IDCC_SIGN_IN2)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDCC_PORT_IN2)->EnableWindow(FALSE);
		GetDlgItem(IDCC_SIGN_IN2)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}
void CDlgOptionHardware::OnClickedIdccSignInEnable3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (TRUE == m_IsSignInEnable[3])
	{
		GetDlgItem(IDCC_PORT_IN3)->EnableWindow(TRUE);
		GetDlgItem(IDCC_SIGN_IN3)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDCC_PORT_IN3)->EnableWindow(FALSE);
		GetDlgItem(IDCC_SIGN_IN3)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}
void CDlgOptionHardware::OnClickedIdccSignInEnable4()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (TRUE == m_IsSignInEnable[4])
	{
		GetDlgItem(IDCC_PORT_IN4)->EnableWindow(TRUE);
		GetDlgItem(IDCC_SIGN_IN4)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDCC_PORT_IN4)->EnableWindow(FALSE);
		GetDlgItem(IDCC_SIGN_IN4)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}
void CDlgOptionHardware::OnClickedIdccSignInEnable5()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (TRUE == m_IsSignInEnable[5])
	{
		GetDlgItem(IDCC_PORT_IN5)->EnableWindow(TRUE);
		GetDlgItem(IDCC_SIGN_IN5)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDCC_PORT_IN5)->EnableWindow(FALSE);
		GetDlgItem(IDCC_SIGN_IN5)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}
void CDlgOptionHardware::OnClickedIdccSignInEnable6()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (TRUE == m_IsSignInEnable[6])
	{
		GetDlgItem(IDCC_PORT_IN6)->EnableWindow(TRUE);
		GetDlgItem(IDCC_SIGN_IN6)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDCC_PORT_IN6)->EnableWindow(FALSE);
		GetDlgItem(IDCC_SIGN_IN6)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}
void CDlgOptionHardware::OnClickedIdccSignInEnable7()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (TRUE == m_IsSignInEnable[7])
	{
		GetDlgItem(IDCC_PORT_IN7)->EnableWindow(TRUE);
		GetDlgItem(IDCC_SIGN_IN7)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDCC_PORT_IN7)->EnableWindow(FALSE);
		GetDlgItem(IDCC_SIGN_IN7)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}
void CDlgOptionHardware::OnClickedIdccSignInEnable8()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (TRUE == m_IsSignInEnable[8])
	{
		GetDlgItem(IDCC_PORT_IN8)->EnableWindow(TRUE);
		GetDlgItem(IDCC_SIGN_IN8)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDCC_PORT_IN8)->EnableWindow(FALSE);
		GetDlgItem(IDCC_SIGN_IN8)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}
void CDlgOptionHardware::OnClickedIdccSignInEnable9()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (TRUE == m_IsSignInEnable[9])
	{
		GetDlgItem(IDCC_PORT_IN9)->EnableWindow(TRUE);
		GetDlgItem(IDCC_SIGN_IN9)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDCC_PORT_IN9)->EnableWindow(FALSE);
		GetDlgItem(IDCC_SIGN_IN9)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}
void CDlgOptionHardware::OnClickedIdccSignOutEnable0()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (TRUE == m_IsSignOutEnable[0])
	{
		GetDlgItem(IDCC_PORT_OUT0)->EnableWindow(TRUE);
		GetDlgItem(IDCC_SIGN_OUT0)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDCC_PORT_OUT0)->EnableWindow(FALSE);
		GetDlgItem(IDCC_SIGN_OUT0)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}
void CDlgOptionHardware::OnClickedIdccSignOutEnable1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (TRUE == m_IsSignOutEnable[1])
	{
		GetDlgItem(IDCC_PORT_OUT1)->EnableWindow(TRUE);
		GetDlgItem(IDCC_SIGN_OUT1)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDCC_PORT_OUT1)->EnableWindow(FALSE);
		GetDlgItem(IDCC_SIGN_OUT1)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}
void CDlgOptionHardware::OnClickedIdccSignOutEnable2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (TRUE == m_IsSignOutEnable[2])
	{
		GetDlgItem(IDCC_PORT_OUT2)->EnableWindow(TRUE);
		GetDlgItem(IDCC_SIGN_OUT2)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDCC_PORT_OUT2)->EnableWindow(FALSE);
		GetDlgItem(IDCC_SIGN_OUT2)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}
void CDlgOptionHardware::OnClickedIdccSignOutEnable3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (TRUE == m_IsSignOutEnable[3])
	{
		GetDlgItem(IDCC_PORT_OUT3)->EnableWindow(TRUE);
		GetDlgItem(IDCC_SIGN_OUT3)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDCC_PORT_OUT3)->EnableWindow(FALSE);
		GetDlgItem(IDCC_SIGN_OUT3)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}
void CDlgOptionHardware::OnClickedIdccSignOutEnable4()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (TRUE == m_IsSignOutEnable[4])
	{
		GetDlgItem(IDCC_PORT_OUT4)->EnableWindow(TRUE);
		GetDlgItem(IDCC_SIGN_OUT4)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDCC_PORT_OUT4)->EnableWindow(FALSE);
		GetDlgItem(IDCC_SIGN_OUT4)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}
void CDlgOptionHardware::OnClickedIdccSignOutEnable5()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (TRUE == m_IsSignOutEnable[5])
	{
		GetDlgItem(IDCC_PORT_OUT5)->EnableWindow(TRUE);
		GetDlgItem(IDCC_SIGN_OUT5)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDCC_PORT_OUT5)->EnableWindow(FALSE);
		GetDlgItem(IDCC_SIGN_OUT5)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}
void CDlgOptionHardware::OnClickedIdccSignOutEnable6()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (TRUE == m_IsSignOutEnable[6])
	{
		GetDlgItem(IDCC_PORT_OUT6)->EnableWindow(TRUE);
		GetDlgItem(IDCC_SIGN_OUT6)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDCC_PORT_OUT6)->EnableWindow(FALSE);
		GetDlgItem(IDCC_SIGN_OUT6)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}
void CDlgOptionHardware::OnClickedIdccSignOutEnable7()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (TRUE == m_IsSignOutEnable[7])
	{
		GetDlgItem(IDCC_PORT_OUT7)->EnableWindow(TRUE);
		GetDlgItem(IDCC_SIGN_OUT7)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDCC_PORT_OUT7)->EnableWindow(FALSE);
		GetDlgItem(IDCC_SIGN_OUT7)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}
void CDlgOptionHardware::OnClickedIdccSignOutEnable8()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (TRUE == m_IsSignOutEnable[8])
	{
		GetDlgItem(IDCC_PORT_OUT8)->EnableWindow(TRUE);
		GetDlgItem(IDCC_SIGN_OUT8)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDCC_PORT_OUT8)->EnableWindow(FALSE);
		GetDlgItem(IDCC_SIGN_OUT8)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}
void CDlgOptionHardware::OnClickedIdccSignOutEnable9()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (TRUE == m_IsSignOutEnable[9])
	{
		GetDlgItem(IDCC_PORT_OUT9)->EnableWindow(TRUE);
		GetDlgItem(IDCC_SIGN_OUT9)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDCC_PORT_OUT9)->EnableWindow(FALSE);
		GetDlgItem(IDCC_SIGN_OUT9)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}

void CDlgOptionHardware::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (1 == nIDEvent)
	{

	}
	CDialogEx::OnTimer(nIDEvent);
}

void CDlgOptionHardware::OnBnClickedButtonRecoverIn()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	for (int i = 0; i < 10; i++)
	{
		m_iPortIn[i] = i;
		m_iSignIn[i] = emSIGN_HIGH;
		m_IsSignInEnable[i] = FALSE;

		m_iPortOut[i] = i;
		m_iSignOut[i] = emSIGN_HIGH;
		m_IsSignOutEnable[i] = FALSE;
	}
	m_iPortIn[emSIGN_IN_HARDWORK] = 0;
	m_iSignIn[emSIGN_IN_HARDWORK] = emSIGN_LOW;
	m_iSignIn[emSIGN_IN_SYSWARN] = emSIGN_LOW;
	m_IsSignInEnable[emSIGN_IN_HARDWORK] = TRUE;
	m_IsSignInEnable[emSIGN_IN_SYSWARN] = TRUE;
	m_IsSignInEnable[emSIGN_IN_SHUTTER] = TRUE;
	m_IsSignInEnable[emSIGN_IN_LASERLIGNT] = TRUE;
	m_IsSignOutEnable[emSIGN_OUT_REDLIGHT] = TRUE;
	UpdateData(FALSE);
}


void CDlgOptionHardware::OnBnClickedButtonRecoverOut()
{
	// TODO: 在此添加控件通知处理程序代码
}
