#pragma once
#include <string>
#include <vector>
#include <deque>

class NetLogger
{
public:
	//д��־
	//�˺������̰߳�ȫ�ģ�����ͬʱ���á�
	static void WriteLog(unsigned int uiUserID,unsigned int uiLogCode,const char *logData);
};
