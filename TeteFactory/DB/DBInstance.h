#ifndef MES_DB_INSTANCE_H_
#define MES_DB_INSTANCE_H_
#include "DBInterface.h"

/*! 创建DB实例, 单例 */
#define DBINSTANCE CDBInstance::Instance()
class CDBInstance
{
public:
	static CDBInstance* Instance();
	CDBInterface * CreateDBInstance(const emDataBaseType_t& emDBType);
	void DeleteDBInstance(CDBInterface * p);

private:
	CDBInstance() {}
};



#endif
