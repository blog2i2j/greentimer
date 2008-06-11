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
	static TCHAR *GetTaskTypeStr(TaskType type)
	{
		switch(type)
		{
		case TT_ONCE:
			return "һ������";
		case TT_DAILY:
			return "ÿ������";
		case TT_DATE: 
			return "��������";
		case TT_WEEKLY: 
			return "ÿ������";
		case TT_MONTHLY: 
			return "ÿ������";
		case TT_YEARLY:
			return "ÿ������";
		default:
			return "δ֪����";
		}
		return "δ֪����";
	}

	ATL::CString GetTimeStr() const
	{
		if (Type==TT_DAILY)
		{
			return TaskTime.Format("%H:%M");
		}
		return TaskTime.Format("%y-%m-%d %H:%M:%S");
	}

	int Id;
	TaskType Type;
	CTime TaskTime;
	CTime LastRunTime;
	CTime CreateTime;
	ATL::CString Tip;

	bool IsTimeNow();
};

//ÿ�����е�����
//class TaskDaily
//{
//public:
//	int Hour;
//	int Minute;
//	ATL::CString strTip;
//};

////������һ�ε�����
//class TaskOnce
//{
//public:
//	time_t Time;
//	BOOL IsRuned;
//	CString strTip;
//};