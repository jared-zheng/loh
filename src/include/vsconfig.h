///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : vsconfig.h                                                                            //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : config defines file for MSVC compiler                                                      //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __VS_CONFIG_H__
#define __VS_CONFIG_H__

#pragma once

#include "config.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER) // _MSC_VER
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// __INTRIN_TARGET__
#if defined(_M_IX86)
	#define __INTRIN_TARGET__                       INTRIN_TARGET_X86
#elif defined(_M_X64) ||  defined(_M_AMD64)
	#define __INTRIN_TARGET__                       INTRIN_TARGET_X64
#elif defined(_M_IA64)
	#define __INTRIN_TARGET__                       INTRIN_TARGET_IA64
#else
	#error "__INTRIN_TARGET__ No Implement"
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// __PLATFORM_TARGET__
#if defined(_WIN32) || defined(_WIN64)
	#define __PLATFORM_TARGET__                     PLATFORM_TARGET_WINDOWS
#else
	#error "__PLATFORM_TARGET__ No Implement"
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// __RUNTIME_CONFIG__
#if defined(_DEBUG) || defined(DEBUG)
	#define RUNTIME_DEBUG
#endif

#if defined(_UNICODE) || defined(UNICODE)
	#define RUNTIME_UNICODE
#endif

#if defined(_LIB) || defined(LIB_RUNTIME)
	#define RUNTIME_STATIC
#endif

// RTTI hash 32-bits comment
//#define RUNTIME_RTTI_HASH64

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// windows Header Files
#include "targetver.h"	
#define WIN32_LEAN_AND_MEAN		
#include <windows.h>
#include <dbghelp.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <mbstring.h>
#include <tchar.h>
#include <assert.h>
#include <new.h>
#include <time.h>
#include <share.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// std namespace : system memory alloc/free trace
#pragma push_macro("new")
#undef new
#pragma push_macro("delete")
#undef delete
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// windows CRT memory alloc/free debug check
#ifdef RUNTIME_DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
	#define CRTDBG_NEW                              new(_NORMAL_BLOCK, __FILE__, __LINE__)
	#define new                                     CRTDBG_NEW
#endif  // RUNTIME_DEBUG
#pragma pop_macro("delete")
#pragma pop_macro("new")

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// placement new operator from new.h
#define GNEW(adr)                                   ::new( (adr) )

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// disable warning
#pragma warning(disable:4127) // conditional expression is constant
#pragma warning(disable:4251) // class 'A' needs to have dll-interface to be used by clients
#pragma warning(disable:4275) // class 'A' needs to have dll-interface to be used by clients
#pragma warning(disable:4480) // nonstandard extension used: specifying underlying type for enum 'enum'
#pragma warning(disable:4180) // qualifier applied to function type has no meaning; ignored

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifdef __cplusplus
	#define LINE_EXTERN_C			                extern "C"
	#define BEGIN_EXTERN_C		                    extern "C" {
	#define END_EXTERN_C		                    }
#else
	#define LINE_EXTERN_C			                
	#define BEGIN_EXTERN_C		                    
	#define END_EXTERN_C		                    
#endif

#define PURE_METHOD                                 = 0

#define API_EXPORT                                  LINE_EXTERN_C __declspec(dllexport)
#define API_IMPORT                                  LINE_EXTERN_C __declspec(dllimport)
#define CLASS_EXPORT                                __declspec(dllexport)
#define CLASS_IMPORT                                __declspec(dllimport)

#define ASM                                         __asm
#define INLINE                                      __inline
#define FORCEINLINE                                 __forceinline
#define FORCENOINLINE                               __declspec(noinline)
#define NOVTABLE                                    __declspec(novtable)
#define SELECTANY                                   __declspec(selectany)
#define NAKED                                       __declspec(naked)

#define STDFUNC                                     WINAPI
#define CXXFUNC                                     WINAPIV

#if _MSC_VER <= 1500 // (Visual Studio 2008)
	#define nullptr                                 NULL
#else
	#define __CXX11_IMP__                           
#endif

#if _MSC_VER <= 1600 // (Visual Studio 2010)
	#define ABSTRACT
	#define FINAL
	#define OVERRIDE
#else
	#define ABSTRACT                                abstract
	#define FINAL                                   final
	#define OVERRIDE                                override   
#endif

#if _MSC_VER >= 1900 // (Visual Studio 2015)
	#define __STRING_CONCATENATION__                // space replace ##                                    
#endif

#else
	#error "config defines file for MSVC compiler only"
#endif  // _MSC_VER

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// common header files
#include "def.h"
#include "platform.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#
//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // __VS_CONFIG_H__