//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Source File : LoginDlg.cpp                                 //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : 登陆服务器管理界面对话框                            //
//                                                              //
//////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "network.h"
#include "servermap.h"
#include "ServerLoader.h"
#include "LoginDlg.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CLoginDlg
BEGIN_DLG_ID_WND_EXCHANGE( CLoginDlg )
DLG_ID_WND( IDC_CONNECT_CENTER,  m_ConnectCenter )
DLG_ID_WND( IDC_CONNECT_LOGINDB, m_ConnectLoginDB )
DLG_ID_WND( IDC_TCP_SERVICE,     m_TCPService)
DLG_ID_WND( IDC_UDP_SERVICE,     m_UDPService)
DLG_ID_WND( IDC_LOGIN_INFO,      m_ServerInfo)
DLG_ID_WND( IDD_GAME,            m_GameSvr)
END_DLG_ID_WND_EXCHANGE( CXDlg )

CLoginDlg::CLoginDlg(void)
: m_uLoginOnline(0)
, m_uLoginAll(0)
, m_uLoginBusy(0)
, m_uGameCount(0)
, m_uGameOnline(0)
, m_uGameAll(0)
{
}

CLoginDlg::~CLoginDlg(void)
{
}

LRESULT CLoginDlg::OnInitDialog(void)
{
	RECT rc;
	GetClientRect(&rc);
	rc.top += (DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_HEIGHT);
	rc.top += (DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_HEIGHT);
	rc.top += (DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_HEIGHT);

	m_GameSvr.MoveWindow(0, rc.top, rc.right, (rc.bottom - rc.top));
	//
	CStringFix strTemp;
	strTemp.Load(IDC_CONNECT_CENTER);
	m_ConnectCenter.SetWindowText(*strTemp);

	strTemp.Load(IDC_CONNECT_LOGINDB);
	m_ConnectLoginDB.SetWindowText(*strTemp);

	strTemp.Load(IDC_TCP_SERVICE);
	m_TCPService.SetWindowText(*strTemp);

	strTemp.Load(IDC_UDP_SERVICE);
	m_UDPService.SetWindowText(*strTemp);
	//
	InitListViewItem(DATA_INDEX_GAME);
	return TRUE;
}

LRESULT CLoginDlg::OnSize(WPARAM, LPARAM lParam, BOOL&)
{
	RECT rc   = { 0 };
	rc.right  = (Long)LOWORD(lParam);
	rc.bottom = (Long)HIWORD(lParam);

	rc.top += (DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_HEIGHT);
	rc.top += (DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_HEIGHT);
	m_ServerInfo.MoveWindow(0, rc.top, rc.right, DLG_ITEM_HEIGHT);

	rc.top += (DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_HEIGHT);
	m_GameSvr.MoveWindow(0, rc.top, rc.right, (rc.bottom - rc.top));
	return 0;
}

void CLoginDlg::OnLive(bool bStart)
{
	CStringFix strTemp;
	if (bStart){
		Int        nPort = 0;
		CStringKey strAddr;

		strTemp.Load(IDC_TCP_SERVICE);
		GServerLoaderInst->m_Config.GetServerAddr(CServerConfig::CFG_DEFAULT_LOGIN, 0, strAddr, nPort);
		strTemp.AppendFormat(TF("[%s]%d"), *strAddr, nPort);
		m_TCPService.SetWindowText(*strTemp);

		strTemp.Load(IDC_UDP_SERVICE);
		GServerLoaderInst->m_Config.GetServerAddr(CServerConfig::CFG_DEFAULT_LOGIN, CServerConfig::CFG_DEFAULT_GAME, strAddr, nPort);
		strTemp.AppendFormat(TF("[%s]%d"), *strAddr, nPort);
		m_UDPService.SetWindowText(*strTemp);
	}
	else{
		strTemp.Load(IDC_CONNECT_CENTER);
		m_ConnectCenter.SetWindowText(*strTemp);

		strTemp.Load(IDC_CONNECT_LOGINDB);
		m_ConnectLoginDB.SetWindowText(*strTemp);

		strTemp.Load(IDC_TCP_SERVICE);
		m_TCPService.SetWindowText(*strTemp);

		strTemp.Load(IDC_UDP_SERVICE);
		m_UDPService.SetWindowText(*strTemp);
	}
	m_ServerInfo.SetWindowText(TF(""));
}

