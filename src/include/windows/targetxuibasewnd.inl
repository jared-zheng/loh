///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : targetxuiwnd.inl                                                                      //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2005-02-02     version 0.0.0.1                                                             //
//   Update : 2015-03-16     version 0.0.5.5                                                             //
//   Detail : xui windows implement                                                                      //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TARGET_XUI_BASE_WND_INL__
#define __TARGET_XUI_BASE_WND_INL__

#pragma once

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXWnd
INLINE CXWnd::CXWnd(Int nType)
: IXWnd(XWNDT_WINDOW|(nType & (XWNDT_TYPEMASK|XWNDT_UIKIT)))
{
}

INLINE CXWnd::~CXWnd(void)
{
	assert(::IsWindow(m_hWnd) == FALSE);
}

INLINE CXWnd::CXWnd(const CXWnd&)
: IXWnd(XWNDT_WINDOW)
{
}

INLINE CXWnd& CXWnd::operator=(const CXWnd&)
{
	return (*this);
}

INLINE PCXStr CXWnd::GetXWndClass(void)
{
	return  TF("__XUI_WND_CLASS_0055__");
}

INLINE LRESULT CXWnd::OnCreate(LPCREATESTRUCT)
{
	return 0;
}

INLINE void CXWnd::OnDestroy(void)
{
	if (IsFrameMode())
	{
		::PostQuitMessage(0);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXDlg
INLINE CXDlg::CXDlg(Int nType)
: IXWnd(XWNDT_DIALOG|(nType & (XWNDT_TYPEMASK|XWNDT_UIKIT)))
{
}

INLINE CXDlg::~CXDlg(void)
{
}

INLINE CXDlg::CXDlg(const CXDlg&)
: IXWnd(XWNDT_DIALOG)
{
}

INLINE CXDlg& CXDlg::operator=(const CXDlg&)
{
	return (*this);
}

INLINE PCXStr CXDlg::GetXWndClass(void)
{
	return  TF("#32770"); // system dialog
}

INLINE bool CXDlg::PreTranslateMessage(LPMSG pMsg)
{
	// translate modeless dialog input events
	if (((pMsg->message >= WM_KEYFIRST  ) && (pMsg->message <= WM_KEYLAST  )) ||
		((pMsg->message >= WM_MOUSEFIRST) && (pMsg->message <= WM_MOUSELAST)))
	{
		if (::IsDialogMessage(m_hWnd, pMsg))
		{
			return true;
		}
	}
	return false;
}

INLINE LRESULT CXDlg::OnInitDialog(void)
{
	return TRUE;
}

INLINE void CXDlg::OnDestroy(void)
{
	if (IsFrameMode())
	{
		::PostQuitMessage(0);
	}
}

INLINE bool CXDlg::OnDDWExchange(Int, IXWnd*&)
{
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXFrameWnd
INLINE CXFrameWnd::CXFrameWnd(void)
: CXWnd(XWNDT_FRAME)
{
}

INLINE CXFrameWnd::~CXFrameWnd(void)
{
}

INLINE CXFrameWnd::CXFrameWnd(const CXFrameWnd&)
: CXWnd(XWNDT_FRAME)
{
}

INLINE CXFrameWnd& CXFrameWnd::operator=(const CXFrameWnd&)
{
	return (*this);
}

INLINE void CXFrameWnd::FrameLayout(void)
{ 
}

INLINE LRESULT CXFrameWnd::OnNotify(Int nID, LPNMHDR pHead, BOOL&)
{   
	// control should handle the message itself. 
	// if need frame handle , send XUIM_NOTIFY
	// rebar with RBS_AUTOSIZE, must use PostMessage 
	//::PostMessage(pHead->hwndFrom, WM_NOTIFY, (WPARAM)nID, (LPARAM)pHead); 
	if (pHead->hwndFrom != m_hWnd)                                   
	{                                                               
		::SendMessage(pHead->hwndFrom, WM_NOTIFY, (WPARAM)nID, (LPARAM)pHead);        
	}                                                               
	return 0L;                                                      
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXView
INLINE CXView::CXView(void)
: CXWnd(XWNDT_VIEW)
{
}

INLINE CXView::~CXView(void)
{
}

INLINE CXView::CXView(const CXView&)
: CXWnd(XWNDT_VIEW)
{
}

INLINE CXView& CXView::operator=(const CXView&)
{
	return (*this);
}

INLINE void CXView::OnPaint(void)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXControl
INLINE CXControl::CXControl(Int nType)
: IXWnd(XWNDT_CONTROL|(nType & (XWNDT_TYPEMASK|XWNDT_UIKIT)))
{
}

INLINE CXControl::~CXControl(void)
{
}

INLINE CXControl::CXControl(const CXControl&)
: IXWnd(XWNDT_CONTROL)
{
}

INLINE CXControl& CXControl::operator=(const CXControl&)
{
	return (*this);
}

INLINE LRESULT CXControl::OnNotify(Int, LPNMHDR, BOOL& bHandled)
{
	bHandled = TRUE;
	return 0;
}

INLINE LRESULT CXControl::OnXLayout(WPARAM, LPARAM, BOOL&)
{
	return 0;
}

INLINE LRESULT CXControl::OnXNotify(WPARAM, LPARAM, BOOL&)
{
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXStatic : WC_STATIC
INLINE CXStatic::CXStatic(void)
: CXControl(XWNDT_STATIC)
{
}

INLINE CXStatic::~CXStatic(void)
{
}

INLINE CXStatic::CXStatic(const CXStatic&)
: CXControl(XWNDT_STATIC)
{
}

INLINE CXStatic& CXStatic::operator=(const CXStatic&)
{
	return (*this);
}

INLINE PCXStr CXStatic::GetXWndClass(void)
{
	return WC_STATIC;
}

INLINE HICON CXStatic::GetIcon(void) const
{
	assert(::IsWindow(m_hWnd));
	return (HICON)::SendMessage(m_hWnd, STM_GETICON, 0, 0L);
}

INLINE HICON CXStatic::SetIcon(HICON hIcon)
{
	assert(::IsWindow(m_hWnd));
	return (HICON)::SendMessage(m_hWnd, STM_SETICON, (WPARAM)hIcon, 0L);
}

INLINE HENHMETAFILE CXStatic::GetEnhMetaFile(void) const
{
	assert(::IsWindow(m_hWnd));
	return (HENHMETAFILE)::SendMessage(m_hWnd, STM_GETIMAGE, IMAGE_ENHMETAFILE, 0L);
}

INLINE HENHMETAFILE CXStatic::SetEnhMetaFile(HENHMETAFILE hMetaFile)
{
	assert(::IsWindow(m_hWnd));
	return (HENHMETAFILE)::SendMessage(m_hWnd, STM_SETIMAGE, IMAGE_ENHMETAFILE, (LPARAM)hMetaFile);
}

INLINE HBITMAP CXStatic::GetBitmap(void) const
{
	assert(::IsWindow(m_hWnd));
	return (HBITMAP)::SendMessage(m_hWnd, STM_GETIMAGE, IMAGE_BITMAP, 0L);
}

INLINE HBITMAP CXStatic::SetBitmap(HBITMAP hBitmap)
{
	assert(::IsWindow(m_hWnd));
	return (HBITMAP)::SendMessage(m_hWnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
}

INLINE HCURSOR CXStatic::GetCursor(void) const
{
	assert(::IsWindow(m_hWnd));
	return (HCURSOR)::SendMessage(m_hWnd, STM_GETIMAGE, IMAGE_CURSOR, 0L);
}

INLINE HCURSOR CXStatic::SetCursor(HCURSOR hCursor)
{
	assert(::IsWindow(m_hWnd));
	return (HCURSOR)::SendMessage(m_hWnd, STM_SETIMAGE, IMAGE_CURSOR, (LPARAM)hCursor);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXButton : WC_BUTTON
INLINE CXButton::CXButton(void)
: CXControl(XWNDT_BUTTON)
{
}

INLINE CXButton::~CXButton(void)
{
}

INLINE CXButton::CXButton(const CXButton&)
: CXControl(XWNDT_BUTTON)
{
}

INLINE CXButton& CXButton::operator=(const CXButton&)
{
	return (*this);
}

INLINE PCXStr CXButton::GetXWndClass(void)
{
	return WC_BUTTON;
}

INLINE UInt CXButton::GetState(void) const
{
	assert(::IsWindow(m_hWnd));
	return (UInt)::SendMessage(m_hWnd, BM_GETSTATE, 0, 0L);
}

INLINE void CXButton::SetState(BOOL bHighlight)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, BM_SETSTATE, bHighlight, 0L);
}

INLINE Int CXButton::GetCheck(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, BM_GETCHECK, 0, 0L);
}

INLINE void CXButton::SetCheck(Int nCheck)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, BM_SETCHECK, nCheck, 0L);
}

INLINE UInt CXButton::GetButtonStyle(void) const
{
	assert(::IsWindow(m_hWnd));
	return (UInt)::GetWindowLong(m_hWnd, GWL_STYLE) & 0xFFFF;
}

INLINE void CXButton::SetButtonStyle(UInt uStyle, BOOL bRedraw)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, BM_SETSTYLE, uStyle, (LPARAM)bRedraw);
}

INLINE HICON CXButton::GetIcon(void) const
{
	assert(::IsWindow(m_hWnd));
	return (HICON)::SendMessage(m_hWnd, BM_GETIMAGE, IMAGE_ICON, 0L);
}

INLINE HICON CXButton::SetIcon(HICON hIcon)
{
	assert(::IsWindow(m_hWnd));
	return (HICON)::SendMessage(m_hWnd, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);
}

INLINE HBITMAP CXButton::GetBitmap(void) const
{
	assert(::IsWindow(m_hWnd));
	return (HBITMAP)::SendMessage(m_hWnd, BM_GETIMAGE, IMAGE_BITMAP, 0L);
}

INLINE HBITMAP CXButton::SetBitmap(HBITMAP hBitmap)
{
	assert(::IsWindow(m_hWnd));
	return (HBITMAP)::SendMessage(m_hWnd, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
}
#if (_WIN32_WINNT >= 0x0501)
INLINE BOOL CXButton::GetIdealSize(LPSIZE lpSize) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, BCM_GETIDEALSIZE, 0, (LPARAM)lpSize);
}

INLINE BOOL CXButton::GetImageList(PBUTTON_IMAGELIST pButtonImagelist) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, BCM_GETIMAGELIST, 0, (LPARAM)pButtonImagelist);
}

INLINE BOOL CXButton::SetImageList(PBUTTON_IMAGELIST pButtonImagelist)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, BCM_SETIMAGELIST, 0, (LPARAM)pButtonImagelist);
}

INLINE BOOL CXButton::GetTextMargin(LPRECT lpRect) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, BCM_GETTEXTMARGIN, 0, (LPARAM)lpRect);
}

INLINE BOOL CXButton::SetTextMargin(LPRECT lpRect)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, BCM_SETTEXTMARGIN, 0, (LPARAM)lpRect);
}
#endif // (_WIN32_WINNT >= 0x0501)
#if (WINVER >= 0x0600)
INLINE void CXButton::SetDontClick(BOOL bDontClick)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, BM_SETDONTCLICK, (WPARAM)bDontClick, 0L);
}
#endif // (WINVER >= 0x0600)
#if (_WIN32_WINNT >= 0x0600)
INLINE BOOL CXButton::SetDropDownState(BOOL bDropDown)
{
	assert(::IsWindow(m_hWnd));
	assert((GetWindowLongPtr(GWL_STYLE) & (BS_SPLITBUTTON | BS_DEFSPLITBUTTON)) != 0);
	return (BOOL)::SendMessage(m_hWnd, BCM_SETDROPDOWNSTATE, (WPARAM)bDropDown, 0L);
}

INLINE BOOL CXButton::GetSplitInfo(PBUTTON_SPLITINFO pSplitInfo) const
{
	assert(::IsWindow(m_hWnd));
	assert((GetWindowLongPtr(GWL_STYLE) & (BS_SPLITBUTTON | BS_DEFSPLITBUTTON)) != 0);
	return (BOOL)::SendMessage(m_hWnd, BCM_GETSPLITINFO, 0, (LPARAM)pSplitInfo);
}

INLINE BOOL CXButton::SetSplitInfo(PBUTTON_SPLITINFO pSplitInfo)
{
	assert(::IsWindow(m_hWnd));
	assert((GetWindowLongPtr(GWL_STYLE) & (BS_SPLITBUTTON | BS_DEFSPLITBUTTON)) != 0);
	return (BOOL)::SendMessage(m_hWnd, BCM_SETSPLITINFO, 0, (LPARAM)pSplitInfo);
}

INLINE Int CXButton::GetNoteLength(void) const
{
	assert(::IsWindow(m_hWnd));
	assert((GetWindowLongPtr(GWL_STYLE) & (BS_COMMANDLINK | BS_DEFCOMMANDLINK)) != 0);
	return (Int)::SendMessage(m_hWnd, BCM_GETNOTELENGTH, 0, 0L);
}

INLINE BOOL CXButton::GetNote(PWStr pszNoteText, Int nNoteText) const
{
	assert(::IsWindow(m_hWnd));
	assert((GetWindowLongPtr(GWL_STYLE) & (BS_COMMANDLINK | BS_DEFCOMMANDLINK)) != 0);
	return (BOOL)::SendMessage(m_hWnd, BCM_GETNOTE, nNoteText, (LPARAM)pszNoteText);
}

INLINE BOOL CXButton::SetNote(PCWStr pszNoteText)
{
	assert(::IsWindow(m_hWnd));
	assert((GetWindowLongPtr(GWL_STYLE) & (BS_COMMANDLINK | BS_DEFCOMMANDLINK)) != 0);
	return (BOOL)::SendMessage(m_hWnd, BCM_SETNOTE, 0, (LPARAM)pszNoteText);
}

INLINE LRESULT CXButton::SetElevationRequiredState(BOOL bSet)
{
	assert(::IsWindow(m_hWnd));
	return ::SendMessage(m_hWnd, BCM_SETSHIELD, 0, (LPARAM)bSet);
}
#endif // (_WIN32_WINNT >= 0x0600)
// Operations
INLINE void CXButton::Click(void)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, BM_CLICK, 0, 0L);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXEdit : WC_EDIT
INLINE CXEdit::CXEdit(void)
: CXControl(XWNDT_EDIT)
{
}

INLINE CXEdit::~CXEdit(void)
{
}

INLINE CXEdit::CXEdit(const CXEdit&)
: CXControl(XWNDT_EDIT)
{
}

INLINE CXEdit& CXEdit::operator=(const CXEdit&)
{
	return (*this);
}

INLINE PCXStr CXEdit::GetXWndClass(void)
{
	return WC_EDIT;
}

INLINE BOOL CXEdit::CanUndo(void) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, EM_CANUNDO, 0, 0L);
}

INLINE Int CXEdit::GetLineCount(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, EM_GETLINECOUNT, 0, 0L);
}

INLINE BOOL CXEdit::GetModify(void) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, EM_GETMODIFY, 0, 0L);
}

INLINE void CXEdit::SetModify(BOOL bModified)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, EM_SETMODIFY, bModified, 0L);
}

INLINE void CXEdit::GetRect(LPRECT lpRect) const
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, EM_GETRECT, 0, (LPARAM)lpRect);
}

INLINE ULong CXEdit::GetSel(void) const
{
	assert(::IsWindow(m_hWnd));
	return (ULong)::SendMessage(m_hWnd, EM_GETSEL, 0, 0L);
}

INLINE void CXEdit::GetSel(Int& lStartChar, Int& lEndChar) const
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, EM_GETSEL, (WPARAM)&lStartChar, (LPARAM)&lEndChar);
}

INLINE HLOCAL CXEdit::GetBuffer(void) const
{
	assert(::IsWindow(m_hWnd));
	return (HLOCAL)::SendMessage(m_hWnd, EM_GETHANDLE, 0, 0L);
}

INLINE void CXEdit::SetBuffer(HLOCAL hBuffer)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, EM_SETHANDLE, (WPARAM)hBuffer, 0L);
}

INLINE ULong CXEdit::GetMargins(void) const
{
	assert(::IsWindow(m_hWnd));
	return (ULong)::SendMessage(m_hWnd, EM_GETMARGINS, 0, 0L);
}

INLINE void CXEdit::SetMargins(UInt uLeft, UInt uRight)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, EM_SETMARGINS, EC_LEFTMARGIN|EC_RIGHTMARGIN, MAKELPARAM(uLeft, uRight));
}

INLINE UInt CXEdit::GetLimitText(void) const
{
	assert(::IsWindow(m_hWnd));
	return (UInt)::SendMessage(m_hWnd, EM_GETLIMITTEXT, 0, 0L);
}

INLINE void CXEdit::SetLimitText(UInt uMax)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, EM_SETLIMITTEXT, uMax, 0L);
}

INLINE POINT CXEdit::PosFromChar(UInt uChar) const
{
	assert(::IsWindow(m_hWnd));
	ULong ulRet = (ULong)::SendMessage(m_hWnd, EM_POSFROMCHAR, uChar, 0);
	POINT point = { GET_X_LPARAM(ulRet), GET_Y_LPARAM(ulRet) };
	return point;
}

INLINE Int CXEdit::CharFromPos(POINT& pt, PInt pnLine) const
{
	assert(::IsWindow(m_hWnd));
	ULong ulRet = (ULong)::SendMessage(m_hWnd, EM_CHARFROMPOS, 0, MAKELPARAM(pt.x, pt.y));
	if (pnLine != nullptr)
	{
		*pnLine = (Int)(Short)HIWORD(ulRet);
	}
	return (Int)(Short)LOWORD(ulRet);
}
// NOTE: first word in pszBuffer must contain the size of the buffer!
INLINE Int CXEdit::GetLine(Int nIndex, PXStr pszBuffer) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, EM_GETLINE, nIndex, (LPARAM)pszBuffer);
}

INLINE Int CXEdit::GetLine(Int nIndex, PXStr pszBuffer, Int nMaxLength) const
{
	assert(::IsWindow(m_hWnd));
	*(PUShort)pszBuffer = (UShort)nMaxLength;
	return (Int)::SendMessage(m_hWnd, EM_GETLINE, nIndex, (LPARAM)pszBuffer);
}

INLINE XChar CXEdit::GetPasswordChar(void) const
{
	assert(::IsWindow(m_hWnd));
	return (XChar)::SendMessage(m_hWnd, EM_GETPASSWORDCHAR, 0, 0L);
}

INLINE void CXEdit::SetPasswordChar(XChar c)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, EM_SETPASSWORDCHAR, c, 0L);
}

INLINE EDITWORDBREAKPROC CXEdit::GetWordBreakProc(void) const
{
	assert(::IsWindow(m_hWnd));
	return (EDITWORDBREAKPROC)::SendMessage(m_hWnd, EM_GETWORDBREAKPROC, 0, 0L);
}

INLINE void CXEdit::SetWordBreakProc(EDITWORDBREAKPROC ewbprc)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, EM_SETWORDBREAKPROC, 0, (LPARAM)ewbprc);
}

INLINE Int CXEdit::GetFirstVisibleLine(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, EM_GETFIRSTVISIBLELINE, 0, 0L);
}

INLINE Int CXEdit::GetThumb(void) const
{
	assert(::IsWindow(m_hWnd));
	assert((GetWindowLongPtr(GWL_STYLE) & ES_MULTILINE) != 0);
	return (Int)::SendMessage(m_hWnd, EM_GETTHUMB, 0, 0L);
}

INLINE BOOL CXEdit::SetReadOnly(BOOL bReadOnly)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, EM_SETREADONLY, bReadOnly, 0L);
}

INLINE UInt CXEdit::GetImeStatus(UInt uStatus) const
{
	assert(::IsWindow(m_hWnd));
	return (UInt)::SendMessage(m_hWnd, EM_GETIMESTATUS, uStatus, 0L);
}

INLINE UInt CXEdit::SetImeStatus(UInt uStatus, UInt uData)
{
	assert(::IsWindow(m_hWnd));
	return (UInt)::SendMessage(m_hWnd, EM_SETIMESTATUS, uStatus, uData);
}
#if (_WIN32_WINNT >= 0x0501)
INLINE BOOL CXEdit::GetCueBannerText(PCWStr pszText, Int nText) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, EM_GETCUEBANNER, (WPARAM)pszText, nText);
}

// bKeepWithFocus - Vista only
INLINE BOOL CXEdit::SetCueBannerText(PCWStr pszText, BOOL bKeepWithFocus)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, EM_SETCUEBANNER, (WPARAM)bKeepWithFocus, (LPARAM)(pszText));
}
#endif // (_WIN32_WINNT >= 0x0501)
// Operations
INLINE void CXEdit::EmptyUndoBuffer(void)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, EM_EMPTYUNDOBUFFER, 0, 0L);
}

INLINE BOOL CXEdit::FmtLines(BOOL bAddEOL)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, EM_FMTLINES, bAddEOL, 0L);
}

INLINE void CXEdit::LimitText(Int nChars)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, EM_LIMITTEXT, nChars, 0L);
}

INLINE Int CXEdit::LineFromChar(Int nIndex) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, EM_LINEFROMCHAR, nIndex, 0L);
}

INLINE Int CXEdit::LineIndex(Int nLine) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, EM_LINEINDEX, nLine, 0L);
}

INLINE Int CXEdit::LineLength(Int nLine) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, EM_LINELENGTH, nLine, 0L);
}

INLINE void CXEdit::LineScroll(Int nLines, Int nChars)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, EM_LINESCROLL, nChars, nLines);
}

INLINE void CXEdit::ReplaceSel(PCXStr pszNewText, BOOL bCanUndo)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, EM_REPLACESEL, (WPARAM) bCanUndo, (LPARAM)pszNewText);
}

INLINE void CXEdit::SetRect(LPCRECT lpRect)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, EM_SETRECT, 0, (LPARAM)lpRect);
}

INLINE void CXEdit::SetRectNP(LPCRECT lpRect)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, EM_SETRECTNP, 0, (LPARAM)lpRect);
}

INLINE void CXEdit::SetSel(ULong ulSelection, BOOL bNoScroll)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, EM_SETSEL, LOWORD(ulSelection), HIWORD(ulSelection));
	if (bNoScroll == FALSE)
	{
		::SendMessage(m_hWnd, EM_SCROLLCARET, 0, 0L);
	}
}

INLINE void CXEdit::SetSel(Int lStartChar, Int lEndChar, BOOL bNoScroll)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, EM_SETSEL, lStartChar, lEndChar);
	if (bNoScroll == FALSE)
	{
		::SendMessage(m_hWnd, EM_SCROLLCARET, 0, 0L);
	}
}

INLINE void CXEdit::SetSelAll(BOOL bNoScroll)
{
	SetSel(0, -1, bNoScroll);
}

INLINE void CXEdit::SetSelNone(BOOL bNoScroll)
{
	SetSel(-1, 0, bNoScroll);
}

INLINE BOOL CXEdit::SetTabStops(Int nTabStops, PInt pnTabStops)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, EM_SETTABSTOPS, nTabStops, (LPARAM)pnTabStops);
}

INLINE BOOL CXEdit::SetTabStops(void)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, EM_SETTABSTOPS, 0, 0L);
}

INLINE BOOL CXEdit::SetTabStops(const Int& nEachStop)    // takes an 'Int'
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, EM_SETTABSTOPS, 1, (LPARAM)(PInt)&nEachStop);
}

INLINE void CXEdit::ScrollCaret(void)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, EM_SCROLLCARET, 0, 0L);
}

INLINE Int CXEdit::Scroll(Int nScrollAction)
{
	assert(::IsWindow(m_hWnd));
	assert((GetWindowLongPtr(GWL_STYLE) & ES_MULTILINE) != 0);
	LRESULT lRet = ::SendMessage(m_hWnd, EM_SCROLL, nScrollAction, 0L);
	if ((BOOL)(HIWORD(lRet)) == FALSE)
	{
		return -1;   // failed
	}
	return (Int)(Short)LOWORD(lRet);
}

INLINE void CXEdit::InsertText(Int nInsertAfterChar, PCXStr pszText, BOOL bNoScroll, BOOL bCanUndo)
{
	SetSel(nInsertAfterChar, nInsertAfterChar, bNoScroll);
	ReplaceSel(pszText, bCanUndo);
}

INLINE void CXEdit::AppendText(PCXStr pszText, BOOL bNoScroll, BOOL bCanUndo)
{
	InsertText(GetWindowTextLength(), pszText, bNoScroll, bCanUndo);
}
#if (_WIN32_WINNT >= 0x0501)
INLINE BOOL CXEdit::ShowBalloonTip(PEDITBALLOONTIP pEditBaloonTip)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, EM_SHOWBALLOONTIP, 0, (LPARAM)pEditBaloonTip);
}

INLINE BOOL CXEdit::HideBalloonTip(void)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, EM_HIDEBALLOONTIP, 0, 0L);
}
#endif // (_WIN32_WINNT >= 0x0501)
#if (_WIN32_WINNT >= 0x0600)
INLINE ULong CXEdit::GetHilite(void) const
{
	assert(::IsWindow(m_hWnd));
	return (ULong)::SendMessage(m_hWnd, EM_GETHILITE, 0, 0L);
}

INLINE void CXEdit::GetHilite(Int& nStartChar, Int& nEndChar) const
{
	assert(::IsWindow(m_hWnd));
	ULong ulRet = (DWORD)::SendMessage(m_hWnd, EM_GETHILITE, 0, 0L);
	nStartChar  = (Int)(Short)LOWORD(ulRet);
	nEndChar    = (Int)(Short)HIWORD(ulRet);
}

INLINE void CXEdit::SetHilite(Int nStartChar, Int nEndChar)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, EM_SETHILITE, nStartChar, nEndChar);
}
#endif // (_WIN32_WINNT >= 0x0600)
// Clipboard operations
INLINE BOOL CXEdit::Undo(void)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, EM_UNDO, 0, 0L);
}

INLINE void CXEdit::Clear(void)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, WM_CLEAR, 0, 0L);
}

INLINE void CXEdit::Copy(void)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, WM_COPY, 0, 0L);
}

INLINE void CXEdit::Cut(void)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, WM_CUT, 0, 0L);
}

INLINE void CXEdit::Paste(void)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, WM_PASTE, 0, 0L);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXListBox : WC_LISTBOX
INLINE CXListBox::CXListBox(void)
: CXControl(XWNDT_LISTBOX)
{
}

INLINE CXListBox::~CXListBox(void)
{
}

INLINE CXListBox::CXListBox(const CXListBox&)
: CXControl(XWNDT_LISTBOX)
{
}

INLINE CXListBox& CXListBox::operator=(const CXListBox&)
{
	return (*this);
}

INLINE PCXStr CXListBox::GetXWndClass(void)
{
	return WC_LISTBOX;
}

// for entire listbox
INLINE Int CXListBox::GetCount(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LB_GETCOUNT, 0, 0L);
}

INLINE Int CXListBox::SetCount(Int nItems)
{
	assert(::IsWindow(m_hWnd));
	assert(((GetWindowLongPtr(GWL_STYLE) & LBS_NODATA) != 0) && ((GetWindowLongPtr(GWL_STYLE) & LBS_HASSTRINGS) == 0));
	return (Int)::SendMessage(m_hWnd, LB_SETCOUNT, nItems, 0L);
}

INLINE Int CXListBox::GetHorizontalExtent(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LB_GETHORIZONTALEXTENT, 0, 0L);
}

INLINE void CXListBox::SetHorizontalExtent(Int nExtent)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, LB_SETHORIZONTALEXTENT, nExtent, 0L);
}

INLINE Int CXListBox::GetTopIndex(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LB_GETTOPINDEX, 0, 0L);
}

INLINE Int CXListBox::SetTopIndex(Int nIndex)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LB_SETTOPINDEX, nIndex, 0L);
}

INLINE LCID CXListBox::GetLocale(void) const
{
	assert(::IsWindow(m_hWnd));
	return (LCID)::SendMessage(m_hWnd, LB_GETLOCALE, 0, 0L);
}

INLINE LCID CXListBox::SetLocale(LCID nNewLocale)
{
	assert(::IsWindow(m_hWnd));
	return (LCID)::SendMessage(m_hWnd, LB_SETLOCALE, (WPARAM)nNewLocale, 0L);
}
#if (WINVER >= 0x0500)
INLINE ULong CXListBox::GetListBoxInfo(void) const
{
	assert(::IsWindow(m_hWnd));
#if (_WIN32_WINNT >= 0x0501)
	return (ULong)::SendMessage(m_hWnd, LB_GETLISTBOXINFO, 0, 0L);
#else // !(_WIN32_WINNT >= 0x0501)
	return ::GetListBoxInfo(m_hWnd);
#endif // !(_WIN32_WINNT >= 0x0501)
}
#endif // (WINVER >= 0x0500)
// for single-selection listboxes
INLINE Int CXListBox::GetCurSel(void) const
{
	assert(::IsWindow(m_hWnd));
	assert((GetWindowLongPtr(GWL_STYLE) & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)) == 0);
	return (Int)::SendMessage(m_hWnd, LB_GETCURSEL, 0, 0L);
}

INLINE Int CXListBox::SetCurSel(Int nSelect)
{
	assert(::IsWindow(m_hWnd));
	assert((GetWindowLongPtr(GWL_STYLE) & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)) == 0);
	return (Int)::SendMessage(m_hWnd, LB_SETCURSEL, nSelect, 0L);
}
// for multiple-selection listboxes
INLINE Int CXListBox::GetSel(Int nIndex) const           // also works for single-selection
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LB_GETSEL, nIndex, 0L);
}

INLINE Int CXListBox::SetSel(Int nIndex, BOOL bSelect)
{
	assert(::IsWindow(m_hWnd));
	assert((GetWindowLongPtr(GWL_STYLE) & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)) != 0);
	return (Int)::SendMessage(m_hWnd, LB_SETSEL, bSelect, nIndex);
}

INLINE Int CXListBox::GetSelCount(void) const
{
	assert(::IsWindow(m_hWnd));
	assert((GetWindowLongPtr(GWL_STYLE) & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)) != 0);
	return (Int)::SendMessage(m_hWnd, LB_GETSELCOUNT, 0, 0L);
}

