#pragma once
#include "CommonData.h"
class CHistoryTrafficFile
{
public:
	CHistoryTrafficFile(const wstring& file_path);
	~CHistoryTrafficFile();

	void Save() const;
	void Load();
	void LoadSize();			//����ȡ�ļ��Ĵ�С
	void Merge(const CHistoryTrafficFile& history_traffic, bool ignore_same_data = false);		//�ϲ���һ��CHistoryTrafficFile�������ignore_same_dataΪtrue���������ͬ���ڵ��������ͬ���ڵ������������

	const wstring& GetFilePath() const { return m_file_path; }
	const void SetFilePath(const wstring& file_path) { m_file_path = file_path; }
	deque<HistoryTraffic>& GetTraffics() { return m_history_traffics; }
	__int64 GetTodayUpTraffic() const { return m_today_up_traffic; }
	__int64 GetTodayDownTraffic() const { return m_today_down_traffic; }
	size_t Size() { return m_size; }

private:
	void MormalizeData();		//����ʷ�����������򲢺ϲ���ͬ��

private:
	wstring m_file_path;
	deque<HistoryTraffic> m_history_traffics;	//������ʷ����
	__int64 m_today_up_traffic{};	//������ʹ�õ��ϴ�����
	__int64 m_today_down_traffic{};	//������ʹ�õ���������
	size_t m_size{};				//�������ݵ�����
};

