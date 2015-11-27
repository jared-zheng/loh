//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Source File : ServerConfig.cpp                             //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : 服务器配置文件                                     //
//                                                              //
//////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "datadef.h"
#include "ServerConfig.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CServerConfig
CPCXStr CServerConfig::ServerConfig               = TF("ServerConfig");
CPCXStr CServerConfig::NetworkPtr                 = TF("NetworkPtr");

#ifdef _DEBUG
CPCXStr CServerConfig::ConfigName                 = TF("ServerConfigD.cfg");
CPCXStr CServerConfig::CenterServerPath           = TF("CenterServerD.dll");
CPCXStr CServerConfig::SelectServerPath           = TF("SelectServerD.dll");
CPCXStr CServerConfig::LoginDBServerPath          = TF("LoginDBServerD.dll");
CPCXStr CServerConfig::LoginServerPath            = TF("LoginServerD.dll");
CPCXStr CServerConfig::GameDBServerPath           = TF("GameDBServerD.dll");
CPCXStr CServerConfig::GameServerPath             = TF("GameServerD.dll");
CPCXStr CServerConfig::ZoneServerPath             = TF("ZoneServerD.dll");
CPCXStr CServerConfig::GateServerPath             = TF("GateServerD.dll");
#else
CPCXStr CServerConfig::ConfigName                 = TF("ServerConfig.cfg");
CPCXStr CServerConfig::CenterServerPath           = TF("CenterServer.dll");
CPCXStr CServerConfig::SelectServerPath           = TF("SelectServer.dll");
CPCXStr CServerConfig::LoginDBServerPath          = TF("LoginDBServer.dll");
CPCXStr CServerConfig::LoginServerPath            = TF("LoginServer.dll");
CPCXStr CServerConfig::GameDBServerPath           = TF("GameDBServer.dll");
CPCXStr CServerConfig::GameServerPath             = TF("GameServer.dll");
CPCXStr CServerConfig::ZoneServerPath             = TF("ZoneServer.dll");
CPCXStr CServerConfig::GateServerPath             = TF("GateServer.dll");
#endif

CPCXStr CServerConfig::NetworkBlock               = TF("NetworkBlock");
CPCXStr CServerConfig::NetworkAttr                = TF("NetworkAttr");
CPCXStr CServerConfig::NetworkThread              = TF("NetworkThread");
CPCXStr CServerConfig::NetworkAck                 = TF("NetworkAck");
CPCXStr CServerConfig::NetworkTimeout             = TF("NetworkTimeout");
CPCXStr CServerConfig::NetworkBuffer              = TF("NetworkBuffer");
CPCXStr CServerConfig::NetworkJumbo               = TF("NetworkJumbo");

CPCXStr CServerConfig::ServersBlock               = TF("ServersBlock");
CPCXStr CServerConfig::UpdateTick                 = TF("UpdateTick");
CPCXStr CServerConfig::ReportTick                 = TF("ReportTick");
CPCXStr CServerConfig::LoadServers                = TF("LoadServers");
CPCXStr CServerConfig::CenterServer               = TF("CenterServer");
CPCXStr CServerConfig::SelectServer               = TF("SelectServer");
CPCXStr CServerConfig::LoginDBServer              = TF("LoginDBServer");
CPCXStr CServerConfig::LoginServer                = TF("LoginServer");
CPCXStr CServerConfig::GameDBServer               = TF("GameDBServer");
CPCXStr CServerConfig::GameServer                 = TF("GameServer");
CPCXStr CServerConfig::ZoneServer                 = TF("ZoneServer");
CPCXStr CServerConfig::GateServer                 = TF("GateServer");

CPCXStr CServerConfig::Service                    = TF("Service");
CPCXStr CServerConfig::Addr                       = TF("Addr");
CPCXStr CServerConfig::Port                       = TF("Port");
CPCXStr CServerConfig::Id                         = TF("Id");
CPCXStr CServerConfig::Incr                       = TF("Incr");
CPCXStr CServerConfig::Queue                      = TF("Queue");
CPCXStr CServerConfig::ExtConfig                  = TF("ExtConfig");
CPCXStr CServerConfig::LocalHost                  = TF("localhost");

