//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Source File : SelectDlg.cpp                                //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : 选择服务器管理界面对话框                            //
//                                                              //
//////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "network.h"
#include "servermap.h"
#include "ServerLoader.h"
#include "SelectDlg.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CSelectDlg
BEGIN_DLG_ID_WND_EXCHANGE( CSelectDlg )
DLG_ID_WND( IDC_CONNECT_CENTER, m_ConnectCenter )
DLG_ID_WND( IDC_UDP_SERVICE,    m_UDPService )
DLG_ID_WND( IDC_SELECT_INFO,    m_ServerInfo )
DLG_ID_WND( IDD_LOGIN,          m_LoginSvr )
END_DLG_ID_WND_EXCHANGE( CXDlg )

CSelectDlg::CSelectDlg(void)
: m_uSelectAll(0)
, m_uLoginCount(0)
, m_uLoginOnline(0)
, m_uLoginAll(0)
{
}

CSelectDlg::~CSelectDlg(void)
{
}

LRESULT CSelectDlg::OnInitDialog(void)
{
	RECT rc;
	GetClientRect(&rc);
	rc.top += (DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_HEIGHT);
	rc.top += (DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_GRAP + DLG_ITEM_HEIGHT);

	m_LoginSvr.MoveWindow(0, rc.top, rc.right, (rc.bottom - rc.top));
	//
	CStringFix strTemp;
	strTemp.Load(IDC_CONNECT_CENTER);
	m_ConnectCenter.SetWindowText(*strTemp);

	strTemp.Load(IDC_UDP_SERVICE);
	m_UDPService.SetWindowText(*strTemp);
	//
	InitListViewItem(DATA_INDEX_LOGIN);
	return TRUE;
}

LRESULT CSelectDlg::OnSize(WPARAM, LPARAM lParam, BOOL&)
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

void CSelectDlg::OnLive(bool bStart)
{
	CStringFix strTemp;
	if (bStart) {
		strTemp.Load(IDC_UDP_SERVICE);

		UShort     usPort = 0;
		CStringKey strAddr;
		GServerLoaderInst->m_Config.GetServerAddr(CServerConfig::CFG_DEFAULT_SELECT, 0, strAddr, usPort);
		strTemp.AppendFormat(TF("[%s]%d"), *strAddr, usPort);
		m_UDPService.SetWindowText(*strTemp);
	}
	else {
		strTemp.Load(IDC_CONNECT_CENTER);
		m_ConnectCenter.SetWindowText(*strTemp);

		strTemp.Load(IDC_UDP_SERVICE);
		m_UDPService.SetWindowText(*strTemp);
	}
	m_ServerInfo.SetWindowText(TF(""));
}

void CSelectDlg::OnLink(Int nServerIndex, uintptr_t utParam)
{
	UNREFERENCED_PARAMETER(nServerIndex);
	assert(nServerIndex == DATA_INDEX_CENTER);

	CStringFix strTemp;
	strTemp.Load(IDC_CONNECT_CENTER);
	if (utParam != 0) {
		CNETTraits::PNET_ADDR pAddr = (reinterpret_cast<CNETTraits::PNET_ADDR>(utParam));

		UShort     usPort = 0;
		CStringKey strAddr;

		GServerLoaderInst->m_Config.GetServerAddr(CServerConfig::CFG_DEFAULT_CENTER, CServerConfig::CFG_DEFAULT_SELECT, strAddr, usPort);
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

void CSelectDlg::OnUpdate(Int, uintptr_t)
{
}

void CSelectDlg::OnSync(Int nServerIndex, uintptr_t utParam)
{
	if (nServerIndex == DATA_INDEX_LOGIN) {
		PSERVER_DATA pData = reinterpret_cast<PSERVER_DATA>(utParam);
		m_uLoginCount  = pData->usBusy;
		m_uLoginOnline = pData->uOnline;
		m_uLoginAll    = pData->uAllCount;
	}
	else {
		PSERVER_DATA pData = reinterpret_cast<PSERVER_DATA>(utParam);
		m_uSelectAll = pData->uAllCount;
	}
}

void CSelectDlg::OnSync(Int nServerIndex, CStream& Stream)
{
	assert(nServerIndex == DATA_INDEX_LOGIN);

	SVR_LOGIN_MAP::SVR_PAIR Pair;
	while (Stream.IsEnd() == false) {
		Pair.Serialize(Stream);

		LVFINDINFO find ={0};
		find.flags  = LVFI_PARAM;
		find.lParam = (LPARAM)(Pair.drKey);
		Int nIndex  = m_LoginSvr.FindItem(&find);

		if (Pair.Value.usStatus == STATUSU_UNLINK) {
			if (nIndex > -1) {
				m_LoginSvr.DeleteItem(nIndex);
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

void CSelectDlg::OnUnlink(Int nServerIndex, uintptr_t utParam)
{
	UNREFERENCED_PARAMETER(nServerIndex);
	UNREFERENCED_PARAMETER(utParam);
	assert(nServerIndex == DATA_INDEX_CENTER);

	m_LoginSvr.DeleteAllItems();
	m_uLoginCount  = 0;
	m_uLoginOnline = 0;
	m_uLoginAll    = 0;

	CStringFix strTemp;
	strTemp.Load(IDC_CONNECT_CENTER);
	m_ConnectCenter.SetWindowText(*strTemp);

	UpdateServerInfo();
}

void CSelectDlg::UpdateServerInfo(void)
{
	CStringFix strInfo;
	strInfo.Load(IDC_SELECT_INFO);

	CStringFix strTemp;
	strTemp.Format(*strInfo, m_uSelectAll, m_uLoginCount, m_uLoginOnline, m_uLoginAll);
	m_ServerInfo.SetWindowText(*strTemp);
}

CXListView* CSelectDlg::GetListView(Int nServerIndex)
{
	UNREFERENCED_PARAMETER(nServerIndex);
	assert(nServerIndex == DATA_INDEX_LOGIN);
	return &m_LoginSvr;
}




