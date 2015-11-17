///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : targetxui.h                                     ¡¡¡¡                                  //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2005-02-02     version 0.0.0.1                                                             //
//   Update : 2015-03-16     version 0.0.5.5                                                             //
//   Detail : xui windows                                                                                //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TARGET_XUI_H__
#define __TARGET_XUI_H__

#pragma once

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#include "subsystem.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class IXWnd;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXUITraits
class NOVTABLE CXUITraits
{
public:
	typedef struct tagCREATE_PARAM
	{
	public:
		tagCREATE_PARAM(void)
		: pParent(nullptr)
		, itMenuID(0)
		, pszName(nullptr)
		, ulStyle(WS_OVERLAPPEDWINDOW)
		, ulExStyle(0)
		, nx(CW_USEDEFAULT)
		, ny(CW_USEDEFAULT)
		, ncx(CW_USEDEFAULT)
		, ncy(CW_USEDEFAULT)
		, bFlag(TRUE)
		, uClassStyle(CS_VREDRAW|CS_HREDRAW|CS_DBLCLKS)
		, pEventHandler(nullptr)
		{
		}

		~tagCREATE_PARAM(void)
		{
		}
	public:
		IXWnd*           pParent;
		intptr_t         itMenuID; 
		PCXStr           pszName;
		ULong            ulStyle;
		ULong            ulExStyle;
		Int              nx;
		Int              ny;
		Int              ncx;
		Int              ncy;
		BOOL             bFlag; // dialog : modal/modeless; controls : subclass/un-subclass
		UInt             uClassStyle;
		CEventHandler*   pEventHandler;
	}CREATE_PARAM, *PCREATE_PARAM;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXUIManager
DECLARE_UUID( CXUIManager,  {4B467976-0C86-4515-AAB7-C7FC00239394} )
class NOVTABLE CXUIManager ABSTRACT : public CComponent, public CXUITraits
{
public:
	virtual UInt     Init(void) PURE_METHOD;
	virtual void     Exit(void) PURE_METHOD;

	virtual void     MsgLoop(void) PURE_METHOD;


	virtual intptr_t Create(IXWnd& xWndRef, CREATE_PARAM& cp) PURE_METHOD;
	virtual intptr_t Create(IXWnd& xWndRef, ULong ulExStyle, PCXStr pszName, ULong ulStyle, 
	                        Int nx = CW_USEDEFAULT, Int ny = CW_USEDEFAULT, Int ncx = CW_USEDEFAULT, Int ncy = CW_USEDEFAULT,
	                        IXWnd* pParent = nullptr, intptr_t itMenuID = 0, BOOL bFlag = TRUE, UInt uClassStyle = CS_VREDRAW|CS_HREDRAW|CS_DBLCLKS,
							CEventHandler* pEventHandler = nullptr) PURE_METHOD;
	virtual bool     Attach(IXWnd& xWndRef, WndRef wrWnd, IXWnd* pParent = nullptr) PURE_METHOD;
	virtual bool     Detach(IXWnd& xWndRef) PURE_METHOD;

	virtual IXWnd*   Find(PCXStr pszName) PURE_METHOD;
	virtual IXWnd*   Find(WndRef wrWnd) PURE_METHOD;
	virtual IXWnd*   GetFrame(void) PURE_METHOD;

	virtual bool     LoadAccel(Int nAccelID) PURE_METHOD;
	virtual bool     LoadResource(PCXStr pszFile) PURE_METHOD;
	virtual void     SetResHandle(LibRef lrResInst, bool bUnload = false) PURE_METHOD;
	virtual LibRef   GetResHandle(void) PURE_METHOD;
};
typedef CTRefCountPtr<CXUIManager> CXUIManagerPtr;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXUIKitRender
DECLARE_UUID(CXUIKitRender, {1BE479F7-D3A1-4238-A2DD-1206930B19BE})
class NOVTABLE CXUIKitRender ABSTRACT : public CComponent
{
public:
	virtual UInt     Init(void) PURE_METHOD;
	virtual void     Exit(void) PURE_METHOD;

	virtual bool     RenderColor(HDC hDC, COLORREF crColor, const RECT& rt, bool bAlpha = false, Byte bSCA = 255) PURE_METHOD;
	virtual bool     RenderGradient(HDC hDC, COLORREF crColor1, COLORREF crColor2, Int nType, const RECT& rt, bool bAlpha = false, Byte bSCA = 255) PURE_METHOD;
	virtual bool     RenderImage(HDC hDC, PINDEX index, const RECT& rt, Int nState, bool bAlpha = false, Byte bSCA = 255) PURE_METHOD;
	virtual bool     RenderImage(HDC hDC, HBITMAP hBitmap, const RECT& rt, Int nState, bool bAlpha= false, Byte bSCA = 255) PURE_METHOD;

	virtual PINDEX   AddImage(PCXStr pszName, PCXStr pszTag, COLORREF crMask = 0) PURE_METHOD;
	virtual PINDEX   AddImage(Int nId, COLORREF crMask = 0) PURE_METHOD;
	virtual bool     RemoveImage(PINDEX index) PURE_METHOD;

	virtual bool     LoadResource(PCXStr pszFile) PURE_METHOD;
	virtual void     SetResHandle(LibRef lrResInst, bool bUnload = false) PURE_METHOD;
	virtual LibRef   GetResHandle(void) PURE_METHOD;
};
typedef CTRefCountPtr<CXUIKitRender> CXUIKitRenderPtr;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXUISystem : CSubSystem
DECLARE_UUID( CXUISystem, {27E66886-958C-4921-97A8-C1B3582B12CC} )

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#endif // __TARGET_XUI_H__