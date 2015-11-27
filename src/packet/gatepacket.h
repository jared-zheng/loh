//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : gatepacket.h                                 //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : 网关服务器信令                                     //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __GATE_PACKET_H__
#define __GATE_PACKET_H__

#pragma once

#include "commonpacket.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
enum GATE_ERROR
{
	GATE_ERROR_SESSION = (DATAD_FAIL + 1),  // 会话错误或者状态错误
	GATE_ERROR_AUTHCODE1,                   // 验证码1校验失败
	GATE_ERROR_AUTHCODE2,                   // 验证码2校验失败
	GATE_ERROR_USERID,                      // 用户id错误
	GATE_ERROR_QUEUE,                       // 排队查询失败
	GATE_ERROR_GAME,                        // 游戏中状态错误, 后面参数会标识是何种状态
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// 网关服务器信令定义
enum PAK_EVENT_GATE
{
	PAK_EVENT_GATE_QUEUE = (PAK_EVENT_GATE_BEGIN + 1), // 申请排队信息, CPAKSession
	PAK_EVENT_GATE_QUEUEACK,                           // 申请排队信息Ack, CPAKGateQueueAck or CPAKAck(error)

	PAK_EVENT_GATE_SELECT,                             // 申请选择角色信息, CPAKGateSelect
	PAK_EVENT_GATE_SELECTACK,                          // 申请选择角色信息Ack, CPAKGateSelectAck or CPAKAck(error)

	PAK_EVENT_GATE_PLAY,                               // 申请选择的角色进入游戏, CPAKGatePlay
	PAK_EVENT_GATE_PLAYACK,                            // 申请选择的角色进入游戏Ack, CPAKSessionAck or CPAKAck(error)

	PAK_EVENT_GATE_CREATE,                             // 申请创建角色
	PAK_EVENT_GATE_CREATEACK,                          // 申请创建角色Ack or CPAKAck(error)

	PAK_EVENT_GATE_DELETE,                             // 申请删除角色
	PAK_EVENT_GATE_DELETEACK,                          // 申请删除角色Ack or CPAKAck(error)

	PAK_EVENT_GATE_RECOVER,                            // 申请恢复角色
	PAK_EVENT_GATE_RECOVERACK,                         // 申请恢复角色Ack or CPAKAck(error)
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 申请排队信息Ack
class CPAKGateQueueAck : public CPAKSessionAck
{
public:
	CPAKGateQueueAck(UInt uEvent = PAK_EVENT_GATE_QUEUEACK, UInt uType = PAK_TYPE_GATE);
	virtual ~CPAKGateQueueAck(void);

	CPAKGateQueueAck(const CPAKGateQueueAck& aSrc);
	CPAKGateQueueAck& operator=(const CPAKGateQueueAck& aSrc);

	virtual size_t Length(void) OVERRIDE;
	virtual void   Serialize(CStream&) OVERRIDE;

	Int    GetParam(void);
	void   SetParam(Int nParam);
private:
	///< 查询成功并且返回的m_nParam值为0, 表示直接可以进入游戏; 大于0表示排队中
	///< 查询错误ACK=GATE_ERROR_GAME, 表示当前已经在游戏状态, 不需要排队, m_nParam值为具体的游戏状态
	Int    m_nParam; 
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 申请选择角色信息
class CPAKGateSelect : public CPAKSession
{
public:
	CPAKGateSelect(UInt uEvent = PAK_EVENT_GATE_SELECT, UInt uType = PAK_TYPE_GATE);
	virtual ~CPAKGateSelect(void);

	CPAKGateSelect(const CPAKGateSelect& aSrc);
	CPAKGateSelect& operator=(const CPAKGateSelect& aSrc);

	virtual size_t Length(void) OVERRIDE;
	virtual void   Serialize(CStream&) OVERRIDE;

	LLong          GetUserId(void);
	void           SetUserId(LLong llUserId);

	Int            GetGameId(void);
	void           SetGameId(Int nGameId);
private:
	LLong          m_llUserId; ///< 用户Id
	Int            m_nGameId;  ///< 游戏服务器Id, 游戏DB服务器可以管理多个游戏的数据, 查询时需要提供哪个游戏服务器(区)的数据
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 申请选择角色信息Ack
class CPAKGateSelectAck : public CPAKSessionAck 
{
public:
	CPAKGateSelectAck(UInt uEvent = PAK_EVENT_GATE_SELECTACK, UInt uType = PAK_TYPE_GATE);
	virtual ~CPAKGateSelectAck(void);

	CPAKGateSelectAck(const CPAKGateSelectAck& aSrc);
	CPAKGateSelectAck& operator=(const CPAKGateSelectAck& aSrc);

	virtual size_t Length(void) OVERRIDE;
	virtual void   Serialize(CStream&) OVERRIDE;

	UInt       GetRoleCount(void);
	void       SetRoleCount(UInt uCount);

	bool       CheckStream(void);
	CStream&   GetStream(void);
	void       SetStream(CStream& Stream);
private:
	UInt       m_uRoleCount; ///< 角色个数
	CStream*   m_pStream;    ///< 角色信息数据流
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 申请进入游戏世界信息
class CPAKGatePlay : public CPAKSession
{
public:
	CPAKGatePlay(UInt uEvent = PAK_EVENT_GATE_PLAY, UInt uType = PAK_TYPE_GATE);
	virtual ~CPAKGatePlay(void);

	CPAKGatePlay(const CPAKGatePlay& aSrc);
	CPAKGatePlay& operator=(const CPAKGatePlay& aSrc);

	virtual size_t Length(void) OVERRIDE;
	virtual void   Serialize(CStream&) OVERRIDE;

	Int            GetRoleId(void);
	void           SetRoleId(Int nRoleId);
private:
	Int            m_nRoleId; ///< 角色Id
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "gatepacket.inl"

#endif // __GATE_PACKET_H__