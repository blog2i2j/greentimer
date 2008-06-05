#pragma once
#include "ITask.h"
#include <vector>


class TaskDB
{
public:
	TaskDB(void);
	~TaskDB(void);
	bool ReadFromDB(const char *strDB);
	bool SaveToDB(const char *strDB);

	//����������ʱ��task_idӦ����Ϊ0.����Ḳ�Ǿɵ�����
	int AddTask(const ITask &task);
	bool RemoveTask(int idTask);

	void GetTaskList(std::vector<int> &vecIdOut);
	ITask *GetTask(int idTask);

	ITask *TaskRunNow();
private:
	std::vector<ITask> m_vecTask;
};
