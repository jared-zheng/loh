///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : network.h                                     ¡¡¡¡                                    //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-10-29     version 0.0.5.8                                                             //
//   Detail : network header                                                                             //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __NETWORK_H__
#define __NETWORK_H__

#pragma once

#include "subsystem.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(NETWORK_EXPORT)
	#define NETWORKAPI            API_EXPORT
	#define NETWORKCLASS          CLASS_EXPORT
#elif defined(RUNTIME_STATIC)
	#define NETWORKAPI                                     
	#define NETWORKCLASS                                
#else   // NETWORK_EXPORT
	#define NETWORKAPI            API_IMPORT
	#define NETWORKCLASS          CLASS_IMPORT
#endif  // NETWORK_EXPORT

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CNETTraits
class CNETTraits
{
public:
	// common
	enum EVENT_TYPE
	{
		EVENT_NONE            = 0,
		// tcp
		EVENT_TCP_ACCEPT,            // after listen socket accept a connect socket(alias : accept socket), return false close accept socket
		EVENT_TCP_CONNECT,           // after connect socket connect to server(utParam include connect error if failed), return false close connect socket
		EVENT_TCP_RECV,              // socket recv data, return false to close socket
		EVENT_TCP_CLOSE,             // socket close 
		// udp
		EVENT_UDP_RECV,              // socket recv data, return false to discard all recved pack from current remote udp 
		EVENT_UDP_CLOSE,             // socket close 
		//
		EVENT_TCP_UNKNOWN     = 98,  // tcp unknown data : exception
		EVENT_UDP_UNKNOWN     = 99,  // udp unknown data : exception
		EVENT_RESERVED        = 100, // user defined network event >= EVENT_RESERVED
	};

	enum SEND_FLAG
	{
		SEND_NORMAL           = 0,
		SEND_LIVE,                   // live-data != 0
		SEND_CLOSE,                  // close socket after send okay
		SEND_BROADCAST,              // tcp : exclude sender; udp : all
		// tcp only
		SEND_BROADCAST_ALL,          // all accept socket
		SEND_BROADCAST_AS,           // accept socket live-data == llParam
		SEND_BROADCAST_NOT,          // accept socket live-data != llParam
		SEND_BROADCAST_OR,           // accept socket (live-data | llParam) != 0
		SEND_BROADCAST_AND,          // accept socket (live-data & llParam) != 0
		SEND_BROADCAST_XOR,          // accept socket (live-data ^ llParam) != 0
	};

	enum ATTR_CONST
	{
		ATTR_NONE             = 0x00000000,
		ATTR_THREAD           = 0x00000001, // work thread count, 1 <--> CPU core * 2, default CPU core * 2

		ATTR_IPV6             = 0x00000100, // ipv6
		ATTR_NETORDER         = 0x00000200, // NET_ADDR
		ATTR_NAGLE            = 0x00000400, // enable TCP Nagle algorithm(http://en.wikipedia.org/wiki/Nagle%27s_algorithm)

		ATTR_ACK_DETECT       = 0x00010000, // TCP accept socket ack flag, TRUE or FALSE, default[FALSE]
		ATTR_TIMEOUT_DETECT   = 0x00020000, // TCP accept socket timeout flag, TRUE or FALSE, default[FALSE]
		ATTR_ACK_TIME         = 0x00050000, // ack time[MS], default[4000], set ack time also will set ack [TRUE]
		ATTR_TIMEOUT_TIME     = 0x000A0000, // timeout time[MS], default[80000], set timeout time also will set timeout [TRUE]

		// http://en.wikipedia.org/wiki/TCP/IP
		// http://en.wikipedia.org/wiki/Jumbo_frame
		// ***windows platform tcp send/recv buffer is fixed to 8192***
		// ***linux platform tcp send.recv is times of 1460***
		ATTR_MAX_BUFFER       = 0x00100000, // send/recv buffer size, default[4096 - ATTR_LMT_PACK_HEAD = 4088]
		ATTR_MAX_JUMBOBUF     = 0x00200000, // send/recv jumbo buffer size, default[8 * 4096 - ATTR_LMT_PACK_HEAD = 32760]

