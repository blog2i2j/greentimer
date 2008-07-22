#include "stdafx.h"
#include ".\itask.h"

ITask::ITask(void)
{
	LastRunTime = 0;
}

ITask::~ITask(void)
{
}

bool ITask::IsTimeNow()
{

	CTime tmNow = CTime::GetCurrentTime();
	switch(Type)
	{
	case TT_DAILY:
		{
			//ÿ����������ֻҪʱ��ͷ�����ȷ���ɡ�
			if (tmNow.GetHour()==TaskTime.GetHour() 
				&& tmNow.GetMinute()==TaskTime.GetMinute())
			{
				//�����ظ����С�
				if (tmNow.GetYear()==LastRunTime.GetYear() 
					&& tmNow.GetMonth()==LastRunTime.GetMonth()
					&& tmNow.GetDay()==LastRunTime.GetDay()
					&& tmNow.GetHour()==LastRunTime.GetHour()
					&& tmNow.GetMinute()==LastRunTime.GetMinute()
					)
				{
					return false;
				}

				//OK,������ʱ��
				return true;
			}
		}
		break;
	case TT_ONCE:
		{

			//һ������������
			if (LastRunTime!=0)	//�Ѿ����й���
			{
				return false;
			}
			if (tmNow>TaskTime)	//������ʱ�仹û��ʾ���Ǿ͵���ʾ
			{
				return true;
			}
		}
	}
	return false;
}

TCHAR * ITask::GetTaskTypeStr( TaskType type )
{
	switch(type)
	{
	case TT_ONCE:
		return "һ������";
	case TT_DAILY:
		return "ÿ������";
		//case TT_DATE: 
		//	return "��������";
		//case TT_WEEKLY: 
		//	return "ÿ������";
		//case TT_MONTHLY: 
		//	return "ÿ������";
		//case TT_YEARLY:
		//	return "ÿ������";
	default:
		return "δ֪����";
	}
	return "δ֪����";
}

ITask::TaskType ITask::GetTaskType( const string &strName )
{
	if (strName.compare("һ������")==0)
	{
		return TT_ONCE;
	}
	if (strName.compare("ÿ������")==0)
	{
		return TT_DAILY;
	}
	return (TaskType)-1;
}

void ITask::GetTaskTypes( vector<string> &vecTypes )
{
	vecTypes.clear();
	vecTypes.push_back("һ������");
	vecTypes.push_back("ÿ������");
}

bool ITask::operator == (const ITask &task)
{
	return  Id==task.Id
		&& Type==task.Type
		&& TaskTime == task.TaskTime
		&& LastRunTime == task.LastRunTime
		&& CreateTime == task.CreateTime
		&& Tip == task.Tip;	//��ʾ����
}