//////////////////////////////////////////////////////////////////
//   The Legend of Heros                                        //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : MainFrame.h                                  //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : 服务加载器主界面                                   //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __MAIN_FRAME_H__
#define __MAIN_FRAME_H__

#pragma once

#include "resource.h"
#include "ServerLoader.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CMainFrame
class CMainFrame : public CXFrameWnd, public CEventHandler
{
public:
	enum FRAME_CONST
	{
		FRAMEC_WIDTH       = 800,
		FRAMEC_HEIGHT      = 600,
		FRAMEC_TABGRAP     = 2,
		FRAMEC_TABHEIGHT   = 28,

		FRAMEC_STATUS      = 4,
		FRAMEC_TIMER       = 101,
		FRAMEC_TIMER_TIME  = 500,
		FRAMEC_TIME_SECOND = 1000,
		FRAMEC_TIME_DAY    = 24,
		FRAMEC_TIME_HOUR   = 3600,
		FRAMEC_TIME_MINUTE = 60,
	};

	static CPCXStr ANM_STATUS;
public:
	CMainFrame(void);
	virtual ~CMainFrame(void);
	// CEventHandler
	virtual bool OnHandle(Int nEvent, uintptr_t utParam, LLong llParam) OVERRIDE;
	virtual bool OnHandle(Int nEvent, CEventBase& pParam, LLong llParam) OVERRIDE;
	virtual bool OnHandle(Int nEvent, CStream& Stream, LLong llParam) OVERRIDE;
	virtual bool OnHandle(Int nEvent, UInt uCount) OVERRIDE;
protected:
	virtual LRESULT OnCreate(LPCREATESTRUCT lpCS) OVERRIDE;
	virtual void    OnDestroy(void) OVERRIDE;
	virtual void    FrameLayout(void) OVERRIDE;
	virtual LRESULT OnNotify(Int nID, LPNMHDR pHead, BOOL& bHandled) OVERRIDE;

	LRESULT OnTimer(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnGetMinMaxInfo(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCommand(WORD wCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnXNotify(WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	BEGIN_INLINE_MSG_MAP( CMainFrame )
	// {{ --
		MSG_HANDLER(WM_TIMER, OnTimer)
		MSG_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
		COMMAND_RANGE_HANDLER(IDC_START, IDC_EXIT, OnCommand)
		MSG_XUIM_NOTIFY()
		MSG_WM_CREATE()
		MSG_WM_DESTROY()
	// --}}
	END_CHAIN_MSG_MAP( CXFrameWnd )
private:
	void OnStart(void);
	void OnPause(void);
	void OnStop(bool bRepaint = true);
	void OnUpdate(void);

	bool LoadServerDlgs(void);
	void LayoutServerDlgs(void);
	void UpdateServerDlgs(bool bStart = true);
	void UnloadServerDlgs(void);

	bool CreateServerDlg(Int nIndex);
	bool InsertServerDlg(Int nLoad, Int nIndex, CXUITraits::CREATE_PARAM& cpm);
private:
	Int           m_nIndex;
	Int           m_nTimes;
	Int           m_nTabSel;
	CCommonDlg*   m_pServerDlg[DATA_INDEX_MAX];
	CXToolBar     m_CmdBar;
	CXStatusBar   m_StatusBar;
	CXTab         m_Servers;
	XChar         m_szStatus[LMT_KEY];
};

#endif // __MAIN_FRAME_H__
