#pragma once

class BlockFinder
{
public:
	bool Init(int iWidth,int iHeight);

	//�ҵ��Ƶ�
	bool FindPoint(BYTE *pData,int &x,int &y,int &d);

	//////////////////////////////////////////////////////////////////////////
	//����Ϊ��������
	//���õ���ɫ
	void SetPointColor(BYTE * pBuffer,int iRow, int iCol,COLORREF color);
	//ȡĳһ�����ɫ
	inline COLORREF GetPointColor(BYTE * pBuffer,int iRow, int iCol);
	//�����ڰ뾶d�ķ�Χ�ڣ���ɫ��ƽ��ֵ��R,G,B�ֿ����㣩
	inline COLORREF GetAvgPointColor(BYTE * pBuffer,int iRow, int iCol, int d);
	//���ƾ���
	void DrawRect(BYTE * pBuffer, int iCol,int iRow,int d,COLORREF color);

	//��ȡ��(iCol,iRow)���ڵ�ɫ�������λ�á��뾶
	void GetColorBlock(BYTE * pBuffer, int iCol,int iRow,int &x,int &y,int &d);
	//����ָ���ķ���ǰ����һֱ��������ɫ�ı߽磬�����䳤��
	//direct����x��y���򲽽��ĳ��ȡ�һ��Ӧ��Ϊ����1��0��
	int GetColorLen(BYTE * pBuffer, int iCol,int iRow, int directX,int directY);
	void GetColorBorder(BYTE * pBuffer, int iCol,int iRow, int directX,int directY,int &x,int &y);
	//������ɫ֮�������̶�
	static int GetColorDistance(COLORREF c1,COLORREF c2);

private:
	int m_iWidth;
	int m_iHeight;
};
