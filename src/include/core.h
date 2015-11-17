///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : core.h                                     ¡¡¡¡                                       //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : core header                                                                                //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CORE_H__
#define __CORE_H__

#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(CORE_EXPORT)
	#define COREAPI            API_EXPORT
	#define CORECLASS          CLASS_EXPORT
#elif defined(RUNTIME_STATIC)
	#define COREAPI                                     
	#define CORECLASS                                
#else   // CORE_EXPORT
	#define COREAPI            API_IMPORT
	#define CORECLASS          CLASS_IMPORT
#endif  // CORE_EXPORT

#define CORE_FUNC_INIT         ("CoreInit")
#define CORE_FUNC_EXIT         ("CoreExit") // C-style api

// init thread, event & object manager
COREAPI bool                   CoreInit(void);
COREAPI void                   CoreExit(void);

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#
//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // __CORE_H__