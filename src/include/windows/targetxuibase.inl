///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : targetxuibase.inl                                                                     //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2005-02-02     version 0.0.0.1                                                             //
//   Update : 2015-03-16     version 0.0.5.5                                                             //
//   Detail : xui windows base                                                                           //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TARGET_XUI_BASE_INL__
#define __TARGET_XUI_BASE_INL__

#pragma once

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// IXWnd
INLINE IXWnd::IXWnd(Int nType)
: m_hWnd(nullptr)
, m_pOwner(nullptr)
, m_pProc(nullptr)
, m_itType(nType)
{ 
}

INLINE IXWnd::~IXWnd(void)
{ 
	assert(IsWindow() == FALSE);
}

INLINE IXWnd::IXWnd(const IXWnd&)
: m_hWnd(nullptr)
, m_pOwner(nullptr)
, m_pProc(nullptr)
, m_itType(XWNDT_NONE)
{
}

INLINE IXWnd& IXWnd::operator=(const IXWnd&)
{
	return (*this);
}

INLINE WNDPROC IXWnd::GetProc(void) const
{
	return m_pProc;
}

INLINE IXWnd::operator HWND(void) const
{ 
	return m_hWnd; 
}

INLINE bool IXWnd::operator==(const IXWnd& xw) const
{ 
	return (xw.m_hWnd == m_hWnd); 
}

INLINE bool IXWnd::operator!=(const IXWnd& xw) const
{ 
	return (xw.m_hWnd != m_hWnd);  
}

INLINE Int IXWnd::GetType(void)
{
	return (Int)m_itType;
}

INLINE void IXWnd::SetFrameMode(void)
{
	m_itType |= XWNDT_FRAMEMODE;
}

INLINE void IXWnd::SetModal(bool bModal)
{
	if (bModal)
	{
		m_itType |= XWNDT_MODAL;
	}
	else
	{
		m_itType &= (~XWNDT_MODAL);
	}
}

INLINE void IXWnd::SubClass(bool bEnable)
{
	if (bEnable)
	{
		m_itType |= XWNDT_SUBCLASS;
	}
	else
	{
		m_itType &= (~XWNDT_SUBCLASS);
	}
}

INLINE bool IXWnd::IsWindowType(void) const
{
	return ((m_itType & XWNDT_WINDOW) != 0);
}

INLINE bool IXWnd::IsDialogType(void) const
{
	if (m_itType & XWNDT_DIALOG)
	{
		assert((m_itType & XWNDT_WINDOW) == 0);
		return true;
	}
	return false;
}

INLINE bool IXWnd::IsControlType(void) const
{
	return ((m_itType & XWNDT_CONTROL) != 0);
}

INLINE bool IXWnd::IsExactWindowType(Int nType) const
{
	if (IsWindowType() == true)
	{
		return ((Int)(m_itType & XWNDT_TYPEMASK) == nType);
	}
	return false;
}

INLINE bool IXWnd::IsExactDialogType(Int nType) const
{
	if (IsDialogType() == true)
	{
		return ((Int)(m_itType & XWNDT_TYPEMASK) == nType);
	}
	return false;
}

INLINE bool IXWnd::IsExactControlType(Int nType) const
{
	if (IsControlType() == true)
	{
		return ((Int)(m_itType & XWNDT_TYPEMASK) == nType);
	}
	return false;
}

INLINE bool IXWnd::IsFrameMode(void) const
{
	return ((m_itType & XWNDT_FRAMEMODE) != 0);
}

INLINE bool IXWnd::IsModal(void) const
{
	return ((m_itType & XWNDT_MODAL) != 0);
}

INLINE bool IXWnd::IsSubClass(void) const
{
	return ((m_itType & XWNDT_SUBCLASS) != 0);
}

INLINE bool IXWnd::IsUIKit(void) const
{
	return ((m_itType & XWNDT_UIKIT) != 0);
}

/////////////////////////////////
INLINE void IXWnd::Attach(HWND hWndNew, IXWnd* pOwner)
{
	assert(m_hWnd == nullptr);
	assert((hWndNew != nullptr) || ::IsWindow(hWndNew));
	m_hWnd   = hWndNew;
	m_pOwner = pOwner;
}

INLINE HWND IXWnd::Detach(void) 
{
	HWND hRet = m_hWnd;
	m_hWnd    = nullptr;
	m_pOwner  = nullptr;
	return hRet;
}

INLINE intptr_t IXWnd::GetWindowLongPtr(Int nIndex) const
{
	assert(::IsWindow(m_hWnd));
	return ::GetWindowLongPtr(m_hWnd, nIndex);
}

