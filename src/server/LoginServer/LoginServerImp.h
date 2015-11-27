//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : LoginServerImp.h                             //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : 登陆服务器实现                                     //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __LOGIN_SERVER_IMP_H__
#define __LOGIN_SERVER_IMP_H__

#pragma once

#include "logindbpacket.h"
#include "loginpacket.h"
#include "servermap.h"
#include "ServerConfig.h"
#include "CommonServer.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 登陆服务器实现
/// - 主要任务 :
/// -# 维护在线游戏客户端登陆事务
/// -# 事务简单, 全部使用网络线程和定时线程来处理
/// -# TODO!!! /// @todo 黑名单校验未完成
///    a. 从登陆服务器扩展配置文件中读取黑名单列表
///    b. 增加按错误屏蔽客户端连接的信息表, 将恶意攻击或者多次连接的加入表中, 按严重程度累加屏蔽时间
///    c. 界面也需要增加屏蔽客户端的信息表的显示及操作功能
///    d. 设置网络层的ACK(1000MS)和Timeout(30000MS)属性
///    e. 短时间(例如小于200MS的连续3次)频繁操作的, 拉入黑名单进行屏蔽
///    f. 存放永久黑名单+定时黑名单(例如1小时清理一次的, 加入最小时间为2小时)
///    g. 临时地址map缓存增长以10000000(1千万, 64位直接耗600M)为基数, 定时清除(例如2小时清理一次的)
class CLoginServer : public ICommonServer, public CPAKHandler
{
public:
	CLoginServer(void);
	virtual ~CLoginServer(void);
	// CComponent
	// Command to set param value
	virtual UInt   Command(PCXStr pszCMD, uintptr_t utParam) OVERRIDE;
	virtual UInt   Update(void) OVERRIDE;

	virtual UInt   Init(CEventHandler& EventHandlerRef) OVERRIDE;
	virtual void   Exit(void) OVERRIDE;

	virtual bool   Start(void) OVERRIDE;
	virtual bool   Pause(bool bPause = true) OVERRIDE;
	virtual void   Stop(void) OVERRIDE;

	virtual bool   OnShareRoutine(Int nEvent, uintptr_t utParam = 0, LLong llParam = 0, CEventQueue::EVENT_TYPE eType = CEventQueue::EVENT_TYPE_NONE) OVERRIDE;//
	virtual bool   OnShareRoutine(Int nEvent, CEventBase& EventRef, LLong llParam = 0, CEventQueue::EVENT_TYPE eType = CEventQueue::EVENT_TYPE_REFCOUNT) OVERRIDE;
	virtual bool   OnShareRoutine(Int nEvent, CStream& Stream, LLong llParam = 0) OVERRIDE;//
	// CPAKHandler
	virtual bool   OnTcpDispatch(const PacketPtr& PktPtr, PTCP_PARAM pTcp) OVERRIDE;
	virtual bool   OnUdpDispatch(const PacketPtr& PktPtr, PUDP_PARAM pUdp) OVERRIDE;
	virtual bool   OnTcpAccept(KeyRef krAccept, KeyRef krListen) OVERRIDE;
	virtual bool   OnTcpConnect(UInt uError, KeyRef krConnect) OVERRIDE;
	virtual bool   OnTcpClose(KeyRef krSocket, LLong llLiveData) OVERRIDE;
	virtual bool   OnUdpClose(KeyRef krSocket, LLong llLiveData) OVERRIDE;
private:
	CLoginServer(const CLoginServer&);
	CLoginServer& operator=(const CLoginServer&);
	/// 获取共享的配置对象和网络对象
	bool  InitLoadShare(void);
	/// 初始化配置
	bool  InitLoadConfig(void);

	/// 清除共享的配置对象和网络对象
	void  ExitUnloadShare(void);
	/// 清除配置
	void  ExitUnloadConfig(void);

	/// 创建连接中心服务器对象
	bool  StartConnectCenterServer(void);
	/// 创建连接登陆DB服务器对象
	bool  StartConnectLoginDBServer(void);
	/// 创建UDP监听游戏服务器对象
	bool  StartUDPService(void);
	/// 创建监听客户端对象
	bool  StartTCPService(void);

	/// 停止连接中心服务器
	void  StopConnectCenterServer(void);
	/// 停止连接登陆DB服务器
	void  StopConnectLoginDBServer(void);
	/// 停止UDP监听游戏服务器
	void  StopUDPService(void);
	/// 停止监听客户端
	void  StopTCPService(void);

