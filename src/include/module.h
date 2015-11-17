///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : module.h                                                                              //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : lib load helper                                                                            //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __MODULE_H__
#define __MODULE_H__

#pragma once

#include "handle.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CModule
class CModule : public MObject
{
public:
	explicit CModule(LibRef libr = nullptr, bool bUnload = false);
	CModule(const CModule& aSrc);

	~CModule(void);

	CModule& operator=(const CModule& aSrc);
	CModule& operator=(LibRef libr); // m_bUnload = true

	bool    operator==(LibRef libr) const;
	bool    operator!=(LibRef libr) const;
	bool    operator==(const CModule& aSrc) const;
	bool    operator!=(const CModule& aSrc) const;

	operator LibRef(void) const;

	template <typename I> I Find(PCStr pszSymbol);

	bool   Attach(PCXStr pszPath, bool bUnload = true);
	void   Attach(LibRef libr, bool bUnload = true);
	LibRef Detach(void);
	void   Close(void);
public:
	bool            m_bUnload;
	CModuleHandle   m_Module;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "module.inl"

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // __MODULE_H__