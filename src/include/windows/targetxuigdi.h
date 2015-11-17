///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : targetxuigdi.h                                     ¡¡¡¡                               //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2005-02-02     version 0.0.0.1                                                             //
//   Update : 2015-03-16     version 0.0.5.5                                                             //
//   Detail : xui windows gdi                                                                            //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TARGET_XUI_GDI_H__
#define __TARGET_XUI_GDI_H__

#pragma once

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#include "targetxuibase.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTXSize
typedef struct tagSTR_ID
{
public:
	tagSTR_ID(PCXStr pszStr)
	: m_pszStr(pszStr)
	{
	}

	tagSTR_ID(UInt uID)
	: m_pszStr(MAKEINTRESOURCE(uID))
	{
	}

	~tagSTR_ID(void)
	{
	}
public:
	PCXStr   m_pszStr;
}STR_ID, *PSTR_ID;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTXSize
template <typename T>
class CTXSize : public MObject
{
public:
	CTXSize(T InX = 0, T InY = 0);
	~CTXSize(void);

	CTXSize(const CTXSize& aSrc);
	CTXSize& operator=(const CTXSize& aSrc);

	CTXSize& operator+=(const CTXSize& aSrc);
	CTXSize& operator-=(const CTXSize& aSrc);

	CTXSize  operator+(const CTXSize& aSrc);
	CTXSize  operator-(const CTXSize& aSrc);
	CTXSize  operator-(void);

	bool     operator==(const CTXSize& aSrc) const;
	bool     operator!=(const CTXSize& aSrc) const;

	void     SetValue(T InX = 0, T InY = 0);

public:
	T	m_tX;
	T	m_tY;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXSize
class CXSize : public CTXSize<Long>
{
public:
	CXSize(Long InX = 0, Long InY = 0);

	CXSize(const SIZE& aSrc);
	CXSize(const POINT& aSrc);

	CXSize& operator=(const SIZE& aSrc);
	CXSize& operator=(const POINT& aSrc);

	CXSize& operator+=(const SIZE& aSrc);
	CXSize& operator+=(const POINT& aSrc);

	CXSize& operator-=(const SIZE& aSrc);
	CXSize& operator-=(const POINT& aSrc);

	CXSize  operator+(const SIZE& aSrc);
	CXSize  operator+(const POINT& aSrc);

	CXSize  operator-(const SIZE& aSrc);
	CXSize  operator-(const POINT& aSrc);

	bool   operator==(const SIZE& aSrc) const;
	bool   operator!=(const SIZE& aSrc) const;

	bool   operator==(const POINT& aSrc) const;
	bool   operator!=(const POINT& aSrc) const;

	operator SIZE*() const;
	operator POINT*() const;

	operator SIZE&() const;
	operator POINT&() const;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXPoint
typedef CXSize CXPoint; 

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTXRect
template <typename T>
class CTXRect : public MObject
{
public:
	CTXRect(T InLeft = 0, T InTop = 0, T InRight = 0, T InBottom = 0);
	~CTXRect(void);

	CTXRect(const CTXRect& aSrc);
	CTXRect& operator=(const CTXRect& aSrc);

	CTXRect& operator+=(const CTXRect& aSrc);
	CTXRect& operator-=(const CTXRect& aSrc);
	CTXRect& operator+=(const CTXSize<T>& tSize);
	CTXRect& operator-=(const CTXSize<T>& tSize);

	CTXRect  operator+(const CTXRect& aSrc);
	CTXRect  operator-(const CTXRect& aSrc);
	CTXRect  operator+(const CTXSize<T>& tSize);
	CTXRect  operator-(const CTXSize<T>& tSize);
	CTXRect  operator-(void);

	bool    operator==(const CTXRect& aSrc) const;
	bool    operator!=(const CTXRect& aSrc) const;

	T       Width(void);
	T       Height(void);

	CTXSize<T> Size(void);
	CTXSize<T> LeftTop(void);
	CTXSize<T> RightBottom(void);
	CTXSize<T> Center(void);

	bool    IsEmpty(void) const; // has no area
	bool    IsNull(void) const;  // at(0,0) and has no area
	bool    IsEqual(const CTXRect& rect) const;
	bool    PtInRect(const CTXSize<T>& point) const;

	void    SetValue(T InLeft = 0, T InTop = 0, T InRight = 0, T InBottom = 0);
	void    InflateRect(T x, T y);
	void    OffseteRect(T x, T y);
	void    Normalize(void);

public:
	T	m_tLeft;
	T	m_tTop;
	T	m_tRight;
	T	m_tBottom;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXRect
class CXRect : public CTXRect<Long>
{
public:
	CXRect(Long InLeft = 0, Long InTop = 0, Long InRight = 0, Long InBottom = 0);

	CXRect(const RECT& aSrc);
	CXRect& operator=(const RECT& aSrc);

	CXRect& operator+=(const RECT& aSrc);
	CXRect& operator-=(const RECT& aSrc);

	CXRect  operator+(const RECT& aSrc);
	CXRect  operator-(const RECT& aSrc);

	bool    operator==(const RECT& aSrc) const;
	bool    operator!=(const RECT& aSrc) const;

	bool    PtInRect(POINT& pt) const;
	operator RECT*() const;
	operator RECT&() const;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXGDITraits
class CXGDITraits
{
public:
	enum XGDI_CONST
	{
		XGDIC_DECIPOINTS_POINT = 10,   // 10 decipoints/point
		XGDIC_POINTS_INCH      = 72,   // 72 points/inch
		XGDIC_HIMETRIC_INCH    = 2540, // HIMETRIC = 1/100th of a millimeter, 25.4mm per inch
	};
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTXPen
template <bool tManaged>
class CTXPen : public MObject
{
public:
	CTXPen(HPEN hPen = nullptr);
	~CTXPen(void);

