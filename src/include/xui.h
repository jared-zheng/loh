///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : xuibase.h                                     ¡¡¡¡                                    //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2005-02-02     version 0.0.0.1                                                             //
//   Update : 2015-03-16     version 0.0.5.5                                                             //
//   Detail : ui base                                                                                    //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __XUI_H__
#define __XUI_H__

#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(XUI_EXPORT)
	#define XUIAPI            API_EXPORT
	#define XUICLASS          CLASS_EXPORT
#elif defined(RUNTIME_STATIC)
	#define XUIAPI                                     
	#define XUICLASS                                
#else   // XUI_EXPORT
	#define XUIAPI            API_IMPORT
	#define XUICLASS          CLASS_IMPORT
#endif  // XUI_EXPORT

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)
	#include "windows/targetxui.h"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_LINUX)
	#include "linux/targetxui.h"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_MAC)
	#include "mac/targetxui.h"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_IOS)
	#include "ios/targetxui.h"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_ANDROID)
	#include "android/targetxui.h"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_HTML5)
	#include "html5/targetxui.h"
#else
	#error "__PLATFORM_TARGET__ No Implement"
#endif

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#
//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // __XUI_H__