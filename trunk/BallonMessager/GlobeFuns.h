#pragma once

class GlobeFuns
{
public:
	// ȡ�ó������е�Ŀ¼���Է�б��\��β��
	static ATL::CString GetAppDirectory(bool bEndWithBackSlash=true);

	//////////////////////////////////////////////////////////////////////////
	//ʱ�����ַ�������ת������
	static ATL::CString TimeToString(CTime t);
	static ATL::CString TimeToFriendlyString(CTime t);
	static CTime StringToTime(ATL::CString strTime);
	//////////////////////////////////////////////////////////////////////////
};