	CTXPen&  operator=(HPEN hPen);
	operator HPEN(void) const;
	bool     IsValid(void) const;

	void Attach(HPEN hPen);
	HPEN Detach(void);

	HPEN CreatePen(Int nPenStyle, Int nWidth, COLORREF clrColor);
	HPEN CreatePen(Int nPenStyle, Int nWidth, const LOGBRUSH& LogBrushRef, Int nStyleCount = 0, PULong pulStyle = nullptr);
	HPEN CreatePenIndirect(LOGPEN& LogPenRef);
	BOOL DeleteObject(void);

	Int  GetLogPen(LOGPEN* pLogPen) const;
	bool GetLogPen(LOGPEN& LogPenRef) const;
	Int  GetExtLogPen(EXTLOGPEN* pLogPen) const;
	bool GetExtLogPen(EXTLOGPEN& ExtLogPenRef) const;
private:
	CTXPen(const CTXPen&);
	CTXPen& operator=(const CTXPen&);
public:
	HPEN   m_hPen;
};
typedef CTXPen<false>   CXPenRef; // when inst is out of used, inst destructor do not destroy m_hPen
typedef CTXPen<true>    CXPen;    // when inst is out of used, inst destructor will destroy m_hPen

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTXBrush
template <bool tManaged>
class CTXBrush : public MObject
{
public:
	CTXBrush(HBRUSH hBrush = nullptr);
	~CTXBrush(void);

	CTXBrush& operator=(HBRUSH hBrush);
	operator  HBRUSH(void) const;
	bool      IsValid(void) const;

	void   Attach(HBRUSH hBrush);
	HBRUSH Detach(void);

	HBRUSH CreateSolidBrush(COLORREF clrColor);
	HBRUSH CreateHatchBrush(Int nIndex, COLORREF clrColor);
	HBRUSH CreateBrushIndirect(const LOGBRUSH& LogBrushRef);
	HBRUSH CreatePatternBrush(HBITMAP hBitmap);
	HBRUSH CreateDIBPatternBrush(HGLOBAL hPackedDIB, UInt uUsage);
	HBRUSH CreateDIBPatternBrush(const void* pPackedDIB, UInt uUsage);
	HBRUSH CreateSysColorBrush(Int nIndex);
	BOOL   DeleteObject(void);

	Int    GetLogBrush(LOGBRUSH* pLogBrush) const;
	bool   GetLogBrush(LOGBRUSH& LogBrushRef) const;
private:
	CTXBrush(const CTXBrush&);
	CTXBrush& operator=(const CTXBrush&);
public:
	HBRUSH   m_hBrush;
};
typedef CTXBrush<false>   CXBrushRef;
typedef CTXBrush<true>    CXBrush;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXLogFont
class CXLogFont : public MObject, public LOGFONT
{
public:
	CXLogFont(HFONT hFont = nullptr);
	~CXLogFont(void);

	CXLogFont(const CXLogFont&);
	CXLogFont& operator=(const CXLogFont&);

	CXLogFont(const LOGFONT& LogFontRef);
	CXLogFont& operator =(const LOGFONT& LogFontRef);
	CXLogFont& operator =(HFONT hFont);

	void  Copy(const LOGFONT& LogFontRef);
	bool  operator ==(const LOGFONT& LogFontRef) const;

	HFONT CreateFontIndirect(void);

	void  SetBold(void);
	bool  IsBold(void) const;
	void  MakeBolder(Int nScale = 1);
	void  MakeLarger(Int nScale);
	void  SetHeight(Long lPointSize, HDC hDC = nullptr);
	Long  GetHeight(HDC hDC = nullptr) const;
	Long  GetDeciPointHeight(HDC hDC = nullptr) const;
	void  SetHeightFromDeciPoint(Long lDeciPtHeight, HDC hDC = nullptr);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTXFont
template <bool tManaged>
class CTXFont : public MObject
{
public:
	CTXFont(HFONT hFont = nullptr);
	~CTXFont(void);

	CTXFont& operator=(HFONT hFont);
	operator HFONT(void) const;
	bool     IsValid(void) const;

	void  Attach(HFONT hFont);
	HFONT Detach(void);

	HFONT CreateFontIndirect(const LOGFONT& LogFontRef);
	HFONT CreateFontIndirectEx(const ENUMLOGFONTEXDV& ExdvRef);
	HFONT CreateFont(Int nHeight, Int nWidth, Int nEscapement, Int nOrientation, Int nWeight, 
	                 Byte bItalic, Byte bUnderline, Byte bStrikeOut, Byte bCharSet, Byte bOutPrecision,
	                 Byte bClipPrecision, Byte bQuality, Byte bPitchAndFamily, PCXStr pszFaceName);

	HFONT CreatePointFont(Long lPointSize, PCXStr pszFaceName, HDC hDC = nullptr, bool bBold = false, bool bItalic = false);
	HFONT CreatePointFontIndirect(const LOGFONT& LogFontRef, HDC hDC = nullptr);
	BOOL  DeleteObject(void);

	Int   GetLogFont(LOGFONT* pLogFont) const;
	bool  GetLogFont(LOGFONT& LogFontRef) const;
private:
	CTXFont(const CTXFont&);
	CTXFont& operator=(const CTXFont&);
public:
	HFONT   m_hFont;
};
typedef CTXFont<false>   CXFontRef;
typedef CTXFont<true>    CXFont;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTXImageList
template <bool tManaged>
class CTXImageList : public MObject
{
public:
	CTXImageList(HIMAGELIST hImg = nullptr);
	~CTXImageList(void);

