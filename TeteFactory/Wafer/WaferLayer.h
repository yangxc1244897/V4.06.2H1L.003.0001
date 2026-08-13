
#ifndef H_GRAPH_LAYER_H
#define H_GRAPH_LAYER_H

class IMarkPen;
class IFileReader;
class IFileWriter;
class CWaferGraphLayer
{
protected:
	bool m_IsLocked;
	bool m_IsVisible;
	bool m_IsMarkable;
    char m_sName[MAX_TXT_SIZE];
    int32_t  m_iLineColor;    //--线条颜色   
    IMarkPen  *m_pThePen;      //--通过ID获取激光器类中对应的笔
public:
	CWaferGraphLayer(void);
    ~CWaferGraphLayer(void);
public:  
	void SetName(const char *sName);
	const char *GetName(void) const;
	void SetPen(IMarkPen *pThePen);
	IMarkPen *GetPen(void);
	void SetLock(bool lock = true);
	bool IsLocked(void) const;
	void SetVisible(bool visible = true);
	bool IsVisible(void) const;
	void SetMarkable(bool mark = true);
	bool IsMarkable(void) const;

	bool LoadFile(IFileReader *pTheReader);
	bool SaveFile(IFileWriter *pTheWriter);
};


#endif
