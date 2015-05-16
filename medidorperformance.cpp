#include "medidorperformance.h"

MedidorPerformance::MedidorPerformance()
{
    GetSystemInfo(&sysInfo);
    numProcessors = sysInfo.dwNumberOfProcessors;

    GetSystemTimeAsFileTime(&ftime);
    memcpy(&lastCPU, &ftime, sizeof(FILETIME));

    self = GetCurrentProcess();
    GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
    memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
    memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));

    /*PdhOpenQuery(NULL, NULL, &cpuQuery);
    PdhAddCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
    PdhCollectQueryData(cpuQuery);*/
}

/**
 * @brief MedidorPerformance::getTotalMemory
 *
 * Retorna valor total de memoria virtual.
 *
 * @return Memoria virtual total
 */
DWORDLONG MedidorPerformance::getTotalMemory()
{
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile;

    return totalVirtualMem;
}

/**
 * @brief MedidorPerformance::getMemoryUsage
 *
 * Retorna uso total da memoria virtual.
 *
 * @return Uso total da memoria virtual
 */
DWORDLONG MedidorPerformance::getMemoryUsage()
{
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    DWORDLONG virtualMemUsed = memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;

    return virtualMemUsed;
}

/**
 * @brief MedidorPerformance::getMemoryUsageProcess
 *
 * Retorna memoria sendo utiliada por este programa.
 *
 * @return Memoria usada por este programa
 */
SIZE_T MedidorPerformance::getMemoryUsageProcess()
{
    PROCESS_MEMORY_COUNTERS pmc;

    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    SIZE_T virtualMemUsedByMe = pmc.WorkingSetSize;

    return virtualMemUsedByMe;
}

/**
 * @brief MedidorPerformance::getUsoCpu
 *
 * Esta funcao retorna o uso atual da CPU.
 *
 * @return Uso total atual da CPU.
 */
double MedidorPerformance::getUsoCpu()
{
   /* PDH_FMT_COUNTERVALUE counterVal;
    PdhCollectQueryData(cpuQuery);
    PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
    return counterVal.doubleValue;*/
    return 100;
}

/**
 * @brief MedidorPerformance::getCpuProcesso
 *
 * Esta funcao retorna o uso de CPU por este programa.
 *
 * @return Uso da CPU por este processo
 */
double MedidorPerformance::getCpuProcesso()
{
    GetSystemTimeAsFileTime(&ftime);
    memcpy(&now, &ftime, sizeof(FILETIME));

    GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
    memcpy(&sys, &fsys, sizeof(FILETIME));
    memcpy(&user, &fuser, sizeof(FILETIME));
    percent = (sys.QuadPart - lastSysCPU.QuadPart) + (user.QuadPart - lastUserCPU.QuadPart);
    percent /= (now.QuadPart - lastCPU.QuadPart);
    percent /= numProcessors;
    lastCPU = now;
    lastUserCPU = user;
    lastSysCPU = sys;

    return percent * 100;
}
