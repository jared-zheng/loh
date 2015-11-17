///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : targetexception.inl                                                                   //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : windows exception                                                                          //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TARGET_EXCEPTION_INL__
#define __TARGET_EXCEPTION_INL__

#pragma once

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CExceptionFilter
class CExceptionFilter : public MObject 
{
public:
	static CPCXStr DumpFileName;
public:
	static bool Init(Int nType = 0);
	static void Exit(void);

	// windows platform exception handle
	static Long WINAPI UnhandledExceptFilter(PEXCEPTION_POINTERS pException);
private:
	CExceptionFilter(void);
	~CExceptionFilter(void);
	CExceptionFilter(const CExceptionFilter&);
	CExceptionFilter& operator=(const CExceptionFilter&);
private:
	static Int                            ms_nType;
	static LPTOP_LEVEL_EXCEPTION_FILTER   ms_pPrevFilter;
};

SELECTANY CPCXStr                      CExceptionFilter::DumpFileName   = TF("%s.%04d-%02d-%02d(%02d-%02d-%02d).dmp");
SELECTANY Int                          CExceptionFilter::ms_nType       = 0;
SELECTANY LPTOP_LEVEL_EXCEPTION_FILTER CExceptionFilter::ms_pPrevFilter = nullptr;

INLINE CExceptionFilter::CExceptionFilter(void)
{
}

INLINE CExceptionFilter::~CExceptionFilter(void)
{
}

INLINE CExceptionFilter::CExceptionFilter(const CExceptionFilter&)
{
}

INLINE CExceptionFilter& CExceptionFilter::operator=(const CExceptionFilter&)
{
	return (*this);
}

INLINE bool CExceptionFilter::Init(Int nType)
{
	if (ms_pPrevFilter != nullptr)
	{
		return true;
	}

	ms_nType       = nType;
	ms_pPrevFilter = ::SetUnhandledExceptionFilter(&CExceptionFilter::UnhandledExceptFilter);
	return true;
}

INLINE void CExceptionFilter::Exit(void)
{
	if (ms_pPrevFilter != nullptr)
	{
		::SetUnhandledExceptionFilter(ms_pPrevFilter);
		ms_pPrevFilter = nullptr;
	}
}

