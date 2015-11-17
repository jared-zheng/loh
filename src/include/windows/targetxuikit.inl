///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : targetxuikit.inl                                                                      //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-03-16     version 0.0.5.5                                                             //
//   Detail : xui windows driect ui kit                                                                  //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TARGET_XUI_KIT_INL__
#define __TARGET_XUI_KIT_INL__

#pragma once

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// IXDUI
SELECTANY CPCXStr IXUIKit::CONTROL = TF("Control");
SELECTANY CPCXStr IXUIKit::STATIC  = TF("Static");
SELECTANY CPCXStr IXUIKit::GROUP   = TF("Group");
SELECTANY CPCXStr IXUIKit::BUTTON  = TF("Button");
SELECTANY CPCXStr IXUIKit::OPTION  = TF("Option");

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXUIKitControl
INLINE CXUIKitControl::CXUIKitControl(CXUIKitRender* pRender)
: m_pRender(pRender)
, m_pParent(nullptr)
, m_nStyle(KIT_STYLE_ALL)
, m_nState(KIT_STATE_RENDER_A)
, m_crBackA(0)
, m_crBackB(0)
, m_crBorderA(0)
, m_crBorderB(0)
, m_inxControl(nullptr)
, m_inxBackImg(nullptr)
{
	assert(m_pRender != nullptr);
}

INLINE CXUIKitControl::~CXUIKitControl(void)
{
}

INLINE CXUIKitControl::CXUIKitControl(const CXUIKitControl&)
: m_pRender(nullptr)
, m_pParent(nullptr)
, m_nStyle(KIT_STYLE_ALL)
, m_nState(KIT_STATE_RENDER_A)
, m_crBackA(0)
, m_crBackB(0)
, m_crBorderA(0)
, m_crBorderB(0)
, m_inxControl(nullptr)
, m_inxBackImg(nullptr)
{
}

INLINE CXUIKitControl& CXUIKitControl::operator=(const CXUIKitControl&)
{
	return (*this);
}

INLINE PCXStr CXUIKitControl::GetUIKitClass(void)
{
	return CONTROL;
}

INLINE CXUIKitControl* CXUIKitControl::GetUIKitExactClass(PCXStr pszClass)
{
	if (CXChar::Compare(CONTROL, pszClass) == 0)
	{
		return this;
	}
	return nullptr;
}

INLINE bool CXUIKitControl::SetAttribute(PCXStr pszAttr, PCXStr pszValue)
{
	bool bRet     = true;
	COLORREF* pcr = nullptr;
	CXRect*   prt = nullptr;
	if (CXChar::Compare(pszAttr, TF("style")) == 0)
	{
		SetStyle((Int)CXChar::ToLong(pszValue, nullptr, RADIXT_HEX));
	}
	else if (CXChar::Compare(pszAttr, TF("staterender")) == 0)
	{
		SetStateRender(CXChar::ToInt(pszValue));
	}
	else if (CXChar::Compare(pszAttr, TF("name")) == 0)
	{
		SetName(pszValue);
	}
	else if (CXChar::Compare(pszAttr, TF("backimg")) == 0)
	{
		// file='path'[ src=0,0,0,0 dst=0,0,0,0 param=0x0 style=0]
	}
	else if (CXChar::Compare(pszAttr, TF("backa")) == 0)
	{
		pcr = &m_crBackA;
	}
	else if (CXChar::Compare(pszAttr, TF("backb")) == 0)
	{
		pcr = &m_crBackB;
	}
	else if (CXChar::Compare(pszAttr, TF("bordera")) == 0)
	{
		pcr = &m_crBorderA;
	}
	else if (CXChar::Compare(pszAttr, TF("borderb")) == 0)
	{
		pcr = &m_crBorderB;
	}
	else if (CXChar::Compare(pszAttr, TF("region")) == 0)
	{
		prt = &m_rtRegion;
	}
	else if (CXChar::Compare(pszAttr, TF("minmax")) == 0)
	{
		prt = &m_rtMinMax;
	}
	else if (CXChar::Compare(pszAttr, TF("padding")) == 0)
	{
		prt = &m_rtPadding;
	}
	else if (CXChar::Compare(pszAttr, TF("border")) == 0)
	{
		prt = &m_rtBorder;
	}
	else
	{
		bRet = false;
	}

	if (pcr != nullptr)
	{
		*pcr = (COLORREF)CXChar::ToLong(pszValue, nullptr, RADIXT_HEX);
	}
	else if (prt != nullptr)
	{
		PXStr pszEnd = nullptr;
		prt->m_tLeft = CXChar::ToLong(pszValue, &pszEnd);
		assert(pszEnd != nullptr);
		assert(*pszEnd != TF(','));
		++pszEnd;
		prt->m_tTop = CXChar::ToLong(pszEnd, &pszEnd);
		assert(pszEnd != nullptr);
		assert(*pszEnd != TF(','));
		++pszEnd;
		prt->m_tRight = CXChar::ToLong(pszEnd, &pszEnd);
		assert(pszEnd != nullptr);
		assert(*pszEnd != TF(','));
		++pszEnd;
		prt->m_tBottom = CXChar::ToLong(pszEnd, &pszEnd);
	}
	return bRet;
}

