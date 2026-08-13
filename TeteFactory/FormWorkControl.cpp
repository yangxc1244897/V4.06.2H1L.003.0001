// FormWorkControl.cpp : 实现文件
//

#include "stdafx.h"
#include "TeteFactory.h"
#include "FormWorkControl.h"
#include "afxdialogex.h"
#include "MarkPlatform.h"
#include "Configure.h"
#include "TeteFactoryDlg.h"
#include "DlgMapAdjust.h"
#include "tlString.h"
#include <set>
#include "DlgMCCheckWarn.h"
#include "DlgSelectLaserParam.h"
#include "Sources\tinyxml2.h"
#include "DlgInputStripID.h"
#include "MySoap.h"
#include <thread>
#include"Mes\HttpComHT.h"
#include "FinsTCP//FinsTcpClient.h"
#include "ICMesComEAPHttpQP.h"
#define SCANSIGNAL_CHECK_TIMER 7
#define ENABLE_SCAN_THREAD


//#define DEBUG_IO

// CFormWorkControl 对话框
static UINT g_nTextName[10] = { IDC_STATIC_TEXT1, IDC_STATIC_TEXT2, IDC_STATIC_TEXT3,
IDC_STATIC_TEXT4, IDC_STATIC_TEXT5, IDC_STATIC_TEXT6,
IDC_STATIC_TEXT7, IDC_STATIC_TEXT8, IDC_STATIC_TEXT9, IDC_STATIC_TEXT10 };

static UINT g_nTextValue[10] = { IDC_EDIT_TEXT1, IDC_EDIT_TEXT2, IDC_EDIT_TEXT3,
IDC_EDIT_TEXT4, IDC_EDIT_TEXT5, IDC_EDIT_TEXT6,
IDC_EDIT_TEXT7, IDC_EDIT_TEXT8, IDC_EDIT_TEXT9, IDC_EDIT_TEXT10 };

static UINT g_nFontName[10] = { IDC_EDIT_NAME1, IDC_EDIT_NAME2, IDC_EDIT_NAME3,
IDC_EDIT_NAME4, IDC_EDIT_NAME5, IDC_EDIT_NAME6,
IDC_EDIT_NAME7, IDC_EDIT_NAME8, IDC_EDIT_NAME9, IDC_EDIT_NAME10 };

static const CString g_sTempConfigPath = _T("config\\factorytemp.ini");


IMPLEMENT_DYNAMIC(CFormWorkControl, CDialogEx)

CFormWorkControl::CFormWorkControl(class CTeteFactoryDlg * p, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FORM_WORK_CONTROL, pParent)
	, m_Scan2DSignal(this)
	, m_StartMarkSingal(this)
{
	m_pMainDlg = p;
	m_bInit = FALSE;
	m_pConfig = CFactoryConfig::Instance();

	m_pMarkContentView = NULL;
	m_pViewAllBin = NULL;
	m_pWaferDoc = NULL;
	m_pMapDoc = NULL;
	m_bOpenFile = FALSE;
	m_nMarkFlag = -1;
	m_bSysOK = FALSE;
	m_nLotFlag = 0;
	m_dbMarkTime = 0;
	m_nMarkCount = 0;
	m_pServer = NULL;
	m_pServerPLC = NULL;
	m_pFactoryMesComm = NULL;
	m_nPrePort = 0;
	m_nPreWorkMode = 0;
	m_bScanBarcode = FALSE;

	m_bManualStopMark = FALSE;
	m_bManualStartMark = FALSE;
	// 从配置档中直接读取
	m_nRetryTimes = GetPrivateProfileInt(_T("Global"), _T("RetryTimes"), 0, g_sTempConfigPath);
	GetPrivateProfileString(_T("Global"), _T("CheckMarkFile"), _T(""), m_sCheckMarkFile.GetBuffer(1024), 1024, g_sTempConfigPath);
	m_sCheckMarkFile.ReleaseBuffer();

	m_pMarkDriver = CMarkPlatform::Instance()->MarkDriver();
	m_pMarkEngine = CMarkPlatform::Instance()->MarkEngine();

	m_BkBrush.CreateSolidBrush(BKCOLOR);
	m_EditBkBrush.CreateSolidBrush(EDIT_BKCOLOR);

	m_LoginUserName = _T("");
	m_LoginUserType = -1;
	m_LaserUsagePath = _T("config\\LaserUsage.ini");
	m_IfConnectCCD = FALSE;
	m_bVisionCheck = FALSE;
	m_markType.Empty();
	m_varName.Empty();
	m_sLaserParamCur.Empty();
	m_sLotIDCur.Empty();
	m_sStripIDCur.Empty();
	m_bSetVisionOK = FALSE;
	m_bPLCSwitchProcessOK = FALSE;
	m_bSwitchVisionCheckProcessOK = FALSE;

	CMarkPlatform::Instance()->RegisterMsgWnd(this);
	// 将CDialogNet对象dlg传递给CFormWorkControl构造函数

	
}

CFormWorkControl::~CFormWorkControl()
{
	m_BkBrush.DeleteObject();
	m_EditBkBrush.DeleteObject();
	m_FileFont.DeleteObject();
	m_TipFont.DeleteObject();
	m_FileFont.DeleteObject();

	delete m_pMySoap;

	
}

void CFormWorkControl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_VIEWFRAME, m_ViewFrame);
	DDX_Control(pDX, IDC_STATIC_TIPS, m_ctrlTips);
	DDX_Control(pDX, IDC_STATIC_LEFTFRAME, m_LeftFrame);
	DDX_Control(pDX, IDC_STATIC_VIEWFRAME_STAMP, m_ctrlViewFrame_Stamp);
	DDX_Control(pDX, IDC_STATIC_VISION_STATUS, m_sStatusVision);
	DDX_Control(pDX, IDC_STATIC_STARTMARK_STATUS, m_sStatusStartMark);
	DDX_Control(pDX, IDC_STATIC_MAPPING_STATUS, m_sStatusMapping);
	DDX_Control(pDX, IDC_STATIC_2DBARCODE_STATUS, m_sStatus2DBarcode);
	DDX_Control(pDX, IDC_STATIC_PWS_LOAD_STATUS, m_sStatusPWS);
	DDX_Control(pDX, IDC_STATIC_2DBARCODE_BEGIN_STATUS, m_sStatus2DBarcodeBegin);
	DDX_Control(pDX, IDC_STATIC_NET, m_sNetConnect);
	DDX_Control(pDX, IDC_LIST_LASER_PARAM, m_listLaserParam);
}


BEGIN_MESSAGE_MAP(CFormWorkControl, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_MAP_ADJUST, &CFormWorkControl::OnBnClickedButtonMapAdjust)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_SET, &CFormWorkControl::OnBnClickedButtonLoadSet)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CFormWorkControl::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_CELL, &CFormWorkControl::OnBnClickedButtonCell)
	ON_BN_CLICKED(IDC_BUTTON_MATRIX, &CFormWorkControl::OnBnClickedButtonMatrix)
	ON_BN_CLICKED(IDC_BUTTON_AREA, &CFormWorkControl::OnBnClickedButtonArea)
	ON_BN_CLICKED(IDC_BUTTON_STARTLOT, &CFormWorkControl::OnBnClickedButtonStartlot)
	ON_BN_CLICKED(IDC_BUTTON_STOPLOT, &CFormWorkControl::OnBnClickedButtonStoplot)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_JOB, &CFormWorkControl::OnBnClickedButtonLoadJob)
	ON_BN_CLICKED(IDC_BUTTON_CANCELJOB, &CFormWorkControl::OnBnClickedButtonCanceljob)
	ON_BN_CLICKED(IDC_BUTTON_STARTMARK, &CFormWorkControl::OnBnClickedButtonStartmark)
	ON_BN_CLICKED(IDC_BUTTON_STOPMARK, &CFormWorkControl::OnBnClickedButtonStopmark)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_LASER_PARAM, &CFormWorkControl::OnBnClickedButtonLaserParam)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_TEXT_CONTONT, &CFormWorkControl::OnBnClickedButtonEditTextContont)
	ON_BN_CLICKED(IDC_BUTTON_FINISH_EDIT_CONTENT, &CFormWorkControl::OnBnClickedButtonFinishEditContent)
	ON_MESSAGE(WM_USER_SCAN_PROCESS, &CFormWorkControl::OnScanProcess)
	ON_MESSAGE(WM_USER_START_PROCESS, &CFormWorkControl::OnStartProcess)
	ON_BN_CLICKED(IDC_CHECK_TRY_DUMMY, &CFormWorkControl::OnBnClickedCheckTryDummy)
	ON_BN_CLICKED(IDC_CHECK_FIRST_PRINT, &CFormWorkControl::OnBnClickedCheckFirstPrint)
	ON_BN_CLICKED(IDC_BUTTON1, &CFormWorkControl::OnBnClickedButton1)
	ON_LBN_SELCHANGE(IDC_LIST_LASER_PARAM, &CFormWorkControl::OnLbnSelchangeListLaserParam)
END_MESSAGE_MAP()


// CFormWorkControl 消息处理程序


HBRUSH CFormWorkControl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_DLG)
		return (HBRUSH)m_BkBrush.GetSafeHandle();

	// TODO:  在此更改 DC 的任何特性
	UINT nID = pWnd->GetDlgCtrlID();
	if (nID == IDC_STATIC_FILE_NAME
		|| nID == IDC_STATIC_MARKTYPE)
	{
		// 背景为透明
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(/*STATIC_TITLE_COLOR*/STATIC_TITLE_COLOR_NAME);
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	}
	else if (nID == IDC_STATIC_LASER_PARAM)
	{
		// 背景为透明
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(STATIC_TITLE_COLOR);
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);

	}
	else if (nID == IDC_LIST_LASER_PARAM)
	{
		// 背景为透明
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(STATIC_TITLE_COLOR);
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	}
	else if (nID == IDC_LASER_TIME)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(STATIC_TITLE_COLOR);
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	}
	else if (nID == IDC_GROUP_TEXT)
	{
		// 背景为透明
		pDC->SetBkColor(BKCOLOR);//设置字体背景颜色
		pDC->SetTextColor(STATIC_COLOR);
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	}
	else if (nID == IDC_STATIC_OPERID
		|| nID == IDC_STATIC_LOTID
		|| nID == IDC_STATIC_MARKSTAMP
		|| nID == IDC_STATIC_TEXT1
		|| nID == IDC_STATIC_TEXT2
		|| nID == IDC_STATIC_TEXT3
		|| nID == IDC_STATIC_TEXT4
		|| nID == IDC_STATIC_TEXT5
		|| nID == IDC_STATIC_TEXT6
		|| nID == IDC_STATIC_TEXT7
		|| nID == IDC_STATIC_TEXT8
		|| nID == IDC_STATIC_TEXT9
		|| nID == IDC_STATIC_TEXT10
		|| nID == IDC_CHECK_TRY_DUMMY
		|| nID == IDC_CHECK_FIRST_PRINT
		)
	{
		// 背景为透明
		pDC->SetBkMode(TRANSPARENT);
		for (int i = 0; i < 10; i++)
		{
			if (g_nTextName[i] == nID && m_bVariable[i]){
				pDC->SetTextColor(RGB(255, 0, 0));
				return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
			}
		}
		pDC->SetTextColor(STATIC_COLOR);
		
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	}
	else if (nID == IDC_EDIT_OPERID
		|| nID == IDC_EDIT_LOTID
		|| nID == IDC_EDIT_MARKFILE
		|| nID == IDC_EDIT_MARKSTAMP
		|| nID == IDC_EDIT_TEXT1
		|| nID == IDC_EDIT_TEXT2
		|| nID == IDC_EDIT_TEXT3
		|| nID == IDC_EDIT_TEXT4
		|| nID == IDC_EDIT_TEXT5
		|| nID == IDC_EDIT_TEXT6
		|| nID == IDC_EDIT_TEXT7
		|| nID == IDC_EDIT_TEXT8
		|| nID == IDC_EDIT_TEXT9
		|| nID == IDC_EDIT_TEXT10
		|| nID == IDC_EDIT_NAME1
		|| nID == IDC_EDIT_NAME2
		|| nID == IDC_EDIT_NAME3
		|| nID == IDC_EDIT_NAME4
		|| nID == IDC_EDIT_NAME5
		|| nID == IDC_EDIT_NAME6
		|| nID == IDC_EDIT_NAME7
		|| nID == IDC_EDIT_NAME8
		|| nID == IDC_EDIT_NAME9
		|| nID == IDC_EDIT_NAME10)
	{
		pDC->SetBkColor(EDIT_BKCOLOR);//设置字体背景颜色
		for (int i = 0; i < 10; i++)
		{
			if (g_nTextValue[i] == nID && m_bVariable[i]) {
				pDC->SetTextColor(RGB(255, 0, 0));
				return (HBRUSH)m_EditBkBrush.GetSafeHandle();
			}
		}
		pDC->SetTextColor(EDIT_TEXT_COLOR);//设置编辑框字体的颜色
		return (HBRUSH)m_EditBkBrush.GetSafeHandle();
	}


	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CFormWorkControl::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (nType != SIZE_MINIMIZED)  //判断窗口是不是最小化了，因为窗口最小化之后 ，窗口的长和宽会变成0，当前一次变化的时就会出现除以0的错误操作
	{
		if (m_bInit)
		{
			GetClientRect(&m_rcClient);   //最后要更新对话框的大小，当做下一次变化的旧坐标；
			AdjustControl();

			// 更新视图控件
			if (NULL != m_pMarkContentView)
			{
				CRect rc;
				m_ViewFrame.GetClientRect(&rc);
				m_pMarkContentView->MoveWindow(rc);
			}

			if (NULL != m_pViewAllBin)
			{
				CRect rc;
				m_ctrlViewFrame_Stamp.GetClientRect(&rc);
				m_pViewAllBin->MoveWindow(rc);
			}
		}
	}

	// TODO: 在此处添加消息处理程序代码
}


BOOL CFormWorkControl::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
#ifdef DEBUG_IO
	GetDlgItem(IDC_EDIT_IO)->ShowWindow(TRUE);
	GetDlgItem(IDC_EDIT_IO_FINISH)->ShowWindow(TRUE);
#endif

	// 设置状态字体
	m_FileFont.CreateFont(20,
		11, 0, 0,
		FW_BOLD,
		FALSE, FALSE, 0,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		CLEARTYPE_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		_T("Arial"));
	m_TipFont.CreatePointFont(130, _T("Arial"));
	m_BtnFont.CreatePointFont(120, _T("Arial"));
	m_TextFont.CreatePointFont(80, _T("Arial"));
	GetDlgItem(IDC_STATIC_TIPS)->SetFont(&/*m_TipFont*/m_FileFont);
	GetDlgItem(IDC_STATIC_MARKTYPE)->SetFont(&m_FileFont);
	GetDlgItem(IDC_STATIC_FILE_NAME)->SetFont(&/*m_TipFont*/m_FileFont);
	GetDlgItem(IDC_EDIT_MARKFILE)->SetFont(&m_TipFont);
	GetDlgItem(IDC_EDIT_MARKSTAMP)->SetFont(&m_TipFont);
	GetDlgItem(IDC_LASER_TIME)->SetFont(&m_BtnFont); 
	GetDlgItem(IDC_EDIT_TEXT1)->SetFont(&m_TextFont);
	GetDlgItem(IDC_EDIT_TEXT2)->SetFont(&m_TextFont);
	GetDlgItem(IDC_EDIT_TEXT3)->SetFont(&m_TextFont);
	GetDlgItem(IDC_EDIT_TEXT4)->SetFont(&m_TextFont);
	GetDlgItem(IDC_EDIT_TEXT5)->SetFont(&m_TextFont);
	GetDlgItem(IDC_EDIT_TEXT6)->SetFont(&m_TextFont);
	GetDlgItem(IDC_EDIT_TEXT7)->SetFont(&m_TextFont);
	GetDlgItem(IDC_EDIT_TEXT8)->SetFont(&m_TextFont);
	GetDlgItem(IDC_EDIT_TEXT9)->SetFont(&m_TextFont);
	GetDlgItem(IDC_EDIT_TEXT10)->SetFont(&m_TextFont);
	GetDlgItem(IDC_EDIT_OPERID)->SetFont(&m_TextFont);
	GetDlgItem(IDC_EDIT_LOTID)->SetFont(&m_TextFont);
	GetDlgItem(IDC_EDIT_NAME1)->SetFont(&m_TextFont);
	GetDlgItem(IDC_EDIT_NAME2)->SetFont(&m_TextFont);
	GetDlgItem(IDC_EDIT_NAME3)->SetFont(&m_TextFont);
	GetDlgItem(IDC_EDIT_NAME4)->SetFont(&m_TextFont);
	GetDlgItem(IDC_EDIT_NAME5)->SetFont(&m_TextFont);
	GetDlgItem(IDC_EDIT_NAME6)->SetFont(&m_TextFont);
	GetDlgItem(IDC_EDIT_NAME7)->SetFont(&m_TextFont);
	GetDlgItem(IDC_EDIT_NAME8)->SetFont(&m_TextFont);
	GetDlgItem(IDC_EDIT_NAME9)->SetFont(&m_TextFont);
	GetDlgItem(IDC_EDIT_NAME10)->SetFont(&m_TextFont);

	// 按钮字体修改
	GetDlgItem(IDC_BUTTON_RESET)->SetFont(&m_BtnFont);
	GetDlgItem(IDC_BUTTON_AREA)->SetFont(&m_BtnFont);
	GetDlgItem(IDC_BUTTON_CELL)->SetFont(&m_BtnFont);
	GetDlgItem(IDC_BUTTON_MATRIX)->SetFont(&m_BtnFont);
	GetDlgItem(IDC_BUTTON_STARTLOT)->SetFont(&m_BtnFont);
	GetDlgItem(IDC_BUTTON_STOPLOT)->SetFont(&m_BtnFont);
	GetDlgItem(IDC_BUTTON_LOAD_JOB)->SetFont(&m_BtnFont);
	GetDlgItem(IDC_BUTTON_CANCELJOB)->SetFont(&m_BtnFont);
	GetDlgItem(IDC_BUTTON_STARTMARK)->SetFont(&m_BtnFont);
	GetDlgItem(IDC_BUTTON_STOPMARK)->SetFont(&m_BtnFont);

	GetDlgItem(IDC_BUTTON_MAP_ADJUST)->SetFont(&m_BtnFont);
	GetDlgItem(IDC_BUTTON_LOAD_SET)->SetFont(&m_BtnFont);
	GetDlgItem(IDC_BUTTON_LASER_PARAM)->SetFont(&m_BtnFont);
	
	GetDlgItem(IDC_BUTTON_EDIT_TEXT_CONTONT)->SetFont(&m_BtnFont);
	GetDlgItem(IDC_BUTTON_FINISH_EDIT_CONTENT)->SetFont(&m_BtnFont);

	GetDlgItem(IDC_STATIC_LASER_PARAM)->SetFont(&m_BtnFont);
	m_listLaserParam.SetFont(&m_BtnFont);

	/// GRoup Caption字体颜色
	SetWindowTheme(GetDlgItem(IDC_GROUP_TEXT)->GetSafeHwnd(), _T(""), _T(""));
	SetWindowTheme(GetDlgItem(IDC_CHECK_TRY_DUMMY)->GetSafeHwnd(), _T(""), _T(""));
	SetWindowTheme(GetDlgItem(IDC_CHECK_FIRST_PRINT)->GetSafeHwnd(), _T(""), _T(""));

	GetClientRect(m_rcClient);

	// 调整控件大小
	AdjustControl();

	// 加载视图
	m_ViewFrame.ModifyStyle(WS_BORDER, 0);
	CRect rcView;
	m_ViewFrame.GetClientRect(&rcView);
	m_pMarkContentView = new CViewFactory;
	m_pMarkContentView->Create(WS_VISIBLE | WS_CHILD, rcView, &m_ViewFrame, ID_WAFER_VIEW, NULL);

	// 加载BIN视图
	m_ctrlViewFrame_Stamp.ModifyStyle(WS_BORDER, 0);
	CRect rcStampView;
	m_ctrlViewFrame_Stamp.GetClientRect(&rcStampView);
	m_pViewAllBin = new CViewAllBin;
	m_pViewAllBin->Create(WS_VISIBLE | WS_CHILD, rcStampView, &m_ctrlViewFrame_Stamp, ID_WAFER_VIEW_STAMP, NULL);

	// 隐藏文本控件
	for (int i = 0; i < 10; i++)
	{
		GetDlgItem(g_nTextName[i])->ShowWindow(FALSE);
		GetDlgItem(g_nTextValue[i])->ShowWindow(FALSE);
		GetDlgItem(g_nFontName[i])->ShowWindow(FALSE);
		m_bVariable[i] = FALSE;
	}

	// 加载Config
	CFactoryConfig * pFactoryConfig = CFactoryConfig::Instance();
	pFactoryConfig->LoadConfig();

	// 提示标语
	//if (pFactoryConfig->m_bOpenRollTip)
	//{
	//	m_ctrlTips.ShowWindow(TRUE);
	//	m_ctrlTips.ShowText(pFactoryConfig->m_sWarnTips);
	//}
	//else
	//{
	//	m_ctrlTips.ShowWindow(FALSE);
	//	m_ctrlTips.ShowText(_T(""));
	//}

	// 模式切换
	SwitchWorkMode(m_pConfig->m_nWorkMode);
	m_nPreWorkMode = m_pConfig->m_nWorkMode;

	m_pFactoryMesComm = new CFactoryMesComm();
	if (emWorkMode_Scan == pFactoryConfig->m_nWorkMode)
	{
		// 创建客户端
		if (0 != m_pFactoryMesComm->Connect(CStringA(pFactoryConfig->m_sMesIP).GetString(), pFactoryConfig->m_nPort))
		{
			PrintMessage(emMsgType_Error, _T("MES系统连接失败！"));
		}
	}
	else if (emWorkMode_Auto == pFactoryConfig->m_nWorkMode
		|| emWorkMode_Manual == pFactoryConfig->m_nWorkMode)
	{
		// 启动服务器
		StartServer(pFactoryConfig->m_nPort);
		StartServerPLC(pFactoryConfig->m_nPortPLC);
		
		if (m_pConfig->m_bVisionCheck)
		{
			StartVisionCheckServer(pFactoryConfig->m_iVisionCheckPort);
		}
	}

	if (!m_IOComm.InitCard())
	{
		CString s;
		s.Format(_T("初始化7230卡%d失败！"), 0);
		PrintMessage(emMsgType_Error, s);
	}

	// 开始系统硬件检测定时器
	SetTimer(1, 1000, NULL);
#ifdef ENABLE_SCAN_THREAD
	m_Scan2DSignal.Init();
#endif

	// 标记完成信号复位(初始状态)
	m_pMarkDriver->WriteExtPortPin(m_pConfig->m_nBusyPort, IO_OUT_INVALID);
	m_pMarkDriver->WriteExtPortPin(m_pConfig->m_nEndPort, IO_OUT_VALID);
	m_pMarkDriver->WriteExtPortPin(m_pConfig->m_nReadyPort, IO_OUT_VALID);

	//信号显示
	UpdateIOStatus(UISIGNAL_VISION);
	UpdateIOStatus(UISIGNAL_MAP);
	UpdateIOStatus(UISIGNAL_2D);
	UpdateIOStatus(UISIGNAL_PWS);
	UpdateIOStatus(UISIGNAL_NET);
	
#ifdef DEBUG_IO
	SetDlgItemInt(IDC_EDIT_IO_FINISH, !m_pConfig->m_nFinishMarkSignalType);
#endif

	// 空闲状态
	m_pMainDlg->SetStaticText(IDS_MARK_STATUS, MARKSTATUS_IDLE);
	//激光参数
	m_pMainDlg->SetStaticText(IDC_STATIC_LASERPARAM, _T(""));
	m_pMySoap = new CMySoap(m_pConfig->m_sMesUrl);

	//StartLog(_T("SystemSetting"));
	InitDiodeTime();
	UpdateLaserDiodeTime(true);
	SetTimer(2,3600*1000, NULL);// 开启激光器使用时间计时, 1小时更新一次

	m_StartMarkSingal.Init();  //lrf 初始化时启动线程，但不检测

	if (!m_2DRepeatCheck.InitDB())
	{
		CString s;
		s.Format(_T("%s"), m_2DRepeatCheck.GetLastErrorMsg());
		PrintMessage(emMsgType_Error, s);
	}



	m_markType = g_sMTUnit;
	m_bInit = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

BOOL CFormWorkControl::StartServer(unsigned short uport)
{
	if (NULL != m_pServer)
	{
		//PrintMessage(emMsgType_Warn, _T("服务器重新启动！"));
		m_pServer->Close();
		delete m_pServer;
		m_pServer = NULL;
	}
	m_pServer = new CFactoryServer(this);
	if (INVALID_SOCKET == m_pServer->m_hSocket)
	{
		if (!m_pServer->Create(uport, SOCK_STREAM, FD_ACCEPT | FD_CLOSE))
		{
			PrintMessage(emMsgType_Error, _T("视觉服务器启动失败！"));
			m_pServer->Close();
			delete m_pServer;
			m_pServer = NULL;
			return FALSE;
		}
		else
		{
			if (!m_pServer->Listen(1))
			{
				int nErrorCode = m_pServer->GetLastError();
				if (nErrorCode != WSAEWOULDBLOCK)
				{
					PrintMessage(emMsgType_Error, _T("视觉服务器启动失败！"));
					m_pServer->Close();
					delete m_pServer;
					m_pServer = NULL;
					return FALSE;
				}
			}
			m_nPrePort = uport;
		}
	}
	PrintMessage(emMsgType_Success, _T("视觉服务器启动成功！"));
	return TRUE;
}

BOOL CFormWorkControl::StopServer()
{
	if (NULL != m_pServer)
	{
		m_pServer->Close();
		delete m_pServer;
		m_pServer = NULL;
	}
	return true;
}
BOOL CFormWorkControl::StartFinsClientPLC(CString strIP, int port) 
{
	if (FINSCLIENT->ConnServer(strIP, port))
	{
		PrintMessage(emMsgType_Success, _T("Fins客户端连接成功！"));
		m_nPreVisionPort = port;
		m_nPreFinsIp = strIP;
		return true;
	}
	else
	{
		PrintMessage(emMsgType_Error, _T("Fins客户端连接失败！"));
		return false;
	}
}
BOOL CFormWorkControl::StartServerPLC(unsigned short uport)
{
	if (NULL != m_pServerPLC)
	{
		//PrintMessage(emMsgType_Warn, _T("服务器重新启动！"));
		delete m_pServerPLC;
		m_pServerPLC = NULL;
	}
	m_pServerPLC = new CFactoryServer(this);
	if (INVALID_SOCKET == m_pServerPLC->m_hSocket)
	{
		if (!m_pServerPLC->Create(uport, SOCK_STREAM, FD_ACCEPT | FD_CLOSE))
		{
			PrintMessage(emMsgType_Error, _T("PLC服务器启动失败！"));
			m_pServerPLC->Close();
			delete m_pServerPLC;
			m_pServerPLC = NULL;
			return FALSE;
		}
		else
		{
			if (!m_pServerPLC->Listen(1))
			{
				int nErrorCode = m_pServerPLC->GetLastError();
				if (nErrorCode != WSAEWOULDBLOCK)
				{
					PrintMessage(emMsgType_Error, _T("PLC服务器启动失败！"));
					m_pServerPLC->Close();
					delete m_pServerPLC;
					m_pServerPLC = NULL;
					return FALSE;
				}
			}
			m_nPrePortPLC = uport;
		}
	}
	PrintMessage(emMsgType_Success, _T("PLC服务器启动成功！"));
	return TRUE;
}

BOOL CFormWorkControl::StopServerPLC()
{
	if (NULL != m_pServerPLC)
	{
		m_pServerPLC->Close();
		delete m_pServerPLC;
		m_pServerPLC = NULL;
	}
	return true;
}

BOOL CFormWorkControl::StartVisionCheckServer(unsigned short uport)
{
	if (NULL != m_pVisionCheckServer)
	{
		delete m_pVisionCheckServer;
		m_pVisionCheckServer = NULL;
	}
	m_pVisionCheckServer = new CVisionCheckServer(this);
	//m_pVisionCheckServer = new CFactoryServer(this);
	if (INVALID_SOCKET == m_pVisionCheckServer->m_hSocket)
	{
		if (!m_pVisionCheckServer->Create(uport, SOCK_STREAM, FD_ACCEPT | FD_CLOSE))
		{
			PrintMessage(emMsgType_Error, _T("视觉检测服务器启动失败！"));
			m_pVisionCheckServer->Close();
			delete m_pVisionCheckServer;
			m_pVisionCheckServer = NULL;
			return FALSE;
		}
		else
		{
			if (!m_pVisionCheckServer->Listen(1))
			{
				int nErrorCode = m_pVisionCheckServer->GetLastError();
				if (nErrorCode != WSAEWOULDBLOCK)
				{
					PrintMessage(emMsgType_Error, _T("视觉检测服务器启动失败！"));
					m_pVisionCheckServer->Close();
					delete m_pVisionCheckServer;
					m_pVisionCheckServer = NULL;
					return FALSE;
				}
			}
			m_nPreVisionPort = uport;
		}
	}
	PrintMessage(emMsgType_Success, _T("视觉检测服务器启动成功！"));
	return TRUE;
}

BOOL CFormWorkControl::StopVisionCheckServer()
{
	if (NULL != m_pVisionCheckServer)
	{
		m_pVisionCheckServer->Close();
		delete m_pVisionCheckServer;
		m_pVisionCheckServer = NULL;
	}
	return true;
}

void CFormWorkControl::SwitchWorkMode(int nMode)
{
	if (emWorkMode_Manual == nMode)
	{
		GetDlgItem(IDC_EDIT_MARKFILE)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_MARKSTAMP)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_MARKSTAMP)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_OPERID)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_OPERID)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_MARKTYPE)->ShowWindow(FALSE);
		//GetDlgItem(IDC_CHECK_TRY_DUMMY)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_LOTID)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_LOTID)->ShowWindow(FALSE);

		GetDlgItem(IDC_BUTTON_STARTLOT)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_EDIT_TEXT_CONTONT)->ShowWindow(TRUE);
		GetDlgItem(IDC_BUTTON_FINISH_EDIT_CONTENT)->ShowWindow(TRUE);
	}
	else if (emWorkMode_Auto == nMode)    
	{
		GetDlgItem(IDC_EDIT_MARKFILE)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_MARKSTAMP)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_MARKSTAMP)->ShowWindow(FALSE);
		//GetDlgItem(IDC_STATIC_OPERID)->ShowWindow(TRUE);
		//GetDlgItem(IDC_EDIT_OPERID)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_MARKTYPE)->ShowWindow(FALSE);
		//GetDlgItem(IDC_CHECK_TRY_DUMMY)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_LOTID)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_LOTID)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_LOTID)->EnableWindow(TRUE);      //lrf  页面切换时的BUG

		GetDlgItem(IDC_BUTTON_STARTLOT)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_EDIT_TEXT_CONTONT)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_FINISH_EDIT_CONTENT)->ShowWindow(FALSE);
	}
	else if (emWorkMode_Scan == nMode)
	{
		GetDlgItem(IDC_EDIT_MARKFILE)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_MARKSTAMP)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_MARKSTAMP)->ShowWindow(FALSE);
		//GetDlgItem(IDC_CHECK_TRY_DUMMY)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_LOTID)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_LOTID)->ShowWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STARTLOT)->EnableWindow(TRUE);

		if (m_pConfig->m_bCheckOrderStatus)
		{
			//GetDlgItem(IDC_STATIC_OPERID)->ShowWindow(TRUE);
			//GetDlgItem(IDC_EDIT_OPERID)->ShowWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_STATIC_OPERID)->ShowWindow(FALSE);
			GetDlgItem(IDC_EDIT_OPERID)->ShowWindow(FALSE);
		}

		GetDlgItem(IDC_BUTTON_EDIT_TEXT_CONTONT)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_FINISH_EDIT_CONTENT)->ShowWindow(FALSE);
	}
	else if (emWorkMode_Ponum == nMode)
	{
		GetDlgItem(IDC_EDIT_MARKFILE)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_MARKSTAMP)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_MARKSTAMP)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_OPERID)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_OPERID)->ShowWindow(FALSE);
		//GetDlgItem(IDC_CHECK_TRY_DUMMY)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_LOTID)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_LOTID)->ShowWindow(FALSE);

		GetDlgItem(IDC_BUTTON_STARTLOT)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_EDIT_TEXT_CONTONT)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_FINISH_EDIT_CONTENT)->ShowWindow(FALSE);
	}

	// 切换模式，所有功能按键变灰
	GetDlgItem(IDC_BUTTON_MAP_ADJUST)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LOAD_SET)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LASER_PARAM)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOPLOT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LOAD_JOB)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CANCELJOB)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STARTMARK)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOPMARK)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_EDIT_TEXT_CONTONT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_FINISH_EDIT_CONTENT)->EnableWindow(FALSE);

}

