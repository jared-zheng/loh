//////////////////////////////////////////////////////////////////
//   The Legend of Heros                                        //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : packet.h                                     //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : 信令定义                                           //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __PACKET_H__
#define __PACKET_H__

#pragma once

#include "datadef.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPAKHead : 信令包数据头
class CPAKHead : public CEventBase
{
public:
	static CPAKHead* Create(CStream&);
public:
	CPAKHead(UInt uEvent = 0, UInt uType = PAK_TYPE_NONE);
	virtual ~CPAKHead(void);

	CPAKHead(const CPAKHead& aSrc);
	CPAKHead& operator=(const CPAKHead& aSrc);

	virtual size_t Length(void) OVERRIDE;
	virtual void   Serialize(CStream&) OVERRIDE;

	UInt   GetEvent(void);
	void   SetEvent(UInt uEvent);

	UInt   GetType(void);
	void   SetType(UInt uType);

	UInt   GetSize(void);
	void   AdjustSize(void);
private:
	UInt   m_uEvent; // 信令号
	UInt   m_uType;  // 发送数据包类型
	UInt   m_uSize;  // 信令包大小
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPAKSimple : 信令包模板
template <UInt uEvent, UInt uType>
class CPAKSimple : public CPAKHead
{
public:
	CPAKSimple(void);
	virtual ~CPAKSimple(void);

	CPAKSimple(const CPAKSimple& aSrc);
	CPAKSimple& operator=(const CPAKSimple& aSrc);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPAKAck : 简单应答包
class CPAKAck : public CPAKHead
{
public:
	CPAKAck(UInt uEvent = 0, UInt uType = PAK_TYPE_NONE);
	virtual ~CPAKAck(void);

	CPAKAck(const CPAKAck& aSrc);
	CPAKAck& operator=(const CPAKAck& aSrc);

	virtual size_t Length(void) OVERRIDE;
	virtual void   Serialize(CStream&) OVERRIDE;

	UInt   GetAck(void);
	void   SetAck(UInt uAck);
private:
	UInt   m_uAck;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef CNetworkEventHandler<CNETDispatch<CPAKHead>> CPAKHandler;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "packet.inl"

#endif // __PACKET_H__