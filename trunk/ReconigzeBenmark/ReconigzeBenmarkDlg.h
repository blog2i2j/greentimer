// ReconigzeBenmarkDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"


// CReconigzeBenmarkDlg �Ի���
class CReconigzeBenmarkDlg : public CDialog
{
// ����
public:
	CReconigzeBenmarkDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_RECONIGZEBENMARK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_slider;
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
};
