// DlgFactorySet.cpp : 实现文件
//

#include "stdafx.h"
#include "TeteFactory.h"
#include "DlgFactorySet.h"
#include "afxdialogex.h"
#include "FactoryConfig.h"
#include "DlgFactoryPwd.h"
#include "TeteFactoryDlg.h"
#include "DlgMarkPoint.h"
#include "MarkPlatform.h"
#include "DlgUserMananger.h"
#include "DlgMultiCalib.h"
#include "DlgOptionHardware.h"
#include "DlgScannerSet.h"
#include "DlgAnchorPointSet.h"
#include "DlgLaserUsage.h"
#include "SettingChangeDetected.h"
#include "FinsTCP//FinsTcpClient.h"
// CDlgFactorySet 对话框


IMPLEMENT_DYNAMIC(CDlgFactorySet, CDialogEx)

CDlgFactorySet::CDlgFactorySet(CTeteFactoryDlg * p, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_FACTORY_SET, pParent)
	, m_sLogFilePath(_T(""))
	, m_sMarkFilePath(_T(""))
	, m_sLotIDXMLPath(_T(""))
	,m_sScripIDXMLPath(_T(""))
	, m_nPort(0)
	, m_nWorkMode(0)
	, m_bOpenRMS(FALSE)
	, m_bOpenRollTip(FALSE)
	, m_sTipText(_T(""))
	, m_sMobFile(_T(""))
	, m_sDeviceNo(_T(""))
	, m_sMesIP(_T(""))
	, m_sRmsIP(_T(""))
	, m_sMarkFileHead(_T(""))
	, m_bMcCheck(FALSE)
	, m_sMcCheckTip(_T(""))
	, m_nMCCheckTimes(0)
	, m_bCheckStatus(FALSE)
	, m_s2DPath(_T(""))
	, m_bMaxMarkTimesLimit(FALSE)
	, m_nMaxMarkTimes(0)
	, m_sStampFile(_T(""))
	, m_bVisualSystem(TRUE)
	, m_sMesUrl2(_T(""))
	, m_iPort2(0)
	, m_sGFHPath(_T(""))
	, m_sLaserParamSave(_T(""))
	, m_bVisionCheck(FALSE)
{
	m_pMainDlg = p;
	m_BkBrush.CreateSolidBrush(BKCOLOR);
	m_EditBkBrush.CreateSolidBrush(EDIT_BKCOLOR);
	m_fntBtn.CreatePointFont(100, _T("Microsoft Sans Serif"));
	m_pFactoryConfig = CFactoryConfig::Instance();
}

CDlgFactorySet::~CDlgFactorySet()
{
	m_EditBkBrush.DeleteObject();
	m_BkBrush.DeleteObject();
}

void CDlgFactorySet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LOGFILEPATH, m_sLogFilePath);
	DDX_Text(pDX, IDC_EDIT_MARKFILE, m_sMarkFilePath);
	DDX_Text(pDX, IDC_EDIT_LOTID_XML_PATH, m_sLotIDXMLPath);
	DDX_Text(pDX, IDC_EDIT_SCRIPID_XML_PATH, m_sScripIDXMLPath);
	DDX_Text(pDX, IDC_EDIT_PORT, m_nPort);
	DDX_Radio(pDX, IDC_RADIO_WORK_AUTO, m_nWorkMode);
	DDX_Check(pDX, IDC_CHECK_OPENRMS, m_bOpenRMS);
	DDX_Check(pDX, IDC_CHECK_OPENROLLTIP, m_bOpenRollTip);
	DDX_Text(pDX, IDC_EDIT_TIPTEXT, m_sTipText);
	DDX_Control(pDX, IDC_COMBO_ENDMARK, m_comboEndMark);
	DDX_Control(pDX, IDC_COMBO_STARTMARK, m_comboStartMark);
	DDX_Text(pDX, IDC_EDIT_MOB_FILE, m_sMobFile);
	DDX_Text(pDX, IDC_EDIT_DEVICE_NO, m_sDeviceNo);
	DDX_Text(pDX, IDC_EDIT_MESIP, m_sMesIP);
	DDX_Text(pDX, IDC_EDIT_RMSIP, m_sRmsIP);
	DDX_Text(pDX, IDC_EDIT_MARKFILEHEAD, m_sMarkFileHead);
	DDX_Check(pDX, IDC_CHECK_MCCHECK, m_bMcCheck);
	DDX_Text(pDX, IDC_EDIT_MCCHECK, m_sMcCheckTip);
	DDX_Text(pDX, IDC_EDIT_MCCHECKTIMES, m_nMCCheckTimes);
	DDX_Check(pDX, IDC_CHECK_ORDER_STATUS, m_bCheckStatus);
	DDX_Text(pDX, IDC_EDIT_MARKSTAMP, m_s2DPath);
	DDX_Check(pDX, IDC_CHECK_MAX_MARKTIMES, m_bMaxMarkTimesLimit);
	DDX_Text(pDX, IDC_EDIT_MAX_MARKTIMES, m_nMaxMarkTimes);
	DDX_Text(pDX, IDC_EDIT_MESURL, m_sMesUrl);
	DDX_Text(pDX, IDC_EDIT_STAMPFILE, m_sStampFile);
	DDX_Check(pDX, IDC_CHECK_VISUALSYSTEM, m_bVisualSystem);
	DDX_Text(pDX, IDC_EDIT_MESURL2, m_sMesUrl2);
	DDX_Text(pDX, IDC_EDIT_MESURL3, m_sMesUrl3);
	DDX_Control(pDX, IDC_COMBO_2D_REPEAT_MODE, m_combo2DRepeatMode);
	DDX_Text(pDX, IDC_EDIT_PORT2, m_iPort2);
	DDX_Text(pDX, IDC_EDIT_GFH_PATH, m_sGFHPath);
	DDX_Text(pDX, IDC_EDIT_LASER_PARAM_SAVE_PATH, m_sLaserParamSave);
	DDX_Check(pDX, IDC_VISION_CHECK, m_bVisionCheck);
	DDX_Text(pDX, IDC_EDIT_URL_IP_PORT, m_sMesUrl4);
	DDX_Text(pDX, IDC_EDIT_URL_PRODUCT_TYPE, m_sMesUrlProducttype);
}


BEGIN_MESSAGE_MAP(CDlgFactorySet, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_MARKFILE, &CDlgFactorySet::OnBnClickedButtonMarkfile)
	ON_BN_CLICKED(IDC_BUTTON_LOGFILE, &CDlgFactorySet::OnBnClickedButtonLogfile)
	ON_BN_CLICKED(IDC_CHECK_OPENRMS, &CDlgFactorySet::OnBnClickedCheckOpenrms)
	ON_BN_CLICKED(IDC_CHECK_OPENROLLTIP, &CDlgFactorySet::OnBnClickedCheckOpenrolltip)
	ON_BN_CLICKED(IDC_RADIO_WORK_AUTO, &CDlgFactorySet::OnBnClickedRadioWorkAuto)
	ON_BN_CLICKED(IDC_RADIO_WORK_MANUL, &CDlgFactorySet::OnBnClickedRadioWorkManul)
	ON_BN_CLICKED(IDC_RADIO_WORK_SCAN, &CDlgFactorySet::OnBnClickedRadioWorkScan)
	ON_BN_CLICKED(IDC_BUTTON_LINKTEST, &CDlgFactorySet::OnBnClickedButtonLinktest)
	ON_BN_CLICKED(IDC_BUTTON_ADJUST_SET, &CDlgFactorySet::OnBnClickedButtonAdjustSet)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_SET, &CDlgFactorySet::OnBnClickedButtonSaveSet)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL_SET, &CDlgFactorySet::OnBnClickedButtonCancelSet)
	ON_BN_CLICKED(IDC_BUTTON_MOB_FILE, &CDlgFactorySet::OnBnClickedButtonMobFile)
	ON_BN_CLICKED(IDC_BUTTON_STARTSERVER, &CDlgFactorySet::OnBnClickedButtonStartserver)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK_MCCHECK, &CDlgFactorySet::OnBnClickedCheckMccheck)
	ON_BN_CLICKED(IDC_BUTTON_MARK_POINT, &CDlgFactorySet::OnBnClickedButtonMarkPoint)
	ON_BN_CLICKED(IDC_BUTTON_MARKSTAMP, &CDlgFactorySet::OnBnClickedButtonMarkstamp)
	ON_BN_CLICKED(IDC_RADIO_WORK_PONUM, &CDlgFactorySet::OnBnClickedRadioWorkPonum)
	ON_BN_CLICKED(IDC_CHECK_MAX_MARKTIMES, &CDlgFactorySet::OnBnClickedCheckMaxMarktimes)
	ON_BN_CLICKED(IDC_BUTTON_USER_MANAGER, &CDlgFactorySet::OnBnClickedButtonUserManager)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_DEVICE_CONFIG, &CDlgFactorySet::OnBnClickedButtonDeviceConfig)
	ON_BN_CLICKED(IDC_BUTTON_LASER_SET, &CDlgFactorySet::OnBnClickedButtonLaserSet)
	ON_BN_CLICKED(IDC_BUTTON_CALIBRATION, &CDlgFactorySet::OnBnClickedButtonCalibration)
	ON_BN_CLICKED(IDC_BUTTON_SCANNER_SET, &CDlgFactorySet::OnBnClickedButtonScannerSet)
	ON_BN_CLICKED(IDC_BUTTON_LOTID_XML_PATH, &CDlgFactorySet::OnBnClickedButtonLotIDXmlPath)
	ON_BN_CLICKED(IDC_BUTTON_STAMPFILE, &CDlgFactorySet::OnBnClickedButtonStampfile)
	ON_BN_CLICKED(IDC_BUTTON_NINE_CALC, &CDlgFactorySet::OnBnClickedButtonNineCalc)
	ON_BN_CLICKED(IDC_BUTTON_SCRIPID_XML_PATH, &CDlgFactorySet::OnBnClickedButtonScripidXmlPath)
	ON_BN_CLICKED(IDC_BUTTON_LASER_USE, &CDlgFactorySet::OnBnClickedButtonLaserUse)
	ON_BN_CLICKED(IDC_BUTTON_STARTSERVER2, &CDlgFactorySet::OnBnClickedButtonStartserver2)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_GFH, &CDlgFactorySet::OnBnClickedButtonGfh)
	ON_BN_CLICKED(IDC_BUTTON_LASER_PARAM_SAVE_PATH, &CDlgFactorySet::OnBnClickedButtonLaserParamSavePath)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_DATA, &CDlgFactorySet::OnBnClickedButtonDeleteData)
	ON_BN_CLICKED(IDC_BUTTON_STARTSERVER3, &CDlgFactorySet::OnBnClickedButtonStartserver3)
	ON_BN_CLICKED(IDC_BUTTON_FINS_CONNECT, &CDlgFactorySet::OnBnClickedButtonFinsConnect)
