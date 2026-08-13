
#include "stdafx.h"
#include "ViewFactory.h"
#include "..\Sources\MarkPlatform.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CViewFactory
CViewFactory::CViewFactory()
{
	m_pWaferGraph = NULL;
	m_pMapDoc = NULL;
	m_nShowType = emShowContent_NULL;
}

CViewFactory::~CViewFactory()
{
}

BEGIN_MESSAGE_MAP(CViewFactory, CWnd)
    //{{AFX_MSG_MAP(CViewFactory)
    ON_WM_PAINT()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CViewFactory::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
    BOOL result;
    //注册窗体类
    static CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW);
    //创建窗体类
    result = CWnd::CreateEx(/*WS_EX_CLIENTEDGE | WS_EX_STATICEDGE*/0,
        className, NULL, dwStyle,
        rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
        pParentWnd->GetSafeHwnd(), (HMENU)nID);
    //更新窗体显示
    if (result != 0)
    {
		m_pMapDoc = CWaferEngine::Instance()->GetMapDoc();
		m_pCoordTrans = CCoordTransform::Instance();
		m_pTheRender = CMarkPlatform::Instance()->RenderEngine()->CreateRender(emRender_GDI, NULL);
        m_pTheRender->InitRender(this);
		m_pTheCoord = m_pTheRender->GetCoord();		
    }
    return TRUE;
}