INLINE intptr_t IXWnd::SetWindowLongPtr(Int nIndex, intptr_t itNew)
{
	assert(::IsWindow(m_hWnd));
	return ::SetWindowLongPtr(m_hWnd, nIndex, itNew);
}

// Message Functions
INLINE LRESULT IXWnd::SendMessage(UInt uMsg, WPARAM wParam, LPARAM lParam)
{
	assert(::IsWindow(m_hWnd));
	return ::SendMessage(m_hWnd, uMsg, wParam, lParam);
}

INLINE BOOL IXWnd::PostMessage(UInt uMsg, WPARAM wParam, LPARAM lParam)
{
	assert(::IsWindow(m_hWnd));
	return ::PostMessage(m_hWnd, uMsg, wParam, lParam);
}

INLINE BOOL IXWnd::SendNotifyMessage(UInt uMsg, WPARAM wParam, LPARAM lParam)
{
	assert(::IsWindow(m_hWnd));
	return ::SendNotifyMessage(m_hWnd, uMsg, wParam, lParam);
}

// Window Text Functions
INLINE BOOL IXWnd::SetWindowText(PCXStr pszString)
{
	assert(::IsWindow(m_hWnd));
	return ::SetWindowText(m_hWnd, pszString);
}

INLINE Int IXWnd::GetWindowText(PXStr pszStringBuf, Int nMaxCount) const
{
	assert(::IsWindow(m_hWnd));
	return ::GetWindowText(m_hWnd, pszStringBuf, nMaxCount);
}

INLINE Int IXWnd::GetWindowTextLength(void) const
{
	assert(::IsWindow(m_hWnd));
	return ::GetWindowTextLength(m_hWnd);
}

// Font Functions
INLINE void IXWnd::SetFont(HFONT hFont, BOOL bRedraw)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(bRedraw, 0));
}

INLINE HFONT IXWnd::GetFont(void) const
{
	assert(::IsWindow(m_hWnd));
	return (HFONT)::SendMessage(m_hWnd, WM_GETFONT, 0, 0);
}

// Menu Functions (non-child windows only)
INLINE HMENU IXWnd::GetMenu(void) const
{
	assert(::IsWindow(m_hWnd));
	return (HMENU)::GetMenu(m_hWnd);
}

INLINE BOOL IXWnd::SetMenu(HMENU hMenu)
{
	assert(::IsWindow(m_hWnd));
	return ::SetMenu(m_hWnd, hMenu);
}

INLINE BOOL IXWnd::DrawMenuBar(void)
{
	assert(::IsWindow(m_hWnd));
	return ::DrawMenuBar(m_hWnd);
}

INLINE HMENU IXWnd::GetSystemMenu(BOOL bRevert) const
{
	assert(::IsWindow(m_hWnd));
	return (HMENU)::GetSystemMenu(m_hWnd, bRevert);
}

INLINE BOOL IXWnd::HiliteMenuItem(HMENU hMenu, UInt uItemHilite, UInt uHilite)
{
	assert(::IsWindow(m_hWnd));
	return ::HiliteMenuItem(m_hWnd, hMenu, uItemHilite, uHilite);
}

// Window Size and Position Functions
INLINE BOOL IXWnd::IsIconic(void) const
{
	assert(::IsWindow(m_hWnd));
	return ::IsIconic(m_hWnd);
}

INLINE BOOL IXWnd::IsZoomed(void) const
{
	assert(::IsWindow(m_hWnd));
	return ::IsZoomed(m_hWnd);
}

INLINE BOOL IXWnd::MoveWindow(Int nx, Int ny, Int nWidth, Int nHeight, BOOL bRepaint)
{
	assert(::IsWindow(m_hWnd));
	return ::MoveWindow(m_hWnd, nx, ny, nWidth, nHeight, bRepaint);
}

INLINE BOOL IXWnd::MoveWindow(LPCRECT lpRect, BOOL bRepaint)
{
	assert(::IsWindow(m_hWnd));
	return ::MoveWindow(m_hWnd, lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top, bRepaint);
}

INLINE BOOL IXWnd::SetWindowPos(HWND hWndInsertAfter, Int nx, Int ny, Int ncx, Int ncy, UInt uFlags)
{
	assert(::IsWindow(m_hWnd));
	return ::SetWindowPos(m_hWnd, hWndInsertAfter, nx, ny, ncx, ncy, uFlags);
}