void CFormWorkControl::ManualLoadFile()
{
	if (IsMarking())
		return;

	CString s;
	CString sFileExtName = _T("IC标记文件(*.twfb)|*.twfb||");
	CFileDialog fd(true, NULL, NULL, OFN_HIDEREADONLY, sFileExtName);
	if (fd.DoModal() == IDOK)
	{
		// 清空状态
		ResetStatus();

		// 清空消息框
		m_pMainDlg->ClearMessage();

		CString sFileName = fd.GetPathName();
		if (sFileName.Right(5) != _T(".twfb"))
			sFileName = sFileName + _T(".twfb");

		m_sMarkFileName = fd.GetFileName();
		// 手动加载时，印章图档名称和阵列图档名称一样
		m_sMarkStampNameNonSuffix =  m_sMarkFileName.Left(m_sMarkFileName.GetLength() - 5);  
		s = m_sMarkFileName;
		s.Replace(_T(".twfb"), _T(""));  // 取消后缀名
		SetDlgItemText(IDC_EDIT_MARKFILE, s);
		SetStaticText(IDC_STATIC_FILE_NAME, s);
		// 开始一个新的LOG
		StartLog(s);
		PrintMessage(emMsgType_Title, s);

		// 加载文档
		if (LoadContent(sFileName))
		{
			// 更新消息
			UpdateShowView();

			s.Format(_T("手动载入%s文件成功！"), sFileName);
			PrintMessage(emMsgType_Success, s);

			// 发送制程给PLC软件
			if (!SwitchVisionProcess())
			{
				//PrintMessage(emMsgType_Error, _T("制程切换失败！"));
				ResetStatus();
				SetDlgItemText(IDC_EDIT_MARKFILE, _T(""));
				GetDlgItem(IDC_EDIT_MARKFILE)->SetFocus();
				s.Format(_T("手动载入%s文件失败！"), sFileName);
				PrintMessage(emMsgType_Error, s);
				return;
			}

			// 调试模式下，加载图档后，进入取消作业模式
			OnBnClickedButtonCanceljob();

			WritePrivateProfileString(_T("MarkFile"), sFileName, _T("1"), g_sTempConfigPath);
			m_bSaveOriginDoc = TRUE;
			m_bCCDOffseted = FALSE;
			m_bFirstLoadDocForDummy = true;
		}
		else
		{
			ResetStatus();
			SetDlgItemText(IDC_EDIT_MARKFILE, _T(""));
			GetDlgItem(IDC_EDIT_MARKFILE)->SetFocus();
			s.Format(_T("手动载入%s文件失败！"), sFileName);
			PrintMessage(emMsgType_Error, s);
		}
	}
}		

BOOL CFormWorkControl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (WM_KEYDOWN == pMsg->message && VK_ESCAPE == pMsg->wParam)
		return TRUE;

	// 调试模式下，文件编辑框使用点击加载文档
	if (pMsg->message == WM_LBUTTONDOWN)
	{
		int buID = GetWindowLong(pMsg->hwnd, GWL_ID);
		if (buID == IDC_EDIT_MARKFILE)
		{
			ManualLoadFile();
			GetDlgItem(IDC_EDIT_MARKFILE)->PostMessage(WM_CANCELMODE, 0, 0);
			return TRUE;
		}
	}

	if (pMsg->wParam == VK_F8)
	{
		if (GetDlgItem(IDC_BUTTON_LOAD_JOB)->IsWindowEnabled())
		{
			//OnBnClickedButtonStartlot();
		}
	}
	else if (pMsg->wParam == VK_F10)
	{
		//if (m_wndToolBar.GetToolBarCtrl().IsButtonEnabled(IDC_FACTORY_STOPLOT))
		{
			//OnBnClickedButtonStoplot();
		}
	}
	else if (pMsg->wParam == VK_F9)   // 开始mark
	{
		//if (m_wndToolBar.GetToolBarCtrl().IsButtonEnabled(IDC_FACTORY_START_MARK))
		{
			//OnBnClickedButtonStartmark();
		}
	}
	else if (pMsg->wParam == VK_F11)  // 停止mark
	{
		//if (m_wndToolBar.GetToolBarCtrl().IsButtonEnabled(IDC_FACTORY_STOP_MARK))
		{
			//OnBnClickedButtonStopmark();
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
void CFormWorkControl::AdjustControl()
{
	// 整个界面分成左-右结构
	int nW = m_rcClient.Width();  // 整个宽度
	int nH = m_rcClient.Height(); // 整个高度
	int L = m_rcClient.left, T = m_rcClient.top, R = m_rcClient.right, B = m_rcClient.bottom;
	int nf = 5;   // 固定间距
	int nLW = 320;   // 左边宽320
	int nLH = nH;  // 左边高度
	int nRW = nW - nLW;  //  剩下的宽度是右边
	int nRH = nH;  // 右边高度
	// 左边
	int nTabH = 20;  // Tab高度
	int nLf = 5;
	int nLUH = 80;   // 左边上半部分高度
	int nLaserPH = 120; // 激光参数显示区域高度
	int nLDH = nLH - nTabH - nLUH - nLaserPH;
	m_LeftFrame.MoveWindow(nLf, nLf, nLW -5, nLH - nLf * 2);
	int nEditH = 30;
	GetDlgItem(IDC_STATIC_OPERID)->MoveWindow(15, 40, 50, 20);                       
	GetDlgItem(IDC_STATIC_MARKTYPE)->MoveWindow(15, 25, nLW - 30, 80);     
	GetDlgItem(IDC_STATIC_LOTID)->MoveWindow(15, 70, 50, 20);  
	GetDlgItem(IDC_EDIT_OPERID)->MoveWindow(60, 35, nLW - 80, nEditH);
	GetDlgItem(IDC_EDIT_LOTID)->MoveWindow(60, 65, nLW - 80, nEditH); 
	GetDlgItem(IDC_EDIT_MARKFILE)->MoveWindow(15, 45, nLW - 20, 35);
	GetDlgItem(IDC_STATIC_MARKSTAMP)->MoveWindow(10, 40, 50, 20);
	GetDlgItem(IDC_EDIT_MARKSTAMP)->MoveWindow(60, 35, nLW - 80, nEditH);	
	GetDlgItem(IDC_CHECK_TRY_DUMMY)->MoveWindow(15, 110, 100, 20);
	GetDlgItem(IDC_CHECK_FIRST_PRINT)->MoveWindow(130, 110, 350, 20);
	GetDlgItem(IDC_GROUP_TEXT)->MoveWindow(10, 140, nLW - 15, nLDH - 50);  
	int TextBtnW = (nLW - 90) * 0.5;
	GetDlgItem(IDC_BUTTON_EDIT_TEXT_CONTONT)->MoveWindow(25, 165, TextBtnW, 25);                       
	GetDlgItem(IDC_BUTTON_FINISH_EDIT_CONTENT)->MoveWindow(25 + TextBtnW + 25, 165, TextBtnW, 25);      
	int nTextW = 145;     
	int nAvgH = (nLDH - 100) * 0.1;
	int nHalfAvgH = nAvgH / 2;
	int nTextTop = 200;
	int nFontTextW = 85;  
	int nStaticW = 60;
	int nTableLeft = 15, nTextL = nStaticW + nTableLeft, nFontNameL = nTextL + nTextW + 5;    
	nEditH = nAvgH;
	if (nEditH > 30) nEditH = 30;
	int nLabelH = nAvgH - 5;

	GetDlgItem(IDC_STATIC_TEXT1)->MoveWindow(nTableLeft, nTextTop, nStaticW, nLabelH);
	GetDlgItem(IDC_EDIT_TEXT1)->MoveWindow(nTextL, nTextTop - 5, nTextW, nEditH);
	GetDlgItem(IDC_EDIT_NAME1)->MoveWindow(nFontNameL, nTextTop - 5, nFontTextW, nEditH);

	GetDlgItem(IDC_STATIC_TEXT2)->MoveWindow(nTableLeft, nTextTop + nAvgH*1, nStaticW, nLabelH);
	GetDlgItem(IDC_EDIT_TEXT2)->MoveWindow(nTextL, nTextTop - 5 + nAvgH*1, nTextW, nEditH);
	GetDlgItem(IDC_EDIT_NAME2)->MoveWindow(nFontNameL, nTextTop - 5 + nAvgH * 1, nFontTextW, nEditH);

	GetDlgItem(IDC_STATIC_TEXT3)->MoveWindow(nTableLeft, nTextTop + nAvgH * 2, nStaticW, nLabelH);
	GetDlgItem(IDC_EDIT_TEXT3)->MoveWindow(nTextL, nTextTop - 5 + nAvgH * 2, nTextW, nEditH);
	GetDlgItem(IDC_EDIT_NAME3)->MoveWindow(nFontNameL, nTextTop - 5 + nAvgH * 2, nFontTextW, nEditH);

	GetDlgItem(IDC_STATIC_TEXT4)->MoveWindow(nTableLeft, nTextTop + nAvgH * 3, nStaticW, nLabelH);
	GetDlgItem(IDC_EDIT_TEXT4)->MoveWindow(nTextL, nTextTop - 5 + nAvgH * 3, nTextW, nEditH);
	GetDlgItem(IDC_EDIT_NAME4)->MoveWindow(nFontNameL, nTextTop - 5 + nAvgH * 3, nFontTextW, nEditH);

	GetDlgItem(IDC_STATIC_TEXT5)->MoveWindow(nTableLeft, nTextTop + nAvgH * 4, nStaticW, nLabelH);
	GetDlgItem(IDC_EDIT_TEXT5)->MoveWindow(nTextL, nTextTop - 5 + nAvgH * 4, nTextW, nEditH);
	GetDlgItem(IDC_EDIT_NAME5)->MoveWindow(nFontNameL, nTextTop - 5 + nAvgH * 4, nFontTextW, nEditH);

	GetDlgItem(IDC_STATIC_TEXT6)->MoveWindow(nTableLeft, nTextTop + nAvgH * 5, nStaticW, nLabelH);
	GetDlgItem(IDC_EDIT_TEXT6)->MoveWindow(nTextL, nTextTop - 5 + nAvgH * 5, nTextW, nEditH);
	GetDlgItem(IDC_EDIT_NAME6)->MoveWindow(nFontNameL, nTextTop - 5 + nAvgH * 5, nFontTextW, nEditH);

	GetDlgItem(IDC_STATIC_TEXT7)->MoveWindow(nTableLeft, nTextTop + nAvgH * 6, nStaticW, nLabelH);
	GetDlgItem(IDC_EDIT_TEXT7)->MoveWindow(nTextL, nTextTop - 5 + nAvgH * 6, nTextW, nEditH);
	GetDlgItem(IDC_EDIT_NAME7)->MoveWindow(nFontNameL, nTextTop - 5 + nAvgH * 6, nFontTextW, nEditH);

	GetDlgItem(IDC_STATIC_TEXT8)->MoveWindow(nTableLeft, nTextTop + nAvgH *7, nStaticW, nLabelH);
	GetDlgItem(IDC_EDIT_TEXT8)->MoveWindow(nTextL, nTextTop - 5 + nAvgH * 7, nTextW, nEditH);
	GetDlgItem(IDC_EDIT_NAME8)->MoveWindow(nFontNameL, nTextTop - 5 + nAvgH * 7, nFontTextW, nEditH);

	GetDlgItem(IDC_STATIC_TEXT9)->MoveWindow(nTableLeft, nTextTop + nAvgH * 8, nStaticW, nLabelH);
	GetDlgItem(IDC_EDIT_TEXT9)->MoveWindow(nTextL, nTextTop - 5 + nAvgH * 8, nTextW, nEditH);
	GetDlgItem(IDC_EDIT_NAME9)->MoveWindow(nFontNameL, nTextTop - 5 + nAvgH * 8, nFontTextW, nEditH);

	GetDlgItem(IDC_STATIC_TEXT10)->MoveWindow(nTableLeft, nTextTop + nAvgH * 9, nStaticW, nLabelH);
	GetDlgItem(IDC_EDIT_TEXT10)->MoveWindow(nTextL, nTextTop - 5 + nAvgH * 9, nTextW, nEditH);
	GetDlgItem(IDC_EDIT_NAME10)->MoveWindow(nFontNameL, nTextTop - 5 + nAvgH * 9, nFontTextW, nEditH);


	// 激光参数区域显示
	CRect rcWindowGroup;
	GetDlgItem(IDC_GROUP_TEXT)->GetWindowRect(rcWindowGroup);
	ScreenToClient(rcWindowGroup);

	int iLaserParamTop = rcWindowGroup.bottom + 2;
	//GetDlgItem(IDC_LIST_LASER_PARAM)->MoveWindow(10, iLaserParamTop, 120, nLaserPH - nf);
	//GetDlgItem(IDC_STATIC_LASER_PARAM)->MoveWindow(135, iLaserParamTop, nLW - 140, nLaserPH - nf);
	GetDlgItem(IDC_LIST_LASER_PARAM)->MoveWindow(10, iLaserParamTop, rcWindowGroup.Width(), nLaserPH - 70);
	GetDlgItem(IDC_STATIC_LASER_PARAM)->MoveWindow(10, iLaserParamTop + nLaserPH - 70, rcWindowGroup.Width(), 70);

	// 中间
	int nBtnH = 35;
	int nBtnW = 110;
	int nMidWidth = nRW - nBtnW - nf * 3;
	int nRollTipH = 20;
	//status相关
	int nMidStatusH = 40;  //状态栏总宽度
	int nStatusH = 23;     //状态信号图标控件高度
	int nStatusW = nBtnW * 3 /5;  //状态信号图标控件宽度 （取控件大小的1/3）
	int nStatusS = nStatusW + 10;     //状态信号之间的间距

	GetDlgItem(IDC_STATIC_TIPS)->MoveWindow(nLW + nf, nf, nRW - nf * 2, nRollTipH);
	GetDlgItem(IDC_STATIC_MAINVIEW)->MoveWindow(nLW + nf, nRollTipH + nf*2, nMidWidth, nRH - nRollTipH - nMidStatusH -  nf*3);
	int nMarkFileW = nMidWidth - nf * 2;
	GetDlgItem(IDC_STATIC_FILE_NAME)->MoveWindow(R - nBtnW - nf * 3 - nMarkFileW, nRollTipH + nf * 3, nMarkFileW, 20);
	CRect rcMainView;
	GetDlgItem(IDC_STATIC_MAINVIEW)->GetClientRect(rcMainView);
	int nTotalViewH = rcMainView.Height() - nRollTipH - nf;
	int nBinViewW = nMidWidth - nf * 2;
	int nBinViewH = nTotalViewH * 12 / 16;
	int nMapViewH = nTotalViewH - nBinViewH - nf;
	GetDlgItem(IDC_STATIC_VIEWFRAME_STAMP)->MoveWindow(nLW + nf * 2, 27 + nf * 2 + nRollTipH, nBinViewW, nBinViewH);
	GetDlgItem(IDC_STATIC_VIEWFRAME)->MoveWindow(nLW + nf * 2, 27 + nf * 2 + nRollTipH + nBinViewH, nBinViewW, nMapViewH);

	GetDlgItem(IDC_STATIC_VISION_STATUS)->MoveWindow(nLW + nf * 2, rcMainView.bottom + nMidStatusH, nStatusW, nStatusH);	
	GetDlgItem(IDC_STATIC_PWS_LOAD_STATUS)->MoveWindow(nLW + nf * 2 + nStatusS * 1, rcMainView.bottom + nMidStatusH, nStatusW, nStatusH);
	GetDlgItem(IDC_STATIC_MAPPING_STATUS)->MoveWindow(nLW + nf * 2 + nStatusS * 2, rcMainView.bottom + nMidStatusH, nStatusW, nStatusH);
	GetDlgItem(IDC_STATIC_2DBARCODE_STATUS)->MoveWindow(nLW + nf * 2 + nStatusS * 3, rcMainView.bottom + nMidStatusH, nStatusW, nStatusH);
	GetDlgItem(IDC_STATIC_STARTMARK_STATUS)->MoveWindow(nLW + nf * 2 + nStatusS * 4, rcMainView.bottom + nMidStatusH, nStatusW + 20, nStatusH);
	GetDlgItem(IDC_STATIC_2DBARCODE_BEGIN_STATUS)->MoveWindow(nLW + nf * 2 + nStatusS * 5 + 20, rcMainView.bottom + nMidStatusH, nStatusW + 20, nStatusH);
	GetDlgItem(IDC_STATIC_NET)->MoveWindow(nLW + nf * 2 + nStatusS * 6+40 , rcMainView.bottom + nMidStatusH, nStatusW + 20, nStatusH);
	
	int nLaserTimeW = rcMainView.right - (nf * 2 + nStatusS * 6 + 40);
	GetDlgItem(IDC_LASER_TIME)->MoveWindow(nLW + nf * 2 + nStatusS * 6 + 40, rcMainView.bottom + nMidStatusH, nLaserTimeW, nStatusH);

	// 右边
	int iRowPitch = (nRH - nBtnH * 8 - nRollTipH - 55 - nf * 7) / 7;
	int nBtnT = T + nRollTipH + 20 + nf * 3;
	int nBtnL = nW - nf - nBtnW;
	GetDlgItem(IDC_BUTTON_MAP_ADJUST)->MoveWindow(nBtnL, nBtnT, nBtnW, nBtnH);
	GetDlgItem(IDC_BUTTON_LOAD_SET)->MoveWindow(nBtnL, nBtnT + nBtnH + iRowPitch, nBtnW, nBtnH);
	GetDlgItem(IDC_BUTTON_LASER_PARAM)->MoveWindow(nBtnL, nBtnT + (nBtnH + iRowPitch) * 2, nBtnW, nBtnH);
	GetDlgItem(IDC_BUTTON_RESET)->MoveWindow(nBtnL, nBtnT + (nBtnH + iRowPitch) * 3, nBtnW, nBtnH);
	//GetDlgItem(IDC_BUTTON_STARTLOT)->MoveWindow(nBtnL, nBtnT + (nBtnH + iRowPitch) * 4, nBtnW, nBtnH);  //lrf 华天无开批结批
	//GetDlgItem(IDC_BUTTON_STOPLOT)->MoveWindow(nBtnL, nBtnT + (nBtnH + iRowPitch) * 5, nBtnW, nBtnH);
	GetDlgItem(IDC_BUTTON_LOAD_JOB)->MoveWindow(nBtnL, nBtnT + (nBtnH + iRowPitch) * 4, nBtnW, nBtnH);
	GetDlgItem(IDC_BUTTON_CANCELJOB)->MoveWindow(nBtnL, nBtnT + (nBtnH + iRowPitch) * 5, nBtnW, nBtnH);
	GetDlgItem(IDC_BUTTON_STARTMARK)->MoveWindow(nBtnL, nBtnT + (nBtnH + iRowPitch) * 6, nBtnW, nBtnH);
	GetDlgItem(IDC_BUTTON_STOPMARK)->MoveWindow(nBtnL, nBtnT + (nBtnH + iRowPitch) * 7, nBtnW, nBtnH);
}

void CFormWorkControl::OnBnClickedButtonMapAdjust()
{
	if (!RightConfirmBox(USER_O_ADJUST_POS))
		return;

	// 正在标记和非调试模式，都不能调整印章位置
	if (IsMarking())
		return;

	// 没有阵列图，返回
	if (NULL == m_pWaferDoc || NULL == m_pMapDoc)
		return;

	CWaferGroup * pWaferGroup = m_pMapDoc->GetGroup(0);
	if (NULL == pWaferGroup)
		return;

	if (m_bCCDOffseted) {
		//AfxMessageBox(_T("图档已经变化，不能修改位置！"));
		// 如果CCD修改阵列位置后没有恢复位置，则在此处恢复
		if (!ResetOriginDoc())
			return;

		m_bCCDOffseted = FALSE;
	}

	bool bAdjust = false;
	CDlgMapAdjust dlg(m_pMapDoc, m_sMarkFileName, m_sMobFilePath, bAdjust, this);
	dlg.DoModal();

	if (bAdjust)   // 避免没有做任何修改却显示“"阵列位置调整成功”
	{
		m_bSaveOriginDoc = TRUE;
		if (m_LoginUserName != _T("")) {
			PrintMessage(emMsgType_Success, _T("用户")+m_LoginUserName + _T(" 阵列位置调整成功！"));
		} else {
			PrintMessage(emMsgType_Success, _T("阵列位置调整成功！"));
		}
	}
	m_pMarkContentView->Invalidate();

	// 自动LoadJob
	if (emWorkMode_Manual != m_pConfig->m_nWorkMode) {
		OnBnClickedButtonLoadJob();
	}
}


void CFormWorkControl::OnBnClickedButtonLoadSet()
{
	if (!RightConfirmBox(USER_O_LOAD_MOB_SET))
		return;

	if (IsMarking())
		return;

	CString sFileExtName;
	sFileExtName.Format(_T("阵列修正文件(*%s)|*%s||"), g_sMapOffsetfile, g_sMapOffsetfile);
	CFileDialog fd(true, NULL, NULL, OFN_HIDEREADONLY, sFileExtName);
	if (fd.DoModal() == IDOK)
	{
		CString s, sNewFilePath; 
		CTime currentTime = CTime::GetCurrentTime();
		CString timeYmdHMS = currentTime.Format(_T("_%Y_%m_%d_%H%M%S"));

		CString sFileName = fd.GetPathName();
		if (sFileName.Right(4) != g_sMapOffsetfile)
			sFileName = sFileName + g_sMapOffsetfile;

		if (sFileName == m_sMobFilePath)
		{
			PrintMessage(emMsgType_Warn, _T("载入文件与当前加载文件一样，阵列位置不调整！"));
			goto TestFinish;
		}

		if (m_bCCDOffseted) {
			//AfxMessageBox(_T("图档已经变化，不能修改位置！"));
			// 如果CCD修改阵列位置后没有恢复位置，则在此处恢复
			if (!ResetOriginDoc())
				return;

			m_bCCDOffseted = FALSE;
		}

		// 根据当前修正文件，复位阵列位置
		if (!ResetMapOffsetFile())
		{
			PrintMessage(emMsgType_Error, _T("阵列位置设置失败！"));
			goto TestFinish;
		}

		// 加载选中的文件
		if (!LoadMapOffsetFile(sFileName))
		{
			AfxMessageBox(_T("阵列位置设置失败！"));
			PrintMessage(emMsgType_Error, _T("阵列位置设置失败！"));
			goto TestFinish;
		}

		// 通过加载的文件重新生成一个偏移文件
		CString sNewMobFilePath = m_pConfig->m_sMobFilePath + _T("\\") + m_sMarkFileName + timeYmdHMS + g_sMapOffsetfile;

		if (!CopyMobFile(sFileName, sNewMobFilePath)) {
			AfxMessageBox(_T("阵列位置设置失败！"));
			PrintMessage(emMsgType_Error, _T("阵列位置设置失败：文件复制失败！"));
			return;
		}
		// 删除三个文件以外的时间最旧的文件
		DeleteOldMobFile();

		m_bSaveOriginDoc = TRUE;

		// 更新阵列修改文件名称
		m_sMobFilePath = sNewMobFilePath;

		if (m_LoginUserName != _T("")) {
			s.Format(_T("用户%s 载入阵列位置文件[%s]"), m_LoginUserName, sFileName);
		} else {
			s.Format(_T("载入阵列位置文件[%s]"), sFileName);
		}
		PrintMessage(emMsgType_Normal, s);
	}

TestFinish:
	// 自动LoadJob
	if (emWorkMode_Manual != m_pConfig->m_nWorkMode) {
		OnBnClickedButtonLoadJob();
	}
}


void CFormWorkControl::OnBnClickedButtonReset()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!RightConfirmBox(USER_O_RESET_CONTENT))
		return;
	
	if (IsMarking())
	{
		AfxMessageBox(_T("正在标记中，不能重置！"));
		return;
	}

	if (IDYES != MessageBox(_T("确认是否重置！"), _T("提示"), MB_YESNO | MB_DEFBUTTON2))
		return;

	if (CheckMesInfo())
	{
		m_nMapOKCount = 0;
		m_nMapNGCount = 0;
		PrintMessage(emMsgType_Normal, _T("MES信息检查通过!"));
	}
	else
	{

		PrintMessage(emMsgType_Error, _T("MES信息检查失败!"));
	}
	
	// 状态清空
	ResetStatus();
	
	// 取消JOB
	OnBnClickedButtonCanceljob();
	GetDlgItem(IDC_BUTTON_LOAD_JOB)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LOAD_SET)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LASER_PARAM)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_MAP_ADJUST)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STARTMARK)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_EDIT_TEXT_CONTONT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_FINISH_EDIT_CONTENT)->EnableWindow(FALSE);

	if (emWorkMode_Manual == m_pConfig->m_nWorkMode)
	{
		SetDlgItemText(IDC_EDIT_MARKFILE, _T(""));
		GetDlgItem(IDC_EDIT_MARKFILE)->SetFocus();
	}
	else if (emWorkMode_Scan == m_pConfig->m_nWorkMode)
	{
		GetDlgItem(IDC_EDIT_OPERID)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LOTID)->EnableWindow(TRUE);
		SetDlgItemText(IDC_EDIT_LOTID, _T(""));
		GetDlgItem(IDC_EDIT_LOTID)->SetFocus();
	}
	else if (emWorkMode_Auto == m_pConfig->m_nWorkMode)
	{
		GetDlgItem(IDC_STATIC_MARKTYPE)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_OPERID)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LOTID)->EnableWindow(TRUE);
		SetDlgItemText(IDC_EDIT_LOTID, _T(""));
		GetDlgItem(IDC_EDIT_LOTID)->SetFocus();
	}
	else if (emWorkMode_Ponum == m_pConfig->m_nWorkMode)
	{
		SetDlgItemText(IDC_EDIT_MARKSTAMP, _T(""));
		GetDlgItem(IDC_EDIT_MARKSTAMP)->EnableWindow();
		GetDlgItem(IDC_EDIT_MARKSTAMP)->SetFocus();
	}
	
	PrintMessage(emMsgType_Normal, _T("重置完成!"));
	
	// 信号复位
	UpdateIOStatus(UISIGNAL_MAP);
	UpdateIOStatus(UISIGNAL_2D);
	UpdateIOStatus(UISIGNAL_PWS);
	UpdateIOStatus(UISIGNAL_NET);
	//StartLog(_T("SystemSetting"));
}


void CFormWorkControl::OnBnClickedButtonCell()
{
	// TODO: 在此添加控件通知处理程序代码
	if (IsMarking())
		return;

	UpdateShowView();
}


void CFormWorkControl::OnBnClickedButtonMatrix()
{
	// TODO: 在此添加控件通知处理程序代码
	if (NULL == m_pWaferDoc || NULL == m_pMapDoc)
		return;

	if (IsMarking())
		return;

	m_pMarkContentView->SetShowContent(emShowContent_Map_Only, (void*)m_pMapDoc);
	m_pMarkContentView->Invalidate();
}


void CFormWorkControl::OnBnClickedButtonArea()
{
	// TODO: 在此添加控件通知处理程序代码
	if (NULL == m_pWaferDoc || NULL == m_pMapDoc)
		return;

	if (IsMarking())
		return;

	m_pMarkContentView->SetShowContent(emShowContent_Map, (void*)m_pMapDoc);
	m_pMarkContentView->Invalidate();
}


void CFormWorkControl::OnBnClickedButtonStartlot()
{
	GetDlgItem(IDC_BUTTON_STARTLOT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STARTLOT)->UpdateWindow();

	//if (m_nLotFlag) return;
	CWaferGroup * pWaferGroup = nullptr;
	CString sOperID, sLotID;
	GetDlgItemText(IDC_EDIT_OPERID, sOperID);
	GetDlgItemText(IDC_EDIT_LOTID, sLotID);
	if (sOperID.IsEmpty())
	{
		AfxMessageBox(_T("开批失败：工号为空！"));
		GetDlgItem(IDC_EDIT_OPERID)->SetFocus();
		goto TestError;
	}
	if (sLotID.IsEmpty())
	{
		AfxMessageBox(_T("开批失败：批次为空！"));
		GetDlgItem(IDC_EDIT_OPERID)->SetFocus();
		goto TestError;
	}

	if (NULL == m_pWaferDoc || NULL == m_pMapDoc)
	{
		PrintMessage(emMsgType_Error, _T("开批失败：没有发现打印图档！"));
		AfxMessageBox(_T("开批失败：没有发现打印图档！"));
		goto TestError;
	}
	pWaferGroup = m_pMapDoc->GetGroup(0);
	if (NULL == pWaferGroup)
	{
		PrintMessage(emMsgType_Error, _T("开批失败：没有发现打印图档！"));
		AfxMessageBox(_T("开批失败：没有发现打印图档！"));
		goto TestError;
	}

	if (!StartLot(sOperID, sLotID))
	{
		AfxMessageBox(_T("开批失败：通讯异常！"));
		goto TestError;
	}
		
	AfxMessageBox(_T("开批成功！"));

	GetDlgItem(IDC_BUTTON_STOPLOT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOPLOT)->SetFocus();
	m_nLotFlag = 1;   // 已经开批
	return;
TestError:
	m_nLotFlag = 0;
	GetDlgItem(IDC_BUTTON_STARTLOT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STARTLOT)->SetFocus();
}


void CFormWorkControl::OnBnClickedButtonStoplot()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON_STOPLOT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOPLOT)->UpdateWindow();

	CString s;
	char szSendbuff[1024];
	char sRet[1024] = { 0 };
	bool bRet = false;
	CString sOperID, sLotID;
	GetDlgItemText(IDC_EDIT_OPERID, sOperID);
	GetDlgItemText(IDC_EDIT_LOTID, sLotID);
	if (sOperID.IsEmpty())
	{
		AfxMessageBox(_T("工号为空！"));
		goto TestError;
	}
	if (sLotID.IsEmpty())
	{
		AfxMessageBox(_T("批次为空！"));
		goto TestError;
	}

	
	if (IsMarking())
	{
		AfxMessageBox(_T("正在标记中，不能结批！"));
		goto TestError;
	}

	if (IDYES != MessageBox(_T("确认是否结束批次！"), _T("提示"), MB_YESNO | MB_DEFBUTTON2))
		goto TestError;

	// 开始批次信息上传
	PrintMessage(emMsgType_Normal, _T("上传结束批次信息！"));
	memset(szSendbuff, 0, 1024);
	sprintf_s(szSendbuff, "ASSY MKBS3<LOT_ID,%s><OPER_ID,%s>", CStringA(sLotID).GetString(), CStringA(sOperID).GetString());
	s.Format(_T("传送指令：%s。等待返回..."), CString(szSendbuff));
	PrintMessage(emMsgType_Normal, s);
	memset(sRet, 0, 1024);
	bRet = m_pFactoryMesComm->SendCmd(szSendbuff, "ASSY MKBR", sRet, sizeof(sRet));
	s.Format(_T("接收内容：%s"), chU2T(sRet).c_str());
	PrintMessage(emMsgType_Normal, s);
	if (!bRet)
	{
		PrintMessage(emMsgType_Error, _T("结批失败：指令发送失败或未收到正确消息！"));
		goto TestError;
	}
	

	// 结束批次
	if (0 != doStopLot(sRet, strlen(sRet)))
		goto TestError;
	m_nLotFlag = 0;  // 开批结束

	// 清空状态
	ResetStatus();

	// 取消JOB
	OnBnClickedButtonCanceljob();
	GetDlgItem(IDC_BUTTON_LOAD_JOB)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LOAD_SET)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LASER_PARAM)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_MAP_ADJUST)->EnableWindow(FALSE);

	AfxMessageBox(_T("结批成功！"));
	GetDlgItem(IDC_BUTTON_STARTLOT)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_OPERID)->EnableWindow(TRUE);
	SetDlgItemText(IDC_EDIT_LOTID, _T(""));
	GetDlgItem(IDC_EDIT_LOTID)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_LOTID)->SetFocus();

	return;
TestError:
	GetDlgItem(IDC_BUTTON_STOPLOT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOPLOT)->SetFocus();
	return;
}


void CFormWorkControl::OnBnClickedButtonLoadJob()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!RightConfirmBox(USER_O_START_WORK))
		return;

	CheckTextSave();   // 加载作业前，检查是否已经保存文本对象变量

	LoadJob();

	m_pMainDlg->SetStaticText(IDS_MARK_STATUS, MARKSTATUS_LOADJOB);
	GetDlgItem(IDC_BUTTON_LOAD_JOB)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CANCELJOB)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_MAP_ADJUST)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LOAD_SET)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LASER_PARAM)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CANCELJOB)->SetFocus();

	GetDlgItem(IDC_BUTTON_STARTMARK)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_EDIT_TEXT_CONTONT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_FINISH_EDIT_CONTENT)->EnableWindow(FALSE);

	GetDlgItem(IDC_CHECK_TRY_DUMMY)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_FIRST_PRINT)->EnableWindow(FALSE);
}


void CFormWorkControl::OnBnClickedButtonCanceljob()
{
	// TODO: 在此添加控件通知处理程序代
	if (!RightConfirmBox(USER_O_STOP_WORK))
		return;

	// CancelJob
	CancelJob();

	//m_pMarkDriver->WriteExtPortPin(m_pConfig->m_nReadyPort, IO_OUT_INVALID);

	m_pMainDlg->SetStaticText(IDS_MARK_STATUS, MARKSTATUS_IDLE);
	GetDlgItem(IDC_BUTTON_CANCELJOB)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_MAP_ADJUST)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_LOAD_SET)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_LASER_PARAM)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_LOAD_JOB)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_LOAD_JOB)->SetFocus();

	GetDlgItem(IDC_BUTTON_STARTMARK)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_EDIT_TEXT_CONTONT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_FINISH_EDIT_CONTENT)->EnableWindow(FALSE);

	GetDlgItem(IDC_CHECK_TRY_DUMMY)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK_FIRST_PRINT)->EnableWindow(TRUE);
}