INLINE void CXUIKitControl::OnPaint(CXDC& dc, const CXRect& rcPaint)
{
	if (::IntersectRect(m_rtPaint, rcPaint, m_rtRegion) != FALSE)
	{	// clip -> back color -> back image -> state image -> text -> border
		if ((m_rtBorder.m_tRight > 0) && (m_rtBorder.m_tBottom > 0))
		{
			CXUIClip Clip;
			Clip.GenerateRoundClip(dc, m_rtPaint, m_rtRegion, m_rtBorder.m_tRight, m_rtBorder.m_tBottom);

			PaintBackColor(dc);
			PaintBackImage(dc);
			PaintStateImage(dc);
			PaintText(dc);
			PaintBorder(dc);
		}
		else
		{
			PaintBackColor(dc);
			PaintBackImage(dc);
			PaintStateImage(dc);
			PaintText(dc);
			PaintBorder(dc);
		}
	}
}

INLINE void CXUIKitControl::PaintBackColor(CXDC& dc)
{
	if (GetStateRender() == KIT_STATE_RENDER_A)
	{
		dc.SetBkColor(m_crBackA);
		dc.ExtTextOut(0, 0, ETO_OPAQUE, m_rtRegion, nullptr);
	}
	else
	{
		m_pRender->RenderGradient(dc, m_crBackA, m_crBackB, m_nState, m_rtRegion);
	}
}

INLINE void CXUIKitControl::PaintBackImage(CXDC& dc)
{
	if (m_inxBackImg != nullptr)
	{
		m_pRender->RenderImage(dc, m_inxBackImg, m_rtRegion, GetState());
	}
}
INLINE void CXUIKitControl::PaintStateImage(CXDC&)
{
}

INLINE void CXUIKitControl::PaintText(CXDC&)
{
}

INLINE void CXUIKitControl::PaintBorder(CXDC& dc)
{
	if ((m_rtBorder.m_tLeft > 0) && (m_rtBorder.m_tTop > 0))
	{
		COLORREF cr = m_crBorderA;
		if (IsStateFocused())
		{
			cr = m_crBorderB;
		}

		CXPen Pen;
		Pen.CreatePen(PS_SOLID | PS_INSIDEFRAME, m_rtBorder.m_tLeft, cr);
		dc.SelectPen(Pen);
		dc.SelectStockBrush(HOLLOW_BRUSH);
		if ((m_rtBorder.m_tRight > 0) && (m_rtBorder.m_tBottom > 0))
		{
			dc.RoundRect(m_rtRegion.m_tLeft, m_rtRegion.m_tTop, m_rtRegion.m_tRight, m_rtRegion.m_tBottom, m_rtBorder.m_tRight, m_rtBorder.m_tBottom);
		}
		else
		{
			dc.Rectangle(m_rtRegion.m_tLeft, m_rtRegion.m_tTop, m_rtRegion.m_tRight, m_rtRegion.m_tBottom);
		}
		dc.SelectStockPen(NULL_PEN);
	}
}

INLINE void CXUIKitControl::OnPostPaint(CXDC&, const CXRect&)
{
}

INLINE void CXUIKitControl::GetName(CString& strName)
{
	strName = m_strName;
}

INLINE void CXUIKitControl::SetName(PCXStr pszName)
{
	m_strName = pszName;
}

INLINE CXUIKitControl* CXUIKitControl::GetParent(void)
{
	return m_pParent;
}

INLINE void CXUIKitControl::SetParent(CXUIKitControl* pParent)
{
	m_pParent = pParent;
}

INLINE Int CXUIKitControl::GetStyle(void)
{
	return m_nStyle;
}

INLINE void CXUIKitControl::SetStyle(Int nStyle)
{
	m_nStyle = nStyle;
}

INLINE bool CXUIKitControl::IsVisible(void)
{
	return ((m_nStyle & KIT_STYLE_VISIBLE) != 0);
}

INLINE void CXUIKitControl::SetVisible(bool bVisible)
{
	if (bVisible)
	{
		m_nStyle |= KIT_STYLE_VISIBLE;
	}
	else
	{
		m_nStyle &= ~KIT_STYLE_VISIBLE;
	}
}

INLINE bool CXUIKitControl::IsEnabled(void)
{
	return ((m_nStyle & KIT_STYLE_ENABLED) != 0);
}

