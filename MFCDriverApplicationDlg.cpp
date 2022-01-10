
// MFCDriverApplicationDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MFCDriverApplication.h"
#include "MFCDriverApplicationDlg.h"
#include "CPasswordDlg.h"
#include "afxdialogex.h"
#include "var.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define MEMBERINDEX 0
#define BUFFERSIZE 1024


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCDriverApplicationDlg dialog



CMFCDriverApplicationDlg::CMFCDriverApplicationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCDRIVERAPPLICATION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCDriverApplicationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC3, m_strText);
}

BEGIN_MESSAGE_MAP(CMFCDriverApplicationDlg, CDialogEx)
	ON_MESSAGE(WM_DEVICECHANGE, CMFCDriverApplicationDlg::OnMyDeviceChange)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CMFCDriverApplicationDlg message handlers

BOOL CMFCDriverApplicationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);    	// Set small icon

	// TODO: Add extra initialization here
	SetWindowText(_T("Driver Detection"));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

LRESULT CMFCDriverApplicationDlg::OnMyDeviceChange(WPARAM wparam, LPARAM lparam)
{
	if (wparam == DBT_DEVICEARRIVAL)
	{

		PDEV_BROADCAST_HDR phdr = (PDEV_BROADCAST_HDR)lparam;
		PDEV_BROADCAST_VOLUME pdevol;
		PDEV_BROADCAST_DEVICEINTERFACE pdevint;
		switch (phdr->dbch_devicetype)
		{
			case DBT_DEVTYP_VOLUME:
			{
				pdevol = (PDEV_BROADCAST_VOLUME)phdr;
				m_strText = "USB flash drive";
				UpdateData(FALSE);
			}
			case DBT_DEVTYP_DEVICEINTERFACE:
			{
				pdevint = (PDEV_BROADCAST_DEVICEINTERFACE)phdr;
				m_strText = pdevint->dbcc_name; // print device name
				UpdateData(FALSE);
				//CMFCDriverApplicationDlg::UpdateDevice(pdevint);
			}
		}
		//CPasswordDlg pwddlg;
		//pwddlg.DoModal();
	}
	return 0;
}

 void CMFCDriverApplicationDlg::UpdateDevice(PDEV_BROADCAST_DEVICEINTERFACE pDevInf)
{
	 // format device name
	 int test = lstrlen(pDevInf->dbcc_name);
	 ASSERT(test > 4);
	 CString szDevId = pDevInf->dbcc_name + 4;
	 int idx = szDevId.ReverseFind(_T('#'));
	 ASSERT(-1 != idx);
	 szDevId.Truncate(idx);
	 szDevId.Replace(_T('#'), _T('\\'));
	 szDevId.MakeUpper();
	 CString szClass;
	 idx = szDevId.Find(_T('\\'));
	 ASSERT(-1 != idx);
	 szClass = szDevId.Left(idx);

	 // locate device in the information set
	 hDevInfo = SetupDiGetClassDevs(NULL, szClass, NULL, NULL);
	 pspDevInfoData = new SP_DEVINFO_DATA;
	 pspDevInfoData->cbSize = sizeof(SP_DEVINFO_DATA);
	 for (int i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, pspDevInfoData); i++)
	 {
		 DWORD DataT;
		 DWORD nSize = 0;
		 TCHAR buf[BUFFERSIZE];

		 if (!SetupDiGetDeviceInstanceIdA(hDevInfo, pspDevInfoData, (PSTR)buf, sizeof(buf), &nSize))
		 {
			 AfxMessageBox(CString("SetupDiGetDeviceInstanceId(): ")
				 + _com_error(GetLastError()).ErrorMessage(), MB_ICONEXCLAMATION);
			 break;
		 }

		 if (szDevId == buf)
		 {
			 // device found
			 if (SetupDiGetDeviceRegistryPropertyA(hDevInfo, pspDevInfoData,
				 SPDRP_FRIENDLYNAME, &DataT, (PBYTE)buf, sizeof(buf), &nSize)) {
				 // do nothing
			 }
			 else if (SetupDiGetDeviceRegistryProperty(hDevInfo, pspDevInfoData,
				 SPDRP_DEVICEDESC, &DataT, (PBYTE)buf, sizeof(buf), &nSize)) {
				 // do nothing
			 }
			 else {
				 lstrcpy(buf, _T("Unknown"));
			 }
			 m_strText = buf; // print device name
			 UpdateData(FALSE);
			 break;
		 }
	 }

	 // remove device upon insertion
	 spPropChangeParams.ClassInstallHeader.cbSize = sizeof(SP_CLASSINSTALL_HEADER);
	 spPropChangeParams.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
	 spPropChangeParams.Scope = DICS_FLAG_GLOBAL;
	 spPropChangeParams.HwProfile = 0; // current hardware profile
	 spPropChangeParams.StateChange = DICS_DISABLE;

	if (!SetupDiSetClassInstallParams(hDevInfo, pspDevInfoData,
		(SP_CLASSINSTALL_HEADER*)&spPropChangeParams, sizeof(SP_PROPCHANGE_PARAMS)))
	{
		exit(1);
	}
	else if (!SetupDiCallClassInstaller(DIF_PROPERTYCHANGE, hDevInfo, pspDevInfoData))
	{
		exit(1);
	}
	else
	{
		// Device removed
	}
} 

void CMFCDriverApplicationDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCDriverApplicationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCDriverApplicationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

