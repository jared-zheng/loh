//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Source File : MainFrame.cpp                                //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : 服务加载器主界面                                   //
//                                                              //
//////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "thread.h"
#include "CenterDlg.h"
#include "SelectDlg.h"
#include "LoginDBDlg.h"
#include "LoginDlg.h"
#include "GameDBDlg.h"
#include "GameDlg.h"
#include "GateDlg.h"
#include "MainFrame.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CMainFrame
CPCXStr CMainFrame::ANM_STATUS = TF("-\\|/");

CMainFrame::CMainFrame(void)
: m_nIndex(FRAMEC_STATUS)
, m_nTimes(0)
, m_nTabSel(-1)
{
	SetFrameMode();
	memset(m_pServerDlg, 0, sizeof(CCommonDlg*) * DATA_INDEX_MAX);
}

CMainFrame::~CMainFrame(void)
{
}

bool CMainFrame::OnHandle(Int, uintptr_t, LLong)
{
	return true;
}

bool CMainFrame::OnHandle(Int, CEventBase&, LLong)
{
	return true;
}

bool CMainFrame::OnHandle(Int, CStream&, LLong)
{
	return true;
}

bool CMainFrame::OnHandle(Int nEvent, UInt uCount)
{
	if (nEvent == FRAMEC_TIMER) {
		for (Int i = 0; i < DATA_INDEX_MAX; ++i) {
			if (m_pServerDlg[i] != nullptr) {
				m_pServerDlg[i]->OnHandle(nEvent, uCount);
			}
		}
	}
	return true;
}

LRESULT CMainFrame::OnCreate(LPCREATESTRUCT)
{
	CXUITraits::CREATE_PARAM cpm;
	// toolbar
	cpm.pParent  = this;
	cpm.itMenuID = IDC_TOOLBAR;
	cpm.ulStyle  = XUIS_TOOLBAR;
	if (GServerLoaderInst->m_UIManagerPtr->Create(m_CmdBar, cpm) == -1) {
		return -1;
	}
	if (m_CmdBar.LoadEx(48, IDC_TOOLBAR, 0, 0, RGB(255, 255, 255)) == false) {
		return -1;
	}
	m_CmdBar.EnableButton(IDC_PAUSE, FALSE);
	m_CmdBar.EnableButton(IDC_STOP, FALSE);
	//
	cpm.itMenuID = IDC_STATUSBAR;
	cpm.ulStyle  = XUIS_STATUSBAR;
	if (GServerLoaderInst->m_UIManagerPtr->Create(m_StatusBar, cpm) == -1) {
		return -1;
	}
	Int nWidth[] = { 320, 440, -1 };
	m_StatusBar.SetParts(3, nWidth);
	m_StatusBar.SetText(0, DefaultReady);
	m_StatusBar.SetText(1, TF("--"));

	CStringFix strFormat;
	strFormat.Load(IDC_STATUSBAR);

	CPlatform::TIMEINFO ti = { 0 };
	CPlatform::GetTimeInfo(ti);

	CStringFix strTemp;
	strTemp.Format(*strFormat, ti.usYear, ti.usMonth, ti.usDay, ti.usHour, ti.usMinute, ti.usSecond);
	m_StatusBar.SetText(2, *strTemp);
	// static controls
	cpm.itMenuID = IDC_SERVERTAB;
	cpm.ulStyle  = WS_CHILD|WS_BORDER|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|TCS_HOTTRACK|TCS_MULTILINE;
	if (GServerLoaderInst->m_UIManagerPtr->Create(m_Servers, cpm) == -1) {
		return -1;
	}
	///////////////////////////////
	RECT rt = { 0, 0, FRAMEC_WIDTH, FRAMEC_HEIGHT };
	///////////////////////////////
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);

	long x = GetSystemMetrics(SM_CXSCREEN);
	long y = GetSystemMetrics(SM_CYSCREEN);
	x = (x - (rt.right - rt.left)) / 2;
	y = (y - (rt.bottom - rt.top)) / 2;

	SetWindowPos(nullptr, x, y, rt.right - rt.left, rt.bottom - rt.top, SWP_NOZORDER|SWP_SHOWWINDOW);
	return 0;
}

