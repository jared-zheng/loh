///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : targetxuikitwnd.h                                     ¡¡¡¡                            //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2005-02-02     version 0.0.0.1                                                             //
//   Update : 2015-03-16     version 0.0.5.5                                                             //
//   Detail : xui windows driect ui kit windows                                                          //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TARGET_XUI_KIT_WND_H__
#define __TARGET_XUI_KIT_WND_H__

#pragma once

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#include "targetxuikit.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXUIKitStatic
class CXUIKitStatic : public CXUIKitControl
{
public:
	CXUIKitStatic(void);
	virtual ~CXUIKitStatic(void);

	virtual PCXStr          GetUIKitClass(void) OVERRIDE;
	virtual CXUIKitControl* GetUIKitExactClass(PCXStr pszClass) OVERRIDE;

	void       GetText(CString& strText);
	void       SetText(PCXStr pszText);

	size_t     GetTextStyle(void);
	void       SetTextStyle(size_t stTextStyle);

	PINDEX     GetTextColorsIndex(void);
	void       SetTextColorsIndex(PINDEX index);
	PINDEX     GetTextFontIndex(void);
	void       SetTextFontIndex(PINDEX index);

	CXRect&    GetTextRegion(void);
	void       SetTextRegion(CXRect& rt);
private:
	CXUIKitStatic(const CXUIKitStatic&);
	CXUIKitStatic& operator=(const CXUIKitStatic&);
protected:
	size_t     m_stTextStyle;
	PINDEX     m_inxTextColors;
	PINDEX     m_inxTextFont;
	CXRect     m_rtTextRegion;
	CString    m_strText;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXUIKitGroup
class CXUIKitGroup : public CXUIKitStatic
{
public:
	CXUIKitGroup(void);
	virtual ~CXUIKitGroup(void);

	virtual PCXStr          GetUIKitClass(void) OVERRIDE;
	virtual CXUIKitControl* GetUIKitExactClass(PCXStr pszClass) OVERRIDE;

	PINDEX          Add(CXUIKitControl* pControl);
	bool            Remove(PINDEX index); // index == nullptr --- remove all

	PINDEX          GetFirst(void);
	CXUIKitControl* GetNext(PINDEX& index);
private:
	CXUIKitGroup(const CXUIKitGroup&);
	CXUIKitGroup& operator=(const CXUIKitGroup&);
protected:
	CTList<CXUIKitControl*>   m_GroupControl;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXUIKitButton
class CXUIKitButton : public CXUIKitStatic
{
public:
	CXUIKitButton(void);
	virtual ~CXUIKitButton(void);

	virtual PCXStr          GetUIKitClass(void) OVERRIDE;
	virtual CXUIKitControl* GetUIKitExactClass(PCXStr pszClass) OVERRIDE;

	PINDEX     GetBackImgIndex(void);
	void       SetBackImgIndex(PINDEX index);
private:
	CXUIKitButton(const CXUIKitButton&);
	CXUIKitButton& operator=(const CXUIKitButton&);
protected:
	PINDEX   m_inxBackImg;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXUIKitOption
class CXUIKitOption : public CXUIKitButton
{
public:
	CXUIKitOption(void);
	virtual ~CXUIKitOption(void);

	virtual PCXStr          GetUIKitClass(void) OVERRIDE;
	virtual CXUIKitControl* GetUIKitExactClass(PCXStr pszClass) OVERRIDE;
private:
	CXUIKitOption(const CXUIKitOption&);
	CXUIKitOption& operator=(const CXUIKitOption&);
protected:
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXUIKitEdit
class CXUIKitEdit : public CXUIKitStatic
{
public:
	CXUIKitEdit(void);
	virtual ~CXUIKitEdit(void);

	virtual PCXStr          GetUIKitClass(void) OVERRIDE;
	virtual CXUIKitControl* GetUIKitExactClass(PCXStr pszClass) OVERRIDE;

	CXEdit*    GetXEdit(void);

	PINDEX     GetBackImgIndex(void);
	void       SetBackImgIndex(PINDEX index);
private:
	CXUIKitEdit(const CXUIKitEdit&);
	CXUIKitEdit& operator=(const CXUIKitEdit&);
protected:
	CXEdit*   m_pEdit;

	PINDEX    m_inxBackImg;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "targetxuikitwnd.inl"

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#endif // __TARGET_XUI_KIT_WND_H__