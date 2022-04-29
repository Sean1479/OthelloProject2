
// OthelloDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Othello.h"
#include "OthelloDlg.h"
#include "Tree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int x, y;
time_t start, now;
Board b = Board();
Tree UCT(0, b);
Node* best;
int res, searchnum, total = 0;
position rm;
position hm;
int searchtime = 1000;
int searchtime2 = 1000;
int AIfirst = 0;
int Humanfirst = 0;
int PVP = 0;
int MVR = 0;
int RVM = 0;
int M1V5= 0;
int M5V1 = 0;
//int whoiswin;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// COthelloDlg 对话框




COthelloDlg::COthelloDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COthelloDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	AIfirst = 0;
	srand(time(NULL));
}

void COthelloDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	for (int j = 1; j < 9; j++)
	{
		for (int i = 1; i < 9; i++)
		{
			DDX_Control(pDX, 1000 + (j - 1) * 8 + i - 1, buttons[j][i]);
		}
	}
	DDX_Control(pDX, IDC_BUTTON_AIFIRST, m_buttonAIFirst);
}

BEGIN_MESSAGE_MAP(COthelloDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BUTTON1, IDC_BUTTON64, OnNumberButtonClick)
	ON_BN_CLICKED(IDC_BUTTON_AIFIRST, &COthelloDlg::OnBnClickedButtonAifirst)
	ON_BN_CLICKED(IDC_BUTTON_RESTART, &COthelloDlg::OnBnClickedButtonRestart)
	ON_BN_CLICKED(IDC_BUTTON_PVP, &COthelloDlg::OnBnClickedButtonPvp)
	ON_BN_CLICKED(IDC_BUTTON_MVR, &COthelloDlg::OnBnClickedButtonMvr)
	ON_BN_CLICKED(IDC_BUTTON_RVM, &COthelloDlg::OnBnClickedButtonRvm)
	ON_BN_CLICKED(IDC_BUTTON_M1V5, &COthelloDlg::OnBnClickedButtonM1v5)
	ON_BN_CLICKED(IDC_BUTTON_M5V1, &COthelloDlg::OnBnClickedButtonM5v1)
END_MESSAGE_MAP()


// COthelloDlg 消息处理程序

