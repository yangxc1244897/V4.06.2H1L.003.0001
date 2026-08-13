#include "stdafx.h"
#include "MapAdjustView.h"
#include "tlGraphObject.h"
#include "MarkPlatform.h"
#include "Wafer\WaferEngine.h"
#include <set>
#include <vector>
#include <tuple>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define  MOUSE_ARROW        0x0001
#define  MOUSE_HAND         0x0002
#define  MOUSE_SIZENS       0x0003
#define  MOUSE_SIZENESW     0x0004
#define  MOUSE_SIZEWE       0x0005
#define  MOUSE_SIZENWSE     0x0006
#define  MOUSE_SIZEALL      0x0007
/////////////////////////////////////////////////////////////////////////////
// CWaferMapView
CWaferMapView::CWaferMapView()
{
    m_clBackColor = RGB(255, 255, 255);
    m_clRulerColor = RGB(0, 0, 0);
    m_clTextColor = RGB(60, 60, 60);
    m_clGridColor = RGB(195, 195, 195);
    m_clCrossColor = RGB(100, 100, 100);
    m_clCurveColor = RGB(128, 128, 0);

	m_InfoFont.CreatePointFont(80, _T("Arial"));

    m_iPolygonSegNum = 6;

    m_iDrawMouseFlag = 0x0000;
    m_iMouseFlag = MOUSE_ARROW;
    m_emDrawType = emGraph_Arrow;
    m_pTheCoord = NULL;
	m_IndexBackground.CreateSolidBrush(GetSysColor(CTLCOLOR_BTN));
}

CWaferMapView::~CWaferMapView()
{
	m_InfoFont.DeleteObject();
	m_IndexBackground.DeleteObject();
}

BEGIN_MESSAGE_MAP(CWaferMapView, CWnd)
    //{{AFX_MSG_MAP(CWaferMapView)
    ON_WM_PAINT()
    //}}AFX_MSG_MAP
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEWHEEL()
    ON_WM_KEYDOWN()
    ON_WM_RBUTTONDOWN()
    ON_WM_CHAR()
    ON_WM_KILLFOCUS()
    ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


BOOL CWaferMapView::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
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
		m_pParentWnd = pParentWnd;
		m_pTheMapDoc = nullptr;
		m_pCoordTrans = CCoordTransform::Instance();
		m_pTheRender = CMarkPlatform::Instance()->RenderEngine()->CreateRender(emRender_GDI, NULL);
		m_pTheRender->InitRender(this);
		m_pTheCoord = m_pTheRender->GetCoord();
	}

    return TRUE;
}
void CWaferMapView::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    if (NULL == GetSafeHwnd())
        return;

	CRect rcClent;
	GetClientRect(rcClent);

    if (NULL != m_pTheRender && NULL != m_pTheMapDoc)
    {
		bool bVisible = false;
		m_pTheRender->SetRenderCtrl(IGraphRender::emGR_CMD_AXIS, &bVisible);
		m_pTheRender->SetRenderCtrl(IGraphRender::emGR_CMD_GRID, &bVisible);
		m_pTheRender->SetRenderCtrl(IGraphRender::emGR_CMD_ACROSS, &bVisible);
		FField_t matrixF = m_pTheMapDoc->GetMatrixField();
		double dbCX = matrixF.CenterX();
		double dbCY = matrixF.CenterY();
		double dbLimitX = matrixF.width() * 0.5;
		double dbLimitY = matrixF.height() * 0.5;
		m_pTheCoord->SetDrawLimit(dbLimitX, dbLimitY);
		m_pTheCoord->SetWindowRgn(rcClent.left, rcClent.top, rcClent.right, rcClent.bottom);
		if(m_bFixedCanvas)	m_pTheCoord->ZoomAll();
		
		m_pTheRender->BeginRender();
		
		if (m_bFixedCanvas)	m_pTheCoord->SetViewportOrg(dbCX, dbCY);
		// 计算视图显示区域的物理范围
		DPoint_t dpTopLeft, dbBottomRight;
		CRect rect;
		GetClientRect(rect);
		pix2mm(rect.TopLeft(), dpTopLeft);
		pix2mm(rect.BottomRight(), dbBottomRight);
		FField_t fClient(dpTopLeft, dbBottomRight);
		m_fClient = fClient;

        m_pTheMapDoc->Draw(m_pTheRender, fClient, false);
        if(0!= m_iDrawMouseFlag)
            DrawDrawStatus();

		// 芯片表面绘制信息
		DrawFaceInfo(m_pTheRender);

		//DrawSelField();

        m_pTheRender->FinishRender();
    }
}

void CWaferMapView::DrawFaceInfo(IGraphRender * pRender)
{
	if (NULL == pRender)
		return;
    
	double dbFontW = 0,dbFontH = 0,  dbFontPitch = 0;
	m_pTheCoord->pix2mm(20, dbFontW);
	m_pTheCoord->pix2mm(16, dbFontH);
	m_pTheCoord->pix2mm(10, dbFontPitch);

	pRender->BeginPaint(PS_SOLID, 1, RGB(0, 0, 0));
	pRender->SetBrush(&m_IndexBackground);
	pRender->SetTextStyle(RGB(255, 97, 0), &m_InfoFont);
	
	CStringA s;
	FField_t field;
	size_t nGroupSize = m_pTheMapDoc->vWaferGroups.size();
	auto funcDrawIndexBackground = [](IGraphRender * pRender, const FField_t& fontField) {
		DPoint_t dpPolygon[5];
		dpPolygon[0] = DPoint_t(fontField.Left, fontField.Top);
		dpPolygon[1] = DPoint_t(fontField.Right, fontField.Top);
		dpPolygon[2] = DPoint_t(fontField.Right, fontField.Bottom);
		dpPolygon[3] = DPoint_t(fontField.Left, fontField.Bottom);
		dpPolygon[4] = DPoint_t(fontField.Left, fontField.Top);
		pRender->DrawPolygon(dpPolygon, 5, true); 
	};
	for (int i = 0; i < nGroupSize; i++)
	{
		CWaferGroup *pWaferGroup = m_pTheMapDoc->vWaferGroups[i];
		size_t nWaferSize = pWaferGroup->vWaferMetas.size();
		if ( 0 == nWaferSize || nWaferSize > 2000)  // 超过2000颗，不再显示
			continue;
		// 显示行和列号
		int nColC = pWaferGroup->GetColCount();
		int nRowC = pWaferGroup->GetRowCount();
		for (int j = 0; j < nWaferSize; j++)
		{
			pWaferGroup->vWaferMetas[j].xDim.GetField(field);
			if (field.Left > m_fClient.Right
				|| field.Right < m_fClient.Left
				|| field.Top < m_fClient.Bottom
				|| field.Bottom > m_fClient.Top)
				continue;
			double dbWidth = field.width();
			double dbHeight = field.height();
			FField_t fontField;
			if (1 == pWaferGroup->vWaferMetas[j].nCol)
			{	
				fontField.Left = field.Left - (dbFontW + dbFontPitch);
				fontField.Right = fontField.Left + dbFontW;
				fontField.Top = field.Top - (field.height() - dbFontH) * 0.5;
				fontField.Bottom = fontField.Top - dbFontH;
				funcDrawIndexBackground(pRender, fontField);
				s.Format("%d", nRowC - pWaferGroup->vWaferMetas[j].nRow + 1);
				pRender->DrawText(s.GetString(), fontField, DT_CENTER);
			}
			if (nColC == pWaferGroup->vWaferMetas[j].nCol)
			{
				fontField.Left = field.Right + dbFontPitch;
				fontField.Right = fontField.Left + dbFontW;
				fontField.Top = field.Top - (field.height() - dbFontH) * 0.5;
				fontField.Bottom = fontField.Top - dbFontH;

				// 芯片行
				funcDrawIndexBackground(pRender, fontField);
				s.Format("%d", nRowC - pWaferGroup->vWaferMetas[j].nRow + 1);
				pRender->DrawText(s.GetString(), fontField, DT_CENTER);
			}
			if (1 == pWaferGroup->vWaferMetas[j].nRow)
			{
				fontField.Left = field.Left + (field.width() - dbFontW) * 0.5;
				fontField.Right = fontField.Left + dbFontW;
				fontField.Top = field.Top + (dbFontH + dbFontPitch);
				fontField.Bottom = fontField.Top - dbFontH;

				// 芯片列
				funcDrawIndexBackground(pRender, fontField);
				s.Format("%d", pWaferGroup->vWaferMetas[j].nCol);
				pRender->DrawText(s.GetString(), fontField, DT_CENTER);
			}
			if (nRowC == pWaferGroup->vWaferMetas[j].nRow)
			{
				fontField.Left = field.Left + (field.width() - dbFontW) * 0.5;
				fontField.Right = fontField.Left + dbFontW;
				fontField.Top = field.Bottom - dbFontPitch;
				fontField.Bottom = field.Bottom - (dbFontH + dbFontPitch);

				// 芯片列
				funcDrawIndexBackground(pRender, fontField);
				s.Format("%d", pWaferGroup->vWaferMetas[j].nCol);
				pRender->DrawText(s.GetString(), fontField, DT_CENTER);
			}
			
			if (field.isInside(m_MousePoint.x, m_MousePoint.y))
			{
				GetParent()->GetParent()->PostMessage(WM_TETE_MAPSET_CELLINFO, i, j);
			}
		}  
	}
	pRender->EndPaint();
}
void CWaferMapView::DrawDrawStatus(void)
{
    m_pTheRender->BeginPaint(PS_DOT, 1, RGB(255, 0, 0));
    bool IsVisible = false;
    m_pTheRender->SetRenderCtrl(IGraphRender::emGR_CMD_ARROW, &IsVisible);
    switch (m_emDrawType)
    {
    case emGraph_Arrow:
        if (0x0002 == m_iDrawMouseFlag)
        {            
            m_pTheRender->DrawRectangle(m_PrevPoint.x, m_PrevPoint.y,m_LastPoint.x, m_LastPoint.y);
        }
        else if (0x0F00 & m_iDrawMouseFlag)
        {            
            if (GetObjSelCount())
            {  			
                m_pTheRender->DrawRectangle(m_rtSelField.Left, m_rtSelField.Top, m_rtSelField.Right, m_rtSelField.Bottom);
            }
        }
        break;
    case emGraph_Hand:
        break;
    case emGraph_ZoomIn:
        break;
    case emGraph_ZoomOut:
        break;
    case emGraph_Whole:
        break;
    case emGraph_Part:
        break;
  
    default:break;
    }

    m_pTheRender->EndPaint();
}
void CWaferMapView::DrawSelField(void)
{
    if (NULL == m_pTheMapDoc)return;
	int iSelCount = GetObjSelCount();
    if (iSelCount > 0)
    {
        FField_t selField;
		GetSelField(selField);
        m_pTheRender->BeginPaint(PS_DOT, 1, RGB(255, 0, 0));
        bool IsVisible = false;
        m_pTheRender->SetRenderCtrl(IGraphRender::emGR_CMD_ARROW, &IsVisible);
        m_pTheRender->DrawRectangle(selField.Left,selField.Top,selField.Right,selField.Bottom);
        double mx, my, w;
        m_pTheCoord->pix2mm(6, w);
        mx = selField.CenterX();
        my = selField.CenterY();
        m_pTheRender->DrawEllipse(mx - w, my + w, mx + w, my - w);
        m_pTheRender->DrawLine(mx, my + w, mx, my - w);
        m_pTheRender->DrawLine(mx - w, my, mx + w, my);
        m_pTheCoord->pix2mm(3, w);
        mx = selField.Left;
        my = selField.Top;
  
        m_pTheRender->EndPaint();        
    }
}

