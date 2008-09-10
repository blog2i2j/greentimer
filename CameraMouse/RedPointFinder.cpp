//#include "stdafx.h"
#include ".\redpointfinder.h"

#define PICK_RED(rgb) ((rgb)%256)

RedPointFinder::RedPointFinder(unsigned int * ptPixels,unsigned int iWidth,unsigned int iHeight)
{
	m_ptPixels = ptPixels;
	m_iWidth = iWidth;
	m_iHeight = iHeight;
}

RedPointFinder::~RedPointFinder(void)
{
}

unsigned int RedPointFinder::CalcPointPower(unsigned int x,unsigned int y,unsigned int r )
{
	//��Ҫ�жϵĵ�̫������Ե����ô��rΪ�뾶��Բ���ܻᳬ��ͼ��߽硣
	if( (abs(x)<(int)r)
		|| (abs(x-m_iWidth)<(int)r)
		|| (abs(y)<(int)r)
		|| (abs(y-m_iHeight)<(int)r) )
	{
		return 0;
	}

	//��8�������������ֵ֮��
	unsigned int uiPower = PICK_RED( GetColor(x, y));
	for (unsigned int i=1;i<r;i++)	//ע�⣬0���Ѿ���ǰ�����
	{
		//�ұ�����
		uiPower += PICK_RED( GetColor(x + i, y + i) );
		uiPower += PICK_RED( GetColor(x + i, y - i) );
		uiPower += PICK_RED( GetColor(x + i, y ) );

		//�������
		uiPower += PICK_RED( GetColor(x - i, y + i) );
		uiPower += PICK_RED( GetColor(x - i, y - i) );
		uiPower += PICK_RED( GetColor(x - i, y ) );

		//�м����㣨���м���һ��ǰ���Ѿ�������ˣ�
		uiPower += PICK_RED( GetColor(x , y + i) );
		uiPower += PICK_RED( GetColor(x , y - i) );
	}

	return uiPower;
}

int RedPointFinder::CheckPowerMost(unsigned int r,const vector<unsigned int> &vecPoints,vector<unsigned int> &vecPointOut )
{
	unsigned int iMaxPower = 0;
	vector<unsigned int>::const_iterator it = vecPoints.begin();
	for (;it!=vecPoints.end();it++)
	{
		int pos = *it;

		int i = GetX(pos);
		int j = GetY(pos);
		//ATLASSERT((i*m_iWidth+j)==pos);
		unsigned int iPower = CalcPointPower(i,j,r);
		if (iPower<iMaxPower)
		{
			continue;
		}
		if (iPower>iMaxPower)	//����������ǿ�ĵ㣬�ɵĵ�ȫ����ѡ
		{
			iMaxPower=iPower;
			vecPointOut.clear();
		}
		vecPointOut.push_back(pos);
	}
	return vecPointOut.size();
}

int RedPointFinder::GetPowerMost(unsigned int atLeastPower )
{
	unsigned int iMaxPower = 0;
	vector<unsigned int> vecPoints;

	//����ɨ��һ�飬�ҳ���2Ϊ�뾶ʱ����ǿ��
	for(int pos=0;pos<m_iWidth*m_iHeight;pos++)
	{
		int i=GetX(pos);
		int j=GetY(pos);

		unsigned int iPower = CalcPointPower(i,j,2);
		if (iPower<atLeastPower)	//û�ﵽ��ֵҪ�󣬿���һ��
		{
			continue;
		}

		if (iPower>iMaxPower)	//����������ǿ�ĵ㣬�ɵĵ�ȫ�������
		{
			iMaxPower=iPower;
			vecPoints.clear();
		}
		vecPoints.push_back(pos);
	}

	if (vecPoints.empty())	//ȫ��������������
	{
		//ATLASSERT(FALSE);
		return -2;
	}

	//Ȼ�󣬼Ӵ�뾶ֵ���ҵ���ǿ����һ����
	vector<unsigned int> vecPointTmp;
	for (unsigned int r=4;r<(m_iWidth/2);r+=2)
	{
		if(CheckPowerMost(r,vecPoints,vecPointTmp)<=1)
		{
			vecPoints.swap(vecPointTmp);
			break;
		}
		vecPoints.swap(vecPointTmp);
		vecPointTmp.clear();
	}
	//if (vecPoints.size()>1)	//��Ȼ����ô����ֲ������£�
	//{
	//	return -1;
	//}
	//�ã���ǿ��ֻ��һ����
	return vecPoints[0];
}