INLINE BOOL IXWnd::SetWindowPos(HWND hWndInsertAfter, LPCRECT lpRect, UInt uFlags)
{
	assert(::IsWindow(m_hWnd));
	return ::SetWindowPos(m_hWnd, hWndInsertAfter, 
						  lpRect->left, lpRect->top, 
						  lpRect->right - lpRect->left, 
						  lpRect->bottom - lpRect->top, uFlags);
}

INLINE UInt IXWnd::ArrangeIconicWindows(void)
{
	assert(::IsWindow(m_hWnd));
	return ::ArrangeIconicWindows(m_hWnd);
}

INLINE BOOL IXWnd::BringWindowToTop(void)
{
	assert(::IsWindow(m_hWnd));
	return ::BringWindowToTop(m_hWnd);
}

INLINE BOOL IXWnd::GetWindowRect(LPRECT lpRect) const
{
	assert(::IsWindow(m_hWnd));
	return ::GetWindowRect(m_hWnd, lpRect);
}

INLINE BOOL IXWnd::GetClientRect(LPRECT lpRect) const
{
	assert(::IsWindow(m_hWnd));
	return ::GetClientRect(m_hWnd, lpRect);
}

INLINE BOOL IXWnd::GetWindowPlacement(WINDOWPLACEMENT* lpwndpl) const
{
	assert(::IsWindow(m_hWnd));
	return ::GetWindowPlacement(m_hWnd, lpwndpl);
}

INLINE BOOL IXWnd::SetWindowPlacement(const WINDOWPLACEMENT* lpwndpl)
{
	assert(::IsWindow(m_hWnd));
	return ::SetWindowPlacement(m_hWnd, lpwndpl);
}

// Coordinate Mapping Functions
INLINE BOOL IXWnd::ClientToScreen(LPPOINT lpPoint) const
{
	assert(::IsWindow(m_hWnd));
	return ::ClientToScreen(m_hWnd, lpPoint);
}

INLINE BOOL IXWnd::ClientToScreen(LPRECT lpRect) const
{
	assert(::IsWindow(m_hWnd));
	if (::ClientToScreen(m_hWnd, (LPPOINT)lpRect) == FALSE)
	{
		return FALSE;
	}
	return ::ClientToScreen(m_hWnd, ((LPPOINT)lpRect)+1);
}

INLINE BOOL IXWnd::ScreenToClient(LPPOINT lpPoint) const
{
	assert(::IsWindow(m_hWnd));
	return ::ScreenToClient(m_hWnd, lpPoint);
}

INLINE BOOL IXWnd::ScreenToClient(LPRECT lpRect) const
{
	assert(::IsWindow(m_hWnd));
	if (::ScreenToClient(m_hWnd, (LPPOINT)lpRect) == FALSE)
	{
		return FALSE;
	}
	return ::ScreenToClient(m_hWnd, ((LPPOINT)lpRect)+1);
}

INLINE Int IXWnd::MapWindowPoints(IXWnd* pWndTo, LPPOINT lpPoint, UInt uCount) const
{
	assert(::IsWindow(m_hWnd));
	assert(pWndTo != nullptr);
	assert(::IsWindow(pWndTo->m_hWnd));
	return ::MapWindowPoints(m_hWnd, pWndTo->m_hWnd, lpPoint, uCount);
}

INLINE Int IXWnd::MapWindowPoints(IXWnd* pWndTo, LPRECT lpRect) const
{
	assert(::IsWindow(m_hWnd));
	assert(pWndTo != nullptr);
	assert(::IsWindow(pWndTo->m_hWnd));
	return ::MapWindowPoints(m_hWnd, pWndTo->m_hWnd, (LPPOINT)lpRect, 2);
}

// Update and Painting Functions
INLINE HDC IXWnd::BeginPaint(LPPAINTSTRUCT lpPaint)
{
	assert(::IsWindow(m_hWnd));
	return ::BeginPaint(m_hWnd, lpPaint);
}

INLINE void IXWnd::EndPaint(LPPAINTSTRUCT lpPaint)
{
	assert(::IsWindow(m_hWnd));
	::EndPaint(m_hWnd, lpPaint);
}

INLINE HDC IXWnd::GetDC(void)
{
	assert(::IsWindow(m_hWnd));
	return ::GetDC(m_hWnd);
}

INLINE HDC IXWnd::GetWindowDC(void)
{
	assert(::IsWindow(m_hWnd));
	return ::GetWindowDC(m_hWnd);
}

INLINE Int IXWnd::ReleaseDC(HDC hDC)
{
	assert(::IsWindow(m_hWnd));
	return ::ReleaseDC(m_hWnd, hDC);
}

INLINE void IXWnd::Print(HDC hDC, ULong ulFlags) const
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, WM_PRINT, (WPARAM)hDC, ulFlags);
}

