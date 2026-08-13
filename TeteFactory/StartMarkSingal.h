#pragma once
class CFormWorkControl;
/**
* @brief 开始标记信号侦测类，内部会启用线程
*/
class CStartMarkSingal
{
	friend class CFormWorkControl;
public:
	CStartMarkSingal(CFormWorkControl* pFormWorkControl);
	~CStartMarkSingal();
public:
	void Init();          // 初始化
	void StartMonitor();  // 开始监听
	void StopMonitor();   // 结束监听
	static UINT __stdcall FuncMonitorThread(LPVOID lpVoid);
private:
	CFormWorkControl* m_pFormWorkControl;
	bool m_bExitThread;  // 监视线程退出标志
	HANDLE m_hThread;    // 监视线程句柄
	bool m_bStartMonitor;// 启用监视标志（不退出线程）
};

