// ShowTip.cpp : 实现文件
//

#include "stdafx.h"
#include "TeteFactory.h"
#include "ShowTip.h"


// CShowTip

IMPLEMENT_DYNAMIC(CShowTip, CStatic)

CShowTip::CShowTip()
{
	m_sText = _T("");
	//m_Font.CreatePointFont(150, _T("Arial"));
	m_Font.CreateFont(20,
		11, 0, 0,
		FW_BOLD,
		FALSE, FALSE, 0,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		CLEARTYPE_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		_T("Arial"));
	m_clrText = STATIC_TITLE_COLOR_NAME/*STATIC_TITLE_COLOR*/;
	m_clrBK = RGB(0, 0, 0);
	m_FramePen.CreatePen(PS_SOLID, 1, RGB(250, 0, 250));
	m_nStartIndex = 0;
	m_nEndIndex = m_sText.GetLength();
	m_bFlag = TRUE;
}

CShowTip::~CShowTip()
{
	m_Font.DeleteObject();
	m_FramePen.DeleteObject();
}


BEGIN_MESSAGE_MAP(CShowTip, CStatic)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CShowTip 消息处理程序




void CShowTip::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1)
	{
		if (m_nEndIndex == m_sText.GetLength())
		{
			m_nStartIndex += 1;
			if (m_nStartIndex >= m_nEndIndex)
			{
				m_nStartIndex = 0;
				m_nEndIndex = 1;
			}
		}
		else
		{
			m_nEndIndex += 1;
		}
		GetWindowRect(&m_rcClient);
		GetParent()->ScreenToClient(&m_rcClient);
		GetParent()->InvalidateRect(m_rcClient);
		GetParent()->UpdateWindow();
		Invalidate(TRUE);
	}

	CStatic::OnTimer(nIDEvent );
}


void CShowTip::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CStatic::OnPaint()
	dc.SetBkMode(TRANSPARENT);
	CFont * pOldFont = dc.SelectObject(&m_Font);
	CPen * pOldPen = dc.SelectObject(&m_FramePen);
	CSize szText = dc.GetTextExtent(m_sText); // 获取字符串尺寸
	GetClientRect(&m_rcClient);           // 获取显示框尺寸

	dc.SetTextColor(m_clrText);

	if (szText.cx < m_rcClient.Width())
	{
		dc.DrawText(m_sText, m_rcClient, DT_CENTER);
	}
	else
	{
		CRect rcDraw = m_rcClient;
		CString s = m_sText.Mid(m_nStartIndex, m_nEndIndex - m_nStartIndex);
		CSize sz = dc.GetTextExtent(s);
		if (0 == m_nStartIndex)
		{
			rcDraw.left = m_rcClient.right - sz.cx;
			if (rcDraw.left <= m_rcClient.left)
			{
				rcDraw.left = 0;
				m_nStartIndex = 0;
				m_nEndIndex = m_sText.GetLength();
			}
		}

		dc.DrawText(s, rcDraw, DT_LEFT);

		if (m_bFlag)
		{
			m_bFlag = FALSE;
			SetTimer(1, 300, NULL);
		}
	}

	dc.SelectObject(pOldFont);
	dc.SelectObject(pOldPen);
}

void CShowTip::ShowText(const CString & sText)
{
	KillTimer(1);
	m_sText = sText;
	m_bFlag = TRUE;
	Invalidate(FALSE);
}


int CShowTip::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码


	return 0;
}
