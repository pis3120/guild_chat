#include "stdafx.h"
#include "SocketServer.h"
#include "SocketServerDlg.h"
#include "CClientSocket.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CSocketServerDlg 대화 상자



CSocketServerDlg::CSocketServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SOCKETSERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSocketServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}

BEGIN_MESSAGE_MAP(CSocketServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_LBN_SELCHANGE(IDC_CLIENT_LIST, &CSocketServerDlg::OnLbnSelchangeClientList)
	ON_BN_CLICKED(IDCANCEL, &CSocketServerDlg::OnBnClickedCancel)
	ON_LBN_SELCHANGE(IDC_LIST1, &CSocketServerDlg::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// CSocketServerDlg 메시지 처리기

BOOL CSocketServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	AfxMessageBox(_T("서버를 시작합니다."));
	m_List.AddString(_T("서버를 시작합니다."));

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

	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);	

	clientList = (CListBox*)GetDlgItem(IDC_CLIENT_LIST);

	if (m_ListenSocket.Create(8100, SOCK_STREAM)) { // 소켓생성
		if (!m_ListenSocket.Listen()) {
			AfxMessageBox(_T("ERROR:Listen() return False"));
		}
	}
	else {
		AfxMessageBox(_T("ERROR:서버 소켓을 생성하는데 실패하였습니다!"));
	}
	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CSocketServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSocketServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}


HCURSOR CSocketServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSocketServerDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	POSITION pos;

	pos = m_ListenSocket.m_ptrClientSocketList.GetHeadPosition();
	CClientSocket* pClient = NULL;

	while (pos != NULL) {
		pClient = (CClientSocket*)m_ListenSocket.m_ptrClientSocketList.GetNext(pos);
		if (pClient != NULL) {
			pClient->ShutDown();
			pClient->Close();

			delete pClient;
		}
	}
	m_ListenSocket.ShutDown();
	m_ListenSocket.Close();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CSocketServerDlg::OnLbnSelchangeClientList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CSocketServerDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}


void CSocketServerDlg::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

