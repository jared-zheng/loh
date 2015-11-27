//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Inline File : packet.inl                                   //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : 信令定义                                           //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __PACKET_INL__
#define __PACKET_INL__

#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPAKHead : 信令包数据头
INLINE CPAKHead::CPAKHead(UInt uEvent, UInt uType)
: m_uEvent(uEvent)
, m_uType(uType)
, m_uSize(0)
{
}

INLINE CPAKHead::~CPAKHead(void)
{
}

INLINE CPAKHead::CPAKHead(const CPAKHead& aSrc)
: m_uEvent(aSrc.m_uEvent)
, m_uType(aSrc.m_uType)
, m_uSize(aSrc.m_uSize)
{
}

INLINE CPAKHead& CPAKHead::operator=(const CPAKHead& aSrc)
{
	if (&aSrc != this) {
		m_uEvent = aSrc.m_uEvent;
		m_uType  = aSrc.m_uType;
		m_uSize  = aSrc.m_uSize;
	}
	return (*this);
}

INLINE size_t CPAKHead::Length(void)
{
	return (sizeof(UInt) + sizeof(UInt) + sizeof(UInt));
}

INLINE void CPAKHead::Serialize(CStream& Stream)
{
	if (Stream.IsRead()) {
		Stream >> m_uEvent >> m_uType >> m_uSize;
	}
	else {
		Stream << m_uEvent << m_uType << m_uSize;
	}
}

INLINE UInt CPAKHead::GetEvent(void)
{
	return m_uEvent;
}

INLINE void CPAKHead::SetEvent(UInt uEvent)
{
	m_uEvent = uEvent;
}

INLINE UInt CPAKHead::GetType(void)
{
	return m_uType;
}

INLINE void CPAKHead::SetType(UInt uType)
{
	m_uType = uType;
}

INLINE UInt CPAKHead::GetSize(void)
{
	return m_uSize;
}

INLINE void CPAKHead::AdjustSize(void)
{
	m_uSize = (UInt)Length();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPAKSimple : 信令包模板
template <UInt uEvent, UInt uType>
INLINE CPAKSimple<uEvent, uType>::CPAKSimple(void)
: CPAKHead(uEvent, uType)
{
}

template <UInt uEvent, UInt uType>
INLINE CPAKSimple<uEvent, uType>::~CPAKSimple(void)
{
}

template <UInt uEvent, UInt uType>
INLINE CPAKSimple<uEvent, uType>::CPAKSimple(const CPAKSimple<uEvent, uType>& aSrc)
: CPAKHead(aSrc)
{
}

template <UInt uEvent, UInt uType>
INLINE CPAKSimple<uEvent, uType>& CPAKSimple<uEvent, uType>::operator=(const CPAKSimple<uEvent, uType>& aSrc)
{
	if (&aSrc != this) {
		CPAKHead::operator=(aSrc);
	}
	return (*this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPAKAck : 简单应答包
INLINE CPAKAck::CPAKAck(UInt uEvent, UInt uType)
: CPAKHead(uEvent, uType)
, m_uAck(0)
{
}

INLINE CPAKAck::~CPAKAck(void)
{
}

INLINE CPAKAck::CPAKAck(const CPAKAck& aSrc)
: CPAKHead(aSrc)
, m_uAck(aSrc.m_uAck)
{
}

INLINE CPAKAck& CPAKAck::operator=(const CPAKAck& aSrc)
{
	if (&aSrc != this) {
		CPAKHead::operator=(aSrc);
		m_uAck = aSrc.m_uAck;
	}
	return (*this);
}

INLINE size_t CPAKAck::Length(void)
{
	return (sizeof(UInt) + CPAKHead::Length());
}

INLINE void CPAKAck::Serialize(CStream& Stream)
{
	CPAKHead::Serialize(Stream);
	if (Stream.IsRead()) {
		Stream >> m_uAck;
	}
	else {
		Stream << m_uAck;
	}
}

INLINE UInt CPAKAck::GetAck(void)
{
	return m_uAck;
}

INLINE void CPAKAck::SetAck(UInt uAck)
{
	m_uAck = uAck;
}

#endif // __PACKET_INL__