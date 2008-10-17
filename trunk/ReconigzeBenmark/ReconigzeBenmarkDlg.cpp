// ReconigzeBenmarkDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ReconigzeBenmark.h"
#include "ReconigzeBenmarkDlg.h"
#include "DataPlotDlg.h"

const int c_iFrameSize = 320*240*3+8;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CReconigzeBenmarkDlg �Ի���




CReconigzeBenmarkDlg::CReconigzeBenmarkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReconigzeBenmarkDlg::IDD, pParent),m_finder(320,240)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CReconigzeBenmarkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_slider);
}

BEGIN_MESSAGE_MAP(CReconigzeBenmarkDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CReconigzeBenmarkDlg::OnBnClickedOk)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CReconigzeBenmarkDlg::OnNMCustomdrawSlider1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &CReconigzeBenmarkDlg::OnBnClickedCancel)
	ON_BN_CLICKED(ID_STOP, &CReconigzeBenmarkDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_ANAYLICS, &CReconigzeBenmarkDlg::OnBnClickedAnaylics)
END_MESSAGE_MAP()


// CReconigzeBenmarkDlg ��Ϣ�������

BOOL CReconigzeBenmarkDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��


	CFile data(_T("aa.data"),CFile::modeRead);
	//data.Seek(0,CFile::end);
	//data.GetLength()
	m_slider.SetRange(0,data.GetLength()/c_iFrameSize);
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CReconigzeBenmarkDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�
struct VData
{
	int x;
	int y;
	unsigned char data[];
};


const int lWidth = 320;

COLORREF GetPointColor(BYTE * pBuffer,int iRow, int iCol)
{
	//��ɫ�ǰ�B-G-R˳�򱣳ֵ�
	return RGB(
		*(pBuffer + 3*(iRow*lWidth + iCol) + 2)
		,*(pBuffer + 3*(iRow*lWidth + iCol) + 1)
		,*(pBuffer + 3*(iRow*lWidth + iCol) ));
}

void CReconigzeBenmarkDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		char buf[c_iFrameSize]={0};
		//E:\svns\svn_new_greenTimer\AirWalker\aa.data
		CFile data(_T("aa.data"),CFile::modeRead);
		int iPos = m_slider.GetPos();
		if(data.Seek(c_iFrameSize*iPos,CFile::begin)<0)
		{
			CDialog::OnPaint();
			return ;
		}

		data.Read(buf,c_iFrameSize);
		VData *vd = (VData *)buf;

		//TRACE(_T("%d %d %d\n"),*(vd->data),*(vd->data + 3),*(vd->data + 6));
		//m_finder.DrawRect(vd->data,320/2,240/2,10,RGB(200,200,0));

		int iX=0,iY=0,iD=0;
		m_finder.FindPoint(vd->data,iX,iY,iD);
		m_finder.DrawRect(vd->data,iX,iY,iD,RGB(200,200,0));

		CPaintDC dc(this); // ���ڻ��Ƶ��豸������
		CRect rect;
		GetClientRect(&rect);
		for(int x=0;x<vd->x;x++)
		{
			for(int y=0;y<vd->y;y++)
			{
				//dc.SetPixel(x,y,x*y);
				dc.SetPixel(x,vd->y - y,GetPointColor(vd->data,y,x));
			}
		}
		

		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CReconigzeBenmarkDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CReconigzeBenmarkDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//OnOK();
	//Invalidate();
	SetTimer(0,500,NULL);
}

void CReconigzeBenmarkDlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

void CReconigzeBenmarkDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (m_slider.GetPos()==m_slider.GetRangeMax())
	{
		KillTimer(0);
	}
	m_slider.SetPos(m_slider.GetPos()+1);
	Invalidate();

	CDialog::OnTimer(nIDEvent);
}

void CReconigzeBenmarkDlg::OnBnClickedCancel()
{
	OnCancel();
}

void CReconigzeBenmarkDlg::OnBnClickedStop()
{
	KillTimer(0);
}

void CReconigzeBenmarkDlg::OnBnClickedAnaylics()
{
	CDataPlotDlg dlg;
	//dlg.AddData(0,100);
	//dlg.AddData(1,200);
	//dlg.AddData(2,10);
	//dlg.AddData(3,10);
	//dlg.AddData(4,100);

	char buf[c_iFrameSize]={0};
	int iX,iY,iD;
	CFile data(_T("aa.data"),CFile::modeRead);
	for (int iPos=0;iPos<data.GetLength()/c_iFrameSize;iPos++)
	{
		if(data.Seek(c_iFrameSize*iPos,CFile::begin)<0)
		{
			return ;
		}
		data.Read(buf,c_iFrameSize);
		VData *vd = (VData *)buf;
		m_finder.FindPoint(vd->data,iX,iY,iD);
		dlg.AddData(iPos,iD);
	}

	dlg.DoModal();
}
