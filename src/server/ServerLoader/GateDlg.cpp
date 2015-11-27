//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Source File : GateDlg.cpp                                  //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : 网关服务器界面对话框                               //
//                                                              //
//////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "network.h"
#include "ServerLoader.h"
#include "GateDlg.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CGateDlg
BEGIN_DLG_ID_WND_EXCHANGE( CGateDlg )
DLG_ID_WND( IDC_CONNECT_GAMEDB,  m_ConnectGameDB )
DLG_ID_WND( IDC_CONNECT_GAME,    m_ConnectGame )
DLG_ID_WND( IDC_TCP_SERVICE,     m_TCPService )
DLG_ID_WND( IDC_UDP_SERVICE,     m_UDPService )
DLG_ID_WND( IDC_GATE_INFO,       m_ServerInfo )
DLG_ID_WND( IDD_GATE,            m_GateBlock )
END_DLG_ID_WND_EXCHANGE( CXDlg )

CGateDlg::CGateDlg(void)
: m_uGateOnline(0)
, m_uGateAll(0)
, m_uGateBusy(0)
, m_uGameId(0)
, m_uQueueUsers(0)
, m_uSelectUsers(0)
, m_uPlayUsers(0)
{
}

CGateDlg::~CGateDlg(void)
{
}

LRESULT CGateDlg::OnInitDialog(void)
{
	RECT rc;
	GetClientRect(&rc);
	rc.top += (DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_HEIGHT);
	rc.top += (DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_HEIGHT);
	rc.top += (DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_HEIGHT);

	m_GateBlock.MoveWindow(0, rc.top, rc.right, (rc.bottom - rc.top));
	//
	CStringFix strTemp;
	strTemp.Load(IDC_CONNECT_GAMEDB);
	m_ConnectGameDB.SetWindowText(*strTemp);

	strTemp.Load(IDC_CONNECT_GAME);
	m_ConnectGame.SetWindowText(*strTemp);

	strTemp.Load(IDC_TCP_SERVICE);
	m_TCPService.SetWindowText(*strTemp);

	strTemp.Load(IDC_UDP_SERVICE);
	m_UDPService.SetWindowText(*strTemp);
	//
	//InitListViewItem(DATA_INDEX_GATE);
	return TRUE;
}

LRESULT CGateDlg::OnSize(WPARAM, LPARAM lParam, BOOL&)
{
	RECT rc   = { 0 };
	rc.right  = (Long)LOWORD(lParam);
	rc.bottom = (Long)HIWORD(lParam);

	rc.top += (DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_HEIGHT);
	rc.top += (DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_HEIGHT);
	m_ServerInfo.MoveWindow(0, rc.top, rc.right, DLG_ITEM_HEIGHT);

	rc.top += (DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_HEIGHT);
	m_GateBlock.MoveWindow(0, rc.top, rc.right, (rc.bottom - rc.top));
	return 0;
}

void CGateDlg::OnLive(bool bStart)
{
	CStringFix strTemp;
	if (bStart){
		UShort     usPort = 0;
		CStringKey strAddr;

		strTemp.Load(IDC_TCP_SERVICE);
		GServerLoaderInst->m_Config.GetServerAddr(CServerConfig::CFG_DEFAULT_GATE, 0, strAddr, usPort);
		strTemp.AppendFormat(TF("[%s]%d"), *strAddr, usPort);
		m_TCPService.SetWindowText(*strTemp);
	}
	else{
		strTemp.Load(IDC_CONNECT_GAMEDB);
		m_ConnectGameDB.SetWindowText(*strTemp);

		strTemp.Load(IDC_CONNECT_GAME);
		m_ConnectGame.SetWindowText(*strTemp);

		strTemp.Load(IDC_TCP_SERVICE);
		m_TCPService.SetWindowText(*strTemp);

		strTemp.Load(IDC_UDP_SERVICE);
		m_UDPService.SetWindowText(*strTemp);
	}
	m_ServerInfo.SetWindowText(TF(""));
}

