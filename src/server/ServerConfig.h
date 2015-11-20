//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : ServerConfig.h                               //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : 服务器配置文件                                     //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __SERVER_CONFIG_H__
#define __SERVER_CONFIG_H__

#pragma once

#include "windows\ini.h"
#include "filelog.h"
#include "filepath.h"
#include "keyvalue.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CServerConfig
class NOVTABLE CServerConfig : public MObject
{
public:
	enum CFG_DEFAULT
	{
		CFG_DEFAULT_NONE       = 0,
		// load server
		CFG_DEFAULT_CENTER     = 0x00000001,
		CFG_DEFAULT_SELECT     = 0x00000002,
		CFG_DEFAULT_LOGINDB    = 0x00000004,
		CFG_DEFAULT_LOGIN      = 0x00000008,
		CFG_DEFAULT_GAMEDB     = 0x00000010,
		CFG_DEFAULT_GAME       = 0x00000020,
		CFG_DEFAULT_ZONE       = 0x00000040,
		CFG_DEFAULT_GATE       = 0x00000080,
		CFG_DEFAULT_ALLSERVER  = 0x000000FF,
		CFG_DEFAULT_CONFIG     = 0x10000000,
		// center
		CFG_CENTER_SELECT_PORT = 11110,   // 中心服务器默认监听端口(选择)
		CFG_CENTER_LOGIN_PORT  = 11111,   // 中心服务器默认监听端口(登陆)
		CFG_CENTER_GAME_PORT   = 11112,   // 中心服务器默认监听端口(游戏)
		// select
		CFG_SELECT_PORT        = 10010,   // 选择服务器默认UDP端口(客户端)
		// logindb
		CFG_LOGINDB_PORT       = 11120,   // 登陆DB服务器默认监听端口(登陆)
		// login
		CFG_LOGIN_PORT         = 10020,   // 登陆服务器默认监听客户端端口(客户端)  
		// gamedb
		CFG_GAMEDB_GAME_PORT   = 11130,   // 游戏DB服务器默认监听端口(游戏)
		CFG_GAMEDB_GATE_PORT   = 11131,   // 游戏DB服务器默认监听端口(网关)
		// game
		CFG_GAME_ZONE_PORT     = 11140,   // 游戏服务器默认监听端口(地图)
		CFG_GAME_GATE_PORT     = 11141,   // 游戏服务器默认监听端口(网关)
		// gate
		CFG_GATE_PORT          = 10030,   // 网关服务器默认监听端口(客户端)
		// update
		CFG_UPDATE_MIN_TIME    = 16,      // 最小同步更新周期
		CFG_UPDATE_DEF_TIME    = 30,      // 同步更新周期默认30秒
		CFG_BUSY_DEF_INCREMENT = 40,      // 每个连接增加的繁忙程度默认值, 万分之一

		CFG_LIVE_MIN_TIME      = 60000,	  // 最小心跳包1分钟
		CFG_LIVE_DEF_TIME      = 300000,  // 默认心跳包5分钟
		CFG_REPORT_MIN_TIME    = 300000,  // 上报服务器信息到数据库的最小时间间隔
		CFG_REPORT_DEF_TIME    = 1800000, // 上报服务器信息到数据库的默认时间间隔
	};
	// 动态变量表
	static CPCXStr ServerConfig;          // 配置对象
	static CPCXStr NetworkPtr;            // 网络对象

	static CPCXStr ConfigName;            // 配置文件名称
	static CPCXStr CenterServerPath;      // 中心服务器模块路径
	static CPCXStr SelectServerPath;      // 选择服务器模块路径
	static CPCXStr LoginDBServerPath;     // 登陆DB服务器模块路径
	static CPCXStr LoginServerPath;       // 登陆服务器模块路径
	static CPCXStr GameDBServerPath;      // 游戏DB服务器模块路径
	static CPCXStr GameServerPath;        // 游戏服务器模块路径
	static CPCXStr ZoneServerPath;        // 地图服务器模块路径
	static CPCXStr GateServerPath;        // 网关服务器模块路径