INLINE Long WINAPI CExceptionFilter::UnhandledExceptFilter(PEXCEPTION_POINTERS pException)
{
	SYSTEMTIME sm;
	::GetLocalTime(&sm);

	XChar szName[LMT_KEY]      = { 0 };
	XChar szPath[LMT_MAX_PATH] = { 0 };
	::GetModuleFileName(nullptr, szPath, LMT_MAX_PATH);
	PCXStr p = CXChar::FindCharRev(szPath, CXChar::BackSlashChar);
	assert(p > szPath);
	++p;
	CXChar::Copy(szName, LMT_KEY, p);
	CXChar::Format(szPath, LMT_MAX_PATH, DumpFileName, szName, sm.wYear, sm.wMonth, sm.wDay, sm.wHour, sm.wMinute, sm.wSecond);
	DEV_DEBUG(TF("Dump file name is : %s"), szPath);
	KeyRef krDump = ::CreateFile(szPath, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr); 
	if (krDump != KEYREF_INVALID)
	{
		MINIDUMP_EXCEPTION_INFORMATION mei;
		mei.ThreadId          = ::GetCurrentThreadId();
		mei.ExceptionPointers = pException;
		mei.ClientPointers    = FALSE;

		::MiniDumpWriteDump(::GetCurrentProcess(), ::GetCurrentProcessId(), krDump, (MINIDUMP_TYPE)ms_nType, &mei, nullptr, nullptr);

		::CloseHandle(krDump);
	}

	DEV_ERROR(TF("Exception Record : -->"));
	MEMORY_BASIC_INFORMATION mbi = { 0 };
	if (::VirtualQuery(reinterpret_cast<void*>(pException->ExceptionRecord->ExceptionAddress), &mbi, sizeof(MEMORY_BASIC_INFORMATION)) > 0)
	{
		LibRef hLibRef = reinterpret_cast<LibRef>(mbi.AllocationBase);
		::GetModuleFileName(hLibRef, szPath, LMT_MAX_PATH);

		DEV_ERROR(TF("ExceptionAddress : %p(%s)"), pException->ExceptionRecord->ExceptionAddress, szPath);
	}
	DEV_ERROR(TF("ExceptionCode    : %#X"), pException->ExceptionRecord->ExceptionCode);
	DEV_ERROR(TF("ExceptionFlags   : %d"),  pException->ExceptionRecord->ExceptionFlags);
	DEV_ERROR(TF("NumberParameters : %d"),  pException->ExceptionRecord->NumberParameters);
	DEV_ERROR(TF("ExceptionStack   :"));
	
	CONTEXT Context;
	MM_SAFE::Memcpy(&Context, sizeof(CONTEXT), pException->ContextRecord, sizeof(CONTEXT));
	STACKFRAME StackFrame;
	MM_SAFE::Memset(&StackFrame, 0, sizeof(STACKFRAME));

#if (__INTRIN_TARGET__ == INTRIN_TARGET_X86)
	StackFrame.AddrPC.Offset     = Context.Eip;
	StackFrame.AddrPC.Mode       = AddrModeFlat;
	StackFrame.AddrStack.Offset  = Context.Esp;
	StackFrame.AddrStack.Mode    = AddrModeFlat;
	StackFrame.AddrFrame.Offset  = Context.Ebp;
	StackFrame.AddrFrame.Mode    = AddrModeFlat;

	ULong ulMachineType          = IMAGE_FILE_MACHINE_I386;
#elif (__INTRIN_TARGET__ == INTRIN_TARGET_X64)
	StackFrame.AddrPC.Offset     = Context.Rip;
	StackFrame.AddrPC.Mode       = AddrModeFlat;
	StackFrame.AddrStack.Offset  = Context.Rsp;
	StackFrame.AddrStack.Mode    = AddrModeFlat;
	StackFrame.AddrFrame.Offset  = Context.Rbp;
	StackFrame.AddrFrame.Mode    = AddrModeFlat;

	ULong ulMachineType          = IMAGE_FILE_MACHINE_AMD64;
#elif (__INTRIN_TARGET__ == INTRIN_TARGET_IA64)
	StackFrame.AddrPC.Offset     = Context.StIIP;
	StackFrame.AddrPC.Mode       = AddrModeFlat;
	StackFrame.AddrStack.Offset  = Context.IntSp;
	StackFrame.AddrStack.Mode    = AddrModeFlat;
	StackFrame.AddrBStore.Offset = Context.RsBSP;
	StackFrame.AddrBStore.Mode   = AddrModeFlat;

	ULong ulMachineType          = IMAGE_FILE_MACHINE_IA64;
#else
	#error "__PLATFORM_TARGET__ No Implement"
#endif

//#ifdef RUNTIME_UNICODE
//	ULLong ullRet = 0;
//	Byte bSymInfo[sizeof(SYMBOL_INFOW) + LMT_BUF] = { 0 };
//	PSYMBOL_INFOW pSymbol  = reinterpret_cast<PSYMBOL_INFOW>(bSymInfo);
//	pSymbol->SizeOfStruct = (sizeof(SYMBOL_INFOW) + LMT_BUF);
//	pSymbol->MaxNameLen   = (ULong)(LMT_BUF / sizeof(XChar));
//
//	HANDLE hProcess = ::GetCurrentProcess();
//	::SymInitializeW(hProcess, nullptr, TRUE);
//	for (; ;)
//	{
//		if (::StackWalk(ulMachineType, hProcess, ::GetCurrentThread(), &StackFrame, &Context, nullptr, SymFunctionTableAccess, SymGetModuleBase, nullptr) == FALSE)
//		{
//			break;
//		}
//		if (StackFrame.AddrFrame.Offset == 0)
//		{
//			break;
//		}
//		::SymFromAddrW(hProcess, StackFrame.AddrPC.Offset, &ullRet, pSymbol);
//
//		IMAGEHLP_LINE ImgLine = { 0 };
//		ImgLine.SizeOfStruct  = sizeof(IMAGEHLP_LINE);
//		::SymGetLineFromAddr(hProcess, StackFrame.AddrPC.Offset, (PULong)&ullRet, &ImgLine);
//
//		DEV_ERROR(TF(" %p : %s[%S, %d]"), reinterpret_cast<void*>(StackFrame.AddrPC.Offset), pSymbol->Name, ImgLine.FileName, ImgLine.LineNumber);
//	}
//	::SymCleanup(hProcess);
//#else   // RUNTIME_UNICODE
	ULong ulRet = 0;
	Byte bSymInfo[sizeof(SYMBOL_INFO) + LMT_BUF] = { 0 };
	PSYMBOL_INFO pSymbol  = reinterpret_cast<PSYMBOL_INFO>(bSymInfo);
	pSymbol->SizeOfStruct = (sizeof(SYMBOL_INFO) + LMT_BUF);
	pSymbol->MaxNameLen   = (ULong)(LMT_BUF / sizeof(XChar));

	HANDLE hProcess = ::GetCurrentProcess();
	::SymInitialize(hProcess, nullptr, TRUE);
	for (; ;)
	{
		if (::StackWalk(ulMachineType, hProcess, ::GetCurrentThread(), &StackFrame, &Context, nullptr, SymFunctionTableAccess, SymGetModuleBase, nullptr) == FALSE)
		{
			break;
		}
		if (StackFrame.AddrFrame.Offset == 0)
		{
			break;
		}
		::SymFromAddr(hProcess, StackFrame.AddrPC.Offset, nullptr, pSymbol);

		IMAGEHLP_LINE ImgLine = { 0 };
		ImgLine.SizeOfStruct  = sizeof(IMAGEHLP_LINE);
		::SymGetLineFromAddr(hProcess, StackFrame.AddrPC.Offset, &ulRet, &ImgLine);
#ifdef RUNTIME_UNICODE
		DEV_ERROR(TF(" %p : %S[%S, %d]"), reinterpret_cast<void*>(StackFrame.AddrPC.Offset), pSymbol->Name, ImgLine.FileName, ImgLine.LineNumber);
#else   // RUNTIME_UNICODE
		DEV_ERROR(TF(" %p : %s[%s, %d]"), reinterpret_cast<void*>(StackFrame.AddrPC.Offset), pSymbol->Name, ImgLine.FileName, ImgLine.LineNumber);
#endif  // RUNTIME_UNICODE
	}
	::SymCleanup(hProcess);
//#endif  // RUNTIME_UNICODE
	return EXCEPTION_CONTINUE_SEARCH;
}

#endif // (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#endif // __TARGET_EXCEPTION_INL__