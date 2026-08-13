#pragma once
#include "DB\DBInstance.h"


class C2DContentRepeatCheck
{
public:
	C2DContentRepeatCheck();
	~C2DContentRepeatCheck();

	bool InitDB();
	CString GetLastErrorMsg();
	bool Check(const std::vector<CString>& vs2DData);
	bool Save(const CString& sStripID, const std::vector<CString>& vs2DData);
	bool DeleteDataOutOfDate();
	
	std::vector<CString>data;
private:
	CDBInterface* m_pDBInterface;
	CString m_sErrMsg;
	// 检查单片料的数据是否都不同
	bool checkSingleRepeat(const std::vector<CString>& vs2DData);
	// 检查单片料的数据是否都相同
	bool checkSingleSame(const std::vector<CString>& vs2DData);
	// 获取2D数据中第一个不为空的数据和序号
	bool getFirstNoNullData(const std::vector<CString>& vs2DData, CString& str, int& index);
	
};

