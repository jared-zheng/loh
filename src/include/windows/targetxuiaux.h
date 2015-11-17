///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : targetxuiaux.h                                     ¡¡¡¡                               //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2005-02-02     version 0.0.0.1                                                             //
//   Update : 2015-03-16     version 0.0.5.5                                                             //
//   Detail : xui windows aux                                                                            //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TARGET_XUI_AUX_H__
#define __TARGET_XUI_AUX_H__

#pragma once

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#include <windows.h>
#include <commctrl.h>
#include <objbase.h>
#include <richedit.h>
#include <richole.h>

#pragma comment(lib, "comctl32.lib")

#if (__INTRIN_TARGET__ == INTRIN_TARGET_X86)
	#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif (__INTRIN_TARGET__ == INTRIN_TARGET_X64)
	#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif (__INTRIN_TARGET__ == INTRIN_TARGET_IA64)
	#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
	#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
enum XUI_MSG
{
	XUIM_CONST  = 0x0108,
	XUIM_LAYOUT = (WM_USER + XUIM_CONST), // UI and controls layout
	XUIM_NOTIFY,                          // ...
};

// XUIM_LAYOUT msg 's WPARAM HIWORD value
enum XUI_LAYOUT_PARAM
{
	XUI_LAYOUT_NONOE = 0,
	XUI_LAYOUT_RECT,      // LPARAM = pointer to a RECT
};

// XUIM_NOTIFY msg 's WPARAM HIWORD value
enum XUI_NOTIFY_PARAM
{
	XUI_NOTIFY_NONOE = 0,
	// CXToolBar, CXMenuBar
	XUI_NOTIFY_TTSHOW,
	XUI_NOTIFY_TTHIDE,
	// CXMenuBar
	XUI_NOTIFY_POPOUT,
	XUI_NOTIFY_BREAK,
};

enum XUI_STYLES
{
	// common child style
	XUIS_CONTAINER             = (0),
	XUIS_CHILD                 = (WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN),
	XUIS_CHILD_TABSTOP         = (WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_TABSTOP),
	XUIS_CHILD_SIBLING         = (WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS),
	XUIS_CHILD_SIBLING_TABSTOP = (WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_TABSTOP),
	// FrameWnd
	XUIS_FRAME_NOSIZE          = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX),
	// InfoTip
	XUIS_INFOTIP               = (WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP),
	// ToolBar 
	XUIS_TOOLBAR               = (WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | TBSTYLE_TOOLTIPS), 
	XUIS_TOOLBAR_FLAT          = (WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | TBSTYLE_TOOLTIPS | TBSTYLE_FLAT),
	XUIS_TOOLBAR_PANE          = (WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | CCS_NODIVIDER | CCS_NORESIZE | CCS_NOPARENTALIGN | TBSTYLE_TOOLTIPS | TBSTYLE_FLAT | TBSTYLE_EX_HIDECLIPPEDBUTTONS),
	// MenuBar
	XUIS_MENUBAR_PANE          = (WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | CCS_NODIVIDER | CCS_NORESIZE | CCS_NOPARENTALIGN | TBSTYLE_FLAT | TBSTYLE_WRAPABLE),
	// StatusBar
	XUIS_STATUSBAR             = (WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | SBARS_SIZEGRIP),
	// ReBar
	XUIS_REBAR                 = (WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | CCS_NODIVIDER | RBS_VARHEIGHT | RBS_BANDBORDERS),
	XUIS_REBAR_NOBORDER        = (WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | CCS_NODIVIDER | RBS_VARHEIGHT | RBS_BANDBORDERS),
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CXToolBar
#ifndef RT_TOOLBAR
#define RT_TOOLBAR			   MAKEINTRESOURCE(241)
#endif // RT_TOOLBAR

// CXMenuBar
#define MENUBARCLASSNAME       TOOLBARCLASSNAME

#define GET_X_LPARAM(lP)       ((Int)(Short)LOWORD(lP))
#define GET_Y_LPARAM(lP)       ((Int)(Short)HIWORD(lP))

CPCXStr DefaultReady           = TF("\nReady");

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// msg mapping
#define DECLARE_MSG_MAP( nclass )                                                                         \
protected:                                                                                                \
    virtual LRESULT WindowProc(UInt uMsg, WPARAM wParam, LPARAM lParam) OVERRIDE;       
