#include "stdafx.h"
#include ".\globefuns.h"

// ȡ�ó������е�Ŀ¼���Է�б��\��β��
ATL::CString GlobeFuns::GetAppDirectory( bool bEndWithBackSlash/*=true*/ )
{
	//ȡϵͳ·��
	char buf[_MAX_PATH];
	::GetModuleFileName(NULL,buf,_MAX_PATH);
	int iIndex = (int)std::string(buf).rfind('\\');
	if(bEndWithBackSlash)
	{
		buf[iIndex+1]='\0';	//�ضϣ����·����(��������'\')
	}
	else
	{
		buf[iIndex]='\0';	//�ضϣ����·����(ȥ������'\')
	}
	return (char *)buf;
}