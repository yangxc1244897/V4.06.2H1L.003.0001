#pragma once
#include "tlTeteDefines.h"
#include "tlGraphEngine.h"
#include "WaferLayer.h"
#include<vector>
#include<map>
#include <set>
using namespace std;

/**
* @brief 文本对齐功能
*/
enum
{
	TEXTALIGN_LEFT = 0,   // 左对齐
	TEXTALIGN_CENTER,     // 中对齐
	TEXTALIGN_RIGHT,      // 右对齐
	TEXTALIGN_JUSTIFY     // 两端对齐
};

typedef struct TextAlign
{
	BYTE uStyle;       // 对齐类型
	IGraphObject * pObject;
}stTextAlign_t;

/*! 标记标志 */
enum
{
	MARKFLAG_UPDATEPEN = 0x01,   /*! 更新激光参数 */
	MARKFLAG_NONUPDATE = 0x02,       /*! 不更新激光参数 */
	MARKFLAG_ONLY_MARK_VAR = 0x04,  /*! 只标记变量 */
	MARKFLAG_MARK_ALL = 0x08,  /*! 标记所有内容 */
	MARKFLAG_ONLY_MARK_NON_VAR = 0x10,  /*! 只标记相同内容 */
};



/****************************************************************************/
/*! 芯片图，内含若干个图形对象 */
class CWaferGraph 
{
protected:
	bool    m_IsMarkable;          //--是否标记
	bool    m_IsLocked;            //--是否锁定
	bool    m_IsModify;            //--是否修改过
    char   m_sName[MAX_TXT_SIZE];          //--名称

	int32_t m_iUsedCount;          //--被使用数量

	int32_t m_iMarkStartCorner;    //--标记起始角落
	int32_t m_iMarkSortType;       //--标记排序方式：0:R-C,1:C-R
	int32_t m_iMarkDirtType;       //--标记方向：0:单向，1:双向
   
	FField_t  m_stFieldAll;        //--产品区域
	FField_t  m_stFieldSel;        //--选中区域

	double m_dbCenterX;     
	double m_dbCenterY;
	double m_dbRotateA;			   //--中心旋转

	double m_dbHeight;             //--整个文档高度
	double m_dbWidth;              //--整个文档宽度

	std::vector<IGraphObject *> m_theObjects;

	IPlatform  *m_pPlatform;

///////////////////////////////////////////////////
///私有接口
protected:
	void updateDoc(bool IsActive = true);
	void updateRange(void);
	void updateSelRange(void);
	void GetSelObj(std::vector<IGraphObject*>& vpTheObj);
	bool IsLayerMarkable(int32_t iLayer);
	void deleteTextAlign(IGraphObject * pObject);
	void UpdateObjID();   // 删除对象，按照对象在数组中的顺序更新对象ID
	void ChangeCopyObjectName(IGraphObject * pCopyObj);  // 修改复制对象的名称
public:
    CWaferGraph(IPlatform *pPlatform);
    ~CWaferGraph();
	CWaferGraph* ChangeWaferDirAndClone(int iDeg);   // 将印章旋转后，返回克隆后的对象 

