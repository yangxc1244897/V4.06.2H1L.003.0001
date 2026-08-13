#pragma once

class CMySoap
{
public:
	CMySoap(const CString& sUrl);
	~CMySoap();

	CString GetLotInfo(const CString& lotid);
	void SetUrl(const CString& sUrl);
	BOOL IsLotIDMatchStripID(const CString& stripid, CString& lotid);
	BOOL IsEquipmentTrackInLotId(const CString& sEquipmentId, const CString& slotid);
	CString& GetLastError();

private:
	CString m_sUrl;
	CString m_sErrMsg;
};

