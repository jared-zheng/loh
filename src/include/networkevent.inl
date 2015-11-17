///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : networkevent.inl                                                                      //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-10-29     version 0.0.5.8                                                             //
//   Detail : network event                                                                              //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __NETWORK_EVENT_INL__
#define __NETWORK_EVENT_INL__

#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CNETDispatch
template <typename PACKET, bool bNetByteOrder>
INLINE CNETDispatch<PACKET, bNetByteOrder>::CNETDispatch(void)
{
}

template <typename PACKET, bool bNetByteOrder>
INLINE CNETDispatch<PACKET, bNetByteOrder>::~CNETDispatch(void)
{
}

template <typename PACKET, bool bNetByteOrder>
INLINE CNETDispatch<PACKET, bNetByteOrder>::CNETDispatch(const CNETDispatch<PACKET, bNetByteOrder>&)
{
}

template <typename PACKET, bool bNetByteOrder>
INLINE CNETDispatch<PACKET, bNetByteOrder>& CNETDispatch<PACKET, bNetByteOrder>::operator=(const CNETDispatch<PACKET, bNetByteOrder>&)
{
	return (*this);
}

template <typename PACKET, bool bNetByteOrder>
INLINE bool CNETDispatch<PACKET, bNetByteOrder>::OnDispatch(Int nEvent, CStream& Stream, LLong llParam)
{
	assert((nEvent == EVENT_TCP_RECV) || (nEvent == EVENT_UDP_RECV));
	Stream.SetByteSwap(bNetByteOrder);
	PacketPtr PktPtrRef = PACKET::Create(Stream);
	if (PktPtrRef != nullptr)
	{
		if (nEvent == EVENT_TCP_RECV)
		{
			PTCP_PARAM pTcp = reinterpret_cast<PTCP_PARAM>(llParam);
			return OnTcpDispatch(PktPtrRef, pTcp);
		}
		else
		{
			PUDP_PARAM pUdp = reinterpret_cast<PUDP_PARAM>(llParam);
			return OnUdpDispatch(PktPtrRef, pUdp);
		}
	}
	else
	{
		DumpHexPacketData(nEvent, Stream, llParam);
	}
	return true;
}

template <typename PACKET, bool bNetByteOrder>
INLINE bool CNETDispatch<PACKET, bNetByteOrder>::OnEvent(Int nEvent, uintptr_t utParam, LLong llParam)
{
	switch (nEvent)
	{
	case EVENT_TCP_ACCEPT:
		{
			return OnTcpAccept(reinterpret_cast<KeyRef>(utParam), reinterpret_cast<KeyRef>(llParam));
		}
		break;
	case EVENT_TCP_CONNECT:
		{
			return OnTcpConnect((UInt)(utParam), reinterpret_cast<KeyRef>(llParam));
		}
		break;
	case EVENT_TCP_CLOSE:
		{
			return OnTcpClose(reinterpret_cast<KeyRef>(utParam), llParam);
		}
		break;
	case EVENT_UDP_CLOSE:
		{
			return OnUdpClose(reinterpret_cast<KeyRef>(utParam), llParam);
		}
		break;
	default:
		{
			DEV_WARN(TF("OnEvent can not diapatch event : %d[%p, %p]"), nEvent, utParam, llParam);
		}
	}
	return true;
}

template <typename PACKET, bool bNetByteOrder>
INLINE void CNETDispatch<PACKET, bNetByteOrder>::DumpHexPacketData(Int nEvent, CStream& Stream, LLong llParam)
{
	Byte bHead[LMT_KEY] = { 0 };
	UInt uRet = (UInt)Stream.Read(bHead, LMT_KEY);

	XChar szDump[LMT_BUF] = { 0 };
	Int nPos = 0;
	if (nEvent == EVENT_TCP_RECV)
	{
		PTCP_PARAM pTcp = reinterpret_cast<PTCP_PARAM>(llParam);
		nPos = CXChar::Format(szDump, LMT_BUF, TF("Socket %p recv TCP unknown packet[size=%d], packet head data :"), pTcp->krSocket, Stream.Size());
	}
	else
	{
		PUDP_PARAM pUdp = reinterpret_cast<PUDP_PARAM>(llParam);
		if ((pUdp->NetAddr.usAttr & ATTR_IPV6) == 0)
		{
			PByte pIP = pUdp->NetAddr.Addr.bAddr;
			nPos = CXChar::Format(szDump, LMT_BUF, TF("Socket %p recv UDP[%d.%d.%d.%d]:%d unknown packet[size=%d], packet head data :"), pUdp->krSocket, 
			                      pIP[3], pIP[2], pIP[1], pIP[0], pUdp->NetAddr.usPort, Stream.Size());
		}
		else
		{
			PUShort pIP = pUdp->NetAddr.Addr.usAddr;
			nPos = CXChar::Format(szDump, LMT_BUF, TF("Socket %p recv UDP[%X:%X:%X:%X:%X:%X:%X:%X]:%d unknown packet[size=%d], packet head data :"), pUdp->krSocket, 
			                      pIP[0], pIP[1], pIP[2], pIP[3], pIP[4], pIP[5], pIP[6], pIP[7], pUdp->NetAddr.usPort, Stream.Size());
		}
	}
	for (UInt i = 0; i < uRet; ++i)
	{
		if ((i % DUMP_HEX_LINE) == 0)
		{
			szDump[nPos] = TF('\n');
			++nPos;
		}
		else if ((i % DUMP_HEX_DWORD) == 0)
		{
			szDump[nPos] = TF('-');
			++nPos;
		}
		nPos += CXChar::Format((szDump + nPos), (LMT_BUF - nPos), TF("%02X"), bHead[i]);
	}
	DEV_DUMP(szDump);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CNetworkEventHandler
template <typename DISPATCH>
INLINE CNetworkEventHandler<DISPATCH>::CNetworkEventHandler(void)
{
}

template <typename DISPATCH>
INLINE CNetworkEventHandler<DISPATCH>::~CNetworkEventHandler(void)
{
}

template <typename DISPATCH>
INLINE CNetworkEventHandler<DISPATCH>::CNetworkEventHandler(const CNetworkEventHandler<DISPATCH>&)
{
}

template <typename DISPATCH>
INLINE CNetworkEventHandler<DISPATCH>& CNetworkEventHandler<DISPATCH>::operator=(const CNetworkEventHandler<DISPATCH>&)
{
	return (*this);
}

template <typename DISPATCH>
INLINE bool CNetworkEventHandler<DISPATCH>::OnHandle(Int nEvent, uintptr_t utParam, LLong llParam)
{
	return OnEvent(nEvent, utParam, llParam);
}

template <typename DISPATCH>
INLINE bool CNetworkEventHandler<DISPATCH>::OnHandle(Int, CEventBase&, LLong)
{
	return true;
}

template <typename DISPATCH>
INLINE bool CNetworkEventHandler<DISPATCH>::OnHandle(Int nEvent, CStream& Stream, LLong llParam)
{
	return OnDispatch(nEvent, Stream, llParam);
}

template <typename DISPATCH>
INLINE bool CNetworkEventHandler<DISPATCH>::OnHandle(Int, UInt)
{
	return true;
}



#endif // __NETWORK_EVENT_INL__