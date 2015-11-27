//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : LoginDBDlg.h                                 //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : 登陆DB服务器界面对话框                              //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __LOGINDB_DLG_H__
#define __LOGINDB_DLG_H__

#pragma once

#include "CommonDlg.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 登陆DB服务器界面对话框
class CLoginDBDlg : public CCommonDlg
{
public:
	CLoginDBDlg(void);
	virtual ~CLoginDBDlg(void);

	DECLARE_DLG_ID_WND_EXCHANGE( CLoginDBDlg )
protected :
	virtual LRESULT OnInitDialog(void) OVERRIDE;

	LRESULT OnSize(WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	BEGIN_INLINE_MSG_MAP( CLoginDBDlg )
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
	UInt                   m_uLoginDBOnline;
	UInt                   m_uLoginDBAll;   
	UInt                   m_uLoginDBBusy;  
	CXStatic               m_ListenLogin;
	CXStatic               m_ServerInfo;
	CXListView             m_LoginSvr;
};

#endif // __LOGINDB_DLG_H__
