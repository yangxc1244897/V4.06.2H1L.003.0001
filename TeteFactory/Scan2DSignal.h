#pragma once
class CFormWorkControl;
/**
* @brief 2D信号侦测类，内部会启用线程
*/
class CScan2DSignal
{
	friend class CFormWorkControl;
public:
	CScan2DSignal(CFormWorkControl* pFormWorkControl);
	~CScan2DSignal();
    
	/**
	* @brief 初始化，启用扫描线程
	*/
	void Init();

	/**
	* @brief 开启侦测信号
	*/
	void StartDetect();
	/**
	* @brief 结束侦测信号
	*/
	void EndDetect();
	static UINT __stdcall s_fncDetectThread(LPVOID lpVoid);

private:
	CFormWorkControl* m_pFormWorkControl;
	bool m_bExitThread;
	HANDLE m_hThread;
	bool m_bStartDetect;
};

