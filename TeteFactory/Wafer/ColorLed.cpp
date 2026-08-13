// ColorLed.cpp : implementation file
//

#include "stdafx.h"
#include "ColorLed.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorLed

CColorLed::CColorLed()
{
    m_iMode = emLED_OFF;
    m_sCaption = L"";
    m_clColor = RGB(200, 200, 200);
}

CColorLed::~CColorLed()
{
}


BEGIN_MESSAGE_MAP(CColorLed, CStatic)
	//{{AFX_MSG_MAP(CColorLed)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
    ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorLed message handlers

void CColorLed::OnPaint() 
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
    //--dc.Rectangle(rect);
    POINT point; point.x = 5; point.y = 5;
    dc.RoundRect(rect,point);
    //--dc.Ellipse(rect);
    //--dc.FillRect(rect, &NewBrush);
    dc.SelectObject(pOldBush);
	NewBrush.DeleteObject();

    CPen NewPen, *pOldPen;
    NewPen.CreatePen(PS_SOLID, 2, RGB(150, 150, 150));
    pOldPen = dc.SelectObject(&NewPen);
    CFont font;
    font.CreateFont(
        14,0,0,0,FW_NORMAL,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,L"宋体");

    dc.SelectObject(&font);
    dc.SetBkMode(TRANSPARENT);
    dc.SetTextColor(RGB(0, 0, 0));
    dc.TextOutW(rect.left+5, rect.top+5, m_sCaption.GetString(), m_sCaption.GetLength());
}

void CColorLed::OnSetLedInfo(CString sCaption,int iMode)
{
	KillTimer(1);
    m_sCaption = sCaption;
    m_iMode = iMode;
    if (m_iMode == emLED_RED)
    {
        m_clColor = RGB(255, 0, 0);
        SetTimer(1, 200,NULL);
    }
    else if (m_iMode == emLED_GREEN)
    {
        m_clColor = RGB(0, 255, 0);
    }
    else if (m_iMode == emLED_YELLOW)
    {
        m_clColor = RGB(255, 255, 0);
    }
    else if (m_iMode == emLED_OFF)
    {
        m_clColor = RGB(150, 150, 150);
    }
}


void CColorLed::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    if (1 == nIDEvent)
    {
        if (m_iMode == emLED_RED)
        {
            if(0x0000ff==m_clColor)
                m_clColor = RGB(200, 200, 200);
            else
                m_clColor = RGB(255, 0, 0);
			Invalidate(false);
        }
        else if (m_iMode == emLED_GREEN)
        {
            m_clColor = RGB(0, 255, 0);
        }
        else if (m_iMode == emLED_YELLOW)
        {
            m_clColor = RGB(255, 255, 0);
        }
        else if (m_iMode == emLED_OFF)
        {
            m_clColor = RGB(150, 150, 150);
        }
    }
    CStatic::OnTimer(nIDEvent);
}
