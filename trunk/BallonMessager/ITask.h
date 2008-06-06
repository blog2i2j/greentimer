#pragma once
#include <atltime.h>

//��������
class ITask
{
public:
	ITask(void);
	virtual ~ITask(void);

	enum TaskType{
		TT_ONCE = 0,		//������һ��
		TT_DAILY = 1,		//ÿ�����У���ȷ��Ϊ���ӣ�
		TT_DATE = 2,		//���ض�����������һ�Σ���ȷ��Ϊ�죩
		TT_WEEKLY = 3,		//ÿ��һ��
		TT_MONTHLY = 4,		//ÿ��һ��
		TT_YEARLY = 5,		//ÿ��һ��
	};

	int Id;
	int Type;
	CTime TaskTime;
	CTime LastRunTime;
	CTime CreateTime;
	ATL::CString Tip;

	bool IsTimeNow();
};

//ÿ�����е�����
class TaskDaily
{
public:
	int Hour;
	int Minute;
	ATL::CString strTip;
};

////������һ�ε�����
//class TaskOnce
//{
//public:
//	time_t Time;
//	BOOL IsRuned;
//	CString strTip;
//};