//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : CenterDlg.h                                  //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : 中心服务器管理界面对话框                            //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __CENTER_DLG_H__
#define __CENTER_DLG_H__

#pragma once

#include "CommonDlg.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CCenterDlg
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
	UInt         m_uCenterOnline; // 在线服务器数量(选择+登陆+游戏)
	UInt         m_uCenterAll;    // 总共连接过的服务器数量
	UInt         m_uCenterBusy;   // 中心服务器繁忙程度
	UInt         m_uSelectCount;  // 连接的选择服务器数量
	UInt         m_uSelectOnline; // 选择服务器连接客户端当前在线数量
	UInt         m_uSelectAll;    // 选择服务器连接客户端总数量
	UInt         m_uLoginCount;   // 连接的登陆服务器数量
	UInt         m_uLoginOnline;  // 登陆服务器连接客户端当前在线数量
	UInt         m_uLoginAll;     // 登陆服务器连接客户端总数量
	UInt         m_uGameCount;    // 连接的游戏服务器数量
	UInt         m_uGameOnline;   // 游戏服务器连接地图&网关服务器当前在线数量
	UInt         m_uGameAll;      // 游戏服务器连接地图&网关服务器总数量
	CXStatic     m_ListenSelect;
	CXStatic     m_ListenLogin;
	CXStatic     m_ListenGame;
	CXStatic     m_ServerInfo;
	CXListView   m_SelectSvr;
	CXListView   m_LoginSvr;
	CXListView   m_GameSvr;
};

#endif // __CENTER_DLG_H__