END_MESSAGE_MAP()


// CDlgFactorySet 消息处理程序
void CDlgFactorySet::OnBnClickedButtonMarkfile()
{
	// TODO: 在此添加控件通知处理程序代码
#if 0
	BROWSEINFO   bi;                           //创建BROWSEINFO结构体;
	TCHAR   Buffer[512] = { 0 };
	TCHAR   FullPath[512] = { 0 };
	bi.hwndOwner = m_hWnd;                 //窗口句柄;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = Buffer;            //返回选择的目录名的缓冲区;
	bi.lpszTitle = _T("选择标记文件目录");           //弹出的窗口的文字提示;
	bi.ulFlags = BIF_RETURNONLYFSDIRS;  //只返回目录。其他标志看MSDN;
	bi.lpfn = NULL;               //回调函数，有时很有用;
	bi.lParam = 0;
	bi.iImage = 0;
	ITEMIDLIST*   pidl = ::SHBrowseForFolder(&bi);   //显示弹出窗口，ITEMIDLIST很重要;
	::SHGetPathFromIDList(pidl, FullPath); //在ITEMIDLIST中得到目录名的整个路径;
	if (_tcslen(FullPath)>0)
	{
		m_sMarkFilePath.Format(_T("%s"), FullPath);
		SetDlgItemText(IDC_EDIT_MARKFILE, m_sMarkFilePath);
	}
#else
	if (OpenFolderSelectDlg(_T("选择标记文件目录"), m_sMarkFilePath, this)) {
		SetDlgItemText(IDC_EDIT_MARKFILE, m_sMarkFilePath);
	}
#endif
}


void CDlgFactorySet::OnBnClickedButtonLogfile()
{
	// TODO: 在此添加控件通知处理程序代码
#if 0
	BROWSEINFO   bi;                           //创建BROWSEINFO结构体;
	TCHAR   Buffer[512] = { 0 };
	TCHAR   FullPath[512] = { 0 };
	bi.hwndOwner = m_hWnd;                 //窗口句柄;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = Buffer;            //返回选择的目录名的缓冲区;
	bi.lpszTitle = _T("选择日志文件目录");           //弹出的窗口的文字提示;
	bi.ulFlags = BIF_RETURNONLYFSDIRS;  //只返回目录。其他标志看MSDN;
	bi.lpfn = NULL;               //回调函数，有时很有用;
	bi.lParam = 0;
	bi.iImage = 0;
	ITEMIDLIST*   pidl = ::SHBrowseForFolder(&bi);   //显示弹出窗口，ITEMIDLIST很重要;
	::SHGetPathFromIDList(pidl, FullPath); //在ITEMIDLIST中得到目录名的整个路径;
	if (_tcslen(FullPath)>0)
	{
		m_sLogFilePath.Format(_T("%s"), FullPath);
		SetDlgItemText(IDC_EDIT_LOGFILEPATH, m_sLogFilePath);
	}
#else
	if (OpenFolderSelectDlg(_T("选择日志文件目录"), m_sLogFilePath, this)) {
		SetDlgItemText(IDC_EDIT_LOGFILEPATH, m_sLogFilePath);
	}
#endif
}


BOOL CDlgFactorySet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetGroupCheckRadioTheme();
	InitBtns();
	Init2DRepeatMode();
	UpdateUIFromConfig();
	CreateDefaultFilePath();
	
	if (m_bOpenRMS)
	{
		//GetDlgItem(IDC_STATIC_RMSIP)->ShowWindow(TRUE);
		//GetDlgItem(IDC_RMSIP)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_STATIC_RMSIP)->ShowWindow(FALSE);
		GetDlgItem(IDC_RMSIP)->ShowWindow(FALSE);
	}
	//if (m_bOpenRollTip)
	//	GetDlgItem(IDC_EDIT_TIPTEXT)->ShowWindow(TRUE);
	//else
	//	GetDlgItem(IDC_EDIT_TIPTEXT)->ShowWindow(FALSE);

	EnableContrlByWork(m_nWorkMode);

	EnableControlByRights();
	//GetDlgItem(IDC_BUTTON_ADJUST_SET)->EnableWindow(TRUE);
	//GetDlgItem(IDC_BUTTON_SAVE_SET)->EnableWindow(FALSE);
	//GetDlgItem(IDC_BUTTON_CANCEL_SET)->EnableWindow(FALSE);
	
	//m_bVisualSystem = ((CButton*)GetDlgItem(IDC_CHECK_VISUALSYSTEM))->GetCheck();
	((CButton*)GetDlgItem(IDC_CHECK_VISUALSYSTEM))->SetCheck(m_bVisualSystem);    //CCD启用默认打开，且不可操作
	m_bCheckStripIDMatchLotID = ((CButton*)GetDlgItem(IDC_CHECK_STRIPID_MATCH_LOTID))->GetCheck();



	CString m_sOld2DPath = m_pFactoryConfig->m_s2DPath;

	((CButton*)GetDlgItem(IDC_FINS_CHECK_MES_CHECK))->SetCheck(m_pFactoryConfig->m_EnableFinishedLot);    //CCD启用默认打开，且不可操作

	SetDlgItemText(IDC_EDIT_URL_FINISHED_LOT, m_pFactoryConfig->m_FinishedLotUrl);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CDlgFactorySet::OnBnClickedCheckOpenrms()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCheck = ((CButton*)GetDlgItem(IDC_CHECK_OPENRMS))->GetCheck();
	if (1 == nCheck)
	{
		//GetDlgItem(IDC_STATIC_RMSIP)->ShowWindow(TRUE);
		//GetDlgItem(IDC_EDIT_RMSIP)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_STATIC_RMSIP)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_RMSIP)->ShowWindow(FALSE);
	}
}


void CDlgFactorySet::OnBnClickedCheckOpenrolltip()
{
	// TODO: 在此添加控件通知处理程序代码
	//int nCheck = ((CButton*)GetDlgItem(IDC_CHECK_OPENROLLTIP))->GetCheck();
	//if (1 == nCheck)
	//{
	//	GetDlgItem(IDC_EDIT_TIPTEXT)->ShowWindow(TRUE);
	//}
	//else
	//{
	//	GetDlgItem(IDC_EDIT_TIPTEXT)->ShowWindow(FALSE);
	//}
}


void CDlgFactorySet::OnBnClickedRadioWorkAuto()
{
	// TODO: 在此添加控件通知处理程序代码
	EnableContrlByWork(emWorkMode_Auto);
}


void CDlgFactorySet::OnBnClickedRadioWorkManul()
{
	// TODO: 在此添加控件通知处理程序代码
	EnableContrlByWork(emWorkMode_Manual);
}


void CDlgFactorySet::OnBnClickedRadioWorkScan()
{
	// TODO: 在此添加控件通知处理程序代码
	EnableContrlByWork(emWorkMode_Scan);
}

