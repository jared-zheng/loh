//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Source File : GameDlg.cpp                                  //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : 游戏服务器管理界面对话框                            //
//                                                              //
//////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "network.h"
#include "servermap.h"
#include "ServerLoader.h"
#include "GameDlg.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CGameDlg
BEGIN_DLG_ID_WND_EXCHANGE( CGameDlg )
DLG_ID_WND( IDC_CONNECT_CENTER, m_ConnectCenter )
DLG_ID_WND( IDC_CONNECT_GAMEDB, m_ConnectGameDB )
DLG_ID_WND( IDC_PING_SERVICE,   m_PingService )
DLG_ID_WND( IDC_LISTEN_ZONE,    m_ListenZone )
DLG_ID_WND( IDC_LISTEN_GATE,    m_ListenGate )
DLG_ID_WND( IDC_UDP_SERVICE,    m_UDPService )
DLG_ID_WND( IDC_GAME_INFO,      m_ServerInfo )
DLG_ID_WND( IDD_ZONE,           m_ZoneSvr )
DLG_ID_WND( IDD_GATE,           m_GateSvr )
END_DLG_ID_WND_EXCHANGE( CXDlg )

CGameDlg::CGameDlg(void)
: m_uGameOnline(0)
, m_uGameAll(0)
, m_uGameBusy(0)
, m_uZoneCount(0)
, m_uZoneOnline(0)
, m_uZoneAll(0)
, m_uGateCount(0)
, m_uGateOnline(0)
, m_uGateAll(0)
{
}

CGameDlg::~CGameDlg(void)
{
}

LRESULT CGameDlg::OnInitDialog(void)
{
	RECT rc;
	GetClientRect(&rc);
	rc.top += (DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_HEIGHT);
	rc.top += (DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_HEIGHT);
	rc.top += (DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_HEIGHT);

	Long lHeight = (rc.bottom - rc.top) / 2;
	m_ZoneSvr.MoveWindow(0, rc.top, rc.right, lHeight);
	rc.top += (DLG_ITEM_GRAP + lHeight);
	m_GateSvr.MoveWindow(0, rc.top, rc.right, lHeight);

	CStringFix strTemp;
	strTemp.Load(IDC_CONNECT_CENTER);
	m_ConnectCenter.SetWindowText(*strTemp);

	strTemp.Load(IDC_CONNECT_GAMEDB);
	m_ConnectGameDB.SetWindowText(*strTemp);

	strTemp.Load(IDC_PING_SERVICE);
	m_PingService.SetWindowText(*strTemp);

	strTemp.Load(IDC_LISTEN_ZONE);
	m_ListenZone.SetWindowText(*strTemp);

	strTemp.Load(IDC_LISTEN_GATE);
	m_ListenGate.SetWindowText(*strTemp);

	strTemp.Load(IDC_UDP_SERVICE);
	m_UDPService.SetWindowText(*strTemp);
	//
	InitListViewItem(DATA_INDEX_ZONE);
	InitListViewItem(DATA_INDEX_GATE);
	return TRUE;
}

LRESULT CGameDlg::OnSize(WPARAM, LPARAM lParam, BOOL&)
{
	RECT rc   = { 0 };
	rc.right  = (Long)LOWORD(lParam);
	rc.bottom = (Long)HIWORD(lParam);

	rc.top += (DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_HEIGHT);
	rc.top += (DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_HEIGHT);
	m_ServerInfo.MoveWindow(0, rc.top, rc.right, DLG_ITEM_HEIGHT);

	rc.top += (DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_HEIGHT);

	Long lHeight = (rc.bottom - rc.top) / 2;
	m_ZoneSvr.MoveWindow(0, rc.top, rc.right, lHeight);
	rc.top += (DLG_ITEM_GRAP + lHeight);
	m_GateSvr.MoveWindow(0, rc.top, rc.right, lHeight);
	return 0;
}