void CMainFrame::OnDestroy(void)
{
	if (m_CmdBar.IsButtonEnabled(IDC_START) == FALSE) {
		CEventQueue::DestroyTickEvent(FRAMEC_TIMER);

		GServerLoaderInst->StopServers();
		GServerLoaderInst->UnloadServers();

		UnloadServerDlgs();
	}
	CXFrameWnd::OnDestroy();
}

void CMainFrame::FrameLayout(void)
{
	RECT rc;
	GetClientRect(&rc);

	::SendMessage(m_CmdBar.m_hWnd, XUIM_LAYOUT, MAKEWPARAM(0, XUI_LAYOUT_RECT), (LPARAM)&rc);
	::SendMessage(m_StatusBar.m_hWnd, XUIM_LAYOUT, MAKEWPARAM(0, XUI_LAYOUT_RECT), (LPARAM)&rc);

	m_Servers.MoveWindow(0, rc.top, rc.right, rc.bottom - rc.top);
	LayoutServerDlgs();
}

LRESULT CMainFrame::OnNotify(Int nID, LPNMHDR pHead, BOOL&)
{
	if ((nID == IDC_SERVERTAB) && (pHead->code == TCN_SELCHANGE)) {
		Int nIndex = m_Servers.GetCurSel();

		TCITEM Item;
		Item.mask = TCIF_PARAM;
		m_Servers.GetItem(nIndex, &Item);
		CCommonDlg* pDlg = reinterpret_cast<CCommonDlg*>(Item.lParam);
		pDlg->ShowWindow(SW_SHOW);

		if (m_nTabSel != -1) {
			m_Servers.GetItem(m_nTabSel, &Item);
			pDlg = reinterpret_cast<CCommonDlg*>(Item.lParam);
			pDlg->ShowWindow(SW_HIDE);
		}
		m_nTabSel = nIndex;
	}
	return 0;
}

LRESULT CMainFrame::OnTimer(WPARAM wParam, LPARAM, BOOL&)
{
	if (wParam == FRAMEC_TIMER) {
		++m_nIndex;
		if (m_nIndex >= FRAMEC_STATUS) {
			m_nIndex = 0;

			m_nTimes += (FRAMEC_STATUS * FRAMEC_TIMER_TIME) / DATAD_TICK_MS;
			Int nSecond = m_nTimes;
			Int nDays   = nSecond / (FRAMEC_TIME_DAY * FRAMEC_TIME_HOUR);
			nSecond    -= nDays * (FRAMEC_TIME_DAY * FRAMEC_TIME_HOUR);
			Int nHour   = nSecond / FRAMEC_TIME_HOUR;
			nSecond    -= nHour * FRAMEC_TIME_HOUR;
			Int nMinute = nSecond / FRAMEC_TIME_MINUTE;
			nSecond    -= nMinute * FRAMEC_TIME_MINUTE;
			CXChar::Format(m_szStatus, LMT_KEY, TF("%c %d-%02d:%02d:%02d"), ANM_STATUS[m_nIndex], nDays, nHour, nMinute, nSecond);
		}
		else {
			m_szStatus[0] = ANM_STATUS[m_nIndex];
		}
		m_StatusBar.SetText(1, m_szStatus);
	}
	return 0;
}

LRESULT CMainFrame::OnGetMinMaxInfo(WPARAM, LPARAM lParam, BOOL&)
{
	LPMINMAXINFO p = (LPMINMAXINFO)lParam;
	///////////////////////////////
	RECT rt = { 0, 0, FRAMEC_WIDTH, FRAMEC_HEIGHT };
	///////////////////////////////
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	if (p->ptMinTrackSize.x < rt.right) {
		p->ptMinTrackSize.x = rt.right;
	}
	if (p->ptMinTrackSize.y < rt.bottom) {
		p->ptMinTrackSize.y = rt.bottom;
	}
	return 0;
}

