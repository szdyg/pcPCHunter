
// ArkProtectAppDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "Global.hpp"
#include "afxcmn.h"
#include "ProcessDlg.h"
#include "DriverDlg.h"
#include "KernelDlg.h"
#include "HookDlg.h"
#include "RegistryDlg.h"

// CArkProtectAppDlg 对话框
class CArkProtectAppDlg : public CDialogEx
{
// 构造
public:
    CArkProtectAppDlg(CWnd* pParent = NULL);    // 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ARKPROTECTAPP_DIALOG };
#endif

    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持


// 实现
protected:
    HICON m_hIcon;

    // 生成的消息映射函数
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg LRESULT OnIconNotify(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnUpdateStatusBarTip(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnUpdateStatusBarDetail(WPARAM wParam, LPARAM lParam);
    afx_msg void OnIconnotifyDisplay();
    afx_msg void OnIconnotifyHide();
    afx_msg void OnIconnotifyExit();
    afx_msg void OnClose();
    afx_msg void OnDestroy();
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnStnClickedProcessStatic();
    afx_msg void OnStnClickedDriverStatic();
    afx_msg void OnStnClickedKernelStatic();
    afx_msg void OnStnClickedHookStatic();
    afx_msg void OnStnClickedRegistryStatic();

    DECLARE_MESSAGE_MAP()
public:

    void APInitializeTray();

    void APEnableCurrentButton(ArkProtect::eChildDlg ChildDlg);

    void APShowChildWindow(ArkProtect::eChildDlg ChildDlg);



    CTabCtrl              m_AppTab;
    CStatic               m_ProcessButton;
    CStatic               m_DriverButton;
    CStatic               m_KernelButton;
    CStatic               m_HookButton;
    CStatic               m_RegistryButton;
    CStatic               m_AboutButton;
    NOTIFYICONDATA        m_NotifyIcon = { 0 };   // 任务栏图标
    CStatusBarCtrl        *m_StatusBar;
    ArkProtect::CGlobal   m_Global;
    ArkProtect::eChildDlg m_CurrentChildDlg = (ArkProtect::eChildDlg)(-1);    // 子对话框
    CProcessDlg           *m_ProcessDlg = NULL;
    CDriverDlg            *m_DriverDlg = NULL;
    CKernelDlg            *m_KernelDlg = NULL;
    CHookDlg              *m_HookDlg = NULL;
    CRegistryDlg          *m_RegistryDlg = NULL;

    

};
