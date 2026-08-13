
// TeteFactoryDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TeteFactory.h"
#include "TeteFactoryDlg.h"
#include "afxdialogex.h"

#include "MarkPlatform.h"
#include "Configure.h"
#include "DlgFactoryPwd.h"
#include "DlgMarkTimesSet.h"
#include "DlgLaserUsage.h"

#include "MyFtp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <ICMesComEAPHttpQP.h>

#define  GETLASTINPUTINFO  1

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetDlgItemText(IDC_STATIC_TITLE, g_sWindowText);
	CString s;
	s.Format(_T("Copyright(C) %s"), g_sUpdateTime);
	SetDlgItemText(IDC_STATIC_TIME, s);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}



// CTeteFactoryDlg 对话框



CTeteFactoryDlg::CTeteFactoryDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TETEFACTORY_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_BkBrush.CreateSolidBrush(BKCOLOR);
	m_EditBkBrush.CreateSolidBrush(EDIT_BKCOLOR);
	m_bInit = FALSE;

}

CTeteFactoryDlg::~CTeteFactoryDlg()
{
	m_BkBrush.DeleteObject();
	m_StatusFont.DeleteObject();
	m_bigFont.DeleteObject();
	m_EditBkBrush.DeleteObject();

	
}


void CTeteFactoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT_INFO, m_RichEdit);
	DDX_Control(pDX, IDC_TAB_MESSAGE, m_tabMessage);
	DDX_Control(pDX, IDC_TAB_MAIN, m_tabMainFrame);
}

BEGIN_MESSAGE_MAP(CTeteFactoryDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, &CTeteFactoryDlg::OnTcnSelchangeTabMain)
	ON_WM_NCHITTEST()
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB_MAIN, &CTeteFactoryDlg::OnSelchangingTabMain)
	ON_BN_CLICKED(IDC_BUTTON_HISTORYLOG, &CTeteFactoryDlg::OnBnClickedButtonHistorylog)
	ON_BN_CLICKED(IDC_BUTTON_LASER_USAGE, &CTeteFactoryDlg::OnBnClickedButtonLaserUsage)
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCRBUTTONDOWN()
	ON_WM_MOUSEACTIVATE()
	ON_BN_CLICKED(IDC_BUTTON_IP, &CTeteFactoryDlg::OnBnClickedButtonIp)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CTeteFactoryDlg::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDC_BUTTON_ROTATE_CENTER, &CTeteFactoryDlg::OnBnClickedButtonRotateCenter)
END_MESSAGE_MAP()


// CTeteFactoryDlg 消息处理程序