//---------------------------------------------------------------------------------------------------------
#define BEGIN_INLINE_MSG_MAP( nclass )                                                                    \
protected:                                                                                                \
    virtual LRESULT WindowProc(UInt uMsg, WPARAM wParam, LPARAM lParam) OVERRIDE                          \
    {                                                                                                     \
        LRESULT lRet = 0;                                                                                 \
        BOOL    bHandled = TRUE;                                                                          \
        UNREFERENCED_PARAMETER( wParam );                                                                 \
        UNREFERENCED_PARAMETER( lParam );                                                                 \
        UNREFERENCED_PARAMETER( lRet );                                                                   \
        UNREFERENCED_PARAMETER( bHandled );   
//---------------------------------------------------------------------------------------------------------
#define BEGIN_MSG_MAP( nclass )                                                                           \
    LRESULT nclass::WindowProc(UInt uMsg, WPARAM wParam, LPARAM lParam)                                   \
    {                                                                                                     \
        LRESULT lRet = 0;                                                                                 \
        BOOL    bHandled = TRUE;                                                                          \
        UNREFERENCED_PARAMETER( wParam );                                                                 \
        UNREFERENCED_PARAMETER( lParam );                                                                 \
        UNREFERENCED_PARAMETER( lRet );                                                                   \
        UNREFERENCED_PARAMETER( bHandled );                                            
//---------------------------------------------------------------------------------------------------------
#define END_MSG_MAP()                                                                                     \
        return lRet;                                                                                      \
    }
//---------------------------------------------------------------------------------------------------------
#define END_CHAIN_MSG_MAP( base )                                                                         \
        lRet = base::WindowProc(uMsg, wParam, lParam);                                                    \
        return lRet;                                                                                      \
    }
//=========================================================================================================
#define DECLARE_KIT_EVENT_MAP( nclass )                                                                   \
    virtual LRESULT KitEventProc(MSG& msg, BOOL&) OVERRIDE;       
//---------------------------------------------------------------------------------------------------------
#define BEGIN_INLINE_KIT_EVENT_MAP( nclass )                                                              \
    virtual LRESULT KitEventProc(MSG& msg, BOOL& bHandled) OVERRIDE                                       \
    {                                                                                                     \
        LRESULT lRet = 0;                                                                                 \
        UNREFERENCED_PARAMETER( lRet );                                                                   \
        UNREFERENCED_PARAMETER( msg );                                                                    \
        UNREFERENCED_PARAMETER( bHandled );                                            
//---------------------------------------------------------------------------------------------------------
#define BEGIN_KIT_EVENT_MAP( nclass )                                                                     \
    LRESULT nclass::KitEventProc(MSG& msg, BOOL& bHandled)                                                \
    {                                                                                                     \
        LRESULT lRet = 0;                                                                                 \
        UNREFERENCED_PARAMETER( lRet );                                                                   \
        UNREFERENCED_PARAMETER( msg );                                                                    \
        UNREFERENCED_PARAMETER( bHandled );                                            
//---------------------------------------------------------------------------------------------------------
#define END_KIT_EVENT_MAP()                                                                               \
        return lRet;                                                                                      \
    }
//---------------------------------------------------------------------------------------------------------
#define END_CHAIN_KIT_EVENT_MAP( base )                                                                   \
        lRet = base::KitEventProc(msg, bHandled);                                                         \
        return lRet;                                                                                      \
    }
//=========================================================================================================
#define DECLARE_KIT_NOTIFY_MAP( nclass )                                                                  \
    virtual LRESULT KitNotifyProc(MSG& msg, BOOL&) OVERRIDE;      
//---------------------------------------------------------------------------------------------------------
#define BEGIN_INLINE_KIT_NOTIFY_MAP( nclass )                                                             \
    virtual LRESULT KitNotifyProc(MSG& msg, BOOL& bHandled) OVERRIDE                                      \
    {                                                                                                     \
        LRESULT lRet = 0;                                                                                 \
        UNREFERENCED_PARAMETER( lRet );                                                                   \
        UNREFERENCED_PARAMETER( msg );                                                                    \
        UNREFERENCED_PARAMETER( bHandled );                                            
//---------------------------------------------------------------------------------------------------------
#define BEGIN_KIT_NOTIFY_MAP( nclass )                                                                    \
    LRESULT nclass::KitNotifyProc(MSG& msg, BOOL& bHandled)                                               \
    {                                                                                                     \
        LRESULT lRet = 0;                                                                                 \
        UNREFERENCED_PARAMETER( lRet );                                                                   \
        UNREFERENCED_PARAMETER( msg );                                                                    \
        UNREFERENCED_PARAMETER( bHandled );                                            
//---------------------------------------------------------------------------------------------------------
#define END_KIT_NOTIFY_MAP()                                                                              \
        return lRet;                                                                                      \
    }
//---------------------------------------------------------------------------------------------------------
#define END_CHAIN_KIT_NOTIFY_MAP( base )                                                                  \
        lRet = base::KitNotifyProc(msg, bHandled);                                                        \
        return lRet;                                                                                      \
    }
