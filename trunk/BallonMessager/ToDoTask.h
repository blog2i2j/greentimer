#pragma once
#include <string>
#include <set>
#include <atltime.h>

//�����¼
class ToDoTask
{
public:
	//��������ID
	enum {
		ERROR_TASKID = 0,
	};
	//�������ȼ�
	enum TaskPriority
	{
		TP_MOST_IMPORTANT = 0,
		TP_IMPORTANT = 1,
		TP_NORMAL = 2,
		TP_NOT_IMPORTANT = 3,
	};
	//����״̬
	enum TaskState
	{
		TS_NOT_START = 0,
		TS_WORKING = 1,
		TS_STOP = 2,
		TS_FINISHED = 3,
		TS_CANCEL = 4,
	};
	ToDoTask(void);
	ToDoTask(int taskid);
	~ToDoTask(void);
	bool operator < (const ToDoTask &task) const;

	//����id
	int id;
	//�������
	std::string strTask;
	//���ȼ�
	TaskPriority priority;
	//״̬
	TaskState state;
	//����
	std::string strRemark;

	//���񴴽�ʱ��
	CTime tmCreateTime;
	//����ʼ������ʱ��
	CTime tmStartWorkTime;

	CTime tmPlanFinshTime;

	//���������ʷ
	std::map<CTime,std::string> mapHistory;
};

//�������ݿ�
class TodoSet
{
public:
	TodoSet();

	bool Load();
	bool Save();

	bool Load(const TCHAR *strDB);
	bool Save(const TCHAR *strDB);
	void GetTodoList(std::set<int> &taskIDs);
	ToDoTask GetToDo(int id);
	bool UpdateToDo(const ToDoTask &task);
	int AddToDo();
	bool DeleteToDo(int id);
private:
	std::set<ToDoTask> m_setTask;
};

extern TodoSet g_todoSet;

