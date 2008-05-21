#pragma once
#include <atltime.h>
#include <string>

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

		char hour[16];
		itoa(iSetHour,hour,10);

		char min[16];
		itoa(iSetMin,min,10);

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

		CWindow WndHour = GetDlgItem(IDC_HOUR);
		CWindow WndMin = GetDlgItem(IDC_MINUTE);

		TCHAR strHour[3]={0};
		TCHAR strMin[3]={0};

		WndHour.GetWindowText(strHour,3);
		WndMin.GetWindowText(strMin,3);
		WndInput.GetWindowText(strMsg,1024);

		//����û��ǲ�������������ʾ��Ϣ��
		ATL::CString strTrimpedMsg = strMsg;

		//ֻ����ո��� :)
		strTrimpedMsg.Trim();

		if (strTrimpedMsg.GetLength()==0)
		{
			MessageBox("����������ʾ��Ϣ��������ʾ��Ϣ�ˡ�","������������ʾ��Ϣ",MB_OK);
			WndInput.SetFocus();
			WndInput.SetWindowText("��������������ʾ��Ϣ"); 
			WndInput.SetSel(0, -1);
		}
		else
		{
			WTL::CString strTime = strHour;
			strTime += "-";
			strTime += strMin;

			//ȷ��һ��:
			WTL::CString strFileName = GetAppDirectory()+"tips.ini";

			char buffer[1024];
			WTL::CString strEmptyTips = "";
			int iCharsRt = GetPrivateProfileString("tips",strTime,strEmptyTips,buffer,sizeof(buffer),strFileName);

			//��ǰ��û��Ϊ�������ʱ���趨�����أ�
			if (iCharsRt != 0)
				//��ǰ�Ѿ�Ϊָ��ʱ���趨����,�Ǿ�Ҫ�û�ȷ��һ����
			{
				WTL::CString strConfirm;
				strConfirm.Format("������Ҫ���ڣ� %s �� %s ����������\r\n\r\n\"%s\"\r\n\r\n��Ҫ�滻���������",strHour,strMin ,buffer);

				if(IDOK==MessageBox(strConfirm,"ȷ��һ��",MB_OKCANCEL))
				{
					//WTL::CString strFileName = GetAppDirectory()+"tips.ini";
					WritePrivateProfileString ("tips", 
						strTime, 
						strTrimpedMsg, 
						strFileName); 
					EndDialog(wID);
				}
			}
			//��ǰû��Ϊ�������ʱ���趨���ѣ��Ǿ�ֱ����Ӱɡ�
			else
			{
				WritePrivateProfileString ("tips", 
					strTime, 
					strTrimpedMsg, 
					strFileName); 
				EndDialog(wID);
			}
		}

		return 0;
	}
	// ȡ�ó������е�Ŀ¼���Է�б��\��β��
	WTL::CString GetAppDirectory(bool bEndWithBackSlash=true)
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
};