void CWaferMapView::DrawAnchor(IGraphRender * pRender)
{
	if (NULL == pRender || NULL == m_pTheCoord)
		return;

	double dbRadius = 0;
	m_pTheCoord->pix2mm(10, dbRadius);

	// 理论定位点
	pRender->BeginPaint(PS_SOLID, 1, RGB(60, 80, 130));
	CBrush brush(RGB(60, 80, 130));
	pRender->SetBrush(&brush);
	DPoint_t dpAnchor;
	
	int nAnchorC = m_pTheMapDoc->GetAnchorCount();
	for (int i = 0; i < nAnchorC; i++)
	{
		m_pTheMapDoc->GetAnchor(i, dpAnchor);
		m_pTheRender->DrawCircle(dpAnchor.x, dpAnchor.y, dbRadius);
	}
	pRender->EndPaint();

	// CCD定位点
	pRender->BeginPaint(PS_SOLID, 1, RGB(255, 0 ,0));
	CBrush brush2(RGB(255, 0, 0));
	pRender->SetBrush(&brush2);
	nAnchorC = m_pTheMapDoc->GetAnchorCount(true);
	for (int i = 0; i < nAnchorC; i++)
	{
		m_pTheMapDoc->GetAnchor(i, dpAnchor, true);
		m_pTheRender->DrawCircle(dpAnchor.x, dpAnchor.y, dbRadius);
	}
	pRender->EndPaint();
}