BOOL COthelloDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void COthelloDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void COthelloDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR COthelloDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//按钮消息响应函数
void COthelloDlg::OnNumberButtonClick(UINT nID)
{
	int n = nID - 1000;
	int x, y;
	x = n % 8;//col
	y = n / 8;//row

	if (AIfirst)
	{
		if (!b.isend()) {
			if (!b.isend()) {
				start = clock();
				now = clock();
				searchnum = 0;
				while ((int)(now - start) < 200) {
					UCT.backup(UCT.gettail(b).simulate());
					now = clock();
					searchnum++;
				}
				hm = b.human_move(y, x);
				if (hm.first == -9999 && hm.second == -9999)
				{
					MessageBox("YOU CAN NOY CHOOSE HERE !");
					return;
				}
				total++;
				UCT.nextnode(hm, b);
				b.graph_board(buttons);

				start = clock();
				now = clock();
				searchnum = 0;
				while ((int)(now - start) < 200) {
					UCT.backup(UCT.gettail(b).simulate());
					now = clock();
					searchnum++;
				}
				GetDlgItem(IDC_STATIC_TIPS)->SetWindowText(b.print_score());
			}

			if (b.isend())
			{
				if (b.whoiswin == 0) {
					MessageBox("AI win! 1");
				}
				else if (b.whoiswin == 1) {
					MessageBox("YOU win! 2");
				}
				else if (b.whoiswin == 2) {
					MessageBox("Draw! 3");
				}
				else {
					MessageBox("Error 4");
				}
				return;
			}
			if (!b.isend()) {
				start = clock();
				now = clock();
				searchnum = 0;
				while ((int)(now - start) < searchtime) {
					UCT.backup(UCT.gettail(b).simulate());
					now = clock();
					searchnum++;
				}
				now = clock();
				best = UCT.best_child(NULL, 1.41);
				b.onemove(best->pos);
				UCT.nextnode(best->pos, b);
				total++;
				b.graph_board(buttons);
				GetDlgItem(IDC_STATIC_TIPS)->SetWindowText(b.print_score());
			}
		}
		else
		{
			GetDlgItem(IDC_STATIC_TIPS)->SetWindowText(b.print_score());
			if (b.whoiswin == 0) {
				MessageBox("AI win! 21");
			}
			else if (b.whoiswin == 1) {
				MessageBox("YOU win! 22");
			}
			else if (b.whoiswin == 2) {
				MessageBox("Draw! 23");
			}
			else {
				MessageBox("Error 24");
			}
			return;
		}
	}

	if(Humanfirst)
	{
		if (!b.isend()) {
			start = clock();
			now = clock();
			searchnum = 0;
			while ((int)(now - start) < 500) {
				UCT.backup(UCT.gettail(b).simulate());
				now = clock();
				searchnum++;
			}

			hm = b.human_move(y, x);
			if (hm.first == -9999 && hm.second == -9999)
			{
				MessageBox("YOU CAN NOY CHOOSE HERE !");
				return;
			}
			total++;
			b.graph_board(buttons);
			GetDlgItem(IDC_STATIC_TIPS)->SetWindowText(b.print_score());
			UCT.nextnode(hm, b);

			if (b.isend())
			{
				if (b.whoiswin == 0) {
					MessageBox("YOU win! 31");
				}
				else if (b.whoiswin == 1) {
					MessageBox("AI win! 32");
				}
				else if (b.whoiswin == 2) {
					MessageBox("Draw! 33");
				}
				else {
					MessageBox("Error 34");
				}
				return;
			}
			if (!b.isend()) {
				start = clock();
				now = clock();
				searchnum = 0;
				while ((int)(now - start) < searchtime) {
					UCT.backup(UCT.gettail(b).simulate());
					now = clock();
					searchnum++;
				}
				now = clock();
				best = UCT.best_child(NULL, 1.41);
				b.onemove(best->pos);
				total++;
				UCT.nextnode(best->pos, b);
				b.graph_board(buttons);
				GetDlgItem(IDC_STATIC_TIPS)->SetWindowText(b.print_score());
			}
		}
		else
		{
			GetDlgItem(IDC_STATIC_TIPS)->SetWindowText(b.print_score());
			if (b.whoiswin == 0) {
				MessageBox("YOU win! 41");
			}
			else if (b.whoiswin == 1) {
				MessageBox("AI win! 42");
			}
			else if (b.whoiswin == 2) {
				MessageBox("Draw! 43");
			}
			else {
				MessageBox("Error 44");
			}
			return;
		}
	}
	if (PVP) {
		if (!b.isend()) {

			if (!b.isend()) {
				hm = b.human_move(y, x);
				if (hm.first == -9999 && hm.second == -9999)
				{
					MessageBox("YOU CAN NOY CHOOSE HERE !");
					return;
				}
				total++;
				UCT.nextnode(hm, b);
				b.graph_board(buttons);
				GetDlgItem(IDC_STATIC_TIPS)->SetWindowText(b.print_score());
			}

			if (b.isend())
			{
				if (b.whoiswin == 0) {
					MessageBox("X win! 1");
				}
				else if (b.whoiswin == 1) {
					MessageBox("O win! 2");
				}
				else if (b.whoiswin == 2) {
					MessageBox("Draw! 3");
				}
				else {
					MessageBox("Error 4");
				}
				return;
			}
		}
	}
}

// AI first
void COthelloDlg::OnBnClickedButtonAifirst()
{
	AIfirst = 1;
	Humanfirst = 0;
	PVP = 0;
	MVR = 0;
	RVM = 0;
	M1V5 = 0;
	M5V1 = 0;

	total = 0;
	int searchtime = 1000;
	b.Init();
	UCT.Init(0, b);

	//==intit==
	if (!b.isend()) {
		start = clock();
		now = clock();
		searchnum = 0;
		while ((int)(now - start) < searchtime) {
			UCT.backup(UCT.gettail(b).simulate());
			now = clock();
			searchnum++;
		}
		now = clock();
		best = UCT.best_child(NULL, 1.41);
		b.onemove(best->pos);
		total++;
		b.graph_board(buttons);
		UCT.nextnode(best->pos, b);
	}
	GetDlgItem(IDC_STATIC_TIPS)->SetWindowText(b.print_score());
}

// Human first
void COthelloDlg::OnBnClickedButtonRestart()
{
	AIfirst = 0;
	Humanfirst = 1;
	PVP = 0;
	MVR = 0;
	RVM = 0;
	M1V5 = 0;
	M5V1 = 0;

	//==init==
	total = 0;
	int searchtime = 1000;
	b.Init();
	UCT.Init(0, b);
	b.graph_board(buttons);
	GetDlgItem(IDC_STATIC_TIPS)->SetWindowText(b.print_score());
}

