///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : targettstring.inl                                                                     //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : windows string                                                                             //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TARGET_TSTRING_INL__
#define __TARGET_TSTRING_INL__

#pragma once

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTStringFix
template <typename T, size_t stFix>
INLINE size_t CTStringFix<T, stFix>::Load(UInt uId, LibRef lrRef)
{
	m_stLen = (size_t)::LoadString(lrRef, uId, m_szBuffer, stFix);
	return m_stLen;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTString
template <typename T>
INLINE size_t CTString<T>::Load(UInt uId, LibRef lrRef)
{
	if (m_stBufferLen > 0)
	{
		m_stLen = (size_t)::LoadString(lrRef, uId, m_pszBuffer, m_stBufferLen);
		return m_stLen;
	}
	return 0;
}

#endif // (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#endif // __TARGET_TSTRING_INL__