void CDlgFactorySet::EnableContrlByWork(int nWorkMode)
{
	if (emWorkMode_Auto == nWorkMode)
	{   
		GetDlgItem(IDC_STATIC_PORT)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_PORT2)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_PORT)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_PORT2)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_PORT3)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_FINS_PORT)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_FINS_IP)->ShowWindow(TRUE);
		
		GetDlgItem(IDC_STATIC_MESIP)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_MESIP)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_LINKTEST)->ShowWindow(FALSE);
		GetDlgItem(IDC_CHECK_ORDER_STATUS)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STARTSERVER)->ShowWindow(TRUE);
		//GetDlgItem(IDC_STATIC_MARKFILEHEAD)->ShowWindow(TRUE);
		//GetDlgItem(IDC_EDIT_MARKFILEHEAD)->ShowWindow(TRUE);
		//GetDlgItem(IDC_CHECK_MCCHECK)->ShowWindow(TRUE);    //不需要MC首检
		GetDlgItem(IDC_STATIC_PONUM)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_MARKSTAMP)->ShowWindow(TRUE);
		GetDlgItem(IDC_BUTTON_MARKSTAMP)->ShowWindow(TRUE);
		GetDlgItem(IDC_CHECK_MAX_MARKTIMES)->ShowWindow(TRUE);
		//GetDlgItem(IDC_CHECK_ADD_PREFIX)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_MESURL)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_URL)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_MESURL2)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_URL2)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_MESURL3)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_URL_IP_PORT)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_URL_PRODUCT_TYPE)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_URL_FINISHED_LOT)->ShowWindow(TRUE);
		
		GetDlgItem(IDC_STATIC_URL3)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_URL4)->ShowWindow(TRUE);
		OnBnClickedCheckMccheck();
		OnBnClickedCheckMaxMarktimes();
	}
	else if (emWorkMode_Scan == nWorkMode)
	{  // 开批模式
		GetDlgItem(IDC_STATIC_PORT)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_PORT2)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_PORT)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_PORT2)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_PORT3)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_FINS_PORT)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_FINS_IP)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_MESIP)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_MESIP)->ShowWindow(TRUE);
		GetDlgItem(IDC_BUTTON_LINKTEST)->ShowWindow(TRUE);
		GetDlgItem(IDC_CHECK_ORDER_STATUS)->ShowWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STARTSERVER)->ShowWindow(FALSE);
		//GetDlgItem(IDC_STATIC_MARKFILEHEAD)->ShowWindow(TRUE);
		//GetDlgItem(IDC_EDIT_MARKFILEHEAD)->ShowWindow(TRUE);
		GetDlgItem(IDC_CHECK_MCCHECK)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_PONUM)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_MARKSTAMP)->ShowWindow(TRUE);
		GetDlgItem(IDC_BUTTON_MARKSTAMP)->ShowWindow(TRUE);
		GetDlgItem(IDC_CHECK_MAX_MARKTIMES)->ShowWindow(TRUE);
		//GetDlgItem(IDC_CHECK_ADD_PREFIX)->ShowWindow(FALSE);
		OnBnClickedCheckMccheck();
		OnBnClickedCheckMaxMarktimes();
	}
	else if (emWorkMode_Manual == nWorkMode) 
	{ // 调试模式
		GetDlgItem(IDC_STATIC_PORT)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_PORT2)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_PORT)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_PORT2)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_PORT3)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_FINS_PORT)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_FINS_IP)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_MESIP)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_MESIP)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_LINKTEST)->ShowWindow(FALSE);
		GetDlgItem(IDC_CHECK_ORDER_STATUS)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STARTSERVER)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_MARKFILEHEAD)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_MARKFILEHEAD)->ShowWindow(FALSE);
		GetDlgItem(IDC_CHECK_MCCHECK)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_MCCHECK)->ShowWindow(FALSE);
		//GetDlgItem(IDC_STATIC_MCCHECK_TIMES)->ShowWindow(FALSE);
		//GetDlgItem(IDC_EDIT_MCCHECKTIMES)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_PONUM)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_MARKSTAMP)->ShowWindow(TRUE);
		GetDlgItem(IDC_BUTTON_MARKSTAMP)->ShowWindow(TRUE);
		GetDlgItem(IDC_CHECK_MAX_MARKTIMES)->ShowWindow(FALSE);
		//GetDlgItem(IDC_EDIT_MAX_MARKTIMES)->ShowWindow(FALSE);
		//GetDlgItem(IDC_CHECK_ADD_PREFIX)->ShowWindow(FALSE);

		GetDlgItem(IDC_EDIT_MESURL)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_URL)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_MESURL2)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_URL2)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_MESURL3)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_URL_IP_PORT)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_URL_PRODUCT_TYPE)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_URL_FINISHED_LOT)->ShowWindow(FALSE);

		GetDlgItem(IDC_STATIC_URL3)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_URL4)->ShowWindow(FALSE);

	}
	else if (emWorkMode_Ponum == nWorkMode)
	{
		GetDlgItem(IDC_STATIC_PORT)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_PORT)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_PORT2)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_PORT2)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_PORT3)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_FINS_PORT)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_FINS_IP)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_MESIP)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_MESIP)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_LINKTEST)->ShowWindow(FALSE);
		GetDlgItem(IDC_CHECK_ORDER_STATUS)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STARTSERVER)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_MARKFILEHEAD)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_MARKFILEHEAD)->ShowWindow(TRUE);
		GetDlgItem(IDC_CHECK_MCCHECK)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_MCCHECK)->ShowWindow(TRUE);
		//GetDlgItem(IDC_STATIC_MCCHECK_TIMES)->ShowWindow(TRUE);
		//GetDlgItem(IDC_EDIT_MCCHECKTIMES)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_PONUM)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_MARKSTAMP)->ShowWindow(TRUE);
		GetDlgItem(IDC_BUTTON_MARKSTAMP)->ShowWindow(TRUE);
		GetDlgItem(IDC_CHECK_MAX_MARKTIMES)->ShowWindow(TRUE);
		//GetDlgItem(IDC_EDIT_MAX_MARKTIMES)->ShowWindow(TRUE);
		//GetDlgItem(IDC_CHECK_ADD_PREFIX)->ShowWindow(FALSE);

		OnBnClickedCheckMccheck();
		OnBnClickedCheckMaxMarktimes();
	}


	// 扫码模式下，启用MAPPING功能，启用2D读码，二维码对比，条数限制，采用管理员权限
	if (nWorkMode == emWorkMode_Auto)
	{
		bool bEnable = (USERMNG->GetCurUser() == USER_A_ADMIN);

		GetDlgItem(IDC_CHECK_STRIP_MAP)->EnableWindow(bEnable);
		GetDlgItem(IDC_CHECK_2D_BARCODE)->EnableWindow(bEnable);
		GetDlgItem(IDC_COMBO_2D_REPEAT_MODE)->EnableWindow(bEnable);
		GetDlgItem(IDC_CHECK_MAX_MARKTIMES)->EnableWindow(bEnable);
		GetDlgItem(IDC_EDIT_MAX_MARKTIMES)->EnableWindow(bEnable);
		GetDlgItem(IDC_CHECK_VISUALSYSTEM)->EnableWindow(bEnable);

		// 自动启用所有勾选
		((CButton*)GetDlgItem(IDC_CHECK_STRIP_MAP))->SetCheck(true);
		((CButton*)GetDlgItem(IDC_CHECK_2D_BARCODE))->SetCheck(true);
		((CButton*)GetDlgItem(IDC_COMBO_2D_REPEAT_MODE))->SetCheck(true);
		((CButton*)GetDlgItem(IDC_CHECK_MAX_MARKTIMES))->SetCheck(true);
		((CButton*)GetDlgItem(IDC_CHECK_VISUALSYSTEM))->SetCheck(true);
	}
	else
	{
		GetDlgItem(IDC_CHECK_STRIP_MAP)->EnableWindow(USER_VERIFY(USER_O_ENABLE_MAPPING));
		GetDlgItem(IDC_CHECK_2D_BARCODE)->EnableWindow(USER_VERIFY(USER_O_ENABLE_2D));
		GetDlgItem(IDC_COMBO_2D_REPEAT_MODE)->EnableWindow(USER_VERIFY(USER_O_BASIC_SET));
		GetDlgItem(IDC_CHECK_MAX_MARKTIMES)->EnableWindow(USER_VERIFY(USER_O_BASIC_SET));
		GetDlgItem(IDC_EDIT_MAX_MARKTIMES)->EnableWindow(USER_VERIFY(USER_O_BASIC_SET));
		GetDlgItem(IDC_CHECK_VISUALSYSTEM)->EnableWindow(USER_VERIFY(USER_O_ENABLE_VISION));
	}
}

