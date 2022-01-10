#pragma once
#include <afxdialogex.h>

class CPasswordDlg :
    public CDialogEx
{
public:
    CPasswordDlg(CWnd* CDialog = nullptr);
#ifdef AFX_DESIGN_TIME
    enum {IDD=IDD_PASSWORD}; //ID of Password Dialog  
#endif
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);
protected:
    HICON m_hIcon;

    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg void OnOK();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
public:
    CString m_passwd;
};