void CWaferMapView::OnChangeDoc(CWaferMapDoc *pTheDoc)
{
	if (nullptr == pTheDoc)
		return;

	m_pTheMapDoc = pTheDoc;

    m_iDrawMouseFlag = 0x0000;
    m_iMouseFlag = MOUSE_ARROW;
    m_emDrawType = emGraph_Arrow;
    m_vDrawPoints.clear();

    Invalidate(false);

}
void CWaferMapView::OnDrawChange(emDrawType_t emType)
{
    // 如果为多边形绘制，则弹出多边形边数输入框
	m_iDrawMouseFlag = 0x0000;
    m_emDrawType = emType;   
    m_vDrawPoints.clear();
    switch(emType)
    {
    case emGraph_Arrow:
        m_iMouseFlag = MOUSE_ARROW;
        SetClassLongPtr(this->m_hWnd, GCLP_HCURSOR, (long)LoadCursor(NULL, IDC_ARROW));
        break;
    case emGraph_Hand:
        m_iMouseFlag = MOUSE_HAND;
        SetClassLongPtr(this->m_hWnd, GCLP_HCURSOR, (long)LoadCursor(NULL, IDC_HAND));
        break;
    default:
        SetClassLongPtr(this->m_hWnd, GCLP_HCURSOR, (long)LoadCursor(NULL, IDC_CROSS));
        break;
    }
}
void CWaferMapView::OnGraphArrowDown(UINT nFlags, CPoint point)
{
    switch (m_iMouseFlag)
    {
    case MOUSE_ARROW:  //--已选择
        if (0x0000 == m_iDrawMouseFlag)
        {
            m_iDrawMouseFlag = 0x0001;
            m_pTheCoord->pix2mm(point.x,point.y, m_PrevPoint.x,m_PrevPoint.y);
            m_LastPoint = m_PrevPoint;
        }
        else
        {
			if (isMutilSel())
			{
				m_iDrawMouseFlag = 0x0001;
			}
			else
			{
				m_iDrawMouseFlag = 0x0000;	
				UnSelectAllObjects();
			}
        }
        break;
    case MOUSE_HAND: //--
        m_iDrawMouseFlag = 0x0001;
        m_pTheCoord->pix2mm(point.x,point.y, m_PrevPoint.x,m_PrevPoint.y); //--m_PrevPoint = point;
        m_LastPoint = m_PrevPoint;
        break;
    case MOUSE_SIZENS:
        if (0x0101 == m_iDrawMouseFlag)//--0001北
        {
            m_iDrawMouseFlag &= ~0x0100;
            m_iDrawMouseFlag |= 0x0200;
            m_pTheCoord->pix2mm(point.x,point.y, m_PrevPoint.x,m_PrevPoint.y); //--m_PrevPoint = point;
            m_LastPoint = m_PrevPoint;
        }
        else if (0x0102 == m_iDrawMouseFlag)//--0010南
        {
            m_iDrawMouseFlag &= ~0x0100;
            m_iDrawMouseFlag |= 0x0200;
            m_pTheCoord->pix2mm(point.x,point.y, m_PrevPoint.x,m_PrevPoint.y); //--m_PrevPoint = point;
            m_LastPoint = m_PrevPoint;
        }
        break;
    case MOUSE_SIZENESW:
        if (0x0109 == m_iDrawMouseFlag)//--1001东北
        {
            m_iDrawMouseFlag &= ~0x0100;
            m_iDrawMouseFlag |= 0x0200;
            m_pTheCoord->pix2mm(point.x,point.y, m_PrevPoint.x,m_PrevPoint.y); //--m_PrevPoint = point;
            m_LastPoint = m_PrevPoint;
        }
        else if (0x0106 == m_iDrawMouseFlag)//--0110西南
        {
            m_iDrawMouseFlag &= ~0x0100;
            m_iDrawMouseFlag |= 0x0200;
            m_pTheCoord->pix2mm(point.x,point.y, m_PrevPoint.x,m_PrevPoint.y); //--m_PrevPoint = point;
            m_LastPoint = m_PrevPoint;
        }
        break;
    case MOUSE_SIZEWE:
        if (0x0108 == m_iDrawMouseFlag)//--1000东
        {
            m_iDrawMouseFlag &= ~0x0100;
            m_iDrawMouseFlag |= 0x0200;
            m_pTheCoord->pix2mm(point.x,point.y, m_PrevPoint.x,m_PrevPoint.y); //--m_PrevPoint = point;
            m_LastPoint = m_PrevPoint;
        }
        else if (0x0104 == m_iDrawMouseFlag)//--0100西
        {
            m_iDrawMouseFlag &= ~0x0100;
            m_iDrawMouseFlag |= 0x0200;
            m_pTheCoord->pix2mm(point.x,point.y, m_PrevPoint.x,m_PrevPoint.y); //--m_PrevPoint = point;
            m_LastPoint = m_PrevPoint;
        }
        break;
    case MOUSE_SIZENWSE:
        if (0x0105 == m_iDrawMouseFlag)//--0101西北
        {
            m_iDrawMouseFlag &= ~0x0100;
            m_iDrawMouseFlag |= 0x0200;
            m_pTheCoord->pix2mm(point.x,point.y, m_PrevPoint.x,m_PrevPoint.y); //--m_PrevPoint = point;
            m_LastPoint = m_PrevPoint;
        }
        else if (0x010A == m_iDrawMouseFlag)//--1010东南
        {
            m_iDrawMouseFlag &= ~0x0100;
            m_iDrawMouseFlag |= 0x0200;
            m_pTheCoord->pix2mm(point.x,point.y, m_PrevPoint.x,m_PrevPoint.y); //--m_PrevPoint = point;
            m_LastPoint = m_PrevPoint;
        }
        break;
    case MOUSE_SIZEALL:   //--拖拽
        if (0x010F == m_iDrawMouseFlag)
        {
            m_iDrawMouseFlag &= ~0x0100;
            m_iDrawMouseFlag |= 0x0200;
            m_pTheCoord->pix2mm(point.x,point.y, m_PrevPoint.x,m_PrevPoint.y); //--m_PrevPoint = point;
            m_LastPoint = m_PrevPoint;
        }
        break;
    default:break;
    }
}
void CWaferMapView::OnGraphArrowMove(UINT nFlags, CPoint point)
{
    pix2mm(point, m_LastPoint);
    //if (0x0100 & m_iDrawMouseFlag)//--已处于选中状态
    //{
    //    doMouseSwitch(m_LastPoint.x, m_LastPoint.y);
    //}
    switch (m_iDrawMouseFlag)
    {
	case 0x0:
	case 0x0001:     //--选择
		if (this == GetCapture())
			m_iDrawMouseFlag = 0x0002;  //--鼠标移动则采用区域选择
		break;
    case 0x0002: //--选择
        m_iDrawMouseFlag = 0x0002;  //--记录是否移动
        break;
//     case 0x0202:  //--0010南
//         m_rtSelectField = m_rtSelRangeOrg;
//         m_rtSelectField.Bottom += (m_LastPoint.y - m_PrevPoint.y);
//         break;
//     case 0x0201: //--0001北
//         m_rtSelectField = m_rtSelRangeOrg;
//         m_rtSelectField.Top += (m_LastPoint.y - m_PrevPoint.y);
//         break;
//     case 0x0209://--1001东北
//         m_rtSelectField = m_rtSelRangeOrg;
//         m_rtSelectField.Top += (m_LastPoint.y - m_PrevPoint.y);
//         m_rtSelectField.Right += (m_LastPoint.x - m_PrevPoint.x);
//         break;
//     case 0x0206://--0110西南
//         m_rtSelectField = m_rtSelRangeOrg;
//         m_rtSelectField.Left += (m_LastPoint.x - m_PrevPoint.x);
//         m_rtSelectField.Bottom += (m_LastPoint.y - m_PrevPoint.y);
//         break;
//     case 0x0208://--1000东
//         m_rtSelectField = m_rtSelRangeOrg;
//         m_rtSelectField.Right += (m_LastPoint.x - m_PrevPoint.x);
//         break;
//     case 0x0204://--0100西
//         m_rtSelectField = m_rtSelRangeOrg;
//         m_rtSelectField.Left += (m_LastPoint.x - m_PrevPoint.x);
//         break;
//     case 0x0205://--0101西北
//         m_rtSelectField = m_rtSelRangeOrg;
//         m_rtSelectField.Left += (m_LastPoint.x - m_PrevPoint.x);
//         m_rtSelectField.Top += (m_LastPoint.y - m_PrevPoint.y);
//         break;
//     case 0x020A://--1010东南
//         m_rtSelectField = m_rtSelRangeOrg;
//         m_rtSelectField.Right += (m_LastPoint.x - m_PrevPoint.x);
//         m_rtSelectField.Bottom += (m_LastPoint.y - m_PrevPoint.y);
//         break;
    case 0x020F:   //--拖拽
    {
  //      double dx = 0, dy = 0;
		//GetSelField(m_rtSelField);
  //      dx = m_LastPoint.x - m_PrevPoint.x;
  //      dy = m_LastPoint.y - m_PrevPoint.y;
		//m_rtSelField.Left += dx;
		//m_rtSelField.Top += dy;
		//m_rtSelField.Right += dx;
		//m_rtSelField.Bottom += dy;
    }
    break;
    default:
        break;
    }
}
void CWaferMapView::OnGraphArrowUp(UINT nFlags, CPoint point)
{
    switch (m_iMouseFlag)
    {
    case MOUSE_ARROW:            //--选择
        if (0x0001 == m_iDrawMouseFlag || 0x0000 == m_iDrawMouseFlag)//--点选对象
		{
			doSelectObject(m_LastPoint.x, m_LastPoint.y);
        }
		else if (0x0002 == m_iDrawMouseFlag)//--区域选对象
		{
			doSelectObject(m_PrevPoint.x, m_PrevPoint.y, m_LastPoint.x, m_LastPoint.y);
        }
        else
		{
         //   CMsgRouter::Instance()->PostMessage(this->m_iID,WM_TETE_SEL_DOC,(uint32_t)((void*)m_pTheMarkDoc));
        }
        break;
    case MOUSE_HAND: //--处理选择区域
        /*if (0x0001 == m_iDrawMouseFlag)//--点选对象
        {
            double px, py;
            px = m_PrevPoint.x; py = m_PrevPoint.y;

            if(m_pTheMarkDoc->SelectObjects(px,py)>0)
                m_iDrawMouseFlag = 0x0100;
        }
        else if (0x0002 == m_iDrawMouseFlag)//--区域选择对象
        {
            double x1, y1, x2, y2;
            x1 = m_PrevPoint.x; y1 = m_PrevPoint.y;
            x2 = m_LastPoint.x; y2 = m_LastPoint.y;
            if(m_pTheMarkDoc->SelectObjects(x1,y1,x2,y2)>0)
                m_iDrawMouseFlag = 0x0100;
        }*/
        break;
    case MOUSE_SIZENS:    //--南、北
    case MOUSE_SIZENESW:  //--东北、西南
    case MOUSE_SIZEWE:    //--东、西
    case MOUSE_SIZENWSE:  //--西北、东南
        if (0x0200 & m_iDrawMouseFlag)//--
        {
            m_iDrawMouseFlag &= ~0x0200;
            m_iDrawMouseFlag |= 0x0100;
// 
//             double x1, y1, x2, y2;
// 
//             x1 = m_rtSelectField.Left;
//             y1 = m_rtSelectField.Top;
//             x2 = m_rtSelectField.Right;
//             y2 = m_rtSelectField.Bottom;
//             m_pTheMarkDoc->ResiseSel(x1, y1, x2, y2);
//             FField_t selField;
//             m_rtSelectField = m_pTheMarkDoc->GetSelField();
//             m_rtSelRangeOrg = m_rtSelectField;
// 
//             //优化问题：选中图形后，出现图形属性，再次点击选中图形时，属性消失
//             if (m_pTheMarkDoc->GetObjSelCount() > 1)
//                 CMsgRouter::Instance()->PostMessage(this->m_iID, WM_TETE_SEL_DOC, (uint32_t)((void*)m_pTheMarkDoc));
        }
        break;
    case MOUSE_SIZEALL:   //--拖拽
        if (0x0200 & m_iDrawMouseFlag)//--
        {
   //         double x, y;
   //         m_iDrawMouseFlag &= ~0x0200;
   //         m_iDrawMouseFlag |= 0x0100;
   //         //--将选中对象移动到指定位置
   //         x = m_rtSelField.Left;
   //         y = m_rtSelField.Top;

			//double doffsetX, doffsetY;
			//doffsetX = m_LastPoint.x - m_PrevPoint.x;
			//doffsetY = m_LastPoint.y - m_PrevPoint.y;
			//size_t nGroupSize = m_pTheMapDoc->vWaferGroups.size();
			//if (nGroupSize < 1)
			//{
			//	return;
			//}
			//for (int i = 0; i < nGroupSize; i++)
			//{
			//	CWaferGroup *pWaferGroup = m_pTheMapDoc->vWaferGroups[i];
			//	size_t nWaferSize = pWaferGroup->vWaferMetas.size();
			//	if (nWaferSize < 1)
			//	{
			//		continue;
			//	}
			//	for (int j = 0; j < nWaferSize; j++)
			//	{
			//		//if ((pWaferGroup->vWaferMetas[j].nFlag&0x0004) == 0x0004)
			//		if (pWaferGroup->vWaferMetas[j].IsSel())
			//		{
			//			pWaferGroup->vWaferMetas[j].xDim.Repos(doffsetX, doffsetY);
			//		}
			//	}
			//}
			//m_rtSelField.Top = 0;
			//m_rtSelField.Bottom = 0;
			//m_rtSelField.Left = 0;
			//m_rtSelField.Right = 0;
			//m_pTheMapDoc->SetModified(true);
        }
        break;
    default:break;
    }
}
void CWaferMapView::doMouseSwitch(double X, double Y)
{
	double dbMouseSizeAllOffsetX = 0.0;
	double dbMouseSizeAllOffsetY = 0.0;
	FField_t fied;
	GetSelField(fied);
	if (isMutilSel())
	{
		dbMouseSizeAllOffsetX = fied.width() * 0.45;
		dbMouseSizeAllOffsetY = fied.height() * 0.45;
	}
    double dbLimit;
    m_pTheCoord->pix2mm(3, dbLimit);
//     if ((X >= fied.CenterX() - dbLimit)
//         && (X <= fied.CenterX() + dbLimit)
//         && (Y >= fied.Top - dbLimit && Y <= fied.Top + dbLimit))//--北
//     {
//         m_iDrawMouseFlag = 0x0101;//--0001
//         m_iMouseFlag = MOUSE_SIZENS;
//         SetClassLongPtr(this->m_hWnd, GCLP_HCURSOR, (long)LoadCursor(NULL, IDC_SIZENS));
//     }
//     else if ((X >= (fied.Left + fied.Right) / 2 - dbLimit)
//         && (X <= (fied.Left + fied.Right) / 2 + dbLimit)
//         && (Y >= fied.Bottom - dbLimit)
//         && (Y <= fied.Bottom + dbLimit))//--南
//     {
//         m_iDrawMouseFlag = 0x0102;//--0010
//         m_iMouseFlag = MOUSE_SIZENS;
//         SetClassLongPtr(this->m_hWnd, GCLP_HCURSOR, (long)LoadCursor(NULL, IDC_SIZENS));
//     }
//     else if ((X >= fied.Right - dbLimit)
//         && (X <= fied.Right + dbLimit)
//         && (Y >= fied.Top - dbLimit)
//         && (Y <= fied.Top + dbLimit))//--东北
//     {
//         m_iDrawMouseFlag = 0x0109;//--1001
//         m_iMouseFlag = MOUSE_SIZENESW;
//         SetClassLongPtr(this->m_hWnd, GCLP_HCURSOR, (long)LoadCursor(NULL, IDC_SIZENESW));
//     }
//     else if ((X >= fied.Left - dbLimit)
//         && (X <= fied.Left + dbLimit)
//         && (Y >= fied.Bottom - dbLimit)
//         && (Y <= fied.Bottom + dbLimit))//--西南
//     {
//         m_iDrawMouseFlag = 0x0106;//--0110
//         m_iMouseFlag = MOUSE_SIZENESW;
//         SetClassLongPtr(this->m_hWnd, GCLP_HCURSOR, (long)LoadCursor(NULL, IDC_SIZENESW));
//     }
//     else if ((Y >= (fied.Top + fied.Bottom) / 2 - dbLimit)
//         && (Y <= (fied.Top + fied.Bottom) / 2 + dbLimit)
//         && (X >= fied.Left - dbLimit)
//         && (X <= fied.Left + dbLimit))//--西
//     {
//         m_iDrawMouseFlag = 0x0104;//--0100
//         m_iMouseFlag = MOUSE_SIZEWE;
//         SetClassLongPtr(this->m_hWnd, GCLP_HCURSOR, (long)LoadCursor(NULL, IDC_SIZEWE));
//     }
//     else if ((Y >= (fied.Top + fied.Bottom) / 2 - dbLimit)
//         && (Y <= (fied.Top + fied.Bottom) / 2 + dbLimit)
//         && (X >= fied.Right - dbLimit)
//         && (X <= fied.Right + dbLimit))//--东
//     {
//         m_iDrawMouseFlag = 0x0108;//--1000
//         m_iMouseFlag = MOUSE_SIZEWE;
//         SetClassLongPtr(this->m_hWnd, GCLP_HCURSOR, (long)LoadCursor(NULL, IDC_SIZEWE));
//     }
//     else if ((X >= fied.Right - dbLimit)
//         && (X <= fied.Right + dbLimit)
//         && (Y >= fied.Bottom - dbLimit)
//         && (Y <= fied.Bottom + dbLimit))//--东南
//     {
//         m_iDrawMouseFlag = 0x010A;//--1010
//         m_iMouseFlag = MOUSE_SIZENWSE;
//         SetClassLongPtr(this->m_hWnd, GCLP_HCURSOR, (long)LoadCursor(NULL, IDC_SIZENWSE));
//     }
//     else if ((X >= fied.Left - dbLimit)
//         && (X <= fied.Left + dbLimit)
//         && (Y >= fied.Top - dbLimit)
//         && (Y <= fied.Top + dbLimit))//--西北
//     {
//         m_iDrawMouseFlag = 0x0105;//--0101
//         m_iMouseFlag = MOUSE_SIZENWSE;
//         SetClassLongPtr(this->m_hWnd, GCLP_HCURSOR, (long)LoadCursor(NULL, IDC_SIZENWSE));
//     }
	 if ((X >= fied.Left + dbLimit + dbMouseSizeAllOffsetX)
		&& (X <= fied.Right - dbLimit - dbMouseSizeAllOffsetX)
		&& (Y <= fied.Top - dbLimit - dbMouseSizeAllOffsetY)
		&& (Y >= fied.Bottom + dbLimit + dbMouseSizeAllOffsetY))//--中间
	{
        m_iDrawMouseFlag = 0x010F;//--1111
        m_iMouseFlag = MOUSE_SIZEALL;
        SetClassLongPtr(this->m_hWnd, GCLP_HCURSOR, (long)LoadCursor(NULL, IDC_SIZEALL));
    }
    else
    {
        m_iDrawMouseFlag = 0x0100;
        m_iMouseFlag = MOUSE_ARROW;
        SetClassLongPtr(this->m_hWnd, GCLP_HCURSOR, (long)LoadCursor(NULL, IDC_ARROW));
    }
}
void CWaferMapView::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    if (NULL == m_pTheMapDoc)
    {
        CWnd::OnLButtonDown(nFlags, point);
        return;
    }

    SetCapture();
    pix2mm(point, m_PrevPoint);
    m_LastPoint = m_PrevPoint;
    //--HCURSOR hCursor;
    switch (m_emDrawType)
    {
    case emGraph_Arrow:
        OnGraphArrowDown(nFlags, point);
        break;
    case emGraph_Hand:
        m_iDrawMouseFlag = 1;
        break;
    case emGraph_ZoomIn:
        break;
    case emGraph_ZoomOut:
        break;
    case emGraph_Whole:
        break;
    case emGraph_Part:
        break;
   
    default:
        m_iDrawMouseFlag = 0x0000;
        m_iMouseFlag = MOUSE_ARROW;
        m_emDrawType = emGraph_Arrow;
        break;
    }
    CWnd::OnLButtonDown(nFlags, point);
}
void CWaferMapView::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    if (NULL == m_pTheMapDoc)
    {
        CWnd::OnMouseMove(nFlags, point);
        return;
    }

    m_ptMouseMove = point;
    pix2mm(point, m_MousePoint);
    this->SetFocus();

    switch (m_emDrawType)
    {
    case emGraph_Arrow:
        OnGraphArrowMove(nFlags, point);
        break;
    case emGraph_Hand:
		// 画布功能
	{
		if (GetCapture() == this) {
			double dx = m_MousePoint.x - m_PrevPoint.x;
			double dy = m_MousePoint.y - m_PrevPoint.y;
			m_pTheCoord->OffsetViewOrg(-dx, -dy);
			pix2mm(point, m_MousePoint);
			m_PrevPoint = m_MousePoint;
		}
	}
        break;
    case emGraph_ZoomIn:
        break;
    case emGraph_ZoomOut:
        break;
    case emGraph_Whole:
        break;
    case emGraph_Part:
        break;
   
    default:break;
    }

    Invalidate(false);
   // m_pParentWnd->Invalidate(false);

    CWnd::OnMouseMove(nFlags, point);
}
void CWaferMapView::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    if (NULL == m_pTheMapDoc)
    {	
        CWnd::OnLButtonUp(nFlags, point);
        return ;
    }
    ReleaseCapture();

    switch (m_emDrawType)
    {
    case emGraph_Arrow:
        OnGraphArrowUp(nFlags, point);
        break;
    case emGraph_Hand:
        /*m_dbCenterX0 = m_dbCenterX;
        m_dbCenterY0 = m_dbCenterY;*/
        m_iDrawMouseFlag = 0;
        break;
    case emGraph_ZoomIn:
        break;
    case emGraph_ZoomOut:
        break;
    case emGraph_Whole:
        break;
    case emGraph_Part:
        break;
    
    default:
        m_iDrawMouseFlag = 0x0000;
        m_iMouseFlag = MOUSE_ARROW;
        m_emDrawType = emGraph_Arrow;
        break;
    }

    Invalidate(false);

    CWnd::OnLButtonUp(nFlags, point);
}
BOOL CWaferMapView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    if (NULL == m_pTheMapDoc)
        return CWnd::OnMouseWheel(nFlags, zDelta, pt);
    double mx0(0), my0(0);//--
	double mx1(0), my1(0);
	m_pTheCoord->pix2mm(0, 0, mx0, my0); 
	m_pTheCoord->pix2mm(10, 10, mx1, my1);
	