//=========================================================================================================
#define MSG_HANDLER(msg, func)                                                                            \
    if (uMsg == msg)                                                                                      \
    {                                                                                                     \
        bHandled = TRUE;                                                                                  \
        lRet = func(wParam, lParam, bHandled);                                                            \
        if (bHandled) return lRet;                                                                        \
    }     
//---------------------------------------------------------------------------------------------------------
#define MSG_SWITCH_HANDLER(func)                                                                          \
    {                                                                                                     \
        bHandled = TRUE;                                                                                  \
        lRet = func(uMsg, wParam, lParam, bHandled);                                                      \
        if (bHandled) return lRet;                                                                        \
    }
//---------------------------------------------------------------------------------------------------------
#define MSG_RANGE_HANDLER(msgFirst, msgLast, func)                                                        \
    if ((uMsg >= msgFirst) && (uMsg <= msgLast))                                                          \
    {                                                                                                     \
        bHandled = TRUE;                                                                                  \
        lRet = func(uMsg, wParam, lParam, bHandled);                                                      \
        if (bHandled) return lRet;                                                                        \
    }                                                                     
//---------------------------------------------------------------------------------------------------------
#define COMMAND_HANDLER(id, code, func)                                                                   \
    if ((uMsg == WM_COMMAND)   &&                                                                         \
        (id == LOWORD(wParam)) &&                                                                         \
        (code == HIWORD(wParam)))                                                                         \
    {                                                                                                     \
        bHandled = TRUE;                                                                                  \
        lRet = func(code, id, (HWND)lParam, bHandled);                                                    \
        if (bHandled) return lRet;                                                                        \
    }
//---------------------------------------------------------------------------------------------------------
#define COMMAND_ID_HANDLER(id, func)                                                                      \
    if ((uMsg == WM_COMMAND) && (id == LOWORD(wParam)))                                                   \
    {                                                                                                     \
        bHandled = TRUE;                                                                                  \
        lRet = func(HIWORD(wParam), id, (HWND)lParam, bHandled);                                          \
        if (bHandled) return lRet;                                                                        \
    }
//---------------------------------------------------------------------------------------------------------
#define COMMAND_CODE_HANDLER(code, func)                                                                  \
    if ((uMsg == WM_COMMAND) && (code == HIWORD(wParam)))                                                 \
    {                                                                                                     \
        bHandled = TRUE;                                                                                  \
        lRet = func(code, LOWORD(wParam), (HWND)lParam, bHandled);                                        \
        if (bHandled) return lRet;                                                                        \
    }
//---------------------------------------------------------------------------------------------------------
#define COMMAND_RANGE_HANDLER(idFirst, idLast, func)                                                      \
    if ((uMsg == WM_COMMAND)        &&                                                                    \
        (LOWORD(wParam) >= idFirst) &&                                                                    \
        (LOWORD(wParam) <= idLast))                                                                       \
    {                                                                                                     \
        bHandled = TRUE;                                                                                  \
        lRet = func(HIWORD(wParam), LOWORD(wParam), (HWND)lParam, bHandled);                              \
        if (bHandled) return lRet;                                                                        \
    }
//---------------------------------------------------------------------------------------------------------
#define COMMAND_RANGE_CODE_HANDLER(idFirst, idLast, code, func)                                           \
    if ((uMsg == WM_COMMAND)        &&                                                                    \
        (code == HIWORD(wParam))    &&                                                                    \
        (LOWORD(wParam) >= idFirst) &&                                                                    \
        (LOWORD(wParam) <= idLast))                                                                       \
    {                                                                                                     \
        bHandled = TRUE;                                                                                  \
        lRet = func(code, LOWORD(wParam), (HWND)lParam, bHandled);                                        \
        if (bHandled) return lRet;                                                                        \
    }
//---------------------------------------------------------------------------------------------------------
#define NOTIFY_HANDLER(id, cd, func)                                                                      \
    if ((uMsg == WM_NOTIFY)               &&                                                              \
        (id == ((LPNMHDR)lParam)->idFrom) &&                                                              \
        (cd == ((LPNMHDR)lParam)->code))                                                                  \
    {                                                                                                     \
        bHandled = TRUE;                                                                                  \
        lRet = func((Int)wParam, (LPNMHDR)lParam, bHandled);                                              \
        if (bHandled) return lRet;                                                                        \
    }
