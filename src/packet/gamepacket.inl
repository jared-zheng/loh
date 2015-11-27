//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Inline File : gamepacket.inl                               //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : 游戏服务器信令                                     //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __GAME_PACKET_INL__
#define __GAME_PACKET_INL__

#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPAKGameIdAck : 游戏服务器Id
INLINE CPAKGameIdAck::CPAKGameIdAck(UInt uEvent, UInt uType)
: CPAKAck(uEvent, uType)
, m_nId(0)
{
}

INLINE CPAKGameIdAck::~CPAKGameIdAck(void)
{
}

INLINE CPAKGameIdAck::CPAKGameIdAck(const CPAKGameIdAck& aSrc)
: CPAKAck(aSrc)
, m_nId(aSrc.m_nId)
{
}

INLINE CPAKGameIdAck& CPAKGameIdAck::operator=(const CPAKGameIdAck& aSrc)
{
	if (&aSrc != this) {
		CPAKAck::operator=(aSrc);
		m_nId = aSrc.m_nId;
	}
	return (*this);
}

INLINE size_t CPAKGameIdAck::Length(void)
{
	return (sizeof(Int) + CPAKAck::Length());
}

INLINE void CPAKGameIdAck::Serialize(CStream& Stream)
{
	CPAKAck::Serialize(Stream);
	if (Stream.IsRead()) {
		Stream >> m_nId;
	}
	else {
		Stream << m_nId;
	}
}

INLINE Int CPAKGameIdAck::GetId(void)
{
	return m_nId;
}

INLINE void CPAKGameIdAck::SetId(Int nId)
{
	m_nId = nId;
}

#endif // __GAME_PACKET_INL__