// 	if ((mx1 - mx0) / 10 < 0.05 || (mx1 - mx0) / 10 > 2.0)
// 	{
// 		return CWnd::OnMouseWheel(nFlags, zDelta, pt);
// 	}

    m_pTheCoord->pix2mm(m_ptMouseMove.x, m_ptMouseMove.y, mx0, my0);//--光标所在点坐标

    if (zDelta > 0)
    {
		//if ((mx1 - mx0) / 10 < 2.0)
		{
			m_pTheCoord->ZoomOut();
		}
    }
    else
	{
		//if ((mx1 - mx0) / 10 > 0.05)
		{
			m_pTheCoord->ZoomIn();
		}
    }
    
    m_pTheCoord->pix2mm(m_ptMouseMove.x, m_ptMouseMove.y, mx1, my1);
    m_pTheCoord->OffsetViewOrg(mx0 - mx1, my0 - my1);

    Invalidate(false);
    m_pParentWnd->Invalidate(false);

    return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}
void CWaferMapView::OnRButtonDown(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    switch (m_emDrawType)
    {
    case emGraph_Arrow:  //--箭头
	{
		m_LastPoint.x = point.x;
		m_LastPoint.y = point.y;

		if (!m_bPopMenu) return;

		if (m_pTheMapDoc->GetMapObjSelCount() == 0) return;

		m_thePopupMenu.CreatePopupMenu();
		//弹出菜单中追加当前图层（笔）项目
		UINT uID = IDM_MAPADJUST_CHANGEWAFER;
		CWaferGraphDoc * pWaferDoc = CWaferEngine::Instance()->GetGraphDoc();
		int nWaferCount = pWaferDoc->GetWaferCount();
		if (1 == nWaferCount)
		{
			m_thePopupMenu.AppendMenu(MF_ENABLED, uID, _T("BIN_0"));
			m_thePopupMenu.AppendMenu(MF_ENABLED, uID + 1, _T("BIN_1"));
		}
		else
		{
			for (int i = 0; i < nWaferCount; i++)
			{
				CWaferGraph *pWafer = pWaferDoc->GetWafer(i);
				if (nullptr == pWafer) continue;
				m_thePopupMenu.AppendMenu(MF_ENABLED, uID + i, CString(pWafer->GetName()));
			}
		}


		// 判断选中的对象都是什么模块
		for (auto & item : m_pTheMapDoc->vWaferGroups)
		{
			for (auto & it : item->vWaferMetas)
			{
				if (it.IsSel())
				{
					int nWaferID = it.GetWaferID();
					m_thePopupMenu.CheckMenuItem(IDM_MAPADJUST_CHANGEWAFER + nWaferID, MF_CHECKED);
				}
			}
		}

		POINT curPos;
		GetCursorPos(&curPos);
		m_thePopupMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, curPos.x, curPos.y, this);
		m_thePopupMenu.DestroyMenu();

		break;
	}

    }
 
    CWnd::OnRButtonDown(nFlags, point);
}
void CWaferMapView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
	return; 

    if (NULL == m_pTheMapDoc)
    {
        CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
        return;
    }
 
    CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CWaferMapView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认


    __super::OnChar(nChar, nRepCnt, nFlags);
}
void CWaferMapView::OnKillFocus(CWnd* pNewWnd)
{
    CWnd::OnKillFocus(pNewWnd);

    // TODO: 在此处添加消息处理程序代码
}
void CWaferMapView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    
	// TODO: 在此添加消息处理程序代码和/或调用默认值


    CWnd::OnLButtonDblClk(nFlags, point);
}

void CWaferMapView::doRotate()
{
	
	
}
void CWaferMapView::doSelGroup()
{
	size_t nGroupSize = m_pTheMapDoc->vWaferGroups.size();
	if (nGroupSize < 1)
	{
		return;
	}
	for (int i = 0; i < nGroupSize; i++)
	{
		CWaferGroup *pWaferGroup = m_pTheMapDoc->vWaferGroups[i];
		size_t nWaferSize = pWaferGroup->vWaferMetas.size();
		if (nWaferSize < 1)
		{
			continue;
		}
		for (int j = 0; j < nWaferSize; j++)
		{
			if ((pWaferGroup->vWaferMetas[j].IsSel()))
			{
				for (int k = 0; k < pWaferGroup->vWaferMetas.size();k++)
				{
					pWaferGroup->vWaferMetas[k].EnableSel(true);
				}
				break;
			}
		}
	}
	m_iDrawMouseFlag = 0x0100;
	m_iMouseFlag = MOUSE_ARROW;
	m_emDrawType = emGraph_Arrow;

}

