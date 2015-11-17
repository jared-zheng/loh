///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : rand.inl                                                                              //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : random                                                                                     //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __RAND_INL__
#define __RAND_INL__

#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CLCGRand
INLINE CLCGRand::CLCGRand(UInt uSeed)
: m_uSeed(uSeed)
{
	m_uSeed = (m_uSeed == 0) ? ((UInt)CPlatform::GetOSRunningTime()) : (m_uSeed);
}

INLINE CLCGRand::~CLCGRand(void)
{
}

INLINE CLCGRand::CLCGRand(const CLCGRand&)
{
}

INLINE CLCGRand& CLCGRand::operator=(const CLCGRand&)
{
	return (*this);
}

INLINE void CLCGRand::SetSeed(UInt uSeed)
{
	m_uSeed = uSeed;
}

INLINE Int CLCGRand::Rand(void) 
{
	m_uSeed = (m_uSeed * LCGS_RADIXA + LCGS_RADIXB) >> 16;
	return (Int)(m_uSeed & LCGS_MAX);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CMPGRand
INLINE CMPGRand::CMPGRand(UInt uSeed)
: m_uSeed(uSeed)
, m_uM(MPGS_INIT_M)
, m_uA(MPGS_INIT_A)
, m_uB(MPGS_INIT_B)
{
	m_uSeed = (m_uSeed == 0) ? (MPGS_INIT_R) : (m_uSeed);
	CPlatform::TIMEINFO ti = { 0 };
	CPlatform::GetTimeInfo(ti, false);
	PUInt puRadix = reinterpret_cast<PUInt>(&ti);

	UInt uTime = puRadix[3];
	uTime ^= puRadix[2];
	uTime ^= puRadix[1];
	uTime ^= puRadix[0];
	uTime += CPlatform::GetCurrentId();
	
	m_uSeed += uTime;
}

INLINE CMPGRand::~CMPGRand(void)
{
}

INLINE CMPGRand::CMPGRand(const CMPGRand&)
{
}

INLINE CMPGRand& CMPGRand::operator=(const CMPGRand&)
{
	return (*this);
}

INLINE void CMPGRand::SetSeed(UInt uSeed)
{
	m_uSeed = uSeed;
}

INLINE Int CMPGRand::Rand(void) 
{
	m_uM += MPGS_STEP_M;
	if (m_uM >= MPGS_MAX_M)
	{
		m_uM -= MPGS_ZOOM_M;
	}
	m_uA += MPGS_STEP_A;
	if (m_uA >= MPGS_MAX_A)
	{
		m_uA -= MPGS_ZOOM_A;
	}
	m_uB += MPGS_STEP_B;
	if (m_uB >= MPGS_MAX_B)
	{
		m_uB -= MPGS_ZOOM_B;
	}
	m_uSeed = (m_uSeed * m_uM) + m_uA + m_uB;
	return (Int)((m_uSeed >> 16) ^ (m_uSeed & MPGS_RAND_M));
}

#endif // __RAND_INL__