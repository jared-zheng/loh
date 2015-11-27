//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : datadef.h                                    //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : 信息数据                                          //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __DATA_DEF_H__
#define __DATA_DEF_H__

#pragma once

#include "networkevent.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// server uuid : 各服务器的UUID定义
DECLARE_UUID( ICenterServer,  {FF2D862B-6BE0-43d7-9450-E76B6116D5B8} ) // 中心服务器UUID
DECLARE_UUID( ISelectServer,  {038ECA57-8A9B-4dd5-9935-96F118054F66} ) // 选择服务器UUID
DECLARE_UUID( ILoginDBServer, {6E510B2A-112E-4272-ACD8-E2F245DA78D5} ) // 登陆DB服务器UUID
DECLARE_UUID( ILoginServer,   {3FAA0C20-40D7-4515-AEA6-2AA0B3E8C322} ) // 登陆服务器UUID
DECLARE_UUID( IGameDBServer,  {953E6911-4954-4411-9FDE-5483E544EE7D} ) // 游戏DB服务器UUID
DECLARE_UUID( IGameServer,    {03ACE2C7-4740-4e5f-91BC-1D1118412D9B} ) // 游戏服务器UUID
DECLARE_UUID( IZoneServer,    {7F8C1EF8-E551-4845-82FA-AC043BE5163E} ) // 地图服务器UUID
DECLARE_UUID( IGateServer,    {E4D27215-5944-4C25-906B-147D01D317C7} ) // 网关服务器UUID

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 通用常量定义
enum DATA_DEF
{
	DATAD_OKAY              = 0,      // 返回成功
	DATAD_FAIL,                       // 具体错误Id各服务器自己定义, 但必须大于DATAD_FAIL数值
	DATAD_PERCENT           = 100,    // 百分比
	DATAD_TICK_MS           = 1000,   // 秒和毫秒转换
	DATAD_MAX_BUSY          = 10000,  // 服务器负载情况(万分比)
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 服务器索引值
enum DATA_INDEX
{
	DATA_INDEX_CENTER,  // 中心服务器
	DATA_INDEX_SELECT,	// 选择服务器
	DATA_INDEX_LOGINDB, // 登陆DB服务器
	DATA_INDEX_LOGIN,	// 登陆服务器
	DATA_INDEX_GAMEDB,	// 游戏DB服务器
	DATA_INDEX_GAME,	// 游戏服务器
	DATA_INDEX_ZONE,	// 地图服务器
	DATA_INDEX_GATE,	// 网关服务器
	DATA_INDEX_MAX,
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 数据类型定义 : 当收到数据包时, 可以按类型来进行分类处理
//               例如对于中心服务器, 选择, 登陆和游戏服务器注册时, 就可以通过类型来区分
//               设置网络层对象对应的类型, 可以将一个数据包让网络层广播给特定类型的所以对象
enum PAK_TYPE
{
	PAK_TYPE_NONE           = 0x00000000, // NONE-空类型
	PAK_TYPE_CENTER         = 0x67788482, // CNTR-中心服务器
	PAK_TYPE_SELECT         = 0x83766784, // SLCT-选择服务器
	PAK_TYPE_LOGINDB        = 0x76786866, // LNDB-登陆DB服务器
	PAK_TYPE_LOGIN          = 0x76717378, // LGIN-登陆服务器
	PAK_TYPE_LOGIN_CENTER   = 0x76718482, // LGTR-登陆服务器(防止中心和登陆DB同进程都使用PAK_TYPE_LOGIN标识登陆服务器, 中心广播时对同个登陆服务器出现2次一样的广播)
	PAK_TYPE_GAMEDB         = 0x71776866, // GMDB-游戏DB服务器
	PAK_TYPE_GAME           = 0x71657769, // GAME-游戏服务器
	PAK_TYPE_ZONE           = 0x90797869, // ZONE-地图服务器
	PAK_TYPE_GATE           = 0x71658469, // GATE-网关服务器
	PAK_TYPE_CLIENT         = 0x67767884, // CLNT-游戏进行中的客户端
	PAK_TYPE_CLIENT_SELECT  = 0x67767883, // CLNS-角色选择中的客户端
	PAK_TYPE_CLIENT_QUEUE   = 0x67767881, // CLNQ-排队中的客户端
	PAK_TYPE_CLIENT_LOGIN   = 0x67767876, // CLNL-登陆中的客户端
	PAK_TYPE_CLIENT_READY   = 0x67767882, // CLNR-准备状态的客户端
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 服务器运行状态
enum STATUS_CONST
{
	STATUSC_NONE            = 0, // 无状态
	STATUSC_INIT,                // 初始状态
	STATUSC_RUN,                 // 运行状态
	STATUSC_PAUSE,               // 暂停状态
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 服务器数据更新状态
enum STATUS_UPDATE
{
	STATUSU_NONE            = 0x00000000, // 初始状态
	STATUSU_LINK            = 0x00000001, // 注册数据状态
	STATUSU_OKAY            = 0x00000002, // 数据处理完成状态
	STATUSU_SYNC            = 0x00000004, // 需要同步更新状态
	STATUSU_UNLINK          = 0x00000008, // 注销状态
	//										 
	STATUSU_PING            = 0x00000010, // 同步时只需要PING地址选项
	STATUSU_UPDATE          = 0x00000020, // 接收方同步数据时直接变成完成状态