void CFormWorkControl::OnBnClickedButtonStartmark()
{

	if (!RightConfirmBox(USER_O_START_MARK))
		return;

	m_bManualStartMark = TRUE;
	
	CheckTextSave();   // 加载作业前，检查是否已经保存文本对象变量

	PrintMessage(emMsgType_Normal, _T("手动强制开始标记。"));
 
	// 开始标记
	StartMark(/*1*/);   // 传入参数1，忽略部分检查

	m_bManualStartMark = FALSE;
}


void CFormWorkControl::OnBnClickedButtonStopmark()
{
	if (!RightConfirmBox(USER_O_STOP_MARK))
		return;

	if (NULL == m_pWaferDoc || NULL == m_pMapDoc)
		return;

	if (IDYES != MessageBox(_T("确认是否结束标记！"), _T("提示"), MB_YESNO | MB_DEFBUTTON2))
		return;

	GetDlgItem(IDC_BUTTON_STOPMARK)->EnableWindow(FALSE);
	PrintMessage(emMsgType_Normal, _T("手动强制停止标记。"));

	CMarkPlatform::Instance()->MarkEngine()->StopMark();

	m_bManualStopMark = TRUE;

	//m_nMarkFlag = 0;  // 板卡完成传输

	// 完成标记
	//FinishMark();
}

void CFormWorkControl::OnOK()
{
	if (GetFocus() == GetDlgItem(IDC_EDIT_OPERID))
	{
		GetDlgItem(IDC_EDIT_LOTID)->SetFocus();   // 及时更新控件
	}
	else if (GetFocus() == GetDlgItem(IDC_EDIT_LOTID))
	{
		CString s, sErrMsg;
		// 清空消息框
		CHttpComHT com(this);
		float fness;
		m_pMainDlg->ClearMessage();

		// 清空激光参数显示
		ClearLaserParamShow();

		CString sOperID, sLotID;
		GetDlgItemText(IDC_EDIT_LOTID, sLotID);
		//GetDlgItemText(IDC_EDIT_OPERID, sOperID);
		if (sLotID.IsEmpty())
		{
			AfxMessageBox(_T("Lot ID为空！"));
			GetDlgItem(IDC_EDIT_LOTID)->SetFocus();
			return;
		}
		/*if (sOperID.IsEmpty())
		{
			AfxMessageBox(_T("Oper ID为空！"));
			GetDlgItem(IDC_EDIT_OPERID)->SetFocus();
			return;
		}*/

	    if (!IsIgnoreCCDCheckAndSwitchVisionProcess())
		{
			if (m_pConfig->m_bCheckVisualSystem)
			{
				if (!CheckCCDStatus()) {         //CCD未连接不能扫码
					s.Format(_T("视觉系统（CCD）未连接！"));
					PrintMessage(emMsgType_Error, s);
					goto TestError;
				}
			}
		}

		//扫入lotID检测与由小写转化为大写
		if (!GetAndCheckLotID(sLotID)) {
			goto TestError;
		}

		GetDlgItem(IDC_EDIT_LOTID)->SetWindowText(sLotID);
		// 判断扫描的LotID是不是已经在本机上扫描过
		int nLotIDTimes = VerifyRepeatLotID(sLotID);
		
		GetDlgItem(IDC_EDIT_OPERID)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LOTID)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_OPERID)->UpdateWindow();  // 及时更新控件
		GetDlgItem(IDC_EDIT_LOTID)->UpdateWindow();   // 及时更新控件

		// 根据LotID创建一个新的Log文件
		StartLog(sLotID);  

		s.Format(_T("%s"), sLotID);
		PrintMessage(emMsgType_Title, s);
		//s.Format(_T("扫入Lot No:%s。Oper ID:%s"), sLotID, sOperID);
		s.Format(_T("扫入LotID：%s"), sLotID);
		PrintMessage(emMsgType_Normal, s);

		// 加载印章
		if(!LoadSeal(sLotID))
			goto TestError;

		// 发送制程给PLC软件
		if (!SwitchVisionProcess())
		{
			//PrintMessage(emMsgType_Error, _T("制程切换失败！"));
			goto TestError;
		}
		//写入欧姆龙PLC晶圆厚度
		if (m_pConfig->m_bCheckFinsPLC) {

			if (com.GetWaferThickness(sLotID))
			{
				fness = com.ReturnThickNess();
				CString str;
				str.Format(L"成功获取塑封体厚度：%f", fness);
				PrintMessage(emMsgType_Normal, str);
				if (FINSCLIENT->ConnServer(m_pConfig->m_sFinsIP, m_pConfig->m_iFinsPort))
				{
					FINSCLIENT->WriteShort("DM5941.1", 1);
					PrintMessage(emMsgType_Normal, _T("清空PLC厚度数据！"));
					int iFness = fness * 1000;
					unsigned short iValue = 0;
					CString str;
					str.Format(L"Fins写入塑封体厚度:%d成功", iValue);
					(FINSCLIENT->WriteShort("DM5941.1", iFness)) == true ? ((FINSCLIENT->ReadShort("DM5941", iValue) == true) ? (PrintMessage(emMsgType_Success, L"Fins写入晶圆厚度成功！")) : (PrintMessage(emMsgType_Error, L"Fins写入晶圆厚度失败！"))) : (PrintMessage(emMsgType_Error, L"Fins写入晶圆厚度失败！"));
				}
				else
				{
					PrintMessage(emMsgType_Error, _T("FincTCP连接失败！"));
					goto TestError;
				}
			}
			else
			{
				PrintMessage(emMsgType_Error, _T("获取塑封体厚度失败！"));
				goto TestError;
			}
		}
	
		// 单独生成参数文件，以供客户调取
		if (!SaveLaserParamFile(sLotID))
		{
			PrintMessage(emMsgType_Error, _T("激光参数记录文件生成失败！"));
			goto TestError;
		}

		// add by hhhuang 20260623 判断产品类型是否符合要求
		if (m_pConfig->m_bCheckProductType) {
			if (!com.CheckProductType(sLotID, m_markType, sErrMsg)) {

				PrintMessage(emMsgType_Error, _T("产品类型对比失败:") + sErrMsg);
				goto TestError;
			}
		}

		// 自动开始JOB
		m_bScanBarcode = TRUE;
		m_bSaveOriginDoc = TRUE;
		m_bCCDOffseted = FALSE;
		OnBnClickedButtonLoadJob();
		
		// 将LotID保存到配置档中
		s.Format(_T("%d"), ++nLotIDTimes);
		WritePrivateProfileString(_T("LotID"), sLotID, s, _T("D:\\HT LotID Record\\") + CTime::GetCurrentTime().Format(_T("%Y%m")) + _T(".ini"));

		// 焦点移到开批按钮上
		GetDlgItem(IDC_BUTTON_STARTLOT)->SetFocus();
		
		//更新Mapping状态
		UpdateIOStatus(UISIGNAL_MAP);
		UpdateIOStatus(UISIGNAL_2D);
		UpdateIOStatus(UISIGNAL_PWS);
		UpdateIOStatus(UISIGNAL_NET);
		// 更新图档类型
		GetDlgItem(IDC_STATIC_MARKTYPE)->ShowWindow(TRUE);
		s.Format(_T("Type: %s"), m_markType);
		SetDlgItemText(IDC_STATIC_MARKTYPE, s);

		m_bFirstLoadDocForDummy = true;
		
		return;

	TestError:
		ResetStatus();

		GetDlgItem(IDC_EDIT_OPERID)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LOTID)->EnableWindow(TRUE);
		SetDlgItemText(IDC_EDIT_LOTID, _T(""));
		GetDlgItem(IDC_EDIT_LOTID)->SetFocus();
	}
	else if (GetFocus() == GetDlgItem(IDC_EDIT_MARKSTAMP))
	{
		// 清空消息框
		m_pMainDlg->ClearMessage();

		CString sMarkStampName;
		GetDlgItemText(IDC_EDIT_MARKSTAMP, sMarkStampName);
		if (sMarkStampName.IsEmpty())
		{
			AfxMessageBox(_T("组装批号为空！"));
			GetDlgItem(IDC_EDIT_LOTID)->SetFocus();
			return;
		}

		GetDlgItem(IDC_EDIT_MARKSTAMP)->EnableWindow(FALSE);
		
		// 根据LotID创建一个新的Log文件
		StartLog(sMarkStampName);

		// 开始获取图章信息
		CString s;
		s.Format(_T("%s"), sMarkStampName);
		PrintMessage(emMsgType_Title, s);
		s.Format(_T("扫入组装批号：%s"), sMarkStampName);
		PrintMessage(emMsgType_Normal, s);

		// 加载文档
		if (LoadStampByPonum(sMarkStampName))
		{
			// 自动开始JOB
			m_bScanBarcode = TRUE;
			OnBnClickedButtonLoadJob();
		}
		else
		{
			ResetStatus();
			SetDlgItemText(IDC_EDIT_MARKSTAMP, _T(""));
			GetDlgItem(IDC_EDIT_MARKSTAMP)->SetFocus();
			GetDlgItem(IDC_EDIT_MARKSTAMP)->EnableWindow(TRUE);
		}
	}
}

BOOL CFormWorkControl::EndLog()
{
	if (TRUE == m_bOpenFile)
		m_LogStdioFile.Close();
	m_bOpenFile = FALSE;
	return TRUE;
}

void CFormWorkControl::ResetStatus()
{
	// 清空信息显示框内容
	//m_pMainDlg->ClearMessage();

	// 标记状态值清空
	m_nMarkCount = 0;
	m_dbMarkTime = 0;
	SetMarkCount(0);

	m_sMobFilePath.Empty();
	m_sMarkFileName.Empty();

	//// 激光笔状态值清空
	m_sPenName.clear();
	m_dbPower.clear();
	m_dbFreq.clear();
	m_nSpeed.clear();

	// 文档复位
	Clear2DData();
	ClearMapData();
	CWaferEngine::Instance()->SetFileName("");    // 清除文件名称
	CWaferEngine::Instance()->RemoveAllDoc();     // 清除所有文档
	CWaferEngine::Instance()->ResetModified();    // 将修改标志复位
	m_pWaferDoc = NULL;
	m_pMapDoc = NULL;
	UpdateShowView();

	// 清空图档名称内容
	SetStaticText(IDC_STATIC_FILE_NAME, _T(""));

	// 清空激光参数显示
	ClearLaserParamShow();

	// 关闭日志
	EndLog();

	m_bManualStopMark = FALSE;
	m_bSetVisionOK = FALSE;
	m_bPLCSwitchProcessOK = FALSE;
	m_bSwitchVisionCheckProcessOK = FALSE;
	m_bScanBarcode = FALSE;
	//模式切换后,lotID,ScripID,markType记录的值清空
	m_sLotIDCur.Empty();
	m_sStripIDCur.Empty();
	m_markType.Empty();
	m_varName.Empty();

	m_ctrlTips.ShowWindow(FALSE);
	m_ctrlTips.ShowText(_T(""));

	//激光参数
	m_pMainDlg->SetStaticText(IDC_STATIC_LASERPARAM, _T(""));
}

void CFormWorkControl::UpdateShowView()
{
	if (NULL == m_pWaferDoc || NULL == m_pMapDoc)
	{
		m_pMarkContentView->SetShowContent(emShowContent_NULL, NULL);
		m_pMarkContentView->Invalidate(FALSE);
		m_pViewAllBin->SetBinContent(NULL);
		m_pViewAllBin->Invalidate(FALSE);
		UpdateObjectList(NULL);
		return;
	}

	// 显示bin0
	CWaferGraph * pWafer = nullptr;
	pWafer = m_pWaferDoc->GetWafer(0);
		
	if (NULL == pWafer)
	{
		m_pViewAllBin->SetBinContent(NULL);
		m_pViewAllBin->Invalidate(FALSE);
		UpdateObjectList(NULL);
		return;
	}

	m_pMarkContentView->SetShowContent(emShowContent_Map_Only);
	m_pViewAllBin->SetBinContent(m_pWaferDoc);

	UpdateObjectList(pWafer);
	
	m_pMarkContentView->Invalidate(FALSE);
	m_pViewAllBin->Invalidate(FALSE);
}

void CFormWorkControl::UpdateObjectList(CWaferGraph * pWafer)
{
	// 清空所有的文本信息
	for (int i = 0; i < 10; i++)
	{
		GetDlgItem(g_nTextName[i])->ShowWindow(FALSE);  
		GetDlgItem(g_nTextValue[i])->ShowWindow(FALSE);
		GetDlgItem(g_nFontName[i])->ShowWindow(FALSE);
	}

	if (nullptr == pWafer)	return;

	// 更新对象列表
	std::vector<std::string> vsrefName, vsrefContent, fontName;
	pWafer->GetAllTextNameAndContent(vsrefName, vsrefContent, fontName);
	int iTextCount = vsrefName.size() <= 10 ? vsrefName.size() : 10;   // 最多只能显示10个对象
	for (int i = 0; i < iTextCount; i++) {
		SetStaticText(g_nTextName[i], CString(vsrefName[i].c_str()));       //文本名称
		SetDlgItemText(g_nTextValue[i], CString(vsrefContent[i].c_str()));  //文本值
		//字体名称
		CString tmpFontName = CString(fontName[i].c_str());
		int n = tmpFontName.ReverseFind('.');
		tmpFontName = tmpFontName.Mid(0, n);  //截取末位.之前的数据
		SetDlgItemText(g_nFontName[i], tmpFontName/*CString(fontName[i].c_str())*/);
		GetDlgItem(g_nTextName[i])->ShowWindow(TRUE);  
		GetDlgItem(g_nTextValue[i])->ShowWindow(TRUE); 
		GetDlgItem(g_nFontName[i])->ShowWindow(TRUE); 
		// 判断文本对象是否为变量，南京华天，以"TEXT"开头都是变量
		if (0 == strncmp(vsrefName[i].c_str(), "TEXT", 4))
			m_bVariable[i] = TRUE;
		else
			m_bVariable[i] = FALSE;
	}
}
BOOL CFormWorkControl::StartLog(const CString & sName)
{
	CTime currentTime = CTime::GetCurrentTime();
	CString timeHM = currentTime.Format(_T("%Y-%m"));
	CString timeYmdHMS = currentTime.Format(_T("%Y_%m_%d_%H%M%S"));

	//************判断log文件目录是否存在***************//
	CString sFilePath, sFileName, s;
	int times = 2;
	while (times > 0)
	{
		sFilePath = CFactoryConfig::Instance()->m_sLogFilePath + _T("\\") + CFactoryConfig::Instance()->m_sDeviceNo + _T("\\") + timeHM + _T("\\");
		if (!PathIsDirectory((LPTSTR)(LPCTSTR)sFilePath))
		{
			if (!MakeSureDirectoryPathExists(CStringA(sFilePath)))
			{
				// 以默认路径创建
				CFactoryConfig::Instance()->m_sLogFilePath = CString(GetSystemDir()) + _T("Log");
				s.Format(_T("日志文件路径修改为：%s！"), CFactoryConfig::Instance()->m_sLogFilePath);
				PrintMessage(emMsgType_Warn, s);
				times--;
				continue;
			}
		}
		break;
	}
	if (0 == times)
	{
		s.Format(_T("%s文件夹创建失败！"), sFilePath);
		PrintMessage(emMsgType_Warn, s);
		return FALSE;
	}

	//************创建Log文件，并写入信息***************//
	sFileName.Format(_T("%s %s.log"), sName, timeYmdHMS);
	sFilePath += sFileName;
	if (!m_LogStdioFile.Open(sFilePath, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite))
	{
		s.Format(_T("%s文件创建失败！"), sFilePath);
		PrintMessage(emMsgType_Warn, s);
		return FALSE;
	}

	m_bOpenFile = TRUE;
	return TRUE;
}

void CFormWorkControl::PrintMessage(int nType, const CString & s, UINT nFlag)
{
	CString sText = s;
	if (emMsgType_Title != nType)
	{
		CString sCurDate = CTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S\t"));
		sText = sCurDate + s + _T("\n");
	}
	else {
		sText += _T("\n");
	}

	if(0x01 & nFlag)
		m_pMainDlg->PrintMessage((emFactoryMsgType_t)nType, sText);

	if (0x02 & nFlag) {
		// 保存到Log中
		if (m_bOpenFile)
		{
			m_LogStdioFile.SeekToEnd();
			m_LogStdioFile.Write(CStringA(sText).GetString(), CStringA(sText).GetLength());
		}
	}
}

BOOL CFormWorkControl::LoadContent(const CString & sMarkFilePath, const CString & sStampFilePath)
{
	CString s, sSrc, sDst;
	bool bStampNameNull = sStampFilePath.IsEmpty();
	std::vector<int> virefLayerID;
	CWaferEngine * pWaferEngine = CWaferEngine::Instance();
	m_sMarkFilePath = sMarkFilePath;

	if ((emWorkMode_Manual == m_pConfig->m_nWorkMode)
		|| !m_pConfig->m_bOpenRMS)
	{
		// 只加载本地图档
		if (!pWaferEngine->LoadFile(CStringA(sMarkFilePath).GetString()))
			goto TestError;

		if (false == bStampNameNull)
		{
			// 直接采用印章模板中的参数名称
			if (!pWaferEngine->LoadGraphDoc(CStringA(sStampFilePath).GetString()))
			{
				s.Format(_T("印章图档[%s]加载失败！"), sStampFilePath);
				PrintMessage(emMsgType_Error, s);
				goto TestError;
			}
		}
		m_pWaferDoc = pWaferEngine->GetGraphDoc();
		m_pMapDoc = pWaferEngine->GetMapDoc();
	}
	else
	{
		// RMS功能-远程下载印章模板内容，替换
		if (false == bStampNameNull) {
			sSrc = sStampFilePath.Right(sStampFilePath.GetLength() - sStampFilePath.ReverseFind(_T('\\')) - 1);
			sDst = _T("DATA\\") + sSrc;
		}
		else {
			sSrc = sMarkFilePath.Right(sMarkFilePath.GetLength() - sMarkFilePath.ReverseFind(_T('\\')) - 1);
			sSrc.Replace(_T("twfb"), _T("twd"));
			sDst = _T("DATA\\") + sSrc;
		}

		s.Format(_T("从服务器下载印章模板%s。正在下载..."), sSrc);
		PrintMessage(emMsgType_Normal, s);

		if (!DownloadFile(_T("twd\\") + sSrc, sDst))
		{
			::DeleteFile(sDst);
			s.Format(_T("%s印章模板下载失败！"), sSrc);
			PrintMessage(emMsgType_Error, s);
			goto TestError;
		}

		s.Format(_T("%s印章模板下载成功！"), sSrc);
		PrintMessage(emMsgType_Success, s);


		// 加载本地MAP和参数，远程加载印章模板和参数名称
		// 只加载本地图档
		if (!pWaferEngine->LoadMapDoc(CStringA(sMarkFilePath).GetString()))
			goto TestError;

		// 根据客户需求，RMS服务器上，除了存放印章模板外， 还需要存放字体文件
		// 所以必须先加载一次图档，获取图档所需的字体名称，然后下载字体到本地
		// 然后再重新加载图档。
		// 加载印章模板-目的：获取字体名称
		vector<string> vsPenName;
		vsPenName.clear();
		pWaferEngine->LoadDocWithoutPen(CStringA(sDst).GetString(), vsPenName);
		if (!DownLoadFontFile())
			goto TestError;

		// 字体加载后，重新加载文档	
		// 直接采用印章模板中的参数名称
		if (!pWaferEngine->LoadGraphDoc(CStringA(sStampFilePath).GetString()))
		{
			s.Format(_T("印章图档[%s]加载失败！"), sStampFilePath);
			PrintMessage(emMsgType_Error, s);
			::DeleteFile(sDst);
			goto TestError;
		}
		::DeleteFile(sDst);

		m_pWaferDoc = pWaferEngine->GetGraphDoc();
		m_pMapDoc = pWaferEngine->GetMapDoc();
	}

	if (emWorkMode_Auto == m_pConfig->m_nWorkMode) 
	{	
		// 校正参数
		vector<CString> vsParamVlue;
		pWaferEngine->GetGraphDoc()->GetAllParamValue(vsParamVlue);
		// 检查激光参数是否与XML中的一致
		if (m_sLaserParamCur != vsParamVlue[0]) {
			s.Format(_T("激光参数名校验失败！xml中激光参数名:%s , 当前图档激光参数名:%s"), m_sLaserParamCur, CString(vsParamVlue[0]));
			PrintMessage(emMsgType_Error, s);
			goto TestError;
		}
				
		//校验字体
		if (!VerifyFontName()) {
			goto TestError;
		}
	}

	//if (FALSE == IsEqualStampSize())
	//{
	//	PrintMessage(emMsgType_Error, _T("阵列图档中的印章尺寸和印章模板中的印章尺寸不一致！"));
	//	goto TestError;
	//}
	
	ShowLaserParam();
	Show2DAndMapInfo();
	ChangeArrayBin_0();

	// 先找到最新的修正文件
	if (!FindNewMobFile())
	{
		PrintMessage(emMsgType_Error, _T("阵列位置设置失败！"));
		goto TestError;
	}

	// 加载阵列修正文件
	if (!LoadMapOffsetFile(m_sMobFilePath))
	{
		PrintMessage(emMsgType_Error, _T("阵列位置设置失败！"));
		goto TestError;
	}
	PrintMessage(emMsgType_Success, _T("阵列位置载入成功！"));

	return TRUE;

TestError:
	return FALSE;
}

BOOL CFormWorkControl::DownloadFile(const CString & sSrcFile, const CString & sDstFile)
{
	if (sSrcFile.IsEmpty() || sDstFile.IsEmpty())
		return FALSE;

	CString s;
	CYWFtpSession  myftp(this);
	myftp.SetServerParam(CFactoryConfig::Instance()->m_sFtpIP, 21);
	BOOL bRet = myftp.ConnectToServer();
	if (bRet == FALSE)
	{
		s.Format(_T("FTP服务器连接失败，检查服务器是否有安装FTP服务！失败原因：%s"), myftp.GetLastErrorMessage());
		PrintMessage(emMsgType_Error, s);
		return FALSE;
	}

	if (!myftp.GetFileFromServer(sSrcFile, sDstFile, FALSE))
	{
		s.Format(_T("[%s]下载失败，请确认文件是否存在!"), sSrcFile);
		PrintMessage(emMsgType_Error, s);
		return FALSE;
	}

	//采用读取文件内容的方式去下载文件，对于很大的文件，可以精确了解下载进度
	//ULONGLONG ulFileSize = 0;
	//CInternetFile *pFtpFile = myftp.OpenFile(sSrcFile, ulFileSize);
	//if (pFtpFile == NULL)
	//{
	//	s.Format(_T("服务器文件[%s]打开失败，请确认文件是否存在!"), sSrcFile);
	//	PrintMessage(emMsgType_Error, s);
	//	return FALSE;
	//}

	//ULONGLONG SrcLength = ulFileSize;   //得到文件大小,大小不准确

	//CFile file;
	//ULONGLONG DownLength = 0; //已经下载了的数据长度
	//if (!file.Open(sDstFile, CFile::modeCreate | CFile::modeWrite))
	//{
	//	s.Format(_T("本地文件[%s]创建失败，请确认文件夹是否存在!"), sDstFile);
	//	PrintMessage(emMsgType_Error, s);
	//	return FALSE;
	//}

	////计算文件去读的步长
	//DWORD dwStep = STEPLEN > SrcLength ? SrcLength : STEPLEN;
	//DWORD dwRead = dwStep;
	////数据缓冲区
	//char *pBuf = new char[dwStep + 1];
	//memset(pBuf, 0x00, dwStep + 1);
	//while ((dwRead = pFtpFile->Read(pBuf, dwStep)) > 0)
	//{
	//	Sleep(1);
	//	//写目标文件;
	//	file.Write(pBuf, dwRead);
	//	DownLength += dwRead;
	//	//更新进度
	//}

	////完成
	//delete pBuf;
	////关闭文件
	//pFtpFile->Close();
	//file.Close();
	return TRUE;
}

BOOL CFormWorkControl::DownLoadFontFile()
{
	if (nullptr == m_pWaferDoc)
		return false;

	// 收集字体文件
	std::set<std::string> vsFontName;
	int nWaferC = m_pWaferDoc->GetWaferCount();
	for (int i = 0; i < nWaferC; i++)
	{
		CWaferGraph * pWafer = m_pWaferDoc->GetWafer(i);
		if (nullptr == pWafer)
			continue;
		
		int32_t nObjC = pWafer->GetObjCount();
		for (int j = 0; j < nObjC; j++)
		{
			IGraphObject * pObj = pWafer->GetObject(j);
			if (nullptr == pObj)
				continue;

			char chFontName[MAX_TXT_SIZE] = { 0 };
			int nType = pObj->GetType();
			if (emObjType_TEXT == nType
				|| emObjType_AUTO == nType
				|| emObjType_TIME == nType)
			{
				memset(chFontName, 0, MAX_TXT_SIZE);
				pObj->GetConfigInfo(Object_TextFontName, (void*)chFontName);
				if ('\0' != chFontName[0])
				{
					vsFontName.insert(chFontName);
				}
			}
		}
	}

	// 如果没有所需的字体，则返回true，不用下载
	if (0 == vsFontName.size())
		return true; 

	CString sSrc, sDst;
	CString s;
	for (auto & item : vsFontName)
	{
		CString sFontName = CString(item.c_str());
		sSrc = _T("fonts\\") + sFontName;
		sDst = _T("fonts\\tch\\") + sFontName;
		s.Format(_T("从服务器下载印章模板所需字体文件%s。正在下载..."), sFontName);
		PrintMessage(emMsgType_Normal, s);

		if (!DownloadFile(sSrc, sDst))
		{
			s.Format(_T("%s字体下载失败！"), sFontName);
			PrintMessage(emMsgType_Error, s);
			return false;
		}

		s.Format(_T("%s字体下载成功！"), sFontName);
		PrintMessage(emMsgType_Success, s);
	}
	return true;
}

void CFormWorkControl::SetStaticText(UINT uCtlID, const CString & s)
{
	SetDlgItemText(uCtlID, s);
	UpdateTransparentControlBK(uCtlID);
}

BOOL CFormWorkControl::IsMarking()
{
	if (1 == m_nMarkFlag)
		return TRUE;
	else if (0 == m_nMarkFlag)
	{
		IPlatform * pPlatform = CMarkPlatform::Instance();
		IMarkEngine *pMarkEngine = pPlatform->MarkEngine();
		if (emMARK_STAT_BUSY == pMarkEngine->GetMarkStat())
			return TRUE;
	}
	return FALSE;
}


void CFormWorkControl::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (1 == nIDEvent)
	{
		IMarkEngine *pTheEngine = m_pMarkEngine;
		uint32_t iStat = pTheEngine->GetStatus();
		m_bSysOK = true;
		m_sSysStatus = _T("");
		m_sHardWareStatus = _T("");
		m_sLaserStatus = _T("");
		m_sShutterStatus = _T("");
		bool bFirstLaserFail = true;   // 只有成功后的第一次激光器失败，才会更新激光器上电时间

		if (SYS_STAT_OK != iStat)
		{
			m_bSysOK = false;
			if (SYS_STAT_SYS_WARN&iStat)
				m_sSysStatus = _T("系统告警");
			else if (SYS_STAT_LINE_WARN&iStat)
				m_sSysStatus = _T("流水线告警");
			else if (SYS_STAT_TEMP_WARN&iStat)
				m_sSysStatus = _T("温度告警");
			else if (SYS_STAT_ELEC_WARN&iStat)
				m_sSysStatus = _T("激光器电路告警");
			else if (SYS_STAT_WATER_WARN&iStat)
				m_sSysStatus = _T("水冷机告警");
			else if (SYS_STAT_LASER_WARN&iStat)
				m_sSysStatus = _T("激光器告警");
		}

		if (!pTheEngine->IsSignInActive(emSIGN_IN_HARDWORK))
		{
			m_bSysOK = false;
			m_sHardWareStatus = _T("电源关闭");
		}

		if (!pTheEngine->IsSignInActive(emSIGN_IN_LASERLIGNT))
		{
			m_bSysOK = false;
			m_sLaserStatus = _T("激光器关闭");
			if (bFirstLaserFail) {
				bFirstLaserFail = false;
				UpdateLaserDiodeTime(false);
			}
		}

		if (!pTheEngine->IsSignInActive(emSIGN_IN_SHUTTER))
		{
			m_bSysOK = false;
			m_sShutterStatus = _T("Shutter关闭");
		}

		static BOOL bShowSuccessInfo = FALSE;
		if (!m_bSysOK)
		{
			m_pMarkDriver->WriteExtPortPin(m_pConfig->m_nReadyPort, IO_OUT_INVALID);// 给自动化发送激光掉线的标志,准备信号设置为没有信号
			CString s;
			s.Format(_T("%s,%s,%s,%s"), m_sSysStatus, m_sHardWareStatus, m_sLaserStatus, m_sShutterStatus);
			PrintMessage(emMsgType_Error, s);
			bShowSuccessInfo = TRUE;  // 失败后，再成功，需要显示一条成功信息
		}
		else {
			bFirstLaserFail = true;
			if (bShowSuccessInfo) {
				m_pMarkDriver->WriteExtPortPin(m_pConfig->m_nReadyPort, IO_OUT_VALID);
				PrintMessage(emMsgType_Success, _T("设备恢复正常"));
				bShowSuccessInfo = FALSE;
				UpdateLaserDiodeTime(true);
			}
		}

		// 标记状态
		if (IsMarking())
		{
			m_pMainDlg->SetStaticText(IDS_MARK_STATUS, MARKSTATUS_MARK);
		}

		/**************************lrf 频繁读取可能会造成信号接收不稳，暂时去除*****************************/
		// 开始信号IO检测， 界面显示
		//uint32_t nBit = 0x00;
		//m_pMarkDriver->ReadExtPortPin(m_pConfig->m_nStartPort + 16, nBit);   // 端口号 + 16
		//m_sStatusStartMark.SetStatus((IO_IN_VALID == nBit) ? CInputStatic::SIGNAL : CInputStatic::NON_SIGNAL);
		//// 2D读码器触发信号显示
		//m_sStatus2DBarcodeBegin.SetStatus(Scan1SignalEffective(0) ? CInputStatic::SIGNAL : CInputStatic::NON_SIGNAL);
	}
	else if (2 == nIDEvent)
	{
		UpdateLaserDiodeTime(true);

		// 判断在指定时间内，清空二维码数据的内容
#if 0   
		if(0 != m_pConfig->m_nCheck2DBarcodeRepeat)
		    Delete2DBarcodeContent();
#endif


	}
//	else if (5 == nIDEvent)
//	{
//		uint32_t nBit = 0x00;
//		m_pMarkDriver->ReadExtPortPin(m_pConfig->m_nStartPort + 16, nBit);   // 端口号 + 16
//#ifdef DEBUG_IO
//		if (GetDlgItemInt(IDC_EDIT_IO))
//#else
//		if (IO_IN_VALID == nBit)   //检测是否为开始标记
//#endif	
//		{
//			PrintMessage(emMsgType_Normal, _T("IO口检测到开始标记信号"));
//			// 开始标记
//			StartMark();
//		}
//	}
	else if (6 == nIDEvent)
	{ 
		// 标记完成后检测
		if (!IsMarking())
		{
			KillTimer(6);

			CString s;
			DWORD dwTime = GetTickCount() - m_dwStartTick;
			m_dwStartTick = 0;
			s.Format(_T("%d.%d"), dwTime / 1000, dwTime % 1000);
			m_dbMarkTime = _ttof(s);
			SetMarkCount(++m_nMarkCount);
			s.Format(_T("标记完成。标记数量：%d，标记时间：%.3fs。"), m_nMarkCount, m_dbMarkTime);
			PrintMessage(emMsgType_Normal, s);
			
			// 标记完成后，EDO6信号输出
			//m_pMarkDriver->WriteExtPortPin(m_pConfig->m_nBusyPort, IO_OUT_INVALID);
			m_pMarkDriver->WriteExtPortPin(m_pConfig->m_nEndPort, IO_OUT_VALID);
#ifdef DEBUG_IO
			SetDlgItemInt(IDC_EDIT_IO_FINISH, !m_pConfig->m_nFinishMarkSignalType);
#endif           
			// “扫入Lot后，自动加载本地图档”模式，判断是否需要送检MC
			if ((emWorkMode_Scan == m_pConfig->m_nWorkMode 
				|| emWorkMode_Auto == m_pConfig->m_nWorkMode 
				|| emWorkMode_Ponum == m_pConfig->m_nWorkMode)
				&& m_pConfig->m_bMCCheckWarn
				&& m_bScanBarcode)
			{
				m_bScanBarcode = FALSE;
				if (IsMCCheck())
				{
					CDlgMCCheckWarn dlg(m_pConfig->m_sMCCheckTip, this);
					dlg.DoModal();
					// 如果满足送检MC文件
					OnBnClickedButtonCanceljob();
				}
				else
				{
					// 结束标记后，自动开启JOB
					OnBnClickedButtonLoadJob();
				}
			}
			else
			{
				// 结束标记后，自动开启JOB
				OnBnClickedButtonLoadJob();
			}


			//if (emWorkMode_Manual == m_pConfig->m_nWorkMode)
			//{
			//	GetDlgItem(IDC_BUTTON_STARTMARK)->EnableWindow(TRUE);
			//}
				
			GetDlgItem(IDC_BUTTON_CELL)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_AREA)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_MATRIX)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_STOPMARK)->EnableWindow(FALSE);
		}
	}
	 if (SCANSIGNAL_CHECK_TIMER == nIDEvent)  //扫描
	{
		//DetectionProcessSignal();   // 检查是不是需要结束标记流程
		int iRet = ScannerTimerProcess();
		if (-1 == iRet) {
			OnBnClickedButtonCanceljob();   // 扫描数据异常，退出侦测流程
			return;
		}

		// 扫描成功后，也需要重新开启2D扫描信号侦测，在后面检查到开始标记信号后，再关闭
		// 防止CCD定位失败，软件流程卡在侦测开始标记信号那个步骤中，自动化复位后，再次扫描2D信号，不会有反应
		SetTimer(SCANSIGNAL_CHECK_TIMER, 100, nullptr);
	}
	__super::OnTimer(nIDEvent);
}

