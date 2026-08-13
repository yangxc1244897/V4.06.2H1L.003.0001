// DlgStartup.cpp : 实现文件
//

#include "stdafx.h"
#include "TeteFactory.h"
#include "DlgStartup.h"
#include "afxdialogex.h"
#include "MarkPlatform.h"
#include "Configure.h"
#include "Common\LogServer.h"
#include "DataProfile.h"
#include "ActionFactory.h"
#include "CoordTransform.h"
// CDlgStartup 对话框

IMPLEMENT_DYNAMIC(CDlgStartup, CDialogEx)

CDlgStartup::CDlgStartup(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_STARTUP, pParent)
{
    m_nIDEvent = 1;
}

CDlgStartup::~CDlgStartup()
{
    
}

void CDlgStartup::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_SYSINFO_LIST, m_ctlSysInfoList);
    DDX_Control(pDX, IDC_PROGRESS1, m_ctlProgressCtrl);
    DDX_Control(pDX, IDC_STATIC_SYSSTAT, m_ctlSysStat);
    DDX_Control(pDX, IDC_STATIC_SYSOFF, m_ctlSysOff);
    DDX_Control(pDX, IDC_STATIC_LASEROFF, m_ctlLaserOff);
    DDX_Control(pDX, IDC_STATIC_SHUTTEROFF, m_ctlShutterOff);
}


BEGIN_MESSAGE_MAP(CDlgStartup, CDialogEx)
    ON_BN_CLICKED(IDCANCEL, &CDlgStartup::OnBnClickedCancel)
    ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgStartup 消息处理程序
void CDlgStartup::OnBnClickedCancel()
{
    // TODO: 在此添加控件通知处理程序代码
    KillTimer(m_nIDEvent);
    
    CDialogEx::OnCancel();
}
BOOL CDlgStartup::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    m_ctlSysInfoList.AddString(L"打开初始化……");
    IsStarted = false;
    m_ctlProgressCtrl.SetRange(0, 100);
    m_ctlProgressCtrl.SetPos(0);
    
	SetTimer(m_nIDEvent, 1000, NULL);  

    m_ctlSysOff.OnSetLedInfo(L"电源关闭", emLED_OFF);
    m_ctlSysOff.OnSetLedInfo(L"电源关闭", emLED_OFF);
    m_ctlLaserOff.OnSetLedInfo(L"激光关闭", emLED_OFF);
    m_ctlShutterOff.OnSetLedInfo(L"Shutter关闭", emLED_OFF);
    m_ctlSysStat.Invalidate(false);
    m_ctlSysOff.Invalidate(false);
    m_ctlLaserOff.Invalidate(false);
    m_ctlShutterOff.Invalidate(false);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}