INLINE Int CXListBox::GetSelItems(Int nMaxItems, PInt pnIndex) const
{
	assert(::IsWindow(m_hWnd));
	assert((GetWindowLongPtr(GWL_STYLE) & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)) != 0);
	return (Int)::SendMessage(m_hWnd, LB_GETSELITEMS, nMaxItems, (LPARAM)pnIndex);
}

INLINE Int CXListBox::GetAnchorIndex(void) const
{
	assert(::IsWindow(m_hWnd));
	assert((GetWindowLongPtr(GWL_STYLE) & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)) != 0);
	return (Int)::SendMessage(m_hWnd, LB_GETANCHORINDEX, 0, 0L);
}

INLINE void CXListBox::SetAnchorIndex(Int nIndex)
{
	assert(::IsWindow(m_hWnd));
	assert((GetWindowLongPtr(GWL_STYLE) & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)) != 0);
	::SendMessage(m_hWnd, LB_SETANCHORINDEX, nIndex, 0L);
}

INLINE Int CXListBox::GetCaretIndex(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LB_GETCARETINDEX, 0, 0);
}

INLINE Int CXListBox::SetCaretIndex(Int nIndex, BOOL bScroll)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LB_SETCARETINDEX, nIndex, MAKELPARAM(bScroll, 0));
}

// for listbox items
INLINE uintptr_t CXListBox::GetItemData(Int nIndex) const
{
	assert(::IsWindow(m_hWnd));
	return (uintptr_t)::SendMessage(m_hWnd, LB_GETITEMDATA, nIndex, 0L);
}

INLINE Int CXListBox::SetItemData(Int nIndex, uintptr_t utItemData)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LB_SETITEMDATA, nIndex, (LPARAM)utItemData);
}

INLINE void* CXListBox::GetItemDataPtr(Int nIndex) const
{
	assert(::IsWindow(m_hWnd));
	return (void*)::SendMessage(m_hWnd, LB_GETITEMDATA, nIndex, 0L);
}

INLINE Int CXListBox::SetItemDataPtr(Int nIndex, void* pData)
{
	assert(::IsWindow(m_hWnd));
	return SetItemData(nIndex, (uintptr_t)pData);
}

INLINE Int CXListBox::GetItemRect(Int nIndex, LPRECT lpRect) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LB_GETITEMRECT, nIndex, (LPARAM)lpRect);
}

INLINE Int CXListBox::GetText(Int nIndex, PXStr pszBuffer) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LB_GETTEXT, nIndex, (LPARAM)pszBuffer);
}

INLINE Int CXListBox::GetTextLen(Int nIndex) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LB_GETTEXTLEN, nIndex, 0L);
}

INLINE Int CXListBox::GetItemHeight(Int nIndex) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LB_GETITEMHEIGHT, nIndex, 0L);
}

INLINE Int CXListBox::SetItemHeight(Int nIndex, UInt uItemHeight)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LB_SETITEMHEIGHT, nIndex, MAKELPARAM(uItemHeight, 0));
}
// Settable only attributes
INLINE void CXListBox::SetColumnWidth(Int nWidth)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, LB_SETCOLUMNWIDTH, nWidth, 0L);
}

INLINE BOOL CXListBox::SetTabStops(Int nTabStops, PInt pnTabStops)
{
	assert(::IsWindow(m_hWnd));
	assert((GetWindowLongPtr(GWL_STYLE) & LBS_USETABSTOPS) != 0);
	return (BOOL)::SendMessage(m_hWnd, LB_SETTABSTOPS, nTabStops, (LPARAM)pnTabStops);
}

INLINE BOOL CXListBox::SetTabStops(void)
{
	assert(::IsWindow(m_hWnd));
	assert((GetWindowLongPtr(GWL_STYLE) & LBS_USETABSTOPS) != 0);
	return (BOOL)::SendMessage(m_hWnd, LB_SETTABSTOPS, 0, 0L);
}

INLINE BOOL CXListBox::SetTabStops(const Int& nEachStop)    // takes an 'Int'
{
	assert(::IsWindow(m_hWnd));
	assert((GetWindowLongPtr(GWL_STYLE) & LBS_USETABSTOPS) != 0);
	return (BOOL)::SendMessage(m_hWnd, LB_SETTABSTOPS, 1, (LPARAM)(PInt)&nEachStop);
}
// Operations
INLINE Int CXListBox::InitStorage(Int nItems, UInt uBytes)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LB_INITSTORAGE, (WPARAM)nItems, uBytes);
}

INLINE void CXListBox::ResetContent(void)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, LB_RESETCONTENT, 0, 0L);
}

INLINE UInt CXListBox::ItemFromPoint(POINT& pt, BOOL& bOutside) const
{
	assert(::IsWindow(m_hWnd));
	ULong dw = (ULong)::SendMessage(m_hWnd, LB_ITEMFROMPOINT, 0, MAKELPARAM(pt.x, pt.y));
	bOutside = (BOOL)HIWORD(dw);
	return (UInt)LOWORD(dw);
}
// manipulating listbox items
INLINE Int CXListBox::AddString(PCXStr pszItem)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LB_ADDSTRING, 0, (LPARAM)pszItem);
}

INLINE Int CXListBox::DeleteString(Int nIndex)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LB_DELETESTRING, nIndex, 0L);
}

INLINE Int CXListBox::InsertString(Int nIndex, PCXStr pszItem)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LB_INSERTSTRING, nIndex, (LPARAM)pszItem);
}

INLINE Int CXListBox::Dir(UInt uAttr, PCXStr pszWildCard)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LB_DIR, uAttr, (LPARAM)pszWildCard);
}

INLINE Int CXListBox::AddFile(PCXStr pszFileName)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LB_ADDFILE, 0, (LPARAM)pszFileName);
}

// selection helpers
INLINE Int CXListBox::FindString(Int nStartAfter, PCXStr pszItem) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LB_FINDSTRING, nStartAfter, (LPARAM)pszItem);
}

INLINE Int CXListBox::FindStringExact(Int nIndexStart, PCXStr pszFind) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LB_FINDSTRINGEXACT, nIndexStart, (LPARAM)pszFind);
}

INLINE Int CXListBox::SelectString(Int nStartAfter, PCXStr pszItem)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LB_SELECTSTRING, nStartAfter, (LPARAM)pszItem);
}

INLINE Int CXListBox::SelItemRange(BOOL bSelect, Int nFirstItem, Int nLastItem)
{
	assert(::IsWindow(m_hWnd));
	assert((GetWindowLongPtr(GWL_STYLE) & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)) != 0);
	assert(nFirstItem <= nLastItem);
	return bSelect ? (Int)::SendMessage(m_hWnd, LB_SELITEMRANGEEX, nFirstItem, nLastItem) : (Int)::SendMessage(m_hWnd, LB_SELITEMRANGEEX, nLastItem, nFirstItem);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXComboBox : WC_COMBOBOX
INLINE CXComboBox::CXComboBox(void)
: CXControl(XWNDT_COMBOBOX)
{
}

INLINE CXComboBox::CXComboBox(Int nType)
: CXControl(nType)
{
}

INLINE CXComboBox::~CXComboBox(void)
{
}

INLINE CXComboBox::CXComboBox(const CXComboBox&)
: CXControl(XWNDT_COMBOBOX)
{
}

INLINE CXComboBox& CXComboBox::operator=(const CXComboBox&)
{
	return (*this);
}

INLINE PCXStr CXComboBox::GetXWndClass(void)
{
	return WC_COMBOBOX;
}

INLINE Int CXComboBox::GetCount(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, CB_GETCOUNT, 0, 0L);
}

INLINE Int CXComboBox::GetCurSel(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, CB_GETCURSEL, 0, 0L);
}

INLINE Int CXComboBox::SetCurSel(Int nSelect)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, CB_SETCURSEL, (WPARAM)nSelect, 0L);
}

INLINE LCID CXComboBox::GetLocale(void) const
{
	assert(::IsWindow(m_hWnd));
	return (LCID)::SendMessage(m_hWnd, CB_GETLOCALE, 0, 0L);
}

INLINE LCID CXComboBox::SetLocale(LCID nNewLocale)
{
	assert(::IsWindow(m_hWnd));
	return (LCID)::SendMessage(m_hWnd, CB_SETLOCALE, (WPARAM)nNewLocale, 0L);
}

INLINE Int CXComboBox::GetTopIndex(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, CB_GETTOPINDEX, 0, 0L);
}

INLINE Int CXComboBox::SetTopIndex(Int nIndex)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, CB_SETTOPINDEX, (WPARAM)nIndex, 0L);
}

INLINE UInt CXComboBox::GetHorizontalExtent(void) const
{
	assert(::IsWindow(m_hWnd));
	return (UInt)::SendMessage(m_hWnd, CB_GETHORIZONTALEXTENT, 0, 0L);
}

INLINE void CXComboBox::SetHorizontalExtent(UInt uExtent)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, CB_SETHORIZONTALEXTENT, (WPARAM)uExtent, 0L);
}

INLINE Int CXComboBox::GetDroppedWidth(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, CB_GETDROPPEDWIDTH, 0, 0L);
}

INLINE Int CXComboBox::SetDroppedWidth(UInt uWidth)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, CB_SETDROPPEDWIDTH, (WPARAM)uWidth, 0L);
}
#if (WINVER >= 0x0500)
INLINE BOOL CXComboBox::GetComboBoxInfo(PCOMBOBOXINFO pComboBoxInfo) const
{
	assert(::IsWindow(m_hWnd));
#if (_WIN32_WINNT >= 0x0501)
	return (BOOL)::SendMessage(m_hWnd, CB_GETCOMBOBOXINFO, 0, (LPARAM)pComboBoxInfo);
#else // !(_WIN32_WINNT >= 0x0501)
	return ::GetComboBoxInfo(m_hWnd, pComboBoxInfo);
#endif // !(_WIN32_WINNT >= 0x0501)
}
#endif // (WINVER >= 0x0500) 
// for edit control
INLINE ULong CXComboBox::GetEditSel(void) const
{
	assert(::IsWindow(m_hWnd));
	return (ULong)::SendMessage(m_hWnd, CB_GETEDITSEL, 0, 0L);
}

INLINE BOOL CXComboBox::SetEditSel(Int lStartChar, Int lEndChar)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, CB_SETEDITSEL, 0, MAKELPARAM(lStartChar, lEndChar));
}
// for combobox item
INLINE uintptr_t CXComboBox::GetItemData(Int nIndex) const
{
	assert(::IsWindow(m_hWnd));
	return (uintptr_t)::SendMessage(m_hWnd, CB_GETITEMDATA, (WPARAM)nIndex, 0L);
}

INLINE Int CXComboBox::SetItemData(Int nIndex, uintptr_t utItemData)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, CB_SETITEMDATA, (WPARAM)nIndex, (LPARAM)utItemData);
}

INLINE PVOID CXComboBox::GetItemDataPtr(Int nIndex) const
{
	assert(::IsWindow(m_hWnd));
	return (void*)GetItemData(nIndex);
}

INLINE Int CXComboBox::SetItemDataPtr(Int nIndex, void* pData)
{
	assert(::IsWindow(m_hWnd));
	return SetItemData(nIndex, (uintptr_t)pData);
}

INLINE Int CXComboBox::GetLBText(Int nIndex, PXStr pszText) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, CB_GETLBTEXT, (WPARAM)nIndex, (LPARAM)pszText);
}

INLINE Int CXComboBox::GetLBTextLen(Int nIndex) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, CB_GETLBTEXTLEN, (WPARAM)nIndex, 0L);
}

INLINE Int CXComboBox::GetItemHeight(Int nIndex) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, CB_GETITEMHEIGHT, (WPARAM)nIndex, 0L);
}

INLINE Int CXComboBox::SetItemHeight(Int nIndex, UInt uItemHeight)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, CB_SETITEMHEIGHT, (WPARAM)nIndex, MAKELPARAM(uItemHeight, 0));
}

INLINE BOOL CXComboBox::GetExtendedUI(void) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, CB_GETEXTENDEDUI, 0, 0L);
}

INLINE Int CXComboBox::SetExtendedUI(BOOL bExtended)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, CB_SETEXTENDEDUI, (WPARAM)bExtended, 0L);
}

INLINE void CXComboBox::GetDroppedControlRect(LPRECT lprect) const
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, CB_GETDROPPEDCONTROLRECT, 0, (LPARAM)lprect);
}

INLINE BOOL CXComboBox::GetDroppedState(void) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, CB_GETDROPPEDSTATE, 0, 0L);
}
#if (_WIN32_WINNT >= 0x0501)
INLINE Int CXComboBox::GetMinVisible(void) const
{
	assert(::IsWindow(m_hWnd));
	return (int)::SendMessage(m_hWnd, CB_GETMINVISIBLE, 0, 0L);
}

INLINE BOOL CXComboBox::SetMinVisible(Int nMinVisible)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, CB_SETMINVISIBLE, nMinVisible, 0L);
}
// Vista only
INLINE BOOL CXComboBox::GetCueBannerText(PWStr pszText, Int nText) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, CB_GETCUEBANNER, (WPARAM)pszText, nText);
}
// Vista only
INLINE BOOL CXComboBox::SetCueBannerText(PCWStr pszText)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, CB_SETCUEBANNER, 0, (LPARAM)pszText);
}
#endif // (_WIN32_WINNT >= 0x0501)
// Operations
INLINE Int CXComboBox::InitStorage(Int nItems, UInt uBytes)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, CB_INITSTORAGE, (WPARAM)nItems, (LPARAM)uBytes);
}

INLINE void CXComboBox::ResetContent(void)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, CB_RESETCONTENT, 0, 0L);
}
// for edit control
INLINE BOOL CXComboBox::LimitText(Int nMaxChars)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, CB_LIMITTEXT, (WPARAM)nMaxChars, 0L);
}
// for drop-down combo boxes
INLINE void CXComboBox::ShowDropDown(BOOL bShowIt)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, CB_SHOWDROPDOWN, (WPARAM)bShowIt, 0L);
}
// manipulating listbox items
INLINE Int CXComboBox::AddString(PCXStr pszString)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, CB_ADDSTRING, 0, (LPARAM)pszString);
}

INLINE Int CXComboBox::DeleteString(Int nIndex)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, CB_DELETESTRING, (WPARAM)nIndex, 0L);
}

INLINE Int CXComboBox::InsertString(Int nIndex, PCXStr pszString)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, CB_INSERTSTRING, (WPARAM)nIndex, (LPARAM)pszString);
}

INLINE Int CXComboBox::Dir(UInt uAttr, PCXStr pszWildCard)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, CB_DIR, (WPARAM)uAttr, (LPARAM)pszWildCard);
}
// selection helpers
INLINE Int CXComboBox::FindString(Int nStartAfter, PCXStr pszString) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, CB_FINDSTRING, (WPARAM)nStartAfter, (LPARAM)pszString);
}

INLINE Int CXComboBox::FindStringExact(Int nIndexStart, PCXStr pszFind) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, CB_FINDSTRINGEXACT, (WPARAM)nIndexStart, (LPARAM)pszFind);
}

INLINE Int CXComboBox::SelectString(Int nStartAfter, PCXStr pszString)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, CB_SELECTSTRING, (WPARAM)nStartAfter, (LPARAM)pszString);
}
// Clipboard operations
INLINE void CXComboBox::Clear(void)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, WM_CLEAR, 0, 0L);
}

INLINE void CXComboBox::Copy(void)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, WM_COPY, 0, 0L);
}

INLINE void CXComboBox::Cut(void)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, WM_CUT, 0, 0L);
}

INLINE void CXComboBox::Paste(void)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, WM_PASTE, 0, 0L);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXComboBoxEx : WC_COMBOBOXEX
INLINE CXComboBoxEx::CXComboBoxEx(void)
: CXComboBox(XWNDT_COMBOBOXEX)
{
}

INLINE CXComboBoxEx::~CXComboBoxEx(void)
{
}

INLINE CXComboBoxEx::CXComboBoxEx(const CXComboBoxEx&)
: CXComboBox(XWNDT_COMBOBOXEX)
{
}

INLINE CXComboBoxEx& CXComboBoxEx::operator=(const CXComboBoxEx&)
{
	return (*this);
}

INLINE PCXStr CXComboBoxEx::GetXWndClass(void)
{
	return WC_COMBOBOXEX;
}
	
INLINE HIMAGELIST CXComboBoxEx::GetImageList(void) const
{
	assert(::IsWindow(m_hWnd));
	return (HIMAGELIST)::SendMessage(m_hWnd, CBEM_GETIMAGELIST, 0, 0L);
}

INLINE HIMAGELIST CXComboBoxEx::SetImageList(HIMAGELIST hImageList)
{
	assert(::IsWindow(m_hWnd));
	return (HIMAGELIST)::SendMessage(m_hWnd, CBEM_SETIMAGELIST, 0, (LPARAM)hImageList);
}

INLINE ULong CXComboBoxEx::GetExtendedStyle(void) const
{
	assert(::IsWindow(m_hWnd));
	return (ULong)::SendMessage(m_hWnd, CBEM_GETEXTENDEDSTYLE, 0, 0L);
}

INLINE ULong CXComboBoxEx::SetExtendedStyle(ULong ulExMask, ULong ulExStyle)
{
	assert(::IsWindow(m_hWnd));
	return (ULong)::SendMessage(m_hWnd, CBEM_SETEXTENDEDSTYLE, (WPARAM)ulExMask, (LPARAM)ulExStyle);
}

INLINE BOOL CXComboBoxEx::GetUnicodeFormat(void) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, CBEM_GETUNICODEFORMAT, 0, 0L);
}

INLINE BOOL CXComboBoxEx::SetUnicodeFormat(BOOL bUnicode)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, CBEM_SETUNICODEFORMAT, (WPARAM)bUnicode, 0L);
}

// Operations
INLINE Int CXComboBoxEx::InsertItem(const COMBOBOXEXITEM FAR* lpcCBItem)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, CBEM_INSERTITEM, 0, (LPARAM)lpcCBItem);
}

INLINE Int CXComboBoxEx::DeleteItem(Int nIndex)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, CBEM_DELETEITEM, (WPARAM)nIndex, 0L);
}

INLINE BOOL CXComboBoxEx::GetItem(PCOMBOBOXEXITEM pCBItem) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, CBEM_GETITEM, 0, (LPARAM)pCBItem);
}

INLINE BOOL CXComboBoxEx::SetItem(const COMBOBOXEXITEM FAR* lpcCBItem)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, CBEM_SETITEM, 0, (LPARAM)lpcCBItem);
}

INLINE HWND CXComboBoxEx::GetComboCtrl(void) const
{
	assert(::IsWindow(m_hWnd));
	return (HWND)::SendMessage(m_hWnd, CBEM_GETCOMBOCONTROL, 0, 0L);
}

INLINE HWND CXComboBoxEx::GetEditCtrl(void) const
{
	assert(::IsWindow(m_hWnd));
	return (HWND)::SendMessage(m_hWnd, CBEM_GETEDITCONTROL, 0, 0L);
}

INLINE BOOL CXComboBoxEx::HasEditChanged(void) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, CBEM_HASEDITCHANGED, 0, 0L);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXScrollBar : WC_SCROLLBAR
INLINE CXScrollBar::CXScrollBar(void)
: CXControl(XWNDT_SCROLLBAR)
{
}

INLINE CXScrollBar::~CXScrollBar(void)
{
}

INLINE CXScrollBar::CXScrollBar(const CXScrollBar&)
: CXControl(XWNDT_SCROLLBAR)
{
}

INLINE CXScrollBar& CXScrollBar::operator=(const CXScrollBar&)
{
	return (*this);
}

INLINE PCXStr CXScrollBar::GetXWndClass(void)
{
	return WC_SCROLLBAR;
}

INLINE Int CXScrollBar::GetScrollPos(void) const
{
	assert(::IsWindow(m_hWnd));
	return ::GetScrollPos(m_hWnd, SB_CTL);
}

INLINE Int CXScrollBar::SetScrollPos(Int nPos, BOOL bRedraw)
{
	assert(::IsWindow(m_hWnd));
	return ::SetScrollPos(m_hWnd, SB_CTL, nPos, bRedraw);
}

INLINE void CXScrollBar::GetScrollRange(PInt lpnMinPos, PInt lpnMaxPos) const
{
	assert(::IsWindow(m_hWnd));
	::GetScrollRange(m_hWnd, SB_CTL, lpnMinPos, lpnMaxPos);
}

INLINE void CXScrollBar::SetScrollRange(Int nMinPos, Int nMaxPos, BOOL bRedraw)
{
	assert(::IsWindow(m_hWnd));
	::SetScrollRange(m_hWnd, SB_CTL, nMinPos, nMaxPos, bRedraw);
}

INLINE BOOL CXScrollBar::GetScrollInfo(LPSCROLLINFO lpScrollInfo) const
{
	assert(::IsWindow(m_hWnd));
	return ::GetScrollInfo(m_hWnd, SB_CTL, lpScrollInfo);
}

INLINE Int CXScrollBar::SetScrollInfo(LPSCROLLINFO lpScrollInfo, BOOL bRedraw)
{
	assert(::IsWindow(m_hWnd));
	return ::SetScrollInfo(m_hWnd, SB_CTL, lpScrollInfo, bRedraw);
}
#if (WINVER >= 0x0500)
INLINE BOOL CXScrollBar::GetScrollBarInfo(PSCROLLBARINFO pScrollBarInfo) const
{
	assert(::IsWindow(m_hWnd));
#if (_WIN32_WINNT >= 0x0501)
	return (BOOL)::SendMessage(m_hWnd, SBM_GETSCROLLBARINFO, 0, (LPARAM)pScrollBarInfo);
#else // !(_WIN32_WINNT >= 0x0501)
	return ::GetScrollBarInfo(m_hWnd, OBJID_CLIENT, pScrollBarInfo);
#endif // !(_WIN32_WINNT >= 0x0501)
}
#endif // (WINVER >= 0x0500)
// Operations
INLINE void CXScrollBar::ShowScrollBar(BOOL bShow)
{
	assert(::IsWindow(m_hWnd));
	::ShowScrollBar(m_hWnd, SB_CTL, bShow);
}

INLINE BOOL CXScrollBar::EnableScrollBar(UInt uArrowFlags)
{
	assert(::IsWindow(m_hWnd));
	return ::EnableScrollBar(m_hWnd, SB_CTL, uArrowFlags);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXTab : WC_TABCONTROL
INLINE CXTab::CXTab(void)
: CXControl(XWNDT_TAB)
{
}

INLINE CXTab::~CXTab(void)
{
}

INLINE CXTab::CXTab(const CXTab&)
: CXControl(XWNDT_TAB)
{
}

INLINE CXTab& CXTab::operator=(const CXTab&)
{
	return (*this);
}

INLINE PCXStr CXTab::GetXWndClass(void)
{
	return  WC_TABCONTROL;
}

INLINE HIMAGELIST CXTab::GetImageList(void) const
{
	assert(::IsWindow(m_hWnd));
	return (HIMAGELIST)::SendMessage(m_hWnd, TCM_GETIMAGELIST, 0, 0L);
}

INLINE HIMAGELIST CXTab::SetImageList(HIMAGELIST hImageList)
{
	assert(::IsWindow(m_hWnd));
	return (HIMAGELIST)::SendMessage(m_hWnd, TCM_SETIMAGELIST, 0, (LPARAM)hImageList);
}

INLINE Int CXTab::GetItemCount(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TCM_GETITEMCOUNT, 0, 0L);
}

INLINE BOOL CXTab::GetItem(Int nItem, LPTCITEM pTabCtrlItem) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TCM_GETITEM, (WPARAM)nItem, (LPARAM)pTabCtrlItem);
}

INLINE BOOL CXTab::SetItem(Int nItem, LPTCITEM pTabCtrlItem)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TCM_SETITEM, (WPARAM)nItem, (LPARAM)pTabCtrlItem);
}

INLINE BOOL CXTab::GetItemRect(Int nItem, LPRECT lpRect) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TCM_GETITEMRECT, (WPARAM)nItem, (LPARAM)lpRect);
}

INLINE Int CXTab::GetCurSel(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TCM_GETCURSEL, 0, 0L);
}

INLINE Int CXTab::SetCurSel(Int nItem)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TCM_SETCURSEL, (WPARAM)nItem, 0L);
}

INLINE SIZE CXTab::SetItemSize(SIZE& size)
{
	assert(::IsWindow(m_hWnd));
	ULong ulSize = (ULong)::SendMessage(m_hWnd, TCM_SETITEMSIZE, 0, MAKELPARAM(size.cx, size.cy));
	SIZE sizeRet = { GET_X_LPARAM(ulSize), GET_Y_LPARAM(ulSize) };
	return sizeRet;
}

INLINE void CXTab::SetPadding(SIZE& size)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TCM_SETPADDING, 0, MAKELPARAM(size.cx, size.cy));
}

INLINE Int CXTab::GetRowCount(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TCM_GETROWCOUNT, 0, 0L);
}

INLINE HWND CXTab::GetTooltips(void) const
{
	assert(::IsWindow(m_hWnd));
	return (HWND)::SendMessage(m_hWnd, TCM_GETTOOLTIPS, 0, 0L);
}

INLINE void CXTab::SetTooltips(HWND hWndToolTip)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TCM_SETTOOLTIPS, (WPARAM)hWndToolTip, 0L);
}

INLINE Int CXTab::GetCurFocus(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TCM_GETCURFOCUS, 0, 0L);
}

INLINE void CXTab::SetCurFocus(Int nItem)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TCM_SETCURFOCUS, (WPARAM)nItem, 0L);
}

INLINE BOOL CXTab::SetItemExtra(Int cbExtra)
{
	assert(::IsWindow(m_hWnd));
	assert(GetItemCount() == 0);   // must be empty
	return (BOOL)::SendMessage(m_hWnd, TCM_SETITEMEXTRA, (WPARAM)cbExtra, 0L);
}

INLINE Int CXTab::SetMinTabWidth(Int nWidth)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TCM_SETMINTABWIDTH, 0, (LPARAM)nWidth);
}

INLINE ULong CXTab::GetExtendedStyle(void) const
{
	assert(::IsWindow(m_hWnd));
	return (ULong)::SendMessage(m_hWnd, TCM_GETEXTENDEDSTYLE, 0, 0L);
}

INLINE ULong CXTab::SetExtendedStyle(ULong ulExMask, ULong ulExStyle)
{
	assert(::IsWindow(m_hWnd));
	return (ULong)::SendMessage(m_hWnd, TCM_SETEXTENDEDSTYLE, (WPARAM)ulExMask, (LPARAM)ulExStyle);
}

INLINE BOOL CXTab::GetUnicodeFormat(void) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TCM_GETUNICODEFORMAT, 0, 0L);
}

INLINE BOOL CXTab::SetUnicodeFormat(BOOL bUnicode)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TCM_SETUNICODEFORMAT, (WPARAM)bUnicode, 0L);
}
// Operations
INLINE Int CXTab::InsertItem(Int nItem, LPTCITEM pTabCtrlItem)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TCM_INSERTITEM, (WPARAM)nItem, (LPARAM)pTabCtrlItem);
}

INLINE BOOL CXTab::DeleteItem(Int nItem)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TCM_DELETEITEM, (WPARAM)nItem, 0L);
}

INLINE BOOL CXTab::DeleteAllItems(void)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TCM_DELETEALLITEMS, 0, 0L);
}

INLINE void CXTab::AdjustRect(BOOL bLarger, LPRECT lpRect)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TCM_ADJUSTRECT, (WPARAM)bLarger, (LPARAM)lpRect);
}

INLINE void CXTab::RemoveImage(Int nImage)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TCM_REMOVEIMAGE, (WPARAM)nImage, 0L);
}

INLINE Int CXTab::HitTest(TC_HITTESTINFO* pHitTestInfo) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TCM_HITTEST, 0, (LPARAM)pHitTestInfo);
}

INLINE void CXTab::DeselectAll(BOOL bExcludeFocus)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TCM_DESELECTALL, (WPARAM)bExcludeFocus, 0L);
}

INLINE BOOL CXTab::HighlightItem(Int nIndex, BOOL bHighlight)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TCM_HIGHLIGHTITEM, (WPARAM)nIndex, MAKELPARAM(bHighlight, 0));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXToolTip : TOOLTIPS_CLASS
INLINE CXToolTip::CXToolTip() 
: CXControl(XWNDT_TOOLTIP)
{
}

INLINE CXToolTip::~CXToolTip() 
{
}

INLINE CXToolTip::CXToolTip(const CXToolTip&)
: CXControl(XWNDT_TOOLTIP)
{
}

INLINE CXToolTip& CXToolTip::operator=(const CXToolTip&)
{
	return (*this);
}

INLINE PCXStr CXToolTip::GetXWndClass(void)
{
	return  TOOLTIPS_CLASS;
}

INLINE void CXToolTip::GetText(LPTOOLINFO lpToolInfo) const
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TTM_GETTEXT, 0, (LPARAM)&lpToolInfo);
}

INLINE BOOL CXToolTip::GetToolInfo(LPTOOLINFO lpToolInfo) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TTM_GETTOOLINFO, 0, (LPARAM)lpToolInfo);
}

INLINE void CXToolTip::SetToolInfo(LPTOOLINFO lpToolInfo)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TTM_SETTOOLINFO, 0, (LPARAM)lpToolInfo);
}

INLINE void CXToolTip::SetToolRect(LPTOOLINFO lpToolInfo)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TTM_NEWTOOLRECT, 0, (LPARAM)lpToolInfo);
}

INLINE Int CXToolTip::GetToolCount(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TTM_GETTOOLCOUNT, 0, 0L);
}

INLINE Int CXToolTip::GetDelayTime(ULong ulType) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TTM_GETDELAYTIME, (WPARAM)ulType, 0L);
}

INLINE void CXToolTip::SetDelayTime(ULong ulType, Int nTime)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TTM_SETDELAYTIME, (WPARAM)ulType, MAKELPARAM(nTime, 0));
}

