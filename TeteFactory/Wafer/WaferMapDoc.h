#pragma once
#include "tlTeteDefines.h"
#include "tlGraphRender.h"
#include<vector>
#include "tlGraphFile.h"
#include "WaferNode.h"
using namespace std;

inline double VectorAngle(double x1, double y1, double x2, double y2)
{
	double dx = x2 - x1;
	double dy = y2 - y1;

	double cross = dx * 1 + dy * 0;
	double len1 = sqrt(dx * dx + dy * dy);

	double a = acos(cross / (len1 * 1));

	if (dy < 0)
		a = TETE_2PI - a;

	return a;
};

/****************************************************************************/
/*  */
/* 三点定位图形对象的大小位置与角度 */
typedef struct _xDimension {
    DPoint_t dimPoints0[3];  //--初始定位点，左上、左下、右上
    DPoint_t dimPoints1[4];  //--实际定位点，左上、左下、右上
    bool isValid;
    double GetOffsetX(void) { return (dimPoints1[0].x - dimPoints0[0].x); };
    double GetOffsetY(void) { return (dimPoints1[0].y - dimPoints0[0].y); };
    double GetZoomX(void) {};
    double GetZoomY(void) {};
    double GetCenterX(void) { return (dimPoints1[1].x + dimPoints1[2].x)*0.5; };//--旋转中心
    double GetCenterY(void) { return (dimPoints1[1].y + dimPoints1[2].y)*0.5; };//--旋转中心
	//	旋转角度，计算左上点到右上点的水平线，与X轴正方向的角度
	//  返回弧度
    double GetRotateAngle(void)   
	{
		double dx = dimPoints1[2].x - dimPoints1[0].x;
		double dy = dimPoints1[2].y - dimPoints1[0].y;

		double cross = dx * 1 + dy * 0;
		double len1 = sqrt(dx * dx + dy * dy);
		
		double a = acos(cross / (len1 * 1));
	
		if (dy < 0)
			a = TETE_2PI - a;

		return a;
	};     
    double GetAffineAngle(void) {
        //double x1, y1, x2, y2; 
        //x1 = dimPoints1[1].x - dimPoints1[0].x;    x2 = dimPoints1[2].x - dimPoints1[0].x;
        //y1 = dimPoints1[1].y - dimPoints1[0].y;    y2 = dimPoints1[2].y - dimPoints1[0].y;
        //double angle = atan2(y2,x2)-atan2(y1,x1); 
        //if (angle > TETE_PI)
        //    angle = angle - TETE_PI * 2;
        //else if(angle <-TETE_PI)
        //    angle = angle + TETE_PI * 2;
        //return angle - TETE_PI * 0.5;

		double a1 = VectorAngle(dimPoints0[0].x, dimPoints0[0].y, dimPoints0[2].x, dimPoints0[2].y);
		double a2 = VectorAngle(dimPoints1[0].x, dimPoints1[0].y, dimPoints1[2].x, dimPoints1[2].y);

		return a2 - a1;
    }  //--错切角度
    void  AffineTransform(double tx,double ty) {};
	void Init(double dbOffsetX, double dbOffsetY, double dbHight, double dbwidth)//--相对偏移复制
	{
		for (int i = 0; i < 3; i++)
		{
			dimPoints0[i].x = dbOffsetX;
			dimPoints0[i].y = dbOffsetY;
			dimPoints1[i].x = dbOffsetX;
			dimPoints1[i].y = dbOffsetY;
		}
		dimPoints0[1].y -= dbHight;
		dimPoints1[1].y -= dbHight;
		dimPoints0[2].x += dbwidth;
		dimPoints1[2].x += dbwidth;
		double dbOffset = min(dbwidth, dbHight) * 0.25;
		dimPoints1[3].x = dimPoints0[1].x + dbOffset;    // 左下角
		dimPoints1[3].y = dimPoints0[1].y + dbOffset;
	}

	void Init(double dbW, double dbH, double dbA)
	{
		// 基于左上角坐标系
		dimPoints1[0] = DPoint_t(0, 0);      // 左上
		dimPoints1[1] = DPoint_t(0, -dbH);   // 左下
		dimPoints1[2] = DPoint_t(dbW, 0);    // 右上

		// 定位孔
		double dbOffset = min(dbW, dbH) * 0.25;
		dimPoints1[3] = DPoint_t(dimPoints1[1].x + dbOffset, dimPoints1[1].y + dbOffset); 
              
		// 旋转
		double dRad = DEG2RAD(dbA);
		double cx = dbW * 0.5;
		double cy = - dbH * 0.5;	
		for (int i = 0; i < 4; i++)
		{
			double dx = dimPoints1[i].x - cx;
			double dy = dimPoints1[i].y - cy;
			dimPoints1[i].x = dx*cos(dRad) - dy*sin(dRad) + cx;
			dimPoints1[i].y = dx*sin(dRad) + dy*cos(dRad) + cy;
		}

		for (int i = 0; i < 3; i++)
		{
			dimPoints0[i] = dimPoints1[i];
		}
	}

	void Zoom(double dOldHigth,double dOldWidth,double dNewHight,double dNewWidth)
	{
		dimPoints1[1].x = (dimPoints1[1].x - dimPoints1[0].x) * (dNewHight / dOldHigth) + dimPoints1[0].x;
		dimPoints1[1].y = (dimPoints1[1].y - dimPoints1[0].y) * (dNewHight / dOldHigth) + dimPoints1[0].y;
		dimPoints1[2].x = (dimPoints1[2].x - dimPoints1[0].x) * (dNewWidth / dOldWidth) + dimPoints1[0].x;
		dimPoints1[2].y = (dimPoints1[2].y - dimPoints1[0].y) * (dNewWidth / dOldWidth) + dimPoints1[0].y;
		dimPoints0[1].x = dimPoints0[1].x - dOldWidth + dNewWidth;
		dimPoints0[2].y = dimPoints0[2].y - dOldHigth + dNewHight;
	}
	bool IsInside(double dx, double dy)
	{
		FField_t field;
		GetField(field);
		if (field.Top > dy && field.Bottom < dy && field.Left < dx && field.Right > dx)
		{
			return true;
		}
		return false;
	}
	bool IsInside(FField_t field)
	{
		FField_t Tmpfield;
		GetField(Tmpfield);
		if (field.Top > Tmpfield.Top && field.Bottom < Tmpfield.Bottom && field.Left <Tmpfield.Left && field.Right > Tmpfield.Right)
		{
			return true;
		}

		return false;
	}	
	void GetField(FField_t &field)
	{
		field.Top = max(dimPoints1[0].y, dimPoints1[1].y);
		field.Top = max(field.Top, dimPoints1[2].y);
		field.Top = max(field.Top, dimPoints1[2].y - dimPoints1[0].y + dimPoints1[1].y);

		field.Bottom = min(dimPoints1[0].y, dimPoints1[1].y);
		field.Bottom = min(field.Bottom, dimPoints1[2].y);
		field.Bottom = min(field.Bottom, dimPoints1[2].y - dimPoints1[0].y + dimPoints1[1].y);

		field.Left = min(dimPoints1[0].x, dimPoints1[1].x);
		field.Left = min(field.Left, dimPoints1[2].x);
		field.Left = min(field.Left, dimPoints1[2].x - dimPoints1[0].x + dimPoints1[1].x);

		field.Right = max(dimPoints1[0].x, dimPoints1[1].x);
		field.Right = max(field.Right, dimPoints1[2].x);
		field.Right = max(field.Right, dimPoints1[2].x - dimPoints1[0].x + dimPoints1[1].x);

	}
	void Repos(double dbOffsetX, double dbOffsetY)//--相对偏移复制
	{
		for (int i = 0; i < 4;i++)
		{
			dimPoints1[i].x += dbOffsetX;
			dimPoints1[i].y += dbOffsetY;
		}
	}

	// dAngle - 角度
	void Rotate(double dAngle)
	{
		double dRad = DEG2RAD(dAngle);
		double cx = (dimPoints1[1].x + dimPoints1[2].x) / 2;
		double cy = (dimPoints1[1].y + dimPoints1[2].y) / 2;

		for (int i = 0; i < 4;i++) 
		{
			double dx = dimPoints1[i].x - cx;
			double dy = dimPoints1[i].y - cy;
			dimPoints1[i].x = dx*cos(dRad) - dy*sin(dRad) + cx;
			dimPoints1[i].y = dx*sin(dRad) + dy*cos(dRad) + cy;
		}
	}

	// dAngle - 角度
	void Rotate(double cx, double cy, double dAngle)
	{
		double dRad = DEG2RAD(dAngle);
		for (int i = 0; i < 4; i++)
		{
			double dx = dimPoints1[i].x - cx;
			double dy = dimPoints1[i].y - cy;
			dimPoints1[i].x = dx*cos(dRad) - dy*sin(dRad) + cx;
			dimPoints1[i].y = dx*sin(dRad) + dy*cos(dRad) + cy;
		}
	}

	void RotateP0(double dAngle)
	{
		double cx = dimPoints1[0].x;
		double cy = dimPoints1[0].y;

		for (int i = 0; i < 4; i++)
		{
			double dx = dimPoints1[i].x - cx;
			double dy = dimPoints1[i].y - cy;
			dimPoints1[i].x = dx*cos(dAngle) - dy*sin(dAngle) + cx;
			dimPoints1[i].y = dx*sin(dAngle) + dy*cos(dAngle) + cy;
		}
	}
}xDim_t;


