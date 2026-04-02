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
    //�رղ�ѯ
    PdhCloseQuery(query);
}

bool CPdhQuery::Initialize()
{
    if (isInitialized)
        return true;

    PDH_STATUS status;
    //�򿪲�ѯ
    status = PdhOpenQuery(NULL, NULL, &query);
    if (status != ERROR_SUCCESS)
        return false;

    //��Ӽ�����
    status = PdhAddCounter(query, fullCounterPath.GetString(), NULL, &counter);
    //�ȵ���PdhAddCounter�����ʧ��ʹ��PdhAddEnglishCounter����һ��
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

    //��ʼ��������
    PdhCollectQueryData(query);
    isInitialized = true;
    return true;
}

bool CPdhQuery::QueryValue(double& value)
{
    if (!isInitialized)
        return false;

    //��������
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

    //��������
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

    // 1. ���豸��� (dwDesiredAccess ��Ϊ 0 ���ɲ�ѯ���ԣ��������ԱȨ��)
    HANDLE hDevice = ::CreateFile(strPath,
        0,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);

    if (hDevice == INVALID_HANDLE_VALUE) return _T("");

    // 2. ���ò�ѯ����
    STORAGE_PROPERTY_QUERY query = { StorageDeviceProperty, PropertyStandardQuery };

    // ׼���㹻��Ļ����� (1KB ͨ���㹻�洢�������������ַ���)
    BYTE buffer[1024] = { 0 };
    DWORD bytesReturned = 0;

    // 3. ���� DeviceIoControl ��ȡ�洢�豸������
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

        // 4. ��ȡ������Ϣ (Vendor ID)
        if (pDesc->VendorIdOffset != 0)
        {
            const char* pVendor = (const char*)(buffer + pDesc->VendorIdOffset);
            strVendor = pVendor;
            strVendor.Trim(); // �������������ص��ַ�������β���ո�
        }

        // 5. ��ȡ��Ʒ�ͺ� (Product ID)
        if (pDesc->ProductIdOffset != 0)
        {
            const char* pProduct = (const char*)(buffer + pDesc->ProductIdOffset);
            strProduct = pProduct;
            strProduct.Trim();
        }

        // 6. ����ƴ��
        if (!strVendor.IsEmpty() && !strProduct.IsEmpty())
        {
            // ����ͺ����Ѿ������˳�������ĳЩ NVMe �����������������ظ�ƴ��
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

    // ���ն��ף�������ǿյģ�����һ��ͨ������
    if (strFullName.IsEmpty()) {
        strFullName.Format(_T("Physical Drive %d"), diskIndex);
    }

    return strFullName;
}