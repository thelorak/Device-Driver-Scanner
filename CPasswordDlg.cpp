#include "pch.h"
#include "CPasswordDlg.h"
#include "resource.h"
#include "framework.h"
#include "var.h"

#define PW "6hoOray9#"

CPasswordDlg::CPasswordDlg(CWnd* pParent /*=nullptr*/)
	:CDialogEx(IDD_PASSWORD, pParent)
	, m_passwd(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Text(pDX, IDC_EDIT1, m_passwd);
}

BEGIN_MESSAGE_MAP(CPasswordDlg, CDialogEx)
END_MESSAGE_MAP()

BOOL CPasswordDlg::OnInitDialog()
{
	SetWindowText(_T("Password for authorization"));
	return TRUE;
}

void CPasswordDlg::OnOK()
{
	UpdateData(TRUE);
	bool flag = hDevInfo && pspDevInfoData;
	if (!m_passwd.Compare(_T(PW)))
	{
		if (flag)
		{
			spPropChangeParams.StateChange = DICS_ENABLE;
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
				MessageBoxW(_T("Access Granted!"), _T("Success"), MB_OK | MB_ICONASTERISK);
				CDialog::OnOK();
			}
		}
	}
	else
	{
		int msg = MessageBox(_T("Wrong Password!"), _T("Error"), MB_RETRYCANCEL | MB_ICONERROR);
		if (msg == IDCANCEL)
			CDialog::OnCancel();
	}
}

void CPasswordDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogEx::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPasswordDlg::OnPaint()
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
HCURSOR CPasswordDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}