void CGameDlg::OnLive(bool bStart)
{
	CStringFix strTemp;
	if (bStart){
		UShort     usPort = 0;
		CStringKey strAddr;

		strTemp.Load(IDC_PING_SERVICE);
		GServerLoaderInst->m_Config.GetServerAddr(CServerConfig::CFG_DEFAULT_GAME, 0, strAddr, usPort);
		m_PingService.SetWindowText(*strAddr);

		strTemp.Load(IDC_LISTEN_ZONE);
		GServerLoaderInst->m_Config.GetServerAddr(CServerConfig::CFG_DEFAULT_GAME, CServerConfig::CFG_DEFAULT_ZONE, strAddr, usPort);
		strTemp.AppendFormat(TF("[%s]%d"), *strAddr, usPort);
		m_ListenZone.SetWindowText(*strTemp);

		strTemp.Load(IDC_LISTEN_GATE);
		GServerLoaderInst->m_Config.GetServerAddr(CServerConfig::CFG_DEFAULT_GAME, CServerConfig::CFG_DEFAULT_GATE, strAddr, usPort);
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
		strTemp.Load(IDC_CONNECT_CENTER);
		m_ConnectCenter.SetWindowText(*strTemp);

		strTemp.Load(IDC_CONNECT_GAMEDB);
		m_ConnectGameDB.SetWindowText(*strTemp);

		strTemp.Load(IDC_PING_SERVICE);
		m_PingService.SetWindowText(*strTemp);

		strTemp.Load(IDC_LISTEN_ZONE);
		m_ListenZone.SetWindowText(*strTemp);

		strTemp.Load(IDC_LISTEN_GATE);
		m_ListenGate.SetWindowText(*strTemp);

		strTemp.Load(IDC_UDP_SERVICE);
		m_UDPService.SetWindowText(*strTemp);
	}
	m_ServerInfo.SetWindowText(TF(""));
}

void CGameDlg::OnLink(Int nServerIndex, uintptr_t utParam)
{
	CStringFix strTemp;
	if (nServerIndex == DATA_INDEX_CENTER) {

		strTemp.Load(IDC_CONNECT_CENTER);

		if (utParam != 0) {
			CNETTraits::PNET_ADDR pAddr = (reinterpret_cast<CNETTraits::PNET_ADDR>(utParam));

			UShort     usPort = 0;
			CStringKey strAddr;

			GServerLoaderInst->m_Config.GetServerAddr(CServerConfig::CFG_DEFAULT_CENTER, CServerConfig::CFG_DEFAULT_GAME, strAddr, usPort);
			strTemp.AppendFormat(TF("[%s]%d"), *strAddr, usPort);

			GServerLoaderInst->m_NetworkPtr->TranslateAddr(strAddr, usPort, *pAddr, false);
			strTemp.AppendFormat(TF("([%s]%d)"), *strAddr, usPort);
		}
		else {
			CStringFix strTemp1;
			strTemp1.Load(IDS_SHARE_CENTER);
			strTemp += strTemp1;
		}
		m_ConnectCenter.SetWindowText(*strTemp);
	}
	else if (nServerIndex == DATA_INDEX_GAMEDB) {

		strTemp.Load(IDC_CONNECT_GAMEDB);

		if (utParam != 0) {
			CNETTraits::PNET_ADDR pAddr = (reinterpret_cast<CNETTraits::PNET_ADDR>(utParam));

			UShort     usPort = 0;
			CStringKey strAddr;

			GServerLoaderInst->m_Config.GetServerAddr(CServerConfig::CFG_DEFAULT_GAMEDB, 0, strAddr, usPort);
			strTemp.AppendFormat(TF("[%s]%d"), *strAddr, usPort);

			GServerLoaderInst->m_NetworkPtr->TranslateAddr(strAddr, usPort, *pAddr, false);
			strTemp.AppendFormat(TF("([%s]%d)"), *strAddr, usPort);
		}
		else {
			CStringFix strTemp1;
			strTemp1.Load(IDS_SHARE_GAMEDB);
			strTemp += strTemp1;
		}
		m_ConnectGameDB.SetWindowText(*strTemp);
	}
	else if (nServerIndex == DATA_INDEX_GAME) {
		strTemp.Load(IDC_UDP_SERVICE);

		CNETTraits::PNET_ADDR pAddr = (reinterpret_cast<CNETTraits::PNET_ADDR>(utParam));

		UShort     usPort = 0;
		CStringKey strAddr;
		GServerLoaderInst->m_NetworkPtr->TranslateAddr(strAddr, usPort, *pAddr, false);
		strTemp.AppendFormat(TF("[%s]%d"), *strAddr, usPort);
		m_UDPService.SetWindowText(*strTemp);
	}
	else {
		assert((nServerIndex == DATA_INDEX_ZONE) || (nServerIndex == DATA_INDEX_GATE));
		AddListViewData(nServerIndex, utParam);
	}
}

