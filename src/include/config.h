///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : config.h                                                                              //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : config                                                                                     //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CONFIG_H__
#define __CONFIG_H__

#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// defines description
// __INTRIN_TARGET__
#define INTRIN_TARGET_X86             0x00000001
#define INTRIN_TARGET_X64             0x00000002
#define INTRIN_TARGET_IA64            0x00000004
#define INTRIN_TARGET_ARM32           0x00000008
#define INTRIN_TARGET_ARM64           0x00000010
#define INTRIN_TARGET_RISC32          0x00000020 // mips, ppc...
#define INTRIN_TARGET_RISC64          0x00000040 // mips, ppc...
#define INTRIN_TARGET_BIGENDIAN       0x00000080
// __PLATFORM_TARGET__
#define PLATFORM_TARGET_WINDOWS       0x00000100
#define PLATFORM_TARGET_LINUX         0x00000200
#define PLATFORM_TARGET_MAC           0x00000400
#define PLATFORM_TARGET_IOS           0x00000800
#define PLATFORM_TARGET_ANDROID       0x00001000
#define PLATFORM_TARGET_HTML5         0x00002000
// __RUNTIME_CONFIG__
#define RUNTIME_CONFIG_DEBUG          0x01000000
#define RUNTIME_CONFIG_UNICODE        0x02000000
#define RUNTIME_CONFIG_STATIC         0x04000000
#define RUNTIME_CONFIG_RTTI_HASH64    0x08000000
#define RUNTIME_CONFIG_MASK           0x0F000000
//
#define RUNTIME_CONFIG_INIT           0x10000000
#define RUNTIME_CONFIG_EXIT           0x20000000

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#
//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // __CONFIG_H__