void CLoginDlg::OnLink(Int nServerIndex, uintptr_t utParam)
{
	CStringFix strTemp;
	if (nServerIndex == DATA_INDEX_CENTER) {

		strTemp.Load(IDC_CONNECT_CENTER);

		if (utParam != 0) {
			CNETTraits::PNET_ADDR pAddr = (reinterpret_cast<CNETTraits::PNET_ADDR>(utParam));

			Int        nPort = 0;
			CStringKey strAddr;

			GServerLoaderInst->m_Config.GetServerAddr(CServerConfig::CFG_DEFAULT_CENTER, CServerConfig::CFG_DEFAULT_LOGIN, strAddr, nPort);
			strTemp.AppendFormat(TF("[%s]%d"), *strAddr, nPort);

			UShort usPort = 0;
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
	else if (nServerIndex == DATA_INDEX_LOGINDB) {

		strTemp.Load(IDC_CONNECT_LOGINDB);

		if (utParam != 0) {
			CNETTraits::PNET_ADDR pAddr = (reinterpret_cast<CNETTraits::PNET_ADDR>(utParam));

			Int        nPort = 0;
			CStringKey strAddr;

			GServerLoaderInst->m_Config.GetServerAddr(CServerConfig::CFG_DEFAULT_LOGINDB, 0, strAddr, nPort);
			strTemp.AppendFormat(TF("[%s]%d"), *strAddr, nPort);

			UShort usPort = 0;
			GServerLoaderInst->m_NetworkPtr->TranslateAddr(strAddr, usPort, *pAddr, false);
			strTemp.AppendFormat(TF("([%s]%d)"), *strAddr, usPort);
		}
		else {
			CStringFix strTemp1;
			strTemp1.Load(IDS_SHARE_LOGINDB);
			strTemp += strTemp1;
		}
		m_ConnectLoginDB.SetWindowText(*strTemp);
	}
}

void CLoginDlg::OnUpdate(Int, uintptr_t)
{
}

void CLoginDlg::OnSync(Int nServerIndex, uintptr_t utParam)
{
	if (nServerIndex == DATA_INDEX_GAME) {
		PSERVER_DATA pData = reinterpret_cast<PSERVER_DATA>(utParam);
		m_uGameCount  = pData->usBusy;
		m_uGameOnline = pData->uOnline;
		m_uGameAll    = pData->uAllCount;
	}
	else {
		PSERVER_DATA pData = reinterpret_cast<PSERVER_DATA>(utParam);
		m_uLoginOnline = pData->uOnline;
		m_uLoginAll    = pData->uAllCount;
		m_uLoginBusy   = pData->usBusy;
	}
}

void CLoginDlg::OnSync(Int nServerIndex, CStream& Stream)
{
	assert(nServerIndex == DATA_INDEX_GAME);

	SVR_LOGIN_MAP::SVR_PAIR Pair;
	while (Stream.IsEnd() == false) {
		Pair.Serialize(Stream);

		LVFINDINFO find ={0};
		find.flags  = LVFI_PARAM;
		find.lParam = (LPARAM)(Pair.drKey);
		Int nIndex  = m_GameSvr.FindItem(&find);

		if (Pair.Value.usStatus == STATUSU_UNLINK) {
			if (nIndex > -1) {
				m_GameSvr.DeleteItem(nIndex);
			}
		}
		else if ((Pair.Value.usStatus == STATUSU_SYNC) || (Pair.Value.usStatus == STATUSU_LINK)) {
			if (nIndex > -1) {
				UpdateListViewData(nServerIndex, reinterpret_cast<uintptr_t>(&Pair));
			}
			else {
				AddListViewData(nServerIndex, reinterpret_cast<uintptr_t>(&Pair));
			}
		}
	}
}

void CLoginDlg::OnUnlink(Int nServerIndex, uintptr_t utParam)
{
	UNREFERENCED_PARAMETER(utParam);
	if (nServerIndex == DATA_INDEX_CENTER) {
		CStringFix strTemp;
		strTemp.Load(IDC_CONNECT_CENTER);
		m_ConnectCenter.SetWindowText(*strTemp);
	}
	else if (nServerIndex == DATA_INDEX_LOGINDB) {
		CStringFix strTemp;
		strTemp.Load(IDC_CONNECT_LOGINDB);
		m_ConnectLoginDB.SetWindowText(*strTemp);
	}
}

void CLoginDlg::UpdateServerInfo(void)
{
	CStringFix strInfo;
	strInfo.Load(IDC_LOGIN_INFO);

	CStringFix strTemp;
	strTemp.Format(*strInfo, m_uLoginOnline, m_uLoginAll, m_uLoginBusy, m_uGameCount, m_uGameOnline, m_uGameAll);
	m_ServerInfo.SetWindowText(*strTemp);
}

CXListView* CLoginDlg::GetListView(Int nServerIndex)
{
	UNREFERENCED_PARAMETER(nServerIndex);
	assert(nServerIndex == DATA_INDEX_GAME);
	return &m_GameSvr;
}




