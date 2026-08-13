/**
******************************************************************************
* @文件   OptoWave Laser Config Class
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
#ifndef H_LASER_MODULE_H
#define H_LASER_MODULE_H

#include <vector>
#include "tlTeteDefines.h"
/*--------------------------------------------------------------*/
typedef struct
{
    char sName[MAX_TEXT_SIZE];
    char sFile[MAX_FILE_NAME];
}tlModule_t;
class CLaserModule
{
public:
    CLaserModule();
    ~CLaserModule();

public:
    tlModule_t GetDefault(void);
    void SetDefault(const tlModule_t *pModule);

    int32_t QueryModule(int32_t iID,tlModule_t * pModule);
    int32_t QueryModule(const char *name,tlModule_t * pModule);
    int32_t LoadModule(tlModule_t * pModule);
    int32_t SaveModule(const tlModule_t * pModule);
    int32_t ModifyModule(const char *name, const tlModule_t *pModule);
    int32_t RemoveModule(int32_t iID);
    int32_t GetAllModule(std::vector<tlModule_t> &vModules);
private:
    char m_sFileName[512];
};

#endif