BOOL CTeteFactoryDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	// 软件最大化
	ShowWindow(SW_SHOWMAXIMIZED);

	// 设置为操作员权限
	USERMNG->ChangeUser(USER_A_OPER);

	//设置标题
	UpdateAppCaption();
	GetClientRect(&m_rcClient);

	// 设置状态字体
	m_StatusFont.CreateFont(25,
		12, 0, 0, 
		FW_BOLD,
		FALSE, FALSE, 0, 
		DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS,
		CLEARTYPE_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		_T("Arial"));
	m_bigFont.CreatePointFont(120, _T("Arial"));
	GetDlgItem(IDS_MARK_STATUS)->SetFont(&m_StatusFont/*m_bigFont*/);
	GetDlgItem(IDC_STATIC_MARKCOUNT)->SetFont(&m_bigFont);
	GetDlgItem(IDC_MARKCOUNT)->SetFont(&m_bigFont);
	GetDlgItem(IDC_STATIC_LASERPARAM)->SetFont(&m_bigFont);
	GetDlgItem(IDC_BUTTON_HISTORYLOG)->SetFont(&m_bigFont);
	//ADD by hcx 20240226
	GetDlgItem(IDC_BUTTON_IP)->SetFont(&m_bigFont);
	GetDlgItem(IDC_BUTTON_TEST)->SetFont(&m_bigFont);
	//end by hcx 20240226
	GetDlgItem(IDC_BUTTON_LASER_USAGE)->SetFont(&m_bigFont);
	GetDlgItem(IDC_STATIC_MAX_MARK_COUNT)->SetFont(&m_bigFont);
	GetDlgItem(IDC_EDIT_MAX_MARK_COUNT)->SetFont(&m_bigFont);

	// 初始化RichEdit
	InitRichFont();

	// 主Tab
	m_tabMainFrame.InsertItem(0, _T("系统设置"));
	m_tabMainFrame.InsertItem(1, _T("作业控制"));
	//m_tabMainFrame.InsertItem(2, _T("工作日志"));
	m_tabMainFrame.SetCurSel(1);

	// 消息Tab
	m_tabMessage.InsertItem(0, _T("消息输出"));

	// 控件位置调整
	AdjustControl();

	// 加载Config
	CFactoryConfig * pFactoryConfig = CFactoryConfig::Instance();
	pFactoryConfig->LoadConfig();

	// 创建子窗口
	CreateChildWnd();
	SwitchChildWnd();

	m_bInit = TRUE;
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTeteFactoryDlg::AdjustControl()
{
	int nOffset = 5;
	int nW = m_rcClient.Width() - 2 * nOffset;
	int nH = m_rcClient.Height() - 2 * nOffset;
	int nTabH = 25, nTabW = 150;
	// 主标签卡， 占整界面85%高， 100%宽
	m_tabMainFrame.SetItemSize(CSize(nTabW, nTabH));
	m_tabMainFrame.MoveWindow(nOffset, nOffset, nW, nH * 0.85);
	// 标记状态栏
	int nMarkStatusW = 350;
	int nMarkStatusH = 20;
	GetDlgItem(IDS_MARK_STATUS)->MoveWindow(m_rcClient.right-nOffset- nMarkStatusW, 
		10, nMarkStatusW, nMarkStatusH);

	int nBtnH = 35;    //按钮高度
	int nBtnW = 110;   //按钮宽度
	// 消息输出Tab， 占整界面30%高，
	int nTabMessageH = 20;         //消息输出框上方的宽度
	int nMessageY = nH*0.85 + nOffset*2;
	m_tabMessage.SetItemSize(CSize(80, nTabMessageH));
	m_tabMessage.MoveWindow(nOffset, nMessageY, nW - nBtnW - nOffset * 2, nH*0.15);
	m_RichEdit.MoveWindow(nOffset*2, nMessageY + nTabMessageH + nOffset,
		nW - nBtnW - nOffset * 4, nH*0.15 - nTabMessageH - nOffset*2);
	CRect rectTemp;
	GetDlgItem(IDC_BUTTON_HISTORYLOG)->GetWindowRect(rectTemp);
	//ADD by hcx 20240226
	GetDlgItem(   IDC_BUTTON_IP)->GetWindowRect(rectTemp);
	GetDlgItem(IDC_BUTTON_TEST)->GetWindowRect(rectTemp);
	//end by hcx 20240226
	
	int bnHeight = rectTemp.Height();

	// 按钮位置
	GetDlgItem(IDC_BUTTON_HISTORYLOG)->MoveWindow(m_rcClient.right - nBtnW - nOffset*2,
		nMessageY + nTabMessageH, nBtnW, nBtnH);
	//ADD by hcx 20240226
	GetDlgItem(IDC_BUTTON_IP)->MoveWindow(m_rcClient.right - nBtnW - nOffset * 2,
		nMessageY + nTabMessageH*2 + nBtnH, nBtnW, nBtnH);

	GetDlgItem(IDC_BUTTON_ROTATE_CENTER)->MoveWindow(m_rcClient.right - nBtnW - nOffset * 2,
		nMessageY + nTabMessageH*2.5 + 2 * nBtnH, nBtnW, nBtnH);
	//end by hcx 20240226
	
	GetDlgItem(IDC_BUTTON_LASER_USAGE)->MoveWindow(m_rcClient.right - nBtnW - nOffset * 2,
		nMessageY + nTabMessageH*2 + bnHeight, nBtnW, nBtnH);

	//打印计数位置
	int nMarkCountW = 100;
	int nMarkCountH = 18;
	GetDlgItem(IDC_MARKCOUNT)->MoveWindow(m_rcClient.right - nOffset - nBtnW - nMarkCountW,
		nMessageY, nMarkCountW, nMarkCountH);
	GetDlgItem(IDC_STATIC_MARKCOUNT)->MoveWindow(m_rcClient.right - nOffset - nBtnW - nMarkCountW * 2, 
		nMessageY, nMarkCountW, nMarkCountH);

	//激光参数位置
	GetDlgItem(IDC_STATIC_LASERPARAM)->MoveWindow(m_rcClient.left + 100,
		nMessageY, m_rcClient.Width() - 100 - nMarkCountW * 2, nMarkCountH);


	//int nMaxMarkCountW = 150;
	//int nMaxMarkCountH = 18;
	//GetDlgItem(IDC_EDIT_MAX_MARK_COUNT)->MoveWindow(m_rcClient.right - nOffset - nBtnW - nMarkCountW * 5,
	//	nMessageY, nMaxMarkCountW, nMaxMarkCountH*2);
	//GetDlgItem(IDC_STATIC_MAX_MARK_COUNT)->MoveWindow(m_rcClient.right - nOffset - nBtnW - nMarkCountW * 5 - nMaxMarkCountW,
	//	nMessageY, nMaxMarkCountW, nMaxMarkCountH);

}