INLINE void CXToolTip::GetMargin(LPRECT lpRect) const
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TTM_GETMARGIN, 0, (LPARAM)lpRect);
}

INLINE void CXToolTip::SetMargin(LPRECT lpRect)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TTM_SETMARGIN, 0, (LPARAM)lpRect);
}

INLINE Int CXToolTip::GetMaxTipWidth(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TTM_GETMAXTIPWIDTH, 0, 0L);
}

INLINE Int CXToolTip::SetMaxTipWidth(Int nWidth)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TTM_SETMAXTIPWIDTH, 0, (LPARAM)nWidth);
}

INLINE COLORREF CXToolTip::GetTipBkColor(void) const
{
	assert(::IsWindow(m_hWnd));
	return (COLORREF)::SendMessage(m_hWnd, TTM_GETTIPBKCOLOR, 0, 0L);
}

INLINE void CXToolTip::SetTipBkColor(COLORREF clr)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TTM_SETTIPBKCOLOR, (WPARAM)clr, 0L);
}

INLINE COLORREF CXToolTip::GetTipTextColor(void) const
{
	assert(::IsWindow(m_hWnd));
	return (COLORREF)::SendMessage(m_hWnd, TTM_GETTIPTEXTCOLOR, 0, 0L);
}

INLINE void CXToolTip::SetTipTextColor(COLORREF clr)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TTM_SETTIPTEXTCOLOR, (WPARAM)clr, 0L);
}

INLINE BOOL CXToolTip::GetCurrentTool(LPTOOLINFO lpToolInfo) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TTM_GETCURRENTTOOL, 0, (LPARAM)lpToolInfo);
}

INLINE SIZE CXToolTip::GetBubbleSize(LPTOOLINFO lpToolInfo) const
{
	assert(::IsWindow(m_hWnd));
	ULong ulRet = (ULong)::SendMessage(m_hWnd, TTM_GETBUBBLESIZE, 0, (LPARAM)lpToolInfo);
	SIZE size = { GET_X_LPARAM(ulRet), GET_Y_LPARAM(ulRet) };
	return size;
}

INLINE BOOL CXToolTip::SetTitle(UInt uIcon, PCXStr pszTitle)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TTM_SETTITLE, (WPARAM)uIcon, (LPARAM)pszTitle);
}
#if (_WIN32_WINNT >= 0x0501)
INLINE void CXToolTip::GetTitle(PTTGETTITLE pTTGetTitle) const
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TTM_GETTITLE, 0, (LPARAM)pTTGetTitle);
}

INLINE void CXToolTip::SetWindowTheme(PCWStr pszTheme)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TTM_SETWINDOWTHEME, 0, (LPARAM)pszTheme);
}
#endif // (_WIN32_WINNT >= 0x0501)
INLINE void CXToolTip::Activate(BOOL bActivate)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TTM_ACTIVATE, (WPARAM)bActivate, 0L);
}

INLINE BOOL CXToolTip::AddTool(LPTOOLINFO lpToolInfo)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TTM_ADDTOOL, 0, (LPARAM)lpToolInfo);
}

INLINE void CXToolTip::DelTool(LPTOOLINFO lpToolInfo)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TTM_DELTOOL, 0, (LPARAM)lpToolInfo);
}

INLINE BOOL CXToolTip::HitTest(LPTTHITTESTINFO lpHitTestInfo) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TTM_HITTEST, 0, (LPARAM)lpHitTestInfo);
}

INLINE void CXToolTip::RelayEvent(LPMSG lpMsg)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TTM_RELAYEVENT, 0, (LPARAM)lpMsg);
}

INLINE void CXToolTip::UpdateTipText(LPTOOLINFO lpToolInfo)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TTM_UPDATETIPTEXT, 0, (LPARAM)lpToolInfo);
}

INLINE BOOL CXToolTip::EnumTools(UInt uTool, LPTOOLINFO lpToolInfo) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TTM_ENUMTOOLS, (WPARAM)uTool, (LPARAM)lpToolInfo);
}

INLINE void CXToolTip::Pop(void)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TTM_POP, 0, 0L);
}

INLINE void CXToolTip::TrackActivate(LPTOOLINFO lpToolInfo, BOOL bActivate)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TTM_TRACKACTIVATE, (WPARAM)bActivate, (LPARAM)lpToolInfo);
}

INLINE void CXToolTip::TrackPosition(Int nxPos, Int nyPos)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TTM_TRACKPOSITION, 0, MAKELPARAM(nxPos, nyPos));
}

INLINE void CXToolTip::Update(void)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TTM_UPDATE, 0, 0L);
}

INLINE BOOL CXToolTip::AdjustRect(LPRECT lpRect, BOOL bLarger)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TTM_ADJUSTRECT, (WPARAM)bLarger, (LPARAM)lpRect);
}
#if (_WIN32_WINNT >= 0x0501)
INLINE void CXToolTip::Popup(void)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TTM_POPUP, 0, 0L);
}
#endif // (_WIN32_WINNT >= 0x0501)

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXListView : WC_LISTVIEW
INLINE CXListView::CXListView(void)
: CXControl(XWNDT_LISTVIEW)
{
}

INLINE CXListView::~CXListView(void)
{
}

INLINE CXListView::CXListView(const CXListView&)
: CXControl(XWNDT_LISTVIEW)
{
}

INLINE CXListView& CXListView::operator=(const CXListView&)
{
	return (*this);
}

INLINE PCXStr CXListView::GetXWndClass(void)
{
	return  WC_LISTVIEW;
}

INLINE COLORREF CXListView::GetBkColor(void) const
{
	assert(::IsWindow(m_hWnd));
	return (COLORREF)::SendMessage(m_hWnd, LVM_GETBKCOLOR, 0, 0L);
}

INLINE BOOL CXListView::SetBkColor(COLORREF clr)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_SETBKCOLOR, 0, clr);
}

INLINE HIMAGELIST CXListView::GetImageList(Int nType) const
{
	assert(::IsWindow(m_hWnd));
	return (HIMAGELIST)::SendMessage(m_hWnd, LVM_GETIMAGELIST, nType, 0L);
}

INLINE HIMAGELIST CXListView::SetImageList(HIMAGELIST hImg, Int nType)
{
	assert(::IsWindow(m_hWnd)); 
	return (HIMAGELIST)::SendMessage(m_hWnd, LVM_SETIMAGELIST, nType, (LPARAM)hImg);
}

INLINE Int CXListView::GetItemCount(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LVM_GETITEMCOUNT, 0, 0L);
}

INLINE BOOL CXListView::SetItemCount(Int nItems)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_SETITEMCOUNT, nItems, 0L);
}

INLINE BOOL CXListView::GetItem(LPLVITEM pItem) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_GETITEM, 0, (LPARAM)pItem);
}

INLINE BOOL CXListView::SetItem(const LVITEM* pItem)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_SETITEM, 0, (LPARAM)pItem);
}

INLINE BOOL CXListView::SetItem(Int nItem, Int nSubItem, 
	                            UInt uMask, PCXStr pszItem,
								Int nImage, UInt uState, 
								UInt uStateMask, LPARAM lParam)
{
	assert(::IsWindow(m_hWnd));
	LVITEM lvi = { 0 };
	lvi.mask = uMask;
	lvi.iItem = nItem;
	lvi.iSubItem = nSubItem;
	lvi.stateMask = uStateMask;
	lvi.state = uState;
	lvi.pszText = (PXStr)pszItem;
	lvi.iImage = nImage;
	lvi.lParam = lParam;
	return (BOOL)::SendMessage(m_hWnd, LVM_SETITEM, 0, (LPARAM)&lvi);
}

INLINE UInt CXListView::GetItemState(Int nItem, UInt uMask) const
{
	assert(::IsWindow(m_hWnd));
	return (UInt)::SendMessage(m_hWnd, LVM_GETITEMSTATE, nItem, uMask);
}

INLINE BOOL CXListView::SetItemState(Int nItem, UInt uState, UInt uStateMask)
{
	assert(::IsWindow(m_hWnd));
	LVITEM lvi = { 0 };
	lvi.state     = uState;
	lvi.stateMask = uStateMask;
	return (BOOL)::SendMessage(m_hWnd, LVM_SETITEMSTATE, nItem, (LPARAM)&lvi);
}

INLINE BOOL CXListView::SetItemState(Int nItem, LPLVITEM pItem)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_SETITEMSTATE, nItem, (LPARAM)pItem);
}

INLINE Int CXListView::GetItemText(Int nItem, Int nSubItem, PXStr pszText, Int nLen) const
{
	assert(::IsWindow(m_hWnd));
	LVITEM lvi = { 0 };
	lvi.iSubItem   = nSubItem;
	lvi.cchTextMax = nLen;
	lvi.pszText    = pszText;
	return (Int)::SendMessage(m_hWnd, LVM_GETITEMTEXT, (WPARAM)nItem, (LPARAM)&lvi);
}

INLINE BOOL CXListView::SetItemText(Int nItem, Int nSubItem, PCXStr pszText)
{
	assert(::IsWindow(m_hWnd));
	return SetItem(nItem, nSubItem, LVIF_TEXT, pszText, 0, 0, 0, 0);
}

INLINE uintptr_t CXListView::GetItemData(Int nItem) const
{
	assert(::IsWindow(m_hWnd));
	LVITEM lvi = { 0 };
	lvi.iItem = nItem;
	lvi.mask  = LVIF_PARAM;
	BOOL bRet = (BOOL)::SendMessage(m_hWnd, LVM_GETITEM, 0, (LPARAM)&lvi);
	return (uintptr_t)(bRet ? lvi.lParam : 0);
}

INLINE BOOL CXListView::SetItemData(Int nItem, uintptr_t utData)
{
	assert(::IsWindow(m_hWnd));
	return SetItem(nItem, 0, LVIF_PARAM, nullptr, 0, 0, 0, (LPARAM)utData);
}

INLINE UInt CXListView::GetCallbackMask(void) const
{
	assert(::IsWindow(m_hWnd));
	return (UInt)::SendMessage(m_hWnd, LVM_GETCALLBACKMASK, 0, 0L);
}

INLINE BOOL CXListView::SetCallbackMask(UInt uMask)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_SETCALLBACKMASK, uMask, 0L);
}

INLINE BOOL CXListView::GetItemPosition(Int nItem, LPPOINT lpPoint) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_GETITEMPOSITION, nItem, (LPARAM)lpPoint);
}

INLINE BOOL CXListView::SetItemPosition(Int nItem, POINT& pt)
{
	assert(::IsWindow(m_hWnd));
	assert(GetWindowLongPtr(GWL_STYLE) & (LVS_ICON|LVS_SMALLICON));
	return (BOOL)::SendMessage(m_hWnd, LVM_SETITEMPOSITION32, nItem, (LPARAM)&pt);
}

INLINE BOOL CXListView::SetItemPosition(Int nItem, Int nx, Int ny)
{
	assert(::IsWindow(m_hWnd));
	assert(GetWindowLongPtr(GWL_STYLE) & (LVS_ICON|LVS_SMALLICON));
	POINT pt = { nx, ny };
	return (BOOL)::SendMessage(m_hWnd, LVM_SETITEMPOSITION32, nItem, (LPARAM)&pt);
}

INLINE Int CXListView::GetStringWidth(PCXStr psz) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LVM_GETSTRINGWIDTH, 0, (LPARAM)psz);
}

INLINE HWND CXListView::GetEditControl(void) const
{
	assert(::IsWindow(m_hWnd));
	return (HWND)::SendMessage(m_hWnd, LVM_GETEDITCONTROL, 0, 0L);
}

INLINE BOOL CXListView::GetColumn(Int nCol, LVCOLUMN* pColumn) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_GETCOLUMN, nCol, (LPARAM)pColumn);
}

INLINE BOOL CXListView::SetColumn(Int nCol, const LVCOLUMN* pColumn)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_SETCOLUMN, nCol, (LPARAM)pColumn);
}

INLINE Int CXListView::GetColumnWidth(Int nCol) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LVM_GETCOLUMNWIDTH, nCol, 0L);
}

INLINE BOOL CXListView::SetColumnWidth(Int nCol, Int nWidth)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_SETCOLUMNWIDTH, nCol, MAKELPARAM(nWidth, 0));
}

INLINE BOOL CXListView::GetViewRect(LPRECT lpRect) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_GETVIEWRECT, 0, (LPARAM)lpRect);
}

INLINE COLORREF CXListView::GetTextColor(void) const
{
	assert(::IsWindow(m_hWnd));
	return (COLORREF)::SendMessage(m_hWnd, LVM_GETTEXTCOLOR, 0, 0L);
}

INLINE BOOL CXListView::SetTextColor(COLORREF clr)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_SETTEXTCOLOR, 0, clr);
}

INLINE COLORREF CXListView::GetTextBkColor(void) const
{
	assert(::IsWindow(m_hWnd));
	return (COLORREF)::SendMessage(m_hWnd, LVM_GETTEXTBKCOLOR, 0, 0L);
}

INLINE BOOL CXListView::SetTextBkColor(COLORREF clr)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_SETTEXTBKCOLOR, 0, clr);
}

INLINE Int CXListView::GetTopIndex(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LVM_GETTOPINDEX, 0, 0L);
}

INLINE Int CXListView::GetCountPerPage(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LVM_GETCOUNTPERPAGE, 0, 0L);
}

INLINE BOOL CXListView::GetOrigin(LPPOINT lpPoint) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_GETORIGIN, 0, (LPARAM)lpPoint);
}

INLINE UInt CXListView::GetSelectedCount(void) const
{
	assert(::IsWindow(m_hWnd));
	return (UInt)::SendMessage(m_hWnd, LVM_GETSELECTEDCOUNT, 0, 0L);
}

INLINE BOOL CXListView::GetItemRect(Int nItem, LPRECT lpRect, UInt uCode) const
{
	assert(::IsWindow(m_hWnd));
	if (lpRect != nullptr)
	{
		lpRect->left = uCode;
		return (BOOL)::SendMessage(m_hWnd, LVM_GETITEMRECT, (WPARAM)nItem, (LPARAM)lpRect);
	}
	return FALSE;
}

INLINE HCURSOR CXListView::GetHotCursor(void) const
{
	assert(::IsWindow(m_hWnd));
	return (HCURSOR)::SendMessage(m_hWnd, LVM_GETHOTCURSOR, 0, 0L);
}

INLINE HCURSOR CXListView::SetHotCursor(HCURSOR hHotCursor)
{
	assert(::IsWindow(m_hWnd));
	return (HCURSOR)::SendMessage(m_hWnd, LVM_SETHOTCURSOR, 0, (LPARAM)hHotCursor);
}

INLINE Int CXListView::GetHotItem(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LVM_GETHOTITEM, 0, 0L);
}

INLINE Int CXListView::SetHotItem(Int nIndex)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LVM_SETHOTITEM, nIndex, 0L);
}

INLINE BOOL CXListView::GetColumnOrderArray(Int nCount, PInt lpnArray) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_GETCOLUMNORDERARRAY, nCount, (LPARAM)lpnArray);
}

INLINE BOOL CXListView::SetColumnOrderArray(Int nCount, PInt lpnArray)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_SETCOLUMNORDERARRAY, nCount, (LPARAM)lpnArray);
}

INLINE HWND CXListView::GetHeader(void) const
{
	assert(::IsWindow(m_hWnd));
	return (HWND)::SendMessage(m_hWnd, LVM_GETHEADER, 0, 0L);
}

INLINE BOOL CXListView::GetSubItemRect(Int nItem, Int nSubItem, Int nFlag, LPRECT lpRect) const
{
	assert(::IsWindow(m_hWnd));
	assert(GetWindowLongPtr(GWL_STYLE) & LVS_REPORT);
	assert(lpRect != nullptr);
	lpRect->top  = nSubItem;
	lpRect->left = nFlag;
	return (BOOL)::SendMessage(m_hWnd, LVM_GETSUBITEMRECT, nItem, (LPARAM)lpRect);
}

INLINE ULong CXListView::SetIconSpacing(Int ncx, Int ncy)
{
	assert(::IsWindow(m_hWnd));
	assert(GetWindowLongPtr(GWL_STYLE) & LVS_ICON);
	return (ULong)::SendMessage(m_hWnd, LVM_SETICONSPACING, 0, MAKELPARAM(ncx, ncy));
}

INLINE Int CXListView::GetISearchString(PXStr pszBuf) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LVM_GETISEARCHSTRING, 0, (LPARAM)pszBuf);
}

INLINE void CXListView::GetItemSpacing(SIZE& size, BOOL bSmallIconView) const
{
	assert(::IsWindow(m_hWnd));
	ULong ulRet = (ULong)::SendMessage(m_hWnd, LVM_GETITEMSPACING, bSmallIconView, 0L);
	size.cx = GET_X_LPARAM(ulRet);
	size.cy = GET_Y_LPARAM(ulRet);
}

// single-selection only
INLINE Int CXListView::GetSelectedIndex(void) const
{
	assert(::IsWindow(m_hWnd));
	assert(GetWindowLongPtr(GWL_STYLE) & LVS_SINGLESEL);
	return (Int)::SendMessage(m_hWnd, LVM_GETNEXTITEM, (WPARAM)-1, MAKELPARAM(LVNI_ALL|LVNI_SELECTED, 0));
}

INLINE BOOL CXListView::GetSelectedItem(LPLVITEM pItem) const
{
	assert(::IsWindow(m_hWnd));
	assert(GetWindowLongPtr(GWL_STYLE) & LVS_SINGLESEL);
	assert(pItem != nullptr);
	pItem->iItem = (Int)::SendMessage(m_hWnd, LVM_GETNEXTITEM, (WPARAM)-1, MAKELPARAM(LVNI_ALL|LVNI_SELECTED, 0));
	if (pItem->iItem == -1)
	{
		return FALSE;
	}
	return (BOOL)::SendMessage(m_hWnd, LVM_GETITEM, 0, (LPARAM)pItem);
}

// extended list view styles
INLINE ULong CXListView::GetExtendedStyle(void) const
{
	assert(::IsWindow(m_hWnd));
	return (ULong)::SendMessage(m_hWnd, LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0L);
}

// ulExMask = 0 means all styles
INLINE ULong CXListView::SetExtendedStyle(ULong ulExStyle, ULong ulExMask)
{
	assert(::IsWindow(m_hWnd));
	return (ULong)::SendMessage(m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE, ulExMask, ulExStyle);
}

// checkboxes only
INLINE BOOL CXListView::GetCheckState(Int nIndex) const
{
	assert(::IsWindow(m_hWnd));
	assert((GetExtendedStyle() & LVS_EX_CHECKBOXES) != 0);
	UInt uRet = GetItemState(nIndex, LVIS_STATEIMAGEMASK);
	return (uRet >> 12) - 1;
}

INLINE BOOL CXListView::SetCheckState(Int nItem, BOOL bCheck)
{
	Int nCheck = bCheck ? 2 : 1;   // one based index
	return SetItemState(nItem, INDEXTOSTATEIMAGEMASK(nCheck), LVIS_STATEIMAGEMASK);
}

// view type
INLINE ULong CXListView::GetViewType(void) const
{
	assert(::IsWindow(m_hWnd));
	return (GetWindowLongPtr(GWL_STYLE) & LVS_TYPEMASK);
}

INLINE ULong CXListView::SetViewType(ULong ulType)
{
	assert(::IsWindow(m_hWnd));
	assert(ulType == LVS_ICON||ulType == LVS_SMALLICON||ulType == LVS_LIST||ulType == LVS_REPORT);
	ULong ulOldType = GetViewType();
	if (ulType != ulOldType)
	{
		ULong ul  = GetWindowLongPtr(GWL_STYLE) & (~LVS_TYPEMASK);
		ul &= (ulType & LVS_TYPEMASK);
		SetWindowLongPtr(GWL_STYLE, ul);
	}
	return ulOldType;
}

INLINE BOOL CXListView::GetBkImage(LPLVBKIMAGE plvbki) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_GETBKIMAGE, 0, (LPARAM)plvbki);
}

INLINE BOOL CXListView::SetBkImage(LPLVBKIMAGE plvbki)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_SETBKIMAGE, 0, (LPARAM)plvbki);
}

INLINE Int CXListView::GetSelectionMark(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LVM_GETSELECTIONMARK, 0, 0L);
}

INLINE Int CXListView::SetSelectionMark(Int nIndex)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LVM_SETSELECTIONMARK, 0, nIndex);
}

INLINE BOOL CXListView::GetWorkAreas(Int nWorkAreas, LPRECT lpRect) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_GETWORKAREAS, nWorkAreas, (LPARAM)lpRect);
}

INLINE BOOL CXListView::SetWorkAreas(Int nWorkAreas, LPRECT lpRect)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_SETWORKAREAS, nWorkAreas, (LPARAM)lpRect);
}

INLINE ULong CXListView::GetHoverTime(void) const
{
	assert(::IsWindow(m_hWnd));
	assert((GetExtendedStyle() & (LVS_EX_TRACKSELECT|LVS_EX_ONECLICKACTIVATE|LVS_EX_TWOCLICKACTIVATE)) != 0);
	return (ULong)::SendMessage(m_hWnd, LVM_GETHOVERTIME, 0, 0L);
}

INLINE ULong CXListView::SetHoverTime(ULong ulHoverTime)
{
	assert(::IsWindow(m_hWnd));
	assert((GetExtendedStyle() & (LVS_EX_TRACKSELECT|LVS_EX_ONECLICKACTIVATE|LVS_EX_TWOCLICKACTIVATE)) != 0);
	return (ULong)::SendMessage(m_hWnd, LVM_SETHOVERTIME, 0, ulHoverTime);
}

INLINE BOOL CXListView::GetNumberOfWorkAreas(PInt pnWorkAreas) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_GETNUMBEROFWORKAREAS, 0, (LPARAM)pnWorkAreas);
}

INLINE BOOL CXListView::SetItemCountEx(Int nItems, ULong ulFlags)
{
	assert(::IsWindow(m_hWnd));
#ifdef _DEBUG
	ULong ul = (ULong)GetWindowLongPtr(GWL_STYLE);
	assert((ul & LVS_OWNERDATA) != 0);
	assert((ul & LVS_TYPEMASK) == LVS_REPORT);
	assert((ul & LVS_TYPEMASK) == LVS_LIST);
#endif
	return (BOOL)::SendMessage(m_hWnd, LVM_SETITEMCOUNT, nItems, ulFlags);
}

INLINE HWND CXListView::GetToolTips(void) const
{
	assert(::IsWindow(m_hWnd));
	return (HWND)::SendMessage(m_hWnd, LVM_GETTOOLTIPS, 0, 0L);
}

INLINE HWND CXListView::SetToolTips(HWND hWndTT)
{
	assert(::IsWindow(m_hWnd));
	return (HWND)::SendMessage(m_hWnd, LVM_SETTOOLTIPS, (WPARAM)hWndTT, 0L);
}

INLINE BOOL CXListView::GetUnicodeFormat(void) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_GETUNICODEFORMAT, 0, 0L);
}

INLINE BOOL CXListView::SetUnicodeFormat(BOOL bUnicode)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_SETUNICODEFORMAT, bUnicode, 0L);
}

#if (_WIN32_WINNT >= 0x0501)
INLINE Int CXListView::GetSelectedColumn(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LVM_GETSELECTEDCOLUMN, 0, 0L);
}

INLINE void CXListView::SetSelectedColumn(Int nColumn)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, LVM_SETSELECTEDCOLUMN, nColumn, 0L);
}

INLINE ULong CXListView::GetView(void) const
{
	assert(::IsWindow(m_hWnd));
	return (ULong)::SendMessage(m_hWnd, LVM_GETVIEW, 0, 0L);
}

INLINE Int CXListView::SetView(ULong ulView)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LVM_SETVIEW, ulView, 0L);
}

INLINE BOOL CXListView::IsGroupViewEnabled(void) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_ISGROUPVIEWENABLED, 0, 0L);
}

INLINE Int CXListView::GetGroupInfo(Int nGroupID, PLVGROUP pGroup) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LVM_GETGROUPINFO, nGroupID, (LPARAM)pGroup);
}

INLINE Int CXListView::SetGroupInfo(Int nGroupID, PLVGROUP pGroup)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LVM_SETGROUPINFO, nGroupID, (LPARAM)pGroup);
}

INLINE void CXListView::GetGroupMetrics(PLVGROUPMETRICS pGroupMetrics) const
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, LVM_GETGROUPMETRICS, 0, (LPARAM)pGroupMetrics);
}

INLINE void CXListView::SetGroupMetrics(PLVGROUPMETRICS pGroupMetrics)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, LVM_SETGROUPMETRICS, 0, (LPARAM)pGroupMetrics);
}

INLINE void CXListView::GetTileViewInfo(PLVTILEVIEWINFO pTileViewInfo) const
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, LVM_GETTILEVIEWINFO, 0, (LPARAM)pTileViewInfo);
}

INLINE BOOL CXListView::SetTileViewInfo(PLVTILEVIEWINFO pTileViewInfo)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_SETTILEVIEWINFO, 0, (LPARAM)pTileViewInfo);
}

INLINE void CXListView::GetTileInfo(PLVTILEINFO pTileInfo) const
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, LVM_GETTILEINFO, 0, (LPARAM)pTileInfo);
}

INLINE BOOL CXListView::SetTileInfo(PLVTILEINFO pTileInfo)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_SETTILEINFO, 0, (LPARAM)pTileInfo);
}

INLINE BOOL CXListView::GetInsertMark(LPLVINSERTMARK pInsertMark) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_GETINSERTMARK, 0, (LPARAM)pInsertMark);
}

INLINE BOOL CXListView::SetInsertMark(LPLVINSERTMARK pInsertMark)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_SETINSERTMARK, 0, (LPARAM)pInsertMark);
}

INLINE Int CXListView::GetInsertMarkRect(LPRECT lpRect) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LVM_GETINSERTMARKRECT, 0, (LPARAM)lpRect);
}

INLINE COLORREF CXListView::GetInsertMarkColor(void) const
{
	assert(::IsWindow(m_hWnd));
	return (COLORREF)::SendMessage(m_hWnd, LVM_GETINSERTMARKCOLOR, 0, 0L);
}

INLINE COLORREF CXListView::SetInsertMarkColor(COLORREF clr)
{
	assert(::IsWindow(m_hWnd));
	return (COLORREF)::SendMessage(m_hWnd, LVM_SETINSERTMARKCOLOR, 0, clr);
}

INLINE COLORREF CXListView::GetOutlineColor(void) const
{
	assert(::IsWindow(m_hWnd));
	return (COLORREF)::SendMessage(m_hWnd, LVM_GETOUTLINECOLOR, 0, 0L);
}

INLINE COLORREF CXListView::SetOutlineColor(COLORREF clr)
{
	assert(::IsWindow(m_hWnd));
	return (COLORREF)::SendMessage(m_hWnd, LVM_SETOUTLINECOLOR, 0, clr);
}
#endif // (_WIN32_WINNT >= 0x0501)

#if (_WIN32_WINNT >= 0x0600)
INLINE Int CXListView::GetGroupCount(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LVM_GETGROUPCOUNT, 0, 0L);
}

INLINE BOOL CXListView::GetGroupInfoByIndex(Int nIndex, PLVGROUP pGroup) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_GETGROUPINFOBYINDEX, nIndex, (LPARAM)pGroup);
}

INLINE BOOL CXListView::GetGroupRect(Int nGroupID, Int nType, LPRECT lpRect) const
{
	assert(::IsWindow(m_hWnd));
	assert(lpRect != nullptr);
	if (lpRect != nullptr)
	{
		lpRect->top = nType;
	}
	return (BOOL)::SendMessage(m_hWnd, LVM_GETGROUPRECT, nGroupID, (LPARAM)lpRect);
}

INLINE UInt CXListView::GetGroupState(Int nGroupID, UInt uMask) const
{
	assert(::IsWindow(m_hWnd));
	return (UInt)::SendMessage(m_hWnd, LVM_GETGROUPSTATE, nGroupID, (LPARAM)uMask);
}

INLINE Int CXListView::GetFocusedGroup(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LVM_GETFOCUSEDGROUP, 0, 0L);
}

INLINE BOOL CXListView::GetEmptyText(PWStr pszText, Int nText) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_GETEMPTYTEXT, nText, (LPARAM)pszText);
}

INLINE BOOL CXListView::GetFooterRect(LPRECT lpRect) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_GETFOOTERRECT, 0, (LPARAM)lpRect);
}

INLINE BOOL CXListView::GetFooterInfo(LPLVFOOTERINFO lpFooterInfo) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_GETFOOTERINFO, 0, (LPARAM)lpFooterInfo);
}

INLINE BOOL CXListView::GetFooterItemRect(Int nItem, LPRECT lpRect) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_GETFOOTERITEMRECT, nItem, (LPARAM)lpRect);
}

INLINE BOOL CXListView::GetFooterItem(Int nItem, LPLVFOOTERITEM lpFooterItem) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_GETFOOTERITEM, nItem, (LPARAM)lpFooterItem);
}

INLINE BOOL CXListView::GetItemIndexRect(PLVITEMINDEX pItemIndex, Int nSubItem, Int nType, LPRECT lpRect) const
{
	assert(::IsWindow(m_hWnd));
	assert(pItemIndex != nullptr);
	assert(lpRect != nullptr);
	if (lpRect != nullptr)
	{
		lpRect->top = nSubItem;
		lpRect->left = nType;
	}
	return (BOOL)::SendMessage(m_hWnd, LVM_GETITEMINDEXRECT, (WPARAM)pItemIndex, (LPARAM)lpRect);
}

INLINE BOOL CXListView::SetItemIndexState(PLVITEMINDEX pItemIndex, UInt uState, UInt uMask)
{
	assert(::IsWindow(m_hWnd));
	LVITEM lvi = { 0 };
	lvi.state = uState;
	lvi.stateMask = uMask;
	return (BOOL)::SendMessage(m_hWnd, LVM_SETITEMINDEXSTATE, (WPARAM)pItemIndex, (LPARAM)&lvi);
}

