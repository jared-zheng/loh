//////////////////////////////////////////////////////////////////
//   The Legend of Heros                                        //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : GateDlg.h                                    //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : 网关服务器管理界面对话框                            //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __GATE_DLG_H__
#define __GATE_DLG_H__

#pragma once

#include "CommonDlg.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CGateDlg
class CGateDlg : public CCommonDlg
{
public:
	CGateDlg(void);
	virtual ~CGateDlg(void);

	DECLARE_DLG_ID_WND_EXCHANGE( CGateDlg )
protected :
	virtual LRESULT OnInitDialog(void) OVERRIDE;

	LRESULT OnSize(WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	BEGIN_INLINE_MSG_MAP( CGateDlg )
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
	UInt                   m_uGateOnline;  // 网关服务器连接客户端当前在线数量
	UInt                   m_uGateAll;     // 网关服务器连接客户端总数量
	UInt                   m_uGateBusy;    // 网关服务器繁忙程度
	UInt                   m_uQueueUsers;
	UInt                   m_uSelectUsers;
	UInt                   m_uPlayUsers;
	CXStatic               m_ConnectGameDB;
	CXStatic               m_ConnectGame;
	CXStatic               m_TCPService;
	CXStatic               m_UDPService;
	CXStatic               m_ServerInfo;
	CXListView             m_GateBlock;
};

#endif // __GATE_DLG_H__