INLINE void IXWnd::PrintClient(HDC hDC, ULong ulFlags) const
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, WM_PRINTCLIENT, (WPARAM)hDC, ulFlags);
}

INLINE BOOL IXWnd::UpdateWindow(void)
{
	assert(::IsWindow(m_hWnd));
	return ::UpdateWindow(m_hWnd);
}

INLINE void IXWnd::SetRedraw(BOOL bRedraw)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, WM_SETREDRAW, (WPARAM)bRedraw, 0);
}

INLINE BOOL IXWnd::GetUpdateRect(LPRECT lpRect, BOOL bErase)
{
	assert(::IsWindow(m_hWnd));
	return ::GetUpdateRect(m_hWnd, lpRect, bErase);
}

INLINE Int IXWnd::GetUpdateRgn(HRGN hRgn, BOOL bErase)
{
	assert(::IsWindow(m_hWnd));
	return ::GetUpdateRgn(m_hWnd, hRgn, bErase);
}

INLINE BOOL IXWnd::Invalidate(BOOL bErase)
{
	assert(::IsWindow(m_hWnd));
	return ::InvalidateRect(m_hWnd, nullptr, bErase);
}

INLINE BOOL IXWnd::InvalidateRect(LPCRECT lpRect, BOOL bErase)
{
	assert(::IsWindow(m_hWnd));
	return ::InvalidateRect(m_hWnd, lpRect, bErase);
}

INLINE BOOL IXWnd::ValidateRect(LPCRECT lpRect)
{
	assert(::IsWindow(m_hWnd));
	return ::ValidateRect(m_hWnd, lpRect);
}

INLINE void IXWnd::InvalidateRgn(HRGN hRgn, BOOL bErase)
{
	assert(::IsWindow(m_hWnd));
	::InvalidateRgn(m_hWnd, hRgn, bErase);
}

INLINE BOOL IXWnd::ValidateRgn(HRGN hRgn)
{
	assert(::IsWindow(m_hWnd));
	return ::ValidateRgn(m_hWnd, hRgn);
}

INLINE BOOL IXWnd::ShowWindow(Int nCmdShow)
{
	assert(::IsWindow(m_hWnd));
	return ::ShowWindow(m_hWnd, nCmdShow);
}

INLINE BOOL IXWnd::IsWindowVisible(void) const
{
	assert(::IsWindow(m_hWnd));
	return ::IsWindowVisible(m_hWnd);
}

INLINE BOOL IXWnd::ShowOwnedPopups(BOOL bShow)
{
	assert(::IsWindow(m_hWnd));
	return ::ShowOwnedPopups(m_hWnd, bShow);
}

INLINE HDC IXWnd::GetDCEx(HRGN hRgnClip, ULong ulFlags)
{
	assert(::IsWindow(m_hWnd));
	return ::GetDCEx(m_hWnd, hRgnClip, ulFlags);
}

INLINE BOOL IXWnd::LockWindowUpdate(BOOL bLock)
{
	assert(::IsWindow(m_hWnd));
	return ::LockWindowUpdate(bLock ? m_hWnd : nullptr);
}

INLINE BOOL IXWnd::RedrawWindow(LPCRECT lpRectUpdate, HRGN hRgnUpdate, UInt uFlags)
{
	assert(::IsWindow(m_hWnd));
	return ::RedrawWindow(m_hWnd, lpRectUpdate, hRgnUpdate, uFlags);
}

// Timer Functions
INLINE uintptr_t IXWnd::SetTimer(uintptr_t utIDEvent, UInt uElapse, TIMERPROC pTimerFunc)
{
	assert(::IsWindow(m_hWnd));
	return ::SetTimer(m_hWnd, utIDEvent, uElapse, pTimerFunc);
}

INLINE BOOL IXWnd::KillTimer(uintptr_t utIDEvent)
{
	assert(::IsWindow(m_hWnd));
	return ::KillTimer(m_hWnd, utIDEvent);
}

INLINE BOOL IXWnd::DestroyWindow(void)
{
	assert(::IsWindow(m_hWnd));
	return ::DestroyWindow(m_hWnd);
}

// Window State Functions
INLINE BOOL IXWnd::IsWindowEnabled(void) const
{
	assert(::IsWindow(m_hWnd));
	return ::IsWindowEnabled(m_hWnd);
}

INLINE BOOL IXWnd::EnableWindow(BOOL bEnable)
{
	assert(::IsWindow(m_hWnd));
	return ::EnableWindow(m_hWnd, bEnable);
}

