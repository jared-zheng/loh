///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : targetxuikit.h                                     ¡¡¡¡                               //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2005-02-02     version 0.0.0.1                                                             //
//   Update : 2015-03-16     version 0.0.5.5                                                             //
//   Detail : xui windows owner-draw ui kit                                                              //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TARGET_XUI_KIT_H__
#define __TARGET_XUI_KIT_H__

#pragma once

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#include "targetxuibasewnd.h"
#include "targetxui.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// IXUIKit
class NOVTABLE IXUIKit ABSTRACT : public MObject
{
public:
	enum KIT_STYLE
	{
		KIT_STYLE_VISIBLE    = 0x40000000,
		KIT_STYLE_ENABLED    = 0x20000000,
		KIT_STYLE_FIXED      = 0x10000000,
		KIT_STYLE_HITTEST    = 0x08000000,
		KIT_STYLE_FOCUS      = 0x04000000,
		KIT_STYLE_ALL        = 0x7C000000,
	};

	enum KIT_STATE
	{
		KIT_STATE_NORMAL,
		KIT_STATE_HILIGHT,
		KIT_STATE_PUSHED,
		KIT_STATE_DISABLED,
		KIT_STATE_MAXCOUNT,
		KIT_STATE_MASK        = 0x0000000F,
		KIT_STATE_FOCUSED     = 0x00000010,

		KIT_STATE_RENDER_A    = 0x02000000,
		KIT_STATE_RENDER_V    = 0x04000000,
		KIT_STATE_RENDER_H    = 0x08000000,
		KIT_STATE_RENDER_MV   = 0x10000000,
		KIT_STATE_RENDER_MH   = 0x20000000,
		KIT_STATE_RENDER_MASK = 0x3F000000,
	};

	static CPCXStr CONTROL;
	static CPCXStr STATIC;
	static CPCXStr GROUP;
	static CPCXStr BUTTON;
	static CPCXStr OPTION;

public:
	// parent to child
	virtual LRESULT KitEventProc(MSG&, BOOL&)  PURE_METHOD;
	// child to parent
	virtual LRESULT KitNotifyProc(MSG&, BOOL&) PURE_METHOD;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXUIKitControl
class NOVTABLE CXUIKitControl ABSTRACT : public IXUIKit
{
public:
	virtual PCXStr          GetUIKitClass(void);
	virtual CXUIKitControl* GetUIKitExactClass(PCXStr pszClass);

	virtual bool    SetAttribute(PCXStr pszAttr, PCXStr pszValue);
	virtual void    OnPaint(CXDC& dc, const CXRect& rcPaint);
	virtual void    PaintBackColor(CXDC& dc);
	virtual void    PaintBackImage(CXDC& dc);
	virtual void    PaintStateImage(CXDC& dc);
	virtual void    PaintText(CXDC& dc);
	virtual void    PaintBorder(CXDC& dc);
	virtual void    OnPostPaint(CXDC& dc, const CXRect& rcPaint);

	void            GetName(CString& strName);
	void            SetName(PCXStr pszName);

	CXUIKitControl* GetParent(void);
	void            SetParent(CXUIKitControl* pParent);

	Int             GetStyle(void);
	void            SetStyle(Int nStyle);

	bool            IsVisible(void);
	void            SetVisible(bool bVisible = true);
	bool            IsEnabled(void);
	void            SetEnabled(bool bEnabled = true);
	bool            IsFixed(void);
	void            SetFixed(bool bFixed = true);
	bool            IsHitTestEnabled(void);
	void            SetHitTestEnabled(bool bEnabled = true);
	bool            IsFocus(void);
	void            SetFocus(bool bEnabled = true);

	Int             GetState(void);
	void            SetState(Int nState);
	Int             GetStateRender(void);
	void            SetStateRender(Int nStateRender);

	bool            IsStateFocused(void);
	void            SetStateFocused(bool bFocused = true);

	COLORREF        GetBackA(void);
	void            SetBackA(COLORREF cr);
	COLORREF        GetBackB(void);
	void            SetBackB(COLORREF cr);
	COLORREF        GetBorderA(void);
	void            SetBorderA(COLORREF cr);
	COLORREF        GetBorderB(void);
	void            SetBorderB(COLORREF cr);

	PINDEX          GetControlIndex(void);
	void            SetControlIndex(PINDEX index);
	PINDEX          GetBackImgIndex(void);
	void            SetBackImgIndex(PINDEX index);

	CXRect&         GetRegion(void);
	void            SetRegion(CXRect& rt);
	CXRect&         GetMinMax(void);
	void            SetMinMax(CXRect& rt);
	CXRect&         GetPadding(void);
	void            SetPadding(CXRect& rt);
	CXRect&         GetBorder(void);
	void            SetBorder(CXRect& rt);
protected:
	CXUIKitControl(CXUIKitRender* pRender);
	virtual ~CXUIKitControl(void);
private:
	CXUIKitControl(const CXUIKitControl&);
	CXUIKitControl& operator=(const CXUIKitControl&);
protected:
	CXUIKitRender*    m_pRender;
	CXUIKitControl*   m_pParent;
	Int               m_nStyle;
	Int               m_nState;
	COLORREF          m_crBackA;
	COLORREF          m_crBackB;
	COLORREF          m_crBorderA;
	COLORREF          m_crBorderB;
	PINDEX            m_inxControl; // DUI map
	PINDEX            m_inxBackImg; //
	CXRect            m_rtRegion;
	CXRect            m_rtMinMax;   // left & top --- min, right & bottom --- max
	CXRect            m_rtPadding;
	CXRect            m_rtBorder;   // left & top --- border, right & bottom --- round
	CXRect            m_rtPaint;
	CStringKey        m_strName;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "targetxuikit.inl"

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#endif // __TARGET_XUI_KIT_H__