//---------------------------------------------------------------------------------------------------------
#define NOTIFY_ID_HANDLER(id, func)                                                                       \
    if ((uMsg == WM_NOTIFY) && (id == ((LPNMHDR)lParam)->idFrom))                                         \
    {                                                                                                     \
        bHandled = TRUE;                                                                                  \
        lRet = func((Int)wParam, (LPNMHDR)lParam, bHandled);                                              \
        if (bHandled) return lRet;                                                                        \
    }
//---------------------------------------------------------------------------------------------------------
#define NOTIFY_CODE_HANDLER(cd, func)                                                                     \
    if ((uMsg == WM_NOTIFY) && (cd == ((LPNMHDR)lParam)->code))                                           \
    {                                                                                                     \
        bHandled = TRUE;                                                                                  \
        lRet = func((Int)wParam, (LPNMHDR)lParam, bHandled);                                              \
        if (bHandled) return lRet;                                                                        \
    }
//---------------------------------------------------------------------------------------------------------
#define NOTIFY_RANGE_HANDLER(idFirst, idLast, func)                                                       \
    if ((uMsg == WM_NOTIFY)                    &&                                                         \
        (((LPNMHDR)lParam)->idFrom >= idFirst) &&                                                         \
        (((LPNMHDR)lParam)->idFrom <= idLast))                                                            \
    {                                                                                                     \
        bHandled = TRUE;                                                                                  \
        lRet = func((Int)wParam, (LPNMHDR)lParam, bHandled);                                              \
        if (bHandled) return lRet;                                                                        \
    }
//---------------------------------------------------------------------------------------------------------
#define NOTIFY_RANGE_CODE_HANDLER(idFirst, idLast, cd, func)                                              \
    if ((uMsg == WM_NOTIFY)                    &&                                                         \
        (cd == ((LPNMHDR)lParam)->code)        &&                                                         \
        (((LPNMHDR)lParam)->idFrom >= idFirst) &&                                                         \
        (((LPNMHDR)lParam)->idFrom <= idLast))                                                            \
    {                                                                                                     \
        bHandled = TRUE;                                                                                  \
        lRet = func((Int)wParam, (LPNMHDR)lParam, bHandled);                                              \
        if (bHandled) return lRet;                                                                        \
    }
//---------------------------------------------------------------------------------------------------------
#define MSG_WM_CREATE()                                                                                   \
    if (uMsg == WM_CREATE)                                                                                \
    {                                                                                                     \
        return (LRESULT)OnCreate(LPCREATESTRUCT(lParam));                                                 \
    }
//---------------------------------------------------------------------------------------------------------
#define MSG_WM_DESTROY()                                                                                  \
    if (uMsg == WM_DESTROY)                                                                               \
    {                                                                                                     \
        OnDestroy();                                                                                      \
        return (LRESULT)0L;                                                                               \
    }
//---------------------------------------------------------------------------------------------------------
#define MSG_WM_INITDIALOG()                                                                               \
    if (uMsg == WM_INITDIALOG)                                                                            \
    {                                                                                                     \
        return (LRESULT)OnInitDialog();                                                                   \
    }
//---------------------------------------------------------------------------------------------------------
#define MSG_WM_COMMAND()                                                                                  \
    if (uMsg == WM_COMMAND)                                                                               \
    {                                                                                                     \
        lRet = (LRESULT)OnCommand(HIWORD(wParam), LOWORD(wParam), (HWND)lParam, bHandled);                \                                                                \
        if (bHandled) return lRet;                                                                        \
    }
//---------------------------------------------------------------------------------------------------------
#define MSG_WM_NOTIFY()                                                                                   \
    if (uMsg == WM_NOTIFY)                                                                                \
    {                                                                                                     \
        lRet = (LRESULT)OnNotify(Int(wParam), (LPNMHDR)lParam, bHandled);                                 \
        if (bHandled) return lRet;                                                                        \
    }
//---------------------------------------------------------------------------------------------------------
#define MSG_XUIM_LAYOUT()                                                                                 \
    if (uMsg == XUIM_LAYOUT)                                                                              \
    {                                                                                                     \
        lRet = (LRESULT)OnXLayout(wParam, lParam, bHandled);                                              \
        if (bHandled) return lRet;                                                                        \
    }
