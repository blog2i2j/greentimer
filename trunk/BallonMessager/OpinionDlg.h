#pragma once
#include <atltime.h>
#include <string>

#include "TaskDB.h"
#include "GlobeFuns.h"

class COpinionDlg : public CDialogImpl<COpinionDlg>
{
public:
	enum { IDD = IDD_DLG_SETUP };

	BEGIN_MSG_MAP(COpinionDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		COMMAND_ID_HANDLER(IDOK, OnBnClickedOk)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CenterWindow(GetParent());

		char hour[16];
		char min[16];

		GetDefaultFillTime(hour, min);

		//�ڶԻ���������ʱ�򣬸��༭���趨��ʼֵ
		CWindow WndHour = GetDlgItem(IDC_HOUR);
		WndHour.SetWindowText(hour);

		CWindow WndMinute = GetDlgItem(IDC_MINUTE);
		WndMinute.SetWindowText(min);

		//�޶�ʱ��ͷ���ֻ����ָ����Χ
		CUpDownCtrl spn1 = GetDlgItem(IDC_SPIN1);
		spn1.SetRange(0,23);

		CUpDownCtrl spn2 = GetDlgItem(IDC_SPIN2);
		spn2.SetRange(0,59);

		return TRUE;
	}

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}
	LRESULT OnBnClickedOk(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		//�������OK����ť��ʱ��ȡ���༭���ֵ���öԻ�����ʾ������
		CEdit WndInput = GetDlgItem(IDC_EDT_INPUT);
		TCHAR strMsg[1024];

		CEdit WndHour = GetDlgItem(IDC_HOUR);
		CEdit WndMin = GetDlgItem(IDC_MINUTE);

		TCHAR strHour[3]={0};
		TCHAR strMin[3]={0};

		WndHour.GetWindowText(strHour,3);
		WndMin.GetWindowText(strMin,3);
		WndInput.GetWindowText(strMsg,1024);

		int iHour = atoi(strHour);
		int iMin = atoi(strMin);

		//����û��ǲ�������������ʾ��Ϣ��
		ATL::CString strTrimpedMsg = strMsg;

		//ֻ����ո��� :)
		strTrimpedMsg.Trim();

		//û������ʾ��Ϣ,�������������Զ�������Ϣ,����
		if (strTrimpedMsg.GetLength()==0 || strTrimpedMsg == ATL::CString("��������������ʾ��Ϣ"))
		{
			MessageBox("����������ʾ��Ϣ��������ʾ��Ϣ�ˡ�","������������ʾ��Ϣ",MB_OK);
			WndInput.SetFocus();
			WndInput.SetWindowText("��������������ʾ��Ϣ"); 
			WndInput.SetSel(0, -1);
		}
		//û����Сʱ��,����
		else if (strlen(strHour) == 0)
		{
			MessageBox("Ӧ��ɶʱ���������������ѽ��Сʱ��Ϊ�ա�","������������Сʱ��",MB_OK);
			WndHour.SetFocus();

			char hour[16];
			char min[16];

			GetDefaultFillTime(hour, min);

			WndHour.SetWindowText(hour);
			//WndMin.SetWindowText(min);
			WndHour.SetSel(0,-1);
		}
		//Сʱ��������Ҫ��,����
		else if (iHour > 23)
		{
			MessageBox("Сʱ����Ӧ�ô���23��Сʱ��Ӧ����0-23֮�䡣","¥���������ʱ���˰ɣ�",MB_OK);
			WndHour.SetFocus();
			WndHour.SetSel(0,-1);
		}
		//û���������,����
		else if (strlen(strMin) == 0)
		{
			MessageBox("Ӧ��ɶʱ���������������ѽ��������Ϊ�ա�","���������������",MB_OK);
			WndMin.SetFocus();

			char hour[16];
			char min[16];

			GetDefaultFillTime(hour, min);

			WndMin.SetWindowText(min);
			//WndHour.SetWindowText(hour);

			WndMin.SetSel(0,-1);
		}
		//������������Ҫ��,����
		else if (iMin > 59)
		{
			MessageBox("������Ӧ����0-59֮�䡣","¥���������ʱ���˰ɣ�",MB_OK);
			WndMin.SetFocus();
			WndMin.SetSel(0,-1);
		}
		//�ƺ���û������
		else
		{
			ITask theTask;
			bool bHaveRecordAlready = g_TaskDB.GetTask(iHour,iMin,ITask::TT_DAILY,theTask);
			ATL::CString Tip = theTask.Tip;

			//��ǰ��û��Ϊ�������ʱ���趨�����أ�
			if (bHaveRecordAlready)
				//��ǰ�Ѿ�Ϊָ��ʱ���趨����,�Ǿ�Ҫ�û�ȷ��һ����
			{
				ATL::CString strConfirm;
				strConfirm.Format("������Ҫ���ڣ� %s �� %s ����������\r\n\r\n\"%s\"\r\n\r\n��Ҫ�滻���������",strHour,strMin ,Tip);

				if(IDOK==MessageBox(strConfirm,"ȷ��һ��",MB_OKCANCEL))
				{
 					//�û�ȷ����Ҫ�滻����¼,���Ȱ�֮ǰ�ļ�¼ɾ��
					g_TaskDB.RemoveTask(theTask.Id);

					//////////////////////////////////////////////////////////////////////////
					//tianzuo,2008-6-6,��Ϊ���ݿⴢ��
					g_TaskDB.AddDailyTask(atoi(strHour), atoi(strMin),strMsg);
					g_TaskDB.SaveToDB(GlobeFuns::GetAppDirectory() + "task.db");
					EndDialog(wID);
					//////////////////////////////////////////////////////////////////////////
				}
			}
			//��ǰû��Ϊ�������ʱ���趨���ѣ��Ǿ�ֱ�����Ӱɡ�
			else
			{
				//////////////////////////////////////////////////////////////////////////
				//tianzuo,2008-6-6,��Ϊ���ݿⴢ��
				g_TaskDB.AddDailyTask(atoi(strHour), atoi(strMin),strMsg);
				g_TaskDB.SaveToDB(GlobeFuns::GetAppDirectory() + "task.db");
				EndDialog(wID);
				//////////////////////////////////////////////////////////////////////////
			}
		}

		return 0;
	}

	//����Ĭ����䵽���ÿ��ʱ��
	// *hour �� *minָ���bufferҪ��һ���Ŀռ�.
	void GetDefaultFillTime(char *hour, char *min)
	{
		//��ǰʱ��,��������ʼֵ
		CTime tm = CTime::GetCurrentTime();
		int iNowHour = tm.GetHour();
		int iNowMin = tm.GetMinute();

		int iSetHour = iNowHour;
		int iSetMin = iNowMin + 10; //��ǰʮ���ӣ�GreenTimer��Ϊ��û���û���Ϊ��ǰʱ���趨���ѵ�
		if (iSetMin > 59)
		{
			iSetMin -= 60;
			iSetHour += 1;
		}

		//char hour[16];
		itoa(iSetHour,hour,10);

		//char min[16];
		itoa(iSetMin,min,10);
	}
};