INLINE void CXUIKitControl::SetEnabled(bool bEnabled)
{
	if (bEnabled)
	{
		m_nStyle |= KIT_STYLE_ENABLED;
	}
	else
	{
		m_nStyle &= ~KIT_STYLE_ENABLED;
	}
}

INLINE bool CXUIKitControl::IsFixed(void)
{
	return ((m_nStyle & KIT_STYLE_FIXED) != 0);
}

INLINE void CXUIKitControl::SetFixed(bool bFixed)
{
	if (bFixed)
	{
		m_nStyle |= KIT_STYLE_FIXED;
	}
	else
	{
		m_nStyle &= ~KIT_STYLE_FIXED;
	}
}

INLINE bool CXUIKitControl::IsHitTestEnabled(void)
{
	return ((m_nStyle & KIT_STYLE_HITTEST) != 0);
}

INLINE void CXUIKitControl::SetHitTestEnabled(bool bEnabled)
{
	if (bEnabled)
	{
		m_nStyle |= KIT_STYLE_HITTEST;
	}
	else
	{
		m_nStyle &= ~KIT_STYLE_HITTEST;
	}
}

INLINE bool CXUIKitControl::IsFocus(void)
{
	return ((m_nStyle & KIT_STYLE_FOCUS) != 0);
}

INLINE void CXUIKitControl::SetFocus(bool bEnabled)
{
	if (bEnabled)
	{
		m_nStyle |= KIT_STYLE_FOCUS;
	}
	else
	{
		m_nStyle &= ~KIT_STYLE_FOCUS;
	}
}

INLINE Int CXUIKitControl::GetState(void)
{
	return (m_nState & KIT_STATE_MASK);
}

INLINE void CXUIKitControl::SetState(Int nState)
{
	m_nState &= (~KIT_STATE_MASK);
	m_nState |= (nState & KIT_STATE_MASK);
}

INLINE Int CXUIKitControl::GetStateRender(void)
{
	return (m_nState & KIT_STATE_RENDER_MASK);
}

INLINE void CXUIKitControl::SetStateRender(Int nStateRender)
{
	m_nState &= (~KIT_STATE_RENDER_MASK);
	m_nState |= (nStateRender & KIT_STATE_RENDER_MASK);
}

INLINE bool CXUIKitControl::IsStateFocused(void)
{
	return ((m_nState & KIT_STATE_FOCUSED) != 0);
}

INLINE void CXUIKitControl::SetStateFocused(bool bFocused)
{
	if (bFocused)
	{
		m_nState |= KIT_STATE_FOCUSED;
	}
	else
	{
		m_nState &= ~KIT_STATE_FOCUSED;
	}
}

INLINE COLORREF CXUIKitControl::GetBackA(void)
{
	return m_crBackA;
}

INLINE void CXUIKitControl::SetBackA(COLORREF cr)
{
	m_crBackA = cr;
}

INLINE COLORREF CXUIKitControl::GetBackB(void)
{
	return m_crBackB;
}

INLINE void CXUIKitControl::SetBackB(COLORREF cr)
{
	m_crBackB = cr;
}

INLINE COLORREF CXUIKitControl::GetBorderA(void)
{
	return m_crBorderA;
}

INLINE void CXUIKitControl::SetBorderA(COLORREF cr)
{
	m_crBorderA = cr;
}

INLINE COLORREF CXUIKitControl::GetBorderB(void)
{
	return m_crBorderB;
}

INLINE void CXUIKitControl::SetBorderB(COLORREF cr)
{
	m_crBorderB = cr;
}

INLINE PINDEX CXUIKitControl::GetControlIndex(void)
{
	return m_inxControl;
}

INLINE void CXUIKitControl::SetControlIndex(PINDEX index)
{
	m_inxControl = index;
}

INLINE PINDEX CXUIKitControl::GetBackImgIndex(void)
{
	return m_inxBackImg;
}

INLINE void CXUIKitControl::SetBackImgIndex(PINDEX index)
{
	m_inxBackImg = index;
}

INLINE CXRect& CXUIKitControl::GetRegion(void)
{
	return m_rtRegion;
}

INLINE void CXUIKitControl::SetRegion(CXRect& rt)
{
	m_rtRegion = rt;
}

INLINE CXRect& CXUIKitControl::GetMinMax(void)
{
	return m_rtMinMax;
}

INLINE void CXUIKitControl::SetMinMax(CXRect& rt)
{
	m_rtMinMax = rt;
}

INLINE CXRect& CXUIKitControl::GetPadding(void)
{
	return m_rtPadding;
}

INLINE void CXUIKitControl::SetPadding(CXRect& rt)
{
	m_rtPadding = rt;
}

INLINE CXRect& CXUIKitControl::GetBorder(void)
{
	return m_rtBorder;
}

INLINE void CXUIKitControl::SetBorder(CXRect& rt)
{
	m_rtBorder = rt;
}

#endif // (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#endif // __TARGET_XUI_KIT_INL__