bool CServerConfig::Init(PCXStr pszName)
{
	if ((m_pFilePath == nullptr) && (m_pKeyValue == nullptr)) {
		DEV_DEBUG(TF("FilePath or KeyValue nullptr"));
		return false;
	}

	if (pszName != nullptr) {
		m_strConfigName = pszName;
	}
	m_pFilePath->GetFullPath(CFilePath::ModulePath, m_strConfigName);

	CIni ini(*m_strConfigName);
	if (ini.IsExist()) {

		m_nNetworkAttr    = (Int)ini.GetKeyIntValue(NetworkBlock, NetworkAttr);
		m_nNetworkThread  = (Int)ini.GetKeyIntValue(NetworkBlock, NetworkThread);
		m_nNetworkAck     = (Int)ini.GetKeyIntValue(NetworkBlock, NetworkAck);
		m_nNetworkTimeout = (Int)ini.GetKeyIntValue(NetworkBlock, NetworkTimeout);
		m_nNetworkBuffer  = (Int)ini.GetKeyIntValue(NetworkBlock, NetworkBuffer);
		m_nNetworkJumbo   = (Int)ini.GetKeyIntValue(NetworkBlock, NetworkJumbo);

		m_nUpdateTick     = (Int)ini.GetKeyIntValue(ServersBlock, UpdateTick, CFG_UPDATE_DEF_TIME);
		m_nReportTick     = (Int)ini.GetKeyIntValue(ServersBlock, ReportTick, CFG_REPORT_DEF_TIME);

		DEF_BASE::Max<Int>(m_nUpdateTick, CFG_UPDATE_MIN_TIME);

		CString strTemp;
		m_nLoadServers = 0;
		ini.GetKeyValue(ServersBlock, LoadServers, strTemp);
		if (strTemp.Find(CenterServer) != -1) {
			m_nLoadServers |= CFG_DEFAULT_CENTER;
			ini.GetKeyValue(ServersBlock, CenterServer,  m_strCenterServerPath);
			if (m_strCenterServerPath.IsEmpty()) {
				m_strCenterServerPath = CenterServerPath;
			}
			LoadAddr(ini, CenterServer, SelectServer, m_strCenterServerSelectAddr, m_nCenterServerSelectPort, CFG_CENTER_SELECT_PORT);
			LoadAddr(ini, CenterServer, LoginServer,  m_strCenterServerLoginAddr,  m_nCenterServerLoginPort,  0);
			LoadAddr(ini, CenterServer, GameServer,   m_strCenterServerGameAddr,   m_nCenterServerGamePort,   0);
			m_nCenterServerId   = (Int)ini.GetKeyIntValue(CenterServer, Id, DATA_INDEX_CENTER);
			m_nCenterServerIncr = (Int)ini.GetKeyIntValue(CenterServer, Incr, CFG_BUSY_DEF_INCREMENT);
			ini.GetKeyValue(CenterServer, ExtConfig, m_strCenterServerExtConfig); 
		}
		else {
			CheckLoadAddr(ini, CenterServer, SelectServer, m_strCenterServerSelectAddr, m_nCenterServerSelectPort, CFG_CENTER_SELECT_PORT);
			CheckLoadAddr(ini, CenterServer, LoginServer,  m_strCenterServerLoginAddr,  m_nCenterServerLoginPort,  CFG_CENTER_LOGIN_PORT);
			CheckLoadAddr(ini, CenterServer, GameServer,   m_strCenterServerGameAddr,   m_nCenterServerGamePort,   CFG_CENTER_GAME_PORT);
		}
		if (strTemp.Find(SelectServer) != -1) {
			m_nLoadServers |= CFG_DEFAULT_SELECT;
			ini.GetKeyValue(ServersBlock, SelectServer,  m_strSelectServerPath);
			if (m_strSelectServerPath.IsEmpty()) {
				m_strSelectServerPath = SelectServerPath;
			}
			LoadAddr(ini, SelectServer, CenterServer, m_strSelectServerCenterAddr, m_nSelectServerCenterPort, 0); 
			LoadAddr(ini, SelectServer, Service,      m_strSelectServerClientAddr, m_nSelectServerClientPort, CFG_SELECT_PORT);
			m_nSelectServerId   = (Int)ini.GetKeyIntValue(SelectServer, Id, DATA_INDEX_SELECT);
			m_nSelectServerIncr = (Int)ini.GetKeyIntValue(SelectServer, Incr, CFG_BUSY_DEF_INCREMENT);
			ini.GetKeyValue(SelectServer, ExtConfig, m_strSelectServerExtConfig);
		}
		if (strTemp.Find(LoginDBServer) != -1) {
			m_nLoadServers |= CFG_DEFAULT_LOGINDB;
			ini.GetKeyValue(ServersBlock, LoginDBServer, m_strLoginDBServerPath);
			if (m_strLoginDBServerPath.IsEmpty()) {
				m_strLoginDBServerPath = LoginDBServerPath;
			}
			LoadAddr(ini, LoginDBServer, LoginServer, m_strLoginDBServerLoginAddr, m_nLoginDBServerLoginPort, CFG_LOGINDB_PORT);
			m_nLoginDBServerId    = (Int)ini.GetKeyIntValue(LoginDBServer, Id, DATA_INDEX_LOGINDB);
			m_nLoginDBServerIncr  = (Int)ini.GetKeyIntValue(LoginDBServer, Incr, CFG_BUSY_DEF_INCREMENT);
			m_nLoginDBServerQueue = (Int)ini.GetKeyIntValue(LoginDBServer, Queue, 0); 
			ini.GetKeyValue(LoginDBServer, ExtConfig, m_strLoginDBServerExtConfig);
		}
		else {
			CheckLoadAddr(ini, LoginDBServer, LoginServer, m_strLoginDBServerLoginAddr, m_nLoginDBServerLoginPort, CFG_LOGINDB_PORT);
		}
		if (strTemp.Find(LoginServer) != -1) {
			m_nLoadServers |= CFG_DEFAULT_LOGIN;
			ini.GetKeyValue(ServersBlock, LoginServer,   m_strLoginServerPath);
			if (m_strLoginServerPath.IsEmpty()) {
				m_strLoginServerPath = LoginServerPath;
			}
			LoadAddr(ini, LoginServer, CenterServer,  m_strLoginServerCenterAddr,  m_nLoginServerCenterPort,  0);
			LoadAddr(ini, LoginServer, LoginDBServer, m_strLoginServerLoginDBAddr, m_nLoginServerLoginDBPort, 0);
			LoadAddr(ini, LoginServer, GameServer,    m_strLoginServerGameAddr,    m_nLoginServerGamePort,   0);
			LoadAddr(ini, LoginServer, Service,       m_strLoginServerClientAddr,  m_nLoginServerClientPort, CFG_LOGIN_PORT);
			m_nLoginServerId   = (Int)ini.GetKeyIntValue(LoginServer, Id, DATA_INDEX_LOGIN);
			m_nLoginServerIncr = (Int)ini.GetKeyIntValue(LoginServer, Incr, CFG_BUSY_DEF_INCREMENT);
			ini.GetKeyValue(LoginServer, ExtConfig, m_strLoginServerExtConfig);
		}
		if (strTemp.Find(GameDBServer) != -1) {
			m_nLoadServers |= CFG_DEFAULT_GAMEDB;
			ini.GetKeyValue(ServersBlock, GameDBServer,  m_strGameDBServerPath);
			if (m_strGameDBServerPath.IsEmpty()) {
				m_strGameDBServerPath = GameDBServerPath;
			}
			LoadAddr(ini, GameDBServer, GameServer, m_strGameDBServerGameAddr, m_nGameDBServerGamePort, CFG_GAMEDB_GAME_PORT);
			LoadAddr(ini, GameDBServer, GateServer, m_strGameDBServerGateAddr, m_nGameDBServerGatePort, 0);
			m_nGameDBServerId    = (Int)ini.GetKeyIntValue(GameDBServer, Id, DATA_INDEX_GAMEDB);
			m_nGameDBServerIncr  = (Int)ini.GetKeyIntValue(GameDBServer, Incr, CFG_BUSY_DEF_INCREMENT);
			m_nGameDBServerQueue = (Int)ini.GetKeyIntValue(GameDBServer, Queue, 0);
			ini.GetKeyValue(GameDBServer, ExtConfig, m_strGameDBServerExtConfig);
		}
		else {
			CheckLoadAddr(ini, GameDBServer, GameServer, m_strGameDBServerGameAddr, m_nGameDBServerGamePort, CFG_GAMEDB_GAME_PORT);
			CheckLoadAddr(ini, GameDBServer, GateServer, m_strGameDBServerGateAddr, m_nGameDBServerGatePort, CFG_GAMEDB_GATE_PORT);
		}
		if (strTemp.Find(GameServer) != -1) {
			m_nLoadServers |= CFG_DEFAULT_GAME;
			ini.GetKeyValue(ServersBlock, GameServer,    m_strGameServerPath);
			if (m_strGameServerPath.IsEmpty()) {
				m_strGameServerPath = GameServerPath;
			}
			LoadAddr(ini, GameServer, CenterServer, m_strGameServerCenterAddr, m_nGameServerCenterPort, 0);
			LoadAddr(ini, GameServer, GameDBServer, m_strGameServerGameDBAddr, m_nGameServerGameDBPort, 0);
			LoadAddr(ini, GameServer, LoginServer,  m_strGameServerLoginAddr,  m_nGameServerLoginPort,  0);
			LoadAddr(ini, GameServer, ZoneServer,   m_strGameServerZoneAddr,   m_nGameServerZonePort,   CFG_GAME_ZONE_PORT);
			LoadAddr(ini, GameServer, GateServer,   m_strGameServerGateAddr,   m_nGameServerGatePort,   0);
			LoadAddr(ini, GameServer, Service,      m_strGameServerPingAddr,   m_nGameServerId,   0);
			m_nGameServerId   = (Int)ini.GetKeyIntValue(GameServer, Id, DATA_INDEX_GAME);
			m_nGameServerIncr = (Int)ini.GetKeyIntValue(GameServer, Incr, CFG_BUSY_DEF_INCREMENT);
			ini.GetKeyValue(GameServer, ExtConfig, m_strGameServerExtConfig);
		}
		else {
			CheckLoadAddr(ini, GameServer, ZoneServer,   m_strGameServerZoneAddr,   m_nGameServerZonePort,   CFG_GAME_ZONE_PORT);
			CheckLoadAddr(ini, GameServer, GateServer,   m_strGameServerGateAddr,   m_nGameServerGatePort,   CFG_GAME_GATE_PORT);
		}
		if (strTemp.Find(ZoneServer) != -1) {
			m_nLoadServers |= CFG_DEFAULT_ZONE;
			ini.GetKeyValue(ServersBlock, ZoneServer,    m_strZoneServerPath);
			if (m_strZoneServerPath.IsEmpty()) {
				m_strZoneServerPath = ZoneServerPath;
			}
			LoadAddr(ini, ZoneServer, GameServer, m_strZoneServerGameAddr, m_nZoneServerGamePort, 0);
			m_nZoneServerId   = (Int)ini.GetKeyIntValue(ZoneServer, Id, DATA_INDEX_ZONE);
			m_nZoneServerIncr = (Int)ini.GetKeyIntValue(ZoneServer, Incr, CFG_BUSY_DEF_INCREMENT);
			ini.GetKeyValue(ZoneServer, ExtConfig, m_strZoneServerExtConfig);
		}
		if (strTemp.Find(GateServer) != -1) {
			m_nLoadServers |= CFG_DEFAULT_GATE;
			ini.GetKeyValue(ServersBlock, GateServer,    m_strGateServerPath);
			if (m_strGateServerPath.IsEmpty()) {
				m_strGateServerPath = GateServerPath;
			}
			LoadAddr(ini, GateServer, GameDBServer, m_strGateServerGameDBAddr, m_nGateServerGameDBPort, 0);
			LoadAddr(ini, GateServer, GameServer,   m_strGateServerGameAddr,   m_nGateServerGamePort,   0);
			LoadAddr(ini, GateServer, LoginServer,  m_strGateServerLoginAddr,  m_nGateServerLoginPort,  0);
			LoadAddr(ini, GateServer, Service,      m_strGateServerClientAddr, m_nGateServerClientPort, CFG_GATE_PORT);
			m_nGateServerId    = (Int)ini.GetKeyIntValue(GateServer, Id, DATA_INDEX_GATE);
			m_nGateServerIncr  = (Int)ini.GetKeyIntValue(GateServer, Incr, CFG_BUSY_DEF_INCREMENT);
			m_nGateServerQueue = (Int)ini.GetKeyIntValue(GateServer, Queue, 0);
			ini.GetKeyValue(GateServer, ExtConfig, m_strGateServerExtConfig);
		}
	}
	if (m_strCenterServerPath.IsEmpty()) {
		m_strCenterServerPath = CenterServerPath;
	}
	if (m_strSelectServerPath.IsEmpty()) {
		m_strSelectServerPath = SelectServerPath;
	}
	if (m_strLoginDBServerPath.IsEmpty()) {
		m_strLoginDBServerPath = LoginDBServerPath;
	}
	if (m_strLoginServerPath.IsEmpty()) {
		m_strLoginServerPath = LoginServerPath;
	}
	if (m_strGameDBServerPath.IsEmpty()) {
		m_strGameDBServerPath = GameDBServerPath;
	}
	if (m_strGameServerPath.IsEmpty()) {
		m_strGameServerPath = GameServerPath;
	}
	if (m_strZoneServerPath.IsEmpty()) {
		m_strZoneServerPath = ZoneServerPath;
	}
	if (m_strGateServerPath.IsEmpty()) {
		m_strGateServerPath = GateServerPath;
	}

	if (m_strCenterServerSelectAddr.IsEmpty()) {
		m_strCenterServerSelectAddr = LocalHost;
	}
	if (m_strCenterServerLoginAddr.IsEmpty()) {
		m_strCenterServerLoginAddr = LocalHost;
	}
	if (m_strCenterServerGameAddr.IsEmpty()) {
		m_strCenterServerGameAddr = LocalHost;
	}

	if (m_strSelectServerCenterAddr.IsEmpty()) {
		m_strSelectServerCenterAddr = LocalHost;
	}
	if (m_strSelectServerClientAddr.IsEmpty()) {
		m_strSelectServerClientAddr = LocalHost;
	}

	if (m_strLoginDBServerLoginAddr.IsEmpty()) {
		m_strLoginDBServerLoginAddr = LocalHost;
	}

	if (m_strLoginServerCenterAddr.IsEmpty()) {
		m_strLoginServerCenterAddr = LocalHost;
	}
	if (m_strLoginServerLoginDBAddr.IsEmpty()) {
		m_strLoginServerLoginDBAddr = LocalHost;
	}
	if (m_strLoginServerGameAddr.IsEmpty()) {
		m_strLoginServerGameAddr = LocalHost;
	}
	if (m_strLoginServerClientAddr.IsEmpty()) {
		m_strLoginServerClientAddr = LocalHost;
	}

	if (m_strGameDBServerGameAddr.IsEmpty()) {
		m_strGameDBServerGameAddr = LocalHost;
	}
	if (m_strGameDBServerGateAddr.IsEmpty()) {
		m_strGameDBServerGateAddr = LocalHost;
	}

	if (m_strGameServerCenterAddr.IsEmpty()) {
		m_strGameServerCenterAddr = LocalHost;
	}
	if (m_strGameServerGameDBAddr.IsEmpty()) {
		m_strGameServerGameDBAddr = LocalHost;
	}
	if (m_strGameServerLoginAddr.IsEmpty()) {
		m_strGameServerLoginAddr = LocalHost;
	}
	if (m_strGameServerZoneAddr.IsEmpty()) {
		m_strGameServerZoneAddr = LocalHost;
	}
	if (m_strGameServerGateAddr.IsEmpty()) {
		m_strGameServerGateAddr = LocalHost;
	}
	if (m_strGameServerPingAddr.IsEmpty()) {
		m_strGameServerPingAddr = LocalHost;
	}

	if (m_strZoneServerGameAddr.IsEmpty()) {
		m_strZoneServerGameAddr = LocalHost;
	}

	if (m_strGateServerGameDBAddr.IsEmpty()) {
		m_strGateServerGameDBAddr = LocalHost;
	}
	if (m_strGateServerGameAddr.IsEmpty()) {
		m_strGateServerGameAddr = LocalHost;
	}
	if (m_strGateServerLoginAddr.IsEmpty()) {
		m_strGateServerLoginAddr = LocalHost;
	}
	if (m_strGateServerClientAddr.IsEmpty()) {
		m_strGateServerClientAddr = LocalHost;
	}

	CFileLog::SetLocal(TF("chs")); // 支持简体中文日志

	m_pKeyValue->AddItem(ServerConfig, (void*)this);
	return true;
}

