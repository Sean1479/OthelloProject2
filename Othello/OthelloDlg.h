
// OthelloDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// COthelloDlg 对话框
class COthelloDlg : public CDialogEx
{
// 构造
public:
	COthelloDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_OTHELLO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNumberButtonClick(UINT nID);
	afx_msg void OnBnClickedButtonAifirst();
	afx_msg void OnBnClickedButtonRestart();

public:
	CButton buttons[10][10];
	CButton m_buttonAIFirst;
	int AIfirst;
	afx_msg void OnBnClickedButtonPvp();
	afx_msg void OnBnClickedButtonMvr();
	afx_msg void OnBnClickedButtonRvm();
	afx_msg void OnBnClickedButtonM1v5();
	afx_msg void OnBnClickedButtonM5v1();
};
