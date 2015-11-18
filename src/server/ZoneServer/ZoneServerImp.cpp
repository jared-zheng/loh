//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Source File : ZoneServerImp.cpp                            //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : 地图服务器管理实现                                 //
//                                                              //
//////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ZoneServerImp.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CZoneServer
UInt CZoneServer::Command(PCXStr pszCMD, uintptr_t utParam)
{
	// 按配置文件中配置项名称更新相关参数
	UNREFERENCED_PARAMETER(pszCMD);
	UNREFERENCED_PARAMETER(utParam);
	return (UInt)RET_OKAY;
}

UInt CZoneServer::Update(void)
{
	return 0;
}