		ATTR_MAX_SEND         = 0x01000000, // max send queue size, default[8]
		ATTR_MAX_EVENT        = 0x02000000, // max wait event array
	};

	enum ATTR_LIMIT
	{
		ATTR_LMT_PACK_HEAD    = 8,
		ATTR_LMT_MIN_TIME     = 1000,
		ATTR_LMT_MAX_TIME     = (60 * 60 * 1000),
		ATTR_LMT_MIN_BUFFER   = 1024,
		ATTR_LMT_MAX_BUFFER   = (512 * 1024),
		ATTR_LMT_MIN_JUMBOBUF = 4096,
		ATTR_LMT_MAX_JUMBOBUF = (8 * 1024 * 1024),
		ATTR_LMT_MIN_QUEUE    = 4,
		ATTR_LMT_MAX_QUEUE    = 4096,
		ATTR_LMT_MIN_BACKLOG  = 5,
		ATTR_LMT_MAX_BACKLOG  = 4096,
	};

	enum ATTR_DEFAULT
	{
		ATTR_DEF_ACK_TIME     = 4000,   // 1000 <--> 60 * 60 * 1000, N * 1000
		ATTR_DEF_TIMEOUT_TIME = 80000,  // 1000 <--> 60 * 60 * 1000, N * 1000
		ATTR_DEF_MAX_BUFFER   = 4088,   // (1024 <--> 512 * 1024) - ATTR_LMT_PACK_HEAD
		ATTR_DEF_MAX_JUMBOBUF = 16376,  // (4096 <--> 8 * 1024 * 1024) - ATTR_LMT_PACK_HEAD
		ATTR_DEF_MAX_SEND     = 8,      // 4 <--> 4096, send queue
		ATTR_DEF_MAX_EVENT    = 64,     // max wait event size

		ATTR_DEF_TIMES        = 4,      // JUMBOBUF >= BUFFER * 4
		ATTR_DEF_MS           = 1000,
	};

	enum SOCKET_CONST
	{
		SOCKET_LIVEDATA       = 0, // socket live data
		// tcp
		SOCKET_TCP,                // socket type flag, TRUE is TCP socket
		SOCKET_TCP_BROADCAST,      // socket broadcast flag, TRUE or FALSE, default[TRUE]
		// udp
		SOCKET_UDP,                // socket type flag, TRUE is UDP socket
		SOCKET_UDP_BROADCAST,
	};
	typedef struct tagNET_ATTR
	{
	public:
		tagNET_ATTR(void) { MM_SAFE::Memset(this, 0, sizeof(tagNET_ATTR)); }
		~tagNET_ATTR(void){ }
	public:
		Int      nAttrs;
		Int      nThread;
		Int      nAckTime;      // during ack time, use SetAttr[SOCKET_LIVEDATA] to accept as live socket
		Int      nTimeout;
		Int      nMaxJumbo;     // sub ATTR_LMT_PACK_HEAD when init network, jumbo buffer >= nMaxBuffer * ATTR_DEF_TIMES
		Int      nMaxBuffer;    // sub ATTR_LMT_PACK_HEAD when init network
		Int      nMaxSend;
		Int      nMaxEvent;
		// out param
		size_t   stBufferOffset;
		size_t   stJumboOffset;
		PINDEX   inxBuffer;
		PINDEX   inxJumbo;
	}NET_ATTR, *PNET_ATTR;