	STATUSU_SYNCXLINK       = (STATUSU_SYNC|STATUSU_LINK|STATUSU_UNLINK), // 状态组合1
	STATUSU_OKAYSYNC        = (STATUSU_OKAY|STATUSU_SYNC),                // 状态组合2
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 各服务器信令定义范围
enum PAK_EVENT_SERVER
{
	// 中心服务器信令范围
	PAK_EVENT_CENTER_BEGIN  = (CNETTraits::EVENT_RESERVED + 100),
	PAK_EVENT_CENTER_END    = (PAK_EVENT_CENTER_BEGIN + 99),
	// 选择服务器信令范围
	PAK_EVENT_SELECT_BEGIN,
	PAK_EVENT_SELECT_END    = (PAK_EVENT_SELECT_BEGIN + 99),
	// 登陆数据服务器信令范围
	PAK_EVENT_LOGINDB_BEGIN,
	PAK_EVENT_LOGINDB_END   = (PAK_EVENT_LOGINDB_BEGIN + 99),
	// 登陆服务器信令范围
	PAK_EVENT_LOGIN_BEGIN,
	PAK_EVENT_LOGIN_END     = (PAK_EVENT_LOGIN_BEGIN + 99),
	// 游戏数据服务器信令范围
	PAK_EVENT_GAMEDB_BEGIN,
	PAK_EVENT_GAMEDB_END    = (PAK_EVENT_GAMEDB_BEGIN + 99),
	// 游戏服务器信令范围
	PAK_EVENT_GAME_BEGIN,
	PAK_EVENT_GAME_END      = (PAK_EVENT_GAME_BEGIN + 99),
	// 地图服务器信令范围
	PAK_EVENT_ZONE_BEGIN,
	PAK_EVENT_ZONE_END      = (PAK_EVENT_ZONE_BEGIN + 99),
	// 网关服务器信令范围
	PAK_EVENT_GATE_BEGIN,
	PAK_EVENT_GATE_END      = (PAK_EVENT_GATE_BEGIN + 99),
	// 游戏事务信令范围
	PAK_EVENT_REALM_BEGIN,
	PAK_EVENT_REALM_END     = (PAK_EVENT_REALM_BEGIN + 9999),
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// DataRef : 为统一32位和64位指针占用空间不同问题, 这里将统一定义为64位
#if (__INTRIN_TARGET__ == INTRIN_TARGET_X86)
	typedef LLong  DataRef;
#elif ((__INTRIN_TARGET__ == INTRIN_TARGET_X64) || (__INTRIN_TARGET__ == INTRIN_TARGET_IA64))
	typedef KeyRef DataRef;
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 服务器信息结构定义 : 信息统计使用
struct tagSERVER_DATA : public MObject
{
public:
	tagSERVER_DATA(void)
	: uAllCount(0)
	, uOnline(0)
	, usBusy(0)
	, usIncr(0)
	, usId(0)
	, usStatus(0)
	{
	}

	~tagSERVER_DATA(void)
	{
	}
public:
	UInt     uAllCount;  ///< 累计数量
	UInt     uOnline;    ///< 在线数量
	UShort   usBusy;     ///< [0 - 10000]服务器负载情况(万分比)
	UShort   usIncr;     ///< 每增加一个增长的负载(万分比)
	UShort   usId;       ///< 服务器Id, 游戏服务器-客户端用来索引名称, 其他服务器可以用来物理地址划分
	UShort   usStatus;   ///< 服务器状态STATUS_UPDATE
};
typedef struct tagSERVER_DATA   SERVER_DATA, *PSERVER_DATA;

#endif // __DATA_DEF_H__