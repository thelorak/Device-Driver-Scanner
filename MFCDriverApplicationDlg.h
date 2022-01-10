
// MFCDriverApplicationDlg.h : header file
//

#pragma once
#pragma comment (lib, "Setupapi.lib")

#include <dbt.h>
#include <iostream>
#include <tchar.h>
#include <setupapi.h>

// CMFCDriverApplicationDlg dialog
class CMFCDriverApplicationDlg : public CDialogEx
{
// Construction
public:
	CMFCDriverApplicationDlg(CWnd* pParenDt = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCDRIVERAPPLICATION_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnMyDeviceChange(WPARAM wparam, LPARAM lparam);
	void UpdateDevice(PDEV_BROADCAST_DEVICEINTERFACE pDevInf); // Update device
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_strText;
};
