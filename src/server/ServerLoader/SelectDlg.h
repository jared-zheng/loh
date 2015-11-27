//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : SelectDlg.h                                  //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : 选择服务器界面对话框                               //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __SELECT_DLG_H__
#define __SELECT_DLG_H__

#pragma once

#include "CommonDlg.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 选择服务器界面对话框
class CSelectDlg : public CCommonDlg
{
public:
	CSelectDlg(void);
	virtual ~CSelectDlg(void);

	DECLARE_DLG_ID_WND_EXCHANGE( CSelectDlg )
protected :
	virtual LRESULT OnInitDialog(void) OVERRIDE;

	LRESULT OnSize(WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	BEGIN_INLINE_MSG_MAP( CSelectDlg )
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
	UInt         m_uSelectAll;  
	UInt         m_uLoginCount; 
	UInt         m_uLoginOnline;
	UInt         m_uLoginAll;   
	CXStatic     m_ConnectCenter;
	CXStatic     m_UDPService;
	CXStatic     m_ServerInfo;
	CXListView   m_LoginSvr;
};

#endif // __SELECT_DLG_H__