int32_t CFormWorkControl::StartMark(int iFlag)
{	
	// 标记前，检查状态是否正常
	if (!PreMarkCheck()) {
		return -1;
	}

	// 只有等于0的时候，才检查以下内容
	if (0 == iFlag)   
	{
		//if (m_pConfig->m_bCheckVisualSystem && !m_bCCDOffseted)
		//{
		//	OnBnClickedButtonCanceljob();   // 退出测试
		//	PrintMessage(emMsgType_Error, _T("已启用视觉定位，但未接收到CCD定位数据！"));
		//	return -1;
		//}

		// 条数限制检查
		if (IsMaxMarkTimes()) {
			return -1;
		}
		if (!IsDummy())
		{
			if (!Check2DBarcode()) {
				PrintMessage(emMsgType_Error, _T("此图档为2D Barcode图档，请检查是否已经扫描2D Barcode！"));
				return -1;
			}

			if (!CheckStripMap()) {
				PrintMessage(emMsgType_Error, _T("此图档为Strip Map图档，请检查是否已经扫描Strip Map！"));
				return -1;
			}
		}
	}

	// Mapping调试设置
	if (!SetMappingDebug())
	{
		return -1;
	}
		
	// 设置标记状态
	SetMarkStatus();   // 后面的代码不可以中途返回，失败或成功只能跳到TestError:中去

	// 设置IO开始标记信号
	//m_pMarkDriver->WriteExtPortPin(m_pConfig->m_nBusyPort, IO_OUT_VALID);
	m_pMarkDriver->WriteExtPortPin(m_pConfig->m_nEndPort, IO_OUT_INVALID);

#ifdef DEBUG_IO
	SetDlgItemInt(IDC_EDIT_IO_FINISH, m_pConfig->m_nFinishMarkSignalType);
#endif


	
	int32_t iError(emREC_OK);
	CString s;
	m_dwStartTick = GetTickCount();

	int32_t nRet = -1;
	// 开始系统硬件检测定时器
	if (0 == m_pMapDoc->GetMapObjMarkCount())
	{
		PrintMessage(emMsgType_Warn, _T("未选中任何印章！"));
		nRet = 0;
	}
	else
	{
		// Add by hhhuang 20260409
		// dummy模式的“不良品对比模式”，不替换2D数据
		if (IsDummy())
		{
			if (1 == m_pConfig->m_iMappingDebugSet
				|| 3 == m_pConfig->m_iMappingDebugSet) {
				m_vs2DData.clear();
				m_vs2DData2.clear();
			}
		}
		nRet = m_pMapDoc->Mark(m_vs2DData, m_vs2DData2, m_pConfig->m_bPenOrder, true);  // 从下往上标记
	}
	// 如果存在2D Barcode数据，更新界面
	if (m_pWaferDoc->IsExist2DBarcode())
		m_pViewAllBin->Invalidate(false);

	// 如果测试成功，保存二维码数据到数据库中
	if (0 == nRet)
	{
		if (!(IsDummy() || IsFirstPrint()))  // Dummy模式和首条打印下不保存二维码信息
		{
			if (!m_2DRepeatCheck.Save(m_sStripIDCur, m_vs2DData))
			{
				s.Format(_T("%s"), m_2DRepeatCheck.GetLastErrorMsg());
				PrintMessage(emMsgType_Error, s);
				// 保存失败，不用停止流程
				iError = -1;
			}

			if (!m_2DRepeatCheck.Save(m_sStripIDCur, m_vs2DData2))
			{
				s.Format(_T("%s"), m_2DRepeatCheck.GetLastErrorMsg());
				PrintMessage(emMsgType_Error, s);
				// 保存失败，不用停止流程
				iError = -1;
			}
		}

		//Add by hcx 20240228
		if (!IsDummy())
		{
			s.Format(_T("判断是否发送MAPPING信息！IsMapping=%d, VisionCheck=%d"), IsMapping(), m_pConfig->m_bVisionCheck);
			PrintMessage(emMsgType_Normal, s);
			if (IsMapping() && m_pConfig->m_bVisionCheck)
			{
				s.Format(_T("发送MAPPING信息！%s"), L"LaserMapping�" + m_stripID + L"�");
				PrintMessage(emMsgType_Normal, s);
				//"LaserMapping�" + StripID + "�" + Data
				CString msg = L"LaserMapping�" + m_stripID + L"�" + m_stripIDInfo;
				CStringA msga = chT2U(msg.GetString()).c_str();
				m_stripIDInfo.Empty();
				if (!m_pVisionCheckServer->SendRemote(msga, msga.GetLength()))
				{
					PrintMessage(emMsgType_Error, _T("视觉检测发送失败！"));
					iError = -1;
				}
			}
		}
		//End by hcx 02240228
	}

	
	// 标记完成后，立刻清空2D Barcode和Strip Map数据
	Clear2DData();
	ClearMapData();
	if (0 != nRet)
	{
		PrintMessage(emMsgType_Error, _T("标记错误！"));
		iError = -1;
	}

TestError:
	//根据CCD偏移且打标完成后，恢复原始图档
	if (m_bCCDOffseted) {
		if (!ResetOriginDoc())
			iError - 1;

		m_bCCDOffseted = FALSE;
	}
	bool bPreDummy = false;
	if (IsDummy())
	{
		bPreDummy = true;
	}
	EnableDummy(0);
	EnableFirstPrint(0);
	ResetMappingDebugSet();

	// 板卡数据传输完成，状态设置为0
	m_nMarkFlag = 0;
	
	FinishMark(iError);

	if (bPreDummy)
	{
		// 如果为Dummy模式，标记完成后，将所有的阵列设置为标记
		m_pMapDoc->SetPrintAll();
	}
	
	
	return iError;
}

BOOL CFormWorkControl::PreMarkCheck()
{
	if (!m_bSysOK)
	{
		//CString s;
		//s.Format(_T("%s,%s,%s,%s"), m_sSysStatus, m_sHardWareStatus, m_sLaserStatus, m_sShutterStatus);
		//PrintMessage(emMsgType_Error, s);
		m_pMarkDriver->WriteExtPortPin(m_pConfig->m_nReadyPort, IO_OUT_INVALID);
		return FALSE;
	}

	if (IsMarking())
	{
		PrintMessage(emMsgType_Error, _T("正在标记中..."));
		return FALSE;
	}

	if (NULL == m_pWaferDoc || NULL == m_pMapDoc)
	{
		PrintMessage(emMsgType_Error, _T("没有发现打印图档！"));
		return FALSE;
	}

	CWaferGroup * pWaferGroup = m_pMapDoc->GetGroup(0);
	if (NULL == pWaferGroup)
	{
		PrintMessage(emMsgType_Error, _T("没有发现打印图档！"));
		return FALSE;
	}

	if (!CheckCaveatTime()) //检查是否达到预警值
	{
		PrintMessage(emMsgType_Error, _T("激光器使用时间达到预警值！"));
		return FALSE;
	}

	return TRUE;
}


///////////////////////////
void CFormWorkControl::ParseReturnData(const CString & s, std::map<CString, CString> & maps)
{
	maps.clear();
	if (s.IsEmpty())
		return;

	TCHAR chS = _T('<');
	TCHAR chE = _T('>');
	TCHAR chDim = _T(',');

	int ns = s.Find(chS);
	while (-1 != ns)
	{
		int ne = s.Find(chE, ns);
		if (-1 != ne)
		{
			CString sf = s.Mid(ns + 1, ne - ns - 1);
			int ndim = sf.Find(chDim);
			if (!sf.IsEmpty() && ndim > 0)
			{
				maps[sf.Left(ndim)] = sf.Right(sf.GetLength() - ndim - 1);
			}
		}
		if (-1 == ne)
			break;
		ns = s.Find(chS, ne);
	}
}

// @StartMark*1.001#0.021#0.017
int32_t CFormWorkControl::PraserFrame(char *sRecvBuff, int32_t iSize, CStringArray &sArray)
{
	sArray.RemoveAll();
	string str = sRecvBuff;
	int n = str.find('*'); //查找*出现位置
	if (string::npos == n)
	{
		PrintMessage(emMsgType_Error, _T("CCD指令格式错误，未找到*"));
		return -1;
	}

	str = str.substr(n + 1, str.length() - n - 1);
	vector<string> vsData;
	CharUtility::sParseStringA(str, vsData, '#');

	for (auto& item : vsData)
	{
		sArray.Add(CString(item.c_str()));
	}
	
	return sArray.GetSize();
}


// 解析返回内容
// 接收成功ASSY MKBR1<RETURN,0><TOP_MK_ID,BB-KK-1561015><CODE7,100><CODE6,539>
// 接收失败ASSY MKBR1<RETURN,1><MSG,Lot is Hold!>
int32_t CFormWorkControl::doLot(char *sRecvBuff, int32_t iSize)
{
	CFactoryConfig *pFactoryConfig = CFactoryConfig::Instance();
	CString s, sFilePath, sFileName, sStampFilePath;
	std::map<CString, CString> maps;
	std::map<CString, CString> mapValue;  // 变量以及内容
	s = CString(sRecvBuff);
	ParseReturnData(CString(sRecvBuff), maps);  // 解析内容
	if (maps.size() <= 0 || -1 == s.Find(_T("ASSY MKBR")))
	{
		s.Format(_T("未接收到印章图号！接收内容格式不正确"));
		PrintMessage(emMsgType_Error, s);
		goto TestError;
	}

	if (maps[_T("RETURN")] != _T("0"))
	{
		s.Format(_T("未接收到印章图号！失败信息：%s"), chU2T(CStringA(maps[_T("MSG")]).GetString()).c_str());
		PrintMessage(emMsgType_Error, s);
		goto TestError;
	}
	sFileName = maps[_T("TOP_MK_ID")] + _T(".twfb");
	m_sMarkFileName = sFileName;
	// 检查印章图档的头部
	if (!CheckMarkFileNameHead(m_sMarkFileName))
		goto TestError;

	sFilePath = pFactoryConfig->m_sMarkFilePath + _T("\\") + sFileName;
	// 检查图档是否初次使用
	if (IsMarkFileFirstUse(sFilePath))
	{
		s.Format(_T("警告：该图档[%s]在本机第一次加载"), sFilePath);
		CDlgMCCheckWarn dlg(s, false);
		dlg.DoModal();
	}

	sStampFilePath = sFilePath;
	sStampFilePath.Left(sStampFilePath.GetLength() - _tcslen(_T(".twfb"))) + _T(".twd");
	if (!LoadContent(sFilePath, sStampFilePath))
	{
		s.Format(_T("印章图档加载失败！路径：%s"), sFilePath);
		PrintMessage(emMsgType_Error, s);
		goto TestError;
	}
	s.Format(_T("印章图档加载成功！路径：%s"), sFilePath);
	PrintMessage(emMsgType_Success, s);

	// 内容替换
	for (auto & item : maps)
	{
		if (item.first == _T("RETURN") || item.first == _T("TOP_MK_ID"))
			continue;
		mapValue[item.first] = item.second;
	}

	if (!VariableSubstitution(mapValue)) {
		goto TestError;
	}
		
	// 更新消息
	UpdateShowView();

	// 加载印章名称
	sFileName.Replace(_T(".twfb"), _T(""));  // 不显示后缀
	SetStaticText(IDC_STATIC_FILE_NAME, sFileName);

	WritePrivateProfileString(_T("MarkFile"), sFilePath, _T("1"), g_sTempConfigPath);
	return 0;

TestError:
	return -1;
}

void CFormWorkControl::PrintMesMessage(int nType, const TCHAR* sMsg)
{
	PrintMessage(nType, sMsg);
}

/*
**  BIN;OriginLocation;Bin1;Bin2;…
**  BIN;0;3,2,1,2,1,1,....
*/
int32_t CFormWorkControl::doBin(char *sRecvBuff, int32_t iSize)
{
	CString s;
	s.Format(_T("远程接收内容：%s"), CString(sRecvBuff));
	PrintMessage(emMsgType_Normal, s);

	// 兼容rofin指令：SETTING;BIN;0
	char* pTmpBuff = strstr(sRecvBuff, "SETTING");
	if (NULL != pTmpBuff)
		return 0;

	// 判断是否正在标记
	if (IsMarking())
	{
		PrintMessage(emMsgType_Error, _T("正在打标中，修改BIN失败！"));
		return -1;
	}

	// 判断是否CancelJob
	if (IsCancelJob())
	{
		PrintMessage(emMsgType_Error, _T("当前处于“取消作业”状态，请点击“开始作业”！"));
		return -1;
	}

	if (NULL == m_pWaferDoc || NULL == m_pMapDoc)
	{
		PrintMessage(emMsgType_Error, _T("无印章图档，BIN修改失败！"));
		return -1;
	}
	CWaferGroup * pWaferGroup = m_pMapDoc->GetGroup(0);
	if (NULL == pWaferGroup)
	{
		PrintMessage(emMsgType_Error, _T("无印章阵列图，BIN修改失败！"));
		return -1;
	}

	int32_t iError(emREC_OK);

	// 清空BIN MAP数据
	ClearMapData();

	CStringArray sArray;
	if (PraserFrame(sRecvBuff, iSize, sArray) > 1)
	{
		// 第一步，判断传入的芯片数量是否与MAP中的芯片数量一致
		int nSendCount = sArray.GetSize() - 2;   // 其中前两个元素（BIN;0;）忽略
		int nMapCount = pWaferGroup->vWaferMetas.size();
		if (nSendCount != nMapCount)
		{
			s.Format(_T("阵列中的模板数量[%d]与传入的模板数量[%d]不一致！"), nMapCount, nSendCount);
			PrintMessage(emMsgType_Error, s);
			return -1;
		}

		// 第二步，替换所有MAP中芯片的类型
		for (int i = 0; i < nMapCount; i++)
		{
			int nWaferID = _ttoi(sArray.GetAt(i + 2));
			if (nWaferID < 0 || nWaferID >= m_pWaferDoc->GetWaferCount())
			{
				nWaferID = -1;    // 当传入的BIN不存在时，设置为-1，避免标记出错
				s.Format(_T("第%d颗印章[行%d，列%d]对应的BIN%d不存在！"), i,
					pWaferGroup->vWaferMetas[i].nRow, pWaferGroup->vWaferMetas[i].nCol, _ttoi(sArray.GetAt(i + 2)));
				PrintMessage(emMsgType_Error, s);
				iError = -1;
			}

			m_iBinMap.push_back(nWaferID);
			pWaferGroup->vWaferMetas[i].SetWaferID(nWaferID);
			pWaferGroup->vWaferMetas[i].EnableMark(true);
		}

		// 更新MAP显示视图
		//pWaferGroup->SetMapMode(true);
		m_pMarkContentView->Invalidate(TRUE);
	}
	else
		PrintMessage(emMsgType_Error, _T("内容解析失败！"));

	if (-1 == iError)
		PrintMessage(emMsgType_Error, _T("BIN切换失败！"));
	else
		PrintMessage(emMsgType_Success, _T("BIN切换成功！"));

	return iError;
}

int32_t CFormWorkControl::doMark(char *sRecvBuff, int32_t iSize)
{
	PrintMessage(emMsgType_Normal, _T("开始标记..."));

	// 判断是否CancelJob
	if (IsCancelJob())
	{
		PrintMessage(emMsgType_Error, _T("当前处于“取消作业”状态，请点击“开始作业”！"));
		return -1;
	}

	return StartMark();
}

/**
* 发送：ASSY MKBS2<LOT_ID, 101100000231212A><OPER_ID, 121254>
* 接收成功：ASSY MKBR2<RETURN, 0><MSG, Service Successful!>
* 接收失败：ASSY MKBR2<RETURN, 1><MSG, Lot is Hold!>
*/
int32_t CFormWorkControl::doStartLot(char *sRecvBuff, int32_t iSize)
{
	std::map<CString, CString> maps;
	CString s = CString(sRecvBuff);
	ParseReturnData(CString(sRecvBuff), maps);
	if (maps.size() <= 0 || -1 == s.Find(_T("ASSY MKBR")))
	{
		s.Format(_T("开批失败！接收内容格式不正确"));
		PrintMessage(emMsgType_Error, s);
		goto TestError;
	}

	if (maps[_T("RETURN")] != _T("0"))
	{
		s.Format(_T("开批失败！失败信息：%s"), chU2T(CStringA(maps[_T("MSG")]).GetString()).c_str());
		PrintMessage(emMsgType_Error, s);
		goto TestError;
	}
	PrintMessage(emMsgType_Success, _T("开批成功！"));
	return 0;

TestError:
	return -1;
}

/**
*发送：ASSY MKBS3<LOT_ID, 101100000231212A><OPER_ID, 121254>
*接收成功：ASSY MKBR3<RETURN, 0><MSG, Service Successful!>
*接收失败：ASSY MKBR3<RETURN, 1><MSG, Lot is Hold!>
*/
int32_t CFormWorkControl::doStopLot(char *sRecvBuff, int32_t iSize)
{
	CString s = CString(sRecvBuff);
	std::map<CString, CString> maps;
	ParseReturnData(CString(sRecvBuff), maps);
	if (maps.size() <= 0 || -1 == s.Find(_T("ASSY MKBR")))
	{
		s.Format(_T("结批失败！接收内容格式不正确"));
		PrintMessage(emMsgType_Error, s);
		goto TestError;
	}
	if (maps[_T("RETURN")] != _T("0"))
	{
		s.Format(_T("结批失败！失败信息：%s"), chU2T(CStringA(maps[_T("MSG")]).GetString()).c_str());
		PrintMessage(emMsgType_Error, s);
		goto TestError;
	}
	PrintMessage(emMsgType_Success, _T("结批成功！"));

	return 0;

TestError:
	return -1;
}

/*
** LOT2;<LotNo>;<Filename>[;<MOName1>;<MOValue1;…]
** LOT2;test-lot;D:\YZ\test.vlm;Text2;ABC;Logo1;anker.log;Var1;12345
*/
int32_t CFormWorkControl::doAutoLot(char *sRecvBuff, int32_t iSize)
{
	CString s, sFilePath, sFileName, sStampFilePath;
	CFactoryConfig *pFactoryConfig = CFactoryConfig::Instance();
	CStringArray sArray;
	std::map<CString, CString> mapValue;  // 保存变量

	RemoveCR(sRecvBuff, iSize);
	s.Format(_T("远程接收内容：%s"), CString(sRecvBuff));
	PrintMessage(emMsgType_Normal, s);

	// 判断是否正在标记
	if (IsMarking())
	{
		PrintMessage(emMsgType_Error, _T("正在标记中，修改印章图档失败！"));
		return -1;
	}

	if (PraserFrame(sRecvBuff, iSize, sArray) < 3)
	{
		s.Format(_T("印章图号解析失败！接收内容格式不正确"));
		PrintMessage(emMsgType_Error, s);
		goto TestError;
	}
	// 清空消息框
	m_pMainDlg->ClearMessage();

	// 清空状态
	ResetStatus(); 

	// 客户为了兼容Rofin的文档格式，印章图档名会带后缀.vlm，需要规避
	sFilePath = sArray.GetAt(2);
	sFilePath.Replace(_T(".vlm"), _T(""));
	sFilePath.Replace(_T(".VLM"), _T(""));
	sFilePath.Replace(_T("/"), _T("\\"));
	// 生成对象名称-内容表
	for (int32_t i = 0, nSize = (sArray.GetSize() - 3) / 2; i < nSize; i++) {
		mapValue[sArray.GetAt(2 * i + 3)] = sArray.GetAt(2 * i + 4);
	}
	// 与金泰模式通讯，且勾选了图档名称添加BB_KK_前缀
	if (m_pConfig->m_bAddFilePrefix && (emWorkMode_Auto == m_pConfig->m_nWorkMode)) {
		CString sLeft = sFilePath.Left(sFilePath.ReverseFind(L'\\') + 1);
		CString sRight = sFilePath.Right(sFilePath.GetLength() - sFilePath.ReverseFind(L'\\') - 1);
		sFilePath = sLeft + _T("BB-KK-") + sRight;
	}
	sFilePath = sFilePath + _T(".twfb");
	sFileName = sFilePath.Right(sFilePath.GetLength() - sFilePath.ReverseFind(L'\\') - 1);
	m_sMarkFileName = sFileName;
	s = m_sMarkFileName;
	s.Replace(_T(".twfb"), _T(""));  // 取消后缀名
	SetDlgItemText(IDC_EDIT_MARKFILE, s);

	// 检查印章图档的头部
	if (!CheckMarkFileNameHead(m_sMarkFileName))
		goto TestError;

	// 开始新的任务
	sFileName.Replace(_T(".twfb"), _T(""));  // 不显示后缀
	StartLog(sFileName);  // 开始Log保存
	PrintMessage(emMsgType_Title, sFileName);

	// 检查图档是否初次使用
	//if (IsMarkFileFirstUse(sFilePath))
	//{
	//	s.Format(_T("警告：该图档[%s]在本机第一次加载"), sFilePath);
	//	CDlgMCCheckWarn dlg(s);
	//	dlg.DoModal();
	//}
	sStampFilePath = sFilePath;
	sStampFilePath = sStampFilePath.Left(sStampFilePath.GetLength() - _tcslen(_T(".twfb"))) + _T(".twd");
	if (!LoadContent(sFilePath, sStampFilePath))
	{
		s.Format(_T("印章图档加载失败！路径：%s"), sFilePath);
		PrintMessage(emMsgType_Error, s);
		goto TestError;
	}
	s.Format(_T("印章图档加载成功！路径：%s"), sFilePath);
	PrintMessage(emMsgType_Success, s);

	if (!VariableSubstitution(mapValue)) {
		goto TestError;
	}

	// 更新消息
	UpdateShowView();

	SetStaticText(IDC_STATIC_FILE_NAME, sFileName);
	// 自动开始JOB
	m_bScanBarcode = TRUE;
	m_bSaveOriginDoc = TRUE;
	m_bCCDOffseted = FALSE;
	OnBnClickedButtonLoadJob();

	WritePrivateProfileString(_T("MarkFile"), sFilePath, _T("1"), g_sTempConfigPath);

	return emREC_OK;

TestError:

	// 清空状态
	ResetStatus();

	return -1;
}

int32_t CFormWorkControl::doFinishLot(char *sRecvBuff, int32_t iSize)
{
	CString s;
	RemoveCR(sRecvBuff, iSize);
	s.Format(_T("远程接收内容：%s"), CString(sRecvBuff));
	PrintMessage(emMsgType_Normal, s);

	if (IsMarking())
	{
		AfxMessageBox(_T("正在标记中，不能结批！"));
		return -1;
	}

	// 清空状态
	ResetStatus();

	// 取消JOB
	OnBnClickedButtonCanceljob();
	GetDlgItem(IDC_BUTTON_LOAD_JOB)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LOAD_SET)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LASER_PARAM)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_MAP_ADJUST)->EnableWindow(FALSE);

	PrintMessage(emMsgType_Success,  _T("结批成功！"));
	return 0;
}

int32_t CFormWorkControl::doReciveSetVisionResult(char *sRecvBuff, int32_t iSize)
{
	CString s;
	RemoveCR(sRecvBuff, iSize);
	s.Format(_T("Vision->LASER：%s"), CString(sRecvBuff));
	PrintMessage(emMsgType_Normal, s);

	CStringA as(sRecvBuff);
	m_bSetVisionOK = (as.Find("OK") > -1);
	
	return 0;
}

int32_t CFormWorkControl::doReciveSetCharCheckResult(char *sRecvBuff, int32_t iSize)
{
	CString s;
	RemoveCR(sRecvBuff, iSize);
	s.Format(_T("视觉检测->LASER：%s"), CString(sRecvBuff));
	PrintMessage(emMsgType_Normal, s);

	CStringA as(sRecvBuff);
	m_bSwitchVisionCheckProcessOK = (as.Find("OK") > -1);

	return 0;
}

int32_t CFormWorkControl::doRecivePLCSwitchProcessResult(char *sRecvBuff, int32_t iSize)
{
	CString s;
	RemoveCR(sRecvBuff, iSize);
	s.Format(_T("PLC->LASER：%s"), CString(sRecvBuff));
	PrintMessage(emMsgType_Normal, s);

	CStringA as(sRecvBuff);
	m_bPLCSwitchProcessOK = (as.Find("OK") > -1);

	return 0;
}

int32_t CFormWorkControl::doCheckMarkStatus(char *sRecvBuff, int32_t iSize)
{
	//CString s;
	//s.Format(_T("接收内容：%s, 数据大小：%d"), CString(sRecvBuff), strlen(sRecvBuff));
	//s.Format(_T("接收内容：%s"), CString(sRecvBuff));
	//PrintMessage(emMsgType_Normal, s);
	//if (!IsMarking())
	//	return emREC_OK;
	//else
	//	return -1;
	if (-1 == m_nMarkFlag)   // 代表已经标记完成
		return emREC_OK;
	else
		return -1;
}


/***
* DEVICEID;<OriginLocation>;<Dev1>;<Dev2>;...;<DevN>
*/
int32_t CFormWorkControl::do2DData(char *sRecvBuff, int32_t iSize)
{
	CString s;
	RemoveCR(sRecvBuff, iSize);
	s.Format(_T("接收内容：%s\r\n接收数据大小：%dbyte"), CString(sRecvBuff), strlen(sRecvBuff));
	PrintMessage(emMsgType_Normal, s);

	// 判断是否正在标记
	if (IsMarking())
	{
		PrintMessage(emMsgType_Error, _T("正在打标中，2D Barcode数据替换失败！"));
		return -1;
	}

	if (NULL == m_pWaferDoc || NULL == m_pMapDoc)
	{
		PrintMessage(emMsgType_Error, _T("无印章图档，2D Barcode数据替换失败！"));
		return -1;
	}
	CWaferGroup * pWaferGroup = m_pMapDoc->GetGroup(0);
	if (NULL == pWaferGroup)
	{
		PrintMessage(emMsgType_Error, _T("无印章阵列图，2D Barcode数据替换失败！"));
		return -1;
	}

	int32_t iError(emREC_OK);

	// 清空之前的数据
	Clear2DData();

	CStringArray sArray;
	if (PraserFrame(sRecvBuff, iSize, sArray) > 1)
	{
		// 第一步，判断传入的芯片数量是否与MAP中的芯片数量一致
		int nSendCount = sArray.GetSize() - 2;   // 其中前两个元素（DEVICEID;0;）忽略
		int nMapCount = pWaferGroup->vWaferMetas.size();
		if (nSendCount != nMapCount)
		{
			PrintMessage(emMsgType_Error, _T("阵列中的模板数量与传入的模板数量不一致！"));
			return -1;
		}
		s.Format(_T("印章数量：%d。"), nMapCount);
		PrintMessage(emMsgType_Normal, s);

		// 第二步，保存所有的2D数据
		for (int i = 0; i < nMapCount; i++)
		{
			m_vs2DData.push_back(sArray.GetAt(i + 2));
		}
	}
	else
		PrintMessage(emMsgType_Error, _T("内容解析失败！"));

	if (-1 == iError)
		PrintMessage(emMsgType_Error, _T("2D Barcode数据替换失败！"));
	else
		PrintMessage(emMsgType_Success, _T("2D Barcode数据替换成功！"));

	//for (auto & item : m_vs2DData) {
	//	PrintMessage(emMsgType_Normal, item);
	//}

	return iError;
}

/**
* POSMATRIX;X;Y;Angle     // x,y,Angle都是相对角度
* 
*/
int32_t CFormWorkControl::doPosMatrix(char *sRecvBuff, int32_t iSize)
{
	int nRet = -1;
	CString s;
	s.Format(_T("接收内容：%s"), CString(sRecvBuff));
	PrintMessage(emMsgType_Normal, s);

	// 判断是否正在标记
	if (IsMarking())
	{
		PrintMessage(emMsgType_Error, _T("正在标记中，阵列位置修改失败！"));
		return -1;
	}

	// 判断是否CancelJob
	if (IsCancelJob())
	{
		PrintMessage(emMsgType_Error, _T("当前处于“取消作业”状态，请点击“开始作业”！"));
		return -1;
	}

	CStringArray sArray;
	if (PraserFrame(sRecvBuff, iSize, sArray) < 3)
	{
		s.Format(_T("阵列位置指令解析失败！接收内容格式不正确"));
		PrintMessage(emMsgType_Error, s);
		return -1;
	}
	//s.Format(_T("解析接收到的数据为：x=%s,y=%s,a=%s"), sArray.GetAt(0), sArray.GetAt(1), sArray.GetAt(2));
	//PrintMessage(emMsgType_Success, s);
	double dx = _ttof(sArray.GetAt(0));
	double dy = _ttof(sArray.GetAt(1));
	double dbAngle = _ttof(sArray.GetAt(2));
	if (fabs(dx) > 0.5 || fabs(dy) > 0.5 || fabs(dbAngle) > 0.5)
	{
		s.Format(_T("CCD偏移值太大，超过0.5mm"));
		PrintMessage(emMsgType_Error, s);
		return -1;
	}

	// 图档偏移
	CWaferMapDoc * pMapDoc = CWaferEngine::Instance()->GetMapDoc();
	int nSize = pMapDoc->GetGroupCount();
	if (0 == nSize)
	{
		s.Format(_T("印章图档中不存在阵列，阵列位置修改失败！"));
		PrintMessage(emMsgType_Error, s);
		return -1;
	}

	if (m_bSaveOriginDoc) {
		if (!SaveOriginDoc()) {
			s.Format(_T("保存原始文档失败！"));
			PrintMessage(emMsgType_Error, s);
			return -1;
		}
		m_bSaveOriginDoc = FALSE;
		m_bCCDOffseted = FALSE;
	}
	else
	{
		if (!ResetOriginDoc())
			return -1;
	}

	// 图像变换
	for (int i = 0; i < nSize; i++)
	{
		CWaferGroup * pGroup = pMapDoc->GetGroup(i);
		if (NULL == pGroup)
			continue;

		// 求阵列的旋转中心
		// 先求出阵列框架没有偏移之前的左下角坐标
		// 然后将其旋转一个角度，阵列角度
		xArray_t xArray;
		pGroup->GetArrayInfo(xArray);
		DPoint_t dbLB(xArray.group.dbCenterX - xArray.group.dbGroupW * 0.5, xArray.group.dbCenterY - xArray.group.dbGroupH * 0.5);
		dbLB.rotate(xArray.group.dbCenterX, xArray.group.dbCenterY, xArray.group.dbGroupDeg);


		// 先偏移再旋转
		if (!pGroup->Repos(dx, dy))
		{
			PrintMessage(emMsgType_Error, _T("阵列位置修改失败！"));
			return -1;
		}


#if 1
		dbLB.x += dx;
		dbLB.y += dy;
#else
		m_pConfig->m_dpRotateCenter.x += dx;
		m_pConfig->m_dpRotateCenter.y += dy;
#endif
		//xArray.group.dbCenterX += dx;
		//xArray.group.dbCenterY += dy;

		if (!pGroup->Rotate(dbLB.x, dbLB.y, dbAngle))
		{
			PrintMessage(emMsgType_Error, _T("阵列位置修改失败！"));
			return -1;
		}
        
		//xArray.group.dbGroupDeg += dbAngle;

		//pGroup->SetArrayInfo(xArray);

		// 注意：CCD定位后，需要重新更新MAP中的BIN，因为在CCD定位前，会先加载原始文档ResetOriginDoc()，所以在doBin中的替换已经失效
		for (int i = 0; i < m_iBinMap.size(); i++) {
			pGroup->vWaferMetas[i].SetWaferID(m_iBinMap[i]);
		}

		// 如果是Dummy模式，则MAPPING不需要改变单颗的打印状态，如果不是Dummy模式，MAPPING则需要全部改为标记状态
		if (!IsDummy())
		{
			for (int i = 0; i < m_iBinMap.size(); i++) {
				pGroup->vWaferMetas[i].EnableMark(true);
			}
		}
	}

	m_pMarkContentView->Invalidate(FALSE);

	PrintMessage(emMsgType_Success, _T("阵列位置修改成功！"));
	m_bCCDOffseted = TRUE;

	return emREC_OK;
}


