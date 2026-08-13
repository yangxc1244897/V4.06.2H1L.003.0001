#pragma once
#include "FactoryConfig.h"




#define SETCHANGEDETECT CSettingChangeDetected::Instance()
/**
* @brief 设置被改动侦测类。会记录改变。
* @remark 数据记录以CFactoryConfig为旧数据
*/
class CSettingChangeDetected
{
public:
	static CSettingChangeDetected* Instance();
	static void Delete();
	static CSettingChangeDetected* _Instance;
	~CSettingChangeDetected();

	bool CompareAndChangeBoolVar(const CString& sVariableName, bool bNow);
	bool CompareAndChangeIntVar(const CString& sVariableName, int iNow);
	bool CompareAndChangeDoubleVar(const CString& sVariableName, double dbNow);
	bool CompareAndChangeStringVar(const CString& sVariableName, const CString& sNow);
	void RecordVarChange(const CString& sVariableName);

private:
	CFile m_File;
	CSettingChangeDetected();
	CFactoryConfig* m_FactoryConfig;
	void recordCommonFile(const CStringA& s);
};

