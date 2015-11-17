///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : targetxuibasewnd.h                                     ¡¡¡¡                           //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2005-02-02     version 0.0.0.1                                                             //
//   Update : 2015-03-16     version 0.0.5.5                                                             //
//   Detail : xui windows implement                                                                      //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TARGET_XUI_BASE_WND_H__
#define __TARGET_XUI_BASE_WND_H__

#pragma once

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#include "targetxuigdi.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXWnd
class CXWnd : public IXWnd
{
public:
	CXWnd(Int nType);
	virtual ~CXWnd(void);

	virtual PCXStr GetXWndClass(void) OVERRIDE;
protected:
	virtual LRESULT OnCreate(LPCREATESTRUCT);
	virtual void    OnDestroy(void);
	BEGIN_INLINE_MSG_MAP( CXWnd )
	// {{ --
		MSG_WM_CREATE()
		MSG_WM_DESTROY()
	// --}}
	END_CHAIN_MSG_MAP( IXWnd )
private:
	CXWnd(const CXWnd&);
	CXWnd& operator=(const CXWnd&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXDlg
class CXDlg : public IXWnd  
{
public:
	CXDlg(Int nType = 0);
	virtual ~CXDlg(void);

	virtual PCXStr GetXWndClass(void) OVERRIDE;
	virtual bool   PreTranslateMessage(LPMSG pMsg) OVERRIDE;

	virtual bool   OnDDWExchange(Int, IXWnd*&);
protected :
	virtual LRESULT OnInitDialog(void);
	virtual void    OnDestroy(void);
	BEGIN_INLINE_MSG_MAP( CXDlg )
	// {{ --
		//MSG_WM_DRAW_MEASURE_ITEM()
		MSG_WM_INITDIALOG()
		MSG_WM_DESTROY()
	// --}}
	END_CHAIN_MSG_MAP( IXWnd )
private:
	CXDlg(const CXDlg&);
	CXDlg& operator=(const CXDlg&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXFrameWnd
class CXFrameWnd : public CXWnd
{
public:
	CXFrameWnd(void);
	virtual ~CXFrameWnd(void);
protected:	
	virtual void    FrameLayout(void);
	virtual LRESULT OnNotify(Int nID, LPNMHDR pHead, BOOL& bHandled);
	BEGIN_INLINE_MSG_MAP( CXFrameWnd )
	// {{ --
		MSG_WM_FRAME_SIZE()
		MSG_WM_FRAME_XLAYOUT()
		MSG_WM_FRAME_NOTIFY()
		//MSG_WM_DRAW_MEASURE_ITEM()
	// --}}
	END_CHAIN_MSG_MAP( CXWnd )
private:
	CXFrameWnd(const CXFrameWnd&);
	CXFrameWnd& operator=(const CXFrameWnd&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXView : ulStyle = XUIS_CHILD
class CXView : public CXWnd
{
public:
	CXView(void);
	virtual ~CXView(void);
protected:	
	virtual void OnPaint(void);
	BEGIN_INLINE_MSG_MAP( CXView )
	// {{ --
		MSG_WM_PAINT()
	// --}}
	END_CHAIN_MSG_MAP( CXWnd )
private:
	CXView(const CXView&);
	CXView& operator=(const CXView&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXControl
class NOVTABLE CXControl : public IXWnd  
{
protected:
	CXControl(Int nType);
	virtual ~CXControl(void);

	virtual LRESULT OnNotify(Int nID, LPNMHDR pHead, BOOL& bHandled);
	virtual LRESULT OnXLayout(WPARAM wParam, LPARAM lParam, BOOL& bHandled); 
	virtual LRESULT OnXNotify(WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	BEGIN_INLINE_MSG_MAP( CXControl )
	// {{ --
		MSG_WM_NOTIFY()
		MSG_XUIM_LAYOUT()
		MSG_XUIM_NOTIFY()
	// --}}
	END_CHAIN_MSG_MAP( IXWnd )
private:
	CXControl(const CXControl&);
	CXControl& operator=(const CXControl&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXStatic : WC_STATIC
class CXStatic : public CXControl
{
public:
	CXStatic(void);
	virtual ~CXStatic(void);

	virtual PCXStr GetXWndClass(void) OVERRIDE;

	HICON GetIcon(void) const;
	HICON SetIcon(HICON hIcon);
	HENHMETAFILE GetEnhMetaFile(void) const;
	HENHMETAFILE SetEnhMetaFile(HENHMETAFILE hMetaFile);
	HBITMAP GetBitmap(void) const;
	HBITMAP SetBitmap(HBITMAP hBitmap);
	HCURSOR GetCursor(void) const;
	HCURSOR SetCursor(HCURSOR hCursor);
private:
	CXStatic(const CXStatic&);
	CXStatic& operator=(const CXStatic&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXButton : WC_BUTTON
class CXButton : public CXControl 
{
public :
	CXButton(void);
	virtual ~CXButton(void);

	virtual PCXStr GetXWndClass(void) OVERRIDE;

	UInt GetState(void) const;
	void SetState(BOOL bHighlight);
	Int  GetCheck(void) const;
	void SetCheck(Int nCheck);
	UInt GetButtonStyle(void) const;
	void SetButtonStyle(UInt uStyle, BOOL bRedraw = TRUE);

	HICON GetIcon(void) const;
	HICON SetIcon(HICON hIcon);

	HBITMAP GetBitmap(void) const;
	HBITMAP SetBitmap(HBITMAP hBitmap);
#if (_WIN32_WINNT >= 0x0501)
	BOOL GetIdealSize(LPSIZE lpSize) const;
	BOOL GetImageList(PBUTTON_IMAGELIST pButtonImagelist) const;
	BOOL SetImageList(PBUTTON_IMAGELIST pButtonImagelist);
	BOOL GetTextMargin(LPRECT lpRect) const;
	BOOL SetTextMargin(LPRECT lpRect);
#endif // (_WIN32_WINNT >= 0x0501)
#if (_WIN32_WINNT >= 0x0600)
	void SetDontClick(BOOL bDontClick);
#endif // (_WIN32_WINNT >= 0x0600)
#if (_WIN32_WINNT >= 0x0600)
	BOOL SetDropDownState(BOOL bDropDown);
	BOOL GetSplitInfo(PBUTTON_SPLITINFO pSplitInfo) const;
	BOOL SetSplitInfo(PBUTTON_SPLITINFO pSplitInfo);
	Int  GetNoteLength(void) const;
	BOOL GetNote(PWStr pszNoteText, Int nNoteText) const;
	BOOL SetNote(PCWStr pszNoteText);
	LRESULT SetElevationRequiredState(BOOL bSet);
#endif // (_WIN32_WINNT >= 0x0600)
	// Operations
	void Click(void);
private:
	CXButton(const CXButton&);
	CXButton& operator=(const CXButton&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXEdit : WC_EDIT
class CXEdit : public CXControl 
{
public :
	CXEdit(void);
	virtual ~CXEdit(void);

	virtual PCXStr GetXWndClass(void) OVERRIDE;

	BOOL   CanUndo(void) const;
	Int    GetLineCount(void) const;
	BOOL   GetModify(void) const;
	void   SetModify(BOOL bModified = TRUE);
	void   GetRect(LPRECT lpRect) const;
	ULong  GetSel(void) const;
	void   GetSel(Int& lStartChar, Int& lEndChar) const;
	HLOCAL GetBuffer(void) const;
	void   SetBuffer(HLOCAL hBuffer);
	ULong  GetMargins(void) const;
	void   SetMargins(UInt uLeft, UInt uRight);
	UInt   GetLimitText(void) const;
	void   SetLimitText(UInt uMax);
	POINT  PosFromChar(UInt uChar) const;
	Int    CharFromPos(POINT& pt, PInt pnLine = nullptr) const;
	// NOTE: first word in pszBuffer must contain the size of the buffer!
	Int    GetLine(Int nIndex, PXStr pszBuffer) const;
	Int    GetLine(Int nIndex, PXStr pszBuffer, Int nMaxLength) const;
	XChar  GetPasswordChar(void) const;
	void   SetPasswordChar(XChar c);

	EDITWORDBREAKPROC GetWordBreakProc(void) const;
	void SetWordBreakProc(EDITWORDBREAKPROC ewbprc);

	Int    GetFirstVisibleLine(void) const;
	Int    GetThumb(void) const;
	BOOL   SetReadOnly(BOOL bReadOnly = TRUE);
	UInt   GetImeStatus(UInt uStatus) const;
	UInt   SetImeStatus(UInt uStatus, UInt uData);
#if (_WIN32_WINNT >= 0x0501)
	BOOL GetCueBannerText(PCWStr pszText, Int nText) const;
	// bKeepWithFocus - Vista only
	BOOL SetCueBannerText(PCWStr pszText, BOOL bKeepWithFocus = FALSE);
#endif // (_WIN32_WINNT >= 0x0501)
	// Operations
	void EmptyUndoBuffer(void);
	BOOL FmtLines(BOOL bAddEOL);
	void LimitText(Int nChars = 0);
	Int  LineFromChar(Int nIndex = -1) const;
	Int  LineIndex(Int nLine = -1) const;
	Int  LineLength(Int nLine = -1) const;
	void LineScroll(Int nLines, Int nChars = 0);
	void ReplaceSel(PCXStr pszNewText, BOOL bCanUndo = FALSE);
	void SetRect(LPCRECT lpRect);
	void SetRectNP(LPCRECT lpRect);
	void SetSel(ULong ulSelection, BOOL bNoScroll = FALSE);
	void SetSel(Int lStartChar, Int lEndChar, BOOL bNoScroll = FALSE);
	void SetSelAll(BOOL bNoScroll = FALSE);
	void SetSelNone(BOOL bNoScroll = FALSE);
	BOOL SetTabStops(Int nTabStops, PInt pnTabStops);
	BOOL SetTabStops(void);
	BOOL SetTabStops(const Int& nEachStop); // takes an 'Int'
	void ScrollCaret(void);
	Int  Scroll(Int nScrollAction);
	void InsertText(Int nInsertAfterChar, PCXStr pszText, BOOL bNoScroll = FALSE, BOOL bCanUndo = FALSE);
	void AppendText(PCXStr pszrText, BOOL bNoScroll = FALSE, BOOL bCanUndo = FALSE);
#if (_WIN32_WINNT >= 0x0501)
	BOOL ShowBalloonTip(PEDITBALLOONTIP pEditBaloonTip);
	BOOL HideBalloonTip(void);
#endif // (_WIN32_WINNT >= 0x0501)
#if (_WIN32_WINNT >= 0x0600)
	ULong GetHilite(void) const;
	void GetHilite(Int& nStartChar, Int& nEndChar) const;
	void SetHilite(Int nStartChar, Int nEndChar);
#endif // (_WIN32_WINNT >= 0x0600)
	// Clipboard operations
	BOOL Undo(void);
	void Clear(void);
	void Copy(void);
	void Cut(void);
	void Paste(void);
private:
	CXEdit(const CXEdit&);
	CXEdit& operator=(const CXEdit&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXListBox : WC_LISTBOX
class CXListBox : public CXControl 
{
public :
	CXListBox(void);
	virtual ~CXListBox(void);

	virtual PCXStr GetXWndClass(void) OVERRIDE;
	// for entire listbox
	Int  GetCount(void) const;
	Int  SetCount(Int nItems);
	Int  GetHorizontalExtent(void) const;
	void SetHorizontalExtent(Int nExtent);
	Int  GetTopIndex(void) const;
	Int  SetTopIndex(Int nIndex);
	LCID GetLocale(void) const;
	LCID SetLocale(LCID nNewLocale);

#if (_WIN32_WINNT >= 0x0500)
	ULong GetListBoxInfo(void) const;
#endif // (_WIN32_WINNT >= 0x0500)

	// for single-selection listboxes
	Int  GetCurSel(void) const;
	Int  SetCurSel(Int nSelect);

	// for multiple-selection listboxes
	Int  GetSel(Int nIndex) const;           // also works for single-selection
	Int  SetSel(Int nIndex, BOOL bSelect = TRUE);
	Int  GetSelCount(void) const;
	Int  GetSelItems(Int nMaxItems, PInt pnIndex) const;
	Int  GetAnchorIndex(void) const;
	void SetAnchorIndex(Int nIndex);
	Int  GetCaretIndex(void) const;
	Int  SetCaretIndex(Int nIndex, BOOL bScroll = TRUE);

	// for listbox items
	uintptr_t GetItemData(Int nIndex) const;
	Int   SetItemData(Int nIndex, uintptr_t utItemData);
	void* GetItemDataPtr(Int nIndex) const;

	Int  SetItemDataPtr(Int nIndex, void* pData);
	Int  GetItemRect(Int nIndex, LPRECT lpRect) const;
	Int  GetText(Int nIndex, PXStr pszBuffer) const;
	Int  GetTextLen(Int nIndex) const;
	Int  GetItemHeight(Int nIndex) const;
	Int  SetItemHeight(Int nIndex, UInt uItemHeight);

	// Settable only attributes
	void SetColumnWidth(Int nWidth);
	BOOL SetTabStops(Int nTabStops, PInt pnTabStops);
	BOOL SetTabStops(void);
	BOOL SetTabStops(const Int& nEachStop);    // takes an 'Int'

	// Operations
	Int  InitStorage(Int nItems, UInt uBytes);
	void ResetContent(void);
	UInt ItemFromPoint(POINT& pt, BOOL& bOutside) const;

	// manipulating listbox items
	Int  AddString(PCXStr pszItem);
	Int  DeleteString(Int nIndex);
	Int  InsertString(Int nIndex, PCXStr pszItem);
	Int  Dir(UInt uAttr, PCXStr pszWildCard);
	Int  AddFile(PCXStr pszFileName);

	// selection helpers
	Int  FindString(Int nStartAfter, PCXStr pszItem) const;
	Int  FindStringExact(Int nIndexStart, PCXStr pszFind) const;
	Int  SelectString(Int nStartAfter, PCXStr pszItem);
	Int  SelItemRange(BOOL bSelect, Int nFirstItem, Int nLastItem);
private:
	CXListBox(const CXListBox&);
	CXListBox& operator=(const CXListBox&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXComboBox : WC_COMBOBOX
class CXComboBox : public CXControl 
{
public :
	CXComboBox(void);
	virtual ~CXComboBox(void);

	virtual PCXStr GetXWndClass(void) OVERRIDE;

	Int   GetCount(void) const;
	Int   GetCurSel(void) const;
	Int   SetCurSel(Int nSelect);
	LCID  GetLocale(void) const;
	LCID  SetLocale(LCID nNewLocale);
	Int   GetTopIndex(void) const;	
	Int   SetTopIndex(Int nIndex);	
	UInt  GetHorizontalExtent(void) const;	
	void  SetHorizontalExtent(UInt uExtent);	
	Int   GetDroppedWidth(void) const;	
	Int   SetDroppedWidth(UInt uWidth);	
#if (_WIN32_WINNT >= 0x0500)
	BOOL  GetComboBoxInfo(PCOMBOBOXINFO pComboBoxInfo) const;
#endif // (_WIN32_WINNT >= 0x0500) 

	// for edit control
	ULong GetEditSel(void) const;	
	BOOL  SetEditSel(Int lStartChar, Int lEndChar);
	// for combobox item
	uintptr_t GetItemData(Int nIndex) const;	
	Int   SetItemData(Int nIndex, uintptr_t utItemData);	
	void* GetItemDataPtr(Int nIndex) const;	
	Int   SetItemDataPtr(Int nIndex, void* pData);	
	Int   GetLBText(Int nIndex, PXStr pszText) const;	
	Int   GetLBTextLen(Int nIndex) const;	
	Int   GetItemHeight(Int nIndex) const;	
	Int   SetItemHeight(Int nIndex, UInt uItemHeight);	
	BOOL  GetExtendedUI(void) const;	
	Int   SetExtendedUI(BOOL bExtended = TRUE);	
	void  GetDroppedControlRect(LPRECT lprect) const;	
	BOOL  GetDroppedState(void) const;		
#if (_WIN32_WINNT >= 0x0501)
	Int GetMinVisible(void) const;
	BOOL SetMinVisible(Int nMinVisible);
	// Vista only
	BOOL GetCueBannerText(PWStr pszText, Int nText) const;
	BOOL SetCueBannerText(PCWStr pszText);
#endif // (_WIN32_WINNT >= 0x0501)
	// Operations
	Int   InitStorage(Int nItems, UInt uBytes);	
	void  ResetContent(void);	
	// for edit control
	BOOL  LimitText(Int nMaxChars);	
	// for drop-down combo boxes
	void  ShowDropDown(BOOL bShowIt = TRUE);	
	// manipulating listbox items
	Int   AddString(PCXStr pszString);	
	Int   DeleteString(Int nIndex);	
	Int   InsertString(Int nIndex, PCXStr pszString);	
	Int   Dir(UInt uAttr, PCXStr pszWildCard);	
	// selection helpers
	Int   FindString(Int nStartAfter, PCXStr pszString) const;	
	Int   FindStringExact(Int nIndexStart, PCXStr pszFind) const;	
	Int   SelectString(Int nStartAfter, PCXStr pszString);	
	// Clipboard operations
	void  Clear(void);	
	void  Copy(void);	
	void  Cut(void);	
	void  Paste(void);	
protected:
	CXComboBox(Int nType);
private:
	CXComboBox(const CXComboBox&);
	CXComboBox& operator=(const CXComboBox&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXComboBoxEx : WC_COMBOBOXEX
class CXComboBoxEx : public CXComboBox
{
public :
	// Constructors
	CXComboBoxEx(void);
	virtual ~CXComboBoxEx(void);
	
	virtual PCXStr GetXWndClass(void) OVERRIDE;

	HIMAGELIST GetImageList(void) const;
	HIMAGELIST SetImageList(HIMAGELIST hImageList);
	ULong GetExtendedStyle(void) const;
	ULong SetExtendedStyle(ULong ulExMask, ULong ulExStyle);
	BOOL  GetUnicodeFormat(void) const;
	BOOL  SetUnicodeFormat(BOOL bUnicode = TRUE);
	// Operations
	Int   InsertItem(const COMBOBOXEXITEM FAR* lpcCBItem);
	Int   DeleteItem(Int nIndex);
	BOOL  GetItem(PCOMBOBOXEXITEM pCBItem) const;
	BOOL  SetItem(const COMBOBOXEXITEM FAR* lpcCBItem);
	HWND  GetComboCtrl(void) const;
	HWND  GetEditCtrl(void) const;
	BOOL  HasEditChanged(void) const;
private:
	CXComboBoxEx(const CXComboBoxEx&);
	CXComboBoxEx& operator=(const CXComboBoxEx&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXScrollBar : WC_SCROLLBAR
class CXScrollBar : public CXControl 
{
public :
	CXScrollBar(void);
	virtual ~CXScrollBar(void);

	virtual PCXStr GetXWndClass(void) OVERRIDE;

	Int  GetScrollPos(void) const;
	Int  SetScrollPos(Int nPos, BOOL bRedraw = TRUE);
	void GetScrollRange(PInt lpnMinPos, PInt lpnMaxPos) const;
	void SetScrollRange(Int nMinPos, Int nMaxPos, BOOL bRedraw = TRUE);
	BOOL GetScrollInfo(LPSCROLLINFO lpScrollInfo) const;
	Int  SetScrollInfo(LPSCROLLINFO lpScrollInfo, BOOL bRedraw = TRUE);
#if (_WIN32_WINNT >= 0x0500)
	BOOL GetScrollBarInfo(PSCROLLBARINFO pScrollBarInfo) const;
#endif // (_WIN32_WINNT >= 0x0500)
	// Operations
	void ShowScrollBar(BOOL bShow = TRUE);
	BOOL EnableScrollBar(UInt uArrowFlags = ESB_ENABLE_BOTH);
private:
	CXScrollBar(const CXScrollBar&);
	CXScrollBar& operator=(const CXScrollBar&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXTab : WC_TABCONTROL
class CXTab : public CXControl  
{
public :
	CXTab(void);
	virtual ~CXTab(void);

	virtual PCXStr GetXWndClass(void) OVERRIDE;

	HIMAGELIST GetImageList(void) const;	
	HIMAGELIST SetImageList(HIMAGELIST hImageList);
	Int   GetItemCount(void) const;	
	BOOL  GetItem(Int nItem, LPTCITEM pTabCtrlItem) const;	
	BOOL  SetItem(Int nItem, LPTCITEM pTabCtrlItem);	
	BOOL  GetItemRect(Int nItem, LPRECT lpRect) const;	
	Int   GetCurSel(void) const;	
	Int   SetCurSel(Int nItem);	
	SIZE  SetItemSize(SIZE& size);
	void  SetPadding(SIZE& size);	
	Int   GetRowCount(void) const;	
	HWND  GetTooltips(void) const;	
	void  SetTooltips(HWND hWndToolTip);	
	Int   GetCurFocus(void) const;	
	void  SetCurFocus(Int nItem);	
	BOOL  SetItemExtra(Int cbExtra);	
	Int   SetMinTabWidth(Int nWidth = -1);	
	ULong GetExtendedStyle(void) const;	
	ULong SetExtendedStyle(ULong ulExMask, ULong ulExStyle);	
	BOOL  GetUnicodeFormat(void) const;	
	BOOL  SetUnicodeFormat(BOOL bUnicode = TRUE);	
	// Operations
	Int   InsertItem(Int nItem, LPTCITEM pTabCtrlItem);	
	BOOL  DeleteItem(Int nItem);	
	BOOL  DeleteAllItems(void);	
	void  AdjustRect(BOOL bLarger, LPRECT lpRect);	
	void  RemoveImage(Int nImage);	
	Int   HitTest(TC_HITTESTINFO* pHitTestInfo) const;	
	void  DeselectAll(BOOL bExcludeFocus = TRUE);	
	BOOL  HighlightItem(Int nIndex, BOOL bHighlight = TRUE);
private:
	CXTab(const CXTab&);
	CXTab& operator=(const CXTab&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXToolTip : TOOLTIPS_CLASS
class CXToolTip : public CXControl
{
public:
	CXToolTip(void); 
	virtual ~CXToolTip(void); 

	virtual PCXStr GetXWndClass(void) OVERRIDE;

	void GetText(LPTOOLINFO lpToolInfo) const;
	BOOL GetToolInfo(LPTOOLINFO lpToolInfo) const;
	void SetToolInfo(LPTOOLINFO lpToolInfo);
	void SetToolRect(LPTOOLINFO lpToolInfo);
	Int  GetToolCount(void) const;
	Int  GetDelayTime(ULong ulType) const;
	void SetDelayTime(ULong ulType, Int nTime);
	void GetMargin(LPRECT lpRect) const;
	void SetMargin(LPRECT lpRect);
	Int  GetMaxTipWidth(void) const;
	Int  SetMaxTipWidth(Int nWidth);
	COLORREF GetTipBkColor(void) const;
	void SetTipBkColor(COLORREF clr);
	COLORREF GetTipTextColor(void) const;
	void SetTipTextColor(COLORREF clr);
	BOOL GetCurrentTool(LPTOOLINFO lpToolInfo) const;
	SIZE GetBubbleSize(LPTOOLINFO lpToolInfo) const;
	BOOL SetTitle(UInt uIcon, PCXStr pszTitle);
#if (_WIN32_WINNT >= 0x0501)
	void GetTitle(PTTGETTITLE pTTGetTitle) const;
	void SetWindowTheme(PCWStr pszTheme);
#endif // (_WIN32_WINNT >= 0x0501)
	void Activate(BOOL bActivate);
	BOOL AddTool(LPTOOLINFO lpToolInfo);
	void DelTool(LPTOOLINFO lpToolInfo);
	BOOL HitTest(LPTTHITTESTINFO lpHitTestInfo) const;
	void RelayEvent(LPMSG lpMsg);
	void UpdateTipText(LPTOOLINFO lpToolInfo);
	BOOL EnumTools(UInt uTool, LPTOOLINFO lpToolInfo) const;
	void Pop(void);
	void TrackActivate(LPTOOLINFO lpToolInfo, BOOL bActivate);
	void TrackPosition(Int nxPos, Int nyPos);
	void Update(void);
	BOOL AdjustRect(LPRECT lpRect, BOOL bLarger = TRUE);
#if (_WIN32_WINNT >= 0x0501)
	void Popup(void);
#endif // (_WIN32_WINNT >= 0x0501)
private:
	CXToolTip(const CXToolTip&);
	CXToolTip& operator=(const CXToolTip&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXListView : WC_LISTVIEW
class CXListView : public CXControl
{
public:
	CXListView(void);
	virtual ~CXListView(void);

	virtual PCXStr GetXWndClass(void) OVERRIDE;

	COLORREF   GetBkColor(void) const;
	BOOL       SetBkColor(COLORREF clr);
	HIMAGELIST GetImageList(Int nType) const;
	HIMAGELIST SetImageList(HIMAGELIST hImg, Int nType);

	Int  GetItemCount(void) const;
	BOOL SetItemCount(Int nItems);
	BOOL GetItem(LPLVITEM pItem) const;
	BOOL SetItem(const LVITEM* pItem);
	BOOL SetItem(Int nItem, Int nSubItem, UInt uMask, 
		         PCXStr pszItem, Int nImage, UInt uState,
				 UInt uStateMask, LPARAM lParam);
	UInt GetItemState(Int nItem, UInt uMask) const;
	BOOL SetItemState(Int nItem, UInt uState, UInt uStateMask);
	BOOL SetItemState(Int nItem, LPLVITEM pItem);
	Int  GetItemText(Int nItem, Int nSubItem, PXStr pszText, Int nLen) const;
	BOOL SetItemText(Int nItem, Int nSubItem, PCXStr pszText);
	uintptr_t GetItemData(Int nItem) const;
	BOOL SetItemData(Int nItem, uintptr_t utData);
	UInt GetCallbackMask(void) const;
	BOOL SetCallbackMask(UInt uMask);
	BOOL GetItemPosition(Int nItem, LPPOINT lpPoint) const;
	BOOL SetItemPosition(Int nItem, POINT& pt);
	BOOL SetItemPosition(Int nItem, Int nx, Int ny);
	Int  GetStringWidth(PCXStr psz) const;
	HWND GetEditControl(void) const;
	BOOL GetColumn(Int nCol, LVCOLUMN* pColumn) const;
	BOOL SetColumn(Int nCol, const LVCOLUMN* pColumn);
	Int  GetColumnWidth(Int nCol) const;
	BOOL SetColumnWidth(Int nCol, Int nWidth);
	BOOL GetViewRect(LPRECT lpRect) const;
	COLORREF GetTextColor(void) const;
	BOOL SetTextColor(COLORREF clr);
	COLORREF GetTextBkColor(void) const;
	BOOL SetTextBkColor(COLORREF clr);
	Int  GetTopIndex(void) const;
	Int  GetCountPerPage(void) const;
	BOOL GetOrigin(LPPOINT lpPoint) const;
	UInt GetSelectedCount(void) const;
	BOOL GetItemRect(Int nItem, LPRECT lpRect, UInt uCode) const;
	HCURSOR GetHotCursor(void) const;
	HCURSOR SetHotCursor(HCURSOR hHotCursor);
	Int  GetHotItem(void) const;
	Int  SetHotItem(Int nIndex);
	BOOL GetColumnOrderArray(Int nCount, PInt lpnArray) const;
	BOOL SetColumnOrderArray(Int nCount, PInt lpnArray);
	HWND GetHeader(void) const;
	BOOL GetSubItemRect(Int nItem, Int nSubItem, Int nFlag, LPRECT lpRect) const;
	ULong SetIconSpacing(Int ncx, Int ncy);
	Int  GetISearchString(PXStr pszBuf) const;
	void GetItemSpacing(SIZE& size, BOOL bSmallIconView = FALSE) const;

	// single-selection only
	Int  GetSelectedIndex(void) const;
	BOOL GetSelectedItem(LPLVITEM pItem) const;
	// extended list view styles
	ULong GetExtendedStyle(void) const;
	// ulExMask = 0 means all styles
	ULong SetExtendedStyle(ULong ulExStyle, ULong ulExMask = 0);
	// checkboxes only
	BOOL GetCheckState(Int nIndex) const;
	BOOL SetCheckState(Int nItem, BOOL bCheck);
	// view type
	ULong GetViewType(void) const;
	ULong SetViewType(ULong ulType);

	BOOL GetBkImage(LPLVBKIMAGE plvbki) const;
	BOOL SetBkImage(LPLVBKIMAGE plvbki);
	Int  GetSelectionMark(void) const;
	Int  SetSelectionMark(Int nIndex);
	BOOL GetWorkAreas(Int nWorkAreas, LPRECT lpRect) const;
	BOOL SetWorkAreas(Int nWorkAreas, LPRECT lpRect);
	ULong GetHoverTime(void) const;
	ULong SetHoverTime(ULong ulHoverTime);
	BOOL GetNumberOfWorkAreas(PInt pnWorkAreas) const;
	BOOL SetItemCountEx(Int nItems, ULong ulFlags);
	HWND GetToolTips(void) const;
	HWND SetToolTips(HWND hWndTT);
	BOOL GetUnicodeFormat(void) const;
	BOOL SetUnicodeFormat(BOOL bUnicode = TRUE);

#if (_WIN32_WINNT >= 0x0501)
	Int  GetSelectedColumn(void) const;
	void SetSelectedColumn(Int nColumn);
	ULong GetView(void) const;
	Int  SetView(ULong ulView);
	BOOL IsGroupViewEnabled(void) const;
	Int  GetGroupInfo(Int nGroupID, PLVGROUP pGroup) const;
	Int  SetGroupInfo(Int nGroupID, PLVGROUP pGroup);
	void GetGroupMetrics(PLVGROUPMETRICS pGroupMetrics) const;
	void SetGroupMetrics(PLVGROUPMETRICS pGroupMetrics);
	void GetTileViewInfo(PLVTILEVIEWINFO pTileViewInfo) const;
	BOOL SetTileViewInfo(PLVTILEVIEWINFO pTileViewInfo);
	void GetTileInfo(PLVTILEINFO pTileInfo) const;
	BOOL SetTileInfo(PLVTILEINFO pTileInfo);
	BOOL GetInsertMark(LPLVINSERTMARK pInsertMark) const;
	BOOL SetInsertMark(LPLVINSERTMARK pInsertMark);
	Int  GetInsertMarkRect(LPRECT lpRect) const;
	COLORREF GetInsertMarkColor(void) const;
	COLORREF SetInsertMarkColor(COLORREF clr);
	COLORREF GetOutlineColor(void) const;
	COLORREF SetOutlineColor(COLORREF clr);
#endif // (_WIN32_WINNT >= 0x0501)

#if (_WIN32_WINNT >= 0x0600)
	Int  GetGroupCount(void) const;
	BOOL GetGroupInfoByIndex(Int nIndex, PLVGROUP pGroup) const;
	BOOL GetGroupRect(Int nGroupID, Int nType, LPRECT lpRect) const;
	UInt GetGroupState(Int nGroupID, UInt uMask) const;
	Int  GetFocusedGroup(void) const;
	BOOL GetEmptyText(PWStr pszText, Int nText) const;
	BOOL GetFooterRect(LPRECT lpRect) const;
	BOOL GetFooterInfo(LPLVFOOTERINFO lpFooterInfo) const;
	BOOL GetFooterItemRect(Int nItem, LPRECT lpRect) const;
	BOOL GetFooterItem(Int nItem, LPLVFOOTERITEM lpFooterItem) const;
	BOOL GetItemIndexRect(PLVITEMINDEX pItemIndex, Int nSubItem, Int nType, LPRECT lpRect) const;
	BOOL SetItemIndexState(PLVITEMINDEX pItemIndex, UInt uState, UInt uMask);
	BOOL GetNextItemIndex(PLVITEMINDEX pItemIndex, UShort usFlags) const;
#endif // (_WIN32_WINNT >= 0x0600)

	// Operations
	Int  InsertColumn(Int nCol, const LVCOLUMN* pColumn);
	Int  InsertColumn(Int nCol, PCXStr psz, Int nFormat = LVCFMT_LEFT,
		              Int nWidth = -1, Int nSubItem = -1, Int iImage = -1, Int iOrder = -1);
	BOOL DeleteColumn(Int nCol);
	Int  InsertItem(UInt nMask, Int nItem, PCXStr pszItem, UInt uState, UInt uStateMask, Int nImage, LPARAM lParam);
	Int  InsertItem(const LVITEM* pItem);
	Int  InsertItem(Int nItem, PCXStr pszItem);
	Int  InsertItem(Int nItem, PCXStr pszItem, Int nImage);
	Int  GetNextItem(Int nItem, Int nFlags) const;
	BOOL DeleteItem(Int nItem);
	BOOL DeleteAllItems(void);
	Int  FindItem(LVFINDINFO* pFindInfo, Int nStart = -1) const;
	Int  HitTest(LVHITTESTINFO* pHitTestInfo) const;
	Int  HitTest(POINT& pt, PUInt puFlags) const;
	BOOL EnsureVisible(Int nItem, BOOL bPartialOK);
	BOOL Scroll(SIZE& size);
	BOOL RedrawItems(Int nFirst, Int nLast);
	BOOL Arrange(UInt uCode);
	HWND EditLabel(Int nItem);
	BOOL Update(Int nItem);
	BOOL SortItems(PFNLVCOMPARE pfnCompare, LPARAM lParamSort);
	HIMAGELIST RemoveImageList(Int nType);
	HIMAGELIST CreateDragImage(Int nItem, LPPOINT lpPoint);
	ULong ApproximateViewRect(Int ncx = -1, Int ncy = -1, Int nCount = -1);
	Int  SubItemHitTest(LPLVHITTESTINFO lpInfo) const;
	Int  AddColumn(PCXStr pszItem, Int nItem, Int nWidth, Int nSubItem = -1,
				   Int nMask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM,
				   Int nFmt = LVCFMT_LEFT);
	Int  AddItem(Int nItem, Int nSubItem, PCXStr pszItem, Int nImageIndex = -1);

#if (_WIN32_WINNT >= 0x0501)
	Int  InsertGroup(Int nItem, PLVGROUP pGroup);
	Int  AddGroup(PLVGROUP pGroup);
	Int  RemoveGroup(Int nGroupID);
	void MoveGroup(Int nGroupID, Int nItem);
	void MoveItemToGroup(Int nItem, Int nGroupID);
	Int  EnableGroupView(BOOL bEnable);
	Int  SortGroups(PFNLVGROUPCOMPARE pCompareFunc, LPVOID lpVoid = nullptr);
	void InsertGroupSorted(PLVINSERTGROUPSORTED pInsertGroupSorted);
	void RemoveAllGroups(void);
	BOOL HasGroup(Int nGroupID);
	BOOL InsertMarkHitTest(LPPOINT lpPoint, LPLVINSERTMARK pInsertMark) const;
	BOOL SetInfoTip(PLVSETINFOTIP pSetInfoTip);
	void CancelEditLabel(void);
	UInt MapIndexToID(Int nIndex) const;
	Int  MapIDToIndex(UInt uID) const;
#endif // (_WIN32_WINNT >= 0x0501)
#if (_WIN32_WINNT >= 0x0600)
	Int  HitTestEx(LPLVHITTESTINFO lpHitTestInfo) const;
	Int  HitTestEx(POINT& pt, PUInt puFlags) const;
	Int  SubItemHitTestEx(LPLVHITTESTINFO lpHitTestInfo) const;
#endif // (_WIN32_WINNT >= 0x0600)
	// single-selection only
	BOOL SelectItem(Int nIndex);

private:
	CXListView(const CXListView&);
	CXListView& operator=(const CXListView&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXTreeView : WC_TREEVIEW
class CXTreeView : public CXControl 
{
public :
	CXTreeView(void);
	virtual ~CXTreeView(void);

	virtual PCXStr GetXWndClass(void) OVERRIDE;

	UInt GetCount(void) const;
	UInt GetIndent(void) const;
	void SetIndent(UInt uIndent);
	HIMAGELIST GetImageList(Int nType = TVSIL_NORMAL) const;
	HIMAGELIST SetImageList(HIMAGELIST hImageList, Int nType = TVSIL_NORMAL);
	BOOL GetItem(LPTVITEM pItem) const;
	BOOL SetItem(LPTVITEM pItem);
	BOOL SetItem(HTREEITEM hItem, UInt uMask, PCXStr pszItem, Int nImage, Int nSelectedImage, UInt uState, UInt uStateMask, LPARAM lParam);
	BOOL GetItemText(HTREEITEM hItem, PXStr pszText, Int nLen) const;
	BOOL SetItemText(HTREEITEM hItem, PCXStr pszItem);
	BOOL GetItemImage(HTREEITEM hItem, Int& nImage, Int& nSelectedImage) const;
	BOOL SetItemImage(HTREEITEM hItem, Int nImage, Int nSelectedImage);
	UInt GetItemState(HTREEITEM hItem, UInt uStateMask) const;
	BOOL SetItemState(HTREEITEM hItem, UInt uState, UInt uStateMask);
	uintptr_t GetItemData(HTREEITEM hItem) const;
	BOOL SetItemData(HTREEITEM hItem, uintptr_t utData);
	HWND GetEditControl(void) const;
	UInt GetVisibleCount(void) const;
	BOOL GetItemRect(HTREEITEM hItem, LPRECT lpRect, BOOL bTextOnly) const;
	BOOL ItemHasChildren(HTREEITEM hItem) const;
	HWND GetToolTips(void) const;
	HWND SetToolTips(HWND hWndTT);
	Int  GetISearchString(PXStr psz) const;
	// checkboxes only
	BOOL GetCheckState(HTREEITEM hItem) const;
	BOOL SetCheckState(HTREEITEM hItem, BOOL bCheck);
	COLORREF GetBkColor(void) const;
	COLORREF SetBkColor(COLORREF clr);
	COLORREF GetInsertMarkColor(void) const;
	COLORREF SetInsertMarkColor(COLORREF clr);
	Int  GetItemHeight(void) const;
	Int  SetItemHeight(Int nHeight);
	Int  GetScrollTime(void) const;
	Int  SetScrollTime(Int nScrollTime);
	COLORREF GetTextColor(void) const;
	COLORREF SetTextColor(COLORREF clr);
	BOOL GetUnicodeFormat(void) const;
	BOOL SetUnicodeFormat(BOOL bUnicode = TRUE);
	COLORREF GetLineColor(void) const;
	COLORREF SetLineColor(COLORREF clrNew);
	BOOL GetItem(LPTVITEMEX pItem) const;
	BOOL SetItem(LPTVITEMEX pItem);
	ULong GetExtendedStyle(void) const;
	ULong SetExtendedStyle(ULong ulStyle, ULong ulMask);
#if (_WIN32_WINNT >= 0x0600)
	BOOL SetAutoScrollInfo(UInt uPixPerSec, UInt uUpdateTime);
	ULong GetSelectedCount(void) const;
	BOOL GetItemPartRect(HTREEITEM hItem, TVITEMPART partID, LPRECT lpRect) const;
#endif // (_WIN32_WINNT >= 0x0600)
	// Operations
	HTREEITEM InsertItem(LPTVINSERTSTRUCT lpInsertStruct);
	HTREEITEM InsertItem(PCXStr pszItem, Int nImage, Int nSelectedImage, HTREEITEM hParent, HTREEITEM hInsertAfter);
	HTREEITEM InsertItem(PCXStr pszItem, HTREEITEM hParent, HTREEITEM hInsertAfter);
	HTREEITEM InsertItem(UInt uMask, PCXStr pszItem, Int nImage, Int nSelectedImage, UInt uState, 
		                 UInt uStateMask, LPARAM lParam, HTREEITEM hParent, HTREEITEM hInsertAfter);
	BOOL DeleteItem(HTREEITEM hItem);
	BOOL DeleteAllItems(void);
	BOOL Expand(HTREEITEM hItem, UInt nCode = TVE_EXPAND);
	HTREEITEM GetNextItem(HTREEITEM hItem, UInt uCode) const;
	HTREEITEM GetChildItem(HTREEITEM hItem) const;
	HTREEITEM GetNextSiblingItem(HTREEITEM hItem) const;
	HTREEITEM GetPrevSiblingItem(HTREEITEM hItem) const;
	HTREEITEM GetParentItem(HTREEITEM hItem) const;
	HTREEITEM GetFirstVisibleItem(void) const;
	HTREEITEM GetNextVisibleItem(HTREEITEM hItem) const;
	HTREEITEM GetPrevVisibleItem(HTREEITEM hItem) const;
	HTREEITEM GetSelectedItem(void) const;
	HTREEITEM GetDropHilightItem(void) const;
	HTREEITEM GetRootItem(void) const;
	HTREEITEM GetLastVisibleItem(void) const;
	HTREEITEM GetNextSelectedItem(void) const;
	BOOL Select(HTREEITEM hItem, UInt uCode);
	BOOL SelectItem(HTREEITEM hItem);
	BOOL SelectDropTarget(HTREEITEM hItem);
	BOOL SelectSetFirstVisible(HTREEITEM hItem);
	HWND EditLabel(HTREEITEM hItem);
	BOOL EndEditLabelNow(BOOL bCancel);
	HTREEITEM HitTest(TVHITTESTINFO* pHitTestInfo) const;
	HTREEITEM HitTest(POINT& pt, PUInt puFlags) const;
	BOOL SortChildren(HTREEITEM hItem, BOOL bRecurse = FALSE);
	BOOL EnsureVisible(HTREEITEM hItem);
	BOOL SortChildrenCB(LPTVSORTCB pSort, BOOL bRecurse = FALSE);
	HIMAGELIST RemoveImageList(Int nImageList);
	HIMAGELIST CreateDragImage(HTREEITEM hItem);
	BOOL SetInsertMark(HTREEITEM hTreeItem, BOOL bAfter);
	BOOL RemoveInsertMark(void);
#if (_WIN32_WINNT >= 0x0501)
	HTREEITEM MapAccIDToHTREEITEM(UInt uID) const;
	UInt MapHTREEITEMToAccID(HTREEITEM hTreeItem) const;
#endif // (_WIN32_WINNT >= 0x0501)
#if (_WIN32_WINNT >= 0x0600)
	void ShowInfoTip(HTREEITEM hItem);
#endif // (_WIN32_WINNT >= 0x0600)
private:
	CXTreeView(const CXTreeView&);
	CXTreeView& operator=(const CXTreeView&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXTrackBar : TRACKBAR_CLASS
class CXTrackBar : public CXControl
{
public:
	CXTrackBar(void);
	virtual ~CXTrackBar(void);

	virtual PCXStr GetXWndClass(void) OVERRIDE;

	Int  GetLineSize(void) const;
	Int  SetLineSize(Int nSize);
	Int  GetPageSize(void) const;
	Int  SetPageSize(Int nSize);
	Int  GetRangeMin(void) const;
	void SetRangeMin(Int nMin, BOOL bRedraw = FALSE);
	Int  GetRangeMax(void) const;
	void SetRangeMax(Int nMax, BOOL bRedraw = FALSE);
	void GetRange(Int& nMin, Int& nMax) const;
	void SetRange(Int nMin, Int nMax, BOOL bRedraw = TRUE);
	Int  GetSelStart(void) const;
	void SetSelStart(Int nMin);
	Int  GetSelEnd(void) const;
	void SetSelEnd(Int nMax);
	void GetSelection(Int& nMin, Int& nMax) const;
	void SetSelection(Int nMin, Int nMax);
	void GetChannelRect(LPRECT lprc) const;
	void GetThumbRect(LPRECT lprc) const;
	Int  GetPos(void) const;
	void SetPos(Int nPos);
	UInt GetNumTics(void) const;
	PULong GetTicArray(void) const;
	Int  GetTic(Int nTic) const;
	BOOL SetTic(Int nTic);
	Int  GetTicPos(Int nTic) const;
	void SetTicFreq(Int nFreq);
	Int  GetThumbLength(void) const;
	void SetThumbLength(Int nLength);
	void SetSel(Int nStart, Int nEnd, BOOL bRedraw = TRUE);
	HWND GetBuddy(BOOL bLeft = TRUE) const;
	HWND SetBuddy(HWND hWndBuddy, BOOL bLeft = TRUE);
	HWND GetToolTips(void) const;
	void SetToolTips(HWND hWndTT);
	Int  SetTipSide(Int nSide);
	BOOL GetUnicodeFormat(void) const;
	BOOL SetUnicodeFormat(BOOL bUnicode = TRUE);
	// Operations
	void ClearSel(BOOL bRedraw = FALSE);
	void VerifyPos(void);
	void ClearTics(BOOL bRedraw = FALSE);
private:
	CXTrackBar(const CXTrackBar&);
	CXTrackBar& operator=(const CXTrackBar&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXUpDown : UPDOWN_CLASS
class CXUpDown : public CXControl
{
public:
	CXUpDown(void);
	virtual ~CXUpDown(void);

	virtual PCXStr GetXWndClass(void) OVERRIDE;

	UInt GetAccel(Int nAccel, UDACCEL* pAccel) const;
	BOOL SetAccel(Int nAccel, UDACCEL* pAccel);
	UInt GetBase(void) const;
	Int  SetBase(Int nBase);
	HWND GetBuddy(void) const;
	HWND SetBuddy(HWND hWndBuddy);
	Int  GetPos(LPBOOL lpbError = nullptr) const;
	Int  SetPos(Int nPos);
	ULong GetRange(void) const;
	void GetRange(Int& nLower, Int& nUpper) const;
	void SetRange(Int nLower, Int nUpper);
	void SetRange32(Int nLower, Int nUpper);
	void GetRange32(Int& nLower, Int& nUpper) const;
	BOOL GetUnicodeFormat(void) const;
	BOOL SetUnicodeFormat(BOOL bUnicode = TRUE);
	Int  GetPos32(LPBOOL lpbError = nullptr) const;
	Int  SetPos32(Int nPos);
private:
	CXUpDown(const CXUpDown&);
	CXUpDown& operator=(const CXUpDown&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXProgressBar : PROGRESS_CLASS
class CXProgressBar : public CXControl
{
public:
	CXProgressBar(void);
	virtual ~CXProgressBar(void);

	virtual PCXStr GetXWndClass(void) OVERRIDE;

	ULong SetRange(Int nLower, Int nUpper);
	Int  SetPos(Int nPos);
	Int  OffsetPos(Int nPos);
	Int  SetStep(Int nStep);
	UInt GetPos(void) const;
	void GetRange(PPBRANGE pPBRange) const;
	void GetRange(Int& nLower, Int& nUpper) const;
	Int  GetRangeLimit(BOOL bLowLimit) const;
	ULong SetRange32(Int nMin, Int nMax);
	COLORREF SetBarColor(COLORREF clr);
	COLORREF SetBkColor(COLORREF clr);
#if (_WIN32_WINNT >= 0x0501) && defined(PBM_SETMARQUEE)
	BOOL SetMarquee(BOOL bMarquee, UInt uUpdateTime = 0U);
#endif // (_WIN32_WINNT >= 0x0501) && defined(PBM_SETMARQUEE)
#if (_WIN32_WINNT >= 0x0600)
	Int  GetStep(void) const;
	COLORREF GetBkColor(void) const;
	COLORREF GetBarColor(void) const;
	Int  GetState(void) const;
	Int  SetState(Int nState);
#endif // (_WIN32_WINNT >= 0x0600)
	// Operations
	Int  StepIt(void);
private:
	CXProgressBar(const CXProgressBar&);
	CXProgressBar& operator=(const CXProgressBar&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXLink : WC_LINK
#if (_WIN32_WINNT >= 0x0501)

class CXLink : public CXControl
{
public:
	CXLink(void);
	virtual ~CXLink(void);

	virtual PCXStr GetXWndClass(void) OVERRIDE;

	Int  GetIdealHeight(Int nMaxWidth = 0) const;
	BOOL GetItem(PLITEM pLItem) const;
	BOOL SetItem(PLITEM pLItem);
	// Vista only
	Int  GetIdealSize(SIZE& size, Int nMaxWidth = 0) const;
	// Operations
	BOOL HitTest(PLHITTESTINFO pLHitTestInfo) const;
private:
	CXLink(const CXLink&);
	CXLink& operator=(const CXLink&);
};
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXRichEdit : RICHEDIT_CLASS or MSFTEDIT_CLASS
class CXRichEdit : public CXControl
{
public:
	enum XRICHEDIT_PARAM
	{
		INIT_VERSION2 = 0,
		INIT_VERSION4,

		FONTSIZE_MIN  = -1636,
		FONTSIZE_MAX  = 1638,
		ZOOM_MIN      = 0, // no zoom
		ZOOM_MAX      = 64,// min 1/64, max 64
	};
public:
	static void InitRichEdit(Int nVersion = INIT_VERSION2);
public:
	CXRichEdit(PCXStr pszClassName = RICHEDIT_CLASS); // MSFTEDIT_CLASS
	virtual ~CXRichEdit(void);

	virtual PCXStr GetXWndClass(void) OVERRIDE;
	virtual void   PreSubClass(void) OVERRIDE;

	Int   GetLineCount(void) const;
	BOOL  GetModify(void) const;
	void  SetModify(BOOL bModified = TRUE);
	void  GetRect(LPRECT lpRect) const;
	ULong GetOptions(void) const;
	ULong SetOptions(UShort usOperation, ULong ulOptions);
	// NOTE: first word in pszBuffer must contain the size of the buffer!
	Int   GetLine(Int nIndex, PXStr pszBuffer) const;
	Int   GetLine(Int nIndex, PXStr pszBuffer, Int nMaxLength) const;
	BOOL  CanUndo(void) const;
	BOOL  CanPaste(UInt nFormat = 0) const;
	void  GetSel(Long& lStartChar, Long& lEndChar) const;
	void  GetSel(CHARRANGE &clr) const;
	Int   SetSel(Long lStartChar, Long lEndChar);
	Int   SetSel(CHARRANGE &clr);
	Int   SetSelAll(void);
	Int   SetSelNone(void);
	ULong GetDefaultCharFormat(CHARFORMAT& cf) const;
	ULong GetSelectionCharFormat(CHARFORMAT& cf) const;
	ULong GetEventMask(void) const;
	Long  GetLimitText(void) const;
	ULong GetParaFormat(PARAFORMAT& pf) const;
	Long  GetSelText(PXStr pszBuff) const;
	UShort  GetSelectionType(void) const;
	COLORREF SetBackgroundColor(COLORREF clr);
	COLORREF SetBackgroundColor(void);   // sets to system background
	BOOL  SetCharFormat(CHARFORMAT& cf, UShort usFlags);
	BOOL  SetDefaultCharFormat(CHARFORMAT& cf);
	BOOL  SetSelectionCharFormat(CHARFORMAT& cf);
	BOOL  SetWordCharFormat(CHARFORMAT& cf);
	ULong SetEventMask(ULong ulEventMask);
	BOOL  SetOLECallback(IRichEditOleCallback* pCallback);
	BOOL  SetParaFormat(PARAFORMAT& pf);
	BOOL  SetTargetDevice(HDC hDC, Int nLineWidth);
	Int   GetTextLength(void) const;
	BOOL  SetReadOnly(BOOL bReadOnly = TRUE);
	Int   GetFirstVisibleLine(void) const;
	EDITWORDBREAKPROCEX GetWordBreakProcEx(void) const;
	EDITWORDBREAKPROCEX SetWordBreakProcEx(EDITWORDBREAKPROCEX pfnEditWordBreakProcEx);
	Int   GetTextRange(TEXTRANGE* pTextRange) const;
	Int   GetTextRange(Long lStartChar, Long lEndChar, PXStr pszText) const;
	ULong GetDefaultCharFormat(CHARFORMAT2& cf) const;
	BOOL  SetCharFormat(CHARFORMAT2& cf, UShort usFlags);
	BOOL  SetDefaultCharFormat(CHARFORMAT2& cf);
	ULong GetSelectionCharFormat(CHARFORMAT2& cf) const;
	BOOL  SetSelectionCharFormat(CHARFORMAT2& cf);
	BOOL  SetWordCharFormat(CHARFORMAT2& cf);
	ULong GetParaFormat(PARAFORMAT2& pf) const;
	BOOL  SetParaFormat(PARAFORMAT2& pf);
	TEXTMODE GetTextMode(void) const;
	BOOL  SetTextMode(TEXTMODE enumTextMode);
	UNDONAMEID GetUndoName(void) const;
	UNDONAMEID GetRedoName(void) const;
	BOOL  CanRedo(void) const;
	BOOL  GetAutoURLDetect(void) const;
	BOOL  SetAutoURLDetect(BOOL bAutoDetect = TRUE);
	UInt  SetUndoLimit(UInt uUndoLimit);
	void  SetPalette(HPALETTE hPalette);
	Int   GetTextEx(GETTEXTEX* pGetTextEx, PXStr pszText) const;
	Int   GetTextEx(PXStr pszText, Int nTextLen, ULong ulFlags = GT_DEFAULT, UInt uCodePage = CP_ACP, LPCSTR lpDefaultChar = nullptr, LPBOOL lpUsedDefChar = nullptr) const;
	Int   GetTextLengthEx(GETTEXTLENGTHEX* pGetTextLengthEx) const;
	Int   GetTextLengthEx(ULong ulFlags = GTL_DEFAULT, UInt uCodePage = CP_ACP) const;
	Int   SetTextEx(SETTEXTEX* pSetTextEx, PCXStr pszText);
	Int   SetTextEx(PCXStr pszText, ULong ulFlags = ST_DEFAULT, UInt uCodePage = CP_ACP);
	Int   GetEditStyle(void) const;
	Int   SetEditStyle(Int nStyle, Int nMask = -1);
	BOOL  SetFontSize(Int nFontSizeDelta);
	void  GetScrollPos(LPPOINT lpPoint) const;
	void  SetScrollPos(LPPOINT lpPoint);
	BOOL  GetZoom(Int& nNum, Int& nDen) const;
	BOOL  SetZoom(Int nNum, Int nDen);
	BOOL  SetZoomOff(void);
	// Operations
	void  LimitText(Long lChars = 0);
	Int   LineFromChar(Long nIndex) const;
	POINT PosFromChar(Long lChars) const;
	Int   CharFromPos(POINT pt) const;
	void  EmptyUndoBuffer(void);
	Int   LineIndex(Int nLine = -1) const;
	Int   LineLength(Int nLine = -1) const;
	BOOL  LineScroll(Int nLines, Int nChars = 0);
	void  ReplaceSel(PCXStr pszNewText, BOOL bCanUndo = FALSE);
	void  SetRect(LPCRECT lpRect);
	BOOL  DisplayBand(LPRECT pDisplayRect);
	Long  FindText(ULong ulFlags, FINDTEXT& ft) const;
	Long  FindText(ULong ulFlags, FINDTEXTEX& ft) const;
	Long  FormatRange(FORMATRANGE& fr, BOOL bDisplay = TRUE);
	Long  FormatRange(FORMATRANGE* pFormatRange, BOOL bDisplay = TRUE);
	void  HideSelection(BOOL bHide = TRUE, BOOL bChangeStyle = FALSE);
	void  PasteSpecial(UInt uClipFormat, ULong ulAspect = 0, HMETAFILE hMF = 0);
	void  RequestResize(void);
	Long  StreamIn(UInt uFormat, EDITSTREAM& es);
	Long  StreamOut(UInt uFormat, EDITSTREAM& es);
	ULong FindWordBreak(Int nCode, Long lStartChar);
	// Additional operations
	void  ScrollCaret(void);
	Int   InsertText(Long lInsertAfterChar, PCXStr pszText, BOOL bCanUndo = FALSE);
	Int   AppendText(PCXStr pszText, BOOL bCanUndo = FALSE);
	// Clipboard operations
	BOOL  Undo(void);
	void  Clear(void);
	void  Copy(void);
	void  Cut(void);
	void  Paste(void);
	BOOL  Redo(void);
	void  StopGroupTyping(void);
	void  ShowScrollBar(Int nBarType, BOOL bVisible = TRUE);
	BOOL  SetTabStops(Int nTabStops, PInt pnTabStops);
	BOOL  SetTabStops(void);
	BOOL  SetTabStops(const Int& nEachStop);
protected:
	virtual LRESULT OnGetDlgCode(WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	BEGIN_INLINE_MSG_MAP( CXRichEdit )
	// {{ --
		MSG_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
		MSG_XUIM_LAYOUT()
	// --}}
	END_CHAIN_MSG_MAP( CXControl )
private:
	CXRichEdit(const CXRichEdit&);
	CXRichEdit& operator=(const CXRichEdit&);
private:
	class CXRichEditOleCallback : public IRichEditOleCallback, public MObject
	{
	public:
		static CPCWStr StorageName;
	public :
		CXRichEditOleCallback(void);
		virtual ~CXRichEditOleCallback(void);
		// IUnknown
		virtual  HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid,  void** ppvObject) OVERRIDE;
		virtual  ULong   STDMETHODCALLTYPE AddRef(void) OVERRIDE;
		virtual  ULong   STDMETHODCALLTYPE Release(void) OVERRIDE;
		// IRichEditOleCallback
		virtual  HRESULT STDMETHODCALLTYPE GetNewStorage(LPSTORAGE* lplpstg) OVERRIDE;
		virtual  HRESULT STDMETHODCALLTYPE GetInPlaceContext(LPOLEINPLACEFRAME FAR* lplpFrame,
		                                                     LPOLEINPLACEUIWINDOW FAR* lplpDoc, 
															 LPOLEINPLACEFRAMEINFO lpFrameInfo) OVERRIDE;
		virtual  HRESULT STDMETHODCALLTYPE ShowContainerUI(BOOL bShow) OVERRIDE;
		virtual  HRESULT STDMETHODCALLTYPE QueryInsertObject(LPCLSID lpclsid, LPSTORAGE lpstg, Long lCP) OVERRIDE;
		virtual  HRESULT STDMETHODCALLTYPE DeleteObject(LPOLEOBJECT lpoleobj) OVERRIDE;
		virtual  HRESULT STDMETHODCALLTYPE QueryAcceptData(LPDATAOBJECT lpdataobj, 
		                                                   CLIPFORMAT FAR* lpcfFormat, ULong ulReco, 
														   BOOL bReally, HGLOBAL hMetaPict) OVERRIDE;
		virtual  HRESULT STDMETHODCALLTYPE ContextSensitiveHelp(BOOL bEnterMode) OVERRIDE;
		virtual  HRESULT STDMETHODCALLTYPE GetClipboardData(CHARRANGE FAR* lpchrg, ULong ulReco,
		                                                    LPDATAOBJECT FAR* lplpdataobj) OVERRIDE;
		virtual  HRESULT STDMETHODCALLTYPE GetDragDropEffect(BOOL bDrag, ULong ulKeyState, PULong pulEffect) OVERRIDE;
		virtual  HRESULT STDMETHODCALLTYPE GetContextMenu(UShort usSeltyp, LPOLEOBJECT lpoleobj, 
		                                                  CHARRANGE FAR* lpchrg, HMENU FAR* lphmenu) OVERRIDE;
	private:
		CXRichEditOleCallback(const CXRichEditOleCallback&);
		CXRichEditOleCallback& operator=(const CXRichEditOleCallback&);
	private:
		Long       m_lRefCount;
		Long       m_lNumStorages;
		IStorage*  m_pStorage;
	};
private:
	bool                    m_bCallBackSet;
	CXRichEditOleCallback   m_OleCallBack;
	CStringKey              m_strRichEditClass;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXToolBar : TOOLBARCLASSNAME
// Style : XUIS_TOOLBAR or XUIS_TOOLBAR_FLAT, within rebar used XUIS_TOOLBAR_PANE
class CXToolBar : public CXControl
{
public:
	typedef struct tagXTOOLBAR_DATA
	{
		UShort usVersion;
		UShort usWidth;
		UShort usHeight;
		UShort usItemCount;
	}XTOOLBAR_DATA, *PXTOOLBAR_DATA;
public:
	CXToolBar(void);
	virtual ~CXToolBar(void);

	virtual PCXStr GetXWndClass(void) OVERRIDE;

	bool  Load(Int nID, bool bLoadBitmap = true, HMODULE hResInst = nullptr);
	// the bitmap color bits lest than 16Bits, you should add bitmap mask color 
	// if greater than 16Bits, uFlag =
	//  ILC_COLOR16
	//	    Use a 16-bit (32/64k-color) DIB section.
	//	ILC_COLOR24
	//	    Use a 24-bit DIB section.
	//	ILC_COLOR32
	//      Use a 32-bit DIB section.
	bool  LoadEx(Int nWidth, Int nID, Int nHot = 0, Int nDisabled = 0,
	             COLORREF clrMask = 0, UInt uFlag = ILC_COLOR32, HMODULE hResInst = nullptr);

	bool  LoadImg(Int nWidth, Int nType, Int nID, COLORREF clrMask, UInt uFlag, HMODULE hResInst = nullptr);

	BOOL  IsButtonEnabled(Int nID) const;
	BOOL  IsButtonChecked(Int nID) const;
	BOOL  IsButtonPressed(Int nID) const;
	BOOL  IsButtonHidden(Int nID) const;
	BOOL  IsButtonIndeterminate(Int nID) const;

	Int   GetState(Int nID) const;
	BOOL  SetState(Int nID, Int nState);
	BOOL  GetButton(Int nIndex, LPTBBUTTON lpButton) const;
	Int   GetButtonCount(void) const;
	BOOL  GetItemRect(Int nIndex, LPRECT lpRect) const;
	void  SetButtonStructSize(Int nSize = sizeof(TBBUTTON));
	BOOL  SetButtonSize(SIZE& size);
	BOOL  SetButtonSize(Int ncx, Int ncy);
	BOOL  SetBitmapSize(SIZE& size);
	BOOL  SetBitmapSize(Int ncx, Int ncy);
	HWND  GetToolTips(void) const;
	void  SetToolTips(HWND hWndToolTip);
	void  SetNotifyWnd(HWND hWnd);
	Int   GetRows(void) const;
	void  SetRows(Int nRows, BOOL bLarger, LPRECT lpRect);
	BOOL  SetCmdID(Int nIndex, Int nID);
	ULong GetBitmapFlags(void) const;
	Int   GetBitmap(Int nID) const;
	Int   GetButtonText(Int nID, PXStr pszText) const;

	HIMAGELIST GetImageList(void) const;
	HIMAGELIST SetImageList(HIMAGELIST hImageList);
	HIMAGELIST GetDisabledImageList(void) const;
	HIMAGELIST SetDisabledImageList(HIMAGELIST hImageList);
	HIMAGELIST GetHotImageList(void) const;
	HIMAGELIST SetHotImageList(HIMAGELIST hImageList);

	ULong GetStyle(void) const;
	void  SetStyle(ULong ulStyle);
	ULong GetButtonSize(void) const;
	void  GetButtonSize(SIZE& size) const;
	BOOL  GetRect(Int nID, LPRECT lpRect) const;
	Int   GetTextRows(void) const;
	BOOL  SetButtonWidth(Int nMin, Int nMax);
	BOOL  SetIndent(Int nIndent);
	BOOL  SetMaxTextRows(Int nMaxTextRows);
	BOOL  GetAnchorHighlight(void) const;
	BOOL  SetAnchorHighlight(BOOL bEnable = TRUE);
	Int   GetButtonInfo(Int nID, LPTBBUTTONINFO lptbbi) const;
	BOOL  SetButtonInfo(Int nID, LPTBBUTTONINFO lptbbi);
	Int   GetHotItem(void) const;
	Int   SetHotItem(Int nItem);
	BOOL  IsButtonHighlighted(Int nID) const;
	ULong SetDrawTextFlags(ULong ulMask, ULong ulFlags);
	BOOL  GetColorScheme(LPCOLORSCHEME lpcs) const;
	void  SetColorScheme(LPCOLORSCHEME lpcs);
	ULong GetExtendedStyle(void) const;
	ULong SetExtendedStyle(ULong ulStyle);
	void  GetInsertMark(LPTBINSERTMARK lptbim) const;
	void  SetInsertMark(LPTBINSERTMARK lptbim);
	COLORREF GetInsertMarkColor(void) const;
	COLORREF SetInsertMarkColor(COLORREF clr);
	BOOL  GetMaxSize(LPSIZE lpSize) const;
	void  GetPadding(LPSIZE lpSizePadding) const;
	void  SetPadding(Int ncx, Int ncy, LPSIZE lpSizePadding = nullptr);
	BOOL  GetUnicodeFormat(void) const;
	BOOL  SetUnicodeFormat(BOOL bUnicode = TRUE);

	Int   GetString(Int nString, PXStr pszString, Int nMaxLen) const;
#if (_WIN32_WINNT >= 0x0501)
	void GetMetrics(LPTBMETRICS lptbm) const;
	void SetMetrics(LPTBMETRICS lptbm);
	void SetWindowTheme(PCWStr pszTheme);
#endif // (_WIN32_WINNT >= 0x0501)
#if (_WIN32_WINNT >= 0x0600)
	HIMAGELIST GetPressedImageList(Int nIndex = 0) const;
	HIMAGELIST SetPressedImageList(HIMAGELIST hImageList, Int nIndex = 0);
#endif // (_WIN32_WINNT >= 0x0600)
	BOOL  EnableButton(Int nID, BOOL bEnable = TRUE);
	BOOL  CheckButton(Int nID, BOOL bCheck = TRUE);
	BOOL  PressButton(Int nID, BOOL bPress = TRUE);
	BOOL  HideButton(Int nID, BOOL bHide = TRUE);
	BOOL  Indeterminate(Int nID, BOOL bIndeterminate = TRUE);
	Int   AddBitmap(Int nNumButtons, Int nBitmapID, HMODULE hResInst = nullptr);
	Int   AddBitmap(Int nNumButtons, HBITMAP hBitmap);
	BOOL  AddButtons(Int nNumButtons, LPTBBUTTON lpButtons);
	BOOL  InsertButton(Int nIndex, LPTBBUTTON lpButton);
	BOOL  DeleteButton(Int nIndex);
	Int   CommandToIndex(Int nID) const;
	void  SaveState(HKEY hKeyRoot, PCXStr pszSubKey, PCXStr pszValueName);
	void  RestoreState(HKEY hKeyRoot, PCXStr pszSubKey, PCXStr pszValueName);
	void  Customize(void);
	Int   AddString(Int nStringID, HMODULE hResInst = nullptr);
	Int   AddStrings(PCXStr pszStrings);
	void  AutoSize(void);
	BOOL  ChangeBitmap(Int nID, Int nBitmap);
	Int   LoadImages(Int nBitmapID, HMODULE hResInst = nullptr);
	Int   LoadStdImages(Int nBitmapID);
	BOOL  ReplaceBitmap(LPTBREPLACEBITMAP ptbrb);
	Int   HitTest(LPPOINT lpPoint);
	BOOL  InsertMarkHitTest(LPPOINT lpPoint, LPTBINSERTMARK lptbim);
	BOOL  InsertMarkHitTest(Int x, Int y, LPTBINSERTMARK lptbim);
	BOOL  MapAccelerator(XChar cAccel, Int& nID) const;
	BOOL  MarkButton(Int nID, BOOL bHighlight = TRUE);
	BOOL  MoveButton(Int nOldPos, Int nNewPos);

	void  SetDropDowmBtn(Int nID, HMENU hMenu);
	void  DeleteAllButtons(void);
protected:
	void  OnToolTip(LPTOOLTIPTEXT lptt, HMODULE hResInst = nullptr);

	virtual LRESULT OnNotify(Int nID, LPNMHDR pHead, BOOL& bHandled) OVERRIDE;
	virtual LRESULT OnXLayout(WPARAM wParam, LPARAM lParam, BOOL& bHandled) OVERRIDE;

	BEGIN_INLINE_MSG_MAP( CXToolBar )
	// {{ --
		MSG_WM_NOTIFY()
		MSG_XUIM_LAYOUT()
	// --}}
	END_CHAIN_MSG_MAP( CXControl )
private:
	CXToolBar(const CXToolBar&);
	CXToolBar& operator=(const CXToolBar&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXMenuBar : MENUBARCLASSNAME
// Style : within rebar used XUIS_MENUBAR_PANE
class CXMenuBar : public CXControl  
{
public:
	CXMenuBar(void);
	virtual ~CXMenuBar(void); 

	virtual PCXStr GetXWndClass(void) OVERRIDE;

	bool  CreateMenu(void);
	void  SetMenuBarEmpty(void);
	BOOL  EnableMenuBarItem(Int nIndex, bool bEnable = true);
	BOOL  AppendMenuBarItem(UInt uFlags, uintptr_t utIDNewItem, PCXStr pszNewItem);
	BOOL  InsertMenuBarItem(Int nIndex, LPCMENUITEMINFO lpmii);
	BOOL  RemoveMenuBarItem(Int nIndex);
	BOOL  GetMenuBarItemString(Int nIndex, PXStr pszBuffer, Int nBufferSize);
	BOOL  SetMenuBarItemString(Int nIndex, PXStr pszBuffer);

	HMENU GetMenuBarSubMenu(Int nIndex);
	BOOL  SetMenuBarSubMenu(Int nIndex, HMENU hSubMenu);
	BOOL  GetMenuInfo(LPMENUINFO lpMenuInfo) const;
	BOOL  SetMenuInfo(LPCMENUINFO lpMenuInfo);
	Int   GetMenuBarItemCount(void) const;
	BOOL  GetMenuBarItemRect(Int nIndex, LPRECT lpRect);
	Int   MenuBarItemFromPoint(LPPOINT lppt); // screen coordinates
	void  HiliteMenuBarItem(Int nIndex);      // -1 unhilite 

	bool  LoadMenuBar(UInt uMenuID, HMODULE hResInst = nullptr);
	bool  LoadMenuBar(HMENU hMenu);
	bool  TrackPopup(void);

	virtual bool PreTranslateMessage(LPMSG pMsg) OVERRIDE;
protected :
	virtual LRESULT OnXNotify(WPARAM wParam, LPARAM lParam, BOOL& bHandled) OVERRIDE;
	LRESULT OnLButtonDown(WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	BEGIN_INLINE_MSG_MAP( CXMenuBar )
	// {{ --
		MSG_XUIM_NOTIFY()
		MSG_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
	// --}}
	END_CHAIN_MSG_MAP( CXControl )

private:
	CXMenuBar(const CXMenuBar&);
	CXMenuBar& operator=(const CXMenuBar&);

	bool OnMenuInput(PMSG pMsg);
	void OnMenuSelect(WPARAM wParam, LPARAM lParam, HMODULE hResInst = nullptr);
private:
	static LRESULT CALLBACK MenuInputFilter(Int nCode, WPARAM wParam, LPARAM lParam);
private:
	static CXMenuBar*   ms_pXMenuBar;
	static HHOOK        ms_hMenuHook;
private:
	HMENU               m_hMenu;
	Int	                m_nItem;
	Int	                m_nPressed;
	Int                 m_nRows;
	POINT               m_ptMouse;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXStatusBar : STATUSCLASSNAME
// Style : XUIS_STATUSBAR
class CXStatusBar : public CXControl
{
public :
	CXStatusBar(void);
	virtual ~CXStatusBar(void);

	virtual PCXStr GetXWndClass(void) OVERRIDE;

	Int   GetParts(Int nParts, PInt pnParts) const;
	BOOL  SetParts(Int nParts, PInt pnWidths);
	Int   GetTextLength(Int nPane, PInt pnType = nullptr) const;
	Int   GetText(Int nPane, PXStr pszText, PInt pnType = nullptr) const;
	BOOL  SetText(Int nPane, PCXStr pszText, Int nType = 0);
	BOOL  GetRect(Int nPane, LPRECT lpRect) const;
	BOOL  GetBorders(PInt pnBorders) const;
	BOOL  GetBorders(Int& nHorz, Int& nVert, Int& nSpacing) const;
	void  SetMinHeight(Int nMin);
	BOOL  SetSimple(BOOL bSimple = TRUE);
	BOOL  IsSimple(void) const;
	BOOL  GetUnicodeFormat(void) const;
	BOOL  SetUnicodeFormat(BOOL bUnicode = TRUE);
	void  GetTipText(Int nPane, PXStr pszText, Int nSize) const;
	void  SetTipText(Int nPane, PCXStr pszText);
	HICON GetIcon(Int nPane) const;
	BOOL  SetIcon(Int nPane, HICON hIcon);	
	COLORREF SetBkColor(COLORREF clrBk);

protected :
	// default : bottom; override this to special layout
	virtual LRESULT OnXLayout(WPARAM wParam, LPARAM lParam, BOOL& bHandled) OVERRIDE;

	BEGIN_INLINE_MSG_MAP( CXStatusBar )
	// {{ --
		MSG_XUIM_LAYOUT()
	// --}}
	END_CHAIN_MSG_MAP( CXControl )
private:
	CXStatusBar(const CXStatusBar&);
	CXStatusBar& operator=(const CXStatusBar&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXReBar : REBARCLASSNAME
// Style : XUIS_REBAR or XUIS_REBAR_NOBORDER
// CXReBar used REBARBANDINFO's cxMinChild, cyMinChild and lParam
// cxMinChild : must be 0, for menubar wrapper
// cyMinChild : the actual height
// lParam     : | -- 2 BYTE -- | -- 2 BYTE -- |
//                     |              | the init cyMinChild value
//                     | menubar row count
class CXReBar : public CXControl  
{
public:
	// SetBarInfo *** after create
	enum XREBAR_BAND
	{
		XREBARB_MIN           = 2, // rebar band min height
		XREBARB_MENU          = 24,
		XREBARB_TOOLBAR       = 28,
		XREBARB_TOOLBAR_LARGE = 56,
	};
public:
	CXReBar(void);
	virtual ~CXReBar(void);

	virtual PCXStr GetXWndClass(void) OVERRIDE;

	UInt GetBandCount(void) const;
	BOOL GetBandInfo(Int nBand, LPREBARBANDINFO lprbbi) const;
	BOOL SetBandInfo(Int nBand, LPREBARBANDINFO lprbbi);
	BOOL GetBarInfo(LPREBARINFO lprbi) const;
	BOOL SetBarInfo(LPREBARINFO lprbi);
	UInt GetRowCount(void) const;
	UInt GetRowHeight(Int nBand) const;

	COLORREF GetTextColor(void) const;
	COLORREF SetTextColor(COLORREF clr);
	COLORREF GetBkColor(void) const;
	COLORREF SetBkColor(COLORREF clr);

	UInt GetBarHeight(void) const;
	BOOL GetRect(Int nBand, LPRECT lpRect) const;
	HWND GetToolTips(void) const;
	void SetToolTips(HWND hwndToolTip);
	void GetBandBorders(Int nBand, LPRECT lpRect) const;
	BOOL GetColorScheme(LPCOLORSCHEME lpColorScheme) const;
	void SetColorScheme(LPCOLORSCHEME lpColorScheme);

	HPALETTE GetPalette(void) const;
	HPALETTE SetPalette(HPALETTE hPalette);

	BOOL GetUnicodeFormat(void) const;
	BOOL SetUnicodeFormat(BOOL bUnicode = TRUE);
	BOOL InsertBand(Int nBand, LPREBARBANDINFO lprbbi);
	BOOL DeleteBand(Int nBand);
	HWND SetNotifyWnd(HWND hWnd);
	void BeginDrag(Int nBand, ULong ulPos);
	void BeginDrag(Int nBand, Int nxPos, Int nyPos);
	void EndDrag(void);
	void DragMove(ULong ulPos);
	void DragMove(Int xPos, Int yPos);
	void MaximizeBand(Int nBand);
	void MinimizeBand(Int nBand);
	BOOL SizeToRect(LPRECT lpRect);
	Int  IdToIndex(UInt uBandID) const;
	Int  HitTest(LPRBHITTESTINFO lprbht) const;
	BOOL ShowBand(Int nBand, BOOL bShow = TRUE);
	BOOL MoveBand(Int nBand, Int nNewPos);
	void PushChevron(Int nBand, LPARAM lAppValue);

	void LockBands(bool bLock = true);
	BOOL InsertBand(HWND hChildWnd,
	                Int  nIndex,
	                Int  nMinHeight = XREBARB_MENU,
	                bool bBreak = false,
	                UInt uMask = RBBIM_STYLE | RBBIM_CHILD | RBBIM_CHILDSIZE | RBBIM_LPARAM | RBBIM_ID);

protected:
	void SetBandLayout(Int nBand);

	virtual LRESULT OnNotify(Int nID, LPNMHDR pHead, BOOL& bHandled) OVERRIDE;
	virtual LRESULT OnXLayout(WPARAM wParam, LPARAM lParam, BOOL& bHandled) OVERRIDE;

	LRESULT OnLBDoubleClick(WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	BEGIN_INLINE_MSG_MAP( CXReBar )
	// {{ --
		MSG_WM_NOTIFY()
		MSG_XUIM_LAYOUT()
		MSG_HANDLER(WM_LBUTTONDBLCLK, OnLBDoubleClick)
	// --}}
	END_CHAIN_MSG_MAP( CXControl )
private:
	CXReBar(const CXReBar&);
	CXReBar& operator=(const CXReBar&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "targetxuibasewnd.inl"

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#endif // __TARGET_XUI_BASE_WND_H__