INLINE HWND IXWnd::SetActiveWindow(void)
{
	assert(::IsWindow(m_hWnd));
	return ::SetActiveWindow(m_hWnd);
}

INLINE HWND IXWnd::SetCapture(void)
{
	assert(::IsWindow(m_hWnd));
	return ::SetCapture(m_hWnd);
}

INLINE HWND IXWnd::SetFocus(void)
{
	assert(::IsWindow(m_hWnd));
	return ::SetFocus(m_hWnd);
}

// Dialog-Box Item Functions
INLINE BOOL IXWnd::CheckDlgButton(Int nIDButton, UInt uCheck)
{
	assert(::IsWindow(m_hWnd));
	return ::CheckDlgButton(m_hWnd, nIDButton, uCheck);
}

INLINE BOOL IXWnd::CheckRadioButton(Int nIDFirstButton, Int nIDLastButton, Int nIDCheckButton)
{
	assert(::IsWindow(m_hWnd));
	return ::CheckRadioButton(m_hWnd, nIDFirstButton, nIDLastButton, nIDCheckButton);
}

INLINE Int IXWnd::DlgDirList(PXStr pszPathSpec, Int nIDListBox, Int nIDStaticPath, UInt uFileType)
{
	assert(::IsWindow(m_hWnd));
	return ::DlgDirList(m_hWnd, pszPathSpec, nIDListBox, nIDStaticPath, uFileType);
}

INLINE Int IXWnd::DlgDirListComboBox(PXStr pszPathSpec, Int nIDComboBox, Int nIDStaticPath, UInt uFileType)
{
	assert(::IsWindow(m_hWnd));
	return ::DlgDirListComboBox(m_hWnd, pszPathSpec, nIDComboBox, nIDStaticPath, uFileType);
}

INLINE BOOL IXWnd::DlgDirSelect(PXStr pszString, Int nCount, Int nIDListBox)
{
	assert(::IsWindow(m_hWnd));
	return ::DlgDirSelectEx(m_hWnd, pszString, nCount, nIDListBox);
}

INLINE BOOL IXWnd::DlgDirSelectComboBox(PXStr pszString, Int nCount, Int nIDComboBox)
{
	assert(::IsWindow(m_hWnd));
	return ::DlgDirSelectComboBoxEx(m_hWnd, pszString, nCount, nIDComboBox);
}

INLINE UInt IXWnd::GetDlgItemInt(Int nID, BOOL* pbTrans, BOOL bSigned) const
{
	assert(::IsWindow(m_hWnd));
	return ::GetDlgItemInt(m_hWnd, nID, pbTrans, bSigned);
}

INLINE UInt IXWnd::GetDlgItemText(Int nID, PXStr pszStr, Int nMaxCount) const
{
	assert(::IsWindow(m_hWnd));
	return ::GetDlgItemText(m_hWnd, nID, pszStr, nMaxCount);
}

INLINE UInt IXWnd::IsDlgButtonChecked(Int nIDButton) const
{
	assert(::IsWindow(m_hWnd));
	return ::IsDlgButtonChecked(m_hWnd, nIDButton);
}

INLINE LRESULT IXWnd::SendDlgItemMessage(Int nID, UInt uMsg, WPARAM wParam, LPARAM lParam)
{
	assert(::IsWindow(m_hWnd));
	return ::SendDlgItemMessage(m_hWnd, nID, uMsg, wParam, lParam);
}

INLINE BOOL IXWnd::SetDlgItemInt(Int nID, UInt uValue, BOOL bSigned)
{
	assert(::IsWindow(m_hWnd));
	return ::SetDlgItemInt(m_hWnd, nID, uValue, bSigned);
}

INLINE BOOL IXWnd::SetDlgItemText(Int nID, PCXStr pszString)
{
	assert(::IsWindow(m_hWnd));
	return ::SetDlgItemText(m_hWnd, nID, pszString);
}

INLINE BOOL IXWnd::EndDialog(intptr_t itResult)
{
	assert(::IsWindow(m_hWnd));
	return ::EndDialog(m_hWnd, itResult);
}

// Scrolling Functions
INLINE Int IXWnd::GetScrollPos(Int nBar) const
{
	assert(::IsWindow(m_hWnd));
	return ::GetScrollPos(m_hWnd, nBar);
}

INLINE BOOL IXWnd::GetScrollRange(Int nBar, PInt pnMinPos, PInt pnMaxPos) const
{
	assert(::IsWindow(m_hWnd));
	return ::GetScrollRange(m_hWnd, nBar, pnMinPos, pnMaxPos);
}