typedef struct _CellArray_t
{
	int32_t nStyle;             // 阵列方式，横向/纵向/旋入/旋出
	int32_t nRowCount;          // 阵列行数
	int32_t nColCount;          // 阵列列数
	int32_t nWaferDeg;          // 单颗芯片角度
	int32_t nPitchType;         // 间距类型 固定为中心间距
	double dCellHight;          // 芯片高度
	double dCellWidth;          // 芯片宽度
	double dbRowPitch;          // 行间距
	double dbColPitch;          // 列间距
	double dbColPitchCS;        // 激光头二的列间距补偿值
	_CellArray_t()
	{
		nStyle = 4;   // 从下往上
		nRowCount = 2;
		nColCount = 2; 
		nPitchType = 0;
		nWaferDeg = 0;
		dCellHight = 6;
		dCellWidth = 6;
		dbRowPitch = 6;
		dbColPitch = 6;
		dbColPitchCS = 0;
	}
}stCellArray_t;

typedef struct _GroupArray_t
{
	int32_t nGroupCount;        // 一个Group中的列数数量，水平方向
	int32_t nGroupCountVer;     // 一个Group中的行数数量，垂直方向
	int32_t nIndentStyle;       // 缩进样式， 0=X轴方向，1=Y轴方向
	int32_t nIterleaveCountX;   // X轴方向，需要缩进的列数
	int32_t nIterleaveCountY;   // Y轴方向，需要缩进的行数
	int32_t nIterleaveDegX;     // X轴方向，缩进去列的芯片角度
	int32_t nIterleaveDegY;     // Y轴方向，缩进去行的芯片角度
	double dbCenterX;           // 底板的中心坐标X
	double dbCenterY;           // 底板的中心坐标Y
	double dbGroupH;            // 底板的高度  
	double dbGroupW;            // 底板的宽度
	double dbGroupDeg;          // 底板的角度
	double dbGroupPitch;        // Group的间距，水平方向
	double dbGroupPitchVer;     // Group的间距，垂直方向
	double dbIterleaveIndentX;  // X轴方向，缩进距离
	double dbIterleaveIndentY;  // Y轴方向，缩进距离
	
	_GroupArray_t()
	{
		nGroupCount = 0;
		nGroupCountVer = 0;
		dbGroupDeg = 0;
		dbCenterX = 0;
		dbCenterY = 0;
		dbGroupH = 16;            
		dbGroupW = 16;            
		dbGroupDeg = 0;                  
		dbGroupPitchVer = 0;
		dbGroupPitch = 0;  
		nIndentStyle = 0;
		nIterleaveCountX = 1;
		nIterleaveCountY = 1;
		nIterleaveDegX = -1;    // -1表示印章方向不变
		nIterleaveDegY = -1;    // -1表示印章方向不变
		dbIterleaveIndentX = 0;
		dbIterleaveIndentY = 0;
	}
}stGroupArray_t;