	static CPCXStr NetworkBlock;          // NetworkBlock
	static CPCXStr NetworkAttr;           // NetworkAttr=0
	static CPCXStr NetworkThread;         // NetworkThread=0(cores * 2)
	static CPCXStr NetworkAck;            // NetworkAck
	static CPCXStr NetworkTimeout;        // NetworkTimeout
	static CPCXStr NetworkBuffer;         // NetworkBuffer
	static CPCXStr NetworkJumbo;          // NetworkJumbo

	static CPCXStr ServersBlock;          // ServersBlock
	static CPCXStr UpdateTick;            // UpdateTick=30(秒)
	static CPCXStr ReportTick;            // ReportTick=30(分)
	static CPCXStr LoadServers;           // 加载的服务器种类; LoadServers=CenterServer|SelectServer
	static CPCXStr CenterServer;          // 加载的服务器DLL路径; CenterServer=CenterServer.dll
	static CPCXStr SelectServer;          // 选择服务器模块路径
	static CPCXStr LoginDBServer;         // 登陆DB服务器模块路径
	static CPCXStr LoginServer;           // 登陆服务器模块路径
	static CPCXStr GameDBServer;          // 游戏DB服务器模块路径
	static CPCXStr GameServer;            // 游戏服务器模块路径
	static CPCXStr ZoneServer;            // 地图服务器模块路径
	static CPCXStr GateServer;            // 网关服务器模块路径

	static CPCXStr Service;
	static CPCXStr Addr;                  // 要和NetworkAttr中ipv4 or ipv6匹配
	static CPCXStr Port;                  // 
	static CPCXStr Id;                    // 
	static CPCXStr Incr;                  // 
	static CPCXStr Queue;                 // 
	static CPCXStr ExtConfig;             // 服务器扩展配置文件路径
	static CPCXStr LocalHost;
public:
	CServerConfig(void);
	~CServerConfig(void);

	void SetFilePath(CFilePath* pFilePath);
	CFilePath* GetFilePath(void);
	void SetKeyValue(CKeyValue* pKeyValue);
	CKeyValue* GetKeyValue(void);

	bool   Init(PCXStr pszName = nullptr);
	void   Exit(void);

	void   Dump(CFileLog& Log);

	Int    GetNetworkAttr(void);
	Int    GetNetworkThread(void);
	Int    GetNetworkAck(void);
	Int    GetNetworkTimeout(void);
	Int    GetNetworkBuffer(void);
	Int    GetNetworkJumbo(void);

	Int    GetUpdateTick(void);
	Int    GetReportTick(void);
	Int    GetLoadServers(void);

	bool   GetServerPath(Int nServer, CStringFix& strPath);
	bool   GetServerAddr(Int nServer, Int nServerAddr, CStringKey& strAddr, UShort& usPort);

	UShort GetServerId(Int nServer);
	UShort GetServerIncr(Int nServer);
	Int    GetServerQueue(Int nServer);

	bool   GetServerExtConfig(Int nServer, CStringFix& strPath);
private:
	CServerConfig(const CServerConfig&);
	CServerConfig& operator=(const CServerConfig&);