INLINE BOOL IXWnd::ScrollWindow(Int nxAmount, Int nyAmount, LPCRECT lpRect, LPCRECT lpClipRect)
{
	assert(::IsWindow(m_hWnd));
	return ::ScrollWindow(m_hWnd, nxAmount, nyAmount, lpRect, lpClipRect);
}

INLINE Int IXWnd::ScrollWindowEx(Int ndx, Int ndy, LPCRECT lpRectScroll, LPCRECT lpRectClip, HRGN hRgnUpdate, LPRECT lpRectUpdate, UInt uFlags)
{
	assert(::IsWindow(m_hWnd));
	return ::ScrollWindowEx(m_hWnd, ndx, ndy, lpRectScroll, lpRectClip, hRgnUpdate, lpRectUpdate, uFlags);
}

INLINE Int IXWnd::ScrollWindowEx(Int ndx, Int ndy, UInt uFlags, LPCRECT lpRectScroll, LPCRECT lpRectClip, HRGN hRgnUpdate, LPRECT lpRectUpdate)
{
	assert(::IsWindow(m_hWnd));
	return ::ScrollWindowEx(m_hWnd, ndx, ndy, lpRectScroll, lpRectClip, hRgnUpdate, lpRectUpdate, uFlags);
}

INLINE Int IXWnd::SetScrollPos(Int nBar, Int nPos, BOOL bRedraw)
{
	assert(::IsWindow(m_hWnd));
	return ::SetScrollPos(m_hWnd, nBar, nPos, bRedraw);
}

INLINE BOOL IXWnd::SetScrollRange(Int nBar, Int nMinPos, Int nMaxPos, BOOL bRedraw)
{
	assert(::IsWindow(m_hWnd));
	return ::SetScrollRange(m_hWnd, nBar, nMinPos, nMaxPos, bRedraw);
}

INLINE BOOL IXWnd::ShowScrollBar(Int nBar, BOOL bShow)
{
	assert(::IsWindow(m_hWnd));
	return ::ShowScrollBar(m_hWnd, nBar, bShow);
}

INLINE BOOL IXWnd::EnableScrollBar(UInt uFlags, UInt uArrowFlags)
{
	assert(::IsWindow(m_hWnd));
	return ::EnableScrollBar(m_hWnd, uFlags, uArrowFlags);
}

// Window Access Functions
INLINE HWND IXWnd::ChildWindowFromPoint(POINT& point) const
{
	assert(::IsWindow(m_hWnd));
	return ::ChildWindowFromPoint(m_hWnd, point);
}

INLINE HWND IXWnd::ChildWindowFromPointEx(POINT& point, UInt uFlags) const
{
	assert(::IsWindow(m_hWnd));
	return ::ChildWindowFromPointEx(m_hWnd, point, uFlags);
}

INLINE HWND IXWnd::GetTopWindow(void) const
{
	assert(::IsWindow(m_hWnd));
	return ::GetTopWindow(m_hWnd);
}

INLINE HWND IXWnd::GetWindow(UInt uCmd) const
{
	assert(::IsWindow(m_hWnd));
	return ::GetWindow(m_hWnd, uCmd);
}

INLINE HWND IXWnd::GetLastActivePopup(void) const
{
	assert(::IsWindow(m_hWnd));
	return ::GetLastActivePopup(m_hWnd);
}

INLINE BOOL IXWnd::IsChild(IXWnd* pWnd) const
{
	assert(::IsWindow(m_hWnd));
	assert(pWnd != nullptr);
	assert(::IsWindow(pWnd->m_hWnd));
	return ::IsChild(m_hWnd, pWnd->m_hWnd);
}

INLINE HWND IXWnd::GetParent(void) const
{
	assert(::IsWindow(m_hWnd));
	return ::GetParent(m_hWnd);
}

INLINE HWND IXWnd::SetParent(IXWnd* pWndNewParent)
{
	assert(::IsWindow(m_hWnd));
	assert(pWndNewParent != nullptr);
	assert(::IsWindow(pWndNewParent->m_hWnd));

	HWND hWnd = ::SetParent(m_hWnd, pWndNewParent->m_hWnd);
	if (hWnd != nullptr)
	{
		m_pOwner = pWndNewParent;
	}
	return hWnd;
}
// Window Tree Access
INLINE Int IXWnd::GetDlgCtrlID(void) const
{
	assert(::IsWindow(m_hWnd));
	return ::GetDlgCtrlID(m_hWnd);
}

INLINE Int IXWnd::SetDlgCtrlID(Int nID)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SetWindowLong(m_hWnd, GWL_ID, nID);
}