	typedef struct tagNET_DUMP
	{
	public:
		tagNET_DUMP(void) { Reset(); }
		~tagNET_DUMP(void){ }
		void Reset(void)  { MM_SAFE::Memset(this, 0, sizeof(tagNET_DUMP)); }
	public:
		LLong   llTCPConnectCount;
		LLong   llTCPCloseCount;
		LLong   llUDPCreateCount;
		LLong   llUDPCloseCount;
		LLong   llSendSize;
		LLong   llRecvSize;
		LLong   llSendTick; // CPU tick - MS : (itSendTick * ATTR_DEF_MS) / CPlatform::GetOSTickFreq();
		LLong   llRecvTick; // CPU tick - MS : (itRecvTick * ATTR_DEF_MS) / CPlatform::GetOSTickFreq();
	}NET_DUMP, *PNET_DUMP;

	typedef struct tagNET_ADDR
	{
	public:
		enum LEN_LIMIT
		{
			LEN_ULLONG = 2,
			LEN_UINT   = 4,
			LEN_USHORT = 8,
			LEN_BYTE   = 16,
		};

		typedef union tagADDR
		{
			ULLong   ullAddr[LEN_ULLONG];
			UInt     uAddr[LEN_UINT];
			UShort   usAddr[LEN_USHORT];
			Byte     bAddr[LEN_BYTE];
		}ADDR;
	public:
		class NET_ADDRTraits
		{
		public:
			typedef const tagNET_ADDR& INARGTYPE;
			typedef tagNET_ADDR& OUTARGTYPE;
		public:
			static void CopyElements(tagNET_ADDR* pDst, const tagNET_ADDR* pSrc, size_t stElements)
			{
				for (size_t i = 0; i < stElements; ++i)
				{
					pDst[i] = pSrc[i];
				}
			}

			static void RelocateElements(tagNET_ADDR* pDst, tagNET_ADDR* pSrc, size_t stElements)
			{
				MM_SAFE::Memmov(pDst, stElements * sizeof(tagNET_ADDR), pSrc, stElements * sizeof(tagNET_ADDR));
			}

			static bool CompareElements(INARGTYPE t1, INARGTYPE t2)
			{
				if ((t1.usAttr == t2.usAttr) && (t1.usPort == t2.usPort))
				{
					return (MM_SAFE::Memcmp(t1.Addr.bAddr, t2.Addr.bAddr, LEN_BYTE) == 0);
				}
				return false;
			}

			static Int CompareElementsOrdered(INARGTYPE t1, INARGTYPE t2)
			{
				if (t1.usAttr == t2.usAttr)
				{
					if (t1.usPort == t2.usPort)
					{
						return MM_SAFE::Memcmp(t1.Addr.bAddr, t2.Addr.bAddr, LEN_BYTE);
					}
					return (Int)(t1.usPort - t2.usPort);
				}
				return ((t1.usAttr > t2.usAttr) ? (Int)(1) : (Int)(-1));
			}

			static size_t HashElements(INARGTYPE t)
			{
				return (CHash::Hash((PByte)(&t), sizeof(tagNET_ADDR)));
			}
		};
	public:
		tagNET_ADDR(void) { Reset(); }
		~tagNET_ADDR(void){ }
		void Reset(void)  { MM_SAFE::Memset(this, 0, sizeof(tagNET_ADDR)); }
	public:
		UShort   usAttr; // ATTR_IPV6
		UShort   usPort; // host bits; if usAttr & ATTR_NETORDER, net bits
		ADDR     Addr;
	}NET_ADDR, *PNET_ADDR;

	typedef struct tagTCP_PARAM
	{
	public:
		tagTCP_PARAM(void)
		: krSocket(nullptr)
		, pData(nullptr)
		, index(nullptr)
		, pCache(nullptr)
		{
		}

		~tagTCP_PARAM(void)
		{
		}
	public:
		KeyRef     krSocket;
		PByte      pData;  // data 
		PINDEX     index;  // Cache index
		PByte      pCache; // Cache ptr
	}TCP_PARAM, *PTCP_PARAM;