void CWaferMapView::doSelectAll()
{
	for (auto & item : m_pTheMapDoc->vWaferGroups)
	{
		for (auto & it : item->vWaferMetas)
		{
			it.EnableSel(true);
		}
	}

	m_iDrawMouseFlag = 0x0100;
	m_iMouseFlag = MOUSE_ARROW;
	m_emDrawType = emGraph_Arrow;
	Invalidate(false);
}

void CWaferMapView::doDelete()
{
	size_t nGroupSize = m_pTheMapDoc->vWaferGroups.size();
	if (nGroupSize < 1)
	{
		return;
	}
	for (int i = 0; i < nGroupSize; i++)
	{
		CWaferGroup *pWaferGroup = m_pTheMapDoc->vWaferGroups[i];
		size_t nWaferSize = pWaferGroup->vWaferMetas.size();
		if (nWaferSize < 1)
		{
			continue;
		}
		for (int j = 0; j < nWaferSize; j++)
		{
			//if ((pWaferGroup->vWaferMetas[j].IsSel()))
			{
				pWaferGroup->vWaferMetas.erase(pWaferGroup->vWaferMetas.begin() + j);
				nWaferSize -= 1;
				j -= 1;
				if (nWaferSize == 0)
				{
					delete m_pTheMapDoc->vWaferGroups[i];
					m_pTheMapDoc->vWaferGroups.erase(m_pTheMapDoc->vWaferGroups.begin() + i);
					nGroupSize -= 1;
					i -= 1;
				}
			}
		}
	}
	m_iDrawMouseFlag = 0x0100;
	m_iMouseFlag = MOUSE_ARROW;
	m_emDrawType = emGraph_Arrow;
	Invalidate(FALSE);
	GetParent()->PostMessageW(WM_TETE_WAFERMAP_ADDMAP);

	m_pTheMapDoc->SetModified(true);
}

void CWaferMapView::doChangePos()
{
	if (NULL == m_pTheMapDoc)
		return;
	m_pTheMapDoc->ShowPosWnd();

	m_pTheMapDoc->SetModified();
	Invalidate(FALSE);
}

void CWaferMapView::doZoomAll()
{
	if (NULL == m_pTheCoord)
		return;
	m_pTheCoord->ZoomAll();
	Invalidate(FALSE);
}

void CWaferMapView::doZoomIn(double dbZoom)
{
	if (NULL == m_pTheCoord)
		return;
	m_pTheCoord->ZoomIn(dbZoom);
	Invalidate(FALSE);
}

void CWaferMapView::doZoomOut(double dbZoom)
{
	if (NULL == m_pTheCoord)
		return;
	m_pTheCoord->ZoomOut(dbZoom);
	Invalidate(FALSE);
}

void CWaferMapView::doChangeStatus()
{
	if (GetObjSelCount() < 1)
		return;

	size_t nGroupSize = m_pTheMapDoc->vWaferGroups.size();
	if (nGroupSize < 1)
		return;
	
	for (int i = 0; i < nGroupSize; i++)
	{
		CWaferGroup *pWaferGroup = m_pTheMapDoc->vWaferGroups[i];
		size_t nWaferSize = pWaferGroup->vWaferMetas.size();
		if (nWaferSize < 1)
		{
			continue;
		}
		for (int j = 0; j < nWaferSize; j++)
		{
			//if (0x0005 == (pWaferGroup->vWaferMetas[j].nFlag & 0x0005))
			if (pWaferGroup->vWaferMetas[j].IsSel() && pWaferGroup->vWaferMetas[j].IsMarkable())
			{
				//pWaferGroup->vWaferMetas[j].nFlag &= ~0x0001;
				pWaferGroup->vWaferMetas[j].EnableMark(false);
			}
			//else if (0x0004 == (pWaferGroup->vWaferMetas[j].nFlag & 0x0004))
			else if (pWaferGroup->vWaferMetas[j].IsSel() && !pWaferGroup->vWaferMetas[j].IsMarkable())
			{
				//pWaferGroup->vWaferMetas[j].nFlag |= 0x0001;
				pWaferGroup->vWaferMetas[j].EnableMark(true);
			}
		}
	}
	m_iDrawMouseFlag = 0x0100;
	m_iMouseFlag = MOUSE_ARROW;
	m_emDrawType = emGraph_Arrow;

	m_pTheMapDoc->SetModified(true);
}

void CWaferMapView::SetSelPrint(bool b)
{
	if (GetObjSelCount() < 1)
		return;

	size_t nGroupSize = m_pTheMapDoc->vWaferGroups.size();
	if (nGroupSize < 1)
		return;

	for (int i = 0; i < nGroupSize; i++)
	{
		CWaferGroup *pWaferGroup = m_pTheMapDoc->vWaferGroups[i];
		size_t nWaferSize = pWaferGroup->vWaferMetas.size();
		if (nWaferSize < 1)
		{
			continue;
		}
		for (int j = 0; j < nWaferSize; j++)
		{
			if (pWaferGroup->vWaferMetas[j].IsSel())
			{
				pWaferGroup->vWaferMetas[j].EnableMark(b);
			}
		}
	}
	m_iDrawMouseFlag = 0x0100;
	m_iMouseFlag = MOUSE_ARROW;
	m_emDrawType = emGraph_Arrow;
}

void CWaferMapView::SetAllPrint(bool b)
{
	size_t nGroupSize = m_pTheMapDoc->vWaferGroups.size();
	if (nGroupSize < 1)
		return;

	for (int i = 0; i < nGroupSize; i++)
	{
		CWaferGroup *pWaferGroup = m_pTheMapDoc->vWaferGroups[i];
		size_t nWaferSize = pWaferGroup->vWaferMetas.size();
		if (nWaferSize < 1)
		{
			continue;
		}
		for (int j = 0; j < nWaferSize; j++)
		{
			pWaferGroup->vWaferMetas[j].EnableMark(b);
		}
	}
	m_iDrawMouseFlag = 0x0100;
	m_iMouseFlag = MOUSE_ARROW;
	m_emDrawType = emGraph_Arrow;
}

void CWaferMapView::doManualAutoAllotWork(int iHead)
{
	size_t nGroupSize = m_pTheMapDoc->vWaferGroups.size();
	vector<xDim_t> vDimTmp;
	FField_t FieldTmp, MetaFieleTmp;
	if (nGroupSize < 1)
	{
		return;
	}
	bool bInHead = true;
	GetSelField(FieldTmp);
	for (int i = 0; i < nGroupSize; i++)
	{
		CWaferGroup *pWaferGroup = m_pTheMapDoc->vWaferGroups[i];
		int iMetaSize = pWaferGroup->vWaferMetas.size();
		for (size_t j = 0; j < iMetaSize; j++)
		{
			if (pWaferGroup->vWaferMetas[j].IsSel())
			{
				if (pWaferGroup->vWaferMetas[j].GetHead() == 0)
				{
					continue;
				}
				else if (pWaferGroup->vWaferMetas[j].GetHead() == 3)
				{
					pWaferGroup->vWaferMetas[j].SetHead(iHead + 1);
				}
				else if (pWaferGroup->vWaferMetas[j].GetHead() == 1)
				{
					if (iHead == 0)
					{
						continue;
					}
					else
					{
						for (int k = 0; k < 3; k++)
						{
							if (!m_pCoordTrans->IsInHead(1, pWaferGroup->vWaferMetas[j].xDim.dimPoints1[k].x, pWaferGroup->vWaferMetas[j].xDim.dimPoints1[k].y))
							{
								bInHead = false;
								break;
							}
						}
						if (!m_pCoordTrans->IsInHead(1, pWaferGroup->vWaferMetas[j].xDim.dimPoints1[2].x - pWaferGroup->vWaferMetas[j].xDim.dimPoints1[0].x + pWaferGroup->vWaferMetas[j].xDim.dimPoints1[1].x, pWaferGroup->vWaferMetas[j].xDim.dimPoints1[2].y - pWaferGroup->vWaferMetas[j].xDim.dimPoints1[0].y + pWaferGroup->vWaferMetas[j].xDim.dimPoints1[1].y))
							continue;
						if (bInHead)
						{
							pWaferGroup->vWaferMetas[j].SetHead(iHead + 1);
						}
						bInHead = true;
					}
				}
				else
				{
					if (iHead == 0)
					{
						for (int k = 0; k < 3; k++)
						{
							if (!m_pCoordTrans->IsInHead(0, pWaferGroup->vWaferMetas[j].xDim.dimPoints1[k].x, pWaferGroup->vWaferMetas[j].xDim.dimPoints1[k].y))
							{
								bInHead = false;
								break;
							}
						}
						if (!m_pCoordTrans->IsInHead(0, pWaferGroup->vWaferMetas[j].xDim.dimPoints1[2].x - pWaferGroup->vWaferMetas[j].xDim.dimPoints1[0].x + pWaferGroup->vWaferMetas[j].xDim.dimPoints1[1].x, pWaferGroup->vWaferMetas[j].xDim.dimPoints1[2].y - pWaferGroup->vWaferMetas[j].xDim.dimPoints1[0].y + pWaferGroup->vWaferMetas[j].xDim.dimPoints1[1].y))
							continue;
						if (bInHead)
						{
							pWaferGroup->vWaferMetas[j].SetHead(iHead + 1);
						}
						bInHead = true;
					}
					else
					{
						continue;
					}
				}
			}
		}
	}
	m_iDrawMouseFlag = 0x0100;
	m_iMouseFlag = MOUSE_ARROW;
	m_emDrawType = emGraph_Arrow;
	m_pTheMapDoc->SetModified(true);
}

void CWaferMapView::doChangeWafer(int nWaferID)
{
	if (0 == GetObjSelCount())
		return;

	for (auto & item : m_pTheMapDoc->vWaferGroups)
	{
		for (auto & it : item->vWaferMetas)
		{
			if (it.IsSel())
				it.SetWaferID(nWaferID);
		}
	}

	m_pTheMapDoc->SetModified();
}

