#include "StdAfx.h"
#include ".\todotask.h"

#include "SQLite/CppSQLite3.h"
#include "GlobeFuns.h"

using namespace std;

TodoSet g_todoSet;

ToDoTask::ToDoTask(void)
{
	id = ERROR_TASKID;
}

ToDoTask::ToDoTask(int taskid)
{
	this->id = taskid;
}
ToDoTask::~ToDoTask(void)
{
}

bool ToDoTask::operator < (const ToDoTask &task) const
{
	return id<task.id;
}
//
//
//bool WriteToDoToDB(CppSQLite3DB &dbTodo, const ToDoTask &todo)
//{
//	CppSQLite3Buffer strSql;
//
//	//"id integer PRIMARY KEY AUTOINCREMENT, "	//����Ψһid
//	//	"title char[128], "			//����
//	//	"priority integer, "		//���ȼ�
//	//	"state integer, "			//״̬
//	//	"time_create char[32], "	//���񴴽�ʱ�䡣ʱ����"��-��-�� ʱ:��:��"�����ĸ�ʽ���棬��ͬ
//	//	"time_start char[32], "		//��ʼʱ��
//	//	"time_finish char[32], "	//����ʱ��
//	//	"remark char[64*1024]);"	//��ʾ���
//	strSql.format("insert into T_todo values(NULL,'%q',%d,%d,'%q','%q','%q','%q');",
//		//todo.id,
//		todo.strTask.c_str(),
//		todo.priority,
//		todo.state,
//		GlobeFuns::TimeToString(todo.tmCreateTime),
//		GlobeFuns::TimeToString(todo.tmStartWorkTime),
//		GlobeFuns::TimeToString(todo.tmPlanFinshTime),
//		todo.strRemark.c_str()
//		);
//	try{
//		dbTodo.execDML(strSql);
//	}
//	catch(CppSQLite3Exception &exp)
//	{
//		exp;
//		ATLTRACE("error:%s\n",exp.errorMessage());
//		ATLASSERT(FALSE);
//		return false;
//	}
//	return true;
//}
bool UpdateToDB(CppSQLite3DB &dbTodo, const ToDoTask &todo)
{
	CppSQLite3Buffer strSql;

	//"id integer PRIMARY KEY AUTOINCREMENT, "	//����Ψһid
	//	"title char[128], "			//����
	//	"priority integer, "		//���ȼ�
	//	"state integer, "			//״̬
	//	"time_create char[32], "	//���񴴽�ʱ�䡣ʱ����"��-��-�� ʱ:��:��"�����ĸ�ʽ���棬��ͬ
	//	"time_start char[32], "		//��ʼʱ��
	//	"time_finish char[32], "	//����ʱ��
	//	"remark char[64*1024]);"	//��ʾ���
	strSql.format("update T_todo set"
		" title='%q', priority=%d, state=%d,"
		" time_create='%q',time_start='%q',time_finish='%q',remark='%q'"
		" where id=%d;",
		todo.strTask.c_str(),
		todo.priority,
		todo.state,
		GlobeFuns::TimeToString(todo.tmCreateTime),
		GlobeFuns::TimeToString(todo.tmStartWorkTime),
		GlobeFuns::TimeToString(todo.tmPlanFinshTime),
		todo.strRemark.c_str(),
		todo.id
		);
	try{
		return 1==dbTodo.execDML(strSql);
	}
	catch(CppSQLite3Exception &exp)
	{
		exp;
		ATLTRACE("error:%s\n",exp.errorMessage());
		ATLASSERT(FALSE);
		return false;
	}
}

