//////////////////////////////////////////////////////////////////
//   The Legend of Heros                                        //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Inline File : loginpacket.inl                              //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : µÇÂ½·þÎñÆ÷ÐÅÁî                                     //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __LOGIN_PACKET_INL__
#define __LOGIN_PACKET_INL__

#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPAKLoginLink : ÕËºÅµÇÂ½
INLINE CPAKLoginLink::CPAKLoginLink(UInt uEvent, UInt uType)
: CPAKSession(uEvent, uType)
{
}

INLINE CPAKLoginLink::~CPAKLoginLink(void)
{
}

INLINE CPAKLoginLink::CPAKLoginLink(const CPAKLoginLink& aSrc)
: CPAKSession(aSrc)
, m_strUserId(aSrc.m_strUserId)
, m_strPassword(aSrc.m_strPassword)
{
}

INLINE CPAKLoginLink& CPAKLoginLink::operator=(const CPAKLoginLink& aSrc)
{
	if (&aSrc != this) {
		CPAKSession::operator=(aSrc);
		m_strUserId   = aSrc.m_strUserId;
		m_strPassword = aSrc.m_strPassword;
	}
	return (*this);
}

INLINE size_t CPAKLoginLink::Length(void)
{
	return (m_strUserId.Length(true) + m_strPassword.Length(true) + CPAKSession::Length());
}

INLINE void CPAKLoginLink::Serialize(CStream& Stream)
{
	CPAKSession::Serialize(Stream);
	if (Stream.IsRead()) {
		Stream >> m_strUserId >> m_strPassword;
	}
	else {
		Stream << m_strUserId << m_strPassword;
	}
}

INLINE CString& CPAKLoginLink::GetUserId(void)
{
	return m_strUserId;
}

INLINE void CPAKLoginLink::SetUserId(CString& strUserId)
{
	m_strUserId = strUserId;
}

INLINE void CPAKLoginLink::SetUserId(PCXStr pszUserId)
{
	m_strUserId = pszUserId;
}

INLINE CString& CPAKLoginLink::GetPassword(void)
{
	return m_strPassword;
}

INLINE void CPAKLoginLink::SetPassword(CString& strPassword)
{
	m_strPassword = strPassword;
}