BOOL CWaferMapView::OnCommand(WPARAM wParam, LPARAM lParam)
{
    // TODO: 在此添加专用代码和/或调用基类
    UINT uMsg = LOWORD(wParam);
    CString sMsg;
    switch (uMsg)
    {
	default:
		doChangeWafer(uMsg - IDM_MAPADJUST_CHANGEWAFER);
		break;
    }

    return __super::OnCommand(wParam, lParam);
}
void CWaferMapView::doSelectObject(double px, double py)
{
    int iSelCnt = 0;
    FField_t field;
	
	size_t nGroupSize = m_pTheMapDoc->vWaferGroups.size();
	CWaferGroup *pTmpWaferGroup = new CWaferGroup;
	if (nGroupSize < 1)
	{
		m_iDrawMouseFlag = 0x0000;
		return;
	}
	for (int i = 0; i < nGroupSize; i++)
	{
		CWaferGroup *pWaferGroup = m_pTheMapDoc->vWaferGroups[i];
		size_t nWaferSize = pWaferGroup->vWaferMetas.size();
		if (nWaferSize < 1)
		{
			continue;
		}
		for (int j = 0; j < nWaferSize; j++)
		{
			if (pWaferGroup->vWaferMetas[j].xDim.IsInside(px,py))
			{
				if (pWaferGroup->vWaferMetas[j].IsSel())
				{
					pWaferGroup->vWaferMetas[j].EnableSel(false);
				}
				else
				{
					pWaferGroup->vWaferMetas[j].EnableSel(true);
					iSelCnt++;
					break;
				}
			}
		}
		if (iSelCnt >= 1 )
		{
			break;
		}
		else
		{
			// 判断是否是选中整行或整列
			if (SelectAllRowOrCol(pWaferGroup, px, py))
			{
				iSelCnt = 1;
				break;
			}
		}
	}

	if (1 == iSelCnt)
	{
		m_iDrawMouseFlag = 0x0100;
	}
    else
    {
        m_iDrawMouseFlag = 0x0000;
    }
}

void CWaferMapView::doSelectObject(double x1, double y1, double x2, double y2)
{
    double t;
    if (x1>x2)
    {
        t = x1; x1 = x2; x2 = t;
    }
    if (y1<y2)
    {
        t = y1; y1 = y2; y2 = t;
    }
    int iSelCnt = 0;
    IGraphObject *pTheObject = NULL;
    FField_t field(x1,y1,fabs(x1-x2),fabs(y1-y2));
	size_t nGroupSize = m_pTheMapDoc->vWaferGroups.size();
	if (nGroupSize < 1)
	{
		m_iDrawMouseFlag = 0x0000;
		return;
	}
	for (int i = 0; i < nGroupSize; i++)
	{
		CWaferGroup *pWaferGroup = m_pTheMapDoc->vWaferGroups[i];
		size_t nWaferSize = pWaferGroup->vWaferMetas.size();
		if (nWaferSize < 1)
		{
			continue;
		}
		for (int j = 0; j < nWaferSize; j++)
		{
			if (pWaferGroup->vWaferMetas[j].xDim.IsInside(field))
			{
				if (!pWaferGroup->vWaferMetas[j].IsSel())
				{
					pWaferGroup->vWaferMetas[j].EnableSel(true);
				}
				iSelCnt++;
			}
		}
	}
    if(iSelCnt>0)
    {
		m_iDrawMouseFlag = 0x0100;
		//GetParent()->GetParent()->PostMessage(WM_TETE_MAPSET_CELLINFO);
    }
    else
    {
        m_iDrawMouseFlag = 0x0000;
    }
}
void CWaferMapView::UnSelectAllObjects()
{
	size_t nGroupSize = m_pTheMapDoc->vWaferGroups.size();
	if (nGroupSize < 1)
	{
		return;
	}
	for (int i = 0; i < nGroupSize; i++)
	{
		CWaferGroup *pWaferGroup = m_pTheMapDoc->vWaferGroups[i];
		size_t nWaferSize = pWaferGroup->vWaferMetas.size();
		if (nWaferSize < 1)
		{
			continue;
		}
		for (int j = 0; j < nWaferSize; j++)
		{
			//if ((pWaferGroup->vWaferMetas[j].nFlag & 0x0004) == 0x0004)
			if (pWaferGroup->vWaferMetas[j].IsSel())
			{
				//pWaferGroup->vWaferMetas[j].nFlag &= ~0x0004;
				pWaferGroup->vWaferMetas[j].EnableSel(false);
			}
		}
	}
}
//按下某个虚拟键时， 绘制垂直或水平的直线
void CWaferMapView::changeLineToVH(DPoint_t& dps, DPoint_t& dpe, int nVirtKey/* = VK_CONTROL*/)
{
	if (::GetKeyState(nVirtKey) < 0)
	{
		if (D_EQUAL(dpe.x, dps.x) || D_EQUAL(dpe.y, dps.y)) return;
		// 判断直线的斜率
		double k = (dpe.y - dps.y) / (dpe.x - dps.x);
		if (fabs(k) < 1)
			dpe.y = dps.y;
		else
			dpe.x = dps.x;
	}
}
//按下某个虚拟键时， 绘制正方形
void CWaferMapView::changeRectToSquare(DPoint_t& dLT, DPoint_t& dRB, int nVirtKey/* = VK_CONTROL*/)
{
	if (::GetKeyState(nVirtKey) < 0)
	{
		double dW = fabs(dLT.x - dRB.x);
		double dH = fabs(dLT.y - dRB.y);
		if (D_EQUAL(dW, dH)) return;
		if (dW > dH)
			dRB.y = dLT.y + ((dRB.y > dLT.y) ? (1) : (-1)) * dW;
		else
			dRB.x = dLT.x + ((dRB.x > dLT.x) ? (1) : (-1)) * dH;
	}
}
// 判断是否多选
bool CWaferMapView::isMutilSel()
{
	return (::GetKeyState(VK_SHIFT) < 0 || ::GetKeyState(VK_CONTROL) < 0);
}
/**
* @功能 通过中心点和起始点计算出一个正多边形
*/
void CWaferMapView::getPolygonInfo(int nSides, const DPoint_t& dCenter, const DPoint_t& dStart, std::vector<DPoint_t>& vdpts)
{
	if (nSides < 3 || (dCenter == dStart)) return;

	// 起始点与中心点连成的直线与X轴的夹角
	double dx = dStart.x - dCenter.x;
	double dy = dStart.y - dCenter.y;
	double dLen = sqrt(dx * dx + dy * dy);
	double k;
	double dRad;
	if (D_EQUAL(dx, 0) && dy > 0)
	{
		dRad = TETE_PI / 2;
	}
	else if (D_EQUAL(dx, 0) && dy < 0)
	{
		dRad = 3 * TETE_PI / 2;
	}
	else
	{
		k = dy / dx;
		dRad = atan(k);
	}
	dRad = (dx < 0) ? (dRad + TETE_PI) : dRad;


	// 根据正多变形的边数，计算每个点的坐标
	vdpts.push_back(dStart);
	double dScale = TETE_2PI / nSides;
	for (int i = 1; i < nSides; i++)
	{
		double db = dRad + i * dScale;
		DPoint_t dp;
		dp.x = dLen * cos(db);
		dp.y = dLen * sin(db);
		dp += dCenter;
		vdpts.push_back(dp);
	}
	vdpts.push_back(dStart);
}
int CWaferMapView::GetObjSelCount()
{
	size_t nGroupSize = m_pTheMapDoc->vWaferGroups.size();
	int nSelMate = 0;
	if (nGroupSize < 1)
	{
		return 0;
	}
	m_vWaferMeta.clear();
	for (int i = 0; i < nGroupSize; i++)
	{
		CWaferGroup *pWaferGroup = m_pTheMapDoc->vWaferGroups[i];
		size_t nWaferSize = pWaferGroup->vWaferMetas.size();
		if (nWaferSize < 1)
		{
			continue;
		}
		for (int j = 0; j < nWaferSize; j++)
		{
			//if ((pWaferGroup->vWaferMetas[j].nFlag & 0x0004) == 0x0004)
			if (pWaferGroup->vWaferMetas[j].IsSel())
			{
				nSelMate += 1;
				m_vWaferMeta.push_back(pWaferGroup->vWaferMetas[j]);
			}
		}
	}
	return nSelMate;
}

int CWaferMapView::GetObjMarkCount()
{
	size_t nGroupSize = m_pTheMapDoc->vWaferGroups.size();
	int nSelMate = 0;
	if (nGroupSize < 1)
	{
		return 0;
	}
	for (int i = 0; i < nGroupSize; i++)
	{
		CWaferGroup *pWaferGroup = m_pTheMapDoc->vWaferGroups[i];
		size_t nWaferSize = pWaferGroup->vWaferMetas.size();
		if (nWaferSize < 1)
		{
			continue;
		}
		for (int j = 0; j < nWaferSize; j++)
		{
			if (pWaferGroup->vWaferMetas[j].IsMarkable())
			{
				nSelMate += 1;
			}
		}
	}
	return nSelMate;
}

