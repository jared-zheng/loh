//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : servermap.h                                  //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : 信息数据存储                                      //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __SERVER_MAP_H__
#define __SERVER_MAP_H__

#pragma once

#include "datadef.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 服务器信息结构定义, 粗略统计使用, 未做多线程数据保护
struct tagSERVER_INFO : public tagSERVER_DATA
{
public:
	tagSERVER_INFO(void);
	~tagSERVER_INFO(void);
	/// 按特定状态要求读写
	/// @param[in] CStream& Stream数据流
	/// @param[in] Int nStatus特定状态
	/// @reremarks 默认只对注册数据状态, 同步更新状态和注销状态进行操作
	void     Info(CStream& Stream, Int nStatus = STATUSU_SYNCXLINK);
	/// 拷贝数据
	/// @param[in] tagSERVER_DATA& sd 服务器数据
	void     Copy(const tagSERVER_DATA& sd);
	/// 服务器组统计增加一台同类型服务器, 客户端在线和总数增加情况
	/// @param[in] tagSERVER_DATA& sd 增加的服务器数据
	void     Incr(const tagSERVER_DATA& sd);
	/// 服务器组统计一台同类型服务器数据更新, 客户端在线和总数变化情况(差值变化)
	/// @param[in] tagSERVER_DATA& sdIncr 服务器更新以后的数据
	/// @param[in] tagSERVER_DATA& sdDecr 服务器更新以前的数据
	void     Diff(const tagSERVER_DATA& sdIncr, const tagSERVER_DATA& sdDecr);
	/// 服务器组统计减少一台同类型服务器, 客户端在线和总数减少情况
	/// @param[in] tagSERVER_DATA& sd 减少的服务器数据
	void     Decr(const tagSERVER_DATA& sd);
	/// 增加客户端数, 增加负载
	void     Incr(void);    
	/// 减少客户端数, 减少负载 
	void     Decr(void); 
	/// 在线数据归零
	void     Zero(void);
	/// 重置数据
	void     Reset(void);
};
typedef tagSERVER_INFO SERVER_INFO, *PSERVER_INFO;
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 不包含地址的服务器信息结构定义
struct tagSERVER_NOADDR : public tagSERVER_INFO
{
public:
	tagSERVER_NOADDR(void);
	~tagSERVER_NOADDR(void);
	/// 包含地址信息的个数=0
	Int  AddrLen(void);
	/// 按特定状态要求读写
	/// @param[in] CStream& Stream数据流
	/// @param[in] Int nStatus特定状态
	void Addr(CStream& Stream, Int nStatus = STATUSU_SYNCXLINK);
//public:
//	Long                   lRefCount;
};
typedef tagSERVER_NOADDR SERVER_NOADDR, *PSERVER_NOADDR;
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 包含地址的服务器信息结构定义
template <size_t stLen>
struct tagSERVER_ADDR : public tagSERVER_INFO
{
public:
	tagSERVER_ADDR(void);
	~tagSERVER_ADDR(void);
	/// 包含地址信息的个数
	Int  AddrLen(void);
	/// 按特定状态要求读写
	/// @param[in] CStream& Stream数据流
	/// @param[in] Int nStatus特定状态
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
	TESTI_PING,
	TESTI_COUNT,
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 服务器信息结构定义
typedef tagSERVER_ADDR<TESTI_COUNT>   TEST_ADDR, *PTEST_ADDR;
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// SELECT_INDEX
enum SELECT_INDEX 
{
	SELECTI_UDP,    // UDP监听客户端地址
	SELECTI_COUNT,
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 选择服务器信息结构定义
typedef tagSERVER_ADDR<SELECTI_COUNT> SELECT_ADDR, *PSELECT_ADDR;
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// LOGIN_INDEX
enum LOGIN_INDEX 
{
	LOGINI_TCP,          // TCP监听客户端地址
	LOGINI_UDP,          // UDP与游戏服务器通信
	LOGINI_LOGINDB,      // 登陆DB地址
	LOGINI_COUNT,
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 登陆服务器信息结构定义
typedef tagSERVER_ADDR<LOGINI_COUNT>  LOGIN_ADDR,  *PLOGIN_ADDR;
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// GAME_INDEX
enum GAME_INDEX 
{
	GAMEI_PING,          // PING服务器地址
	GAMEI_ZONE,          // TCP监听地图服务器地址
	GAMEI_GATE,          // TCP监听网关服务器地址
	GAMEI_UDP,           // UDP与登陆服务器通信       
	GAMEI_GAMEDB,        // 游戏DB地址
	GAMEI_COUNT,
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 游戏服务器信息结构定义
typedef tagSERVER_ADDR<GAMEI_COUNT>   GAME_ADDR,   *PGAME_ADDR;
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// ZONE_INDEX
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 游戏地图服务器信息结构定义
typedef tagSERVER_NOADDR   ZONE_ADDR,   *PZONE_ADDR;
// 游戏地图服务器扩展信息结构定义
struct tagZONE_ADDR_INDEX : public ZONE_ADDR {
public:
	tagZONE_ADDR_INDEX(void);
	~tagZONE_ADDR_INDEX(void);
public:
	typedef CTArray<PINDEX>   ARY_INDEX;
public:
	ARY_INDEX   Index;
};

typedef tagZONE_ADDR_INDEX   ZONE_ADDR_INDEX, *PZONE_ADDR_INDEX;
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// GATE_INDEX
enum GATE_INDEX 
{
	GATEI_TCP,           // TCP监听客户端地址
	GATEI_UDP,           // UDP与登陆/游戏服务器通信
	GATEI_GAMEDB,        // 游戏DB地址
	GATEI_COUNT,
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 游戏网关服务器信息结构定义
typedef tagSERVER_ADDR<GATEI_COUNT>   GATE_ADDR,   *PGATE_ADDR;


///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 服务器信息映射表结构定义
template <typename V>
struct tagSERVER_MAP : public MObject
{
public:
	tagSERVER_MAP(void);
	~tagSERVER_MAP(void);
	/// 返回读写锁对象
	CSyncLock& GetLock(void);
	/// 更新STATUSU_LINK&STATUSU_SYNC为STATUSU_OKAY状态, STATUSU_UNLINK状态删除
	void  Update(void);

