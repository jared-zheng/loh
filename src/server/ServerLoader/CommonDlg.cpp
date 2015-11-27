//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Source File : CommonDlg.cpp                                //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : 服务器界面TAB子对话框基类                          //
//                                                              //
//////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "network.h"
#include "servermap.h"
#include "ServerLoader.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CCommonDlg
bool CCommonDlg::OnHandle(Int nEvent, uintptr_t utParam, LLong llParam)
{
	switch (nEvent) {
	case PAK_EVENT_LIVE:
		{
			OnLive(utParam != false);
		}
		break;
	case PAK_EVENT_LINK:
		{
			OnLink((Int)llParam, utParam);
		}
		break;
	case PAK_EVENT_UPDATE:
		{
			OnUpdate((Int)llParam, utParam);
		}
		break;
	case PAK_EVENT_SYNC:
		{
			OnSync((Int)llParam, utParam);
		}
		break;
	case PAK_EVENT_UNLINK:
		{
			OnUnlink((Int)llParam, utParam);
		}
		break;
	case DATA_INDEX_CENTER:
	case DATA_INDEX_SELECT:
	case DATA_INDEX_LOGINDB:
	case DATA_INDEX_LOGIN:
	case DATA_INDEX_GAMEDB:
	case DATA_INDEX_GAME:
	case DATA_INDEX_ZONE:
	case DATA_INDEX_GATE:
		{
			SetServer(reinterpret_cast<ICommonServer*>(utParam));
		}
		break;
	case CServerConfig::CFG_CONFIG_PTR:
		{
			*(reinterpret_cast<CServerConfig**>(utParam)) = &(GServerLoaderInst->m_Config);
		}
		break;
	default:
		{
			assert(0);
		}
	}
	return true;
}

bool CCommonDlg::OnHandle(Int, CEventBase&, LLong)
{
	return true;
}

bool CCommonDlg::OnHandle(Int nEvent, CStream& Stream, LLong llParam)
{
	if (nEvent == PAK_EVENT_SYNC) {
		OnSync((Int)llParam, Stream);
	}
	return true;
}

bool CCommonDlg::OnHandle(Int, UInt)
{
	assert(m_pServer != nullptr);
	m_pServer->Update();

	UpdateServerInfo();
	return true;
}

void CCommonDlg::InitListViewItem(Int nServerIndex)
{
	CXListView* pListView = GetListView(nServerIndex);
	if (pListView != nullptr) {
		pListView->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

		CStringFix strTemp;
		strTemp.Load(nServerIndex + IDS_ID_BASE);
		pListView->AddColumn(*strTemp, 0, 60, -1, LVCF_WIDTH | LVCF_TEXT);
		// common
		strTemp.Load(IDS_ALL_COUNT);
		pListView->AddColumn(*strTemp, 1, 80, -1, LVCF_WIDTH | LVCF_TEXT);

		strTemp.Load(IDS_ONLINE_COUNT);
		pListView->AddColumn(*strTemp, 2, 80, -1, LVCF_WIDTH | LVCF_TEXT);

		if (nServerIndex != DATA_INDEX_ZONE) {
			strTemp.Load(IDS_BUSY_PER);
		}
		else {
			strTemp.Load(IDS_ZONE_SCENE);
		}
		pListView->AddColumn(*strTemp, 3, 80, -1, LVCF_WIDTH | LVCF_TEXT);
		switch (nServerIndex) {
		case DATA_INDEX_SELECT:
			{
				strTemp.Load(IDS_UDP_SERVICE);
				pListView->AddColumn(*strTemp, 4, 120, -1, LVCF_WIDTH | LVCF_TEXT);
			}
			break;
		case DATA_INDEX_LOGIN:
			{
				strTemp.Load(IDS_TCP_SERVICE);
				pListView->AddColumn(*strTemp, 4, 120, -1, LVCF_WIDTH | LVCF_TEXT);

				strTemp.Load(IDS_UDP_TO_GAME);
				pListView->AddColumn(*strTemp, 5, 120, -1, LVCF_WIDTH | LVCF_TEXT);

				strTemp.Load(IDD_LOGINDB);
				pListView->AddColumn(*strTemp, 8, 120, -1, LVCF_WIDTH | LVCF_TEXT);
			}
			break;
		case DATA_INDEX_GAME:
			{
				strTemp.Load(IDS_LISTEN_PING);
				pListView->AddColumn(*strTemp, 4, 80, -1, LVCF_WIDTH | LVCF_TEXT);

				strTemp.Load(IDS_LISTEN_ZONE);
				pListView->AddColumn(*strTemp, 5, 120, -1, LVCF_WIDTH | LVCF_TEXT);

				strTemp.Load(IDS_LISTEN_GATE);
				pListView->AddColumn(*strTemp, 6, 120, -1, LVCF_WIDTH | LVCF_TEXT);

				strTemp.Load(IDS_UDP_TO_LOGIN);
				pListView->AddColumn(*strTemp, 7, 120, -1, LVCF_WIDTH | LVCF_TEXT);

				strTemp.Load(IDD_GAMEDB);
				pListView->AddColumn(*strTemp, 8, 120, -1, LVCF_WIDTH | LVCF_TEXT);
			}
			break;
		case DATA_INDEX_ZONE:
			{
				strTemp.Load(IDS_ZONE_THREAD);
				pListView->AddColumn(*strTemp, 4, 80, -1, LVCF_WIDTH | LVCF_TEXT);
			}
			break;
		case DATA_INDEX_GATE:
			{
				strTemp.Load(IDS_TCP_SERVICE);
				pListView->AddColumn(*strTemp, 4, 120, -1, LVCF_WIDTH | LVCF_TEXT);

				strTemp.Load(IDS_UDP_TO_LOGIN);
				pListView->AddColumn(*strTemp, 5, 120, -1, LVCF_WIDTH | LVCF_TEXT);

				strTemp.Load(IDD_GAMEDB);
				pListView->AddColumn(*strTemp, 6, 120, -1, LVCF_WIDTH | LVCF_TEXT);
			}
			break;
		default:
			{
				assert(0);
			}
		}
	}
}