void CWaferMapView::GetSelField(FField_t &field)
{
	vector<FField_t> vfield;
	FField_t Tmpfield;
	if (m_vWaferMeta.size()<=0)
	{
		return;
	}
	for (int i = 0; i < m_vWaferMeta.size();i++)
	{
		m_vWaferMeta[i].xDim.GetField(Tmpfield);
		vfield.push_back(Tmpfield);
	}

	field.Top = -88888;
	field.Bottom = 88888;
	field.Left = 88888;
	field.Right = -88888;
	for (int i = 0; i < m_vWaferMeta.size();i++)
	{
		if (field.Top < vfield[i].Top)
		{
			field.Top = vfield[i].Top;
		}
		if (field.Bottom > vfield[i].Bottom)
		{
			field.Bottom = vfield[i].Bottom;
		}
		if (field.Left > vfield[i].Left)
		{
			field.Left = vfield[i].Left;
		}
		if (field.Right < vfield[i].Right)
		{
			field.Right = vfield[i].Right;
		}
	}
}
void CWaferMapView::ChangeSize()
{
	size_t nGroupSize = m_pTheMapDoc->vWaferGroups.size();
	
	int nSelMate = 0;
	if (nGroupSize < 1)
	{
		return;
	}
	for (int i = 0; i < nGroupSize; i++)
	{
		//CWaferGroup *pWaferGroup = m_pTheMapDoc->vWaferGroups[i];
		//if (fabs(pWaferGroup->dbHeight - pWaferGroup->pUnitMeta->GetHeight()) < 0.0000005 && fabs(pWaferGroup->dbWidth - pWaferGroup->pUnitMeta->GetWidth()) < 0.00000005)
		//{
		//	continue;
		//}
		//size_t nWaferSize = pWaferGroup->vWaferMetas.size();
		//if (nWaferSize < 1)
		//{
		//	continue;
		//}
		//for (int j = 0; j < nWaferSize; j++)
		//{
		//	pWaferGroup->vWaferMetas[j].xDim.Zoom(pWaferGroup->dbHeight, pWaferGroup->dbWidth, pWaferGroup->pUnitMeta->GetHeight(), pWaferGroup->pUnitMeta->GetWidth());
		//}
		//pWaferGroup->dbHeight = pWaferGroup->pUnitMeta->GetHeight();
		//pWaferGroup->dbWidth = pWaferGroup->pUnitMeta->GetWidth();
	}
	Invalidate(FALSE);
}
void CWaferMapView::DeleteAll()
{
	m_pTheMapDoc->RemoveAllGroup();
	m_pTheMapDoc->SetModified(true);
}
void CWaferMapView::OnAlignHorleft()
{
	size_t nGroupSize = m_pTheMapDoc->vWaferGroups.size();
	vector<xDim_t> vDimTmp;
	FField_t FieldTmp, MetaFieleTmp;
	if (nGroupSize < 1)
	{
		return;
	}
	GetSelField(FieldTmp);
	for (int i = 0; i < nGroupSize; i++)
	{
		CWaferGroup *pWaferGroup = m_pTheMapDoc->vWaferGroups[i];
		int iMetaSize = pWaferGroup->vWaferMetas.size();
		for (int j = 0; j < iMetaSize; j++)
		{
			//if (pWaferGroup->vWaferMetas[j].nFlag & 0x0004)
			if (pWaferGroup->vWaferMetas[j].IsSel())
			{
				pWaferGroup->vWaferMetas[j].xDim.GetField(MetaFieleTmp);
				pWaferGroup->vWaferMetas[j].xDim.Repos(FieldTmp.Left - MetaFieleTmp.Left,0);
			}
		}
	}
	m_pTheMapDoc->SetModified(true);
}
void CWaferMapView::OnAlignHorcenter()
{
	size_t nGroupSize = m_pTheMapDoc->vWaferGroups.size();
	vector<xDim_t> vDimTmp;
	FField_t FieldTmp, MetaFieleTmp;
	if (nGroupSize < 1)
	{
		return;
	}
	GetSelField(FieldTmp);
	for (int i = 0; i < nGroupSize; i++)
	{
		CWaferGroup *pWaferGroup = m_pTheMapDoc->vWaferGroups[i];
        size_t iMetaSize = pWaferGroup->vWaferMetas.size();
		for (size_t j = 0; j < iMetaSize; j++)
		{
			//if (pWaferGroup->vWaferMetas[j].nFlag & 0x0004)
			if (pWaferGroup->vWaferMetas[j].IsSel())
			{
				pWaferGroup->vWaferMetas[j].xDim.GetField(MetaFieleTmp);
				pWaferGroup->vWaferMetas[j].xDim.Repos((FieldTmp.Left + FieldTmp.Right)/2 - (MetaFieleTmp.Left + MetaFieleTmp.Right)/2, 0);
			}
		}
	}
	m_pTheMapDoc->SetModified(true);

}
void CWaferMapView::OnAlignHorright()
{
	size_t nGroupSize = m_pTheMapDoc->vWaferGroups.size();
	vector<xDim_t> vDimTmp;
	FField_t FieldTmp, MetaFieleTmp;
	if (nGroupSize < 1)
	{
		return;
	}
	GetSelField(FieldTmp);
	for (size_t i = 0; i < nGroupSize; i++)
	{
		CWaferGroup *pWaferGroup = m_pTheMapDoc->vWaferGroups[i];
		int iMetaSize = pWaferGroup->vWaferMetas.size();
		for (size_t j = 0; j < iMetaSize; j++)
		{
			//if (pWaferGroup->vWaferMetas[j].nFlag & 0x0004)
			if (pWaferGroup->vWaferMetas[j].IsSel())
			{
				pWaferGroup->vWaferMetas[j].xDim.GetField(MetaFieleTmp);
				pWaferGroup->vWaferMetas[j].xDim.Repos(FieldTmp.Right - MetaFieleTmp.Right, 0);
			}
		}
	}
	m_pTheMapDoc->SetModified(true);
	
}
void CWaferMapView::OnAlignVtop()
{
	size_t nGroupSize = m_pTheMapDoc->vWaferGroups.size();
	vector<xDim_t> vDimTmp;
	FField_t FieldTmp, MetaFieleTmp;
	if (nGroupSize < 1)
	{
		return;
	}
	GetSelField(FieldTmp);
	for (int i = 0; i < nGroupSize; i++)
	{
		CWaferGroup *pWaferGroup = m_pTheMapDoc->vWaferGroups[i];
		int iMetaSize = pWaferGroup->vWaferMetas.size();
		for (size_t j = 0; j < iMetaSize; j++)
		{
			//if (pWaferGroup->vWaferMetas[j].nFlag & 0x0004)
			if (pWaferGroup->vWaferMetas[j].IsSel())
			{
				pWaferGroup->vWaferMetas[j].xDim.GetField(MetaFieleTmp);
				pWaferGroup->vWaferMetas[j].xDim.Repos(0, FieldTmp.Top - MetaFieleTmp.Top);
			}
		}
	}
	m_pTheMapDoc->SetModified(true);
	
}
void CWaferMapView::OnAlignVcenter()
{
	size_t nGroupSize = m_pTheMapDoc->vWaferGroups.size();
	vector<xDim_t> vDimTmp;
	FField_t FieldTmp, MetaFieleTmp;
	if (nGroupSize < 1)
	{
		return;
	}
	GetSelField(FieldTmp);
	for (int i = 0; i < nGroupSize; i++)
	{
		CWaferGroup *pWaferGroup = m_pTheMapDoc->vWaferGroups[i];
		int iMetaSize = pWaferGroup->vWaferMetas.size();
		for (size_t j = 0; j < iMetaSize; j++)
		{
			//if (pWaferGroup->vWaferMetas[j].nFlag & 0x0004)
			if (pWaferGroup->vWaferMetas[j].IsSel())
			{
				pWaferGroup->vWaferMetas[j].xDim.GetField(MetaFieleTmp);
				pWaferGroup->vWaferMetas[j].xDim.Repos(0, (FieldTmp.Top + FieldTmp.Bottom) / 2 - (MetaFieleTmp.Top + MetaFieleTmp.Bottom) / 2);
			}
		}
	}
	m_pTheMapDoc->SetModified(true);
	
}
void CWaferMapView::OnAlignVbottom()
{
	size_t nGroupSize = m_pTheMapDoc->vWaferGroups.size();
	vector<xDim_t> vDimTmp;
	FField_t FieldTmp, MetaFieleTmp;
	if (nGroupSize < 1)
	{
		return;
	}
	GetSelField(FieldTmp);
	for (int i = 0; i < nGroupSize; i++)
	{
		CWaferGroup *pWaferGroup = m_pTheMapDoc->vWaferGroups[i];
		int iMetaSize = pWaferGroup->vWaferMetas.size();
		for (size_t j = 0; j < iMetaSize; j++)
		{
			//if (pWaferGroup->vWaferMetas[j].nFlag & 0x0004)
			if (pWaferGroup->vWaferMetas[j].IsSel())
			{
				pWaferGroup->vWaferMetas[j].xDim.GetField(MetaFieleTmp);
				pWaferGroup->vWaferMetas[j].xDim.Repos(0, FieldTmp.Bottom - MetaFieleTmp.Bottom);
			}
		}
	}
	m_pTheMapDoc->SetModified(true);
	
}
void CWaferMapView::OnAlignCenter()
{
	size_t nGroupSize = m_pTheMapDoc->vWaferGroups.size();
	vector<xDim_t> vDimTmp;
	FField_t FieldTmp, MetaFieleTmp;
	if (nGroupSize < 1)
	{
		return;
	}
	GetSelField(FieldTmp);
	for (int i = 0; i < nGroupSize; i++)
	{
		CWaferGroup *pWaferGroup = m_pTheMapDoc->vWaferGroups[i];
		int iMetaSize = pWaferGroup->vWaferMetas.size();
		for (size_t j = 0; j < iMetaSize; j++)
		{
			//if (pWaferGroup->vWaferMetas[j].nFlag & 0x0004)
			if (pWaferGroup->vWaferMetas[j].IsSel())
			{
				pWaferGroup->vWaferMetas[j].xDim.GetField(MetaFieleTmp);
				pWaferGroup->vWaferMetas[j].xDim.Repos(-(FieldTmp.Left + FieldTmp.Right)/2, -(FieldTmp.Top + FieldTmp.Bottom) / 2);
			}
		}
	}
	m_pTheMapDoc->SetModified(true);
}
void CWaferMapView::DoRedo()
{
	m_pTheMapDoc->LoadTmpFile(1);
	Invalidate(FALSE);
	GetParent()->PostMessageW(WM_TETE_WAFERMAP_ADDMAP);
	GetParent()->PostMessage(WM_TETE_MAPSET_BASEBOARD, (WPARAM)m_pTheMapDoc);
	m_iDrawMouseFlag = 0x0100;
	m_iMouseFlag = MOUSE_ARROW;
	m_emDrawType = emGraph_Arrow;
}
void CWaferMapView::DoUndo()
{
	m_pTheMapDoc->LoadTmpFile(-1);
	Invalidate(FALSE);
	m_iDrawMouseFlag = 0x0100;
	m_iMouseFlag = MOUSE_ARROW;
	m_emDrawType = emGraph_Arrow;
}

