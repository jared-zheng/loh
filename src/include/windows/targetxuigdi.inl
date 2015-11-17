///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : targetxuigdi.inl                                                                      //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2005-02-02     version 0.0.0.1                                                             //
//   Update : 2015-03-16     version 0.0.5.5                                                             //
//   Detail : xui windows gdi                                                                            //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TARGET_XUI_GDI_INL__
#define __TARGET_XUI_GDI_INL__

#pragma once

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTXSize
template <typename T>
INLINE CTXSize<T>::CTXSize(T InX, T InY) 
: m_tX(InX)
, m_tY(InY)
{ 
}

template <typename T>
INLINE CTXSize<T>::~CTXSize(void) 
{ 
}

template <typename T>
INLINE CTXSize<T>::CTXSize(const CTXSize<T>& aSrc) 
: m_tX(aSrc.m_tX)
, m_tY(aSrc.m_tY)
{ 
}

template <typename T>
INLINE CTXSize<T>& CTXSize<T>::operator=(const CTXSize<T>& aSrc) 
{ 
	if (this != &aSrc)
	{
		m_tX = aSrc.m_tX;
		m_tY = aSrc.m_tY;
	}
	return (*this);
}

template <typename T>
INLINE CTXSize<T>& CTXSize<T>::operator+=(const CTXSize<T>& aSrc) 
{ 
	m_tX += aSrc.m_tX;
	m_tY += aSrc.m_tY;
	return (*this);
}

template <typename T>
INLINE CTXSize<T>& CTXSize<T>::operator-=(const CTXSize<T>& aSrc) 
{ 
	m_tX -= aSrc.m_tX;
	m_tY -= aSrc.m_tY;
	return (*this);
}

template <typename T>
INLINE CTXSize<T> CTXSize<T>::operator+(const CTXSize<T>& aSrc) 
{ 
	return CTXSize<T>(m_tX + aSrc.m_tX, m_tY + aSrc.m_tY);
}

template <typename T>
INLINE CTXSize<T> CTXSize<T>::operator-(const CTXSize<T>& aSrc) 
{ 
	return CTXSize<T>(m_tX - aSrc.m_tX, m_tY - aSrc.m_tY);
}

template <typename T>
INLINE CTXSize<T> CTXSize<T>::operator-(void) 
{ 
	return CTXSize<T>(-m_tX, -m_tY);
}

template <typename T>
INLINE bool CTXSize<T>::operator==(const CTXSize<T>& aSrc) const 
{ 
	return ((m_tX == aSrc.m_tX) && (m_tY == aSrc.m_tY));
}

template <typename T>
INLINE bool CTXSize<T>::operator!=(const CTXSize<T>& aSrc) const 
{ 
	return ((m_tX != aSrc.m_tX) || (m_tY != aSrc.m_tY));
}

