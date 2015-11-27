//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Source File : packet.cpp                                   //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : ÐÅÁî¶¨Òå                                           //
//                                                              //
//////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "centerpacket.h"
#include "selectpacket.h"
#include "logindbpacket.h"
#include "loginpacket.h"
#include "gamedbpacket.h"
#include "gamepacket.h"
#include "zonepacket.h"
#include "gatepacket.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPAKHead
CPAKHead* CPAKHead::Create(CStream& Stream)
{
	CPAKHead* pPkt = nullptr;

	UInt uEvent = 0;
	Stream >> uEvent;
	switch (uEvent) {
	// common
	case PAK_EVENT_LIVE:
		{
			pPkt = MNEW CPAKHead(PAK_EVENT_LIVE);
		}
		break;
	case PAK_EVENT_LIVEACK:
		{
			pPkt = MNEW CPAKHead(PAK_EVENT_LIVEACK);
		}
		break;
	case PAK_EVENT_LINK:
		{
			pPkt = MNEW CPAKLink;
		}
		break;
	case PAK_EVENT_LINKACK:
		{
			pPkt = MNEW CPAKHead(PAK_EVENT_LINKACK);
		}
		break;
	case PAK_EVENT_UPDATE:
		{
			pPkt = MNEW CPAKUpdate;
		}
		break;
	case PAK_EVENT_UPDATEACK:
		{
			pPkt = MNEW CPAKHead(PAK_EVENT_UPDATEACK);
		}
		break;
	case PAK_EVENT_SYNC:
		{
			pPkt = MNEW CPAKSync;
		}
		break;
	case PAK_EVENT_SYNCACK:
		{
			pPkt = MNEW CPAKHead(PAK_EVENT_SYNCACK);
		}
		break;
	case PAK_EVENT_UNLINK:
		{
			pPkt = MNEW CPAKHead(PAK_EVENT_UNLINK);
		}
		break;
	case PAK_EVENT_UNLINKACK:
		{
			pPkt = MNEW CPAKHead(PAK_EVENT_UNLINKACK);
		}
		break;
	// center
	// select
	case PAK_EVENT_SELECT_LOGIN:
		{
			pPkt = MNEW CPAKHead(PAK_EVENT_SELECT_LOGIN);
		}
		break;
	case PAK_EVENT_SELECT_LOGINACK:
		{
			pPkt = MNEW CPAKSelectLoginAck;
		}
		break;
	// logindb
	// login
	case PAK_EVENT_LOGIN_LINK:
		{
			pPkt = MNEW CPAKLoginLink;
		}
		break;
	case PAK_EVENT_LOGIN_LINKACK:
		{
			pPkt = MNEW CPAKLoginLinkAck;
		}
		break;
	case PAK_EVENT_LOGIN_UNLINK:
		{
			pPkt = MNEW CPAKLoginUnlink;
		}
		break;
	case PAK_EVENT_LOGIN_UNLINKACK:
		{
			pPkt = MNEW CPAKSessionAck(PAK_EVENT_LOGIN_UNLINKACK);
		}
		break;
	case PAK_EVENT_LOGIN_SELECT_GAME:
		{
			pPkt = MNEW CPAKLoginSelectGame;
		}
		break;
	case PAK_EVENT_LOGIN_LINK_GAME:
		{
			pPkt = MNEW CPAKLoginLinkGame;
		}
		break;
	// gamedb
	// game
	// zone
	// gate
	case PAK_EVENT_GATE_QUEUE:
		{
			pPkt = MNEW CPAKSession(PAK_EVENT_GATE_QUEUE);
		}
		break;
	case PAK_EVENT_GATE_QUEUEACK:
		{
			pPkt = MNEW CPAKGateQueueAck;
		}
		break;
	case PAK_EVENT_GATE_SELECT:
		{
			pPkt = MNEW CPAKGateSelect;
		}
		break;
	case PAK_EVENT_GATE_SELECTACK:
		{
			pPkt = MNEW CPAKGateSelectAck;
		}
		break;
	case PAK_EVENT_GATE_PLAY:
		{
			pPkt = MNEW CPAKGatePlay;
		}
		break;
	case PAK_EVENT_GATE_PLAYACK:
		{
			pPkt = MNEW CPAKSessionAck(PAK_EVENT_GATE_PLAYACK);
		}
		break;
	default:
		{
		}
	}
	if (pPkt != nullptr) {
		Stream.Seek(0);
		pPkt->Serialize(Stream);
	}
	return pPkt;
}