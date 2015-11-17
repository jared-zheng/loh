//////////////////////////////////////////////////////////////////
//   The Legend of Heros                                        //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Inline File : selectpacket.inl                             //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : 选择服务器信令                                     //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __SELECT_PACKET_INL__
#define __SELECT_PACKET_INL__

#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPAKSelectLoginAck : 登录服务器选择包Ack
INLINE CPAKSelectLoginAck::CPAKSelectLoginAck(UInt uEvent, UInt uType)
: CPAKAck(uEvent, uType)
{
}

INLINE CPAKSelectLoginAck::~CPAKSelectLoginAck(void)
{
}

INLINE CPAKSelectLoginAck::CPAKSelectLoginAck(const CPAKSelectLoginAck& aSrc)
: CPAKAck(aSrc)
{
	m_LoginAddr = aSrc.m_LoginAddr;
}

INLINE CPAKSelectLoginAck& CPAKSelectLoginAck::operator=(const CPAKSelectLoginAck& aSrc)
{
	if (&aSrc != this) {
		CPAKAck::operator=(aSrc);
		m_LoginAddr = aSrc.m_LoginAddr;
	}
	return (*this);
}

INLINE size_t CPAKSelectLoginAck::Length(void)
{
	return (sizeof(CNETTraits::NET_ADDR) + CPAKAck::Length());
}

INLINE void CPAKSelectLoginAck::Serialize(CStream& Stream)
{
	CPAKAck::Serialize(Stream);
	if (Stream.IsRead()) {
		Stream.Read(&m_LoginAddr, sizeof(CNETTraits::NET_ADDR));
	}
	else {
		Stream.Write(&m_LoginAddr, sizeof(CNETTraits::NET_ADDR));
	}
}

INLINE CNETTraits::NET_ADDR& CPAKSelectLoginAck::GetAddr(void)
{
	return m_LoginAddr;
}

INLINE void CPAKSelectLoginAck::SetAddr(CNETTraits::NET_ADDR& LoginAddr)
{
	m_LoginAddr = LoginAddr;
}

#endif // __SELECT_PACKET_INL__