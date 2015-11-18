//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : servermap.h                                  //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : 信息数据存储                                      //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __SERVER_MAP_H__
#define __SERVER_MAP_H__

#pragma once

#include "datadef.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// tagSERVER_INFO : 服务器信息结构定义
struct tagSERVER_INFO : public tagSERVER_DATA
{
public:
	tagSERVER_INFO(void);
	~tagSERVER_INFO(void);

	void     Info(CStream& Stream, Int nStatus = STATUSU_SYNCXLINK);
	void     Copy(const tagSERVER_DATA& sd);
	// 统计时候用到的
	void     Incr(const tagSERVER_DATA& sd);
	void     Diff(const tagSERVER_DATA& sdIncr, const tagSERVER_DATA& sdDecr);
	void     Decr(const tagSERVER_DATA& sd);
	void     Drop(const tagSERVER_DATA& sd);

	void     Incr(void);    // 增加连接数, 增加负载
	void     Decr(void);    // 减少连接数, 减少负载
	void     Zero(void);    // 归零
	void     Reset(void);   //
};
typedef tagSERVER_INFO SERVER_INFO, *PSERVER_INFO;
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// tagSERVER_ADDR : 包含地址的服务器信息结构定义
template <size_t stLen>
struct tagSERVER_ADDR : public tagSERVER_INFO
{
public:
	tagSERVER_ADDR(void);
	~tagSERVER_ADDR(void);

	void Addr(CStream& Stream, Int nStatus = STATUSU_SYNCXLINK);
public:
//	Long                   lRefCount;
	CNETTraits::NET_ADDR   NetAddr[stLen];
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 这里服务器数据的差别只是网络地址数量的多少, 所以使用简单的模板结构来描述各服务器的信息结构
// 如果特定的服务器需要包含特殊的数据(例如包含服务器名称的), 另外声明服务器信息结构
// TEST_INDEX
enum TEST_INDEX
{
	TESTI_UNKNOWN,
	TESTI_COUNT,
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// TEST_ADDR : 服务器信息结构定义
typedef tagSERVER_ADDR<TESTI_COUNT>   TEST_ADDR, *PTEST_ADDR;   //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// SELECT_INDEX
enum SELECT_INDEX 
{
	SELECTI_UDP,    // UDP监听客户端地址
	SELECTI_CENTER, // 连接中心的特定地址(多网卡可以指定特定网卡或者特定端口)
	SELECTI_COUNT,
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// SELECT_ADDR : 选择服务器信息结构定义
typedef tagSERVER_ADDR<SELECTI_COUNT> SELECT_ADDR, *PSELECT_ADDR; // 0-外部UDP地址; 1-内部链接中心服务器地址
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// LOGIN_INDEX
enum LOGIN_INDEX 
{
	LOGINI_TCP,          // TCP监听客户端地址
	LOGINI_UDP,          // UDP与游戏服务器通信
	LOGINI_CENTER,       // 连接中心的特定地址(多网卡可以指定特定网卡或者特定端口)
	LOGINI_LOGINDB,      // 连接登陆DB的特定地址(多网卡可以指定特定网卡或者特定端口)
	LOGINI_LOGINDB_ADDR, // 连接的登陆DB地址
	LOGINI_COUNT,
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// LOGIN_ADDR : 登陆服务器信息结构定义
typedef tagSERVER_ADDR<LOGINI_COUNT>  LOGIN_ADDR,  *PLOGIN_ADDR;  // 0-外部地址; 1-内部对game的UDP地址; 2-logindb地址; 3-内部链接中心服务器地址; 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// GAME_INDEX
enum GAME_INDEX 
{
	GAMEI_GATE,          // TCP监听网关服务器地址
	GAMEI_ZONE,          // TCP监听地图服务器地址
	GAMEI_UDP,           // UDP与登陆服务器通信
	GAMEI_CENTER,        // 连接中心的特定地址(多网卡可以指定特定网卡或者特定端口)
	GAMEI_GAMEDB,        // 连接游戏DB的特定地址(多网卡可以指定特定网卡或者特定端口)
	GAMEI_GAMEDB_ADDR,   // 连接的游戏DB地址
	GAMEI_COUNT,
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// GAME_ADDR : 游戏服务器信息结构定义
typedef tagSERVER_ADDR<GAMEI_COUNT>   GAME_ADDR,   *PGAME_ADDR;   // 0-对gate内部地址; 1-对zone内部地址; 2-内部对login的UDP地址; 3-gamedb地址; 4-内部链接中心服务器地址; 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// ZONE_INDEX
enum ZONE_INDEX 
{
	ZONEI_GAME, // 连接游戏的特定地址(多网卡可以指定特定网卡或者特定端口)
	ZONEI_COUNT,
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// ZONE_ADDR : 游戏地图服务器信息结构定义
typedef tagSERVER_ADDR<ZONEI_COUNT>   ZONE_ADDR,   *PZONE_ADDR;   // 0-内部链接游戏服务器地址

struct tagZONE_ADDR_INDEX : public ZONE_ADDR {
public:
	tagZONE_ADDR_INDEX(void);
	~tagZONE_ADDR_INDEX(void);
public:
	typedef CTArray<PINDEX>   ARY_INDEX;
public:
	ARY_INDEX   Index;
};

typedef tagZONE_ADDR_INDEX            ZONE_ADDR_INDEX, *PZONE_ADDR_INDEX;
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// GATE_INDEX
enum GATE_INDEX 
{
	GATEI_TCP,           // TCP监听客户端地址
	GATEI_UDP,           // UDP与登陆服务器通信
	GATEI_GAME,          // 连接游戏的特定地址(多网卡可以指定特定网卡或者特定端口)
	GATEI_GAMEDB,        // 连接游戏DB的特定地址(多网卡可以指定特定网卡或者特定端口)
	GATEI_GAMEDB_ADDR,   // 连接的游戏DB地址
	GATEI_COUNT,
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// GATE_ADDR : 游戏网关服务器信息结构定义
typedef tagSERVER_ADDR<GATEI_COUNT>   GATE_ADDR,   *PGATE_ADDR;   // 0-外部地址; 1-内部链接游戏服务器地址; 2-内部链接游戏DB服务器地址; 3-内部对login的UDP地址
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// tagSERVER_NOADDR : 不包含地址的服务器信息结构定义
struct tagSERVER_NOADDR : public tagSERVER_INFO
{
public:
	tagSERVER_NOADDR(void);
	~tagSERVER_NOADDR(void);

