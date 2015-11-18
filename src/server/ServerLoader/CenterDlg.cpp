//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Source File : CenterDlg.cpp                                //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : 中心服务器管理界面对话框                            //
//                                                              //
//////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "servermap.h"
#include "ServerLoader.h"
#include "CenterDlg.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CCenterDlg
BEGIN_DLG_ID_WND_EXCHANGE( CCenterDlg )
DLG_ID_WND( IDC_SELECT_ADDR,  m_ListenSelect )
DLG_ID_WND( IDC_LOGIN_ADDR,   m_ListenLogin )
DLG_ID_WND( IDC_GAME_ADDR,    m_ListenGame )
DLG_ID_WND( IDC_CENTER_INFO,  m_ServerInfo)
DLG_ID_WND( IDD_SELECT,       m_SelectSvr )
DLG_ID_WND( IDD_LOGIN,        m_LoginSvr )
DLG_ID_WND( IDD_GAME,         m_GameSvr )
END_DLG_ID_WND_EXCHANGE( CXDlg )

CCenterDlg::CCenterDlg(void)
: m_uCenterOnline(0)
, m_uCenterAll(0)
, m_uCenterBusy(0)
, m_uSelectCount(0)
, m_uSelectOnline(0)
, m_uSelectAll(0)
, m_uLoginCount(0)
, m_uLoginOnline(0)
, m_uLoginAll(0)
, m_uGameCount(0)
, m_uGameOnline(0)
, m_uGameAll(0)
{
}

CCenterDlg::~CCenterDlg(void)
{
}

LRESULT CCenterDlg::OnInitDialog(void)
{
	RECT rc;
	GetClientRect(&rc);
	rc.top += (DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_HEIGHT);
	rc.top += (DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_HEIGHT);

	Long lHeight = (rc.bottom - rc.top) / 3;
	m_SelectSvr.MoveWindow(0, rc.top, rc.right, lHeight);
	rc.top += (DLG_ITEM_GRAP + lHeight);
	m_LoginSvr.MoveWindow(0, rc.top, rc.right, lHeight);
	rc.top += (DLG_ITEM_GRAP + lHeight);
	m_GameSvr.MoveWindow(0, rc.top, rc.right, lHeight);
	//
	CStringFix strTemp;
	strTemp.Load(IDC_SELECT_ADDR);
	m_ListenSelect.SetWindowText(*strTemp);

	strTemp.Load(IDC_LOGIN_ADDR);
	m_ListenLogin.SetWindowText(*strTemp);

	strTemp.Load(IDC_GAME_ADDR);
	m_ListenGame.SetWindowText(*strTemp);
	//
	InitListViewItem(DATA_INDEX_SELECT);
	InitListViewItem(DATA_INDEX_LOGIN);
	InitListViewItem(DATA_INDEX_GAME);
	return TRUE;
}

LRESULT CCenterDlg::OnSize(WPARAM, LPARAM lParam, BOOL&)
{
	RECT rc   = { 0 };
	rc.right  = (Long)LOWORD(lParam);
	rc.bottom = (Long)HIWORD(lParam);

	rc.top += (DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_HEIGHT);
	m_ServerInfo.MoveWindow(0, rc.top, rc.right, DLG_ITEM_HEIGHT);

	rc.top += (DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_HEIGHT);

	Long lHeight = (rc.bottom - rc.top) / 3;
	m_SelectSvr.MoveWindow(0, rc.top, rc.right, lHeight);
	rc.top += (DLG_ITEM_GRAP + lHeight);
	m_LoginSvr.MoveWindow(0, rc.top, rc.right, lHeight);
	rc.top += (DLG_ITEM_GRAP + lHeight);
	m_GameSvr.MoveWindow(0, rc.top, rc.right, lHeight);
	return 0;
}

