#pragma once
#include "FactoryConfig.h"
typedef  class CHttpFile* (*PostData)(class CHttpConnection*,const CString& rootObject, const CString& strData, const CString &csLotID);

class CHttpComHT 
{
public:
	CHttpComHT(class CFormWorkControl * pMainDlg);
	~CHttpComHT();
	bool ConnectMes() ;
	bool CloseMes() ;
	bool GetWaferThickness(const CString& sLotID) ;
	// 判断产品类型是否满足要求
	bool CheckProductType(const CString& strLotID, const CString& strProductType, CString& errmsg);
	float ReturnThickNess();
	CString csLotID;
	CFactoryConfig * m_pConfig;
protected:
	static class CHttpFile* WaferThicknessPos(class CHttpConnection* pConnection, const CString& rootObject, const CString& strData, const CString &csLotID);
	
	bool PostDataToServer(PostData pFunc);
private:
	CCriticalSection m_csMes;  // MES访问临界区
	float m_fthickness; //晶圆厚度
	class CFormWorkControl * m_pMainDlg = nullptr;
};