	/// 处理中心服务器的信令包
	bool  DispatchCenterServer(const PacketPtr& PktPtr, KeyRef krSocket);
	/// 处理登陆DB服务器的信令包
	bool  DispatchLoginDBServer(const PacketPtr& PktPtr, KeyRef krSocket);
	/// 处理游戏客户端的信令包
	bool  DispatchGameService(const PacketPtr& PktPtr, KeyRef krSocket);

	/// 向中心服务器注册
	void  LinkCenterServer(void);
	/// 向中心服务器注销
	void  UnlinkCenterServer(void);
	/// 向登陆DB服务器注册
	void  LinkLoginDBServer(void);
	/// 向登陆DB服务器注销
	void  UnlinkLoginDBServer(void);

	/// 黑名单列表检测
	bool  CheckAddrBlacklist(NET_ADDR& ClientAddr);
	/// 定时检测客户端有效性
	bool  CheckSessionTimeout(void);

	/// 定时检测连接中心服务器对象是否有效
	bool  CheckConnectCenterServer(void);
	/// 定时检测连接登陆DB服务器对象是否有效
	bool  CheckConnectLoginDBServer(void);
	/// 定时检测UDP监听游戏服务器对象是否有效
	bool  CheckUDPService(void);
	/// 定时检测监听客户端对象是否有效
	bool  CheckTCPService(void);

	/// 同步服务器信息给界面
	bool  SyncServerInfo(void);
	/// 同步游戏服务器负载数组并同步给界面
	bool  SyncGameServerInfo(CStream& Stream);
	/// 同步游戏服务器信息给客户端
	bool  SyncGameServerInfo(void);

	/// 客户端处理-登陆
	bool  OnClientLink(CPAKLoginLink* pLink, KeyRef krSocket);
	/// 客户端处理-登出
	bool  OnClientUnlink(CPAKLoginUnlink* pUnlink, KeyRef krSocket);
	/// 客户端处理-选择游戏服务器
	bool  OnClientSelectGame(CPAKLoginSelectGame* pSelect, KeyRef krSocket);

	/// 发送登陆处理结果给客户端
	void  GameLinkAck(CPAKSessionAck* pAck);
	/// 发送登出处理结果给客户端
	void  GameUnlinkAck(CPAKSessionAck* pAck);
	/// 选择游戏服务器回复包
	void  SelectGameAck(CPAKSessionAck* pAck);
	/// 验证游戏服务器回复包
	void  LinkGameAck(CPAKLoginLinkGame* pLink);
	/// 用户进入/退出游戏排队或者游戏世界
	void  PlayGameAck(CPAKSessionAck* pAck);
private:
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// 会话对象数据
	struct tagSESSION_DATA : public MObject
	{
	public:
		/// 会话对象状态
		enum SESSION_STATUS
		{
			SESSION_STATUS_NONE,          ///< 无状态
			SESSION_STATUS_LINK,          ///< 收到登陆请求
			SESSION_STATUS_LINKACK,       ///< DB回复登陆成功
			SESSION_STATUS_SELECT,        ///< 收到选择游戏服务器请求
			SESSION_STATUS_SELECTACK,     ///< 选择的游戏服务器回复可以连接
			SESSION_STATUS_GAME,          ///< 选择的游戏服务器转发验证码2
			SESSION_STATUS_GAMEACK,       ///< 游戏服务器回复验证完成, 客户端排队或者游戏中
			SESSION_STATUS_UNLINK,        ///< 收到登出请求
			SESSION_STATUS_UNLINKACK,     ///< 登出状态
		};
		/// 会话对象超时检测
		enum SESSION_TIME
		{
			SESSION_TIME_LINK_MAX    = 5,             ///< 最多密码验证机会或者选择游戏次数, 不行按严重程度累加屏蔽时间
			SESSION_TIME_OUT_MIN     = 30000,         ///< 登陆或者登出提交DB服务器, 选择游戏服务器等待回复时间, 超时需要断开客户端
			SESSION_TIME_OUT_MAX     = 60 * 1000,     ///< 角色排队或者游戏最多等待时间
			SESSION_TIME_LIMIT       = 5 * 60 * 1000, ///< 登陆完成以后, 没有操作最多等待时间
		};
	public:
		tagSESSION_DATA(void) { Reset(); }
		~tagSESSION_DATA(void) { }