void CViewFactory::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    if (NULL == GetSafeHwnd())
        return;

	CRect rcClent;
	GetClientRect(rcClent);
    
    if (NULL != m_pTheRender)
    {
		 bool bVisible = false;
		 m_pTheRender->SetRenderCtrl(IGraphRender::emGR_CMD_AXIS, &bVisible);
		 m_pTheRender->SetRenderCtrl(IGraphRender::emGR_CMD_GRID, &bVisible);
		 m_pTheRender->SetRenderCtrl(IGraphRender::emGR_CMD_ACROSS, &bVisible);
		 m_pTheRender->SetRenderCtrl(IGraphRender::emGR_CMD_ARROW, &bVisible);
		 double dbLimitX, dbLimitY;
		 if (emShowContent_Bin == m_nShowType)
		 {
			if (NULL != m_pWaferGraph)
			{
				dbLimitX = m_pWaferGraph->GetWidth() * 0.5;
				dbLimitY = m_pWaferGraph->GetHeight() * 0.5;
			}
			else
			{
				dbLimitX = 0;
				dbLimitY = 0;
			}
			m_pTheCoord->SetDrawLimit(dbLimitX, dbLimitY);
			m_pTheCoord->SetWindowRgn(rcClent.left, rcClent.top, rcClent.right, rcClent.bottom);
			m_pTheCoord->ZoomAll();
			m_pTheRender->BeginRender();
			if (NULL != m_pWaferGraph)
			{
				// 绘制背景刻度线
				// 0.1mm 小格
				m_pTheRender->BeginPaint(PS_SOLID, 1, RGB(220, 220, 220));
				double dbScale = 0.1;
				// 纵线
				double dbStep = dbScale;
				while(-dbLimitX + dbStep < dbLimitX && (!D_EQUAL(-dbLimitX + dbStep, dbLimitX)))
				{
					m_pTheRender->DrawLine(-dbLimitX + dbStep, dbLimitY, -dbLimitX + dbStep, -dbLimitY);
					dbStep += dbScale;
				}

				// 横线
				dbStep = dbScale;
				while (-dbLimitY + dbStep < dbLimitY && (!D_EQUAL(-dbLimitX + dbStep, dbLimitY)))
				{
					m_pTheRender->DrawLine(-dbLimitX, -dbLimitY + dbStep, dbLimitX, -dbLimitY + dbStep);
					dbStep += dbScale;
				}
				m_pTheRender->EndPaint();

				// 1mm大格
				m_pTheRender->BeginPaint(PS_DOT, 1, RGB(240, 130, 80));
				dbScale = 1;
				// 纵线
				dbStep = dbScale;
				while (-dbLimitX + dbStep < dbLimitX && (!D_EQUAL(-dbLimitX + dbStep, dbLimitX)))
				{
					m_pTheRender->DrawLine(-dbLimitX + dbStep, dbLimitY, -dbLimitX + dbStep, -dbLimitY);
					dbStep += dbScale;
				}

				// 横线
				dbStep = dbScale;
				while (-dbLimitY + dbStep < dbLimitY && (!D_EQUAL(-dbLimitX + dbStep, dbLimitY)))
				{
					m_pTheRender->DrawLine(-dbLimitX, -dbLimitY + dbStep, dbLimitX, -dbLimitY + dbStep);
					dbStep += dbScale;
				}
				m_pTheRender->EndPaint();

				m_pTheRender->SetRenderCtrl(IGraphRender::emGR_CMD_ARROW, &bVisible);
				m_pWaferGraph->Draw(m_pTheRender);
			}
		 }
		 else if(emShowContent_Map_Only == m_nShowType)
		 {
			CWaferGroup * pGroup = m_pMapDoc->GetGroup(0);
			xArray_t xArray;
			pGroup->GetArrayInfo(xArray);
			dbLimitX = xArray.group.dbGroupW * 0.5;
			dbLimitY = xArray.group.dbGroupH * 0.5;
			m_pTheCoord->SetDrawLimit(dbLimitX, dbLimitY);
			m_pTheCoord->SetWindowRgn(rcClent.left, rcClent.top, rcClent.right, rcClent.bottom);
			m_pTheCoord->ZoomAll();
			
			m_pTheRender->BeginRender();


			m_pTheCoord->SetViewportOrg(xArray.group.dbCenterX, xArray.group.dbCenterY);
			// 计算视图显示区域的物理范围
			DPoint_t dpTopLeft, dbBottomRight;
			CRect rect;
			GetClientRect(rect);
			pix2mm(rect.TopLeft(), dpTopLeft);
			pix2mm(rect.BottomRight(), dbBottomRight);
			FField_t fClient(dpTopLeft, dbBottomRight);
			m_pTheRender->SetRenderCtrl(IGraphRender::emGR_CMD_ARROW, &bVisible);
		
			m_pMapDoc->Draw(m_pTheRender, fClient, false);

		 }
		 else if (emShowContent_Map == m_nShowType)
		 {
			 xCoord_t CoordTmp0, CoordTmp1;
			 m_pCoordTrans->GetHeadCoord(0, CoordTmp0);
			 m_pCoordTrans->GetHeadCoord(1, CoordTmp1);
			 dbLimitX = (fabs(CoordTmp1.cx - CoordTmp0.cx) + CoordTmp0.rx + CoordTmp1.rx) / 2;
			 dbLimitY = max(CoordTmp0.ry, CoordTmp1.ry);
			 m_pTheCoord->SetDrawLimit(dbLimitX, dbLimitY);
			 m_pTheCoord->SetWindowRgn(rcClent.left, rcClent.top, rcClent.right, rcClent.bottom);
			 m_pTheCoord->ZoomAll();

			 FField_t fMarkA, fMarkB;
			 fMarkA.Left = CoordTmp0.cx - CoordTmp0.rx;
			 fMarkA.Top = CoordTmp0.cy + CoordTmp0.ry;
			 fMarkA.Right = CoordTmp0.cx + CoordTmp0.rx;
			 fMarkA.Bottom = CoordTmp0.cy - CoordTmp0.ry;

			 fMarkB.Left = CoordTmp1.cx - CoordTmp1.rx;
			 fMarkB.Top = CoordTmp1.cy + CoordTmp1.ry;
			 fMarkB.Right = CoordTmp1.cx + CoordTmp1.rx;
			 fMarkB.Bottom = CoordTmp1.cy - CoordTmp1.ry;

			 m_pTheRender->BeginRender();
			 m_pTheRender->BeginPaint(PS_DOT, 1, RGB(255, 0, 0));
			 m_pTheRender->DrawRectangle(fMarkA.Left, fMarkA.Top, fMarkA.Right, fMarkA.Bottom);
			 m_pTheRender->EndPaint();
			 m_pTheRender->BeginPaint(PS_DOT, 1, RGB(21, 6, 206));
			 m_pTheRender->DrawRectangle(fMarkB.Left, fMarkB.Top, fMarkB.Right, fMarkB.Bottom);
			 m_pTheRender->EndPaint();

			 // 计算视图显示区域的物理范围
			 DPoint_t dpTopLeft, dbBottomRight;
			 CRect rect;
			 GetClientRect(rect);
			 pix2mm(rect.TopLeft(), dpTopLeft);
			 pix2mm(rect.BottomRight(), dbBottomRight);
			 FField_t fClient(dpTopLeft, dbBottomRight);

			 m_pTheRender->SetRenderCtrl(IGraphRender::emGR_CMD_ARROW, &bVisible);
			 m_pMapDoc->Draw(m_pTheRender, fClient);
		 }
		else
		{
			m_pTheCoord->SetDrawLimit(0, 0);
			m_pTheRender->BeginRender();
		}
        m_pTheRender->FinishRender();
    }
}



BOOL CViewFactory::OnCommand(WPARAM wParam, LPARAM lParam)
{
    // TODO: 在此添加专用代码和/或调用基类
    UINT uMsg = LOWORD(wParam);
    CString sMsg;
 
    return __super::OnCommand(wParam, lParam);
}

void CViewFactory::SetShowContent(int nType, void * p)
{
	m_nShowType = nType;
	if (emShowContent_Bin == nType)
	{
		m_pWaferGraph = (CWaferGraph*)p;
	}
}