void CFormWorkControl::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	__super::OnClose();
}

void CFormWorkControl::ClearDialog()
{
	EndLog();  // 关闭log文件

	StopServer();
	StopServerPLC();
	StopVisionCheckServer();
	m_pFactoryMesComm->DisConnect();

	UpdateLaserDiodeTime(false);
}

void CFormWorkControl::UpdateSet()
{
	// 提示标语
	//if (m_pConfig->m_bOpenRollTip)
	//{
	//	m_ctrlTips.ShowWindow(TRUE);
	//	m_ctrlTips.ShowText(m_pConfig->m_sWarnTips);
	//}
	//else
	//{
	//	m_ctrlTips.ShowWindow(FALSE);
	//	m_ctrlTips.ShowText(_T(""));
	//}

	// 模式切换
	if (m_nPreWorkMode != m_pConfig->m_nWorkMode)
	{
		SwitchWorkMode(m_pConfig->m_nWorkMode);
		 // 模式被改变， 重置所有区域
		ResetStatus();
		m_nPreWorkMode = m_pConfig->m_nWorkMode;
	}
	SetMarkCount(m_nMarkCount);   // 为了更新最大打印条数

	//m_pMarkDriver->WriteExtPortPin(m_pConfig->m_nBusyPort, IO_OUT_INVALID);
	m_pMarkDriver->WriteExtPortPin(m_pConfig->m_nEndPort, IO_OUT_VALID);
	//m_pMarkDriver->WriteExtPortPin(m_pConfig->m_nReadyPort, IO_OUT_INVALID);
#ifdef DEBUG_IO
	SetDlgItemInt(IDC_EDIT_IO_FINISH, !m_pConfig->m_nFinishMarkSignalType);
#endif

	if (emWorkMode_Scan == m_pConfig->m_nWorkMode)
	{
		StopServer();
		StopServerPLC();
		StopVisionCheckServer();
		m_pFactoryMesComm->DisConnect();
		// 创建客户端
		if (0 != m_pFactoryMesComm->Connect(CStringA(m_pConfig->m_sMesIP).GetString(), m_pConfig->m_nPort))
		{
			PrintMessage(emMsgType_Error, _T("MES系统连接失败！"));
		}
	}
	else if (emWorkMode_Auto == m_pConfig->m_nWorkMode
		|| emWorkMode_Manual == m_pConfig->m_nWorkMode)
	{
		m_pFactoryMesComm->DisConnect();
		if (m_nPrePort != m_pConfig->m_nPort || nullptr == m_pServer)
			StartServer(m_pConfig->m_nPort);

		if (m_nPrePortPLC != m_pConfig->m_nPortPLC || nullptr == m_pServerPLC)
			StartServerPLC(m_pConfig->m_nPortPLC);

		if (m_pConfig->m_bVisionCheck)
		{
			if (m_nPreVisionPort != m_pConfig->m_iVisionCheckPort || nullptr == m_pVisionCheckServer)
				StartVisionCheckServer(m_pConfig->m_iVisionCheckPort);
		}
		if (m_pConfig->m_bCheckFinsPLC)
		{
			if (m_nPreFinsPort != m_pConfig->m_iFinsPort || m_nPreFinsIp!= m_pConfig->m_sFinsIP)
			{
				StartFinsClientPLC(m_pConfig->m_sFinsIP, m_pConfig->m_iFinsPort);
			}
			
		}

	}
	else if (emWorkMode_Ponum == m_pConfig->m_nWorkMode)
	{
		StopServer();
		StopServerPLC();
		StopVisionCheckServer();
		m_pFactoryMesComm->DisConnect();
	}

	//更新 启用mapping、2D扫码器的状态
	UpdateIOStatus(UISIGNAL_VISION);
	UpdateIOStatus(UISIGNAL_MAP);
	UpdateIOStatus(UISIGNAL_2D);
	UpdateIOStatus(UISIGNAL_PWS);
	UpdateIOStatus(UISIGNAL_NET);
}

BOOL CFormWorkControl::ContectTest(const char * ip, unsigned short uport)
{
	StopServer();
	StopServerPLC();
	StopVisionCheckServer();
	m_pFactoryMesComm->DisConnect();
	// 创建客户端
	if (0 != m_pFactoryMesComm->Connect(ip, uport))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CFormWorkControl::CheckClose()
{
	if (!RightConfirmBox(USER_O_CLOSE_SW))
		return FALSE;

	if (IsMarking())
	{
		AfxMessageBox(_T("正在标记中！请停止标记后再关闭窗口！"));
		return FALSE;
	}
	if (1 == m_nLotFlag)
	{
		AfxMessageBox(_T("尚未结批！请结批后再关闭窗口！"));
		return FALSE;
	}

	if ((!GetDlgItem(IDC_BUTTON_LOAD_JOB)->IsWindowEnabled() && GetDlgItem(IDC_BUTTON_CANCELJOB)->IsWindowEnabled()))
	{
		AfxMessageBox(_T("作业已加载，请取消作业后，再关闭软件！"));
		return FALSE;
	}

	if (IDYES != MessageBox(_T("确认是否关闭窗口！"), _T("提示"), MB_YESNO | MB_DEFBUTTON2))
		return FALSE;

	ClearDialog();

	//m_pMarkDriver->WriteExtPortPin(m_pConfig->m_nBusyPort, IO_OUT_INVALID);
	m_pMarkDriver->WriteExtPortPin(m_pConfig->m_nEndPort, IO_OUT_INVALID);
	m_pMarkDriver->WriteExtPortPin(m_pConfig->m_nReadyPort, IO_OUT_INVALID);

	m_pConfig->SaveConfig();
	CConfigure::Instance()->Delete();
	m_pMarkEngine->Close();

	return TRUE;
}

BOOL CFormWorkControl::IsSwitchSetTab()
{
	// 如果正在标记或者已经开批将不能切换，loadjob后禁止切换
	if (IsMarking() || 1 == m_nLotFlag 
		|| (!GetDlgItem(IDC_BUTTON_LOAD_JOB)->IsWindowEnabled() && GetDlgItem(IDC_BUTTON_CANCELJOB)->IsWindowEnabled()))
	{
		return FALSE;
	}

	// 不清空图档，不能进入基本设置
	if (m_bScanBarcode)
	{
		AfxMessageBox(_T("清空图档后，再进入系统设置！"));
		return FALSE;
	}

	CheckTextSave();   // 加载作业前，检查是否已经保存文本对象变量

	return TRUE;
}

void CFormWorkControl::SetMarkCount(int n)
{
	CString s;
	m_nMarkCount = n;
	if(emWorkMode_Manual != m_pConfig->m_nWorkMode && m_pConfig->m_bMaxMarkTimesLimit)
		s.Format(_T("%04d(%d)"), m_nMarkCount, m_pConfig->m_nMaxMarkTimes);
	else 
		s.Format(_T("%04d"), m_nMarkCount);
	
	m_pMainDlg->SetStaticText(IDC_MARKCOUNT, s);
}

BOOL CFormWorkControl::StartLot(const CString & sOperID, const CString & sLotID)
{
	CString s;
	// 开始批次信息上传
	PrintMessage(emMsgType_Normal, _T("上传开始批次信息！"));

	char szSendbuff[1024];
	sprintf_s(szSendbuff, "ASSY MKBS2<LOT_ID,%s><OPER_ID,%s>", CStringA(sLotID).GetString(), CStringA(sOperID).GetString());
	s.Format(_T("传送指令：%s。等待返回..."), CString(szSendbuff));
	PrintMessage(emMsgType_Normal, s);
	char sRet[1024] = { 0 };
	bool bRet = m_pFactoryMesComm->SendCmd(szSendbuff, "ASSY MKBR", sRet, sizeof(sRet));
	s.Format(_T("接收内容：%s"), chU2T(sRet).c_str());
	PrintMessage(emMsgType_Normal, s);
	if (!bRet)
	{
		PrintMessage(emMsgType_Error, _T("指令发送失败或未收到正确消息！"));
		return FALSE;
	}

	// 开批
	if (0 != doStartLot(sRet, strlen(sRet)))
		return FALSE;

	return TRUE;
}

BOOL CFormWorkControl::ResetMapOffsetFile()
{
	// 没有阵列图，返回
	if (NULL == m_pWaferDoc || NULL == m_pMapDoc)
		return FALSE;

	CWaferGroup * pWaferGroup = m_pMapDoc->GetGroup(0);
	if (NULL == pWaferGroup)
		return FALSE;
	
	// 分配内存
	stOffsetValue_t * stOffsetValue;
	int nCellCount = pWaferGroup->vWaferMetas.size();
	stOffsetValue = new stOffsetValue_t[nCellCount]();
	CStdioFile file;
	if (!file.Open(m_sMobFilePath, CFile::modeRead | CFile::modeCreate | CFile::modeNoTruncate))
	{
		delete[] stOffsetValue;
		PrintMessage(emMsgType_Error, _T("阵列修正文件打开失败！"));
		return FALSE;
	}

	// 自定义格式：index,x,y,a
	CString s;
	while (file.ReadString(s))
	{
		int index;
		std::vector<TString> vs;
		chParseString(s.GetString(), vs, _T(','));
		if (vs.size() != 4)
			continue;
		index = _ttoi(vs[0].c_str());
		if (index < nCellCount && index >= 0)
		{
			stOffsetValue[index].dx = _ttof(vs[1].c_str());
			stOffsetValue[index].dy = _ttof(vs[2].c_str());
			stOffsetValue[index].deg = _ttof(vs[3].c_str());
		}
	}

	// 修改阵列, 记住是反方向
	for (int i = 0; i < nCellCount; i++)
	{
		if (!(D_EQUAL(stOffsetValue[i].dx, 0)
			&& D_EQUAL(stOffsetValue[i].dy, 0)))
			pWaferGroup->vWaferMetas[i].xDim.Repos(-stOffsetValue[i].dx, -stOffsetValue[i].dy);
		if (!D_EQUAL(stOffsetValue[i].deg, 0))
			pWaferGroup->vWaferMetas[i].xDim.Rotate(-stOffsetValue[i].deg);
	}
	m_pMarkContentView->Invalidate();

	file.Close();
	
	delete[] stOffsetValue;
	return TRUE;
}

BOOL CFormWorkControl::LoadMapOffsetFile(const CString & sFileName)
{
	// 没有阵列图，返回
	if (NULL == m_pWaferDoc || NULL == m_pMapDoc)
		return FALSE;

	CWaferGroup * pWaferGroup = m_pMapDoc->GetGroup(0);
	if (NULL == pWaferGroup)
	{
		PrintMessage(emMsgType_Error, _T("图档中缺少阵列！"));
		return FALSE;
	}
		
	// 分配内存
	stOffsetValue_t * stOffsetValue;
	int nCellCount = pWaferGroup->vWaferMetas.size();
	stOffsetValue = new stOffsetValue_t[nCellCount]();
	// 初始化默认偏移数据
	CStdioFile file;
	if (!file.Open(sFileName, CFile::modeRead | CFile::modeCreate | CFile::modeNoTruncate))
	{
		delete[] stOffsetValue;
		PrintMessage(emMsgType_Error, _T("阵列修正文件打开失败！"));
		return FALSE;
	}

	// 自定义格式：index,x,y,a
	CString s;
	while (file.ReadString(s))
	{
		int index;
		std::vector<TString> vs;
		chParseString(s.GetString(), vs, _T(','));
		if (vs.size() != 4)
			continue;
		index = _ttoi(vs[0].c_str());
		if (index < nCellCount && index >= 0)
		{
			stOffsetValue[index].dx = _ttof(vs[1].c_str());
			stOffsetValue[index].dy = _ttof(vs[2].c_str());
			stOffsetValue[index].deg = _ttof(vs[3].c_str());
		}
	}
	file.Close();

	// 修改阵列
	for (int i = 0; i < nCellCount; i++)
	{
		if (!(D_EQUAL(stOffsetValue[i].dx, 0)
			&& D_EQUAL(stOffsetValue[i].dy, 0)))
			pWaferGroup->vWaferMetas[i].xDim.Repos(stOffsetValue[i].dx, stOffsetValue[i].dy);
		if (!D_EQUAL(stOffsetValue[i].deg, 0))
			pWaferGroup->vWaferMetas[i].xDim.Rotate(stOffsetValue[i].deg);
	}
	m_pMarkContentView->Invalidate();

	delete[] stOffsetValue;
	return TRUE;
}

BOOL CFormWorkControl::FindNewMobFile()
{
	// 在指定文件夹中，找到最新的阵列修正文件
	// 阵列修正文件格式：指定路径 + 打印图档名 + "_" + 日期（年月日时分秒 + 修正文件后缀（.mob）
	WIN32_FIND_DATA filedata;
	CString sFileFilter, sNewFile;
	FILETIME filetime;
	m_sMobFilePath.Empty();   // 清空阵列修正文件
	sFileFilter.Format(_T("%s\\%s*.mob"), CFactoryConfig::Instance()->m_sMobFilePath, m_sMarkFileName);
	HANDLE hFile = FindFirstFile(sFileFilter, &filedata);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		// 未找到相关文件，分配一个初始文件名，但是不创建文件，等到后面打开的时候再创建
		goto TestError;
	}

	filetime = filedata.ftLastWriteTime;
	sNewFile = filedata.cFileName;
	while (FindNextFile(hFile, &filedata))
	{
		if (-1 == CompareFileTime(&filetime, &filedata.ftLastWriteTime))
		{
			// filetime 比 filedata.ftLastWriteTime早
			filetime = filedata.ftLastWriteTime;
			sNewFile = filedata.cFileName;
		}
	}
	FindClose(hFile); 
		
	if (sNewFile.IsEmpty())
		goto TestError;

	m_sMobFilePath.Format(_T("%s\\%s"), CFactoryConfig::Instance()->m_sMobFilePath, sNewFile);

	return TRUE;

TestError:
	// 防止阵列修正文件被意外删除后，重新加载图档，而没有提示，导致阵列位置异常
	if (IsMarkFileFirstUse(m_sMarkFilePath) ? TRUE :
		(IDYES == MessageBox(_T("警告：该图档对应的阵列修正文件丢失，是否重新创建？"), _T("警告"), MB_YESNO | MB_DEFBUTTON2 | MB_ICONERROR)))
	{
		CTime currentTime = CTime::GetCurrentTime();
		CString timeYmdHMS = currentTime.Format(_T("_%Y_%m_%d_%H%M%S"));
		m_sMobFilePath = CFactoryConfig::Instance()->m_sMobFilePath +
			_T("\\") + m_sMarkFileName + timeYmdHMS + g_sMapOffsetfile;
		return TRUE;
	}
	return FALSE; 
}

void CFormWorkControl::FinishMark(int nflag)
{
	BOOL bExit = FALSE;
	while (!bExit)
	{
		// 标记完成后检测,此处肯定已经标记完成，不再检查
		//if (!IsMarking())
		{
			bExit = TRUE;

			CString s;
			DWORD dwTime = GetTickCount() - m_dwStartTick;
			m_dwStartTick = 0;
			s.Format(_T("%d.%d"), dwTime / 1000, dwTime % 1000);
			m_dbMarkTime = _ttof(s);
			SetMarkCount(++m_nMarkCount);
			s.Format(_T("标记完成。标记数量：%d，标记时间：%.3fs。"), m_nMarkCount, m_dbMarkTime);
			PrintMessage(emMsgType_Normal, s);
			
			m_nMarkFlag = -1;   // 完成标记

			if (((0 == nflag) || m_bManualStopMark) && m_bSysOK)
			{
				// 标记完成，记录StripID
				RecordMarkedStripID();

				// 标记完成后，EDO6信号输出
				m_pMarkDriver->WriteExtPortPin(m_pConfig->m_nEndPort, IO_OUT_VALID);
				//m_pMarkDriver->WriteExtPortPin(m_pConfig->m_nBusyPort, IO_OUT_INVALID);
#ifdef DEBUG_IO
				SetDlgItemInt(IDC_EDIT_IO_FINISH, !m_pConfig->m_nFinishMarkSignalType);
#endif           
				// “扫入Lot后，自动加载本地图档”模式，判断是否需要送检MC
				if ((emWorkMode_Scan == m_pConfig->m_nWorkMode
					|| emWorkMode_Auto == m_pConfig->m_nWorkMode
					|| emWorkMode_Ponum == m_pConfig->m_nWorkMode)
					&& m_pConfig->m_bMCCheckWarn
					&& m_bScanBarcode)
				{
					m_bScanBarcode = FALSE;
					if (IsMCCheck())
					{
						CDlgMCCheckWarn dlg(m_pConfig->m_sMCCheckTip, true);
						if (IDOK != dlg.DoModal())
						{
							PrintMessage(emMsgType_Normal, _T("退出作业，请进行首检"));
						}
						// 如果满足送检MC文件
						OnBnClickedButtonCanceljob();
					}
					else
					{
						// 点击停止标记按钮或手动标记后，退出自动模式
						if (m_bManualStopMark || m_bManualStartMark)
							OnBnClickedButtonCanceljob();
						else
							// 结束标记后，自动开启JOB
							OnBnClickedButtonLoadJob();
					}
				}
				else
				{
					// 点击停止标记按钮或手动标记后，退出自动模式
					if (m_bManualStopMark || m_bManualStartMark)
						OnBnClickedButtonCanceljob();
					else
						// 结束标记后，自动开启JOB
						OnBnClickedButtonLoadJob();
				}
			}
			else
			{
				// CancelJob
				OnBnClickedButtonCanceljob();
			}


			//if (emWorkMode_Manual == m_pConfig->m_nWorkMode)
			//{
			//	GetDlgItem(IDC_BUTTON_STARTMARK)->EnableWindow(TRUE);
			//}

			GetDlgItem(IDC_BUTTON_CELL)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_AREA)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_MATRIX)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_STOPMARK)->EnableWindow(FALSE);
		}
	}
}

BOOL CFormWorkControl::DeleteOldMobFile()
{
	// 在指定文件夹中，找到最旧的阵列修正文件，
	// 阵列修正文件格式：指定路径 + 打印图档名 + "_" + 日期（年月日时分秒 + 修正文件后缀（.mob）
	WIN32_FIND_DATA filedata;
	CString sFileFilter, sOldFile;
	FILETIME filetime;
	int nFileCount = 0;  // 文件数量
	sFileFilter.Format(_T("%s\\%s*.mob"), m_pConfig->m_sMobFilePath, m_sMarkFileName);
	HANDLE hFile = FindFirstFile(sFileFilter, &filedata);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		// 未找到相关文件，分配一个初始文件名，但是不创建文件，等到后面打开的时候再创建
		return FALSE;
	}

	nFileCount++;
	filetime = filedata.ftLastWriteTime;
	sOldFile = filedata.cFileName;
	while (FindNextFile(hFile, &filedata))
	{
		nFileCount++;
		if (1 == CompareFileTime(&filetime, &filedata.ftLastWriteTime))
		{
			// filetime 比 filedata.ftLastWriteTime大
			filetime = filedata.ftLastWriteTime;
			sOldFile = filedata.cFileName;
		}
	}
	FindClose(hFile);

	if (nFileCount > 3) {
		sOldFile.Format(_T("%s\\%s"), m_pConfig->m_sMobFilePath, sOldFile);
		DeleteFile(sOldFile);
	}

	return TRUE;
}

BOOL CFormWorkControl::CopyMobFile(const CString & sOldFile, const CString & sNewFile)
{
	if (sOldFile.IsEmpty() || sNewFile.IsEmpty())
		return FALSE;

	CFile oldfile, newfile;
	if (!oldfile.Open(sOldFile, CFile::modeRead)) {
		return FALSE;
	}
	ULONGLONG ulFileSize = oldfile.GetLength();
	if (0 == ulFileSize) {
		if (!newfile.Open(sNewFile, CFile::modeWrite | CFile::modeCreate)) {
			oldfile.Close();
			return FALSE;
		}
		newfile.Close();
		return TRUE;
	}

	// 读取旧文件内容
	oldfile.SeekToBegin();
	char * buff = new char[ulFileSize];
	int nRet = oldfile.Read(buff, ulFileSize);
	if (nRet <= 0) {
		oldfile.Close();
		delete[] buff;
		return FALSE;
	}
	oldfile.Close();

	// 写入新的文件
	if (!newfile.Open(sNewFile, CFile::modeWrite | CFile::modeCreate)) {
		oldfile.Close();
		delete[] buff;
		return FALSE;
	}
	newfile.Write(buff, ulFileSize);
	newfile.Close();
	delete[] buff;

	return TRUE;
}

void CFormWorkControl::SetMarkStatus()
{
	CancelJob();

	// 开始标记后，禁止所有按钮
	GetDlgItem(IDC_BUTTON_STARTMARK)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LOAD_JOB)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CANCELJOB)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CELL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_AREA)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_MATRIX)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_MAP_ADJUST)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LOAD_SET)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LASER_PARAM)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOPMARK)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_EDIT_TEXT_CONTONT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_FINISH_EDIT_CONTENT)->EnableWindow(FALSE);

	// 标记状态设置为1
	m_nMarkFlag = 1;   
	m_bManualStopMark = FALSE;
}


void CFormWorkControl::LoadJob()
{
	m_bEnablePLCCheck = TRUE;
	//m_pMarkDriver->WriteExtPortPin(m_pConfig->m_nReadyPort, IO_OUT_VALID);
	if (emWorkMode_Auto == m_pConfig->m_nWorkMode) 
	{
		// Normal模式,勾选启用2D扫描器时，开启2D扫描定时器
		// Card和Unit和Map强制启用2D扫描器
		// 开启mapping强制启用2D扫码器
		if ((m_pConfig->m_bCheck2DBarcode &&  g_sMTNormal == m_markType)
			|| g_sMTCard == m_markType
			|| g_sMTUnit == m_markType
			|| g_sMTCardAndUnit == m_markType
			|| (m_pConfig->m_bCheckStripMap && !IsDummy())
			/*|| m_pConfig->m_bCheckEQPStripIdIsMatchLotId*/)
		{
			if (emScannerType_None == m_pConfig->m_emUseScan)
			{
				PrintMessage(emMsgType_Error, _T("未开启2D扫描枪，请前往【系统设置-扫描枪设置】中启用2D扫描枪"));
				return;
			}
#ifdef ENABLE_SCAN_THREAD
			m_Scan2DSignal.StartDetect();
#else
			SetTimer(SCANSIGNAL_CHECK_TIMER, 100, nullptr);
#endif
			PrintMessage(emMsgType_Normal, _T("正在检测2D扫描信号..."));
			//SetCommIOStatus(m_pConfig->m_nScannReadySingle, IO_OUT_VALID);
			SetCommIOStatus(m_pConfig->m_nScannOKSignal, IO_2D_OUT_INVALID);
			SetCommIOStatus(m_pConfig->m_nScannNGSignal, IO_2D_OUT_INVALID);
			return;
		}
	}
	PrintMessage(emMsgType_Normal, _T("正在检测标记信号..."));
	//SetTimer(5, 100, nullptr);  //循环检测9608的标记信号
	m_StartMarkSingal.StartMonitor();
}

void CFormWorkControl::CancelJob()
{
	m_bEnablePLCCheck = FALSE;
	if (emWorkMode_Auto == m_pConfig->m_nWorkMode) {
		// Normal模式,勾选启用2D扫描器时，开启2D扫描定时器
		// Card和Unit和Map强制启用2D扫描器
		if ((m_pConfig->m_bCheck2DBarcode &&  g_sMTNormal == m_markType)
			|| g_sMTCard == m_markType
			|| g_sMTUnit == m_markType
			|| g_sMTCardAndUnit == m_markType
			|| (m_pConfig->m_bCheckStripMap && !IsDummy())
			/*|| m_pConfig->m_bCheckEQPStripIdIsMatchLotId*/)
		{
			PrintMessage(emMsgType_Normal, _T("2D扫描信号停止检测"));
			//SetCommIOStatus(m_pConfig->m_nScannReadySingle, IO_OUT_INVALID);
			SetCommIOStatus(m_pConfig->m_nScannOKSignal, IO_2D_OUT_INVALID);
			SetCommIOStatus(m_pConfig->m_nScannNGSignal, IO_2D_OUT_INVALID);
#ifdef ENABLE_SCAN_THREAD
			m_Scan2DSignal.EndDetect();
#else
			KillTimer(SCANSIGNAL_CHECK_TIMER);
#endif	
		}
	}

	//KillTimer(5);
	m_StartMarkSingal.StopMonitor();
	PrintMessage(emMsgType_Normal, _T("标记信号停止检测"));
}

BOOL CFormWorkControl::CheckMarkFileNameHead(const CString & sFileName)
{
	CFactoryConfig * pFactoryConfig = CFactoryConfig::Instance();
	int nHeadLen = pFactoryConfig->m_sBarcodeHead.GetLength();
	if (pFactoryConfig->m_sBarcodeHead == sFileName.Left(nHeadLen))
		return TRUE;
	
	// 对比失败
	CString s;
	s.Format(_T("图档名头部检查失败！系统设置的图档头部[%s]与实际图档名[%s]不符合。"),
		pFactoryConfig->m_sBarcodeHead, sFileName);
	PrintMessage(emMsgType_Error, s);
	return FALSE;
}

BOOL CFormWorkControl::IsMCCheck()
{
	if (CFactoryConfig::Instance()->m_nRetryTimes == m_nRetryTimes
		|| m_sCheckMarkFile != m_sMarkFileName)
	{
		m_nRetryTimes = 1;
		m_sCheckMarkFile = m_sMarkFileName;
		CString s;
		s.Format(_T("%d"), m_nRetryTimes);
		WritePrivateProfileString(_T("Global"), _T("RetryTimes"), s, g_sTempConfigPath);
		WritePrivateProfileString(_T("Global"), _T("CheckMarkFile"), m_sMarkFileName, g_sTempConfigPath);
		return TRUE;
	}
	if (m_sCheckMarkFile == m_sMarkFileName)
	{
		m_nRetryTimes++;
		CString s;
		s.Format(_T("%d"), m_nRetryTimes);
		WritePrivateProfileString(_T("Global"), _T("RetryTimes"), s, g_sTempConfigPath);
	}
	return FALSE;
}

BOOL CFormWorkControl::LoadStampByPonum(const CString & sMarkStamp)
{
#if 0
    // 从本地配置档TWD-TWFB.txt中查找组装批号对应的图档名
	CString s, sFileName, sFilePath;    // 图档名称
	CString sConfigPath = m_pConfig->m_sMarkFilePath + _T("\\TWD-TWFB.txt");
	TCHAR tchMarkFile[256] = { 0 };
	GetPrivateProfileString(_T("TWD-TWFB"), sMarkStamp, _T(""), tchMarkFile, 256, sConfigPath);
	sFileName = tchMarkFile;
	sFileName.TrimLeft();
	sFileName.TrimRight();
	if (sFileName.IsEmpty())
	{
		PrintMessage(emMsgType_Error, _T("组装批号没有对应的图档号!"));
		goto TestError;
	}
	s.Format(_T("组装批号%s对应的图档名称%s"), sMarkStamp, sFileName);
	PrintMessage(emMsgType_Success, s);

	sFileName += _T(".twfb");
	m_sMarkFileName = sFileName;
	// 检查印章图档的头部
	if (!CheckMarkFileNameHead(m_sMarkFileName))
		goto TestError;

	sFilePath = m_pConfig->m_sMarkFilePath + _T("\\") + sFileName;
	// 检查图档是否初次使用
	if (IsMarkFileFirstUse(sFilePath))
	{
		s.Format(_T("警告：该图档[%s]在本机第一次加载"), sFilePath);
		CDlgMCCheckWarn dlg(s);
		dlg.DoModal();
	}
	if (!LoadContent(sFilePath, m_pConfig->m_sMarkStampPath + _T("\\") + sMarkStamp + _T(".twd")))
	{
		s.Format(_T("印章图档加载失败！路径：%s"), sFilePath);
		PrintMessage(emMsgType_Error, s);
		goto TestError;
	}
	s.Format(_T("印章图档加载成功！路径：%s"), sFilePath);
	PrintMessage(emMsgType_Success, s);

	// 更新消息
	UpdateShowView();

	// 加载印章名称
	sFileName.Replace(_T(".twfb"), _T(""));  // 不显示后缀
	SetStaticText(IDC_STATIC_FILE_NAME, sFileName);

	WritePrivateProfileString(_T("MarkFile"), sFilePath, _T("1"), g_sTempConfigPath);
	return TRUE;

TestError:
#endif
	return FALSE;
}

int CFormWorkControl::VerifyRepeatLotID(const CString & sLotID)
{
	int nLotIDTimes = GetPrivateProfileInt(_T("LotID"), sLotID, 0, _T("D:\\HT LotID Record\\") + CTime::GetCurrentTime().Format(_T("%Y%m")) + _T(".ini"));

	if (nLotIDTimes > 0)
	{
		CString s;
		s.Format(_T("警告：该LotID[%s]在本机已经成功扫描过 %d 次"), sLotID, nLotIDTimes);
		CDlgMCCheckWarn dlg(s, true);
		dlg.DoModal();
		return nLotIDTimes;
	}
	return 0;
}


int CFormWorkControl::VerifyRepeatStripID(const CString & sStripID)
{
	CString tmpLot = m_sLotIDCur.Mid(0, 8);   // Add by hhhuang 20241023 所有的StripID保存在一个主批次号中。下面的m_sLotIDCur用tmpLot代替
	int nStripIDTimes = GetPrivateProfileInt(_T("StripID"), sStripID, 0, _T("D:\\HT StripID Record\\") + tmpLot + _T(".ini"));
	CString s;
	s.Format(_T("StripID[%s]之前扫描次数：%d"), sStripID, nStripIDTimes);
	PrintMessage(emMsgType_Normal, s);
	if (nStripIDTimes > 0)
	{
		CString s;
		s.Format(_T("警告：该StripID[%s]在本机已经成功扫描过 %d 次, 是否继续标记？"), sStripID, nStripIDTimes);
		CDlgMCCheckWarn dlg(s, true, true);
		if (IDOK != dlg.DoModal())
			return -1;
				
		if (!RightConfirmBox(USER_O_STRIPID_REPEAT, true)) 
		{
			return -1;
		}

		// 当出现StripID重复扫描时，设置为 不检查二维码
		EnableFirstPrint(true);

		return nStripIDTimes;
	}
	return 0;
}

void CFormWorkControl::RecordMarkedStripID()
{
	if (m_sStripIDCur.IsEmpty()) return;
	
	CString tmpLot = m_sLotIDCur.Mid(0, 8);   // Add by hhhuang 20241023 所有的StripID保存在一个主批次号中。下面的m_sLotIDCur用tmpLot代替

	int nStripIDTimes = GetPrivateProfileInt(_T("StripID"), m_sStripIDCur, 0, _T("D:\\HT StripID Record\\") + tmpLot + _T(".ini"));
	CString s;
	s.Format(_T("%d"), nStripIDTimes + 1);
	WritePrivateProfileString(_T("StripID"), m_sStripIDCur, s, _T("D:\\HT StripID Record\\") + tmpLot + _T(".ini"));
}

BOOL CFormWorkControl::IsMarkFileFirstUse(const CString & sMarkFile)
{
	int nMarkFileTimes = GetPrivateProfileInt(_T("MarkFile"), sMarkFile, 0, g_sTempConfigPath);

	return (0 == nMarkFileTimes) ? TRUE : FALSE;
}

BOOL CFormWorkControl::IsEqualStampSize()
{
	if (nullptr == m_pMapDoc || nullptr == m_pWaferDoc)
		return FALSE;

	CWaferGroup * pGroup = m_pMapDoc->GetGroup(0);
	CWaferGraph * pWafer = m_pWaferDoc->GetWafer(0);
	if (nullptr == pGroup || nullptr == pWafer)
		return FALSE;

	// 比较阵列文件和印章文件的尺寸大小
	xArray_t xArray;
	pGroup->GetArrayInfo(xArray);

	return (D_EQUAL(xArray.cell.dCellWidth, pWafer->GetWidth())
		&& D_EQUAL(xArray.cell.dCellHight, pWafer->GetHeight()));
}

BOOL CFormWorkControl::IsMaxMarkTimes()
{
	if (! m_pConfig->m_bMaxMarkTimesLimit
		|| emWorkMode_Manual == m_pConfig->m_nWorkMode) 
		return FALSE;

	if (m_nMarkCount >= m_pConfig->m_nMaxMarkTimes)
	{
		// Cancel Job
		OnBnClickedButtonCanceljob();
		PrintMessage(emMsgType_Error, _T("打印条数达到上限，禁止标记！"));
		return TRUE;
	}
	return FALSE;
}

BOOL CFormWorkControl::SaveOriginDoc()
{
	ChangeArrayBin_0();   // 确保保存的阵列都是正常的
	int nRet = m_pMapDoc->CCDAnchor();
	if (0 != nRet) {
		PrintMessage(emMsgType_Error, _T("图档保存错误！"));
		return FALSE;
	}
	return TRUE;
}