void CTeteFactoryDlg::CreateChildWnd()
{
	CRect rcMainFrame;
	m_tabMainFrame.GetClientRect(&rcMainFrame);
	rcMainFrame.InflateRect(0, -28, 0, -2);
	m_pSet = new CDlgFactorySet(this, &m_tabMainFrame);
	m_pSet->Create(IDD_DIALOG_FACTORY_SET, &m_tabMainFrame);
	m_pSet->MoveWindow(rcMainFrame);
	m_pSet->ShowWindow(FALSE);

	m_pWorkControl = new CFormWorkControl(this, &m_tabMainFrame);
	m_pWorkControl->Create(IDD_FORM_WORK_CONTROL, &m_tabMainFrame);
	m_pWorkControl->MoveWindow(rcMainFrame);
	m_pWorkControl->ShowWindow(FALSE);

	//m_pHisLog = new CFormHisLog(&m_tabMainFrame);
	//m_pHisLog->Create(IDD_FORM_HISTORY_LOG, &m_tabMainFrame);
	//m_pHisLog->MoveWindow(rcMainFrame);
	//m_pHisLog->ShowWindow(FALSE);
}

void CTeteFactoryDlg::SwitchChildWnd()
{
	m_pSet->ShowWindow(FALSE);
	m_pWorkControl->ShowWindow(FALSE);
	//m_pHisLog->ShowWindow(FALSE);
	int nSel = m_tabMainFrame.GetCurSel();
	if (0 == nSel)
	{
		m_pSet->ShowWindow(TRUE);
	}
	else if (1 == nSel)
	{
		m_pWorkControl->ShowWindow(TRUE);
	}
	else if (2 == nSel)
	{
		//m_pHisLog->ShowWindow(TRUE);
	}
}

void CTeteFactoryDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTeteFactoryDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTeteFactoryDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTeteFactoryDlg::OnSize(UINT nType, int cx, int cy)
{
	// TODO: 在此处添加消息处理程序代码

	CDialogEx::OnSize(nType, cx, cy);
	if (nType != SIZE_MINIMIZED)  //判断窗口是不是最小化了，因为窗口最小化之后 ，窗口的长和宽会变成0，当前一次变化的时就会出现除以0的错误操作
	{
		if (m_bInit)
		{
			GetClientRect(&m_rcClient);   //最后要更新对话框的大小，当做下一次变化的旧坐标；
			AdjustControl();
		}
	}
}

BOOL CTeteFactoryDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (WM_KEYDOWN == pMsg->message && VK_ESCAPE == pMsg->wParam)
		return TRUE;

	//不响应标题栏双击
	if (pMsg->message == WM_NCLBUTTONDBLCLK) // 非client区域的双击消息
		if (pMsg->wParam == HTCAPTION) // 光标所在区域为标题栏
			return TRUE;

	if (pMsg->wParam == VK_F8)
	{
	}
	else if (pMsg->wParam == VK_F10)
	{
	}
	else if (pMsg->wParam == VK_F9)   // 开始mark
	{
	}
	else if (pMsg->wParam == VK_F11)  // 停止mark
	{
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CTeteFactoryDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	
}

