#pragma once
class CWindowsSettingHelper
{
public:
    static bool IsWindows10LightTheme();				//�ж�Windows�Ƿ�Ϊǳɫ����
    static void CheckWindows10LightTheme();
    static bool IsDotNetFramework4Point5Installed();	//�ж��Ƿ�װ��.Net Framework 4.5 (https://docs.microsoft.com/zh-cn/dotnet/framework/migration-guide/how-to-determine-which-versions-are-installed)
    static bool IsTaskbarShowingInAllDisplays();        //�Ƿ���������ʾ������ʾ��������

    //Windows11�������
    static bool IsTaskbarWidgetsBtnShown();             //�������Ƿ���ʾ�ˡ�С�������ť
    static bool IsTaskbarCenterAlign();                  //�������Ƿ����

private:
    static LONG GetDWORDRegKeyData(HKEY hKey, const wstring& strValueName, DWORD& dwValueData);
    static bool GetDWORDRegKeyData(HKEY keyParent, const wstring& strKeyName, const wstring& strValueName, DWORD& dwValueData);

private:
    static 	bool m_light_theme;
};