void CGameDlg::OnUpdate(Int nServerIndex, uintptr_t utParam)
{
	UpdateListViewData(nServerIndex, utParam);
}

void CGameDlg::OnSync(Int nServerIndex, uintptr_t utParam)
{
	if (nServerIndex == DATA_INDEX_ZONE) {
		PSERVER_DATA pData = reinterpret_cast<PSERVER_DATA>(utParam);
		m_uZoneCount  = pData->usBusy;
		m_uZoneOnline = pData->uOnline;
		m_uZoneAll    = pData->uAllCount;
	}
	else if (nServerIndex == DATA_INDEX_GATE) {
		PSERVER_DATA pData = reinterpret_cast<PSERVER_DATA>(utParam);
		m_uGateCount  = pData->usBusy;
		m_uGateOnline = pData->uOnline;
		m_uGateAll    = pData->uAllCount;
	}
	else {
		assert(nServerIndex == DATA_INDEX_GAME);
		PSERVER_DATA pData = reinterpret_cast<PSERVER_DATA>(utParam);
		m_uGameOnline = pData->uOnline;
		m_uGameAll    = pData->uAllCount;
		m_uGameBusy   = pData->usBusy / DATAD_PERCENT;
	}
}

void CGameDlg::OnSync(Int, CStream&)
{
}

void CGameDlg::OnUnlink(Int nServerIndex, uintptr_t utParam)
{
	if (nServerIndex == DATA_INDEX_CENTER) {
		CStringFix strTemp;
		strTemp.Load(IDC_CONNECT_CENTER);
		m_ConnectCenter.SetWindowText(*strTemp);
	}
	else if (nServerIndex == DATA_INDEX_GAMEDB) {
		CStringFix strTemp;
		strTemp.Load(IDC_CONNECT_GAMEDB);
		m_ConnectGameDB.SetWindowText(*strTemp);
	}
	else {
		assert((nServerIndex == DATA_INDEX_ZONE) || (nServerIndex == DATA_INDEX_GATE));
		RemoveListViewData(nServerIndex, utParam);
	}
}

void CGameDlg::UpdateServerInfo(void)
{
	CStringFix strInfo;
	strInfo.Load(IDC_GAME_INFO);

	CStringFix strTemp;
	strTemp.Format(*strInfo, m_uGameOnline, m_uGameAll, m_uGameBusy,
				   m_uZoneCount, m_uZoneOnline, m_uZoneAll,
				   m_uGateCount, m_uGateOnline, m_uGateAll);
	m_ServerInfo.SetWindowText(*strTemp);
}

CXListView* CGameDlg::GetListView(Int nServerIndex)
{
	if (nServerIndex == DATA_INDEX_ZONE) {
		return &m_ZoneSvr;
	}
	else {
		assert(nServerIndex == DATA_INDEX_GATE);
		return &m_GateSvr;
	}
}






