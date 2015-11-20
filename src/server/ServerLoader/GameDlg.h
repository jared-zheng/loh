//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : GameDlg.h                                    //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : 游戏服务器管理界面对话框                            //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __GAME_DLG_H__
#define __GAME_DLG_H__

#pragma once

#include "CommonDlg.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CGameDlg
class CGameDlg : public CCommonDlg
{
public:
	CGameDlg(void);
	virtual ~CGameDlg(void);

	DECLARE_DLG_ID_WND_EXCHANGE( CGameDlg )
protected :
	virtual LRESULT OnInitDialog(void) OVERRIDE;

	LRESULT OnSize(WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	BEGIN_INLINE_MSG_MAP( CGameDlg )
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
	UInt                   m_uGameOnline; // 游戏服务器连接地图&网关服务器当前在线数量
	UInt                   m_uGameAll;    // 游戏服务器连接地图&网关服务器总数量
	UInt                   m_uGameBusy;   // 游戏服务器繁忙程度
	UInt                   m_uZoneCount;  // 连接的地图服务器数量
	UInt                   m_uZoneOnline; // 地图服务器连接客户端当前在线数量
	UInt                   m_uZoneAll;    // 地图服务器连接客户端总数量
	UInt                   m_uGateCount;  // 连接的网关服务器数量
	UInt                   m_uGateOnline; // 网关服务器连接客户端当前在线数量
	UInt                   m_uGateAll;    // 网关服务器连接客户端总数量
	CXStatic               m_ConnectCenter;
	CXStatic               m_ConnectGameDB;
	CXStatic               m_PingService;
	CXStatic               m_ListenZone;
	CXStatic               m_ListenGate;
	CXStatic               m_UDPService;
	CXStatic               m_ServerInfo;
	CXListView             m_ZoneSvr;
	CXListView             m_GateSvr;
};

#endif // __GAME_DLG_H__
