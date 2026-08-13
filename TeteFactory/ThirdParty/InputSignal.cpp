// ColorLed.cpp : implementation file
//

#include "stdafx.h"
#include "InputSignal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputStatic

CInputStatic::CInputStatic()
{
    m_clColor = RGB(150, 150, 150);
}

CInputStatic::~CInputStatic()
{
}


BEGIN_MESSAGE_MAP(CInputStatic, CStatic)
	//{{AFX_MSG_MAP(CColorLed)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorLed message handlers

void CInputStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
    CRect rect;
    CString	sText;
    CSize	szText;	
    GetWindowRect(&rect);
    ScreenToClient(&rect);

    CBrush	NewBrush,*pOldBush;
    NewBrush.CreateSolidBrush(m_clColor);
    pOldBush = dc.SelectObject(&NewBrush);
	dc.BeginPath();
	//dc.Ellipse(rect);
	dc.Rectangle(rect);
	dc.EndPath();
	dc.FillPath();
    dc.SelectObject(pOldBush);
	NewBrush.DeleteObject();

	// 绘制按钮原来的文字
	GetWindowText(sText);
	if (!sText.IsEmpty())
	{
		CFont *pFont = GetFont();
		CFont *pOldFont = dc.SelectObject(pFont);
		CSize szExtent = dc.GetTextExtent(sText, sText.GetLength());
		szExtent.cx += 5;
		CRect rectText = rect;

		rectText.DeflateRect(rect.CenterPoint().x - szExtent.cx / 2
			, rect.CenterPoint().y - szExtent.cy / 2
			, rect.CenterPoint().x - szExtent.cx / 2
			, rect.CenterPoint().y - szExtent.cy / 2);
		int nOldBkMode = dc.SetBkMode(TRANSPARENT);
		dc.SetTextColor(RGB(60,60,60));
		dc.DrawText(sText, -1, rectText, /*DT_WORDBREAK | */DT_CENTER);
		dc.SelectObject(pOldFont);
		dc.SetBkMode(nOldBkMode);
	}
}

void CInputStatic::SetStatus(unsigned short status)
{
	if (status == SIGNAL)	    //绿色
	{
		m_clColor = RGB(0, 255, 0);
	}
	else if (status == NON_SIGNAL) //灰色默认色
	{
		m_clColor = RGB(160, 160, 160);
	}
	else if (status == ALARM)	 //红色报警
	{
		m_clColor = RGB(255, 0, 0);
	}

	Invalidate(FALSE);
}


void CInputStatic::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类

	CStatic::PreSubclassWindow();
}
