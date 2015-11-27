//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : TestLaunch.h                                 //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : 测试客户端                                         //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __TEST_LAUNCH_H__
#define __TEST_LAUNCH_H__

#pragma once

#include "singleton.h"

#include "xui.h"
#include "xuikit.h"

#include "selectpacket.h"
#include "loginpacket.h"
#include "gatepacket.h"
#include "servermap.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 测试客户端子系统加载器
class CTestSystem : public MObject
{
	DECLARE_CLASS_LOADER( CTestSystem, XUI )
	DECLARE_CLASS_LOADER( CTestSystem, NET )
public:
	CTestSystem(void);
	~CTestSystem(void);

	bool Init(void);
	void Exit(void);

public:
	CXUIManagerPtr     m_UIManagerPtr;
	CNetworkPtr        m_NetworkPtr;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 测试客户端子系统加载器单件
class CTestSystemSingleton : public CTSingletonInst<CTestSystem>
{
private:
	CTestSystemSingleton(void);
	~CTestSystemSingleton(void);
	CTestSystemSingleton(const CTestSystemSingleton&);
	CTestSystemSingleton& operator=(const CTestSystemSingleton&);
};
typedef CTestSystemSingleton GTestSystem;
#define GTestSystemInst      CTestSystemSingleton::GetInstance()

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 测试客户端视图
class CTestView : public CXView
{
protected:
	virtual void OnPaint(void) OVERRIDE
	{
		CXPaintDC dc(m_hWnd);
		dc.TextOut(100, 100, TF("玺帝语录 ： 程序猿无香蕉[QQ群:367944520]"));
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 测试客户端登陆对话框
class CTestLoginDlg : public CXDlg
{
public:
	PCXStr GetId(void);
	PCXStr GetPw(void);

	DECLARE_DLG_ID_WND_EXCHANGE( CTestLoginDlg )
protected :
	//virtual LRESULT OnInitDialog(void);
	//virtual void    OnDestroy(void);

	LRESULT OnOk(WORD wCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	BEGIN_INLINE_MSG_MAP( CTestLoginDlg )
	// {{ --
		COMMAND_ID_HANDLER( IDOK,     OnOk )
	// --}}
	END_CHAIN_MSG_MAP( CXDlg )
private:
	CXEdit     m_xuiId;
	CXEdit     m_xuiPw;
	CXStatic   m_xuiInfo;
	XChar      m_szId[LMT_KEY];
	XChar      m_szPw[LMT_KEY];
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 测试客户端游戏服务器对话框
class CTestSelGameDlg : public CXDlg
{
public:
	CTestSelGameDlg(void);
	~CTestSelGameDlg(void);
	/// 登陆成功返回的游戏服务器信息
	/// @param[in] CPAKLoginLinkAck* pLinkAck 登陆成功返回的数据包
	void   LinkAck(CPAKLoginLinkAck* pLinkAck);
	/// 同步游戏服务器信息
	/// @param[in] CLoginSync* pSync 同步数据包
	void   Sync(CLoginSync* pSync);
	/// 获得用户选择的游戏服务器索引值
	LLong  GetGameKey(void) { return m_llGameKey;  }

	DECLARE_DLG_ID_WND_EXCHANGE( CTestSelGameDlg )
protected :
	virtual LRESULT OnInitDialog(void) OVERRIDE;

	LRESULT OnOk(WORD wCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	BEGIN_INLINE_MSG_MAP( CTestLoginDlg )
	// {{ --
		COMMAND_ID_HANDLER( IDOK, OnOk )
	// --}}
	END_CHAIN_MSG_MAP( CXDlg )
private:
	void Update(void);
	void Link(DataRef drKey, SERVER_DATA& sd, CNETTraits::NET_ADDR* pAddr);
	void Update(Int nIndex, SERVER_DATA& sd);
public:
	Int                   m_nGameId;
	LLong                 m_llGameKey;
	SERVER_DATA           m_ServerData;
	CXListView            m_xuiGameSvr;
	SVR_TEST_MAP          m_ServerMap;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 测试客户端主窗口
class CTestMainFrame : public CXFrameWnd, public CPAKHandler
{
public:
	/// 主窗口常量
	enum FRAME_CONST
	{
		FRAMEC_RETRYCOUNT  = 3,     ///< 最多尝试次数

		FRAMEC_TICK_ID     = 1000,  ///< UDP获取登录服务器地址最长等待时间的定时Id
		FRAMEC_TICK_TIME   = 30000, ///< UDP获取登录服务器地址最长等待时间

		FRAMEC_SELECTPORT  = 10010, ///< 选择服务器默认端口

		FRAMEC_WIDTH       = 800,   ///< 主窗口默认宽度
		FRAMEC_HEIGHT      = 600,   ///< 主窗口默认高度
		FRAMEC_LIST_HEIGHT = 200,   ///< 主窗口日志窗口高度

		FRAMEC_TRAITS      = WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX, ///< ///< 主窗口属性
	};
	/// 主窗口游戏状态
	enum FRAME_STATE
	{
		FRAME_STATE_NONE = 0,
		FRAME_STATE_SELECT,
		FRAME_STATE_LOGIN,
		FRAME_STATE_SELGAME,
		FRAME_STATE_CNTGAME,
		FRAME_STATE_LINKGAME1,
		FRAME_STATE_LINKGAME2,
		FRAME_STATE_QUEUEGAME,
		FRAME_STATE_SELECTROLE,
		FRAME_STATE_PLAYGAME,
		FRAME_STATE_EXIT,
	};
	/// 主窗口游戏状态对应的自定义消息
	enum FRAME_MSG
	{
		FRAME_MSG_SELECT = WM_USER + 0x200,
		FRAME_MSG_LOGIN,
		FRAME_MSG_SELGAME,
		FRAME_MSG_CNTGAME,
		FRAME_MSG_LINKGAME1,
		FRAME_MSG_LINKGAME2,
		FRAME_MSG_QUEUEGAME,
		FRAME_MSG_SELECTROLE,
		FRAME_MSG_PLAYGAME,
		FRAME_MSG_EXIT,
	};

	static CPCXStr SELECT_SERVER_ADDR;
public:
	CTestMainFrame(void);
	virtual ~CTestMainFrame(void);
	// CPAKHandler
	virtual bool   OnTcpDispatch(const PacketPtr& PktPtr, PTCP_PARAM pTcp) OVERRIDE;
	virtual bool   OnUdpDispatch(const PacketPtr& PktPtr, PUDP_PARAM pUdp) OVERRIDE;
	virtual bool   OnTcpAccept(KeyRef krAccept, KeyRef krListen) OVERRIDE;
	virtual bool   OnTcpConnect(UInt uError, KeyRef krConnect) OVERRIDE;
	virtual bool   OnTcpClose(KeyRef krSocket, LLong llLiveData) OVERRIDE;
	virtual bool   OnUdpClose(KeyRef krSocket, LLong llLiveData) OVERRIDE;

	virtual bool   OnHandle(Int nEvent, UInt uCount) OVERRIDE;
protected:
	virtual LRESULT OnCreate(LPCREATESTRUCT lpCS) OVERRIDE;
	virtual void    OnDestroy(void) OVERRIDE;
	virtual void    FrameLayout(void) OVERRIDE;

	LRESULT OnSelect(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLogin(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSelGame(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCntGame(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLinkGame1(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//LRESULT OnLinkGame2(WPARAM wParam, LPARAM lParam, BOOL& bHandled);


	BEGIN_INLINE_MSG_MAP( CTestMainFrame )
	// {{ --
		MSG_HANDLER( FRAME_MSG_SELECT,   OnSelect )
		MSG_HANDLER( FRAME_MSG_LOGIN,    OnLogin )
		MSG_HANDLER( FRAME_MSG_SELGAME,  OnSelGame )
		MSG_HANDLER( FRAME_MSG_CNTGAME,  OnCntGame )
		MSG_HANDLER( FRAME_MSG_LINKGAME1, OnLinkGame1 )
		//MSG_HANDLER( FRAME_MSG_LINKGAME2, OnLinkGame2 )
		MSG_WM_CREATE()
		MSG_WM_DESTROY()
	// --}}
	END_CHAIN_MSG_MAP( CXFrameWnd )
private:
	void OnSelectAck(CPAKSelectLoginAck* pSelectAck, NET_ADDR& ServerAddr);
	void OnLoginAck(CPAKLoginLinkAck* pLoginAck);
	void OnSelectGameAck(CPAKLoginSelectGame* pSelectGameAck);
	void OnLinkGame1Ack(CPAKLoginLinkGame* pLinkGame1Ack);
	void OnLinkGame2Ack(CPAKLoginLinkGame* pLinkGame2Ack);
private:
	Int                    m_nState;
	Int                    m_nRetryCount;
	KeyRef                 m_krUDPSelect;
	KeyRef                 m_krTCPLogin;
	KeyRef                 m_krTCPGame;
	LLong                  m_llTimeout;
	LLong                  m_llLoginSession;
	LLong                  m_llUserId;
	LLong                  m_llGameSession;
	NET_ADDR               m_adrSelGate;    // [ipv4 or ipv6]
	CTestView              m_xuiView;
	CXListBox              m_xuiList;
	CTestLoginDlg          m_xuiLoginDlg;
	CTestSelGameDlg        m_xuiSelGameDlg;
};

#endif // __TEST_LAUNCH_H__