void CDlgFactorySet::EnableControlByRights()  //根据权限决定是否可操作
{
	GetDlgItem(IDC_RADIO_WORK_AUTO)->EnableWindow(USER_VERIFY(USER_O_WORK_MODE_SET));
	GetDlgItem(IDC_RADIO_WORK_MANUL)->EnableWindow(USER_VERIFY(USER_O_WORK_MODE_SET));

	GetDlgItem(IDC_EDIT_MESURL)->EnableWindow(USER_VERIFY(USER_O_MES_PATH_SET));
	GetDlgItem(IDC_EDIT_MESURL2)->EnableWindow(USER_VERIFY(USER_O_MES_PATH_SET));
	GetDlgItem(IDC_EDIT_MESURL3)->EnableWindow(USER_VERIFY(USER_O_MES_PATH_SET));
	GetDlgItem(IDC_EDIT_URL_IP_PORT)->EnableWindow(USER_VERIFY(USER_O_MES_PATH_SET));
	GetDlgItem(IDC_EDIT_URL_PRODUCT_TYPE)->EnableWindow(USER_VERIFY(USER_O_MES_PATH_SET));
	GetDlgItem(IDC_EDIT_URL_FINISHED_LOT)->EnableWindow(USER_VERIFY(USER_O_MES_PATH_SET));



	GetDlgItem(IDC_EDIT_PORT)->EnableWindow(USER_VERIFY(USER_O_PLC_COMM_SET));
	GetDlgItem(IDC_EDIT_PORT2)->EnableWindow(USER_VERIFY(USER_O_PLC_COMM_SET));
	GetDlgItem(IDC_EDIT_PORT3)->EnableWindow(USER_VERIFY(USER_O_PLC_COMM_SET));
	GetDlgItem(IDC_EDIT_FINS_PORT)->EnableWindow(USER_VERIFY(USER_O_PLC_COMM_SET));
	GetDlgItem(IDC_EDIT_FINS_IP)->EnableWindow(USER_VERIFY(USER_O_PLC_COMM_SET));
	GetDlgItem(IDC_EDIT_URL_FINISHED_LOT)->EnableWindow(USER_VERIFY(USER_O_PLC_COMM_SET));

	
	GetDlgItem(IDC_BUTTON_STARTSERVER)->EnableWindow(USER_VERIFY(USER_O_PLC_COMM_SET));
	GetDlgItem(IDC_BUTTON_STARTSERVER2)->EnableWindow(USER_VERIFY(USER_O_PLC_COMM_SET));
	GetDlgItem(IDC_BUTTON_STARTSERVER3)->EnableWindow(USER_VERIFY(USER_O_PLC_COMM_SET));
	GetDlgItem(IDC_BUTTON_FINS_CONNECT)->EnableWindow(USER_VERIFY(USER_O_PLC_COMM_SET));
	
	GetDlgItem(IDC_CHECK_VISION)->EnableWindow(USER_VERIFY(USER_O_PLC_COMM_SET));
	GetDlgItem(IDC_CHECK_PLC)->EnableWindow(USER_VERIFY(USER_O_PLC_COMM_SET));
	GetDlgItem(IDC_FINS_CHECK)->EnableWindow(USER_VERIFY(USER_O_PLC_COMM_SET));
	
	GetDlgItem(IDC_VISION_CHECK)->EnableWindow(USER_VERIFY(USER_O_PLC_COMM_SET));

	GetDlgItem(IDC_CHECK_STRIP_MAP)->EnableWindow(USER_VERIFY(USER_O_ENABLE_MAPPING));
	GetDlgItem(IDC_CHECK_2D_BARCODE)->EnableWindow(USER_VERIFY(USER_O_ENABLE_2D));
	GetDlgItem(IDC_CHECK_STRIPID_MATCH_LOTID)->EnableWindow(USER_VERIFY(USER_O_BASIC_SET));
	GetDlgItem(IDC_CHECK_PRODUCT_TYPE)->EnableWindow(USER_VERIFY(USER_O_BASIC_SET));
	GetDlgItem(IDC_FINS_CHECK_MES_CHECK)->EnableWindow(USER_VERIFY(USER_O_BASIC_SET));
	GetDlgItem(IDC_CHECK_MES_AUTO_LOT)->EnableWindow(USER_VERIFY(USER_O_BASIC_SET));


	GetDlgItem(IDC_COMBO_2D_REPEAT_MODE)->EnableWindow(USER_VERIFY(USER_O_BASIC_SET));
	GetDlgItem(IDC_BUTTON_DELETE_DATA)->EnableWindow(USER_VERIFY(USER_O_BASIC_SET));
	GetDlgItem(IDC_CHECK_MAX_MARKTIMES)->EnableWindow(USER_VERIFY(USER_O_BASIC_SET));
	GetDlgItem(IDC_EDIT_MAX_MARKTIMES)->EnableWindow(USER_VERIFY(USER_O_BASIC_SET));
	GetDlgItem(IDC_CHECK_VISUALSYSTEM)->EnableWindow(USER_VERIFY(USER_O_ENABLE_VISION));

	GetDlgItem(IDC_BUTTON_MARKFILE)->EnableWindow(USER_VERIFY(USER_O_FILE_PATH_SET));
	GetDlgItem(IDC_BUTTON_STAMPFILE)->EnableWindow(USER_VERIFY(USER_O_FILE_PATH_SET));
	GetDlgItem(IDC_BUTTON_LOTID_XML_PATH)->EnableWindow(USER_VERIFY(USER_O_FILE_PATH_SET));
	GetDlgItem(IDC_BUTTON_SCRIPID_XML_PATH)->EnableWindow(USER_VERIFY(USER_O_FILE_PATH_SET));
	GetDlgItem(IDC_BUTTON_LOGFILE)->EnableWindow(USER_VERIFY(USER_O_FILE_PATH_SET));
	GetDlgItem(IDC_BUTTON_MARKSTAMP)->EnableWindow(USER_VERIFY(USER_O_FILE_PATH_SET));
	GetDlgItem(IDC_BUTTON_MOB_FILE)->EnableWindow(USER_VERIFY(USER_O_FILE_PATH_SET));
	GetDlgItem(IDC_BUTTON_GFH)->EnableWindow(USER_VERIFY(USER_O_FILE_PATH_SET));
	GetDlgItem(IDC_BUTTON_LASER_PARAM_SAVE_PATH)->EnableWindow(USER_VERIFY(USER_O_FILE_PATH_SET));

	GetDlgItem(IDC_BUTTON_MARK_POINT)->EnableWindow(USER_VERIFY(USER_O_POWER_MEASURE));

	GetDlgItem(IDC_BUTTON_LASER_SET)->EnableWindow(USER_VERIFY(USER_O_LASER_SET));

	GetDlgItem(IDC_BUTTON_SCANNER_SET)->EnableWindow(USER_VERIFY(USER_O_IO_SET));

	GetDlgItem(IDC_BUTTON_DEVICE_CONFIG)->EnableWindow(USER_VERIFY(USER_O_HARDWARE_SET));

	GetDlgItem(IDC_BUTTON_CALIBRATION)->EnableWindow(USER_VERIFY(USER_O_MUTIL_CALIB_SET));

	GetDlgItem(IDC_BUTTON_NINE_CALC)->EnableWindow(USER_VERIFY(USER_O_CCD_NINE_SET));
	
	GetDlgItem(IDC_BUTTON_USER_MANAGER)->EnableWindow(USER_VERIFY(USER_O_USER_MNG_SET));
	
	GetDlgItem(IDC_BUTTON_LASER_USE)->EnableWindow(USER_VERIFY(USER_O_DEVICE_USE_SET));
	
	// 扫码模式下，启用MAPPING功能，启用2D读码，二维码对比，条数限制，采用管理员权限
	bool bAutoMode = ((CButton*)GetDlgItem(IDC_RADIO_WORK_AUTO))->GetCheck();
	if (bAutoMode)
	{
		bool bEnable = (USERMNG->GetCurUser() == USER_A_ADMIN);

		GetDlgItem(IDC_CHECK_STRIP_MAP)->EnableWindow(bEnable);
		GetDlgItem(IDC_CHECK_2D_BARCODE)->EnableWindow(bEnable);
		GetDlgItem(IDC_COMBO_2D_REPEAT_MODE)->EnableWindow(bEnable);
		GetDlgItem(IDC_CHECK_MAX_MARKTIMES)->EnableWindow(bEnable);
		GetDlgItem(IDC_EDIT_MAX_MARKTIMES)->EnableWindow(bEnable);
		GetDlgItem(IDC_CHECK_VISUALSYSTEM)->EnableWindow(bEnable);
	}
}

BOOL CDlgFactorySet::SaveParam()
{
	//if (!GetDlgItem(IDC_BUTTON_SAVE_SET)->IsWindowEnabled())
	//	return TRUE;

	//启用mapping则默认启用2D读码器
	int bMappingStatus = ((CButton*)GetDlgItem(IDC_CHECK_STRIP_MAP))->GetCheck();
	if (1 == bMappingStatus) {
		((CButton*)GetDlgItem(IDC_CHECK_2D_BARCODE))->SetCheck(1);
	}

	SaveConfigFromUI();

	// 修改了2D路径
	if (m_pFactoryConfig->m_s2DPath != m_sOld2DPath)
	{
		if (!m_pMainDlg->m_pWorkControl->m_2DRepeatCheck.InitDB())
		{
			AfxMessageBox(_T("二维码数据保存数据库初始化失败！"));
			return FALSE;
		}
	}

	//GetDlgItem(IDC_BUTTON_ADJUST_SET)->EnableWindow(TRUE);
	//GetDlgItem(IDC_BUTTON_SAVE_SET)->EnableWindow(FALSE);
	//GetDlgItem(IDC_BUTTON_CANCEL_SET)->EnableWindow(FALSE);

	EnableControlByRights();

	m_pMainDlg->UpdateSet();

	m_bVisualSystem = ((CButton*)GetDlgItem(IDC_CHECK_VISUALSYSTEM))->GetCheck();

	m_bCheckStripIDMatchLotID = ((CButton*)GetDlgItem(IDC_CHECK_STRIPID_MATCH_LOTID))->GetCheck();

	return TRUE;
}

void CDlgFactorySet::OnBnClickedButtonLinktest()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON_LINKTEST)->EnableWindow(FALSE);
	int nPort = 0;
	CString ip;
	nPort = GetDlgItemInt(IDC_EDIT_PORT);
	GetDlgItemText(IDC_EDIT_MESIP, ip);
	if (m_pMainDlg->ContectTest(CStringA(ip).GetString(), nPort))
	{
		AfxMessageBox(_T("连接成功！"));
	}
	else
	{
		AfxMessageBox(_T("连接失败！"));
	}
	GetDlgItem(IDC_BUTTON_LINKTEST)->EnableWindow(TRUE);
}


void CDlgFactorySet::OnBnClickedButtonAdjustSet()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgFactoryPwd pwddlg;
	if (IDOK != pwddlg.DoModal())
		return;
	
	//GetDlgItem(IDC_BUTTON_ADJUST_SET)->EnableWindow(FALSE);
	//GetDlgItem(IDC_BUTTON_SAVE_SET)->EnableWindow(TRUE);
	//GetDlgItem(IDC_BUTTON_CANCEL_SET)->EnableWindow(TRUE);

	EnableControlByRights();

	m_pMainDlg->UpdateAppCaption();
	CString msg;
	msg.Format(_T("系统设置界面，切换到用户[%s]"), USERMNG->GetCurUserName());
	m_pMainDlg->m_pWorkControl->PrintMessage(emMSG_TYPE_INFO, msg);
}


void CDlgFactorySet::OnBnClickedButtonSaveSet()
{
	SaveParam();
}


BOOL CDlgFactorySet::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDlgFactorySet::OnBnClickedButtonCancelSet()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateUIFromConfig();

	if (m_bOpenRMS)
	{
		//GetDlgItem(IDC_STATIC_RMSIP)->ShowWindow(TRUE);
		//GetDlgItem(IDC_RMSIP)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_STATIC_RMSIP)->ShowWindow(FALSE);
		GetDlgItem(IDC_RMSIP)->ShowWindow(FALSE);
	}
	//if (m_bOpenRollTip)
	//{
	//	GetDlgItem(IDC_EDIT_TIPTEXT)->ShowWindow(TRUE);
	//}
	//else
	//{
	//	GetDlgItem(IDC_EDIT_TIPTEXT)->ShowWindow(FALSE);
	//}

	EnableContrlByWork(m_nWorkMode);
	EnableControlByRights();
	//GetDlgItem(IDC_BUTTON_ADJUST_SET)->EnableWindow(TRUE);
	//GetDlgItem(IDC_BUTTON_SAVE_SET)->EnableWindow(FALSE);
	//GetDlgItem(IDC_BUTTON_CANCEL_SET)->EnableWindow(FALSE);
}


void CDlgFactorySet::OnBnClickedButtonMobFile()
{
	// TODO: 在此添加控件通知处理程序代码
#if 0
	BROWSEINFO   bi;                           //创建BROWSEINFO结构体;
	TCHAR   Buffer[512] = { 0 };
	TCHAR   FullPath[512] = { 0 };
	bi.hwndOwner = m_hWnd;                 //窗口句柄;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = Buffer;            //返回选择的目录名的缓冲区;
	bi.lpszTitle = _T("选择阵列位置文件目录");           //弹出的窗口的文字提示;
	bi.ulFlags = BIF_RETURNONLYFSDIRS;  //只返回目录。其他标志看MSDN;
	bi.lpfn = NULL;               //回调函数，有时很有用;
	bi.lParam = 0;
	bi.iImage = 0;
	ITEMIDLIST*   pidl = ::SHBrowseForFolder(&bi);   //显示弹出窗口，ITEMIDLIST很重要;
	::SHGetPathFromIDList(pidl, FullPath); //在ITEMIDLIST中得到目录名的整个路径;
	if (_tcslen(FullPath)>0)
	{
		m_sMobFile.Format(_T("%s"), FullPath);
		SetDlgItemText(IDC_EDIT_MOB_FILE, m_sMobFile);
	}
#else
if (OpenFolderSelectDlg(_T("选择阵列位置文件目录"), m_sMobFile, this)) {
	SetDlgItemText(IDC_EDIT_MOB_FILE, m_sMobFile);
}
#endif
}


