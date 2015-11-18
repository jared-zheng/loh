//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : ServerLoader.h                               //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : ·þÎñ¼ÓÔØÆ÷                                         //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __SERVER_LOADER_H__
#define __SERVER_LOADER_H__

#pragma once

#include "singleton.h"

#include "centerpacket.h"
#include "selectpacket.h"
#include "logindbpacket.h"
#include "loginpacket.h"
#include "gamedbpacket.h"
#include "gamepacket.h"
#include "zonepacket.h"
#include "gatepacket.h"

#include "ServerConfig.h"
#include "CommonServer.h"

#include "CommonDlg.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CServerLoader
class CServerLoader : public MObject
{
	DECLARE_CLASS_LOADER( CServerLoader, XUI )
	DECLARE_CLASS_LOADER( CServerLoader, NET )
	DECLARE_CLASS_LOADER( CServerLoader, Center )
	DECLARE_CLASS_LOADER( CServerLoader, Select )
	DECLARE_CLASS_LOADER( CServerLoader, LoginDB )
	DECLARE_CLASS_LOADER( CServerLoader, Login )
	DECLARE_CLASS_LOADER( CServerLoader, GameDB )
	DECLARE_CLASS_LOADER( CServerLoader, Game )
	DECLARE_CLASS_LOADER( CServerLoader, Zone )
	DECLARE_CLASS_LOADER( CServerLoader, Gate )
public:
	CServerLoader(void);
	~CServerLoader(void);

	bool Init(PCXStr pszName = nullptr);
	void Exit(void);

	bool LoadServer(Int nServer, CEventHandler* pUIHandler);
	void UnloadServers(void);

	bool StartServers(void);
	void StopServers(void);
private:
	bool InitSystems(void);
	void ExitSystems(void);
public:
	CFilePath           m_FilePath;
	CKeyValue           m_KeyValue;
	CServerConfig       m_Config;

	CXUIManagerPtr      m_UIManagerPtr;
	CNetworkPtr         m_NetworkPtr;

	CCommonServerPtr    m_CenterServer;
	CCommonServerPtr    m_SelectServer;
	CCommonServerPtr    m_LoginDBServer;
	CCommonServerPtr    m_LoginServer;
	CCommonServerPtr    m_GameDBServer;
	CCommonServerPtr    m_GameServer;
	CCommonServerPtr    m_ZoneServer;
	CCommonServerPtr    m_GateServer;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CServerLoaderSingleton
class CServerLoaderSingleton : public CTSingletonInst<CServerLoader>
{
//public:
//	CServerLoaderSingleton(void);
//	~CServerLoaderSingleton(void);
//	CServerLoaderSingleton(const CServerLoaderSingleton&);
//	CServerLoaderSingleton& operator=(const CServerLoaderSingleton&);
};
typedef CServerLoaderSingleton GServerLoader;
#define GServerLoaderInst      CServerLoaderSingleton::GetInstance()

#endif // __SERVER_LOADER_H__
