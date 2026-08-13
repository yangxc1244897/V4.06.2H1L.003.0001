// DlgMultiCalib.cpp : 实现文件
//

#include "stdafx.h"
#include "TeteFactory.h"
#include "DlgMultiCalib.h"
#include "afxdialogex.h"
#include "MarkPlatform.h"
#include "CoordTransform.h"
#include "Configure.h"
uint32_t SafeCheck(INT dwValue)
{
    if (dwValue < -32768)dwValue = -32768;
    if (dwValue > 32768)dwValue = 32768;
    return (dwValue+32768);
}
// CDlgMultiCalib 对话框

IMPLEMENT_DYNAMIC(CDlgMultiCalib, CDialogEx)

CDlgMultiCalib::CDlgMultiCalib(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_MULTI_CALIB, pParent)
    , m_dwMasterSafeX(0)
    , m_dwMasterSafeY(0)
    , m_dwSlaverSafeX(0)
    , m_dwSlaverSafeY(0)
    , m_dbMasterOffsetX(0)
    , m_dbMasterOffsetY(0)
    , m_dbMasterRotateA(0)
    , m_dbSlaverOffsetX(0)
    , m_dbSlaverOffsetY(0)
    , m_dbSlaverRotateA(0)
    , m_dbOppoPitch(100)
{
    m_pThePen = CMarkPlatform::Instance()->MarkDevice()->CreatePen();
}

CDlgMultiCalib::~CDlgMultiCalib()
{
	delete m_pThePen;
}

void CDlgMultiCalib::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDCE_MASTER_SAFE_X, m_dwMasterSafeX);
    DDX_Text(pDX, IDCE_MASTER_SAFE_Y, m_dwMasterSafeY);
    DDX_Text(pDX, IDCE_SLAVER_SAFE_X, m_dwSlaverSafeX);
    DDX_Text(pDX, IDCE_SLAVER_SAFE_Y, m_dwSlaverSafeY);
    DDX_Text(pDX, IDCE_MASTER_OFFSET_X, m_dbMasterOffsetX);
    DDX_Text(pDX, IDCE_MASTER_OFFSET_Y, m_dbMasterOffsetY);
    DDX_Text(pDX, IDCE_MASTER_ROTATE_A, m_dbMasterRotateA);
    DDX_Text(pDX, IDCE_SLAVER_OFFSET_X, m_dbSlaverOffsetX);
    DDX_Text(pDX, IDCE_SLAVER_OFFSET_Y, m_dbSlaverOffsetY);
    DDX_Text(pDX, IDCE_SLAVER_ROTATE_A, m_dbSlaverRotateA);
    DDX_Text(pDX, IDCE_OPPO_PITCH, m_dbOppoPitch);
}

BEGIN_MESSAGE_MAP(CDlgMultiCalib, CDialogEx)
    ON_BN_CLICKED(IDCB_OPPO_TEST, &CDlgMultiCalib::OnBnClickedOppoTest)
    ON_BN_CLICKED(IDCB_MASTER_CALIB, &CDlgMultiCalib::OnBnClickedMasterCalib)
    ON_BN_CLICKED(IDCB_MASTER_DATA, &CDlgMultiCalib::OnBnClickedMasterData)
    ON_BN_CLICKED(IDCB_SLAVER_CALIB, &CDlgMultiCalib::OnBnClickedSlaverCalib)    
    ON_BN_CLICKED(IDCB_SLAVER_DATA, &CDlgMultiCalib::OnBnClickedSlaverData)
    ON_BN_CLICKED(IDOK, &CDlgMultiCalib::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CDlgMultiCalib::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_MASTER_CALIB_TOOL, &CDlgMultiCalib::OnBnClickedButtonMasterCalibTool)
	ON_BN_CLICKED(IDC_BUTTON_SLAVER_CALIB_TOOL, &CDlgMultiCalib::OnBnClickedButtonSlaverCalibTool)
END_MESSAGE_MAP()


