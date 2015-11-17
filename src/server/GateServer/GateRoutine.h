//////////////////////////////////////////////////////////////////
//   The Legend of Heros                                        //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : GateRoutine.h                                //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : 网关服务器事务实现                                 //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __GATE_ROUTINE_H__
#define __GATE_ROUTINE_H__

#pragma once

#include "CommonRoutine.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CGateRoutine
class CGateRoutine : public ICommonRoutine
{
	DECLARE_RTTI_CREATE( CGateRoutine )
public:
	enum SESSION_LIMIT
	{
		SESSION_LIMIT_QUEUE  = 128,
		SESSION_LIMIT_GAME   = 128,
	};
public:
	CGateRoutine(void);
	virtual ~CGateRoutine(void);
	// CEventHandler
	virtual bool OnHandle(Int nEvent, uintptr_t utParam, LLong llParam) OVERRIDE;
	virtual bool OnHandle(Int nEvent, CEventBase&, LLong) OVERRIDE;
	virtual bool OnHandle(Int nEvent, CStream& Stream, LLong llParam) OVERRIDE;//
	virtual bool OnHandle(Int nEvent, UInt uCount) OVERRIDE;//
	// ICommonRoutine
	virtual bool Start(class ICommonServer* pServer) OVERRIDE;
	virtual bool Pause(bool bPause = true) OVERRIDE;
	virtual bool Update(void) OVERRIDE;
	virtual void Stop(void) OVERRIDE;

	virtual bool Add(Int nEvent, uintptr_t utParam = 0, LLong llParam = 0, CEventQueue::EVENT_TYPE eType = CEventQueue::EVENT_TYPE_NONE) OVERRIDE;//
	virtual bool Add(Int nEvent, CEventBase& EventRef, LLong llParam = 0, CEventQueue::EVENT_TYPE eType = CEventQueue::EVENT_TYPE_REFCOUNT) OVERRIDE;
	virtual bool Add(Int nEvent, CStream& Stream, LLong llParam = 0) OVERRIDE;//
private:
	// 黑名单列表检测
	bool  CheckAddrBlacklist(CNETTraits::NET_ADDR& ClientAddr);

	bool  CheckSessionTimeout(void);

	void SelectGame(CPAKLoginSelectGame* pSelect, LLong llParam);
	void LinkGame(CPAKLoginLinkGame* pLink, KeyRef krSocket);
	// 登陆游戏服务器第一次校验
	void LinkGame1(LLong llDataRef, CPAKLoginLinkGame* pLink, KeyRef krSocket);
	// 登陆游戏服务器第二次校验
	void LinkGame2(LLong llDataRef, CPAKLoginLinkGame* pLink, KeyRef krSocket);

	void QueueRank(CPAKSession* pQueue, KeyRef krSocket);
	void SelectRole(CPAKGateSelect* pSelect, KeyRef krSocket);
	void PlayRole(CPAKGatePlay* pPlay, KeyRef krSocket);