void CTeteFactoryDlg::InitRichFont()
{
	// 正常
	memset(&m_cfNor, '\0', sizeof m_cfNor);
	m_cfNor.cbSize = sizeof(m_cfNor);
	m_cfNor.dwMask = CFM_SIZE | CFM_COLOR | CFM_BOLD | CFM_ITALIC;
	m_cfNor.dwEffects &= ~CFE_BOLD;
	m_cfNor.dwEffects &= ~CFE_ITALIC;
	m_cfNor.yHeight = 200;
	m_cfNor.crTextColor = STATIC_COLOR;

	// 标题
	memset(&m_cfTitle, '\0', sizeof m_cfTitle);
	m_cfTitle.cbSize = sizeof(m_cfTitle);
	m_cfTitle.dwMask = CFM_SIZE | CFM_COLOR | CFM_BOLD | CFM_ITALIC;
	m_cfTitle.dwEffects &= ~CFE_BOLD;
	m_cfTitle.dwEffects &= ~CFE_ITALIC;
	m_cfTitle.yHeight = 250;
	m_cfTitle.crTextColor = STATIC_TITLE_COLOR;

	// 警告
	memset(&m_cfWarn, '\0', sizeof m_cfWarn);
	m_cfWarn.cbSize = sizeof(m_cfWarn);
	m_cfWarn.dwMask = CFM_SIZE | CFM_COLOR | CFM_BOLD | CFM_ITALIC;
	m_cfWarn.dwEffects &= ~CFE_BOLD;
	m_cfWarn.dwEffects &= ~CFE_ITALIC;
	m_cfWarn.yHeight = 200;
	m_cfWarn.crTextColor = RGB(250, 0, 250);

	// 错误
	memset(&m_cfError, '\0', sizeof m_cfError);
	m_cfError.cbSize = sizeof(m_cfError);
	m_cfError.dwMask = CFM_SIZE | CFM_COLOR | CFM_BOLD | CFM_ITALIC;
	m_cfError.dwEffects &= ~CFE_BOLD;
	m_cfError.dwEffects &= ~CFE_ITALIC;
	m_cfError.yHeight = 200;
	m_cfError.crTextColor = RGB(250, 0, 0);

	// 成功
	memset(&m_cfPass, '\0', sizeof m_cfPass);
	m_cfPass.cbSize = sizeof(m_cfPass);
	m_cfPass.dwMask = CFM_SIZE | CFM_COLOR | CFM_BOLD | CFM_ITALIC;
	m_cfPass.dwEffects &= ~CFE_BOLD;
	m_cfPass.dwEffects &= ~CFE_ITALIC;
	m_cfPass.yHeight = 200;
	m_cfPass.crTextColor = RGB(0, 250, 0);


	//RichEdit 去除控件自动字体功能, 初始化字体
	DWORD dwFontStyle = m_RichEdit.SendMessage(EM_GETLANGOPTIONS, NULL, NULL);
	if (dwFontStyle & IMF_AUTOFONT)
	{
		dwFontStyle &= ~IMF_AUTOFONT;
		m_RichEdit.SendMessage(EM_SETLANGOPTIONS, NULL, (LPARAM)dwFontStyle);
	}
	m_RichEdit.SetDefaultCharFormat(m_cfNor);

	m_RichEdit.ModifyStyleEx(0, WS_EX_TRANSPARENT, SWP_FRAMECHANGED);
}

void CTeteFactoryDlg::SetRichEditText(const CString& csText, CHARFORMAT& newcf, BOOL bChangecf)
{
	m_RichEdit.SetSel(-1, -1);  // 解决拷贝编辑框中的内容后，颜色无法改变的异常
	if (bChangecf)
		m_RichEdit.SetWordCharFormat(newcf);
	m_RichEdit.SetSel(-1, -1);
	m_RichEdit.ReplaceSel(csText);
	if (bChangecf)
		m_RichEdit.SetWordCharFormat(m_cfNor);
}

void CTeteFactoryDlg::PrintMessage(emFactoryMsgType_t nType, const CString & s)
{
	CString sText = s;
	switch (nType)
	{
	case emMsgType_Normal:
		SetRichEditText(sText, m_cfNor, 0);
		break;
	case emMsgType_Title:
		sText = _T("\r\n>>>>> ") + s + _T("\n");
		SetRichEditText(sText, m_cfTitle, 1);
		break;
	case emMsgType_Warn:
		SetRichEditText(sText, m_cfWarn, 1);
		break;
	case emMsgType_Error:
		SetRichEditText(sText, m_cfError, 1);
		break;
	case emMsgType_Success:
		SetRichEditText(sText, m_cfPass, 1);
		break;
	}

	m_RichEdit.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
}

void CTeteFactoryDlg::PrintAndLogMessage(emFactoryMsgType_t nType, const CString & s)
{
	m_pWorkControl->PrintMessage(nType, s);
}

void CTeteFactoryDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
    // 检查
	if (!m_pWorkControl->CheckClose())
		return;

	if (m_uGetLastInputInfo)
	{
		KillTimer(GETLASTINPUTINFO);
	}

	CDialogEx::OnClose();
}