INLINE BOOL CXListView::GetNextItemIndex(PLVITEMINDEX pItemIndex, UShort usFlags) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_GETNEXTITEMINDEX, (WPARAM)pItemIndex, MAKELPARAM(usFlags, 0));
}
#endif // (_WIN32_WINNT >= 0x0600)

// Operations
INLINE Int CXListView::InsertColumn(Int nCol, const LVCOLUMN* pColumn)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LVM_INSERTCOLUMN, nCol, (LPARAM)pColumn);
}

INLINE Int CXListView::InsertColumn(Int nCol, PCXStr psz, Int nFormat, Int nWidth, Int nSubItem, Int iImage, Int iOrder)
{
	LVCOLUMN column = { 0 };
	column.mask    = LVCF_TEXT|LVCF_FMT;
	column.pszText = (PXStr)psz;
	column.fmt     = nFormat;
	if (nWidth != -1)
	{
		column.mask |= LVCF_WIDTH;
		column.cx = nWidth;
	}
	if (nSubItem != -1)
	{
		column.mask |= LVCF_SUBITEM;
		column.iSubItem = nSubItem;
	}
	if (iImage != -1)
	{
		column.mask |= LVCF_IMAGE;
		column.iImage = iImage;
	}
	if (iOrder != -1)
	{
		column.mask |= LVCF_ORDER;
		column.iOrder = iOrder;
	}
	return InsertColumn(nCol, &column);
}

INLINE BOOL CXListView::DeleteColumn(Int nCol)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_DELETECOLUMN, nCol, 0L);
}

INLINE Int CXListView::InsertItem(UInt nMask, Int nItem, PCXStr pszItem, UInt uState, UInt uStateMask, Int nImage, LPARAM lParam)
{
	assert(::IsWindow(m_hWnd));
	LVITEM item = { 0 };
	item.mask      = nMask;
	item.iItem     = nItem;
	item.iSubItem  = 0;
	item.pszText   = (PXStr)pszItem;
	item.state     = uState;
	item.stateMask = uStateMask;
	item.iImage    = nImage;
	item.lParam    = lParam;
	return InsertItem(&item);
}

INLINE Int CXListView::InsertItem(const LVITEM* pItem)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LVM_INSERTITEM, 0, (LPARAM)pItem);
}

INLINE Int CXListView::InsertItem(Int nItem, PCXStr pszItem)
{
	assert(::IsWindow(m_hWnd));
	return InsertItem(LVIF_TEXT, nItem, pszItem, 0, 0, 0, 0);
}

INLINE Int CXListView::InsertItem(Int nItem, PCXStr pszItem, Int nImage)
{
	assert(::IsWindow(m_hWnd));
	return InsertItem(LVIF_TEXT|LVIF_IMAGE, nItem, pszItem, 0, 0, nImage, 0);
}

INLINE Int CXListView::GetNextItem(Int nItem, Int nFlags) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LVM_GETNEXTITEM, nItem, MAKELPARAM(nFlags, 0));
}

INLINE BOOL CXListView::DeleteItem(Int nItem)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_DELETEITEM, nItem, 0L);
}

INLINE BOOL CXListView::DeleteAllItems(void)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_DELETEALLITEMS, 0, 0L);
}

INLINE Int CXListView::FindItem(LVFINDINFO* pFindInfo, Int nStart) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LVM_FINDITEM, nStart, (LPARAM)pFindInfo);
}

INLINE Int CXListView::HitTest(LVHITTESTINFO* pHitTestInfo) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LVM_HITTEST, 0, (LPARAM)pHitTestInfo);
}

INLINE Int CXListView::HitTest(POINT& pt, PUInt puFlags) const
{
	assert(::IsWindow(m_hWnd));
	LVHITTESTINFO hti = { 0 };
	hti.pt = pt;
	Int nRet = (Int)::SendMessage(m_hWnd, LVM_HITTEST, 0, (LPARAM)&hti);
	if (puFlags != nullptr)
	{
		*puFlags = hti.flags;
	}
	return nRet;
}

INLINE BOOL CXListView::EnsureVisible(Int nItem, BOOL bPartialOK)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_ENSUREVISIBLE, nItem, MAKELPARAM(bPartialOK, 0));
}

INLINE BOOL CXListView::Scroll(SIZE& size)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_SCROLL, size.cx, size.cy);
}

INLINE BOOL CXListView::RedrawItems(Int nFirst, Int nLast)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_REDRAWITEMS, nFirst, nLast);
}

INLINE BOOL CXListView::Arrange(UInt uCode)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_ARRANGE, uCode, 0L);
}

INLINE HWND CXListView::EditLabel(Int nItem)
{
	assert(::IsWindow(m_hWnd));
	return (HWND)::SendMessage(m_hWnd, LVM_EDITLABEL, nItem, 0L);
}

INLINE BOOL CXListView::Update(Int nItem)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_UPDATE, nItem, 0L);
}

INLINE BOOL CXListView::SortItems(PFNLVCOMPARE pfnCompare, LPARAM lParamSort)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_SORTITEMS, (WPARAM)lParamSort, (LPARAM)pfnCompare);
}

INLINE HIMAGELIST CXListView::RemoveImageList(Int nType)
{
	assert(::IsWindow(m_hWnd));
	return (HIMAGELIST)::SendMessage(m_hWnd, LVM_SETIMAGELIST, (WPARAM)nType, 0);
}

INLINE HIMAGELIST CXListView::CreateDragImage(Int nItem, LPPOINT lpPoint)
{
	assert(::IsWindow(m_hWnd));
	return (HIMAGELIST)::SendMessage(m_hWnd, LVM_CREATEDRAGIMAGE, nItem, (LPARAM)lpPoint);
}

INLINE ULong CXListView::ApproximateViewRect(Int ncx, Int ncy, Int nCount)
{
	assert(::IsWindow(m_hWnd));
	return (ULong)::SendMessage(m_hWnd, LVM_APPROXIMATEVIEWRECT, nCount, MAKELPARAM(ncx, ncy));
}

INLINE Int CXListView::SubItemHitTest(LPLVHITTESTINFO lpInfo) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LVM_SUBITEMHITTEST, 0, (LPARAM)lpInfo);
}

INLINE Int CXListView::AddColumn(PCXStr pszItem, Int nItem, Int nWidth, Int nSubItem, Int nMask, Int nFmt)
{
	assert(::IsWindow(m_hWnd));
	LVCOLUMN lvc = { 0 };
	lvc.mask    = nMask;
	lvc.fmt     = nFmt;
	lvc.pszText = (PXStr)pszItem;
	lvc.cx      = nWidth;
	if (nMask & LVCF_SUBITEM)
	{
		lvc.iSubItem = (nSubItem != -1) ? nSubItem : nItem;
	}
	return InsertColumn(nItem, &lvc);
}

INLINE Int CXListView::AddItem(Int nItem, Int nSubItem, PCXStr pszItem, Int nImageIndex)
{
	assert(::IsWindow(m_hWnd));
	LVITEM lvItem = { 0 };
	lvItem.mask     = LVIF_TEXT;
	lvItem.iItem    = nItem;
	lvItem.iSubItem = nSubItem;
	lvItem.pszText  = (PXStr)pszItem;
	if (nImageIndex != -1)
	{
		lvItem.mask  |= LVIF_IMAGE;
		lvItem.iImage = nImageIndex;
	}
	if (nSubItem == 0)
	{
		return InsertItem(&lvItem);
	}
	return SetItem(&lvItem) ? nItem : -1;
}

#if (_WIN32_WINNT >= 0x0501)
INLINE Int CXListView::InsertGroup(Int nItem, PLVGROUP pGroup)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LVM_INSERTGROUP, nItem, (LPARAM)pGroup);
}

INLINE Int CXListView::AddGroup(PLVGROUP pGroup)
{
	return InsertGroup(-1, pGroup);
}

INLINE Int CXListView::RemoveGroup(Int nGroupID)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LVM_REMOVEGROUP, nGroupID, 0L);
}

INLINE void CXListView::MoveGroup(Int nGroupID, Int nItem)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, LVM_MOVEGROUP, nGroupID, nItem);
}

INLINE void CXListView::MoveItemToGroup(Int nItem, Int nGroupID)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, LVM_MOVEITEMTOGROUP, nItem, nGroupID);
}

INLINE Int CXListView::EnableGroupView(BOOL bEnable)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LVM_ENABLEGROUPVIEW, bEnable, 0L);
}

INLINE Int CXListView::SortGroups(PFNLVGROUPCOMPARE pCompareFunc, LPVOID lpVoid)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LVM_SORTGROUPS, (WPARAM)pCompareFunc, (LPARAM)lpVoid);
}

INLINE void CXListView::InsertGroupSorted(PLVINSERTGROUPSORTED pInsertGroupSorted)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, LVM_INSERTGROUPSORTED, (WPARAM)pInsertGroupSorted, 0L);
}

INLINE void CXListView::RemoveAllGroups(void)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, LVM_REMOVEALLGROUPS, 0, 0L);
}

INLINE BOOL CXListView::HasGroup(Int nGroupID)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_HASGROUP, nGroupID, 0L);
}

INLINE BOOL CXListView::InsertMarkHitTest(LPPOINT lpPoint, LPLVINSERTMARK pInsertMark) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_INSERTMARKHITTEST, (WPARAM)lpPoint, (LPARAM)pInsertMark);
}

INLINE BOOL CXListView::SetInfoTip(PLVSETINFOTIP pSetInfoTip)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LVM_SETINFOTIP, 0, (LPARAM)pSetInfoTip);
}

INLINE void CXListView::CancelEditLabel(void)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, LVM_CANCELEDITLABEL, 0, 0L);
}

INLINE UInt CXListView::MapIndexToID(Int nIndex) const
{
	assert(::IsWindow(m_hWnd));
	return (UInt)::SendMessage(m_hWnd, LVM_MAPINDEXTOID, nIndex, 0L);
}

INLINE Int CXListView::MapIDToIndex(UInt uID) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LVM_MAPIDTOINDEX, uID, 0L);
}
#endif // (_WIN32_WINNT >= 0x0501)

#if (_WIN32_WINNT >= 0x0600)
INLINE Int CXListView::HitTestEx(LPLVHITTESTINFO lpHitTestInfo) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LVM_HITTEST, (WPARAM)-1, (LPARAM)lpHitTestInfo);
}

INLINE Int CXListView::HitTestEx(POINT& pt, PUInt puFlags) const
{
	assert(::IsWindow(m_hWnd));
	LVHITTESTINFO hti = { 0 };
	hti.pt = pt;
	Int nRet = (Int)::SendMessage(m_hWnd, LVM_HITTEST, (WPARAM)-1, (LPARAM)&hti);
	if (puFlags != nullptr)
	{
		*puFlags = hti.flags;
	}
	return nRet;
}

INLINE Int CXListView::SubItemHitTestEx(LPLVHITTESTINFO lpHitTestInfo) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LVM_SUBITEMHITTEST, (WPARAM)-1, (LPARAM)lpHitTestInfo);
}
#endif // (_WIN32_WINNT >= 0x0600)

// single-selection only
INLINE BOOL CXListView::SelectItem(Int nIndex)
{
	assert(::IsWindow(m_hWnd));
	assert(GetWindowLongPtr(GWL_STYLE) & LVS_SINGLESEL);

	BOOL bRet = SetItemState(nIndex, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	if (bRet)
	{
		bRet = EnsureVisible(nIndex, FALSE);
	}
	return bRet;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXTreeView : WC_TREEVIEW
INLINE CXTreeView::CXTreeView(void)
: CXControl(XWNDT_TREEVIEW)
{
}

INLINE CXTreeView::~CXTreeView(void)
{
}

INLINE CXTreeView::CXTreeView(const CXTreeView&)
: CXControl(XWNDT_TREEVIEW)
{
}

INLINE CXTreeView& CXTreeView::operator=(const CXTreeView&)
{
	return (*this);
}

INLINE PCXStr CXTreeView::GetXWndClass(void)
{
	return  WC_TREEVIEW;
}

INLINE UInt CXTreeView::GetCount(void) const
{
	assert(::IsWindow(m_hWnd));
	return (UInt)::SendMessage(m_hWnd, TVM_GETCOUNT, 0, 0L);
}

INLINE UInt CXTreeView::GetIndent(void) const
{
	assert(::IsWindow(m_hWnd));
	return (UInt)::SendMessage(m_hWnd, TVM_GETINDENT, 0, 0L);
}

INLINE void CXTreeView::SetIndent(UInt uIndent)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TVM_SETINDENT, uIndent, 0L);
}

INLINE HIMAGELIST CXTreeView::GetImageList(Int nType) const
{
	assert(::IsWindow(m_hWnd));
	return (HIMAGELIST)::SendMessage(m_hWnd, TVM_GETIMAGELIST, (WPARAM)nType, 0L);
}

INLINE HIMAGELIST CXTreeView::SetImageList(HIMAGELIST hImageList, Int nType)
{
	assert(::IsWindow(m_hWnd));
	return (HIMAGELIST)::SendMessage(m_hWnd, TVM_SETIMAGELIST, (WPARAM)nType, (LPARAM)hImageList);
}

INLINE BOOL CXTreeView::GetItem(LPTVITEM pItem) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM)pItem);
}

INLINE BOOL CXTreeView::SetItem(LPTVITEM pItem)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TVM_SETITEM, 0, (LPARAM)pItem);
}

INLINE BOOL CXTreeView::SetItem(HTREEITEM hItem, UInt uMask, PCXStr pszItem, Int nImage, Int nSelectedImage, UInt uState, UInt uStateMask, LPARAM lParam)
{
	assert(::IsWindow(m_hWnd));
	TVITEM item = { 0 };
	item.hItem          = hItem;
	item.mask           = uMask;
	item.pszText        = (PXStr) pszItem;
	item.iImage         = nImage;
	item.iSelectedImage = nSelectedImage;
	item.state          = uState;
	item.stateMask      = uStateMask;
	item.lParam         = lParam;
	return (BOOL)::SendMessage(m_hWnd, TVM_SETITEM, 0, (LPARAM)&item);
}

INLINE BOOL CXTreeView::GetItemText(HTREEITEM hItem, PXStr pszText, Int nLen) const
{
	assert(::IsWindow(m_hWnd));
	assert(pszText != nullptr);

	TVITEM item = { 0 };
	item.hItem      = hItem;
	item.mask       = TVIF_TEXT;
	item.pszText    = pszText;
	item.cchTextMax = nLen;

	return (BOOL)::SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM)&item);
}

INLINE BOOL CXTreeView::SetItemText(HTREEITEM hItem, PCXStr pszItem)
{
	assert(::IsWindow(m_hWnd));
	return SetItem(hItem, TVIF_TEXT, pszItem, 0, 0, 0, 0, 0);
}

INLINE BOOL CXTreeView::GetItemImage(HTREEITEM hItem, Int& nImage, Int& nSelectedImage) const
{
	assert(::IsWindow(m_hWnd));
	TVITEM item = { 0 };
	item.hItem = hItem;
	item.mask  = TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	BOOL bRes  = (BOOL)::SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM)&item);
	if (bRes)
	{
		nImage         = item.iImage;
		nSelectedImage = item.iSelectedImage;
	}
	return bRes;
}

INLINE BOOL CXTreeView::SetItemImage(HTREEITEM hItem, Int nImage, Int nSelectedImage)
{
	assert(::IsWindow(m_hWnd));
	return SetItem(hItem, TVIF_IMAGE|TVIF_SELECTEDIMAGE, nullptr, nImage, nSelectedImage, 0, 0, 0);
}

INLINE UInt CXTreeView::GetItemState(HTREEITEM hItem, UInt uStateMask) const
{
	assert(::IsWindow(m_hWnd));
	return (((UInt)::SendMessage(m_hWnd, TVM_GETITEMSTATE, (WPARAM)hItem, (LPARAM)uStateMask)) & uStateMask);
}

INLINE BOOL CXTreeView::SetItemState(HTREEITEM hItem, UInt uState, UInt uStateMask)
{
	assert(::IsWindow(m_hWnd));
	return SetItem(hItem, TVIF_STATE, nullptr, 0, 0, uState, uStateMask, 0);
}

INLINE uintptr_t CXTreeView::GetItemData(HTREEITEM hItem) const
{
	assert(::IsWindow(m_hWnd));
	TVITEM item = { 0 };
	item.hItem = hItem;
	item.mask  = TVIF_PARAM;
	BOOL bRet  = (BOOL)::SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM)&item);
	return (uintptr_t)(bRet ? item.lParam : 0);
}

INLINE BOOL CXTreeView::SetItemData(HTREEITEM hItem, uintptr_t utData)
{
	assert(::IsWindow(m_hWnd));
	return SetItem(hItem, TVIF_PARAM, nullptr, 0, 0, 0, 0, (LPARAM)utData);
}

INLINE HWND CXTreeView::GetEditControl(void) const
{
	assert(::IsWindow(m_hWnd));
	return (HWND)::SendMessage(m_hWnd, TVM_GETEDITCONTROL, 0, 0L);
}

INLINE UInt CXTreeView::GetVisibleCount(void) const
{
	assert(::IsWindow(m_hWnd));
	return (UInt)::SendMessage(m_hWnd, TVM_GETVISIBLECOUNT, 0, 0L);
}

INLINE BOOL CXTreeView::GetItemRect(HTREEITEM hItem, LPRECT lpRect, BOOL bTextOnly) const
{
	assert(::IsWindow(m_hWnd));
	*(HTREEITEM*)lpRect = hItem;
	return (BOOL)::SendMessage(m_hWnd, TVM_GETITEMRECT, (WPARAM)bTextOnly, (LPARAM)lpRect);
}

INLINE BOOL CXTreeView::ItemHasChildren(HTREEITEM hItem) const
{
	assert(::IsWindow(m_hWnd));
	TVITEM item = { 0 };
	item.hItem = hItem;
	item.mask  = TVIF_CHILDREN;
	::SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM)&item);
	return item.cChildren;
}

INLINE HWND CXTreeView::GetToolTips(void) const
{
	assert(::IsWindow(m_hWnd));
	return (HWND)::SendMessage(m_hWnd, TVM_GETTOOLTIPS, 0, 0L);
}

INLINE HWND CXTreeView::SetToolTips(HWND hWndTT)
{
	assert(::IsWindow(m_hWnd));
	return (HWND)::SendMessage(m_hWnd, TVM_SETTOOLTIPS, (WPARAM)hWndTT, 0L);
}

INLINE Int CXTreeView::GetISearchString(PXStr psz) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TVM_GETISEARCHSTRING, 0, (LPARAM)psz);
}

// checkboxes only
INLINE BOOL CXTreeView::GetCheckState(HTREEITEM hItem) const
{
	assert(::IsWindow(m_hWnd));
	assert(GetWindowLongPtr(GWL_STYLE) & TVS_CHECKBOXES);
	UInt uRet = GetItemState(hItem, TVIS_STATEIMAGEMASK);
	return (uRet >> 12) - 1;
}

INLINE BOOL CXTreeView::SetCheckState(HTREEITEM hItem, BOOL bCheck)
{
	Int nCheck = bCheck ? 2 : 1;   // one based index
	return SetItemState(hItem, INDEXTOSTATEIMAGEMASK(nCheck), TVIS_STATEIMAGEMASK);
}

INLINE COLORREF CXTreeView::GetBkColor(void) const
{
	assert(::IsWindow(m_hWnd));
	return (COLORREF)::SendMessage(m_hWnd, TVM_GETBKCOLOR, 0, 0L);
}

INLINE COLORREF CXTreeView::SetBkColor(COLORREF clr)
{
	assert(::IsWindow(m_hWnd));
	return (COLORREF)::SendMessage(m_hWnd, TVM_SETBKCOLOR, 0, (LPARAM)clr);
}

INLINE COLORREF CXTreeView::GetInsertMarkColor(void) const
{
	assert(::IsWindow(m_hWnd));
	return (COLORREF)::SendMessage(m_hWnd, TVM_GETINSERTMARKCOLOR, 0, 0L);
}

INLINE COLORREF CXTreeView::SetInsertMarkColor(COLORREF clr)
{
	assert(::IsWindow(m_hWnd));
	return (COLORREF)::SendMessage(m_hWnd, TVM_SETINSERTMARKCOLOR, 0, (LPARAM)clr);
}

INLINE Int CXTreeView::GetItemHeight(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TVM_GETITEMHEIGHT, 0, 0L);
}

INLINE Int CXTreeView::SetItemHeight(Int nHeight)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TVM_SETITEMHEIGHT, nHeight, 0L);
}

INLINE Int CXTreeView::GetScrollTime(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TVM_GETSCROLLTIME, 0, 0L);
}

INLINE Int CXTreeView::SetScrollTime(Int nScrollTime)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TVM_SETSCROLLTIME, nScrollTime, 0L);
}

INLINE COLORREF CXTreeView::GetTextColor(void) const
{
	assert(::IsWindow(m_hWnd));
	return (COLORREF)::SendMessage(m_hWnd, TVM_GETTEXTCOLOR, 0, 0L);
}

INLINE COLORREF CXTreeView::SetTextColor(COLORREF clr)
{
	assert(::IsWindow(m_hWnd));
	return (COLORREF)::SendMessage(m_hWnd, TVM_SETTEXTCOLOR, 0, (LPARAM)clr);
}

INLINE BOOL CXTreeView::GetUnicodeFormat(void) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TVM_GETUNICODEFORMAT, 0, 0L);
}

INLINE BOOL CXTreeView::SetUnicodeFormat(BOOL bUnicode)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TVM_SETUNICODEFORMAT, bUnicode, 0L);
}

INLINE COLORREF CXTreeView::GetLineColor(void) const
{
	assert(::IsWindow(m_hWnd));
	return (COLORREF)::SendMessage(m_hWnd, TVM_GETLINECOLOR, 0, 0L);
}

INLINE COLORREF CXTreeView::SetLineColor(COLORREF clrNew)
{
	assert(::IsWindow(m_hWnd));
	return (COLORREF)::SendMessage(m_hWnd, TVM_SETLINECOLOR, 0, (LPARAM)clrNew);
}

INLINE BOOL CXTreeView::GetItem(LPTVITEMEX pItem) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM)pItem);
}

INLINE BOOL CXTreeView::SetItem(LPTVITEMEX pItem)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TVM_SETITEM, 0, (LPARAM)pItem);
}

INLINE ULong CXTreeView::GetExtendedStyle(void) const
{
#ifndef TVM_GETEXTENDEDSTYLE
#define TVM_GETEXTENDEDSTYLE      (TV_FIRST + 45)
#endif // TVM_GETEXTENDEDSTYLE
	assert(::IsWindow(m_hWnd));
	return (ULong)::SendMessage(m_hWnd, TVM_GETEXTENDEDSTYLE, 0, 0L);
}

INLINE ULong CXTreeView::SetExtendedStyle(ULong ulStyle, ULong ulMask)
{
#ifndef TVM_SETEXTENDEDSTYLE
#define TVM_SETEXTENDEDSTYLE      (TV_FIRST + 44)
#endif // TVM_SETEXTENDEDSTYLE
	assert(::IsWindow(m_hWnd));
	return (ULong)::SendMessage(m_hWnd, TVM_SETEXTENDEDSTYLE, ulMask, ulStyle);
}

#if (_WIN32_WINNT >= 0x0600)
INLINE BOOL CXTreeView::SetAutoScrollInfo(UInt uPixPerSec, UInt uUpdateTime)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TVM_SETAUTOSCROLLINFO, (WPARAM)uPixPerSec, (LPARAM)uUpdateTime);
}

INLINE ULong CXTreeView::GetSelectedCount(void) const
{
	assert(::IsWindow(m_hWnd));
	return (ULong)::SendMessage(m_hWnd, TVM_GETSELECTEDCOUNT, 0, 0L);
}

INLINE BOOL CXTreeView::GetItemPartRect(HTREEITEM hItem, TVITEMPART partID, LPRECT lpRect) const
{
	assert(::IsWindow(m_hWnd));
	TVGETITEMPARTRECTINFO gipri = { hItem, lpRect, partID };
	return (BOOL)::SendMessage(m_hWnd, TVM_GETITEMPARTRECT, 0, (LPARAM)&gipri);
}
#endif // (_WIN32_WINNT >= 0x0600)

// Operations
INLINE HTREEITEM CXTreeView::InsertItem(LPTVINSERTSTRUCT lpInsertStruct)
{
	assert(::IsWindow(m_hWnd));
	return (HTREEITEM)::SendMessage(m_hWnd, TVM_INSERTITEM, 0, (LPARAM)lpInsertStruct);
}

INLINE HTREEITEM CXTreeView::InsertItem(PCXStr pszItem, Int nImage, Int nSelectedImage, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	assert(::IsWindow(m_hWnd));
	return InsertItem(TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE, pszItem, nImage, nSelectedImage, 0, 0, 0, hParent, hInsertAfter); 
}

INLINE HTREEITEM CXTreeView::InsertItem(PCXStr pszItem, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	assert(::IsWindow(m_hWnd));
	return InsertItem(TVIF_TEXT, pszItem, 0, 0, 0, 0, 0, hParent, hInsertAfter);
}

INLINE HTREEITEM CXTreeView::InsertItem(UInt uMask, PCXStr pszItem, Int nImage, Int nSelectedImage, 
	                                    UInt uState, UInt uStateMask, LPARAM lParam, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	assert(::IsWindow(m_hWnd));
	TVINSERTSTRUCT tvis = { 0 };
	tvis.hParent             = hParent;
	tvis.hInsertAfter        = hInsertAfter;
	tvis.item.mask           = uMask;
	tvis.item.pszText        = (PXStr) pszItem;
	tvis.item.iImage         = nImage;
	tvis.item.iSelectedImage = nSelectedImage;
	tvis.item.state          = uState;
	tvis.item.stateMask      = uStateMask;
	tvis.item.lParam         = lParam;
	return (HTREEITEM)::SendMessage(m_hWnd, TVM_INSERTITEM, 0, (LPARAM)&tvis);
}

INLINE BOOL CXTreeView::DeleteItem(HTREEITEM hItem)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TVM_DELETEITEM, 0, (LPARAM)hItem);
}

INLINE BOOL CXTreeView::DeleteAllItems(void)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TVM_DELETEITEM, 0, (LPARAM)TVI_ROOT);
}

INLINE BOOL CXTreeView::Expand(HTREEITEM hItem, UInt nCode)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TVM_EXPAND, nCode, (LPARAM)hItem);
}

INLINE HTREEITEM CXTreeView::GetNextItem(HTREEITEM hItem, UInt uCode) const
{
	assert(::IsWindow(m_hWnd)); 
	return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, uCode, (LPARAM)hItem);
}

INLINE HTREEITEM CXTreeView::GetChildItem(HTREEITEM hItem) const
{
	assert(::IsWindow(m_hWnd)); 
	return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hItem);
}

INLINE HTREEITEM CXTreeView::GetNextSiblingItem(HTREEITEM hItem) const
{
	assert(::IsWindow(m_hWnd)); 
	return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)hItem); 
}

INLINE HTREEITEM CXTreeView::GetPrevSiblingItem(HTREEITEM hItem) const
{
	assert(::IsWindow(m_hWnd)); 
	return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_PREVIOUS, (LPARAM)hItem);
}

INLINE HTREEITEM CXTreeView::GetParentItem(HTREEITEM hItem) const
{
	assert(::IsWindow(m_hWnd)); 
	return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_PARENT, (LPARAM)hItem); 
}

INLINE HTREEITEM CXTreeView::GetFirstVisibleItem(void) const
{
	assert(::IsWindow(m_hWnd)); 
	return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_FIRSTVISIBLE, 0L);
}

INLINE HTREEITEM CXTreeView::GetNextVisibleItem(HTREEITEM hItem) const
{
	assert(::IsWindow(m_hWnd));
	return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_NEXTVISIBLE, (LPARAM)hItem);
}

INLINE HTREEITEM CXTreeView::GetPrevVisibleItem(HTREEITEM hItem) const
{
	assert(::IsWindow(m_hWnd));
	return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_PREVIOUSVISIBLE, (LPARAM)hItem);
}

INLINE HTREEITEM CXTreeView::GetSelectedItem(void) const
{
	assert(::IsWindow(m_hWnd));
	return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_CARET, 0L);
}

INLINE HTREEITEM CXTreeView::GetDropHilightItem(void) const
{
	assert(::IsWindow(m_hWnd));
	return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_DROPHILITE, 0L);
}

INLINE HTREEITEM CXTreeView::GetRootItem(void) const
{
	assert(::IsWindow(m_hWnd));
	return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_ROOT, 0L);
}

INLINE HTREEITEM CXTreeView::GetLastVisibleItem(void) const
{
	assert(::IsWindow(m_hWnd));
	return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_LASTVISIBLE, 0L);
}

INLINE HTREEITEM CXTreeView::GetNextSelectedItem(void) const
{
#ifndef TVGN_NEXTSELECTED
#define TVGN_NEXTSELECTED       0x000B
#endif // TVGN_NEXTSELECTED
	assert(::IsWindow(m_hWnd));
	return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_NEXTSELECTED, 0L);
}

INLINE BOOL CXTreeView::Select(HTREEITEM hItem, UInt uCode)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TVM_SELECTITEM, uCode, (LPARAM)hItem);
}

INLINE BOOL CXTreeView::SelectItem(HTREEITEM hItem)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TVM_SELECTITEM, TVGN_CARET, (LPARAM)hItem);
}

INLINE BOOL CXTreeView::SelectDropTarget(HTREEITEM hItem)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TVM_SELECTITEM, TVGN_DROPHILITE, (LPARAM)hItem);
}

INLINE BOOL CXTreeView::SelectSetFirstVisible(HTREEITEM hItem)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TVM_SELECTITEM, TVGN_FIRSTVISIBLE, (LPARAM)hItem);
}

INLINE HWND CXTreeView::EditLabel(HTREEITEM hItem)
{
	assert(::IsWindow(m_hWnd));
	return (HWND)::SendMessage(m_hWnd, TVM_EDITLABEL, 0, (LPARAM)hItem);
}

