//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Inline File : gatepacket.inl                               //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : 网关服务器信令                                     //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __GATE_PACKET_INL__
#define __GATE_PACKET_INL__

#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPAKGateQueueAck : 申请排队信息Ack
INLINE CPAKGateQueueAck::CPAKGateQueueAck(UInt uEvent, UInt uType)
: CPAKSessionAck(uEvent, uType)
, m_nParam(0)
{
}

INLINE CPAKGateQueueAck::~CPAKGateQueueAck(void)
{
}

INLINE CPAKGateQueueAck::CPAKGateQueueAck(const CPAKGateQueueAck& aSrc)
: CPAKSessionAck(aSrc)
, m_nParam(aSrc.m_nParam)
{
}

INLINE CPAKGateQueueAck& CPAKGateQueueAck::operator=(const CPAKGateQueueAck& aSrc)
{
	if (&aSrc != this)
	{
		CPAKSessionAck::operator=(aSrc);
		m_nParam = aSrc.m_nParam;
	}
	return (*this);
}

INLINE size_t CPAKGateQueueAck::Length(void)
{
	return (sizeof(Int) + CPAKSessionAck::Length());
}

INLINE void CPAKGateQueueAck::Serialize(CStream& Stream)
{
	CPAKSessionAck::Serialize(Stream);
	if (Stream.IsRead())
	{
		Stream >> m_nParam;
	}
	else
	{
		Stream << m_nParam;
	}
}

INLINE Int CPAKGateQueueAck::GetParam(void)
{
	return m_nParam;
}

INLINE void CPAKGateQueueAck::SetParam(Int nParam)
{
	m_nParam = nParam;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPAKGateSelect : 申请选择英雄角色信息
INLINE CPAKGateSelect::CPAKGateSelect(UInt uEvent, UInt uType)
: CPAKSession(uEvent, uType)
, m_llUserId(0)
, m_nGameId(0)
{
}

INLINE CPAKGateSelect::~CPAKGateSelect(void)
{
}

INLINE CPAKGateSelect::CPAKGateSelect(const CPAKGateSelect& aSrc)
: CPAKSession(aSrc)
, m_llUserId(aSrc.m_llUserId)
, m_nGameId(aSrc.m_nGameId)
{
}

INLINE CPAKGateSelect& CPAKGateSelect::operator=(const CPAKGateSelect& aSrc)
{
	if (&aSrc != this)
	{
		CPAKSession::operator=(aSrc);
		m_llUserId = aSrc.m_llUserId;
		m_nGameId  = aSrc.m_nGameId;
	}
	return (*this);
}

INLINE size_t CPAKGateSelect::Length(void)
{
	return (sizeof(LLong) + sizeof(Int) + CPAKSession::Length());
}

INLINE void CPAKGateSelect::Serialize(CStream& Stream)
{
	CPAKSession::Serialize(Stream);
	if (Stream.IsRead())
	{
		Stream >> m_llUserId >> m_nGameId;
	}
	else
	{
		Stream << m_llUserId << m_nGameId;
	}
}

INLINE LLong CPAKGateSelect::GetUserId(void)
{
	return m_llUserId;
}

INLINE void CPAKGateSelect::SetUserId(LLong llUserId)
{
	m_llUserId = llUserId;
}

INLINE Int CPAKGateSelect::GetGameId(void)
{
	return m_nGameId;
}

INLINE void CPAKGateSelect::SetGameId(Int nGameId)
{
	m_nGameId = nGameId;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPAKGateSelectAck : 申请选择英雄角色信息Ack
INLINE CPAKGateSelectAck::CPAKGateSelectAck(UInt uEvent, UInt uType)
: CPAKSessionAck(uEvent, uType)
, m_uRoleCount(0)
, m_pStream(nullptr)
{
}

INLINE CPAKGateSelectAck::~CPAKGateSelectAck(void)
{
}

INLINE CPAKGateSelectAck::CPAKGateSelectAck(const CPAKGateSelectAck& aSrc)
: CPAKSessionAck(aSrc)
, m_uRoleCount(aSrc.m_uRoleCount)
, m_pStream(aSrc.m_pStream)
{
}

INLINE CPAKGateSelectAck& CPAKGateSelectAck::operator=(const CPAKGateSelectAck& aSrc)
{
	if (&aSrc != this)
	{
		CPAKSessionAck::operator=(aSrc);
		m_uRoleCount = aSrc.m_uRoleCount;
		m_pStream    = aSrc.m_pStream;
	}
	return (*this);
}

INLINE size_t CPAKGateSelectAck::Length(void)
{
	return (sizeof(UInt) + CPAKSessionAck::Length());
}

INLINE void CPAKGateSelectAck::Serialize(CStream& Stream)
{
	CPAKSessionAck::Serialize(Stream);
	if (Stream.IsRead())
	{
		Stream >> m_uRoleCount;
		m_pStream = &Stream;
	}
	else
	{
		Stream << m_uRoleCount;
	}
}

INLINE UInt CPAKGateSelectAck::GetRoleCount(void)
{
	return m_uRoleCount;
}

INLINE void CPAKGateSelectAck::SetRoleCount(UInt uCount)
{
	m_uRoleCount = uCount;
}

INLINE bool CPAKGateSelectAck::CheckStream(void)
{
	return (m_pStream != nullptr);
}

INLINE CStream& CPAKGateSelectAck::GetStream(void)
{
	return (*m_pStream);
}

INLINE void CPAKGateSelectAck::SetStream(CStream& Stream)
{
	m_pStream = &Stream;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPAKGatePlay : 申请进入游戏世界信息
INLINE CPAKGatePlay::CPAKGatePlay(UInt uEvent, UInt uType)
: CPAKSession(uEvent, uType)
, m_nRoleId(0)
{
}

INLINE CPAKGatePlay::~CPAKGatePlay(void)
{
}

INLINE CPAKGatePlay::CPAKGatePlay(const CPAKGatePlay& aSrc)
: CPAKSession(aSrc)
, m_nRoleId(aSrc.m_nRoleId)
{
}

INLINE CPAKGatePlay& CPAKGatePlay::operator=(const CPAKGatePlay& aSrc)
{
	if (&aSrc != this)
	{
		CPAKSession::operator=(aSrc);
		m_nRoleId  = aSrc.m_nRoleId;
	}
	return (*this);
}

INLINE size_t CPAKGatePlay::Length(void)
{
	return (sizeof(LLong) + sizeof(Int) + CPAKSession::Length());
}

INLINE void CPAKGatePlay::Serialize(CStream& Stream)
{
	CPAKSession::Serialize(Stream);
	if (Stream.IsRead())
	{
		Stream >> m_nRoleId;
	}
	else
	{
		Stream  << m_nRoleId;
	}
}

INLINE Int CPAKGatePlay::GetRoleId(void)
{
	return m_nRoleId;
}

INLINE void CPAKGatePlay::SetRoleId(Int nRoleId)
{
	m_nRoleId = nRoleId;
}

#endif // __GATE_PACKET_INL__