bool CheckCreateTable(CppSQLite3DB &dbTask)
{
	try{
		//�����񲻴��ڣ��򴴽����
		if (!dbTask.tableExists("T_todo"))                //�����¼���־��
		{
			//���ݿ��ֶΣ�����id���������ͣ�����ʱ�䣬�ϴ���ʾʱ�䣬��ʾ���ȡ�
			//last_run_time������������ȷ����ʾ�Ƿ��Ѿ�ִ�У������ظ�
			dbTask.execDML("Create table T_todo("
				"id integer PRIMARY KEY AUTOINCREMENT, "	//����Ψһid
				"title char[128], "			//����
				"priority integer, "		//���ȼ�
				"state integer, "			//״̬
				"time_create char[32], "	//���񴴽�ʱ�䡣ʱ����"��-��-�� ʱ:��:��"�����ĸ�ʽ���棬��ͬ
				"time_start char[32], "		//��ʼʱ��
				"time_finish char[32], "	//����ʱ��
				"remark char[64*1024]);"	//��ʾ���
				);

			//Ϊ�����ֶν�������
			dbTask.execDML("create index idx_time_create on T_todo(time_create);");
		}
		return true;
	}
	catch(CppSQLite3Exception &exp)
	{
		exp;
		ATLTRACE("error:%s\n",exp.errorMessage());
		ATLASSERT(FALSE);
		return false;
	}
}

TodoSet::TodoSet()
{
	m_strDB = GlobeFuns::GetAppDirectory() + _T("task.db");
}
//
//bool TodoSet::Load(const TCHAR *strDB)
//{
//	if (strlen(strDB)<=1)
//	{
//		ATLASSERT(FALSE);
//		return false;
//	}
//	m_strDB = strDB;
//	return true;
//}
//
//bool TodoSet::Load()
//{
//	return Load(GlobeFuns::GetAppDirectory() + "task.db");
//}
//bool TodoSet::Save(const TCHAR *strDB)
//{
//	return true;
//	////������
//	//CppSQLite3DB dbTask;
//	//dbTask.open(strDB);
//
//	//dbTask.execDML("begin;");                                 //��ʼ��������
//
//	////�����񲻴��ڣ��򴴽����
//	//if (!dbTask.tableExists("T_todo"))                //�����¼���־��
//	//{
//	//	//���ݿ��ֶΣ�����id���������ͣ�����ʱ�䣬�ϴ���ʾʱ�䣬��ʾ���ȡ�
//	//	//last_run_time������������ȷ����ʾ�Ƿ��Ѿ�ִ�У������ظ�
//	//	dbTask.execDML("Create table T_todo("
//	//		"id integer PRIMARY KEY AUTOINCREMENT, "	//����Ψһid
//	//		"title char[128], "			//����
//	//		"priority integer, "		//���ȼ�
//	//		"state integer, "			//״̬
//	//		"time_create char[32], "	//���񴴽�ʱ�䡣ʱ����"��-��-�� ʱ:��:��"�����ĸ�ʽ���棬��ͬ
//	//		"time_start char[32], "		//��ʼʱ��
//	//		"time_finish char[32], "	//����ʱ��
//	//		"remark char[64*1024]);"	//��ʾ���
//	//		);
//
//	//	//Ϊ�����ֶν�������
//	//	dbTask.execDML("create index idx_time_create on T_todo(time_create);");
//	//}
//	//else
//	//{
//	//	//��վ�����
//	//	dbTask.execDML("delete from T_todo;");
//	//}
//
//	//std::set<ToDoTask>::iterator it = m_setTask.begin();
//	//for (;it!=m_setTask.end();it++)
//	//{
//	//	if(!WriteToDoToDB(dbTask,*it))
//	//	{
//	//		dbTask.execDML("rollback transaction;");
//	//		return false;
//	//	}
//	//}
//
//	//dbTask.execDML("end;");                                    //�������������ύ����
//	//return true;
//}
//
//bool TodoSet::Save()
//{
//	return Save(GlobeFuns::GetAppDirectory() + "task.db");
//}

void TodoSet::GetTodoList( std::set<int> &taskIDs )
{
	//������
	CppSQLite3DB dbTask;
	dbTask.open(m_strDB.c_str());
	CheckCreateTable(dbTask);

	//�����񲻴��ڣ���˵�������ݿ��ʽ����
	if (!dbTask.tableExists("T_todo"))                //�����¼���־��
	{
		ATLASSERT(FALSE);
		return;
	}

	CppSQLite3Buffer buf;
	buf.format("select id from T_todo where state!=%d;",(int)ToDoTask::TS_DELETED);
	CppSQLite3Query q = dbTask.execQuery(buf);
	while(!q.eof())
	{
		ATLASSERT(ToDoTask::ERROR_TASKID!=q.getIntField("id"));
		taskIDs.insert(q.getIntField("id"));
		q.nextRow();
	}
}

