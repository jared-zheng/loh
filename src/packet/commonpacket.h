//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : commonpacket.h                               //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : 通用信令包                                         //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __COMMON_PACKET_H__
#define __COMMON_PACKET_H__

#pragma once

#include "packet.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 信令定义
enum PAK_EVENT
{
	PAK_EVENT_LIVE          = (CNETTraits::EVENT_RESERVED + 1), // 心跳包, CPAKSimple<PAK_EVENT_LIVE, PAK_TYPE>
	PAK_EVENT_LIVEACK,      // 心跳包Ack, CPAKSimple<PAK_EVENT_LIVEACK, PAK_TYPE>

	PAK_EVENT_LINK,         // 注册, CPAKLink
	PAK_EVENT_LINKACK,      // 注册Ack, CPAKHead<PAK_EVENT_LINKACK, PAK_TYPE>

	PAK_EVENT_UPDATE,       // 更新, CPAKUpdate
	PAK_EVENT_UPDATEACK,    // 更新Ack, CPAKHead<PAK_EVENT_UPDATEACK, PAK_TYPE>

	PAK_EVENT_SYNC,         // 同步, CPAKSync
	PAK_EVENT_SYNCACK,      // 同步Ack, CPAKHead<PAK_EVENT_SYNCACK, PAK_TYPE>

	PAK_EVENT_UNLINK,       // 注销, CPAKHead<PAK_EVENT_UNLINK, PAK_TYPE>
	PAK_EVENT_UNLINKACK,    // 注销Ack, CPAKHead<PAK_EVENT_UNLINKACK, PAK_TYPE>
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 服务器信息数据包
template <UInt uEvent, UInt uType>
class CPAKServerData : public CPAKSimple<uEvent, uType>
{
public:
	CPAKServerData(void);
	virtual ~CPAKServerData(void);

	CPAKServerData(const CPAKServerData& aSrc);
	CPAKServerData& operator=(const CPAKServerData& aSrc);

	virtual size_t Length(void) OVERRIDE;
	virtual void   Serialize(CStream&) OVERRIDE;
	/// 检测数据流对象是否存在
	bool           CheckStream(void);
	/// 获取数据流对象
	CStream&       GetStream(void);
	/// 设置数据流对象
	void           SetStream(CStream& Stream);
	/// 获取服务器信息
	SERVER_DATA&   GetServerData(void);
	/// 设置服务器信息
	void           SetServerData(SERVER_DATA& ServerData);
private:
	CStream*       m_pStream;    ///< 本地数据, 网络传输序列化时忽略大小
	SERVER_DATA    m_ServerData; ///< 服务器信息
};

typedef CPAKServerData<PAK_EVENT_LINK, PAK_TYPE_NONE>       CPAKLink;
typedef CPAKServerData<PAK_EVENT_UPDATE, PAK_TYPE_NONE>     CPAKUpdate;
typedef CPAKServerData<PAK_EVENT_SYNC, PAK_TYPE_NONE>       CPAKSync;

typedef CPAKServerData<PAK_EVENT_SYNC, PAK_TYPE_CENTER>     CCenterSync;
typedef CPAKServerData<PAK_EVENT_SYNC, PAK_TYPE_LOGIN>      CLoginSync;

typedef CPAKServerData<PAK_EVENT_LINK, PAK_TYPE_SELECT>     CSelectLink;
typedef CPAKServerData<PAK_EVENT_UPDATE, PAK_TYPE_SELECT>   CSelectUpdate;

typedef CPAKServerData<PAK_EVENT_LINK, PAK_TYPE_LOGIN>      CLoginLink;
typedef CPAKServerData<PAK_EVENT_UPDATE, PAK_TYPE_LOGIN>    CLoginUpdate;

typedef CPAKServerData<PAK_EVENT_LINK, PAK_TYPE_GAME>       CGameLink;
typedef CPAKServerData<PAK_EVENT_UPDATE, PAK_TYPE_GAME>     CGameUpdate;

typedef CPAKServerData<PAK_EVENT_LINK, PAK_TYPE_ZONE>       CZoneLink;
typedef CPAKServerData<PAK_EVENT_UPDATE, PAK_TYPE_ZONE>     CZoneUpdate;

typedef CPAKServerData<PAK_EVENT_LINK, PAK_TYPE_GATE>       CGateLink;
typedef CPAKServerData<PAK_EVENT_UPDATE, PAK_TYPE_GATE>     CGateUpdate;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 包含Session的数据包
class CPAKSession : public CPAKHead
{
public:
	CPAKSession(UInt uEvent, UInt uType = PAK_TYPE_NONE);
	virtual ~CPAKSession(void);

	CPAKSession(const CPAKSession& aSrc);
	CPAKSession& operator=(const CPAKSession& aSrc);

	virtual size_t Length(void) OVERRIDE;
	virtual void   Serialize(CStream&) OVERRIDE;
	/// 获取Session值
	LLong          GetSessionId(void);
	/// 设置Session值
	void           SetSessionId(LLong llSessionId);
private:
	LLong          m_llSessionId;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 包含Session的ACK据包
class CPAKSessionAck : public CPAKAck
{
public:
	CPAKSessionAck(UInt uEvent, UInt uType = PAK_TYPE_NONE);
	virtual ~CPAKSessionAck(void);

	CPAKSessionAck(const CPAKSessionAck& aSrc);
	CPAKSessionAck& operator=(const CPAKSessionAck& aSrc);

	virtual size_t Length(void) OVERRIDE;
	virtual void   Serialize(CStream&) OVERRIDE;
	/// 获取Session值
	LLong          GetSessionId(void);
	/// 设置Session值
	void           SetSessionId(LLong llSessionId);
private:
	LLong          m_llSessionId;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "commonpacket.inl"

#endif // __COMMON_PACKET_H__