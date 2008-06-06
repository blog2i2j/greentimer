#pragma once
#include "ITask.h"
#include <atlapp.h>
#include <atlmisc.h>
using namespace ATL;
#include <vector>

#include "ThreadLock.h"

class TaskDB
{
public:
	TaskDB(void);
	~TaskDB(void);
	bool ReadFromDB(const char *strDB);
	bool SaveToDB(const char *strDB);

	//����������ʱ��task_idӦ����Ϊ0.����Ḳ�Ǿɵ�����
	int AddTask(const ITask &task);
	bool AddDailyTask(int iHour, int iMinute, ATL::CString strMessage);
	bool RemoveTask(int idTask);

	void GetTaskList(std::vector<int> &vecIdOut);
	//ITask *GetTask(int idTask);
	bool GetTask(int idTask,ITask &task);
	bool FindTaskRunNow(ITask &task);
	bool UpdateTask(const ITask &task);
private:
	CThreadLock m_cs;
	std::vector<ITask> m_vecTask;
};

extern TaskDB g_TaskDB;
