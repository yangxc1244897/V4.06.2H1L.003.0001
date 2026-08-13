
#ifndef H_TL_OBJECT_FACTORY_H
#define H_TL_OBJECT_FACTORY_H

#include "tlGraphObject.h"

class TL_API IObjectEngine
{
public:
	virtual IGraphObject * Create(int iObjType) = 0;

	virtual int32_t Open(IPlatform *pPlatform) = 0;
	virtual int32_t Close(void) = 0;
	virtual int32_t GetEngineCtrl(int iCmd, void *pArg) = 0;
	virtual int32_t SetEngineCtrl(int iCmd, void *pArg) = 0;
};


#endif
