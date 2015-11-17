//////////////////////////////////////////////////////////////////
//   The Legend of Heros                                        //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Source File : LoginDBServer.cpp                            //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : µÇÂ½DB·þÎñÆ÷                                       //
//                                                              //
//////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommonServer.hxx"
#include "LoginDBServerImp.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CCommonServerSystem : CSubSystem
// CSubSystem
INLINE Int CCommonServerSystem::GetComponentInfo(MAP_COMPONENT_INFO& Components)
{
	// ILoginDBServer
	Components.Add(UUID_OF(ILoginDBServer), CStringKey(COM_OF( ILoginDBServer )));

	return Components.GetSize();
}

INLINE bool CCommonServerSystem::FindComponent(const CUUID& uuId)
{
	// ILoginDBServer
	if (uuId == UUID_OF( ILoginDBServer )) {
		return true;
	}
	return false;
}

INLINE bool CCommonServerSystem::CreateComponent(const CUUID& uuId, CComponentPtr& CComponentPtrRef)
{
	CComponentPtrRef = nullptr;
	// ILoginDBServer
	if (uuId == UUID_OF( ILoginDBServer )) {
		CLoginDBServer* pServer = MNEW CLoginDBServer;
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


