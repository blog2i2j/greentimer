// testDlg.h : header file
//

#if !defined(AFX_TESTDLG_H__C91FF804_3B3F_4C37_AC28_CEE88E549887__INCLUDED_)
#define AFX_TESTDLG_H__C91FF804_3B3F_4C37_AC28_CEE88E549887__INCLUDED_

#include "CaptureVideo.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTestDlg dialog

class CTestDlg : public CDialog
{
// Construction
public:
	CTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestDlg)
	enum { IDD = IDD_TEST_DIALOG };
	CStatic	m_staticScreen;
	CComboBox	m_ListCtrl;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnPhoto();
	afx_msg void OnHavealook();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CCaptureVideo m_cap;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTDLG_H__C91FF804_3B3F_4C37_AC28_CEE88E549887__INCLUDED_)