void CCenterDlg::OnLive(bool bStart)
{
	if (bStart) {
		Int        nPort = 0;
		CStringKey strAddr;

		CStringFix strTemp;
		strTemp.Load(IDC_SELECT_ADDR);
		GServerLoaderInst->m_Config.GetServerAddr(CServerConfig::CFG_DEFAULT_CENTER, CServerConfig::CFG_DEFAULT_SELECT, strAddr, nPort);
		strTemp.AppendFormat(TF("[%s]%d"), *strAddr, nPort);
		m_ListenSelect.SetWindowText(*strTemp);

		strTemp.Load(IDC_LOGIN_ADDR);
		GServerLoaderInst->m_Config.GetServerAddr(CServerConfig::CFG_DEFAULT_CENTER, CServerConfig::CFG_DEFAULT_LOGIN, strAddr, nPort);
		strTemp.AppendFormat(TF("[%s]%d"), *strAddr, nPort);
		m_ListenLogin.SetWindowText(*strTemp);

		strTemp.Load(IDC_GAME_ADDR);
		GServerLoaderInst->m_Config.GetServerAddr(CServerConfig::CFG_DEFAULT_CENTER, CServerConfig::CFG_DEFAULT_GAME, strAddr, nPort);
		strTemp.AppendFormat(TF("[%s]%d"), *strAddr, nPort);
		m_ListenGame.SetWindowText(*strTemp);
	}
	else {
		CStringFix strTemp;
		strTemp.Load(IDC_SELECT_ADDR);
		m_ListenSelect.SetWindowText(*strTemp);

		strTemp.Load(IDC_LOGIN_ADDR);
		m_ListenLogin.SetWindowText(*strTemp);

		strTemp.Load(IDC_GAME_ADDR);
		m_ListenGame.SetWindowText(*strTemp);
	}
	m_ServerInfo.SetWindowText(TF(""));
}

void CCenterDlg::OnLink(Int nServerIndex, uintptr_t utParam)
{
	AddListViewData(nServerIndex, utParam);
}

void CCenterDlg::OnUpdate(Int nServerIndex, uintptr_t utParam)
{
	UpdateListViewData(nServerIndex, utParam);
}

void CCenterDlg::OnSync(Int nServerIndex, uintptr_t utParam)
{
	PSERVER_DATA pData = reinterpret_cast<PSERVER_DATA>(utParam);
	if (nServerIndex == DATA_INDEX_GAME) {
		m_uGameCount    = pData->usBusy;
		m_uGameOnline   = pData->uOnline;
		m_uGameAll      = pData->uAllCount;
	}
	else if (nServerIndex == DATA_INDEX_LOGIN) {
		m_uLoginCount   = pData->usBusy;
		m_uLoginOnline  = pData->uOnline;
		m_uLoginAll     = pData->uAllCount;
	}
	else if (nServerIndex == DATA_INDEX_SELECT) {
		m_uSelectCount  = pData->usBusy;
		m_uSelectOnline = pData->uOnline;
		m_uSelectAll    = pData->uAllCount;
	}
	else {
		assert(nServerIndex == DATA_INDEX_CENTER);
		m_uCenterOnline = pData->uOnline;
		m_uCenterAll    = pData->uAllCount;
		m_uCenterBusy   = pData->usBusy / DATAD_PERCENT;
	}
}

void CCenterDlg::OnSync(Int, CStream&)
{
}

void CCenterDlg::OnUnlink(Int nServerIndex, uintptr_t utParam)
{
	RemoveListViewData(nServerIndex, utParam);
}

void CCenterDlg::UpdateServerInfo(void)
{
	CStringFix strInfo;
	strInfo.Load(IDC_CENTER_INFO);

	CStringFix strTemp;
	strTemp.Format(*strInfo, m_uCenterOnline, m_uCenterAll, m_uCenterBusy,
				   m_uSelectCount, m_uSelectOnline, m_uSelectAll,
				   m_uLoginCount, m_uLoginOnline, m_uLoginAll,
				   m_uGameCount, m_uGameOnline, m_uGameAll);
	m_ServerInfo.SetWindowText(*strTemp);
}

CXListView* CCenterDlg::GetListView(Int nServerIndex)
{
	if (nServerIndex == DATA_INDEX_GAME) {
		return &m_GameSvr;
	}
	else if (nServerIndex == DATA_INDEX_LOGIN) {
		return &m_LoginSvr;
	}
	else {
		assert(nServerIndex == DATA_INDEX_SELECT);
		return &m_SelectSvr;
	}
}