ToDoTask TodoSet::GetToDo( int id )
{
	//������
	CppSQLite3DB dbTask;
	dbTask.open(m_strDB.c_str());
	CheckCreateTable(dbTask);

	//�����񲻴��ڣ���˵�������ݿ��ʽ����
	if (!dbTask.tableExists("T_todo"))                //�����¼���־��
	{
		return ToDoTask(ToDoTask::ERROR_TASKID);
	}

	CppSQLite3Buffer buf;
	buf.format("select id, title,priority , state, time_create, time_start, time_finish, remark  from T_todo where id=%d;",id);
	CppSQLite3Query q = dbTask.execQuery(buf);
	if(!q.eof())
	{
		ToDoTask todo;

		todo.id = q.getIntField("id");
		todo.strTask = q.getStringField("title");
		todo.priority = (ToDoTask::TaskPriority)q.getIntField("priority");
		todo.state = (ToDoTask::TaskState)q.getIntField("state");
		todo.tmCreateTime = GlobeFuns::StringToTime(q.getStringField("time_create"));
		todo.tmStartWorkTime = GlobeFuns::StringToTime(q.getStringField("time_start"));
		todo.tmPlanFinshTime = GlobeFuns::StringToTime(q.getStringField("time_finish"));
		todo.strRemark = q.getStringField("remark");

		return todo;
		//q.nextRow();
	}
	return ToDoTask(ToDoTask::ERROR_TASKID);
}

bool TodoSet::UpdateToDo( const ToDoTask &task )
{
	//������
	CppSQLite3DB dbTask;
	dbTask.open(m_strDB.c_str());
	CheckCreateTable(dbTask);
	return UpdateToDB(dbTask,task);
}

int TodoSet::AddToDo()
{
	ToDoTask todo(0);
	todo.tmCreateTime = CTime::GetCurrentTime();
	todo.strTask = _T("������");
	todo.priority = ToDoTask::TP_NORMAL;
	todo.state = ToDoTask::TS_WORKING;
	todo.tmPlanFinshTime = 0;
	todo.tmStartWorkTime = 0;

	//�����ݿ�
	CppSQLite3DB dbTask;
	dbTask.open(m_strDB.c_str());
	CheckCreateTable(dbTask);
	
	CppSQLite3Buffer strSql;
	strSql.format("insert into T_todo values(NULL,'%q',%d,%d,'%q','%q','%q','%q');",
		todo.strTask.c_str(),
		todo.priority,
		todo.state,
		GlobeFuns::TimeToString(todo.tmCreateTime),
		GlobeFuns::TimeToString(todo.tmStartWorkTime),
		GlobeFuns::TimeToString(todo.tmPlanFinshTime),
		todo.strRemark.c_str()
		);
	try{
		if(1!=dbTask.execDML(strSql))
		{
			return false;
		}
	}
	catch(CppSQLite3Exception &exp)
	{
		exp;
		ATLTRACE("error:%s\n",exp.errorMessage());
		ATLASSERT(FALSE);
		return false;
	}

	return (int)dbTask.lastRowId();
}

bool TodoSet::DeleteToDo( int id )
{
	//�����ݿ�
	CppSQLite3DB dbTask;
	dbTask.open(m_strDB.c_str());
	CheckCreateTable(dbTask);

	CppSQLite3Buffer strSql;
	strSql.format("delete from T_todo where id=%d;",id);
	try{
		return 1==dbTask.execDML(strSql);
	}
	catch(CppSQLite3Exception &exp)
	{
		exp;
		ATLTRACE("error:%s\n",exp.errorMessage());
		ATLASSERT(FALSE);
		return false;
	}
}

bool TodoSet::CheckDBValid()
{
	//�����ݿ�
	CppSQLite3DB dbTask;
	dbTask.open(m_strDB.c_str());
	CheckCreateTable(dbTask);

	return dbTask.tableExists("T_todo");
}