	void   CheckLoadAddr(CIni& ini, PCXStr pszSection, PCXStr pszServer, CStringKey& strAddr, Int& nPort, Int nDefault);
	void   LoadAddr(CIni& ini, PCXStr pszSection, PCXStr pszServer, CStringKey& strAddr, Int& nPort, Int nDefault);
public:
	CFilePath*   m_pFilePath;
	CKeyValue*   m_pKeyValue;
	// network
	Int          m_nNetworkAttr;
	Int          m_nNetworkThread;
	Int          m_nNetworkAck;
	Int          m_nNetworkTimeout;
	Int          m_nNetworkBuffer;
	Int          m_nNetworkJumbo;
	// servers
	Int          m_nUpdateTick;
	Int          m_nReportTick;
	Int          m_nLoadServers;
	// center-server
	Int          m_nCenterServerSelectPort;
	Int          m_nCenterServerLoginPort;
	Int          m_nCenterServerGamePort;
	Int          m_nCenterServerId;
	Int          m_nCenterServerIncr;
	// select-server
	Int          m_nSelectServerCenterPort;
	Int          m_nSelectServerClientPort;
	Int          m_nSelectServerId;
	Int          m_nSelectServerIncr;
	// logindb-server
	Int          m_nLoginDBServerLoginPort;
	Int          m_nLoginDBServerId;
	Int          m_nLoginDBServerIncr;
	Int          m_nLoginDBServerQueue;
	// login-server
	Int          m_nLoginServerCenterPort;
	Int          m_nLoginServerLoginDBPort;
	Int          m_nLoginServerGamePort;
	Int          m_nLoginServerClientPort;
	Int          m_nLoginServerId;
	Int          m_nLoginServerIncr;
	// gamedb-server
	Int          m_nGameDBServerGamePort;
	Int          m_nGameDBServerGatePort;
	Int          m_nGameDBServerId;
	Int          m_nGameDBServerIncr;
	Int          m_nGameDBServerQueue;
	// game-server
	Int          m_nGameServerCenterPort;
	Int          m_nGameServerGameDBPort;
	Int          m_nGameServerLoginPort;
	Int          m_nGameServerZonePort;
	Int          m_nGameServerGatePort;
	Int          m_nGameServerId;
	Int          m_nGameServerIncr;
	// zone-server
	Int          m_nZoneServerGamePort;
	Int          m_nZoneServerId;
	Int          m_nZoneServerIncr;
	// gate-server
	Int          m_nGateServerGameDBPort;
	Int          m_nGateServerGamePort;
	Int          m_nGateServerLoginPort;
	Int          m_nGateServerClientPort;
	Int          m_nGateServerId;
	Int          m_nGateServerIncr;
	Int          m_nGateServerQueue;
	//
	CStringFix   m_strCenterServerPath;
	CStringFix   m_strSelectServerPath;
	CStringFix   m_strLoginDBServerPath;
	CStringFix   m_strLoginServerPath;
	CStringFix   m_strGameDBServerPath;
	CStringFix   m_strGameServerPath;
	CStringFix   m_strZoneServerPath;
	CStringFix   m_strGateServerPath;
	// center-server
	CStringKey   m_strCenterServerSelectAddr; // 中心服务器监听选择地址
	CStringKey   m_strCenterServerLoginAddr;  // 中心服务器监听登陆地址
	CStringKey   m_strCenterServerGameAddr;   // 中心服务器监听游戏地址
	CStringFix   m_strCenterServerExtConfig;
	// select-server
	CStringKey   m_strSelectServerCenterAddr; // 选择服务器连接中心地址
	CStringKey   m_strSelectServerClientAddr; // 选择服务器监听客户端UDP地址
	CStringFix   m_strSelectServerExtConfig;
	// logindb-server
	CStringKey   m_strLoginDBServerLoginAddr; // 登陆DB服务器监听登陆地址
	CStringFix   m_strLoginDBServerExtConfig;
	// login-server
	CStringKey   m_strLoginServerCenterAddr;  // 登陆服务器连接中心地址
	CStringKey   m_strLoginServerLoginDBAddr; // 登陆服务器连接登陆DB地址
	CStringKey   m_strLoginServerGameAddr;    // 登陆服务器监听游戏UDP地址
	CStringKey   m_strLoginServerClientAddr;  // 登陆服务器监听客户端地址
	CStringFix   m_strLoginServerExtConfig;
	// gamedb-server
	CStringKey   m_strGameDBServerGameAddr;   // 游戏DB服务器监听游戏地址
	CStringKey   m_strGameDBServerGateAddr;   // 游戏DB服务器监听网关地址
	CStringFix   m_strGameDBServerExtConfig;
	// game-server
	CStringKey   m_strGameServerCenterAddr;   // 游戏服务器连接中心地址
	CStringKey   m_strGameServerGameDBAddr;   // 游戏服务器连接游戏DB地址
	CStringKey   m_strGameServerLoginAddr;    // 游戏服务器监听登陆UDP地址
	CStringKey   m_strGameServerZoneAddr;     // 游戏服务器监听地图地址
	CStringKey   m_strGameServerGateAddr;     // 游戏服务器监听网关地址
	CStringKey   m_strGameServerPingAddr;     // 游戏服务器PING地址
	CStringFix   m_strGameServerExtConfig;
	// zone-server
	CStringKey   m_strZoneServerGameAddr;     // 地图服务器连接中心地址
	CStringFix   m_strZoneServerExtConfig;
	// gate-server
	CStringKey   m_strGateServerGameDBAddr;   // 网关服务器连接游戏DB地址
	CStringKey   m_strGateServerGameAddr;     // 网关服务器连接游戏地址
	CStringKey   m_strGateServerLoginAddr;    // 网关服务器监听登陆UDP地址
	CStringKey   m_strGateServerClientAddr;   // 网关服务器监听客户端地址
	CStringFix   m_strGateServerExtConfig;
	// config file name
	CStringFix   m_strConfigName;
};

