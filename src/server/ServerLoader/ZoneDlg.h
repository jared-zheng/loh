//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : ZoneDlg.h                                    //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : 地图服务器界面对话框                               //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __ZONE_DLG_H__
#define __ZONE_DLG_H__

#pragma once

#include "CommonDlg.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 地图服务器界面对话框
class CZoneDlg : public CCommonDlg
{
public:
	CZoneDlg(void);
	virtual ~CZoneDlg(void);

	DECLARE_DLG_ID_WND_EXCHANGE( CZoneDlg )
protected :
	virtual LRESULT OnInitDialog(void) OVERRIDE;

	LRESULT OnSize(WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	BEGIN_INLINE_MSG_MAP( CZoneDlg )
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
	UInt                   m_uZoneOnline;
	UInt                   m_uZoneAll;   
	UInt                   m_uZoneIds;
	UInt                   m_uZoneThreads; 
	CXStatic               m_ConnectGame;
	CXStatic               m_ZoneConfig;
	CXStatic               m_ServerInfo;
	CXListView             m_Scenes;
};

#endif // __ZONE_DLG_H__