	multimap<IGraphObject *, stTextAlign_t> m_mapAlign;  // 文本对象对齐对象

///////////////////////////////////////////////////
///属性接口
public:
	void SetUsed(bool b);	/*! 设置是否使用 */
	bool IsUsed(void) const;		/*! 获取是否使用 */
	void SetLock(bool lock);		/*! 设置锁定 */
	bool IsLocked(void) const;		/*! 获取是否锁定 */
	void SetMarkable(bool mark);		/*! 设置是否标记 */
	bool IsMarkable(void) const;		/*! 获取是否标记 */
	bool GetModified() const;		/*! 获取是否已经修改 */
	void SetModified(bool fModify = true, bool bAddUndo = true);		/*! 设置修改，是否加入撤销队列 */
	void SetName(const char * s);		/*! 设置名称 */
	const char * GetName(void) const;		/*! 获取名称 */
	void SetHeight(double h);		/*! 设置高度 */
	double GetHeight(void) const;		/*! 获取高度 */
	void SetWidth(double w);		/*! 设置宽度 */
	double GetWidth(void) const;		/*! 获取宽度 */

///////////////////////////////////////////////////
///文件的加载与保存，绘制，标记，剪切板
public:    
	void Draw(IGraphRender *pTheRender);
	/**
	* @brief 标记对象
	* @param pTheEngine 标记引擎
	* @param nMarkLayer 标记指定的层。等于-1时标记所有，且可以通过设置nMarkFlag标志位设置激光参数或不设置激光参数。
	* 标记指定层时，忽略nMarkFlag参数，都不更新激光参数。
	* @param nMarkFlag 标记标志。MARKFLAG_UPDATEPEN，标记之前，更新激光参数，只有当nMarkLayer=-1时才有效；
	* MARKFLAG_NONUPDATE，标记之前，不更新激光参数，只有当nMarkLayer=-1时才有效；
	*/
    int32_t Mark(IMarkEngine *pTheEngine, int32_t nMarkLayer = -1, uint32_t nMarkFlag = MARKFLAG_NONUPDATE);
	bool LoadFile(IFileReader *pTheReader);
	bool SaveFile(IFileWriter *pTheWriter);
	bool ImportFile(const char *sName);
	bool ExportFile(const char *sName);

	bool CopyToClipboard();       
	bool CopyFromClipboard();

///////////////////////////////////////////////////
///对象管理
public:
	int32_t GetObjCount(void) const;
	int32_t GetObjSelCount(void) const;
	IGraphObject *AppendObject(int iObjType);
	void AppendObject(IGraphObject *pTheObj);
	bool DeleteObject(int32_t iObjIndex);
	bool DeleteObject(IGraphObject * pObject);
	void DeleteObjectByLayer(int32_t iLayerID);   /*! 删除指定层的对象 */
	bool DeleteSelObjects(void);
	bool DeleteAllObjects(void);
	IGraphObject * GetObject(int32_t iObjIndex);
	IGraphObject * GetObject(const char * sName);
	void UpdateAllObjects(void);
	void UpdateSelObjects(void);

	virtual void MergeSelObjects(void);   //--合并选中的对象
	virtual void SplitSelObjects(void);   //--拆分选中的对象 

	virtual bool ShowAttributeWnd(void); // 显示第一个选中对象或文档的属性框
	virtual void ShowArrayWnd(void);     // 阵列
	virtual void ShowSortWnd(void);      // 排序
	virtual void ShowHatchWnd(void);     // 填充
	virtual void ShowRotateWnd(void);    // 旋转
	virtual void ShowSizeSelWnd(void);   // 位置大小
										
	void SortByIDAsc(void);   // 将文档中的对象按ID的升序排序
	
	bool SelectAllObjects(void);
	bool UnSelectAllObjects(void);

	//--void RemoveSel(void);    
	FField_t GetAllField(void) { updateRange(); return m_stFieldAll; };
	FField_t GetSelField(void) { updateSelRange(); return m_stFieldSel; };

    /**
	* @brief 设置对象内容
	* @param[in] sObjName 对象名称
	* @param[in] pContent 对象内容，如果对象是固定文本，则pContent指向char*类型文本
	*                               如果对象是日期，则pContent指向char*类型日期格式，详细日期格式参见说明书
	*                               如果对象是自动增量，则pContent指向stAutoData_t结构体指针
	*/
	bool SetObjectContent(const char * sObjName, void * pContent);  
	bool GetObjectContent(const char * sObjName, void * pContent);

