//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Inline File : commonpacket.inl                              //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : 通用信令包                                         //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __COMMON_PACKET_INL__
#define __COMMON_PACKET_INL__

#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPAKServerData
template <UInt uEvent, UInt uType>
INLINE CPAKServerData<uEvent, uType>::CPAKServerData(void)
: m_pStream(nullptr)
{
	memset(&m_ServerData, 0, sizeof(SERVER_DATA));
}

template <UInt uEvent, UInt uType>
INLINE CPAKServerData<uEvent, uType>::~CPAKServerData(void)
{
}

template <UInt uEvent, UInt uType>
INLINE CPAKServerData<uEvent, uType>::CPAKServerData(const CPAKServerData& aSrc)
: m_pStream(aSrc.m_pStream)
{
	m_ServerData = aSrc.m_ServerData;
}

template <UInt uEvent, UInt uType>
INLINE CPAKServerData<uEvent, uType>& CPAKServerData<uEvent, uType>::operator=(const CPAKServerData& aSrc)
{
	if (&aSrc != this) {
		CPAKSimple::operator=(aSrc);
		m_pStream    = aSrc.m_pStream;
		m_ServerData = aSrc.m_ServerData;
	}
	return (*this);
}

template <UInt uEvent, UInt uType>
INLINE size_t CPAKServerData<uEvent, uType>::Length(void)
{
	return (sizeof(SERVER_DATA) + CPAKSimple::Length());
}

template <UInt uEvent, UInt uType>
INLINE void CPAKServerData<uEvent, uType>::Serialize(CStream& Stream)
{
	CPAKHead::Serialize(Stream);
	if (Stream.IsRead()) {
		Stream.Read(&m_ServerData, sizeof(SERVER_DATA));
		m_pStream = &Stream;
	}
	else {
		Stream.Write(&m_ServerData, sizeof(SERVER_DATA));
	}
}

template <UInt uEvent, UInt uType>
INLINE bool CPAKServerData<uEvent, uType>::CheckStream(void)
{
	return (m_pStream != nullptr);
}

template <UInt uEvent, UInt uType>
INLINE CStream& CPAKServerData<uEvent, uType>::GetStream(void)
{
	return (*m_pStream);
}

template <UInt uEvent, UInt uType>
INLINE void CPAKServerData<uEvent, uType>::SetStream(CStream& Stream)
{
	m_pStream = &Stream;
}

template <UInt uEvent, UInt uType>
INLINE SERVER_DATA& CPAKServerData<uEvent, uType>::GetServerData(void)
{
	return m_ServerData;
}

template <UInt uEvent, UInt uType>
INLINE void CPAKServerData<uEvent, uType>::SetServerData(SERVER_DATA& ServerData)
{
	m_ServerData = ServerData;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPAKSession : Session
INLINE CPAKSession::CPAKSession(UInt uEvent, UInt uType)
: CPAKHead(uEvent, uType)
, m_llSessionId(0)
{
}

INLINE CPAKSession::~CPAKSession(void)
{
}

INLINE CPAKSession::CPAKSession(const CPAKSession& aSrc)
: CPAKHead(aSrc)
, m_llSessionId(aSrc.m_llSessionId)
{
}

INLINE CPAKSession& CPAKSession::operator=(const CPAKSession& aSrc)
{
	if (&aSrc != this) {
		CPAKHead::operator=(aSrc);
		m_llSessionId = aSrc.m_llSessionId;
	}
	return (*this);
}

INLINE size_t CPAKSession::Length(void)
{
	return (sizeof(LLong) + CPAKHead::Length());
}

INLINE void CPAKSession::Serialize(CStream& Stream)
{
	CPAKHead::Serialize(Stream);
	if (Stream.IsRead()) {
		Stream >> m_llSessionId;
	}
	else {
		Stream << m_llSessionId;
	}
}

INLINE LLong CPAKSession::GetSessionId(void)
{
	return m_llSessionId;
}

INLINE void CPAKSession::SetSessionId(LLong llSessionId)
{
	m_llSessionId = llSessionId;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPAKSessionAck : SessionAck
INLINE CPAKSessionAck::CPAKSessionAck(UInt uEvent, UInt uType)
: CPAKAck(uEvent, uType)
, m_llSessionId(0)
{
}

INLINE CPAKSessionAck::~CPAKSessionAck(void)
{
}

INLINE CPAKSessionAck::CPAKSessionAck(const CPAKSessionAck& aSrc)
: CPAKAck(aSrc)
, m_llSessionId(aSrc.m_llSessionId)
{
}

INLINE CPAKSessionAck& CPAKSessionAck::operator=(const CPAKSessionAck& aSrc)
{
	if (&aSrc != this) {
		CPAKAck::operator=(aSrc);
		m_llSessionId = aSrc.m_llSessionId;
	}
	return (*this);
}

INLINE size_t CPAKSessionAck::Length(void)
{
	return (sizeof(LLong) + CPAKAck::Length());
}

INLINE void CPAKSessionAck::Serialize(CStream& Stream)
{
	CPAKAck::Serialize(Stream);
	if (Stream.IsRead()) {
		Stream >> m_llSessionId;
	}
	else {
		Stream << m_llSessionId;
	}
}

INLINE LLong CPAKSessionAck::GetSessionId(void)
{
	return m_llSessionId;
}

INLINE void CPAKSessionAck::SetSessionId(LLong llSessionId)
{
	m_llSessionId = llSessionId;
}

#endif // __COMMON_PACKET_INL__