void CDlgFactorySet::OnBnClickedButtonStartserver()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON_STARTSERVER)->EnableWindow(FALSE);
	int nPort = 0;
	CString ip;
	nPort = GetDlgItemInt(IDC_EDIT_PORT);
	if (m_pMainDlg->StartServer(nPort))
	{
		AfxMessageBox(_T("视觉服务器启动成功！"));
	}
	else
	{
		AfxMessageBox(_T("视觉服务器启动失败！"));
	}
	GetDlgItem(IDC_BUTTON_STARTSERVER)->EnableWindow(TRUE);
}


HBRUSH CDlgFactorySet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (nCtlColor == CTLCOLOR_DLG)
		return (HBRUSH)m_BkBrush.GetSafeHandle();

	// TODO:  在此更改 DC 的任何特性
	UINT nID = pWnd->GetDlgCtrlID();
	if (nID == IDC_EDIT_DEVICE_NO
		|| nID == IDC_EDIT_PORT
		|| nID == IDC_EDIT_PORT2
		|| nID == IDC_EDIT_PORT3
		|| nID == IDC_EDIT_FINS_PORT
		|| nID == IDC_EDIT_FINS_IP
		|| nID == IDC_EDIT_TIPTEXT
		|| nID == IDC_EDIT_MARKFILE
		|| nID == IDC_EDIT_STAMPFILE
		|| nID == IDC_EDIT_MOB_FILE
		|| nID == IDC_EDIT_LOGFILEPATH
		|| nID == IDC_EDIT_LOTID_XML_PATH
		|| nID == IDC_EDIT_SCRIPID_XML_PATH
		|| nID == IDC_EDIT_MESIP
		|| nID == IDC_EDIT_MESURL
		|| nID == IDC_EDIT_MESURL2
		|| nID == IDC_EDIT_MESURL3
		|| nID == IDC_EDIT_URL_IP_PORT
		|| nID == IDC_EDIT_URL_PRODUCT_TYPE
		|| nID == IDC_EDIT_RMSIP
		|| nID == IDC_EDIT_MARKFILEHEAD
		|| nID == IDC_EDIT_MCCHECK
		|| nID == IDC_EDIT_MCCHECKTIMES
		|| nID == IDC_EDIT_MARKSTAMP
		|| nID == IDC_EDIT_MAX_MARKTIMES
		|| nID == IDC_EDIT_GFH_PATH
		|| nID == IDC_EDIT_LASER_PARAM_SAVE_PATH
		|| nID == IDC_EDIT_URL_FINISHED_LOT)
	{
		
		pDC->SetTextColor(EDIT_TEXT_COLOR);//设置编辑框字体的颜色
		pDC->SetBkColor(EDIT_BKCOLOR);//设置字体背景颜色
		return (HBRUSH)m_EditBkBrush.GetSafeHandle();
	}
	
	if(nID == IDC_RADIO_WORK_AUTO
		|| nID == IDC_RADIO_WORK_SCAN
		|| nID == IDC_RADIO_WORK_MANUL
		|| nID == IDC_RADIO_WORK_PONUM
		|| nID == IDC_CHECK_OPENRMS
		|| nID == IDC_CHECK_OPENROLLTIP
		|| nID == IDC_CHECK_MCCHECK
		|| nID == IDC_CHECK_ORDER_STATUS
		|| nID == IDC_CHECK_MAX_MARKTIMES
		|| nID == IDC_CHECK_PEN_ORDER
		|| nID == IDC_CHECK_STRIP_MAP
		|| nID == IDC_CHECK_2D_BARCODE
		|| nID == IDC_CHECK_VISUALSYSTEM
		|| nID == IDC_VISION_CHECK
		|| nID == IDC_CHECK_ADD_PREFIX
		|| nID == IDC_CHECK_STRIPID_MATCH_LOTID
		|| nID == IDC_CHECK_PRODUCT_TYPE
		|| nID == IDC_CHECK_VISION
		|| nID == IDC_CHECK_PLC
		|| nID == IDC_FINS_CHECK
		|| nID == IDC_FINS_CHECK_MES_CHECK
		|| nID == IDC_CHECK_MES_AUTO_LOT
		)
	{
		pDC->SetTextColor(STATIC_COLOR);// 设置编辑框字体的颜色
		pDC->SetBkColor(BKCOLOR);       // 设置字体背景颜色
		return (HBRUSH)m_BkBrush.GetSafeHandle();
	}
	
	if (nID == IDC_GROUP_WORKMODE
		|| nID == IDC_GROUP_MES
		|| nID == IDC_GROUP_RMS
		|| nID == IDC_GROUP_TIP
		|| nID == IDC_GROUP_FILEPATH)
	{
		pDC->SetTextColor(STATIC_COLOR);//设置编辑框字体的颜色
		pDC->SetBkColor(BKCOLOR);//设置字体背景颜色
		return (HBRUSH)m_BkBrush.GetSafeHandle();
	}

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		// 背景为透明
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(STATIC_COLOR);
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	}

	
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔

	return hbr;
}


void CDlgFactorySet::OnBnClickedCheckMccheck()
{
	int nCheck = ((CButton*)GetDlgItem(IDC_CHECK_MCCHECK))->GetCheck();
	if (1 == nCheck)
	{
		GetDlgItem(IDC_EDIT_MCCHECK)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_MCCHECK_TIMES)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_MCCHECKTIMES)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_MCCHECK)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_MCCHECK_TIMES)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_MCCHECKTIMES)->ShowWindow(FALSE);
	}
}


void CDlgFactorySet::OnBnClickedButtonMarkPoint()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgMarkPoint dlg(this);
	dlg.DoModal();
}


void CDlgFactorySet::OnBnClickedButtonMarkstamp()
{
	// TODO: 在此添加控件通知处理程序代码
#if 0
	BROWSEINFO   bi;                           //创建BROWSEINFO结构体;
	TCHAR   Buffer[512] = { 0 };
	TCHAR   FullPath[512] = { 0 };
	bi.hwndOwner = m_hWnd;                 //窗口句柄;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = Buffer;            //返回选择的目录名的缓冲区;
	bi.lpszTitle = _T("选择二维码远程保存路径");           //弹出的窗口的文字提示;
	bi.ulFlags = BIF_RETURNONLYFSDIRS;  //只返回目录。其他标志看MSDN;
	bi.lpfn = NULL;               //回调函数，有时很有用;
	bi.lParam = 0;
	bi.iImage = 0;
	ITEMIDLIST*   pidl = ::SHBrowseForFolder(&bi);   //显示弹出窗口，ITEMIDLIST很重要;
	::SHGetPathFromIDList(pidl, FullPath); //在ITEMIDLIST中得到目录名的整个路径;
	if (_tcslen(FullPath)>0)
	{
		m_s2DPath.Format(_T("%s"), FullPath);
		SetDlgItemText(IDC_EDIT_MARKSTAMP, m_s2DPath);
	}
#else
	if (OpenFolderSelectDlg(_T("选择二维码远程保存路径"), m_s2DPath, this)) {
		SetDlgItemText(IDC_EDIT_MARKSTAMP, m_s2DPath);
	}
#endif
}

void CDlgFactorySet::OnBnClickedRadioWorkPonum()
{
	// TODO: 在此添加控件通知处理程序代码
	EnableContrlByWork(emWorkMode_Ponum);
}


void CDlgFactorySet::OnBnClickedCheckMaxMarktimes()
{
	// TODO: 在此添加控件通知处理程序代码
	//int nCheck = ((CButton*)GetDlgItem(IDC_CHECK_MAX_MARKTIMES))->GetCheck();
	//if (1 == nCheck)
	//{
	//	GetDlgItem(IDC_EDIT_MAX_MARKTIMES)->ShowWindow(TRUE);
	//}
	//else
	//{
	//	GetDlgItem(IDC_EDIT_MAX_MARKTIMES)->ShowWindow(FALSE);
	//}
}

void CDlgFactorySet::InitBtns()
{
	GetDlgItem(IDC_BUTTON_ADJUST_SET)->SetFont(&m_fntBtn);
	GetDlgItem(IDC_BUTTON_SAVE_SET)->SetFont(&m_fntBtn);
	GetDlgItem(IDC_BUTTON_CANCEL_SET)->SetFont(&m_fntBtn);
	GetDlgItem(IDC_BUTTON_DEVICE_CONFIG)->SetFont(&m_fntBtn);
	GetDlgItem(IDC_BUTTON_LASER_SET)->SetFont(&m_fntBtn);
	GetDlgItem(IDC_BUTTON_CALIBRATION)->SetFont(&m_fntBtn);
	GetDlgItem(IDC_BUTTON_MARK_POINT)->SetFont(&m_fntBtn);
	GetDlgItem(IDC_BUTTON_SCANNER_SET)->SetFont(&m_fntBtn);
	GetDlgItem(IDC_BUTTON_USER_MANAGER)->SetFont(&m_fntBtn);
	GetDlgItem(IDC_BUTTON_NINE_CALC)->SetFont(&m_fntBtn);
	GetDlgItem(IDC_BUTTON_LASER_USE)->SetFont(&m_fntBtn);
}