INLINE void CPAKLoginLink::SetPassword(PCXStr pszPassword)
{
	m_strPassword = pszPassword;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPAKLoginLinkAck : ÕËºÅµÇÂ½Ack
INLINE CPAKLoginLinkAck::CPAKLoginLinkAck(UInt uEvent, UInt uType)
: CPAKSessionAck(uEvent, uType)
, m_llUserId(0)
, m_llTime(0)
, m_pStream(nullptr)
, m_nGameId(0)
{
}

INLINE CPAKLoginLinkAck::~CPAKLoginLinkAck(void)
{
}

INLINE CPAKLoginLinkAck::CPAKLoginLinkAck(const CPAKLoginLinkAck& aSrc)
: CPAKSessionAck(aSrc)
, m_llUserId(aSrc.m_llUserId)
, m_llTime(aSrc.m_llTime)
, m_pStream(aSrc.m_pStream)
, m_nGameId(aSrc.m_nGameId)
{
	m_Addr = aSrc.m_Addr;
}

INLINE CPAKLoginLinkAck& CPAKLoginLinkAck::operator=(const CPAKLoginLinkAck& aSrc)
{
	if (&aSrc != this) {
		CPAKSessionAck::operator=(aSrc);
		m_llUserId   = aSrc.m_llUserId;
		m_llTime     = aSrc.m_llTime;
		m_pStream    = aSrc.m_pStream;
		m_nGameId    = aSrc.m_nGameId;
		m_Addr       = aSrc.m_Addr;
	}
	return (*this);
}

INLINE size_t CPAKLoginLinkAck::Length(void)
{
	return (sizeof(LLong) + sizeof(LLong) + sizeof(Int) + sizeof(CNETTraits::NET_ADDR) + CPAKSessionAck::Length());
}

INLINE void CPAKLoginLinkAck::Serialize(CStream& Stream)
{
	CPAKSessionAck::Serialize(Stream);
	if (Stream.IsRead()) {
		Stream >> m_llUserId >> m_llTime >> m_nGameId;
		m_pStream = &Stream;
		Stream.Read(&m_Addr, sizeof(CNETTraits::NET_ADDR));
	}
	else {
		Stream << m_llUserId << m_llTime << m_nGameId;
		Stream.Write(&m_Addr, sizeof(CNETTraits::NET_ADDR));
	}
}

INLINE LLong CPAKLoginLinkAck::GetUserId(void)
{
	return m_llUserId;
}

INLINE void CPAKLoginLinkAck::SetUserId(LLong llUserId)
{
	m_llUserId = llUserId;
}

INLINE LLong CPAKLoginLinkAck::GetTime(void)
{
	return m_llTime;
}

INLINE void CPAKLoginLinkAck::SetTime(LLong llTime)
{
	m_llTime = llTime;
}

INLINE Int CPAKLoginLinkAck::GetGameId(void)
{
	return m_nGameId;
}

INLINE void CPAKLoginLinkAck::SetGameId(Int nGameId)
{
	m_nGameId = nGameId;
}

INLINE bool CPAKLoginLinkAck::CheckStream(void)
{
	return (m_pStream != nullptr);
}

INLINE CStream& CPAKLoginLinkAck::GetStream(void)
{
	return (*m_pStream);
}

INLINE void CPAKLoginLinkAck::SetStream(CStream& Stream)
{
	m_pStream = &Stream;
}

INLINE CNETTraits::NET_ADDR& CPAKLoginLinkAck::GetCltLast(void)
{
	return m_Addr;
}

INLINE void CPAKLoginLinkAck::SetCltLast(CNETTraits::NET_ADDR& CltLast)
{
	m_Addr = CltLast;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPAKLoginUnlink : ÕËºÅµÇ³ö
INLINE CPAKLoginUnlink::CPAKLoginUnlink(UInt uEvent, UInt uType)
: CPAKSession(uEvent, uType)
, m_llUserId(0)
, m_llTime(0)
, m_nGameId(0)
{
}

INLINE CPAKLoginUnlink::~CPAKLoginUnlink(void)
{
}

INLINE CPAKLoginUnlink::CPAKLoginUnlink(const CPAKLoginUnlink& aSrc)
: CPAKSession(aSrc)
, m_llUserId(aSrc.m_llUserId)
, m_llTime(aSrc.m_llTime)
, m_nGameId(aSrc.m_nGameId)
{
	m_Addr = aSrc.m_Addr;
}

INLINE CPAKLoginUnlink& CPAKLoginUnlink::operator=(const CPAKLoginUnlink& aSrc)
{
	if (&aSrc != this) {
		CPAKSession::operator=(aSrc);
		m_llUserId = aSrc.m_llUserId;
		m_llTime   = aSrc.m_llTime;
		m_nGameId  = aSrc.m_nGameId;
		m_Addr     = aSrc.m_Addr;
	}
	return (*this);
}

INLINE size_t CPAKLoginUnlink::Length(void)
{
	return (sizeof(LLong) + sizeof(LLong) + sizeof(Int) + sizeof(CNETTraits::NET_ADDR) + CPAKSession::Length());
}

INLINE void CPAKLoginUnlink::Serialize(CStream& Stream)
{
	CPAKSession::Serialize(Stream);
	if (Stream.IsRead()) {
		Stream >> m_llUserId >> m_llTime >> m_nGameId;
		Stream.Read(&m_Addr, sizeof(CNETTraits::NET_ADDR));
	}
	else {
		Stream << m_llUserId << m_llTime << m_nGameId;
		Stream.Write(&m_Addr, sizeof(CNETTraits::NET_ADDR));
	}
}

INLINE LLong CPAKLoginUnlink::GetUserId(void)
{
	return m_llUserId;
}

INLINE void CPAKLoginUnlink::SetUserId(LLong llUserId)
{
	m_llUserId = llUserId;
}

INLINE LLong CPAKLoginUnlink::GetTime(void)
{
	return m_llTime;
}

INLINE void CPAKLoginUnlink::SetTime(LLong llTime)
{
	m_llTime = llTime;
}

INLINE Int CPAKLoginUnlink::GetGameId(void)
{
	return m_nGameId;
}

INLINE void CPAKLoginUnlink::SetGameId(Int nGameId)
{
	m_nGameId = nGameId;
}

INLINE CNETTraits::NET_ADDR& CPAKLoginUnlink::GetCltLast(void)
{
	return m_Addr;
}

INLINE void CPAKLoginUnlink::SetCltLast(CNETTraits::NET_ADDR& CltLast)
{
	m_Addr = CltLast;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPAKLoginSelectGame : Ñ¡ÔñÓÎÏ··þÎñÆ÷
INLINE CPAKLoginSelectGame::CPAKLoginSelectGame(UInt uEvent, UInt uType)
: CPAKSessionAck(uEvent, uType)
, m_llAuthCode(0)
{
}

INLINE CPAKLoginSelectGame::~CPAKLoginSelectGame(void)
{
}

INLINE CPAKLoginSelectGame::CPAKLoginSelectGame(const CPAKLoginSelectGame& aSrc)
: CPAKSessionAck(aSrc)
, m_llAuthCode(aSrc.m_llAuthCode)
{
	m_Addr = aSrc.m_Addr;
}

INLINE CPAKLoginSelectGame& CPAKLoginSelectGame::operator=(const CPAKLoginSelectGame& aSrc)
{
	if (&aSrc != this) {
		CPAKSessionAck::operator=(aSrc);
		m_llAuthCode = aSrc.m_llAuthCode;
		m_Addr       = aSrc.m_Addr;
	}
	return (*this);
}

INLINE size_t CPAKLoginSelectGame::Length(void)
{
	return (sizeof(LLong) + sizeof(CNETTraits::NET_ADDR) + CPAKSessionAck::Length());
}

INLINE void CPAKLoginSelectGame::Serialize(CStream& Stream)
{
	CPAKSessionAck::Serialize(Stream);
	if (Stream.IsRead()) {
		Stream >> m_llAuthCode;
		Stream.Read(&m_Addr, sizeof(CNETTraits::NET_ADDR));
	}
	else {
		Stream << m_llAuthCode;
		Stream.Write(&m_Addr, sizeof(CNETTraits::NET_ADDR));
	}
}

INLINE LLong CPAKLoginSelectGame::GetAuthCode(void)
{
	return m_llAuthCode;
}

INLINE void CPAKLoginSelectGame::SetAuthCode(LLong llAuthCode)
{
	m_llAuthCode = llAuthCode;
}

INLINE CNETTraits::NET_ADDR& CPAKLoginSelectGame::GetAddr(void)
{
	return m_Addr;
}

INLINE void CPAKLoginSelectGame::SetAddr(CNETTraits::NET_ADDR& SelGate)
{
	m_Addr = SelGate;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPAKLoginLinkGame : ÑéÖ¤ÓÎÏ··þÎñÆ÷
INLINE CPAKLoginLinkGame::CPAKLoginLinkGame(UInt uEvent, UInt uType)
: CPAKSessionAck(uEvent, uType)
, m_llAuthCode(0)
{
}

INLINE CPAKLoginLinkGame::~CPAKLoginLinkGame(void)
{
}

INLINE CPAKLoginLinkGame::CPAKLoginLinkGame(const CPAKLoginLinkGame& aSrc)
: CPAKSessionAck(aSrc)
, m_llAuthCode(aSrc.m_llAuthCode)
{
}

INLINE CPAKLoginLinkGame& CPAKLoginLinkGame::operator=(const CPAKLoginLinkGame& aSrc)
{
	if (&aSrc != this) {
		CPAKSessionAck::operator=(aSrc);
		m_llAuthCode = aSrc.m_llAuthCode;
	}
	return (*this);
}

INLINE size_t CPAKLoginLinkGame::Length(void)
{
	return (sizeof(LLong) + CPAKSessionAck::Length());
}

INLINE void CPAKLoginLinkGame::Serialize(CStream& Stream)
{
	CPAKSessionAck::Serialize(Stream);
	if (Stream.IsRead()) {
		Stream >> m_llAuthCode;
	}
	else {
		Stream << m_llAuthCode;
	}
}

INLINE LLong CPAKLoginLinkGame::GetAuthCode(void)
{
	return m_llAuthCode;
}

INLINE void CPAKLoginLinkGame::SetAuthCode(LLong llAuthCode)
{
	m_llAuthCode = llAuthCode;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//

#endif // __LOGIN_PACKET_INL__