typedef struct _Array_t {
	stCellArray_t cell;
	stGroupArray_t group;
}xArray_t;

class CWaferGroup;
typedef struct _WaferMeta_t
{    
    uint32_t nFlag;  //--芯片标识，[31:0]按位状态标识;  
	                 //--[0]:是否可打,[1]:内容可见，[2]:是否选中 [3]：所属切割头 0：左 1：右
    xDim_t   xDim;              //--位置信息,视图中显示对应位置的轮廓矩形,支持平移旋转不得缩放    
    CWaferGroup *pWaferGroup;   //--所属组
	int m_nWaferID;        // 芯片模组的编号
	uint32_t nCol, nRow;   //-- 所在芯片组行列位置, 行：最上面是第一行，依次类推，列：最左边是第一列
	int m_nBlockID;        // 芯片所在水平块的编号，用于修改块间距时，修改距离
	int m_nBlockIDVer;     // 芯片所在垂直块的编号，用于修改块间距时，修改距离
	_WaferMeta_t()
	{
		nFlag = 0x0001;
		m_nWaferID = -1;
		m_nBlockID = 0;
		m_nBlockIDVer = 0;
	}
	void Draw(IGraphRender *pTheRender, bool bDrawPin = true, bool bRenderBK = true, bool bMapMode = false);
    //--int32_t Mark(IMarkEngine *pTheEngine);

	void EnableMark(bool Flag)
	{
		if (Flag)
		{
			nFlag |= 0x0001;
		}
		else
		{
			nFlag &= ~0x0001;
		}
	}
	bool IsMarkable()
	{
		return ((nFlag & 0x0001)==0x0001);
	}

	void EnableSel(bool Flag)
	{
		if (Flag)
		{
			nFlag |= 0x0004;
		}
		else
		{
			nFlag &= ~0x0004;
		}
	}
	
	bool IsSel()
	{
		return ((nFlag & 0x0004)==0x0004);
	}

	void SetHead(int iHead)
	{
		nFlag &= ~0x0030;
		nFlag |= (iHead << 4);
	}

	/**
	* 0 未分配  1 主头  2 从头  3 双头
	*/
	int GetHead()
	{
		return ((nFlag >> 4 ) & 0x0003) ;
	}

	// 设置pUnitWafer在CWaferGraphDoc中的编号
	void SetWaferID(int nID)
	{
		m_nWaferID = nID;
	}

	// 获取pUnitWafer在CWaferGraphDoc中的编号
	int GetWaferID()
	{
		return m_nWaferID;
	}

	void DrawBinInfo(IGraphRender* pGraphRender);


}xWaferMeta_t;
/****************************************************************************/
/*
** 芯片组，用来管理一组芯片，组内的芯片由阵列方式添加
*/
typedef struct _WorkItem_t
{
	uint32_t uMarkFlag; // 标记标志，MARKFLAG_ONLY_MARK_VAR只标记变量， MARKFLAG_MARK_ALL标记所有，MARKFLAG_ONLY_MARK_NON_VAR只标记非变量
    int32_t nMasterID;  //--主头芯片编号,负值标记无从头匹配芯片
    int32_t nSlaverID;  //--从头芯片编号,负值标记无主头匹配芯片
}xWorkItem_t;
class CWaferGraph;
class IPlatform;
class CWaferGroup {
protected:
	xArray_t m_ArrayInfo;  // 阵列信息
	bool m_bMapMode = true;

public:
    char sName[MAX_TXT_SIZE];         //--名称