	CTXImageList& operator=(HIMAGELIST hImg);
	operator      HIMAGELIST(void) const;
	bool          IsValid(void) const;

	void       Attach(HIMAGELIST hImg);
	HIMAGELIST Detach(void);

	Int        GetImageCount(void) const;
	COLORREF   GetBkColor(void) const;
	COLORREF   SetBkColor(COLORREF clr);

	BOOL       GetImageInfo(Int nImage, IMAGEINFO& ImageInfoRef) const;
	HICON      GetIcon(Int nIndex, UInt uFlags = ILD_NORMAL) const;
	BOOL       GetIconSize(Int& nx, Int& ny) const;
	BOOL       GetIconSize(SIZE& size) const;
	BOOL       SetIconSize(Int nx, Int ny);
	BOOL       SetIconSize(const SIZE& size);
	BOOL       SetImageCount(UInt uNewCount);
	BOOL       SetOverlayImage(Int nImage, Int nOverlay);

	BOOL       Create(Int nx, Int ny, UInt uFlags, Int nInitial, Int nGrow);
	BOOL       Create(STR_ID Bmp, Int nx, Int nGrow, COLORREF clrMask, HMODULE hResInst = nullptr);
	BOOL       CreateFromImage(STR_ID Img, Int nx, Int nGrow, COLORREF clrMask, UInt uType, UInt uFlags = LR_DEFAULTCOLOR | LR_DEFAULTSIZE, HMODULE hResInst = nullptr);
	BOOL       Merge(HIMAGELIST hImageList1, Int nImage1, HIMAGELIST hImageList2, Int nImage2, Int ndx, Int ndy);
	BOOL       Destroy(void);
	Int        Add(HBITMAP hBitmap, HBITMAP hBitmapMask = nullptr);
	Int        Add(HBITMAP hBitmap, COLORREF clrMask);
	BOOL       Remove(Int nImage);
	BOOL       RemoveAll(void);
	BOOL       Replace(Int nImage, HBITMAP hBitmap, HBITMAP hBitmapMask);
	Int        AddIcon(HICON hIcon);
	Int        ReplaceIcon(Int nImage, HICON hIcon);
	HICON      ExtractIcon(Int nImage);
	BOOL       Draw(HDC hDC, Int nImage, Int nx, Int ny, UInt uStyle);
	BOOL       Draw(HDC hDC, Int nImage, POINT& pt, UInt uStyle);
	BOOL       DrawEx(Int nImage, HDC hDC, Int nx, Int ny, Int ndx, Int ndy, COLORREF clrBack, COLORREF clrFore, UInt uStyle);
	BOOL       DrawEx(Int nImage, HDC hDC, RECT& rect, COLORREF clrBack, COLORREF clrFore, UInt uStyle);
	BOOL       Copy(Int nSrc, Int nDst, UInt uFlags = ILCF_MOVE);
	// Drag operations
	BOOL       BeginDrag(Int nImage, POINT& ptHotSpot);
	BOOL       BeginDrag(Int nImage, Int nxHotSpot, Int nyHotSpot);
	BOOL       SetDragCursorImage(Int nDrag, POINT& ptHotSpot);
	BOOL       SetDragCursorImage(Int nDrag, Int nxHotSpot, Int nyHotSpot);

	CTXImageList Duplicate(void) const;
public:
	static CTXImageList Duplicate(HIMAGELIST hImg);	
	static BOOL DrawIndirect(IMAGELISTDRAWPARAMS* pimldp);
	static void EndDrag(void);
	static BOOL DragMove(POINT& pt);
	static BOOL DragMove(Int nx, Int ny);
	static BOOL DragShowNolock(BOOL bShow = TRUE);
	static CTXImageList GetDragImage(POINT& pt, POINT& ptHotSpot);
	static BOOL DragEnter(HWND hWnd, POINT& point);
	static BOOL DragEnter(HWND hWnd, Int nx, Int ny);
	static BOOL DragLeave(HWND hWnd);
private:
	CTXImageList(const CTXImageList&);
	CTXImageList& operator=(const CTXImageList&);
public:
	HIMAGELIST m_hImg;
};
typedef CTXImageList<false>   CXImageListRef;
typedef CTXImageList<true>    CXImageList;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTXBitmap
template <bool tManaged>
class CTXBitmap : public MObject
{
public:
	CTXBitmap(HBITMAP hBitmap = nullptr);
	~CTXBitmap(void);

	CTXBitmap& operator=(HBITMAP hBitmap);
	operator   HBITMAP(void) const;
	bool       IsValid(void) const;

	void    Attach(HBITMAP hBitmap);
	HBITMAP Detach(void);

	HBITMAP LoadBitmap(HINSTANCE hInst, PXStr pszBitmap);
	HBITMAP LoadOEMBitmap(UInt uIDBitmap); // for OBM_/OCR_/OIC_
	HBITMAP LoadMappedBitmap(HINSTANCE hInst, UInt uIDBitmap, UShort usFlags = 0, LPCOLORMAP pColorMap = nullptr, Int nMapSize = 0);
	HBITMAP CreateBitmap(Int nWidth, Int nHeight, UInt uPlanes, UInt uBitsPerPixel, const void* pBits);
	HBITMAP CreateBitmapIndirect(BITMAP& BitmapRef);
	HBITMAP CreateCompatibleBitmap(HDC hDC, Int nWidth, Int nHeight);
	HBITMAP CreateDiscardableBitmap(HDC hDC, Int nWidth, Int nHeight);
	BOOL    DeleteObject(void);

