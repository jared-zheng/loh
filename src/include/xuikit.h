///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : xuikit.h                                     ¡¡¡¡                                     //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2005-02-02     version 0.0.0.1                                                             //
//   Update : 2015-03-16     version 0.0.5.5                                                             //
//   Detail : ui controls                                                                                //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __XUI_KIT_H__
#define __XUI_KIT_H__

#pragma once

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)
	#include "windows/targetxuikit.h"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_LINUX)
	#include "linux/targetxuikit.h"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_MAC)
	#include "mac/targetxuikit.h"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_IOS)
	#include "ios/targetxuikit.h"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_ANDROID)
	#include "android/targetxuikit.h"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_HTML5)
	#include "html5/targetxuikit.h"
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

#endif // __XUI_KIT_H__