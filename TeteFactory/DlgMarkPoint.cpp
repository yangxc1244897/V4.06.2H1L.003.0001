// DlgMarkPoint.cpp : 实现文件
//

#include "stdafx.h"
#include "TeteFactory.h"
#include "DlgMarkPoint.h"
#include "afxdialogex.h"
#include "MarkPlatform.h"

#include "CoordTransform.h"

// CDlgMarkPoint 对话框

IMPLEMENT_DYNAMIC(CDlgMarkPoint, CDialogEx)

CDlgMarkPoint::CDlgMarkPoint(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_MARK_POINT, pParent)
    , m_dbPosX(0)
    , m_dbPosY(0)
	, m_dbPosX2(0)
	, m_dbPosY2(0)
{
    m_pThePen = CMarkPlatform::Instance()->MarkDevice()->CreatePen();
}

CDlgMarkPoint::~CDlgMarkPoint()
{
    delete m_pThePen;
    CMarkPlatform::Instance()->MarkEngine()->LaserOff();
}

void CDlgMarkPoint::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDCE_POS_X, m_dbPosX);
	DDX_Text(pDX, IDCE_POS_Y, m_dbPosY);
	DDX_Text(pDX, IDCE_POS_X2, m_dbPosX2);
	DDX_Text(pDX, IDCE_POS_Y2, m_dbPosY2);
}


BEGIN_MESSAGE_MAP(CDlgMarkPoint, CDialogEx)
    ON_BN_CLICKED(IDCB_OPTION_PEN, &CDlgMarkPoint::OnBnClickedOptionPen)
    ON_BN_CLICKED(IDCB_LASER_ON, &CDlgMarkPoint::OnBnClickedLaserOn)
    ON_BN_CLICKED(IDCB_LASER_OFF, &CDlgMarkPoint::OnBnClickedLaserOff)
	ON_BN_CLICKED(IDCB_LASER_ON2, &CDlgMarkPoint::OnBnClickedLaserOn2)
	ON_BN_CLICKED(IDOK, &CDlgMarkPoint::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgMarkPoint 消息处理程序
BOOL CDlgMarkPoint::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化

    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}
void CDlgMarkPoint::OnBnClickedOptionPen()
{
    // TODO: 在此添加控件通知处理程序代码
	int nPenOptionWndFlag = PenOptionWndFlag_EDIT;
	CMarkPlatform::Instance()->MarkDevice()->SetDeviceCtrl(Laser_SetPenOptionWndFlag_I, (void*)&nPenOptionWndFlag);
    CMarkPlatform::Instance()->MarkDevice()->ShowPenOptionWnd(m_pThePen);
}
void CDlgMarkPoint::OnBnClickedLaserOn()
{
    // TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDCB_LASER_ON)->EnableWindow(FALSE);
	GetDlgItem(IDCB_LASER_ON2)->EnableWindow(FALSE);
	UpdateData(TRUE);
	
	MarkPoint(0, m_dbPosX, m_dbPosY);
}
void CDlgMarkPoint::OnBnClickedLaserOff()
{
    // TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDCB_LASER_ON)->EnableWindow(TRUE);
	GetDlgItem(IDCB_LASER_ON2)->EnableWindow(TRUE);
	CMarkPlatform::Instance()->MarkEngine()->ActiveOutputSign(emSIGN_OUT_REDLIGHT, true);
	CMarkPlatform::Instance()->MarkEngine()->LaserOff();

	CMarkPlatform::Instance()->MarkEngine()->MarkEnd();     // 标定结束
}


void CDlgMarkPoint::OnBnClickedLaserOn2()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDCB_LASER_ON)->EnableWindow(FALSE);
	GetDlgItem(IDCB_LASER_ON2)->EnableWindow(FALSE);
	UpdateData(TRUE);

	MarkPoint(1, m_dbPosX2, m_dbPosY2);
}

void CDlgMarkPoint::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
}

BOOL CDlgMarkPoint::MarkPoint(int nHead, double x, double y)
{
	IMarkEngine * pMarkEngine = CMarkPlatform::Instance()->MarkEngine();
	IMarkDriver * pMarkDriver = CMarkPlatform::Instance()->MarkDriver();
	if (NULL == pMarkEngine || NULL == pMarkDriver)
	{
		AfxMessageBox(_T("硬件模块加载失败，请重启软件！"));
		return FALSE;
	}

	// 标记流程
	pMarkEngine->ActiveOutputSign(emSIGN_OUT_REDLIGHT, false);   // 关闭红光

	// 将标记中的坐标系位置参数清空
	xHeadTurn_t xHeadTurn;
	xHeadTurn.dx = 0.0;
	xHeadTurn.dy = 0.0;
	xHeadTurn.cx = 0.0;
	xHeadTurn.cy = 0.0;
	xHeadTurn.angle = 0.0;
	pMarkDriver->SetDriverCtrl(CMD_MASTER_TURN, &xHeadTurn);
	pMarkDriver->SetDriverCtrl(CMD_SLAVER_TURN, &xHeadTurn);

	pMarkDriver->SetScanHead(1, 1);   // 设定双头

	pMarkEngine->MarkBegin();    // 标定开始
	pMarkEngine->UpdatePen(m_pThePen);    // 更新笔

	pMarkEngine->LaserOn(x, y);

	return TRUE;
}