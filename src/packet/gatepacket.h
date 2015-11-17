//////////////////////////////////////////////////////////////////
//   The Legend of Heros                                        //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : gatepacket.h                                 //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
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
	GATE_ERROR_SESSION = (DATAD_FAIL + 1),  // session错误或者状态错误
	GATE_ERROR_AUTHCODE1,                   // 验证码1校验失败
	GATE_ERROR_AUTHCODE2,                   // 验证码2校验失败
	GATE_ERROR_USERID,                      // 用户id错误
	GATE_ERROR_QUEUE,                       // 排队查询失败
	GATE_ERROR_GAME,                        // 游戏中状态错误, 后面参数会标识是何种状态
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// gate信令定义
enum PAK_EVENT_GATE
{
	PAK_EVENT_GATE_QUEUE = (PAK_EVENT_GATE_BEGIN + 1), // 申请排队信息, CPAKSession
	PAK_EVENT_GATE_QUEUEACK,                           // 申请排队信息Ack, CPAKGateQueueAck or CPAKAck(error)

	PAK_EVENT_GATE_SELECT,                             // 申请选择角色信息, CPAKGateSelect
	PAK_EVENT_GATE_SELECTACK,                          // 申请选择角色信息Ack, CPAKGateSelectAck or CPAKAck(error)

	PAK_EVENT_GATE_PLAY,                               // 申请选择的角色进入游戏, CPAKGatePlay
	PAK_EVENT_GATE_PLAYACK,                            // 申请选择的角色进入游戏Ack, CPAKSessionAck or CPAKAck(error)
	// TODO!!!角色操作是客户端需要表明是否是排队中还是游戏中的
	PAK_EVENT_GATE_CREATE,                             // 申请创建角色
	PAK_EVENT_GATE_CREATEACK,                          // 申请创建角色Ack or CPAKAck(error)

	PAK_EVENT_GATE_DELETE,                             // 申请删除角色
	PAK_EVENT_GATE_DELETEACK,                          // 申请删除角色Ack or CPAKAck(error)

	PAK_EVENT_GATE_RECOVER,                            // 申请恢复角色
	PAK_EVENT_GATE_RECOVERACK,                         // 申请恢复角色Ack or CPAKAck(error)
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPAKGateQueueAck : 申请排队信息Ack
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
	Int    m_nParam; // ack=0, m_nParam=0表示可以进入游戏了
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPAKGateSelect : 申请选择角色信息
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
private:
	LLong          m_llUserId;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPAKGateSelectAck : 申请选择角色信息Ack
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
	UInt       m_uRoleCount; // 角色个数
	CStream*   m_pStream;    // 角色信息
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPAKGatePlay : 申请进入游戏世界信息
class CPAKGatePlay : public CPAKSession
{
public:
	CPAKGatePlay(UInt uEvent = PAK_EVENT_GATE_PLAY, UInt uType = PAK_TYPE_GATE);
	virtual ~CPAKGatePlay(void);

	CPAKGatePlay(const CPAKGatePlay& aSrc);
	CPAKGatePlay& operator=(const CPAKGatePlay& aSrc);

	virtual size_t Length(void) OVERRIDE;
	virtual void   Serialize(CStream&) OVERRIDE;

	LLong          GetUserId(void);
	void           SetUserId(LLong llUserId);

	Int            GetRoleId(void);
	void           SetRoleId(Int nRoleId);
private:
	LLong          m_llUserId;
	Int            m_nRoleId;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "gatepacket.inl"

#endif // __GATE_PACKET_H__