///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : uuid.h                                                                                //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : universally unique identifier                                                              //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __UUID_H__
#define __UUID_H__

#pragma once

#include "stream.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CUUID
class CUUID : public MObject
{
public:
	enum UUID_FROMAT 
	{
		UUID_FROMAT_DIGITS_BRACES,    // {XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX} - 32 hex-digits + 4 Hyphens + 2 Braces
		UUID_FROMAT_DIGITS_HYPHENS,   //  XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX  - 32 hex-digits + 4 Hyphens
		UUID_FROMAT_DIGITS_AVERAGE,   //  XXXXXXXX-XXXXXXXX-XXXXXXXX-XXXXXXXX   - 32 hex-digits + 3 Hyphens
		UUID_FROMAT_DIGITS_ONLY,      //  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX      - 32 hex-digits
	};

	enum UUID_LEN
	{
		UUIDC_LEN_BRACES  = 38,
		UUIDC_LEN_HYPHENS = 36,
		UUIDC_LEN_AVERAGE = 35,
		UUIDC_LEN_ONLY    = 32,
		UUIDC_LEN_MAX     = 40,
	};
	typedef CTStringFix<CXChar, UUIDC_LEN_MAX>   CStringUUID;

	static CPCXStr FROMAT_DIGITS_BRACES;
	static CPCXStr FROMAT_DIGITS_HYPHENS;
	static CPCXStr FROMAT_DIGITS_AVERAGE;
	static CPCXStr FROMAT_DIGITS_ONLY;

	class CUUIDTraits
	{
	public:
		typedef const CUUID& INARGTYPE;
		typedef CUUID& OUTARGTYPE;
	public:
		static void CopyElements(CUUID* pDst, const CUUID* pSrc, size_t stElements)
		{
			for (size_t i = 0; i < stElements; ++i)
			{
				pDst[i] = pSrc[i];
			}
		}

		static void RelocateElements(CUUID* pDst, CUUID* pSrc, size_t stElements)
		{
			MM_SAFE::Memmov(pDst, stElements * sizeof(CUUID), pSrc, stElements * sizeof(CUUID));
		}

		static bool CompareElements(INARGTYPE t1, INARGTYPE t2)
		{
			return (t1 == t2);
		}

		static Int CompareElementsOrdered(INARGTYPE t1, INARGTYPE t2)
		{
			const UInt* puData1 = reinterpret_cast<const UInt*>(&t1);
			const UInt* puData2 = reinterpret_cast<const UInt*>(&t2);

			for (Int i = 3; i >= 0; --i)
			{
				if (puData1[i] > puData2[i] )
				{
					return (Int)(1);
				}
				else if (puData1[i] < puData2[i])
				{
					return (Int)(-1);
				}
			}
			return (Int)(0);
		}

		static size_t HashElements(INARGTYPE t)
		{
			const UInt* puData = reinterpret_cast<const UInt*>(&t);
			return (size_t)(puData[0] ^ puData[1] ^ puData[2] ^ puData[3]);
		}
	};
public:
	CUUID(void);
	~CUUID(void);
	CUUID(const UID& uuid);
	CUUID(PCXStr pszUUID);

	CUUID(const CUUID& aSrc);
	CUUID& operator=(const CUUID& aSrc);

	bool operator==(const CUUID& aSrc) const;
	bool operator!=(const CUUID& aSrc) const;

	void Serialize(CStream&);

	void GetUUID(UID& uuid) const;
	void SetUUID(const UID& uuid);

	// default : {XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX}
	void ToString(CString& str, bool bAppend = false, UUID_FROMAT eFormat = UUID_FROMAT_DIGITS_BRACES) const;
	template <size_t stLenT> void ToString(CTStringFix<CXChar, stLenT>& strFix, bool bAppend = false, UUID_FROMAT eFormat = UUID_FROMAT_DIGITS_BRACES) const;
	void ToString(PXStr pszBuf, size_t stLen, UUID_FROMAT eFormat = UUID_FROMAT_DIGITS_BRACES) const;

	// pszBuf pointer to {XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX}
	bool ToUUID(PCXStr pszBuf, UUID_FROMAT eFormat = UUID_FROMAT_DIGITS_BRACES);
private:
	UID   m_UUID;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
#define DECLARE_UUID( name, uuid ) static CPCXStr UUID_##name = (TF(#uuid));   \
                                   static CPCXStr COM_##name = (TF(#name));
#define UUID_OF( name )            (CUUID((UUID_##name)))
#define COM_OF( name )             (COM_##name)

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)
	#include "windows/targetuuid.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_LINUX)
	#include "linux/targetuuid.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_MAC)
	#include "mac/targetuuid.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_IOS)
	#include "ios/targetuuid.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_ANDROID)
	#include "android/targetuuid.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_HTML5)
	#include "html5/targetuuid.inl"
#else
	#error "__PLATFORM_TARGET__ No Implement"
#endif

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // __UUID_H__