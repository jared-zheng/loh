//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : selectpacket.h                               //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : 选择服务器信令                                     //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __SELECT_PACKET_H__
#define __SELECT_PACKET_H__

#pragma once

#include "commonpacket.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
enum SELECT_ERROR
{
	SELECT_ERROR_NONE_LOGIN = 2,   // 选择登陆服务器时出错, > DATA_DEF::DATAD_FAIL
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// select信令定义
enum PAK_EVENT_SELECT
{
	PAK_EVENT_SELECT_LOGIN = (PAK_EVENT_SELECT_BEGIN + 1), // 选择空闲登陆服务器, CPAKSimple<PAK_EVENT_SELECT_LOGIN, PAK_TYPE>
	PAK_EVENT_SELECT_LOGINACK, // CPAKSelectLoginAck
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPAKSelectLoginAck : 选择空闲登陆服务器Ack
class CPAKSelectLoginAck : public CPAKAck
{
public:
	CPAKSelectLoginAck(UInt uEvent = PAK_EVENT_SELECT_LOGINACK, UInt uType = PAK_TYPE_SELECT);
	virtual ~CPAKSelectLoginAck(void);

	CPAKSelectLoginAck(const CPAKSelectLoginAck& aSrc);
	CPAKSelectLoginAck& operator=(const CPAKSelectLoginAck& aSrc);

	virtual size_t Length(void) OVERRIDE;
	virtual void   Serialize(CStream&) OVERRIDE;

	CNETTraits::NET_ADDR&  GetAddr(void);
	void                   SetAddr(CNETTraits::NET_ADDR& LoginAddr);
private:
	CNETTraits::NET_ADDR   m_LoginAddr;  // outer addr[ipv4 or ipv6]
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "selectpacket.inl"

#endif // __SELECT_PACKET_H__