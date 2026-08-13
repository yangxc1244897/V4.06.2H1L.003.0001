
#include "stdafx.h"
#include "ViewSingleBin.h"
#include "..\Sources\MarkPlatform.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static COLORREF g_clrTitleBk[] = {RGB(128, 0, 0), RGB(0, 128, 0), RGB(0, 0, 128),
									RGB(128, 128, 0), RGB(128, 0, 128), RGB(0, 128, 128), RGB(128, 128, 255), RGB(0, 0, 0)};
/////////////////////////////////////////////////////////////////////////////
// CViewSingleBin
CViewSingleBin::CViewSingleBin()
{
	m_pWaferGraph = NULL;
	m_pWaferGraphDraw = NULL;
	m_bShowSelFlag = FALSE;
}

CViewSingleBin::~CViewSingleBin()
{
	for (auto & item : m_Brush)
	{
		item.DeleteObject();
	}

	m_BrushSelFlag.DeleteObject();
	m_BrushOKFlag.DeleteObject();
}

BEGIN_MESSAGE_MAP(CViewSingleBin, CWnd)
    //{{AFX_MSG_MAP(CViewSingleBin)
    ON_WM_PAINT()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CViewSingleBin::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
    BOOL result;
    //注册窗体类
    static CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW);
    //创建窗体类
    result = CWnd::CreateEx(/*WS_EX_CLIENTEDGE | */WS_EX_STATICEDGE,
        className, NULL, dwStyle,
        rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
        pParentWnd->GetSafeHwnd(), (HMENU)nID);
    //更新窗体显示
    if (result != 0)
    {
		m_pTheRender = CMarkPlatform::Instance()->RenderEngine()->CreateRender(emRender_GDI, NULL);
        m_pTheRender->InitRender(this);
		m_pTheCoord = m_pTheRender->GetCoord();
		for (int i = 0; i < MAX_BIN_COUNT; i++)
		{
			m_Brush[i].CreateSolidBrush(g_clrTitleBk[i%8]);
		}
		m_BrushSelFlag.CreateSolidBrush(RGB(0, 255, 0));
		m_BrushOKFlag.CreateSolidBrush(RGB(255, 255, 255));
		m_nMarkTimes = 0;
		m_bShowSelFlag = FALSE;
		m_bShowTitle = TRUE;
    }
    return TRUE;
}