	struct tagUDP_PARAM : public tagTCP_PARAM
	{
		NET_ADDR   NetAddr;
	};
	typedef tagUDP_PARAM UDP_PARAM, *PUDP_PARAM;
	// common
	typedef CTArray<CStringKey, CStringKey::CStringFixTraits>   ARY_STRINGKEY;
	typedef CTArray<NET_ADDR>                                   ARY_NETADDR;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CStreamScopePtr
typedef CTScopePtr<CStream>   CStreamScopePtr;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CNetwork
DECLARE_UUID( CNetwork, {10C538F9-9756-4f28-B868-81D182FE8733} )
class NOVTABLE CNetwork ABSTRACT : public CComponent, public CNETTraits
{
public:
	virtual UInt   Init(NET_ATTR& Attr, CEventHandler* pEventHandler = nullptr) PURE_METHOD;
	virtual void   Exit(void) PURE_METHOD;

	// nFlag : SOCKET_TCP           - unicast TCP socket
	//         SOCKET_TCP_BROADCAST - broadcast TCP socket
	//         SOCKET_UDP           - unicast UDP socket
	//         SOCKET_UDP_BROADCAST - broadcast UDP socket
	virtual KeyRef Create(UShort usLocalPort = 0, PCXStr pszLocalAddr = nullptr, Int nFlag = SOCKET_TCP_BROADCAST) PURE_METHOD;
	virtual KeyRef Create(CEventHandler& EventHandlerRef, UShort usLocalPort = 0, PCXStr pszLocalAddr = nullptr, Int nFlag = SOCKET_TCP_BROADCAST) PURE_METHOD;
	virtual KeyRef Create(NET_ADDR& NetAddrRef, Int nFlag = SOCKET_TCP_BROADCAST) PURE_METHOD;
	virtual KeyRef Create(CEventHandler& EventHandlerRef, NET_ADDR& NetAddrRef, Int nFlag = SOCKET_TCP_BROADCAST) PURE_METHOD;
	virtual bool   Destroy(KeyRef krSocket, bool bAsync = true) PURE_METHOD;
	// TCP socket only
	// To understand the nAccept argument, we must realize that for a given listening socket, the kernel maintains two queues
	// 1.An incomplete connection queue, which contains an entry for each SYN that has arrived from a client for which the server is awaiting completion of the TCP three-way handshake. 
	//   These sockets are in the SYN_RCVD state .
	// 2.A completed connection queue, which contains an entry for each client with whom the TCP three-way handshake has completed. These sockets are in the ESTABLISHED state
	//
	// The nBlock argument to the listen function has historically specified the maximum value for the sum of both queues.
	// Berkeley-derived implementations add a fudge factor to the backlog: It is multiplied by 1.5, and the default value is 5
	// 
	// When a SYN arrives from a client, TCP creates a new entry on the incomplete queue and then responds with the second segment of the three-way handshake: 
	// the server's SYN with an ACK of the client's SYN (Section 2.6). 
	// This entry will remain on the incomplete queue until the third segment of the three-way handshake arrives (the client's ACK of the server's SYN), 
	// or until the entry times out. (Berkeley-derived implementations have a timeout of 75 seconds for these incomplete entries.)
	// 
	// If the queues are full when a client SYN arrives, TCP ignores the arriving SYN (pp. 930¨C931 of TCPv2);
	// it does not send an RST. This is because the condition is considered temporary, and the client TCP will retransmit its SYN, 
	// hopefully finding room on the queue in the near future. If the server TCP immediately responded with an RST, 
	// the client's connect would return an error, forcing the application to handle this condition instead of letting TCP's normal retransmission take over.
	// Also, the client could not differentiate between an RST in response to a SYN meaning "there is no server at this port" versus "there is a server at this port but its queues are full."
	// THREE-WAY HANDSHAKE : see wiki's[tcp option.txt]
	virtual bool   Listen(KeyRef krSocket, Int nAccept = ATTR_LMT_MIN_BACKLOG) PURE_METHOD;
	virtual bool   Connect(KeyRef krSocket, UShort usRemotePort, PCXStr pszRemoteAddr) PURE_METHOD;
	virtual bool   Connect(KeyRef krSocket, NET_ADDR& RemoteNetAddrRef) PURE_METHOD;
	// Stream must alloc from AllocBuffer or AllocJumboBuffer
	// TCP
	virtual bool   Send(KeyRef krSocket, CStream& Stream, Int nFlag = SEND_NORMAL, LLong llParam = 0) PURE_METHOD;
	virtual bool   Send(KeyRef krSocket, CEventBase& EventRef, Int nFlag = SEND_NORMAL, LLong llParam = 0) PURE_METHOD;
	// UDP
	virtual bool   SendTo(KeyRef krSocket, CStream& Stream, PCXStr pszRemoteAddr, UShort usRemotePort, Int nFlag = SEND_NORMAL) PURE_METHOD;
	virtual bool   SendTo(KeyRef krSocket, CStream& Stream, NET_ADDR& RemoteNetAddrRef, Int nFlag = SEND_NORMAL) PURE_METHOD;
	virtual bool   SendTo(KeyRef krSocket, CEventBase& EventRef, PCXStr pszRemoteAddr, UShort usRemotePort, Int nFlag = SEND_NORMAL) PURE_METHOD;
	virtual bool   SendTo(KeyRef krSocket, CEventBase& EventRef, NET_ADDR& RemoteNetAddrRef, Int nFlag = SEND_NORMAL) PURE_METHOD;
	// TCP : listen can not get remote addr, connect can not get remote addr until connect okay
	// UDP : get local addr only
	virtual bool   GetAddr(KeyRef krSocket, CStringKey& strAddr, UShort& usPort, bool bRemote = true) PURE_METHOD;
	virtual bool   GetAddr(KeyRef krSocket, NET_ADDR& NetAddrRef, bool bRemote = true) PURE_METHOD;
	virtual bool   GetLocalAddr(ARY_STRINGKEY& strAddrs, Int nAttr = ATTR_NONE) PURE_METHOD;  // ATTR_IPV6, ATTR_NETORDER
	virtual bool   GetLocalAddr(ARY_NETADDR& NetAddrs, Int nAttr = ATTR_NONE) PURE_METHOD;    // ATTR_IPV6, ATTR_NETORDER
	virtual bool   TranslateAddr(CStringKey& strAddr, UShort& usPort, NET_ADDR& NetAddrRef, bool bString2Addr = true) PURE_METHOD;

