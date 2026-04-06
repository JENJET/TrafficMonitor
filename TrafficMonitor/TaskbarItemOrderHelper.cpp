#include "stdafx.h"
#include "TaskbarItemOrderHelper.h"
#include "Common.h"
#include "TrafficMonitor.h"

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

CTaskbarItemOrderHelper::CTaskbarItemOrderHelper(bool displayed_only)
    : m_displayed_only(displayed_only)
{
}

void CTaskbarItemOrderHelper::Init()
{
    m_all_item_in_default_order.clear();
    
    // 遍历所有内置显示项和插件显示项
    for (const auto& item : theApp.m_plugins.AllDisplayItemsWithPlugins())
    {
        // 对于GPU功率项，如果启用了GPU监控且有多个GPU，则为每个GPU创建独立的显示项
        if (!item.IsPlugin() && item.ItemType() == TDI_GPU_POWER && 
            theApp.m_general_data.IsHardwareEnable(HI_GPU) && 
            !theApp.m_all_gpu_power.empty())
        {
            // 为每个GPU创建独立的显示项
            for (const auto& gpu_pair : theApp.m_all_gpu_power)
            {
                CommonDisplayItem gpu_item(TDI_GPU_POWER, gpu_pair.first);
                m_all_item_in_default_order.push_back(gpu_item);
            }
        }
        else
        {
            m_all_item_in_default_order.push_back(item);
        }
    }
    
    // 保存当前的GPU名称列表
    m_last_gpu_names.clear();
    for (const auto& gpu_pair : theApp.m_all_gpu_power)
    {
        m_last_gpu_names.insert(gpu_pair.first);
    }
}

std::vector<CommonDisplayItem> CTaskbarItemOrderHelper::GetAllDisplayItemsWithOrder() const
{
    std::vector<CommonDisplayItem> items;
    for (auto i : m_item_order)
    {
        if (i >= 0 && i < static_cast<int>(m_all_item_in_default_order.size()))
        {
            if (m_displayed_only && !IsItemDisplayed(m_all_item_in_default_order[i]))
            {
                continue;
            }
            items.push_back(m_all_item_in_default_order[i]);
        }
    }

    return items;
}

void CTaskbarItemOrderHelper::FromString(const std::wstring& str)
{
    m_item_order.clear();
    std::vector<std::wstring> str_list;
    CCommon::StringSplit(str, L',', str_list);
    for (const auto& s : str_list)
    {
        m_item_order.push_back(_wtoi(s.c_str()));
    }
    NormalizeItemOrder();
}

std::wstring CTaskbarItemOrderHelper::ToString() const
{
    std::wstring result;
    for (int i : m_item_order)
    {
        result += std::to_wstring(i);
        result.push_back(L',');
    }
    if (!m_item_order.empty())
        result.pop_back();
    return result;
}

void CTaskbarItemOrderHelper::SetOrder(const vector<int>& item_order)
{
    m_item_order = item_order;
    NormalizeItemOrder();
}

const vector<int>& CTaskbarItemOrderHelper::GetItemOrderConst() const
{
    return m_item_order;
}

vector<int>& CTaskbarItemOrderHelper::GetItemOrder()
{
    return m_item_order;
}

CString CTaskbarItemOrderHelper::GetItemDisplayName(CommonDisplayItem item)
{
    return item.GetItemName();
}

bool CTaskbarItemOrderHelper::IsItemDisplayed(CommonDisplayItem item)
{
    bool displayed = true;
    if (!item.IsPlugin())
    {
        if ((item == TDI_CPU_POWER) && !theApp.m_general_data.IsHardwareEnable(HI_CPU))
			displayed = false;
        if ((item == TDI_CPU_TEMP) && !theApp.m_general_data.IsHardwareEnable(HI_CPU))
            displayed = false;
        if ((item == TDI_GPU_POWER) && !theApp.m_general_data.IsHardwareEnable(HI_GPU))
            displayed = false;
        if ((item == TDI_GPU_TEMP) && !theApp.m_general_data.IsHardwareEnable(HI_GPU))
            displayed = false;
        if ((item == TDI_HDD_TEMP) && !theApp.m_general_data.IsHardwareEnable(HI_HDD))
            displayed = false;
        if (item == TDI_MAIN_BOARD_TEMP && !theApp.m_general_data.IsHardwareEnable(HI_MBD))
            displayed = false;
    }

    return displayed;
}

void CTaskbarItemOrderHelper::RefreshGpuPowerItems()
{
    // 检测GPU列表是否发生变化
    std::set<std::wstring> current_gpu_names;
    for (const auto& gpu_pair : theApp.m_all_gpu_power)
    {
        current_gpu_names.insert(gpu_pair.first);
    }
    
    if (current_gpu_names != m_last_gpu_names)
    {
        // GPU列表发生变化，重新初始化
        Init();
        // 规范化索引顺序，确保新添加的GPU项有对应的索引
        NormalizeItemOrder();
    }
}

void CTaskbarItemOrderHelper::NormalizeItemOrder()
{
    //检查是否有超出范围的序号（使用m_all_item_in_default_order的大小）
    int item_num = static_cast<int>(m_all_item_in_default_order.size());
    for (auto iter = m_item_order.begin(); iter != m_item_order.end();)
    {
        if (*iter < 0 || *iter >= item_num)
            iter = m_item_order.erase(iter);
        else
            ++iter;
    }
    //删除不显示的序号
    if (m_displayed_only)
    {
        for (auto iter = m_item_order.begin(); iter != m_item_order.end();)
        {
            if (*iter >= 0 && *iter < static_cast<int>(m_all_item_in_default_order.size()))
            {
                CommonDisplayItem item = m_all_item_in_default_order[*iter];
                if (!IsItemDisplayed(item))
                {
                    iter = m_item_order.erase(iter);
                    continue;
                }
            }
            ++iter;
        }
    }
    //删除重复的序号
    CCommon::RemoveVectorDuplicateItem(m_item_order);
    //检查是否有缺少的序号
    for (int i = 0; i < item_num; i++)
    {
        auto iter = std::find(m_item_order.begin(), m_item_order.end(), i);
        if (iter == m_item_order.end())
            m_item_order.push_back(i);
    }
}
