//////////////////////////////////////////////////////////////////
//   The Legend of Heros                                        //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : GameDBDlg.h                                  //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : 游戏DB服务器管理界面对话框                          //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __GAMEDB_DLG_H__
#define __GAMEDB_DLG_H__

#pragma once

#include "CommonDlg.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CGameDBDlg
class CGameDBDlg : public CCommonDlg
{
public:
	CGameDBDlg(void);
	virtual ~CGameDBDlg(void);

	DECLARE_DLG_ID_WND_EXCHANGE( CGameDBDlg )
protected :
	virtual LRESULT OnInitDialog(void) OVERRIDE;

	LRESULT OnSize(WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	BEGIN_INLINE_MSG_MAP( CGameDBDlg )
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
	UInt                   m_uGameDBOnline; // 在线服务器数量(游戏+网关)
	UInt                   m_uGameDBAll;    // 总共连接过的服务器数量
	UInt                   m_uGameDBBusy;   // 游戏DB服务器繁忙程度
	UInt                   m_uGameCount;    // 连接的游戏服务器数量
	UInt                   m_uGameOnline;   // 游戏服务器连接地图&网关服务器当前在线数量
	UInt                   m_uGameAll;      // 游戏服务器连接地图&网关服务器总数量
	UInt                   m_uGateCount;    // 连接的网关服务器数量
	UInt                   m_uGateOnline;	// 网关服务器连接客户端当前在线数量
	UInt                   m_uGateAll;      // 网关服务器连接客户端总数量
	CXStatic               m_ListenGame;
	CXStatic               m_ListenGate;
	CXStatic               m_ServerInfo;
	CXListView             m_GameSvr;
	CXListView             m_GateSvr;
};

#endif // __GAMEDB_DLG_H__