void CServerConfig::Exit(void)
{
	if (m_pKeyValue != nullptr){
		m_pKeyValue->Remove(ServerConfig);
	}
}

void CServerConfig::Dump(CFileLog& Log)
{
	LOG_DUMP(Log, *m_strConfigName);
	LOGV_DUMP(Log, TF("[%s]%s=%d, %s=%d, %s=%d, %s=%d, %s=%d, %s=%d"), NetworkBlock, 
		      NetworkAttr,    m_nNetworkAttr, 
			  NetworkThread,  m_nNetworkThread,
			  NetworkAck,     m_nNetworkAck,
			  NetworkTimeout, m_nNetworkTimeout,
			  NetworkBuffer,  m_nNetworkBuffer,
			  NetworkJumbo,   m_nNetworkJumbo);

	LOGV_DUMP(Log, TF("[%s]%s=%d[S], %s=%d[MS], %s=%X"), ServersBlock,
			  UpdateTick,   m_nUpdateTick,
			  ReportTick,   m_nReportTick,
		      LoadServers,  m_nLoadServers);

	if (m_nLoadServers & CFG_DEFAULT_CENTER) {
		LOGV_DUMP(Log, TF("[(%d)%s-->%s]%s=%s[%d], %s=%s[%d], %s=%s[%d], %s=%d, %s=%s"), m_nCenterServerId,
				  CenterServer, *m_strCenterServerPath,
				  SelectServer, *m_strCenterServerSelectAddr, m_nCenterServerSelectPort,
				  LoginServer,  *m_strCenterServerLoginAddr,  m_nCenterServerLoginPort,
				  GameServer,   *m_strCenterServerGameAddr,   m_nCenterServerGamePort,
			      Incr,          m_nCenterServerIncr,
				  ExtConfig,    *m_strCenterServerExtConfig);
	}
	if (m_nLoadServers & CFG_DEFAULT_SELECT) {
		if ((m_nLoadServers & CFG_DEFAULT_CENTER) == 0) {
			LOGV_DUMP(Log, TF("[%s]%s=%s[%d]"), CenterServer, SelectServer, *m_strCenterServerSelectAddr, m_nCenterServerSelectPort);
		}
		LOGV_DUMP(Log, TF("[(%d)%s-->%s]%s=%s[%d], %s=%s[%d], %s=%d, %s=%s"), m_nSelectServerId,
				  SelectServer, *m_strSelectServerPath,
				  CenterServer, *m_strSelectServerCenterAddr, m_nSelectServerCenterPort,
				  Service,      *m_strSelectServerClientAddr, m_nSelectServerClientPort,
			      Incr,          m_nSelectServerIncr,
				  ExtConfig,    *m_strSelectServerExtConfig);
	}
	if (m_nLoadServers & CFG_DEFAULT_LOGINDB) {
		LOGV_DUMP(Log, TF("[(%d)%s-->%s]%s=%s[%d], %s=%d, %s=%d, %s=%s"), m_nLoginDBServerId,
				  LoginDBServer, *m_strLoginDBServerPath,
				  LoginServer,   *m_strLoginDBServerLoginAddr, m_nLoginDBServerLoginPort,
			      Incr,           m_nLoginDBServerIncr, 
			      Queue,          m_nLoginDBServerQueue,
				  ExtConfig,     *m_strLoginDBServerExtConfig);
	}
	if (m_nLoadServers & CFG_DEFAULT_LOGIN) {
		if ((m_nLoadServers & CFG_DEFAULT_CENTER) == 0) {
			LOGV_DUMP(Log, TF("[%s]%s=%s[%d]"), CenterServer, LoginServer, *m_strCenterServerLoginAddr, m_nCenterServerLoginPort);
		}
		if ((m_nLoadServers & CFG_DEFAULT_LOGINDB) == 0) {
			LOGV_DUMP(Log, TF("[%s]%s=%s[%d]"), LoginDBServer, LoginServer,   *m_strLoginDBServerLoginAddr, m_nLoginDBServerLoginPort);
		}
		LOGV_DUMP(Log, TF("[(%d)%s-->%s]%s=%s[%d], %s=%s[%d], %s=%s[%d], %s=%s[%d], %s=%d, %s=%s"), m_nLoginServerId,
				  LoginServer,   *m_strLoginServerPath,
			      CenterServer,  *m_strLoginServerCenterAddr,  m_nLoginServerCenterPort,
			      LoginDBServer, *m_strLoginServerLoginDBAddr, m_nLoginServerLoginDBPort,
				  GameServer,    *m_strLoginServerGameAddr,    m_nLoginServerGamePort,
				  Service,       *m_strLoginServerClientAddr,  m_nLoginServerClientPort,
			      Incr,           m_nLoginServerIncr,
				  ExtConfig,     *m_strLoginServerExtConfig);
	}
	if (m_nLoadServers & CFG_DEFAULT_GAMEDB) {
		LOGV_DUMP(Log, TF("[(%d)%s-->%s]%s=%s[%d], %s=%s[%d], %s=%d, %s=%d, %s=%s"), m_nGameDBServerId,
				  GameDBServer, *m_strGameDBServerPath,
				  GameServer,   *m_strGameDBServerGameAddr, m_nGameDBServerGamePort,
				  GateServer,   *m_strGameDBServerGateAddr, m_nGameDBServerGatePort,
			      Incr,          m_nGameDBServerIncr,
			      Queue,         m_nGameDBServerQueue,
				  ExtConfig,    *m_strGameDBServerExtConfig);
	}
	if (m_nLoadServers & CFG_DEFAULT_GAME) {
		if ((m_nLoadServers & CFG_DEFAULT_CENTER) == 0) {
			LOGV_DUMP(Log, TF("[%s]%s=%s[%d]"), CenterServer, GameServer,   *m_strCenterServerGameAddr,   m_nCenterServerGamePort);
		}
		if ((m_nLoadServers & CFG_DEFAULT_GAMEDB) == 0) {
			LOGV_DUMP(Log, TF("[%s]%s=%s[%d]"), GameDBServer, GameServer,   *m_strGameDBServerGameAddr, m_nGameDBServerGamePort);
		}
		LOGV_DUMP(Log, TF("[(%d)%s-->%s]%s=%s[%d], %s=%s[%d], %s=%s[%d], %s=%s[%d], %s=%s[%d], %s=%s, %s=%d, %s=%s"), m_nGameServerId,
				  GameServer,   *m_strGameServerPath,
				  CenterServer, *m_strGameServerCenterAddr, m_nGameServerCenterPort,
			      GameDBServer, *m_strGameServerGameDBAddr, m_nGameServerGameDBPort,
				  LoginServer,  *m_strGameServerLoginAddr,  m_nGameServerLoginPort,
				  ZoneServer,   *m_strGameServerZoneAddr,   m_nGameServerZonePort,
				  GateServer,   *m_strGameServerGateAddr,   m_nGameServerGatePort,
				  Service,      *m_strGameServerPingAddr,
			      Incr,          m_nGameServerIncr,
				  ExtConfig,    *m_strGameServerExtConfig);
	}
	if (m_nLoadServers & CFG_DEFAULT_ZONE) {
		if ((m_nLoadServers & CFG_DEFAULT_GAME) == 0) {
			LOGV_DUMP(Log, TF("[%s]%s=%s[%d]"), GameServer, ZoneServer,   *m_strGameServerZoneAddr,   m_nGameServerZonePort);
		}
		LOGV_DUMP(Log, TF("[(%d)%s-->%s]%s=%s[%d], %s=%d, %s=%s"), m_nZoneServerId,
				  ZoneServer, *m_strZoneServerPath,
				  GameServer, *m_strZoneServerGameAddr, m_nZoneServerGamePort,
			      Incr,        m_nZoneServerIncr,
				  ExtConfig,  *m_strZoneServerExtConfig);
	}
	if (m_nLoadServers & CFG_DEFAULT_GATE) {
		if ((m_nLoadServers & CFG_DEFAULT_GAMEDB) == 0) {
			LOGV_DUMP(Log, TF("[%s]%s=%s[%d]"), GameDBServer, GateServer,   *m_strGameDBServerGateAddr, m_nGameDBServerGatePort);
		}
		if ((m_nLoadServers & CFG_DEFAULT_GAME) == 0) {
			LOGV_DUMP(Log, TF("[%s]%s=%s[%d]"), GameServer, GateServer,   *m_strGameServerGateAddr,   m_nGameServerGatePort);
		}
		LOGV_DUMP(Log, TF("[(%d)%s-->%s]%s=%s[%d], %s=%s[%d], %s=%s[%d], %s=%s[%d], %s=%d, %s=%d, %s=%s"), m_nGateServerId,
				  GateServer,   *m_strGateServerPath,
			      GameDBServer, *m_strGateServerGameDBAddr, m_nGateServerGameDBPort,
				  GameServer,   *m_strGateServerGameAddr,   m_nGateServerGamePort,
				  LoginServer,  *m_strGateServerLoginAddr,  m_nGateServerLoginPort,
				  Service,      *m_strGateServerClientAddr, m_nGateServerClientPort,
			      Incr,          m_nGateServerIncr,
			      Queue,         m_nGateServerQueue,
				  ExtConfig,    *m_strGateServerExtConfig);
	}
}

