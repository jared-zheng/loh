///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : targethandle.h                                                                        //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : windows handle                                                                             //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TARGET_HANDLE_H__
#define __TARGET_HANDLE_H__

#pragma once

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#include "mobject.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Windows Objects
// 1. Kernel Objects https://msdn.microsoft.com/en-us/library/windows/desktop/ms724485(v=vs.85).aspx
// 2. GDI Objects    https://msdn.microsoft.com/en-us/library/windows/desktop/ms724291(v=vs.85).aspx
// 3. User Objects   https://msdn.microsoft.com/en-us/library/windows/desktop/ms725486(v=vs.85).aspx

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CHandle : basic Kernel Objects handle, closed by CloseHandle
class CHandle : public MObject
{
public:
	explicit CHandle(KeyRef kr = nullptr);
	CHandle(const CHandle& aSrc); // aSrc.Detach

	~CHandle(void);

	CHandle& operator=(const CHandle& aSrc); // aSrc.Detach
	CHandle& operator=(KeyRef kr);

	bool   operator==(KeyRef kr) const;
	bool   operator!=(KeyRef kr) const;
	bool   operator==(const CHandle& aSrc) const;
	bool   operator!=(const CHandle& aSrc) const;

	operator KeyRef(void) const;

	bool     IsValid(void) const;

	void     Attach(KeyRef kr);
	KeyRef   Detach(void);

	void     Close(void);
public:
	KeyRef   m_krHandle;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// module: basic Kernel Objects handle, closed by FreeLibrary(dll or exe)
class CModuleHandle : public MObject
{
public:
	explicit CModuleHandle(LibRef lr = nullptr);
	CModuleHandle(const CModuleHandle& aSrc); // aSrc.Detach

	~CModuleHandle(void);

	CModuleHandle& operator=(const CModuleHandle& aSrc); // aSrc.Detach
	CModuleHandle& operator=(LibRef lr);

	bool   operator==(LibRef lr) const;
	bool   operator!=(LibRef lr) const;
	bool   operator==(const CModuleHandle& aSrc) const;
	bool   operator!=(const CModuleHandle& aSrc) const;

	operator LibRef(void) const;

	bool     IsValid(void) const;

	void     Attach(LibRef lr);
	LibRef   Detach(void);

	void     Close(void);

	FARPROC  Find(PCStr pszSymbol);
	bool     Load(PCXStr pszPath);
public:
	LibRef   m_lrHandle;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "targethandle.inl"

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#endif // __TARGET_HANDLE_H__