void CDlgFactorySet::SetGroupCheckRadioTheme()
{
	SetWindowTheme(GetDlgItem(IDC_GROUP_WORKMODE)->GetSafeHwnd(), _T(""), _T(""));
	SetWindowTheme(GetDlgItem(IDC_GROUP_MES)->GetSafeHwnd(), _T(""), _T(""));
	SetWindowTheme(GetDlgItem(IDC_GROUP_RMS)->GetSafeHwnd(), _T(""), _T(""));
	SetWindowTheme(GetDlgItem(IDC_GROUP_TIP)->GetSafeHwnd(), _T(""), _T(""));
	SetWindowTheme(GetDlgItem(IDC_GROUP_FILEPATH)->GetSafeHwnd(), _T(""), _T(""));

	SetWindowTheme(GetDlgItem(IDC_RADIO_WORK_AUTO)->GetSafeHwnd(), _T(""), _T(""));
	SetWindowTheme(GetDlgItem(IDC_RADIO_WORK_SCAN)->GetSafeHwnd(), _T(""), _T(""));
	SetWindowTheme(GetDlgItem(IDC_RADIO_WORK_MANUL)->GetSafeHwnd(), _T(""), _T(""));
	SetWindowTheme(GetDlgItem(IDC_RADIO_WORK_PONUM)->GetSafeHwnd(), _T(""), _T(""));
	SetWindowTheme(GetDlgItem(IDC_CHECK_OPENRMS)->GetSafeHwnd(), _T(""), _T(""));
	SetWindowTheme(GetDlgItem(IDC_CHECK_OPENROLLTIP)->GetSafeHwnd(), _T(""), _T(""));
	SetWindowTheme(GetDlgItem(IDC_CHECK_MCCHECK)->GetSafeHwnd(), _T(""), _T(""));
	SetWindowTheme(GetDlgItem(IDC_CHECK_ORDER_STATUS)->GetSafeHwnd(), _T(""), _T(""));
	SetWindowTheme(GetDlgItem(IDC_CHECK_MAX_MARKTIMES)->GetSafeHwnd(), _T(""), _T(""));
	//SetWindowTheme(GetDlgItem(IDC_CHECK_PEN_ORDER)->GetSafeHwnd(), _T(""), _T(""));
	SetWindowTheme(GetDlgItem(IDC_CHECK_STRIP_MAP)->GetSafeHwnd(), _T(""), _T("")); 
	SetWindowTheme(GetDlgItem(IDC_CHECK_2D_BARCODE)->GetSafeHwnd(), _T(""), _T(""));
	SetWindowTheme(GetDlgItem(IDC_CHECK_ADD_PREFIX)->GetSafeHwnd(), _T(""), _T(""));
	SetWindowTheme(GetDlgItem(IDC_CHECK_VISUALSYSTEM)->GetSafeHwnd(), _T(""), _T(""));
	SetWindowTheme(GetDlgItem(IDC_VISION_CHECK)->GetSafeHwnd(), _T(""), _T(""));
	SetWindowTheme(GetDlgItem(IDC_CHECK_STRIPID_MATCH_LOTID)->GetSafeHwnd(), _T(""), _T(""));
	SetWindowTheme(GetDlgItem(IDC_CHECK_PRODUCT_TYPE)->GetSafeHwnd(), _T(""), _T(""));
	SetWindowTheme(GetDlgItem(IDC_FINS_CHECK_MES_CHECK)->GetSafeHwnd(), _T(""), _T(""));
	SetWindowTheme(GetDlgItem(IDC_CHECK_MES_AUTO_LOT)->GetSafeHwnd(), _T(""), _T(""));
	

	SetWindowTheme(GetDlgItem(IDC_CHECK_VISION)->GetSafeHwnd(), _T(""), _T(""));
	SetWindowTheme(GetDlgItem(IDC_CHECK_PLC)->GetSafeHwnd(), _T(""), _T(""));
	SetWindowTheme(GetDlgItem(IDC_FINS_CHECK)->GetSafeHwnd(), _T(""), _T(""));
}

void CDlgFactorySet::UpdateUIFromConfig()
{
	m_nWorkMode = m_pFactoryConfig->m_nWorkMode;
	m_bOpenRMS = m_pFactoryConfig->m_bOpenRMS;
	m_bOpenRollTip = m_pFactoryConfig->m_bOpenRollTip;
	m_nPort = m_pFactoryConfig->m_nPort;
	m_iPort2 = m_pFactoryConfig->m_nPortPLC;
	m_sMarkFilePath = m_pFactoryConfig->m_sMarkFilePath;
	m_sLogFilePath = m_pFactoryConfig->m_sLogFilePath;
	m_sLotIDXMLPath = m_pFactoryConfig->m_sLotIDXMLPath;
	m_sScripIDXMLPath = m_pFactoryConfig->m_sScripIDXMLPath;
	m_sRmsIP = m_pFactoryConfig->m_sFtpIP;
	m_sTipText = m_pFactoryConfig->m_sWarnTips;
	m_sMesIP = m_pFactoryConfig->m_sMesIP;
	m_sMesUrl = m_pFactoryConfig->m_sMesUrl;
	m_sMesUrl2 = m_pFactoryConfig->m_sMesCheckUrl;
	m_sMesUrl3 = m_pFactoryConfig->m_sMesTrackInUrl;
	m_sMesUrl4 = m_pFactoryConfig->m_sMesThickUrl;
	m_sMesUrlProducttype = m_pFactoryConfig->m_sMesProductType;
	m_sMobFile = m_pFactoryConfig->m_sMobFilePath;
	m_sDeviceNo = m_pFactoryConfig->m_sDeviceNo;
	m_sMarkFileHead = m_pFactoryConfig->m_sBarcodeHead;
	m_bMcCheck = m_pFactoryConfig->m_bMCCheckWarn;
	m_sMcCheckTip = m_pFactoryConfig->m_sMCCheckTip;
	m_nMCCheckTimes = m_pFactoryConfig->m_nRetryTimes;
	m_bCheckStatus = m_pFactoryConfig->m_bCheckOrderStatus;
	m_s2DPath = m_pFactoryConfig->m_s2DPath;
	m_bMaxMarkTimesLimit = m_pFactoryConfig->m_bMaxMarkTimesLimit;
	m_nMaxMarkTimes = m_pFactoryConfig->m_nMaxMarkTimes;
	m_sStampFile = m_pFactoryConfig->m_sStampPath;
	m_sGFHPath = m_pFactoryConfig->m_sGFHPath;
	m_sLaserParamSave = m_pFactoryConfig->m_sLaserParamSave;
	m_bVisionCheck = m_pFactoryConfig->m_bVisionCheck;
	SetDlgItemInt(IDC_EDIT_PORT3, m_pFactoryConfig->m_iVisionCheckPort);
	SetDlgItemInt(IDC_EDIT_FINS_PORT, m_pFactoryConfig->m_iFinsPort);
	SetDlgItemText(IDC_EDIT_FINS_IP, m_pFactoryConfig->m_sFinsIP);
	UpdateData(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_PEN_ORDER))->SetCheck(m_pFactoryConfig->m_bPenOrder);
	((CButton*)GetDlgItem(IDC_CHECK_2D_BARCODE))->SetCheck(m_pFactoryConfig->m_bCheck2DBarcode); 
	((CButton*)GetDlgItem(IDC_CHECK_STRIP_MAP))->SetCheck(m_pFactoryConfig->m_bCheckStripMap);
	((CButton*)GetDlgItem(IDC_CHECK_ADD_PREFIX))->SetCheck(m_pFactoryConfig->m_bAddFilePrefix);
	((CButton*)GetDlgItem(IDC_CHECK_VISUALSYSTEM))->SetCheck(m_pFactoryConfig->m_bCheckVisualSystem);
	((CButton*)GetDlgItem(IDC_VISION_CHECK))->SetCheck(m_pFactoryConfig->m_bVisionCheck);
	((CButton*)GetDlgItem(IDC_CHECK_STRIPID_MATCH_LOTID))->SetCheck(m_pFactoryConfig->m_bCheckEQPStripIdIsMatchLotId);
	((CButton*)GetDlgItem(IDC_CHECK_PRODUCT_TYPE))->SetCheck(m_pFactoryConfig->m_bCheckProductType);
	((CButton*)GetDlgItem(IDC_CHECK_PLC))->SetCheck(m_pFactoryConfig->m_bCheckPLCSwitchProcess);
	((CButton*)GetDlgItem(IDC_FINS_CHECK))->SetCheck(m_pFactoryConfig->m_bCheckFinsPLC);
	
	 m_combo2DRepeatMode.SetCurSel(m_pFactoryConfig->m_nCheck2DBarcodeRepeat);
}

