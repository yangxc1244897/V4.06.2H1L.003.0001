
// TeteFactory.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "TeteFactory.h"
#include "TeteFactoryDlg.h"
#include "DlgStartup.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define APPINSTANCENAME _T("TeteFactory")
// CTeteFactoryApp

BEGIN_MESSAGE_MAP(CTeteFactoryApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTeteFactoryApp 构造

CTeteFactoryApp::CTeteFactoryApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CTeteFactoryApp 对象

CTeteFactoryApp theApp;


// CTeteFactoryApp 初始化

BOOL CTeteFactoryApp::InitInstance()
{
	//软件防呆
	HANDLE   hOnlyoneMutex = CreateMutex(NULL, TRUE, _T("TeteLaser IC Mark Mutex"));
	if (hOnlyoneMutex == NULL)
	{
		return   FALSE;
	}
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		////软件已经打开，显示该软件
		//HWND hWnd = ::FindWindow(APPINSTANCENAME, NULL);
		//if (hWnd != NULL)
		//{
		//	::ShowWindow(hWnd, SW_MAXIMIZE);
		//	return   FALSE;
		//}
		AfxMessageBox(_T("软件已经运行或调试软件已经开启！请关闭调试软件"));
		CloseHandle(hOnlyoneMutex);
		hOnlyoneMutex = NULL;
		return FALSE;
	}

	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxInitRichEdit();

	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("TeteLaserV4.06.2H1L.Factory"));
	// Just get default class for the dialogs
	
	CDlgStartup StartUpdlg(NULL);
	StartUpdlg.DoModal();

	WNDCLASS wndcls;
	::GetClassInfo(AfxGetInstanceHandle(), MAKEINTRESOURCE(32770), &wndcls);

	// Set our own class name
	wndcls.lpszClassName = APPINSTANCENAME;

	// Just register the class
	if (!AfxRegisterClass(&wndcls))
	{
		_ASSERTE(!__FUNCTION__ " Failed to register window class");
		return FALSE;
	}

	CTeteFactoryDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