INLINE CServerConfig::CServerConfig(void)
: m_pFilePath(nullptr)
, m_pKeyValue(nullptr)
, m_nNetworkAttr(CFG_DEFAULT_NONE)
, m_nNetworkThread(0)
, m_nNetworkAck(0)
, m_nNetworkTimeout(0)
, m_nNetworkBuffer(0)
, m_nNetworkJumbo(0)
, m_nUpdateTick(CFG_UPDATE_DEF_TIME)
, m_nReportTick(CFG_REPORT_DEF_TIME)
, m_nLoadServers(CFG_DEFAULT_ALLSERVER)
, m_nCenterServerSelectPort(CFG_CENTER_SELECT_PORT)
, m_nCenterServerLoginPort(CFG_CENTER_LOGIN_PORT)
, m_nCenterServerGamePort(CFG_CENTER_GAME_PORT)
, m_nCenterServerId(DATA_INDEX_CENTER)
, m_nCenterServerIncr(CFG_BUSY_DEF_INCREMENT)
, m_nSelectServerCenterPort(0)
, m_nSelectServerClientPort(CFG_SELECT_PORT)
, m_nSelectServerId(DATA_INDEX_SELECT)
, m_nSelectServerIncr(CFG_BUSY_DEF_INCREMENT)
, m_nLoginDBServerLoginPort(CFG_LOGINDB_PORT)
, m_nLoginDBServerId(DATA_INDEX_LOGINDB)
, m_nLoginDBServerIncr(CFG_BUSY_DEF_INCREMENT)
, m_nLoginDBServerQueue(1)
, m_nLoginServerCenterPort(0)
, m_nLoginServerLoginDBPort(0)
, m_nLoginServerGamePort(0)
, m_nLoginServerClientPort(CFG_LOGIN_PORT)
, m_nLoginServerId(DATA_INDEX_LOGIN)
, m_nLoginServerIncr(CFG_BUSY_DEF_INCREMENT)
, m_nGameDBServerGamePort(CFG_GAMEDB_GAME_PORT)
, m_nGameDBServerGatePort(CFG_GAMEDB_GATE_PORT)
, m_nGameDBServerId(DATA_INDEX_GAMEDB)
, m_nGameDBServerIncr(CFG_BUSY_DEF_INCREMENT)
, m_nGameDBServerQueue(1)
, m_nGameServerCenterPort(0)
, m_nGameServerGameDBPort(0)
, m_nGameServerLoginPort(0)
, m_nGameServerZonePort(CFG_GAME_ZONE_PORT)
, m_nGameServerGatePort(CFG_GAME_GATE_PORT)
, m_nGameServerId(DATA_INDEX_GAME)
, m_nGameServerIncr(CFG_BUSY_DEF_INCREMENT)
, m_nZoneServerGamePort(0)
, m_nZoneServerId(DATA_INDEX_ZONE)
, m_nZoneServerIncr(CFG_BUSY_DEF_INCREMENT)
, m_nGateServerGameDBPort(0)
, m_nGateServerGamePort(0)
, m_nGateServerLoginPort(0)
, m_nGateServerClientPort(CFG_GATE_PORT)
, m_nGateServerId(DATA_INDEX_GATE)
, m_nGateServerIncr(CFG_BUSY_DEF_INCREMENT)
, m_nGateServerQueue(1)
, m_strCenterServerPath(CenterServerPath)
, m_strSelectServerPath(SelectServerPath)
, m_strLoginDBServerPath(LoginDBServerPath)
, m_strLoginServerPath(LoginServerPath)
, m_strGameDBServerPath(GameDBServerPath)
, m_strGameServerPath(GameServerPath)
, m_strZoneServerPath(ZoneServerPath)
, m_strGateServerPath(GateServerPath)
, m_strCenterServerSelectAddr(LocalHost)
, m_strCenterServerLoginAddr(LocalHost)
, m_strCenterServerGameAddr(LocalHost)
, m_strSelectServerCenterAddr(LocalHost)
, m_strSelectServerClientAddr(LocalHost)
, m_strLoginDBServerLoginAddr(LocalHost)
, m_strLoginServerCenterAddr(LocalHost)
, m_strLoginServerLoginDBAddr(LocalHost)
, m_strLoginServerGameAddr(LocalHost)
, m_strLoginServerClientAddr(LocalHost)
, m_strGameServerCenterAddr(LocalHost)
, m_strGameServerGameDBAddr(LocalHost)
, m_strGameServerLoginAddr(LocalHost)
, m_strGameServerZoneAddr(LocalHost)
, m_strGameServerGateAddr(LocalHost)
, m_strGameServerPingAddr(LocalHost)
, m_strZoneServerGameAddr(LocalHost)
, m_strGateServerGameDBAddr(LocalHost)
, m_strGateServerGameAddr(LocalHost)
, m_strGateServerLoginAddr(LocalHost)
, m_strGateServerClientAddr(LocalHost)
, m_strConfigName(ConfigName)
{
}

