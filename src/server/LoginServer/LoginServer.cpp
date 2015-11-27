//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Source File : LoginServer.cpp                              //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : µÇÂ½·þÎñÆ÷                                         //
//                                                              //
//////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommonServer.hxx"
#include "LoginServerImp.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CCommonServerSystem : CSubSystem
// CSubSystem
INLINE Int CCommonServerSystem::GetComponentInfo(MAP_COMPONENT_INFO& Components)
{
	// ILoginServer
	Components.Add(UUID_OF(ILoginServer), CStringKey(COM_OF( ILoginServer )));

	return Components.GetSize();
}

INLINE bool CCommonServerSystem::FindComponent(const CUUID& uuId)
{
	// ILoginServer
	if (uuId == UUID_OF( ILoginServer )) {
		return true;
	}
	return false;
}

INLINE bool CCommonServerSystem::CreateComponent(const CUUID& uuId, CComponentPtr& CComponentPtrRef)
{
	CComponentPtrRef = nullptr;
	// ILoginServer
	if (uuId == UUID_OF( ILoginServer )) {
		CLoginServer* pServer = MNEW CLoginServer;
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


