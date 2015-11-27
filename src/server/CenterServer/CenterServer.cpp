//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Source File : CenterServer.cpp                             //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : 中心服务器                                         //
//                                                              //
//////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommonServer.hxx"
#include "CenterServerImp.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CCommonServerSystem : CSubSystem
// CSubSystem
INLINE Int CCommonServerSystem::GetComponentInfo(MAP_COMPONENT_INFO& Components)
{
	// ICenterServer
	Components.Add(UUID_OF( ICenterServer ), CStringKey(COM_OF( ICenterServer )));

	return Components.GetSize();
}

INLINE bool CCommonServerSystem::FindComponent(const CUUID& uuId)
{
	// ICenterServer
	if (uuId == UUID_OF( ICenterServer )) {
		return true;
	}
	return false;
}

INLINE bool CCommonServerSystem::CreateComponent(const CUUID& uuId, CComponentPtr& CComponentPtrRef)
{
	CComponentPtrRef = nullptr;
	// ICenterServer
	if (uuId == UUID_OF( ICenterServer )) {
		CCenterServer* pServer = MNEW CCenterServer;
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