INLINE BOOL CXTreeView::EndEditLabelNow(BOOL bCancel)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TVM_ENDEDITLABELNOW, bCancel, 0L);
}

INLINE HTREEITEM CXTreeView::HitTest(TVHITTESTINFO* pHitTestInfo) const
{
	assert(::IsWindow(m_hWnd));
	return (HTREEITEM)::SendMessage(m_hWnd, TVM_HITTEST, 0, (LPARAM)pHitTestInfo);
}

INLINE HTREEITEM CXTreeView::HitTest(POINT& pt, PUInt puFlags) const
{
	assert(::IsWindow(m_hWnd));
	TVHITTESTINFO hti = { 0 };
	hti.pt = pt;
	HTREEITEM hTreeItem = (HTREEITEM)::SendMessage(m_hWnd, TVM_HITTEST, 0, (LPARAM)&hti);
	if (puFlags != nullptr)
	{
		*puFlags = hti.flags;
	}
	return hTreeItem;
}

INLINE BOOL CXTreeView::SortChildren(HTREEITEM hItem, BOOL bRecurse)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TVM_SORTCHILDREN, (WPARAM)bRecurse, (LPARAM)hItem);
}

INLINE BOOL CXTreeView::EnsureVisible(HTREEITEM hItem)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TVM_ENSUREVISIBLE, 0, (LPARAM)hItem);
}

INLINE BOOL CXTreeView::SortChildrenCB(LPTVSORTCB pSort, BOOL bRecurse)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TVM_SORTCHILDRENCB, (WPARAM)bRecurse, (LPARAM)pSort);
}

INLINE HIMAGELIST CXTreeView::RemoveImageList(Int nImageList)
{
	assert(::IsWindow(m_hWnd));
	return (HIMAGELIST)::SendMessage(m_hWnd, TVM_SETIMAGELIST, (WPARAM)nImageList, 0);
}

INLINE HIMAGELIST CXTreeView::CreateDragImage(HTREEITEM hItem)
{
	assert(::IsWindow(m_hWnd));
	return (HIMAGELIST)::SendMessage(m_hWnd, TVM_CREATEDRAGIMAGE, 0, (LPARAM)hItem);
}

INLINE BOOL CXTreeView::SetInsertMark(HTREEITEM hTreeItem, BOOL bAfter)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TVM_SETINSERTMARK, bAfter, (LPARAM)hTreeItem);
}

INLINE BOOL CXTreeView::RemoveInsertMark(void)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TVM_SETINSERTMARK, 0, 0L);
}

#if (_WIN32_WINNT >= 0x0501)
INLINE HTREEITEM CXTreeView::MapAccIDToHTREEITEM(UInt uID) const
{
	assert(::IsWindow(m_hWnd));
	return (HTREEITEM)::SendMessage(m_hWnd, TVM_MAPACCIDTOHTREEITEM, uID, 0L);
}

INLINE UInt CXTreeView::MapHTREEITEMToAccID(HTREEITEM hTreeItem) const
{
	assert(::IsWindow(m_hWnd));
	return (UInt)::SendMessage(m_hWnd, TVM_MAPHTREEITEMTOACCID, (WPARAM)hTreeItem, 0L);
}
#endif // (_WIN32_WINNT >= 0x0501)
#if (_WIN32_WINNT >= 0x0600)
INLINE void CXTreeView::ShowInfoTip(HTREEITEM hItem)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TVM_SHOWINFOTIP, 0, (LPARAM)hItem);
}
#endif // (_WIN32_WINNT >= 0x0600)

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXTrackBar : TRACKBAR_CLASS
INLINE CXTrackBar::CXTrackBar(void)
: CXControl(XWNDT_TRACKBAR)
{
}

INLINE CXTrackBar::~CXTrackBar(void)
{
}

INLINE CXTrackBar::CXTrackBar(const CXTrackBar&)
: CXControl(XWNDT_TRACKBAR)
{
}

INLINE CXTrackBar& CXTrackBar::operator=(const CXTrackBar&)
{
	return (*this);
}

INLINE PCXStr CXTrackBar::GetXWndClass(void)
{
	return TRACKBAR_CLASS;
}

INLINE Int CXTrackBar::GetLineSize(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TBM_GETLINESIZE, 0, 0L);
}

INLINE Int CXTrackBar::SetLineSize(Int nSize)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TBM_SETLINESIZE, 0, nSize);
}

INLINE Int CXTrackBar::GetPageSize(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TBM_GETPAGESIZE, 0, 0L);
}

INLINE Int CXTrackBar::SetPageSize(Int nSize)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TBM_SETPAGESIZE, 0, nSize);
}

INLINE Int CXTrackBar::GetRangeMin(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TBM_GETRANGEMIN, 0, 0L);
}

INLINE void CXTrackBar::SetRangeMin(Int nMin, BOOL bRedraw)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TBM_SETRANGEMIN, bRedraw, nMin);
}

INLINE Int CXTrackBar::GetRangeMax(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TBM_GETRANGEMAX, 0, 0L);
}

INLINE void CXTrackBar::SetRangeMax(Int nMax, BOOL bRedraw)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TBM_SETRANGEMAX, bRedraw, nMax);
}

INLINE void CXTrackBar::GetRange(Int& nMin, Int& nMax) const
{
	nMin = GetRangeMin();
	nMax = GetRangeMax();
}

INLINE void CXTrackBar::SetRange(Int nMin, Int nMax, BOOL bRedraw)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TBM_SETRANGE, bRedraw, MAKELPARAM(nMin, nMax));
}

INLINE Int CXTrackBar::GetSelStart(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TBM_GETSELSTART, 0, 0L);
}

INLINE void CXTrackBar::SetSelStart(Int nMin)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TBM_SETSELSTART, 0, (LPARAM)nMin);
}

INLINE Int CXTrackBar::GetSelEnd(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TBM_GETSELEND, 0, 0L);
}

INLINE void CXTrackBar::SetSelEnd(Int nMax)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TBM_SETSELEND, 0, (LPARAM)nMax);
}

INLINE void CXTrackBar::GetSelection(Int& nMin, Int& nMax) const
{
	nMin = GetSelStart();
	nMax = GetSelEnd();
}

INLINE void CXTrackBar::SetSelection(Int nMin, Int nMax)
{
	SetSelStart(nMin);
	SetSelEnd(nMax);
}

INLINE void CXTrackBar::GetChannelRect(LPRECT lprc) const
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TBM_GETCHANNELRECT, 0, (LPARAM)lprc);
}

INLINE void CXTrackBar::GetThumbRect(LPRECT lprc) const
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TBM_GETTHUMBRECT, 0, (LPARAM)lprc);
}

INLINE Int CXTrackBar::GetPos(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TBM_GETPOS, 0, 0L);
}

INLINE void CXTrackBar::SetPos(Int nPos)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TBM_SETPOS, TRUE, nPos);
}

INLINE UInt CXTrackBar::GetNumTics(void) const
{
	assert(::IsWindow(m_hWnd));
	return (UInt)::SendMessage(m_hWnd, TBM_GETNUMTICS, 0, 0L);
}

INLINE PULong CXTrackBar::GetTicArray(void) const
{
	assert(::IsWindow(m_hWnd));
	return (PULong)::SendMessage(m_hWnd, TBM_GETPTICS, 0, 0L);
}

INLINE Int CXTrackBar::GetTic(Int nTic) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TBM_GETTIC, nTic, 0L);
}

INLINE BOOL CXTrackBar::SetTic(Int nTic)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TBM_SETTIC, 0, nTic);
}

INLINE Int CXTrackBar::GetTicPos(Int nTic) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TBM_GETTICPOS, nTic, 0L);
}

INLINE void CXTrackBar::SetTicFreq(Int nFreq)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TBM_SETTICFREQ, nFreq, 0L);
}

INLINE Int CXTrackBar::GetThumbLength(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TBM_GETTHUMBLENGTH, 0, 0L);
}

INLINE void CXTrackBar::SetThumbLength(Int nLength)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TBM_SETTHUMBLENGTH, nLength, 0L);
}

INLINE void CXTrackBar::SetSel(Int nStart, Int nEnd, BOOL bRedraw)
{
	assert(::IsWindow(m_hWnd));
	assert(GetWindowLongPtr(GWL_STYLE) & TBS_ENABLESELRANGE);
	::SendMessage(m_hWnd, TBM_SETSEL, bRedraw, MAKELPARAM(nStart, nEnd));
}

INLINE HWND CXTrackBar::GetBuddy(BOOL bLeft) const
{
	assert(::IsWindow(m_hWnd));
	return (HWND)::SendMessage(m_hWnd, TBM_GETBUDDY, bLeft, 0L);
}

INLINE HWND CXTrackBar::SetBuddy(HWND hWndBuddy, BOOL bLeft)
{
	assert(::IsWindow(m_hWnd));
	return (HWND)::SendMessage(m_hWnd, TBM_SETBUDDY, bLeft, (LPARAM)hWndBuddy);
}

INLINE HWND CXTrackBar::GetToolTips(void) const
{
	assert(::IsWindow(m_hWnd));
	return (HWND)::SendMessage(m_hWnd, TBM_GETTOOLTIPS, 0, 0L);
}

INLINE void CXTrackBar::SetToolTips(HWND hWndTT)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TBM_SETTOOLTIPS, (WPARAM)hWndTT, 0L);
}

INLINE Int CXTrackBar::SetTipSide(Int nSide)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TBM_SETTIPSIDE, nSide, 0L);
}

INLINE BOOL CXTrackBar::GetUnicodeFormat(void) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TBM_GETUNICODEFORMAT, 0, 0L);
}

INLINE BOOL CXTrackBar::SetUnicodeFormat(BOOL bUnicode)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TBM_SETUNICODEFORMAT, bUnicode, 0L);
}
// Operations
INLINE void CXTrackBar::ClearSel(BOOL bRedraw)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TBM_CLEARSEL, bRedraw, 0L);
}

INLINE void CXTrackBar::VerifyPos(void)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TBM_SETPOS, FALSE, 0L);
}

INLINE void CXTrackBar::ClearTics(BOOL bRedraw)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TBM_CLEARTICS, bRedraw, 0L);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXUpDown : UPDOWN_CLASS
INLINE CXUpDown::CXUpDown(void)
: CXControl(XWNDT_UPDOWN)
{
}

INLINE CXUpDown::~CXUpDown(void)
{
}

INLINE CXUpDown::CXUpDown(const CXUpDown&)
: CXControl(XWNDT_UPDOWN)
{
}

INLINE CXUpDown& CXUpDown::operator=(const CXUpDown&)
{
	return (*this);
}

INLINE PCXStr CXUpDown::GetXWndClass(void)
{
	return UPDOWN_CLASS;
}

INLINE UInt CXUpDown::GetAccel(Int nAccel, UDACCEL* pAccel) const
{
	assert(::IsWindow(m_hWnd));
	return (UInt)LOWORD(::SendMessage(m_hWnd, UDM_GETACCEL, nAccel, (LPARAM)pAccel));
}

INLINE BOOL CXUpDown::SetAccel(Int nAccel, UDACCEL* pAccel)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)LOWORD(::SendMessage(m_hWnd, UDM_SETACCEL, nAccel, (LPARAM)pAccel));
}

INLINE UInt CXUpDown::GetBase(void) const
{
	assert(::IsWindow(m_hWnd));
	return (UInt)LOWORD(::SendMessage(m_hWnd, UDM_GETBASE, 0, 0L));
}

INLINE Int CXUpDown::SetBase(Int nBase)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, UDM_SETBASE, nBase, 0L);
}

INLINE HWND CXUpDown::GetBuddy(void) const
{
	assert(::IsWindow(m_hWnd));
	return (HWND)::SendMessage(m_hWnd, UDM_GETBUDDY, 0, 0L);
}

INLINE HWND CXUpDown::SetBuddy(HWND hWndBuddy)
{
	assert(::IsWindow(m_hWnd));
	return (HWND)::SendMessage(m_hWnd, UDM_SETBUDDY, (WPARAM)hWndBuddy, 0L);
}

INLINE Int CXUpDown::GetPos(LPBOOL lpbError) const
{
	assert(::IsWindow(m_hWnd));
	ULong dwRet = (ULong)::SendMessage(m_hWnd, UDM_GETPOS, 0, 0L);
	// Note: Seems that Windows always sets error to TRUE if
	// UDS_SETBUDDYINT style is not used
	if (lpbError != nullptr)
	{
		*lpbError = (HIWORD(dwRet) != 0) ? TRUE : FALSE;
	}
	return (Int)(Short)LOWORD(dwRet);
}

INLINE Int CXUpDown::SetPos(Int nPos)
{
	assert(::IsWindow(m_hWnd));
	return (Int)(Short)LOWORD(::SendMessage(m_hWnd, UDM_SETPOS, 0, MAKELPARAM(nPos, 0)));
}

INLINE ULong CXUpDown::GetRange(void) const
{
	assert(::IsWindow(m_hWnd));
	return (ULong)::SendMessage(m_hWnd, UDM_GETRANGE, 0, 0L);
}

INLINE void CXUpDown::GetRange(Int& nLower, Int& nUpper) const
{
	assert(::IsWindow(m_hWnd));
	ULong dwRet = (ULong)::SendMessage(m_hWnd, UDM_GETRANGE, 0, 0L);
	nLower = (Int)(Short)HIWORD(dwRet);
	nUpper = (Int)(Short)LOWORD(dwRet);
}

INLINE void CXUpDown::SetRange(Int nLower, Int nUpper)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, UDM_SETRANGE, 0, MAKELPARAM(nUpper, nLower));
}

INLINE void CXUpDown::SetRange32(Int nLower, Int nUpper)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, UDM_SETRANGE32, nLower, nUpper);
}

INLINE void CXUpDown::GetRange32(Int& nLower, Int& nUpper) const
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, UDM_GETRANGE32, (WPARAM)&nLower, (LPARAM)&nUpper);
}

INLINE BOOL CXUpDown::GetUnicodeFormat(void) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, UDM_GETUNICODEFORMAT, 0, 0L);
}

INLINE BOOL CXUpDown::SetUnicodeFormat(BOOL bUnicode)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, UDM_SETUNICODEFORMAT, bUnicode, 0L);
}

INLINE Int CXUpDown::GetPos32(LPBOOL lpbError) const
{
	assert(::IsWindow(m_hWnd));
	// Note: Seems that Windows always sets error to TRUE if
	// UDS_SETBUDDYINT style is not used
	return (Int)::SendMessage(m_hWnd, UDM_GETPOS32, 0, (LPARAM)lpbError);
}

INLINE Int CXUpDown::SetPos32(Int nPos)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, UDM_SETPOS32, 0, (LPARAM)nPos);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXProgressBar : PROGRESS_CLASS
INLINE CXProgressBar::CXProgressBar(void)
: CXControl(XWNDT_PROGRESS)
{
}

INLINE CXProgressBar::~CXProgressBar(void)
{
}

INLINE CXProgressBar::CXProgressBar(const CXProgressBar&)
: CXControl(XWNDT_PROGRESS)
{
}

INLINE CXProgressBar& CXProgressBar::operator=(const CXProgressBar&)
{
	return (*this);
}

INLINE PCXStr CXProgressBar::GetXWndClass(void)
{
	return  PROGRESS_CLASS;
}

INLINE ULong CXProgressBar::SetRange(Int nLower, Int nUpper)
{
	assert(::IsWindow(m_hWnd));
	return (ULong)::SendMessage(m_hWnd, PBM_SETRANGE, 0, MAKELPARAM(nLower, nUpper));
}

INLINE Int CXProgressBar::SetPos(Int nPos)
{
	assert(::IsWindow(m_hWnd));
	return (Int)(Short)LOWORD(::SendMessage(m_hWnd, PBM_SETPOS, nPos, 0L));
}

INLINE Int CXProgressBar::OffsetPos(Int nPos)
{
	assert(::IsWindow(m_hWnd));
	return (Int)(Short)LOWORD(::SendMessage(m_hWnd, PBM_DELTAPOS, nPos, 0L));
}

INLINE Int CXProgressBar::SetStep(Int nStep)
{
	assert(::IsWindow(m_hWnd));
	return (Int)(Short)LOWORD(::SendMessage(m_hWnd, PBM_SETSTEP, nStep, 0L));
}

INLINE UInt CXProgressBar::GetPos(void) const
{
	assert(::IsWindow(m_hWnd));
	return (UInt)::SendMessage(m_hWnd, PBM_GETPOS, 0, 0L);
}

INLINE void CXProgressBar::GetRange(PPBRANGE pPBRange) const
{
	assert(::IsWindow(m_hWnd));
	assert(pPBRange != nullptr);
	::SendMessage(m_hWnd, PBM_GETRANGE, TRUE, (LPARAM)pPBRange);
}

INLINE void CXProgressBar::GetRange(Int& nLower, Int& nUpper) const
{
	assert(::IsWindow(m_hWnd));
	PBRANGE range = { 0 };
	::SendMessage(m_hWnd, PBM_GETRANGE, TRUE, (LPARAM)&range);
	nLower = range.iLow;
	nUpper = range.iHigh;
}

INLINE Int CXProgressBar::GetRangeLimit(BOOL bLowLimit) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, PBM_GETRANGE, bLowLimit, (LPARAM)nullptr);
}

INLINE ULong CXProgressBar::SetRange32(Int nMin, Int nMax)
{
	assert(::IsWindow(m_hWnd));
	return (ULong)::SendMessage(m_hWnd, PBM_SETRANGE32, nMin, nMax);
}

INLINE COLORREF CXProgressBar::SetBarColor(COLORREF clr)
{
	assert(::IsWindow(m_hWnd));
	return (COLORREF)::SendMessage(m_hWnd, PBM_SETBARCOLOR, 0, (LPARAM)clr);
}

INLINE COLORREF CXProgressBar::SetBkColor(COLORREF clr)
{
	assert(::IsWindow(m_hWnd));
	return (COLORREF)::SendMessage(m_hWnd, PBM_SETBKCOLOR, 0, (LPARAM)clr);
}

#if (_WIN32_WINNT >= 0x0501) && defined(PBM_SETMARQUEE)
INLINE BOOL CXProgressBar::SetMarquee(BOOL bMarquee, UInt uUpdateTime)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, PBM_SETMARQUEE, (WPARAM)bMarquee, (LPARAM)uUpdateTime);
}
#endif // (_WIN32_WINNT >= 0x0501) && defined(PBM_SETMARQUEE)

#if (_WIN32_WINNT >= 0x0600)
INLINE Int CXProgressBar::GetStep(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, PBM_GETSTEP, 0, 0L);
}

INLINE COLORREF CXProgressBar::GetBkColor(void) const
{
	assert(::IsWindow(m_hWnd));
	return (COLORREF)::SendMessage(m_hWnd, PBM_GETBKCOLOR, 0, 0L);
}

INLINE COLORREF CXProgressBar::GetBarColor(void) const
{
	assert(::IsWindow(m_hWnd));
	return (COLORREF)::SendMessage(m_hWnd, PBM_GETBARCOLOR, 0, 0L);
}

INLINE Int CXProgressBar::GetState(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, PBM_GETSTATE, 0, 0L);
}

INLINE Int CXProgressBar::SetState(Int nState)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, PBM_SETSTATE, nState, 0L);
}
#endif // (_WIN32_WINNT >= 0x0600)

// Operations
INLINE Int CXProgressBar::StepIt(void)
{
	assert(::IsWindow(m_hWnd));
	return (Int)(Short)LOWORD(::SendMessage(m_hWnd, PBM_STEPIT, 0, 0L));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXLink : WC_LINK
#if (_WIN32_WINNT >= 0x0501)

INLINE CXLink::CXLink(void)
: CXControl(XWNDT_LINK)
{
}

INLINE CXLink::~CXLink(void)
{
}

INLINE CXLink::CXLink(const CXLink&)
: CXControl(XWNDT_LINK)
{
}

INLINE CXLink& CXLink::operator=(const CXLink&)
{
	return (*this);
}

INLINE PCXStr CXLink::GetXWndClass(void)
{
	return  WC_LINK;
}

INLINE Int CXLink::GetIdealHeight(Int nMaxWidth) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LM_GETIDEALHEIGHT, nMaxWidth, 0L);
}

INLINE BOOL CXLink::GetItem(PLITEM pLItem) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LM_GETITEM, 0, (LPARAM)pLItem);
}

INLINE BOOL CXLink::SetItem(PLITEM pLItem)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LM_SETITEM, 0, (LPARAM)pLItem);
}

// Vista only
INLINE Int CXLink::GetIdealSize(SIZE& size, Int nMaxWidth) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, LM_GETIDEALSIZE, nMaxWidth, (LPARAM)&size);
}

// Operations
INLINE BOOL CXLink::HitTest(PLHITTESTINFO pLHitTestInfo) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, LM_HITTEST, 0, (LPARAM)pLHitTestInfo);
}

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXRichEdit : RICHEDIT_CLASS or MSFTEDIT_CLASS
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXRichEditOleCallback
SELECTANY CPCWStr CXRichEdit::CXRichEditOleCallback::StorageName = TF("XRichEdit_Storage_%d");

INLINE CXRichEdit::CXRichEditOleCallback::CXRichEditOleCallback(void)
: m_lRefCount(0)
, m_lNumStorages(0)
, m_pStorage(nullptr)
{
	::StgCreateDocfile(nullptr, STGM_READWRITE|STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_TRANSACTED, 0, &m_pStorage);
}

INLINE CXRichEdit::CXRichEditOleCallback::~CXRichEditOleCallback(void)
{
}

INLINE CXRichEdit::CXRichEditOleCallback::CXRichEditOleCallback(const CXRichEdit::CXRichEditOleCallback&)
: m_lRefCount(0)
, m_lNumStorages(0)
, m_pStorage(nullptr)
{
}

INLINE CXRichEdit::CXRichEditOleCallback& CXRichEdit::CXRichEditOleCallback::operator=(const CXRichEdit::CXRichEditOleCallback&)
{
	return (*this);
}
// IUnknown
INLINE HRESULT STDMETHODCALLTYPE CXRichEdit::CXRichEditOleCallback::QueryInterface(REFIID iid,  void** ppvObject)
{
	*ppvObject = nullptr;
	if ((iid == IID_IUnknown) || (iid == IID_IRichEditOleCallback))
	{
		*ppvObject = this;
		AddRef();
		return S_OK;
	}
	return E_NOINTERFACE;
}

INLINE ULong STDMETHODCALLTYPE CXRichEdit::CXRichEditOleCallback::AddRef(void)
{
	return (ULong)(::InterlockedIncrement(&m_lRefCount));
}

INLINE ULong STDMETHODCALLTYPE CXRichEdit::CXRichEditOleCallback::Release(void)
{
	if (::InterlockedDecrement(&m_lRefCount) == 0)
	{
		delete this;
		return 0;
	}
	return (ULong)(m_lRefCount);
}
// IRichEditOleCallback
INLINE HRESULT STDMETHODCALLTYPE CXRichEdit::CXRichEditOleCallback::GetNewStorage(LPSTORAGE* lplpstg)
{
	if (m_pStorage != nullptr)
	{
		++m_lNumStorages;
		WChar szName[LMT_KEY] = { 0 };
		CWChar::Format(szName, LMT_KEY, L"XRichEdit_Storage_%d", m_lNumStorages);
		return m_pStorage->CreateStorage(szName, STGM_READWRITE|STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_TRANSACTED,
		                                 0, 0, lplpstg);
	}
	return E_NOTIMPL;

}

INLINE HRESULT STDMETHODCALLTYPE CXRichEdit::CXRichEditOleCallback::GetInPlaceContext(LPOLEINPLACEFRAME FAR*,// lplpFrame,
                                                                                      LPOLEINPLACEUIWINDOW FAR*,// lplpDoc, 
													                                  LPOLEINPLACEFRAMEINFO)// lpFrameInfo)
{
	return  S_OK;
}

INLINE HRESULT STDMETHODCALLTYPE CXRichEdit::CXRichEditOleCallback::ShowContainerUI(BOOL)// bShow)
{
	return  S_OK;
}

INLINE HRESULT STDMETHODCALLTYPE CXRichEdit::CXRichEditOleCallback::QueryInsertObject(LPCLSID,// lpclsid, 
																					  LPSTORAGE,// lpstg, 
																					  Long)// lCP)
{
	return  S_OK;
}

INLINE HRESULT STDMETHODCALLTYPE CXRichEdit::CXRichEditOleCallback::DeleteObject(LPOLEOBJECT)// lpoleobj)
{
	return  S_OK;
}

INLINE HRESULT STDMETHODCALLTYPE CXRichEdit::CXRichEditOleCallback::QueryAcceptData(LPDATAOBJECT,// lpdataobj, 
                                                                                    CLIPFORMAT FAR*,// lpcfFormat, 
																					ULong,// ulReco, 
												                                    BOOL,// bReally, 
																					HGLOBAL)// hMetaPict)
{
	return  S_OK;
}

INLINE HRESULT STDMETHODCALLTYPE CXRichEdit::CXRichEditOleCallback::ContextSensitiveHelp(BOOL)// bEnterMode)
{
	return  S_OK;
}

INLINE HRESULT STDMETHODCALLTYPE CXRichEdit::CXRichEditOleCallback::GetClipboardData(CHARRANGE FAR*,// lpchrg, 
																					 ULong,// ulReco,
                                                                                     LPDATAOBJECT FAR*)// lplpdataobj)
{
	return  E_NOTIMPL;
}

INLINE HRESULT STDMETHODCALLTYPE CXRichEdit::CXRichEditOleCallback::GetDragDropEffect(BOOL,// bDrag, 
																					  ULong,// ulKeyState, 
																					  PULong)// pulEffect)
{
	return  S_OK;
}

INLINE HRESULT STDMETHODCALLTYPE CXRichEdit::CXRichEditOleCallback::GetContextMenu(UShort,// usSeltyp, 
																				   LPOLEOBJECT,// lpoleobj, 
                                                                                   CHARRANGE FAR*,// lpchrg, 
																				   HMENU FAR*)// lphmenu)
{
	return  S_OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
INLINE void CXRichEdit::InitRichEdit(Int nVersion)
{
	if (nVersion == INIT_VERSION4)
	{
		::LoadLibrary(TF("msftedit.dll"));
	}
	::LoadLibrary(TF("riched20.dll"));
}

INLINE CXRichEdit::CXRichEdit(PCXStr pszClassName)
: CXControl(XWNDT_RICHEDIT)
, m_bCallBackSet(false)
{
	m_strRichEditClass = pszClassName;
}

INLINE CXRichEdit::~CXRichEdit(void)
{
}

INLINE CXRichEdit::CXRichEdit(const CXRichEdit&)
: CXControl(XWNDT_RICHEDIT)
{
}

INLINE CXRichEdit& CXRichEdit::operator=(const CXRichEdit&)
{
	return (*this);
}

INLINE PCXStr CXRichEdit::GetXWndClass(void)
{
	return m_strRichEditClass.GetBuffer();
}

INLINE void CXRichEdit::PreSubClass(void)
{
	m_bCallBackSet = (SetOLECallback(&m_OleCallBack) == TRUE) ? true : false;
}

INLINE Int CXRichEdit::GetLineCount(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, EM_GETLINECOUNT, 0, 0L);
}

INLINE BOOL CXRichEdit::GetModify(void) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, EM_GETMODIFY, 0, 0L);
}

INLINE void CXRichEdit::SetModify(BOOL bModified)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, EM_SETMODIFY, bModified, 0L);
}

INLINE void CXRichEdit::GetRect(LPRECT lpRect) const
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, EM_GETRECT, 0, (LPARAM)lpRect);
}

INLINE ULong CXRichEdit::GetOptions(void) const
{
	assert(::IsWindow(m_hWnd));
	return (ULong)::SendMessage(m_hWnd, EM_GETOPTIONS, 0, 0L);
}

INLINE ULong CXRichEdit::SetOptions(UShort usOperation, ULong ulOptions)
{
	assert(::IsWindow(m_hWnd));
	return (ULong)::SendMessage(m_hWnd, EM_SETOPTIONS, usOperation, ulOptions);
}

// NOTE: first word in pszBuffer must contain the size of the buffer!
INLINE Int CXRichEdit::GetLine(Int nIndex, PXStr pszBuffer) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, EM_GETLINE, nIndex, (LPARAM)pszBuffer);
}

INLINE Int CXRichEdit::GetLine(Int nIndex, PXStr pszBuffer, Int nMaxLength) const
{
	assert(::IsWindow(m_hWnd));
	*(LPWORD)pszBuffer = (UShort)nMaxLength;
	return (Int)::SendMessage(m_hWnd, EM_GETLINE, nIndex, (LPARAM)pszBuffer);
}

INLINE BOOL CXRichEdit::CanUndo(void) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, EM_CANUNDO, 0, 0L);
}

INLINE BOOL CXRichEdit::CanPaste(UInt nFormat) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, EM_CANPASTE, nFormat, 0L);
}

INLINE void CXRichEdit::GetSel(Long& lStartChar, Long& lEndChar) const
{
	assert(::IsWindow(m_hWnd));
	CHARRANGE clr = { 0, 0 };
	::SendMessage(m_hWnd, EM_EXGETSEL, 0, (LPARAM)&clr);
	lStartChar = clr.cpMin;
	lEndChar   = clr.cpMax;
}

INLINE void CXRichEdit::GetSel(CHARRANGE &clr) const
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, EM_EXGETSEL, 0, (LPARAM)&clr);
}

INLINE Int CXRichEdit::SetSel(Long lStartChar, Long lEndChar)
{
	assert(::IsWindow(m_hWnd));
	CHARRANGE clr = { lStartChar, lEndChar };
	return (Int)::SendMessage(m_hWnd, EM_EXSETSEL, 0, (LPARAM)&clr);
}

INLINE Int CXRichEdit::SetSel(CHARRANGE &clr)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, EM_EXSETSEL, 0, (LPARAM)&clr);
}

INLINE Int CXRichEdit::SetSelAll(void)
{
	return SetSel(0, -1);
}

INLINE Int CXRichEdit::SetSelNone(void)
{
	return SetSel(-1, 0);
}