BOOL CFormWorkControl::ResetOriginDoc()
{
	int nRet = m_pMapDoc->ResetAnchor();
	if (0 != nRet) {
		PrintMessage(emMsgType_Error, _T("标记图档恢复错误！"));
		return FALSE;
	}
	// Mapping模式，恢复原始图档后，必须要恢复成全选模式
	if (m_pConfig->m_bCheckStripMap || m_markType == g_sMTUnit || m_markType == g_sMTCardAndUnit)
	{
		// 如果是Dummy模式，则不改变打印状态
		if(!IsDummy())m_pMapDoc->SetPrintAll();	
	}
	m_pMarkContentView->Invalidate(FALSE);
	return TRUE;
}

void CFormWorkControl::RemoveCR(char *sRecvBuff, int32_t& iSize)
{
	if (nullptr == sRecvBuff || iSize < 0)
		return;

	while (iSize > 0) {
		if ('\n' == sRecvBuff[iSize - 1] || '\r' == sRecvBuff[iSize - 1]) {
			sRecvBuff[iSize - 1] = '\0';
			iSize--;
		}
		else {
			break;
		}
	}
}

BOOL CFormWorkControl::IsEqualPenName(const std::vector<string> & vsPenName, std::vector<int>& virefLayerID)
{
	if (nullptr == m_pWaferDoc)
		return FALSE;
	for (auto & item : vsPenName)
	{
		int irefLayerID = 0;
		if (!m_pWaferDoc->IsExistLayer(item.c_str(), irefLayerID))
		{
			return FALSE;
		}
		virefLayerID.push_back(irefLayerID);
	}
	return TRUE;
}

void CFormWorkControl::ShowLaserParam()
{

	ClearLaserParamShow();

	m_sPenName.clear();
	m_dbPower.clear();
	m_dbFreq.clear();
	m_nSpeed.clear();
	CString s;
	int32_t nLayerC = m_pWaferDoc->GetLayerCount();
	for (int i = 0; i < nLayerC; i++)
	{
		CWaferGraphLayer * pLayer = m_pWaferDoc->GetLayer(i);
		IMarkPen * pMarkPen = pLayer->GetPen();
		m_sPenName.push_back(CString(pMarkPen->sName));
		m_dbPower.push_back(pMarkPen->dbPower);
		m_dbFreq.push_back(pMarkPen->dbFreq);
		m_nSpeed.push_back(pMarkPen->dwMarkSpeed);

		if (m_LoginUserName != _T("")) {
			s.Format(_T("用户%s 激光参数%d：名称：%s, 功率：%.2f%%, 频率：%.2fKHz, 速度：%dmm/s"),
				m_LoginUserName, i + 1, m_sPenName[i], m_dbPower[i] * 100, m_dbFreq[i], m_nSpeed[i]);
		}
		else {
			s.Format(_T("激光参数%d：名称：%s, 功率：%.2f%%, 频率：%.2fKHz, 速度：%dmm/s"),
				i + 1, m_sPenName[i], m_dbPower[i] * 100, m_dbFreq[i], m_nSpeed[i]);
		}
		PrintMessage(emMsgType_Normal, s);

		//if (i == 0) {
		//	s.Format(_T("激光参数名称：%s, 功率：%.0f%%, 频率：%.0fKHz, 速度：%dmm/s"), m_sPenName[i], m_dbPower[i] * 100, m_dbFreq[i], m_nSpeed[i]);
		//	m_pMainDlg->SetStaticText(IDC_STATIC_LASERPARAM, s);
		//}

		m_listLaserParam.AddString(m_sPenName[i]);
	}

	if (m_sPenName.size() > 0)
	{
		m_listLaserParam.SetCurSel(0);
		OnLbnSelchangeListLaserParam();
	}
}

void CFormWorkControl::Show2DAndMapInfo()
{
	if (nullptr == m_pWaferDoc)
		return;

	CString sText;
	if (m_pWaferDoc->IsExist2DBarcode())
		sText += CString(_T("2D Barcode图档，")) + (m_pConfig->m_bCheck2DBarcode ? _T("检查。") : _T("不检查。"));

	if (m_pWaferDoc->GetStripMap())
		sText += CString(_T("Map图档，")) + (m_pConfig->m_bCheckStripMap ? _T("检查。") : _T("不检查。"));

	if(!sText.IsEmpty())
		PrintMessage(emMsgType_Normal, sText);
}

BOOL CFormWorkControl::Check2DBarcode()
{
	// 只有Unit和CardAndUnit模式才需要检查2DBarcode数据
	if (m_pConfig->m_bCheck2DBarcode && m_pWaferDoc->IsExist2DBarcode() && (m_markType == g_sMTUnit || m_markType == g_sMTCardAndUnit))
		return m_vs2DData.size() >= 1;
	return TRUE;
}

BOOL CFormWorkControl::CheckStripMap()
{
	if (IsMapping())
		return m_iBinMap.size() >= 1;
	else
	{
		ChangeArrayBin_0();   // 所有BIN
		m_pMarkContentView->Invalidate(TRUE);
	}
	return TRUE;
}

void CFormWorkControl::Clear2DData()
{
	m_vs2DData.clear();
	m_vs2DData2.clear();
}

void CFormWorkControl::ClearMapData()
{
	m_iBinMap.clear();

	// 启用MAP后，每次标记完后，需要清空map信息
	if (IsMapping())
	{
		ChangeArrayBin_0();   // 所有BIN
		m_pMarkContentView->Invalidate(TRUE);
	}
}

BOOL CFormWorkControl::IsCancelJob()
{
	return !m_bEnablePLCCheck;
}

void CFormWorkControl::OnBnClickedButtonLaserParam()
{
	if (!RightConfirmBox(USER_O_EDIT_PARAM))
		return;
	// 正在标记和非调试模式，都不能调整印章位置
	if (IsMarking())
		return;

	// 没有阵列图，返回
	if (NULL == m_pWaferDoc || NULL == m_pMapDoc)
		return;

	CString s;
	int32_t nLayerC = m_pWaferDoc->GetLayerCount();
	if (1 == nLayerC) {
		// 当只有一组激光参数时，直接弹出激光参数属性框
		CWaferGraphLayer * pLayer = m_pWaferDoc->GetLayer(0);
		int nPenOptionWndFlag = PenOptionWndFlag_EDIT_PARAM;
		CMarkPlatform::Instance()->MarkDevice()->SetDeviceCtrl(Laser_SetPenOptionWndFlag_I, (void*)&nPenOptionWndFlag);
		if (-1 == CMarkPlatform::Instance()->MarkDevice()->ShowPenOptionWnd(pLayer->GetPen()))
		{
			m_pWaferDoc->UpdateAllLayerFromDB();  // 如果修改失败，应该重新获取本地参数库中的参数，以免出现信息不对等的情况
		}
	}
	else {
		// 当有多组参数时，弹出参数选择框，并将参数指针传递过去
		vector<IMarkPen*> refvMarkPen;
		for (int i = 0; i < nLayerC; i++)
		{
			CWaferGraphLayer * pLayer = m_pWaferDoc->GetLayer(i);
			refvMarkPen.push_back(pLayer->GetPen());
		}
		CDlgSelectLaserParam dlg(refvMarkPen, this);
		dlg.DoModal();
	}
	ShowLaserParam();
	// 自动LoadJob
	if (emWorkMode_Manual != m_pConfig->m_nWorkMode) {
		OnBnClickedButtonLoadJob();
	}
}

void CFormWorkControl::ChangeArrayBin_0()
{
	if (nullptr == m_pWaferDoc || nullptr == m_pMapDoc)
		return;

	int iSize = m_pWaferDoc->GetWaferCount();
	if (iSize <= 1)   // 只有一个BIN，则不需要切换到BIN0
		return;

	const int iBinNo = 0;  // 设置为BIN_0
	m_pMapDoc->ChangeWaferID(iBinNo);
}

void CFormWorkControl::ChangeObjLayerID(const std::vector<int>& viLayerID)
{
	if (nullptr == m_pWaferDoc)
		return;

	int iSize = m_pWaferDoc->GetWaferCount();
	for (int i = 0; i < iSize; i++) {
		CWaferGraph* pWaferGraph = m_pWaferDoc->GetWafer(i);
		if (nullptr == pWaferGraph)
			continue;

		int iObjSize = pWaferGraph->GetObjCount();
		for (int j = 0; j < iObjSize; j++) {
			IGraphObject* pObject = pWaferGraph->GetObject(j);
			if (nullptr == pObject)
				continue;

			/*
			viLayerID，按顺序保存印章模板中Layer对应的阵列文件中的第几层
			viLayerID[0],表示层号为0的pObject，应该要设置的层号（也就是阵列文件中的对应层号）
			viLayerID[1],表示层号为1的pObject，应该要设置的层号（也就是阵列文件中的对应层号）
			*/
			int iLayerIndex = pObject->GetLayer();
			int iNewLayerID = viLayerID[iLayerIndex];
			pObject->SetLayer(iNewLayerID);
		}
	}
	
}

void CFormWorkControl::OnBnClickedButtonEditTextContont()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!RightConfirmBox(USER_O_EDIT_TEXT_CONTENT))
		return;
	
	GetDlgItem(IDC_BUTTON_EDIT_TEXT_CONTONT)->EnableWindow(FALSE);

	// 将所有文本编辑框设置成可编辑状态
	for (int i = 0; i < 10; i++){
		((CEdit*)GetDlgItem(g_nTextName[i]))->SetReadOnly(FALSE);
		((CEdit*)GetDlgItem(g_nTextValue[i]))->SetReadOnly(FALSE);
	}

	GetDlgItem(IDC_BUTTON_FINISH_EDIT_CONTENT)->EnableWindow(TRUE);
}
void CFormWorkControl::OnBnClickedButtonFinishEditContent()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON_FINISH_EDIT_CONTENT)->EnableWindow(FALSE);
	// 将所有文本编辑框设置成可编辑状态
	for (int i = 0; i < 10; i++) {
		((CEdit*)GetDlgItem(g_nTextName[i]))->SetReadOnly(TRUE);
		((CEdit*)GetDlgItem(g_nTextValue[i]))->SetReadOnly(TRUE);
	}
	// 获取所有修改的文本内容，然后进行修改
	// 根据文本名称进行修改，注意：如果出现重名对象，则会出现不确定异常。不能有重名对象
	std::map<CString, CString> mapValue;  // 保存变量
	for (int i = 0; i < 10; i++) {
		if (!GetDlgItem(g_nTextName[i])->IsWindowVisible())	break;
		CString sName, sValue;
		GetDlgItemText(g_nTextName[i], sName);
		GetDlgItemText(g_nTextValue[i], sValue);
		mapValue[sName] = sValue;
	}
	if (!VariableSubstitution(mapValue)) {
		AfxMessageBox(_T("文本对象修改失败"));
		goto TestError;
	}
TestError:
	// 更新消息
	UpdateShowView();
	GetDlgItem(IDC_BUTTON_EDIT_TEXT_CONTONT)->EnableWindow(TRUE);
}

BOOL CFormWorkControl::VariableSubstitution(const std::map<CString, CString>& mapValue)
{
	CString s;
	for (auto & item : mapValue) {
		BOOL bExist = FALSE;   // 变量是否存在
		CStringA sObjNameA, sObjTextA;
		sObjNameA = CStringA(item.first);
		sObjTextA = CStringA(item.second);
		int32_t nCount = m_pWaferDoc->GetWaferCount();
		for (int32_t n = 0; n < nCount; n++) {
			// 检查变量位数是不是超过范围
			char szText[512] = { 0 };
			if (m_pWaferDoc->GetWafer(n)->GetObjectContent(sObjNameA
				, (void *)szText)) {
				//if (sObjTextA.GetLength() > strlen(szText)) {
				//	s.Format(_T("变量替换失败：变量（%s=%s）位数太长！"), item.first, item.second);
				//	PrintMessage(emMsgType_Error, s);
				//	return FALSE;
				//}
				bExist = TRUE;
			}

			if (m_pWaferDoc->GetWafer(n)->SetObjectContent(sObjNameA
				, (void *)sObjTextA.GetBuffer())) {
				s.Format(_T("变量替换成功：%s = %s"), item.first, item.second);
				PrintMessage(emMsgType_Success, s);
				bExist = TRUE;
			}
		}

		if (!bExist) {
			s.Format(_T("变量替换失败：%s = %s"), item.first, item.second);
			PrintMessage(emMsgType_Error, s);
			return FALSE;
		}
	}

	// 多行文本对齐
	int32_t nCount = m_pWaferDoc->GetWaferCount();
	for (int32_t n = 0; n < nCount; n++) {
		if (!m_pWaferDoc->GetWafer(n)->TextAlign()) {
			s.Format(_T("文本对齐变换失败！"));
			PrintMessage(emMsgType_Error, s);
			return FALSE;
		}
	}
	return TRUE;
}
void CFormWorkControl::CheckTextSave()
{
	if (emWorkMode_Manual == m_pConfig->m_nWorkMode)
	{  // 只有手动模式下，才检查
		if (GetDlgItem(IDC_BUTTON_FINISH_EDIT_CONTENT)->IsWindowEnabled()) {
			OnBnClickedButtonFinishEditContent();
		}
	}
}

void CFormWorkControl::InitDiodeTime()
{
	CString sUsedTime;
	GetPrivateProfileString(_T("LaserDiodeTime"), _T("DiodeTime"), _T("0"), sUsedTime.GetBuffer(100), 100, m_LaserUsagePath);
	sUsedTime.ReleaseBuffer();
	sUsedTime.Format(_T("%.2f"), _ttof(sUsedTime));
	SetStaticText(IDC_LASER_TIME, _T("激光器使用时间：") + sUsedTime + _T("h"));
}

void CFormWorkControl::UpdateLaserDiodeTime(bool bOpen)
{
	// 更新激光器的上电时间
	if (1 == GetPrivateProfileInt(_T("LaserDiodeTime"), _T("IsOpen"), 0, m_LaserUsagePath))
	{
		int y, m, d, H, M, S;
		y = GetPrivateProfileInt(_T("LaserDiodeTime"), _T("Year"), 0, m_LaserUsagePath);
		m = GetPrivateProfileInt(_T("LaserDiodeTime"), _T("Month"), 0, m_LaserUsagePath);
		d = GetPrivateProfileInt(_T("LaserDiodeTime"), _T("Day"), 0, m_LaserUsagePath);
		H = GetPrivateProfileInt(_T("LaserDiodeTime"), _T("Hour"), 0, m_LaserUsagePath);
		M = GetPrivateProfileInt(_T("LaserDiodeTime"), _T("Minute"), 0, m_LaserUsagePath);
		S = GetPrivateProfileInt(_T("LaserDiodeTime"), _T("Second"), 0, m_LaserUsagePath);
		CTime tm1(y, m, d, H, M, S);
		CTime tm2 = CTime::GetCurrentTime();
		CTimeSpan span = tm2 - tm1;
		CString sUsedTime;
		GetPrivateProfileString(_T("LaserDiodeTime"), _T("DiodeTime"), _T("0"), sUsedTime.GetBuffer(100), 100, m_LaserUsagePath);
		sUsedTime.ReleaseBuffer();
		sUsedTime.Format(_T("%.2f"), _ttof(sUsedTime) + span.GetTotalMinutes() / 60.0);
		WritePrivateProfileString(_T("LaserDiodeTime"), _T("DiodeTime"), sUsedTime.GetString(), m_LaserUsagePath);

		SetStaticText(IDC_LASER_TIME, _T("激光器使用时间：") + sUsedTime + _T("h"));
	}

	// 保存当前的状态
	CTime tm = CTime::GetCurrentTime();
	CString s;
	s.Format(_T("%d"), (int)bOpen);
	WritePrivateProfileString(_T("LaserDiodeTime"), _T("IsOpen"), s.GetString(), m_LaserUsagePath);
	s.Format(_T("%d"), tm.GetYear());
	WritePrivateProfileString(_T("LaserDiodeTime"), _T("Year"), s.GetString(), m_LaserUsagePath);
	s.Format(_T("%d"), tm.GetMonth());
	WritePrivateProfileString(_T("LaserDiodeTime"), _T("Month"), s.GetString(), m_LaserUsagePath);
	s.Format(_T("%d"), tm.GetDay());
	WritePrivateProfileString(_T("LaserDiodeTime"), _T("Day"), s.GetString(), m_LaserUsagePath);
	s.Format(_T("%d"), tm.GetHour());
	WritePrivateProfileString(_T("LaserDiodeTime"), _T("Hour"), s.GetString(), m_LaserUsagePath);
	s.Format(_T("%d"), tm.GetMinute());
	WritePrivateProfileString(_T("LaserDiodeTime"), _T("Minute"), s.GetString(), m_LaserUsagePath);
	s.Format(_T("%d"), tm.GetSecond());
	WritePrivateProfileString(_T("LaserDiodeTime"), _T("Second"), s.GetString(), m_LaserUsagePath);

}

bool CFormWorkControl::CheckCaveatTime()    //（开始标记前）检查是否已达到报警值
{
	CString caveatTime, sUsedTime;
	GetPrivateProfileString(_T("LaserDiodeTime"), _T("DiodeTime"), _T("0"), sUsedTime.GetBuffer(100), 100, m_LaserUsagePath);
	sUsedTime.ReleaseBuffer();
	GetPrivateProfileString(_T("LaserDiodeTime"), _T("CaveatTime"), _T("20000"), caveatTime.GetBuffer(100), 100, m_LaserUsagePath);
	caveatTime.ReleaseBuffer();
	if (_ttof(sUsedTime) >= _ttoi(caveatTime)) {   //达到预警值
		MessageBox(_T("激光器使用时间达到预警值！\n（详情可见仪器使用情况）"), _T("提示"), MB_OK | MB_ICONWARNING);
		return false;
	}
	return true;
}

bool CFormWorkControl::LoadSeal(CString lotID)  //从XML中解析数据，加载印章
{
	CFactoryConfig *pFactoryConfig = CFactoryConfig::Instance();
	CString s, sFilePath, sFileName, sStampFilePath;
	std::map<CString, CString> mapValue;  // 变量以及内容
	CString showStampInfo = _T("");  //提示信息,印章名
	CString showArrayInfo = _T("");  //提示信息,阵列名
	m_markType.Empty();        //清除上一次信息
	
	lotID = lotID.Left(8);  //截取前八位
	if (!GetStampInfoFromXml(lotID, sFileName, sStampFilePath, mapValue)) {
		goto TestError;
	}

	//showTip.Format(_T("CellVLM: %s    FrameVLM: %s"), sStampFilePath, sFileName);
	showStampInfo.Format(_T("CellVLM: %s"), sStampFilePath);
	showArrayInfo.Format(_T("FrameVLM: %s"), sFileName);
	sFileName = sFileName + _T(".twfb");  //阵列名称
	m_sMarkFileName = sFileName;
	m_sMarkStampNameNonSuffix = sStampFilePath;
	m_sLotID = lotID;
	// 检查印章图档的头部
	if (!CheckMarkFileNameHead(m_sMarkFileName))
		goto TestError;

	sFilePath = pFactoryConfig->m_sMarkFilePath + _T("\\") + sFileName;
	// 检查图档是否初次使用
	if (IsMarkFileFirstUse(sFilePath))
	{
		s.Format(_T("警告：该图档[%s]在本机第一次加载"), sFilePath);
		CDlgMCCheckWarn dlg(s, false);
		dlg.DoModal();
	}

	sStampFilePath = pFactoryConfig->m_sStampPath + _T("\\") + sStampFilePath + _T(".twd");
	if (!LoadContent(sFilePath, sStampFilePath))
	{
		s.Format(_T("印章图档加载失败！路径：%s，%s"), sFilePath, sStampFilePath);
		PrintMessage(emMsgType_Error, s);
		goto TestError;
	}
	s.Format(_T("印章图档加载成功！路径：%s, %s"), sFilePath ,sStampFilePath);
	PrintMessage(emMsgType_Success, s);
	

	// 内容替换
	if (!VariableSubstitution(mapValue)) {
		goto TestError;
	}

	// 更新消息
	UpdateShowView();

	m_ctrlTips.ShowWindow(TRUE);
	m_ctrlTips.ShowText(showStampInfo);   // 显示印章名称
	SetStaticText(IDC_STATIC_FILE_NAME, showArrayInfo);   // 显示阵列名称

	ShowLaserParam();

	WritePrivateProfileString(_T("MarkFile"), sFilePath, _T("1"), g_sTempConfigPath);

	return true;

TestError:
	return false;
}

bool CFormWorkControl::GetStampInfoFromXml(CString lotID, CString &sFilePath, CString &sStampFilePath, map<CString, CString>& textValue)   //从XML中解析印章数据
{
	CString s = _T("");
	CString filePath = CFactoryConfig::Instance()->m_sLotIDXMLPath;
	filePath = filePath + _T("\\") + lotID + _T(".xml");
	string str = ((CStringA)filePath).GetBuffer();

	tinyxml2::XmlDocument doc;
	if (tinyxml2::XML_SUCCESS != doc.LoadFile(str.c_str())) {
		s.Format(_T("未找到对应%s.xml文档！路径为：%s"), lotID, filePath);
		PrintMessage(emMsgType_Error, s);
		return false;
	}

	tinyxml2::XmlElement* root = doc.FirstChildElement("LotData");
	if (nullptr == root) {
		s.Format(_T("%s.xml文档中未找到LotData节点！"), lotID);
		PrintMessage(emMsgType_Error, s);
		return false;
	}

	for (int i = 1; i <= 10; i++)  //遍历包含的文字信息
	{
		string str = "TEXT" + to_string(i);     //从TEXT1开始
		tinyxml2::XmlElement* text = root->FirstChildElement(str.c_str());  
		if (nullptr == text) {
			s.Format(_T("%s.xml文档中未找到%s节点！"), lotID, str.c_str());
			PrintMessage(emMsgType_Error, s);
			return false;
		}

		CString tmp(str.c_str());
		CString cstr(text->GetText());
		if(!cstr.IsEmpty())
			textValue[tmp] = text->GetText();
	}

	CString fontInfo;
	tinyxml2::XmlElement* Fonts = root->FirstChildElement("Fonts");  //字体名
	if (nullptr == Fonts) {
		s.Format(_T("%s.xml文档中未找到FrameVLM节点！"), lotID);
		PrintMessage(emMsgType_Error, s);
		return false;
	}
	fontInfo = Fonts->GetText();
	m_vsFontName.clear();
	if (!fontInfo.IsEmpty()) {       //字体名称可能为空（图档无字体）
		GetFontNameInfo(fontInfo);    //解析文字名称信息
	}

	tinyxml2::XmlElement* LaserParam = root->FirstChildElement("LaserParam");  //激光参数名
	if (nullptr == LaserParam) {
		s.Format(_T("%s.xml文档中未找到LaserParam节点！"), lotID);
		PrintMessage(emMsgType_Error, s);
		return false;
	}
	m_sLaserParamCur = LaserParam->GetText();
	if (m_sLaserParamCur.IsEmpty()) {
		/*s.Format(_T("%s.xml文档中LaserParam节点信息为空"), lotID);
		PrintMessage(emMsgType_Error, s);
		return false;*/
		m_sLaserParamCur = _T("QFN");      //激光参数名为空时取默认值
	}

	tinyxml2::XmlElement* FrameVLM = root->FirstChildElement("FrameVLM");  //阵列名
	if (nullptr == FrameVLM) {
		s.Format(_T("%s.xml文档中未找到FrameVLM节点！"), lotID);
		PrintMessage(emMsgType_Error, s);
		return false;
	}
	sFilePath = FrameVLM->GetText();
	if (sFilePath.IsEmpty()) {
		/*s.Format(_T("%s.xml文档中FrameVLM节点信息为空"), lotID);
		PrintMessage(emMsgType_Error, s);
		return false;*/
		sFilePath = _T("SH110-5X5-1-Q");      //阵列名为空时取默认值
	}
	
	

	tinyxml2::XmlElement* CellVLM = root->FirstChildElement("CellVLM");  //印章名
	if (nullptr == CellVLM) {
		s.Format(_T("%s.xml文档中未找到CellVLM节点！"), lotID);
		PrintMessage(emMsgType_Error, s);
		return false;
	}
	sStampFilePath = CellVLM->GetText();  
	if (sStampFilePath.IsEmpty()) {
		/*s.Format(_T("%s.xml文档中CellVLM节点信息为空"), lotID);
		PrintMessage(emMsgType_Error, s);
		return false;*/
		sStampFilePath = _T("SH110-5X5-5X5PIN1-");      //印章名为空时取默认值
	}

	tinyxml2::XmlElement* MarkType = root->FirstChildElement("MarkType");  //类型
	if (nullptr == MarkType) {
		s.Format(_T("%s.xml文档中未找到MarkType节点！"), lotID);
		PrintMessage(emMsgType_Error, s);
		return false;
	}
	m_markType = MarkType->GetText();
	m_markType.MakeUpper();

	if (m_markType == g_sMTCard || m_markType == g_sMTCardAndUnit) {   //单卡特殊处理
		//tinyxml2::XmlElement* PWSText = root->FirstChildElement("PWSText");  //变换的文字项
		//if (nullptr == PWSText) {
		//	s.Format(_T("%s.xml文档中未找到PWSText节点！"), lotID);
		//	PrintMessage(emMsgType_Error, s);
		//	return false;
		//}
		//m_varName = PWSText->GetText();   //变量名
		//if (m_varName.IsEmpty()) {
		//	s.Format(_T("%s.xml文档中PWSText节点信息为空"), lotID);
		//	PrintMessage(emMsgType_Error, s);
		//	return false;
		//}
	}
	return true;
}

void CFormWorkControl::GetFontNameInfo(CString fontname)  //获取字体名称信息
{
	m_vsFontName.clear();
	chParseString(fontname.GetString(), m_vsFontName, ';');  //以';'区分
}

bool CFormWorkControl::VerifyFontName()   
{
	std::vector <std::string> fontNameFromStampDoc;
	m_pWaferDoc->GetAllTextFontName(fontNameFromStampDoc);

	int nXMLCnt = m_vsFontName.size();
	int nDocCnt = fontNameFromStampDoc.size();

	CString s;
	s.Format(_T("印章模板中字体数量：%d; XML中字体的数量：%d"), nDocCnt, nXMLCnt);
	PrintMessage(emMsgType_Normal, s);

	if ((nXMLCnt == 0) && (nDocCnt == 0)) {     //存在字体名称为空的情况（无字体）
		return true;
	}

	if ( (nXMLCnt != nDocCnt) && (nXMLCnt != 1) )  //nXMLCnt有两种情况：一种字体与多种字体
	{
		PrintMessage(emMsgType_Error, _T("图档中的字体数量与XML中不一致！"));
		return false;
	}
	
	CString sTmpXmlFontName;
	for (int i = 0; i < nDocCnt; i++)
	{
		if (nXMLCnt == 1) {
			sTmpXmlFontName = chT2A(m_vsFontName[0]).c_str();  //只有一种字体
		} else {
			sTmpXmlFontName = chT2A(m_vsFontName[i]).c_str();
		}

		s = CString(fontNameFromStampDoc[i].c_str());

		// 兼容TTF字体，TTF字体不带点
		if(s.ReverseFind(L'.') != -1)
		    s = s.Mid(0, s.ReverseFind(L'.'));

		if (s != sTmpXmlFontName) {
			s.Format(_T("图档中的字体文件【%s】未在XML中找到！"), s);
			PrintMessage(emMsgType_Error, s);
			return false;
		}
	}
	/*for (auto& item : fontNameFromStampDoc)
	{
		CString s;
		s = CString(item.c_str());
		s = s.Mid(0, s.ReverseFind(L'.'));
		if (m_vsFontName.end() == std::find(m_vsFontName.begin(), m_vsFontName.end(), s.GetString()))
		{
			s.Format(_T("图档中的字体文件【%s】未在XML中找到！"), s.GetString());
			PrintMessage(emMsgType_Error, s);
			return false;
		}
	}*/

	return true;
}

bool CFormWorkControl::GetDataFromScripIDXml(const CString& xmlName, CStringArray &binInfo, int &column, CStringArray &barcordInfo, CStringArray &barcordInfo2)
{
	//清空上一次数据
	binInfo.RemoveAll();
	barcordInfo.RemoveAll();
	barcordInfo2.RemoveAll();
	CString s = _T("");

	//路径组成
	CString filePath = CFactoryConfig::Instance()->m_sScripIDXMLPath;
	CString tmpLot = m_sLotIDCur.Mid(0, 8);
	filePath = filePath + _T("\\") + tmpLot + _T("\\") + xmlName + _T(".xml");   //文件实际路径： 界面上的ScripID路径 + lotID前8位 + ScripID.xml
	string str = ((CStringA)filePath).GetBuffer();
	
	//Add by hcx 20240304
	m_stripID = xmlName;
	ReadFileContent(filePath);
	//End by hcx 20240304

	//xml文件读取
	tinyxml2::XmlDocument doc;
	if (tinyxml2::XML_SUCCESS != doc.LoadFile(str.c_str())) {
		s.Format(_T("未找到StripID对应的xml文档！路径为：%s"), filePath);
		PrintMessage(emMsgType_Error, s);
		return false;
	}

	tinyxml2::XmlNode* root = doc.RootElement();
	if (nullptr == root) {
		s.Format(_T("xml文档中未找到MapData根节点！"));
		PrintMessage(emMsgType_Error, s);
		return false;
	}

	tinyxml2::XmlElement* child0 = root->FirstChildElement("SubstrateMaps");
	if (nullptr == child0) {
		s.Format(_T("xml文档中未找到SubstrateMaps节点！"));
		PrintMessage(emMsgType_Error, s);
		return false;
	}

	tinyxml2::XmlElement* child1 = child0->FirstChildElement("SubstrateMap");
	if (nullptr == child1) {
		s.Format(_T("xml文档中未找到SubstrateMap节点1！"));
		PrintMessage(emMsgType_Error, s);
		return false;
	}

	tinyxml2::XmlElement* child2 = child1->FirstChildElement("SubstrateMap");
	if (nullptr == child2) {
		s.Format(_T("xml文档中未找到SubstrateMap节点2！"));
		PrintMessage(emMsgType_Error, s);
		return false;
	}
	
	
	//SubstrateMap中colQty、rowQty值的读取
	int col = 0, row = 0;  //row行数，col列数
	const tinyxml2::XmlAttribute * nameAttr = child2->FirstAttribute();
	while (nameAttr) {
		string perName = nameAttr->Name();
		if ("colQty" == perName) {
			col = nameAttr->IntValue();
			column = col;
		}
		if ("rowQty" == perName) {
			row = nameAttr->IntValue();
		}
		nameAttr = nameAttr->Next();
	}
	//错误校验，以免读取到的值有误，影响后续操作
	if ((col == 0) || (row == 0)) {   
		s.Format(_T("xml文档中colQty或rowQty的值为0！当前值为：colQty = %d ,rowQty = %d"), col, row);
		PrintMessage(emMsgType_Error, s);
		return false;
	}

	tinyxml2::XmlElement* Overlay = child2->FirstChildElement("Overlay");
	if (nullptr == Overlay) {
		s.Format(_T("xml文档中未找到Overlay节点！"));
		PrintMessage(emMsgType_Error, s);
		return false;
	}

	while (Overlay) {
		const tinyxml2::XmlAttribute * nameAttr2 = Overlay->FirstAttribute();
		string MapName = nameAttr2->Value(); 

		//数据为Mapping(BinCodeMap),且启用了此功能/或为单颗（单颗必须启用mapping）
		if ( ("BinCodeMap" == MapName) && IsMapping()) {
			tinyxml2::XmlElement* BinCodeMap = Overlay->FirstChildElement("BinCodeMap");
			if (nullptr == BinCodeMap) {
				s.Format(_T("xml文档中未找到BinCodeMap节点！"));
				PrintMessage(emMsgType_Error, s);
				continue;
			}

			tinyxml2::XmlElement * perAttr = BinCodeMap->FirstChildElement();
			while (perAttr)
			{
				string value = perAttr->GetText();
				CStringA tmp = value.c_str();
				binInfo.Add((CString)tmp);      //将BinCode中的数据插入
				
				s.Format(_T("%s"), (CString)tmp);           //打印得到的BIN相关字符
				PrintMessage(emMsgType_Success, s, 0x02);

				perAttr = perAttr->NextSiblingElement();
			}
		}

		//检测到2D数据
		if ("PrintMap" == MapName) {
			tinyxml2::XmlElement * perAttr = Overlay->FirstChildElement();

			for (int i = 1; i <= col; i++)    
			{
				for (int j = row; j >=1; j--)   
				{
					while (perAttr)    //循环防错，获取行列值进行校验，防止xml内数据乱序
					{
						const tinyxml2::XmlAttribute * rowNo = perAttr->FirstAttribute();
						string sRow = rowNo->Value();   //获取xml中的行值
						const tinyxml2::XmlAttribute * colNo = rowNo->Next();
						string sCol = colNo->Value();   //获取xml中的列值
						if ((sRow == to_string(j)) && (sCol == to_string(i))) {      //校验行列值
							const tinyxml2::XmlAttribute * content = colNo->Next();
							string sInfo = content->Value();   //实际数据
							CStringA tmp = sInfo.c_str();
							barcordInfo.Add((CString)tmp);

							// 获取第二组数据
							CStringA tmp2;
							const tinyxml2::XmlAttribute * content2 = content->Next();
							if (nullptr != content2)
							{
								string sInfo2 = content2->Value();   //实际数据
								tmp2 = sInfo2.c_str();
								barcordInfo2.Add((CString)tmp2);
							}

							s.Format(_T("第%d行，第%d列 ：Content1=%s, Content2=%s"), row - j + 1, i, (CString)tmp, (CString)tmp2);           //打印得到的2D数据
							PrintMessage(emMsgType_Normal, s, 0x02);   // 只保存到日志中
							break;
						}
						perAttr = perAttr->NextSiblingElement();
					  }
					perAttr = Overlay->FirstChildElement();//指针回到起点，而不是继续往下
				}
			}
		}

		Overlay = Overlay->NextSiblingElement();
	}

	return true;
}

