//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Source File : LoginDBDlg.cpp                               //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : 登陆DB服务器界面对话框                             //
//                                                              //
//////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "ServerLoader.h"
#include "LoginDBDlg.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CLoginDBDlg
BEGIN_DLG_ID_WND_EXCHANGE( CLoginDBDlg )
DLG_ID_WND( IDC_LISTEN_LOGIN,   m_ListenLogin )
DLG_ID_WND( IDC_LOGINDB_INFO,   m_ServerInfo )
DLG_ID_WND( IDD_LOGIN,          m_LoginSvr )
END_DLG_ID_WND_EXCHANGE( CXDlg )

CLoginDBDlg::CLoginDBDlg(void)
: m_uLoginDBOnline(0)
, m_uLoginDBAll(0)
, m_uLoginDBBusy(0)
{
}

CLoginDBDlg::~CLoginDBDlg(void)
{
}

LRESULT CLoginDBDlg::OnInitDialog(void)
{
	RECT rc;
	GetClientRect(&rc);
	rc.top += (DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_HEIGHT);
	rc.top += (DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_HEIGHT);

	m_LoginSvr.MoveWindow(0, rc.top, rc.right, (rc.bottom - rc.top));
	//
	CStringFix strTemp;
	strTemp.Load(IDC_LISTEN_LOGIN);
	m_ListenLogin.SetWindowText(*strTemp);
	//
	InitListViewItem(DATA_INDEX_LOGIN);
	return TRUE;
}

LRESULT CLoginDBDlg::OnSize(WPARAM, LPARAM lParam, BOOL&)
{
	RECT rc   = { 0 };
	rc.right  = (Long)LOWORD(lParam);
	rc.bottom = (Long)HIWORD(lParam);

	rc.top += (DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_HEIGHT);
	m_ServerInfo.MoveWindow(0, rc.top, rc.right, DLG_ITEM_HEIGHT);

	rc.top += (DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_HEIGHT);
	m_LoginSvr.MoveWindow(0, rc.top, rc.right, (rc.bottom - rc.top));
	return 0;
}

void CLoginDBDlg::OnLive(bool bStart)
{
	CStringFix strTemp;
	strTemp.Load(IDC_LISTEN_LOGIN);
	
	if (bStart){
		UShort     usPort = 0;
		CStringKey strAddr;

		GServerLoaderInst->m_Config.GetServerAddr(CServerConfig::CFG_DEFAULT_LOGINDB, 0, strAddr, usPort);
		strTemp.AppendFormat(TF("[%s]%d"), *strAddr, usPort);
		m_ListenLogin.SetWindowText(*strTemp);
	}
	else{
		m_ListenLogin.SetWindowText(*strTemp);
	}
	m_ServerInfo.SetWindowText(TF(""));
}

void CLoginDBDlg::OnLink(Int nServerIndex, uintptr_t utParam)
{
	AddListViewData(nServerIndex, utParam);
}

void CLoginDBDlg::OnUpdate(Int nServerIndex, uintptr_t utParam)
{
	UpdateListViewData(nServerIndex, utParam);
}

void CLoginDBDlg::OnSync(Int nServerIndex, uintptr_t utParam)
{
	UNREFERENCED_PARAMETER(nServerIndex);
	assert(nServerIndex == DATA_INDEX_LOGINDB);

	PSERVER_DATA pData = reinterpret_cast<PSERVER_DATA>(utParam);
	m_uLoginDBOnline = pData->uOnline;
	m_uLoginDBAll    = pData->uAllCount;
	m_uLoginDBBusy   = pData->usBusy / DATAD_PERCENT;
}

void CLoginDBDlg::OnSync(Int, CStream&)
{
}

void CLoginDBDlg::OnUnlink(Int nServerIndex, uintptr_t utParam)
{
	RemoveListViewData(nServerIndex, utParam);
}

void CLoginDBDlg::UpdateServerInfo(void)
{
	CStringFix strInfo;
	strInfo.Load(IDC_LOGINDB_INFO);

	CStringFix strTemp;
	strTemp.Format(*strInfo, m_uLoginDBOnline, m_uLoginDBAll, m_uLoginDBBusy);
	m_ServerInfo.SetWindowText(*strTemp);
}

CXListView* CLoginDBDlg::GetListView(Int nServerIndex)
{
	UNREFERENCED_PARAMETER(nServerIndex);
	assert(nServerIndex == DATA_INDEX_LOGIN);
	return &m_LoginSvr;
}