bool CCommonDlg::AddListViewData(Int nServerIndex, uintptr_t utParam)
{
	CXListView* pListView = GetListView(nServerIndex);
	if (pListView != nullptr) {
		assert(utParam != 0);
		SVR_TEST_MAP::PSVR_PAIR pPair = reinterpret_cast<SVR_TEST_MAP::PSVR_PAIR>(utParam);
		assert(pPair->Value.usStatus & STATUSU_LINK);
#ifdef _DEBUG
		LVFINDINFO find ={0};
		find.flags  = LVFI_PARAM;
		find.lParam = (LPARAM)(pPair->drKey);
		assert(pListView->FindItem(&find) == -1);
#endif
		CStringFix strTemp;
		strTemp.ToString((ULong)pPair->Value.usId);
		Int nIndex = pListView->InsertItem(LMT_MAX, *strTemp);
		if (nIndex >= 0) {
			strTemp.ToString((ULong)pPair->Value.uAllCount);
			pListView->SetItemText(nIndex, 1, *strTemp);

			strTemp.ToString((ULong)pPair->Value.uOnline);
			pListView->SetItemText(nIndex, 2, *strTemp);

			if (nServerIndex != DATA_INDEX_ZONE) {
				strTemp.ToString((ULong)(pPair->Value.usBusy / DATAD_PERCENT));
				pListView->SetItemText(nIndex, 3, *strTemp);

				AddListViewAddr(nServerIndex, nIndex, pPair->Value.NetAddr);
			}
			else {
				strTemp.ToString((ULong)(pPair->Value.usBusy));
				pListView->SetItemText(nIndex, 3, *strTemp);

				strTemp.ToString((ULong)(pPair->Value.usIncr));
				pListView->SetItemText(nIndex, 4, *strTemp);
			}

			pListView->SetItemData(nIndex, (uintptr_t)pPair->drKey);
			return true;
		}
		DEV_WARN(TF("Dlg界面增加%d索引服务器信息失败!"), nServerIndex);
	}
	return false;
}

bool CCommonDlg::UpdateListViewData(Int nServerIndex, uintptr_t utParam)
{
	CXListView* pListView = GetListView(nServerIndex);
	if (pListView != nullptr) {
		assert(utParam != 0);
		SVR_TEST_MAP::PSVR_PAIR pPair = reinterpret_cast<SVR_TEST_MAP::PSVR_PAIR>(utParam);
		assert(pPair->Value.usStatus & (STATUSU_SYNC | STATUSU_LINK));
		LVFINDINFO find ={0};
		find.flags  = LVFI_PARAM;
		find.lParam = (LPARAM)(pPair->drKey);
		Int nIndex = pListView->FindItem(&find);
		if (nIndex > -1) {
			CStringFix strTemp;
			strTemp.ToString((ULong)pPair->Value.uAllCount);
			pListView->SetItemText(nIndex, 1, *strTemp);

			strTemp.ToString((ULong)pPair->Value.uOnline);
			pListView->SetItemText(nIndex, 2, *strTemp);

			if (nServerIndex != DATA_INDEX_ZONE) {
				strTemp.ToString((ULong)(pPair->Value.usBusy / DATAD_PERCENT));
				pListView->SetItemText(nIndex, 3, *strTemp);
			}
			else {
				strTemp.ToString((ULong)(pPair->Value.usBusy));
				pListView->SetItemText(nIndex, 3, *strTemp);

				strTemp.ToString((ULong)(pPair->Value.usIncr));
				pListView->SetItemText(nIndex, 4, *strTemp);
			}
			return true;
		}
		DEV_WARN(TF("Dlg界面更新%d索引服务器信息失败!"), nServerIndex);
	}
	return false;
}

