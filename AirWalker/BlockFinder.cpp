#include "StdAfx.h"
#include "BlockFinder.h"
#include <set>
#include <map>
using namespace std;

//Ŀ����ɫ
const COLORREF c_colorTarget = RGB(0XFF,0XFF,0);

bool BlockFinder::Init(int iWidth,int iHeight)
{
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	return true;
}

bool BlockFinder::FindPoint( BYTE *pData,int &x,int &y,int &d )
{
	int powerMAX = 0;
	set<pair<int,int> > stPoints;

	//��ȡ��ѡ��
	//�뾶Ϊ4�ķ�Χ�ڣ�����Щ��ǿ�㣿
	for (int i=0;i<m_iWidth;i++)
	{
		for (int j=0;j<m_iHeight;j++)
		{
			COLORREF colorTmp = GetAvgPointColor(pData,j,i,10);

			//�����ɫ���ֵ�ǿ��
			int iPower = GetRValue(colorTmp) + GetGValue(colorTmp) 
				- 2*GetBValue(colorTmp)	//���۳��ҶȲ��֣�
				- abs(GetRValue(colorTmp) - GetGValue(colorTmp)); //��ɫ�����ɫ��Ҫ�ӽ�
			if (iPower>powerMAX)
			{
				powerMAX = iPower;
				stPoints.clear();
				stPoints.insert(make_pair(i,j));
			}
			if (iPower==powerMAX)
			{
				stPoints.insert(make_pair(i,j));
			}
		}
	}
	if (stPoints.empty())
	{
		ASSERT(FALSE);
		return false;
	}

	//�Ӻ�ѡ���У�ɸѡ����ѵĵ�
	//�������ж���������Ǹ���
	//set<pair<int,int> > stLastPoints;
	//for (d=11;d<m_iHeight/3;d++)
	//{
	//	powerMAX = 0;
	//	set<pair<int,int> >::iterator it = stPoints.begin();
	//	for (;it!=stPoints.end();it++)
	//	{
	//		x = (*it).first;
	//		y = (*it).second;
	//		COLORREF colorTmp = GetAvgPointColor(pData,x,y,d);

	//		//�����ɫ���ֵ�ǿ��
	//		int iPower = GetRValue(colorTmp) + GetGValue(colorTmp) 
	//			- 2*GetBValue(colorTmp)	//���۳��ҶȲ��֣�
	//			- abs(GetRValue(colorTmp) - GetGValue(colorTmp)); //��ɫ�����ɫ��Ҫ�ӽ�

	//		if (iPower>powerMAX)
	//		{
	//			powerMAX = iPower;
	//			stLastPoints.clear();
	//			stLastPoints.insert(*it);
	//		}
	//		if (iPower==powerMAX)
	//		{
	//			stLastPoints.insert(*it);
	//		}
	//	}
	//	if (stLastPoints.empty())	//�ڷ�Χ�������Щɫ��ľ�ֵ���С��������Ϊ0����
	//	{
	//		break;
	//	}
	//	stPoints = stLastPoints;
	//	if (stPoints.size()==1)
	//	{
	//	}
	//}
	//if (stPoints.empty())
	//{
	//	ASSERT(FALSE);
	//	return false;
	//}

	//x = (*stPoints.begin()).first;
	//y = (*stPoints.begin()).second;

	//////////////////////////////////////////////////////////////////////////
	//�ڶ���ɸѡ����������ÿ����İ뾶
	map<int,pair<int,int> > mapBlocks;
	set<pair<int,int> >::iterator it = stPoints.begin();
	for (;it!=stPoints.end();it++)
	{
		x = (*it).first;
		y = (*it).second;

		int newX,newY,newD;
		GetColorBlock(pData,x,y,newX,newY,newD);
		mapBlocks.insert(make_pair(newD,make_pair(newX,newY)));
	}
	d = (*mapBlocks.rbegin()).first;
	x = (*mapBlocks.rbegin()).second.first;
	y = (*mapBlocks.rbegin()).second.second;
	
	return false;
}