		void Reset(void) { memset(this, 0, sizeof(tagSESSION_DATA)); }
	public:
		LLong      llUserId;    ///< 用户Id
		LLong      llTimeout;   ///< 超时时间检测
		LLong      llOnline;    ///< 登陆成功的时间(本地时区)
		Int        nGameId;     ///< 选择的游戏服务器Id
		Int        nStatus;     ///< 会话对象状态
		Int        nCount;      ///< 最多密码验证机会或者选择游戏次数 <= SESSION_TIME_LINK_MAX
		NET_ADDR   CltAddr;     ///< 本次登陆客户端地址
	};
	typedef struct tagSESSION_DATA   SESSION_DATA, *PSESSION_DATA;

	typedef tagDATA_MAP<DataRef, SESSION_DATA>   SESSION_MAP, *PSESSION_MAP;
	typedef CTMap<KeyRef, UInt>                  TIMEOUT_MAP, *PTIMEOUT_MAP;
	typedef CTMap<KeyRef, UInt>::PAIR            TIMEOUT_MAP_PAIR;
private:
	Int              m_nStatus;          ///< 服务器状态
	bool             m_bCenterCnnted;    ///< 是否连接中心服务器完成
	bool             m_bCenterLinked;    ///< 是否向中心服务器注册完成
	bool             m_bLoginDBCnnted;   ///< 是否连接登陆DB服务器完成
	bool             m_bLoginDBLinked;   ///< 是否向登陆DB服务器注册完成
	CEventHandler*   m_pUIHandler;       ///< 界面回调接口
	CServerConfig*   m_pConfig;          ///< 配置对象
	KeyRef           m_krConnectCenter;  ///< 内网, 连接中心服务器
	KeyRef           m_krConnectLoginDB; ///< 内网, 连接登陆DB服务器
	KeyRef           m_krUDPService;     ///< 内网, 监听游戏服务器
	KeyRef           m_krTCPService;     ///< 外网, 监听客户端
	ICommonServer*   m_pShareCenterSvr;  ///< 同进程中心服务器
	ICommonServer*   m_pShareLoginDBSvr; ///< 同进程登陆DB服务器
	ICommonServer*   m_pShareGameSvr;    ///< 同进程游戏服务器
	CFileLog         m_FileLog;          ///< 简单文本日志
	CNetworkPtr      m_NetworkPtr;       ///< 网络对象
	LOGIN_ADDR       m_ServerInfo;       ///< 本服务器信息
	SERVER_INFO      m_GameSvrInfo;      ///< 游戏服务器信息
	SVR_GAME_MAP     m_GameSvrMap;       ///< 游戏服务器信息  
	SESSION_MAP      m_SessionMap;       ///< 会话客户端信息
};

INLINE CLoginServer::CLoginServer(void)
: m_nStatus(STATUSC_NONE)
, m_bCenterCnnted(false)
, m_bCenterLinked(false)
, m_bLoginDBCnnted(false)
, m_bLoginDBLinked(false)
, m_pUIHandler(nullptr)
, m_pConfig(nullptr)
, m_krConnectCenter(nullptr)
, m_krConnectLoginDB(nullptr)
, m_krUDPService(nullptr)
, m_krTCPService(nullptr)
, m_pShareCenterSvr(nullptr)
, m_pShareLoginDBSvr(nullptr)
, m_pShareGameSvr(nullptr)
, m_FileLog(true, LOGL_ALL, nullptr, CServerConfig::LoginServer)
{
}

INLINE CLoginServer::~CLoginServer(void)
{
}

INLINE CLoginServer::CLoginServer(const CLoginServer&)
: m_nStatus(STATUSC_NONE)
, m_bCenterCnnted(false)
, m_bCenterLinked(false)
, m_bLoginDBCnnted(false)
, m_bLoginDBLinked(false)
, m_pUIHandler(nullptr)
, m_pConfig(nullptr)
, m_krConnectCenter(nullptr)
, m_krConnectLoginDB(nullptr)
, m_krUDPService(nullptr)
, m_krTCPService(nullptr)
, m_pShareCenterSvr(nullptr)
, m_pShareLoginDBSvr(nullptr)
, m_pShareGameSvr(nullptr)
, m_FileLog(true, LOGL_ALL, nullptr, CServerConfig::LoginServer)
{
}

INLINE CLoginServer& CLoginServer::operator=(const CLoginServer&)
{
	return (*this);
}

INLINE bool CLoginServer::OnShareRoutine(Int, uintptr_t, LLong, CEventQueue::EVENT_TYPE)
{
	return false;
}

#endif // __LOGIN_SERVER_IMP_H__