	bool IsExistVarObj();   // 文档中的对象，是否有变量存在
	bool IsExist2DBarcode();  // 是否存在2D Barcode对象，检查勾选从2DBarcode中获取内容
	bool IsExistTwo2DBarcode();  // 是否存在两组2D Barcode对象
	bool IsExistFirst2DBarcode();  // 是否存在第一组2D Barcode对象
	bool IsExistBarcode();  // 是否存在二维码对象，只要是二维码对象就检查
	/**
	* @brief 替换2D Barcode数据
	* @param[in] index 第几组2D Barcode数据
	* @param[in] pContent 替换内容
	*/
	bool Replace2DBarcodeData(int index, const char* pContent);

	bool IsExistTextIndex();
	// 如果印章中存在序号对象，则替换，不存在则忽略
	bool ReplaceTextIndexData(int index);

	// 获取图档中所有文本对象的名称/内容/对象
	void GetAllTextNameAndContent(std::vector<std::string>& vsrefName
		, std::vector<std::string>& vsrefContent, std::vector<std::string>& fontName);  

	// 获取所有文本对象的字体名称
	void GetAllTextFontName(std::vector<std::string>& fontName);

							 
///////////////////////////////////////////////////
///对象缩放与布局
public:
	void Resise(double dLeft, double dTop, double dRight, double dBottom, int iAbs);//--缩放
	void Repos(double px, double py, int iAbs);         //--移动
	void Rotate(double cx, double cy, double angle, int iAbs);  //--旋转

	void ResiseSel(double dLeft, double dTop, double dRight, double dBottom);//--缩放
	void ReposSel(double px, double py, int iAbs);         //--移动
	void RotateSel(double cx, double cy, double angle, int iAbs);  //--旋转
	//----Objects Layout----
	bool AlignHorLeft(void);       // 水平左对齐
	bool AlignHorCenter(void);     // 水平中对齐
	bool AlignHorRight(void);      // 水平右对齐
	bool AlignVerTop(void);        // 垂直上对齐
	bool AlignVerCenter(void);     // 垂直中对齐
	bool AlignVerBottom(void);     // 垂直下对齐
	bool AlignCenter(void);        // 居中显示
	bool AlignHorAvgSpace(void);       // 水平平均间距
	bool AlignVerAvgSpace(void);       // 垂直平均间距

	// 文字排版，文本内容远程改变后，自动对齐功能
	bool TextAlign(void);  // 文本对齐


///////////////////////////////////////////////////
///文档的撤销与恢复
public:
	int32_t m_uCurrent;              //--临时文件当前序号
	std::vector<int32_t> m_vuUndo;   //--保存撤销记录
	std::vector<int32_t> m_vuRedo;   //--保存恢复记录
	void AddUndoList();
	bool Redo();//--恢复
	bool Undo();//--撤销
	bool CanRedo() const { return (m_vuRedo.size() > 0); }  //是否能够执行恢复动作
	bool CanUndo() const { return (m_vuUndo.size() > 1); }  //是否能够执行撤销动作
	bool SaveTmpLayer();
	bool OpenTmpLayer(int Current);
	bool DeleteAllTempFile();    //删除文档对应的历史记录文件

	IPlatform *GetPlatform(void) { return m_pPlatform; }


};
/****************************************************************************/
/* 单元文档，管理单元 */
const int g_clLayerColor[] = { 0x000000,0xff0000,0x000080,0xff00ff,0x800080,0x008080,0x808000 };

class CWaferGraphDoc
{
protected:
	bool     m_IsModified;      //--文件是否修改过
	int m_nActiveWafer;     // 当前单元文档的ID
	int32_t m_nActiveLayer;        //--当前图层的索引
	vector<CWaferGraphLayer *> m_theLayers;
	char m_sName[MAX_TXT_SIZE];   // 文档名称
    CWnd *m_pOwnerWnd;

	PropertyBuilderByName(bool, b, StripMap, false, protected)
public:
    vector<CWaferGraph *> m_vWafterGraphs;//--单元集

public:
    CWaferGraphDoc();
    ~CWaferGraphDoc();

//////////////////////////////////////////////////////////
///属性操作
public:
	bool GetModified() const; 	/*! 获取是否已经修改 */
	void SetModified(bool fModify = true);		/*! 设置修改*/
	void SetName(const char * s);		/*! 设置名称 */
	const char * GetName(void) const;		/*! 获取名称 */