bool CCommonDlg::RemoveListViewData(Int nServerIndex, uintptr_t utParam)
{
	CXListView* pListView = GetListView(nServerIndex);
	if (pListView != nullptr) {
		if (utParam != 0) {
			LVFINDINFO find ={0};
			find.flags  = LVFI_PARAM;
			find.lParam = (LPARAM)(utParam);
			Int nIndex = pListView->FindItem(&find);
			if (nIndex > -1) {
				pListView->DeleteItem(nIndex);
			}
			else {
				DEV_WARN(TF("Dlg界面删除%d索引服务器信息失败!"), nServerIndex);
			}
		}
		else {
			pListView->DeleteAllItems();
		}
	}
	return true;
}

void CCommonDlg::AddListViewAddr(Int nServerIndex, Int nIndex, void* pAddr)
{
	CXListView* pListView = GetListView(nServerIndex);
	assert(pListView != nullptr);

	CNETTraits::PNET_ADDR pNetAddr = reinterpret_cast<CNETTraits::PNET_ADDR>(pAddr);

	CStringFix strTemp;

	CStringKey strAddr;
	UShort     usPort = 0;

	switch (nServerIndex) {
	case DATA_INDEX_SELECT:
		{
			// udp
			GServerLoaderInst->m_NetworkPtr->TranslateAddr(strAddr, usPort, *pNetAddr, false);
			strTemp.Format(TF("[%s]:%d"), *strAddr, usPort);
			pListView->SetItemText(nIndex, 4, *strTemp);
		}
		break;
	case DATA_INDEX_LOGIN:
		{
			// tcp
			GServerLoaderInst->m_NetworkPtr->TranslateAddr(strAddr, usPort, *pNetAddr, false);
			strTemp.Format(TF("[%s]:%d"), *strAddr, usPort);
			pListView->SetItemText(nIndex, 4, *strTemp);
			// udp
			++pNetAddr;
			GServerLoaderInst->m_NetworkPtr->TranslateAddr(strAddr, usPort, *pNetAddr, false);
			strTemp.Format(TF("[%s]:%d"), *strAddr, usPort);
			pListView->SetItemText(nIndex, 5, *strTemp);
			// logindb
			++pNetAddr;
			GServerLoaderInst->m_NetworkPtr->TranslateAddr(strAddr, usPort, *pNetAddr, false);
			strTemp.Format(TF("[%s]:%d"), *strAddr, usPort);
			pListView->SetItemText(nIndex, 6, *strTemp);
		}
		break;
	case DATA_INDEX_GAME:
		{
			// ping
			GServerLoaderInst->m_NetworkPtr->TranslateAddr(strAddr, usPort, *pNetAddr, false);
			pListView->SetItemText(nIndex, 4, *strAddr);
			// zone
			++pNetAddr;
			GServerLoaderInst->m_NetworkPtr->TranslateAddr(strAddr, usPort, *pNetAddr, false);
			strTemp.Format(TF("[%s]:%d"), *strAddr, usPort);
			pListView->SetItemText(nIndex, 5, *strTemp);
			// gate
			++pNetAddr;
			if (pNetAddr->usPort != 0) {
				GServerLoaderInst->m_NetworkPtr->TranslateAddr(strAddr, usPort, *pNetAddr, false);
				strTemp.Format(TF("[%s]:%d"), *strAddr, usPort);
			}
			pListView->SetItemText(nIndex, 6, *strTemp);
			// udp
			++pNetAddr;
			GServerLoaderInst->m_NetworkPtr->TranslateAddr(strAddr, usPort, *pNetAddr, false);
			strTemp.Format(TF("[%s]:%d"), *strAddr, usPort);
			pListView->SetItemText(nIndex, 7, *strTemp);
			// gamedb
			++pNetAddr;
			GServerLoaderInst->m_NetworkPtr->TranslateAddr(strAddr, usPort, *pNetAddr, false);
			strTemp.Format(TF("[%s]:%d"), *strAddr, usPort);
			pListView->SetItemText(nIndex, 8, *strTemp);
		}
		break;
	case DATA_INDEX_GATE:
		{
			// tcp
			GServerLoaderInst->m_NetworkPtr->TranslateAddr(strAddr, usPort, *pNetAddr, false);
			strTemp.Format(TF("[%s]:%d"), *strAddr, usPort);
			pListView->SetItemText(nIndex, 4, *strTemp);
			// udp
			++pNetAddr;
			GServerLoaderInst->m_NetworkPtr->TranslateAddr(strAddr, usPort, *pNetAddr, false);
			strTemp.Format(TF("[%s]:%d"), *strAddr, usPort);
			pListView->SetItemText(nIndex, 5, *strTemp);
			// gamedb
			++pNetAddr;
			GServerLoaderInst->m_NetworkPtr->TranslateAddr(strAddr, usPort, *pNetAddr, false);
			strTemp.Format(TF("[%s]:%d"), *strAddr, usPort);
			pListView->SetItemText(nIndex, 6, *strTemp);
		}
		break;
	default:
		{
			assert(0);
		}
	}
}