COLORREF BlockFinder::GetPointColor( BYTE * pBuffer,int iRow, int iCol )
{
	if(iRow<0 || iRow>=m_iHeight)
	{
		return 0;
	}
	if(iCol<0 || iCol>=m_iWidth)
	{
		return 0;
	}
	//��ɫ�ǰ�B-G-R˳�򱣳ֵ�
	return RGB(
		*(pBuffer + 3*(iRow*m_iWidth + iCol) + 2)
		,*(pBuffer + 3*(iRow*m_iWidth + iCol) + 1)
		,*(pBuffer + 3*(iRow*m_iWidth + iCol) ));
}

COLORREF BlockFinder::GetAvgPointColor( BYTE * pBuffer,int iRow, int iCol, int d )
{
	COLORREF color = GetPointColor(pBuffer,iRow,iCol);
	unsigned int lRed = GetRValue(color);
	unsigned int lGreen = GetGValue(color);
	unsigned int lBlue = GetBValue(color);
	for (int i=1;i<d;i++)	//ע�⣬0���Ѿ���ǰ�����
	{
		//////////////////////////////////////////////////////////////////////////
		//red
		//�ұ�����
		COLORREF colorTmp = GetPointColor(pBuffer, iRow + i,iCol + i);
		lRed += GetRValue( colorTmp );
		lGreen += GetGValue(colorTmp);
		lBlue += GetBValue(colorTmp);
		colorTmp = GetPointColor(pBuffer, iRow - i,iCol + i);
		lRed += GetRValue( colorTmp );
		lGreen += GetGValue(colorTmp);
		lBlue += GetBValue(colorTmp);
		colorTmp = GetPointColor(pBuffer, iRow,iCol + i );
		lRed += GetRValue( colorTmp );
		lGreen += GetGValue(colorTmp);
		lBlue += GetBValue(colorTmp);

		//�������
		colorTmp = GetPointColor(pBuffer, iRow + i,iCol - i);
		lRed += GetRValue( colorTmp );
		lGreen += GetGValue(colorTmp);
		lBlue += GetBValue(colorTmp);
		colorTmp = GetPointColor(pBuffer, iRow - i,iCol - i);
		lRed += GetRValue( colorTmp );
		lGreen += GetGValue(colorTmp);
		lBlue += GetBValue(colorTmp);
		colorTmp = GetPointColor(pBuffer, iRow,iCol - i);
		lRed += GetRValue( colorTmp );
		lGreen += GetGValue(colorTmp);
		lBlue += GetBValue(colorTmp);

		//�м����㣨���м���һ��ǰ���Ѿ�������ˣ�
		colorTmp = GetPointColor(pBuffer, iRow + i,iCol);
		lRed += GetRValue( colorTmp );
		lGreen += GetGValue(colorTmp);
		lBlue += GetBValue(colorTmp);
		colorTmp = GetPointColor(pBuffer, iRow - i,iCol);
		lRed += GetRValue( colorTmp );
		lGreen += GetGValue(colorTmp);
		lBlue += GetBValue(colorTmp);
	}

	int iPointCount = 8*(d-1) + 1;
	return RGB(lRed/iPointCount,lGreen/iPointCount,lBlue/iPointCount);
}

void BlockFinder::SetPointColor( BYTE * pBuffer,int iRow, int iCol,COLORREF color )
{
	if(iRow<0 || iRow>=m_iHeight)
	{
		//ASSERT(FALSE);
		return ;
	}
	if(iCol<0 || iCol>=m_iWidth)
	{
		//ASSERT(FALSE);
		return ;
	}
	*(pBuffer + 3*(iRow*m_iWidth + iCol) ) = GetBValue(color);	//RED
	*(pBuffer + 3*(iRow*m_iWidth + iCol) + 1) = GetGValue(color);	//GREEN
	*(pBuffer + 3*(iRow*m_iWidth + iCol) + 2) = GetRValue(color);//255;
}

