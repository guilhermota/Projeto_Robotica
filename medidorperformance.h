#ifndef MEDIDORPERFORMANCE_H
#define MEDIDORPERFORMANCE_H

#include "Windows.h"
#include "Psapi.h"
#include "tchar.h"
#include "Pdh.h"

class MedidorPerformance
{
public:
    MedidorPerformance();
    DWORDLONG getTotalMemory();
    DWORDLONG getMemoryUsage();
    SIZE_T getMemoryUsageProcess();
    double getUsoCpu();
    double getCpuProcesso();
    void init();

private:
    ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
    int numProcessors;
    HANDLE self;
    SYSTEM_INFO sysInfo;
    FILETIME ftime, fsys, fuser;
    ULARGE_INTEGER now, sys, user;
    double percent;
    /*PDH_HQUERY cpuQuery;
    PDH_HCOUNTER cpuTotal;*/

};

#endif // MEDIDORPERFORMANCE_H
