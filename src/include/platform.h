///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : platform.h                                                                            //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : basic platform info                                                                        //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#pragma once

#include "core.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPlatform
class CORECLASS CPlatform
{
public:
	enum CPU_VENDOR
	{
		CPUV_UNKNOWN       = 0,
		CPUV_INTEL,
		CPUV_ITANIUM,
		CPUV_AMD,
		CPUV_ARM,
		CPUV_RISC, // mips, ppc...
	};

	enum CPU_TECH
	{
		CPUT_MAX           = 8,
	};

	enum OS_TYPE
	{
		OST_UNKNOWN        = 0,
		OST_WINDOWS,
		OST_LINUX,
		OST_MAC,
		OST_IOS,
		OST_ANDROID,
	};

	enum OS_PAGESIZE
	{
		OSP_DEFAULT        = 0x00001000, // 4K
	};

	typedef struct tagCPUINFO
	{
		UInt    uSpeed;
		UShort  usCores;
		UShort  usVendor;
		XChar   szBrand[LMT_KEY];
	}CPUINFO, *PCPUINFO;

	typedef struct tagCPUTECH
	{
		ULLong   ullTech[CPUT_MAX]; // 
	}CPUTECH, *PCPUTECH;

	typedef struct tagMEMINFO
	{
		UInt   uUsedPercent;  // 0 - 100       
		UInt   uPageSize;     // KB
		UInt   uTotalPhys;    // MB
		UInt   uAvailPhys;    // MB
		UInt   uTotalPages;   // MB
		UInt   uAvailPages;   // MB
		UInt   uTotalVirtual; // MB
		UInt   uAvailVirtual; // MB
	}MEMINFO, *PMEMINFO;

	typedef struct tagOSINFO
	{
		UInt    uOSType;
		UInt    uMajor;	   
		UInt    uMinor;
		UInt    uBuild;
		XChar   szDescription[LMT_MIN];
	}OSINFO, *POSINFO;

	typedef struct tagTIMEINFO
	{
		UShort   usYear;
		UShort   usMonth;
		UShort   usWeekDay;
		UShort   usDay;
		UShort   usHour;
		UShort   usMinute;
		UShort   usSecond;
		UShort   usMSecond;
	}TIMEINFO, *PTIMEINFO;
public:
	static UInt    GetRuntimeConfig(void); // defined in config.h
	static UInt    GetPageSize(void);
	static UInt    GetCurrentId(void);     // current process id

	static void    GetCPUInfo(CPUINFO& cpui);
	static void    GetCPUTech(CPUTECH& cput, Int nFeature, Int nParam = 0);
	static void    GetMemInfo(MEMINFO& mem);
	static void    GetOSInfo(OSINFO& os);
	static void    GetTimeInfo(TIMEINFO& time, bool bLocal = true);

	static LLong   GetRunningTime(void);   // MS
	static LLong   GetOSRunningTime(void); // MS
	static LLong   GetOSRunningTick(void);
	static LLong   GetOSTickFreq(void);
	static LLong   Tick2MilliSecond(LLong& llTick);

	static UShort  ByteSwap(UShort usValue);
	static UInt    ByteSwap(UInt   uValue);
	static ULong   ByteSwap(ULong  ulValue);
	static ULLong  ByteSwap(ULLong ullValue);
	// ---develop output only, max output string length is 1000---
	static void    DevelopPrint(UInt uLevel, PCXStr pszFormat, ...);
protected:
	CPlatform(void);
	~CPlatform(void);
	CPlatform(const CPlatform&);
	CPlatform& operator=(const CPlatform&);
};

#ifdef RUNTIME_DEBUG
	#define DEV_MEMMG(...)    CPlatform::DevelopPrint(LOGL_MEMMG, __VA_ARGS__)
	#define DEV_TRACE(...)    CPlatform::DevelopPrint(LOGL_TRACE, __VA_ARGS__)
	#define DEV_DEBUG(...)    CPlatform::DevelopPrint(LOGL_DEBUG, __VA_ARGS__)
#else   // RUNTIME_DEBUG
	#define DEV_MEMMG(...)
	#define DEV_TRACE(...)
	#define DEV_DEBUG(...)
#endif  // RUNTIME_DEBUG
#define DEV_INFO(...)         CPlatform::DevelopPrint(LOGL_INFO,  __VA_ARGS__)
#define DEV_DUMP(...)         CPlatform::DevelopPrint(LOGL_DUMP,  __VA_ARGS__)
#define DEV_WARN(...)         CPlatform::DevelopPrint(LOGL_WARN,  __VA_ARGS__)
#define DEV_ERROR(...)        CPlatform::DevelopPrint(LOGL_ERROR, __VA_ARGS__)

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // __PLATFORM_H__