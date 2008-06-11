// SimpleTaskViewDlg.h : CSimpleTaskViewDlg ������

#pragma once

#include "resource.h"       // ������
#include <atlhost.h>
#include <atlctrls.h>
#include "cwebpage/cwebpage.h"
#include "TaskDB.h"

//HTML�ĵ�ģ��
static const TCHAR *TASKLIST_HTML_TEMPLATE = "<html><head></head><BODY leftmargin=0 topmargin=0 bgcolor=#FFFFFF>"
"<TABLE id=""Table1"" cellSpacing=""2"" cellPadding=""2"" width=""100%%"" bgColor=""#ccffcc"" border=""0"">"
"<TR>"
"<TD width=""22"" bgColor=""#66cc33""><FONT face=""����"">ID</FONT></TD>"
"<TD width=""80"" bgColor=""#66cc33""><FONT face=""����"">��ʾ����</FONT></TD>"
"<TD width=""55"" bgColor=""#66cc33""><FONT face=""����"">ʱ��</FONT></TD>"
"<TD bgColor=""#66cc33""><FONT face=""����"">��ʾ���</FONT></TD>"
"</TR>"
"%s"		//�������������������
"</TABLE>"
"</BODY>"
"</html>";

//��������ģ�壬��TASKLIST_HTML_TEMPLATE���ʹ��
static const TCHAR *TASK_TMPPLATE = 
"<TR>"
"<TD width=""22"">%d</TD>"
"<TD width=""80"">%s</TD>"
"<TD width=""55"">%s</TD>"
"<TD>%s</TD>"
"</TR>";

static ATL::CString GetTaskDesc(const ITask &task)
{
	//ATL::CString strDesc;
	//strDesc.Format("%d \t%d \t%d:%d \t%s",task.Id,task.Type,task.TaskTime.GetHour(),task.TaskTime.GetMinute(),task.Tip);
	ATL::CString strTask;
	strTask.Format(TASK_TMPPLATE,
		task.Id,
		ITask::GetTaskTypeStr(task.Type),
		task.GetTimeStr(),
		task.Tip);
	return strTask;
}

static ATL::CString GetTaskListDesc()
{
	ATL::CString strTasks;

	std::vector<int> vecId;
	g_TaskDB.GetTaskList(vecId);
	for (int i=0;i<vecId.size();i++)
	{
		ITask task;
		if(g_TaskDB.GetTask(vecId[i],task))
		{
			strTasks += GetTaskDesc(task);
		}
	}

	ATL::CString strHtm;
	strHtm.Format(TASKLIST_HTML_TEMPLATE,strTasks);
	return strHtm;
}
// CSimpleTaskViewDlg

class CSimpleTaskViewDlg : 
	public CAxDialogImpl<CSimpleTaskViewDlg>
{
public:
	CSimpleTaskViewDlg()
	{
	}

	~CSimpleTaskViewDlg()
	{
	}

	enum { IDD = IDD_SIMPLETASKVIEWDLG };

BEGIN_MSG_MAP(CSimpleTaskViewDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
	COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	CHAIN_MSG_MAP(CAxDialogImpl<CSimpleTaskViewDlg>)
	MESSAGE_HANDLER(WM_CLOSE, OnClose)
END_MSG_MAP()

// �������ԭ��: 
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CAxDialogImpl<CSimpleTaskViewDlg>::OnInitDialog(uMsg, wParam, lParam, bHandled);

		//CEdit edit(GetDlgItem(IDC_EDT_TASKLIST));
		////edit.SetWindowText(GetTaskListDesc());
		//edit.ShowWindow(SW_HIDE);

		EmbedBrowserObject(m_hWnd);

		//CRect rt
		//ResizeBrowser(m_hWnd,500,500);
		DisplayHTMLStr(m_hWnd,GetTaskListDesc());

		return 1;  // ʹϵͳ���ý���
	}

	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		UnEmbedBrowserObject(m_hWnd);
		EndDialog(wID);
		return 0;
	}

	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		UnEmbedBrowserObject(m_hWnd);
		EndDialog(wID);
		return 0;
	}


	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		UnEmbedBrowserObject(m_hWnd);
		EndDialog(IDOK);
		return 0;
	}
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};


