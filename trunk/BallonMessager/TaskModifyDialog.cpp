// TaskModifyDialog.cpp : CTaskModifyDialog ��ʵ��

#include "stdafx.h"
#include "TaskModifyDialog.h"
#include "TaskDB.h"
#include ".\taskmodifydialog.h"

// CTaskModifyDialog

LRESULT CTaskModifyDialog::OnInitDialog( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	CAxDialogImpl<CTaskModifyDialog>::OnInitDialog(uMsg, wParam, lParam, bHandled);
	m_edtID = GetDlgItem(IDC_EDT_ID);
	m_edtType = GetDlgItem(IDC_EDT_TYPE);
	m_edtTime = GetDlgItem(IDC_EDT_TIME);
	m_edtTips = GetDlgItem(IDC_EDT_TIPS);

	ITask task;
	if(!g_TaskDB.GetTask(m_taskid,task))
	{
		return 0;
	}

	m_edtID.SetWindowText(task.GetIDStr());
	m_edtType.SetWindowText(ITask::GetTaskTypeStr(task.Type));
	m_edtTime.SetWindowText(task.GetTimeStr());
	m_edtTips.SetWindowText(task.Tip);

	return 1;  // ʹϵͳ���ý���
}

LRESULT CTaskModifyDialog::OnClickedOK( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled )
{
	EndDialog(wID);

	ITask task;
	if(!g_TaskDB.GetTask(m_taskid,task))
	{
		MessageBox(_T("����ID���������޷����棬���뿪����Ա��ϵ"));
		return 0;
	}
	m_edtTips.GetWindowText(task.Tip);
	g_TaskDB.UpdateTask(task);
	g_TaskDB.SaveToDB();

	return 0;
}

LRESULT CTaskModifyDialog::OnClickedCancel( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled )
{
	EndDialog(wID);
	return 0;
}

LRESULT CTaskModifyDialog::OnBnClickedBtnTimebrowse(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	return 0;
}
