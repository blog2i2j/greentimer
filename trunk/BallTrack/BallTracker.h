#pragma once

#include "ReverseVector.h"
#include "MouseCtrler.h"

struct BallPos 
{
	BallPos(int _x,int _y)
		:x(_x),y(_y)
	{
	}
	int x;
	int y;
};

class BallTracker
{
public:
	BallTracker();
	~BallTracker(void);

	void Init(int iWidth, int iHeight, MouseCtrler *pMouseCtrler);
	void OnBallPos(int x,int y,int d);
private:
	MouseCtrler *m_pMouseCtrler;
	int m_iWidth;
	int m_iHeight; 

	//����ͷ�е���һ���൱����Ļ����λ�ã�
	std::pair<int,int> m_ptCenter;
	//С��������ͷ���ƶ�һ�����൱���ڽ������ƶ��������أ�
	int m_iZoom;

	int m_pfs;
	std::pair<int,int> m_ScreenSize;

	ReverseVector<BallPos> m_rvecPoints;

private:
	//�ж��Ƿ���iTimeʱ���ھ�ֹ��������λ�����룩
	bool IsStatic(int iTime) const;
	static BallPos CalcAveragePos(const BallPos *pData,int count);
	static int Distance(const BallPos &bp1,const BallPos &bp2);
};
