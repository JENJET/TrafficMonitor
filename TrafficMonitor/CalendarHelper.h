#pragma once
#define CALENDAR_WIDTH 7
#define CALENDAR_HEIGHT 6

struct DayTraffic
{
	int day;
	__int64 up_traffic;
	__int64 down_traffic;
	bool mixed;
	CRect rect;

	__int64 traffic() const
	{
		return up_traffic + down_traffic;
	}
};

class CCalendarHelper
{
public:
	CCalendarHelper();
	~CCalendarHelper();

	//�Ƿ�Ϊ����
	static bool IsLeapYear(int year);
	//�������ڼ�������(0~6��������~������)
	static int CaculateWeekDay(int y, int m, int d);
	//����һ�����ж�����
	static int DaysInMonth(int year, int month);

	//��ȡָ���·ݵ��������ݣ�������������calendar��
	//���sunday_firstΪtrue����������Ϊÿ�ܵĵ�һ�죬���򣬽���һ��Ϊÿ�ܵĵ�һ��
	static void GetCalendar(int year, int month, DayTraffic calendar[CALENDAR_HEIGHT][CALENDAR_WIDTH], bool sunday_first = true);

};