LRESULT CMainFrame::OnCommand(WORD, WORD wID, HWND, BOOL&)
{
	switch (wID) {
	case IDC_START:
		{
			OnStart();
		}
		break;
	case IDC_PAUSE:
		{
		}
		break;
	case IDC_STOP:
		{
			OnStop();
		}
		break;
	case IDC_UPDATE:
		{
			OnUpdate();
		}
		break;
	case IDC_EXIT:
		{
			OnStop(false);
			DestroyWindow();
		}
		break;
	default:
		{
		}
	}
	return 0;
}

LRESULT CMainFrame::OnXNotify(WPARAM wParam, LPARAM lParam, BOOL&)
{
	switch (wParam) {
	case XUI_NOTIFY_TTSHOW:
		{
			m_StatusBar.SetText(0, (LPCTSTR)lParam);
		}
		break;
	case XUI_NOTIFY_TTHIDE:
		{
			m_StatusBar.SetText(0, DefaultReady);
		}
		break;
	default:
		{
		}
	}
	return 0;
}

void CMainFrame::OnStart(void)
{
	if (LoadServerDlgs() == false) {
		DEV_ERROR(TF("创建服务器界面对话框失败!"));
		GServerLoaderInst->StopServers();
		GServerLoaderInst->UnloadServers();
		return;
	}
	if (GServerLoaderInst->StartServers()) {
		CEventQueue::CreateTickEvent(FRAMEC_TIMER, GServerLoaderInst->m_Config.GetUpdateTick() * DATAD_TICK_MS, *this);

		m_CmdBar.EnableButton(IDC_START, FALSE);
		m_CmdBar.EnableButton(IDC_UPDATE, FALSE);
		m_CmdBar.EnableButton(IDC_PAUSE, TRUE);
		m_CmdBar.EnableButton(IDC_STOP, TRUE);

		SetTimer(FRAMEC_TIMER, FRAMEC_TIMER_TIME);

		// 创建对话框， TAB加入
		CStringFix strTemp;
		CStringFix strTitle;
		strTitle.Load(IDS_CENTER_ID);
		strTitle += TF("[");
		if (GServerLoaderInst->m_Config.GetLoadServers() & CServerConfig::CFG_DEFAULT_CENTER) {
			strTemp.Load(IDD_CENTER);
			strTitle += strTemp;
			strTitle += TF("-");
		}
		if (GServerLoaderInst->m_Config.GetLoadServers() & CServerConfig::CFG_DEFAULT_SELECT) {
			strTemp.Load(IDD_SELECT);
			strTitle += strTemp;
			strTitle += TF("-");
		}
		if (GServerLoaderInst->m_Config.GetLoadServers() & CServerConfig::CFG_DEFAULT_LOGINDB) {
			strTemp.Load(IDD_LOGINDB);
			strTitle += strTemp;
			strTitle += TF("-");
		}
		if (GServerLoaderInst->m_Config.GetLoadServers() & CServerConfig::CFG_DEFAULT_LOGIN) {
			strTemp.Load(IDD_LOGIN);
			strTitle += strTemp;
			strTitle += TF("-");
		}
		if (GServerLoaderInst->m_Config.GetLoadServers() & CServerConfig::CFG_DEFAULT_GAMEDB) {
			strTemp.Load(IDD_GAMEDB);
			strTitle += strTemp;
			strTitle += TF("-");
		}
		if (GServerLoaderInst->m_Config.GetLoadServers() & CServerConfig::CFG_DEFAULT_GAME) {
			strTemp.Load(IDD_GAME);
			strTitle += strTemp;
			strTitle += TF("-");
		}
		if (GServerLoaderInst->m_Config.GetLoadServers() & CServerConfig::CFG_DEFAULT_ZONE) {
			strTemp.Load(IDD_ZONE);
			strTitle += strTemp;
			strTitle += TF("-");
		}
		if (GServerLoaderInst->m_Config.GetLoadServers() & CServerConfig::CFG_DEFAULT_GATE) {
			strTemp.Load(IDD_GATE);
			strTitle += strTemp;
			strTitle += TF("-");
		}
		strTitle.TrimRight(TF('-'));
		strTitle += TF("]");
		SetWindowText(*strTitle);
		UpdateServerDlgs();
		FrameLayout();
	}
	else {
		GServerLoaderInst->StopServers();
		GServerLoaderInst->UnloadServers();
	}
}

