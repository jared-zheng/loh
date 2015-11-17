///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : targetxuikitwnd.inl                                                                   //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-03-16     version 0.0.5.5                                                             //
//   Detail : xui windows driect ui kit windows                                                          //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TARGET_XUI_KIT_WND_INL__
#define __TARGET_XUI_KIT_WND_INL__

#pragma once

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXUIKitStatic
INLINE CXUIKitStatic::CXUIKitStatic(void)
: m_stTextStyle(0)
, m_inxTextColors(nullptr)
, m_inxTextFont(nullptr)
{
}

INLINE CXUIKitStatic::~CXUIKitStatic(void)
{
}

INLINE CXUIKitStatic::CXUIKitStatic(const CXUIKitStatic&)
: m_stTextStyle(0)
, m_inxTextColors(nullptr)
, m_inxTextFont(nullptr)
{
}

INLINE CXUIKitStatic& CXUIKitStatic::operator=(const CXUIKitStatic&)
{
	return (*this);
}

INLINE PCXStr CXUIKitStatic::GetUIKitClass(void)
{
	return STATIC;
}

INLINE CXUIKitControl* CXUIKitStatic::GetUIKitExactClass(PCXStr pszClass)
{
	if (CXChar::Compare(STATIC, pszClass) == 0)
	{
		return this;
	}
	return nullptr;
}

INLINE void CXUIKitStatic::GetText(CString& strText)
{
	strText = m_strText;
}

INLINE void CXUIKitStatic::SetText(PCXStr pszText)
{
	m_strText = pszText;
}

INLINE size_t CXUIKitStatic::GetTextStyle(void)
{
	return m_stTextStyle;
}

INLINE void CXUIKitStatic::SetTextStyle(size_t stTextStyle)
{
	m_stTextStyle = stTextStyle;
}

INLINE PINDEX CXUIKitStatic::GetTextColorsIndex(void)
{
	return m_inxTextColors;
}

INLINE void CXUIKitStatic::SetTextColorsIndex(PINDEX index)
{
	m_inxTextColors = index;
}

INLINE PINDEX CXUIKitStatic::GetTextFontIndex(void)
{
	return m_inxTextFont;
}

INLINE void CXUIKitStatic::SetTextFontIndex(PINDEX index)
{
	m_inxTextFont = index;
}

INLINE CXRect& CXUIKitStatic::GetTextRegion(void)
{
	return m_rtTextRegion;
}

INLINE void CXUIKitStatic::SetTextRegion(CXRect& rt)
{
	m_rtTextRegion = rt;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXUIKitGroup
INLINE CXUIKitGroup::CXUIKitGroup(void)
{
}

INLINE CXUIKitGroup::~CXUIKitGroup(void)
{
	m_GroupControl.RemoveAll();
}

INLINE CXUIKitGroup::CXUIKitGroup(const CXUIKitGroup&)
{
}

INLINE CXUIKitGroup& CXUIKitGroup::operator=(const CXUIKitGroup&)
{
	return (*this);
}

INLINE PCXStr CXUIKitGroup::GetUIKitClass(void)
{
	return GROUP;
}

INLINE CXUIKitControl* CXUIKitGroup::GetUIKitExactClass(PCXStr pszClass)
{
	if (CXChar::Compare(GROUP, pszClass) == 0)
	{
		return this;
	}
	return nullptr;
}

INLINE PINDEX CXUIKitGroup::Add(CXUIKitControl* pControl)
{
	return m_GroupControl.AddTail(pControl);
}

INLINE bool CXUIKitGroup::Remove(PINDEX index)
{
	return m_GroupControl.RemoveAt(index);
}

INLINE PINDEX CXUIKitGroup::GetFirst(void)
{
	return m_GroupControl.GetHeadIndex();
}

INLINE CXUIKitControl* CXUIKitGroup::GetNext(PINDEX& index)
{
	return m_GroupControl.GetNext(index);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXUIKitButton
INLINE CXUIKitButton::CXUIKitButton(void)
: m_inxBackImg(nullptr)
{
}

INLINE CXUIKitButton::~CXUIKitButton(void)
{
}

INLINE CXUIKitButton::CXUIKitButton(const CXUIKitButton&)
: m_inxBackImg(nullptr)
{
}

INLINE CXUIKitButton& CXUIKitButton::operator=(const CXUIKitButton&)
{
	return (*this);
}

INLINE PCXStr CXUIKitButton::GetUIKitClass(void)
{
	return BUTTON;
}

INLINE CXUIKitControl* CXUIKitButton::GetUIKitExactClass(PCXStr pszClass)
{
	if (CXChar::Compare(BUTTON, pszClass) == 0)
	{
		return this;
	}
	return nullptr;
}

INLINE PINDEX CXUIKitButton::GetBackImgIndex(void)
{
	return m_inxBackImg;
}

INLINE void CXUIKitButton::SetBackImgIndex(PINDEX index)
{
	m_inxBackImg = index;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXUIKitOption
INLINE CXUIKitOption::CXUIKitOption(void)
{
}

INLINE CXUIKitOption::~CXUIKitOption(void)
{
}

INLINE CXUIKitOption::CXUIKitOption(const CXUIKitOption&)
{
}

INLINE CXUIKitOption& CXUIKitOption::operator=(const CXUIKitOption&)
{
	return (*this);
}

INLINE PCXStr CXUIKitOption::GetUIKitClass(void)
{
	return OPTION;
}

INLINE CXUIKitControl* CXUIKitOption::GetUIKitExactClass(PCXStr pszClass)
{
	if (CXChar::Compare(OPTION, pszClass) == 0)
	{
		return this;
	}
	return nullptr;
}

#endif // (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#endif // __TARGET_XUI_KIT_WND_INL__