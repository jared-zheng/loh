///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : networkevent.h                                     ¡¡¡¡                               //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-10-29     version 0.0.5.8                                                             //
//   Detail : network event                                                                              //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __NETWORK_EVENT_H__
#define __NETWORK_EVENT_H__

#pragma once

#include "network.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CNETDispatch
template <typename PACKET, bool bNetByteOrder = false>
class NOVTABLE CNETDispatch : public CNETTraits
{
public:
	enum DUMP_HEX
	{
		DUMP_HEX_DWORD = 4,
		DUMP_HEX_LINE  = 32,
	};

	typedef CTRefCountPtr<PACKET> PacketPtr;
public:
	virtual bool OnDispatch(Int nEvent, CStream& Stream, LLong llParam);
	virtual bool OnEvent(Int nEvent, uintptr_t utParam, LLong llParam);
protected:
	CNETDispatch(void);
	virtual ~CNETDispatch(void);

	virtual bool OnTcpDispatch(const PacketPtr& PktPtr, PTCP_PARAM pTcp) PURE_METHOD;
	virtual bool OnUdpDispatch(const PacketPtr& PktPtr, PUDP_PARAM pUdp) PURE_METHOD;
	virtual bool OnTcpAccept(KeyRef krAccept, KeyRef krListen) PURE_METHOD;
	virtual bool OnTcpConnect(UInt uError, KeyRef krConnect) PURE_METHOD;
	virtual bool OnTcpClose(KeyRef krSocket, LLong llLiveData) PURE_METHOD;
	virtual bool OnUdpClose(KeyRef krSocket, LLong llLiveData) PURE_METHOD;

	void    DumpHexPacketData(Int nEvent, CStream& Stream, LLong llParam);
private:
	CNETDispatch(const CNETDispatch&);
	CNETDispatch& operator=(const CNETDispatch&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CNetworkEventHandler
template <typename DISPATCH>
class NOVTABLE CNetworkEventHandler : public CEventHandler, public DISPATCH
{
public:
	virtual bool OnHandle(Int nEvent, uintptr_t utParam, LLong llParam) OVERRIDE;
	virtual bool OnHandle(Int nEvent, CEventBase& EventRef, LLong llParam) OVERRIDE;
	virtual bool OnHandle(Int nEvent, CStream& Stream, LLong llParam) OVERRIDE;
	virtual bool OnHandle(Int nEvent, UInt uCount) OVERRIDE;
protected:
	CNetworkEventHandler(void);
	virtual ~CNetworkEventHandler(void);
private:
	CNetworkEventHandler(const CNetworkEventHandler&);
	CNetworkEventHandler& operator=(const CNetworkEventHandler&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "networkevent.inl"

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // __NETWORK_EVENT_H__