void CWaferMapView::SelectLeftHead()
{
	size_t nGroupSize = m_pTheMapDoc->vWaferGroups.size();
	for (int i = 0; i < nGroupSize; i++)
	{
		CWaferGroup *pWaferGroup = m_pTheMapDoc->vWaferGroups[i];
		int iMetaSize = pWaferGroup->vWaferMetas.size();
		for (size_t j = 0; j < iMetaSize; j++)
		{
			pWaferGroup->vWaferMetas[j].EnableSel(false);  // 先取消选中
			int nhead = pWaferGroup->vWaferMetas[j].GetHead();
			if (1 == nhead || 3 == nhead)
			{
				pWaferGroup->vWaferMetas[j].EnableSel(true);  // 选中
			}
		}
	}
	m_iDrawMouseFlag = 0x0100;
}

void CWaferMapView::SelectRightHead()
{
	size_t nGroupSize = m_pTheMapDoc->vWaferGroups.size();
	for (int i = 0; i < nGroupSize; i++)
	{
		CWaferGroup *pWaferGroup = m_pTheMapDoc->vWaferGroups[i];
		int iMetaSize = pWaferGroup->vWaferMetas.size();
		for (size_t j = 0; j < iMetaSize; j++)
		{
			pWaferGroup->vWaferMetas[j].EnableSel(false);  // 先取消选中
			int nhead = pWaferGroup->vWaferMetas[j].GetHead();
			if (2 == nhead)
			{
				pWaferGroup->vWaferMetas[j].EnableSel(true);  // 选中
			}
		}
	}
	m_iDrawMouseFlag = 0x0100;
}

void CWaferMapView::SelectRow(int istart, int iend)
{
	size_t nGroupSize = m_pTheMapDoc->vWaferGroups.size();
	for (int i = 0; i < nGroupSize; i++)
	{
		CWaferGroup *pWaferGroup = m_pTheMapDoc->vWaferGroups[i];
		int iMetaSize = pWaferGroup->vWaferMetas.size(); 
		int itotalrow = pWaferGroup->GetRowCount();
		for (size_t j = 0; j < iMetaSize; j++)
		{
			//pWaferGroup->vWaferMetas[j].EnableSel(false);  // 先取消选中
			int irow = itotalrow - pWaferGroup->vWaferMetas[j].nRow + 1;    // nRow是从第一行等于1， 而传入的参数为最下面一行为1
			if (irow >= istart && irow <= iend)
			{
				if (pWaferGroup->vWaferMetas[j].IsSel())
					pWaferGroup->vWaferMetas[j].EnableSel(false);
				else
					pWaferGroup->vWaferMetas[j].EnableSel(true);
			}
		}
	}
	m_iDrawMouseFlag = 0x0100;
}

void CWaferMapView::SelectCol(int istart, int iend)
{
	size_t nGroupSize = m_pTheMapDoc->vWaferGroups.size();
	for (int i = 0; i < nGroupSize; i++)
	{
		CWaferGroup *pWaferGroup = m_pTheMapDoc->vWaferGroups[i];
		int iMetaSize = pWaferGroup->vWaferMetas.size();
		for (size_t j = 0; j < iMetaSize; j++)
		{
			int icol = pWaferGroup->vWaferMetas[j].nCol;
			if (icol >= istart && icol <= iend)
			{
				if(pWaferGroup->vWaferMetas[j].IsSel())
					pWaferGroup->vWaferMetas[j].EnableSel(false); 
				else
					pWaferGroup->vWaferMetas[j].EnableSel(true);
			}
		}
	}
	m_iDrawMouseFlag = 0x0100;
}


void CWaferMapView::MessageProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_TETE_WAFERMAP_REFRESH:
        Invalidate(FALSE);	
		GetParent()->PostMessage(WM_TETE_MAPSET_BASEBOARD,(WPARAM)m_pTheMapDoc);	
        m_iDrawMouseFlag = 0x0100;
        break;
    case WM_TETE_WAFERMAP_ARRAY:
        break;
    case WM_TETE_WAFERMAP_CHANGEARRAY:
        break;
    case  WM_TETE_WAFERMAP_DELETE:
        doDelete();
        break;
    case  WM_TETE_WAFERMAP_MODIFYDOC:
        ChangeSize();
        break;
    case  WM_TETE_EDIT_H_LEFT:
        OnAlignHorleft();
        Invalidate(FALSE);
        m_iDrawMouseFlag = 0x0100;
        break;
    case  WM_TETE_EDIT_H_CENTER:
        OnAlignHorcenter();
        Invalidate(FALSE);
        m_iDrawMouseFlag = 0x0100;
        break;
    case  WM_TETE_EDIT_H_RIGHT:
        OnAlignHorright();
        Invalidate(FALSE);
        m_iDrawMouseFlag = 0x0100;
        break;
    case  WM_TETE_EDIT_V_TOP:
        OnAlignVtop();
        Invalidate(FALSE);
        m_iDrawMouseFlag = 0x0100;
        break;
    case  WM_TETE_EDIT_V_CENTER:
        OnAlignVcenter();
        Invalidate(FALSE);
        m_iDrawMouseFlag = 0x0100;
        break;
    case  WM_TETE_EDIT_V_BOTTOM:
        OnAlignVbottom();
        Invalidate(FALSE);
        m_iDrawMouseFlag = 0x0100;
        break;
    case  WM_TETE_EDIT_HV_CENTER:
        OnAlignCenter();
        Invalidate(FALSE);
        m_iDrawMouseFlag = 0x0100;
        break;
    case  WM_TETE_EDIT_UNDO:
        DoUndo();
        break;
    case  WM_TETE_EDIT_REDO:
        DoRedo();
        break;
	case WM_TETE_EDIT_ZOOMALL:
		doZoomAll();
		break;
	case WM_TETE_WAFERMAP_SHOWWAFERINFO:
		Invalidate(FALSE);
		break;
	case WM_TETE_EDIT_SELALL:
		doSelectAll();
		break;
	case WM_TETE_MAPSET_VIEW_BASEBOARD:
		Invalidate(FALSE);
		break;
	case WM_TETE_MAPSET_VIEW_CELLSET:
		Invalidate(FALSE);
		break;
	case WM_TETE_MAPSET_VIEW_SELLEFT:
		SelectLeftHead();
		Invalidate(FALSE);
		break;
	case WM_TETE_MAPSET_VIEW_SELRIGHT:
		SelectRightHead();
		Invalidate(FALSE);
		break;
    default:
        break;
    }
}

bool CWaferMapView::SelectAllRowOrCol(CWaferGroup* pWaferGroup, double px, double py)
{
	assert(nullptr != pWaferGroup);

	//  区域
	double dbFontW = 0, dbFontH = 0, dbFontPitch = 0;
	m_pTheCoord->pix2mm(20, dbFontW);
	m_pTheCoord->pix2mm(20, dbFontH);
	m_pTheCoord->pix2mm(10, dbFontPitch);

	size_t nWaferSize = pWaferGroup->vWaferMetas.size();
	if (0 == nWaferSize)
		return false;
	
	vector<tuple<char, int, FField_t>> vtAllField;   // tuple三个元素，[‘c’列，‘r’行]，int-编号  
	FField_t field;
	int nColC = pWaferGroup->GetColCount();
	int nRowC = pWaferGroup->GetRowCount();
	for (int j = 0; j < nWaferSize; j++)
	{
		pWaferGroup->vWaferMetas[j].xDim.GetField(field);
		if (field.Left > m_fClient.Right
			|| field.Right < m_fClient.Left
			|| field.Top < m_fClient.Bottom
			|| field.Bottom > m_fClient.Top)
			continue;
		double dbWidth = field.width();
		double dbHeight = field.height();
		FField_t fontField;
		if (1 == pWaferGroup->vWaferMetas[j].nCol)
		{
			fontField.Left = field.Left - (dbFontW + dbFontPitch);
			fontField.Right = fontField.Left + dbFontW;
			fontField.Top = field.Top - (field.height() - dbFontH) * 0.5;
			fontField.Bottom = fontField.Top - dbFontH;
			vtAllField.push_back(std::make_tuple('r', nRowC - pWaferGroup->vWaferMetas[j].nRow + 1, fontField));
		}
		if (nColC == pWaferGroup->vWaferMetas[j].nCol)
		{
			fontField.Left = field.Right + dbFontPitch;
			fontField.Right = fontField.Left + dbFontW;
			fontField.Top = field.Top - (field.height() - dbFontH) * 0.5;
			fontField.Bottom = fontField.Top - dbFontH;
			vtAllField.push_back(std::make_tuple('r', nRowC - pWaferGroup->vWaferMetas[j].nRow + 1, fontField));
		}
		if (1 == pWaferGroup->vWaferMetas[j].nRow)
		{
			fontField.Left = field.Left + (field.width() - dbFontW) * 0.5;
			fontField.Right = fontField.Left + dbFontW;
			fontField.Top = field.Top + (dbFontH + dbFontPitch);
			fontField.Bottom = fontField.Top - dbFontH;
			vtAllField.push_back(std::make_tuple('c', pWaferGroup->vWaferMetas[j].nCol, fontField));
		}
		if (nRowC == pWaferGroup->vWaferMetas[j].nRow)
		{
			fontField.Left = field.Left + (field.width() - dbFontW) * 0.5;
			fontField.Right = fontField.Left + dbFontW;
			fontField.Top = field.Bottom - dbFontPitch;
			fontField.Bottom = field.Bottom - (dbFontH + dbFontPitch);
			vtAllField.push_back(std::make_tuple('c', pWaferGroup->vWaferMetas[j].nCol, fontField));
		}
	}

	for (auto& item : vtAllField)
	{
		if (std::get<2>(item).isInside(px, py))
		{
			if ('c' == std::get<0>(item))
			{
				SelectCol(std::get<1>(item), std::get<1>(item));
			}
			else if ('r' == std::get<0>(item))
			{
				SelectRow(std::get<1>(item), std::get<1>(item));
			}
			return true;
		}
	}

	return false;
}