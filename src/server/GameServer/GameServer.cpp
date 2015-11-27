//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Source File : GameServer.cpp                               //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : ÓÎÏ··þÎñÆ÷                                         //
//                                                              //
//////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommonServer.hxx"
#include "GameServerImp.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CCommonServerSystem : CSubSystem
// CSubSystem
INLINE Int CCommonServerSystem::GetComponentInfo(MAP_COMPONENT_INFO& Components)
{
	// IGameServer
	Components.Add(UUID_OF(IGameServer), CStringKey(COM_OF( IGameServer )));

	return Components.GetSize();
}

INLINE bool CCommonServerSystem::FindComponent(const CUUID& uuId)
{
	// IGameServer
	if (uuId == UUID_OF( IGameServer )) {
		return true;
	}
	return false;
}

INLINE bool CCommonServerSystem::CreateComponent(const CUUID& uuId, CComponentPtr& CComponentPtrRef)
{
	CComponentPtrRef = nullptr;
	// IGameServer
	if (uuId == UUID_OF( IGameServer )) {
		CGameServer* pServer = MNEW CGameServer;
		if (pServer != nullptr) {
			CComponentPtrRef = static_cast<ICommonServer*>(pServer);
			return true;
		}
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CreateSubSystem
CCommonServerSystem   GServerSystem;

API_EXPORT CSubSystem* CreateSubSystem(const CUUID& uuId)
{
	if (uuId == UUID_OF( CCommonServerSystem )) {
		return (&GServerSystem);
	}
	return nullptr;
}

BOOL WINAPI DllMain(HMODULE hModule, ULong ulReason, void* lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);

	if (ulReason == DLL_PROCESS_ATTACH) {
		::DisableThreadLibraryCalls(hModule);
	}
	else if (ulReason == DLL_PROCESS_DETACH) {
	}
	return TRUE;
}


