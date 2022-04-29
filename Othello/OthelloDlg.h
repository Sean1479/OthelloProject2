
// OthelloDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// COthelloDlg �Ի���
class COthelloDlg : public CDialogEx
{
// ����
public:
	COthelloDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_OTHELLO_DIALOG };

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
