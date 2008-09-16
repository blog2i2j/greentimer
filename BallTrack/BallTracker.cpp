#include "stdafx.h"
#include ".\balltracker.h"
#include <atldef.h>
#include <math.h>


BallTracker::BallTracker()
:m_rvecPoints(10000)
{
	m_ScreenSize.first = GetSystemMetrics(SM_CXSCREEN);
	m_ScreenSize.second = GetSystemMetrics(SM_CYSCREEN);
}

BallTracker::~BallTracker(void)
{
}

void BallTracker::OnBallPos( int x,int y,int d )
{
	int sx = m_ScreenSize.first/2 + x - m_ptCenter.first;
	int sy = m_ScreenSize.second/2 + y - m_ptCenter.second;
	m_rvecPoints.push_back(BallPos(sx,sy));

	if (IsStatic(2*1000))
	{
		m_pMouseCtrler->OnMouseMove(m_ScreenSize.first/2,m_ScreenSize.second/2);
	}
}

bool BallTracker::IsStatic( int iTime ) const
{
	const int c_iPRECISION = 10;	//��ȷ�ȣ�10����

	int iFrameCount = m_pfs*iTime/1000;
	const BallPos *pData = m_rvecPoints.GetDataPointer(iFrameCount);
	if (pData)
	{
		ATLTRACE("���ݲ��㣬�޷��жϡ�\n");
		return false;
	}
	//�оݣ����е������ĵ��ƫ�붼�������趨ֵ
	BallPos bpAvg = CalcAveragePos(pData,iFrameCount);
	for (int i=0;i<iFrameCount;i++)
	{
		const BallPos &bpTmp = *(pData+i);
		if (Distance(bpAvg,bpTmp)>c_iPRECISION)	//��һ����ƫ���˶����㣨�Ѿ���������ˣ�
		{
			return false;
		}
	}
	return true;
}

BallPos BallTracker::CalcAveragePos(const BallPos *pData,int count )
{
	BallPos bpAvg(0,0);
	for (int i=0;i<count;i++)
	{
		const BallPos &bpTmp = *(pData+i);
		bpAvg.x += bpTmp.x;
		bpAvg.y += bpTmp.y;
	}
	bpAvg.x /= count;
	bpAvg.y /= count;
	return bpAvg;
}

int BallTracker::Distance( const BallPos &bp1,const BallPos &bp2 )
{
	double dTmp = (bp1.x-bp2.x)*(bp1.x-bp2.x) + (bp1.y-bp2.y)*(bp1.y-bp2.y);
	return (int)sqrt(dTmp);
}

void BallTracker::Init( int iWidth, int iHeight, MouseCtrler *pMouseCtrler )
{
	ATLASSERT(pMouseCtrler);
	m_pMouseCtrler = pMouseCtrler;
	m_iWidth = iWidth;
	m_iHeight = iHeight;

	//��ʱ���������
	m_ptCenter.first = iWidth/2;
	m_ptCenter.second = iHeight/2;

	m_iZoom = iWidth/m_ScreenSize.first;
	m_pfs = 12;	//����ÿ��ɼ�12����
}