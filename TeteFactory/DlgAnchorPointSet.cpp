// DlgMarkPointSet.cpp : 实现文件
//

#include "stdafx.h"
#include "TeteFactory.h"
#include "DlgAnchorPointSet.h"
#include "afxdialogex.h"
#include "Configure.h"
#include "MarkPlatform.h"
#include "Wafer\WaferEngine.h"
#include "tlString.h"
#include "CoordTransform.h"     

// CDlgAnchorPointSet 对话框

IMPLEMENT_DYNAMIC(CDlgAnchorPointSet, CDialogEx)

CDlgAnchorPointSet::CDlgAnchorPointSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_ANCHORPOINT_SET, pParent)
	, m_dbCenterX(0)
	, m_dbCenterY(0)
	,m_dbCenterX2(0)
	,m_dbCenterY2(0)
	, m_dbPitch(0)
	, m_dbFillPitch(0)
	, m_dbRadius(0)
{
	m_MarkPen = CMarkPlatform::Instance()->MarkDevice()->CreatePen();
	m_pConfig = CConfigure::Instance();
}

CDlgAnchorPointSet::~CDlgAnchorPointSet()
{
	// 记得释放笔对象
	delete m_MarkPen;
}

void CDlgAnchorPointSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CENTER_X, m_dbCenterX);
	DDX_Text(pDX, IDC_EDIT_CENTER_Y, m_dbCenterY);

	DDX_Text(pDX, IDC_EDIT_CENTER_X2, m_dbCenterX2);
	DDX_Text(pDX, IDC_EDIT_CENTER_Y_RIGHT, m_dbCenterY2);

	DDX_Text(pDX, IDC_EDIT_PITCH, m_dbPitch);
	DDX_Text(pDX, IDC_EDIT_FILLPITCH, m_dbFillPitch);
	DDX_Text(pDX, IDC_EDIT_RADIUS, m_dbRadius);
	DDX_Control(pDX, IDC_LIST_NINEPOINT, m_listNine);
}


BEGIN_MESSAGE_MAP(CDlgAnchorPointSet, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgAnchorPointSet::OnBnClickedOk)
	ON_BN_CLICKED(IDB_ANCHOR, &CDlgAnchorPointSet::OnBnClickedAnchor)
	ON_BN_CLICKED(IDB_ANCHOR2, &CDlgAnchorPointSet::OnBnClickedAnchor2)
	ON_BN_CLICKED(IDC_BUTTON_PENSET, &CDlgAnchorPointSet::OnBnClickedButtonPenset)
	ON_BN_CLICKED(IDCANCEL, &CDlgAnchorPointSet::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CDlgAnchorPointSet::OnBnClickedButtonApply)
END_MESSAGE_MAP()


// CDlgAnchorPointSet 消息处理程序


void CDlgAnchorPointSet::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!Apply())
	{
		return;
	}

	CDialogEx::OnOK();
}


BOOL CDlgAnchorPointSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitList(m_listNine);

	// 读取九点点位点信息
	CString s;
	s.Format(_T("%.3f"), m_pConfig->m_dbNinePRadius);
	m_dbRadius = _ttof(s);
	s.Format(_T("%.3f"), m_pConfig->m_dbNinePPitch);
	m_dbPitch = _ttof(s);
	s.Format(_T("%.3f"), m_pConfig->m_dbNinePFillPitch);
	m_dbFillPitch = _ttof(s);
	s.Format(_T("%.3f"), m_pConfig->m_dbNinePCenterX);
	m_dbCenterX = _ttof(s);
	s.Format(_T("%.3f"), m_pConfig->m_dbNinePCenterY);
	m_dbCenterY = _ttof(s);

	s.Format(_T("%.3f"), m_pConfig->m_dbNinePCenterX2);
	m_dbCenterX2 = _ttof(s);
	s.Format(_T("%.3f"), m_pConfig->m_dbNinePCenterY2);
	m_dbCenterY2 = _ttof(s);

	SetList(m_listNine, m_dbCenterX, m_dbCenterY);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDlgAnchorPointSet::InitList(CListCtrl & list)
{
	DWORD dwStyle = list.GetExtendedStyle();
	dwStyle |= LVS_EX_GRIDLINES;
	list.SetExtendedStyle(dwStyle);
	list.ModifyStyle(0, LVS_REPORT);

	CRect rect;
	list.GetClientRect(rect);
	int nW = rect.Width() / c_nAnchorCol;
	for (int i = 0; i < c_nAnchorCol; i++)
	{
		list.InsertColumn(i, _T(""), LVCFMT_LEFT, nW);
	}
	for (int i = 0; i < c_nAnchorRow; i++)
	{
		list.InsertItem(i, _T(""));
	}
}

void CDlgAnchorPointSet::SetList(CListCtrl & list, double dbX, double dbY)
{
	double X[c_nAnchorRow][c_nAnchorCol];
	double Y[c_nAnchorRow][c_nAnchorCol];
	CalcAnchorP(dbX, dbY, X, Y);
	
	CString s;
	int nCol = list.GetHeaderCtrl()->GetItemCount();
	int nRow = list.GetItemCount();
	for (int i = 0; i < nRow; i++)
	{
		for (int j = 0; j < nCol; j++)
		{
			s.Format(_T("(%.3f, %.3f)"), X[i][j], Y[i][j]);
			list.SetItemText(i, j, s);
		}
	}
}

void CDlgAnchorPointSet::CalcAnchorP(double dbX, double dbY, double(*X)[c_nAnchorCol], double(*Y)[c_nAnchorCol])
{
	// 计算左上角的点的坐标
	double dbPitch = m_dbPitch;
	double dx = dbX - ((c_nAnchorCol - 1) / 2) * dbPitch;
	double dy = dbY + ((c_nAnchorRow - 1) / 2) * dbPitch;
	
	for (int i = 0; i < c_nAnchorRow; i++)
	{
		for (int j = 0; j < c_nAnchorCol; j++)
		{
			X[i][j] = dx + j * dbPitch;
			Y[i][j] = dy - i * dbPitch;
		}
	}
}

