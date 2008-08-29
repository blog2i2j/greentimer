#include "StdAfx.h"
#include ".\todotask.h"

#include "SQLite/CppSQLite3.h"
#include "GlobeFuns.h"
#include "DBLog.h"

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
//	strSql.format("insert into T_todo2 values(NULL,'%q',%d,%d,'%q','%q','%q','%q');",
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
	strSql.format("update T_todo2 set"
		" title='%q', priority=%d, state=%d,"
		" time_create=%d,time_start=%d,time_finish=%d,remark='%q'"
		" where id=%d;",
		todo.strTask.c_str(),
		todo.priority,
		todo.state,
		GlobeFuns::TimeToInt(todo.tmCreateTime),
		GlobeFuns::TimeToInt(todo.tmStartWorkTime),
		GlobeFuns::TimeToInt(todo.tmPlanFinshTime),
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
		if (!dbTask.tableExists("T_todo2"))                //�����¼���־��
		{
			//���ݿ��ֶΣ�����id���������ͣ�����ʱ�䣬�ϴ���ʾʱ�䣬��ʾ���ȡ�
			//last_run_time������������ȷ����ʾ�Ƿ��Ѿ�ִ�У������ظ�
			dbTask.execDML("Create table T_todo2("
				"id integer PRIMARY KEY AUTOINCREMENT, "	//����Ψһid
				"title char[128], "			//����
				"priority integer, "		//���ȼ�
				"state integer, "			//״̬
				"time_create integer, "	//���񴴽�ʱ�䡣ʱ����"��-��-�� ʱ:��:��"�����ĸ�ʽ���棬��ͬ
				"time_start integer, "		//��ʼʱ��
				"time_finish integer, "	//����ʱ��
				"remark char[64*1024]);"	//��ʾ���
				);

			//Ϊ�����ֶν�������
			dbTask.execDML("create index idx_todo2_time_create on T_todo2(time_create);");

			LOG_TODO(LOG_CONST::MTV_CREATE_TABLE,-1,"�ɹ�:������ ");
		}
		return true;
	}
	catch(CppSQLite3Exception &exp)
	{
		exp;
		ATLTRACE("error:%s\n",exp.errorMessage());
		ATLASSERT(FALSE);
		LOG_TODO(LOG_CONST::MTV_CREATE_TABLE,-1,((string)"�쳣:������ " + exp.errorMessage()).c_str());
		return false;
	}
}

TodoSet::TodoSet()
{
	m_strDB = GlobeFuns::GetAppDirectory() + _T("task.db");
}

void TodoSet::GetTodoList( std::set<int> &taskIDs )
{
	//������
	CppSQLite3DB dbTask;
	dbTask.open(m_strDB.c_str());
	CheckCreateTable(dbTask);

	//�����񲻴��ڣ���˵�������ݿ��ʽ����
	if (!dbTask.tableExists("T_todo2"))                //�����¼���־��
	{
		ATLASSERT(FALSE);
		return;
	}

	CppSQLite3Buffer buf;
	buf.format("select id from T_todo2 where state!=%d;",(int)ToDoTask::TS_DELETED);
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
	if (!dbTask.tableExists("T_todo2"))                //�����¼���־��
	{
		return ToDoTask(ToDoTask::ERROR_TASKID);
	}

	CppSQLite3Buffer buf;
	buf.format("select id, title,priority , state, time_create, time_start, time_finish, remark  from T_todo2 where id=%d;",id);
	CppSQLite3Query q = dbTask.execQuery(buf);
	if(!q.eof())
	{
		ToDoTask todo;

		todo.id = q.getIntField("id");
		todo.strTask = q.getStringField("title");
		todo.priority = (ToDoTask::TaskPriority)q.getIntField("priority");
		todo.state = (ToDoTask::TaskState)q.getIntField("state");
		todo.tmCreateTime = GlobeFuns::IntToTime(q.getIntField("time_create"));
		todo.tmStartWorkTime = GlobeFuns::IntToTime(q.getIntField("time_start"));
		todo.tmPlanFinshTime = GlobeFuns::IntToTime(q.getIntField("time_finish"));
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
	todo.state = ToDoTask::TS_NOT_START;
	todo.tmPlanFinshTime = 0;
	todo.tmStartWorkTime = 0;

	//�����ݿ�
	CppSQLite3DB dbTask;
	dbTask.open(m_strDB.c_str());
	CheckCreateTable(dbTask);
	
	CppSQLite3Buffer strSql;
	strSql.format("insert into T_todo2 values(NULL,'%q',%d,%d,%d,%d,%d,'%q');",
		todo.strTask.c_str(),
		todo.priority,
		todo.state,
		GlobeFuns::TimeToInt(todo.tmCreateTime),
		GlobeFuns::TimeToInt(todo.tmStartWorkTime),
		GlobeFuns::TimeToInt(todo.tmPlanFinshTime),
		todo.strRemark.c_str()
		);
	try{
		if(1!=dbTask.execDML(strSql))
		{
			LOG_TODO(LOG_CONST::MTV_ADD,-1,"ʧ��:��Ӵ�������ʧ�ܡ�");
			return false;
		}
	}
	catch(CppSQLite3Exception &exp)
	{
		exp;
		ATLTRACE("error:%s\n",exp.errorMessage());
		ATLASSERT(FALSE);
		LOG_TODO(LOG_CONST::MTV_ADD,-1,((string)"�쳣:" + exp.errorMessage()).c_str());
		return false;
	}

	LOG_TODO(LOG_CONST::MTV_ADD,(int)dbTask.lastRowId(),"�ɹ�:��Ӵ�������ɹ���");
	return (int)dbTask.lastRowId();
}

bool TodoSet::DeleteToDo( int id )
{
	//�����ݿ�
	CppSQLite3DB dbTask;
	dbTask.open(m_strDB.c_str());
	CheckCreateTable(dbTask);

	CppSQLite3Buffer strSql;
	strSql.format("delete from T_todo2 where id=%d;",id);
	try{
		if(1==dbTask.execDML(strSql))
		{
			LOG_TODO(LOG_CONST::MTV_DEL,id,"�ɹ�:ɾ����������ɹ�");
			return true;
		}
		else
		{
			LOG_TODO(LOG_CONST::MTV_DEL,id,"ʧ��:ɾ����������ʧ��");
			return false;
		}
	}
	catch(CppSQLite3Exception &exp)
	{
		exp;
		ATLTRACE("error:%s\n",exp.errorMessage());
		ATLASSERT(FALSE);
		LOG_TODO(LOG_CONST::MTV_DEL,id,((string)"�쳣:ɾ����������ʧ��" + exp.errorMessage()).c_str());
		return false;
	}
}

bool TodoSet::CheckDBValid()
{
	//�����ݿ�
	CppSQLite3DB dbTask;
	dbTask.open(m_strDB.c_str());
	CheckCreateTable(dbTask);

	return dbTask.tableExists("T_todo2");
}