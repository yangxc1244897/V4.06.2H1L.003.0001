
#include "stdafx.h"
#include "ViewAllBin.h"
#include "resource.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CViewAllBin
CViewAllBin::CViewAllBin()
{
}

CViewAllBin::~CViewAllBin()
{

}

BEGIN_MESSAGE_MAP(CViewAllBin, CWnd)
    //{{AFX_MSG_MAP(CViewAllBin)
    ON_WM_PAINT()
    //}}AFX_MSG_MAP
	ON_WM_SIZE()
END_MESSAGE_MAP()


BOOL CViewAllBin::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
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
		m_nBinCount = 0;
    }
    return TRUE;
}

void CViewAllBin::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    if (NULL == GetSafeHwnd())
        return;

	CRect rcClient;
	GetClientRect(&rcClient);

	//--CDC *m_pWindowDC = &dc;
	CDC *m_pWindowDC = &dc;
	CDC m_thePaintDC;
	CBitmap  m_memBitmap, *m_pOldBitmap;
	m_thePaintDC.CreateCompatibleDC(m_pWindowDC);
	m_memBitmap.CreateCompatibleBitmap(m_pWindowDC, rcClient.Width(), rcClient.Height());
	m_pOldBitmap = (CBitmap *)m_thePaintDC.SelectObject(&m_memBitmap);

	CBrush brushBack;
	brushBack.CreateSolidBrush(RGB(195, 195, 195));
	m_thePaintDC.FillRect(rcClient, &brushBack);
	m_thePaintDC.SetBkMode(TRANSPARENT);

	m_pWindowDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &m_thePaintDC, 0, 0, SRCCOPY);
	m_thePaintDC.SelectObject(m_pOldBitmap);
	m_memBitmap.DeleteObject();  //--删除内存位图GDI对象
	m_thePaintDC.DeleteDC();  //--删除内存绘图环境
	ReleaseDC(m_pWindowDC);  // 释放内存绘图环境
}

BOOL CViewAllBin::OnCommand(WPARAM wParam, LPARAM lParam)
{
    // TODO: 在此添加专用代码和/或调用基类
    UINT uMsg = LOWORD(wParam);
    CString sMsg;
 
    return __super::OnCommand(wParam, lParam);
}

void CViewAllBin::SetBinContent(CWaferGraphDoc * pDoc)
{
	for (auto & item : m_vpViewSingleBin)
	{
		delete item;
	}
	m_vpViewSingleBin.clear();
	m_nBinCount = 0;

	if (nullptr == pDoc)
		return;

#if 0  // 显示多个印章
	m_nBinCount = pDoc->GetWaferCount();
	for (int i = 0; i < m_nBinCount; i++)
	{
		CViewSingleBin *pViewSingleBin = new CViewSingleBin;
		pViewSingleBin->Create(WS_VISIBLE | WS_CHILD, CRect(0, 0, 10, 10), this, ID_SINGLE_BIN, NULL);
		pViewSingleBin->SetShowContent(i, pDoc->GetWafer(i));
		m_vpViewSingleBin.push_back(pViewSingleBin);
	}
	if (m_nBinCount == 1)
		m_vpViewSingleBin[0]->ShowTitle(FALSE);

	CRect rcClient;
	GetClientRect(rcClient);
	int nMinCol = m_nBinCount, nWhiteSpace = 1;
	//if (m_nBinCount > 0 && m_nBinCount <= 4)
	{
		int nW = rcClient.Width() / m_nBinCount;
		int nH = rcClient.Height();
		CRect rc;
		for (int i = 0; i < m_nBinCount; i++)
		{
			rc.left = rcClient.left + i * nW + nWhiteSpace;
			rc.top = rcClient.top + nWhiteSpace;
			rc.right = (i + 1) * nW - nWhiteSpace;
			rc.bottom = rcClient.bottom - nWhiteSpace;
			m_vpViewSingleBin[i]->MoveWindow(&rc);
		}
	}
	/*else if (m_nBinCount > nMinCol)
	{
		int nW = rcClient.Width() / nMinCol;
		int nH = rcClient.Height() * 0.5;
		CRect rc;
		int nC = 0;
		rc.top = rcClient.top + nWhiteSpace;
		rc.bottom = rcClient.top + nH - nWhiteSpace;
		for (int i = 0; i < m_nBinCount; i++, nC++)
		{
			if (i == nMinCol)
			{
				rc.top = rcClient.top + nH + nWhiteSpace;
				rc.bottom = rcClient.bottom - nWhiteSpace;
				nC = 0;
			}
			rc.left = rcClient.left + nC * nW + nWhiteSpace;
			rc.right = (nC + 1) * nW - nWhiteSpace;
			m_vpViewSingleBin[i]->MoveWindow(&rc);
		}
	}*/
#else
	int iBinIndex = 0;
	m_nBinCount = pDoc->GetWaferCount();
	if (1 == m_nBinCount) {
		iBinIndex = 0;
	}

	CViewSingleBin *pViewSingleBin = new CViewSingleBin;
	pViewSingleBin->Create(WS_VISIBLE | WS_CHILD, CRect(0, 0, 10, 10), this, ID_SINGLE_BIN, NULL);
	pViewSingleBin->SetShowContent(iBinIndex, pDoc->GetWafer(iBinIndex));
	m_vpViewSingleBin.push_back(pViewSingleBin);
	CRect rcClient;
	GetClientRect(rcClient);
    m_vpViewSingleBin[0]->MoveWindow(&rcClient);


#endif
}


void CViewAllBin::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}


void CViewAllBin::SetMarkCount(int nBin, int nC)
{
	if (nBin < 0 || nBin >= m_vpViewSingleBin.size())
		return;

	int nBinC = m_vpViewSingleBin.size();
	if (nBin < 0 || nBin >= nBinC
		|| nBinC == 1)
		return;

	if (nullptr != m_vpViewSingleBin[nBin])
	{
		m_vpViewSingleBin[nBin]->SetMarkTimes(nC);
		m_vpViewSingleBin[nBin]->Invalidate(FALSE);
	}
}

void CViewAllBin::SetSelBin(int nBin)
{
	int nBinC = m_vpViewSingleBin.size();
	if (nBin < 0 || nBin >= nBinC
		|| nBinC == 1)
		return;

	for (int i = 0; i < nBinC; i++)
	{
		if(nBin == i)
			m_vpViewSingleBin[i]->ShowSelFlag(TRUE);
		else
			m_vpViewSingleBin[i]->ShowSelFlag(FALSE);

		m_vpViewSingleBin[i]->Invalidate(FALSE);
	}
}