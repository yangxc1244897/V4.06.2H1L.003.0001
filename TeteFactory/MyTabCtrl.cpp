// MyTabCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "TeteFactory.h"
#include "MyTabCtrl.h"


// CMyTabCtrl

IMPLEMENT_DYNAMIC(CMyTabCtrl, CTabCtrl)

CMyTabCtrl::CMyTabCtrl()
{

}

CMyTabCtrl::~CMyTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyTabCtrl, CTabCtrl)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CMyTabCtrl 消息处理程序




void CMyTabCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	// TODO:  添加您的代码以绘制指定项
	//创建画刷
	CBrush      cbr;
	cbr.CreateSolidBrush(BKCOLOR);

	//获取选项卡文字内容
	TCHAR        szTabText[100];
	TC_ITEM     tci;
	memset(szTabText, '\0', sizeof(szTabText));
	tci.mask = TCIF_TEXT;
	tci.pszText = szTabText;
	tci.cchTextMax = sizeof(szTabText) - 1;
	GetItem(lpDrawItemStruct->itemID, &tci);
	
	//填充选项卡背景
	CDC *dc = CDC::FromHandle(lpDrawItemStruct->hDC);
	dc->FillRect(&lpDrawItemStruct->rcItem, &cbr);

	//绘制选项卡文字
	dc->SetBkColor(BKCOLOR);
	dc->SetTextColor(STATIC_COLOR);
	RECT rc;
	rc = lpDrawItemStruct->rcItem;
	rc.top += 3;
	CFont font;
	font.CreatePointFont(120, _T("Arial"));
	CFont * pOldPen = dc->SelectObject(&font);
	dc->DrawText(tci.pszText, lstrlen(tci.pszText), &rc, DT_CENTER);
	dc->SelectObject(pOldPen);
	font.DeleteObject();
}


BOOL CMyTabCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//获取控件矩形
	CRect   rect;
	GetClientRect(&rect);

	//创建画刷
	CBrush   brush;
	brush.CreateSolidBrush(BKCOLOR);

	//填充控件背景
	pDC->FillRect(&rect, &brush);

	brush.DeleteObject();
	//return CTabCtrl::OnEraseBkgnd(pDC);
	return true;
}