template <typename T>
INLINE void CTXSize<T>::SetValue(T InX, T InY)
{ 
	m_tX = InX;
	m_tY = InY;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXSize
INLINE CXSize::CXSize(Long InX, Long InY)
: CTXSize<Long>(InX, InY)
{
}

INLINE CXSize::CXSize(const SIZE& aSrc) 
: CTXSize<Long>(aSrc.cx, aSrc.cy)
{ 
}

INLINE CXSize::CXSize(const POINT& aSrc) 
: CTXSize<Long>(aSrc.x, aSrc.y)
{ 
}

INLINE CXSize& CXSize::operator=(const SIZE& aSrc) 
{ 
	m_tX = aSrc.cx;
	m_tY = aSrc.cy;
	return (*this);
}

INLINE CXSize& CXSize::operator=(const POINT& aSrc) 
{ 
	m_tX = aSrc.x;
	m_tY = aSrc.y;
	return (*this);
}

INLINE CXSize& CXSize::operator+=(const SIZE& aSrc) 
{ 
	m_tX += aSrc.cx;
	m_tY += aSrc.cy;
	return (*this);
}

INLINE CXSize& CXSize::operator+=(const POINT& aSrc) 
{ 
	m_tX += aSrc.x;
	m_tY += aSrc.y;
	return (*this);
}

INLINE CXSize& CXSize::operator-=(const SIZE& aSrc) 
{ 
	m_tX -= aSrc.cx;
	m_tY -= aSrc.cy;
	return (*this);
}

INLINE CXSize& CXSize::operator-=(const POINT& aSrc) 
{ 
	m_tX -= aSrc.x;
	m_tY -= aSrc.y;
	return (*this);
}

INLINE CXSize CXSize::operator+(const SIZE& aSrc) 
{ 
	return CXSize(m_tX + aSrc.cx, m_tY + aSrc.cy);
}

INLINE CXSize CXSize::operator+(const POINT& aSrc) 
{ 
	return CXSize(m_tX + aSrc.x, m_tY + aSrc.y);
}

INLINE CXSize CXSize::operator-(const SIZE& aSrc) 
{ 
	return CXSize(m_tX - aSrc.cx, m_tY - aSrc.cy);
}

INLINE CXSize CXSize::operator-(const POINT& aSrc) 
{ 
	return CXSize(m_tX - aSrc.x, m_tY - aSrc.y);
}

INLINE bool CXSize::operator==(const SIZE& aSrc) const 
{ 
	return ((m_tX == aSrc.cx) && (m_tY == aSrc.cy));
}

INLINE bool CXSize::operator!=(const SIZE& aSrc) const 
{ 
	return ((m_tX != aSrc.cx) || (m_tY != aSrc.cy));
}

INLINE bool CXSize::operator==(const POINT& aSrc) const 
{ 
	return ((m_tX == aSrc.x) && (m_tY == aSrc.y));
}

INLINE bool CXSize::operator!=(const POINT& aSrc) const 
{ 
	return ((m_tX != aSrc.x) || (m_tY != aSrc.y));
}

INLINE CXSize::operator SIZE*() const
{
	return (SIZE*)this;
}

INLINE CXSize::operator POINT*() const
{
	return (POINT*)(this);
}

INLINE CXSize::operator SIZE&() const
{
	return *((SIZE*)this);
}

INLINE CXSize::operator POINT&() const
{
	return *((POINT*)(this));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXPoint

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTXRect
template <typename T>
INLINE CTXRect<T>::CTXRect(T InLeft, T InTop, T InRight, T InBottom) 
: m_tLeft(InLeft)
, m_tTop(InTop)
, m_tRight(InRight)
, m_tBottom(InBottom)
{ 
}

template <typename T>
INLINE CTXRect<T>::~CTXRect(void) 
{
}

template <typename T>
INLINE CTXRect<T>::CTXRect(const CTXRect<T>& aSrc) 
: m_tLeft(aSrc.m_tLeft)
, m_tTop(aSrc.m_tTop)
, m_tRight(aSrc.m_tRight)
, m_tBottom(aSrc.m_tBottom)
{
}

template <typename T>
INLINE CTXRect<T>& CTXRect<T>::operator=(const CTXRect<T>& aSrc) 
{
	if (this != &aSrc)
	{
		m_tLeft   = aSrc.m_tLeft;
		m_tTop    = aSrc.m_tTop;
		m_tRight  = aSrc.m_tRight;
		m_tBottom = aSrc.m_tBottom;
	}
	return (*this);
}

template <typename T>
INLINE CTXRect<T>& CTXRect<T>::operator+=(const CTXRect<T>& aSrc) 
{
	m_tLeft   += aSrc.m_tLeft;
	m_tTop    += aSrc.m_tTop;
	m_tRight  += aSrc.m_tRight;
	m_tBottom += aSrc.m_tBottom;
	return (*this);
}

template <typename T>
INLINE CTXRect<T>& CTXRect<T>::operator-=(const CTXRect<T>& aSrc) 
{
	m_tLeft   -= aSrc.m_tLeft;
	m_tTop    -= aSrc.m_tTop;
	m_tRight  -= aSrc.m_tRight;
	m_tBottom -= aSrc.m_tBottom;
	return (*this);
}

template <typename T>
INLINE CTXRect<T>& CTXRect<T>::operator+=(const CTXSize<T>& tSize) 
{
	m_tLeft   += tSize.cx;
	m_tTop    += tSize.cy;
	m_tRight  += tSize.cx;
	m_tBottom += tSize.cy;
	return (*this);
}

template <typename T>
INLINE CTXRect<T>& CTXRect<T>::operator-=(const CTXSize<T>& tSize) 
{
	m_tLeft   -= tSize.cx;
	m_tTop    -= tSize.cy;
	m_tRight  -= tSize.cx;
	m_tBottom -= tSize.cy;
	return (*this);
}

template <typename T>
INLINE CTXRect<T> CTXRect<T>::operator+(const CTXRect<T>& aSrc) 
{
	return CTXRect<T>(m_tLeft   + aSrc.m_tLeft,
					 m_tTop    + aSrc.m_tTop,
					 m_tRight  + aSrc.m_tRight,
					 m_tBottom + aSrc.m_tBottom);
}

template <typename T>
INLINE CTXRect<T> CTXRect<T>::operator-(const CTXRect<T>& aSrc) 
{
	return CTXRect<T>(m_tLeft   - aSrc.m_tLeft,
					 m_tTop    - aSrc.m_tTop,
					 m_tRight  - aSrc.m_tRight,
					 m_tBottom - aSrc.m_tBottom);
}

template <typename T>
INLINE CTXRect<T> CTXRect<T>::operator+(const CTXSize<T>& tSize) 
{
	return CTXRect<T>(m_tLeft   + tSize.cx,
					 m_tTop    + tSize.cy,
					 m_tRight  + tSize.cx,
					 m_tBottom + tSize.cy);
}

template <typename T>
INLINE CTXRect<T> CTXRect<T>::operator-(const CTXSize<T>& tSize) 
{
	return CTXRect<T>(m_tLeft   - tSize.cx,
					 m_tTop    - tSize.cy,
					 m_tRight  - tSize.cx,
					 m_tBottom - tSize.cy);
}

template <typename T>
INLINE CTXRect<T> CTXRect<T>::operator-(void) 
{
	return CTXRect<T>(-m_tLeft, -m_tTop, -m_tRight, -m_tBottom);
}

template <typename T>
INLINE bool CTXRect<T>::operator==(const CTXRect<T>& aSrc) const 
{
	if ((m_tLeft == aSrc.m_tLeft) && (m_tTop == aSrc.m_tTop))
	{
		return ((m_tRight == aSrc.m_tRight) && (m_tBottom == aSrc.m_tBottom));
	}
	return false;
}

template <typename T>
INLINE bool CTXRect<T>::operator!=(const CTXRect<T>& aSrc) const 
{
	if ((m_tLeft != aSrc.m_tLeft) || (m_tTop != aSrc.m_tTop))
	{
		return true;
	}
	return ((m_tRight != aSrc.m_tRight) || (m_tBottom != aSrc.m_tBottom));
}

template <typename T>
INLINE T CTXRect<T>::Width(void)
{
	return (m_tRight - m_tLeft);
}

template <typename T>
INLINE T CTXRect<T>::Height(void)
{
	return (m_tBottom - m_tTop);
}

template <typename T>
INLINE CTXSize<T> CTXRect<T>::Size(void)
{
	return CTXSize<T>(m_tRight - m_tLeft, m_tBottom - m_tTop);
}

template <typename T>
INLINE CTXSize<T> CTXRect<T>::LeftTop(void)
{
	return CTXSize<T>(m_tLeft, m_tTop);
}

template <typename T>
INLINE CTXSize<T> CTXRect<T>::RightBottom(void)
{
	return CTXSize<T>(m_tRight, m_tBottom);
}

template <typename T>
INLINE CTXSize<T> CTXRect<T>::Center(void)
{
	return CTXSize<T>((m_tLeft + m_tRight) / 2, (m_tTop + m_tBottom) / 2);
}

template <typename T>
INLINE bool CTXRect<T>::IsEmpty(void) const 
{
	return ((m_tLeft == m_tRight) || (m_tTop == m_tBottom));
}

template <typename T>
INLINE bool CTXRect<T>::IsNull(void) const 
{
	if ((m_tLeft == 0) && (m_tTop == 0))
	{
		return ((m_tRight == 0) && (m_tBottom == 0));
	}
	return false;
}

template <typename T>
INLINE bool CTXRect<T>::IsEqual(const CTXRect<T>& rect) const 
{
	if ((m_tLeft == rect.m_tLeft) && (m_tTop == rect.m_tTop))
	{
		return ((m_tRight == rect.m_tRight) && (m_tBottom == rect.m_tBottom));
	}
	return false;
}

template <typename T>
INLINE bool CTXRect<T>::PtInRect(const CTXSize<T>& point) const
{
	if ((point.cx >= m_tLeft) && (point.cx <= m_tRight))
	{
		return ((point.cy >= m_tTop) && (point.cy <= m_tBottom));
	}
	return false;
}

template <typename T>
INLINE void CTXRect<T>::SetValue(T InLeft, T InTop, T InRight, T InBottom) 
{
	m_tLeft   = InLeft;
	m_tTop    = InTop;
	m_tRight  = InRight;
	m_tBottom = InBottom;
}

template <typename T>
INLINE void CTXRect<T>::InflateRect(T x, T y) 
{
	m_tLeft   -= x;
	m_tTop    -= y;
	m_tRight  += x;
	m_tBottom += y;
}

template <typename T>
INLINE void CTXRect<T>::OffseteRect(T x, T y) 
{
	m_tLeft   += x;
	m_tTop    += y;
	m_tRight  += x;
	m_tBottom += y;
}

template <typename T>
INLINE void CTXRect<T>::Normalize(void) 
{
	T tSwap;
	if (m_tLeft > m_tRight)
	{
		tSwap = m_tLeft;
		m_tLeft  = m_tRight;
		m_tRight = tSwap;
	}
	if (m_tTop > m_tBottom)
	{
		tSwap  = m_tTop;
		m_tTop    = m_tBottom;
		m_tBottom = tSwap;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXRect
INLINE CXRect::CXRect(Long InLeft, Long InTop, Long InRight, Long InBottom)
: CTXRect<Long>(InLeft, InTop, InRight, InBottom)
{
}

INLINE CXRect::CXRect(const RECT& aSrc) 
: CTXRect<Long>(aSrc.left, aSrc.top, aSrc.right, aSrc.bottom)
{
}

INLINE CXRect& CXRect::operator=(const RECT& aSrc) 
{
	m_tLeft   = aSrc.left;
	m_tTop    = aSrc.top;
	m_tRight  = aSrc.right;
	m_tBottom = aSrc.bottom;
	return (*this);
}

INLINE CXRect& CXRect::operator+=(const RECT& aSrc) 
{
	m_tLeft   += aSrc.left;
	m_tTop    += aSrc.top;
	m_tRight  += aSrc.right;
	m_tBottom += aSrc.bottom;
	return (*this);
}

INLINE CXRect& CXRect::operator-=(const RECT& aSrc) 
{
	m_tLeft   -= aSrc.left;
	m_tTop    -= aSrc.top;
	m_tRight  -= aSrc.right;
	m_tBottom -= aSrc.bottom;
	return (*this);
}

INLINE CXRect CXRect::operator+(const RECT& aSrc) 
{
	return CXRect(m_tLeft   + aSrc.left,
				 m_tTop    + aSrc.top,
				 m_tRight  + aSrc.right,
				 m_tBottom + aSrc.bottom);
}

INLINE CXRect CXRect::operator-(const RECT& aSrc) 
{
	return CXRect(m_tLeft   - aSrc.left,
				 m_tTop    - aSrc.top,
				 m_tRight  - aSrc.right,
				 m_tBottom - aSrc.bottom);
}

INLINE bool CXRect::operator==(const RECT& aSrc) const 
{
	if ((m_tLeft == aSrc.left) && (m_tTop == aSrc.top))
	{
		return ((m_tRight == aSrc.right) && (m_tBottom == aSrc.bottom));
	}
	return false;
}

INLINE bool CXRect::operator!=(const RECT& aSrc) const 
{
	if ((m_tLeft != aSrc.left) || (m_tTop != aSrc.top))
	{
		return true;
	}
	return ((m_tRight != aSrc.right) || (m_tBottom != aSrc.bottom));
}

INLINE bool CXRect::PtInRect(POINT& pt) const
{
	return (::PtInRect((RECT*)this, pt) ? true : false);
}

INLINE CXRect::operator RECT*() const
{
	return (RECT*)this;
}

INLINE CXRect::operator RECT&() const
{
	return *((RECT*)this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTXPen
template <bool tManaged>
INLINE CTXPen<tManaged>::CTXPen(HPEN hPen)
: m_hPen(hPen)
{
}

template <bool tManaged>
INLINE CTXPen<tManaged>::~CTXPen(void)
{
	if (tManaged && (m_hPen != nullptr))
	{
		DeleteObject();
	}
}

template <bool tManaged>
INLINE CTXPen<tManaged>::CTXPen(const CTXPen<tManaged>&)
: m_hPen(nullptr)
{
}

template <bool tManaged>
INLINE CTXPen<tManaged>& CTXPen<tManaged>::operator=(const CTXPen<tManaged>&)
{
	return (*this);
}

template <bool tManaged>
INLINE CTXPen<tManaged>& CTXPen<tManaged>::operator=(HPEN hPen)
{
	Attach(hPen);
	return (*this);
}

template <bool tManaged>
INLINE CTXPen<tManaged>::operator HPEN(void) const
{
	return m_hPen;
}

template <bool tManaged>
INLINE bool CTXPen<tManaged>::IsValid(void) const
{
	return (m_hPen != nullptr);
}

template <bool tManaged>
INLINE void CTXPen<tManaged>::Attach(HPEN hPen)
{
	assert(hPen);
	if (m_hPen != hPen)
	{
		if (tManaged && (m_hPen != nullptr))
		{
			DeleteObject();
		}
		m_hPen = hPen;
	}
}

template <bool tManaged>
INLINE HPEN CTXPen<tManaged>::Detach(void)
{
	HPEN hPen = m_hPen;
	m_hPen    = nullptr;
	return hPen;
}

// Create methods
template <bool tManaged>
INLINE HPEN CTXPen<tManaged>::CreatePen(Int nPenStyle, Int nWidth, COLORREF clrColor)
{
	assert(m_hPen == nullptr);
	m_hPen = ::CreatePen(nPenStyle, nWidth, clrColor);
	return m_hPen;
}

template <bool tManaged>
INLINE HPEN CTXPen<tManaged>::CreatePen(Int nPenStyle, Int nWidth, const LOGBRUSH& LogBrushRef, Int nStyleCount, PULong pulStyle)
{
	assert(m_hPen == nullptr);
	m_hPen = ::ExtCreatePen(nPenStyle, nWidth, &LogBrushRef, nStyleCount, pulStyle);
	return m_hPen;
}

template <bool tManaged>
INLINE HPEN CTXPen<tManaged>::CreatePenIndirect(LOGPEN& LogPenRef)
{
	assert(m_hPen == nullptr);
	m_hPen = ::CreatePenIndirect(&LogPenRef);
	return m_hPen;
}

template <bool tManaged>
INLINE BOOL CTXPen<tManaged>::DeleteObject(void)
{
	assert(m_hPen != nullptr);
	if (::DeleteObject(m_hPen))
	{
		m_hPen = nullptr;
	}
	return (m_hPen == nullptr);
}

template <bool tManaged>
INLINE Int CTXPen<tManaged>::GetLogPen(LOGPEN* pLogPen) const
{
	assert(m_hPen != nullptr);
	return ::GetObject(m_hPen, sizeof(LOGPEN), pLogPen);
}

template <bool tManaged>
INLINE bool CTXPen<tManaged>::GetLogPen(LOGPEN& LogPenRef) const
{
	assert(m_hPen != nullptr);
	return (::GetObject(m_hPen, sizeof(LOGPEN), &LogPenRef) == sizeof(LOGPEN));
}

template <bool tManaged>
INLINE Int CTXPen<tManaged>::GetExtLogPen(EXTLOGPEN* pLogPen) const
{
	assert(m_hPen != nullptr);
	return ::GetObject(m_hPen, sizeof(EXTLOGPEN), pLogPen);
}

template <bool tManaged>
INLINE bool CTXPen<tManaged>::GetExtLogPen(EXTLOGPEN& ExtLogPenRef) const
{
	assert(m_hPen != nullptr);
	return (::GetObject(m_hPen, sizeof(EXTLOGPEN), &ExtLogPenRef) == sizeof(EXTLOGPEN));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTXBrush
template <bool tManaged>
INLINE CTXBrush<tManaged>::CTXBrush(HBRUSH hBrush)
: m_hBrush(hBrush)
{
}

template <bool tManaged>
INLINE CTXBrush<tManaged>::~CTXBrush(void)
{
	if (tManaged && (m_hBrush != nullptr))
	{
		DeleteObject();
	}
}

template <bool tManaged>
INLINE CTXBrush<tManaged>::CTXBrush(const CTXBrush<tManaged>&)
: m_hBrush(nullptr)
{
}

template <bool tManaged>
INLINE CTXBrush<tManaged>& CTXBrush<tManaged>::operator=(const CTXBrush<tManaged>&)
{
	return (*this);
}

template <bool tManaged>
INLINE CTXBrush<tManaged>& CTXBrush<tManaged>::operator=(HBRUSH hBrush)
{
	Attach(hBrush);
	return (*this);
}

template <bool tManaged>
INLINE CTXBrush<tManaged>::operator HBRUSH(void) const
{
	return m_hBrush;
}

template <bool tManaged>
INLINE bool CTXBrush<tManaged>::IsValid(void) const
{
	return (m_hBrush != nullptr);
}

template <bool tManaged>
INLINE void CTXBrush<tManaged>::Attach(HBRUSH hBrush)
{
	assert(hBrush);
	if (m_hBrush != hBrush)
	{
		if (tManaged && (m_hBrush != nullptr))
		{
			DeleteObject();
		}
		m_hBrush = hBrush;
	}
}

template <bool tManaged>
INLINE HBRUSH CTXBrush<tManaged>::Detach(void)
{
	HBRUSH hBrush = m_hBrush;
	m_hBrush      = nullptr;
	return hBrush;
}

template <bool tManaged>
INLINE HBRUSH CTXBrush<tManaged>::CreateSolidBrush(COLORREF clrColor)
{
	assert(m_hBrush == nullptr);
	m_hBrush = ::CreateSolidBrush(clrColor);
	return m_hBrush;
}

template <bool tManaged>
INLINE HBRUSH CTXBrush<tManaged>::CreateHatchBrush(Int nIndex, COLORREF clrColor)
{
	assert(m_hBrush == nullptr);
	m_hBrush = ::CreateHatchBrush(nIndex, clrColor);
	return m_hBrush;
}

template <bool tManaged>
INLINE HBRUSH CTXBrush<tManaged>::CreateBrushIndirect(const LOGBRUSH& LogBrushRef)
{
	assert(m_hBrush == nullptr);
	m_hBrush = ::CreateBrushIndirect(&LogBrushRef);
	return m_hBrush;
}

template <bool tManaged>
INLINE HBRUSH CTXBrush<tManaged>::CreatePatternBrush(HBITMAP hBitmap)
{
	assert(m_hBrush == nullptr);
	m_hBrush = ::CreatePatternBrush(hBitmap);
	return m_hBrush;
}

template <bool tManaged>
INLINE HBRUSH CTXBrush<tManaged>::CreateDIBPatternBrush(HGLOBAL hPackedDIB, UInt uUsage)
{
	assert(hPackedDIB != nullptr);
	const void* lpPackedDIB = ::GlobalLock(hPackedDIB);
	assert(lpPackedDIB != nullptr);
	m_hBrush = ::CreateDIBPatternBrushPt(lpPackedDIB, uUsage);
	::GlobalUnlock(hPackedDIB);
	return m_hBrush;
}

template <bool tManaged>
INLINE HBRUSH CTXBrush<tManaged>::CreateDIBPatternBrush(const void* lpPackedDIB, UInt uUsage)
{
	assert(m_hBrush == nullptr);
	m_hBrush = ::CreateDIBPatternBrushPt(lpPackedDIB, uUsage);
	return m_hBrush;
}

template <bool tManaged>
INLINE HBRUSH CTXBrush<tManaged>::CreateSysColorBrush(Int nIndex)
{
	assert(m_hBrush == nullptr);
	m_hBrush = ::GetSysColorBrush(nIndex);
	return m_hBrush;
}

template <bool tManaged>
INLINE BOOL CTXBrush<tManaged>::DeleteObject(void)
{
	assert(m_hBrush != nullptr);
	if (::DeleteObject(m_hBrush))
	{
		m_hBrush = nullptr;
	}
	return (m_hBrush == nullptr);
}

template <bool tManaged>
INLINE Int CTXBrush<tManaged>::GetLogBrush(LOGBRUSH* pLogBrush) const
{
	assert(m_hBrush != nullptr);
	return ::GetObject(m_hBrush, sizeof(LOGBRUSH), pLogBrush);
}

template <bool tManaged>
INLINE bool CTXBrush<tManaged>::GetLogBrush(LOGBRUSH& LogBrushRef) const
{
	assert(m_hBrush != nullptr);
	return (::GetObject(m_hBrush, sizeof(LOGBRUSH), &LogBrushRef) == sizeof(LOGBRUSH));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXLogFont
INLINE CXLogFont::CXLogFont(HFONT hFont)
{
	if (hFont != nullptr)
	{
		assert(::GetObjectType(hFont) == OBJ_FONT);
		::GetObject(hFont, sizeof(LOGFONT), (LOGFONT*)this);
	}
	else
	{
		MM_SAFE::Memset(this, 0, sizeof(LOGFONT));
	}
}

INLINE CXLogFont::~CXLogFont(void)
{
}

INLINE CXLogFont::CXLogFont(const CXLogFont& aSrc)
{
	Copy(aSrc);
}

INLINE CXLogFont& CXLogFont::operator=(const CXLogFont& aSrc)
{
	assert(this != &aSrc);
	if (this != &aSrc)
	{
		Copy(aSrc);
	}
	return (*this);
}

INLINE CXLogFont::CXLogFont(const LOGFONT& LogFontRef)
{
	Copy(LogFontRef);
}

INLINE CXLogFont& CXLogFont::operator =(const LOGFONT& LogFontRef)
{
	Copy(LogFontRef);
	return (*this);
}

INLINE CXLogFont& CXLogFont::operator =(HFONT hFont)
{
	assert(::GetObjectType(hFont) == OBJ_FONT);
	::GetObject(hFont, sizeof(LOGFONT), (LOGFONT*)this);
	return (*this);
}

INLINE void CXLogFont::Copy(const LOGFONT& LogFontRef)
{
	*(LOGFONT*)this = LogFontRef;
}

INLINE bool CXLogFont::operator ==(const LOGFONT& LogFontRef) const
{
	return((LogFontRef.lfHeight         == lfHeight)         &&
	       (LogFontRef.lfWidth          == lfWidth)          &&
	       (LogFontRef.lfEscapement     == lfEscapement)     &&
	       (LogFontRef.lfOrientation    == lfOrientation)    &&
	       (LogFontRef.lfWeight         == lfWeight)         &&
	       (LogFontRef.lfItalic         == lfItalic)         &&
	       (LogFontRef.lfUnderline      == lfUnderline)      &&
	       (LogFontRef.lfStrikeOut      == lfStrikeOut)      &&
	       (LogFontRef.lfCharSet        == lfCharSet)        &&
	       (LogFontRef.lfOutPrecision   == lfOutPrecision)   &&
	       (LogFontRef.lfClipPrecision  == lfClipPrecision)  &&
	       (LogFontRef.lfQuality        == lfQuality)        &&
	       (LogFontRef.lfPitchAndFamily == lfPitchAndFamily) &&
		   (CXChar::Comparen(LogFontRef.lfFaceName, lfFaceName, LF_FACESIZE) == 0));
}

INLINE HFONT CXLogFont::CreateFontIndirect(void)
{
	return ::CreateFontIndirect(this);
}

INLINE void CXLogFont::SetBold(void)
{
	lfWeight = FW_BOLD;
}

INLINE bool CXLogFont::IsBold(void) const
{
	return (lfWeight >= FW_BOLD);
}

INLINE void CXLogFont::MakeBolder(Int nScale)
{
	lfWeight += (FW_BOLD * nScale);
}

INLINE void CXLogFont::MakeLarger(Int nScale)
{
	if (lfHeight > 0)
	{
		lfHeight += nScale;
	}
	else
	{
		lfHeight -= nScale;
	}
}

INLINE void CXLogFont::SetHeight(Long lPointSize, HDC hDC)
{
	// For MM_TEXT mapping mode
	lfHeight = -(::MulDiv(lPointSize, ::GetDeviceCaps(hDC, LOGPIXELSY), CXGDITraits::XGDIC_POINTS_INCH));
}

INLINE Long CXLogFont::GetHeight(HDC hDC) const
{
	// For MM_TEXT mapping mode
	return ::MulDiv(-lfHeight, CXGDITraits::XGDIC_POINTS_INCH, ::GetDeviceCaps(hDC, LOGPIXELSY));
}

INLINE Long CXLogFont::GetDeciPointHeight(HDC hDC) const
{
	POINT ptOrg = { 0, 0 };
	::DPtoLP(hDC, &ptOrg, 1);
	POINT pt = { 0, 0 };
	pt.y = DEF_BASE::Abs<Long>(lfHeight) + ptOrg.y;
	::LPtoDP(hDC, &pt, 1);
	return ::MulDiv(pt.y, CXGDITraits::XGDIC_POINTS_INCH * CXGDITraits::XGDIC_DECIPOINTS_POINT, ::GetDeviceCaps(hDC, LOGPIXELSY));
}

INLINE void CXLogFont::SetHeightFromDeciPoint(Long lDeciPtHeight, HDC hDC)
{
	POINT pt = { 0, 0 };
	pt.y = ::MulDiv(::GetDeviceCaps(hDC, LOGPIXELSY), lDeciPtHeight, CXGDITraits::XGDIC_POINTS_INCH * CXGDITraits::XGDIC_DECIPOINTS_POINT);
	::DPtoLP(hDC, &pt, 1);
	POINT ptOrg = { 0, 0 };
	::DPtoLP(hDC, &ptOrg, 1);
	lfHeight = -DEF_BASE::Abs<Long>(pt.y - ptOrg.y);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTXFont
template <bool tManaged>
INLINE CTXFont<tManaged>::CTXFont(HFONT hFont)
: m_hFont(hFont)
{
}

template <bool tManaged>
INLINE CTXFont<tManaged>::~CTXFont(void)
{
	if (tManaged && (m_hFont != nullptr))
	{
		DeleteObject();
	}
}

template <bool tManaged>
INLINE CTXFont<tManaged>::CTXFont(const CTXFont<tManaged>&)
: m_hFont(nullptr)
{
}

template <bool tManaged>
INLINE CTXFont<tManaged>& CTXFont<tManaged>::operator=(const CTXFont<tManaged>&)
{
	return (*this);
}

template <bool tManaged>
INLINE CTXFont<tManaged>& CTXFont<tManaged>::operator=(HFONT hFont)
{
	Attach(hFont);
	return (*this);
}

template <bool tManaged>
INLINE CTXFont<tManaged>::operator HFONT(void) const
{
	return m_hFont;
}

template <bool tManaged>
INLINE bool CTXFont<tManaged>::IsValid(void) const
{
	return (m_hFont != nullptr);
}

template <bool tManaged>
INLINE void CTXFont<tManaged>::Attach(HFONT hFont)
{
	assert(hFont);
	if (m_hFont != hFont)
	{
		if (tManaged && (m_hFont != nullptr))
		{
			DeleteObject();
		}
		m_hFont = hFont;
	}
}

template <bool tManaged>
INLINE HFONT CTXFont<tManaged>::Detach(void)
{
	HFONT hFont = m_hFont;
	m_hFont     = nullptr;
	return hFont;
}

// Create methods
template <bool tManaged>
INLINE HFONT CTXFont<tManaged>::CreateFontIndirect(const LOGFONT& LogFontRef)
{
	assert(m_hFont == nullptr);
	m_hFont = ::CreateFontIndirect(&LogFontRef);
	return m_hFont;
}

template <bool tManaged>
INLINE HFONT CTXFont<tManaged>::CreateFontIndirectEx(const ENUMLOGFONTEXDV& ExdvRef)
{
	assert(m_hFont == nullptr);
	m_hFont = ::CreateFontIndirectEx(&ExdvRef);
	return m_hFont;
}

template <bool tManaged>
INLINE HFONT CTXFont<tManaged>::CreateFont(Int nHeight, Int nWidth, Int nEscapement, Int nOrientation, Int nWeight, 
                                           Byte bItalic, Byte bUnderline, Byte bStrikeOut, Byte bCharSet, Byte bOutPrecision,
                                           Byte bClipPrecision, Byte bQuality, Byte bPitchAndFamily, PCXStr pszFaceName)
{
	assert(m_hFont == nullptr);
	m_hFont = ::CreateFont(nHeight, nWidth, nEscapement, nOrientation, nWeight,
	                       bItalic, bUnderline, bStrikeOut, bCharSet, bOutPrecision, 
						   bClipPrecision, bQuality, bPitchAndFamily, pszFaceName);
	return m_hFont;
}

template <bool tManaged>
INLINE HFONT CTXFont<tManaged>::CreatePointFont(Long lPointSize, PCXStr pszFaceName, HDC hDC, bool bBold, bool bItalic)
{
	LOGFONT lf = { 0 };
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfHeight  = lPointSize;
	CXChar::Copy(lf.lfFaceName, LF_FACESIZE, pszFaceName);

	if (bBold)
	{
		lf.lfWeight = FW_BOLD;
	}
	if (bItalic)
	{
		lf.lfItalic = TRUE;
	}
	return CreatePointFontIndirect(&lf, hDC);
}

template <bool tManaged>
INLINE HFONT CTXFont<tManaged>::CreatePointFontIndirect(const LOGFONT& LogFontRef, HDC hDC)
{
	HDC hCopyDC = hDC;
	if (hCopyDC == nullptr)
	{
		hCopyDC = ::GetDC(nullptr)
	}
	// convert nPointSize to logical units based on hDC
	LOGFONT lf  = LogFontRef;
	POINT   pt  = { 0, 0 };
	pt.y        = ::MulDiv(::GetDeviceCaps(hCopyDC, LOGPIXELSY), LogFont.lfHeight, CXGDITraits::XGDIC_POINTS_INCH * CXGDITraits::XGDIC_DECIPOINTS_POINT);
	::DPtoLP(hDC1, &pt, 1);
	POINT ptOrg = { 0, 0 };
	::DPtoLP(hDC1, &ptOrg, 1);
	lf.lfHeight = -DEF_BASE<Long>Abs(pt.y - ptOrg.y);

	if (hDC == nullptr)
	{
		::ReleaseDC(nullptr, hCopyDC);
	}

	return CreateFontIndirect(&lf);
}

template <bool tManaged>
INLINE BOOL CTXFont<tManaged>::DeleteObject(void)
{
	assert(m_hFont != nullptr);
	if (::DeleteObject(m_hFont))
	{
		m_hFont = nullptr;
	}
	return (m_hFont == nullptr);
}
// Attributes
template <bool tManaged>
INLINE Int CTXFont<tManaged>::GetLogFont(LOGFONT* pLogFont) const
{
	assert(m_hFont != nullptr);
	return ::GetObject(m_hFont, sizeof(LOGFONT), pLogFont);
}

template <bool tManaged>
INLINE bool CTXFont<tManaged>::GetLogFont(LOGFONT& LogFontRef) const
{
	assert(m_hFont != nullptr);
	return (::GetObject(m_hFont, sizeof(LOGFONT), &LogFontRef) == sizeof(LOGFONT));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTXImageList
template <bool tManaged>
INLINE CTXImageList<tManaged>::CTXImageList(HIMAGELIST hImg) 
: m_hImg(hImg)
{ 
}

template <bool tManaged>
INLINE CTXImageList<tManaged>::~CTXImageList(void)
{
	if (tManaged && (m_hImg != nullptr))
	{
		Destroy();
	}
}

template <bool tManaged>
INLINE CTXImageList<tManaged>::CTXImageList(const CTXImageList<tManaged>&)
: m_hImg(nullptr)
{
}

template <bool tManaged>
INLINE CTXImageList<tManaged>& CTXImageList<tManaged>::operator=(const CTXImageList<tManaged>&)
{
	return (*this);
}

template <bool tManaged>
INLINE CTXImageList<tManaged>& CTXImageList<tManaged>::operator=(HIMAGELIST hImg)
{
	Attach(hImg);
	return (*this);
}

template <bool tManaged>
INLINE CTXImageList<tManaged>::operator HIMAGELIST(void) const 
{
	return m_hImg;
}

template <bool tManaged>
INLINE bool CTXImageList<tManaged>::IsValid(void) const 
{ 
	return (m_hImg != nullptr);
}

template <bool tManaged>
INLINE void CTXImageList<tManaged>::Attach(HIMAGELIST hImg)
{
	assert(hImg);
	if (m_hImg != hImg)
	{
		if (tManaged && (m_hImg != nullptr))
		{
			Destroy();
		}
		m_hImg = hImg;
	}
}

template <bool tManaged>
INLINE HIMAGELIST CTXImageList<tManaged>::Detach(void)
{
	HIMAGELIST hImg = m_hImg;
	m_hImg          = nullptr;
	return hImg;
}

template <bool tManaged>
INLINE Int CTXImageList<tManaged>::GetImageCount(void) const
{
	assert(m_hImg != nullptr);
	return ::ImageList_GetImageCount(m_hImg);
}

template <bool tManaged>
INLINE COLORREF CTXImageList<tManaged>::GetBkColor(void) const
{
	assert(m_hImg != nullptr);
	return ::ImageList_GetBkColor(m_hImg);
}

template <bool tManaged>
INLINE COLORREF CTXImageList<tManaged>::SetBkColor(COLORREF clr)
{
	assert(m_hImg != nullptr);
	return ::ImageList_SetBkColor(m_hImg, clr);
}

template <bool tManaged>
INLINE BOOL CTXImageList<tManaged>::GetImageInfo(Int nImage, IMAGEINFO& ImageInfoRef) const
{
	assert(m_hImg != nullptr);
	return ::ImageList_GetImageInfo(m_hImg, nImage, &ImageInfoRef);
}

template <bool tManaged>
INLINE HICON CTXImageList<tManaged>::GetIcon(Int nIndex, UInt uFlags) const
{
	assert(m_hImg != nullptr);
	return ::ImageList_GetIcon(m_hImg, nIndex, uFlags);
}

template <bool tManaged>
INLINE BOOL CTXImageList<tManaged>::GetIconSize(Int& nx, Int& ny) const
{
	assert(m_hImg != nullptr);
	return ::ImageList_GetIconSize(m_hImg, &nx, &ny);
}

template <bool tManaged>
INLINE BOOL CTXImageList<tManaged>::GetIconSize(SIZE& size) const
{
	assert(m_hImg != nullptr);
	return ::ImageList_GetIconSize(m_hImg, (PInt)&size.cx, (PInt)&size.cy);
}

template <bool tManaged>
INLINE BOOL CTXImageList<tManaged>::SetIconSize(Int nx, Int ny)
{
	assert(m_hImg != nullptr);
	return ::ImageList_SetIconSize(m_hImg, nx, ny);
}

template <bool tManaged>
INLINE BOOL CTXImageList<tManaged>::SetIconSize(const SIZE& size)
{
	assert(m_hImg != nullptr);
	return ::ImageList_SetIconSize(m_hImg, size.cx, size.cy);
}

template <bool tManaged>
INLINE BOOL CTXImageList<tManaged>::SetImageCount(UInt uNewCount)
{
	assert(m_hImg != nullptr);
	return ::ImageList_SetImageCount(m_hImg, uNewCount);
}

template <bool tManaged>
INLINE BOOL CTXImageList<tManaged>::SetOverlayImage(Int nImage, Int nOverlay)
{
	assert(m_hImg != nullptr);
	return ::ImageList_SetOverlayImage(m_hImg, nImage, nOverlay);
}

template <bool tManaged>
INLINE BOOL CTXImageList<tManaged>::Create(Int nx, Int ny, UInt uFlags, Int nInitial, Int nGrow)
{
	assert(m_hImg == nullptr);
	m_hImg = ::ImageList_Create(nx, ny, uFlags, nInitial, nGrow);
	return (m_hImg != nullptr) ? TRUE : FALSE;
}

template <bool tManaged>
INLINE BOOL CTXImageList<tManaged>::Create(STR_ID Bmp, Int nx, Int nGrow, COLORREF clrMask, HMODULE hResInst)
{
	assert(m_hImg == nullptr);
	hResInst = hResInst ? hResInst : (::GetModuleHandle(nullptr));
	m_hImg   = ::ImageList_LoadBitmap(hResInst, Bmp.m_pszStr, nx, nGrow, clrMask);
	return (m_hImg != nullptr) ? TRUE : FALSE;
}

template <bool tManaged>
INLINE BOOL CTXImageList<tManaged>::CreateFromImage(STR_ID Img, Int nx, Int nGrow, COLORREF clrMask, UInt uType, UInt uFlags, HMODULE hResInst)
{
	assert(m_hImg == nullptr);
	hResInst = hResInst ? hResInst : (::GetModuleHandle(nullptr));
	m_hImg   = ::ImageList_LoadImage(hResInst, Img.m_pszStr, nx, nGrow, clrMask, uType, uFlags);
	return (m_hImg != nullptr) ? TRUE : FALSE;
}

template <bool tManaged>
INLINE BOOL CTXImageList<tManaged>::Merge(HIMAGELIST hImageList1, Int nImage1, HIMAGELIST hImageList2, Int nImage2, Int ndx, Int ndy)
{
	assert(m_hImg == nullptr);
	m_hImg = ::ImageList_Merge(hImageList1, nImage1, hImageList2, nImage2, ndx, ndy);
	return (m_hImg != nullptr) ? TRUE : FALSE;
}

template <bool tManaged>
INLINE BOOL CTXImageList<tManaged>::Destroy(void)
{
	assert(m_hImg != nullptr);
	if (::ImageList_Destroy(m_hImg))
	{
		m_hImg = nullptr;
	}
	return (m_hImg == nullptr);
}

template <bool tManaged>
INLINE Int CTXImageList<tManaged>::Add(HBITMAP hBitmap, HBITMAP hBitmapMask)
{
	assert(m_hImg != nullptr);
	return ::ImageList_Add(m_hImg, hBitmap, hBitmapMask);
}

template <bool tManaged>
INLINE Int CTXImageList<tManaged>::Add(HBITMAP hBitmap, COLORREF clrMask)
{
	assert(m_hImg != nullptr);
	return ::ImageList_AddMasked(m_hImg, hBitmap, clrMask);
}

template <bool tManaged>
INLINE BOOL CTXImageList<tManaged>::Remove(Int nImage)
{
	assert(m_hImg != nullptr);
	return ::ImageList_Remove(m_hImg, nImage);
}

template <bool tManaged>
INLINE BOOL CTXImageList<tManaged>::RemoveAll(void)
{
	assert(m_hImg != nullptr);
	return ::ImageList_RemoveAll(m_hImg);
}

template <bool tManaged>
INLINE BOOL CTXImageList<tManaged>::Replace(Int nImage, HBITMAP hBitmap, HBITMAP hBitmapMask)
{
	assert(m_hImg != nullptr);
	return ::ImageList_Replace(m_hImg, nImage, hBitmap, hBitmapMask);
}

template <bool tManaged>
INLINE Int CTXImageList<tManaged>::AddIcon(HICON hIcon)
{
	assert(m_hImg != nullptr);
	return ::ImageList_AddIcon(m_hImg, hIcon);
}

template <bool tManaged>
INLINE Int CTXImageList<tManaged>::ReplaceIcon(Int nImage, HICON hIcon)
{
	assert(m_hImg != nullptr);
	return ::ImageList_ReplaceIcon(m_hImg, nImage, hIcon);
}

template <bool tManaged>
INLINE HICON CTXImageList<tManaged>::ExtractIcon(Int nImage)
{
	assert(m_hImg != nullptr);
	return ::ImageList_ExtractIcon(nullptr, m_hImg, nImage);
}

template <bool tManaged>
INLINE BOOL CTXImageList<tManaged>::Draw(HDC hDC, Int nImage, Int nx, Int ny, UInt uStyle)
{
	assert(m_hImg != nullptr);
	assert(hDC != nullptr);
	return ::ImageList_Draw(m_hImg, nImage, hDC, nx, ny, uStyle);
}

template <bool tManaged>
INLINE BOOL CTXImageList<tManaged>::Draw(HDC hDC, Int nImage, POINT& pt, UInt uStyle)
{
	assert(m_hImg != nullptr);
	assert(hDC != nullptr);
	return ::ImageList_Draw(m_hImg, nImage, hDC, pt.x, pt.y, uStyle);
}

template <bool tManaged>
INLINE BOOL CTXImageList<tManaged>::DrawEx(Int nImage, HDC hDC, Int nx, Int ny, Int ndx, Int ndy, COLORREF clrBack, COLORREF clrFore, UInt uStyle)
{
	assert(m_hImg != nullptr);
	assert(hDC != nullptr);
	return ::ImageList_DrawEx(m_hImg, nImage, hDC, nx, ny, ndx, ndy, clrBack, clrFore, uStyle);
}

template <bool tManaged>
INLINE BOOL CTXImageList<tManaged>::DrawEx(Int nImage, HDC hDC, RECT& rect, COLORREF clrBack, COLORREF clrFore, UInt uStyle)
{
	assert(m_hImg != nullptr);
	assert(hDC != nullptr);
	return ::ImageList_DrawEx(m_hImg, nImage, hDC, rect.left, rect.top, (rect.right - rect.left), (rect.bottom - rect.top), clrBack, clrFore, uStyle);
}

template <bool tManaged>
INLINE BOOL CTXImageList<tManaged>::Copy(Int nSrc, Int nDst, UInt uFlags)
{
	assert(m_hImg != nullptr);
	return ::ImageList_Copy(m_hImg, nDst, m_hImg, nSrc, uFlags);
}
// Drag operations
template <bool tManaged>
INLINE BOOL CTXImageList<tManaged>::BeginDrag(Int nImage, POINT& ptHotSpot)
{
	assert(m_hImg != nullptr);
	return ::ImageList_BeginDrag(m_hImg, nImage, ptHotSpot.x, ptHotSpot.y);
}

template <bool tManaged>
INLINE BOOL CTXImageList<tManaged>::BeginDrag(Int nImage, Int nxHotSpot, Int nyHotSpot)
{
	assert(m_hImg != nullptr);
	return ::ImageList_BeginDrag(m_hImg, nImage, nxHotSpot, nyHotSpot);
}

template <bool tManaged>
INLINE BOOL CTXImageList<tManaged>::SetDragCursorImage(Int nDrag, POINT& ptHotSpot)
{
	assert(m_hImg != nullptr);
	return ::ImageList_SetDragCursorImage(m_hImg, nDrag, ptHotSpot.x, ptHotSpot.y);
}

template <bool tManaged>
INLINE BOOL CTXImageList<tManaged>::SetDragCursorImage(Int nDrag, Int nxHotSpot, Int nyHotSpot)
{
	assert(m_hImg != nullptr);
	return ::ImageList_SetDragCursorImage(m_hImg, nDrag, nxHotSpot, nyHotSpot);
}

template <bool tManaged>
INLINE CTXImageList<tManaged> CTXImageList<tManaged>::Duplicate(void) const
{
	assert(m_hImg != nullptr);
	return CTXImageList<tManaged>(::ImageList_Duplicate(m_hImg));
}

template <bool tManaged>
INLINE CTXImageList<tManaged> CTXImageList<tManaged>::Duplicate(HIMAGELIST hImg)
{
	assert(hImg != nullptr);
	return CTXImageList<tManaged>(::ImageList_Duplicate(hImg));
}

template <bool tManaged>
INLINE BOOL CTXImageList<tManaged>::DrawIndirect(IMAGELISTDRAWPARAMS* pimldp)
{
	return ::ImageList_DrawIndirect(pimldp);
}

template <bool tManaged>
INLINE void CTXImageList<tManaged>::EndDrag(void)
{
	::ImageList_EndDrag();
}

template <bool tManaged>
INLINE BOOL CTXImageList<tManaged>::DragMove(POINT& pt)
{
	return ::ImageList_DragMove(pt.x, pt.y);
}

template <bool tManaged>
INLINE BOOL CTXImageList<tManaged>::DragMove(Int nx, Int ny)
{
	return ::ImageList_DragMove(nx, ny);
}

template <bool tManaged>
INLINE BOOL CTXImageList<tManaged>::DragShowNolock(BOOL bShow)
{
	return ::ImageList_DragShowNolock(bShow);
}

template <bool tManaged>
INLINE CTXImageList<tManaged> CTXImageList<tManaged>::GetDragImage(POINT& pt, POINT& ptHotSpot)
{
	return CTXImageList<tManaged>(::ImageList_GetDragImage(&pt, &ptHotSpot));
}

template <bool tManaged>
INLINE BOOL CTXImageList<tManaged>::DragEnter(HWND hWnd, POINT& point)
{
	return ::ImageList_DragEnter(hWnd, point.x, point.y);
}

template <bool tManaged>
INLINE BOOL CTXImageList<tManaged>::DragEnter(HWND hWnd, Int nx, Int ny)
{
	return ::ImageList_DragEnter(hWnd, nx, ny);
}

template <bool tManaged>
INLINE BOOL CTXImageList<tManaged>::DragLeave(HWND hWnd)
{
	return ::ImageList_DragLeave(hWnd);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTXBitmap
template <bool tManaged>
INLINE CTXBitmap<tManaged>::CTXBitmap(HBITMAP hBitmap)
: m_hBitmap(hBitmap)
{
}

template <bool tManaged>
INLINE CTXBitmap<tManaged>::~CTXBitmap(void)
{
	if (tManaged && (m_hBitmap != nullptr))
	{
		DeleteObject();
	}
}

template <bool tManaged>
INLINE CTXBitmap<tManaged>::CTXBitmap(const CTXBitmap<tManaged>&)
: m_hBitmap(nullptr)
{
}

template <bool tManaged>
INLINE CTXBitmap<tManaged>& CTXBitmap<tManaged>::operator=(const CTXBitmap<tManaged>&)
{
	return (*this);
}

template <bool tManaged>
INLINE CTXBitmap<tManaged>& CTXBitmap<tManaged>::operator=(HBITMAP hBitmap)
{
	Attach(hBitmap);
	return (*this);
}

template <bool tManaged>
INLINE CTXBitmap<tManaged>::operator HBITMAP(void) const
{
	return m_hBitmap;
}

template <bool tManaged>
INLINE bool CTXBitmap<tManaged>::IsValid(void) const
{
	return (m_hBitmap != nullptr);
}

template <bool tManaged>
INLINE void CTXBitmap<tManaged>::Attach(HBITMAP hBitmap)
{
	assert(hBitmap);
	if (m_hBitmap != hBitmap)
	{
		if (tManaged && (m_hBitmap != nullptr))
		{
			DeleteObject();
		}
		m_hBitmap = hBitmap;
	}
}

template <bool tManaged>
INLINE HBITMAP CTXBitmap<tManaged>::Detach(void)
{
	HBITMAP hBitmap = m_hBitmap;
	m_hBitmap       = nullptr;
	return hBitmap;
}

// Create and load methods
template <bool tManaged>
INLINE HBITMAP CTXBitmap<tManaged>::LoadBitmap(HINSTANCE hInst, PXStr pszBitmap)
{
	assert(m_hBitmap == nullptr);
	m_hBitmap = ::LoadBitmap(hInst, pszBitmap);
	return m_hBitmap;
}

template <bool tManaged>
INLINE HBITMAP CTXBitmap<tManaged>::LoadOEMBitmap(UInt uIDBitmap) // for OBM_/OCR_/OIC_
{
	assert(m_hBitmap == nullptr);
	m_hBitmap = ::LoadBitmap(nullptr, MAKEINTRESOURCE(uIDBitmap));
	return m_hBitmap;
}

template <bool tManaged>
INLINE HBITMAP CTXBitmap<tManaged>::LoadMappedBitmap(HINSTANCE hInst, UInt uIDBitmap, UShort usFlags, LPCOLORMAP pColorMap, Int nMapSize)
{
	assert(m_hBitmap == nullptr);
	m_hBitmap = ::CreateMappedBitmap(hInst, uIDBitmap, usFlags, pColorMap, nMapSize);
	return m_hBitmap;
}

template <bool tManaged>
INLINE HBITMAP CTXBitmap<tManaged>::CreateBitmap(Int nWidth, Int nHeight, UInt uPlanes, UInt uBitsPerPixel, const void* pBits)
{
	assert(m_hBitmap == nullptr);
	m_hBitmap = ::CreateBitmap(nWidth, nHeight, uPlanes, uBitsPerPixel, pBits);
	return m_hBitmap;
}

template <bool tManaged>
INLINE HBITMAP CTXBitmap<tManaged>::CreateBitmapIndirect(BITMAP& BitmapRef)
{
	assert(m_hBitmap == nullptr);
	m_hBitmap = ::CreateBitmapIndirect(&BitmapRef);
	return m_hBitmap;
}

template <bool tManaged>
INLINE HBITMAP CTXBitmap<tManaged>::CreateCompatibleBitmap(HDC hDC, Int nWidth, Int nHeight)
{
	assert(m_hBitmap == nullptr);
	m_hBitmap = ::CreateCompatibleBitmap(hDC, nWidth, nHeight);
	return m_hBitmap;
}

template <bool tManaged>
INLINE HBITMAP CTXBitmap<tManaged>::CreateDiscardableBitmap(HDC hDC, Int nWidth, Int nHeight)
{
	assert(m_hBitmap == nullptr);
	m_hBitmap = ::CreateDiscardableBitmap(hDC, nWidth, nHeight);
	return m_hBitmap;
}

template <bool tManaged>
INLINE BOOL CTXBitmap<tManaged>::DeleteObject(void)
{
	assert(m_hBitmap != nullptr);
	if (::DeleteObject(m_hBitmap))
	{
		m_hBitmap = nullptr;
	}
	return (m_hBitmap == nullptr);
}

template <bool tManaged>
INLINE Int CTXBitmap<tManaged>::GetBitmap(BITMAP* pBitMap) const
{
	assert(m_hBitmap != nullptr);
	return ::GetObject(m_hBitmap, sizeof(BITMAP), pBitMap);
}

template <bool tManaged>
INLINE bool CTXBitmap<tManaged>::GetBitmap(BITMAP& BitmapRef) const
{
	assert(m_hBitmap != nullptr);
	return (::GetObject(m_hBitmap, sizeof(BITMAP), &BitmapRef) == sizeof(BITMAP));
}

template <bool tManaged>
INLINE bool CTXBitmap<tManaged>::GetSize(SIZE& size) const
{
	assert(m_hBitmap != nullptr);
	BITMAP bm = { 0 };
	if (GetBitmap(&bm))
	{
		size.cx = bm.bmWidth;
		size.cy = bm.bmHeight;
		return true;
	}
	return false;
}

template <bool tManaged>
INLINE ULong CTXBitmap<tManaged>::GetBitmapBits(ULong ulCount, void* lpBits) const
{
	assert(m_hBitmap != nullptr);
	return ::GetBitmapBits(m_hBitmap, ulCount, lpBits);
}

template <bool tManaged>
INLINE ULong CTXBitmap<tManaged>::SetBitmapBits(ULong ulCount, const void* lpBits)
{
	assert(m_hBitmap != nullptr);
	return ::SetBitmapBits(m_hBitmap, ulCount, lpBits);
}

template <bool tManaged>
INLINE BOOL CTXBitmap<tManaged>::GetBitmapDimension(LPSIZE lpSize) const
{
	assert(m_hBitmap != nullptr);
	return ::GetBitmapDimensionEx(m_hBitmap, lpSize);
}

template <bool tManaged>
INLINE BOOL CTXBitmap<tManaged>::SetBitmapDimension(Int nWidth, Int nHeight, LPSIZE lpSize)
{
	assert(m_hBitmap != nullptr);
	return ::SetBitmapDimensionEx(m_hBitmap, nWidth, nHeight, lpSize);
}

template <bool tManaged>
INLINE HBITMAP CTXBitmap<tManaged>::CreateDIBitmap(HDC hDC, const BITMAPINFOHEADER* lpbmih, ULong ulInit, const void* lpbInit, const BITMAPINFO* lpbmi, UInt uColorUse)
{
	assert(m_hBitmap == nullptr);
	m_hBitmap = ::CreateDIBitmap(hDC, lpbmih, ulInit, lpbInit, lpbmi, uColorUse);
	return m_hBitmap;
}

template <bool tManaged>
INLINE HBITMAP CTXBitmap<tManaged>::CreateDIBSection(HDC hDC, const BITMAPINFO* lpbmi, UInt uColorUse, void** ppvBits, HANDLE hSection, ULong ulOffset)
{
	assert(m_hBitmap == nullptr);
	m_hBitmap = ::CreateDIBSection(hDC, lpbmi, uColorUse, ppvBits, hSection, ulOffset);
	return m_hBitmap;
}

template <bool tManaged>
INLINE Int CTXBitmap<tManaged>::GetDIBits(HDC hDC, UInt uStartScan, UInt uScanLines, void* lpvBits, LPBITMAPINFO lpbmi, UInt uColorUse) const
{
	assert(m_hBitmap != nullptr);
	return ::GetDIBits(hDC, m_hBitmap, uStartScan, uScanLines,  lpvBits, lpbmi, uColorUse);
}

template <bool tManaged>
INLINE Int CTXBitmap<tManaged>::SetDIBits(HDC hDC, UInt uStartScan, UInt uScanLines, const void* lpvBits, const BITMAPINFO* lpbmi, UInt uColorUse)
{
	assert(m_hBitmap != nullptr);
	return ::SetDIBits(hDC, m_hBitmap, uStartScan, uScanLines, lpvBits, lpbmi, uColorUse);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTXRgn
template <bool tManaged>
INLINE CTXRgn<tManaged>::CTXRgn(HRGN hRgn)
: m_hRgn(hRgn)
{
}

template <bool tManaged>
INLINE CTXRgn<tManaged>::~CTXRgn(void)
{
	if (tManaged && (m_hRgn != nullptr))
	{
		DeleteObject();
	}
}

template <bool tManaged>
INLINE CTXRgn<tManaged>::CTXRgn(const CTXRgn<tManaged>&)
: m_hRgn(nullptr)
{
}

template <bool tManaged>
INLINE CTXRgn<tManaged>& CTXRgn<tManaged>::operator=(const CTXRgn<tManaged>&)
{
	return (*this);
}

template <bool tManaged>
INLINE CTXRgn<tManaged>& CTXRgn<tManaged>::operator=(HRGN hRgn)
{
	Attach(hRgn);
	return (*this);
}

template <bool tManaged>
INLINE CTXRgn<tManaged>::operator HRGN(void) const
{
	return m_hRgn;
}

template <bool tManaged>
INLINE bool CTXRgn<tManaged>::IsValid(void) const
{
	return (m_hRgn != nullptr);
}

template <bool tManaged>
INLINE void CTXRgn<tManaged>::Attach(HRGN hRgn)
{
	assert(hRgn);
	if (m_hRgn != hRgn)
	{
		if (tManaged && (m_hRgn != nullptr))
		{
			DeleteObject();
		}
		m_hRgn = hRgn;
	}
}

template <bool tManaged>
INLINE HRGN CTXRgn<tManaged>::Detach(void)
{
	HRGN hRgn = m_hRgn;
	m_hRgn    = nullptr;
	return hRgn;
}

template <bool tManaged>
INLINE HRGN CTXRgn<tManaged>::CreateRectRgn(Int nx1, Int ny1, Int nx2, Int ny2)
{
	assert(m_hRgn == nullptr);
	m_hRgn = ::CreateRectRgn(nx1, ny1, nx2, ny2)
	return m_hRgn;
}

template <bool tManaged>
INLINE HRGN CTXRgn<tManaged>::CreateRectRgnIndirect(LPCRECT lpRect)
{
	assert(m_hRgn == nullptr);
	m_hRgn = ::CreateRectRgnIndirect(lpRect);
	return m_hRgn;
}

template <bool tManaged>
INLINE HRGN CTXRgn<tManaged>::CreateEllipticRgn(Int nx1, Int ny1, Int nx2, Int ny2)
{
	assert(m_hRgn == nullptr);
	m_hRgn = ::CreateEllipticRgn(nx1, ny1, nx2, ny2);
	return m_hRgn;
}

template <bool tManaged>
INLINE HRGN CTXRgn<tManaged>::CreateEllipticRgnIndirect(LPCRECT lpRect)
{
	assert(m_hRgn == nullptr);
	m_hRgn = ::CreateEllipticRgnIndirect(lpRect);
	return m_hRgn;
}

template <bool tManaged>
INLINE HRGN CTXRgn<tManaged>::CreatePolygonRgn(LPPOINT lpPoints, Int nCount, Int nMode)
{
	assert(m_hRgn == nullptr);
	m_hRgn = ::CreatePolygonRgn(lpPoints, nCount, nMode);
	return m_hRgn;
}

template <bool tManaged>
INLINE HRGN CTXRgn<tManaged>::CreatePolyPolygonRgn(LPPOINT lpPoints, PInt lpnPolyCounts, Int nCount, Int nPolyFillMode)
{
	assert(m_hRgn == nullptr);
	m_hRgn = ::CreatePolyPolygonRgn(lpPoints, lpPolyCounts, nCount, nPolyFillMode);
	return m_hRgn;
}

template <bool tManaged>
INLINE HRGN CTXRgn<tManaged>::CreateRoundRectRgn(Int nx1, Int ny1, Int nx2, Int ny2, Int nx3, Int ny3)
{
	assert(m_hRgn == nullptr);
	m_hRgn = ::CreateRoundRectRgn(nx1, ny1, nx2, ny2, nx3, ny3);
	return m_hRgn;
}

template <bool tManaged>
INLINE HRGN CTXRgn<tManaged>::CreateFromPath(HDC hDC)
{
	assert(m_hRgn == nullptr);
	assert(hDC != nullptr);
	m_hRgn = ::PathToRegion(hDC);
	return m_hRgn;
}

template <bool tManaged>
INLINE HRGN CTXRgn<tManaged>::CreateFromData(const XFORM* lpXForm, Int nCount, const RGNDATA* pRgnData)
{
	assert(m_hRgn == nullptr);
	m_hRgn = ::ExtCreateRegion(lpXForm, nCount, pRgnData);
	return m_hRgn;
}

template <bool tManaged>
INLINE BOOL CTXRgn<tManaged>::DeleteObject(void)
{
	assert(m_hRgn != nullptr);
	if (::DeleteObject(m_hRgn))
	{
		m_hRgn = nullptr;
	}
	return (m_hRgn == nullptr);
}

template <bool tManaged>
INLINE void CTXRgn<tManaged>::SetRectRgn(Int nx1, Int ny1, Int nx2, Int ny2)
{
	assert(m_hRgn != nullptr);
	::SetRectRgn(m_hRgn, nx1, ny1, nx2, ny2);
}

template <bool tManaged>
INLINE void CTXRgn<tManaged>::SetRectRgn(LPCRECT lpRect)
{
	assert(m_hRgn != nullptr);
	::SetRectRgn(m_hRgn, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
}

template <bool tManaged>
INLINE Int CTXRgn<tManaged>::CombineRgn(HRGN hRgnSrc1, HRGN hRgnSrc2, Int nCombineMode)
{
	assert(m_hRgn != nullptr);
	return ::CombineRgn(m_hRgn, hRgnSrc1, hRgnSrc2, nCombineMode);
}

template <bool tManaged>
INLINE Int CTXRgn<tManaged>::CombineRgn(HRGN hRgnSrc, Int nCombineMode)
{
	assert(m_hRgn != nullptr);
	return ::CombineRgn(m_hRgn, m_hRgn, hRgnSrc, nCombineMode);
}

template <bool tManaged>
INLINE Int CTXRgn<tManaged>::CopyRgn(HRGN hRgnSrc)
{
	assert(m_hRgn != nullptr);
	return ::CombineRgn(m_hRgn, hRgnSrc, nullptr, RGN_COPY);
}

template <bool tManaged>
INLINE BOOL CTXRgn<tManaged>::EqualRgn(HRGN hRgn) const
{
	assert(m_hRgn != nullptr);
	return ::EqualRgn(m_hRgn, hRgn);
}

template <bool tManaged>
INLINE Int CTXRgn<tManaged>::OffsetRgn(Int nx, Int ny)
{
	assert(m_hRgn != nullptr);
	return ::OffsetRgn(m_hRgn, nx, ny);
}

template <bool tManaged>
INLINE Int CTXRgn<tManaged>::OffsetRgn(POINT& point)
{
	assert(m_hRgn != nullptr);
	return ::OffsetRgn(m_hRgn, point.x, point.y);
}

template <bool tManaged>
INLINE Int CTXRgn<tManaged>:: GetRgnBox(LPRECT lpRect) const
{
	assert(m_hRgn != nullptr);
	return ::GetRgnBox(m_hRgn, lpRect);
}

template <bool tManaged>
INLINE BOOL CTXRgn<tManaged>::PtInRegion(Int nx, Int ny) const
{
	assert(m_hRgn != nullptr);
	return ::PtInRegion(m_hRgn, nx, ny);
}

template <bool tManaged>
INLINE BOOL CTXRgn<tManaged>::PtInRegion(POINT& point) const
{
	assert(m_hRgn != nullptr);
	return ::PtInRegion(m_hRgn, point.x, point.y);
}

template <bool tManaged>
INLINE BOOL CTXRgn<tManaged>::RectInRegion(LPCRECT lpRect) const
{
	assert(m_hRgn != nullptr);
	return ::RectInRegion(m_hRgn, lpRect);
}

template <bool tManaged>
INLINE Int CTXRgn<tManaged>::GetRegionData(LPRGNDATA lpRgnData, Int nDataSize) const
{
	assert(m_hRgn != nullptr);
	return (Int)::GetRegionData(m_hRgn, nDataSize, lpRgnData);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTXDC
template <bool tManaged>
INLINE CTXDC<tManaged>::CTXDC(HDC hDC)
: m_hDC(hDC)
{
}

template <bool tManaged>
INLINE CTXDC<tManaged>::~CTXDC(void)
{
	if (tManaged && (m_hDC != nullptr))
	{
		DeleteDC();
	}
}

template <bool tManaged>
INLINE CTXDC<tManaged>::CTXDC(const CTXDC<tManaged>&)
: m_hDC(nullptr)
{
}

template <bool tManaged>
INLINE CTXDC<tManaged>& CTXDC<tManaged>::operator=(const CTXDC<tManaged>&)
{
	return (*this);
}

template <bool tManaged>
INLINE CTXDC<tManaged>& CTXDC<tManaged>::operator=(HDC hDC)
{
	Attach(hDC);
	return (*this);
}

template <bool tManaged>
INLINE CTXDC<tManaged>::operator HDC(void) const
{
	return m_hDC;
}

template <bool tManaged>
INLINE bool CTXDC<tManaged>::IsValid(void) const
{
	return (m_hDC != nullptr);
}

template <bool tManaged>
INLINE void CTXDC<tManaged>::Attach(HDC hDC)
{
	assert(hDC);
	if (m_hDC != hDC)
	{
		if (tManaged && (m_hDC != nullptr))
		{
			DeleteDC();
		}
		m_hDC = hDC;
	}
}

template <bool tManaged>
INLINE HDC CTXDC<tManaged>::Detach(void)
{
	HDC hDC = m_hDC;
	m_hDC   = nullptr;
	return hDC;
}

// Operations
template <bool tManaged>
INLINE HWND CTXDC<tManaged>::WindowFromDC(void) const
{
	assert(m_hDC != nullptr);
	return ::WindowFromDC(m_hDC);
}

template <bool tManaged>
INLINE CXPenRef CTXDC<tManaged>::GetCurrentPen(void) const
{
	assert(m_hDC != nullptr);
	return CXPenRef((HPEN)::GetCurrentObject(m_hDC, OBJ_PEN));
}

template <bool tManaged>
INLINE CXBrushRef CTXDC<tManaged>::GetCurrentBrush(void) const
{
	assert(m_hDC != nullptr);
	return CXBrushRef((HBRUSH)::GetCurrentObject(m_hDC, OBJ_BRUSH));
}

template <bool tManaged>
INLINE CXFontRef CTXDC<tManaged>::GetCurrentFont(void) const
{
	assert(m_hDC != nullptr);
	return CXFontRef((HFONT)::GetCurrentObject(m_hDC, OBJ_FONT));
}

template <bool tManaged>
INLINE CXBitmapRef CTXDC<tManaged>::GetCurrentBitmap(void) const
{
	assert(m_hDC != nullptr);
	return CXBitmapRef((HBITMAP)::GetCurrentObject(m_hDC, OBJ_BITMAP));
}

template <bool tManaged>
INLINE HDC CTXDC<tManaged>::CreateDC(PCXStr pszDriverName, PCXStr pszDeviceName, PCXStr pszOutput, const DEVMODE* lpInitData)
{
	assert(m_hDC == nullptr);
	m_hDC = ::CreateDC(pszDriverName, pszDeviceName, pszOutput, lpInitData);
	return m_hDC;
}

template <bool tManaged>
INLINE HDC CTXDC<tManaged>::CreateCompatibleDC(HDC hDC)
{
	assert(m_hDC == nullptr);
	m_hDC = ::CreateCompatibleDC(hDC);
	return m_hDC;
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::DeleteDC(void)
{
	if (m_hDC != nullptr)
	{
		if (::DeleteDC(m_hDC))
		{
			m_hDC = nullptr;
		}
	}
	return (m_hDC == nullptr);
}

// Device-Context Functions
template <bool tManaged>
INLINE Int CTXDC<tManaged>::SaveDC(void)
{
	assert(m_hDC != nullptr);
	return ::SaveDC(m_hDC);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::RestoreDC(Int nSavedDC)
{
	assert(m_hDC != nullptr);
	return ::RestoreDC(m_hDC, nSavedDC);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::GetDeviceCaps(Int nIndex) const
{
	assert(m_hDC != nullptr);
	return ::GetDeviceCaps(m_hDC, nIndex);
}

template <bool tManaged>
INLINE UInt CTXDC<tManaged>::SetBoundsRect(LPCRECT lpRectBounds, UInt uFlags)
{
	assert(m_hDC != nullptr);
	return ::SetBoundsRect(m_hDC, lpRectBounds, uFlags);
}

template <bool tManaged>
INLINE UInt CTXDC<tManaged>::GetBoundsRect(LPRECT lpRectBounds, UInt uFlags) const
{
	assert(m_hDC != nullptr);
	return ::GetBoundsRect(m_hDC, lpRectBounds, uFlags);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::ResetDC(const DEVMODE* lpDevMode)
{
	assert(m_hDC != nullptr);
	return ::ResetDC(m_hDC, lpDevMode) != nullptr;
}
// Drawing-Tool Functions
template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::GetBrushOrg(LPPOINT lpPoint) const
{
	assert(m_hDC != nullptr);
	return ::GetBrushOrgEx(m_hDC, lpPoint);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::SetBrushOrg(Int nx, Int ny, LPPOINT lpPoint)
{
	assert(m_hDC != nullptr);
	return ::SetBrushOrgEx(m_hDC, nx, ny, lpPoint);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::SetBrushOrg(POINT& point, LPPOINT lpPointRet)
{
	assert(m_hDC != nullptr);
	return ::SetBrushOrgEx(m_hDC, point.x, point.y, lpPointRet);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::EnumObjects(Int nObjectType, Int (CALLBACK* lpfn)(void*, LPARAM), LPARAM lpData)
{
	assert(m_hDC != nullptr);
	return ::EnumObjects(m_hDC, nObjectType, (GOBJENUMPROC)lpfn, lpData);
}
// Type-safe selection helpers
template <bool tManaged>
INLINE HPEN CTXDC<tManaged>::SelectPen(HPEN hPen)
{
	assert(m_hDC != nullptr);
	assert((hPen != nullptr) || (::GetObjectType(hPen) == OBJ_PEN) || (::GetObjectType(hPen) == OBJ_EXTPEN));
	return (HPEN)::SelectObject(m_hDC, hPen);
}

template <bool tManaged>
INLINE HBRUSH CTXDC<tManaged>::SelectBrush(HBRUSH hBrush)
{
	assert(m_hDC != nullptr);
	assert((hBrush == nullptr) || (::GetObjectType(hBrush) == OBJ_BRUSH));
	return (HBRUSH)::SelectObject(m_hDC, hBrush);
}

template <bool tManaged>
INLINE HFONT CTXDC<tManaged>::SelectFont(HFONT hFont)
{
	assert(m_hDC != nullptr);
	assert((hFont == nullptr) || (::GetObjectType(hFont) == OBJ_FONT));
	return (HFONT)::SelectObject(m_hDC, hFont);
}

template <bool tManaged>
INLINE HBITMAP CTXDC<tManaged>::SelectBitmap(HBITMAP hBitmap)
{
	assert(m_hDC != nullptr);
	assert((hBitmap == nullptr) || (::GetObjectType(hBitmap) == OBJ_BITMAP));
	return (HBITMAP)::SelectObject(m_hDC, hBitmap);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::SelectRgn(HRGN hRgn)       // special return for regions
{
	assert(m_hDC != nullptr);
	assert((hRgn == nullptr) || (::GetObjectType(hRgn) == OBJ_REGION));
	return (Int)(intptr_t)(::SelectObject(m_hDC, hRgn));
}
// Type-safe selection helpers for stock objects
template <bool tManaged>
INLINE HPEN CTXDC<tManaged>::SelectStockPen(Int nPen)
{
	assert(m_hDC != nullptr);
	assert((nPen == WHITE_PEN) || (nPen == BLACK_PEN) || (nPen == NULL_PEN) || (nPen == DC_PEN));
	return SelectPen((HPEN)::GetStockObject(nPen));
}

template <bool tManaged>
INLINE HBRUSH CTXDC<tManaged>::SelectStockBrush(Int nBrush)
{
	assert(((nBrush >= WHITE_BRUSH) && (nBrush <= HOLLOW_BRUSH)) || (nBrush == DC_BRUSH));
	return SelectBrush((HBRUSH)::GetStockObject(nBrush));
}

template <bool tManaged>
INLINE HFONT CTXDC<tManaged>::SelectStockFont(Int nFont)
{
	assert(((nFont >= OEM_FIXED_FONT) && (nFont <= SYSTEM_FIXED_FONT)) || (nFont == DEFAULT_GUI_FONT));
	return SelectFont((HFONT)::GetStockObject(nFont));
}

template <bool tManaged>
INLINE HPALETTE CTXDC<tManaged>::SelectStockPalette(Int nPalette, BOOL bForceBackground)
{
	assert(nPalette == DEFAULT_PALETTE); // the only one supported
	return SelectPalette((HPALETTE)::GetStockObject(nPalette), bForceBackground);
}
// Color and Color Palette Functions
template <bool tManaged>
INLINE COLORREF CTXDC<tManaged>::GetNearestColor(COLORREF clrColor) const
{
	assert(m_hDC != nullptr);
	return ::GetNearestColor(m_hDC, clrColor);
}

template <bool tManaged>
INLINE HPALETTE CTXDC<tManaged>::SelectPalette(HPALETTE hPalette, BOOL bForceBackground)
{
	assert(m_hDC != nullptr);
	return ::SelectPalette(m_hDC, hPalette, bForceBackground);
}

template <bool tManaged>
INLINE UInt CTXDC<tManaged>::RealizePalette(void)
{
	assert(m_hDC != nullptr);
	return ::RealizePalette(m_hDC);
}

template <bool tManaged>
INLINE void CTXDC<tManaged>::UpdateColors(void)
{
	assert(m_hDC != nullptr);
	::UpdateColors(m_hDC);
}
// Drawing-Attribute Functions
template <bool tManaged>
INLINE COLORREF CTXDC<tManaged>::GetBkColor(void) const
{
	assert(m_hDC != nullptr);
	return ::GetBkColor(m_hDC);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::GetBkMode(void) const
{
	assert(m_hDC != nullptr);
	return ::GetBkMode(m_hDC);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::GetPolyFillMode(void) const
{
	assert(m_hDC != nullptr);
	return ::GetPolyFillMode(m_hDC);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::GetROP2(void) const
{
	assert(m_hDC != nullptr);
	return ::GetROP2(m_hDC);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::GetStretchBltMode(void) const
{
	assert(m_hDC != nullptr);
	return ::GetStretchBltMode(m_hDC);
}

template <bool tManaged>
INLINE COLORREF CTXDC<tManaged>::GetTextColor(void) const
{
	assert(m_hDC != nullptr);
	return ::GetTextColor(m_hDC);
}

template <bool tManaged>
INLINE COLORREF CTXDC<tManaged>::SetBkColor(COLORREF clrColor)
{
	assert(m_hDC != nullptr);
	return ::SetBkColor(m_hDC, clrColor);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::SetBkMode(Int nBkMode)
{
	assert(m_hDC != nullptr);
	return ::SetBkMode(m_hDC, nBkMode);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::SetPolyFillMode(Int nPolyFillMode)
{
	assert(m_hDC != nullptr);
	return ::SetPolyFillMode(m_hDC, nPolyFillMode);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::SetROP2(Int nDrawMode)
{
	assert(m_hDC != nullptr);
	return ::SetROP2(m_hDC, nDrawMode);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::SetStretchBltMode(Int nStretchMode)
{
	assert(m_hDC != nullptr);
	return ::SetStretchBltMode(m_hDC, nStretchMode);
}

template <bool tManaged>
INLINE COLORREF CTXDC<tManaged>::SetTextColor(COLORREF clrColor)
{
	assert(m_hDC != nullptr);
	return ::SetTextColor(m_hDC, clrColor);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::GetColorAdjustment(LPCOLORADJUSTMENT lpColorAdjust) const
{
	assert(m_hDC != nullptr);
	return ::GetColorAdjustment(m_hDC, lpColorAdjust);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::SetColorAdjustment(const COLORADJUSTMENT* lpColorAdjust)
{
	assert(m_hDC != nullptr);
	return ::SetColorAdjustment(m_hDC, lpColorAdjust);
}
// Mapping Functions
template <bool tManaged>
INLINE Int CTXDC<tManaged>::GetMapMode(void) const
{
	assert(m_hDC != nullptr);
	return ::GetMapMode(m_hDC);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::GetViewportOrg(LPPOINT lpPoint) const
{
	assert(m_hDC != nullptr);
	return ::GetViewportOrgEx(m_hDC, lpPoint);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::SetMapMode(Int nMapMode)
{
	assert(m_hDC != nullptr);
	return ::SetMapMode(m_hDC, nMapMode);
}
// Viewport Origin
template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::SetViewportOrg(Int nx, Int ny, LPPOINT lpPoint)
{
	assert(m_hDC != nullptr);
	return ::SetViewportOrgEx(m_hDC, nx, ny, lpPoint);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::SetViewportOrg(POINT& point, LPPOINT lpPointRet)
{
	assert(m_hDC != nullptr);
	return SetViewportOrg(point.x, point.y, lpPointRet);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::OffsetViewportOrg(Int nWidth, Int nHeight, LPPOINT lpPoint)
{
	assert(m_hDC != nullptr);
	return ::OffsetViewportOrgEx(m_hDC, nWidth, nHeight, lpPoint);
}
// Viewport Extent
template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::GetViewportExt(LPSIZE lpSize) const
{
	assert(m_hDC != nullptr);
	return ::GetViewportExtEx(m_hDC, lpSize);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::SetViewportExt(Int nx, Int ny, LPSIZE lpSize)
{
	assert(m_hDC != nullptr);
	return ::SetViewportExtEx(m_hDC, nx, ny, lpSize);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::SetViewportExt(SIZE& size, LPSIZE lpSizeRet)
{
	assert(m_hDC != nullptr);
	return SetViewportExt(size.cx, size.cy, lpSizeRet);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::ScaleViewportExt(Int nxNum, Int nxDenom, Int nyNum, Int nyDenom, LPSIZE lpSize)
{
	assert(m_hDC != nullptr);
	return ::ScaleViewportExtEx(m_hDC, nxNum, nxDenom, nyNum, nyDenom, lpSize);
}
// Window Origin
template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::GetWindowOrg(LPPOINT lpPoint) const
{
	assert(m_hDC != nullptr);
	return ::GetWindowOrgEx(m_hDC, lpPoint);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::SetWindowOrg(Int nx, Int ny, LPPOINT lpPoint)
{
	assert(m_hDC != nullptr);
	return ::SetWindowOrgEx(m_hDC, nx, ny, lpPoint);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::SetWindowOrg(POINT& point, LPPOINT lpPointRet)
{
	assert(m_hDC != nullptr);
	return SetWindowOrg(point.x, point.y, lpPointRet);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::OffsetWindowOrg(Int nWidth, Int nHeight, LPPOINT lpPoint)
{
	assert(m_hDC != nullptr);
	return ::OffsetWindowOrgEx(m_hDC, nWidth, nHeight, lpPoint);
}
// Window extent
template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::GetWindowExt(LPSIZE lpSize) const
{
	assert(m_hDC != nullptr);
	return ::GetWindowExtEx(m_hDC, lpSize);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::SetWindowExt(Int nx, Int ny, LPSIZE lpSize)
{
	assert(m_hDC != nullptr);
	return ::SetWindowExtEx(m_hDC, nx, ny, lpSize);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::SetWindowExt(SIZE& size, LPSIZE lpSizeRet)
{
	assert(m_hDC != nullptr);
	return SetWindowExt(size.cx, size.cy, lpSizeRet);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::ScaleWindowExt(Int nxNum, Int nxDenom, Int nyNum, Int nyDenom, LPSIZE lpSize)
{
	assert(m_hDC != nullptr);
	return ::ScaleWindowExtEx(m_hDC, nxNum, nxDenom, nyNum, nyDenom, lpSize);
}
// Coordinate Functions
template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::DPtoLP(LPPOINT lpPoints, Int nCount) const
{
	assert(m_hDC != nullptr);
	return ::DPtoLP(m_hDC, lpPoints, nCount);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::DPtoLP(LPRECT lpRect) const
{
	assert(m_hDC != nullptr);
	return ::DPtoLP(m_hDC, (LPPOINT)lpRect, 2);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::DPtoLP(LPSIZE lpSize) const
{
	SIZE sizeWinExt = { 0, 0 };
	if (GetWindowExt(&sizeWinExt) == FALSE)
	{
		return FALSE;
	}
	SIZE sizeVpExt = { 0, 0 };
	if (GetViewportExt(&sizeVpExt) == FALSE)
	{
		return FALSE;
	}
	lpSize->cx = ::MulDiv(lpSize->cx, DEF_BASE<Long>Abs(sizeWinExt.cx), DEF_BASE<Long>Abs(sizeVpExt.cx));
	lpSize->cy = ::MulDiv(lpSize->cy, DEF_BASE<Long>Abs(sizeWinExt.cy), DEF_BASE<Long>Abs(sizeVpExt.cy));
	return TRUE;
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::LPtoDP(LPPOINT lpPoints, Int nCount) const
{
	assert(m_hDC != nullptr);
	return ::LPtoDP(m_hDC, lpPoints, nCount);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::LPtoDP(LPRECT lpRect) const
{
	assert(m_hDC != nullptr);
	return ::LPtoDP(m_hDC, (LPPOINT)lpRect, 2);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::LPtoDP(LPSIZE lpSize) const
{
	SIZE sizeWinExt = { 0, 0 };
	if (GetWindowExt(&sizeWinExt) == FALSE)
	{
		return FALSE;
	}
	SIZE sizeVpExt = { 0, 0 };
	if (GetViewportExt(&sizeVpExt) == FALSE)
	{
		return FALSE;
	}
	lpSize->cx = ::MulDiv(lpSize->cx, DEF_BASE<Long>Abs(sizeVpExt.cx), DEF_BASE<Long>Abs(sizeWinExt.cx));
	lpSize->cy = ::MulDiv(lpSize->cy, DEF_BASE<Long>Abs(sizeVpExt.cy), DEF_BASE<Long>Abs(sizeWinExt.cy));
	return TRUE;
}

template <bool tManaged>
INLINE void CTXDC<tManaged>::DPtoHIMETRIC(LPSIZE lpSize) const
{
	assert(m_hDC != nullptr);
	Int nMapMode = ::GetMapMode(m_hDC);
	if ((nMapMode < MM_ISOTROPIC) && (nMapMode != MM_TEXT))
	{
		::SetMapMode(m_hDC, MM_HIMETRIC);
		DPtoLP(lpSize);
		::SetMapMode(m_hDC, nMapMode);
	}
	else
	{
		Int nxPerInch = ::GetDeviceCaps(m_hDC, LOGPIXELSX);
		Int nyPerInch = ::GetDeviceCaps(m_hDC, LOGPIXELSY);
		assert((nxPerInch != 0) && (nyPerInch != 0));
		lpSize->cx = ::MulDiv(lpSize->cx, CXGDITraits::XGDIC_HIMETRIC_INCH, nxPerInch);
		lpSize->cy = ::MulDiv(lpSize->cy, CXGDITraits::XGDIC_HIMETRIC_INCH, nyPerInch);
	}
}

template <bool tManaged>
INLINE void CTXDC<tManaged>::HIMETRICtoDP(LPSIZE lpSize) const
{
	assert(m_hDC != nullptr);
	Int nMapMode = ::GetMapMode(m_hDC);
	if ((nMapMode < MM_ISOTROPIC) && (nMapMode != MM_TEXT))
	{
		::SetMapMode(m_hDC, MM_HIMETRIC);
		LPtoDP(lpSize);
		::SetMapMode(m_hDC, nMapMode);
	}
	else
	{
		Int nxPerInch = ::GetDeviceCaps(m_hDC, LOGPIXELSX);
		Int nyPerInch = ::GetDeviceCaps(m_hDC, LOGPIXELSY);
		assert((nxPerInch != 0) && (nyPerInch != 0));
		lpSize->cx = ::MulDiv(lpSize->cx, nxPerInch, CXGDITraits::XGDIC_HIMETRIC_INCH);
		lpSize->cy = ::MulDiv(lpSize->cy, nyPerInch, CXGDITraits::XGDIC_HIMETRIC_INCH);
	}
}

template <bool tManaged>
INLINE void CTXDC<tManaged>::LPtoHIMETRIC(LPSIZE lpSize) const
{
	LPtoDP(lpSize);
	DPtoHIMETRIC(lpSize);
}

template <bool tManaged>
INLINE void CTXDC<tManaged>::HIMETRICtoLP(LPSIZE lpSize) const
{
	HIMETRICtoDP(lpSize);
	DPtoLP(lpSize);
}
// Region Functions
template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::FillRgn(HRGN hRgn, HBRUSH hBrush)
{
	assert(m_hDC != nullptr);
	return ::FillRgn(m_hDC, hRgn, hBrush);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::FrameRgn(HRGN hRgn, HBRUSH hBrush, Int nWidth, Int nHeight)
{
	assert(m_hDC != nullptr);
	return ::FrameRgn(m_hDC, hRgn, hBrush, nWidth, nHeight);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::InvertRgn(HRGN hRgn)
{
	assert(m_hDC != nullptr);
	return ::InvertRgn(m_hDC, hRgn);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::PaintRgn(HRGN hRgn)
{
	assert(m_hDC != nullptr);
	return ::PaintRgn(m_hDC, hRgn);
}
// Clipping Functions
template <bool tManaged>
INLINE Int CTXDC<tManaged>::GetClipBox(LPRECT lpRect) const
{
	assert(m_hDC != nullptr);
	return ::GetClipBox(m_hDC, lpRect);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::PtVisible(Int nx, Int ny) const
{
	assert(m_hDC != nullptr);
	return ::PtVisible(m_hDC, nx, ny);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::PtVisible(POINT& point) const
{
	assert(m_hDC != nullptr);
	return ::PtVisible(m_hDC, point.x, point.y);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::RectVisible(LPCRECT lpRect) const
{
	assert(m_hDC != nullptr);
	return ::RectVisible(m_hDC, lpRect);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::SelectClipRgn(HRGN hRgn)
{
	assert(m_hDC != nullptr);
	return ::SelectClipRgn(m_hDC, (HRGN)hRgn);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::ExcludeClipRect(Int nx1, Int ny1, Int nx2, Int ny2)
{
	assert(m_hDC != nullptr);
	return ::ExcludeClipRect(m_hDC, nx1, ny1, nx2, ny2);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::ExcludeClipRect(LPCRECT lpRect)
{
	assert(m_hDC != nullptr);
	return ::ExcludeClipRect(m_hDC, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::ExcludeUpdateRgn(HWND hWnd)
{
	assert(m_hDC != nullptr);
	return ::ExcludeUpdateRgn(m_hDC, hWnd);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::IntersectClipRect(Int nx1, Int ny1, Int nx2, Int ny2)
{
	assert(m_hDC != nullptr);
	return ::IntersectClipRect(m_hDC, nx1, ny1, nx2, ny2);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::IntersectClipRect(LPCRECT lpRect)
{
	assert(m_hDC != nullptr);
	return ::IntersectClipRect(m_hDC, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::OffsetClipRgn(Int nx, Int ny)
{
	assert(m_hDC != nullptr);
	return ::OffsetClipRgn(m_hDC, nx, ny);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::OffsetClipRgn(SIZE& size)
{
	assert(m_hDC != nullptr);
	return ::OffsetClipRgn(m_hDC, size.cx, size.cy);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::SelectClipRgn(HRGN hRgn, Int nMode)
{
	assert(m_hDC != nullptr);
	return ::ExtSelectClipRgn(m_hDC, hRgn, nMode);
}
// Line-Output Functions
template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::GetCurrentPosition(LPPOINT lpPoint) const
{
	assert(m_hDC != nullptr);
	return ::GetCurrentPositionEx(m_hDC, lpPoint);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::MoveTo(Int nx, Int ny, LPPOINT lpPoint = nullptr)
{
	assert(m_hDC != nullptr);
	return ::MoveToEx(m_hDC,nx, ny, lpPoint);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::MoveTo(POINT& point, LPPOINT lpPointRet = nullptr)
{
	assert(m_hDC != nullptr);
	return MoveTo(point.x, point.y, lpPointRet);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::LineTo(Int nx, Int ny)
{
	assert(m_hDC != nullptr);
	return ::LineTo(m_hDC, nx, ny);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::LineTo(POINT& point)
{
	assert(m_hDC != nullptr);
	return LineTo(point.x, point.y);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::Arc(Int nx1, Int ny1, Int nx2, Int ny2, Int nx3, Int ny3, Int nx4, Int ny4)
{
	assert(m_hDC != nullptr);
	return ::Arc(m_hDC, nx1, ny1, nx2, ny2, nx3, ny3, nx4, ny4);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::Arc(LPCRECT lpRect, POINT& ptStart, POINT& ptEnd)
{
	assert(m_hDC != nullptr);
	return ::Arc(m_hDC, lpRect->left, lpRect->top,
		lpRect->right, lpRect->bottom, ptStart.x, ptStart.y,
		ptEnd.x, ptEnd.y);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::Polyline(LPPOINT lpPoints, Int nCount)
{
	assert(m_hDC != nullptr);
	return ::Polyline(m_hDC, lpPoints, nCount);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::AngleArc(Int nx, Int ny, Int nRadius, Float fStartAngle, Float fSweepAngle)
{
	assert(m_hDC != nullptr);
	return ::AngleArc(m_hDC, nx, ny, nRadius, fStartAngle, fSweepAngle);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::ArcTo(Int nx1, Int ny1, Int nx2, Int ny2, Int nx3, Int ny3, Int nx4, Int ny4)
{
	assert(m_hDC != nullptr);
	return ::ArcTo(m_hDC, nx1, ny1, nx2, ny2, nx3, ny3, nx4, ny4);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::ArcTo(LPCRECT lpRect, POINT& ptStart, POINT& ptEnd)
{
	assert(m_hDC != nullptr);
	return ArcTo(lpRect->left, lpRect->top, lpRect->right,
	lpRect->bottom, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::GetArcDirection(void) const
{
	assert(m_hDC != nullptr);
	return ::GetArcDirection(m_hDC);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::SetArcDirection(Int nArcDirection)
{
	assert(m_hDC != nullptr);
	return ::SetArcDirection(m_hDC, nArcDirection);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::PolyDraw(const POINT* lpPoints, const PByte lpbTypes, Int nCount)
{
	assert(m_hDC != nullptr);
	return ::PolyDraw(m_hDC, lpPoints, lpbTypes, nCount);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::PolylineTo(const POINT* lpPoints, Int nCount)
{
	assert(m_hDC != nullptr);
	return ::PolylineTo(m_hDC, lpPoints, nCount);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::PolyPolyline(const POINT* lpPoints, const PULong lpulPolyPoints, Int nCount)
{
	assert(m_hDC != nullptr);
	return ::PolyPolyline(m_hDC, lpPoints, lpulPolyPoints, nCount);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::PolyBezier(const POINT* lpPoints, Int nCount)
{
	assert(m_hDC != nullptr);
	return ::PolyBezier(m_hDC, lpPoints, nCount);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::PolyBezierTo(const POINT* lpPoints, Int nCount)
{
	assert(m_hDC != nullptr);
	return ::PolyBezierTo(m_hDC, lpPoints, nCount);
}
// Simple Drawing Functions
template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::FillRect(LPCRECT lpRect, HBRUSH hBrush)
{
	assert(m_hDC != nullptr);
	return ::FillRect(m_hDC, lpRect, hBrush);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::FrameRect(LPCRECT lpRect, HBRUSH hBrush)
{
	assert(m_hDC != nullptr);
	return ::FrameRect(m_hDC, lpRect, hBrush);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::InvertRect(LPCRECT lpRect)
{
	assert(m_hDC != nullptr);
	return ::InvertRect(m_hDC, lpRect);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::DrawIcon(Int nx, Int ny, HICON hIcon)
{
	assert(m_hDC != nullptr);
	return ::DrawIcon(m_hDC, nx, ny, hIcon);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::DrawIcon(POINT& point, HICON hIcon)
{
	assert(m_hDC != nullptr);
	return ::DrawIcon(m_hDC, point.x, point.y, hIcon);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::DrawIconEx(Int nx, Int ny, HICON hIcon, Int nxWidth, Int nyWidth, UInt uStepIfAniCur, HBRUSH hbrFlickerFreeDraw, UInt uFlags)
{
	assert(m_hDC != nullptr);
	return ::DrawIconEx(m_hDC, nx, ny, hIcon, nxWidth, nyWidth, uStepIfAniCur, hbrFlickerFreeDraw, uFlags);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::DrawIconEx(POINT& point, HICON hIcon, SIZE& size, UInt uStepIfAniCur, HBRUSH hbrFlickerFreeDraw, UInt uFlags)
{
	assert(m_hDC != nullptr);
	return ::DrawIconEx(m_hDC, point.x, point.y, hIcon, size.cx, size.cy, uStepIfAniCur, hbrFlickerFreeDraw, uFlags);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::DrawState(POINT& pt, SIZE& size, HBITMAP hBitmap, UInt nFlags, HBRUSH hBrush)
{
	assert(m_hDC != nullptr);
	return ::DrawState(m_hDC, hBrush, nullptr, (LPARAM)hBitmap, 0, pt.x, pt.y, size.cx, size.cy, nFlags | DST_BITMAP);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::DrawState(POINT& pt, SIZE& size, HICON hIcon, UInt nFlags, HBRUSH hBrush)
{
	assert(m_hDC != nullptr);
	return ::DrawState(m_hDC, hBrush, nullptr, (LPARAM)hIcon, 0, pt.x, pt.y, size.cx, size.cy, nFlags | DST_ICON);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::DrawState(POINT& pt, SIZE& size, PCXStr pszText, UInt nFlags, BOOL bPrefixText, Int nTextLen, HBRUSH hBrush)
{
	assert(m_hDC != nullptr);
	return ::DrawState(m_hDC, hBrush, nullptr, (LPARAM)pszText, (WPARAM)nTextLen, pt.x, pt.y, size.cx, size.cy, nFlags | (bPrefixText ? DST_PREFIXTEXT : DST_TEXT));
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::DrawState(POINT& pt, SIZE& size, DRAWSTATEPROC lpDrawProc, LPARAM lData, UInt uFlags, HBRUSH hBrush)
{
	assert(m_hDC != nullptr);
	return ::DrawState(m_hDC, hBrush, lpDrawProc, lData, 0, pt.x, pt.y, size.cx, size.cy, uFlags | DST_COMPLEX);
}
// Ellipse and Polygon Functions
template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::Chord(Int nx1, Int ny1, Int nx2, Int ny2, Int nx3, Int ny3, Int nx4, Int ny4)
{
	assert(m_hDC != nullptr);
	return ::Chord(m_hDC, nx1, ny1, nx2, ny2, nx3, ny3, nx4, ny4);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::Chord(LPCRECT lpRect, POINT& ptStart, POINT& ptEnd)
{
	assert(m_hDC != nullptr);
	return ::Chord(m_hDC, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
}

template <bool tManaged>
INLINE void  CTXDC<tManaged>::DrawFocusRect(LPCRECT lpRect)
{
	assert(m_hDC != nullptr);
	::DrawFocusRect(m_hDC, lpRect);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::Ellipse(Int nx1, Int ny1, Int nx2, Int ny2)
{
	assert(m_hDC != nullptr);
	return ::Ellipse(m_hDC, nx1, ny1, nx2, ny2);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::Ellipse(LPCRECT lpRect)
{
	assert(m_hDC != nullptr);
	return ::Ellipse(m_hDC, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::Pie(Int nx1, Int ny1, Int nx2, Int ny2, Int nx3, Int ny3, Int nx4, Int ny4)
{
	assert(m_hDC != nullptr);
	return ::Pie(m_hDC, nx1, ny1, nx2, ny2, nx3, ny3, nx4, ny4);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::Pie(LPCRECT lpRect, POINT& ptStart, POINT& ptEnd)
{
	assert(m_hDC != nullptr);
	return ::Pie(m_hDC, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::Polygon(LPPOINT lpPoints, Int nCount)
{
	assert(m_hDC != nullptr);
	return ::Polygon(m_hDC, lpPoints, nCount);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::PolyPolygon(LPPOINT lpPoints, PInt lpnPolyCounts, Int nCount)
{
	assert(m_hDC != nullptr);
	return ::PolyPolygon(m_hDC, lpPoints, lpnPolyCounts, nCount);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::Rectangle(Int nx1, Int ny1, Int nx2, Int ny2)
{
	assert(m_hDC != nullptr);
	return ::Rectangle(m_hDC, nx1, ny1, nx2, ny2);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::Rectangle(LPCRECT lpRect)
{
	assert(m_hDC != nullptr);
	return ::Rectangle(m_hDC, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::RoundRect(Int nx1, Int ny1, Int nx2, Int ny2, Int nx3, Int ny3)
{
	assert(m_hDC != nullptr);
	return ::RoundRect(m_hDC, nx1, ny1, nx2, ny2, nx3, ny3);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::RoundRect(LPCRECT lpRect, POINT& point)
{
	assert(m_hDC != nullptr);
	return ::RoundRect(m_hDC, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom, point.x, point.y);
}
// Bitmap Functions
template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::PatBlt(Int nx, Int ny, Int nWidth, Int nHeight, ULong ulRop)
{
	assert(m_hDC != nullptr);
	return ::PatBlt(m_hDC, nx, ny, nWidth, nHeight, ulRop);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::BitBlt(Int nx, Int ny, Int nWidth, Int nHeight, HDC hSrcDC, Int nxSrc, Int nySrc, ULong ulRop)
{
	assert(m_hDC != nullptr);
	return ::BitBlt(m_hDC, nx, ny, nWidth, nHeight, hSrcDC, nxSrc, nySrc, ulRop);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::StretchBlt(Int nx, Int ny, Int nWidth, Int nHeight, HDC hSrcDC, Int nxSrc, Int nySrc, Int nSrcWidth, Int nSrcHeight, ULong ulRop)
{
	assert(m_hDC != nullptr);
	return ::StretchBlt(m_hDC, nx, ny, nWidth, nHeight, hSrcDC, nxSrc, nySrc, nSrcWidth, nSrcHeight, ulRop);
}

template <bool tManaged>
INLINE COLORREF CTXDC<tManaged>::GetPixel(Int nx, Int ny) const
{
	assert(m_hDC != nullptr);
	return ::GetPixel(m_hDC, nx, ny);
}

template <bool tManaged>
INLINE COLORREF CTXDC<tManaged>::GetPixel(POINT& point) const
{
	assert(m_hDC != nullptr);
	return ::GetPixel(m_hDC, point.x, point.y);
}

template <bool tManaged>
INLINE COLORREF CTXDC<tManaged>::SetPixel(Int nx, Int ny, COLORREF clrColor)
{
	assert(m_hDC != nullptr);
	return ::SetPixel(m_hDC, nx, ny, clrColor);
}

template <bool tManaged>
INLINE COLORREF CTXDC<tManaged>::SetPixel(POINT& point, COLORREF clrColor)
{
	assert(m_hDC != nullptr);
	return ::SetPixel(m_hDC, point.x, point.y, clrColor);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::FloodFill(Int nx, Int ny, COLORREF clrColor)
{
	assert(m_hDC != nullptr);
	return ::FloodFill(m_hDC, nx, ny, clrColor);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::ExtFloodFill(Int nx, Int ny, COLORREF clrColor, UInt uFillType)
{
	assert(m_hDC != nullptr);
	return ::ExtFloodFill(m_hDC, nx, ny, clrColor, uFillType);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::MaskBlt(Int nx, Int ny, Int nWidth, Int nHeight, HDC hSrcDC, Int nxSrc, Int nySrc, HBITMAP hMaskBitmap, Int nxMask, Int nyMask, ULong ulRop)
{
	assert(m_hDC != nullptr);
	return ::MaskBlt(m_hDC, nx, ny, nWidth, nHeight, hSrcDC, nxSrc, nySrc, hMaskBitmap, nxMask, nyMask, ulRop);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::PlgBlt(LPPOINT lpPoint, HDC hSrcDC, Int nxSrc, Int nySrc, Int nWidth, Int nHeight, HBITMAP hMaskBitmap, Int nxMask, Int nyMask)
{
	assert(m_hDC != nullptr);
	return ::PlgBlt(m_hDC, lpPoint, hSrcDC, nxSrc, nySrc, nWidth, nHeight, hMaskBitmap, nxMask, nyMask);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::SetPixelV(Int nx, Int ny, COLORREF clrColor)
{
	assert(m_hDC != nullptr);
	return ::SetPixelV(m_hDC, nx, ny, clrColor);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::SetPixelV(POINT& point, COLORREF clrColor)
{
	assert(m_hDC != nullptr);
	return ::SetPixelV(m_hDC, point.x, point.y, clrColor);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::TransparentBlt(Int nx, Int ny, Int nWidth, Int nHeight, HDC hSrcDC, Int nxSrc, Int nySrc, Int nSrcWidth, Int nSrcHeight, UInt uTransparent)
{
	assert(m_hDC != nullptr);
	return ::TransparentBlt(m_hDC, nx, ny, nWidth, nHeight, hSrcDC, nxSrc, nySrc, nSrcWidth, nSrcHeight, uTransparent);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::GradientFill(const PTRIVERTEX pVertices, ULong ulVertices, void* pMeshElements, ULong ulMeshElements, ULong ulMode)
{
	assert(m_hDC != nullptr);
	return ::GradientFill(m_hDC, pVertices, ulVertices, pMeshElements, ulMeshElements, ulMode);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::AlphaBlend(Int nx, Int ny, Int nWidth, Int nHeight, HDC hSrcDC, Int nxSrc, Int nySrc, Int nSrcWidth, Int nSrcHeight, BLENDFUNCTION bf)
{
	assert(m_hDC != nullptr);
	return ::AlphaBlend(m_hDC, nx, ny, nWidth, nHeight, hSrcDC, nxSrc, nySrc, nSrcWidth, nSrcHeight, bf);
}
// Text Functions
template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::TextOut(Int nx, Int ny, PCXStr pszString, Int nCount)
{
	assert(m_hDC != nullptr);
	if (nCount == -1)
	{
		nCount = (Int)CXChar::Length(pszString);
	}
	return ::TextOut(m_hDC, nx, ny, pszString, nCount);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::ExtTextOut(Int nx, Int ny, UInt uOptions, LPCRECT lpRect, PCXStr pszString, UInt uCount, PInt lpnDxWidths)
{
	assert(m_hDC != nullptr);
	if (uCount == (UInt)-1)
	{
		uCount = (UInt)CXChar::Length(pszString);
	}
	return ::ExtTextOut(m_hDC, nx, ny, uOptions, lpRect, pszString, uCount, lpnDxWidths);
}

template <bool tManaged>
INLINE SIZE CTXDC<tManaged>::TabbedTextOut(Int nx, Int ny, PCXStr pszString, Int nCount, Int nTabPositions, PInt lpnTabStopPositions, Int nTabOrigin)
{
	assert(m_hDC != nullptr);
	if (nCount == -1)
	{
		nCount = (Int)CXChar::Length(pszString);
	}
	Long lRet = ::TabbedTextOut(m_hDC, nx, ny, pszString, nCount, nTabPositions, lpnTabStopPositions, nTabOrigin);
	SIZE size = { GET_X_LPARAM(lRet), GET_Y_LPARAM(lRet) };
	return size;
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::DrawText(PCXStr pszText, Int nText, LPRECT lpRect, UInt uFormat)
{
	assert(m_hDC != nullptr);
	assert((uFormat & DT_MODIFYSTRING) == 0);
	return ::DrawText(m_hDC, pszText, nText, lpRect, uFormat);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::DrawText(PXStr pszText, Int nText, LPRECT lpRect, UInt uFormat)
{
	assert(m_hDC != nullptr);
	return ::DrawText(m_hDC, pszText, nText, lpRect, uFormat);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::DrawTextEx(PXStr pszText, Int nText, LPRECT lpRect, UInt uFormat, LPDRAWTEXTPARAMS lpDTParams)
{
	assert(m_hDC != nullptr);
	return ::DrawTextEx(m_hDC, pszText, nText, lpRect, uFormat, lpDTParams);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::GetTextExtent(PCXStr pszString, Int nCount, LPSIZE lpSize) const
{
	assert(m_hDC != nullptr);
	if (nCount == -1)
	{
		nCount = (Int)CXChar::Length(pszString);
	}
	return ::GetTextExtentPoint32(m_hDC, pszString, nCount, lpSize);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::GetTextExtentExPoint(PCXStr pszString, Int nCount, LPSIZE lpSize, Int nMaxExtent, PInt lpnFit, PInt lpnDx)
{
	assert(m_hDC != nullptr);
	return ::GetTextExtentExPoint(m_hDC, pszString, nCount, nMaxExtent, lpnFit, lpnDx, lpSize);
}

template <bool tManaged>
INLINE ULong CTXDC<tManaged>::GetTabbedTextExtent(PCXStr pszString, Int nCount, Int nTabPositions, PInt lpnTabStopPositions) const
{
	assert(m_hDC != nullptr);
	if (nCount == -1)
	{
		nCount = (Int)CXChar::Length(pszString);
	}
	return ::GetTabbedTextExtent(m_hDC, pszString, nCount, nTabPositions, lpnTabStopPositions);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::GrayString(HBRUSH hBrush, BOOL (CALLBACK* lpfnOutput)(HDC, LPARAM, Int), LPARAM lpData, Int nCount, Int nx, Int ny, Int nWidth, Int nHeight)
{
	assert(m_hDC != nullptr);
	return ::GrayString(m_hDC, hBrush, (GRAYSTRINGPROC)lpfnOutput, lpData, nCount, nx, ny, nWidth, nHeight);
}

template <bool tManaged>
INLINE UInt CTXDC<tManaged>::GetTextAlign(void) const
{
	assert(m_hDC != nullptr);
	return ::GetTextAlign(m_hDC);
}

template <bool tManaged>
INLINE UInt CTXDC<tManaged>::SetTextAlign(UInt uFlags)
{
	assert(m_hDC != nullptr);
	return ::SetTextAlign(m_hDC, uFlags);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::GetTextFace(PXStr pszFaceName, Int nCount) const
{
	assert(m_hDC != nullptr);
	return ::GetTextFace(m_hDC, nCount, pszFaceName);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::GetTextFaceLen(void) const
{
	assert(m_hDC != nullptr);
	return ::GetTextFace(m_hDC, 0, nullptr);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::GetTextMetrics(LPTEXTMETRIC lpMetrics) const
{
	assert(m_hDC != nullptr);
	return ::GetTextMetrics(m_hDC, lpMetrics);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::SetTextJustification(Int nBreakExtra, Int nBreakCount)
{
	assert(m_hDC != nullptr);
	return ::SetTextJustification(m_hDC, nBreakExtra, nBreakCount);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::GetTextCharacterExtra(void) const
{
	assert(m_hDC != nullptr);
	return ::GetTextCharacterExtra(m_hDC);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::SetTextCharacterExtra(Int nCharExtra)
{
	assert(m_hDC != nullptr);
	return ::SetTextCharacterExtra(m_hDC, nCharExtra);
}
// Advanced Drawing
template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::DrawEdge(LPRECT lpRect, UInt uEdge, UInt uFlags)
{
	assert(m_hDC != nullptr);
	return ::DrawEdge(m_hDC, lpRect, uEdge, uFlags);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::DrawFrameControl(LPRECT lpRect, UInt uType, UInt uState)
{
	assert(m_hDC != nullptr);
	return ::DrawFrameControl(m_hDC, lpRect, uType, uState);
}
// Scrolling Functions
template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::ScrollDC(Int ndx, Int ndy, LPCRECT lpRectScroll, LPCRECT lpRectClip, HRGN hRgnUpdate, LPRECT lpRectUpdate)
{
	assert(m_hDC != nullptr);
	return ::ScrollDC(m_hDC, ndx, ndy, lpRectScroll, lpRectClip, hRgnUpdate, lpRectUpdate);
}
// Font Functions
template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::GetCharWidth(UInt uFirstChar, UInt uLastChar, PInt lpnBuffer) const
{
	assert(m_hDC != nullptr);
	return ::GetCharWidth(m_hDC, uFirstChar, uLastChar, lpnBuffer);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::GetCharWidth32(UInt uFirstChar, UInt uLastChar, PInt lpnBuffer) const
{
	assert(m_hDC != nullptr);
	return ::GetCharWidth32(m_hDC, uFirstChar, uLastChar, lpnBuffer);
}

template <bool tManaged>
INLINE ULong CTXDC<tManaged>::SetMapperFlags(ULong ulFlag)
{
	assert(m_hDC != nullptr);
	return ::SetMapperFlags(m_hDC, ulFlag);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::GetAspectRatioFilter(LPSIZE lpSize) const
{
	assert(m_hDC != nullptr);
	return ::GetAspectRatioFilterEx(m_hDC, lpSize);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::GetCharABCWidths(UInt uFirstChar, UInt uLastChar, LPABC lpabc) const
{
	assert(m_hDC != nullptr);
	return ::GetCharABCWidths(m_hDC, uFirstChar, uLastChar, lpabc);
}

template <bool tManaged>
INLINE ULong CTXDC<tManaged>::GetFontData(ULong ulTable, ULong ulOffset, void* lpData, ULong ulData) const
{
	assert(m_hDC != nullptr);
	return ::GetFontData(m_hDC, ulTable, ulOffset, lpData, ulData);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::GetKerningPairs(Int nPairs, LPKERNINGPAIR lpkrnpair) const
{
	assert(m_hDC != nullptr);
	return ::GetKerningPairs(m_hDC, nPairs, lpkrnpair);
}

template <bool tManaged>
INLINE UInt CTXDC<tManaged>::GetOutlineTextMetrics(UInt uData, LPOUTLINETEXTMETRIC lpotm) const
{
	assert(m_hDC != nullptr);
	return ::GetOutlineTextMetrics(m_hDC, uData, lpotm);
}

template <bool tManaged>
INLINE ULong CTXDC<tManaged>::GetGlyphOutline(UInt uChar, UInt uFormat, LPGLYPHMETRICS lpgm, ULong ulBuffer, void* lpBuffer, const MAT2* lpmat2) const
{
	assert(m_hDC != nullptr);
	return ::GetGlyphOutline(m_hDC, uChar, uFormat, lpgm, ulBuffer, lpBuffer, lpmat2);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::GetCharABCWidths(UInt uFirstChar, UInt uLastChar, LPABCFLOAT lpABCF) const
{
	assert(m_hDC != nullptr);
	return ::GetCharABCWidthsFloat(m_hDC, uFirstChar, uLastChar, lpABCF);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::GetCharWidth(UInt uFirstChar, UInt uLastChar, PFloat lpfBuffer) const
{
	assert(m_hDC != nullptr);
	return ::GetCharWidthFloat(m_hDC, uFirstChar, uLastChar, lpfBuffer);
}
// Printer/Device Escape Functions
template <bool tManaged>
INLINE Int CTXDC<tManaged>::Escape(Int nEscape, Int nCount, PCStr pszInData, void* lpOutData)
{
	assert(m_hDC != nullptr);
	return ::Escape(m_hDC, nEscape, nCount, pszInData, lpOutData);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::Escape(Int nEscape, Int nInputSize, PCStr pszInputData, Int nOutputSize, PStr pszOutputData)
{
	assert(m_hDC != nullptr);
	return ::ExtEscape(m_hDC, nEscape, nInputSize, pszInputData, nOutputSize, pszOutputData);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::DrawEscape(Int nEscape, Int nInputSize, PCStr pszInputData)
{
	assert(m_hDC != nullptr);
	return ::DrawEscape(m_hDC, nEscape, nInputSize, pszInputData);
}
// Escape helpers
template <bool tManaged>
INLINE Int CTXDC<tManaged>::StartDoc(PCXStr pszDocName)  // old Win3.0 version
{
	DOCINFO di = { 0 };
	di.cbSize      = sizeof(DOCINFO);
	di.lpszDocName = pszDocName;
	return StartDoc(&di);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::StartDoc(LPDOCINFO lpDocInfo)
{
	assert(m_hDC != nullptr);
	return ::StartDoc(m_hDC, lpDocInfo);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::StartPage(void)
{
	assert(m_hDC != nullptr);
	return ::StartPage(m_hDC);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::EndPage(void)
{
	assert(m_hDC != nullptr);
	return ::EndPage(m_hDC);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::SetAbortProc(BOOL (CALLBACK* lpfn)(HDC, Int))
{
	assert(m_hDC != nullptr);
	return ::SetAbortProc(m_hDC, (ABORTPROC)lpfn);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::AbortDoc(void)
{
	assert(m_hDC != nullptr);
	return ::AbortDoc(m_hDC);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::EndDoc(void)
{
	assert(m_hDC != nullptr);
	return ::EndDoc(m_hDC);
}
// MetaFile Functions
template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::PlayMetaFile(HMETAFILE hMF)
{
	assert(m_hDC != nullptr);
	if (::GetDeviceCaps(m_hDC, TECHNOLOGY) == DT_METAFILE)
	{
		// playing metafile in metafile, just use core windows API
		return ::PlayMetaFile(m_hDC, hMF);
	}

	// for special playback, lParam == pDC
	return ::EnumMetaFile(m_hDC, hMF, EnumMetaFileProc, (LPARAM)this);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::PlayMetaFile(HENHMETAFILE hEnhMetaFile, LPCRECT lpBounds)
{
	assert(m_hDC != nullptr);
	return ::PlayEnhMetaFile(m_hDC, hEnhMetaFile, lpBounds);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::AddMetaFileComment(UInt uDataSize, const PByte pbCommentData) // can be used for enhanced metafiles only
{
	assert(m_hDC != nullptr);
	return ::GdiComment(m_hDC, uDataSize, pbCommentData);
}
// Path Functions
template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::AbortPath(void)
{
	assert(m_hDC != nullptr);
	return ::AbortPath(m_hDC);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::BeginPath(void)
{
	assert(m_hDC != nullptr);
	return ::BeginPath(m_hDC);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::CloseFigure(void)
{
	assert(m_hDC != nullptr);
	return ::CloseFigure(m_hDC);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::EndPath(void)
{
	assert(m_hDC != nullptr);
	return ::EndPath(m_hDC);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::FillPath(void)
{
	assert(m_hDC != nullptr);
	return ::FillPath(m_hDC);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::FlattenPath(void)
{
	assert(m_hDC != nullptr);
	return ::FlattenPath(m_hDC);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::StrokeAndFillPath(void)
{
	assert(m_hDC != nullptr);
	return ::StrokeAndFillPath(m_hDC);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::StrokePath(void)
{
	assert(m_hDC != nullptr);
	return ::StrokePath(m_hDC);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::WidenPath(void)
{
	assert(m_hDC != nullptr);
	return ::WidenPath(m_hDC);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::GetMiterLimit(PFloat pfMiterLimit) const
{
	assert(m_hDC != nullptr);
	return ::GetMiterLimit(m_hDC, pfMiterLimit);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::SetMiterLimit(Float fMiterLimit)
{
	assert(m_hDC != nullptr);
	return ::SetMiterLimit(m_hDC, fMiterLimit, nullptr);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::GetPath(LPPOINT lpPoints, PByte lpbTypes, Int nCount) const
{
	assert(m_hDC != nullptr);
	return ::GetPath(m_hDC, lpPoints, lpbTypes, nCount);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::SelectClipPath(Int nMode)
{
	assert(m_hDC != nullptr);
	return ::SelectClipPath(m_hDC, nMode);
}

template <bool tManaged>
INLINE void  CTXDC<tManaged>::FillSolidRect(LPCRECT lpRect, COLORREF clr)
{
	assert(m_hDC != nullptr);

	COLORREF clrOld = ::SetBkColor(m_hDC, clr);
	assert(clrOld != CLR_INVALID);
	if (clrOld != CLR_INVALID)
	{
		::ExtTextOut(m_hDC, 0, 0, ETO_OPAQUE, lpRect, nullptr, 0, nullptr);
		::SetBkColor(m_hDC, clrOld);
	}
}

template <bool tManaged>
INLINE void  CTXDC<tManaged>::FillSolidRect(Int nx, Int ny, Int ncx, Int ncy, COLORREF clr)
{
	assert(m_hDC != nullptr);

	RECT rect = { nx, ny, nx + ncx, ny + ncy };
	FillSolidRect(&rect, clr);
}

template <bool tManaged>
INLINE void  CTXDC<tManaged>::Draw3dRect(LPCRECT lpRect, COLORREF clrTopLeft, COLORREF clrBottomRight)
{
	Draw3dRect(lpRect->left, lpRect->top, lpRect->right - lpRect->left,
		       lpRect->bottom - lpRect->top, clrTopLeft, clrBottomRight);
}

template <bool tManaged>
INLINE void  CTXDC<tManaged>::Draw3dRect(Int nx, Int ny, Int ncx, Int ncy, COLORREF clrTopLeft, COLORREF clrBottomRight)
{
	FillSolidRect(nx,       ny,       ncx - 1,  1,       clrTopLeft);
	FillSolidRect(nx,       ny,       1,        ncy - 1, clrTopLeft);
	FillSolidRect(nx + ncx, ny,       -1,       ncy,     clrBottomRight);
	FillSolidRect(nx,       ny + ncy, ncx,      -1,      clrBottomRight);
}
// DIB support
template <bool tManaged>
INLINE Int CTXDC<tManaged>::SetDIBitsToDevice(Int nx, Int ny, ULong ulWidth, ULong ulHeight, Int nxSrc, Int nySrc, UInt uStartScan, UInt uScanLines, const void* lpvBits, const BITMAPINFO* lpbmi, UInt uColorUse)
{
	assert(m_hDC != nullptr);
	return ::SetDIBitsToDevice(m_hDC, nx, ny, ulWidth, ulHeight, nxSrc, nySrc, uStartScan, uScanLines, lpvBits, lpbmi, uColorUse);
}

template <bool tManaged>
INLINE Int CTXDC<tManaged>::StretchDIBits(Int nx, Int ny, Int nWidth, Int nHeight, Int nxSrc, Int nySrc, Int nSrcWidth, Int nSrcHeight, const void* lpvBits, const BITMAPINFO* lpbmi, UInt uColorUse, ULong ulRop)
{
	assert(m_hDC != nullptr);
	return ::StretchDIBits(m_hDC, nx, ny, nWidth, nHeight, nxSrc, nySrc, nSrcWidth, nSrcHeight, lpvBits, lpbmi, uColorUse, ulRop);
}

template <bool tManaged>
INLINE UInt CTXDC<tManaged>::GetDIBColorTable(UInt uStartIndex, UInt uEntries, RGBQUAD* pColors) const
{
	assert(m_hDC != nullptr);
	return ::GetDIBColorTable(m_hDC, uStartIndex, uEntries, pColors);
}

template <bool tManaged>
INLINE UInt CTXDC<tManaged>::SetDIBColorTable(UInt uStartIndex, UInt uEntries, const RGBQUAD* pColors)
{
	assert(m_hDC != nullptr);
	return ::SetDIBColorTable(m_hDC, uStartIndex, uEntries, pColors);
}

template <bool tManaged>
INLINE COLORREF CTXDC<tManaged>::GetDCPenColor(void) const
{
	assert(m_hDC != nullptr);
	return ::GetDCPenColor(m_hDC);
}

template <bool tManaged>
INLINE COLORREF CTXDC<tManaged>::SetDCPenColor(COLORREF clr)
{
	assert(m_hDC != nullptr);
	return ::SetDCPenColor(m_hDC, clr);
}

template <bool tManaged>
INLINE COLORREF CTXDC<tManaged>::GetDCBrushColor(void) const
{
	assert(m_hDC != nullptr);
	return ::GetDCBrushColor(m_hDC);
}

template <bool tManaged>
INLINE COLORREF CTXDC<tManaged>::SetDCBrushColor(COLORREF clr)
{
	assert(m_hDC != nullptr);
	return ::SetDCBrushColor(m_hDC, clr);
}

template <bool tManaged>
INLINE ULong CTXDC<tManaged>::GetFontUnicodeRanges(LPGLYPHSET lpgs) const
{
	assert(m_hDC != nullptr);
	return ::GetFontUnicodeRanges(m_hDC, lpgs);
}

template <bool tManaged>
INLINE ULong CTXDC<tManaged>::GetGlyphIndices(PCXStr pszStr, Int nCount, PUShort pgi, ULong ulFlags) const
{
	assert(m_hDC != nullptr);
	return ::GetGlyphIndices(m_hDC, pszStr, nCount, pgi, ulFlags);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::GetTextExtentPointI(PUShort pgiIn, Int ncgi, LPSIZE lpSize) const
{
	assert(m_hDC != nullptr);
	return ::GetTextExtentPointI(m_hDC, pgiIn, ncgi, lpSize);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::GetTextExtentExPointI(PUShort pgiIn, Int ncgi, Int nMaxExtent, PInt lpnFit, PInt lpnDx, LPSIZE lpSize) const
{
	assert(m_hDC != nullptr);
	return ::GetTextExtentExPointI(m_hDC, pgiIn, ncgi, nMaxExtent, lpnFit, lpnDx, lpSize);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::GetCharWidthI(UInt giFirst, UInt cgi, PUShort pgi, PInt lpBuffer) const
{
	assert(m_hDC != nullptr);
	return ::GetCharWidthI(m_hDC, giFirst, cgi, pgi, lpBuffer);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::GetCharABCWidthsI(UInt ugiFirst, UInt ucgi, PUShort pgi, LPABC lpabc) const
{
	assert(m_hDC != nullptr);
	return ::GetCharABCWidthsI(m_hDC, ugiFirst, ucgi, pgi, lpabc);
}

template <bool tManaged>
INLINE BOOL CTXDC<tManaged>::ColorCorrectPalette(HPALETTE hPalette, ULong ulFirstEntry, ULong ulNumOfEntries)
{
	assert(m_hDC != nullptr);
	return ::ColorCorrectPalette(m_hDC, hPalette, ulFirstEntry, ulNumOfEntries);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXPaintDC
INLINE CXPaintDC::CXPaintDC(HWND hWnd)
: m_hWnd(hWnd)
{
	assert(::IsWindow(hWnd));
	m_hDC = ::BeginPaint(hWnd, &m_ps);
}

INLINE CXPaintDC::~CXPaintDC(void)
{
	assert(m_hDC != nullptr);
	assert(::IsWindow(m_hWnd));
	::EndPaint(m_hWnd, &m_ps);
	Detach();
}

INLINE CXPaintDC::CXPaintDC(const CXPaintDC&)
: m_hWnd(nullptr)
{
}

INLINE CXPaintDC&  CXPaintDC::operator=(const CXPaintDC&)
{
	return (*this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXClientDC
INLINE CXClientDC::CXClientDC(HWND hWnd)
: m_hWnd(hWnd)
{
	assert(::IsWindow(hWnd));
	m_hDC = ::GetDC(hWnd);
}

INLINE CXClientDC::~CXClientDC(void)
{
	assert(m_hDC != nullptr);
	assert(::IsWindow(m_hWnd));
	::ReleaseDC(m_hWnd, Detach());
}

INLINE CXClientDC::CXClientDC(const CXClientDC&)
: m_hWnd(nullptr)
{
}

INLINE CXClientDC&  CXClientDC::operator=(const CXClientDC&)
{
	return (*this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXWindowDC
INLINE CXWindowDC::CXWindowDC(HWND hWnd)
: m_hWnd(hWnd)
{
	assert(::IsWindow(hWnd));
	m_hDC = ::GetWindowDC(hWnd);
}

INLINE CXWindowDC::~CXWindowDC(void)
{
	assert(m_hDC != nullptr);
	assert(::IsWindow(m_hWnd));
	::ReleaseDC(m_hWnd, Detach());
}

INLINE CXWindowDC::CXWindowDC(const CXWindowDC&)
: m_hWnd(nullptr)
{
}

INLINE CXWindowDC&  CXWindowDC::operator=(const CXWindowDC&)
{
	return (*this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXMemDC
INLINE CXMemDC::CXMemDC(HDC hDC, PRECT pRect)
: m_hDstDC(hDC)
, m_hOld(nullptr)
{
	CreateCompatibleDC(hDC);
	assert(m_hDC != nullptr);
	if (pRect != nullptr)
	{
		m_rtPaint = *pRect;
		m_hBmp.CreateCompatibleBitmap(hDC, m_rtPaint.Width(), m_rtPaint.Height());
		assert(m_hBmp.m_hBitmap != nullptr);
		m_hOld = SelectBitmap(m_hBmp);
	}
}

INLINE CXMemDC::CXMemDC(HDC hDC, Long lWidth, Long lHeight)
: m_hDstDC(hDC)
, m_hOld(nullptr)
{
	CreateCompatibleDC(hDC);
	assert(m_hDC != nullptr);
	if ((lWidth > 0) && (lHeight > 0))
	{
		m_rtPaint.m_tLeft   = 0;
		m_rtPaint.m_tTop    = 0;
		m_rtPaint.m_tRight  = lWidth;
		m_rtPaint.m_tBottom = lHeight;

		m_hBmp.CreateCompatibleBitmap(hDC, lWidth, lHeight);
		assert(m_hBmp.m_hBitmap != nullptr);
		m_hOld = SelectBitmap(m_hBmp);
	}
}

INLINE CXMemDC::~CXMemDC(void)
{
	if (m_hDstDC != nullptr)
	{
		if (m_hBmp.IsValid())
		{
			::BitBlt(m_hDstDC, m_rtPaint.m_tLeft, m_rtPaint.m_tTop, m_rtPaint.Width(), m_rtPaint.Height(), m_hDC, 0, 0, SRCCOPY);
			SelectBitmap(m_hOld);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXUIClip
INLINE CXUIClip::CXUIClip(void)
{
}

INLINE CXUIClip::~CXUIClip(void)
{
	m_RefDC.SelectClipRgn(m_OldRgn);
}

INLINE void CXUIClip::GenerateClip(CXDC& dc, CXRect& rt)
{
	RECT rtClip = { 0 };
	dc.GetClipBox(&rtClip);
	m_OldRgn.CreateRectRgnIndirect(&rtClip);
	m_CurRgn.CreateRectRgnIndirect(rt);
	dc.SelectClipRgn(m_CurRgn, RGN_AND);
}

INLINE void CXUIClip::GenerateRoundClip(CXDC& dc, CXRect& rtValid, CXRect& rcControl, Int nWidth, Int nHeight)
{
	RECT rtClip = { 0 };
	dc.GetClipBox(&rtClip);
	m_OldRgn.CreateRectRgnIndirect(&rtClip);
	m_CurRgn.CreateRectRgnIndirect(rtValid);

	CXRgn ControlRgn;
	ControlRgn.CreateRoundRectRgn(rcControl.m_tLeft, rcControl.m_tTop, rcControl.m_tRight + 1, rcControl.m_tBottom + 1, nWidth, nHeight);

	m_CurRgn.CombineRgn(ControlRgn, RGN_AND);
	dc.SelectClipRgn(m_CurRgn, RGN_AND);
}

INLINE void CXUIClip::UseOldClipBegin(CXDC& dc)
{
	dc.SelectClipRgn(m_OldRgn);
}

INLINE void CXUIClip::UseOldClipEnd(CXDC& dc)
{
	dc.SelectClipRgn(m_CurRgn);
}

#endif // (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#endif // __TARGET_XUI_GDI_INL__