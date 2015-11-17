///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : targetxuibase.h                                     ¡¡¡¡                              //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2005-02-02     version 0.0.0.1                                                             //
//   Update : 2015-03-16     version 0.0.5.5                                                             //
//   Detail : xui windows base                                                                           //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TARGET_XUI_BASE_H__
#define __TARGET_XUI_BASE_H__

#pragma once

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#include "tstring.h"
#include "targetxuiaux.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// IXWnd
class NOVTABLE IXWnd ABSTRACT : public MObject
{
	friend class CXUIManagerWindows;
public:
	enum XWND_TYPE
	{
		XWNDT_NONE       = 0x00000000,
		XWNDT_FRAME,
		XWNDT_VIEW,
		// control
		XWNDT_STATIC,
		XWNDT_BUTTON,
		XWNDT_EDIT,
		XWNDT_LISTBOX,
		XWNDT_COMBOBOX,
		XWNDT_COMBOBOXEX,
		XWNDT_SCROLLBAR,
		XWNDT_TAB,
		XWNDT_TOOLTIP,
		XWNDT_LISTVIEW,
		XWNDT_TREEVIEW,
		XWNDT_TRACKBAR,
		XWNDT_UPDOWN,
		XWNDT_PROGRESS,
		XWNDT_LINK,
		XWNDT_RICHEDIT,
		XWNDT_TOOLBAR,
		XWNDT_MENUBAR,
		XWNDT_STATUSBAR,
		XWNDT_REBAR,
		XWNDT_COMMONEND,
		// special windows
		// use XWNDT_COMMONEND + X

		XWNDT_WINDOW     = 0x01000000,
		XWNDT_DIALOG     = 0x02000000,
		XWNDT_CONTROL    = 0x04000000,
		XWNDT_FRAMEMODE  = 0x08000000,
		XWNDT_MODAL      = 0x10000000,
		XWNDT_SUBCLASS   = 0x20000000,
		XWNDT_UIKIT      = 0x40000000,
		XWNDT_TYPEMASK   = 0x00FFFFFF,
	};
public:
	IXWnd(Int nType = XWNDT_NONE);
	virtual ~IXWnd(void);

	WNDPROC  GetProc(void) const;
	operator HWND(void) const;
	bool     operator==(const IXWnd& xw) const;
	bool     operator!=(const IXWnd& xw) const;

	Int      GetType(void);
	void     SetFrameMode(void);

	bool     IsWindowType(void) const;
	bool     IsDialogType(void) const;
	bool     IsControlType(void) const;
	bool     IsExactWindowType(Int nType) const;
	bool     IsExactDialogType(Int nType) const;
	bool     IsExactControlType(Int nType) const;
	bool     IsFrameMode(void) const;
	bool     IsModal(void) const;
	bool     IsSubClass(void) const;
	bool     IsUIKit(void) const;

//---------------------------------------------------------------------------------------------------------
	// Windows function wrapper
	void     Attach(HWND hWndNew, IXWnd* pOwner = nullptr);
	HWND     Detach(void);

	intptr_t GetWindowLongPtr(Int nIndex) const;
	intptr_t SetWindowLongPtr(Int nIndex, intptr_t itNew);

	// Message Functions
	LRESULT  SendMessage(UInt uMsg, WPARAM wParam = 0, LPARAM lParam = 0);
	BOOL     PostMessage(UInt uMsg, WPARAM wParam = 0, LPARAM lParam = 0);
	BOOL     SendNotifyMessage(UInt uMsg, WPARAM wParam = 0, LPARAM lParam = 0);

	// Window Text Functions
	BOOL     SetWindowText(PCXStr pszString);
	Int      GetWindowText(PXStr pszStringBuf, Int nMaxCount) const;
	Int      GetWindowTextLength(void) const;

	// Font Functions
	void     SetFont(HFONT hFont, BOOL bRedraw = TRUE);
	HFONT    GetFont(void) const;

	// Menu Functions (non-child windows only)
	HMENU    GetMenu(void) const;
	BOOL     SetMenu(HMENU hMenu);
	BOOL     DrawMenuBar(void);
	HMENU    GetSystemMenu(BOOL bRevert) const;
	BOOL     HiliteMenuItem(HMENU hMenu, UInt uItemHilite, UInt uHilite);