	Int     GetBitmap(BITMAP* pBitMap) const;
	bool    GetBitmap(BITMAP& BitmapRef) const;
	bool    GetSize(SIZE& size) const;
	ULong   GetBitmapBits(ULong ulCount, void* lpBits) const;
	ULong   SetBitmapBits(ULong ulCount, const void* lpBits);
	BOOL    GetBitmapDimension(LPSIZE lpSize) const;
	BOOL    SetBitmapDimension(Int nWidth, Int nHeight, LPSIZE lpSize = nullptr);
	HBITMAP CreateDIBitmap(HDC hDC, const BITMAPINFOHEADER* lpbmih, ULong ulInit, const void* lpbInit, const BITMAPINFO* lpbmi, UInt uColorUse);
	HBITMAP CreateDIBSection(HDC hDC, const BITMAPINFO* lpbmi, UInt uColorUse, void** ppvBits, HANDLE hSection, ULong ulOffset);
	Int     GetDIBits(HDC hDC, UInt uStartScan, UInt uScanLines, void* lpvBits, LPBITMAPINFO lpbmi, UInt uColorUse) const;
	Int     SetDIBits(HDC hDC, UInt uStartScan, UInt uScanLines, const void* lpvBits, const BITMAPINFO* lpbmi, UInt uColorUse);
private:
	CTXBitmap(const CTXBitmap&);
	CTXBitmap& operator=(const CTXBitmap&);
public:
	HBITMAP   m_hBitmap;
};
typedef CTXBitmap<false>   CXBitmapRef;
typedef CTXBitmap<true>    CXBitmap;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTXRgn
template <bool tManaged>
class CTXRgn : public MObject
{
public:
	CTXRgn(HRGN hRgn = nullptr);
	~CTXRgn(void);

	CTXRgn&  operator=(HRGN hRgn);
	operator HRGN(void) const;
	bool     IsValid(void) const;

	void Attach(HRGN hRgn);
	HRGN Detach(void);

	HRGN CreateRectRgn(Int nx1, Int ny1, Int nx2, Int ny2);
	HRGN CreateRectRgnIndirect(LPCRECT lpRect);
	HRGN CreateEllipticRgn(Int nx1, Int ny1, Int nx2, Int ny2);
	HRGN CreateEllipticRgnIndirect(LPCRECT lpRect);
	HRGN CreatePolygonRgn(LPPOINT lpPoints, Int nCount, Int nMode);
	HRGN CreatePolyPolygonRgn(LPPOINT lpPoints, PInt lpnPolyCounts, Int nCount, Int nPolyFillMode);
	HRGN CreateRoundRectRgn(Int nx1, Int ny1, Int nx2, Int ny2, Int nx3, Int ny3);
	HRGN CreateFromPath(HDC hDC);
	HRGN CreateFromData(const XFORM* lpXForm, Int nCount, const RGNDATA* pRgnData);
	BOOL DeleteObject(void);

	void SetRectRgn(Int nx1, Int ny1, Int nx2, Int ny2);
	void SetRectRgn(LPCRECT lpRect);
	Int  CombineRgn(HRGN hRgnSrc1, HRGN hRgnSrc2, Int nCombineMode);
	Int  CombineRgn(HRGN hRgnSrc, Int nCombineMode);
	Int  CopyRgn(HRGN hRgnSrc);
	BOOL EqualRgn(HRGN hRgn) const;
	Int  OffsetRgn(Int nx, Int ny);
	Int  OffsetRgn(POINT& point);
	Int  GetRgnBox(LPRECT lpRect) const;
	BOOL PtInRegion(Int nx, Int ny) const;
	BOOL PtInRegion(POINT& point) const;
	BOOL RectInRegion(LPCRECT lpRect) const;
	Int  GetRegionData(LPRGNDATA lpRgnData, Int nDataSize) const;
private:
	CTXRgn(const CTXRgn&);
	CTXRgn& operator=(const CTXRgn&);
public:
	HRGN   m_hRgn;
};
typedef CTXRgn<false>   CXRgnRef;
typedef CTXRgn<true>    CXRgn;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTXDC
template <bool tManaged>
class CTXDC : public MObject
{
public:
	CTXDC(HDC hDC = nullptr);
	~CTXDC(void);

	CTXDC&   operator=(HDC hDC);
	operator HDC(void) const;
	bool     IsValid(void) const;

	void     Attach(HDC hDC);
	HDC      Detach(void);

	// Operations
	HWND WindowFromDC(void) const;

	CXPenRef    GetCurrentPen(void) const;
	CXBrushRef  GetCurrentBrush(void) const;
	CXFontRef   GetCurrentFont(void) const;
	CXBitmapRef GetCurrentBitmap(void) const;