INLINE CServerConfig::~CServerConfig(void)
{
}

INLINE CServerConfig::CServerConfig(const CServerConfig&)
: m_pFilePath(nullptr)
, m_pKeyValue(nullptr)
, m_nNetworkAttr(CFG_DEFAULT_NONE)
, m_nNetworkThread(0)
, m_nNetworkAck(0)
, m_nNetworkTimeout(0)
, m_nNetworkBuffer(0)
, m_nNetworkJumbo(0)
, m_nUpdateTick(CFG_UPDATE_DEF_TIME)
, m_nReportTick(CFG_REPORT_DEF_TIME)
, m_nLoadServers(CFG_DEFAULT_ALLSERVER)
, m_nCenterServerSelectPort(CFG_CENTER_SELECT_PORT)
, m_nCenterServerLoginPort(CFG_CENTER_LOGIN_PORT)
, m_nCenterServerGamePort(CFG_CENTER_GAME_PORT)
, m_nCenterServerId(DATA_INDEX_CENTER)
, m_nCenterServerIncr(CFG_BUSY_DEF_INCREMENT)
, m_nSelectServerCenterPort(0)
, m_nSelectServerClientPort(CFG_SELECT_PORT)
, m_nSelectServerId(DATA_INDEX_SELECT)
, m_nSelectServerIncr(CFG_BUSY_DEF_INCREMENT)
, m_nLoginDBServerLoginPort(CFG_LOGINDB_PORT)
, m_nLoginDBServerId(DATA_INDEX_LOGINDB)
, m_nLoginDBServerIncr(CFG_BUSY_DEF_INCREMENT)
, m_nLoginDBServerQueue(1)
, m_nLoginServerCenterPort(0)
, m_nLoginServerLoginDBPort(0)
, m_nLoginServerGamePort(0)
, m_nLoginServerClientPort(CFG_LOGIN_PORT)
, m_nLoginServerId(DATA_INDEX_LOGIN)
, m_nLoginServerIncr(CFG_BUSY_DEF_INCREMENT)
, m_nGameDBServerGamePort(CFG_GAMEDB_GAME_PORT)
, m_nGameDBServerGatePort(CFG_GAMEDB_GATE_PORT)
, m_nGameDBServerId(DATA_INDEX_GAMEDB)
, m_nGameDBServerIncr(CFG_BUSY_DEF_INCREMENT)
, m_nGameDBServerQueue(1)
, m_nGameServerCenterPort(0)
, m_nGameServerGameDBPort(0)
, m_nGameServerLoginPort(0)
, m_nGameServerZonePort(CFG_GAME_ZONE_PORT)
, m_nGameServerGatePort(CFG_GAME_GATE_PORT)
, m_nGameServerId(DATA_INDEX_GAME)
, m_nGameServerIncr(CFG_BUSY_DEF_INCREMENT)
, m_nZoneServerGamePort(0)
, m_nZoneServerId(DATA_INDEX_ZONE)
, m_nZoneServerIncr(CFG_BUSY_DEF_INCREMENT)
, m_nGateServerGameDBPort(0)
, m_nGateServerGamePort(0)
, m_nGateServerLoginPort(0)
, m_nGateServerClientPort(CFG_GATE_PORT)
, m_nGateServerId(DATA_INDEX_GATE)
, m_nGateServerIncr(CFG_BUSY_DEF_INCREMENT)
, m_nGateServerQueue(1)
, m_strCenterServerPath(CenterServerPath)
, m_strSelectServerPath(SelectServerPath)
, m_strLoginDBServerPath(LoginDBServerPath)
, m_strLoginServerPath(LoginServerPath)
, m_strGameDBServerPath(GameDBServerPath)
, m_strGameServerPath(GameServerPath)
, m_strZoneServerPath(ZoneServerPath)
, m_strGateServerPath(GateServerPath)
, m_strCenterServerSelectAddr(LocalHost)
, m_strCenterServerLoginAddr(LocalHost)
, m_strCenterServerGameAddr(LocalHost)
, m_strSelectServerCenterAddr(LocalHost)
, m_strSelectServerClientAddr(LocalHost)
, m_strLoginDBServerLoginAddr(LocalHost)
, m_strLoginServerCenterAddr(LocalHost)
, m_strLoginServerLoginDBAddr(LocalHost)
, m_strLoginServerGameAddr(LocalHost)
, m_strLoginServerClientAddr(LocalHost)
, m_strGameServerCenterAddr(LocalHost)
, m_strGameServerGameDBAddr(LocalHost)
, m_strGameServerLoginAddr(LocalHost)
, m_strGameServerZoneAddr(LocalHost)
, m_strGameServerGateAddr(LocalHost)
, m_strGameServerPingAddr(LocalHost)
, m_strZoneServerGameAddr(LocalHost)
, m_strGateServerGameDBAddr(LocalHost)
, m_strGateServerGameAddr(LocalHost)
, m_strGateServerLoginAddr(LocalHost)
, m_strGateServerClientAddr(LocalHost)
, m_strConfigName(ConfigName)
{
}

