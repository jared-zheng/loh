//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : gamepacket.h                                 //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : 游戏服务器信令                                     //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __GAME_PACKET_H__
#define __GAME_PACKET_H__

#pragma once

#include "commonpacket.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// 游戏服务器信令定义
enum PAK_EVENT_GAME
{
	PAK_EVENT_GAME_ID = (PAK_EVENT_GAME_BEGIN + 1), // 获取游戏服务器的Id, 网关服务器由于查询角色信息使用, CPAKHead<PAK_EVENT_GAME_ID, PAK_TYPE>
	PAK_EVENT_GAME_IDACK,                           // CPAKGameIdAck
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 游戏服务器Id
class CPAKGameIdAck : public CPAKAck
{
public:
	CPAKGameIdAck(UInt uEvent = PAK_EVENT_GAME_IDACK, UInt uType = PAK_TYPE_GAME);
	virtual ~CPAKGameIdAck(void);

	CPAKGameIdAck(const CPAKGameIdAck& aSrc);
	CPAKGameIdAck& operator=(const CPAKGameIdAck& aSrc);

	virtual size_t Length(void) OVERRIDE;
	virtual void   Serialize(CStream&) OVERRIDE;

	Int            GetId(void);
	void           SetId(Int nId);
private:
	Int            m_nId;  
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "gamepacket.inl"

#endif // __GAME_PACKET_H__