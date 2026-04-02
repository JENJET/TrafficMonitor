#pragma once
#include "afxcmn.h"
#include "DrawCommon.h"
class CHistoryTrafficListCtrl :
	public CListCtrl
{
DECLARE_DYNAMIC(CHistoryTrafficListCtrl)
public:
	CHistoryTrafficListCtrl();
	~CHistoryTrafficListCtrl();

	void EnableDrawItemRange(bool draw = true) { m_draw_item_range = draw; }	//�����Ƿ�Ҫ��ĳһ�е�Ԫ���л��Ʊ�ʾ��ֵ��С�ľ���
	void SetDrawItemRangeRow(int row) { m_draw_item_range_row = row; }			//����Ҫ���Ʊ�ʾ��ֵ��С�ľ��ε��У�ע����б���Ϊ���ұߵ��У����򣬴����ұߵ������н��޷���ʾ
	void SetDrawItemRangeData(int item, double range, COLORREF color);			//����ĳһ�е���Ŀ���ݴ�С��ȡֵΪ1~1000������ɫ
	void SetDrawItemRangMargin(int margin) { m_margin = margin; }			//���û��ƻ��Ƶľ��εı�Ե����Ԫ��߿�ľ��Σ���ֵԽ������Ƶľ���Խϸ�����ǲ��ܳ����б��о��һ��
	void SetDrawItemRangInLogScale(bool log_scale);				//����Ҫ���Ʊ�ʾ��ֵ��С�ľ���ʱ�Ƿ�ʹ�ö�������������ʹ�����Ա���

protected:
	struct ItemData
	{
		double data_value;		//Ҫ���Ƶľ��α�ʾ����ֵ��С����ΧΪ0~1000
		COLORREF color;		//Ҫ���Ƶľ��ε���ɫ
	};
	bool m_draw_item_range{ false };		//�Ƿ���Ҫ��ĳһ�е�Ԫ���л��Ʊ�ʾ��ֵ��С�ľ���
	int m_draw_item_range_row{};		//��Ҫ���Ʊ�ʾ��ֵ��С�ľ��ε���
	int m_margin{};
	vector<ItemData> m_item_rage_data;	//���ڱ�ʾÿһ����Ҫ���Ƶ����ݴ�С�ͻ�ͼ����ɫ
	bool m_use_log_scale{ false };		//���Ϊtrue��ʹ�ö����������ƾ��Σ�����ʹ�����Ա���

	DECLARE_MESSAGE_MAP()
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
};