	/// 按特定状态要求读写
	/// @param[in] CStream& Stream数据流
	/// @param[in] Int nStatus特定状态
	/// @return 有读取或者写入数据返回true, 否则返回false
	/// @remarks
	/// -# 登陆服务器将游戏服务器信息发送给客户端增加STATUSU_PING, 只发送PING服务器地址
	/// -# 发送同步+完成的服务器信息STATUSU_OKAYSYNC, 注册完成时发送
	/// -# TODO!!! /// @todo 这么没有考虑数据超出流对象保存上限的问题, 这里只是使用jumbo buffer发送, 
	///    后期如果不满足分多个buffer/jumbo buffer发送, 默认jumbo buffer可以最多发送同类型120个服务器的全部信息,
	///    目前同步发送只有在中心服务器同步服务器信息给选择和登陆时存在, 还有登陆服务器同步游戏服务器信息给客户端
	bool  Serialize(CStream& Stream, Int nStatus = STATUSU_SYNCXLINK);
public:
	/// 按特定状态要求读取服务器数据
	/// @param[in] CStream& Stream数据流
	/// @param[in] Int nStatus特定状态
	/// @return 有读取或者写入数据返回true, 否则返回false
	bool  Read(CStream& Stream, Int nStatus);
	/// 按特定状态要求写入服务器数据
	/// @param[in] CStream& Stream数据流
	/// @param[in] Int nStatus特定状态
	/// @return 有读取或者写入数据返回true, 否则返回false
	bool  Write(CStream& Stream, Int nStatus);
public:
	/// 单个服务器数据的key-value数据结构
	struct tagSVR_PAIR : public MObject
	{
	public:
		void Serialize(CStream& Stream);
	public:
		DataRef   drKey;
		V         Value;
	};
	typedef struct tagSVR_PAIR   SVR_PAIR, *PSVR_PAIR;
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
	SVR_MAP       m_SvrMap;   ///< 服务器信息映射表
	CSyncLock     m_SyncLock; ///< 读写锁
};
typedef tagSERVER_MAP<SERVER_NOADDR>     SVR_SERVER_MAP,     *PSVR_SERVER_MAP;
typedef tagSERVER_MAP<TEST_ADDR>         SVR_TEST_MAP,       *PSVR_TEST_MAP;
typedef tagSERVER_MAP<SELECT_ADDR>       SVR_SELECT_MAP,     *PSVR_SELECT_MAP;
typedef tagSERVER_MAP<LOGIN_ADDR>        SVR_LOGIN_MAP,      *PSVR_LOGIN_MAP;
typedef tagSERVER_MAP<GAME_ADDR>         SVR_GAME_MAP,       *PSVR_GAME_MAP;
typedef tagSERVER_MAP<ZONE_ADDR>         SVR_ZONE_MAP,       *PSVR_ZONE_MAP;
typedef tagSERVER_MAP<ZONE_ADDR_INDEX>   SVR_ZONE_INDEX_MAP, *PSVR_ZONE_INDEX_MAP;
typedef tagSERVER_MAP<GATE_ADDR>         SVR_GATE_MAP,       *PSVR_GATE_MAP;
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 数据信息映射表
template <typename K, typename V, class KTraits = CTElementTraits<K>, class VTraits = CTElementTraits<V>>
struct tagDATA_MAP : public MObject
{
public:
	tagDATA_MAP(void);
	~tagDATA_MAP(void);
	/// 返回读写锁对象
	CSyncLock& GetLock(void);
	/// 读取或者写入数据
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
	DATA_MAP       m_DataMap;  ///< 数据映射表
	CSyncLock      m_SyncLock; ///< 读写锁
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "servermap.inl"

#endif // __SERVER_MAP_H__