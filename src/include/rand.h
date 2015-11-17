///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : rand.h                                                                                //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : random                                                                                     //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __RAND_H__
#define __RAND_H__

#pragma once

#include "mobject.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CLCGRand : LCG(Linear Congruence Generator)
class CLCGRand : public MObject
{
public:
	enum LCG_SEED
	{
		LCGS_MAX    = 32767,  // 0x7FFF = RAND_MAX
		LCGS_RADIXA = 214013,
		LCGS_RADIXB = 2531011,
	};
public:
	CLCGRand(UInt uSeed = 0);
	~CLCGRand(void);

	void   SetSeed(UInt uSeed);
	Int	   Rand(void);
private:
	CLCGRand(const CLCGRand&);
	CLCGRand& operator=(const CLCGRand&);
private:
	UInt   m_uSeed;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CMPGRand : The Multiple Prime Random Number Generator
class CMPGRand : public MObject
{
public:
	enum MPG_SEED
	{
		MPGS_INIT_M = 971,   
		MPGS_INIT_A = 11113, 
		MPGS_INIT_B = 104322,
		MPGS_INIT_R = 4181,  

		MPGS_STEP_M = 7,     
		MPGS_MAX_M  = 9973,  
		MPGS_ZOOM_M = 9871,  

		MPGS_STEP_A = 1907,  
		MPGS_MAX_A  = 99991, 
		MPGS_ZOOM_A = 89989, 

		MPGS_STEP_B = 73939, 
		MPGS_MAX_B  = 224729,
		MPGS_ZOOM_B = 96233, 

		MPGS_RAND_M = 16383, 
	};
public:
	CMPGRand(UInt uSeed = 0);
	~CMPGRand(void);

	void   SetSeed(UInt uSeed);
	Int	   Rand(void);
private:
	CMPGRand(const CMPGRand&);
	CMPGRand& operator=(const CMPGRand&);
private:
	UInt   m_uSeed;
	UInt   m_uM;
	UInt   m_uA;
	UInt   m_uB;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "rand.inl"

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // __RAND_H__