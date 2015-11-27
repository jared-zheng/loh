//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : SelectServerImp.h                            //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : 选择服务器实现                                     //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __SELECT_SERVER_IMP_H__
#define __SELECT_SERVER_IMP_H__

#pragma once

#include "selectpacket.h"
#include "servermap.h"
#include "serversort.h"
#include "ServerConfig.h"
#include "CommonServer.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///选择服务器实现
/// - 主要任务 :
/// -# 按规则选取登陆服务器地址通过UDP发送给准备登陆的客户端
/// -# 事务简单, 全部使用网络线程和定时线程来处理
/// -# TODO!!! /// @todo 黑名单校验未完成
///    a. 从选择服务器扩展配置文件中读取黑名单列表
///    b. 增加按错误屏蔽客户端的信息表, 将恶意攻击或者多次连接的加入表中, 按严重程度累加屏蔽时间
///    c. 界面也需要增加屏蔽客户端的信息表的显示及操作功能
///    d. 短时间(例如小于200MS的连续3次)频繁操作的, 拉入黑名单进行屏蔽
///    e. 存放永久黑名单+定时黑名单(例如1小时清理一次的, 加入最小时间为2小时)
///    f. 临时地址map缓存增长以10000000(1千万, 64位直接耗600M)为基数, 定时清除(例如2小时清理一次的)
class CSelectServer : public ICommonServer, public CPAKHandler
{
public:
	CSelectServer(void);
	virtual ~CSelectServer(void);
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
	virtual bool   OnShareRoutine(Int nEvent, CEventBase& EventRef, LLong llParam = 0, CEventQueue::EVENT_TYPE eType = CEventQueue::EVENT_TYPE_REFCOUNT) OVERRIDE;//
	virtual bool   OnShareRoutine(Int nEvent, CStream& Stream, LLong llParam = 0) OVERRIDE;
	// CPAKHandler
	virtual bool   OnTcpDispatch(const PacketPtr& PktPtr, PTCP_PARAM pTcp) OVERRIDE;
	virtual bool   OnUdpDispatch(const PacketPtr& PktPtr, PUDP_PARAM pUdp) OVERRIDE;
	virtual bool   OnTcpAccept(KeyRef krAccept, KeyRef krListen) OVERRIDE;//
	virtual bool   OnTcpConnect(UInt uError, KeyRef krConnect) OVERRIDE;
	virtual bool   OnTcpClose(KeyRef krSocket, LLong llLiveData) OVERRIDE;
	virtual bool   OnUdpClose(KeyRef krSocket, LLong llLiveData) OVERRIDE;
private:
	CSelectServer(const CSelectServer&);
	CSelectServer& operator=(const CSelectServer&);
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
	/// 创建UDP监听客户端对象
	bool  StartUDPService(void);

	/// 停止连接中心服务器
	void  StopConnectCenterServer(void);
	/// 停止UDP监听客户端
	void  StopUDPService(void);
	/// 向中心服务器注册
	void  LinkCenterServer(void);
	/// 向中心服务器注销
	void  UnlinkCenterServer(void);

	/// 黑名单列表检测
	bool  CheckAddrBlacklist(NET_ADDR& ClientAddr);

	/// 定时检测连接中心服务器对象是否有效
	bool  CheckConnectCenterServer(void);
	/// 定时检测UDP监听客户端对象是否有效
	bool  CheckUDPService(void);

	/// 同步服务器信息给界面
	bool  SyncServerInfo(void);
	/// 同步登陆服务器负载情况并同步给界面
	bool  SyncLoginSortInfo(CStream& Stream);
private:
	Int              m_nStatus;         ///< 服务器状态
	bool             m_bCenterCnnted;   ///< 是否连接中心服务器完成
	bool             m_bCenterLinked;   ///< 是否向中心服务器注册完成
	CEventHandler*   m_pUIHandler;      ///< 界面回调接口
	CServerConfig*   m_pConfig;         ///< 配置对象
	KeyRef           m_krConnectCenter; ///< 内网, 连接中心服务器
	KeyRef           m_krUDPService;    ///< 外网, 监听客户端
	ICommonServer*   m_pShareCenterSvr; ///< 同进程中心服务器
	CFileLog         m_FileLog;         ///< 简单文本日志
	CNetworkPtr      m_NetworkPtr;      ///< 网络对象
	SELECT_ADDR      m_ServerInfo;      ///< 本服务器信息
	SERVER_INFO      m_LoginInfo;       ///< 登陆服务器组统计数据
	RESULT_CHAIN     m_SortChain;       ///< 登陆服务器排序结果
	SVR_LOGIN_MAP    m_LoginSvrMap;     ///< 登陆服务器信息   
};

INLINE CSelectServer::CSelectServer(void)
: m_nStatus(STATUSC_NONE)
, m_bCenterCnnted(false)
, m_bCenterLinked(false)
, m_pUIHandler(nullptr)
, m_pConfig(nullptr)
, m_krConnectCenter(nullptr)
, m_krUDPService(nullptr)
, m_pShareCenterSvr(nullptr)
, m_FileLog(true, LOGL_ALL, nullptr, CServerConfig::SelectServer)
{
}

INLINE CSelectServer::~CSelectServer(void)
{
}

INLINE CSelectServer::CSelectServer(const CSelectServer&)
: m_nStatus(STATUSC_NONE)
, m_bCenterCnnted(false)
, m_bCenterLinked(false)
, m_pUIHandler(nullptr)
, m_pConfig(nullptr)
, m_krConnectCenter(nullptr)
, m_krUDPService(nullptr)
, m_pShareCenterSvr(nullptr)
, m_FileLog(true, LOGL_ALL, nullptr, CServerConfig::SelectServer)
{
}

INLINE CSelectServer& CSelectServer::operator=(const CSelectServer&)
{
	return (*this);
}

INLINE bool CSelectServer::OnShareRoutine(Int, uintptr_t, LLong, CEventQueue::EVENT_TYPE)
{
	return false;
}

INLINE bool CSelectServer::OnShareRoutine(Int, CEventBase&, LLong, CEventQueue::EVENT_TYPE)
{
	return false;
}

INLINE bool CSelectServer::OnTcpAccept(KeyRef, KeyRef)
{
	return false;
}
#endif // __SELECT_SERVER_IMP_H__
