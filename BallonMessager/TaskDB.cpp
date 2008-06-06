#include ".\taskdb.h"
#include <ATLComTime.h>
#include "SQLite/CppSQLite3.h"

TaskDB g_TaskDB;

//////////////////////////////////////////////////////////////////////////
//ʱ�����ַ�������ת������
ATL::CString TimeToString(CTime t)
{
	ATL::CString strDateTime;
	strDateTime.Format("%d-%02d-%02d %02d:%02d:%02d", 
		t.GetYear(), t.GetMonth(), t.GetDay(),
		t.GetHour(), t.GetMinute(), t.GetSecond());
	return strDateTime;
}

CTime StringToTime(ATL::CString strTime)
{;  
	COleDateTime   tm;  
	tm.ParseDateTime(strTime);  
	SYSTEMTIME   st;  
	tm.GetAsSystemTime(st);  
	return CTime(st);   
}
//////////////////////////////////////////////////////////////////////////


bool WriteTaskToDB(CppSQLite3DB &dbTask, const ITask &task)
{
	ATL::CString strTime = TimeToString(CTime::GetCurrentTime());
	CppSQLite3Buffer strSql;

	//"id integer PRIMARY KEY AUTOINCREMENT, "	//����Ψһid
	//	"type integer, "			//���е�type�ֶ���ITask::TaskType���塣
	//	"time char[32], "			//����ʱ�䡣ʱ����"��-��-�� ʱ:��:��"�����ĸ�ʽ���棬��ͬ
	//	"last_run_time char[32], "	//���ִ��ʱ��
	//	"task_create_time char[32], "	//���񴴽�ʱ��
	//	"tip char[2048] );"			//��ʾ���
	strSql.format("insert into T_task values(NULL,%d, '%q','%q','%q','%q');",
		task.Type,
		TimeToString(task.TaskTime),
		TimeToString(task.LastRunTime),
		TimeToString(task.CreateTime),
		task.Tip
		);
	try{
		dbTask.execDML(strSql);
	}
	catch(CppSQLite3Exception &exp)
	{
		ATLTRACE("error:%s\n",exp.errorMessage());
		ATLASSERT(FALSE);
		return false;
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////

TaskDB::TaskDB(void)
{
}

TaskDB::~TaskDB(void)
{
}

bool TaskDB::ReadFromDB( const char *strDB )
{
	LOCK_THIS_RANGE(m_cs);

	//������
	CppSQLite3DB dbTask;
	dbTask.open(strDB);

	//�����񲻴��ڣ���˵�������ݿ��ʽ����
	if (!dbTask.tableExists("T_task"))                //�����¼���־��
	{
		//////////////////////////////////////////////////////////////////////////
		//����Ĭ�ϵĳ�ʼ����
		//0-55,"ע�����壬����Ϣ��
		//1-55,"ע�����壬����Ϣ��
		//2-55  = ע�����壬����Ϣ��
		//3-55  = ע�����壬����Ϣ��
		//4-55  = ע�����壬����Ϣ��
		//5-55  = ���Ϻã��������
		//6-55  = ���Ϻá�
		//7-55  = ���Ϻá�
		//8-55  = �ϰ��ˣ��ȶ��������ƻ�
		//9-55  = �г���Ϣһ����ɡ�
		//10-55 = ��������Сʱ�ˣ�����Щ�˶���!
		//11-55 = �°��ˣ��ú���Ϣ��
		//12-55 = ������Ϣһ�£������Ǹ����ı�Ǯ
		//13-55 = �����ƻ���������ʲô��
		//14-55 = �����ȿ�ˮ����������������һ��
		//15-55 = ��Ϣһ�¡�
		//16-55 = ��Ϣһ�¡�
		//17-55 = �°�ʱ�䵽������һ����ͷ�Ĺ���������������ؼҰɡ�
		//18-55 = �ڹ������������֣���������Ŷ��
		//19-55 = �ڹ������������֣�����ڼҾͳ�ȥɢɢ���ɡ�
		//10-55 = ��Щ���飬�ǿ�����������������
		//21-55 = �ܽ�һ�½���Ĺ�����дһ���ռǣ�׼����Ϣ����
		//22-55 = ҹ�ˣ����ƣ�˯����
		//23-55 = ע�����壬����Ϣ��
		AddDailyTask(0,55,"ע�����壬����Ϣ��");
		AddDailyTask(1,55,"ע�����壬����Ϣ��");
		AddDailyTask(2,55,"ע�����壬����Ϣ��");
		AddDailyTask(3,55,"ע�����壬����Ϣ��");
		AddDailyTask(4,55,"ע�����壬����Ϣ��");
		AddDailyTask(5,55,"���Ϻã��������");
		AddDailyTask(6,55,"���Ϻã��������");
		AddDailyTask(7,55,"���Ϻã��������");
		AddDailyTask(8,55,"�ϰ��ˣ��ȶ��������ƻ�");
		AddDailyTask(9,55,"�г���Ϣһ����ɡ�");
		AddDailyTask(10,55,"��������Сʱ�ˣ�����Щ�˶���!");
		AddDailyTask(11,55,"�°��ˣ��ú���Ϣ��");
		AddDailyTask(12,55,"������Ϣһ�£������Ǹ����ı�Ǯ");
		AddDailyTask(13,55,"�����ƻ���������ʲô��");
		AddDailyTask(14,55,"�����ȿ�ˮ����������������һ��");
		AddDailyTask(15,55,"��Ϣһ�¡�");
		AddDailyTask(16,55,"��Ϣһ�¡�");
		AddDailyTask(17,55,"�°�ʱ�䵽������һ����ͷ�Ĺ���������������ؼҰɡ�");
		AddDailyTask(18,55,"�ڹ������������֣���������Ŷ��");
		AddDailyTask(19,55,"�ڹ������������֣�����ڼҾͳ�ȥɢɢ���ɡ�");
		AddDailyTask(10,55,"��Щ���飬�ǿ�����������������");
		AddDailyTask(21,55,"�ܽ�һ�½���Ĺ�����дһ���ռǣ�׼����Ϣ����");
		AddDailyTask(22,55,"ҹ�ˣ����ƣ�˯����");
		AddDailyTask(23,55,"ע�����壬����Ϣ��");
		//////////////////////////////////////////////////////////////////////////
		return true;
	}
	CppSQLite3Query q = dbTask.execQuery("select id, type, time, last_run_time, task_create_time, tip from T_task");
	while (!q.eof())
	{
		ITask task;
		task.Id = q.getIntField("id");
		task.Type = q.getIntField("type");
		task.TaskTime = StringToTime(q.getStringField("time"));
		task.LastRunTime = StringToTime(q.getStringField("last_run_time"));
		task.CreateTime = StringToTime(q.getStringField("task_create_time"));
		task.Tip = q.getStringField("tip");

		m_vecTask.push_back(task);

		q.nextRow();
	}
	return true;
}

bool TaskDB::SaveToDB( const char *strDB )
{
	LOCK_THIS_RANGE(m_cs);
	//������
	CppSQLite3DB dbTask;
	dbTask.open(strDB);

	dbTask.execDML("begin;");                                 //��ʼ��������

	//�����񲻴��ڣ��򴴽����
	if (!dbTask.tableExists("T_task"))                //�����¼���־��
	{
		//���ݿ��ֶΣ�����id���������ͣ�����ʱ�䣬�ϴ���ʾʱ�䣬��ʾ���ȡ�
		//last_run_time������������ȷ����ʾ�Ƿ��Ѿ�ִ�У������ظ�
		dbTask.execDML("Create table T_task("
			"id integer PRIMARY KEY AUTOINCREMENT, "	//����Ψһid
			"type integer, "			//���е�type�ֶ���ITask::TaskType���塣
			"time char[32], "			//����ʱ�䡣ʱ����"��-��-�� ʱ:��:��"�����ĸ�ʽ���棬��ͬ
			"last_run_time char[32], "	//���ִ��ʱ��
			"task_create_time char[32], "	//���񴴽�ʱ��
			"tip char[2048] );"			//��ʾ���
			);

		//Ϊ�����ֶν�������
		dbTask.execDML("create index idx_type on T_task(type);");
	}
	else
	{
		//��վ�����
		dbTask.execDML("delete from T_task;");
	}

	std::vector<ITask>::iterator it = m_vecTask.begin();
	for (;it!=m_vecTask.end();it++)
	{
		WriteTaskToDB(dbTask,*it);
	}

	dbTask.execDML("end;");                                    //�������������ύ����
	return true;
}

int TaskDB::AddTask(const ITask &task )
{
	LOCK_THIS_RANGE(m_cs);

	int id = 0;
	if (!m_vecTask.empty())
	{
		id = m_vecTask.back().Id + 1;
	}
	m_vecTask.push_back(task);
	m_vecTask.back().Id = id;
	return -1;
}

bool TaskDB::RemoveTask( int idTask )
{
	LOCK_THIS_RANGE(m_cs);

	std::vector<ITask>::iterator it = m_vecTask.begin();
	for (;it!=m_vecTask.end();it++)
	{
		if(idTask==(*it).Id)
		{
			m_vecTask.erase(it);
			return true;
		}
	}
	return false;
}

void TaskDB::GetTaskList( std::vector<int> &vecIdOut )
{
	LOCK_THIS_RANGE(m_cs);

	vecIdOut.clear();
	std::vector<ITask>::iterator it = m_vecTask.begin();
	for (;it!=m_vecTask.end();it++)
	{
		vecIdOut.push_back((*it).Id);
	}
}

bool TaskDB::GetTask(int idTask,ITask &task)
{
	LOCK_THIS_RANGE(m_cs);

	std::vector<ITask>::iterator it = m_vecTask.begin();
	for (;it!=m_vecTask.end();it++)
	{
		if(idTask==(*it).Id)
		{
			task = *it;
			return true;
		}
	}
	return false;
}

bool TaskDB::FindTaskRunNow(ITask &task)
{
	LOCK_THIS_RANGE(m_cs);

	std::vector<ITask>::iterator it = m_vecTask.begin();
	for (;it!=m_vecTask.end();it++)
	{
		if ((*it).IsTimeNow())
		{
			task = *it;
			return true;
		}
	}
	return false;
}

bool TaskDB::AddDailyTask( int iHour, int iMinute, ATL::CString strMessage )
{
	ITask task;
	task.Type = ITask::TT_DAILY;
	task.CreateTime = CTime::GetCurrentTime();
	task.TaskTime = CTime(2000,1,1,iHour,iMinute,0);
	task.LastRunTime = task.CreateTime;
	task.Tip = strMessage;
	AddTask(task);
	return false;
}

bool TaskDB::UpdateTask( const ITask &task )
{
	LOCK_THIS_RANGE(m_cs);

	std::vector<ITask>::iterator it = m_vecTask.begin();
	for (;it!=m_vecTask.end();it++)
	{
		if(task.Id==(*it).Id)
		{
			*it = task;
			return true;
		}
	}
	return false;
}