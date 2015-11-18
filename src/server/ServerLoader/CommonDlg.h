//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : CommonDlg.h                                  //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : 服务器管理界面TAB子对话框基类                       //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __COMMON_DLG_H__
#define __COMMON_DLG_H__

#pragma once

#include "xui.h"
#include "xuikit.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CCommonDlg
class ICommonServer;
class CCommonDlg : public CXDlg, public CEventHandler
{
public:
	enum DLG_ITEM
	{
		DLG_ITEM_GRAP   = 2,
		DLG_ITEM_HEIGHT = 18,
		DLG_ITEM_WIDTH  = 180,
	};
public:
	CCommonDlg(void);
	virtual ~CCommonDlg(void);

	Int   GetTabIndex(void);
	void  SetTabIndex(Int nIndex);

	ICommonServer* GetServer(void);
	void           SetServer(ICommonServer* pServer);
	// CEventHandler
	virtual bool OnHandle(Int nEvent, uintptr_t utParam, LLong llParam) OVERRIDE;
	virtual bool OnHandle(Int nEvent, CEventBase& pParam, LLong llParam) OVERRIDE;
	virtual bool OnHandle(Int nEvent, CStream& Stream, LLong llParam) OVERRIDE;
	virtual bool OnHandle(Int nEvent, UInt uCount) OVERRIDE;
protected:
	virtual void OnLive(bool bStart) PURE_METHOD;
	virtual void OnLink(Int nServerIndex, uintptr_t utParam) PURE_METHOD;
	virtual void OnUpdate(Int nServerIndex, uintptr_t utParam) PURE_METHOD;
	virtual void OnSync(Int nServerIndex, uintptr_t utParam) PURE_METHOD;
	virtual void OnSync(Int nServerIndex, CStream& Stream) PURE_METHOD;
	virtual void OnUnlink(Int nServerIndex, uintptr_t utParam) PURE_METHOD;

	virtual void UpdateServerInfo(void) PURE_METHOD;

	virtual CXListView* GetListView(Int nServerIndex) PURE_METHOD;

	void  InitListViewItem(Int nServerIndex);
	bool  AddListViewData(Int nServerIndex, uintptr_t utParam);
	bool  UpdateListViewData(Int nServerIndex, uintptr_t utParam);
	bool  RemoveListViewData(Int nServerIndex, uintptr_t utParam);

	void  AddListViewAddr(Int nServerIndex, Int nIndex, void* pAddr);
protected:
	Int              m_nTabIndex;
	ICommonServer*   m_pServer;
};

INLINE CCommonDlg::CCommonDlg(void)
: m_nTabIndex(-1)
, m_pServer(nullptr)
{
}

INLINE CCommonDlg::~CCommonDlg(void)
{
}

INLINE Int CCommonDlg::GetTabIndex(void)
{
	return m_nTabIndex;
}

INLINE void CCommonDlg::SetTabIndex(Int nIndex)
{
	m_nTabIndex = nIndex;
}

INLINE ICommonServer* CCommonDlg::GetServer(void)
{
	return m_pServer;
}

INLINE void CCommonDlg::SetServer(ICommonServer* pServer)
{
	m_pServer = pServer;
}

#endif // __COMMON_DLG_H__
