/**
  ******************************************************************************
  * @模块 
  * @作者 jchq
  * @版本 V1.00.00
  * @日期 2017-09-21
  * @email:jchq0101net@sina.com
  ******************************************************************************
  * @功能描述
  * 
  *   
  *
  *
  *
  ******************************************************************************
  */

#ifndef H_ACTION_FACTORY_H
#define H_ACTION_FACTORY_H

#include <vector>
#include "tlMarkDefine.h"
#include "tlPlatform.h"
#include "MarkAction.h"
 
class CActionFactory
{
public:
    static CActionFactory *Instance();
    ~CActionFactory();
protected:
    CActionFactory(void);    //--
private:
    static CActionFactory *_instance;

public:
    int32_t Open(IPlatform *pPlatform) { m_pPlatform = pPlatform; return 0; }
    IMarkAction *NewAction(int32_t iActionType);       //--
	IMarkAction *GetAction(int32_t iActionID);         //--
	int32_t ActiveAction(int32_t iActionID);
	int32_t GetActionCount(void){ return m_theActionsList.size(); };
	void DeleteAction(IMarkAction *pTheAction);       //--
	IMarkAction *FirstAction(void);
	IMarkAction *NextAction(IMarkAction *pTheAction);
    IMarkAction *LastAction(void);

    void EnableActive(bool IsEnable);

	bool ImportActions(const char *sFileName);
    bool ExportActions(const char *sFileName);

	void SortByID(const std::vector<int> &vid);
private:
    IPlatform *m_pPlatform;
    bool m_IsActiveEnable;
    std::vector<IMarkAction *>  m_theActionsList;
};

#endif

