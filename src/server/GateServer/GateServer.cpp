//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Source File : GateServer.cpp                               //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : Íø¹Ø·þÎñÆ÷                                         //
//                                                              //
//////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommonServer.hxx"
#include "GateServerImp.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CCommonServerSystem : CSubSystem
// CSubSystem
INLINE Int CCommonServerSystem::GetComponentInfo(MAP_COMPONENT_INFO& Components)
{
	// IGateServer
	Components.Add(UUID_OF(IGateServer), CStringKey(COM_OF( IGateServer )));

	return Components.GetSize();
}

INLINE bool CCommonServerSystem::FindComponent(const CUUID& uuId)
{
	// IGateServer
	if (uuId == UUID_OF( IGateServer )) {
		return true;
	}
	return false;
}

INLINE bool CCommonServerSystem::CreateComponent(const CUUID& uuId, CComponentPtr& CComponentPtrRef)
{
	CComponentPtrRef = nullptr;
	// IGateServer
	if (uuId == UUID_OF( IGateServer )) {
		CGateServer* pServer = MNEW CGateServer;
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