HBRUSH CTeteFactoryDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	//在这加一条是否为对话框的判断语句
	if (nCtlColor == CTLCOLOR_DLG)
		return (HBRUSH)m_BkBrush.GetSafeHandle(); 

	if (pWnd->GetDlgCtrlID() == IDS_MARK_STATUS)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(STATIC_TITLE_COLOR);
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	}
	else if (pWnd->GetDlgCtrlID() == IDC_MARKCOUNT
		|| pWnd->GetDlgCtrlID() == IDC_STATIC_MARKCOUNT
		|| pWnd->GetDlgCtrlID() == IDC_STATIC_MAX_MARK_COUNT
		|| pWnd->GetDlgCtrlID()== IDC_STATIC_LASERPARAM)
	{
		// 背景为透明
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(STATIC_TITLE_COLOR);
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	}
	else if (pWnd->GetDlgCtrlID() == IDC_MARKCOUNT) {
		pDC->SetBkColor(EDIT_BKCOLOR);//设置字体背景颜色
		pDC->SetTextColor(EDIT_TEXT_COLOR);//设置编辑框字体的颜色
		return (HBRUSH)m_EditBkBrush.GetSafeHandle();
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CTeteFactoryDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	LASTINPUTINFO lpi;
	DWORD dwTime = 0;

	switch (nIDEvent)
	{
	case GETLASTINPUTINFO:// 获取键盘和鼠标处于空闲状态的时间

		lpi.cbSize = sizeof(lpi);
		GetLastInputInfo(&lpi);
		dwTime = ::GetTickCount() - lpi.dwTime;

		if (dwTime >= 1000 * 60 * 1)  // 1分钟
		{
			if (m_uGetLastInputInfo)
			{
				KillTimer(GETLASTINPUTINFO);
				m_uGetLastInputInfo = 0;
			}
			USERMNG->ChangeUser(USER_A_OPER);
			UpdateAppCaption();
			m_pSet->EnableControlByRights();
			CString msg;
			msg.Format(_T("1分钟未操作软件，切换到用户[操作员]"));
			m_pWorkControl->PrintMessage(emMsgType_Normal, msg);
		}
		break;
	default:
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

BOOL CTeteFactoryDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值


	return __super::OnEraseBkgnd(pDC);
}


void CTeteFactoryDlg::OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel = m_tabMainFrame.GetCurSel();
	//切到"系统控制"页 
	if (!m_pWorkControl->IsSwitchSetTab() && 0 == nSel)
	{
		m_tabMainFrame.SetCurSel(1);
		*pResult = 1;  // 阻止选项卡切换
		return;
	}

	if (1 == nSel)
	{
		//切到"作业控制"页 
		if (FALSE == m_pSet->SaveParam())
		{
			m_tabMainFrame.SetCurSel(0);
			*pResult = 1;  // 阻止选项卡切换
			return;
		}
		m_pSet->ChangeTab(); //lrf 切换视图后重置页面
	}

	SwitchChildWnd();

	*pResult = 0;
}

void CTeteFactoryDlg::ClearMessage()
{
	SetDlgItemText(IDC_RICHEDIT_INFO, _T(""));
	GetDlgItem(IDC_RICHEDIT_INFO)->UpdateWindow();
}

void CTeteFactoryDlg::SetStaticText(UINT uCtlID, const CString & s)
{
	SetDlgItemText(uCtlID, s);
	CRect rc;
	GetDlgItem(uCtlID)->GetWindowRect(&rc);
	ScreenToClient(&rc);
	InvalidateRect(rc);
	UpdateWindow();  //一定要加这一句
}

LRESULT CTeteFactoryDlg::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//int ret = CDialog::OnNcHitTest(point);
	////if语句的前两行是用来禁止改变大小的，最后一行是用来禁止移动的
	//if (HTTOP == ret || HTBOTTOM == ret || HTLEFT == ret || HTRIGHT == ret || HTBOTTOMLEFT == ret || HTBOTTOMRIGHT == ret || HTTOPLEFT == ret || HTTOPRIGHT == ret || HTCAPTION == ret)
	//	return HTCLIENT;
	//return ret;

	return CDialogEx::OnNcHitTest(point);
}

void CTeteFactoryDlg::UpdateSet()
{
	if (nullptr == m_pWorkControl)
		return;

	m_pWorkControl->UpdateSet();
}