void CMainFrame::OnStop(bool bRepaint)
{
	if (m_CmdBar.IsButtonEnabled(IDC_START) == FALSE) {
		CEventQueue::DestroyTickEvent(FRAMEC_TIMER);
		CThread::SleepEx(FRAMEC_TIME_SECOND);

		GServerLoaderInst->StopServers();
		GServerLoaderInst->UnloadServers();

		KillTimer(FRAMEC_TIMER);

		UnloadServerDlgs();

		m_nIndex  = FRAMEC_STATUS;
		m_nTimes  = 0;
		m_nTabSel = -1;

		m_StatusBar.SetText(1, TF("--"));

		m_CmdBar.EnableButton(IDC_START, TRUE);
		m_CmdBar.EnableButton(IDC_UPDATE, TRUE);
		m_CmdBar.EnableButton(IDC_PAUSE, FALSE);
		m_CmdBar.EnableButton(IDC_STOP, FALSE);

		CStringFix strTitle;
		strTitle.Load(IDS_CENTER_ID);
		SetWindowText(*strTitle);

		UpdateServerDlgs(false);

		if (bRepaint) {
			SendMessage(WM_SIZE, 0, 0);
		}
	}
}

void CMainFrame::OnUpdate(void)
{
	GServerLoaderInst->m_Config.Init();
}

bool CMainFrame::LoadServerDlgs(void)
{
	RECT rc;
	m_Servers.GetClientRect(&rc);

	CXUITraits::CREATE_PARAM cpm;
	cpm.pParent  = &m_Servers;
	cpm.bFlag    = FALSE;
	cpm.nx       = FRAMEC_TABGRAP;
	cpm.ny       = FRAMEC_TABGRAP;
	cpm.ncx      = (rc.right  - FRAMEC_TABGRAP - FRAMEC_TABGRAP);
	cpm.ncy      = (rc.bottom - FRAMEC_TABGRAP - FRAMEC_TABHEIGHT);

	for (Int i = 0; i < DATA_INDEX_MAX; ++i) {
		InsertServerDlg((1 << i), i, cpm);
	}

	CCommonDlg* pShowDlg = nullptr;
	for (Int i = 0; i < DATA_INDEX_MAX; ++i) {
		if (m_pServerDlg[i] != nullptr) {
			pShowDlg = m_pServerDlg[i];
			break;
		}
	}
	if (pShowDlg != nullptr) {
		m_Servers.SetCurSel(pShowDlg->GetTabIndex());
		pShowDlg->ShowWindow(SW_SHOW);
		m_nTabSel = pShowDlg->GetTabIndex();
	}
	return (pShowDlg != nullptr);
}

void CMainFrame::LayoutServerDlgs(void)
{
	RECT rc;
	m_Servers.GetClientRect(&rc);
	rc.left   += FRAMEC_TABGRAP;
	rc.top    += FRAMEC_TABHEIGHT;
	rc.right  -= (FRAMEC_TABGRAP + FRAMEC_TABGRAP);
	rc.bottom -= (FRAMEC_TABGRAP + FRAMEC_TABHEIGHT);
	for (Int i = 0; i < DATA_INDEX_MAX; ++i) {
		if (m_pServerDlg[i] != nullptr) {
			m_pServerDlg[i]->MoveWindow(rc.left, rc.top, rc.right, rc.bottom);
		}
	}
}