bool CServerConfig::GetServerPath(Int nServer, CStringFix& strPath)
{
	bool bRet = true;
	switch (nServer) {
	case CFG_DEFAULT_CENTER:
		{
			strPath = m_strCenterServerPath;
		}
		break;
	case CFG_DEFAULT_SELECT:
		{
			strPath = m_strSelectServerPath;
		}
		break;
	case CFG_DEFAULT_LOGINDB:
		{
			strPath = m_strLoginDBServerPath;
		}
		break;
	case CFG_DEFAULT_LOGIN:
		{
			strPath = m_strLoginServerPath;
		}
		break;
	case CFG_DEFAULT_GAMEDB:
		{
			strPath = m_strGameDBServerPath;
		}
		break;
	case CFG_DEFAULT_GAME:
		{
			strPath = m_strGameServerPath;
		}
		break;
	case CFG_DEFAULT_ZONE:
		{
			strPath = m_strZoneServerPath;
		}
		break;
	case CFG_DEFAULT_GATE:
		{
			strPath = m_strGateServerPath;
		}
		break;
	default:
		{
			assert(0);
			bRet = false;
		}
	}
	return bRet;
}

bool CServerConfig::GetServerAddr(Int nServer, Int nServerAddr, CStringKey& strAddr, UShort& usPort)
{
	bool bRet = true;
	switch (nServer) {
	case CFG_DEFAULT_CENTER:
		{
			if (nServerAddr == CFG_DEFAULT_LOGIN) {
				strAddr = m_strCenterServerLoginAddr;
				usPort  = (UShort)m_nCenterServerLoginPort;
			}
			else if (nServerAddr == CFG_DEFAULT_GAME) {
				strAddr = m_strCenterServerGameAddr;
				usPort  = (UShort)m_nCenterServerGamePort;
			}
			else {
				strAddr = m_strCenterServerSelectAddr;
				usPort  = (UShort)m_nCenterServerSelectPort;
			}
		}
		break;
	case CFG_DEFAULT_SELECT:
		{
			if (nServerAddr == CFG_DEFAULT_CENTER) {
				strAddr = m_strSelectServerCenterAddr;
				usPort  = (UShort)m_nSelectServerCenterPort;
			}
			else {
				strAddr = m_strSelectServerClientAddr;
				usPort  = (UShort)m_nSelectServerClientPort;
			}
		}
		break;
	case CFG_DEFAULT_LOGINDB:
		{
			strAddr = m_strLoginDBServerLoginAddr;
			usPort  = (UShort)m_nLoginDBServerLoginPort;
		}
		break;
	case CFG_DEFAULT_LOGIN:
		{
			if (nServerAddr == CFG_DEFAULT_CENTER) {
				strAddr = m_strSelectServerCenterAddr;
				usPort  = (UShort)m_nSelectServerCenterPort;
			}
			else if (nServerAddr == CFG_DEFAULT_LOGINDB) {
				strAddr = m_strLoginServerLoginDBAddr;
				usPort  = (UShort)m_nLoginServerLoginDBPort;
			}
			else if (nServerAddr == CFG_DEFAULT_GAME) {
				strAddr = m_strLoginServerGameAddr;
				usPort  = (UShort)m_nLoginServerGamePort;
			}
			else {
				strAddr = m_strLoginServerClientAddr;
				usPort  = (UShort)m_nLoginServerClientPort;
			}
		}
		break;
	case CFG_DEFAULT_GAMEDB:
		{
			if (nServerAddr == CFG_DEFAULT_GATE) {
				strAddr = m_strGameDBServerGateAddr;
				usPort  = (UShort)m_nGameDBServerGatePort;
			}
			else {
				strAddr = m_strGameDBServerGameAddr;
				usPort  = (UShort)m_nGameDBServerGamePort;
			}
		}
		break;
	case CFG_DEFAULT_GAME:
		{
			if (nServerAddr == CFG_DEFAULT_CENTER) {
				strAddr = m_strGameServerCenterAddr;
				usPort  = (UShort)m_nGameServerCenterPort;
			}
			else if (nServerAddr == CFG_DEFAULT_GAMEDB) {
				strAddr = m_strGameServerGameDBAddr;
				usPort  = (UShort)m_nGameServerGameDBPort;
			}
			else if (nServerAddr == CFG_DEFAULT_LOGIN) {
				strAddr = m_strGameServerLoginAddr;
				usPort  = (UShort)m_nGameServerLoginPort;
			}
			else if (nServerAddr == CFG_DEFAULT_GATE) {
				strAddr = m_strGameServerGateAddr;
				usPort  = (UShort)m_nGameServerGatePort;
			}
			else if (nServerAddr == CFG_DEFAULT_ZONE) {
				strAddr = m_strGameServerZoneAddr;
				usPort  = (UShort)m_nGameServerZonePort;
			}
			else {
				strAddr = m_strGameServerPingAddr;
				usPort  = 0;
			}
		}
		break;
	case CFG_DEFAULT_ZONE:
		{
			strAddr = m_strZoneServerGameAddr;
			usPort  = (UShort)m_nZoneServerGamePort;
		}
		break;
	case CFG_DEFAULT_GATE:
		{
			if (nServerAddr == CFG_DEFAULT_GAMEDB) {
				strAddr = m_strGateServerGameDBAddr;
				usPort  = (UShort)m_nGateServerGameDBPort;
			}
			else if (nServerAddr == CFG_DEFAULT_GAME) {
				strAddr = m_strGateServerGameAddr;
				usPort  = (UShort)m_nGateServerGamePort;
			}
			else if (nServerAddr == CFG_DEFAULT_LOGIN) {
				strAddr = m_strGateServerLoginAddr;
				usPort  = (UShort)m_nGateServerLoginPort;
			}
			else {
				strAddr = m_strGateServerClientAddr;
				usPort  = (UShort)m_nGateServerClientPort;
			}
		}
		break;
	default:
		{
			assert(0);
			bRet = false;
		}
	}
	return bRet;
}

void CServerConfig::CheckLoadAddr(CIni& ini, PCXStr pszSection, PCXStr pszServer, CStringKey& strAddr, Int& nPort, Int nDefault)
{
	if (ini.IsSection(pszSection)) {
		LoadAddr(ini, pszSection, pszServer, strAddr, nPort, nDefault);
	}
}

void CServerConfig::LoadAddr(CIni& ini, PCXStr pszSection, PCXStr pszServer, CStringKey& strAddr, Int& nPort, Int nDefault)
{
	CStringKey strKey = pszServer;
	strKey += Addr;
	ini.GetKeyValue(pszSection, *strKey, strAddr);

	strKey  = pszServer;
	strKey += Port;
	nPort = (Int)ini.GetKeyIntValue(pszSection, *strKey, nDefault);
}