BOOL CTeteFactoryDlg::ContectTest(const char * ip, unsigned short uport)
{
	if (nullptr == m_pWorkControl)
		return FALSE;

	return m_pWorkControl->ContectTest(ip, uport);
}

BOOL CTeteFactoryDlg::StartServer(unsigned short uport)
{
	if (nullptr == m_pWorkControl)
		return FALSE;

	return m_pWorkControl->StartServer(uport);
}

BOOL CTeteFactoryDlg::StartServerPLC(unsigned short uport)
{
	if (nullptr == m_pWorkControl)
		return FALSE;

	return m_pWorkControl->StartServerPLC(uport);
}

BOOL CTeteFactoryDlg::StartServerVisionCheck(unsigned short uport)
{
	if (nullptr == m_pWorkControl)
		return FALSE;

	return m_pWorkControl->StartVisionCheckServer(uport);
}


void CTeteFactoryDlg::OnSelchangingTabMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CTeteFactoryDlg::OnBnClickedButtonHistorylog()
{
	// TODO: 在此添加控件通知处理程序代码

	CString sLotID;
	GetDlgItemText(IDC_EDIT_LOTID, sLotID);


	
	if (!m_pWorkControl->RightConfirmBox(USER_O_EDIT_PRINT_COUNT))
		return;

	if (nullptr == m_pWorkControl)
		return;

	if (m_pWorkControl->IsMarking())
		return;

	CDlgMarkTimesSet dlg(m_pWorkControl->m_nMarkCount);
	if (IDOK == dlg.DoModal())
	{
		m_pWorkControl->SetMarkCount(m_pWorkControl->m_nMarkCount);
		CString sText;
		CString sCurDate = CTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S"));
		sText.Format(_T("%s\t打印总条数设置为%d, 打印计数重置%d成功。\n"), sCurDate, CFactoryConfig::Instance()->m_nMaxMarkTimes, m_pWorkControl->m_nMarkCount);
		PrintMessage(emMsgType_Normal, sText);
	}
}


void CTeteFactoryDlg::OnBnClickedButtonLaserUsage()
{
		//CDlgLaserUsage usageDlg;
		//usageDlg.DoModal();
}


void CTeteFactoryDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	if (nHitTest == 3)  //左键单击非客户区（标题栏）的图标
	{
		return;
	}
	CDialogEx::OnNcLButtonDown(nHitTest, point);
}


void CTeteFactoryDlg::OnNcRButtonDown(UINT nHitTest, CPoint point)
{
	return;
	CDialogEx::OnNcRButtonDown(nHitTest, point);
}


int CTeteFactoryDlg::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	if (nHitTest == 7 && message == 516)  //右键单击RichEditCtrl控件的滚动条
	{
		return MA_NOACTIVATEANDEAT;  //不要激活CWnd对象并丢弃鼠标事件。
	}
	return CDialogEx::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CTeteFactoryDlg::UpdateAppCaption()
{
	SetWindowText(CString(g_sWindowText) +_T("-") + g_sUpdateTime + _T("---") + USERMNG->GetCurUserName());
	if (USERMNG->GetCurUser() != USER_A_OPER)
	{
		m_uGetLastInputInfo = SetTimer(GETLASTINPUTINFO, 1000, NULL);//设置获取键盘和鼠标处于空闲状态的时间的计时器
	}
}

void CTeteFactoryDlg::OnBnClickedButtonIp()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pWorkControl->RightConfirmBox(USER_O_BASIC_SET))
		return;
}







void CTeteFactoryDlg::OnBnClickedButtonTest()
{
	// TODO: 在此添加控件通知处理程序代码
	//CStdioFileCodePage file;
	//CString m_stripIDInfo;
	//CString sContent;
	//CString filepath = L"E:\\TK47E116.2-01.xml";
	//if (!file.OpenWithoutType(filepath, CFile::modeRead))
	//{
	//	AfxMessageBox(_T("文件打开失败"));

	//}

	//while (file.ReadStringWithOutType(sContent))
	//{
	//	 m_stripIDInfo += sContent;
	//}

	//CStringA senda(m_stripIDInfo);
	//
	//m_pWorkControl->m_pVisionCheckServer->SendRemote(senda, senda.GetLength());
	////"LaserMapping�" + StripID + "�" + Data
	
}


#include "DlgRotateCenter.h"
void CTeteFactoryDlg::OnBnClickedButtonRotateCenter()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgRotateCenter dlg;
	dlg.DoModal();
}