    uint32_t dwColor;        //--颜色
    vector<xWaferMeta_t> vWaferMetas;

	void SetArrayInfo(const xArray_t & xArray);
	void GetArrayInfo(xArray_t & xArray);

	bool SaveFile(IFileWriter *pTheWriter);
	bool LoadFile(IFileReader *pTheReader);

    void Draw(IGraphRender *pTheRender, const FField_t & fClient, bool bDrawPin = true);
    int32_t Mark(IPlatform *pPlatform, const vector<CString>& v2DBarcodeData, const vector<CString>& v2DBarcodeData2, bool bPenOrder = false, bool bUp = true);

	int32_t GetWorkTable(vector<xWorkItem_t> &vWorkItems);
	int32_t GetWorkTableNew(vector<xWorkItem_t> &vWorkItems, bool bUp);

	int32_t GetColCount();   // 获取列数
	int32_t GetRowCount();   // 获取行数
	int32_t GetWaferMetaID(int32_t row, int32_t col);
	double GetCellHeight();
	double GetCellWidth();

	bool Rotate(double cx, double cy, double dbDeg);
	bool Repos(double dx, double dy);
	/**
	* @brief 所有对象绕着自己的中心旋转
	* @param[in] angle 角度
	*/
	bool Rotate(double angle);

