// ReconigzeBenmark.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CReconigzeBenmarkApp:
// �йش����ʵ�֣������ ReconigzeBenmark.cpp
//

class CReconigzeBenmarkApp : public CWinApp
{
public:
	CReconigzeBenmarkApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CReconigzeBenmarkApp theApp;