INLINE ULong CXRichEdit::GetDefaultCharFormat(CHARFORMAT& cf) const
{
	assert(::IsWindow(m_hWnd));
	cf.cbSize = sizeof(CHARFORMAT);
	return (ULong)::SendMessage(m_hWnd, EM_GETCHARFORMAT, 0, (LPARAM)&cf);
}

INLINE ULong CXRichEdit::GetSelectionCharFormat(CHARFORMAT& cf) const
{
	assert(::IsWindow(m_hWnd));
	cf.cbSize = sizeof(CHARFORMAT);
	return (ULong)::SendMessage(m_hWnd, EM_GETCHARFORMAT, 1, (LPARAM)&cf);
}

INLINE ULong CXRichEdit::GetEventMask(void) const
{
	assert(::IsWindow(m_hWnd));
	return (ULong)::SendMessage(m_hWnd, EM_GETEVENTMASK, 0, 0L);
}

INLINE Long CXRichEdit::GetLimitText(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Long)::SendMessage(m_hWnd, EM_GETLIMITTEXT, 0, 0L);
}

INLINE ULong CXRichEdit::GetParaFormat(PARAFORMAT& pf) const
{
	assert(::IsWindow(m_hWnd));
	pf.cbSize = sizeof(PARAFORMAT);
	return (ULong)::SendMessage(m_hWnd, EM_GETPARAFORMAT, 0, (LPARAM)&pf);
}

INLINE Long CXRichEdit::GetSelText(PXStr pszBuff) const
{
	assert(::IsWindow(m_hWnd));
	return (Long)::SendMessage(m_hWnd, EM_GETSELTEXT, 0, (LPARAM)pszBuff);
}

INLINE UShort CXRichEdit::GetSelectionType(void) const
{
	assert(::IsWindow(m_hWnd));
	return (UShort)::SendMessage(m_hWnd, EM_SELECTIONTYPE, 0, 0L);
}

INLINE COLORREF CXRichEdit::SetBackgroundColor(COLORREF clr)
{
	assert(::IsWindow(m_hWnd));
	return (COLORREF)::SendMessage(m_hWnd, EM_SETBKGNDCOLOR, 0, clr);
}

INLINE COLORREF CXRichEdit::SetBackgroundColor(void)   // sets to system background
{
	assert(::IsWindow(m_hWnd));
	return (COLORREF)::SendMessage(m_hWnd, EM_SETBKGNDCOLOR, 1, 0);
}

INLINE BOOL CXRichEdit::SetCharFormat(CHARFORMAT& cf, UShort usFlags)
{
	assert(::IsWindow(m_hWnd));
	cf.cbSize = sizeof(CHARFORMAT);
	return (BOOL)::SendMessage(m_hWnd, EM_SETCHARFORMAT, (WPARAM)usFlags, (LPARAM)&cf);
}

INLINE BOOL CXRichEdit::SetDefaultCharFormat(CHARFORMAT& cf)
{
	assert(::IsWindow(m_hWnd));
	cf.cbSize = sizeof(CHARFORMAT);
	return (BOOL)::SendMessage(m_hWnd, EM_SETCHARFORMAT, 0, (LPARAM)&cf);
}

INLINE BOOL CXRichEdit::SetSelectionCharFormat(CHARFORMAT& cf)
{
	assert(::IsWindow(m_hWnd));
	cf.cbSize = sizeof(CHARFORMAT);
	return (BOOL)::SendMessage(m_hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
}

INLINE BOOL CXRichEdit::SetWordCharFormat(CHARFORMAT& cf)
{
	assert(::IsWindow(m_hWnd));
	cf.cbSize = sizeof(CHARFORMAT);
	return (BOOL)::SendMessage(m_hWnd, EM_SETCHARFORMAT, SCF_SELECTION | SCF_WORD, (LPARAM)&cf);
}

INLINE ULong CXRichEdit::SetEventMask(ULong ulEventMask)
{
	assert(::IsWindow(m_hWnd));
	return (ULong)::SendMessage(m_hWnd, EM_SETEVENTMASK, 0, ulEventMask);
}

INLINE BOOL CXRichEdit::SetOLECallback(IRichEditOleCallback* pCallback)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, EM_SETOLECALLBACK, 0, (LPARAM)pCallback);
}

INLINE BOOL CXRichEdit::SetParaFormat(PARAFORMAT& pf)
{
	assert(::IsWindow(m_hWnd));
	pf.cbSize = sizeof(PARAFORMAT);
	return (BOOL)::SendMessage(m_hWnd, EM_SETPARAFORMAT, 0, (LPARAM)&pf);
}

INLINE BOOL CXRichEdit::SetTargetDevice(HDC hDC, Int nLineWidth)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, EM_SETTARGETDEVICE, (WPARAM)hDC, nLineWidth);
}

INLINE Int CXRichEdit::GetTextLength(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, WM_GETTEXTLENGTH, 0, 0L);
}

INLINE BOOL CXRichEdit::SetReadOnly(BOOL bReadOnly)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, EM_SETREADONLY, bReadOnly, 0L);
}

INLINE Int CXRichEdit::GetFirstVisibleLine(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, EM_GETFIRSTVISIBLELINE, 0, 0L);
}

INLINE EDITWORDBREAKPROCEX CXRichEdit::GetWordBreakProcEx(void) const
{
	assert(::IsWindow(m_hWnd));
	return (EDITWORDBREAKPROCEX)::SendMessage(m_hWnd, EM_GETWORDBREAKPROCEX, 0, 0L);
}

INLINE EDITWORDBREAKPROCEX CXRichEdit::SetWordBreakProcEx(EDITWORDBREAKPROCEX pfnEditWordBreakProcEx)
{
	assert(::IsWindow(m_hWnd));
	return (EDITWORDBREAKPROCEX)::SendMessage(m_hWnd, EM_SETWORDBREAKPROCEX, 0, (LPARAM)pfnEditWordBreakProcEx);
}

INLINE Int CXRichEdit::GetTextRange(TEXTRANGE* pTextRange) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, EM_GETTEXTRANGE, 0, (LPARAM)pTextRange);
}

INLINE Int CXRichEdit::GetTextRange(Long lStartChar, Long lEndChar, PXStr pszText) const
{
	assert(::IsWindow(m_hWnd));
	TEXTRANGE tr = { 0 };
	tr.chrg.cpMin = lStartChar;
	tr.chrg.cpMax = lEndChar;
	tr.lpstrText  = pszText;
	return (Int)::SendMessage(m_hWnd, EM_GETTEXTRANGE, 0, (LPARAM)&tr);
}

INLINE ULong CXRichEdit::GetDefaultCharFormat(CHARFORMAT2& cf) const
{
	assert(::IsWindow(m_hWnd));
	cf.cbSize = sizeof(CHARFORMAT2);
	return (ULong)::SendMessage(m_hWnd, EM_GETCHARFORMAT, 0, (LPARAM)&cf);
}

INLINE BOOL CXRichEdit::SetCharFormat(CHARFORMAT2& cf, UShort usFlags)
{
	assert(::IsWindow(m_hWnd));
	cf.cbSize = sizeof(CHARFORMAT2);
	return (BOOL)::SendMessage(m_hWnd, EM_SETCHARFORMAT, (WPARAM)usFlags, (LPARAM)&cf);
}

INLINE BOOL CXRichEdit::SetDefaultCharFormat(CHARFORMAT2& cf)
{
	assert(::IsWindow(m_hWnd));
	cf.cbSize = sizeof(CHARFORMAT2);
	return (BOOL)::SendMessage(m_hWnd, EM_SETCHARFORMAT, 0, (LPARAM)&cf);
}

INLINE ULong CXRichEdit::GetSelectionCharFormat(CHARFORMAT2& cf) const
{
	assert(::IsWindow(m_hWnd));
	cf.cbSize = sizeof(CHARFORMAT2);
	return (ULong)::SendMessage(m_hWnd, EM_GETCHARFORMAT, 1, (LPARAM)&cf);
}

INLINE BOOL CXRichEdit::SetSelectionCharFormat(CHARFORMAT2& cf)
{
	assert(::IsWindow(m_hWnd));
	cf.cbSize = sizeof(CHARFORMAT2);
	return (BOOL)::SendMessage(m_hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
}

INLINE BOOL CXRichEdit::SetWordCharFormat(CHARFORMAT2& cf)
{
	assert(::IsWindow(m_hWnd));
	cf.cbSize = sizeof(CHARFORMAT2);
	return (BOOL)::SendMessage(m_hWnd, EM_SETCHARFORMAT, SCF_SELECTION | SCF_WORD, (LPARAM)&cf);
}

INLINE ULong CXRichEdit::GetParaFormat(PARAFORMAT2& pf) const
{
	assert(::IsWindow(m_hWnd));
	pf.cbSize = sizeof(PARAFORMAT2);
	return (ULong)::SendMessage(m_hWnd, EM_GETPARAFORMAT, 0, (LPARAM)&pf);
}

INLINE BOOL CXRichEdit::SetParaFormat(PARAFORMAT2& pf)
{
	assert(::IsWindow(m_hWnd));
	pf.cbSize = sizeof(PARAFORMAT2);
	return (BOOL)::SendMessage(m_hWnd, EM_SETPARAFORMAT, 0, (LPARAM)&pf);
}

INLINE TEXTMODE CXRichEdit::GetTextMode(void) const
{
	assert(::IsWindow(m_hWnd));
	return (TEXTMODE)::SendMessage(m_hWnd, EM_GETTEXTMODE, 0, 0L);
}

INLINE BOOL CXRichEdit::SetTextMode(TEXTMODE enumTextMode)
{
	assert(::IsWindow(m_hWnd));
	return !(BOOL)::SendMessage(m_hWnd, EM_SETTEXTMODE, enumTextMode, 0L);
}

INLINE UNDONAMEID CXRichEdit::GetUndoName(void) const
{
	assert(::IsWindow(m_hWnd));
	return (UNDONAMEID)::SendMessage(m_hWnd, EM_GETUNDONAME, 0, 0L);
}

INLINE UNDONAMEID CXRichEdit::GetRedoName(void) const
{
	assert(::IsWindow(m_hWnd));
	return (UNDONAMEID)::SendMessage(m_hWnd, EM_GETREDONAME, 0, 0L);
}

INLINE BOOL CXRichEdit::CanRedo(void) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, EM_CANREDO, 0, 0L);
}

INLINE BOOL CXRichEdit::GetAutoURLDetect(void) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, EM_GETAUTOURLDETECT, 0, 0L);
}

INLINE BOOL CXRichEdit::SetAutoURLDetect(BOOL bAutoDetect)
{
	assert(::IsWindow(m_hWnd));
	return !(BOOL)::SendMessage(m_hWnd, EM_AUTOURLDETECT, bAutoDetect, 0L);
}

INLINE UInt CXRichEdit::SetUndoLimit(UInt uUndoLimit)
{
	assert(::IsWindow(m_hWnd));
	return (UInt)::SendMessage(m_hWnd, EM_SETUNDOLIMIT, uUndoLimit, 0L);
}

INLINE void CXRichEdit::SetPalette(HPALETTE hPalette)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, EM_SETPALETTE, (WPARAM)hPalette, 0L);
}

INLINE Int CXRichEdit::GetTextEx(GETTEXTEX* pGetTextEx, PXStr pszText) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, EM_GETTEXTEX, (WPARAM)pGetTextEx, (LPARAM)pszText);
}

INLINE Int CXRichEdit::GetTextEx(PXStr pszText, Int nTextLen, ULong ulFlags, UInt uCodePage, LPCSTR lpDefaultChar, LPBOOL lpUsedDefChar) const
{
	assert(::IsWindow(m_hWnd));
	GETTEXTEX gte = { 0 };
	gte.cb = nTextLen * sizeof(XChar);
	gte.codepage      = uCodePage;
	gte.flags         = ulFlags;
	gte.lpDefaultChar = lpDefaultChar;
	gte.lpUsedDefChar = lpUsedDefChar;
	return (Int)::SendMessage(m_hWnd, EM_GETTEXTEX, (WPARAM)&gte, (LPARAM)pszText);
}

INLINE Int CXRichEdit::GetTextLengthEx(GETTEXTLENGTHEX* pGetTextLengthEx) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, EM_GETTEXTLENGTHEX, (WPARAM)pGetTextLengthEx, 0L);
}

INLINE Int CXRichEdit::GetTextLengthEx(ULong ulFlags, UInt uCodePage) const
{
	assert(::IsWindow(m_hWnd));
	GETTEXTLENGTHEX gtle = { 0 };
	gtle.codepage = uCodePage;
	gtle.flags    = ulFlags;
	return (Int)::SendMessage(m_hWnd, EM_GETTEXTLENGTHEX, (WPARAM)&gtle, 0L);
}

INLINE Int CXRichEdit::SetTextEx(SETTEXTEX* pSetTextEx, PCXStr pszText)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, EM_SETTEXTEX, (WPARAM)pSetTextEx, (LPARAM)pszText);
}

INLINE Int CXRichEdit::SetTextEx(PCXStr pszText, ULong ulFlags, UInt uCodePage)
{
	assert(::IsWindow(m_hWnd));
	SETTEXTEX ste = { 0 };
	ste.flags    = ulFlags;
	ste.codepage = uCodePage;
	return (Int)::SendMessage(m_hWnd, EM_SETTEXTEX, (WPARAM)&ste, (LPARAM)pszText);
}

INLINE Int CXRichEdit::GetEditStyle(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, EM_GETEDITSTYLE, 0, 0L);
}

INLINE Int CXRichEdit::SetEditStyle(Int nStyle, Int nMask)
{
	assert(::IsWindow(m_hWnd));
	if (nMask == -1)
	{
		nMask = nStyle;   // set everything specified
	}
	return (Int)::SendMessage(m_hWnd, EM_SETEDITSTYLE, nStyle, nMask);
}

INLINE BOOL CXRichEdit::SetFontSize(Int nFontSizeDelta)
{
	assert(::IsWindow(m_hWnd));
	assert((nFontSizeDelta >= FONTSIZE_MIN) && (nFontSizeDelta <= FONTSIZE_MAX));
	return (BOOL)::SendMessage(m_hWnd, EM_SETFONTSIZE, nFontSizeDelta, 0L);
}

INLINE void CXRichEdit::GetScrollPos(LPPOINT lpPoint) const
{
	assert(::IsWindow(m_hWnd));
	assert(lpPoint != nullptr);
	::SendMessage(m_hWnd, EM_GETSCROLLPOS, 0, (LPARAM)lpPoint);
}

INLINE void CXRichEdit::SetScrollPos(LPPOINT lpPoint)
{
	assert(::IsWindow(m_hWnd));
	assert(lpPoint != nullptr);
	::SendMessage(m_hWnd, EM_SETSCROLLPOS, 0, (LPARAM)lpPoint);
}

INLINE BOOL CXRichEdit::GetZoom(Int& nNum, Int& nDen) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, EM_GETZOOM, (WPARAM)&nNum, (LPARAM)&nDen);
}

INLINE BOOL CXRichEdit::SetZoom(Int nNum, Int nDen)
{
	assert(::IsWindow(m_hWnd));
	assert((nNum >= 0) && (nNum <= 64));
	assert((nDen >= 0) && (nDen <= 64));
	return (BOOL)::SendMessage(m_hWnd, EM_SETZOOM, nNum, nDen);
}

INLINE BOOL CXRichEdit::SetZoomOff(void)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, EM_SETZOOM, 0, 0L);
}

// Operations
INLINE void CXRichEdit::LimitText(Long lChars)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, EM_EXLIMITTEXT, 0, lChars);
}

INLINE Int CXRichEdit::LineFromChar(Long nIndex) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, EM_EXLINEFROMCHAR, 0, nIndex);
}

INLINE POINT CXRichEdit::PosFromChar(Long lChars) const
{
	assert(::IsWindow(m_hWnd));
	POINT point = { 0, 0 };
	::SendMessage(m_hWnd, EM_POSFROMCHAR, (WPARAM)&point, lChars);
	return point;
}

INLINE Int CXRichEdit::CharFromPos(POINT pt) const
{
	assert(::IsWindow(m_hWnd));
	POINTL ptl = { pt.x, pt.y };
	return (Int)::SendMessage(m_hWnd, EM_CHARFROMPOS, 0, (LPARAM)&ptl);
}

INLINE void CXRichEdit::EmptyUndoBuffer(void)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, EM_EMPTYUNDOBUFFER, 0, 0L);
}

INLINE Int CXRichEdit::LineIndex(Int nLine) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, EM_LINEINDEX, nLine, 0L);
}

INLINE Int CXRichEdit::LineLength(Int nLine) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, EM_LINELENGTH, nLine, 0L);
}

INLINE BOOL CXRichEdit::LineScroll(Int nLines, Int nChars)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, EM_LINESCROLL, nChars, nLines);
}

INLINE void CXRichEdit::ReplaceSel(PCXStr pszNewText, BOOL bCanUndo)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, EM_REPLACESEL, (WPARAM) bCanUndo, (LPARAM)pszNewText);
}

INLINE void CXRichEdit::SetRect(LPCRECT lpRect)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, EM_SETRECT, 0, (LPARAM)lpRect);
}

INLINE BOOL CXRichEdit::DisplayBand(LPRECT pDisplayRect)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, EM_DISPLAYBAND, 0, (LPARAM)pDisplayRect);
}

INLINE Long CXRichEdit::FindText(ULong ulFlags, FINDTEXT& ft) const
{
	assert(::IsWindow(m_hWnd));
#if defined(_UNICODE)
	return (Long)::SendMessage(m_hWnd, EM_FINDTEXTW, ulFlags, (LPARAM)&ft);
#else
	return (Long)::SendMessage(m_hWnd, EM_FINDTEXT, ulFlags, (LPARAM)&ft);
#endif
}

INLINE Long CXRichEdit::FindText(ULong ulFlags, FINDTEXTEX& ft) const
{
	assert(::IsWindow(m_hWnd));
#if defined(_UNICODE)
	return (Long)::SendMessage(m_hWnd, EM_FINDTEXTEXW, ulFlags, (LPARAM)&ft);
#else
	return (Long)::SendMessage(m_hWnd, EM_FINDTEXTEX, ulFlags, (LPARAM)&ft);
#endif
}

INLINE Long CXRichEdit::FormatRange(FORMATRANGE& fr, BOOL bDisplay)
{
	assert(::IsWindow(m_hWnd));
	return (Long)::SendMessage(m_hWnd, EM_FORMATRANGE, bDisplay, (LPARAM)&fr);
}

INLINE Long CXRichEdit::FormatRange(FORMATRANGE* pFormatRange, BOOL bDisplay)
{
	assert(::IsWindow(m_hWnd));
	return (Long)::SendMessage(m_hWnd, EM_FORMATRANGE, bDisplay, (LPARAM)pFormatRange);
}

INLINE void CXRichEdit::HideSelection(BOOL bHide, BOOL bChangeStyle)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, EM_HIDESELECTION, bHide, bChangeStyle);
}

INLINE void CXRichEdit::PasteSpecial(UInt uClipFormat, ULong ulAspect, HMETAFILE hMF)
{
	assert(::IsWindow(m_hWnd));
	REPASTESPECIAL reps = { ulAspect, (uintptr_t)hMF };
	::SendMessage(m_hWnd, EM_PASTESPECIAL, uClipFormat, (LPARAM)&reps);
}

INLINE void CXRichEdit::RequestResize(void)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, EM_REQUESTRESIZE, 0, 0L);
}

INLINE Long CXRichEdit::StreamIn(UInt uFormat, EDITSTREAM& es)
{
	assert(::IsWindow(m_hWnd));
	return (Long)::SendMessage(m_hWnd, EM_STREAMIN, uFormat, (LPARAM)&es);
}

INLINE Long CXRichEdit::StreamOut(UInt uFormat, EDITSTREAM& es)
{
	assert(::IsWindow(m_hWnd));
	return (Long)::SendMessage(m_hWnd, EM_STREAMOUT, uFormat, (LPARAM)&es);
}

INLINE ULong CXRichEdit::FindWordBreak(Int nCode, Long lStartChar)
{
	assert(::IsWindow(m_hWnd));
	return (ULong)::SendMessage(m_hWnd, EM_FINDWORDBREAK, nCode, lStartChar);
}

// Additional operations
INLINE void CXRichEdit::ScrollCaret(void)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, EM_SCROLLCARET, 0, 0L);
}

INLINE Int CXRichEdit::InsertText(Long lInsertAfterChar, PCXStr pszText, BOOL bCanUndo)
{
	Int nRet = SetSel(lInsertAfterChar, lInsertAfterChar);
	ReplaceSel(pszText, bCanUndo);
	return nRet;
}

INLINE Int CXRichEdit::AppendText(PCXStr pszText, BOOL bCanUndo)
{
	return InsertText(GetWindowTextLength(), pszText, bCanUndo);
}

// Clipboard operations
INLINE BOOL CXRichEdit::Undo(void)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, EM_UNDO, 0, 0L);
}

INLINE void CXRichEdit::Clear(void)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, WM_CLEAR, 0, 0L);
}

INLINE void CXRichEdit::Copy(void)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, WM_COPY, 0, 0L);
}

INLINE void CXRichEdit::Cut(void)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, WM_CUT, 0, 0L); 
}

INLINE void CXRichEdit::Paste(void)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, WM_PASTE, 0, 0L);
}

INLINE BOOL CXRichEdit::Redo(void)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, EM_REDO, 0, 0L);
}

INLINE void CXRichEdit::StopGroupTyping(void)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, EM_STOPGROUPTYPING, 0, 0L);
}

INLINE void CXRichEdit::ShowScrollBar(Int nBarType, BOOL bVisible)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, EM_SHOWSCROLLBAR, nBarType, bVisible);
}

INLINE BOOL CXRichEdit::SetTabStops(Int nTabStops, PInt pnTabStops)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, EM_SETTABSTOPS, nTabStops, (LPARAM)pnTabStops);
}

INLINE BOOL CXRichEdit::SetTabStops(void)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, EM_SETTABSTOPS, 0, 0L);
}

INLINE BOOL CXRichEdit::SetTabStops(const Int& nEachStop)    // takes an 'Int'
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, EM_SETTABSTOPS, 1, (LPARAM)(PInt)&nEachStop);
}

INLINE LRESULT OnGetDlgCode(WPARAM, LPARAM, BOOL&)
{
	return DLGC_WANTTAB;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXToolBar : TOOLBARCLASSNAME
INLINE CXToolBar::CXToolBar(void)
: CXControl(XWNDT_TOOLBAR)
{
}

INLINE CXToolBar::~CXToolBar(void)
{
}

INLINE CXToolBar::CXToolBar(const CXToolBar&)
: CXControl(XWNDT_TOOLBAR)
{
}

INLINE CXToolBar& CXToolBar::operator=(const CXToolBar&)
{
	return (*this);
}

INLINE PCXStr CXToolBar::GetXWndClass(void)
{
	return TOOLBARCLASSNAME;
}

INLINE bool CXToolBar::Load(Int nID, bool bLoadBitmap, HMODULE hResInst)
{
	assert(::IsWindow(m_hWnd));
	if (::IsWindow(m_hWnd) == FALSE)
	{
		return false;
	}
	SetButtonStructSize();

	hResInst = hResInst ? hResInst : (::GetModuleHandle(nullptr));
	HRSRC hRsrc = ::FindResource(hResInst, MAKEINTRESOURCE(nID), RT_TOOLBAR); 
	if (hRsrc == nullptr)
	{
		return false;
	}
	HGLOBAL hGlobal = ::LoadResource(hResInst, hRsrc);
	if (hGlobal == nullptr)
	{
		return false;
	}
	PXTOOLBAR_DATA pData = (PXTOOLBAR_DATA)::LockResource(hGlobal);
	if (pData == nullptr)
	{
		::FreeResource(hGlobal);
		return false;
	}

	DeleteAllButtons(); // remove buttons
	bool bRet   = false;
	Int  nCount = (Int)pData->usItemCount;

	LPTBBUTTON ptbb = reinterpret_cast<LPTBBUTTON>( ALLOC( nCount * sizeof(TBBUTTON) ) );
	if (ptbb != nullptr)
	{
		Int     nIndex = 0;
		PUShort pItem  = (PUShort)(++pData);
		for (Int j = 0; j < nCount; ++j)
		{
			if (pItem[j] == 0)
			{
				ptbb[j].iBitmap   = -1;
				ptbb[j].idCommand = 0;
				ptbb[j].fsState   = 0;
				ptbb[j].fsStyle   = TBSTYLE_SEP;
				ptbb[j].dwData    = 0;
				ptbb[j].iString   = 0;
			}
			else
			{
				ptbb[j].iBitmap   = nIndex++;
				ptbb[j].idCommand = pItem[j];
				ptbb[j].fsState   = TBSTATE_ENABLED;
				ptbb[j].fsStyle   = TBSTYLE_BUTTON;
				ptbb[j].dwData    = 0;
				ptbb[j].iString   = 0;
			}
		}
		bRet = AddButtons(nCount, ptbb) ? true : false;
		assert(bRet);

		if (bLoadBitmap)
		{
			AddBitmap(nCount, nID);
		}
		AutoSize();

		FREE( ptbb );
	}
	DEV_DEBUG(TF("  ToolBar[%p] load %d items"), this, nCount);
	UnlockResource(hGlobal);
	::FreeResource(hGlobal);
	return bRet;
}

INLINE bool CXToolBar::LoadEx(Int nWidth, Int nID, Int nHot, Int nDisabled, COLORREF clrMask, UInt uFlag, HMODULE hResInst)
{
	if (Load(nID, false, hResInst) == false)
	{
		return false;
	}
	
	if (LoadImg(nWidth, TB_SETIMAGELIST, nID, clrMask, uFlag, hResInst) == false)
	{
		return false;
	}
	if (nHot != 0) 
	{
		LoadImg(nWidth, TB_SETHOTIMAGELIST, nHot, clrMask, uFlag, hResInst);
	}
	if (nDisabled != 0)
	{
		LoadImg(nWidth, TB_SETDISABLEDIMAGELIST, nDisabled, clrMask, uFlag, hResInst);
	}
	AutoSize();
	return true;
}

INLINE bool CXToolBar::LoadImg(Int nWidth, Int nType, Int nID, COLORREF clrMask, UInt uFlag, HMODULE hResInst)
{
	hResInst = hResInst ? hResInst : (::GetModuleHandle(nullptr));
	HBITMAP hBmp = (HBITMAP)LoadImage(hResInst, MAKEINTRESOURCE(nID), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE|LR_CREATEDIBSECTION);
	if (hBmp == nullptr)
	{
		return false;
	}
	BITMAP bitmap;
	::GetObject(hBmp, sizeof(BITMAP), &bitmap);

	if (clrMask == 0)
	{
		RGBTRIPLE* rgb = (RGBTRIPLE*)(bitmap.bmBits);
		clrMask	= RGB(rgb[0].rgbtRed, rgb[0].rgbtGreen, rgb[0].rgbtBlue);
	}

	HIMAGELIST hImg = ::ImageList_Create(nWidth, bitmap.bmHeight, (uFlag|ILC_MASK), (bitmap.bmWidth / nWidth), 0);
	if (hImg == nullptr)
	{
		::DeleteObject(hBmp);
		return false;
	}
	::ImageList_AddMasked(hImg, hBmp, clrMask);
	::DeleteObject(hBmp);

	::SendMessage(m_hWnd, nType, 0, (LPARAM)hImg);
	return true;
}

INLINE BOOL CXToolBar::IsButtonEnabled(Int nID) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_ISBUTTONENABLED, nID, 0L);
}

INLINE BOOL CXToolBar::IsButtonChecked(Int nID) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_ISBUTTONCHECKED, (WPARAM)nID, 0L);
}

INLINE BOOL CXToolBar::IsButtonPressed(Int nID) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_ISBUTTONPRESSED, (WPARAM)nID, 0L);
}

INLINE BOOL CXToolBar::IsButtonHidden(Int nID) const
{
	assert(::IsWindow(m_hWnd));
	return(BOOL)::SendMessage(m_hWnd, TB_ISBUTTONHIDDEN, (WPARAM)nID, 0L);
}

INLINE BOOL CXToolBar::IsButtonIndeterminate(Int nID) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_ISBUTTONINDETERMINATE, (WPARAM)nID, 0L);
}

INLINE Int CXToolBar::GetState(Int nID) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TB_GETSTATE, (WPARAM)nID, 0L);
}

INLINE BOOL CXToolBar::SetState(Int nID, Int nState)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_SETSTATE, (WPARAM)nID, MAKELPARAM(nState, 0));
}

INLINE BOOL CXToolBar::GetButton(Int nIndex, LPTBBUTTON lpButton) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_GETBUTTON, (WPARAM)nIndex, (LPARAM)lpButton);
}

INLINE Int CXToolBar::GetButtonCount(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TB_BUTTONCOUNT, 0, 0L);
}

INLINE BOOL CXToolBar::GetItemRect(Int nIndex, LPRECT lpRect) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_GETITEMRECT, (WPARAM)nIndex, (LPARAM)lpRect);
}

INLINE void CXToolBar::SetButtonStructSize(Int nSize)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TB_BUTTONSTRUCTSIZE, (WPARAM)nSize, 0L);
}

INLINE BOOL CXToolBar::SetButtonSize(SIZE& size)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_SETBUTTONSIZE, 0, MAKELPARAM(size.cx, size.cy));
}

INLINE BOOL CXToolBar::SetButtonSize(Int ncx, Int ncy)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_SETBUTTONSIZE, 0, MAKELPARAM(ncx, ncy));
}

INLINE BOOL CXToolBar::SetBitmapSize(SIZE& size)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_SETBITMAPSIZE, 0, MAKELPARAM(size.cx, size.cy));
}

INLINE BOOL CXToolBar::SetBitmapSize(Int ncx, Int ncy)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_SETBITMAPSIZE, 0, MAKELPARAM(ncx, ncy));
}

INLINE HWND CXToolBar::GetToolTips(void) const
{
	assert(::IsWindow(m_hWnd));
	return (HWND)::SendMessage(m_hWnd, TB_GETTOOLTIPS, 0, 0L);
}

