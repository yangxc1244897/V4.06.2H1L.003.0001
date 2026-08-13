#include "stdAfx.h"
#include "..\stdAfx.h"
#include "TimeMeter.h"
CTimeMeter::CTimeMeter()
{
    LARGE_INTEGER tc;
    QueryPerformanceFrequency(&tc);
    m_dbFreq = (double)tc.QuadPart;
}
CTimeMeter::~CTimeMeter(void)
{
}
void CTimeMeter::StartTimer(void)
{
    LARGE_INTEGER tc;
    QueryPerformanceFrequency(&tc);
    m_dbFreq = (double)tc.QuadPart;
    QueryPerformanceCounter(&tc);
    m_dbCount0 = (double)tc.QuadPart;
    m_dbCount1 = m_dbCount0;
}
double CTimeMeter::GetTimer(void)
{
    LARGE_INTEGER tc;
    QueryPerformanceCounter(&tc);
    m_dbCount1 = (double)tc.QuadPart;

    return (m_dbCount1-m_dbCount0)/m_dbFreq;
}