INLINE HWND IXWnd::GetDlgItem(Int nID) const
{
	assert(::IsWindow(m_hWnd));
	return ::GetDlgItem(m_hWnd, nID);
}

// Alert Functions
INLINE BOOL IXWnd::FlashWindow(BOOL bInvert)
{
	assert(::IsWindow(m_hWnd));
	return ::FlashWindow(m_hWnd, bInvert);
}

INLINE Int IXWnd::MessageBox(PCXStr pszText, PCXStr pszCaption, UInt uType)
{
	assert(::IsWindow(m_hWnd));
	return ::MessageBox(m_hWnd, pszText, pszCaption, uType);
}

// Clipboard Functions
INLINE BOOL IXWnd::ChangeClipboardChain(IXWnd* pWndNewNext)
{
	assert(::IsWindow(m_hWnd));
	assert(pWndNewNext != nullptr);
	assert(::IsWindow(pWndNewNext->m_hWnd));
	return ::ChangeClipboardChain(m_hWnd, pWndNewNext->m_hWnd);
}

INLINE HWND IXWnd::SetClipboardViewer(void)
{
	assert(::IsWindow(m_hWnd));
	return ::SetClipboardViewer(m_hWnd);
}

INLINE BOOL IXWnd::OpenClipboard(void)
{
	assert(::IsWindow(m_hWnd));
	return ::OpenClipboard(m_hWnd);
}

// Caret Functions
INLINE BOOL IXWnd::CreateCaret(HBITMAP hBitmap)
{
	assert(::IsWindow(m_hWnd));
	return ::CreateCaret(m_hWnd, hBitmap, 0, 0);
}

INLINE BOOL IXWnd::CreateSolidCaret(Int nWidth, Int nHeight)
{
	assert(::IsWindow(m_hWnd));
	return ::CreateCaret(m_hWnd, (HBITMAP)0, nWidth, nHeight);
}

INLINE BOOL IXWnd::CreateGrayCaret(Int nWidth, Int nHeight)
{
	assert(::IsWindow(m_hWnd));
	return ::CreateCaret(m_hWnd, (HBITMAP)1, nWidth, nHeight);
}

INLINE BOOL IXWnd::HideCaret(void)
{
	assert(::IsWindow(m_hWnd));
	return ::HideCaret(m_hWnd);
}

INLINE BOOL IXWnd::ShowCaret(void)
{
	assert(::IsWindow(m_hWnd));
	return ::ShowCaret(m_hWnd);
}

// Icon Functions
INLINE HICON IXWnd::SetIcon(HICON hIcon, BOOL bBigIcon)
{
	assert(::IsWindow(m_hWnd));
	return (HICON)::SendMessage(m_hWnd, WM_SETICON, bBigIcon, (LPARAM)hIcon);
}

INLINE HICON IXWnd::GetIcon(BOOL bBigIcon) const
{
	assert(::IsWindow(m_hWnd));
	return (HICON)::SendMessage(m_hWnd, WM_GETICON, bBigIcon, 0);
}

// Help Functions
INLINE BOOL IXWnd::WinHelp(PCXStr pszHelp, UInt uCmd, ULong ulData)
{
	assert(::IsWindow(m_hWnd));
	return ::WinHelp(m_hWnd, pszHelp, uCmd, ulData);
}

INLINE BOOL IXWnd::SetWindowContextHelpId(ULong ulContextHelpId)
{
	assert(::IsWindow(m_hWnd));
	return ::SetWindowContextHelpId(m_hWnd, ulContextHelpId);
}

INLINE ULong IXWnd::GetWindowContextHelpId(void) const
{
	assert(::IsWindow(m_hWnd));
	return ::GetWindowContextHelpId(m_hWnd);
}

// Hot Key Functions
INLINE Int IXWnd::SetHotKey(UShort usVirtualKeyCode, UShort usModifiers)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, WM_SETHOTKEY, MAKEWORD(usVirtualKeyCode, usModifiers), 0);
}

INLINE ULong IXWnd::GetHotKey(void) const
{
	assert(::IsWindow(m_hWnd));
	return (ULong)::SendMessage(m_hWnd, WM_GETHOTKEY, 0, 0);
}

//N new
INLINE BOOL IXWnd::GetScrollInfo(Int nBar, LPSCROLLINFO lpScrollInfo)
{
	assert(::IsWindow(m_hWnd));
	return ::GetScrollInfo(m_hWnd, nBar, lpScrollInfo);
}

INLINE Int IXWnd::SetScrollInfo(Int nBar, LPSCROLLINFO lpScrollInfo, BOOL bRedraw)
{
	assert(::IsWindow(m_hWnd));
	return ::SetScrollInfo(m_hWnd, nBar, lpScrollInfo, bRedraw);
}