// CDlgMultiCalib 消息处理程序
void CDlgMultiCalib::LockButton(BOOL IsLock)
{
    GetDlgItem(IDCB_OPPO_TEST)->EnableWindow(!IsLock);
    GetDlgItem(IDCB_MASTER_CALIB)->EnableWindow(!IsLock);
    GetDlgItem(IDCB_MASTER_DATA)->EnableWindow(!IsLock);
    GetDlgItem(IDCB_SLAVER_CALIB)->EnableWindow(!IsLock);
    GetDlgItem(IDCB_SLAVER_DATA)->EnableWindow(!IsLock);
}
BOOL CDlgMultiCalib::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
	// 直接读取配置挡
	int nPos[2];
	nPos[0] = GetPrivateProfileInt(_T("Master"), _T("SafePosX"), 0, g_sMarkDriverConfigFile);
	nPos[1] = GetPrivateProfileInt(_T("Master"), _T("SafePosY"), 0, g_sMarkDriverConfigFile);
	m_dwMasterSafeX = (int32_t)nPos[0]-32768;
	m_dwMasterSafeY = (int32_t)nPos[1]-32768;
	nPos[0] = GetPrivateProfileInt(_T("Slaver"), _T("SafePosX"), 0, g_sMarkDriverConfigFile);
	nPos[1] = GetPrivateProfileInt(_T("Slaver"), _T("SafePosY"), 0, g_sMarkDriverConfigFile);
	m_dwSlaverSafeX = (int32_t)nPos[0]-32768;
	m_dwSlaverSafeY = (int32_t)nPos[1]-32768;

    xHeadTurn_t xTurn;
	CCoordTransform::Instance()->GetHeadTurn(0, xTurn);
    m_dbMasterOffsetX = xTurn.dx;
    m_dbMasterOffsetY = xTurn.dy;
    m_dbMasterRotateA = xTurn.angle;
	s_SetEditItemValue(this, IDCE_MASTER_OFFSET_X, m_dbMasterOffsetX);
	s_SetEditItemValue(this, IDCE_MASTER_OFFSET_Y, m_dbMasterOffsetY);
	s_SetEditItemValue(this, IDCE_MASTER_ROTATE_A, m_dbMasterRotateA);

	CCoordTransform::Instance()->GetHeadTurn(1, xTurn);
    m_dbSlaverOffsetX = xTurn.dx;
    m_dbSlaverOffsetY = xTurn.dy;
    m_dbSlaverRotateA = xTurn.angle;
	s_SetEditItemValue(this, IDCE_SLAVER_OFFSET_X, m_dbSlaverOffsetX);
	s_SetEditItemValue(this, IDCE_SLAVER_OFFSET_Y, m_dbSlaverOffsetY);
	s_SetEditItemValue(this, IDCE_SLAVER_ROTATE_A, m_dbSlaverRotateA);

	// 中心距离读取
	CConfigure * pConfig = CConfigure::Instance();
	double dbLimit1 = pConfig->GetDoubleValue("MasterHead", "cx", -50);
	double dbLimit2 = pConfig->GetDoubleValue("SlaverHead", "cx", 50);
	m_dbOppoPitch = fabs(dbLimit1) + fabs(dbLimit2);

    UpdateData(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}

