#pragma once


// CTabCtrlEx

class CTabCtrlEx : public CTabCtrl
{
	DECLARE_DYNAMIC(CTabCtrlEx)

public:
	CTabCtrlEx();
	virtual ~CTabCtrlEx();

	void AddWindow(CWnd* pWnd, LPCTSTR lable_text);		//��ǰtab�ؼ����һ���Ӵ���
	void SetCurTab(int index);
    CWnd* GetCurrentTab();
    void AdjustTabWindowSize();

protected:
    void CalSubWindowSize();

	DECLARE_MESSAGE_MAP()

protected:
	vector<CWnd*> m_tab_list;		//����tab�ؼ�ÿ���Ӵ��ڵ�ָ��
public:
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void PreSubclassWindow();

	CRect m_tab_rect;
    afx_msg void OnSize(UINT nType, int cx, int cy);
};