void CDlgAnchorPointSet::OnBnClickedAnchor()
{
	// TODO: 在此添加控件通知处理程序代码
	EnableButton(FALSE);
	UpdateData(TRUE);
	SetList(m_listNine, m_dbCenterX, m_dbCenterY);

	// 保存九点定位信息
	m_pConfig->m_dbNinePRadius = m_dbRadius;
	m_pConfig->m_dbNinePFillPitch = m_dbFillPitch;
	m_pConfig->m_dbNinePPitch = m_dbPitch;
	m_pConfig->m_dbNinePCenterX = m_dbCenterX;
	m_pConfig->m_dbNinePCenterY = m_dbCenterY;

	m_pConfig->m_dbNinePCenterX2 = m_dbCenterX2;
	m_pConfig->m_dbNinePCenterY2 = m_dbCenterY2;

	MarkAnchorP(0, m_dbCenterX, m_dbCenterY, m_dbRadius);

	EnableButton(TRUE);
}

void CDlgAnchorPointSet::OnBnClickedAnchor2()
{
	// TODO: 在此添加控件通知处理程序代码
	EnableButton(FALSE);
	UpdateData(TRUE);
	SetList(m_listNine, m_dbCenterX2, m_dbCenterY2);

	// 保存九点定位信息
	m_pConfig->m_dbNinePRadius = m_dbRadius;
	m_pConfig->m_dbNinePFillPitch = m_dbFillPitch;
	m_pConfig->m_dbNinePPitch = m_dbPitch;
	m_pConfig->m_dbNinePCenterX = m_dbCenterX;
	m_pConfig->m_dbNinePCenterY = m_dbCenterY;

	m_pConfig->m_dbNinePCenterX2 = m_dbCenterX2;
	m_pConfig->m_dbNinePCenterY2 = m_dbCenterY2;
	
	MarkAnchorP(1, m_dbCenterX2, m_dbCenterY2, m_dbRadius);

	EnableButton(TRUE);
}

BOOL CDlgAnchorPointSet::MarkAnchorP(int nHead, double dbX, double dbY, double dbR, BOOL bNine)
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

	if (0 == nHead)
	{
		CCoordTransform::Instance()->Base2Head(0, dbX, dbY);
		pMarkDriver->SetScanHead(1, 0);   // 设定主头
	}
	else if (1 == nHead)
	{
		CCoordTransform::Instance()->Base2Head(1, dbX, dbY);
		pMarkDriver->SetScanHead(0, 1);   // 设定从头
	}
			
	pMarkEngine->MarkBegin();    // 标定开始
	pMarkEngine->UpdatePen(m_MarkPen);    // 更新笔

	if (bNine)
	{
		// 标记9个点
		double dbPitch = m_dbPitch;
		double X[c_nAnchorRow][c_nAnchorCol];
		double Y[c_nAnchorRow][c_nAnchorCol];
		CalcAnchorP(dbX, dbY, X, Y);

		for (int i = 0; i < c_nAnchorRow; i++)
		{
			for (int j = 0; j < c_nAnchorCol; j++)
			{
				double dbRadius = dbR;
				while (dbRadius > 0.1)
				{
					pMarkEngine->MarkCircle(X[i][j], Y[i][j], dbRadius);
					dbRadius -= m_dbFillPitch;
				}
			}
		}
	}
	else
	{
		double dbRadius = dbR;
		while (dbRadius > 0)
		{
			pMarkEngine->MarkCircle(dbX, dbY, dbRadius);
			dbRadius -= m_dbFillPitch;
		}
	}


	pMarkEngine->MarkEnd();     // 标定结束
	pMarkEngine->ActiveOutputSign(emSIGN_OUT_REDLIGHT, true);   // 开启红光

	return TRUE;
}

void CDlgAnchorPointSet::OnBnClickedButtonPenset()
{
	// TODO: 在此添加控件通知处理程序代码
	int nPenOptionWndFlag = PenOptionWndFlag_EDIT;
	CMarkPlatform::Instance()->MarkDevice()->SetDeviceCtrl(Laser_SetPenOptionWndFlag_I, (void*)&nPenOptionWndFlag);
	CMarkPlatform::Instance()->MarkDevice()->ShowPenOptionWnd(m_MarkPen);
}



void CDlgAnchorPointSet::EnableButton(BOOL b)
{
	GetDlgItem(IDB_ANCHOR)->EnableWindow(b);
	GetDlgItem(IDB_ANCHOR2)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON_PENSET)->EnableWindow(b); 
}

void CDlgAnchorPointSet::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码

	CDialogEx::OnCancel();
}

void CDlgAnchorPointSet::OnBnClickedButtonApply()
{
	// TODO: 在此添加控件通知处理程序代码
	Apply();
}

BOOL CDlgAnchorPointSet::Apply()
{
	UpdateData(TRUE);
	// 保存九点定位信息
	m_pConfig->m_dbNinePRadius = m_dbRadius;
	m_pConfig->m_dbNinePFillPitch = m_dbFillPitch;
	m_pConfig->m_dbNinePPitch = m_dbPitch;
	m_pConfig->m_dbNinePCenterX = m_dbCenterX;
	m_pConfig->m_dbNinePCenterY = m_dbCenterY;

	m_pConfig->m_dbNinePCenterX2 = m_dbCenterX2;
	m_pConfig->m_dbNinePCenterY2 = m_dbCenterY2;
	return TRUE;
}


