// TaskListDialog.cpp : CTaskListDialog ��ʵ��

#include "stdafx.h"
#include "TaskListDialog.h"
#include ".\tasklistdialog.h"
#include "TaskDB.h"

#include ".\OpinionDlg.h"
#include "GlobeFuns.h"
#include "TaskModifyDialog.h"

// CTaskListDialog

LRESULT CTaskListDialog::OnBnClickedBtnAdd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	COpinionDlg dlg;
	dlg.DoModal();

	ReloadTasks();
	return 0;
}

LRESULT CTaskListDialog::OnBnClickedBtnDel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	vector<int> vecTask;
	GetSelectUser(vecTask);
	for (int i=0;i<(int)vecTask.size();i++)
	{
		g_TaskDB.RemoveTask(vecTask[i]);
	}
	g_TaskDB.SaveToDB();
	ReloadTasks();
	return 0;
}

LRESULT CTaskListDialog::OnBnClickedBtnEdit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	vector<int> vecTask;
	GetSelectUser(vecTask);
	if (vecTask.empty())
	{
		return 0;
	}
	//if (vecTask.size()>1)
	//{
	//	MessageBox(_T("��ͬʱָ���˶������Ҫ��������б༭����ֻѡ��"));
	//	return 0;
	//}
	CTaskModifyDialog dlg(vecTask[0]);
	dlg.DoModal();
	ReloadTasks();
	return 0;
}

void CTaskListDialog::ReloadTasks()
{
	m_ctlList.SetRedraw(FALSE);

	m_ctlList.DeleteAllItems();

	std::vector<int> vecId;
	g_TaskDB.GetTaskList(vecId);
	for (int i=0;i<(int)vecId.size();i++)
	{
		ITask task;
		if(!g_TaskDB.GetTask(vecId[i],task))
		{
			ATLASSERT(FALSE);
			continue;
		}
		char buf[64];
		int iItemIndex  = m_ctlList.InsertItem(i,itoa(task.Id,buf,10));
		m_ctlList.SetItemData(iItemIndex,task.Id);
		m_ctlList.SetItemText(iItemIndex,1,ITask::GetTaskTypeStr(task.Type));
		m_ctlList.SetItemText(iItemIndex,2,task.GetTimeStr().GetBuffer(0));
		//m_ctlList.SetItemText(iItemIndex,3,task..GetBuffer(0));
		//m_ctlList.SetItemText(iItemIndex,4,task.GetTimeStr().GetBuffer(0));
		m_ctlList.SetItemText(iItemIndex,3,task.Tip.GetBuffer(0));

	}
	m_ctlList.SetRedraw(TRUE);
}

LRESULT CTaskListDialog::OnInitDialog( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	CAxDialogImpl<CTaskListDialog>::OnInitDialog(uMsg, wParam, lParam, bHandled);

	m_ctlList = GetDlgItem(IDC_LST_TASK);

	ResetTaskList();
	ReloadTasks();
	return 1;  // ʹϵͳ���ý���
}

LRESULT CTaskListDialog::OnClickedOK( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled )
{
	EndDialog(wID);
	return 0;
}

LRESULT CTaskListDialog::OnClickedCancel( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled )
{
	EndDialog(wID);
	return 0;
}

void CTaskListDialog::GetSelectUser( vector<int> &vecSels )
{
	//CListViewCtrl _ctlList(GetDlgItem(IDC_LST_USERLIST));
	for (int i=0;i<m_ctlList.GetItemCount();i++)
	{
		UINT iState = m_ctlList.GetItemState(i,LVIS_SELECTED);
		if(!iState)
		{
			continue;
		}
		int id = m_ctlList.GetItemData(i);
		ATLASSERT(id!=0);
		vecSels.push_back(id);
	}
}

void CTaskListDialog::ResetTaskList()
{
	while(m_ctlList.DeleteColumn(0));	//ɾ��������
	int iCol = 0;
	m_ctlList.InsertColumn(iCol++,_T("ID"));          //������
	m_ctlList.InsertColumn(iCol++,_T("����"));
	m_ctlList.InsertColumn(iCol++,_T("ʱ��"));
	//m_ctlList.InsertColumn(iCol++,_T("�ϴ�����"));
	//m_ctlList.InsertColumn(iCol++,_T("����"));
	m_ctlList.InsertColumn(iCol++,_T("��ʾ���"));
	CRect rectList;
	m_ctlList.GetClientRect(rectList);                     //��õ�ǰ�ͻ�����Ϣ
	m_ctlList.SetColumnWidth(0,25);        //�����еĿ�ȡ�
	m_ctlList.SetColumnWidth(1,80);
	m_ctlList.SetColumnWidth(2,50);
	m_ctlList.SetColumnWidth(3,rectList.Width()-15-50);
	m_ctlList.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT);
	m_ctlList.DeleteAllItems();
}
LRESULT CTaskListDialog::OnNMDblclkLstTask(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	vector<int> vecTask;
	GetSelectUser(vecTask);
	if (vecTask.empty())
	{
		return 0;
	}
	//if (vecTask.size()>1)
	//{
	//	MessageBox(_T("��ͬʱָ���˶������Ҫ��������б༭����ֻѡ��"));
	//	return 0;
	//}
	CTaskModifyDialog dlg(vecTask[0]);
	dlg.DoModal();
	ReloadTasks();
	return 0;
}