void CDlgMultiCalib::OnBnClickedOppoTest()
{
    // TODO: 在此添加控件通知处理程序代码
    LockButton(TRUE);
    UpdateData(TRUE);
    double dbLimit = m_dbOppoPitch*0.5;
    IMarkEngine *pTheEngine = CMarkPlatform::Instance()->MarkEngine();   

    CMarkPlatform::Instance()->MarkDriver()->SetScanHead(1, 1);
    pTheEngine->ActiveOutputSign(emSIGN_OUT_REDLIGHT,false);
    pTheEngine->MarkBegin();
    pTheEngine->UpdatePen(m_pThePen);

    MarkCross(pTheEngine,dbLimit,dbLimit);
    pTheEngine->MarkRectangle(-dbLimit,dbLimit,dbLimit,-dbLimit);
    pTheEngine->MarkEnd();

    xCoord_t xCoord;    
    CCoordTransform::Instance()->GetHeadCoord(0, xCoord);
    xCoord.cx = -dbLimit;
    xCoord.cy = 0.0;
    CCoordTransform::Instance()->SetHeadCoord(0, xCoord);

    CCoordTransform::Instance()->GetHeadCoord(1, xCoord);
    xCoord.cx = dbLimit;
    xCoord.cy = 0.0;
    CCoordTransform::Instance()->SetHeadCoord(1, xCoord);
    CConfigure::Instance()->SetDataValue("MasterHead","cx",-dbLimit);
    CConfigure::Instance()->SetDataValue("MasterHead","cy",0.0);

    CConfigure::Instance()->SetDataValue("SlaverHead","cx",dbLimit);
    CConfigure::Instance()->SetDataValue("SlaverHead","cy",0.0);

    UpdateData(FALSE);
    LockButton(FALSE);
}

