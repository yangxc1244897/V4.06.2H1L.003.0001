/**
******************************************************************************
* @文件     
* @作者  
* @版本 V1.00.00
* @日期 2017-04-11
* @简述   
* @email:jchq0101net@sina.com
******************************************************************************
* @说明
*
*
* 
******************************************************************************
*/
#pragma once

#include <vector>
using namespace std;

#include "tlGraphRender.h"
#include "Wafer\WaferMapDoc.h"
#include "CoordTransform.h"

#define   IDM_MAPADJUST_CHANGEWAFER         0x01  // 改变所属的Wafer

class CWaferMapDoc;
class CWaferMapView : public CWnd
{
    // Construction
public:
    CWaferMapView();

    void MessageProc(UINT message, WPARAM wParam, LPARAM lParam);
    // Attributes
public:
    COLORREF m_clBackColor;
    COLORREF m_clRulerColor;
    COLORREF m_clGridColor;
    COLORREF m_clTextColor;
    COLORREF m_clCrossColor;
    COLORREF m_clCurveColor;

    CRect   m_rectClient;
    DPoint_t m_PrevPoint;   //--光标上一点
    DPoint_t m_LastPoint;   //--光标的当前点
    DPoint_t m_MousePoint;  //--光标移动时的当前位置
    CPoint   m_ptMouseMove;
    FField_t  m_rtSelectField;
	FField_t m_rtSelField;
    FField_t  m_rtSelRangeOrg;
	CBrush m_IndexBackground;

    double m_dbMarkLimitX;   // 整个双头标记宽度
    double m_dbMarkLimitY;   // 整个双头标记高度

	FField_t m_MarkFieldA;   // 主头标记区域
	FField_t m_MarkFieldB;   // 从头标记区域

    emDrawType_t  m_emDrawType;   //--绘图按钮的当前功能
    int  m_iDrawMouseFlag;        //--鼠标在绘图区的功能
    int  m_iMouseFlag;            //--鼠标在绘图区的功能
    int  m_csCursorType;          //--光标类型
    int m_iPolygonSegNum;         //--输入的多边形边数
    vector<DPoint_t>  m_vDrawPoints;

    ID2Coord  *m_pTheCoord;
    IGraphRender *m_pTheRender;
	CCoordTransform *m_pCoordTrans;
    // Operations
public:
    void DrawDrawStatus(void);
    void DrawSelField(void);
	void DrawFaceInfo(IGraphRender * pRender);
	void DrawAnchor(IGraphRender * pRender);
    void OnChangeDoc(CWaferMapDoc *pTheDoc);
    void OnDrawChange(emDrawType_t emType);
    void OnGraphArrowDown(UINT nFlags, CPoint point);
    void OnGraphArrowMove(UINT nFlags, CPoint point);
    void OnGraphArrowUp(UINT nFlags, CPoint point);
    void doMouseSwitch(double X, double Y);    
    void doSelectObject(double px,double py);
    void doSelectObject(double x1,double y1,double x2,double y2);
	void UnSelectAllObjects();
    void pix2mm(CPoint pt, DPoint_t &dp) { m_pTheCoord->pix2mm(pt.x, pt.y, dp.x, dp.y); };
    void pix2mm(POINT pt, DPoint_t &dp) { m_pTheCoord->pix2mm(pt.x, pt.y, dp.x, dp.y); };
    void mm2pix(DPoint_t dp, CPoint &pt) { m_pTheCoord->mm2pix(dp.x, dp.y, pt.x, pt.y); };
    void mm2pix(DPoint_t dp,  POINT &pt) { m_pTheCoord->mm2pix(dp.x, dp.y, pt.x, pt.y); };
	bool SelectAllRowOrCol(CWaferGroup* pWaferGroup, double px, double py);   // 选中整行或整列
	void EnablePopMenu(bool b) { m_bPopMenu = b; }
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CViewDoc)
public:
    virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext);
    //}}AFX_VIRTUAL

    // Implementation
public:
    virtual ~CWaferMapView();

    // Generated message map functions
protected:
    //{{AFX_MSG(CViewDoc)
    afx_msg void OnPaint();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    CWnd       *m_pParentWnd;
	CMenu       m_thePopupMenu;
	CWaferMapDoc  *m_pTheMapDoc;
	CFont m_InfoFont;   // 芯片信息字体
	FField_t m_fClient;  // 绘图客户区
	bool m_bPopMenu = true;   // 是否弹出右键菜单
public:
	// 鼠标右键动作
	void doRotate();
	void doSelectAll();
	void doSelGroup(); 
	void doDelete(); 
	void doChangeStatus();
	void doManualAutoAllotWork(int iHead);
	void doChangeWafer(int nWaferID);
	void doChangePos();
	void doZoomAll();
	void doZoomIn(double dbZoom);
	void doZoomOut(double dbZoom);
	void SelectLeftHead();
	void SelectRightHead();
	/*! istart iend 都是从序号1开始*/
	void SelectRow(int istart, int iend);
	void SelectCol(int istart, int iend);
	void SetSelPrint(bool b = true);
	void SetAllPrint(bool b = true);
	
	bool m_bFixedCanvas = false;   // 是否采用固定画布

public:
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
private:
	bool isMutilSel();
	void changeLineToVH(DPoint_t& dps, DPoint_t& dpe, int nVirtKey = VK_CONTROL);
	void changeRectToSquare(DPoint_t& dLT, DPoint_t& dRB, int nVirtKey = VK_CONTROL);
	void getPolygonInfo(int nSides, const DPoint_t& dCenter, const DPoint_t& dStart, std::vector<DPoint_t>& vdpts);
public:
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

public:
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);


	int GetObjSelCount();  //获取选中对象个数与各个芯片的信息
	void GetSelField(FField_t &field);//获取选中对象的最大矩形框
	vector<xWaferMeta_t> m_vWaferMeta;

	int GetObjMarkCount();  // 获取打印的芯片数量

	void ChangeSize();
	void DeleteAll();

	//对齐操作
	void OnAlignHorleft();
	void OnAlignHorcenter();
	void OnAlignHorright();
	void OnAlignVtop();
	void OnAlignVcenter();
	void OnAlignVbottom();
	void OnAlignCenter();

	//撤销操作
	void DoRedo(); //重做
	void DoUndo();//撤销
};