//---------------------------------------------------------------------------------------------------------
#define MSG_XUIM_NOTIFY()                                                                                 \
    if (uMsg == XUIM_NOTIFY)                                                                              \
    {                                                                                                     \
        lRet = (LRESULT)OnXNotify(wParam, lParam, bHandled);                                              \
        if (bHandled) return lRet;                                                                        \
    }
//---------------------------------------------------------------------------------------------------------
#define MSG_WM_PAINT()                                                                                    \
    if (uMsg == WM_PAINT)                                                                                 \
    {                                                                                                     \
        OnPaint();                                                                                        \
        return 0L;                                                                                        \
    }
//---------------------------------------------------------------------------------------------------------
#define MSG_WM_FRAME_SIZE()                                                                               \
    if (uMsg == WM_SIZE)                                                                                  \
    {                                                                                                     \
        if (::IsWindowVisible(m_hWnd))                                                                    \
        {                                                                                                 \
            FrameLayout();                                                                                \
        }                                                                                                 \
        return 0L;                                                                                        \
    }
//---------------------------------------------------------------------------------------------------------
#define MSG_WM_FRAME_XLAYOUT()                                                                            \
    if (uMsg == XUIM_LAYOUT)                                                                              \
    {                                                                                                     \
        if (::IsWindowVisible(m_hWnd))                                                                    \
        {                                                                                                 \
            FrameLayout();                                                                                \
        }                                                                                                 \
        return 0L;                                                                                        \
    }
////---------------------------------------------------------------------------------------------------------
//// control should handle the message itself.
//#define MSG_WM_DRAW_MEASURE_ITEM()                                                                        \
//    if ((uMsg == WM_DRAWITEM) || (uMsg == WM_MEASUREITEM))                                                \
//    {                                                                                                     \
//        HWND hWndCtl = GetDlgItem((Int)wParam);                                                           \
//        if (hWndCtl != nullptr)                                                                           \
//        {                                                                                                 \
//            return ::SendMessage(hWndCtl, uMsg, wParam, lParam);                                          \
//        }                                                                                                 \
//        return 0L;                                                                                        \
//    }
//---------------------------------------------------------------------------------------------------------
#define MSG_WM_FRAME_NOTIFY()                                                                             \
    if (uMsg == WM_NOTIFY)                                                                                \
    {                                                                                                     \
        lRet = (LRESULT)OnNotify(Int(wParam), (LPNMHDR)lParam, bHandled);                                 \
        if (bHandled) return lRet;                                                                        \
    }
//---------------------------------------------------------------------------------------------------------
#define DECLARE_DLG_ID_WND_EXCHANGE( nclass )                                                             \
	virtual bool OnDDWExchange(Int, IXWnd*&) OVERRIDE;
//---------------------------------------------------------------------------------------------------------
#define BEGIN_INLINE_DLG_ID_WND_EXCHANGE( nclass )                                                        \
	virtual bool OnDDWExchange(Int nID, IXWnd*& pChild) OVERRIDE                                          \
    {                                                                                                     \
        UNREFERENCED_PARAMETER( nID );                                                                    \
        UNREFERENCED_PARAMETER( pChild );                                                                 \
        bool bFind = false;                                                                               \
        switch (nID)                                                                                      \
        {  
//---------------------------------------------------------------------------------------------------------
#define BEGIN_DLG_ID_WND_EXCHANGE( nclass )                                                               \
    bool nclass::OnDDWExchange(Int nID, IXWnd*& pChild)                                                   \
    {                                                                                                     \
        UNREFERENCED_PARAMETER( nID );                                                                    \
        UNREFERENCED_PARAMETER( pChild );                                                                 \
        bool bFind = false;                                                                               \
        switch (nID)                                                                                      \
        {                                                                     
//---------------------------------------------------------------------------------------------------------
#define DLG_ID_WND( id, xwnd )                                                                            \
        case (id):                                                                                        \
            {                                                                                             \
                pChild = &xwnd;                                                                           \
                bFind  = true;                                                                            \
            }                                                                                             \
            break;
//---------------------------------------------------------------------------------------------------------
#define END_DLG_ID_WND_EXCHANGE( base )                                                                   \
        default:                                                                                          \
            {                                                                                             \
                bFind = base::OnDDWExchange(nID, pChild);                                                 \
            }                                                                                             \
        }                                                                                                 \
        return bFind;                                                                                     \
    }
//---------------------------------------------------------------------------------------------------------

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#endif // __TARGET_XUI_AUX_H__