void CDlgMultiCalib::OnBnClickedMasterCalib()
{
    // TODO: 在此添加控件通知处理程序代码
    LockButton(TRUE);
    //--主头网格、从头中十字线
    double dbLimitX, dbLimitY;
    uint32_t nGridX, nGridY;
    CMarkPlatform::Instance()->MasterCalib()->GetCalibCtrl(MC_CALIB_LIMIT_X, &dbLimitX);
    CMarkPlatform::Instance()->MasterCalib()->GetCalibCtrl(MC_CALIB_LIMIT_Y, &dbLimitY);
    CMarkPlatform::Instance()->MasterCalib()->GetCalibCtrl(MC_CALIB_GRID_X, &nGridX);
    CMarkPlatform::Instance()->MasterCalib()->GetCalibCtrl(MC_CALIB_GRID_Y, &nGridY);

    IMarkEngine *pTheEngine = CMarkPlatform::Instance()->MarkEngine();   
	pTheEngine->ActiveOutputSign(emSIGN_OUT_REDLIGHT, false);

	CMarkPlatform::Instance()->MarkDriver()->SetScanHead(0, 1);
	pTheEngine->MarkBegin();
	pTheEngine->UpdatePen(m_pThePen);
	MarkCross(pTheEngine, 10, 10);
	pTheEngine->MarkEnd();

    CMarkPlatform::Instance()->MarkDriver()->SetScanHead(1, 0);
    pTheEngine->MarkBegin();
    pTheEngine->UpdatePen(m_pThePen);
    MarkGrid(pTheEngine,dbLimitX,dbLimitY,nGridX,nGridY);
    pTheEngine->MarkEnd();


    pTheEngine->ActiveOutputSign(emSIGN_OUT_REDLIGHT,true);
    LockButton(FALSE);
}
void CDlgMultiCalib::OnBnClickedMasterData()
{
    // TODO: 在此添加控件通知处理程序代码
    LockButton(TRUE);
    CString sFileExtName = L"手动测量数据(*.txt)|*.txt|自动测量数据(*.ini)|*.ini||"; 
    CFileDialog fd(true, NULL, NULL, OFN_HIDEREADONLY, sFileExtName);
    if (fd.DoModal() == IDOK)
    {
        CString sFileName = fd.GetPathName();
        CStringA sFileNameA;
        sFileNameA = sFileName;
        if(CMarkPlatform::Instance()->MasterCalib()->LoadGridFile(sFileNameA)<0)
        {
            MessageBox(L"数据载入失败,请检查测量数据点是否与当前网格数设置一致！");
			LockButton(FALSE);
            return;
        }
		MessageBox(L"网格数据载入成功！");
    }
    LockButton(FALSE);
}
void CDlgMultiCalib::OnBnClickedSlaverCalib()
{
    // TODO: 在此添加控件通知处理程序代码
    LockButton(TRUE);
    double dbLimitX, dbLimitY;
    uint32_t nGridX, nGridY;
    CMarkPlatform::Instance()->SlaverCalib()->GetCalibCtrl(MC_CALIB_LIMIT_X, &dbLimitX);
    CMarkPlatform::Instance()->SlaverCalib()->GetCalibCtrl(MC_CALIB_LIMIT_Y, &dbLimitY);
    CMarkPlatform::Instance()->SlaverCalib()->GetCalibCtrl(MC_CALIB_GRID_X, &nGridX);
    CMarkPlatform::Instance()->SlaverCalib()->GetCalibCtrl(MC_CALIB_GRID_Y, &nGridY);

    IMarkEngine *pTheEngine = CMarkPlatform::Instance()->MarkEngine(); 
    pTheEngine->ActiveOutputSign(emSIGN_OUT_REDLIGHT,false);

    CMarkPlatform::Instance()->MarkDriver()->SetScanHead(1, 0);
    pTheEngine->MarkBegin();
    pTheEngine->UpdatePen(m_pThePen);
    MarkCross(pTheEngine,10,10);
    pTheEngine->MarkEnd();

    CMarkPlatform::Instance()->MarkDriver()->SetScanHead(0, 1);
    pTheEngine->MarkBegin();
    pTheEngine->UpdatePen(m_pThePen);
    MarkGrid(pTheEngine,dbLimitX,dbLimitY,nGridX,nGridY);
    pTheEngine->MarkEnd();


    pTheEngine->ActiveOutputSign(emSIGN_OUT_REDLIGHT,true);
    LockButton(FALSE);
}
void CDlgMultiCalib::OnBnClickedSlaverData()
{
    // TODO: 在此添加控件通知处理程序代码
    LockButton(TRUE);
    CString sFileExtName = L"手动测量数据(*.txt)|*.txt|自动测量数据(*.ini)|*.ini||";
    CFileDialog fd(true, NULL, NULL, OFN_HIDEREADONLY, sFileExtName);
    if (fd.DoModal() == IDOK)
    {
        CString sFileName = fd.GetPathName();
        CStringA sFileNameA;
        sFileNameA = sFileName;
        if (CMarkPlatform::Instance()->SlaverCalib()->LoadGridFile(sFileNameA) < 0)
        {
            MessageBox(L"数据载入失败,请检查测量数据点是否与当前网格数设置一致！");
			LockButton(FALSE);
            return;
        }
		MessageBox(L"网格数据载入成功！");
    }
    LockButton(FALSE);
}
void CDlgMultiCalib::MarkGrid(IMarkEngine *pTheEngine, double dbLimitX, double dbLimitY,uint32_t nGridX,uint32_t nGridY)
{
    double dbPitchX = dbLimitX /(nGridX/2);
    double dbPitchY = dbLimitY /(nGridY/2);
    double mx(0.0), my(0.0);
    double minX(-dbLimitX), maxX(dbLimitX);
    double minY(-dbLimitY), maxY(dbLimitY);
    //--X轴线来回标记
    for (UINT y = 0; y<nGridX; y++)
    {
        if ((y & 0x01) == 0x00) //--向右
        {
            mx = minX - 2;    my = maxY - dbPitchY*y;
            pTheEngine->MoveTo(mx, my);
            mx = maxX + 2;
            pTheEngine->LineTo(mx, my);
        }
        else //--向左
        {
            mx = maxX + 2;   my = maxY - dbPitchY*y;
            pTheEngine->MoveTo(mx, my);
            mx = minX - 2; 
            pTheEngine->LineTo(mx, my);
        }     
    }
    pTheEngine->MoveTo(0.0,0.0);
    //--Y轴线来回标记
    for (UINT x = 0; x<nGridY; x++)
    {
        if ((x & 0x01) == 0x00) //--向下
        {
            mx = minX + dbPitchX*x;   my = maxY + 2;
            pTheEngine->MoveTo(mx, my);
            mx; my = minY - 2;
            pTheEngine->LineTo(mx, my);
        }
        else //--向上
        {
            mx = minX + dbPitchX*x;  my = minY - 2;
            pTheEngine->MoveTo(mx, my);
            my = maxY + 2;
            pTheEngine->LineTo(mx, my);
        }  
    }

	MarkX(pTheEngine, maxX + 6.0, 0.0);
	MarkY(pTheEngine, 0.0, maxY + 6.0);
	
	double dbR = min(dbPitchX, dbPitchY) * 0.5;
	pTheEngine->MarkCircle(0, 0, dbR);

    while (emMARK_STAT_BUSY == pTheEngine->GetMarkStat());
    pTheEngine->ActiveOutputSign(emSIGN_OUT_REDLIGHT,true);
}
void CDlgMultiCalib::MarkCross(IMarkEngine *pTheEngine, double dbLimitX, double dbLimitY)
{
    pTheEngine->MarkLine(dbLimitX,0.0,-dbLimitX,0.0);
    pTheEngine->MarkLine(0.0,dbLimitY,0.0,-dbLimitY);
	pTheEngine->MoveTo(0, 0);
}
BOOL CDlgMultiCalib::PreTranslateMessage(MSG* pMsg)
{
    // TODO: 在此添加专用代码和/或调用基类
    if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)  
    {
        int iID = GetFocus()->GetDlgCtrlID();
        /*if (iID == IDCE_AXIS_SIZE_X)
        {
            UpdateData(TRUE);
            m_pTheCalib->m_dbMarkLimitX = m_dbLimitSizeX*0.5;
            m_pTheCalib->UpdateScale();
            m_pWndCalibView->Invalidate(FALSE);
        }*/        

        return TRUE;
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}
void CDlgMultiCalib::OnBnClickedOk()
{
    // TODO: 在此添加控件通知处理程序代码
	OnBnClickedButtonApply();
    CDialogEx::OnOK();
}

