/**
******************************************************************************
* @模块 
* @作者 jchq
* @版本 V1.00.00
* @日期 2018-09-21
* @简述 
* @email:jchq0101net@sina.com
******************************************************************************
* @说明 
*    
*    
*    
*      
*        
*    
*    
******************************************************************************
*/
#ifndef H_PLUGIN_MANAGER_H
#define H_PLUGIN_MANAGER_H
#include "tlTeteDefines.h"

class CPluginManager
{
public:
    enum PluginType {
        emPT_DRIVER = 0,
        emPT_DEVICE,
        emPT_MARKENG
    };
        
public:
    static CPluginManager *Instance();
    ~CPluginManager();

public:  
    bool RegisterPlugin(int32_t iType, const char *sName, const char *sFile);
    bool UnregisterPlugin(int32_t iType, const char *sName);

    const char *GetAllPluginName(int32_t iType);
protected:
    CPluginManager(void);    
private:
    static CPluginManager *_instance;

    void LoadConfig(void);
    void SaveConfig(void);
};

#endif