INLINE CServerConfig& CServerConfig::operator=(const CServerConfig&)
{
	return (*this);
}

INLINE void CServerConfig::SetFilePath(CFilePath* pFilePath)
{
	m_pFilePath = pFilePath;
}

INLINE CFilePath* CServerConfig::GetFilePath(void)
{
	return m_pFilePath;
}

INLINE void CServerConfig::SetKeyValue(CKeyValue* pKeyValue)
{
	m_pKeyValue = pKeyValue;
}

INLINE CKeyValue* CServerConfig::GetKeyValue(void)
{
	return m_pKeyValue;
}

INLINE Int CServerConfig::GetNetworkAttr(void)
{
	return m_nNetworkAttr;
}

INLINE Int CServerConfig::GetNetworkThread(void)
{
	return m_nNetworkThread;
}

INLINE Int CServerConfig::GetNetworkAck(void)
{
	return m_nNetworkAck;
}

INLINE Int CServerConfig::GetNetworkTimeout(void)
{
	return m_nNetworkTimeout;
}

INLINE Int CServerConfig::GetNetworkBuffer(void)
{
	return m_nNetworkBuffer;
}

INLINE Int CServerConfig::GetNetworkJumbo(void)
{
	return m_nNetworkJumbo;
}

INLINE Int CServerConfig::GetUpdateTick(void)
{
	return m_nUpdateTick;
}

INLINE Int CServerConfig::GetReportTick(void)
{
	return m_nReportTick;
}

INLINE Int CServerConfig::GetLoadServers(void)
{
	return m_nLoadServers;
}