void CDlgMultiCalib::OnBnClickedButtonApply()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	// 主头安全位置保存
	uint32_t nPos[2];
	nPos[0] = SafeCheck(m_dwMasterSafeX);
	nPos[1] = SafeCheck(m_dwMasterSafeY);
	CString s;
	s.Format(_T("%d"), nPos[0]);
	WritePrivateProfileString(_T("Master"), _T("SafePosX"), s, g_sMarkDriverConfigFile);
	s.Format(_T("%d"), nPos[1]);
	WritePrivateProfileString(_T("Master"), _T("SafePosY"), s, g_sMarkDriverConfigFile);
	CMarkPlatform::Instance()->MarkDriver()->SetDriverCtrl(CMD_MASTER_SAFE, nPos);
	// 从头安全位置保存
	nPos[0] = SafeCheck(m_dwSlaverSafeX);
	nPos[1] = SafeCheck(m_dwSlaverSafeY);
	s.Format(_T("%d"), nPos[0]);
	WritePrivateProfileString(_T("Slaver"), _T("SafePosX"), s, g_sMarkDriverConfigFile);
	s.Format(_T("%d"), nPos[1]);
	WritePrivateProfileString(_T("Slaver"), _T("SafePosY"), s, g_sMarkDriverConfigFile);
	CMarkPlatform::Instance()->MarkDriver()->SetDriverCtrl(CMD_SLAVER_SAFE, nPos);

	// 中心距离保存
	double dbLimit = m_dbOppoPitch*0.5;
	CConfigure::Instance()->SetDataValue("MasterHead", "cx", -dbLimit);
	CConfigure::Instance()->SetDataValue("MasterHead", "cy", 0.0);
	CConfigure::Instance()->SetDataValue("SlaverHead", "cx", dbLimit);
	CConfigure::Instance()->SetDataValue("SlaverHead", "cy", 0.0);
	// 设置中心距离
	xCoord_t xCoord;
	CCoordTransform::Instance()->GetHeadCoord(0, xCoord);
	xCoord.cx = -dbLimit;
	xCoord.cy = 0.0;
	CCoordTransform::Instance()->SetHeadCoord(0, xCoord);

	CCoordTransform::Instance()->GetHeadCoord(1, xCoord);
	xCoord.cx = dbLimit;
	xCoord.cy = 0.0;
	CCoordTransform::Instance()->SetHeadCoord(1, xCoord);

	// 设置偏移值
	m_dbMasterOffsetX = s_GetEditItemValue(this, IDCE_MASTER_OFFSET_X);
	m_dbMasterOffsetY = s_GetEditItemValue(this, IDCE_MASTER_OFFSET_Y);
	m_dbMasterRotateA = s_GetEditItemValue(this, IDCE_MASTER_ROTATE_A);
	xHeadTurn_t xTurn;
	CConfigure::Instance()->SetDataValue("MasterHead", "offset_cx", m_dbMasterOffsetX);
	CConfigure::Instance()->SetDataValue("MasterHead", "offset_cy", m_dbMasterOffsetY);
	CConfigure::Instance()->SetDataValue("MasterHead", "offset_a", m_dbMasterRotateA);
	xTurn.dx = m_dbMasterOffsetX;
	xTurn.dy = m_dbMasterOffsetY;
	xTurn.angle = m_dbMasterRotateA;
	CCoordTransform::Instance()->SetHeadTurn(0, xTurn);
	m_dbSlaverOffsetX = s_GetEditItemValue(this, IDCE_SLAVER_OFFSET_X);
	m_dbSlaverOffsetY = s_GetEditItemValue(this, IDCE_SLAVER_OFFSET_Y);
	m_dbSlaverRotateA = s_GetEditItemValue(this, IDCE_SLAVER_ROTATE_A);
	CConfigure::Instance()->SetDataValue("SlaverHead", "offset_cx", m_dbSlaverOffsetX);
	CConfigure::Instance()->SetDataValue("SlaverHead", "offset_cy", m_dbSlaverOffsetY);
	CConfigure::Instance()->SetDataValue("SlaverHead", "offset_a", m_dbSlaverRotateA);
	xTurn.dx = m_dbSlaverOffsetX;
	xTurn.dy = m_dbSlaverOffsetY;
	xTurn.angle = m_dbSlaverRotateA;
	CCoordTransform::Instance()->SetHeadTurn(1, xTurn);

	// 校正偏移
	CMarkPlatform::Instance()->MasterCalib()->SetTransform(0, m_dbMasterOffsetX, m_dbMasterOffsetY, DEG2RAD(m_dbMasterRotateA));
	CMarkPlatform::Instance()->SlaverCalib()->SetTransform(0, m_dbSlaverOffsetX, m_dbSlaverOffsetY, DEG2RAD(m_dbSlaverRotateA));
}

void CDlgMultiCalib::MarkX(IMarkEngine *pTheEngine, double x, double y)
{
	pTheEngine->MarkLine(-1.5 + x, 2.0 + y, 1.5 + x, -2.0 + y);
	pTheEngine->MarkLine(1.5 + x, 2.0 + y, -1.5 + x, -2.0 + y);
}

void CDlgMultiCalib::MarkY(IMarkEngine *pTheEngine, double x, double y)
{
	pTheEngine->MarkLine(-1.0 + x, 1.5 + y, x, y);
	pTheEngine->MarkLine(1.0 + x, 1.5 + y, x, y);
	pTheEngine->MarkLine(x, -2.0 + y, x, y);
}

void CDlgMultiCalib::OnBnClickedButtonMasterCalibTool()
{
	// TODO: 在此添加控件通知处理程序代码
	CMarkPlatform::Instance()->MasterCalib()->ShowWnd();

}


void CDlgMultiCalib::OnBnClickedButtonSlaverCalibTool()
{
	// TODO: 在此添加控件通知处理程序代码
	CMarkPlatform::Instance()->SlaverCalib()->ShowWnd();
}