    void SetWnd(CWnd *pWnd) { m_pOwnerWnd = pWnd; }
    CWnd *GetWnd(void) { return m_pOwnerWnd; }
//////////////////////////////////////////////////////////
///图层管理
protected:
	void UpdateObjectLayerID();                   /*! 将所有对象更新为指定活动层 */
	void DeleteObjectByLayer(int32_t iLayerID);   /*! 删除指定层的对象 */
	void UpdateObjectLayerID(int32_t iLayerID);   /*! 更新层ID大于iLayerID的对象的层ID */
public: 
	int32_t GetLayerCount() const;		/*! 获取图层的数量 */
	CWaferGraphLayer *AppendLayer(void);		/*! 添加一个图层 */
	bool DeleteLayer(int32_t iLayerID);			/*! 删除一个图层 */
	bool DeleteLayer(CWaferGraphLayer * pLayer);  /*! 删除一个图层 */
	void DeleteAllLayer(void);      	/*! 清空所有图层 */
	int32_t ActiveLayerID(void) const;		/*! 获取当前图层的ID */
	CWaferGraphLayer * GetActiveLayer(void);		/*! 获取当前图层的指针 */
	CWaferGraphLayer *GetLayer(int32_t iLayerID);		/*! 获取指定索引的图层 */
	CWaferGraphLayer *SetActiveLayer(int iLayerID);		/*! 设置指定索引的图层为当前图层 */
	CWaferGraphLayer *SetActiveLayer(const char *sPenName);		/*! 设置指定笔名称的图层为当前图层 */
	CWaferGraphLayer *SetActiveLayer(CWaferGraphLayer * pLayer);  /*! 设置指定层为当前层 */
	bool IsExistLayer(const char *sPenNamee, int& irefLayerID);  /*! 判断指定的激光笔名对应的层是否存在，如果存在就返回层号 */

	BOOL IsLayerUsed(int32_t iLayerID);           /*! 判断层是不是已经被使用 */
//////////////////////////////////////////////////////////
///芯片模块管理
public:
	bool LoadFile(IFileReader *pTheReader);
	bool SaveFile(IFileWriter *pTheWriter);
	CWaferGraph * AppendWafer(const char *sDocName);		/*! 添加一个芯片模块 */
	int GetWaferCount(void) const;		/*! 获取所有的模块数量 */
	CWaferGraph * GetActiveWafer(void);		/*! 获取当前的单元模块 */
	CWaferGraph * GetWafer(uint32_t nID);		/*! 获取指定ID的单元模块  */
	CWaferGraph *  SetActiveWafer(int nID);		/*! 设置当前单元模块的ID, 如果设置失败返回false */
	void RemoveWafer(const char *sDocName);			
	void RemoveWafer(uint32_t iDocID);					
	void RemoveAllWafer(void);     
	bool UpdateAllLayerFromDB();

	/*! 加载文档，激光参数，只加载参数名称，不加载内容 */
	bool LoadFileWithoutPen(IFileReader *pTheReader, std::vector<string> & vsPenName);

	bool IsExist2DBarcode();  // 是否存在2D Barcode对象
	bool IsExistBarcode();    // 是否存在二维码对象
	bool IsExistTwo2DBarcode();  // 是否存在两组2D Barcode对象
	bool IsExistFirst2DBarcode();  // 是否存在第一组2D Barcode对象
	/*! 加载激光参数库中的同名参数 */
	bool LoadParamFromDataBase(IMarkPen* pMarkPen);

	// 获取所有变量文本对象的字体名称
	void GetAllTextFontName(std::vector<std::string>& fontName);

	// 获取所有激光参数的名称
	bool GetAllParamValue(std::vector<CString>& sParamName);

};