void BlockFinder::DrawRect( BYTE * pBuffer, int iCol,int iRow,int d,COLORREF color )
{
	for(int i=0;i<d;i++)
	{
		//�ұ�����
		SetPointColor(pBuffer, iRow + i,iCol + i,color);
		SetPointColor(pBuffer, iRow - i,iCol + i,color);
		SetPointColor(pBuffer, iRow,iCol + i ,color);

		//�������
		SetPointColor(pBuffer, iRow + i,iCol - i,color);
		SetPointColor(pBuffer, iRow - i,iCol - i,color);
		SetPointColor(pBuffer, iRow,iCol - i,color);

		//�м����㣨���м���һ��ǰ���Ѿ�������ˣ�
		SetPointColor(pBuffer, iRow + i,iCol,color);
		SetPointColor(pBuffer, iRow - i,iCol,color);
	}
}

int BlockFinder::GetColorLen( BYTE * pBuffer, int iCol,int iRow, int directX,int directY )
{
	if (directX==0 && directY==0)
	{
		ASSERT(FALSE);
		return 0;
	}
	const int DISTURB = 3;	//ƽ��ϵ��������������Ӱ��
	const int MAX_COLOR_DISTANCE = 10*10*3;	//��ɫͻ����پ���Ϊ����ɫ���Ե

	COLORREF colorAvg = GetAvgPointColor(pBuffer,iRow,iCol,DISTURB);

	//��ɫ�ص���ֵ��Ϊ�˼���ƽ��ֵ��
	unsigned int lRed = GetRValue(colorAvg);
	unsigned int lGreen = GetGValue(colorAvg);
	unsigned int lBlue = GetBValue(colorAvg);

	for (int d=1;d<m_iWidth;d++)
	{
		//�����ǰ������ƽ��ֵ
		colorAvg = RGB(lRed/d,lGreen/d,lBlue/d);	

		//���µ�һ���Ƿ����ͻ��
		COLORREF colorCurr = GetAvgPointColor(pBuffer,iRow+d*directY,iCol+d*directX,DISTURB);
		int iDistance = GetColorDistance(colorAvg,colorCurr);
		if (iDistance>MAX_COLOR_DISTANCE)
		{
			//��ͻ�䣬game over
			return d - 1;
		}

		//����һ�����ɫ���뵽��ɫ��ֵ��
		lRed += GetRValue(colorCurr);
		lGreen += GetGValue(colorCurr);
		lBlue += GetBValue(colorCurr);
	}
	return 0;
}

void BlockFinder::GetColorBorder( BYTE * pBuffer, int iCol,int iRow, int directX,int directY,int &x,int &y )
{
	int d = GetColorLen(pBuffer,iCol,iRow,directX,directY);
	x = iCol + d*directX;
	y = iRow + d*directY;
}
//static
int BlockFinder::GetColorDistance( COLORREF c1,COLORREF c2 )
{
	return (GetRValue(c1) - GetRValue(c2))*(GetRValue(c1) - GetRValue(c2)) + 
		(GetGValue(c1) - GetGValue(c2))*(GetGValue(c1) - GetGValue(c2)) + 
		(GetBValue(c1) - GetBValue(c2))*(GetBValue(c1) - GetBValue(c2));
}

void BlockFinder::GetColorBlock( BYTE * pBuffer, int iCol,int iRow,int &x,int &y,int &d )
{
	int AvgX = 0;
	int AvgY = 0;
	int tmpX,tmpY;
	
	//��˸�����ȡ������ȡ�߽�λ�ã�Ȼ��ȡƽ��ֵ���õ�ɫ������λ��
	for (int dirctX=-1;dirctX<=1;dirctX++)
	{
		for (int dirctY=-1;dirctY<=1;dirctY++)
		{
			if (dirctX==0 && dirctY==0)
			{
				continue;
			}
			GetColorBorder(pBuffer,iCol,iRow,dirctX,dirctY,tmpX,tmpY);
			AvgX += tmpX;
			AvgY += tmpY;
		}
	}
	x = AvgX/8;
	y = AvgY/8;

	//��δ����ĵ㿪ʼ���ٴ���˸�����ȡ������ȡƽ�����ȣ���Ϊɫ������ĳ���
	int r = 0;
	for (int dirctX=-1;dirctX<=1;dirctX++)
	{
		for (int dirctY=-1;dirctY<=1;dirctY++)
		{
			if (dirctX==0 && dirctY==0)
			{
				continue;
			}
			r += GetColorLen(pBuffer,iCol,iRow,dirctX,dirctY);
		}
	}
	d = r/8;
}
