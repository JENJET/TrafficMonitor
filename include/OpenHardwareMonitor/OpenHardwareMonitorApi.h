#pragma once
#include <memory>
#include "OpenHardwareMonitorGlobal.h"
#include <map>
#include <string>

namespace OpenHardwareMonitorApi
{
    class IOpenHardwareMonitor
    {
    public:
        virtual void GetHardwareInfo() = 0;     //????????
        virtual float CpuPower() = 0;           //??????CPU??
        virtual float CpuTemperature() = 0;     //??????CPU??
        virtual float GpuTemperature() = 0;     //??????GPU??
        virtual float GpuPower() = 0;           //??????GPU??
        virtual float HDDTemperature() = 0;     //??????????
        virtual float MainboardTemperature() = 0;   //??????????
        virtual float GpuUsage() = 0;           //??????GPU???
        virtual float CpuFreq() = 0;            //??????CPU??
        virtual float CpuUsage() = 0;           //?????CPU???
        virtual const std::map<std::wstring, float>& AllHDDTemperature() = 0;   //??????????map?key???????value??????
        virtual const std::map<std::wstring, float>& AllCpuTemperature() = 0;   //????CPU????????map?key?CPU????value??????
        virtual const std::map<std::wstring, float>& AllHDDUsage() = 0;         //??????????

        virtual void SetCpuEnable(bool enable) = 0;
        virtual void SetGpuEnable(bool enable) = 0;
        virtual void SetHddEnable(bool enable) = 0;
        virtual void SetMainboardEnable(bool enable) = 0;
    };

    OPENHARDWAREMONITOR_API std::shared_ptr<IOpenHardwareMonitor> CreateInstance();
    OPENHARDWAREMONITOR_API std::wstring GetErrorMessage();
}