	// Window Size and Position Functions
	BOOL     IsIconic(void) const;
	BOOL     IsZoomed(void) const;
	BOOL     MoveWindow(Int nx, Int ny, Int nWidth, Int nHeight, BOOL bRepaint = TRUE);
	BOOL     MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE);
	BOOL     SetWindowPos(HWND hWndInsertAfter, Int nx, Int ny, Int ncx, Int ncy, UInt uFlags);
	BOOL     SetWindowPos(HWND hWndInsertAfter, LPCRECT lpRect, UInt uFlags);
	UInt     ArrangeIconicWindows(void);
	BOOL     BringWindowToTop(void);
	BOOL     GetWindowRect(LPRECT lpRect) const;
	BOOL     GetClientRect(LPRECT lpRect) const;
	BOOL     GetWindowPlacement(WINDOWPLACEMENT* lpwndpl) const;
	BOOL     SetWindowPlacement(const WINDOWPLACEMENT* lpwndpl);

	// Coordinate Mapping Functions
	BOOL     ClientToScreen(LPPOINT lpPoint) const;
	BOOL     ClientToScreen(LPRECT lpRect) const;
	BOOL     ScreenToClient(LPPOINT lpPoint) const;
	BOOL     ScreenToClient(LPRECT lpRect) const;
	Int      MapWindowPoints(IXWnd* pWndTo, LPPOINT lpPoint, UInt uCount) const;
	Int      MapWindowPoints(IXWnd* pWndTo, LPRECT lpRect) const;

	// Update and Painting Functions
	HDC      BeginPaint(LPPAINTSTRUCT lpPaint);
	void     EndPaint(LPPAINTSTRUCT lpPaint);
	HDC      GetDC(void);
	HDC      GetWindowDC(void);
	Int      ReleaseDC(HDC hDC);
	void     Print(HDC hDC, ULong ulFlags) const;
	void     PrintClient(HDC hDC, ULong ulFlags) const;
	BOOL     UpdateWindow(void);
	void     SetRedraw(BOOL bRedraw = TRUE);
	BOOL     GetUpdateRect(LPRECT lpRect, BOOL bErase = FALSE);
	Int      GetUpdateRgn(HRGN hRgn, BOOL bErase = FALSE);
	BOOL     Invalidate(BOOL bErase = TRUE);
	BOOL     InvalidateRect(LPCRECT lpRect, BOOL bErase = TRUE);
	BOOL     ValidateRect(LPCRECT lpRect);
	void     InvalidateRgn(HRGN hRgn, BOOL bErase = TRUE);
	BOOL     ValidateRgn(HRGN hRgn);
	BOOL     ShowWindow(Int nCmdShow = SW_SHOW);
	BOOL     IsWindowVisible(void) const;
	BOOL     ShowOwnedPopups(BOOL bShow = TRUE);
	HDC      GetDCEx(HRGN hRgnClip, ULong ulFlags);
	BOOL     LockWindowUpdate(BOOL bLock = TRUE);
	BOOL     RedrawWindow(LPCRECT lpRectUpdate = nullptr, HRGN hRgnUpdate = nullptr, UInt uFlags = RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE);

	// Timer Functions
	uintptr_t SetTimer(uintptr_t utIDEvent, UInt uElapse, TIMERPROC pTimerFunc = nullptr);
	BOOL      KillTimer(uintptr_t utIDEvent);

	// Window State Functions
	BOOL     DestroyWindow(void);
	BOOL     IsWindowEnabled(void) const;
	BOOL     EnableWindow(BOOL bEnable = TRUE);
	HWND     SetActiveWindow(void);
	HWND     SetCapture(void);
	HWND     SetFocus(void);

	// Dialog-Box Item Functions
	BOOL     CheckDlgButton(Int nIDButton, UInt uCheck);
	BOOL     CheckRadioButton(Int nIDFirstButton, Int nIDLastButton, Int nIDCheckButton);
	Int      DlgDirList(PXStr pszPathSpec, Int nIDListBox, Int nIDStaticPath, UInt uFileType);
	Int      DlgDirListComboBox(PXStr pszPathSpec, Int nIDComboBox, Int nIDStaticPath, UInt uFileType);
	BOOL     DlgDirSelect(PXStr pszString, Int nCount, Int nIDListBox);
	BOOL     DlgDirSelectComboBox(PXStr pszString, Int nCount, Int nIDComboBox);
	UInt     GetDlgItemInt(Int nID, BOOL* pbTrans = nullptr, BOOL bSigned = TRUE) const;
	UInt     GetDlgItemText(Int nID, PXStr pszStr, Int nMaxCount) const;
	UInt     IsDlgButtonChecked(Int nIDButton) const;
	LRESULT  SendDlgItemMessage(Int nID, UInt uMsg, WPARAM wParam = 0, LPARAM lParam = 0);
	BOOL     SetDlgItemInt(Int nID, UInt uValue, BOOL bSigned = TRUE);
	BOOL     SetDlgItemText(Int nID, PCXStr pszString);
	BOOL     EndDialog(intptr_t itResult = 0);

	// Scrolling Functions
	Int      GetScrollPos(Int nBar) const;
	BOOL     GetScrollRange(Int nBar, PInt pnMinPos, PInt pnMaxPos) const;
	BOOL     ScrollWindow(Int nxAmount, Int nyAmount, LPCRECT lpRect = nullptr, LPCRECT lpClipRect = nullptr);
	Int      ScrollWindowEx(Int ndx, Int ndy, LPCRECT lpRectScroll, LPCRECT lpRectClip, HRGN hRgnUpdate, LPRECT lpRectUpdate, UInt uFlags);
	Int      ScrollWindowEx(Int ndx, Int ndy, UInt uFlags, LPCRECT lpRectScroll = nullptr, LPCRECT lpRectClip = nullptr, HRGN hRgnUpdate = nullptr, LPRECT lpRectUpdate = nullptr);
	Int      SetScrollPos(Int nBar, Int nPos, BOOL bRedraw = TRUE);
	BOOL     SetScrollRange(Int nBar, Int nMinPos, Int nMaxPos, BOOL bRedraw = TRUE);
	BOOL     ShowScrollBar(Int nBar, BOOL bShow = TRUE);
	BOOL     EnableScrollBar(UInt uFlags, UInt uArrowFlags = ESB_ENABLE_BOTH);

	// Window Access Functions
	HWND     ChildWindowFromPoint(POINT& point) const;
	HWND     ChildWindowFromPointEx(POINT& point, UInt uFlags) const;
	HWND     GetTopWindow(void) const;
	HWND     GetWindow(UInt uCmd) const;
	HWND     GetLastActivePopup(void) const;
	BOOL     IsChild(IXWnd* pWnd) const;
	HWND     GetParent(void) const;
	HWND     SetParent(IXWnd* pWndNewParent);

	// Window Tree Access
	Int      GetDlgCtrlID(void) const;
	Int      SetDlgCtrlID(Int nID);
	HWND     GetDlgItem(Int nID) const;

	// Alert Functions
	BOOL     FlashWindow(BOOL bInvert);
	Int      MessageBox(PCXStr pszText, PCXStr pszCaption = nullptr, UInt uType = MB_OK);

	// Clipboard Functions
	BOOL     ChangeClipboardChain(IXWnd* pWndNewNext);
	HWND     SetClipboardViewer(void);
	BOOL     OpenClipboard(void);

	// Caret Functions
	BOOL     CreateCaret(HBITMAP hBitmap);
	BOOL     CreateSolidCaret(Int nWidth, Int nHeight);
	BOOL     CreateGrayCaret(Int nWidth, Int nHeight);
	BOOL     HideCaret(void);
	BOOL     ShowCaret(void);

	// Icon Functions
	HICON    SetIcon(HICON hIcon, BOOL bBigIcon = TRUE);
	HICON    GetIcon(BOOL bBigIcon = TRUE) const;

	// Help Functions
	BOOL     WinHelp(PCXStr pszHelp, UInt uCmd = HELP_CONTEXT, ULong ulData = 0);
	BOOL     SetWindowContextHelpId(ULong ulContextHelpId);
	ULong    GetWindowContextHelpId(void) const;

	// Hot Key Functions
	Int      SetHotKey(UShort usVirtualKeyCode, UShort usModifiers);
	ULong    GetHotKey(void) const;

	// Misc. Operations
	BOOL     GetScrollInfo(Int nBar, LPSCROLLINFO lpScrollInfo);
	Int      SetScrollInfo(Int nBar, LPSCROLLINFO lpScrollInfo, BOOL bRedraw = TRUE);
	BOOL     IsDialogMessage(LPMSG lpMsg);
	void     NextDlgCtrl(void) const;
	void     PrevDlgCtrl(void) const;
	void     GotoDlgCtrl(IXWnd* pWndCtrl) const;
	Int      GetWindowRgn(HRGN hRgn);
	Int      SetWindowRgn(HRGN hRgn, BOOL bRedraw = FALSE);
	ULong    GetWindowThreadID(void);
	ULong    GetWindowProcessID(void);
	BOOL     IsWindow(void) const;
	BOOL     IsWindowUnicode(void) const;
	BOOL     ShowWindowAsync(Int nCmdShow);
	/////////////////////////////////

	virtual PCXStr  GetXWndClass(void) PURE_METHOD;
	virtual void    PreSubClass(void);
	virtual bool    OnIdle(void);
	// before translate&dispatch the message, 
	// frame call main window's PreTranslateMessage
	// any child window's PreTranslateMessage,
	// return true will end translate the message to any other child windows,
	// and prevent to translate&dispatch the message
	virtual bool    PreTranslateMessage(LPMSG pMsg);
	virtual void    SetEventHandler(class CEventHandler* pEventHandler);

public:
	static BOOL     AdjustWndRect(LPRECT lpRect, ULong ulStyle, BOOL bMenu = FALSE, ULong ulExStyle = 0);
	static bool     InitCommonCtrls(ULong ulFlags);
	static Int      MessageBoxParam(PCXStr pszText, PCXStr pszCaption = nullptr, UInt uType = MB_OK, HWND hWnd = nullptr, ...);
protected:
	virtual LRESULT WindowProc(UInt uMsg, WPARAM wParam, LPARAM lParam);
private:
	IXWnd(const IXWnd&);
	IXWnd& operator=(const IXWnd&);

	void   SetModal(bool bModal = true);
	void   SubClass(bool bEnable = true);
public : 
	HWND        m_hWnd;
	IXWnd*      m_pOwner;
private:
	WNDPROC	    m_pProc;
	intptr_t    m_itType;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "targetxuibase.inl"

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#endif // __TARGET_XUI_BASE_H__