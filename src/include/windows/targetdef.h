///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : targetdef.h                                                                           //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : windows basic type define file                                                             //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TARGET_DEF_H__
#define __TARGET_DEF_H__

#pragma once

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
typedef   HANDLE                                    KeyRef, *PKeyRef;
typedef   HINSTANCE                                 LibRef, *PLibRef;
typedef   HWND                                      WndRef, *PWndRef;

// UUID defined in rpcdce.h
#ifndef UUID_DEFINED
	typedef   GUID                                  UID,   *PUID;
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#define TOSTR( x )                                  __STR__( x )
#define __STR__( x )                                #x
#define TF( x )                                     __T__( x )
#ifdef RUNTIME_UNICODE
	#define XChar                                   WChar
	#define __T__( x )                              L ## x
#else   // RUNTIME_UNICODE
	#define XChar                                   Char
	#define __T__( x )                              x  
#endif  // RUNTIME_UNICODE

#define KEYREF_INVALID                              INVALID_HANDLE_VALUE
#define SEEKTO_INVALID                              INVALID_SET_FILE_POINTER

#define TRAITS_PAGE_ACP                             CP_ACP
#define TRAITS_PAGE_UTF8                            CP_UTF8

#define EXCEPTION_OUTOFMEMORY                       E_OUTOFMEMORY
//#define EXCEPTION_ACCESS_VIOLATION                  EXCEPTION_ACCESS_VIOLATION
//#define EXCEPTION_ARRAY_BOUNDS_EXCEEDED             EXCEPTION_ARRAY_BOUNDS_EXCEEDED

#define ALLOC_TYPE_NORMAL                           PAGE_READWRITE
#define ALLOC_TYPE_EXECUTE                          PAGE_EXECUTE_READWRITE

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#ifdef RUNTIME_STATIC
	#define CORE_MODULE_NAME
	#define NETWORK_MODULE_NAME
	#define XUI_MODULE_NAME 
#else  // RUNTIME_STATIC
	#ifdef RUNTIME_DEBUG
		#define CORE_MODULE_NAME                    TF("coreD.dll")
		#define NETWORK_MODULE_NAME                 TF("networkD.dll")
		#define XUI_MODULE_NAME                     TF("xuiD.dll")
	#else  // RUNTIME_DEBUG
		#define CORE_MODULE_NAME                    TF("core.dll")
		#define NETWORK_MODULE_NAME                 TF("network.dll")
		#define XUI_MODULE_NAME                     TF("xuiD.dll")
	#endif // RUNTIME_DEBUG
#endif // RUNTIME_STATIC

#endif // (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#endif // __TARGET_DEF_H__