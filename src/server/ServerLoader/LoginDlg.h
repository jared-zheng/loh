//////////////////////////////////////////////////////////////////
//   The Legend of Heros                                        //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : LoginDlg.h                                   //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : 登陆服务器管理界面对话框                            //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __LOGIN_DLG_H__
#define __LOGIN_DLG_H__

#pragma once

#include "CommonDlg.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CLoginDlg
class CLoginDlg : public CCommonDlg
{
public:
	CLoginDlg(void);
	virtual ~CLoginDlg(void);

	DECLARE_DLG_ID_WND_EXCHANGE( CLoginDlg )
protected :
	virtual LRESULT OnInitDialog(void);

	LRESULT OnSize(WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	BEGIN_INLINE_MSG_MAP( CLoginDlg )
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
	UInt                   m_uLoginOnline; // 登陆服务器连接客户端当前在线数量
	UInt                   m_uLoginAll;    // 登陆服务器连接客户端总数量
	UInt                   m_uLoginBusy;   // 登陆服务器繁忙程度
	UInt                   m_uGameCount;   // 连接的游戏服务器数量
	UInt                   m_uGameOnline;  // 游戏服务器连接地图&网关服务器当前在线数量
	UInt                   m_uGameAll;     // 游戏服务器连接地图&网关服务器总数量
	CXStatic               m_ConnectCenter;
	CXStatic               m_ConnectLoginDB;
	CXStatic               m_TCPService;
	CXStatic               m_UDPService;
	CXStatic               m_ServerInfo;
	CXListView             m_GameSvr;
};

#endif // __LOGIN_DLG_H__