//Human VS Human
void COthelloDlg::OnBnClickedButtonPvp()
{
	AIfirst = 0;
	Humanfirst = 0;
	PVP = 1;
	MVR = 0;
	RVM = 0;
	M1V5 = 0;
	M5V1 = 0;

	total = 0;
	b.Init();
	UCT.Init(0, b);
	b.graph_board(buttons);
	GetDlgItem(IDC_STATIC_TIPS)->SetWindowText(b.print_score());

}


void COthelloDlg::OnBnClickedButtonMvr()
{
	AIfirst = 0;
	Humanfirst = 0;
	PVP = 0;
	MVR = 1;
	RVM = 0;
	M1V5 = 0;
	M5V1 = 0;

	total = 0;
	b.Init();
	UCT.Init(0, b);
	b.graph_board(buttons);
	GetDlgItem(IDC_STATIC_TIPS)->SetWindowText(b.print_score());

	while (!b.isend()) {

		start = clock();
		now = clock();
		searchnum = 0;
		while ((int)(now - start) < searchtime) {
			UCT.backup(UCT.gettail(b).simulate());
			now = clock();
			searchnum++;
		}
		now = clock();
		best = UCT.best_child(NULL, 1.41);
		b.onemove(best->pos);
		UCT.nextnode(best->pos, b);
		total++;
		b.graph_board(buttons);
		GetDlgItem(IDC_STATIC_TIPS)->SetWindowText(b.print_score());

		if (b.isend())
		{
			if (b.whoiswin == 0) {
				MessageBox("MCTS win! 1");
			}
			else if (b.whoiswin == 1) {
				MessageBox("Random win! 2");
			}
			else if (b.whoiswin == 2) {
				MessageBox("Draw! 3");
			}
			else {
				MessageBox("Error 4");
			}
			return;
		}

		if (!b.isend()) {
			hm = b.random_move();
			if (hm.first == -9999 && hm.second == -9999)
			{
				MessageBox("YOU CAN NOY CHOOSE HERE !");
				return;
			}
			total++;
			UCT.nextnode(hm, b);
			b.graph_board(buttons);
			GetDlgItem(IDC_STATIC_TIPS)->SetWindowText(b.print_score());
		}
	}

	if (b.isend())
	{
		if (b.whoiswin == 0) {
			MessageBox("MCTS win! 1");
		}
		else if (b.whoiswin == 1) {
			MessageBox("Random win! 2");
		}
		else if (b.whoiswin == 2) {
			MessageBox("Draw! 3");
		}
		else {
			MessageBox("Error 4");
		}
		return;
	}
}


void COthelloDlg::OnBnClickedButtonRvm()
{
	AIfirst = 0;
	Humanfirst = 0;
	PVP = 0;
	MVR = 0;
	RVM = 1;
	M1V5 = 0;
	M5V1 = 0;

	total = 0;
	b.Init();
	UCT.Init(0, b);
	b.graph_board(buttons);
	GetDlgItem(IDC_STATIC_TIPS)->SetWindowText(b.print_score());

	while (!b.isend()) {

		if (!b.isend()) {
			hm = b.random_move();
			if (hm.first == -9999 && hm.second == -9999)
			{
				MessageBox("YOU CAN NOY CHOOSE HERE !");
				return;
			}
			total++;
			UCT.nextnode(hm, b);
			b.graph_board(buttons);
			GetDlgItem(IDC_STATIC_TIPS)->SetWindowText(b.print_score());
		}

		if (b.isend())
		{
			if (b.whoiswin == 0) {
				MessageBox("Random win! 1");
			}
			else if (b.whoiswin == 1) {
				MessageBox("MCTS win! 2");
			}
			else if (b.whoiswin == 2) {
				MessageBox("Draw! 3");
			}
			else {
				MessageBox("Error 4");
			}
			return;
		}
		start = clock();
		now = clock();
		searchnum = 0;
		while ((int)(now - start) < searchtime) {
			UCT.backup(UCT.gettail(b).simulate());
			now = clock();
			searchnum++;
		}
		now = clock();
		best = UCT.best_child(NULL, 1.41);
		b.onemove(best->pos);
		UCT.nextnode(best->pos, b);
		total++;
		b.graph_board(buttons);
		GetDlgItem(IDC_STATIC_TIPS)->SetWindowText(b.print_score());
	}

	if (b.isend())
	{
		if (b.whoiswin == 0) {
			MessageBox("Random win! 1");
		}
		else if (b.whoiswin == 1) {
			MessageBox("MCTS win! 2");
		}
		else if (b.whoiswin == 2) {
			MessageBox("Draw! 3");
		}
		else {
			MessageBox("Error 4");
		}
		return;
	}
}


