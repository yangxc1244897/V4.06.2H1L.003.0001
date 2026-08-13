#pragma once


// CMyButton

class CMyButton : public CButton {
public: 
	CMyButton(void);
	~CMyButton(void); 
public:
	enum { MAX_BTN_COLOR = 3 };
	enum { COLOR_NORMAL = 0, COLOR_FOCUSED = 1, COLOR_PRESSED = 2 }; //按钮的三种状态  
	COLORREF m_crForeColor[MAX_BTN_COLOR];  
	UINT  m_nTypeStyle; 
	int   m_nColorType;  
	BOOL  m_bMouseOnButton; // Is mouse over the button? 
	BOOL  m_bIsPressed;   // Is button pressed?
	BOOL  m_bIsFocused;  // Is button focused?
	BOOL  m_bIsDisabled;  // Is button disabled? 
	CString  m_sFontName; //字体名字 
	int   m_nFontWidth, m_nFontHeight; //字体大小 
	BOOL  m_bPointFont;
public: 
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct); //这里是重点，通过调用Invalidate()可以马上重绘按钮，从而调用这个函数 
	virtual BOOL PreTranslateMessage(MSG *pMsg);  
	void SetForeColor(int nIndex, COLORREF color, BOOL bRepaint = TRUE);//设置前景色
	void SetCaptionFont(CString sFontName, int nWidth, int nHeight, BOOL bPointFont=FALSE);
protected:
	virtual void DrawBackground(CDC *dc, int nWidth, int nHeight);//绘制背景 
	virtual void DrawBtnCaption(CDC *dc, int nWidth, int nHeight);//绘制字体 
private:
	void FreeResources(); 
	void CancelHover(); 
public: 
	afx_msg BOOL OnClicked();//点击事件 
	afx_msg void OnKillFocus(CWnd* pNewWnd); 
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);//鼠标移动事件 
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized); 
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam); 
	DECLARE_MESSAGE_MAP()
};