bool CFormWorkControl::GetMesInfoFromXml(const CString& xmlContent, std::map<CString, CString> &varValue)  //从MES返回的xml中解析所需变量
{
	CString s;
	CStringA xmlContentA = CStringA(xmlContent);
	tinyxml2::XmlDocument doc;
	if (tinyxml2::XML_SUCCESS != doc.Parse(xmlContentA, xmlContentA.GetLength())) {
		s.Format(_T("MES返回的xml数据有误！"));
		PrintMessage(emMsgType_Error, s);
		return false;
	}

	//根节点LotInfoData
	tinyxml2::XmlNode* root = doc.RootElement();
	if (nullptr == root) {
		s.Format(_T("返回的xml数据中未找到LotInfoData根节点！"));
		PrintMessage(emMsgType_Error, s);
		return false;
	}

	//LotID
	tinyxml2::XmlElement* LotId = root->FirstChildElement("LotId");
	if (nullptr == LotId) {
		s.Format(_T("返回的xml数据中未找到LotId节点！"));
		PrintMessage(emMsgType_Error, s);
		return false;
	}

	//校验返回数据lotID是否正确
	CString checkStr = GetLotIDCur();
	if (checkStr != CString(LotId->GetText())) {
		s.Format(_T("返回的xml数据中LotId数据有误！ 当前扫入LotId：%s, MES返回LotId：%s"), checkStr, CString(LotId->GetText()));
		PrintMessage(emMsgType_Error, s);
		return false;
	}

	//LotParams
	tinyxml2::XmlElement* LotParams = root->FirstChildElement("LotParams");
	if (nullptr == LotParams) {
		s.Format(_T("返回的xml数据中未找到LotParams节点！"));
		PrintMessage(emMsgType_Error, s);
		return false;
	}

	//找到第一个LotParamInfo,用于接下来的遍历
	tinyxml2::XmlElement* LotParamInfo = LotParams->FirstChildElement("LotParamInfo");
	if (nullptr == LotParamInfo) {
		s.Format(_T("返回的xml数据中未找到LotParamInfo节点！"));
		PrintMessage(emMsgType_Error, s);
		return false;
	}

	CString sMarkLoc;                                              //类型（正印/背印/双印）
	CString tmpNameZ = _T("$") + m_varName.MakeUpper();   //正印
	map<CString, CString> tmpVarValue;     //临时存储所有的变量名与值
	vector<CString> tmpTextValue;          //按顺序存储所有非**（非LOGO)数据

	// 读取所有数据
	while (LotParamInfo) {
		tinyxml2::XmlElement* ParamName = LotParamInfo->FirstChildElement("ParamName");
		tinyxml2::XmlElement*  ParamValue = LotParamInfo->FirstChildElement("ParamValue");
		if (ParamValue == nullptr || ParamName == nullptr) {
			s.Format(_T("返回的xml数据中未找到ParamValue和ParamName节点！"));
			PrintMessage(emMsgType_Error, s);
			return false;
		}

		CString valueStr = CString(ParamValue->GetText());
		if ( _T("**") != valueStr) {                      //判断值是否为**（logo标识）
			tmpTextValue.push_back(valueStr);             //不为**则插入，排除**干扰
		}

		LotParamInfo = LotParamInfo->NextSiblingElement();
	}

	int oldNum = GetNumberFromCString();  //变量所在位数
	for (int i = 0; i < tmpTextValue.size(); i++)
	{
		if (i == (oldNum - 1)) {      //找到变量对应位置
			varValue[m_varName.MakeUpper()] = tmpTextValue[i];
		}
	}

	return true;
}

int CFormWorkControl::GetNumberFromCString()  //从CString中取出末尾包含的数字值
{
	string str = CStringA(m_varName).GetString();
	string tmp;
	for (int i = 0; i < str.length(); i++) {
		if (str[i] >= '0' && str[i] <= '9') {
			tmp += str[i];
		}
	}

	int n = atoi(tmp.c_str());
	return n;
}

bool CFormWorkControl::GetMesInfoFromXmlAndGFH(const CString& xmlContent, std::map<CString, CString> &varValue)  //从MES返回的xml中解析所需变量
{
	CString s;
	CStringA xmlContentA = CStringA(xmlContent);
	tinyxml2::XmlDocument doc;
	if (tinyxml2::XML_SUCCESS != doc.Parse(xmlContentA, xmlContentA.GetLength())) {
		s.Format(_T("MES返回的xml数据有误！"));
		PrintMessage(emMsgType_Error, s);
		return false;
	}

	//根节点LotInfoData
	tinyxml2::XmlNode* root = doc.RootElement();
	if (nullptr == root) {
		s.Format(_T("返回的xml数据中未找到LotInfoData根节点！"));
		PrintMessage(emMsgType_Error, s);
		return false;
	}

	//LotID
	tinyxml2::XmlElement* LotId = root->FirstChildElement("LotId");
	if (nullptr == LotId) {
		s.Format(_T("返回的xml数据中未找到LotId节点！"));
		PrintMessage(emMsgType_Error, s);
		return false;
	}

	//校验返回数据lotID是否正确
	CString checkStr = GetLotIDCur();
	if (checkStr != CString(LotId->GetText())) {
		s.Format(_T("返回的xml数据中LotId数据有误！ 当前扫入LotId：%s, MES返回LotId：%s"), checkStr, CString(LotId->GetText()));
		PrintMessage(emMsgType_Error, s);
		return false;
	}

	//LotParams
	tinyxml2::XmlElement* LotParams = root->FirstChildElement("LotParams");
	if (nullptr == LotParams) {
		s.Format(_T("返回的xml数据中未找到LotParams节点！"));
		PrintMessage(emMsgType_Error, s);
		return false;
	}

	//找到第一个LotParamInfo,用于接下来的遍历
	tinyxml2::XmlElement* LotParamInfo = LotParams->FirstChildElement("LotParamInfo");
	if (nullptr == LotParamInfo) {
		s.Format(_T("返回的xml数据中未找到LotParamInfo节点！"));
		PrintMessage(emMsgType_Error, s);
		return false;
	}

	// 读取所有数据
	map<CString, CString> tmpVarValue;     //临时存储所有的变量名与值
	while (LotParamInfo) {
		tinyxml2::XmlElement* ParamName = LotParamInfo->FirstChildElement("ParamName");
		tinyxml2::XmlElement*  ParamValue = LotParamInfo->FirstChildElement("ParamValue");
		if (ParamValue == nullptr || ParamName == nullptr) {
			s.Format(_T("返回的xml数据中未找到ParamValue和ParamName节点！"));
			PrintMessage(emMsgType_Error, s);
			return false;
		}

		CString valueStr = CString(ParamValue->GetText());
		CString NameStr = CString(ParamName->GetText());
		tmpVarValue[NameStr] = valueStr;

		LotParamInfo = LotParamInfo->NextSiblingElement();
	}

	// 根据规范号 $MARKING-SPEC 找到规范文件，找到图档中文本对应替换变量
	if (tmpVarValue.end() == tmpVarValue.find(_T("$MARKING-SPEC")))
	{
		s.Format(_T("返回的xml数据中未找到规范号名称[$MARKING-SPEC]节点！"));
		PrintMessage(emMsgType_Error, s);
		return false;
	}
	std::map<CString, CString> varName;
	if (!GetVarRelationFromGFH(tmpVarValue[_T("$MARKING-SPEC")], varName))
	{
		return false;
	}

	for (auto& item : varName)
	{
		if (tmpVarValue[item.second].IsEmpty() || tmpVarValue[item.second] == _T("**"))
		{
			s.Format(_T("MES中的变量内容格式不正确！"));
			PrintMessage(emMsgType_Error, s);
			return false;
		}
		varValue[item.first] = tmpVarValue[item.second];
	}

	return true;
}

bool CFormWorkControl::GetVarRelationFromGFH(const CString& sGFHName, std::map<CString, CString> &varValue)
{
	CString sGFHPath = m_pConfig->m_sGFHPath + _T("\\") + sGFHName + _T(".ini");
	if (!PathFileExists(sGFHPath))
	{
		CString s;
		s.Format(_T("对应的规范号文件[%s]不存在!"), sGFHPath);
		PrintMessage(emMsgType_Error, s);
		return false;
	}
	for (int i = 1; i <= 10; i++)
	{
		CString sText, sName;
		sName.Format(_T("TEXT%d"), i);
		GetPrivateProfileString(_T("TEXT"), to_wstring(i).c_str(), _T(""), sText.GetBuffer(1024),1024, sGFHPath);
		sText.ReleaseBuffer();
		sText.TrimLeft();
		sText.TrimRight();
		if (!sText.IsEmpty())
		{
			varValue[sName] = CString(_T("$TEXT")) + sText;
		}
	}
	return true;
}

BOOL CFormWorkControl::ReplaceVarByWebService()   //根据WebService通讯内容替换变量
{
	CString searchStr = GetLotIDCur();

	// 通过soap获取指定lotid的信息
	m_pMySoap->SetUrl(m_pConfig->m_sMesUrl);
	CString sRetLotInfo = m_pMySoap->GetLotInfo(searchStr);
	if (sRetLotInfo.IsEmpty())
	{
		PrintMessage(emMsgType_Error, CString(_T("Mes系统未返回任何内容或通讯失败！")) + m_pMySoap->GetLastError());
		return FALSE;
	}

	//显示并记录MES返回的内容
	CString s;
	s.Format(_T("%s"), sRetLotInfo);
	PrintMessage(emMsgType_Normal, s, 0x02);

	// 解析返回信息
	std::map<CString, CString> mapValue;  // 变量内容
#if 0  
	// 根据PWS中的的PWSText文本来替换变量
	if (!GetMesInfoFromXml(sRetLotInfo, mapValue))
	{
		PrintMessage(emMsgType_Error, _T("Mes系统返回内容格式错误！"));
		return FALSE;
	}
#else
	// 根据规范号文件中的对应关系，来替换变量
	if (!GetMesInfoFromXmlAndGFH(sRetLotInfo, mapValue))
	{
		PrintMessage(emMsgType_Error, _T("Mes系统返回内容格式错误！"));
		return FALSE;
	}
#endif

	if (!VariableSubstitution(mapValue)) {
		return FALSE;
	}

	// 更新文本对象显示
	UpdateShowView();

	return TRUE;
}

BOOL CFormWorkControl::CheckStripIDMatchLotIDByWebService()
{
	CString msg;
	m_pMySoap->SetUrl(m_pConfig->m_sMesCheckUrl);
	BOOL bMatch = m_pMySoap->IsLotIDMatchStripID(m_sStripIDCur, m_sLotIDBindStripIDCur);
	if (!bMatch)
	{
		msg.Format(_T("Strip ID[%s]查询不到对应的Lot ID。详细原因：%s"), m_sStripIDCur, m_pMySoap->GetLastError());
		PrintMessage(emMsgType_Error, msg);
		return FALSE;
	}
	msg.Format(_T("Strip ID[%s]对应的Lot ID[%s]"), m_sStripIDCur, m_sLotIDBindStripIDCur, m_pMySoap->GetLastError());
	PrintMessage(emMsgType_Normal, msg);
	return TRUE;
}

int32_t CFormWorkControl::ChangeBin(CStringArray &binInfo, int column)  //2D Mapping 修改BIN
{
	// 判断是否正在标记
	if (IsMarking())
	{
		PrintMessage(emMsgType_Error, _T("正在打标中，修改BIN失败！"));
		return -1;
	}

	// 判断是否CancelJob
	if (IsCancelJob())
	{
		PrintMessage(emMsgType_Error, _T("当前处于“取消作业”状态，请点击“开始作业”！"));
		return -1;
	}

	if (NULL == m_pWaferDoc || NULL == m_pMapDoc)
	{
		PrintMessage(emMsgType_Error, _T("无印章图档，BIN修改失败！"));
		return -1;
	}
	CWaferGroup * pWaferGroup = m_pMapDoc->GetGroup(0);
	if (NULL == pWaferGroup)
	{
		PrintMessage(emMsgType_Error, _T("无印章阵列图，BIN修改失败！"));
		return -1;
	}

	int32_t iError(emREC_OK);

	// 清空BIN MAP数据
	ClearMapData();

	CString s;
	CStringArray sArray;
	if (ResolveBinInfo(binInfo, column, sArray) > 0)   //因无多余数据，修改为有数据则执行
	{
		// 第一步，判断传入的芯片数量是否与MAP中的芯片数量一致
		int nSendCount = sArray.GetSize(); 
		int nMapCount = pWaferGroup->vWaferMetas.size();
		if (nSendCount != nMapCount)
		{
			s.Format(_T("阵列中的模板数量[%d]与传入的模板数量[%d]不一致！"), nMapCount, nSendCount);
			PrintMessage(emMsgType_Error, s);
			return -1;
		}

		// 第二步，替换所有MAP中芯片的类型
		const int iBinNo = 0;  // BIN_0为正常内容
		int iOK = 0;
		for (int i = 0; i < nMapCount; i++)
		{
			int nWaferID = _ttoi(sArray.GetAt(i));
			if (nWaferID < 0 || nWaferID >= m_pWaferDoc->GetWaferCount())
			{
				nWaferID = -1;    // 当传入的BIN不存在时，设置为-1，避免标记出错
				s.Format(_T("第%d颗印章[行%d，列%d]对应的BIN%d不存在！"), i,
					pWaferGroup->vWaferMetas[i].nRow, pWaferGroup->vWaferMetas[i].nCol, _ttoi(sArray.GetAt(i)));
				PrintMessage(emMsgType_Error, s);
				iError = -1;
			}
			if (iBinNo == nWaferID) iOK++;
			m_iBinMap.push_back(nWaferID);
			pWaferGroup->vWaferMetas[i].SetWaferID(nWaferID);
		}
		m_nMapOKCount = iOK;
		m_nMapNGCount = nMapCount - iOK;
		s.Format(_T("Mapping数据统计：打正常产品：%d颗，打废产品：%d颗"), iOK, nMapCount - iOK);
		PrintMessage(emMsgType_Normal, s);

		// 更新MAP显示视图
		//pWaferGroup->SetMapMode(true);
		m_pMarkContentView->Invalidate(TRUE);
	}
	else
	{
		PrintMessage(emMsgType_Error, _T("内容解析失败！"));
		iError = -1;
	}	

	if (-1 == iError)
		PrintMessage(emMsgType_Error, _T("BIN切换失败！"));
	else
		PrintMessage(emMsgType_Success, _T("BIN切换成功！"));

	return iError;
}

int32_t CFormWorkControl::ResolveBinInfo(CStringArray &binInfo, int column, CStringArray &sArray)  //解析BIN数据
{
	sArray.RemoveAll();
	int row = binInfo.GetSize();  
	for (int i=0; i<column; i++)  //column列数，即一个<BinCode>中含有多少个BIN相关数据
	{
		for (int j = row; j >0; j--) 
		{
			CString str = binInfo.GetAt(j-1);        //按照ScriptID.xml文档中的顺序，与我们的原有顺序不同
			//每次截取前四位，用于判断属于哪个BIN
			CString tmp = str.Mid(0, 4);
			if ("0100" == tmp) {  //Good ,对应BIN0（需打印）
				sArray.Add(_T("0"));
			}
			else if ("0f00" == tmp) {  //Bad,对应BIN1
				sArray.Add(_T("1"));
			}
			else if ("0000" == tmp) {  //NULL,对应BIN2
				sArray.Add(_T("2"));
			}
			else if ("010f" == tmp) {  //XOUT,对应BIN3
				sArray.Add(_T("3"));
			}

			int n = str.GetLength();
			if (str.GetLength() != 4) //判断是否到最后一次数据（4位）
			{
				CString newStr = str.Mid(4);  //去掉前4位
				binInfo.SetAt(j-1, newStr);     //将去掉前4位（已取值）的字符串赋回给原位置
			}
		}
	}

	return sArray.GetSize();
}

int32_t CFormWorkControl::Write2DBarcodeInfo(const CStringArray &barcordInfo, CStringArray &barcordInfo2)   //写入二维码数据
{
	CString s;
	// 判断是否正在标记
	if (IsMarking())
	{
		PrintMessage(emMsgType_Error, _T("正在打标中，2D Barcode数据替换失败！"));
		return -1;
	}

	if (NULL == m_pWaferDoc || NULL == m_pMapDoc)
	{
		PrintMessage(emMsgType_Error, _T("无印章图档，2D Barcode数据替换失败！"));
		return -1;
	}
	CWaferGroup * pWaferGroup = m_pMapDoc->GetGroup(0);
	if (NULL == pWaferGroup)
	{
		PrintMessage(emMsgType_Error, _T("无印章阵列图，2D Barcode数据替换失败！"));
		return -1;
	}

	int32_t iError(emREC_OK);

	// 清空之前的数据
	Clear2DData();

	int nSendCount = barcordInfo.GetSize();
	if (nSendCount > 0)
	{
		PrintMessage(emMsgType_Normal, _T("2DBarcode数据大于0"));
		if (!m_pConfig->m_bCheck2DBarcode
			&& (m_markType == g_sMTNormal || m_markType == g_sMTCard) 
			&& (m_pWaferDoc->IsExist2DBarcode() || m_pWaferDoc->IsExistBarcode()))
		{
			// Normal 和Card模式, 如果存在2D或者二维码，但是没有勾选2D Barcode,则弹出确认框(技术员或以上权限)
			CString s;
			s.Format(_T("警告：Normal和Card模式下，打印模板中存在二维码信息，但是软件系统设置中未勾选，是否继续？"));
			CDlgMCCheckWarn dlg(s, true, true);
			if (IDOK != dlg.DoModal())
			{
				PrintMessage(emMsgType_Error, _T("Normal和Card模式下，打印模板中存在二维码信息，但是软件系统设置中未勾选！"));
				return -1;
			}
				
			if (!RightConfirmBox(USER_O_CHECK_MAPPING_INFO, true))
			{
				PrintMessage(emMsgType_Error, _T("Normal和Card模式下，打印模板中存在二维码信息，但是软件系统设置中未勾选！"));
				return -1;
			}
		}

		// 检查第一组2D数据的正确性
		if (m_pWaferDoc->IsExistFirst2DBarcode() && barcordInfo.IsEmpty())
		{
			PrintMessage(emMsgType_Error, _T("打印模板中勾选了从第2DBarcode中获取内容，但是没有从Mes系统中找到Content！"));
			return -1;
		}
#if 0   // Delete by hhhuang 20251204 客户要求取消这个防呆，只要没有勾选2D Barcode就不打印
		if (!m_pWaferDoc->IsExistFirst2DBarcode() && !barcordInfo.IsEmpty())
		{
			PrintMessage(emMsgType_Error, _T("从Mes系统中找到Content,但是打印模板中未勾选从第2DBarcode中获取内容"));
			return -1;
		}
#endif

		// 检查第二组2D数据的正确性
		if (m_pWaferDoc->IsExistTwo2DBarcode() && barcordInfo2.IsEmpty())
		{
			PrintMessage(emMsgType_Error, _T("打印模板中勾选了从第2DBarcode2中获取内容，但是没有从Mes系统中找到Content2！"));
			return -1;
		}
#if 0   // Delete by hhhuang 20251204 客户要求取消这个防呆，只要没有勾选2D Barcode就不打印
		if (!m_pWaferDoc->IsExistTwo2DBarcode() && !barcordInfo2.IsEmpty())
		{
			PrintMessage(emMsgType_Error, _T("从Mes系统中找到Content2,但是打印模板中未勾选从第2DBarcode2中获取内容"));
			return -1;
		}
#endif

		// 保存所有的2D数据
		int iBarcodeInfoSize2 = barcordInfo2.GetSize();
		for (int i = 0; i < nSendCount; i++)
		{
			m_vs2DData.push_back(barcordInfo.GetAt(i));
			if(i < iBarcodeInfoSize2) m_vs2DData2.push_back(barcordInfo2.GetAt(i));
		}

		PrintMessage(emMsgType_Normal, _T("检查2DBarcode数据"));
		if (!Verify2DData())
		{
			Clear2DData();
			return -1;
		}
	}
	else
	{
		if ((m_markType == g_sMTNormal || m_markType == g_sMTCard) && (m_pWaferDoc->IsExist2DBarcode() || m_pWaferDoc->IsExistBarcode()))
		{
			// Normal 和Card模式  如果存在2D，但是没有勾选2D Barcode或未从服务器下载二维码，则弹出确认框(技术员或以上权限)
			CString s;
			s.Format(_T("警告：Normal和Card模式下，打印模板中存在2D信息，但是没有从系统中接收到2D信息，是否继续？"));
			CDlgMCCheckWarn dlg(s, true, true);
			if (IDOK != dlg.DoModal())
			{
				PrintMessage(emMsgType_Error, _T("Normal和Card模式下，打印模板中存在2D信息，但是没有从系统中接收到2D信息！"));
				return -1;
			}

			if (!RightConfirmBox(USER_O_CHECK_MAPPING_INFO, true))
			{
				PrintMessage(emMsgType_Error, _T("Normal和Card模式下，打印模板中存在2D信息，但是没有从系统中接收到2D信息！"));
				return -1;
			}
		}
		else
		{
			PrintMessage(emMsgType_Error, _T("缺失二维码内容！"));
			return -1;
		}
	}	




	return iError;
}

bool CFormWorkControl::Verify2DData()
{
	CString s;
	CWaferGroup * pWaferGroup = m_pMapDoc->GetGroup(0);
	// 第一步，判断传入的芯片数量是否与MAP中的芯片数量一致
	int nSendCount = m_vs2DData.size();   
	int nMapCount = pWaferGroup->vWaferMetas.size();
	if (nSendCount != nMapCount)
	{
		s.Format(_T("缺失二维码内容：阵列中的模板数量[%d]与传入的模板数量[%d]不一致！"), nMapCount, nSendCount);
		PrintMessage(emMsgType_Error, s);
		return false;
	}

	// 检查正常产品和正常2D数据是否匹配
	for(int i = 0; i < m_vs2DData.size(); i++)
	{
		if (m_vs2DData[i].IsEmpty() && (m_vs2DData2.size() > 0 && m_vs2DData2[i].IsEmpty()))
		{
			if (pWaferGroup->vWaferMetas[i].GetWaferID() == 0)
			{
				s.Format(_T("缺失二维码内容：MAPPING数据与2D数据不一致！index=%d应该为正常产品"), i);
				PrintMessage(emMsgType_Error, s);
				return false;
			}
		}
	}

	// 第三步，检查数据是否有重复
	if (!(IsDummy() || IsFirstPrint()))    // Dummy模式和首条打印下不检查二维码重复信息
	{
		if (0 != CFactoryConfig::Instance()->m_nCheck2DBarcodeRepeat)
		{
#if 0  // 指定时间点删除
			PrintMessage(emMsgType_Normal, _T("删除超出日期的2D数据"));
			m_2DRepeatCheck.DeleteDataOutOfDate();
#endif
			PrintMessage(emMsgType_Normal, _T("检查2D数据是否重复"));
			if (!m_2DRepeatCheck.Check(m_vs2DData))
			{
				s.Format(_T("%s"), m_2DRepeatCheck.GetLastErrorMsg());
				PrintMessage(emMsgType_Error, s);
				return false;
			}
			PrintMessage(emMsgType_Normal, _T("检查第二组2D数据是否重复"));
			if (!m_2DRepeatCheck.Check(m_vs2DData2))
			{
				s.Format(_T("%s"), m_2DRepeatCheck.GetLastErrorMsg());
				PrintMessage(emMsgType_Error, s);
				return false;
			}
		}
	}

	// 检查印章模板中是否存在2D数据，不存在则报警
	if (!m_pWaferDoc->IsExist2DBarcode())
	{
		s.Format(_T("模板中不存在BIN Data类型的二维码对象！请检查二维码对象是否勾选了从2D中获取数据！"));
		PrintMessage(emMsgType_Error, s);
		return false;
	}

	s.Format(_T("印章数量：%d。"), nMapCount);
	PrintMessage(emMsgType_Normal, s);

	return true;
}

LRESULT CFormWorkControl::OnStartProcess(WPARAM wParam, LPARAM lParam)
{
	m_StartMarkSingal.StopMonitor();
	uint32_t nBit = (uint32_t)lParam;
#ifdef DEBUG_IO
	if (GetDlgItemInt(IDC_EDIT_IO))
#else
	if (IO_IN_VALID == nBit)   //检测是否为开始标记
#endif	
	{
		//PrintMessage(emMsgType_Normal, _T("IO口检测到开始标记信号"));
		// 开始标记
		StartMark();
	}
	return 0;
}

LRESULT CFormWorkControl::OnScanProcess(WPARAM wParam, LPARAM lParam)
{
	int iRet = Scanned2DProcess();
	if (-1 == iRet) {
		OnBnClickedButtonCanceljob();   // 扫描数据异常，退出侦测流程
		return -1;
	}
	// 扫描成功后，也需要重新开启2D扫描信号侦测，在后面检查到开始标记信号后，再关闭
	// 防止CCD定位失败，软件流程卡在侦测开始标记信号那个步骤中，自动化复位后，再次扫描2D信号，不会有反应
	m_Scan2DSignal.StartDetect();
	return 0;
}

int CFormWorkControl::Scanned2DProcess(int nScanNo, int nScanTimes)
{
	m_StartMarkSingal.StopMonitor();
#ifdef ENABLE_SCAN_THREAD
	m_Scan2DSignal.EndDetect();
#else
	KillTimer(SCANSIGNAL_CHECK_TIMER);
#endif	
	PrintMessage(emMsgType_Normal, _T("2D扫描信号停止检测"));
	PrintMessage(emMsgType_Normal, _T("开始标记信号停止检测"));
	// 如果正在标记，则返回正常，这种情况正常不会发送
	if (IsMarking())
	{
		PrintMessage(emMsgType_Warn, _T("检测到2D扫描信号，正在标记中..."));
		return 0;
	}
	CString s;
	s.Format(_T("扫描头%d，第%d次扫描信号触发，开始扫描..."), nScanNo, nScanTimes);
	PrintMessage(emMsgType_Normal, s);
	CString sScanStr;
	if (!GetScannerContent(nScanNo, sScanStr)) {
		PrintMessage(emMsgType_Normal, _T("条码枪扫描失败！手动输入Strip ID"));
		if (1 == nScanTimes) {
			if (!ManualInputScanStripID(sScanStr)) {
				PrintMessage(emMsgType_Error, _T("手动输入Strip ID超时！"));
				SetPulseSignal(m_pConfig->m_nScanInputTimeOutPort, IO_2D_OUT_VALID, m_pConfig->m_nScannTime);
				SetPulseSignal(m_pConfig->m_nScannNGSignal, IO_2D_OUT_VALID, m_pConfig->m_nScannTime);
				return -2;  // 继续侦察扫描
			}
		}
		else {
			PrintMessage(emMsgType_Error, _T("扫描失败！"));
			SetPulseSignal(m_pConfig->m_nScannNGSignal, IO_2D_OUT_VALID, m_pConfig->m_nScannTime);
			return -2;   // 继续侦察扫描
		}
	}

	s.Format(_T("Strip ID：%s"), sScanStr);
	PrintMessage(emMsgType_Success, s);

	//if (!IsDummy())  // delete by hhhuang 20251113 在ScannerContentProcess中判断Mapping模式
	{   
		if (!ScannerContentProcess(sScanStr)) {    //处理扫描枪返回数据
			SetPulseSignal(m_pConfig->m_nScannNGSignal, IO_2D_OUT_VALID, m_pConfig->m_nScannTime);
			PrintMessage(emMsgType_Error, _T("数据处理异常,发送报警信号！"));
			return -1;   // 退出扫描流程
		}
	}

	SetPulseSignal(m_pConfig->m_nScannOKSignal, IO_2D_OUT_VALID, m_pConfig->m_nScannTime);
	PrintMessage(emMsgType_Normal, _T("扫描结束,数据处理成功,发送OK信号"));
	
	// 单卡模式
	if (m_markType == g_sMTCard || m_markType == g_sMTCardAndUnit) {
		// 从Mes中获取变量内容
		if (!ReplaceVarByWebService())
		{
			SetPulseSignal(m_pConfig->m_nScanPWSNG, IO_2D_OUT_VALID, m_pConfig->m_nScannTime);
			PrintMessage(emMsgType_Error, _T("从MES调取印章失败！"));
			return -1;  // 退出扫码流程
		}
		SetPulseSignal(m_pConfig->m_nScanPWSOK, IO_2D_OUT_VALID, m_pConfig->m_nScannTime);
		PrintMessage(emMsgType_Success, _T("从MES调取印章成功！"));
	}


	// 开启标记信号侦测
	m_StartMarkSingal.StartMonitor();
	PrintMessage(emMsgType_Normal, _T("正在检测标记信号..."));
	return 0;
}


int CFormWorkControl::ScannerTimerProcess()
{
	//if (emScannerType_None == m_pConfig->m_emUseScan) {
	//	KillTimer(SCANSIGNAL_CHECK_TIMER);
	//	PrintMessage(emMsgType_Normal, _T("未启用扫描枪"));
	//	// 开启标记信号侦测
	//	SetTimer(5, 100, nullptr);
	//	return TRUE;
	//}
	bool bFirstScan1 = false, bSecondScan1 = false, bFirstScan2 = false, bSecondScan2 = false;
	bFirstScan1 = Scan1SignalEffective(0);
//	bSecondScan1 = Scan1SignalEffective(1);
//	bFirstScan2 = Scan2SignalEffective(0);
//	bSecondScan2 = Scan2SignalEffective(1);
	if (bFirstScan1 || bSecondScan1 || bFirstScan2 || bSecondScan2) 
	{
		return Scanned2DProcess((bFirstScan1 || bSecondScan1) ? 1 : 2, (bFirstScan1 || bFirstScan2) ? 1 : 2);
	}
	return 0;   // 继续
}

BOOL CFormWorkControl::GetScannerContent(int nScanNo, CString &sScan)
{
	CString ip;
	int port;
	switch (m_pConfig->m_emUseScan) {
	case emScannerType_Client1:
		ip = m_pConfig->m_sScannerIP[0];
		port = m_pConfig->m_nScannerPort[0];
		break;
	case emScannerType_Client2:
		ip = m_pConfig->m_sScannerIP[1];
		port = m_pConfig->m_nScannerPort[1];
		break;
	case emScannerType_Client_All:
		ip = m_pConfig->m_sScannerIP[nScanNo];
		port = m_pConfig->m_nScannerPort[nScanNo];
		break;
	default:
		return FALSE;
	}

	CString s;
	if (0 != m_SocketComClient.Connect(ip.GetString(), port)) {

		s.Format(_T("扫描枪[ip=%s, port=%d]连接失败！失败原因：%s"), ip, port, m_SocketComClient.GetLastError());
		PrintMessage(emMsgType_Error, s);
		return FALSE;
	}
	BOOL bRecvScanOK = FALSE;
	int nRetryTimes = 3;
	while (nRetryTimes > 0)
	{
		const char* cmdSend = "LON\r";
		if (0 != m_SocketComClient.SendData(cmdSend, 4)) {
			s.Format(_T("第%d次发送给扫描枪的指令[%s]执行失败！失败原因：%s"), 4 - nRetryTimes, cmdSend, m_SocketComClient.GetLastError());
			PrintMessage(emMsgType_Error, s);
			continue;
		}
		PrintMessage(emMsgType_Normal, _T("发送给扫描枪：LON\\r"));

		const char* cmdOFF = "LOFF\r";
		if (0 != m_SocketComClient.SendData(cmdOFF, 5)) {
			s.Format(_T("发送给扫描枪的指令[%s]执行失败！失败原因：%s"), cmdOFF, m_SocketComClient.GetLastError());
			PrintMessage(emMsgType_Error, s);
			return FALSE;
		}
		PrintMessage(emMsgType_Normal, _T("发送给扫描枪：LOFF\\r"));

		int nTimes = 5;   // 100ms单位， 5s
		while (nTimes > 0) {
			char recvbuf[1024] = { 0 };
			if (0 != m_SocketComClient.RecvData(recvbuf, 1024, 0)) {
				s.Format(_T("读取扫描枪的扫描数据失败！失败原因：%s"), m_SocketComClient.GetLastError());
				PrintMessage(emMsgType_Error, s);
				break;
			}
			if (strlen(recvbuf) > 0) {
				s.Format(_T("扫描枪返回内容：%s"), CString(recvbuf).GetString());
				PrintMessage(emMsgType_Normal, s);
				if (nullptr != strstr(recvbuf, "ERROR")) {
					break;
				}

				bRecvScanOK = TRUE;
				sScan = CString(recvbuf);
				sScan.TrimLeft();
				sScan.TrimRight();
				break;
			}

			CMarkPlatform::Instance()->Delay(100);

			nTimes--;
		}
		if (bRecvScanOK) break;
		nRetryTimes--;
	}

	m_SocketComClient.Close();

	return bRecvScanOK;
}

