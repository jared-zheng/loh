//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Source File : ZoneDlg.cpp                                  //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : 地图服务器界面对话框                               //
//                                                              //
//////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "network.h"
#include "ServerLoader.h"
#include "ZoneDlg.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CZoneDlg
BEGIN_DLG_ID_WND_EXCHANGE( CZoneDlg )
DLG_ID_WND( IDC_CONNECT_GAME,    m_ConnectGame )
DLG_ID_WND( IDC_ZONE_CONFIG,     m_ZoneConfig )
DLG_ID_WND( IDC_ZONE_INFO,       m_ServerInfo )
DLG_ID_WND( IDD_ZONE,            m_Scenes )
END_DLG_ID_WND_EXCHANGE( CXDlg )

CZoneDlg::CZoneDlg(void)
: m_uZoneOnline(0)
, m_uZoneAll(0)
, m_uZoneIds(0)
, m_uZoneThreads(0)
{
}

CZoneDlg::~CZoneDlg(void)
{
}

LRESULT CZoneDlg::OnInitDialog(void)
{
	RECT rc;
	GetClientRect(&rc);
	rc.top += (DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_HEIGHT);
	rc.top += (DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_HEIGHT);

	m_Scenes.MoveWindow(0, rc.top, rc.right, (rc.bottom - rc.top));
	//
	CStringFix strTemp;
	strTemp.Load(IDC_CONNECT_GAME);
	m_ConnectGame.SetWindowText(*strTemp);

	strTemp.Load(IDC_ZONE_CONFIG);
	m_ZoneConfig.SetWindowText(*strTemp);
	//
	InitListViewItem(DATA_INDEX_ZONE);
	return TRUE;
}

LRESULT CZoneDlg::OnSize(WPARAM, LPARAM lParam, BOOL&)
{
	RECT rc   = { 0 };
	rc.right  = (Long)LOWORD(lParam);
	rc.bottom = (Long)HIWORD(lParam);

	rc.top += (DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_HEIGHT);
	m_ServerInfo.MoveWindow(0, rc.top, rc.right, DLG_ITEM_HEIGHT);

	rc.top += (DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_HEIGHT);
	m_Scenes.MoveWindow(0, rc.top, rc.right, (rc.bottom - rc.top));
	return 0;
}

void CZoneDlg::OnLive(bool bStart)
{
	CStringFix strTemp;
	if (bStart){
		CStringFix strConfig;
		GServerLoaderInst->m_Config.GetServerExtConfig(CServerConfig::CFG_DEFAULT_ZONE, strConfig);

		strTemp.Load(IDC_ZONE_CONFIG);
		strTemp += strConfig;
		m_ZoneConfig.SetWindowText(*strTemp);
	}
	else{
		strTemp.Load(IDC_CONNECT_GAME);
		m_ConnectGame.SetWindowText(*strTemp);

		strTemp.Load(IDC_ZONE_CONFIG);
		m_ZoneConfig.SetWindowText(*strTemp);
	}
	m_ServerInfo.SetWindowText(TF(""));
}

void CZoneDlg::OnLink(Int nServerIndex, uintptr_t utParam)
{
	CStringFix strTemp;
	if (nServerIndex == DATA_INDEX_GAME) {

		strTemp.Load(IDC_CONNECT_GAME);

		if (utParam != 0) {
			CNETTraits::PNET_ADDR pAddr = (reinterpret_cast<CNETTraits::PNET_ADDR>(utParam));

			UShort     usPort = 0;
			CStringKey strAddr;

			GServerLoaderInst->m_Config.GetServerAddr(CServerConfig::CFG_DEFAULT_GAME, CServerConfig::CFG_DEFAULT_ZONE, strAddr, usPort);
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
}

void CZoneDlg::OnUpdate(Int, uintptr_t)
{
}

void CZoneDlg::OnSync(Int nServerIndex, uintptr_t utParam)
{
	if (nServerIndex == DATA_INDEX_ZONE) {
		PSERVER_DATA pData = reinterpret_cast<PSERVER_DATA>(utParam);
		m_uZoneOnline  = pData->uOnline;
		m_uZoneAll     = pData->uAllCount;
		m_uZoneIds     = pData->usBusy;
		m_uZoneThreads = pData->usIncr;
	}
	else {
	}
}

void CZoneDlg::OnSync(Int, CStream&)
{
}

void CZoneDlg::OnUnlink(Int nServerIndex, uintptr_t utParam)
{
	UNREFERENCED_PARAMETER(utParam);
	assert(nServerIndex == DATA_INDEX_GAME);

	CStringFix strTemp;
	strTemp.Load(IDC_CONNECT_GAME);
	m_ConnectGame.SetWindowText(*strTemp);
}

void CZoneDlg::UpdateServerInfo(void)
{
	CStringFix strInfo;
	strInfo.Load(IDC_ZONE_INFO);

	CStringFix strTemp;
	strTemp.Format(*strInfo, m_uZoneOnline, m_uZoneAll, m_uZoneIds, m_uZoneThreads);
	m_ServerInfo.SetWindowText(*strTemp);
}

CXListView* CZoneDlg::GetListView(Int nServerIndex)
{
	UNREFERENCED_PARAMETER(nServerIndex);
	assert(nServerIndex == DATA_INDEX_ZONE);
	return &m_Scenes;
}