INLINE UShort CServerConfig::GetServerId(Int nServer)
{
	Int nRet = 0;
	switch (nServer) {
	case CFG_DEFAULT_CENTER:
		{
			nRet = m_nCenterServerId;
		}
		break;
	case CFG_DEFAULT_SELECT:
		{
			nRet = m_nSelectServerId;
		}
		break;
	case CFG_DEFAULT_LOGINDB:
		{
			nRet = m_nLoginDBServerId;
		}
		break;
	case CFG_DEFAULT_LOGIN:
		{
			nRet = m_nLoginServerId;
		}
		break;
	case CFG_DEFAULT_GAMEDB:
		{
			nRet = m_nGameDBServerId;
		}
		break;
	case CFG_DEFAULT_GAME:
		{
			nRet = m_nGameServerId;
		}
		break;
	case CFG_DEFAULT_ZONE:
		{
			nRet = m_nZoneServerId;
		}
		break;
	case CFG_DEFAULT_GATE:
		{
			nRet = m_nGateServerId;
		}
		break;
	default:
		{
			assert(0);
		}
	}
	return (UShort)nRet;
}

INLINE UShort CServerConfig::GetServerIncr(Int nServer)
{
	Int nRet = 0;
	switch (nServer) {
	case CFG_DEFAULT_CENTER:
		{
			nRet = m_nCenterServerIncr;
		}
		break;
	case CFG_DEFAULT_SELECT:
		{
			nRet = m_nSelectServerIncr;
		}
		break;
	case CFG_DEFAULT_LOGINDB:
		{
			nRet = m_nLoginDBServerIncr;
		}
		break;
	case CFG_DEFAULT_LOGIN:
		{
			nRet = m_nLoginServerIncr;
		}
		break;
	case CFG_DEFAULT_GAMEDB:
		{
			nRet = m_nGameDBServerIncr;
		}
		break;
	case CFG_DEFAULT_GAME:
		{
			nRet = m_nGameServerIncr;
		}
		break;
	case CFG_DEFAULT_ZONE:
		{
			nRet = m_nZoneServerIncr;
		}
		break;
	case CFG_DEFAULT_GATE:
		{
			nRet = m_nGateServerIncr;
		}
		break;
	default:
		{
			assert(0);
		}
	}
	return (UShort)nRet;
}

INLINE Int CServerConfig::GetServerQueue(Int nServer)
{
	Int nRet = 0;
	switch (nServer) {
	case CFG_DEFAULT_LOGINDB:
		{
			nRet = m_nLoginDBServerQueue;
		}
		break;
	case CFG_DEFAULT_GAMEDB:
		{
			nRet = m_nGameDBServerQueue;
		}
		break;
	case CFG_DEFAULT_GATE:
		{
			nRet = m_nGateServerQueue;
		}
		break;
	default:
		{
			assert(0);
		}
	}
	return nRet;
}

INLINE bool CServerConfig::GetServerExtConfig(Int nServer, CStringFix& strPath)
{
	switch (nServer) {
	case CFG_DEFAULT_CENTER:
		{
			strPath = m_strCenterServerExtConfig; // db info
		}
		break;
	case CFG_DEFAULT_SELECT:
		{
			strPath = m_strSelectServerExtConfig; // block ip list
		}
		break;
	case CFG_DEFAULT_LOGINDB:
		{
			strPath = m_strLoginDBServerExtConfig; // db info
		}
		break;
	case CFG_DEFAULT_LOGIN:
		{
			strPath = m_strLoginServerExtConfig; // block ip list
		}
		break;
	case CFG_DEFAULT_GAMEDB:
		{
			strPath = m_strGameDBServerExtConfig; // db info
		}
		break;
	case CFG_DEFAULT_GAME:
		{
			strPath = m_strGameServerExtConfig; // ...
		}
		break;
	case CFG_DEFAULT_ZONE:
		{
			strPath = m_strZoneServerExtConfig; // zone res
		}
		break;
	case CFG_DEFAULT_GATE:
		{
			strPath = m_strGateServerExtConfig; // block ip list
		}
		break;
	default:
		{
			assert(0);
		}
	}
	return (strPath.Length() != 0);
}

#endif // __SERVER_CONFIG_H__
