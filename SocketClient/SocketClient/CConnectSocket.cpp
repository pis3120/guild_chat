#include "stdafx.h"
#include "CConnectSocket.h"
#include "SocketClientDlg.h"


CConnectSocket::CConnectSocket()
{
}


CConnectSocket::~CConnectSocket()
{
}

void CConnectSocket::OnClose(int nErrorCode)
{
	ShutDown();
	Close();

	CSocket::OnClose(nErrorCode);

	AfxMessageBox(_T("ERROR:서버와의 연결이 끊겼습니다.!"));
	::PostQuitMessage(0);
}

void CConnectSocket::OnReceive(int nErrorCode)
{
	TCHAR szBuffer[1024];
	::ZeroMemory(szBuffer, sizeof(szBuffer));

	if (Receive(szBuffer, sizeof(szBuffer)) > 0) {
		CSocketClientDlg* pMain = (CSocketClientDlg*)AfxGetMainWnd();
		int cnt = pMain->m_List.GetCount();
		CString originText = szBuffer;

		CString RecName = pMain->name;
		RecName = L"[" + RecName + L"]:";

		if (originText.Find(RecName) != -1) {
			originText = L"(나) " + originText;
		}
		else {
			originText = L"   " + originText;
		}
		pMain->m_List.InsertString(cnt, originText);
		pMain->checknew = 1;
	}
	CSocket::OnReceive(nErrorCode);
}