	FField_t GetMatrixField() const;
	/**
	* @brief 获取已经使用的激光参数的数量 
	* @param nLayerID 引用变量，如果只有一个激光参数被使用，则返回使用的激光参数ID
	*/
	int GetUsedLaserPenCount(int & nLayerID); 

	void SetMapMode(bool bMap) {
		m_bMapMode = bMap;
	}

 public:
     //CWaferGroup();
     //~CWaferGroup();

	// 下面的函数，废弃
	bool ArrayR(int rc, int cc, double sx, double sy, double dx, double dy, int c, double dp);   // 水平向右
	bool ArrayRG(int rc, int cc, double sx, double sy, double dx, double dy, int c, double dp);  // 水平向右，来回
	bool ArrayD(int rc, int cc, double sx, double sy, double dx, double dy, int c, double dp);   // 垂直向下
	bool ArrayDG(int rc, int cc, double sx, double sy, double dx, double dy, int c, double dp);  // 垂直向下，来回
	bool ArraySO(int rc, int cc, double sx, double sy, double dx, double dy, int c, double dp);  // 螺旋，由外向里
	bool ArraySI(int rc, int cc, double sx, double sy, double dx, double dy, int c, double dp);  // 螺旋，由里向外
	bool ArrayU(int rc, int cc, double sx, double sy, double dx, double dy, int c, double dp);   // 垂直向上

	/**
	* @brief 垂直向上，生成阵列
	* @param sx 左上角印章左上角X坐标
	* @param sy 左上角印章左上角Y坐标
	* @param pitchCol 列间距
	* @param pitchRow 行间距
	*/
	bool ArrayU(double sx, double sy, double pitchCol, double pitchRow);

	// dbColPitchCS  - 激光器2头列间距偏移
	bool ChangeArrayU(int rc, int cc, double dx, double dy, double dbColPitchCS = 0);   // 垂直向上，修改阵列
	bool ChangeArrayU(int rc, int cc, double dx, double dy, double grouppitch, double grouppitchVer, double dbColPitchCS = 0);   // 垂直向上，修改阵列

	/**
	* @brief 修改缩进距离
	* @param xNewArray 新的阵列参数
	*/
	bool ChangeArrayIndent(const xArray_t &xNewArray);

	void PrintHeadID(int nMasterID, int nSlaverID);
};

/****************************************************************************/
/*
** 用来管理芯片组，芯片的添加、删除以组为单位进行,不得单独添加/删除某组内的单个芯片
* 一个Group代表一个轨道的阵列，
*/

// 轨道信息
typedef struct _TrackWay_t {
	int nTrackCount;         // 轨道数量，即工位的数量，默认为1。
	double dbTrackPitchX;    // 轨道之间的水平方向间距。上一个轨道的左边界和下一个轨道的左边界的水平距离
	double dbTrackPitchY;    // 轨道之间的垂直方向间距。上一个轨道的下边界到下一个轨道的上边界的距离
	double dbTrackDeg;       // 轨道之间的相对角度。上一个轨道到下一个轨道的角度
	_TrackWay_t()
	{
		nTrackCount = 1;
		dbTrackPitchX = 0;
		dbTrackPitchY = 0;
		dbTrackDeg = 0;
	}
}stTrackWay_t;

