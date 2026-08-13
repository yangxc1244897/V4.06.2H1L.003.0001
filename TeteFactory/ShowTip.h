#pragma once

enum
{
	emRollStyle_Auto = 0,    // 自动判断是否滚动
	emRollStyle_Open = 0,    // 开启滚动
	emRollStyle_Close = 0    // 关闭滚动
};

// CShowTip

class CShowTip : public CStatic
{
	DECLARE_DYNAMIC(CShowTip)

public:
	CShowTip();
	virtual ~CShowTip();

protected:
	DECLARE_MESSAGE_MAP()

	int m_nRollStyle;

	CString m_sText;
	int m_nStartIndex;  // 文本从哪一个字符开始显示，m_nStartIndex起始索引
	int m_nEndIndex;    // 文本从哪一个字符结束显示，m_nEndIndex起始索引

	CFont m_Font;
	CPen m_FramePen;
	COLORREF m_clrText;
	COLORREF m_clrBK;
	CRect m_rcClient;
	BOOL m_bFlag;

public:
	void ShowText(const CString & sText);

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