void CDlgFactorySet::SaveConfigFromUI()
{
	UpdateData(TRUE);
	SETCHANGEDETECT->CompareAndChangeStringVar(_T("m_sLotIDXMLPath"), m_sLotIDXMLPath);
	SETCHANGEDETECT->CompareAndChangeStringVar(_T("m_sScripIDXMLPath"), m_sScripIDXMLPath);
	SETCHANGEDETECT->CompareAndChangeIntVar(_T("m_nWorkMode"), m_nWorkMode);
	SETCHANGEDETECT->CompareAndChangeBoolVar(_T("m_bOpenRMS"), m_bOpenRMS);
	SETCHANGEDETECT->CompareAndChangeBoolVar(_T("m_bOpenRollTip"), m_bOpenRollTip);
	SETCHANGEDETECT->CompareAndChangeIntVar(_T("m_nPort"), m_nPort);
	SETCHANGEDETECT->CompareAndChangeIntVar(_T("m_nPortPLC"), m_iPort2);
	SETCHANGEDETECT->CompareAndChangeStringVar(_T("m_sMarkFilePath"), m_sMarkFilePath);
	SETCHANGEDETECT->CompareAndChangeStringVar(_T("m_sLogFilePath"), m_sLogFilePath);
	SETCHANGEDETECT->CompareAndChangeStringVar(_T("m_sFtpIP"), m_sRmsIP);
	m_sTipText.Replace(_T("\r\n"), _T(" "));
	SETCHANGEDETECT->CompareAndChangeStringVar(_T("m_sWarnTips"), m_sTipText);
	SETCHANGEDETECT->CompareAndChangeStringVar(_T("m_sMesIP"), m_sMesIP);
	SETCHANGEDETECT->CompareAndChangeStringVar(_T("m_sMesUrl"), m_sMesUrl);
	SETCHANGEDETECT->CompareAndChangeStringVar(_T("m_sMesCheckUrl"), m_sMesUrl2);
	SETCHANGEDETECT->CompareAndChangeStringVar(_T("m_sMesTrackInUrl"), m_sMesUrl3);
	SETCHANGEDETECT->CompareAndChangeStringVar(_T("m_sMesThickUrl"), m_sMesUrl4);
	SETCHANGEDETECT->CompareAndChangeStringVar(_T("m_sMesUrlProducttype"), m_sMesUrlProducttype);
	SETCHANGEDETECT->CompareAndChangeStringVar(_T("m_sMobFilePath"), m_sMobFile);
	SETCHANGEDETECT->CompareAndChangeStringVar(_T("m_sDeviceNo"), m_sDeviceNo);
	SETCHANGEDETECT->CompareAndChangeStringVar(_T("m_sBarcodeHead"), m_sMarkFileHead);
	SETCHANGEDETECT->CompareAndChangeBoolVar(_T("m_bMCCheckWarn"), m_bMcCheck);
	SETCHANGEDETECT->CompareAndChangeStringVar(_T("m_sMCCheckTip"), m_sMcCheckTip);
	SETCHANGEDETECT->CompareAndChangeIntVar(_T("m_nRetryTimes"), m_nMCCheckTimes);
	SETCHANGEDETECT->CompareAndChangeBoolVar(_T("m_bCheckOrderStatus"), m_bCheckStatus);
	SETCHANGEDETECT->CompareAndChangeStringVar(_T("m_s2DPath"), m_s2DPath);
	SETCHANGEDETECT->CompareAndChangeBoolVar(_T("m_bMaxMarkTimesLimit"), m_bMaxMarkTimesLimit);
	SETCHANGEDETECT->CompareAndChangeIntVar(_T("m_nMaxMarkTimes"), m_nMaxMarkTimes);
	SETCHANGEDETECT->CompareAndChangeBoolVar(_T("m_bPenOrder"), ((CButton*)GetDlgItem(IDC_CHECK_PEN_ORDER))->GetCheck());
	SETCHANGEDETECT->CompareAndChangeBoolVar(_T("m_bCheck2DBarcode"), ((CButton*)GetDlgItem(IDC_CHECK_2D_BARCODE))->GetCheck());
	SETCHANGEDETECT->CompareAndChangeBoolVar(_T("m_bCheckStripMap"), ((CButton*)GetDlgItem(IDC_CHECK_STRIP_MAP))->GetCheck());
	SETCHANGEDETECT->CompareAndChangeBoolVar(_T("m_bAddFilePrefix"), ((CButton*)GetDlgItem(IDC_CHECK_ADD_PREFIX))->GetCheck());
	SETCHANGEDETECT->CompareAndChangeBoolVar(_T("m_bCheckVisualSystem"), ((CButton*)GetDlgItem(IDC_CHECK_VISUALSYSTEM))->GetCheck());
	SETCHANGEDETECT->CompareAndChangeBoolVar(_T("m_bVisionCheck"), ((CButton*)GetDlgItem(IDC_VISION_CHECK))->GetCheck());
	SETCHANGEDETECT->CompareAndChangeBoolVar(_T("m_bCheckEQPStripIdIsMatchLotId"), ((CButton*)GetDlgItem(IDC_CHECK_STRIPID_MATCH_LOTID))->GetCheck());
	SETCHANGEDETECT->CompareAndChangeBoolVar(_T("m_bCheckProductType"), ((CButton*)GetDlgItem(IDC_CHECK_PRODUCT_TYPE))->GetCheck());
	SETCHANGEDETECT->CompareAndChangeBoolVar(_T("m_bCheckVisionSwitchProcess"), ((CButton*)GetDlgItem(IDC_CHECK_VISION))->GetCheck());
	SETCHANGEDETECT->CompareAndChangeBoolVar(_T("m_bCheckPLCSwitchProcess"), ((CButton*)GetDlgItem(IDC_CHECK_PLC))->GetCheck());
	SETCHANGEDETECT->CompareAndChangeBoolVar(_T("m_bCheckFinsPLC"), ((CButton*)GetDlgItem(IDC_FINS_CHECK))->GetCheck());

	// 启用mes校验
	SETCHANGEDETECT->CompareAndChangeBoolVar(_T("m_EnableFinishedLot"), ((CButton*)GetDlgItem(IDC_FINS_CHECK_MES_CHECK))->GetCheck());


	SETCHANGEDETECT->CompareAndChangeStringVar(_T("m_sStampPath"), m_sStampFile);
	SETCHANGEDETECT->CompareAndChangeStringVar(_T("m_sGFHPath"), m_sGFHPath);
	SETCHANGEDETECT->CompareAndChangeStringVar(_T("m_sLaserParamSave"), m_sLaserParamSave);
	SETCHANGEDETECT->CompareAndChangeIntVar(_T("m_nCheck2DBarcodeRepeat"), m_combo2DRepeatMode.GetCurSel());
	SETCHANGEDETECT->CompareAndChangeIntVar(_T("m_iVisionCheckPort"), GetDlgItemInt(IDC_EDIT_PORT3));
	SETCHANGEDETECT->CompareAndChangeIntVar(_T("m_iFinsPort"), GetDlgItemInt(IDC_EDIT_FINS_PORT));
	CString strInfo;
	GetDlgItemText(IDC_EDIT_URL_FINISHED_LOT, strInfo);
	SETCHANGEDETECT->CompareAndChangeStringVar(_T("m_FinishedLotUrl"), strInfo);

	

	CString str;
	GetDlgItemText(IDC_EDIT_FINS_IP, str);
	SETCHANGEDETECT->CompareAndChangeStringVar(_T("m_sFinsIP"), str);

	

	m_pFactoryConfig->SaveConfig();
}

bool CDlgFactorySet::CreateDefaultFilePath()
{
	// 自动检测默认路径是否存在，不存在创建路径
	BOOL bRet = CreateMultiPath(m_sMarkFilePath);
	if (!bRet) {
		AfxMessageBox(_T("文件路径创建失败！"));
		return false;
	}
	bRet = CreateMultiPath(m_sLogFilePath);
	if (!bRet) {
		AfxMessageBox(_T("文件路径创建失败！"));
		return false;
	}
	bRet = CreateMultiPath(m_sMobFile);
	if (!bRet) {
		AfxMessageBox(_T("文件路径创建失败！"));
		return false;
	}
	bRet = CreateMultiPath(m_s2DPath);
	if (!bRet) {
		AfxMessageBox(_T("文件路径创建失败！"));
		return false;
	}
	bRet = CreateMultiPath(m_sLotIDXMLPath);
	if (!bRet)
	{
		AfxMessageBox(_T("文件路径创建失败！"));
		return false;
	}

	bRet = CreateMultiPath(m_sScripIDXMLPath);
	if (!bRet)
	{
		AfxMessageBox(_T("文件路径创建失败！"));
		return false;
	}

	bRet = CreateMultiPath(m_sGFHPath);
	if (!bRet)
	{
		AfxMessageBox(_T("文件路径创建失败！"));
		return false;
	}

	bRet = CreateMultiPath(m_sLaserParamSave);
	if (!bRet)
	{
		AfxMessageBox(_T("文件路径创建失败！"));
		return false;
	}

	bRet = CreateMultiPath(m_sStampFile);
	if (!bRet)
	{
		AfxMessageBox(_T("文件路径创建失败！"));
		return false;
	}

	bRet = CreateMultiPath(_T("D:\\HT StripID Record\\"));
	if (!bRet)
	{
		AfxMessageBox(_T("文件路径创建失败！"));
		return false;
	}

	bRet = CreateMultiPath(_T("D:\\HT LotID Record\\"));
	if (!bRet)
	{
		AfxMessageBox(_T("文件路径创建失败！"));
		return false;
	}
	return true;
}

void CDlgFactorySet::OnBnClickedButtonUserManager()//用户管理
{
	CDlgUserMananger manaDlg;
	if (IDOK == manaDlg.DoModal()) {
		GetDlgItem(IDC_BUTTON_ADJUST_SET)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SAVE_SET)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CANCEL_SET)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_USER_MANAGER)->EnableWindow(FALSE);
	}
}

void CDlgFactorySet::ChangeTab()  //视图切换到非“系统设置”页
{
	EnableControlByRights();
}


void CDlgFactorySet::OnBnClickedButtonDeviceConfig()  //硬件平台设置
{
	CDlgOptionHardware dlg(this);
	if (IDOK == dlg.DoModal()) {
		//ShowCCDControl();
		//((CButton*)GetDlgItem(IDC_CHECK_CCD))->SetCheck(CConfigure::Instance()->m_IsVisionEnable);
	}
}


void CDlgFactorySet::OnBnClickedButtonLaserSet()  //激光器设置
{
	CMarkPlatform::Instance()->MarkDevice()->ShowDevOptionWnd();
}


void CDlgFactorySet::OnBnClickedButtonCalibration()  //双头校正
{
	CDlgMultiCalib dlg;
	dlg.DoModal();
}


void CDlgFactorySet::OnBnClickedButtonScannerSet()  //扫描枪设置
{
	CDlgScannerSet dlg(m_pMainDlg->m_pWorkControl);
	dlg.DoModal();
}


void CDlgFactorySet::OnBnClickedButtonLotIDXmlPath()
{
#if 0
	BROWSEINFO   bi;                           //创建BROWSEINFO结构体;
	TCHAR   Buffer[512] = { 0 };
	TCHAR   FullPath[512] = { 0 };
	bi.hwndOwner = m_hWnd;                 //窗口句柄;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = Buffer;            //返回选择的目录名的缓冲区;
	bi.lpszTitle = _T("选择LotID(XML)目录");           //弹出的窗口的文字提示;
	bi.ulFlags = BIF_RETURNONLYFSDIRS;  //只返回目录。其他标志看MSDN;
	bi.lpfn = NULL;               //回调函数，有时很有用;
	bi.lParam = 0;
	bi.iImage = 0;
	ITEMIDLIST*   pidl = ::SHBrowseForFolder(&bi);   //显示弹出窗口，ITEMIDLIST很重要;
	::SHGetPathFromIDList(pidl, FullPath); //在ITEMIDLIST中得到目录名的整个路径;
	if (_tcslen(FullPath) > 0)
	{
		m_sLotIDXMLPath.Format(_T("%s"), FullPath);
		SetDlgItemText(IDC_EDIT_LOTID_XML_PATH, m_sLotIDXMLPath);
	}
#else
	if (OpenFolderSelectDlg(_T("选择LotID(XML)目录"), m_sLotIDXMLPath, this)) {
		SetDlgItemText(IDC_EDIT_LOTID_XML_PATH, m_sLotIDXMLPath);
	}

#endif
}


