//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : datadef.h                                    //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : 信息数据                                          //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __DATA_DEF_H__
#define __DATA_DEF_H__

#pragma once

#include "networkevent.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// server uuid
DECLARE_UUID( ICenterServer,  {FF2D862B-6BE0-43d7-9450-E76B6116D5B8} )
DECLARE_UUID( ISelectServer,  {038ECA57-8A9B-4dd5-9935-96F118054F66} )
DECLARE_UUID( ILoginDBServer, {6E510B2A-112E-4272-ACD8-E2F245DA78D5} )
DECLARE_UUID( ILoginServer,   {3FAA0C20-40D7-4515-AEA6-2AA0B3E8C322} )
DECLARE_UUID( IGameDBServer,  {953E6911-4954-4411-9FDE-5483E544EE7D} )
DECLARE_UUID( IGameServer,    {03ACE2C7-4740-4e5f-91BC-1D1118412D9B} )
DECLARE_UUID( IZoneServer,    {7F8C1EF8-E551-4845-82FA-AC043BE5163E} )
DECLARE_UUID( IGateServer,    {E4D27215-5944-4C25-906B-147D01D317C7} )

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 通用
enum DATA_DEF
{
	DATAD_OKAY              = 0,
	DATAD_FAIL,                       // 具体错误Id各服务器自己定义
	//
	DATAD_PERCENT           = 100,
	DATAD_TICK_MS           = 1000,
	DATAD_MAX_BUSY          = 10000,
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 索引
enum DATA_INDEX
{
	DATA_INDEX_CENTER,
	DATA_INDEX_SELECT,
	DATA_INDEX_LOGINDB,
	DATA_INDEX_LOGIN,
	DATA_INDEX_GAMEDB,
	DATA_INDEX_GAME,
	DATA_INDEX_ZONE,
	DATA_INDEX_GATE,
	DATA_INDEX_MAX,
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 数据包类型定义
enum PAK_TYPE
{
	PAK_TYPE_NONE           = 0x00000000, // NONE
	PAK_TYPE_CENTER         = 0x67788482, // CNTR
	PAK_TYPE_SELECT         = 0x83766784, // SLCT
	PAK_TYPE_LOGINDB        = 0x76786866, // LNDB
	PAK_TYPE_LOGIN          = 0x76717378, // LGIN
	PAK_TYPE_LOGIN_CENTER   = 0x76718482, // LGTR --- 防止center和logindb同进程都使用PAK_TYPE_LOGIN标识, cente广播时对同个login出现2次一样的广播
	PAK_TYPE_GAMEDB         = 0x71776866, // GMDB
	PAK_TYPE_GAME           = 0x71657769, // GAME
	PAK_TYPE_ZONE           = 0x90797869, // ZONE
	PAK_TYPE_GATE           = 0x71658469, // GATE
	PAK_TYPE_CLIENT         = 0x67767884, // CLNT
	PAK_TYPE_CLIENT_SELECT  = 0x67767883, // CLNS
	PAK_TYPE_CLIENT_QUEUE   = 0x67767881, // CLNQ
	PAK_TYPE_CLIENT_LOGIN   = 0x67767876, // CLNL
	PAK_TYPE_CLIENT_READY   = 0x67767882, // CLNR
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 服务器运行状态
enum STATUS_CONST
{
	STATUSC_NONE            = 0,
	STATUSC_INIT,
	STATUSC_RUN,
	STATUSC_PAUSE,
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 服务器更新状态
enum STATUS_UPDATE
{
	STATUSU_NONE            = 0x00000000,
	STATUSU_LINK            = 0x00000001,
	STATUSU_OKAY            = 0x00000002,
	STATUSU_SYNC            = 0x00000004,
	STATUSU_UNLINK          = 0x00000008,
	//
	STATUSU_PING            = 0x00000010,
	STATUSU_UPDATE          = 0x00000020,

	STATUSU_SYNCXLINK       = (STATUSU_SYNC|STATUSU_LINK|STATUSU_UNLINK),
	STATUSU_OKAYSYNC        = (STATUSU_OKAY|STATUSU_SYNC),
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 各服务器信令定义范围
enum PAK_EVENT_SERVER
{
	// center
	PAK_EVENT_CENTER_BEGIN  = (CNETTraits::EVENT_RESERVED + 100),
	PAK_EVENT_CENTER_END    = (PAK_EVENT_CENTER_BEGIN + 99),
	// select
	PAK_EVENT_SELECT_BEGIN,
	PAK_EVENT_SELECT_END    = (PAK_EVENT_SELECT_BEGIN + 99),
	// logindb
	PAK_EVENT_LOGINDB_BEGIN,
	PAK_EVENT_LOGINDB_END   = (PAK_EVENT_LOGINDB_BEGIN + 99),
	// login
	PAK_EVENT_LOGIN_BEGIN,
	PAK_EVENT_LOGIN_END     = (PAK_EVENT_LOGIN_BEGIN + 99),
	// gamedb
	PAK_EVENT_GAMEDB_BEGIN,
	PAK_EVENT_GAMEDB_END    = (PAK_EVENT_GAMEDB_BEGIN + 99),
	// game
	PAK_EVENT_GAME_BEGIN,
	PAK_EVENT_GAME_END      = (PAK_EVENT_GAME_BEGIN + 99),
	// zone
	PAK_EVENT_ZONE_BEGIN,
	PAK_EVENT_ZONE_END      = (PAK_EVENT_ZONE_BEGIN + 99),
	// gate
	PAK_EVENT_GATE_BEGIN,
	PAK_EVENT_GATE_END      = (PAK_EVENT_GATE_BEGIN + 99),
	// realm
	PAK_EVENT_REALM_BEGIN,
	PAK_EVENT_REALM_END     = (PAK_EVENT_REALM_BEGIN + 9999),
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// DataRef
#if (__INTRIN_TARGET__ == INTRIN_TARGET_X86)
	typedef LLong  DataRef;
#elif ((__INTRIN_TARGET__ == INTRIN_TARGET_X64) || (__INTRIN_TARGET__ == INTRIN_TARGET_IA64))
	typedef KeyRef DataRef;
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// tagSERVER_DATA : 服务器信息结构定义
typedef struct tagSERVER_DATA
{
	UInt     uAllCount;  // 累计连接数量
	UInt     uOnline;    // 在线连接数量
	UShort   usBusy;     // [0 - 10000]服务器负载万分比
	UShort   usIncr;     // 每增加一个连接增长的万分比
	UShort   usId;       // 服务器Id, 客户端用来索引名称
	UShort   usStatus;   // 服务器状态
}SERVER_DATA, *PSERVER_DATA;

#endif // __DATA_DEF_H__