BOOL CFormWorkControl::Scan1SignalEffective(int times)
{
	// 高电平有效
	uint32_t nBit = 0x00;
#ifdef DEBUG_IO
	nBit = GetPrivateProfileInt(_T("IO"), _T("ScannerStart_1"), 0, _T("config\\IOTest.ini"));
#else
	GetCommIOStatus(m_pConfig->m_nScannStartSignal[times], nBit);
#endif	
	return (IO_2D_IN_VALID == nBit);
}

BOOL CFormWorkControl::Scan2SignalEffective(int times)
{
	// 高电平有效
	uint32_t nBit = 0x00;
#ifdef DEBUG_IO
	nBit = GetPrivateProfileInt(_T("IO"), _T("ScannerStart_2"), 0, _T("config\\IOTest.ini"));
#else
	//m_pMarkDriver->ReadExtPortPin(m_pConfig->m_nScannStartSignal2[times] + 16, nBit);
	GetCommIOStatus(m_pConfig->m_nScannStartSignal2[times], nBit);
#endif	
	return (IO_2D_IN_VALID == nBit);
}

BOOL CFormWorkControl::ManualInputScanStripID(CString& sScan)
{
	CDlgInputStripID dlg(sScan);
	return (IDOK == dlg.DoModal());
}

void CFormWorkControl::SetPulseSignal(int port, bool status, int delaytime_ms)
{
	//CString strlog;
	//strlog.Format(_T("端口EDO%d设置为%d电平持续%d毫秒"), port, (int)status, delaytime_ms);
	//PrintMessage(emMsgType_Normal, strlog);

	SetCommIOStatus(port, status);
	CMarkPlatform::Instance()->Delay(delaytime_ms);
	SetCommIOStatus(port, !status);
}

BOOL CFormWorkControl::ScannerContentProcess(CString scanstr)
{
	if (!IsDummy()) {
		// 检查Strip ID的LotID格式，是否扫描过
		PrintMessage(emMsgType_Normal, _T("检查StripID"));
		if (!VerifyStripID(scanstr))
		{
			return FALSE;
		}
	}

	// 获取Mapping和2D Barcode
	int col = 0;                //列数，即一个（行）<BinCode>中含有多少个BIN相关数据
	CStringArray binInfo;       //Map数据    （MAPPING功能）
	CStringArray barcordInfo;   //二维码数据 （单颗二维码打印功能）
	CStringArray barcordInfo2;   //第二组二维码数据 （单颗二维码打印功能）

	// MAPPING模式、模板中存在2D Barcode类型数据、模板中存在二维码，都需要找下XML文件
	if (IsMapping() || m_pWaferDoc->IsExist2DBarcode() || m_pWaferDoc->IsExistBarcode())
	{
		PrintMessage(emMsgType_Normal, _T("获取StripID对应的MAPPING信息和2D信息"));
		bool bReadXml = GetDataFromScripIDXml(scanstr, binInfo, col, barcordInfo, barcordInfo2);
		if (!bReadXml) return FALSE;

		PrintMessage(emMsgType_Normal, _T("更新MAPPING信息"));
		if (-1 == ChangeBin(binInfo, col)) {
			return FALSE;
		}
	}
	else
	{
		PrintMessage(emMsgType_Normal, _T("非MAPPING模式"));
	}

	if (m_markType == g_sMTUnit || m_markType == g_sMTCardAndUnit || m_pWaferDoc->IsExist2DBarcode() || m_pWaferDoc->IsExistBarcode()) {     //开启单颗后，必须要检查2D信息，没有就报警
		PrintMessage(emMsgType_Normal, _T("更新2DBarcode信息"));
		if (-1 == Write2DBarcodeInfo(barcordInfo, barcordInfo2)) {
			return FALSE;
		}
	}
	return TRUE;
}

void CFormWorkControl::DetectionProcessSignal()
{
	if (StopScannerSignalEffective()) {
		KillTimer(SCANSIGNAL_CHECK_TIMER);
		PrintMessage(emMsgType_Normal, _T("侦测到标记结束端口信号，退出批次标记流程"));
		OnBnClickedButtonCanceljob();   // 退出自动模式
		GetDlgItem(IDC_EDIT_OPERID)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LOTID)->EnableWindow(TRUE);
		SetDlgItemText(IDC_EDIT_OPERID, _T(""));
		SetDlgItemText(IDC_EDIT_LOTID, _T(""));
		GetDlgItem(IDC_EDIT_OPERID)->SetFocus();
		return;
	}

	if (CleanMarkFileSignalEffective()) {
		KillTimer(SCANSIGNAL_CHECK_TIMER);
		PrintMessage(emMsgType_Normal, _T("侦测到清空标记图档端口信号"));

		// 清空状态
		ResetStatus();

		// 恢复矩阵按钮状态
		VerifyMutilTrack();

		// 取消JOB
		OnBnClickedButtonCanceljob();
		GetDlgItem(IDC_BUTTON_LOAD_JOB)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_LOAD_SET)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_LASER_PARAM)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MAP_ADJUST)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STARTMARK)->EnableWindow(FALSE);
		//GetDlgItem(IDC_BUTTON_MUTIL_STARTMARK)->EnableWindow(FALSE);
		//GetDlgItem(IDC_EDIT_2D_BARCODE_INFO)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_OPERID)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LOTID)->EnableWindow(TRUE);
		//GetDlgItem(IDC_CHECK_PRINT_TYPE)->EnableWindow(TRUE);
		//SetDlgItemText(IDC_EDIT_2D_BARCODE_INFO, _T(""));
		SetDlgItemText(IDC_EDIT_OPERID, _T(""));
		SetDlgItemText(IDC_EDIT_LOTID, _T(""));
		//GetDlgItem(IDC_EDIT_2D_BARCODE_INFO)->SetFocus();
		return;
	}
}

BOOL CFormWorkControl::StopScannerSignalEffective()
{
	// 低电平有效
	uint32_t nBit = 0x01;
#ifdef DEBUG_IO
	nBit = GetPrivateProfileInt(_T("IO"), _T("StopScanner"), 0, _T("config\\IOTest.ini"));
#else
	GetCommIOStatus(m_pConfig->m_StopScannerSignal, nBit);
#endif	
	return (0x0 == nBit);
}

BOOL CFormWorkControl::CleanMarkFileSignalEffective()
{
	// 低电平有效
	uint32_t nBit = 0x01;
#ifdef DEBUG_IO
	nBit = GetPrivateProfileInt(_T("IO"), _T("CleanMarkFile"), 0, _T("config\\IOTest.ini"));
#else
	GetCommIOStatus(m_pConfig->m_CleanMarkFileSignal, nBit);
#endif	
	return (0x0 == nBit);
}

void CFormWorkControl::VerifyMutilTrack()
{
	//if (nullptr == m_pMapDoc)
	//{
	//	//GetDlgItem(IDC_BUTTON_MATRIX)->ShowWindow(TRUE);
	//	//GetDlgItem(IDC_BUTTON_MUTIL_MATRIX)->ShowWindow(FALSE);

	//	GetDlgItem(IDC_BUTTON_STARTMARK)->ShowWindow(TRUE);
	//	GetDlgItem(IDC_BUTTON_MUTIL_STARTMARK)->ShowWindow(FALSE);
	//	return;
	//}

	//if (IsMutilTrack())
	//{
	//	//GetDlgItem(IDC_BUTTON_MATRIX)->ShowWindow(FALSE);
	//	//GetDlgItem(IDC_BUTTON_MUTIL_MATRIX)->ShowWindow(TRUE);
	//	//SetDlgItemText(IDC_BUTTON_MUTIL_MATRIX, _T("矩阵"));

	//	GetDlgItem(IDC_BUTTON_STARTMARK)->ShowWindow(FALSE);
	//	GetDlgItem(IDC_BUTTON_MUTIL_STARTMARK)->ShowWindow(TRUE);
	//	SetDlgItemText(IDC_BUTTON_MUTIL_STARTMARK, _T("开始标记"));
	//}
	//else
	//{
	//	//GetDlgItem(IDC_BUTTON_MATRIX)->ShowWindow(TRUE);
	//	//GetDlgItem(IDC_BUTTON_MUTIL_MATRIX)->ShowWindow(FALSE);

	//	GetDlgItem(IDC_BUTTON_STARTMARK)->ShowWindow(TRUE);
	//	GetDlgItem(IDC_BUTTON_MUTIL_STARTMARK)->ShowWindow(FALSE);
	//}
}

BOOL CFormWorkControl::ConnectMes()
{
	return TRUE;
}


bool CFormWorkControl::SetCommIOStatus(int port, int sigle)
{
	return m_IOComm.SetIOStatus(port, sigle);
}


bool CFormWorkControl::GetCommIOStatus(int port, unsigned int &sigle)
{
	return m_IOComm.GetIOStatus(port,sigle);
}

void CFormWorkControl::UpdateIOStatus(int type)  //更新状态到界面控件
{
	switch (type)
	{
	case UISIGNAL_MAP:
		m_sStatusMapping.SetStatus(IsMapping() ? CInputStatic::SIGNAL : CInputStatic::NON_SIGNAL);
		break;
	case UISIGNAL_VISION:
		m_sStatusVision.SetStatus(m_IfConnectCCD ? CInputStatic::SIGNAL : CInputStatic::NON_SIGNAL);
		break;
	case UISIGNAL_2D:
		if (m_markType == g_sMTCard || m_markType == g_sMTUnit || m_markType == g_sMTCardAndUnit/* || m_pConfig->m_bCheckEQPStripIdIsMatchLotId*/){
			m_sStatus2DBarcode.SetStatus(CInputStatic::SIGNAL);  
			SetCommIOStatus(m_pConfig->m_nScanUse2DPort, IO_2D_OUT_VALID);
			SetCommIOStatus(m_pConfig->m_nScannReadySingle, IO_2D_OUT_VALID);
		}
		else
		{
			m_sStatus2DBarcode.SetStatus(((m_pConfig->m_bCheckStripMap || m_pConfig->m_bCheck2DBarcode) && !IsDummy()) ? CInputStatic::SIGNAL : CInputStatic::NON_SIGNAL);
			SetCommIOStatus(m_pConfig->m_nScanUse2DPort, ((m_pConfig->m_bCheckStripMap || m_pConfig->m_bCheck2DBarcode) && !IsDummy()) ? IO_2D_OUT_VALID : IO_2D_OUT_INVALID);
			SetCommIOStatus(m_pConfig->m_nScannReadySingle, ((m_pConfig->m_bCheckStripMap || m_pConfig->m_bCheck2DBarcode) && !IsDummy()) ? IO_2D_OUT_VALID : IO_2D_OUT_INVALID);
		}
		break;
	case UISIGNAL_PWS:
		m_sStatusPWS.SetStatus((m_markType == g_sMTCard || m_markType == g_sMTCardAndUnit /*|| m_pConfig->m_bCheckEQPStripIdIsMatchLotId*/) ? CInputStatic::SIGNAL : CInputStatic::NON_SIGNAL);
		SetCommIOStatus(m_pConfig->m_nScanEnablePWS, (m_markType == g_sMTCard || m_markType == g_sMTCardAndUnit /*|| m_pConfig->m_bCheckEQPStripIdIsMatchLotId*/) ? IO_2D_OUT_VALID : IO_2D_OUT_INVALID);
		break;
	case UISIGNAL_DUMMY:
	{
		UpdateIOStatus(UISIGNAL_MAP);
		//UpdateIOStatus(UISIGNAL_2D);
	}
		break;
	case UISIGNAL_NET:
	{		
 		m_sNetConnect.SetStatus(m_bVisionCheck ? CInputStatic::SIGNAL : CInputStatic::NON_SIGNAL);
	}
	break;
	}
	
}

bool CFormWorkControl::GetAndCheckLotID(CString &sLotID)  //LotID检测与转化
{
	if (sLotID.GetLength() < 8) {
		AfxMessageBox(_T("扫入的数据有误：小于8位！"));
		return false;
	}

	//合法性校验
	int n = sLotID.Find(_T("."));
	if (-1 == n) {
		AfxMessageBox(_T("扫入的数据有误：未找到‘.’号！"));
		return false;
	}

	if (8 != n) {
		AfxMessageBox(_T("扫入的数据有误：‘.’ 号前的数据位数不为8位！"));
		return false;
	}
	
	//小写转化为大写
	sLotID = sLotID.MakeUpper();
	m_sLotIDCur = sLotID;
	m_sStripIDCur.Empty();   //每次重新扫码清空stripID

	// 检查过站信息
	if (!USER_VERIFY(USER_O_IGNORE_CHECK_EQUIPMENTID))
	{
		m_pMySoap->SetUrl(m_pConfig->m_sMesTrackInUrl);
		BOOL bMatch = m_pMySoap->IsEquipmentTrackInLotId(m_pConfig->m_sDeviceNo, m_sLotIDCur);
		if (!bMatch)
		{
			CString s;
			s.Format(_T("过站信息[设备ID:%s, LotID:%s]检查失败。详细失败原因：%s"), m_pConfig->m_sDeviceNo, m_sLotIDCur, m_pMySoap->GetLastError());
			AfxMessageBox(s);
			return FALSE;
		}
	}

	return true;
}

bool CFormWorkControl::VerifyStripID(const CString &sStripID)  //检查StripID批次及重复情况
{
	// 目前华天存在两种格式的Strip ID，一种厂内格式(和LotID格式一样)，一个厂外格式（与LotID格式不一样，绑定）
	m_sLotIDBindStripIDCur.Empty();
	CString msg;
	m_sStripIDCur = sStripID;
	if (sStripID.Mid(0, 8) == m_sLotIDCur.Mid(0, 8))
	{
		int iRet = VerifyRepeatStripID(sStripID);
		return (-1 != iRet);
	}

	if (!CheckStripIDMatchLotIDByWebService())
	{
		msg.Format(_T("检测到非本批次号产品！扫入的StripID: %s, 当前批次: %s"), sStripID, m_sLotIDCur);
		PrintMessage(emMsgType_Error, msg);
		return false;
	}

	// 对于厂外基板ID(StripID)绑定的批次号(LotID)也要检查是否混批
	if (m_sLotIDBindStripIDCur.Mid(0, 8) != m_sLotIDCur.Mid(0, 8))
	{
		msg.Format(_T("检测到非本批次号产品！产品绑定的LotID: %s, 当前批次: %s"), m_sLotIDBindStripIDCur, m_sLotIDCur);
		PrintMessage(emMsgType_Error, msg);
		return false;
	}

	int iRet = VerifyRepeatStripID(sStripID);
	return (-1 != iRet);

}

bool CFormWorkControl::CheckCCDStatus()  //检测CCD是否连接
{
	return m_IfConnectCCD;
}

BOOL CFormWorkControl::IsIgnoreCCDCheckAndSwitchVisionProcess()
{
	// 工程师调试版本，取消制程
	if (1 == GetPrivateProfileInt(_T("Global"), _T("IsIgnoreSwitchVisionProcess"), 0, _T(".\\config\\IgnorSwitchVisionProcess.ini")))
	{
		//PrintMessage(emMsgType_Warn, _T("工程调试，取消制程切换动作！"));
		return TRUE;
	}
	return FALSE;
}

BOOL CFormWorkControl::SwitchVisionProcess()
{
	if (IsIgnoreCCDCheckAndSwitchVisionProcess()) return TRUE;
		

	if (nullptr == m_pServer)
	{
		//PrintMessage(emMsgType_Error, _T("未开启视觉服务器！"));
		return FALSE;
	}

	if (nullptr == m_pServerPLC)
	{
		//PrintMessage(emMsgType_Error, _T("未开启PLC服务器！"));
		return FALSE;
	}

	CStringA sVisionProcessName = CStringA(m_sMarkFileName);
	sVisionProcessName = sVisionProcessName.Left(sVisionProcessName.GetLength() - 5);  // 去掉后缀.twfb
	
	// 切换视觉软件制程
	if (m_pConfig->m_bCheckVisionSwitchProcess)
	{
		CStringA sSendCmd = "@SetVisionProcess*" + sVisionProcessName;
		PrintMessage(emMsgType_Normal, CString(_T("LASER->Vision：")) + CString(sSendCmd));
		if (!m_pServer->SendRemote(sSendCmd.GetString(), sSendCmd.GetLength()))
			return FALSE;
	}
	else
	{
		//PrintMessage(emMsgType_Warn, _T("未启用视觉软件制程切换动作！"));
	}

	// 切换PLC软件制程
	if (m_pConfig->m_bCheckPLCSwitchProcess)
	{
		CStringA sSendCmd2 = sVisionProcessName;
		PrintMessage(emMsgType_Normal, CString(_T("LASER->PLC：")) + CString(sSendCmd2));
		if (!m_pServerPLC->SendRemote(sSendCmd2.GetString(), sSendCmd2.GetLength()))
			return FALSE;
	}
	else
	{
		//PrintMessage(emMsgType_Warn, _T("未启用PLC软件制程切换动作！"));
	}

	// 切换标后检测制程
	bool bCheckVision = m_pConfig->m_bVisionCheck;
	if (bCheckVision){
		// 视觉检测采用印章名称，不带后缀
		CString sSendCmd3 = L"@SetVisionProcess�" + m_sMarkStampNameNonSuffix + L";" + m_sLotID;
		PrintMessage(emMsgType_Normal, _T("LASER->视觉检测：") + sSendCmd3);
		CStringA msga = chT2U(sSendCmd3.GetString()).c_str();
		if (!m_pVisionCheckServer->SendRemote(msga, msga.GetLength()))
		{
			PrintMessage(emMsgType_Error, _T("视觉检测切换制程失败！"));
			return FALSE;
		}
	}
	else{
		//PrintMessage(emMsgType_Warn, _T("未启用标后检测功能！"));
	}

	if (!m_pConfig->m_bCheckVisionSwitchProcess 
		&& !m_pConfig->m_bCheckPLCSwitchProcess
		&& !(bCheckVision))
	{
		return TRUE;
	}
	CMarkPlatform::Instance()->Delay(100);

	int times = 600;
	while (times > 0)
	{
		if ((!m_pConfig->m_bCheckVisionSwitchProcess || m_bSetVisionOK) 
			&& (!m_pConfig->m_bCheckPLCSwitchProcess || m_bPLCSwitchProcessOK)
			&& (!bCheckVision || m_bSwitchVisionCheckProcessOK))
			return TRUE;

		CMarkPlatform::Instance()->Delay(100);
		times--;
	}

	if (m_pConfig->m_bCheckVisionSwitchProcess && !m_bSetVisionOK) {
		PrintMessage(emMsgType_Error, _T("CCD切换制程失败！"));
	}
	if (m_pConfig->m_bCheckPLCSwitchProcess && !m_bPLCSwitchProcessOK) {
		PrintMessage(emMsgType_Error, _T("PLC切换制程失败！"));
	}
	if (bCheckVision && !m_bSwitchVisionCheckProcessOK) {
		PrintMessage(emMsgType_Error, _T("视觉检测切换制程失败！"));
	}

	return FALSE;
}



void CFormWorkControl::OnBnClickedCheckTryDummy()
{
	// TODO: 在此添加控件通知处理程序代码
	if (IsDummy())
	{
		// 首检选打，在工程师权限下不限次数选用
		if ((!m_bFirstLoadDocForDummy) && (USERMNG->GetCurUser() != USER_A_ADMIN))
		{
			EnableDummy(0);
		}
		else
		{
			if (!RightConfirmBox(USER_O_ENABLE_DUMMY))
			{
				EnableDummy(0);
			}
			else
			{
				if(USERMNG->GetCurUser() != USER_A_ADMIN) m_bFirstLoadDocForDummy = false;
				EnableDummy(1);
			}
		}
	}
	else
	{
		EnableDummy(0);
	}
}

bool CFormWorkControl::IsDummy()
{
	return (1 == ((CButton*)GetDlgItem(IDC_CHECK_TRY_DUMMY))->GetCheck());
}

void CFormWorkControl::EnableDummy(bool b)
{
	((CButton*)GetDlgItem(IDC_CHECK_TRY_DUMMY))->SetCheck((int)b);
	UpdateIOStatus(UISIGNAL_DUMMY);
	m_pConfig->m_bEnableDummy = b;

	if (b) EnableFirstPrint(false);

}

bool CFormWorkControl::IsFirstPrint()
{
	return (1 == ((CButton*)GetDlgItem(IDC_CHECK_FIRST_PRINT))->GetCheck());
}

void CFormWorkControl::EnableFirstPrint(bool b)
{
	((CButton*)GetDlgItem(IDC_CHECK_FIRST_PRINT))->SetCheck((int)b);
	m_pConfig->m_bEnableFirstPrint = b;
	if (b) EnableDummy(false);
}

bool CFormWorkControl::RightConfirmBox(const emUserOperatorType_t& emUserRight, bool bChangeUserToOper)
{
	CString msg;

	// 切换用户到操作员
	auto ChangeUserToOper = [&]() {
		if (bChangeUserToOper)
		{
			USERMNG->ChangeUser(USER_A_OPER);
			msg.Format(_T("切换到用户[%s]"), USERMNG->GetCurUserName());
			PrintMessage(emMsgType_Normal, msg);
		}
		m_pMainDlg->UpdateAppCaption();
	};

	if (USER_VERIFY(emUserRight))
	{
		ChangeUserToOper();	
		return true;
	}
	else
	{
		CDlgFactoryPwd pwd;
		if (IDOK == pwd.DoModal())
		{
			if (USER_VERIFY(emUserRight))
			{
				msg.Format(_T("用户执行[%s]动作，切换到用户[%s]"), g_sUserOperatorCaption[emUserRight], USERMNG->GetCurUserName());
				PrintMessage(emMsgType_Normal, msg);
				ChangeUserToOper();
				return true;
			}
			else
			{
				AfxMessageBox(_T("此用户不存在操作权限!"));
			}
		}
		ChangeUserToOper();
		return false;
	}
}

CString CFormWorkControl::GetLotIDCur()
{
	CString sLotID;
	//第一次为lotID扫入，从第二次开始为从扫码器扫取（再截取出lotID）
	if (m_sStripIDCur.IsEmpty()) 
	{
		sLotID = m_sLotIDCur;
	}
	else
	{
		if (!m_sLotIDBindStripIDCur.IsEmpty())
		{
			// 厂外StripID，采用系统绑定的LotID
			sLotID = m_sLotIDBindStripIDCur;
		}
		else
		{
			int n = m_sStripIDCur.Find(_T("-"));
			sLotID = m_sStripIDCur.Mid(0, n);
		}
	}

	return sLotID;
}

bool CFormWorkControl::IsMapping()
{
	if(IsDummy())
	{
		// 首检模式时，UNIT和CARDANDUNIT模式，以及勾选Mapping设置并设置了Mapping调试(不良品对比)，启动Mapping
		return m_markType == g_sMTUnit 
			|| m_markType == g_sMTCardAndUnit 
			|| (m_pConfig->m_bCheckStripMap && 1 == m_pConfig->m_iMappingDebugSet);
	}
	else
	{
		return m_pConfig->m_bCheckStripMap || m_markType == g_sMTUnit || m_markType == g_sMTCardAndUnit;
	}
}

void CFormWorkControl::OnBnClickedCheckFirstPrint()
{
	// TODO: 在此添加控件通知处理程序代码
	if (IsFirstPrint())
	{
		if (!RightConfirmBox(USER_O_ENABLE_DUMMY))
		{
			EnableFirstPrint(0);
		}
		else
		{
			EnableFirstPrint(1);
		}
	}
	else
	{
		EnableFirstPrint(0);
	}
}


bool CFormWorkControl::SaveLaserParamFile(const CString& sLotID)
{
	if (sLotID.IsEmpty()) return false;

	CString sPath = m_pConfig->m_sLaserParamSave + _T("\\") + sLotID + _T(".txt");
    
	CStdioFile f;
	if (!f.Open(sPath, CFile::modeReadWrite | CFile::modeCreate))
	{
		return false;
	}

	// 只保存一组
	for (int i = 0; i < 1; i++)
	{
		CString s;
		s.Format(_T("P=%s;I=%.1f;F=%.0f;T=%d\r\n"), m_sPenName[i], m_dbPower[i] * 100, m_dbFreq[i], m_nSpeed[i]);
		f.WriteString(s);
	}

	f.Close();

	return true;
}

void CFormWorkControl::Delete2DBarcodeContent()
{
	// 中午12点开始清空数据
	CTime tm = CTime::GetCurrentTime();
	if (tm.GetHour() == 12)
	{
		std::thread t1([&]() {
			m_2DRepeatCheck.DeleteDataOutOfDate(); 
		});
		t1.detach();
	}
}

void CFormWorkControl::ReadFileContent(const CString&filepath)
{
	
	CStdioFileCodePage file;
	CString sContent;
	if (!file.OpenWithoutType(filepath, CFile::modeRead))
	{
		AfxMessageBox(_T("文件打开失败"));
		
	}

	m_stripIDInfo.Empty();

	while (file.ReadStringWithOutType(sContent))
	{
		m_stripIDInfo += sContent;
	}

}

void CFormWorkControl::ResetMappingDebugSet()
{
	m_pConfig->m_iMappingDebugSet = 0;
	m_pConfig->m_iManualBinMap.clear();
}

bool CFormWorkControl::SetMappingDebug()
{
	// 只有dummy模式才会设置
	if (IsDummy())
	{
		CString s;
		if (1 == m_pConfig->m_iMappingDebugSet)
		{
			const int iBinNo = 0;  // BIN_0为正常内容
			CWaferGroup* pWaferGroup = m_pMapDoc->GetGroup(0);
			int iWaferSize = m_iBinMap.size();
			for (int i = 0; i < iWaferSize; i++)
			{
				// 良品设置了打印，也要报警
				if (pWaferGroup->vWaferMetas[i].IsMarkable()) {
					// 如果设置了打印良品，报警
					if (m_iBinMap[i] == iBinNo) {
						s.Format(_T("设置了打印良品，请确认！"));
						PrintMessage(emMsgType_Error, s);
						return false;
					}
				}
			}

			m_pMapDoc->ChangeWaferID(0);   // 全部改为BIN0
		}
		else if (2 == m_pConfig->m_iMappingDebugSet)
		{
			if (m_pWaferDoc->GetWaferCount() <= 1) 
			{
				s.Format(_T("BIN1不存在！在印章模板中添加BIN1"));
				PrintMessage(emMsgType_Error, s);
				return false;
			}
			m_pMapDoc->ChangeWaferID(1);   // 全部改为BIN1
		}
		// add by hhhuang 20260629 : 打印设置的BIN，按照m_iManualBinMap打印
		else if (3 == m_pConfig->m_iMappingDebugSet)
		{
			CWaferGroup* pWaferGroup = m_pMapDoc->GetGroup(0);
			int iWaferSize = m_pConfig->m_iManualBinMap.size();
			for (int i = 0; i < iWaferSize; i++)
			{
				pWaferGroup->vWaferMetas[i].SetWaferID(m_pConfig->m_iManualBinMap[i]);
			}
		}
	}

	return true;
}



void CFormWorkControl::OnBnClickedButton1()
{
	CHttpComHT com(this);
	CString errMsg;
	CString sLotID = L"TQ27D931.1";
	if (!com.CheckProductType(sLotID, _T("Card"), errMsg))
	{
		AfxMessageBox(errMsg);
		return;
	}
	AfxMessageBox(_T("校验成功"));
	return;

	float fness;

	if (com.GetWaferThickness(sLotID))
	{
		fness = com.ReturnThickNess();
		CString str;
		str.Format(L"成功获取晶圆厚度：%f", fness);

		PrintMessage(emMsgType_Success, str);
		if (m_pConfig->m_bCheckFinsPLC)
		{
			if (FINSCLIENT->ConnServer(m_pConfig->m_sFinsIP, m_pConfig->m_iFinsPort))
			{
				FINSCLIENT->WriteShort("DM5941.1", 1);
				PrintMessage(emMsgType_Normal, _T("清空PLC厚度数据！"));
				int iFness = fness * 1000;
				unsigned short iValue = 0;
				CString str;
				str.Format(L"Fins写入晶圆厚度:%d成功", iValue);
				(FINSCLIENT->WriteShort("DM5941.1", iFness)) == true ? ((FINSCLIENT->ReadShort("DM5941", iValue) == true) ? (PrintMessage(emMsgType_Success, L"Fins写入晶圆厚度成功！")) : (PrintMessage(emMsgType_Error, L"Fins写入晶圆厚度失败！"))) : (PrintMessage(emMsgType_Error, L"Fins写入晶圆厚度失败！"));
				return;
			}
			PrintMessage(emMsgType_Error, _T("FincTCP连接失败！"));
		}


	}
	else
	{
		PrintMessage(emMsgType_Error, _T("获取晶圆厚度失败！"));
	}
}

void CFormWorkControl::UpdateTransparentControlBK(UINT uID)
{
	CRect rc;
	GetDlgItem(uID)->GetWindowRect(&rc);
	ScreenToClient(&rc);
	InvalidateRect(rc);
	UpdateWindow();  //一定要加这一句
}

void CFormWorkControl::UpdateBK()
{
	CRect rc;
	GetClientRect(&rc);
	InvalidateRect(rc);
	UpdateWindow();  //一定要加这一句
}

void CFormWorkControl::ShowLaserParamLeft(int index)
{
	if (index >= m_sPenName.size()) return;
	CString sLaserCaption;
	sLaserCaption.Format(_T(" 功率：%.2f%%\r\n 频率：%.2fKHz\r\n 标记速度：%dmm/s"),
		m_dbPower[index] * 100, m_dbFreq[index], m_nSpeed[index]);
	SetStaticText(IDC_STATIC_LASER_PARAM, sLaserCaption);
}

void CFormWorkControl::ClearLaserParamShow()
{
	SetStaticText(IDC_STATIC_LASER_PARAM, _T(""));
	while (m_listLaserParam.GetCount() > 0)
		m_listLaserParam.DeleteString(0);

	UpdateTransparentControlBK(IDC_LIST_LASER_PARAM);
}


void CFormWorkControl::OnLbnSelchangeListLaserParam()
{
	// TODO: 在此添加控件通知处理程序代码
	int iSel = m_listLaserParam.GetCurSel();
	if (iSel == -1) return;

	UpdateTransparentControlBK(IDC_LIST_LASER_PARAM);

	ShowLaserParamLeft(iSel);
}

BOOL CFormWorkControl::CheckMesInfo()
{
	if (m_nMapOKCount == 0)
	{
		return TRUE;
	}
	if(!m_pConfig->m_EnableFinishedLot)
	{
		//PrintMesMessage(emMsgType_Warn, _T("未启用MES完成批次信息上传功能！"));
		return TRUE;
	}
	//m_pFactoryConfig->m_sDeviceNo
	CString sLotID;
	GetDlgItemText(IDC_EDIT_LOTID, sLotID);

	CString strServer, strObject;
	INTERNET_PORT nPort;
	DWORD dwServiceType;
	if (!AfxParseURL(m_pConfig->m_FinishedLotUrl, dwServiceType, strServer, strObject, nPort))
	{
		CString errmsg;
		errmsg.Format(_T("检查产品类型的URL[%s]格式错误"), m_pConfig->m_FinishedLotUrl);
		return FALSE;
	}
	CICMesComEAPHttpQp p;
	
	p.ConnectMes(strServer, nPort,this);
	map<CString, CString> mapInfo;
	mapInfo[_T("EQPID")] = CFactoryConfig::Instance()->m_sDeviceNo;
	mapInfo[_T("LOTID")] = sLotID;
	mapInfo[_T("EQPMODEL")] = _T("HTM-3032");
	CString s;
	s.Format(_T("%d"), m_nMapOKCount+m_nMapNGCount);// CFactoryConfig::Instance()->m_nMaxMarkTimes);
	mapInfo[_T("QTY")] = s;
	s.Format(_T("%d"), m_nMapNGCount);
	mapInfo[_T("DEFECTQTY")] =s;
	return p.GetFinishLotInfo(mapInfo);
	//return FALSE;
}

BOOL CFormWorkControl::QueryEqpInfo()
{

	if (!m_pConfig->m_EnableFinishedLot)
	{
		//PrintMesMessage(emMsgType_Warn, _T("未启用MES完成批次信息上传功能！"));
		return TRUE;
	}
	//m_pFactoryConfig->m_sDeviceNo
	CString sLotID;
	GetDlgItemText(IDC_EDIT_LOTID, sLotID);

	CString strServer, strObject;
	INTERNET_PORT nPort;
	DWORD dwServiceType;
	if (!AfxParseURL(m_pConfig->m_FinishedLotUrl, dwServiceType, strServer, strObject, nPort))
	{
		CString errmsg;
		errmsg.Format(_T("检查产品类型的URL[%s]格式错误"), m_pConfig->m_FinishedLotUrl);
		return FALSE;
	}
	CICMesComEAPHttpQp p;

	p.ConnectMes(strServer, nPort, this);
	map<CString, CString> mapInfo;
	mapInfo[_T("lotid")] = _T("");
	mapInfo[_T("eqpid")] = _T("");
	mapInfo[_T("fatr")] = _T("false");

	return p.GetFinishLotInfo(mapInfo);
	return 0;
}