void CMainFrame::UpdateServerDlgs(bool bStart)
{
	for (Int i = 0; i < DATA_INDEX_MAX; ++i) {
		if (m_pServerDlg[i] != nullptr) {
			m_pServerDlg[i]->OnHandle(PAK_EVENT_LIVE, bStart, 0);
		}
	}
}

void CMainFrame::UnloadServerDlgs(void)
{
	m_Servers.DeleteAllItems();
	for (Int i = 0; i < DATA_INDEX_MAX; ++i) {
		if (m_pServerDlg[i] != nullptr) {
			m_pServerDlg[i]->DestroyWindow();
			MDELETE m_pServerDlg[i];
			m_pServerDlg[i] = nullptr;
		}
	}
}

bool CMainFrame::CreateServerDlg(Int nIndex)
{
	if (m_pServerDlg[nIndex] == nullptr) {
		switch (nIndex) {
		case DATA_INDEX_CENTER:
			{
				m_pServerDlg[nIndex] = MNEW CCenterDlg;
			}
			break;
		case DATA_INDEX_SELECT:
			{
				m_pServerDlg[nIndex] = MNEW CSelectDlg;
			}
			break;
		case DATA_INDEX_LOGINDB:
			{
				m_pServerDlg[nIndex] = MNEW CLoginDBDlg;
			}
			break;
		case DATA_INDEX_LOGIN:
			{
				m_pServerDlg[nIndex] = MNEW CLoginDlg;
			}
			break;
		case DATA_INDEX_GAMEDB:
			{
				m_pServerDlg[nIndex] = MNEW CGameDBDlg;
			}
			break;
		case DATA_INDEX_GAME:
			{
				m_pServerDlg[nIndex] = MNEW CGameDlg;
			}
			break;
		case DATA_INDEX_GATE:
			{
				m_pServerDlg[nIndex] = MNEW CGateDlg;
			}
			break;
		default:
			{
				assert(0);
			}
		}
	}
	return (m_pServerDlg[nIndex] != nullptr);
}

bool CMainFrame::InsertServerDlg(Int nLoad, Int nIndex, CXUITraits::CREATE_PARAM& cpm)
{
	TCITEM Item;
	Item.mask = TCIF_PARAM | TCIF_TEXT;

	if ((GServerLoaderInst->m_Config.GetLoadServers() & nLoad) && CreateServerDlg(nIndex)) {
		cpm.itMenuID = IDD_INDEXBASE + nIndex;
		if ((GServerLoaderInst->m_UIManagerPtr->Create(*(m_pServerDlg[nIndex]), cpm) != -1) &&
			(GServerLoaderInst->LoadServer(nLoad, m_pServerDlg[nIndex]) == true)) {
			///////////////////////////////////////////////////////
			CStringKey strName;
			strName.Load(IDD_INDEXBASE + nIndex);
			strName.AppendFormat(TF("[%d]"), GServerLoaderInst->m_Config.GetServerId(nLoad));
			Item.pszText = *strName;
			Item.lParam = (LPARAM)m_pServerDlg[nIndex];

			Int nRet = m_Servers.GetItemCount();
			nRet = m_Servers.InsertItem(nRet, &Item);
			if (nRet >= 0) {
				m_pServerDlg[nIndex]->SetTabIndex(nRet);
				return true;
			}
			///////////////////////////////////////////////////////
		}
	}
	if (m_pServerDlg[nIndex] != nullptr) {
		m_pServerDlg[nIndex]->DestroyWindow();
		MDELETE m_pServerDlg[nIndex];
		m_pServerDlg[nIndex] = nullptr;
	}
	return false;
}