	HDC      CreateDC(PCXStr pszDriverName, PCXStr pszDeviceName, PCXStr pszOutput, const DEVMODE* lpInitData);
	HDC      CreateCompatibleDC(HDC hDC = nullptr);
	BOOL     DeleteDC(void);
	// Device-Context Functions
	Int      SaveDC(void);
	BOOL     RestoreDC(Int nSavedDC);
	Int      GetDeviceCaps(Int nIndex) const;
	UInt     SetBoundsRect(LPCRECT lpRectBounds, UInt uFlags);
	UInt     GetBoundsRect(LPRECT lpRectBounds, UInt uFlags) const;
	BOOL     ResetDC(const DEVMODE* lpDevMode);
	// Drawing-Tool Functions
	BOOL     GetBrushOrg(LPPOINT lpPoint) const;
	BOOL     SetBrushOrg(Int nx, Int ny, LPPOINT lpPoint = nullptr);
	BOOL     SetBrushOrg(POINT& point, LPPOINT lpPointRet = nullptr);
	Int      EnumObjects(Int nObjectType, Int (CALLBACK* lpfn)(void*, LPARAM), LPARAM lpData);
	// Type-safe selection helpers
	HPEN     SelectPen(HPEN hPen);
	HBRUSH   SelectBrush(HBRUSH hBrush);
	HFONT    SelectFont(HFONT hFont);
	HBITMAP  SelectBitmap(HBITMAP hBitmap);
	Int      SelectRgn(HRGN hRgn);       // special return for regions
	// Type-safe selection helpers for stock objects
	HPEN     SelectStockPen(Int nPen);
	HBRUSH   SelectStockBrush(Int nBrush);
	HFONT    SelectStockFont(Int nFont);
	HPALETTE SelectStockPalette(Int nPalette, BOOL bForceBackground);
	// Color and Color Palette Functions
	COLORREF GetNearestColor(COLORREF clrColor) const;
	HPALETTE SelectPalette(HPALETTE hPalette, BOOL bForceBackground);
	UInt     RealizePalette(void);
	void     UpdateColors(void);
	// Drawing-Attribute Functions
	COLORREF GetBkColor(void) const;
	Int      GetBkMode(void) const;
	Int      GetPolyFillMode(void) const;
	Int      GetROP2(void) const;
	Int      GetStretchBltMode(void) const;
	COLORREF GetTextColor(void) const;
	COLORREF SetBkColor(COLORREF clrColor);
	Int      SetBkMode(Int nBkMode);
	Int      SetPolyFillMode(Int nPolyFillMode);
	Int      SetROP2(Int nDrawMode);
	Int      SetStretchBltMode(Int nStretchMode);
	COLORREF SetTextColor(COLORREF clrColor);
	BOOL     GetColorAdjustment(LPCOLORADJUSTMENT lpColorAdjust) const;
	BOOL     SetColorAdjustment(const COLORADJUSTMENT* lpColorAdjust);
	// Mapping Functions
	Int      GetMapMode(void) const;
	BOOL     GetViewportOrg(LPPOINT lpPoint) const;
	Int      SetMapMode(Int nMapMode);
	// Viewport Origin
	BOOL     SetViewportOrg(Int nx, Int ny, LPPOINT lpPoint = nullptr);
	BOOL     SetViewportOrg(POINT& point, LPPOINT lpPointRet = nullptr);
	BOOL     OffsetViewportOrg(Int nWidth, Int nHeight, LPPOINT lpPoint = nullptr);
	// Viewport Extent
	BOOL     GetViewportExt(LPSIZE lpSize) const;
	BOOL     SetViewportExt(Int nx, Int ny, LPSIZE lpSize = nullptr);
	BOOL     SetViewportExt(SIZE& size, LPSIZE lpSizeRet = nullptr);
	BOOL     ScaleViewportExt(Int nxNum, Int nxDenom, Int nyNum, Int nyDenom, LPSIZE lpSize = nullptr);
	// Window Origin
	BOOL     GetWindowOrg(LPPOINT lpPoint) const;
	BOOL     SetWindowOrg(Int nx, Int ny, LPPOINT lpPoint = nullptr);
	BOOL     SetWindowOrg(POINT& point, LPPOINT lpPointRet = nullptr);

	BOOL     OffsetWindowOrg(Int nWidth, Int nHeight, LPPOINT lpPoint = nullptr);
	// Window extent
	BOOL     GetWindowExt(LPSIZE lpSize) const;
	BOOL     SetWindowExt(Int nx, Int ny, LPSIZE lpSize = nullptr);
	BOOL     SetWindowExt(SIZE& size, LPSIZE lpSizeRet = nullptr);
	BOOL     ScaleWindowExt(Int nxNum, Int nxDenom, Int nyNum, Int nyDenom, LPSIZE lpSize = nullptr);
	// Coordinate Functions
	BOOL     DPtoLP(LPPOINT lpPoints, Int nCount = 1) const;
	BOOL     DPtoLP(LPRECT lpRect) const;
	BOOL     DPtoLP(LPSIZE lpSize) const;
	BOOL     LPtoDP(LPPOINT lpPoints, Int nCount = 1) const;
	BOOL     LPtoDP(LPRECT lpRect) const;
	BOOL     LPtoDP(LPSIZE lpSize) const;

	void     DPtoHIMETRIC(LPSIZE lpSize) const;
	void     HIMETRICtoDP(LPSIZE lpSize) const;
	void     LPtoHIMETRIC(LPSIZE lpSize) const;
	void     HIMETRICtoLP(LPSIZE lpSize) const;