INLINE void CXToolBar::SetToolTips(HWND hWndToolTip)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TB_SETTOOLTIPS, (WPARAM)hWndToolTip, 0L);
}

INLINE void CXToolBar::SetNotifyWnd(HWND hWnd)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TB_SETPARENT, (WPARAM)hWnd, 0L);
}

INLINE Int CXToolBar::GetRows(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TB_GETROWS, 0, 0L);
}

INLINE void CXToolBar::SetRows(Int nRows, BOOL bLarger, LPRECT lpRect)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TB_SETROWS, MAKELPARAM(nRows, bLarger), (LPARAM)lpRect);
}

INLINE BOOL CXToolBar::SetCmdID(Int nIndex, Int nID)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_SETCMDID, (WPARAM)nIndex, (LPARAM)nID);
}

INLINE ULong CXToolBar::GetBitmapFlags(void) const
{
	assert(::IsWindow(m_hWnd));
	return (ULong)::SendMessage(m_hWnd, TB_GETBITMAPFLAGS, 0, 0L);
}

INLINE Int CXToolBar::GetBitmap(Int nID) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TB_GETBITMAP, (WPARAM)nID, 0L);
}

INLINE Int CXToolBar::GetButtonText(Int nID, PXStr pszText) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TB_GETBUTTONTEXT, (WPARAM)nID, (LPARAM)pszText);
}

INLINE HIMAGELIST CXToolBar::GetImageList(void) const
{
	assert(::IsWindow(m_hWnd));
	return (HIMAGELIST)::SendMessage(m_hWnd, TB_GETIMAGELIST, 0, 0L);
}

INLINE HIMAGELIST CXToolBar::SetImageList(HIMAGELIST hImageList)
{
	assert(::IsWindow(m_hWnd));
	return (HIMAGELIST)::SendMessage(m_hWnd, TB_SETIMAGELIST, 0, (LPARAM)hImageList);
}

INLINE HIMAGELIST CXToolBar::GetDisabledImageList(void) const
{
	assert(::IsWindow(m_hWnd));
	return (HIMAGELIST)::SendMessage(m_hWnd, TB_GETDISABLEDIMAGELIST, 0, 0L);
}

INLINE HIMAGELIST CXToolBar::SetDisabledImageList(HIMAGELIST hImageList)
{
	assert(::IsWindow(m_hWnd));
	return (HIMAGELIST)::SendMessage(m_hWnd, TB_SETDISABLEDIMAGELIST, 0, (LPARAM)hImageList);
}

INLINE HIMAGELIST CXToolBar::GetHotImageList(void) const
{
	assert(::IsWindow(m_hWnd));
	return (HIMAGELIST)::SendMessage(m_hWnd, TB_GETHOTIMAGELIST, 0, 0L);
}

INLINE HIMAGELIST CXToolBar::SetHotImageList(HIMAGELIST hImageList)
{
	assert(::IsWindow(m_hWnd));
	return (HIMAGELIST)::SendMessage(m_hWnd, TB_SETHOTIMAGELIST, 0, (LPARAM)hImageList);
}

INLINE ULong CXToolBar::GetStyle(void) const
{
	assert(::IsWindow(m_hWnd));
	return (ULong)::SendMessage(m_hWnd, TB_GETSTYLE, 0, 0L);
}

INLINE void CXToolBar::SetStyle(ULong ulStyle)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TB_SETSTYLE, 0, (LPARAM)ulStyle);
}

INLINE ULong CXToolBar::GetButtonSize(void) const
{
	assert(::IsWindow(m_hWnd));
	return (ULong)::SendMessage(m_hWnd, TB_GETBUTTONSIZE, 0, 0L);
}

INLINE void CXToolBar::GetButtonSize(SIZE& size) const
{
	assert(::IsWindow(m_hWnd));
	ULong ulRet = (ULong)::SendMessage(m_hWnd, TB_GETBUTTONSIZE, 0, 0L);
	size.cx = GET_X_LPARAM(ulRet);
	size.cy = GET_Y_LPARAM(ulRet);
}

INLINE BOOL CXToolBar::GetRect(Int nID, LPRECT lpRect) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_GETRECT, (WPARAM)nID, (LPARAM)lpRect);
}

INLINE Int CXToolBar::GetTextRows(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TB_GETTEXTROWS, 0, 0L);
}

INLINE BOOL CXToolBar::SetButtonWidth(Int nMin, Int nMax)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_SETBUTTONWIDTH, 0, MAKELPARAM(nMin, nMax));
}

INLINE BOOL CXToolBar::SetIndent(Int nIndent)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_SETINDENT, (WPARAM)nIndent, 0L);
}

INLINE BOOL CXToolBar::SetMaxTextRows(Int nMaxTextRows)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_SETMAXTEXTROWS, (WPARAM)nMaxTextRows, 0L);
}

INLINE BOOL CXToolBar::GetAnchorHighlight(void) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_GETANCHORHIGHLIGHT, 0, 0L);
}

INLINE BOOL CXToolBar::SetAnchorHighlight(BOOL bEnable)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_SETANCHORHIGHLIGHT, (WPARAM)bEnable, 0L);
}

INLINE Int CXToolBar::GetButtonInfo(Int nID, LPTBBUTTONINFO lptbbi) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TB_GETBUTTONINFO, (WPARAM)nID, (LPARAM)lptbbi);
}

INLINE BOOL CXToolBar::SetButtonInfo(Int nID, LPTBBUTTONINFO lptbbi)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_SETBUTTONINFO, (WPARAM)nID, (LPARAM)lptbbi);
}

INLINE Int CXToolBar::GetHotItem(void) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TB_GETHOTITEM, 0, 0L);
}

INLINE Int CXToolBar::SetHotItem(Int nItem)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TB_SETHOTITEM, (WPARAM)nItem, 0L);
}

INLINE BOOL CXToolBar::IsButtonHighlighted(Int nID) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_ISBUTTONHIGHLIGHTED, (WPARAM)nID, 0L);
}

INLINE ULong CXToolBar::SetDrawTextFlags(ULong ulMask, ULong ulFlags)
{
	assert(::IsWindow(m_hWnd));
	return (ULong)::SendMessage(m_hWnd, TB_SETDRAWTEXTFLAGS, (WPARAM)ulMask, (LPARAM)ulFlags);
}

INLINE BOOL CXToolBar::GetColorScheme(LPCOLORSCHEME lpcs) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_GETCOLORSCHEME, 0, (LPARAM)lpcs);
}

INLINE void CXToolBar::SetColorScheme(LPCOLORSCHEME lpcs)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TB_SETCOLORSCHEME, 0, (LPARAM)lpcs);
}

INLINE ULong CXToolBar::GetExtendedStyle(void) const
{
	assert(::IsWindow(m_hWnd));
	return (ULong)::SendMessage(m_hWnd, TB_GETEXTENDEDSTYLE, 0, 0L);
}

INLINE ULong CXToolBar::SetExtendedStyle(ULong ulStyle)
{
	assert(::IsWindow(m_hWnd));
	return (ULong)::SendMessage(m_hWnd, TB_SETEXTENDEDSTYLE, 0, (LPARAM)ulStyle);
}

INLINE void CXToolBar::GetInsertMark(LPTBINSERTMARK lptbim) const
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TB_GETINSERTMARK, 0, (LPARAM)lptbim);
}

INLINE void CXToolBar::SetInsertMark(LPTBINSERTMARK lptbim)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TB_SETINSERTMARK, 0, (LPARAM)lptbim);
}

INLINE COLORREF CXToolBar::GetInsertMarkColor(void) const
{
	assert(::IsWindow(m_hWnd));
	return (COLORREF)::SendMessage(m_hWnd, TB_GETINSERTMARKCOLOR, 0, 0L);
}

INLINE COLORREF CXToolBar::SetInsertMarkColor(COLORREF clr)
{
	assert(::IsWindow(m_hWnd));
	return (COLORREF)::SendMessage(m_hWnd, TB_SETINSERTMARKCOLOR, 0, (LPARAM)clr);
}

INLINE BOOL CXToolBar::GetMaxSize(LPSIZE lpSize) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_GETMAXSIZE, 0, (LPARAM)lpSize);
}

INLINE void CXToolBar::GetPadding(LPSIZE lpSizePadding) const
{
	assert(::IsWindow(m_hWnd));
	assert(lpSizePadding != nullptr);
	ULong ulRet = (ULong)::SendMessage(m_hWnd, TB_GETPADDING, 0, 0L);
	lpSizePadding->cx = GET_X_LPARAM(ulRet);
	lpSizePadding->cy = GET_Y_LPARAM(ulRet);
}

INLINE void CXToolBar::SetPadding(Int ncx, Int ncy, LPSIZE lpSizePadding)
{
	assert(::IsWindow(m_hWnd));
	ULong ulRet = (ULong)::SendMessage(m_hWnd, TB_SETPADDING, 0, MAKELPARAM(ncx, ncy));
	if (lpSizePadding != nullptr)
	{
		lpSizePadding->cx = GET_X_LPARAM(ulRet);
		lpSizePadding->cy = GET_Y_LPARAM(ulRet);
	}
}

INLINE BOOL CXToolBar::GetUnicodeFormat(void) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_GETUNICODEFORMAT, 0, 0L);
}

INLINE BOOL CXToolBar::SetUnicodeFormat(BOOL bUnicode)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_SETUNICODEFORMAT, (WPARAM)bUnicode, 0L);
}
#if (_WIN32_WINNT >= 0x0501)
INLINE void CXToolBar::GetMetrics(LPTBMETRICS lptbm) const
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TB_GETMETRICS, 0, (LPARAM)lptbm);
}

INLINE void CXToolBar::SetMetrics(LPTBMETRICS lptbm)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TB_SETMETRICS, 0, (LPARAM)lptbm);
}

INLINE void CXToolBar::SetWindowTheme(PCWStr pszTheme)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TB_SETWINDOWTHEME, 0, (LPARAM)pszTheme);
}
#endif // (_WIN32_WINNT >= 0x0501)
#if (_WIN32_WINNT >= 0x0600)
INLINE HIMAGELIST CXToolBar::GetPressedImageList(Int nIndex) const
{
	assert(::IsWindow(m_hWnd));
	return (HIMAGELIST)::SendMessage(m_hWnd, TB_GETPRESSEDIMAGELIST, nIndex, 0L);
}

INLINE HIMAGELIST CXToolBar::SetPressedImageList(HIMAGELIST hImageList, Int nIndex)
{
	assert(::IsWindow(m_hWnd));
	return (HIMAGELIST)::SendMessage(m_hWnd, TB_SETPRESSEDIMAGELIST, nIndex, (LPARAM)hImageList);
}
#endif // (_WIN32_WINNT >= 0x0600)
INLINE Int CXToolBar::GetString(Int nString, PXStr pszString, Int nMaxLen) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TB_GETSTRING, MAKEWPARAM(nMaxLen, nString), (LPARAM)pszString);
}

INLINE BOOL CXToolBar::EnableButton(Int nID, BOOL bEnable)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_ENABLEBUTTON, (WPARAM)nID, MAKELPARAM(bEnable, 0));
}

INLINE BOOL CXToolBar::CheckButton(Int nID, BOOL bCheck)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_CHECKBUTTON, (WPARAM)nID, MAKELPARAM(bCheck, 0));
}

INLINE BOOL CXToolBar::PressButton(Int nID, BOOL bPress)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_PRESSBUTTON, (WPARAM)nID, MAKELPARAM(bPress, 0));
}

INLINE BOOL CXToolBar::HideButton(Int nID, BOOL bHide)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_HIDEBUTTON, (WPARAM)nID, MAKELPARAM(bHide, 0));
}

INLINE BOOL CXToolBar::Indeterminate(Int nID, BOOL bIndeterminate)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_INDETERMINATE, (WPARAM)nID, MAKELPARAM(bIndeterminate, 0));
}

INLINE Int CXToolBar::AddBitmap(Int nNumButtons, Int nBitmapID, HMODULE hResInst)
{
	assert(::IsWindow(m_hWnd));
	hResInst = hResInst ? hResInst : (::GetModuleHandle(nullptr));

	TBADDBITMAP tbab;
	tbab.hInst = hResInst;
	tbab.nID   = nBitmapID;
	return (Int)::SendMessage(m_hWnd, TB_ADDBITMAP, (WPARAM)nNumButtons, (LPARAM)&tbab);
}

INLINE Int CXToolBar::AddBitmap(Int nNumButtons, HBITMAP hBitmap)
{
	assert(::IsWindow(m_hWnd));
	TBADDBITMAP tbab;
	tbab.hInst = nullptr;
	tbab.nID   = (uintptr_t)hBitmap;
	return (Int)::SendMessage(m_hWnd, TB_ADDBITMAP, (WPARAM)nNumButtons, (LPARAM)&tbab);
}

INLINE BOOL CXToolBar::AddButtons(Int nNumButtons, LPTBBUTTON lpButtons)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_ADDBUTTONS, (WPARAM)nNumButtons, (LPARAM)lpButtons);
}

INLINE BOOL CXToolBar::InsertButton(Int nIndex, LPTBBUTTON lpButton)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_INSERTBUTTON, (WPARAM)nIndex, (LPARAM)lpButton);
}

INLINE BOOL CXToolBar::DeleteButton(Int nIndex)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_DELETEBUTTON, (WPARAM)nIndex, 0L);
}

INLINE Int CXToolBar::CommandToIndex(Int nID) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TB_COMMANDTOINDEX, (WPARAM)nID, 0L);
}

INLINE void CXToolBar::SaveState(HKEY hKeyRoot, PCXStr pszSubKey, PCXStr pszValueName)
{
	assert(::IsWindow(m_hWnd));
	TBSAVEPARAMS tbs;
	tbs.hkr          = hKeyRoot;
	tbs.pszSubKey    = pszSubKey;
	tbs.pszValueName = pszValueName;
	::SendMessage(m_hWnd, TB_SAVERESTORE, (WPARAM)TRUE, (LPARAM)&tbs);
}

INLINE void CXToolBar::RestoreState(HKEY hKeyRoot, PCXStr pszSubKey, PCXStr pszValueName)
{
	assert(::IsWindow(m_hWnd));
	TBSAVEPARAMS tbs;
	tbs.hkr          = hKeyRoot;
	tbs.pszSubKey    = pszSubKey;
	tbs.pszValueName = pszValueName;
	::SendMessage(m_hWnd, TB_SAVERESTORE, (WPARAM)FALSE, (LPARAM)&tbs);
}

INLINE void CXToolBar::Customize(void)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TB_CUSTOMIZE, 0, 0L);
}

INLINE Int CXToolBar::AddString(Int nStringID, HMODULE hResInst)
{
	assert(::IsWindow(m_hWnd));
	hResInst = hResInst ? hResInst : (::GetModuleHandle(nullptr));
	return (Int)::SendMessage(m_hWnd, TB_ADDSTRING, (WPARAM)hResInst, (LPARAM)nStringID);
}

INLINE Int CXToolBar::AddStrings(PCXStr pszStrings)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TB_ADDSTRING, 0, (LPARAM)pszStrings);
}

INLINE void CXToolBar::AutoSize(void)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TB_AUTOSIZE, 0, 0L);
}

INLINE BOOL CXToolBar::ChangeBitmap(Int nID, Int nBitmap)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_CHANGEBITMAP, (WPARAM)nID, MAKELPARAM(nBitmap, 0));
}

INLINE Int CXToolBar::LoadImages(Int nBitmapID, HMODULE hResInst)
{
	assert(::IsWindow(m_hWnd));
	hResInst = hResInst ? hResInst : (::GetModuleHandle(nullptr));
	return (Int)::SendMessage(m_hWnd, TB_LOADIMAGES, (WPARAM)nBitmapID, (LPARAM)hResInst);
}

INLINE Int CXToolBar::LoadStdImages(Int nBitmapID)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TB_LOADIMAGES, (WPARAM)nBitmapID, (LPARAM)HINST_COMMCTRL);
}

INLINE BOOL CXToolBar::ReplaceBitmap(LPTBREPLACEBITMAP ptbrb)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_REPLACEBITMAP, 0, (LPARAM)ptbrb);
}

INLINE Int CXToolBar::HitTest(LPPOINT lpPoint)
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, TB_HITTEST, 0, (LPARAM)lpPoint);
}

INLINE BOOL CXToolBar::InsertMarkHitTest(LPPOINT lpPoint, LPTBINSERTMARK lptbim)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_INSERTMARKHITTEST, (WPARAM)lpPoint, (LPARAM)lptbim);
}

INLINE BOOL CXToolBar::InsertMarkHitTest(Int nx, Int ny, LPTBINSERTMARK lptbim)
{
	assert(::IsWindow(m_hWnd));
	POINT pt = { nx, ny };
	return (BOOL)::SendMessage(m_hWnd, TB_INSERTMARKHITTEST, (WPARAM)&pt, (LPARAM)lptbim);
}

INLINE BOOL CXToolBar::MapAccelerator(XChar cAccel, Int& nID) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_MAPACCELERATOR, (WPARAM)cAccel, (LPARAM)&nID);
}

INLINE BOOL CXToolBar::MarkButton(Int nID, BOOL bHighlight)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_MARKBUTTON, (WPARAM)nID, MAKELPARAM(bHighlight, 0));
}

INLINE BOOL CXToolBar::MoveButton(Int nOldPos, Int nNewPos)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_MOVEBUTTON, (WPARAM)nOldPos, (LPARAM)nNewPos);
}

INLINE void CXToolBar::SetDropDowmBtn(Int nID, HMENU hMenu)
{
	assert(::IsWindow(m_hWnd)); 
	ULong ulStyle = GetExtendedStyle();
	ulStyle |= TBSTYLE_EX_DRAWDDARROWS;
	SetExtendedStyle(ulStyle);

	TBBUTTONINFO tbi;
	tbi.cbSize = sizeof(TBBUTTONINFO);
	tbi.dwMask = TBIF_STYLE|TBIF_LPARAM;

	GetButtonInfo(nID, &tbi);
	tbi.fsStyle |= TBSTYLE_DROPDOWN;
	tbi.lParam = (LPARAM)hMenu;
	SetButtonInfo(nID, &tbi);
}

INLINE void CXToolBar::DeleteAllButtons(void)
{ 
	for (Int i = GetButtonCount() - 1; i >= 0; --i)
	{
		DeleteButton(i);
	}
}

INLINE void CXToolBar::OnToolTip(LPTOOLTIPTEXT lptt, HMODULE hResInst)
{
	// hResInst special should modify
	hResInst = hResInst ? hResInst : (::GetModuleHandle(nullptr));
	lptt->hinst = hResInst;

	XChar szText[LMT_MIN] = { 0 };
	if (::LoadString(lptt->hinst, (UInt)(lptt->hdr.idFrom), szText, LMT_MIN) == 0)
	{
		return;
	}

	PXStr pszShow = (PXStr)CXChar::FindChar(szText, CXChar::LineChar);
	if (pszShow != nullptr)
	{
		*pszShow = 0;
		++pszShow;
	}
	lptt->lpszText = szText;
	assert(m_pOwner);
	::SendMessage(m_pOwner->m_hWnd, XUIM_NOTIFY, XUI_NOTIFY_TTSHOW, (LPARAM)pszShow);
}

INLINE LRESULT CXToolBar::OnNotify(Int, LPNMHDR pHead, BOOL&)
{
	switch (pHead->code)
	{
	case TTN_NEEDTEXT:
		{
			OnToolTip((LPTOOLTIPTEXT)pHead);
		}
		break;
	case TTN_POP:
		{
			assert(m_pOwner);
			::SendMessage(m_pOwner->m_hWnd, XUIM_NOTIFY, XUI_NOTIFY_TTHIDE, 0L);
		}
		break;
	case TBN_DROPDOWN:
		{
			LPNMTOOLBAR lpT = (LPNMTOOLBAR)pHead;

			TBBUTTONINFO tbi;
			tbi.cbSize = sizeof(TBBUTTONINFO);
			tbi.dwMask = TBIF_LPARAM;
			GetButtonInfo(lpT->iItem, &tbi);

			HMENU hMenu = (HMENU)tbi.lParam;
			if (::IsMenu(hMenu))
			{
				RECT rt;
				GetItemRect(CommandToIndex(lpT->iItem), &rt);
				ClientToScreen(&rt);
				::TrackPopupMenuEx(hMenu,
								   TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
								   rt.left, rt.bottom, m_pOwner->m_hWnd, nullptr);
			}
			return TBDDRET_DEFAULT;
		}
		break;
	default:
		{
		}
	}
	return 0;
}

INLINE LRESULT CXToolBar::OnXLayout(WPARAM wParam, LPARAM lParam, BOOL&)
{
	assert(::IsWindowVisible(m_hWnd));
	assert(HIWORD(wParam) == XUI_LAYOUT_RECT);
	::SendMessage(m_hWnd, WM_SIZE, 0, 0L);
	RECT rt;
	GetWindowRect(&rt);
	((LPRECT)lParam)->top += (rt.bottom - rt.top);
	return HIWORD(wParam);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXMenuBar : MENUBARCLASSNAME
// Style : within rebar used XUIS_MENUBAR_PANE
SELECTANY CXMenuBar*   CXMenuBar::ms_pXMenuBar = nullptr;
SELECTANY HHOOK	       CXMenuBar::ms_hMenuHook = nullptr;

INLINE CXMenuBar::CXMenuBar(void)
: CXControl(XWNDT_MENUBAR)
, m_hMenu(nullptr)
, m_nItem(0)
, m_nPressed(0)
, m_nRows(0)
{
	m_ptMouse.x = 0;
	m_ptMouse.y = 0;
}

INLINE CXMenuBar::~CXMenuBar(void)
{
	if (::IsMenu(m_hMenu))
	{
		::DestroyMenu(m_hMenu);
	}
}

INLINE CXMenuBar::CXMenuBar(const CXMenuBar&)
: CXControl(XWNDT_MENUBAR)
, m_hMenu(nullptr)
, m_nItem(0)
, m_nPressed(0)
, m_nRows(0)
{
}

INLINE CXMenuBar& CXMenuBar::operator=(const CXMenuBar&)
{
	return (*this);
}

INLINE PCXStr CXMenuBar::GetXWndClass(void)
{
	return MENUBARCLASSNAME;
}

INLINE bool CXMenuBar::CreateMenu(void)
{
	assert(::IsWindow(m_hWnd));
	SetMenuBarEmpty(); 
	::SendMessage(m_hWnd, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0L);
	m_hMenu = ::CreateMenu();
	return (m_hMenu != nullptr);
}

INLINE void CXMenuBar::SetMenuBarEmpty(void)
{
	assert(::IsWindow(m_hWnd));
	if (m_hMenu != nullptr)
	{
		::DestroyMenu(m_hMenu);
	}
	m_hMenu      = nullptr;
	m_nPressed   = 0;
	m_nRows      = 0;
	m_nItem      = (Int)::SendMessage(m_hWnd, TB_BUTTONCOUNT, 0, 0L);
	for (; m_nItem > 0;)
	{
		--m_nItem;
		::SendMessage(m_hWnd, TB_DELETEBUTTON, (WPARAM)m_nItem, 0L);
	}
	m_nItem = 0;
	m_ptMouse.x = 0;
	m_ptMouse.y = 0;
	DEV_DEBUG(TF("  MenulBar set empty"));
}

INLINE BOOL CXMenuBar::EnableMenuBarItem(Int nIndex, bool bEnable)
{
	assert(::IsMenu(m_hMenu));
	assert(nIndex < m_nItem); 
	if ((nIndex < 0) || (nIndex >= m_nItem))
	{
		return FALSE;
	}
	MENUITEMINFO mii;
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask  = MIIM_STATE;
	mii.fState = bEnable ? MFS_ENABLED : MFS_DISABLED; 
	::SetMenuItemInfo(m_hMenu, nIndex, TRUE, &mii);

	return (BOOL)::SendMessage(m_hWnd, TB_ENABLEBUTTON, (WPARAM)nIndex, MAKELPARAM(bEnable, 0));
}

INLINE BOOL CXMenuBar::AppendMenuBarItem(UInt uFlags, uintptr_t utIDNewItem, PCXStr pszNewItem)
{
	assert(::IsMenu(m_hMenu));
	// current menubar item only support string
	// have no check status and break 
	assert((uFlags & MF_BITMAP) != MF_BITMAP);
	assert((uFlags & MF_OWNERDRAW) != MF_OWNERDRAW); 

	TBBUTTON tbb; 
	tbb.iBitmap   = -1;
	tbb.idCommand = m_nItem;
	if ((uFlags & MF_DISABLED) || (uFlags & MF_GRAYED))
	{
		tbb.fsState = 0;
	}
	else
	{
		tbb.fsState = TBSTATE_ENABLED;
	}
	tbb.fsStyle = TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE;
	tbb.dwData  = 0;
	tbb.iString = (Int)::SendMessage(m_hWnd, TB_ADDSTRING, 0, (LPARAM)pszNewItem);

	if (::SendMessage(m_hWnd, TB_ADDBUTTONS, 1, (LPARAM)&tbb)) 
	{
		++m_nItem;
		return ::AppendMenu(m_hMenu, uFlags, utIDNewItem, pszNewItem);
	}
	return FALSE;
}

INLINE BOOL CXMenuBar::InsertMenuBarItem(Int nIndex, LPCMENUITEMINFO lpmii)
{
	assert(::IsMenu(m_hMenu));
	assert(lpmii != nullptr);

	TBBUTTON tbb; 
	tbb.iBitmap   = -1;
	tbb.idCommand = nIndex;
	if ((lpmii->fMask & MIIM_STATE) && (lpmii->fState & MFS_GRAYED))
	{
		tbb.fsState = 0;
	}
	else
	{
		tbb.fsState = TBSTATE_ENABLED;
	}
	tbb.fsStyle = TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE;
	tbb.dwData = 0;
	tbb.iString = (Int)::SendMessage(m_hWnd, TB_ADDSTRING, 0, (LPARAM)(lpmii->dwTypeData)); 

	if (::SendMessage(m_hWnd, TB_INSERTBUTTON, (WPARAM)nIndex, (LPARAM)&tbb))
	{
		++m_nItem;
		for (Int i = nIndex + 1; i < m_nItem; ++i)
		{
			::SendMessage(m_hWnd, TB_SETCMDID, (WPARAM)i, (LPARAM)i);
		}
		return ::InsertMenuItem(m_hMenu, nIndex, TRUE, lpmii);
	}
	return FALSE;
}

INLINE BOOL CXMenuBar::RemoveMenuBarItem(Int nIndex)
{
	assert(::IsMenu(m_hMenu));
	if (::SendMessage(m_hWnd, TB_DELETEBUTTON, (WPARAM)nIndex, 0L))
	{
		::DeleteMenu(m_hMenu, nIndex, MF_BYPOSITION);
		--m_nItem;
		for (Int i = nIndex; i < m_nItem; ++i)
		{
			::SendMessage(m_hWnd, TB_SETCMDID, (WPARAM)i, (LPARAM)i);
		}
	}
	return FALSE;
}

INLINE BOOL CXMenuBar::GetMenuBarItemString(Int nIndex, PXStr pszBuffer, Int nBufferSize)
{
	assert(::IsMenu(m_hMenu));
	Int nCount = (Int)::SendMessage(m_hWnd, TB_GETBUTTONTEXT, (WPARAM)nIndex, 0L);
	if (nCount > 0)
	{ 
		assert(nCount < LMT_MAX_PATH);
		XChar szText[LMT_MAX_PATH] = { 0 };
		::SendMessage(m_hWnd, TB_GETBUTTONTEXT, (WPARAM)nIndex, (LPARAM)szText);
		szText[nCount] = 0;

		CXChar::Copy(pszBuffer, nBufferSize, szText, LMT_MAX_PATH);
		return TRUE;
	}
	return FALSE;
}

INLINE BOOL CXMenuBar::SetMenuBarItemString(Int nIndex, PXStr pszBuffer)
{
	assert(::IsMenu(m_hMenu));
	TBBUTTONINFO tbbi;
	tbbi.cbSize  = sizeof(TBBUTTONINFO);
	tbbi.dwMask  = TBIF_TEXT;
	tbbi.pszText = pszBuffer;

	return (BOOL)::SendMessage(m_hWnd, TB_SETBUTTONINFO, (WPARAM)nIndex, (LPARAM)&tbbi);
}

INLINE HMENU CXMenuBar::GetMenuBarSubMenu(Int nIndex)
{
	assert(::IsMenu(m_hMenu));
	return ::GetSubMenu(m_hMenu, nIndex);
}

INLINE BOOL CXMenuBar::SetMenuBarSubMenu(Int nIndex, HMENU hSubMenu)
{
	assert(::IsMenu(m_hMenu));
	assert(::IsMenu(hSubMenu));
	MENUITEMINFO mii;
	mii.cbSize   = sizeof(MENUITEMINFO);
	mii.fMask    = MIIM_SUBMENU;
	mii.hSubMenu = hSubMenu; 
	return ::SetMenuItemInfo(m_hMenu, nIndex, TRUE, &mii);
}

INLINE BOOL CXMenuBar::GetMenuInfo(LPMENUINFO lpMenuInfo) const
{
	assert(::IsMenu(m_hMenu));
	return ::GetMenuInfo(m_hMenu, lpMenuInfo);
}

INLINE BOOL CXMenuBar::SetMenuInfo(LPCMENUINFO lpMenuInfo)
{
	assert(::IsMenu(m_hMenu));
	return ::SetMenuInfo(m_hMenu, lpMenuInfo);
}

INLINE Int CXMenuBar::GetMenuBarItemCount(void) const
{
	return m_nItem;
}

INLINE BOOL CXMenuBar::GetMenuBarItemRect(Int nIndex, LPRECT lpRect)
{
	assert(lpRect != nullptr);
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TB_GETITEMRECT, (WPARAM)nIndex, (LPARAM)lpRect);
}

