//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Source File : GameDBDlg.cpp                                //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : 游戏DB服务器管理界面对话框                          //
//                                                              //
//////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "servermap.h"
#include "ServerLoader.h"
#include "GameDBDlg.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CGameDBDlg
BEGIN_DLG_ID_WND_EXCHANGE( CGameDBDlg )
DLG_ID_WND( IDC_LISTEN_GAME,   m_ListenGame )
DLG_ID_WND( IDC_LISTEN_GATE,   m_ListenGate )
DLG_ID_WND( IDC_GAMEDB_INFO,   m_ServerInfo )
DLG_ID_WND( IDD_GAME,          m_GameSvr )
DLG_ID_WND( IDD_GATE,          m_GateSvr )
END_DLG_ID_WND_EXCHANGE( CXDlg )

CGameDBDlg::CGameDBDlg(void)
: m_uGameDBOnline(0)
, m_uGameDBAll(0)
, m_uGameDBBusy(0)
, m_uGameCount(0)
, m_uGameOnline(0)
, m_uGameAll(0)
, m_uGateCount(0)
, m_uGateOnline(0)
, m_uGateAll(0)
{
}

CGameDBDlg::~CGameDBDlg(void)
{
}

LRESULT CGameDBDlg::OnInitDialog(void)
{
	RECT rc;
	GetClientRect(&rc);
	rc.top += (DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_HEIGHT);
	rc.top += (DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_HEIGHT);

	Long lHeight = (rc.bottom - rc.top) / 2;
	m_GameSvr.MoveWindow(0, rc.top, rc.right, lHeight);
	rc.top += (DLG_ITEM_GRAP + lHeight);
	m_GateSvr.MoveWindow(0, rc.top, rc.right, lHeight);
	//
	CStringFix strTemp;
	strTemp.Load(IDC_LISTEN_GAME);
	m_ListenGame.SetWindowText(*strTemp);

	strTemp.Load(IDC_LISTEN_GATE);
	m_ListenGate.SetWindowText(*strTemp);
	//
	InitListViewItem(DATA_INDEX_GAME);
	InitListViewItem(DATA_INDEX_GATE);
	return TRUE;
}

LRESULT CGameDBDlg::OnSize(WPARAM, LPARAM lParam, BOOL&)
{
	RECT rc   = { 0 };
	rc.right  = (Long)LOWORD(lParam);
	rc.bottom = (Long)HIWORD(lParam);

	rc.top += (DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_HEIGHT);
	m_ServerInfo.MoveWindow(0, rc.top, rc.right, DLG_ITEM_HEIGHT);

	rc.top += (DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_HEIGHT);
	Long lHeight = (rc.bottom - rc.top) / 2;
	m_GameSvr.MoveWindow(0, rc.top, rc.right, lHeight);
	rc.top += (DLG_ITEM_GRAP + lHeight);
	m_GateSvr.MoveWindow(0, rc.top, rc.right, lHeight);
	return 0;
}

void CGameDBDlg::OnLive(bool bStart)
{
	CStringFix strTemp;
	strTemp.Load(IDC_LISTEN_GAME);
	
	if (bStart){
		UShort     usPort = 0;
		CStringKey strAddr;

		GServerLoaderInst->m_Config.GetServerAddr(CServerConfig::CFG_DEFAULT_GAMEDB, 0, strAddr, usPort);
		strTemp.AppendFormat(TF("[%s]%d"), *strAddr, usPort);
		m_ListenGame.SetWindowText(*strTemp);

		strTemp.Load(IDC_LISTEN_GATE);
		GServerLoaderInst->m_Config.GetServerAddr(CServerConfig::CFG_DEFAULT_GAMEDB, CServerConfig::CFG_DEFAULT_GATE, strAddr, usPort);
		if (usPort > 0) {
			strTemp.AppendFormat(TF("[%s]%d"), *strAddr, usPort);
		}
		else {
			strAddr.Load(IDS_SHARE_LISTEN);
			strTemp += strAddr;
		}
		m_ListenGate.SetWindowText(*strTemp);
	}
	else{
		m_ListenGame.SetWindowText(*strTemp);

		strTemp.Load(IDC_LISTEN_GATE);
		m_ListenGate.SetWindowText(*strTemp);
	}
	m_ServerInfo.SetWindowText(TF(""));
}

void CGameDBDlg::OnLink(Int nServerIndex, uintptr_t utParam)
{
	AddListViewData(nServerIndex, utParam);
}

void CGameDBDlg::OnUpdate(Int nServerIndex, uintptr_t utParam)
{
	UpdateListViewData(nServerIndex, utParam);
}

void CGameDBDlg::OnSync(Int nServerIndex, uintptr_t utParam)
{
	PSERVER_DATA pData = reinterpret_cast<PSERVER_DATA>(utParam);
	if (nServerIndex == DATA_INDEX_GATE) {
		m_uGateCount  = pData->usBusy;
		m_uGateOnline = pData->uOnline;
		m_uGateAll    = pData->uAllCount;
	}
	else if (nServerIndex == DATA_INDEX_GAME) {
		m_uGameCount  = pData->usBusy;
		m_uGameOnline = pData->uOnline;
		m_uGameAll    = pData->uAllCount;
	}
	else {
		assert(nServerIndex == DATA_INDEX_GAMEDB);
		m_uGameDBOnline = pData->uOnline;
		m_uGameDBAll    = pData->uAllCount;
		m_uGameDBBusy   = pData->usBusy / DATAD_PERCENT;
	}
}

void CGameDBDlg::OnSync(Int, CStream&)
{
}

void CGameDBDlg::OnUnlink(Int nServerIndex, uintptr_t utParam)
{
	RemoveListViewData(nServerIndex, utParam);
}

void CGameDBDlg::UpdateServerInfo(void)
{
	CStringFix strInfo;
	strInfo.Load(IDC_GAMEDB_INFO);

	CStringFix strTemp;
	strTemp.Format(*strInfo, m_uGameDBOnline, m_uGameDBAll, m_uGameDBBusy,
				   m_uGameCount, m_uGameOnline, m_uGameAll,
				   m_uGateCount, m_uGateOnline, m_uGateAll);
	m_ServerInfo.SetWindowText(*strTemp);
}

CXListView* CGameDBDlg::GetListView(Int nServerIndex)
{
	if (nServerIndex == DATA_INDEX_GAME) {
		return &m_GameSvr;
	}
	else {
		assert(nServerIndex == DATA_INDEX_GATE);
		return &m_GateSvr;
	}
}





