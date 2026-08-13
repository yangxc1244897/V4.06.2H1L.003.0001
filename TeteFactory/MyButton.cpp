// MyButton.cpp : 实现文件
//

#include "stdafx.h"
#include "TeteFactory.h"
#include "MyButton.h"


CMyButton::CMyButton(void)
{
	m_crForeColor[COLOR_NORMAL] = RGB(0, 0, 0);
	m_crForeColor[COLOR_FOCUSED] = RGB(240, 240, 240);
	m_crForeColor[COLOR_PRESSED] = RGB(255, 255, 255);
	m_nColorType = 0;  // Brown or Red
	m_bMouseOnButton = FALSE;
	m_bIsPressed = FALSE;
	m_bIsFocused = FALSE;
	m_bIsDisabled = FALSE;
	m_nFontHeight = 17;
	m_nFontWidth = 6;
	m_bPointFont = FALSE;
	m_sFontName = _T("Arial");

}

CMyButton::~CMyButton(void)
{
	FreeResources();
}

void CMyButton::FreeResources()
{

}

void CMyButton::CancelHover()
{
	if (m_bMouseOnButton)
	{
		m_bMouseOnButton = FALSE;
		Invalidate();
	}
}


BOOL CMyButton::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_LBUTTONDBLCLK)
		pMsg->message = WM_LBUTTONDOWN;

	return CButton::PreTranslateMessage(pMsg);
}

void CMyButton::SetCaptionFont(CString sFontName, int nWidth, int nHeight, BOOL bPointFont)
{
	m_sFontName = sFontName;
	m_nFontWidth = nWidth;
	m_nFontHeight = nHeight;
	m_bPointFont = bPointFont;
	Invalidate();
}

BEGIN_MESSAGE_MAP(CMyButton, CButton)
	ON_WM_KILLFOCUS()
	ON_CONTROL_REFLECT_EX(BN_CLICKED, OnClicked)  //消息反射
	ON_WM_ACTIVATE()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

void CMyButton::OnMouseMove(UINT nFlags, CPoint point)
{
	CWnd   *wndUnderMouse = NULL;
	CWnd   *wndActive = this;
	TRACKMOUSEEVENT csTME;
	CButton::OnMouseMove(nFlags, point);
	ClientToScreen(&point);
	wndUnderMouse = WindowFromPoint(point);//获得点下面的组件

	// If the mouse enter the button with the left button pressed then do nothing
	if (nFlags & MK_LBUTTON && m_bMouseOnButton == FALSE)
		return;

	if (wndUnderMouse && wndUnderMouse->m_hWnd == m_hWnd && wndActive)
	{
		if (!m_bMouseOnButton)
		{
			m_bMouseOnButton = TRUE;
			Invalidate();
			csTME.cbSize = sizeof(csTME);
			csTME.dwFlags = TME_LEAVE;
			csTME.hwndTrack = m_hWnd;
			::_TrackMouseEvent(&csTME);//发送鼠标离开的消息，见我之前的博客
		}
	}
	else
		CancelHover();
}

LRESULT CMyButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	CancelHover();
	return 0;
}


BOOL CMyButton::OnClicked()
{
	SetFocus();//设置焦点
	Invalidate();
	return FALSE;
}

void CMyButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC   *pDC = CDC::FromHandle(lpDIS->hDC); //获取CDC
	int   nWidth, nHeight;
	CRect  itemRect = lpDIS->rcItem;
	nWidth = itemRect.Width();
	nHeight = itemRect.Height();
	m_bIsPressed = (lpDIS->itemState & ODS_SELECTED);//是不是已经按下
	m_bIsFocused = (lpDIS->itemState & ODS_FOCUS);   //是不是获得了焦点
	m_bIsDisabled = (lpDIS->itemState & ODS_DISABLED); //是不是没使能

	pDC->SetBkMode(TRANSPARENT);//设置背景透明
	DrawBackground(pDC, nWidth, nHeight);//画背景
	DrawBtnCaption(pDC, nWidth, nHeight); //画字体
}

void CMyButton::DrawBackground(CDC *dc, int nWidth, int nHeight)
{
	if (m_bIsPressed)
	{
		CPen pen(PS_SOLID, 1, STATIC_COLOR);
		CPen * pOldPen = dc->SelectObject(&pen);

		dc->Rectangle(2, 2, nWidth - 4, nHeight - 4);

		dc->SelectObject(pOldPen);
		pen.DeleteObject();
	}
	else
	{
		CPen pen(PS_SOLID, 1, STATIC_COLOR);
		CPen * pOldPen = dc->SelectObject(&pen);

		dc->Rectangle(2, 2, nWidth - 4, nHeight - 4);

		dc->SelectObject(pOldPen);
		pen.DeleteObject();
	}
}

void CMyButton::DrawBtnCaption(CDC *dc, int nWidth, int nHeight)
{
	CString   str;
	CRect   rect;
	int    nColorIndex;
	CFont   font, *old_font;
	if (m_bIsPressed)
		nColorIndex = COLOR_PRESSED;
	else if (m_bMouseOnButton)
		nColorIndex = COLOR_FOCUSED;
	else
		nColorIndex = COLOR_NORMAL;

	if (m_bPointFont)
		font.CreatePointFont(m_nFontHeight * 10, m_sFontName);
	else
	{
		font.CreateFont(m_nFontHeight, m_nFontWidth, 0, 0, FW_BOLD, 0, 0, 0,
			DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, FIXED_PITCH | FF_MODERN, m_sFontName);
	}
	GetWindowText(str);
	rect = CRect(2, 2, nWidth - 4, nHeight - 4);
	old_font = dc->SelectObject(&font);
	dc->SetTextColor(m_crForeColor[nColorIndex]);
	//显示字体
	dc->DrawText(str, str.GetLength(), &rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	dc->SelectObject(old_font);
	font.DeleteObject();
}

void CMyButton::SetForeColor(int nIndex, COLORREF crColor, BOOL bRepaint)
{
	m_crForeColor[nIndex] = crColor;
	if (bRepaint)
		Invalidate();
}

void CMyButton::OnKillFocus(CWnd * pNewWnd)

{

	CButton::OnKillFocus(pNewWnd);

	CancelHover();//当失去焦点的时候，需要改变字体颜色，需要重绘，调用Invalidate();

}

void CMyButton::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)

{

	CButton::OnActivate(nState, pWndOther, bMinimized);

	if (nState == WA_INACTIVE)  //当失去焦点的时候，需要改变字体颜色，需要重绘，调用Invalidate();

		CancelHover();

}