void CViewSingleBin::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    if (NULL == GetSafeHwnd())
        return;

	if (NULL != m_pWaferGraph && NULL != m_pTheRender)
	{
		CRect rcClient, rcTitle, rcWafer;
		GetClientRect(rcClient);
		rcWafer.left = rcClient.left;
		rcWafer.top = rcClient.top;
		rcWafer.right = rcClient.right;
		rcWafer.bottom = rcClient.bottom;

		rcTitle.left = rcClient.left;
		rcTitle.top = rcClient.bottom - 20;
		rcTitle.right = rcClient.right;
		rcTitle.bottom = rcClient.bottom;

		// 绘制中间芯片部分
		bool bVisible = false;
		m_pTheRender->SetRenderCtrl(IGraphRender::emGR_CMD_AXIS, &bVisible);
		m_pTheRender->SetRenderCtrl(IGraphRender::emGR_CMD_GRID, &bVisible);
		m_pTheRender->SetRenderCtrl(IGraphRender::emGR_CMD_ACROSS, &bVisible);
		m_pTheRender->SetRenderCtrl(IGraphRender::emGR_CMD_ARROW, &bVisible);
		double dbLimitX, dbLimitY;
		if (NULL != m_pWaferGraph){
			ChangeWaferDir();  // 改变印章方向
			dbLimitX = m_pWaferGraphDraw->GetWidth() * 0.5;
			dbLimitY = m_pWaferGraphDraw->GetHeight() * 0.5;
		}
		else{
			dbLimitX = 0;
			dbLimitY = 0;
		}
		m_pTheCoord->SetDrawLimit(dbLimitX, dbLimitY);
		m_pTheCoord->SetWindowRgn(rcWafer.left, rcWafer.top, rcWafer.right, rcWafer.bottom);
		m_pTheCoord->ZoomAll();
		m_pTheRender->BeginRender();
		if (NULL != m_pWaferGraphDraw)
		{
			//lrf start 2021/1/3 去掉背景的网格线
			// 绘制背景刻度线
			// 0.1mm 小格
			//m_pTheRender->BeginPaint(PS_SOLID, 1, RGB(220, 220, 220));
			//double dbScale = 0.1;
			//// 纵线
			//double dbStep = dbScale;
			//while (-dbLimitX + dbStep < dbLimitX && (!D_EQUAL(-dbLimitX + dbStep, dbLimitX)))
			//{
			//	m_pTheRender->DrawLine(-dbLimitX + dbStep, dbLimitY, -dbLimitX + dbStep, -dbLimitY);
			//	dbStep += dbScale;
			//}

			//// 横线
			//dbStep = dbScale;
			//while (-dbLimitY + dbStep < dbLimitY && (!D_EQUAL(-dbLimitY + dbStep, dbLimitY)))
			//{
			//	m_pTheRender->DrawLine(-dbLimitX, -dbLimitY + dbStep, dbLimitX, -dbLimitY + dbStep);
			//	dbStep += dbScale;
			//}
			//m_pTheRender->EndPaint();

			//// 1mm大格
			//m_pTheRender->BeginPaint(PS_DOT, 1, RGB(240, 130, 80));
			//dbScale = 1;
			//// 纵线
			//dbStep = dbScale;
			//while (-dbLimitX + dbStep < dbLimitX && (!D_EQUAL(-dbLimitX + dbStep, dbLimitX)))
			//{
			//	m_pTheRender->DrawLine(-dbLimitX + dbStep, dbLimitY, -dbLimitX + dbStep, -dbLimitY);
			//	dbStep += dbScale;
			//}

			//// 横线
			//dbStep = dbScale;
			//while (-dbLimitY + dbStep < dbLimitY && (!D_EQUAL(-dbLimitX + dbStep, dbLimitY)))
			//{
			//	m_pTheRender->DrawLine(-dbLimitX, -dbLimitY + dbStep, dbLimitX, -dbLimitY + dbStep);
			//	dbStep += dbScale;
			//}
			//m_pTheRender->EndPaint();
			//lrf  end

			m_pTheRender->SetRenderCtrl(IGraphRender::emGR_CMD_ARROW, &bVisible);

			m_pWaferGraphDraw->Draw(m_pTheRender);
		}
		m_pTheRender->FinishRender();

		delete m_pWaferGraphDraw;
		//CBrush *pOldBrush = nullptr;
		//if (m_bShowTitle)
		//{
		//	// 绘制外框和底部标题部分
		//	pOldBrush = dc.SelectObject(&m_Brush[m_nBinIndex]);
		//	dc.Rectangle(rcTitle);
		//	dc.SelectObject(pOldBrush);
		//	dc.SetBkMode(TRANSPARENT);
		//	dc.SetTextColor(RGB(255, 255, 255));
		//	CString s;
		//	s.Format(_T("BIN%d"), m_nBinIndex);
		//	dc.DrawText(s, rcTitle, DT_CENTER);
		//}
	}
}

BOOL CViewSingleBin::OnCommand(WPARAM wParam, LPARAM lParam)
{
    // TODO: 在此添加专用代码和/或调用基类
    UINT uMsg = LOWORD(wParam);
    CString sMsg;
 
    return __super::OnCommand(wParam, lParam);
}

void CViewSingleBin::SetShowContent(int nBin, CWaferGraph * pWafer)
{
	m_nBinIndex = nBin;
	m_pWaferGraph = pWafer;
}

void CViewSingleBin::SetMarkTimes(int n)
{
	m_nMarkTimes = n;
}

void CViewSingleBin::ShowSelFlag(BOOL bShow)
{
	m_bShowSelFlag = bShow;
}

void CViewSingleBin::ShowTitle(BOOL bShow)
{
	m_bShowTitle = bShow;
}

void CViewSingleBin::ChangeWaferDir()
{
	CWaferGroup* pWaferGroup = CWaferEngine::Instance()->GetMapDoc()->GetGroup(0);
	if (nullptr == pWaferGroup)
		return;

	xArray_t xArray;
	pWaferGroup->GetArrayInfo(xArray);
	m_pWaferGraphDraw = m_pWaferGraph->ChangeWaferDirAndClone(xArray.cell.nWaferDeg);
}