	// Region Functions
	BOOL     FillRgn(HRGN hRgn, HBRUSH hBrush);
	BOOL     FrameRgn(HRGN hRgn, HBRUSH hBrush, Int nWidth, Int nHeight);
	BOOL     InvertRgn(HRGN hRgn);
	BOOL     PaintRgn(HRGN hRgn);
	// Clipping Functions
	Int      GetClipBox(LPRECT lpRect) const;
	BOOL     PtVisible(Int nx, Int ny) const;
	BOOL     PtVisible(POINT& point) const;
	BOOL     RectVisible(LPCRECT lpRect) const;
	Int      SelectClipRgn(HRGN hRgn);
	Int      ExcludeClipRect(Int nx1, Int ny1, Int nx2, Int ny2);
	Int      ExcludeClipRect(LPCRECT lpRect);
	Int      ExcludeUpdateRgn(HWND hWnd);
	Int      IntersectClipRect(Int nx1, Int ny1, Int nx2, Int ny2);
	Int      IntersectClipRect(LPCRECT lpRect);
	Int      OffsetClipRgn(Int nx, Int ny);
	Int      OffsetClipRgn(SIZE& size);
	Int      SelectClipRgn(HRGN hRgn, Int nMode);
	// Line-Output Functions
	BOOL     GetCurrentPosition(LPPOINT lpPoint) const;
	BOOL     MoveTo(Int nx, Int ny, LPPOINT lpPoint = nullptr);
	BOOL     MoveTo(POINT& point, LPPOINT lpPointRet = nullptr);
	BOOL     LineTo(Int nx, Int ny);
	BOOL     LineTo(POINT& point);
	BOOL     Arc(Int nx1, Int ny1, Int nx2, Int ny2, Int nx3, Int ny3, Int nx4, Int ny4);
	BOOL     Arc(LPCRECT lpRect, POINT& ptStart, POINT& ptEnd);
	BOOL     Polyline(LPPOINT lpPoints, Int nCount);
	BOOL     AngleArc(Int nx, Int ny, Int nRadius, Float fStartAngle, Float fSweepAngle);
	BOOL     ArcTo(Int nx1, Int ny1, Int nx2, Int ny2, Int nx3, Int ny3, Int nx4, Int ny4);
	BOOL     ArcTo(LPCRECT lpRect, POINT& ptStart, POINT& ptEnd);
	Int      GetArcDirection(void) const;
	Int      SetArcDirection(Int nArcDirection);
	BOOL     PolyDraw(const POINT* lpPoints, const PByte lpbTypes, Int nCount);
	BOOL     PolylineTo(const POINT* lpPoints, Int nCount);
	BOOL     PolyPolyline(const POINT* lpPoints, const PULong lpulPolyPoints, Int nCount);
	BOOL     PolyBezier(const POINT* lpPoints, Int nCount);
	BOOL     PolyBezierTo(const POINT* lpPoints, Int nCount);
	// Simple Drawing Functions
	BOOL     FillRect(LPCRECT lpRect, HBRUSH hBrush);
	BOOL     FrameRect(LPCRECT lpRect, HBRUSH hBrush);
	BOOL     InvertRect(LPCRECT lpRect);
	BOOL     DrawIcon(Int nx, Int ny, HICON hIcon);
	BOOL     DrawIcon(POINT& point, HICON hIcon);
	BOOL     DrawIconEx(Int nx, Int ny, HICON hIcon, Int nxWidth, Int nyWidth, UInt uStepIfAniCur = 0, HBRUSH hbrFlickerFreeDraw = nullptr, UInt uFlags = DI_NORMAL);
	BOOL     DrawIconEx(POINT& point, HICON hIcon, SIZE& size, UInt uStepIfAniCur = 0, HBRUSH hbrFlickerFreeDraw = nullptr, UInt uFlags = DI_NORMAL);
	BOOL     DrawState(POINT& pt, SIZE& size, HBITMAP hBitmap, UInt nFlags, HBRUSH hBrush = nullptr);
	BOOL     DrawState(POINT& pt, SIZE& size, HICON hIcon, UInt nFlags, HBRUSH hBrush = nullptr);
	BOOL     DrawState(POINT& pt, SIZE& size, PCXStr pszText, UInt nFlags, BOOL bPrefixText = TRUE, Int nTextLen = 0, HBRUSH hBrush = nullptr);
	BOOL     DrawState(POINT& pt, SIZE& size, DRAWSTATEPROC lpDrawProc, LPARAM lData, UInt uFlags, HBRUSH hBrush = nullptr);
	// Ellipse and Polygon Functions
	BOOL     Chord(Int nx1, Int ny1, Int nx2, Int ny2, Int nx3, Int ny3, Int nx4, Int ny4);
	BOOL     Chord(LPCRECT lpRect, POINT& ptStart, POINT& ptEnd);
	void     DrawFocusRect(LPCRECT lpRect);
	BOOL     Ellipse(Int x1, Int y1, Int x2, Int y2);
	BOOL     Ellipse(LPCRECT lpRect);
	BOOL     Pie(Int nx1, Int ny1, Int nx2, Int ny2, Int nx3, Int ny3, Int nx4, Int ny4);
	BOOL     Pie(LPCRECT lpRect, POINT& ptStart, POINT& ptEnd);
	BOOL     Polygon(LPPOINT lpPoints, Int nCount);
	BOOL     PolyPolygon(LPPOINT lpPoints, PInt lpnPolyCounts, Int nCount);
	BOOL     Rectangle(Int nx1, Int ny1, Int nx2, Int ny2);
	BOOL     Rectangle(LPCRECT lpRect);
	BOOL     RoundRect(Int nx1, Int ny1, Int nx2, Int ny2, Int nx3, Int ny3);
	BOOL     RoundRect(LPCRECT lpRect, POINT& point);
	// Bitmap Functions
	BOOL     PatBlt(Int nx, Int ny, Int nWidth, Int nHeight, ULong ulRop);
	BOOL     BitBlt(Int nx, Int ny, Int nWidth, Int nHeight, HDC hSrcDC, Int nxSrc = 0, Int nySrc = 0, ULong ulRop = SRCCOPY);
	BOOL     StretchBlt(Int nx, Int ny, Int nWidth, Int nHeight, HDC hSrcDC, Int nxSrc, Int nySrc, Int nSrcWidth, Int nSrcHeight, ULong ulRop = SRCCOPY);
	COLORREF GetPixel(Int nx, Int ny) const;
	COLORREF GetPixel(POINT& point) const;
	COLORREF SetPixel(Int nx, Int ny, COLORREF clrColor);
	COLORREF SetPixel(POINT& point, COLORREF clrColor);
	BOOL     FloodFill(Int nx, Int ny, COLORREF clrColor);
	BOOL     ExtFloodFill(Int nx, Int ny, COLORREF clrColor, UInt uFillType);
	BOOL     MaskBlt(Int nx, Int ny, Int nWidth, Int nHeight, HDC hSrcDC, Int nxSrc, Int nySrc, HBITMAP hMaskBitmap, Int nxMask, Int nyMask, ULong ulRop);
	BOOL     PlgBlt(LPPOINT lpPoint, HDC hSrcDC, Int nxSrc, Int nySrc, Int nWidth, Int nHeight, HBITMAP hMaskBitmap, Int nxMask, Int nyMask);
	BOOL     SetPixelV(Int nx, Int ny, COLORREF clrColor);
	BOOL     SetPixelV(POINT& point, COLORREF clrColor);
	BOOL     TransparentBlt(Int nx, Int ny, Int nWidth, Int nHeight, HDC hSrcDC, Int nxSrc, Int nySrc, Int nSrcWidth, Int nSrcHeight, UInt uTransparent);
	BOOL     GradientFill(const PTRIVERTEX pVertices, ULong ulVertices, void* pMeshElements, ULong ulMeshElements, ULong ulMode);
	BOOL     AlphaBlend(Int nx, Int ny, Int nWidth, Int nHeight, HDC hSrcDC, Int nxSrc, Int nySrc, Int nSrcWidth, Int nSrcHeight, BLENDFUNCTION bf);
	// Text Functions
	BOOL     TextOut(Int nx, Int ny, PCXStr pszString, Int nCount = -1);
	BOOL     ExtTextOut(Int nx, Int ny, UInt uOptions, LPCRECT lpRect, PCXStr pszString, UInt uCount = -1, PInt lpnDxWidths = nullptr);
	SIZE     TabbedTextOut(Int nx, Int ny, PCXStr pszString, Int nCount = -1, Int nTabPositions = 0, PInt lpnTabStopPositions = nullptr, Int nTabOrigin = 0);
	Int      DrawText(PCXStr pszText, Int nText, LPRECT lpRect, UInt uFormat);
	Int      DrawText(PXStr pszText, Int nText, LPRECT lpRect, UInt uFormat);
	Int      DrawTextEx(PXStr pszText, Int nText, LPRECT lpRect, UInt uFormat, LPDRAWTEXTPARAMS lpDTParams = nullptr);
	BOOL     GetTextExtent(PCXStr pszString, Int nCount, LPSIZE lpSize) const;
	BOOL     GetTextExtentExPoint(PCXStr pszString, Int nCount, LPSIZE lpSize, Int nMaxExtent, PInt lpnFit = nullptr, PInt lpnDx = nullptr);
	ULong    GetTabbedTextExtent(PCXStr pszString, Int nCount = -1, Int nTabPositions = 0, PInt lpnTabStopPositions = nullptr) const;
	BOOL     GrayString(HBRUSH hBrush, BOOL (CALLBACK* lpfnOutput)(HDC, LPARAM, Int), LPARAM lpData, Int nCount, Int nx, Int ny, Int nWidth, Int nHeight);
	UInt     GetTextAlign(void) const;
	UInt     SetTextAlign(UInt uFlags);
	Int      GetTextFace(PXStr pszFaceName, Int nCount) const;
	Int      GetTextFaceLen(void) const;
	BOOL     GetTextMetrics(LPTEXTMETRIC lpMetrics) const;
	Int      SetTextJustification(Int nBreakExtra, Int nBreakCount);
	Int      GetTextCharacterExtra(void) const;
	Int      SetTextCharacterExtra(Int nCharExtra);
	// Advanced Drawing
	BOOL     DrawEdge(LPRECT lpRect, UInt uEdge, UInt uFlags);
	BOOL     DrawFrameControl(LPRECT lpRect, UInt uType, UInt uState);
	// Scrolling Functions
	BOOL     ScrollDC(Int ndx, Int ndy, LPCRECT lpRectScroll, LPCRECT lpRectClip, HRGN hRgnUpdate, LPRECT lpRectUpdate);
	// Font Functions
	BOOL     GetCharWidth(UInt uFirstChar, UInt uLastChar, PInt lpnBuffer) const;
	BOOL     GetCharWidth32(UInt uFirstChar, UInt uLastChar, PInt lpnBuffer) const;
	ULong    SetMapperFlags(ULong ulFlag);
	BOOL     GetAspectRatioFilter(LPSIZE lpSize) const;
	BOOL     GetCharABCWidths(UInt uFirstChar, UInt uLastChar, LPABC lpabc) const;
	ULong    GetFontData(ULong ulTable, ULong ulOffset, void* lpData, ULong ulData) const;
	Int      GetKerningPairs(Int nPairs, LPKERNINGPAIR lpkrnpair) const;
	UInt     GetOutlineTextMetrics(UInt uData, LPOUTLINETEXTMETRIC lpotm) const;
	ULong    GetGlyphOutline(UInt uChar, UInt uFormat, LPGLYPHMETRICS lpgm, ULong ulBuffer, void* lpBuffer, const MAT2* lpmat2) const;
	BOOL     GetCharABCWidths(UInt uFirstChar, UInt uLastChar, LPABCFLOAT lpABCF) const;
	BOOL     GetCharWidth(UInt uFirstChar, UInt uLastChar, PFloat lpfBuffer) const;
	// Printer/Device Escape Functions
	Int      Escape(Int nEscape, Int nCount, PCStr pszInData, void* lpOutData);
	Int      Escape(Int nEscape, Int nInputSize, PCStr pszInputData, Int nOutputSize, PStr pszOutputData);
	Int      DrawEscape(Int nEscape, Int nInputSize, PCStr pszInputData);
	// Escape helpers
	Int      StartDoc(PCXStr pszDocName);  // old Win3.0 version
	Int      StartDoc(LPDOCINFO lpDocInfo);
	Int      StartPage(void);
	Int      EndPage(void);
	Int      SetAbortProc(BOOL (CALLBACK* lpfn)(HDC, Int));
	Int      AbortDoc(void);
	Int      EndDoc(void);
	// MetaFile Functions
	BOOL     PlayMetaFile(HMETAFILE hMF);
	BOOL     PlayMetaFile(HENHMETAFILE hEnhMetaFile, LPCRECT lpBounds);
	BOOL     AddMetaFileComment(UInt uDataSize, const PByte pbCommentData); // can be used for enhanced metafiles only
	// Path Functions
	BOOL     AbortPath(void);
	BOOL     BeginPath(void);
	BOOL     CloseFigure(void);
	BOOL     EndPath(void);
	BOOL     FillPath(void);
	BOOL     FlattenPath(void);
	BOOL     StrokeAndFillPath(void);
	BOOL     StrokePath(void);
	BOOL     WidenPath(void);
	BOOL     GetMiterLimit(PFloat pfMiterLimit) const;
	BOOL     SetMiterLimit(Float fMiterLimit);
	Int      GetPath(LPPOINT lpPoints, PByte lpbTypes, Int nCount) const;
	BOOL     SelectClipPath(Int nMode);
	void     FillSolidRect(LPCRECT lpRect, COLORREF clr);
	void     FillSolidRect(Int nx, Int ny, Int ncx, Int ncy, COLORREF clr);
	void     Draw3dRect(LPCRECT lpRect, COLORREF clrTopLeft, COLORREF clrBottomRight);
	void     Draw3dRect(Int nx, Int ny, Int ncx, Int ncy, COLORREF clrTopLeft, COLORREF clrBottomRight);
	// DIB support
	Int      SetDIBitsToDevice(Int nx, Int ny, ULong ulWidth, ULong ulHeight, Int nxSrc, Int nySrc, UInt uStartScan, UInt uScanLines, const void* lpvBits, const BITMAPINFO* lpbmi, UInt uColorUse);
	Int      StretchDIBits(Int nx, Int ny, Int nWidth, Int nHeight, Int nxSrc, Int nySrc, Int nSrcWidth, Int nSrcHeight, const void* lpvBits, const BITMAPINFO* lpbmi, UInt uColorUse, ULong ulRop);
	UInt     GetDIBColorTable(UInt uStartIndex, UInt uEntries, RGBQUAD* pColors) const;
	UInt     SetDIBColorTable(UInt uStartIndex, UInt uEntries, const RGBQUAD* pColors);
	COLORREF GetDCPenColor(void) const;
	COLORREF SetDCPenColor(COLORREF clr);
	COLORREF GetDCBrushColor(void) const;
	COLORREF SetDCBrushColor(COLORREF clr);
	ULong    GetFontUnicodeRanges(LPGLYPHSET lpgs) const;
	ULong    GetGlyphIndices(PCXStr pszStr, Int nCount, PUShort pgi, ULong ulFlags) const;
	BOOL     GetTextExtentPointI(PUShort pgiIn, Int ncgi, LPSIZE lpSize) const;
	BOOL     GetTextExtentExPointI(PUShort pgiIn, Int ncgi, Int nMaxExtent, PInt lpnFit, PInt lpnDx, LPSIZE lpSize) const;
	BOOL     GetCharWidthI(UInt ugiFirst, UInt ucgi, PUShort pgi, PInt lpnBuffer) const;
	BOOL     GetCharABCWidthsI(UInt ugiFirst, UInt ucgi, PUShort pgi, LPABC lpabc) const;
	BOOL     ColorCorrectPalette(HPALETTE hPalette, ULong ulFirstEntry, ULong ulNumOfEntries);
private:
	CTXDC(const CTXDC&);
	CTXDC& operator=(const CTXDC&);
public:
	HDC   m_hDC;
};
typedef CTXDC<false>   CXDCRef;
typedef CTXDC<true>    CXDC;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXPaintDC
class CXPaintDC : public CXDC
{
public:
	CXPaintDC(HWND hWnd);
	~CXPaintDC(void);
private:
	CXPaintDC(const CXPaintDC&);
	CXPaintDC& operator=(const CXPaintDC&);
public:
	HWND          m_hWnd;
	PAINTSTRUCT   m_ps;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXClientDC
class CXClientDC : public CXDC
{
public:
	CXClientDC(HWND hWnd);
	~CXClientDC(void);
private:
	CXClientDC(const CXClientDC&);
	CXClientDC& operator=(const CXClientDC&);
public:
	HWND   m_hWnd;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXWindowDC
class CXWindowDC : public CXDC
{
public:
	CXWindowDC(HWND hWnd);
	~CXWindowDC(void);
private:
	CXWindowDC(const CXWindowDC&);
	CXWindowDC& operator=(const CXWindowDC&);
public:
	HWND   m_hWnd;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXMemDC
class CXMemDC : public CXDC
{
public:
	CXMemDC(HDC hDC, PRECT pRect = nullptr);
	CXMemDC(HDC hDC, Long lWidth, Long lHeight);
	~CXMemDC(void);
private:
	CXMemDC(const CXMemDC&);
	CXMemDC& operator=(const CXMemDC&);
public:
	HDC       m_hDstDC;
	HBITMAP   m_hOld;
	CXBitmap  m_hBmp;
	CXRect    m_rtPaint;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXUIClip
class CXUIClip : public MObject 
{
public:
	CXUIClip(void);
	~CXUIClip(void);

	void GenerateClip(CXDC& dc, CXRect& rt);
	void GenerateRoundClip(CXDC& dc, CXRect& rtValid, CXRect& rcControl, Int nWidth, Int nHeight);
	void UseOldClipBegin(CXDC& dc);
	void UseOldClipEnd(CXDC& dc);
private:
	CXDCRef   m_RefDC;
	CXRgn     m_OldRgn;
	CXRgn     m_CurRgn;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "targetxuigdi.inl"

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#endif // __TARGET_XUI_GDI_H__