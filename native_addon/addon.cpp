
#include <iostream>
#include <cstdint>
#include <vector>

#include <napi.h>

#ifdef _WIN32
#include <Windows.h>
#include <psapi.h>
#else
#include <sys/resource.h>
#include <sys/sysinfo.h>
#include <sys/time.h>
#endif

struct SystemMemoryInfo
{
    size_t totalPhysicalMemory_;
    size_t freeMemory_;
};

struct ProcessMemoryInfo
{
    size_t usedPhysicalMemory_;
};

bool getSystemMemoryInfo(SystemMemoryInfo* pMemoryInfoOut)
{
#ifdef _WIN32
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    if (!GlobalMemoryStatusEx(&memInfo)) 
    {
        return false;
    }

    pMemoryInfoOut->totalPhysicalMemory_ = memInfo.ullTotalPhys;
    pMemoryInfoOut->freeMemory_ = memInfo.ullAvailPhys;
#else
    struct sysinfo si;
    if (sysinfo(&si) == -1) 
    {
        return false;
    }

    pMemoryInfoOut->totalPhysicalMemory_ = si.totalram * si.mem_unit;
    pMemoryInfoOut->freeMemory_ = si.freeram * si.mem_unit;
#endif

    return true;
}

bool getProcessMemoryInfo(ProcessMemoryInfo* pMemoryInfoOut)
{
#ifdef _WIN32
    HANDLE hProcess = GetCurrentProcess();
    PROCESS_MEMORY_COUNTERS pmc;
    if (!GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) 
    {
        return false;
    }

    pMemoryInfoOut->usedPhysicalMemory_ = pmc.WorkingSetSize;
#else
    long rss = 0L;
    FILE* fp = NULL;
    fp = fopen("/proc/self/statm", "r");
    if (fp == NULL)
    {
        return false;
    }
       
    auto numValuesRead = fscanf(fp, "%*s%ld", &rss);
    fclose(fp);
    if (numValuesRead != 1)
    {        
        return false;
    }
    
    pMemoryInfoOut->usedPhysicalMemory_ = (size_t)rss * (size_t)sysconf(_SC_PAGESIZE);
#endif

    return true;
}

std::vector<std::vector<uint8_t>> memoryBlocks;

Napi::Value allocateMemoryBlock(const Napi::CallbackInfo& info) {

    memoryBlocks.emplace_back(1024*1024*1024, -1);
    std::cout << "Allocated 1GB block. Now " << memoryBlocks.size() << " blocks" << std::endl;

    Napi::Object data = Napi::Object::New(info.Env());

    data.Set("numBlocks", memoryBlocks.size());
    data.Set("totalNumMB", 0);
    data.Set("freeNumMB", 0);
    data.Set("usedNumMB", 0);

    SystemMemoryInfo sysMemInfo;
    ProcessMemoryInfo processMemInfo;
    if (getSystemMemoryInfo(&sysMemInfo) && getProcessMemoryInfo(&processMemInfo))
    {
        float totalNumMB = (float)sysMemInfo.totalPhysicalMemory_/(1024.0f*1024.0f);
        float freeNumMB = (float)sysMemInfo.freeMemory_/(1024.0f*1024.0f);
        float usedNumMB = (float)processMemInfo.usedPhysicalMemory_/(1024.0f*1024.0f);

        std::cout << "Mem MB - Total: " << totalNumMB
            << " Free: " << freeNumMB
            << " Used: " << usedNumMB << std::endl << std::flush;

        data.Set("totalNumMB", totalNumMB);
        data.Set("freeNumMB", freeNumMB);
        data.Set("usedNumMB", usedNumMB);
    }

    return data;
}

Napi::Object initModule(Napi::Env env, Napi::Object exports) 
{
    exports.Set(Napi::String::New(env, "allocateMemoryBlock"), Napi::Function::New(env, allocateMemoryBlock));

    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, initModule)