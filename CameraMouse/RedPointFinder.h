#pragma once

#include <map>
#include <vector>
using namespace std;

class RedPointFinder
{
public:
	//ptPixels��λͼ���ݣ������½ǿ�ʼ�������Ͻǽ�����
	RedPointFinder(unsigned int * ptPixels,unsigned int iWidth,unsigned int iHeight);
	~RedPointFinder(void);

	//�����ҵ��������ĵط������Բ��У�
	// atLeastPower:����뾶2������ֵ
	//����ֵ��
	// ���ڵ���0��ʾ���ҵ��ĵ��λ��
	// -1��ʾ�����ĵ����������ϣ��޷�����
	// -2��ʾû�з���Ҫ��ĵ㣨û�κ�һ����ﵽ��ֵҪ��
	int GetPowerMost(unsigned int atLeastPower);
private:
	unsigned int *m_ptPixels;
	unsigned int m_iWidth;
	unsigned int m_iHeight;
private:
	unsigned int CalcPointPower(unsigned int x,unsigned int y,unsigned int r);

	//��rΪ�뾶���ж�vecPoint�е��ĸ�����ǿ
	int CheckPowerMost(unsigned int r,const vector<unsigned int> &vecPoints,vector<unsigned int> &vecPointOut);
};