	void Addr(CStream& Stream, Int nStatus = STATUSU_SYNCXLINK);
//public:
//	Long                   lRefCount;
};
typedef tagSERVER_NOADDR SERVER_NOADDR, *PSERVER_NOADDR;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// tagSERVER_MAP : 服务器信息映射表结构定义
template <typename V>
struct tagSERVER_MAP : public MObject
{
public:
	tagSERVER_MAP(void);
	~tagSERVER_MAP(void);

	CSyncLock& GetLock(void);

	void  Update(void);

	// 1. 客户端传送增加STATUSU_NOADDR
	// 2. 发送同步+完成的服务器信息STATUSU_OKAYSYNC, Link完成时发送
	// 3. TODO!!!这么没有考虑数据超出流对象保存上限的问题, 这里只是使用jumbo buffer发送, 
	//    后期如果不满足分多个buffer/jumbo buffer发送, 默认jumbo buffer可以最多发送同类型120个服务器的全部信息,
	//    目前同步发送只有在中心服务器同步服务器信息给选择和登陆时存在, 还有登陆服务器同步游戏服务器信息给客户端
	bool  Serialize(CStream& Stream, Int nStatus = STATUSU_SYNCXLINK);
public:
	bool  Read(CStream& Stream, Int nStatus);
	bool  Write(CStream& Stream, Int nStatus);
public:
	typedef struct tagSVR_PAIR
	{
	public:
		void Serialize(CStream& Stream);
	public:
		DataRef   drKey;
		V         Value;
	}SVR_PAIR, *PSVR_PAIR;
	typedef CTMap<DataRef, V>                  SVR_MAP;
	typedef typename CTMap<DataRef, V>::PAIR   SVR_MAP_PAIR;
public:
	Int           Size(void);

	PINDEX        Add(DataRef drKey);
	PINDEX        Add(DataRef drKey, V& v);

	SVR_MAP_PAIR* Find(DataRef drKey);
	SVR_MAP_PAIR* At(PINDEX index);

	PINDEX        First(void);
	SVR_MAP_PAIR* Next(PINDEX& index);

	bool          Remove(DataRef drKey);
	bool          RemoveAt(PINDEX index);
	void          RemoveAll(void);
private:
	SVR_MAP       m_SvrMap;
	CSyncLock     m_SyncLock;
};
typedef tagSERVER_MAP<SELECT_ADDR>       SVR_SELECT_MAP,     *PSVR_SELECT_MAP;
typedef tagSERVER_MAP<LOGIN_ADDR>        SVR_LOGIN_MAP,      *PSVR_LOGIN_MAP;
typedef tagSERVER_MAP<GAME_ADDR>         SVR_GAME_MAP,       *PSVR_GAME_MAP;
typedef tagSERVER_MAP<ZONE_ADDR>         SVR_ZONE_MAP,       *PSVR_ZONE_MAP;
typedef tagSERVER_MAP<ZONE_ADDR_INDEX>   SVR_ZONE_INDEX_MAP, *PSVR_ZONE_INDEX_MAP;
typedef tagSERVER_MAP<GATE_ADDR>         SVR_GATE_MAP,       *PSVR_GATE_MAP;
typedef tagSERVER_MAP<TEST_ADDR>         SVR_TEST_MAP,       *PSVR_TEST_MAP;
typedef tagSERVER_MAP<SERVER_NOADDR>     SVR_SERVER_MAP,     *PSVR_SERVER_MAP;
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// DATA_MAP : 数据信息映射表
template <typename K, typename V, class KTraits = CTElementTraits<K>, class VTraits = CTElementTraits<V>>
struct tagDATA_MAP : public MObject
{
public:
	tagDATA_MAP(void);
	~tagDATA_MAP(void);

	CSyncLock& GetLock(void);

	bool Serialize(CStream& Stream);
public:
	typedef CTMap<K, V, KTraits, VTraits>                   DATA_MAP;
	typedef typename CTMap<K, V, KTraits, VTraits>::PAIR    DATA_MAP_PAIR;
public:
	Int            Size(void);
	void           Cache(Int nCache);

	PINDEX         Add(K key);
	PINDEX         Add(K key, V& val);

	DATA_MAP_PAIR* Find(K key);

	PINDEX         First(void);
	DATA_MAP_PAIR* Next(PINDEX& index);

	DATA_MAP_PAIR* Get(PINDEX index);

	bool           Remove(K key);
	bool           RemoveAt(PINDEX index);
	void           RemoveAll(void);
private:
	DATA_MAP       m_DataMap;
	CSyncLock      m_SyncLock;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "servermap.inl"

#endif // __SERVER_MAP_H__