INLINE Int CXMenuBar::MenuBarItemFromPoint(LPPOINT lppt)
{
	assert(lppt != nullptr);
	assert(::IsWindow(m_hWnd));
	::ScreenToClient(m_hWnd, lppt);
	return (Int)::SendMessage(m_hWnd, TB_HITTEST, 0, (LPARAM)lppt);
}

INLINE void CXMenuBar::HiliteMenuBarItem(Int nIndex)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TB_SETHOTITEM, (WPARAM)nIndex, 0L);
}

INLINE bool CXMenuBar::LoadMenuBar(UInt uMenuID, HMODULE hResInst)
{
	assert(::IsWindow(m_hWnd));

	hResInst = hResInst ? hResInst : (::GetModuleHandle(nullptr));
	HMENU hMenu = ::LoadMenu(hResInst, MAKEINTRESOURCE(uMenuID));
	if (hMenu == nullptr) 
	{
		return false;
	}
	return LoadMenuBar(hMenu);
}

INLINE bool CXMenuBar::LoadMenuBar(HMENU hMenu)
{
	assert(::IsWindow(m_hWnd));
	assert(::IsMenu(hMenu));
	assert(m_hMenu != hMenu);
	if ((::IsWindow(m_hWnd) == FALSE) || (::IsMenu(hMenu) == FALSE))
	{
		return false;
	}
	SetMenuBarEmpty();
	::SendMessage(m_hWnd, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0L);
	::SendMessage(m_hWnd, TB_SETBITMAPSIZE, 0, MAKELPARAM(0, 0)); //no icon

	TBBUTTON tbb; 
	tbb.iBitmap   = -1;
	tbb.fsStyle   = TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE;
	tbb.dwData    = 0;

	XChar szText[LMT_MIN] = { 0 };

	MENUITEMINFO mii;
	mii.cbSize     = sizeof(MENUITEMINFO);
	mii.fMask      = MIIM_STRING | MIIM_STATE;
	mii.dwTypeData = szText;
	bool  bRet     = false;
	m_nItem = ::GetMenuItemCount(hMenu);
	for (Int i = 0; i < m_nItem; ++i)
	{
		mii.cch = LMT_MIN; 
		bRet = ::GetMenuItemInfo(hMenu, i, TRUE, &mii) ? true : false;
		assert(bRet);

		tbb.idCommand = i;
		tbb.fsState   = ((mii.fState & MFS_DISABLED) == 0) ? TBSTATE_ENABLED : 0;
		tbb.iString   = (Int)::SendMessage(m_hWnd, TB_ADDSTRING, 0, (LPARAM)szText);

		bRet = ::SendMessage(m_hWnd, TB_INSERTBUTTON, (WPARAM)-1, (LPARAM)&tbb) ? true : false;
		assert(bRet);
	}
	m_nRows = (Int)::SendMessage(m_hWnd, TB_GETROWS, 0, 0L);
	DEV_DEBUG(TF("  MenulBar[%p] load %d items"), this, m_nItem);

	m_hMenu = hMenu;
	::SetMenu(m_pOwner->m_hWnd, nullptr); // make sure system default menubar is nullptr
	return bRet;
}

INLINE bool CXMenuBar::TrackPopup(void)
{
	Int nButton = m_nPressed;
	HMENU hSubMenu = ::GetSubMenu(m_hMenu, nButton);
	if (hSubMenu == nullptr)
	{
		return false;
	}

	::SendMessage(m_hWnd, TB_PRESSBUTTON, (WPARAM)nButton, MAKELPARAM(TRUE, 0));

	TPMPARAMS tpm;
	tpm.cbSize    = sizeof(TPMPARAMS);

	::SendMessage(m_hWnd, TB_GETITEMRECT, (WPARAM)nButton, (LPARAM)&tpm.rcExclude);
	ClientToScreen(&tpm.rcExclude);

	// filter
	ms_pXMenuBar = this;
	ms_hMenuHook = ::SetWindowsHookEx(WH_MSGFILTER, MenuInputFilter, nullptr, ::GetCurrentThreadId());

	::TrackPopupMenuEx(hSubMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
		               tpm.rcExclude.left, tpm.rcExclude.bottom, m_pOwner->m_hWnd, &tpm);

	::UnhookWindowsHookEx(ms_hMenuHook);
	ms_hMenuHook = nullptr;
	ms_pXMenuBar = nullptr;

	// end filter
	::SendMessage(m_hWnd, TB_PRESSBUTTON, (WPARAM)nButton, MAKELPARAM(FALSE, 0));
	return true;
}

INLINE bool CXMenuBar::PreTranslateMessage(LPMSG pMsg)
{
	if ((pMsg->message == WM_SYSCHAR) && ::IsWindowEnabled(m_hWnd))
	{
		Int nID = 0;
		if (::SendMessage(m_hWnd, TB_MAPACCELERATOR, pMsg->wParam, (LPARAM)&nID))
		{
			m_nPressed = nID;
			::PostMessage(m_pOwner->m_hWnd, WM_CANCELMODE, 0, 0L);
			::PostMessage(m_hWnd, XUIM_NOTIFY, XUI_NOTIFY_POPOUT, 0L);
			return true;
		}
	}
	return false;
}

INLINE LRESULT CXMenuBar::OnXNotify(WPARAM wParam, LPARAM, BOOL&)
{
	switch (wParam)
	{
	case XUI_NOTIFY_POPOUT :
		{
			if (m_nPressed < 0)
			{
				m_nPressed = m_nItem - 1;
			}
			else if (m_nPressed >= m_nItem)
			{
				m_nPressed = 0;
			}

			if (::SendMessage(m_hWnd, TB_ISBUTTONENABLED, (WPARAM)m_nPressed, 0L))
			{
				TrackPopup();
			}
		}
		break;
	case XUI_NOTIFY_BREAK :
		{
			Int i = (Int)::SendMessage(m_hWnd, TB_GETROWS, 0, 0L);
			if (i != m_nRows)
			{		
				m_nRows = i;
				return i;
			}
		}
		break;
	default :
		{
		}
	}
	return 0;
}

INLINE LRESULT CXMenuBar::OnLButtonDown(WPARAM, LPARAM lParam, BOOL&)
{
	m_ptMouse.x = GET_X_LPARAM(lParam);
	m_ptMouse.y = GET_Y_LPARAM(lParam);

	Int nTest = (Int)::SendMessage(m_hWnd, TB_HITTEST, 0, (LPARAM)&m_ptMouse);

	if ((nTest >= 0) && (nTest < m_nItem))
	{
		m_nPressed = nTest;
		if (::SendMessage(m_hWnd, TB_ISBUTTONENABLED, (WPARAM)nTest, 0L))
		{
			TrackPopup();
		}
	}
	return 0;
}

INLINE bool CXMenuBar::OnMenuInput(PMSG pMsg)
{
	bool bRet = FALSE;

	switch (pMsg->message)
	{
	case WM_MOUSEMOVE:
		{
			POINT pt = { GET_X_LPARAM(pMsg->lParam), GET_Y_LPARAM(pMsg->lParam) };
			::ScreenToClient(ms_pXMenuBar->m_hWnd, &pt);
			
			if ((m_ptMouse.x == pt.x) && (m_ptMouse.y == pt.y))
			{
				return true;
			}
			m_ptMouse.x = pt.x;
			m_ptMouse.y = pt.y;

			Int nTest = (Int)::SendMessage(m_hWnd, TB_HITTEST, 0, (LPARAM)&pt);
			if ((nTest >= 0) && 
				(nTest < m_nItem)  && 
				(nTest != m_nPressed))
			{
				::PostMessage(m_pOwner->m_hWnd, WM_CANCELMODE, 0, 0L);
				m_nPressed = nTest;
				::PostMessage(m_hWnd, XUIM_NOTIFY, XUI_NOTIFY_POPOUT, 0L);
				bRet = true;
			}
		}
		break;
	case WM_MENUSELECT:
		{
			OnMenuSelect(pMsg->wParam, pMsg->lParam);
		}
		break;	
	case WM_LBUTTONDOWN :
		{
			::PostMessage(m_pOwner->m_hWnd, XUIM_NOTIFY, XUI_NOTIFY_TTHIDE, 0L);

			POINT pt = { GET_X_LPARAM(pMsg->lParam), GET_Y_LPARAM(pMsg->lParam) };
			::ScreenToClient(m_hWnd, &pt);

			if (m_nPressed == ::SendMessage(m_hWnd, TB_HITTEST, 0, (LPARAM)&pt))
			{
				::PostMessage(m_pOwner->m_hWnd, WM_CANCELMODE, 0, 0L);
				bRet = true;
			}
		}
		break;
	case WM_KEYDOWN :
		{
			switch (pMsg->wParam)
			{
			case VK_LEFT :
				{
					--(m_nPressed);
					::PostMessage(m_pOwner->m_hWnd, WM_CANCELMODE, 0, 0L);
					::PostMessage(m_hWnd, XUIM_NOTIFY, XUI_NOTIFY_POPOUT, 0L);
					bRet = true;
				}
				break;
			case VK_RIGHT :
				{
					++(m_nPressed);
					::PostMessage(m_pOwner->m_hWnd, WM_CANCELMODE, 0, 0L);
					::PostMessage(m_hWnd, XUIM_NOTIFY, XUI_NOTIFY_POPOUT, 0L);
					bRet = true;
				}
				break;
			case VK_ESCAPE : 
				{
					::PostMessage(m_pOwner->m_hWnd, XUIM_NOTIFY, XUI_NOTIFY_TTHIDE, 0L);
					::PostMessage(m_pOwner->m_hWnd, WM_CANCELMODE, 0, 0L);
					bRet = true;
				}
				break;
			default :
				{
				}
			}
		}
		break;
	default :
		{
		}
	}
	return bRet;
}

INLINE void CXMenuBar::OnMenuSelect(WPARAM wParam, LPARAM, HMODULE hResInst)
{
	// hResInst special should modify
	hResInst = hResInst ? hResInst : (::GetModuleHandle(nullptr));
	XChar szText[LMT_MIN] = { 0 };
	if (::LoadString(hResInst, (UInt)LOWORD(wParam), szText, LMT_MIN) == 0)
	{
		CXChar::Copy(szText, LMT_MIN, DefaultReady, LMT_MIN);
	}

	PXStr pszShow = (PXStr)CXChar::FindChar(szText, CXChar::LineChar);
	if (pszShow == nullptr)
	{
		CXChar::Copy(szText, LMT_MIN, DefaultReady, LMT_MIN);
		pszShow = szText;
	}
	++pszShow;
	::SendMessage(m_pOwner->m_hWnd, XUIM_NOTIFY, XUI_NOTIFY_TTSHOW, (LPARAM)pszShow);
}

INLINE LRESULT CALLBACK CXMenuBar::MenuInputFilter(Int nCode, WPARAM wParam, LPARAM lParam)
{
	if ((nCode == MSGF_MENU) && (ms_pXMenuBar != nullptr))
	{
		if (ms_pXMenuBar->OnMenuInput((PMSG)lParam))
		{
			return TRUE;
		}
	}
	return CallNextHookEx(ms_hMenuHook, nCode, wParam, lParam);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXStatusBar : STATUSCLASSNAME
// Style : XUIS_STATUSBAR
INLINE CXStatusBar::CXStatusBar(void)
: CXControl(XWNDT_STATUSBAR)
{
}

INLINE CXStatusBar::~CXStatusBar(void)
{
}

INLINE CXStatusBar::CXStatusBar(const CXStatusBar&)
: CXControl(XWNDT_STATUSBAR)
{
}

INLINE CXStatusBar& CXStatusBar::operator=(const CXStatusBar&)
{
	return (*this);
}

INLINE PCXStr CXStatusBar::GetXWndClass(void)
{
	return STATUSCLASSNAME;
}

INLINE Int CXStatusBar::GetParts(Int nParts, PInt pnParts) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, SB_GETPARTS, (WPARAM)nParts, (LPARAM)pnParts);
}

INLINE BOOL CXStatusBar::SetParts(Int nParts, PInt pnWidths)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, SB_SETPARTS, (WPARAM)nParts, (LPARAM)pnWidths);
}

INLINE Int CXStatusBar::GetTextLength(Int nPane, PInt pnType) const
{
	assert(::IsWindow(m_hWnd));
	ULong ulRet = (ULong)::SendMessage(m_hWnd, SB_GETTEXTLENGTH, (WPARAM)nPane, 0L);

	if (pnType != nullptr)
	{
		*pnType = GET_Y_LPARAM(ulRet);
	}
	return GET_X_LPARAM(ulRet);
}

INLINE Int  CXStatusBar::GetText(Int nPane, PXStr pszText, PInt pnType) const
{
	assert(::IsWindow(m_hWnd));

	ULong ulRet = (ULong)::SendMessage(m_hWnd, SB_GETTEXT, (WPARAM)nPane, (LPARAM)pszText);

	if (pnType != nullptr)
	{
		*pnType = GET_Y_LPARAM(ulRet);
	}
	return GET_X_LPARAM(ulRet);
}

INLINE BOOL CXStatusBar::SetText(Int nPane, PCXStr pszText, Int nType)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, SB_SETTEXT, (WPARAM)(nPane|nType), (LPARAM)pszText);
}

INLINE BOOL CXStatusBar::GetRect(Int nPane, LPRECT lpRect) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, SB_GETRECT, (WPARAM)nPane, (LPARAM)lpRect);
}

INLINE BOOL CXStatusBar::GetBorders(PInt pnBorders) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, SB_GETBORDERS, 0, (LPARAM)pnBorders);
}

INLINE BOOL CXStatusBar::GetBorders(Int& nHorz, Int& nVert, Int& nSpacing) const
{
	assert(::IsWindow(m_hWnd));

	Int borders[3] = { 0, 0, 0 };
	BOOL bRet = (BOOL)::SendMessage(m_hWnd, SB_GETBORDERS, 0, (LPARAM)&borders);
	if (bRet)
	{
		nHorz    = borders[0];
		nVert    = borders[1];
		nSpacing = borders[2];
	}
	return bRet;
}

INLINE void CXStatusBar::SetMinHeight(Int nMin)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, SB_SETMINHEIGHT, (WPARAM)nMin, 0L);
}

INLINE BOOL CXStatusBar::SetSimple(BOOL bSimple)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, SB_SIMPLE, (WPARAM)bSimple, 0L);
}

INLINE BOOL CXStatusBar::IsSimple(void) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, SB_ISSIMPLE, 0, 0L);
}

INLINE BOOL CXStatusBar::GetUnicodeFormat(void) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, SB_GETUNICODEFORMAT, 0, 0L);
}

INLINE BOOL CXStatusBar::SetUnicodeFormat(BOOL bUnicode)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, SB_SETUNICODEFORMAT, (WPARAM)bUnicode, 0L);
}

INLINE void CXStatusBar::GetTipText(Int nPane, PXStr pszText, Int nSize) const
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, SB_GETTIPTEXT, MAKEWPARAM(nPane, nSize), (LPARAM)pszText);
}

INLINE void CXStatusBar::SetTipText(Int nPane, PCXStr pszText)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, SB_SETTIPTEXT, (WPARAM)nPane, (LPARAM)pszText);
}

INLINE HICON CXStatusBar::GetIcon(Int nPane) const
{
	assert(::IsWindow(m_hWnd));
	return (HICON)::SendMessage(m_hWnd, SB_GETICON, (WPARAM)nPane, 0L);
}

INLINE BOOL CXStatusBar::SetIcon(Int nPane, HICON hIcon)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, SB_SETICON, (WPARAM)nPane, (LPARAM)hIcon);
}

INLINE COLORREF CXStatusBar::SetBkColor(COLORREF clrBk)
{
	assert(::IsWindow(m_hWnd));
	return (COLORREF)::SendMessage(m_hWnd, SB_SETBKCOLOR, 0, (LPARAM)clrBk);
}

INLINE LRESULT CXStatusBar::OnXLayout(WPARAM wParam, LPARAM lParam, BOOL&)
{
	assert(::IsWindowVisible(m_hWnd));
	assert(HIWORD(wParam) == XUI_LAYOUT_RECT);
	::SendMessage(m_hWnd, WM_SIZE, 0, 0L);
	RECT rt;
	GetWindowRect(&rt);
	((LPRECT)lParam)->bottom -= (rt.bottom - rt.top);
	return HIWORD(wParam);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXReBar : REBARCLASSNAME
// Style : XUIS_REBAR or XUIS_REBAR_NOBORDER
// CXReBar used REBARBANDINFO's cxMinChild, cyMinChild and lParam
// cxMinChild : must be 0, for menubar wrapper
// cyMinChild : the actual height
// lParam     : | -- 2 BYTE -- | -- 2 BYTE -- |
//                     |              | the init cyMinChild value
//                     | menubar row count
INLINE CXReBar::CXReBar(void)
: CXControl(XWNDT_REBAR)
{
}

INLINE CXReBar::~CXReBar(void)
{
}

INLINE CXReBar::CXReBar(const CXReBar&)
: CXControl(XWNDT_REBAR)
{
}

INLINE CXReBar& CXReBar::operator=(const CXReBar&)
{
	return (*this);
}

INLINE PCXStr CXReBar::GetXWndClass(void)
{
	return REBARCLASSNAME;
}

INLINE UInt CXReBar::GetBandCount(void) const
{
	assert(::IsWindow(m_hWnd));
	return (UInt)::SendMessage(m_hWnd, RB_GETBANDCOUNT, 0, 0L);
}

INLINE BOOL CXReBar::GetBandInfo(Int nBand, LPREBARBANDINFO lprbbi) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, RB_GETBANDINFO, (WPARAM)nBand, (LPARAM)lprbbi);
}

INLINE BOOL CXReBar::SetBandInfo(Int nBand, LPREBARBANDINFO lprbbi)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, RB_SETBANDINFO, (WPARAM)nBand, (LPARAM)lprbbi);
}

INLINE BOOL CXReBar::GetBarInfo(LPREBARINFO lprbi) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, RB_GETBARINFO, 0, (LPARAM)lprbi);
}

INLINE BOOL CXReBar::SetBarInfo(LPREBARINFO lprbi)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, RB_SETBARINFO, 0, (LPARAM)lprbi);
}

INLINE UInt CXReBar::GetRowCount(void) const
{
	assert(::IsWindow(m_hWnd));
	return (UInt)::SendMessage(m_hWnd, RB_GETROWCOUNT, 0, 0L);
}

INLINE UInt CXReBar::GetRowHeight(Int nBand) const
{
	assert(::IsWindow(m_hWnd));
	return (UInt)::SendMessage(m_hWnd, RB_GETROWHEIGHT, (WPARAM)nBand, 0L);
}

INLINE COLORREF CXReBar::GetTextColor(void) const
{
	assert(::IsWindow(m_hWnd));
	return (COLORREF)::SendMessage(m_hWnd, RB_GETTEXTCOLOR, 0, 0L);
}

INLINE COLORREF CXReBar::SetTextColor(COLORREF clr)
{
	assert(::IsWindow(m_hWnd));
	return (COLORREF)::SendMessage(m_hWnd, RB_SETTEXTCOLOR, 0, (LPARAM)clr);
}

INLINE COLORREF CXReBar::GetBkColor(void) const
{
	assert(::IsWindow(m_hWnd));
	return (COLORREF)::SendMessage(m_hWnd, RB_GETBKCOLOR, 0, 0L);
}

INLINE COLORREF CXReBar::SetBkColor(COLORREF clr)
{
	assert(::IsWindow(m_hWnd));
	return (COLORREF)::SendMessage(m_hWnd, RB_SETBKCOLOR, 0, (LPARAM)clr);
}

INLINE UInt CXReBar::GetBarHeight(void) const
{
	assert(::IsWindow(m_hWnd));
	return (UInt)::SendMessage(m_hWnd, RB_GETBARHEIGHT, 0, 0L);
}

INLINE BOOL CXReBar::GetRect(Int nBand, LPRECT lpRect) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, RB_GETRECT, (WPARAM)nBand, (LPARAM)lpRect);
}

INLINE HWND CXReBar::GetToolTips(void) const
{
	assert(::IsWindow(m_hWnd));
	return (HWND)::SendMessage(m_hWnd, RB_GETTOOLTIPS, 0, 0L);
}

INLINE void CXReBar::SetToolTips(HWND hwndToolTip)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, RB_SETTOOLTIPS, (WPARAM)hwndToolTip, 0L);
}

INLINE void CXReBar::GetBandBorders(Int nBand, LPRECT lpRect) const
{
	assert(::IsWindow(m_hWnd));
	assert(lpRect != nullptr);
	::SendMessage(m_hWnd, RB_GETBANDBORDERS, (WPARAM)nBand, (LPARAM)lpRect);
}

INLINE BOOL CXReBar::GetColorScheme(LPCOLORSCHEME lpColorScheme) const
{
	assert(::IsWindow(m_hWnd));
	assert(lpColorScheme != nullptr);
	return (BOOL)::SendMessage(m_hWnd, RB_GETCOLORSCHEME, 0, (LPARAM)lpColorScheme);
}

INLINE void CXReBar::SetColorScheme(LPCOLORSCHEME lpColorScheme)
{
	assert(::IsWindow(m_hWnd));
	assert(lpColorScheme != nullptr);
	::SendMessage(m_hWnd, RB_SETCOLORSCHEME, 0, (LPARAM)lpColorScheme);
}

INLINE HPALETTE CXReBar::GetPalette(void) const
{
	assert(::IsWindow(m_hWnd));
	return (HPALETTE)::SendMessage(m_hWnd, RB_GETPALETTE, 0, 0L);
}

INLINE HPALETTE CXReBar::SetPalette(HPALETTE hPalette)
{
	assert(::IsWindow(m_hWnd));
	return (HPALETTE)::SendMessage(m_hWnd, RB_SETPALETTE, 0, (LPARAM)hPalette);
}

INLINE BOOL CXReBar::GetUnicodeFormat(void) const
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, RB_GETUNICODEFORMAT, 0, 0L);
}

INLINE BOOL CXReBar::SetUnicodeFormat(BOOL bUnicode)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, RB_SETUNICODEFORMAT, (WPARAM)bUnicode, 0L);
}

INLINE BOOL CXReBar::InsertBand(Int nBand, LPREBARBANDINFO lprbbi)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, RB_INSERTBAND, (WPARAM)nBand, (LPARAM)lprbbi);
}

INLINE BOOL CXReBar::DeleteBand(Int nBand)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, RB_DELETEBAND, (WPARAM)nBand, 0L);
}

INLINE HWND CXReBar::SetNotifyWnd(HWND hWnd)
{
	assert(::IsWindow(m_hWnd));
	return (HWND)::SendMessage(m_hWnd, RB_SETPARENT, (WPARAM)hWnd, 0L);
}

INLINE void CXReBar::BeginDrag(Int nBand, ULong ulPos)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, RB_BEGINDRAG, (WPARAM)nBand, (LPARAM)ulPos);
}

INLINE void CXReBar::BeginDrag(Int nBand, Int nxPos, Int nyPos)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, RB_BEGINDRAG, (WPARAM)nBand, MAKELPARAM(nxPos, nyPos));
}

INLINE void CXReBar::EndDrag(void)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, RB_ENDDRAG, 0, 0L);
}

INLINE void CXReBar::DragMove(ULong ulPos)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, RB_DRAGMOVE, 0, (LPARAM)ulPos);
}

INLINE void CXReBar::DragMove(Int nxPos, Int nyPos)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, RB_DRAGMOVE, 0, MAKELPARAM(nxPos, nyPos));
}

INLINE void CXReBar::MaximizeBand(Int nBand)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, RB_MAXIMIZEBAND, (WPARAM)nBand, 0L);
}

INLINE void CXReBar::MinimizeBand(Int nBand)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, RB_MINIMIZEBAND, (WPARAM)nBand, 0L);
}

INLINE BOOL CXReBar::SizeToRect(LPRECT lpRect)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, RB_SIZETORECT, 0, (LPARAM)lpRect);
}

INLINE Int CXReBar::IdToIndex(UInt uBandID) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, RB_IDTOINDEX, (WPARAM)uBandID, 0L);
}

INLINE Int CXReBar::HitTest(LPRBHITTESTINFO lprbht) const
{
	assert(::IsWindow(m_hWnd));
	return (Int)::SendMessage(m_hWnd, RB_HITTEST, 0, (LPARAM)lprbht);
}

INLINE BOOL CXReBar::ShowBand(Int nBand, BOOL bShow)
{
	assert(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, RB_SHOWBAND, (WPARAM)nBand, (LPARAM)bShow);
}

INLINE BOOL CXReBar::MoveBand(Int nBand, Int nNewPos)
{
	assert(::IsWindow(m_hWnd));
	assert(nNewPos >= 0 && nNewPos <= ((Int)GetBandCount() - 1));
	return (BOOL)::SendMessage(m_hWnd, RB_MOVEBAND, (WPARAM)nBand, (LPARAM)nNewPos);
}

INLINE void CXReBar::PushChevron(Int nBand, LPARAM lAppValue)
{
	assert(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, RB_PUSHCHEVRON, (WPARAM)nBand, lAppValue);
}

INLINE void CXReBar::LockBands(bool bLock)
{
	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof(REBARBANDINFO);
	rbbi.fMask = RBBIM_STYLE;
	Int nBandCount = GetBandCount();
	for (Int i = 0; i < nBandCount; ++i)
	{
		GetBandInfo(i, &rbbi);

		if (bLock)
		{
			rbbi.fStyle |= RBBS_NOGRIPPER;
		}
		else
		{
			rbbi.fStyle &= ~RBBS_NOGRIPPER;
		}
		SetBandInfo(i, &rbbi);
	}
}

INLINE BOOL CXReBar::InsertBand(HWND hChildWnd, Int nIndex, Int nMinHeight, bool bBreak, UInt uMask)
{
	DEV_DEBUG(TF("  Rebar insert child %#08x, min height is %d"), hChildWnd, nMinHeight);
	REBARBANDINFO info;
	info.cbSize     = sizeof(REBARBANDINFO);
	info.fMask      = uMask;
	info.fStyle     = bBreak ? (RBBS_CHILDEDGE|RBBS_BREAK) : RBBS_CHILDEDGE;
	info.hwndChild  = hChildWnd;
	info.cxMinChild = 0; // must be 0
	info.cyMinChild = nMinHeight;
	info.lParam     = MAKELPARAM((ULong)nMinHeight, 1);
	info.wID        = (UInt)::GetWindowLongPtr(hChildWnd, GWLP_ID);
	return InsertBand(nIndex, &info);
}

INLINE void CXReBar::SetBandLayout(Int nBand)
{
	assert(nBand >= 0);
	REBARBANDINFO info;
	info.cbSize = sizeof(REBARBANDINFO);
	info.fMask = RBBIM_CHILD|RBBIM_CHILDSIZE|RBBIM_LPARAM;
	GetBandInfo(nBand, &info);

	info.fMask = RBBIM_CHILDSIZE; 
	if (info.cyMinChild == XREBARB_MIN) // restore band height
	{
		DEV_DEBUG(TF("  Rebar's %d band restore"), nBand);
		UInt uHeight     = (UInt)LOWORD(info.lParam);  // low 2 BYTE is menubar each row height
		info.cyMinChild  = (UInt)HIWORD(info.lParam);  // high 2 BYTE is menubar row count
		info.cyMinChild *= uHeight; 
		::EnableWindow(info.hwndChild, TRUE);
	}
	else // minimize band
	{
		DEV_DEBUG(TF("  Rebar's %d band minimized"), nBand);
		info.cyMinChild = XREBARB_MIN;  
		::EnableWindow(info.hwndChild, FALSE);
	}
	SetBandInfo(nBand, &info);
	assert(m_pOwner != nullptr);
}

INLINE LRESULT CXReBar::OnNotify(Int, LPNMHDR pHead, BOOL&)
{
	switch (pHead->code)
	{
	case RBN_HEIGHTCHANGE:
		{
			assert(m_pOwner != nullptr);
			return ::PostMessage(m_pOwner->m_hWnd, XUIM_LAYOUT, 0, 0L);
		}
		break;
	case RBN_CHILDSIZE: 
		{
			LPNMREBARCHILDSIZE lp = (LPNMREBARCHILDSIZE)pHead;

			REBARBANDINFO info;
			info.cbSize = sizeof(REBARBANDINFO);
			info.fMask = RBBIM_CHILD|RBBIM_CHILDSIZE|RBBIM_LPARAM;
			GetBandInfo(lp->uBand, &info);

			UInt uRet = (UInt)::SendMessage(info.hwndChild, XUIM_NOTIFY, XUI_NOTIFY_BREAK, 0L);
			if (uRet != 0)
			{
				info.fMask   = RBBIM_CHILDSIZE|RBBIM_LPARAM;
				UInt uHeight = (UInt)LOWORD(info.lParam); // low 2 BYTE is menubar each row height
				if (::IsWindowEnabled(info.hwndChild))
				{
					info.cyMinChild = uHeight * uRet;
				}
				info.lParam = MAKELPARAM((ULong)uHeight, (ULong)uRet);
				SetBandInfo(lp->uBand, &info);
			}
		}
		break;
	default :
		{
		}
	}
	return 0;
}

INLINE LRESULT CXReBar::OnXLayout(WPARAM wParam, LPARAM lParam, BOOL&) 
{
	assert(::IsWindowVisible(m_hWnd));
	assert(HIWORD(wParam) == XUI_LAYOUT_RECT);
	::SendMessage(m_hWnd, WM_SIZE, 0, 0L);
	RECT rt;
	GetWindowRect(&rt);
	((LPRECT)lParam)->top += (rt.bottom - rt.top);
	return HIWORD(wParam);
}

INLINE LRESULT CXReBar::OnLBDoubleClick(WPARAM, LPARAM lParam, BOOL&)
{
	// minimize the band
	RBHITTESTINFO info;
	info.pt.x = GET_X_LPARAM(lParam);
	info.pt.y = GET_Y_LPARAM(lParam);
	info.flags = RBHT_GRABBER;
	Int nBand = HitTest(&info);
	if (nBand > -1) // -1 : no rebar band was at the point
	{
		SetBandLayout(nBand);
	}
	return 0;
}

#endif // (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#endif // __TARGET_XUI_BASE_WND_INL__