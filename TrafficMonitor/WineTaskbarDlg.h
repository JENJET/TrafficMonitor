#pragma once
#include "TaskBarDlg.h"
class CWineTaskbarDlg :
    public CTaskBarDlg
{
private:
    // ͨ�� CTaskBarDlg �̳�
    void InitTaskbarWnd() override;
    void AdjustTaskbarWndPos(bool force_adjust) override;
    void ResetTaskbarPos() override;
    void CheckTaskbarOnTopOrBottom() override;
    HWND GetParentHwnd() override;
};

