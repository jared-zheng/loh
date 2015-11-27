//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : CenterDlg.h                                  //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : 中心服务器界面对话框                               //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __CENTER_DLG_H__
#define __CENTER_DLG_H__

#pragma once

#include "CommonDlg.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 中心服务器界面对话框
class CCenterDlg : public CCommonDlg
{
public:
	CCenterDlg(void);
	virtual ~CCenterDlg(void);

	DECLARE_DLG_ID_WND_EXCHANGE( CCenterDlg )
protected :
	virtual LRESULT OnInitDialog(void) OVERRIDE;

	LRESULT OnSize(WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	BEGIN_INLINE_MSG_MAP( CCenterDlg )
	// {{ --
		MSG_HANDLER(WM_SIZE, OnSize)
	// --}}
	END_CHAIN_MSG_MAP( CXDlg )
private:
	virtual void OnLive(bool bStart) OVERRIDE;
	virtual void OnLink(Int nServerIndex, uintptr_t utParam) OVERRIDE;
	virtual void OnUpdate(Int nServerIndex, uintptr_t utParam) OVERRIDE;
	virtual void OnSync(Int nServerIndex, uintptr_t utParam) OVERRIDE;
	virtual void OnSync(Int nServerIndex, CStream& Stream) OVERRIDE;
	virtual void OnUnlink(Int nServerIndex, uintptr_t utParam) OVERRIDE;

	virtual void UpdateServerInfo(void) OVERRIDE;

	virtual CXListView* GetListView(Int nServerIndex) OVERRIDE;
private:
	UInt         m_uCenterOnline; 
	UInt         m_uCenterAll;    
	UInt         m_uCenterBusy;   
	UInt         m_uSelectCount;  
	UInt         m_uSelectOnline; 
	UInt         m_uSelectAll;    
	UInt         m_uLoginCount;   
	UInt         m_uLoginOnline;  
	UInt         m_uLoginAll;     
	UInt         m_uGameCount;    
	UInt         m_uGameOnline;   
	UInt         m_uGameAll;      
	CXStatic     m_ListenSelect;
	CXStatic     m_ListenLogin;
	CXStatic     m_ListenGame;
	CXStatic     m_ServerInfo;
	CXListView   m_SelectSvr;
	CXListView   m_LoginSvr;
	CXListView   m_GameSvr;
};

#endif // __CENTER_DLG_H__