INLINE BOOL IXWnd::IsDialogMessage(LPMSG lpMsg)
{
	assert(::IsWindow(m_hWnd));
	return ::IsDialogMessage(m_hWnd, lpMsg);
}

INLINE void IXWnd::NextDlgCtrl(void) const
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, WM_NEXTDLGCTL, 0, 0L);
}

INLINE void IXWnd::PrevDlgCtrl(void) const
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, WM_NEXTDLGCTL, 1, 0L);
}

INLINE void IXWnd::GotoDlgCtrl(IXWnd* pWndCtrl) const
{
	assert(::IsWindow(m_hWnd));
	assert(pWndCtrl != nullptr);
	assert(::IsWindow(pWndCtrl->m_hWnd));
	::SendMessage(m_hWnd, WM_NEXTDLGCTL, (WPARAM)pWndCtrl->m_hWnd, 1L);
}

INLINE Int IXWnd::GetWindowRgn(HRGN hRgn)
{
	assert(::IsWindow(m_hWnd));
	return ::GetWindowRgn(m_hWnd, hRgn);
}

INLINE Int IXWnd::SetWindowRgn(HRGN hRgn, BOOL bRedraw)
{
	assert(::IsWindow(m_hWnd));
	return ::SetWindowRgn(m_hWnd, hRgn, bRedraw);
}

INLINE ULong IXWnd::GetWindowThreadID(void)
{
	assert(::IsWindow(m_hWnd));
	return ::GetWindowThreadProcessId(m_hWnd, nullptr);
}

INLINE ULong IXWnd::GetWindowProcessID(void)
{
	assert(::IsWindow(m_hWnd));
	ULong ulProcessID = 0;
	::GetWindowThreadProcessId(m_hWnd, &ulProcessID);
	return ulProcessID;
}

INLINE BOOL IXWnd::IsWindow(void) const
{
	return ::IsWindow(m_hWnd);
}

INLINE BOOL IXWnd::IsWindowUnicode(void) const
{
	assert(::IsWindow(m_hWnd));
	return ::IsWindowUnicode(m_hWnd);
}

INLINE BOOL IXWnd::ShowWindowAsync(Int nCmdShow)
{
	assert(::IsWindow(m_hWnd));
	return ::ShowWindowAsync(m_hWnd, nCmdShow);
}

INLINE void IXWnd::PreSubClass(void)
{
}

INLINE bool IXWnd::OnIdle(void)
{
	// return true to keep idle state
	return true;
}

INLINE bool IXWnd::PreTranslateMessage(LPMSG)
{
	return false;
}

INLINE void IXWnd::SetEventHandler(class CEventHandler*)
{
}

INLINE BOOL IXWnd::AdjustWndRect(LPRECT lpRect, ULong ulStyle, BOOL bMenu, ULong ulExStyle)
{
	return ::AdjustWindowRectEx(lpRect, ulStyle, bMenu, ulExStyle);
}

INLINE bool IXWnd::InitCommonCtrls(ULong ulFlags)
{
	INITCOMMONCONTROLSEX iccx = { sizeof(INITCOMMONCONTROLSEX), ulFlags };
	return ::InitCommonControlsEx(&iccx) ? true : false;
}

INLINE Int IXWnd::MessageBoxParam(PCXStr pszText, PCXStr pszCaption, UInt uType, HWND hWnd, ...)
{
	assert(pszText != nullptr);

	XChar szBuf[LMT_BUF] = { 0 };
	va_list vl;
	va_start(vl, hWnd);
	Int nLen = CXChar::VFormat(szBuf, LMT_BUF, pszText, vl);
	va_end(vl);

	if (nLen > 0) 
	{
		return ::MessageBox(hWnd, szBuf, pszCaption, uType);
	}
	return 0;
}

INLINE LRESULT IXWnd::WindowProc(UInt uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL    bHandled = FALSE;
	LRESULT lRet     = 0;

	if ((bHandled == FALSE) && (m_pProc != nullptr))
	{
		lRet = ::CallWindowProc(m_pProc, m_hWnd, uMsg, wParam, lParam);
	}
	// dialog default or cancel button support
	if (IsDialogType() && (uMsg == WM_COMMAND))
	{
		if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
		{
			if (IsModal())
			{
				EndDialog(LOWORD(wParam));
			}
			else
			{
				lRet = DestroyWindow();
			}
		}
	}
	return lRet;
}

#endif // (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#endif // __TARGET_XUI_BASE_INL__