void CDlgFactorySet::OnBnClickedButtonScripidXmlPath()
{
#if 0
	BROWSEINFO   bi;                           //创建BROWSEINFO结构体;
	TCHAR   Buffer[512] = { 0 };
	TCHAR   FullPath[512] = { 0 };
	bi.hwndOwner = m_hWnd;                 //窗口句柄;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = Buffer;            //返回选择的目录名的缓冲区;
	bi.lpszTitle = _T("选择StripID(XML)目录");           //弹出的窗口的文字提示;
	bi.ulFlags = BIF_RETURNONLYFSDIRS;  //只返回目录。其他标志看MSDN;
	bi.lpfn = NULL;               //回调函数，有时很有用;
	bi.lParam = 0;
	bi.iImage = 0;
	ITEMIDLIST*   pidl = ::SHBrowseForFolder(&bi);   //显示弹出窗口，ITEMIDLIST很重要;
	::SHGetPathFromIDList(pidl, FullPath); //在ITEMIDLIST中得到目录名的整个路径;
	if (_tcslen(FullPath) > 0)
	{
		m_sScripIDXMLPath.Format(_T("%s"), FullPath);
		SetDlgItemText(IDC_EDIT_SCRIPID_XML_PATH, m_sScripIDXMLPath);
	}
#else
	if (OpenFolderSelectDlg(_T("选择StripID(XML)目录"), m_sScripIDXMLPath, this)) {
		SetDlgItemText(IDC_EDIT_SCRIPID_XML_PATH, m_sScripIDXMLPath);
	}
#endif
}


void CDlgFactorySet::OnBnClickedButtonStampfile()
{
	// TODO: 在此添加控件通知处理程序代码
#if 0
	BROWSEINFO   bi;                           //创建BROWSEINFO结构体;
	TCHAR   Buffer[512] = { 0 };
	TCHAR   FullPath[512] = { 0 };
	bi.hwndOwner = m_hWnd;                 //窗口句柄;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = Buffer;            //返回选择的目录名的缓冲区;
	bi.lpszTitle = _T("选择印章图档文件目录");           //弹出的窗口的文字提示;
	bi.ulFlags = BIF_RETURNONLYFSDIRS;  //只返回目录。其他标志看MSDN;
	bi.lpfn = NULL;               //回调函数，有时很有用;
	bi.lParam = 0;
	bi.iImage = 0;
	ITEMIDLIST*   pidl = ::SHBrowseForFolder(&bi);   //显示弹出窗口，ITEMIDLIST很重要;
	::SHGetPathFromIDList(pidl, FullPath); //在ITEMIDLIST中得到目录名的整个路径;
	if (_tcslen(FullPath)>0)
	{
		m_sStampFile.Format(_T("%s"), FullPath);
		SetDlgItemText(IDC_EDIT_STAMPFILE, m_sStampFile);
	}

#else
if (OpenFolderSelectDlg(_T("选择印章图档文件目录"), m_sStampFile, this)) {
	SetDlgItemText(IDC_EDIT_STAMPFILE, m_sStampFile);
}
#endif
}


void CDlgFactorySet::OnBnClickedButtonNineCalc()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgAnchorPointSet dlg;
	dlg.DoModal();

}


void CDlgFactorySet::OnBnClickedButtonLaserUse()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgLaserUsage usageDlg;
	usageDlg.DoModal();
	m_pMainDlg->m_pWorkControl->InitDiodeTime();
}

void CDlgFactorySet::Init2DRepeatMode()
{
	m_combo2DRepeatMode.AddString(_T("不比对"));
	m_combo2DRepeatMode.AddString(_T("按条比对"));
	m_combo2DRepeatMode.AddString(_T("按颗比对"));
	CString s;
	s.Format(_T("二维码比对模式(%d天以内)"), m_pFactoryConfig->m_nSavePeriod);
	SetDlgItemText(IDC_STATIC_2D_REPEAT_MODE, s);
}


void CDlgFactorySet::OnBnClickedButtonStartserver2()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON_STARTSERVER2)->EnableWindow(FALSE);
	int nPort = 0;
	CString ip;
	nPort = GetDlgItemInt(IDC_EDIT_PORT2);
	if (m_pMainDlg->StartServerPLC(nPort))
	{
		AfxMessageBox(_T("PLC服务器启动成功！"));
	}
	else
	{
		AfxMessageBox(_T("PLC服务器启动失败！"));
	}
	GetDlgItem(IDC_BUTTON_STARTSERVER2)->EnableWindow(TRUE);
}


void CDlgFactorySet::OnDestroy()
{
	CDialogEx::OnDestroy();
	SETCHANGEDETECT->Delete();
	// TODO: 在此处添加消息处理程序代码
}


void CDlgFactorySet::OnBnClickedButtonGfh()
{
	// TODO: 在此添加控件通知处理程序代码
#if 0
	BROWSEINFO   bi;                           //创建BROWSEINFO结构体;
	TCHAR   Buffer[512] = { 0 };
	TCHAR   FullPath[512] = { 0 };
	bi.hwndOwner = m_hWnd;                 //窗口句柄;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = Buffer;            //返回选择的目录名的缓冲区;
	bi.lpszTitle = _T("选择印章规范号路径");           //弹出的窗口的文字提示;
	bi.ulFlags = BIF_RETURNONLYFSDIRS;  //只返回目录。其他标志看MSDN;
	bi.lpfn = NULL;               //回调函数，有时很有用;
	bi.lParam = 0;
	bi.iImage = 0;
	ITEMIDLIST*   pidl = ::SHBrowseForFolder(&bi);   //显示弹出窗口，ITEMIDLIST很重要;
	::SHGetPathFromIDList(pidl, FullPath); //在ITEMIDLIST中得到目录名的整个路径;
	if (_tcslen(FullPath)>0)
	{
		m_sGFHPath.Format(_T("%s"), FullPath);
		SetDlgItemText(IDC_EDIT_GFH_PATH, m_sGFHPath);
	}
#else
	if (OpenFolderSelectDlg(_T("选择印章规范号路径"), m_sGFHPath, this)) {
		SetDlgItemText(IDC_EDIT_GFH_PATH, m_sGFHPath);
	}
#endif
}


void CDlgFactorySet::OnBnClickedButtonLaserParamSavePath()
{
	// TODO: 在此添加控件通知处理程序代码
#if 0
	BROWSEINFO   bi;                           //创建BROWSEINFO结构体;
	TCHAR   Buffer[512] = { 0 };
	TCHAR   FullPath[512] = { 0 };
	bi.hwndOwner = m_hWnd;                 //窗口句柄;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = Buffer;            //返回选择的目录名的缓冲区;
	bi.lpszTitle = _T("选择激光参数保存路径");           //弹出的窗口的文字提示;
	bi.ulFlags = BIF_RETURNONLYFSDIRS;  //只返回目录。其他标志看MSDN;
	bi.lpfn = NULL;               //回调函数，有时很有用;
	bi.lParam = 0;
	bi.iImage = 0;
	ITEMIDLIST*   pidl = ::SHBrowseForFolder(&bi);   //显示弹出窗口，ITEMIDLIST很重要;
	::SHGetPathFromIDList(pidl, FullPath); //在ITEMIDLIST中得到目录名的整个路径;
	if (_tcslen(FullPath)>0)
	{
		m_sLaserParamSave.Format(_T("%s"), FullPath);
		SetDlgItemText(IDC_EDIT_LASER_PARAM_SAVE_PATH, m_sLaserParamSave);
	}
#else
	if (OpenFolderSelectDlg(_T("选择激光参数保存路径"), m_sLaserParamSave, this)) {
		SetDlgItemText(IDC_EDIT_LASER_PARAM_SAVE_PATH, m_sLaserParamSave);
	}
#endif
}


void CDlgFactorySet::OnBnClickedButtonDeleteData()
{
	// TODO: 在此添加控件通知处理程序代码
	if (IDYES == MessageBox(_T("确认删除过期的数据？（等待时间60秒左右）"), _T("提示"), MB_YESNO))
	{
		
		m_pMainDlg->m_pWorkControl->m_2DRepeatCheck.DeleteDataOutOfDate();
		AfxMessageBox(_T("删除完成！"));
	}
}


void CDlgFactorySet::OnBnClickedButtonStartserver3()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON_STARTSERVER3)->EnableWindow(FALSE);
	int nPort = 0;
	CString ip;
	nPort = GetDlgItemInt(IDC_EDIT_PORT3);
	if (m_pMainDlg->StartServerVisionCheck(nPort))
	{
		AfxMessageBox(_T("视觉检测服务器启动成功！"));
	}
	else
	{
		AfxMessageBox(_T("视觉检测服务器启动失败！"));
	}
	GetDlgItem(IDC_BUTTON_STARTSERVER3)->EnableWindow(TRUE);
}


void CDlgFactorySet::OnBnClickedButtonFinsConnect()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strIP;
	int nPort = 0;
	GetDlgItem(IDC_EDIT_FINS_IP)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_FINS_PORT)->EnableWindow(FALSE);

	
	nPort = GetDlgItemInt(IDC_EDIT_FINS_PORT);
	GetDlgItemText(IDC_EDIT_FINS_IP, strIP);
	if (FINSCLIENT->ConnServer(strIP, nPort))
	{
		AfxMessageBox(_T("Fins客户端连接成功！"));
		
	}
	else
	{
		AfxMessageBox(_T("Fins客户端连接失败！"));
	}
}