void COthelloDlg::OnBnClickedButtonM1v5()
{
	AIfirst = 0;
	Humanfirst = 0;
	PVP = 0;
	MVR = 0;
	RVM = 0;
	M1V5 = 1;
	M5V1 = 0;

	total = 0;
	b.Init();
	UCT.Init(0, b);
	b.graph_board(buttons);
	GetDlgItem(IDC_STATIC_TIPS)->SetWindowText(b.print_score());

	while (!b.isend()) {
		start = clock();
		now = clock();
		searchnum = 0;
		while ((int)(now - start) < 1000) {
			UCT.backup(UCT.gettail(b).simulate());
			now = clock();
			searchnum++;
		}
		now = clock();
		best = UCT.best_child(NULL, 1.41);
		b.onemove(best->pos);
		UCT.nextnodeclear(best->pos, b);
		total++;
		b.graph_board(buttons);
		GetDlgItem(IDC_STATIC_TIPS)->SetWindowText(b.print_score());


		if (b.isend())
		{
			if (b.whoiswin == 0) {
				MessageBox("MCTS-1 win! 1");
			}
			else if (b.whoiswin == 1) {
				MessageBox("MCTS-5 win! 2");
			}
			else if (b.whoiswin == 2) {
				MessageBox("Draw! 3");
			}
			else {
				MessageBox("Error 4");
			}
			return;
		}
		start = clock();
		now = clock();
		searchnum = 0;
		while ((int)(now - start) < 5000) {
			UCT.backup(UCT.gettail(b).simulate());
			now = clock();
			searchnum++;
		}
		now = clock();
		best = UCT.best_child(NULL, 1.41);
		b.onemove(best->pos);
		UCT.nextnodeclear(best->pos, b);
		total++;
		b.graph_board(buttons);
		GetDlgItem(IDC_STATIC_TIPS)->SetWindowText(b.print_score());
	}

	if (b.isend())
	{
		if (b.whoiswin == 0) {
			MessageBox("MCTS-1 win! 1");
		}
		else if (b.whoiswin == 1) {
			MessageBox("MCTS-5 win! 2");
		}
		else if (b.whoiswin == 2) {
			MessageBox("Draw! 3");
		}
		else {
			MessageBox("Error 4");
		}
		return;
	}
}


void COthelloDlg::OnBnClickedButtonM5v1()
{
	AIfirst = 0;
	Humanfirst = 0;
	PVP = 0;
	MVR = 0;
	RVM = 0;
	M1V5 = 1;
	M5V1 = 0;

	total = 0;
	b.Init();
	UCT.Init(0, b);
	b.graph_board(buttons);
	GetDlgItem(IDC_STATIC_TIPS)->SetWindowText(b.print_score());

	while (!b.isend()) {
		start = clock();
		now = clock();
		searchnum = 0;
		while ((int)(now - start) < 5000) {
			UCT.backup(UCT.gettail(b).simulate());
			now = clock();
			searchnum++;
		}
		now = clock();
		best = UCT.best_child(NULL, 1.41);
		b.onemove(best->pos);
		UCT.nextnodeclear(best->pos, b);
		total++;
		b.graph_board(buttons);
		GetDlgItem(IDC_STATIC_TIPS)->SetWindowText(b.print_score());


		if (b.isend())
		{
			if (b.whoiswin == 0) {
				MessageBox("MCTS-5 win! 1");
			}
			else if (b.whoiswin == 1) {
				MessageBox("MCTS-1 win! 2");
			}
			else if (b.whoiswin == 2) {
				MessageBox("Draw! 3");
			}
			else {
				MessageBox("Error 4");
			}
			return;
		}
		start = clock();
		now = clock();
		searchnum = 0;
		while ((int)(now - start) < 1000) {
			UCT.backup(UCT.gettail(b).simulate());
			now = clock();
			searchnum++;
		}
		now = clock();
		best = UCT.best_child(NULL, 1.41);
		b.onemove(best->pos);
		UCT.nextnodeclear(best->pos, b);
		total++;
		b.graph_board(buttons);
		GetDlgItem(IDC_STATIC_TIPS)->SetWindowText(b.print_score());
	}

	if (b.isend())
	{
		if (b.whoiswin == 0) {
			MessageBox("MCTS-5 win! 1");
		}
		else if (b.whoiswin == 1) {
			MessageBox("MCTS-1 win! 2");
		}
		else if (b.whoiswin == 2) {
			MessageBox("Draw! 3");
		}
		else {
			MessageBox("Error 4");
		}
		return;
	}
}