	void SelectRoleAck(CPAKSessionAck* pAck);
	void PlayRoleAck(CPAKSessionAck* pAck);
private:
	// 客户端登陆流程
	// 1. 接受到登陆服务器UDP转发的客户端(user-id)登陆请求
	// 2. 如果登陆队列+排队队列没有到达人数上限, UDP回复登陆服务器可以登录本网关(登陆队列维持GATE_TIME_SELECT_GAME, 没有在时间内登陆将被删除)
	// 3. 客户端收到登陆服务器的登陆网关地址以及登陆验证码1, 链接网关服务器进行验证码1校验
	// 4. 网关服务器验证码1校验通过, 产生验证码2通过客户端登陆服务器转发给客户端
	// 5. 客户端用user-id+验证码2再次进行校验, 通过校验的, 进入排队队列(如果游戏到达人数上限)或者游戏队列
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// SESSION_DATA : 网关客户端登陆或者角色选取排队信息结构定义
	typedef struct tagSESSION_DATA
	{
	public:
		enum SESSION_STATUS
		{
			SESSION_STATUS_NONE,
			//
			SESSION_STATUS_READY,
			SESSION_STATUS_QUEUE,
			SESSION_STATUS_GAME,
			// 角色选择
			SESSION_STATUS_SELECT,    // 客户端请求角色选择状态
			SESSION_STATUS_SELECTACK, // 表示服务器已经回应将角色数据发给了客户端
			// 进入游戏
			SESSION_STATUS_PLAY,      // 客户端请求进入游戏状态
			// TODO!!!注意:对于同个用户多重游戏的, 游戏服务器需要通知原先的连接并关闭连接, Game DB需要记录user-id登陆的gate-keyref
			SESSION_STATUS_PLAYACK,   // 表示服务器已经回应将进入游戏发给了客户端(游戏DB已经将数据发送给了游戏服务器)
			// 
			SESSION_STATUS_REALM,     // 进入游戏模式
		};

		enum SESSION_TIME
		{
			SESSION_TIME_SELECT_GAME = 1000,           // 进入登陆队列的最多等待时间, 超时需要从登陆服务器重新开始登陆游戏流程
			SESSION_TIME_SELECT_ROLE = 5 * 60 * 1000,  // 角色操作, 没有操作最多等待5分钟
			SESSION_TIME_REALM_IDLE  = 30 * 60 * 1000, // 进入游戏, 没有操作最多等待30分钟
		};
	public:
		tagSESSION_DATA(void) : llUserId(0), llTimeout(0), llOnline(0), index(nullptr), nData(0), nStatus(SESSION_STATUS_NONE), llLoginIndex(0), pLoginServer(nullptr) { }
		~tagSESSION_DATA(void) { }
	public:
		LLong                  llUserId;      // 
		LLong                  llTimeout;     //
		LLong                  llOnline;      // online time[_time_t]
		PINDEX                 index;         // 进入游戏排队索引号
		Int                    nData;         // 排队序号 or 角色Id
		Int                    nStatus;       // SESSION_STATUS, 表示角色信息是否已经发送给客户端
		LLong                  llLoginIndex;  // login index
		ICommonServer*         pLoginServer;  // 
		CNETTraits::NET_ADDR   AddrLogin;     // [ipv4 or ipv6]
	}SESSION_DATA, *PSESSION_DATA;
	typedef tagDATA_MAP<DataRef, SESSION_DATA>    SESSION_DATA_MAP, *PSESSION_DATA_MAP;
	typedef CTList<PINDEX>                        SESSION_RANK_LST, *PSESSION_RANK_LST;
	typedef CTMap<KeyRef, UInt>                   TIMEOUT_MAP,      *PTIMEOUT_MAP;
	typedef CTMap<KeyRef, UInt>::PAIR             TIMEOUT_MAP_PAIR;
private:
	class CGateServer*        m_pServer;
	// 1. 从网关服务器扩展配置文件中读取黑名单列表
	// 2. 增加按错误屏蔽客户端连接的信息表, 将恶意攻击或者多次连接的加入表中, 按严重程度累加屏蔽时间
	// 3. 界面也需要增加屏蔽客户端的信息表的显示及操作功能
	// 4. 设置网络层的ACK(1000MS)和Timeout(30000MS)属性
	// 5. 短时间(例如小于200MS的连续3次)频繁操作的, 拉入黑名单进行屏蔽
	// 6. 存放永久黑名单+定时黑名单(例如1小时清理一次的, 加入最小时间为2小时)
	// 7. 临时地址map缓存增长以10000000(1千万, 64位直接耗600M)为基数, 定时清除(例如2小时清理一次的)
	Int                       m_nQueueCur;        // 等待游戏最前面的序列号
	Int                       m_nQueueEnd;        // 等待游戏最后面的序列号
	Int                       m_nQueueLimit;      // 等待人数上限:登陆人数+排队人数
	Int                       m_nGameCur;         // 当前游戏人数
	Int                       m_nGameSelect;      // 游戏队列停留在角色操作的人数
	Int                       m_nGameLimit;       // 游戏人数上限
	SESSION_RANK_LST          m_QueueRank;        // 排队优先级
	SESSION_DATA_MAP          m_SessionQueue;     // 排队
	SESSION_DATA_MAP          m_SessionLogin;     // 登陆排队
	SESSION_DATA_MAP          m_SessionGame;      // 游戏

	CEventQueuePtr            m_EventQueuePtr;    // 事件队列 
};

INLINE CGateRoutine::CGateRoutine(void)
: m_pServer(nullptr)
, m_nQueueCur(0)
, m_nQueueEnd(0)
, m_nQueueLimit(SESSION_LIMIT_QUEUE)
, m_nGameCur(0)
, m_nGameSelect(0)
, m_nGameLimit(SESSION_LIMIT_GAME)
{
}

INLINE CGateRoutine::~CGateRoutine(void)
{
}

INLINE bool CGateRoutine::OnHandle(Int, CStream&, LLong)
{ 
	return false; 
}

INLINE bool CGateRoutine::OnHandle(Int, UInt)
{ 
	return false;
}

INLINE bool CGateRoutine::Add(Int, uintptr_t, LLong, CEventQueue::EVENT_TYPE)
{
	return false;
}

INLINE bool CGateRoutine::Add(Int, CStream&, LLong)
{
	return false;
}

#endif // __GATE_ROUTINE_H__