void CDlgStartup::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    int iPos = m_ctlProgressCtrl.GetPos();
    iPos = (iPos + 1) % 100;
    m_ctlProgressCtrl.SetPos(iPos);  

    if (!IsStarted)
    {
		IsStarted = true;
        SystemInit();  
    }
    UpdateStartupInfo();
    

    CDialogEx::OnTimer(nIDEvent);
}
void CDlgStartup::UpdateStartupInfo(void)
{
    IMarkEngine *pTheEngine = CMarkPlatform::Instance()->MarkEngine();
    if (NULL==pTheEngine)
        return;
    if (!pTheEngine->IsActive())
        return;
    uint32_t iStat = pTheEngine->GetStatus();

    if(SYS_STAT_OK!=iStat)
    {
    if(SYS_STAT_SYS_WARN&iStat)
    m_ctlSysStat.OnSetLedInfo(L"系统告警", emLED_RED);
    else if(SYS_STAT_LINE_WARN&iStat)
    m_ctlSysStat.OnSetLedInfo(L"流水线告警", emLED_RED);
    else if(SYS_STAT_TEMP_WARN&iStat)
    m_ctlSysStat.OnSetLedInfo(L"温度告警", emLED_RED);
    else if(SYS_STAT_ELEC_WARN&iStat)
    m_ctlSysStat.OnSetLedInfo(L"激光器电路告警", emLED_RED);
    else if(SYS_STAT_WATER_WARN&iStat)
    m_ctlSysStat.OnSetLedInfo(L"水冷机告警", emLED_RED);
    else if(SYS_STAT_LASER_WARN&iStat)
    m_ctlSysStat.OnSetLedInfo(L"激光器告警", emLED_RED);
    }
    else
    m_ctlSysStat.OnSetLedInfo(L"系统正常", emLED_GREEN);

    if (pTheEngine->IsSignInActive(emSIGN_IN_HARDWORK))
    m_ctlSysOff.OnSetLedInfo(L"电源正常", emLED_GREEN);
    else
    m_ctlSysOff.OnSetLedInfo(L"电源关闭", emLED_OFF);

    if (pTheEngine->IsSignInActive(emSIGN_IN_LASERLIGNT))
    m_ctlLaserOff.OnSetLedInfo(L"激光正常", emLED_GREEN);
    else
    m_ctlLaserOff.OnSetLedInfo(L"激光关闭", emLED_OFF);

    if (pTheEngine->IsSignInActive(emSIGN_IN_SHUTTER))
    m_ctlShutterOff.OnSetLedInfo(L"Shutter正常", emLED_GREEN);
    else
    m_ctlShutterOff.OnSetLedInfo(L"Shutter关闭", emLED_OFF);

    m_ctlSysStat.Invalidate(false);
    m_ctlSysOff.Invalidate(false);
    m_ctlLaserOff.Invalidate(false);
    m_ctlShutterOff.Invalidate(false);
}
void CDlgStartup::SystemInit(void)
{
    CMarkPlatform *pThePlatform = CMarkPlatform::Instance();
    pThePlatform->Open();
    
	//--校正
	IMarkCalib *pMasterCalib = pThePlatform->MasterCalib();
	if (NULL == pMasterCalib)
		m_ctlSysInfoList.AddString(L"主头校正系统加载失败");
	else
		m_ctlSysInfoList.AddString(L"主头校正系统加载成功");
	CStringA sFile, sMainExeDir = GetSystemDir();
	sFile = CConfigure::Instance()->GetStringValue("CALIB", "MasterFile", "");
	sFile = sMainExeDir + "Module\\master.tlcf";
	//if (PathFileExistsA(sFile))
	{
		if (pMasterCalib->LoadCalibFile(sFile) < 0)
		{
			m_ctlSysInfoList.AddString(L"主头校正文件加载出错！");
			pThePlatform->Delay(0, 0, 20);
		}
	}
	/* else
	{
	m_ctlSysInfoList.AddString(L"主扫描头校正文件不存在！");
	pThePlatform->Delay(0,0,20);
	}  */
	IMarkCalib *pSlaverCalib = pThePlatform->SlaverCalib();
	if (NULL == pSlaverCalib)
		m_ctlSysInfoList.AddString(L"从头校正系统加载失败");
	else
		m_ctlSysInfoList.AddString(L"从头校正系统加载成功");
	sFile = CConfigure::Instance()->GetStringValue("CALIB", "SlaverFile", "");
	sFile = sMainExeDir + "Module\\slaver.tlcf";
	// if (PathFileExistsA(sFile))
	{
		if (pSlaverCalib->LoadCalibFile(sFile) < 0)
		{
			m_ctlSysInfoList.AddString(L"从扫描头校正文件加载出错！");
			pThePlatform->Delay(0, 0, 20);
		}
	}
	//else
	//{
	//    m_ctlSysInfoList.AddString(L"从扫描头校正文件不存在！");
	//    pThePlatform->Delay(0,0,20);
	//}
    
    //--板卡
    int iDrvType = 0;
    IMarkDriver *pMarkDriver = pThePlatform->MarkDriver();
    if (NULL == pMarkDriver)
    {
        m_ctlSysInfoList.AddString(L"标记控制卡加载失败");
        pThePlatform->Delay(0,0,20);
    }
    else
    {
        CStringA sNameA;        
        sNameA = pMarkDriver->GetName();
        CString sName;
        sName = sNameA;
        m_ctlSysInfoList.AddString(L"标记控制卡"+sName+L"加载成功");
        pThePlatform->Delay(0,0,20);
    }
    pMarkDriver->LoadConfig(pThePlatform->GetSystemPath("config\\markDriver.ini"));
    if (0 != pMarkDriver->Open(pThePlatform))
    {
        pMarkDriver->Close();
        m_ctlSysInfoList.AddString(L"标记控制卡打开失败！");
        pThePlatform->Delay(0,0,20);
    }
    else
        pThePlatform->Delay(0,0,20);

	// 必须在板卡打开之后，激光器打开之前设置
	int iGateLevel = CConfigure::Instance()->GetBoolValue("MarkDriver", "GateLevel", 0);
	pMarkDriver->SetDriverCtrl(CMD_GATE_LEVEL, &iGateLevel);
	pMarkDriver->SetDriverCtrl(CMD_HEAD_TYPE, &CConfigure::Instance()->m_iScanerType);

    //--激光器
    IMarkDevice *pMarkDevice = pThePlatform->MarkDevice();
    if (NULL == pMarkDevice)
    {
        m_ctlSysInfoList.AddString(L"激光器驱动加载失败");
        pThePlatform->Delay(0,0,20);
    }
    else
    {
        m_ctlSysInfoList.AddString(L"激光器驱动加载成功");
        pThePlatform->Delay(0,0,20);
    }
    
    char strName[512];
    CStringA sNameA;
    pMarkDevice->GetName(strName);
    sNameA = strName;
    CString sName;
    sName = sNameA;
	
	if (!sName.IsEmpty()) {  // lrf 将当前激光器可使用时间写入配置文件
		CString laserTime;
		GetPrivateProfileString(sName, _T("time"), _T("20000"), laserTime.GetBuffer(100), 100, _T("config\\LaserTime.ini"));
		laserTime.ReleaseBuffer();
		WritePrivateProfileString(_T("LASERUSAGE"), _T("LaserTime"), laserTime, _T("config\\LaserUsage.ini"));
	}

    m_ctlSysInfoList.AddString(L"正在打开"+sName+L"激光器......");
    if (0 != pMarkDevice->Open(pThePlatform))
    {
        //pMarkDevice->Close();
        m_ctlSysInfoList.AddString(L"激光器打开失败！");
		AfxMessageBox(_T("激光器打开失败！"));
        pThePlatform->Delay(0,0,20);
    }
    else
    {             
        m_ctlSysInfoList.AddString(sName + L"激光器打开成功");
        pThePlatform->Delay(0,0,20);
    }
    //--引擎
    IMarkEngine *pMarkEngine = pThePlatform->MarkEngine();
    if(0!=pMarkEngine->LoadConfig(pThePlatform->GetSystemPath("config\\mark.xml")))
        m_ctlSysInfoList.AddString(L"载入标记引擎配置失败");
    if (0 != pMarkEngine->Open(pThePlatform))
    {
        m_ctlSysInfoList.AddString(L"标记引擎打开失败！");
        pThePlatform->Delay(0,0,20);
    } 
    else
    {
        m_ctlSysInfoList.AddString(L"标记引擎打开成功！");
        pThePlatform->Delay(0,0,20);
    }
    IObjectEngine *pGraphEngine = pThePlatform->ObjectEngine();
    if (0 != pGraphEngine->Open(pThePlatform))
    {
        m_ctlSysInfoList.AddString(L"图形引擎打开失败！");
        pThePlatform->Delay(0,0,20);
    } 
    else
    {
        m_ctlSysInfoList.AddString(L"图形引擎打开成功！");
        pThePlatform->Delay(0,0,20);
    }
    if(NULL!=pMasterCalib)
        pMasterCalib->Open(pThePlatform);
    if(NULL!=pSlaverCalib)
        pSlaverCalib->Open(pThePlatform);

    if (CConfigure::Instance()->m_IsMotionEnable)
    {
        if (0 == pThePlatform->MotionDriver()->Open())
            m_ctlSysInfoList.AddString(L"运动控制模块打开成功！");
        else
            m_ctlSysInfoList.AddString(L"运动控制模块打开失败！");
    }

	// 加载视觉DLL
	/*IVisionDriver * pVisionDriver = pThePlatform->VisionDriver();
	if (NULL == pVisionDriver)
	{
		pThePlatform->PrintLog(emLOG_LEVEL_ERROR, "缺少tlVisionDriver.dll文件！");
		m_ctlSysInfoList.AddString(_T("缺少tlVisionDriver.dll文件！"));
		AfxMessageBox(_T("缺少tlVisionDriver.dll文件！"));
	}
	else 
	{
		if (CConfigure::Instance()->m_IsVisionEnable)
		{
			if (0 == pVisionDriver->Open(pThePlatform))
				m_ctlSysInfoList.AddString(L"CCD视觉模块打开成功！");
			else
			{
				m_ctlSysInfoList.AddString(L"CCD视觉模块打开失败！");
				AfxMessageBox(_T("CCD视觉模块打开失败！"));
				pThePlatform->Delay(0, 0, 20);
			}
		}
	}*/

    pThePlatform->Delay(0,0,50);
    for (int i = 0; i < 10; i++)
    {
        if (SYS_STAT_OK == pMarkEngine->GetStatus())
            break;
        pThePlatform->Delay(0,0,100);
    }
    CActionFactory::Instance()->Open(pThePlatform);

    double dbLimitX, dbLimitY;
    pMasterCalib->GetCalibCtrl(MC_CALIB_LIMIT_X, &dbLimitX);
    pMasterCalib->GetCalibCtrl(MC_CALIB_LIMIT_Y, &dbLimitY);
    xCoord_t xCoord;    
    CCoordTransform::Instance()->GetHeadCoord(0, xCoord);
    xCoord.cx = CConfigure::Instance()->GetDoubleValue("MasterHead","cx",0.0);
    xCoord.cy = CConfigure::Instance()->GetDoubleValue("MasterHead","cy",0.0);
    xCoord.rx = dbLimitX;
    xCoord.ry = dbLimitY;
    CCoordTransform::Instance()->SetHeadCoord(0, xCoord);

    pSlaverCalib->GetCalibCtrl(MC_CALIB_LIMIT_X, &dbLimitX);
    pSlaverCalib->GetCalibCtrl(MC_CALIB_LIMIT_Y, &dbLimitY);
    CCoordTransform::Instance()->GetHeadCoord(1, xCoord);
    xCoord.cx = CConfigure::Instance()->GetDoubleValue("SlaverHead","cx",0.0);
    xCoord.cy = CConfigure::Instance()->GetDoubleValue("SlaverHead","cy",0.0);
    xCoord.rx = dbLimitX;
    xCoord.ry = dbLimitY;
    CCoordTransform::Instance()->SetHeadCoord(1, xCoord);

	xHeadTurn_t xTurn;
	xTurn.dx = CConfigure::Instance()->GetDoubleValue("MasterHead", "offset_cx", 0.0);
	xTurn.dy = CConfigure::Instance()->GetDoubleValue("MasterHead", "offset_cy", 0.0);
	xTurn.angle = CConfigure::Instance()->GetDoubleValue("MasterHead", "offset_a", 0.0);
	CCoordTransform::Instance()->SetHeadTurn(0, xTurn);  // 采用下面的校正偏移，不采用此处坐标变换
	CMarkPlatform::Instance()->MasterCalib()->SetTransform(0, xTurn.dx, xTurn.dy, DEG2RAD(xTurn.angle));  
	xTurn.dx = CConfigure::Instance()->GetDoubleValue("SlaverHead", "offset_cx", 0.0);
	xTurn.dy = CConfigure::Instance()->GetDoubleValue("SlaverHead", "offset_cy", 0.0);
	xTurn.angle = CConfigure::Instance()->GetDoubleValue("SlaverHead", "offset_a", 0.0);
	CCoordTransform::Instance()->SetHeadTurn(1, xTurn);// 采用下面的校正偏移，不采用此处坐标变换
	CMarkPlatform::Instance()->SlaverCalib()->SetTransform(0, xTurn.dx, xTurn.dy, DEG2RAD(xTurn.angle));

	// 安全位置
	int nPos[2];
	nPos[0] = GetPrivateProfileInt(_T("Master"), _T("SafePosX"), 0, g_sMarkDriverConfigFile);
	nPos[1] = GetPrivateProfileInt(_T("Master"), _T("SafePosY"), 0, g_sMarkDriverConfigFile);
	pMarkDriver->SetDriverCtrl(CMD_MASTER_SAFE, nPos);
	nPos[0] = GetPrivateProfileInt(_T("Slaver"), _T("SafePosX"), 0, g_sMarkDriverConfigFile);
	nPos[1] = GetPrivateProfileInt(_T("Slaver"), _T("SafePosY"), 0, g_sMarkDriverConfigFile);
	pMarkDriver->SetDriverCtrl(CMD_SLAVER_SAFE, nPos);

    KillTimer(m_nIDEvent);

	

    CDialog::OnOK();
}
