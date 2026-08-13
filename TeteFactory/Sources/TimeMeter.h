/**
******************************************************************************
* @文件 计时器  
* @作者 jchq 
* @版本 V1.00.00
* @日期 2018-04-11
* @简述 
* @email:jchq0101net@sina.com
******************************************************************************
* @说明
*
*
* 
******************************************************************************
*/
#ifndef H_TIME_METER_H
#define H_TIME_METER_H

class CTimeMeter
{
public:
    CTimeMeter();
	~CTimeMeter(void);

    void StartTimer(void);
    double GetTimer(void);
    

private:
    double m_dbFreq;
    double m_dbCount0;
    double m_dbCount1;
};

#endif