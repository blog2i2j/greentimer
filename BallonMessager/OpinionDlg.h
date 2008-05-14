#pragma once

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

		//�ڶԻ���������ʱ�򣬸��༭���趨��ʼֵ
		CWindow Wnd = GetDlgItem(IDC_EDT_INPUT);
		Wnd.SetWindowText(_T("Hello,world!"));
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
		CWindow Wnd = GetDlgItem(IDC_EDT_INPUT);
		TCHAR strText[1024];
		Wnd.GetWindowText(strText,1024);
		WTL::CString str = strText;
		str += "\r\n ��ȷ���رնԻ���";
		if(IDOK==MessageBox(str,"����",MB_OKCANCEL))
		{
			EndDialog(wID);
		}
		return 0;
	}
};
