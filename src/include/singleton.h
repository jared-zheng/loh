///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : singleton.h                                                                           //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : singleton                                                                                  //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#pragma once

#include "sync.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTSingleton
template <typename T> 
class CTSingleton : public T
{
public:
	static T*   GetInstance(void);

	static T*   NewInstance(void);
	static void DelInstance(void);
private:
	CTSingleton(void);
	~CTSingleton(void);
	CTSingleton(const CTSingleton&);
	CTSingleton& operator=(const CTSingleton&);
private:
	static CTSingleton*   ms_pSingleton;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTSingletonSync
template <typename T> 
class CTSingletonSync : public T
{
public:
	static T*   GetInstance(void);

	static T*   NewInstance(CSyncLock& Lock);
	static void DelInstance(CSyncLock& Lock);
private:
	CTSingletonSync(void);
	~CTSingletonSync(void);
	CTSingletonSync(const CTSingletonSync&);
	CTSingletonSync& operator=(const CTSingletonSync&);
private:
	static CTSingletonSync*   ms_pSingleton;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTSingletonInst
template <typename T> 
class CTSingletonInst : public T
{
public:
	static T*   GetInstance(void);

	static T*   NewInstance(void);
	static void DelInstance(void);
public:
	CTSingletonInst(void);
	~CTSingletonInst(void);
	CTSingletonInst(const CTSingletonInst&);
	CTSingletonInst& operator=(const CTSingletonInst&);
private:
	static CTSingletonInst   ms_Singleton;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTSingletonUM : singleton create/destroy in system heap
template <typename T> 
class CTSingletonUM : public T
{
public:
	static T*   GetInstance(void);

	static T*   NewInstance(void);
	static void DelInstance(void);
private:
	CTSingletonUM(void);
	~CTSingletonUM(void);
	CTSingletonUM(const CTSingletonUM&);
	CTSingletonUM& operator=(const CTSingletonUM&);
private:
	static CTSingletonUM*   ms_pSingleton;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "singleton.inl"

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // __SINGLETON_H__