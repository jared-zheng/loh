///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : streamfile.inl                                                                        //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : file serialize                                                                             //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __STREAM_FILE_INL__
#define __STREAM_FILE_INL__

#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CFileStream
INLINE CFileStream::CFileStream(Int nMode)
: CStream(nMode)
{
}

INLINE CFileStream::~CFileStream(void)
{
}

INLINE CFileStream::CFileStream(const CFileStream&)
{
}

INLINE CFileStream& CFileStream::operator=(const CFileStream&)
{
	return (*this);
}

#endif // __STREAM_FILE_INL__