class CWaferMapDoc
{
public:
    vector<CWaferGroup *> vWaferGroups;
	int m_iColor;
	bool m_bArrayChange;
	bool m_IsModify;            //--是否修改过
	int m_iFileIndex;
	int m_iMaxFileIndex;
    CWnd *m_pOwnerWnd;
	vector<DPoint_t> m_vAnchorPoint;   // 标定点数组
	vector<DPoint_t> m_vCCDAnchorPoint;  // CCD实际拍摄的定位点
	stTrackWay_t m_stTrackWay;
	int m_iMarkOrder = 0;   // 标记顺序。0=先打双头，1=先打单头
private:


public:
    CWaferMapDoc();
    ~CWaferMapDoc();
    
	bool GetModified() const;		/*! 获取是否已经修改 */
	void SetModified(bool fModify = true, bool bAddUndo = true);		/*! 设置修改，是否加入撤销队列 */
	bool DeleteAllTempFile();    // 删除所有的临时文件

    bool SaveFile(IFileWriter *pTheWriter);
    bool LoadFile(IFileReader *pTheReader);

	// 撤销恢复功能-保存和加载文件
	bool SaveTmpFile();
	bool LoadTmpFile(int iIndex);

	// CCD定位前后保存和加载文档
	bool SaveOriginDoc();
	bool LoadOriginDoc();

	int32_t GetGroupCount();   // 获取阵列组的数量
	CWaferGroup * GetGroup(int32_t index);  // 获取指定组

	int32_t GetMapObjCount(const char * sName) const;  // 根据芯片模块名称在MAP中查找模块的数量
	int32_t GetMapObjCount(void) const;
	int32_t GetMapObjSelCount(void) const;
	int32_t GetMapObjMarkCount(void) const;
    CWaferGroup *  AppendWaferGroup(const xArray_t &xArray);   //--通过阵列方式添加一组芯片
	void ChangeWaferGroup(const xArray_t &xOldArray, const xArray_t &xNewArray, int iPosit);//修改一组阵列的芯片组
    void RemoveWaferGroup(const char *sName);  //--删除指定组别的所有芯片
    void RemoveAllGroup();
    void Draw(IGraphRender *pTheRender, const FField_t & fClient, bool bDrawPin = true);
	int32_t Mark(const vector<CString>& v2DBarcodeData, const vector<CString>& v2DBarcodeData2, bool bPenOrder, bool bUp);

	int32_t CCDAnchor(void);   // 标记前CCD定位
	int32_t ResetAnchor(void);    // 标记后恢复

    void SetWnd(CWnd *pWnd) { m_pOwnerWnd = pWnd; }
    CWnd *GetWnd(void) { return m_pOwnerWnd; }

	int GetAnchorCount(bool bCCD = false);
	void ClearAnchor(bool bCCD = false);
	void AddAnchor(const DPoint_t & dp, bool bCCD = false);
	void GetAnchor(int id, DPoint_t & dp, bool bCCD = false);

	FField_t GetField(void) const;
	FField_t GetSelField(void) const;
	FField_t GetLeftSelField(void) const;
	FField_t GetRightSelField(void) const;

	FField_t GetMatrixField() const;

	/**
	* @brief 移动所有对象
	* @param[in] iAbs iAbs = 0,相对，iAbs = 1，绝对
	*/
	void Repos(double px, double py, int iAbs); 
	/**
	* @brief 旋转所有对象
	* @param[in] angle 角度
	*/
	void Rotate(double cx, double cy, double angle); 
	/**
	* @brief 移动所有选中对象
	* @param[in] iAbs iAbs = 0,相对，iAbs = 1，绝对
	*/
	void ReposSel(double px, double py, int iAbs);
	/**
	* @brief 旋转所有选中对象
	* @param[in] angle 角度
	*/
	void RotateSel(double cx, double cy, double angle);
	/**
	* @brief 所有对象绕着自己的中心旋转
	* @param[in] angle 角度
	*/
	void Rotate(double angle);

	void ShowPosWnd();   // 显示位置对话框

	// 设置所有单元标记
	void SetPrintAll();

	// 设置所有单元的BIN
	void ChangeWaferID(int iBinID);
};