void CGateDlg::OnLink(Int nServerIndex, uintptr_t utParam)
{
	CStringFix strTemp;
	if (nServerIndex == DATA_INDEX_GAMEDB) {

		strTemp.Load(IDC_CONNECT_GAMEDB);

		if (utParam != 0) {
			CNETTraits::PNET_ADDR pAddr = (reinterpret_cast<CNETTraits::PNET_ADDR>(utParam));

			UShort     usPort = 0;
			CStringKey strAddr;

			GServerLoaderInst->m_Config.GetServerAddr(CServerConfig::CFG_DEFAULT_GAMEDB, CServerConfig::CFG_DEFAULT_GATE, strAddr, usPort);
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

		strTemp.Load(IDC_CONNECT_GAME);

		if (utParam != 0) {
			CNETTraits::PNET_ADDR pAddr = (reinterpret_cast<CNETTraits::PNET_ADDR>(utParam));

			UShort     usPort = 0;
			CStringKey strAddr;

			GServerLoaderInst->m_Config.GetServerAddr(CServerConfig::CFG_DEFAULT_GAME, CServerConfig::CFG_DEFAULT_GATE, strAddr, usPort);
			strTemp.AppendFormat(TF("[%s]%d"), *strAddr, usPort);

			GServerLoaderInst->m_NetworkPtr->TranslateAddr(strAddr, usPort, *pAddr, false);
			strTemp.AppendFormat(TF("([%s]%d)"), *strAddr, usPort);
		}
		else {
			CStringFix strTemp1;
			strTemp1.Load(IDS_SHARE_GAME);
			strTemp += strTemp1;
		}
		m_ConnectGame.SetWindowText(*strTemp);
	}
	else if (nServerIndex == DATA_INDEX_GATE) {
		strTemp.Load(IDC_UDP_SERVICE);

		CNETTraits::PNET_ADDR pAddr = (reinterpret_cast<CNETTraits::PNET_ADDR>(utParam));

		UShort     usPort = 0;
		CStringKey strAddr;
		GServerLoaderInst->m_NetworkPtr->TranslateAddr(strAddr, usPort, *pAddr, false);
		strTemp.AppendFormat(TF("[%s]%d"), *strAddr, usPort);
		m_UDPService.SetWindowText(*strTemp);
	}
}

void CGateDlg::OnUpdate(Int, uintptr_t)
{
}

void CGateDlg::OnSync(Int nServerIndex, uintptr_t utParam)
{
	if (nServerIndex == DATA_INDEX_GATE) {
		PSERVER_DATA pData = reinterpret_cast<PSERVER_DATA>(utParam);
		m_uGateOnline = pData->uOnline;
		m_uGateAll    = pData->uAllCount;
		m_uGateBusy   = pData->usBusy / DATAD_PERCENT;
	}
	else {
		PUInt p = reinterpret_cast<PUInt>(utParam);
		m_uGameId      = p[0];
		m_uQueueUsers  = p[1];
		m_uSelectUsers = p[2];
		m_uPlayUsers   = p[3];
	}
}

void CGateDlg::OnSync(Int, CStream&)
{
}

void CGateDlg::OnUnlink(Int nServerIndex, uintptr_t utParam)
{
	UNREFERENCED_PARAMETER(utParam);
	if (nServerIndex == DATA_INDEX_GAMEDB) {
		CStringFix strTemp;
		strTemp.Load(IDC_CONNECT_GAMEDB);
		m_ConnectGameDB.SetWindowText(*strTemp);
	}
	else if (nServerIndex == DATA_INDEX_GAME) {
		CStringFix strTemp;
		strTemp.Load(IDC_CONNECT_GAME);
		m_ConnectGame.SetWindowText(*strTemp);
	}
}

void CGateDlg::UpdateServerInfo(void)
{
	CStringFix strInfo;
	strInfo.Load(IDC_GATE_INFO);

	CStringFix strTemp;
	strTemp.Format(*strInfo, m_uGateOnline, m_uGateAll, m_uGateBusy, m_uGameId, m_uQueueUsers, m_uSelectUsers, m_uPlayUsers);
	m_ServerInfo.SetWindowText(*strTemp);
}

CXListView* CGateDlg::GetListView(Int nServerIndex)
{
	UNREFERENCED_PARAMETER(nServerIndex);
	assert(nServerIndex == DATA_INDEX_GATE);
	return &m_GateBlock;
}




