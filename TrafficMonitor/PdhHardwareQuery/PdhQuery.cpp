#include "stdafx.h"
#include "PdhQuery.h"

#include <winioctl.h>

CPdhQuery::CPdhQuery(LPCTSTR _fullCounterPath)
    : fullCounterPath(_fullCounterPath)
{
    Initialize();
}

CPdhQuery::~CPdhQuery()
{
    //关闭查询
    PdhCloseQuery(query);
}

bool CPdhQuery::Initialize()
{
    if (isInitialized)
        return true;

    PDH_STATUS status;
    //打开查询
    status = PdhOpenQuery(NULL, NULL, &query);
    if (status != ERROR_SUCCESS)
        return false;

    //添加计数器
    status = PdhAddCounter(query, fullCounterPath.GetString(), NULL, &counter);
    //先调用PdhAddCounter，如果失败使用PdhAddEnglishCounter再试一次
    if (status != ERROR_SUCCESS)
    {
        status = PdhAddEnglishCounter(query, fullCounterPath.GetString(), NULL, &counter);
        if (status != ERROR_SUCCESS)
        {
            PdhCloseQuery(query);
            query = nullptr;
            return false;
        }
    }

    //初始化计数器
    PdhCollectQueryData(query);
    isInitialized = true;
    return true;
}

bool CPdhQuery::QueryValue(double& value)
{
    if (!isInitialized)
        return false;

    //更新数据
    PdhCollectQueryData(query);
    PDH_FMT_COUNTERVALUE pdhValue;
    DWORD dwValue;
    PDH_STATUS status = PdhGetFormattedCounterValue(counter, PDH_FMT_DOUBLE, &dwValue, &pdhValue);
    if (status != ERROR_SUCCESS)
    {
        return false;
    }
    value = pdhValue.doubleValue;
    return true;
}

bool CPdhQuery::QueryValues(std::vector<CounterValueItem>& values)
{
    values.clear();
    if (!isInitialized)
        return false;

    //更新数据
    PdhCollectQueryData(query);
    DWORD dwBufferSize = 0;         // Size of the pItems buffer
    DWORD dwItemCount = 0;          // Number of items in the pItems buffer
    PDH_FMT_COUNTERVALUE_ITEM* pItems = NULL;
    PDH_STATUS status = PdhGetFormattedCounterArray(counter, PDH_FMT_DOUBLE, &dwBufferSize, &dwItemCount, pItems);
    if (PDH_MORE_DATA == status)
    {
        pItems = (PDH_FMT_COUNTERVALUE_ITEM*)malloc(dwBufferSize);
        if (pItems)
        {
            status = PdhGetFormattedCounterArray(counter, PDH_FMT_DOUBLE, &dwBufferSize, &dwItemCount, pItems);
            if (ERROR_SUCCESS == status)
            {
                // Loop through the array and print the instance name and counter value.
                for (DWORD i = 0; i < dwItemCount; i++)
                {
                    CounterValueItem value_item;
                    value_item.name = pItems[i].szName;
                    value_item.value = pItems[i].FmtValue.doubleValue;
                    auto name = GetDiskNameByIndex(i);
                    if (!name.IsEmpty())
						value_item.name = std::wstring(name);
                    else
                        continue;
                    values.push_back(value_item);
                }
            }
            else
            {
                return false;
            }

            free(pItems);
            pItems = NULL;
            dwBufferSize = dwItemCount = 0;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
}

CString CPdhQuery::GetDiskNameByIndex(int diskIndex)
{
    CString strFullName = _T("");
    CString strPath;
    strPath.Format(_T("\\\\.\\PhysicalDrive%d"), diskIndex);

    // 1. 打开设备句柄 (dwDesiredAccess 设为 0 即可查询属性，无需管理员权限)
    HANDLE hDevice = ::CreateFile(strPath,
        0,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);

    if (hDevice == INVALID_HANDLE_VALUE) return _T("");

    // 2. 配置查询参数
    STORAGE_PROPERTY_QUERY query = { StorageDeviceProperty, PropertyStandardQuery };

    // 准备足够大的缓冲区 (1KB 通常足够存储描述符及所有字符串)
    BYTE buffer[1024] = { 0 };
    DWORD bytesReturned = 0;

    // 3. 调用 DeviceIoControl 获取存储设备描述符
    if (::DeviceIoControl(hDevice,
        IOCTL_STORAGE_QUERY_PROPERTY,
        &query, sizeof(query),
        buffer, sizeof(buffer),
        &bytesReturned,
        NULL))
    {
        STORAGE_DEVICE_DESCRIPTOR* pDesc = (STORAGE_DEVICE_DESCRIPTOR*)buffer;
        CString strVendor = _T("");
        CString strProduct = _T("");

        // 4. 获取厂家信息 (Vendor ID)
        if (pDesc->VendorIdOffset != 0)
        {
            const char* pVendor = (const char*)(buffer + pDesc->VendorIdOffset);
            strVendor = pVendor;
            strVendor.Trim(); // 物理驱动器返回的字符串常带尾部空格
        }

        // 5. 获取产品型号 (Product ID)
        if (pDesc->ProductIdOffset != 0)
        {
            const char* pProduct = (const char*)(buffer + pDesc->ProductIdOffset);
            strProduct = pProduct;
            strProduct.Trim();
        }

        // 6. 智能拼接
        if (!strVendor.IsEmpty() && !strProduct.IsEmpty())
        {
            // 如果型号中已经包含了厂家名（某些 NVMe 驱动会这样），则不重复拼接
            if (strProduct.Find(strVendor) != -1)
                strFullName = strProduct;
            else
                strFullName = strVendor + _T(" ") + strProduct;
        }
        else if (!strProduct.IsEmpty())
        {
            strFullName = strProduct;
        }
        else
        {
            strFullName = strVendor;
        }
    }

    ::CloseHandle(hDevice);

    // 最终兜底：如果还是空的，返回一个通用描述
    if (strFullName.IsEmpty()) {
        strFullName.Format(_T("Physical Drive %d"), diskIndex);
    }

    return strFullName;
}