	// nFlag : SOCKET_LIVEDATA      - TCP & UDP socket get/set live-data
	//         SOCKET_TCP           - TCP socket return TRUE, else FALSE
	//         SOCKET_TCP_BROADCAST - TCP get/set broadcast flag
	//         SOCKET_UDP           - UDP socket return TRUE, else FALSE
	//         SOCKET_UDP_BROADCAST - UDP get TRUE if broadcast
	virtual LLong  GetAttr(KeyRef krSocket, Int nFlag = SOCKET_LIVEDATA) PURE_METHOD;
	virtual bool   SetAttr(KeyRef krSocket, LLong llData, Int nAttr = SOCKET_LIVEDATA) PURE_METHOD;
	// ***recommend use this***, pData from TCP_PARAM.pData or UDP_PARAM.pData
	// CStream::STREAMM_WRITE or CStream::STREAMM_READ
	virtual bool   AllocBuffer(CStreamScopePtr& StreamScopePtrRef, PByte pData = nullptr) PURE_METHOD;
	virtual bool   AllocJumboBuffer(CStreamScopePtr& StreamScopePtrRef, PByte pData = nullptr) PURE_METHOD;

	virtual bool   ReferBuffer(CStreamScopePtr& StreamScopePtrRef, CStream& StreamSrc) PURE_METHOD;

	virtual void   Dump(NET_DUMP& Dump) PURE_METHOD;
	virtual void   Attr(NET_ATTR& Attr) PURE_METHOD;
};
typedef CTRefCountPtr<CNetwork> CNetworkPtr;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CNetworkSystem : CSubSystem
DECLARE_UUID( CNetworkSystem, {9154AA45-406F-4365-BD71-99A3D36176D9} )

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // __NETWORK_H__