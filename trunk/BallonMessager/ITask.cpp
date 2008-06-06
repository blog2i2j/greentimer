#include ".\itask.h"

ITask::ITask(void)
{
}

ITask::~ITask(void)
{
}

bool ITask::IsTimeNow()
{
	switch(Type)
	{
	case TT_DAILY:
		{
			CTime tmNow = CTime::GetCurrentTime();
			

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
	}
	return false;
}