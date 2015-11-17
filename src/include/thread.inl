///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : thread.inl                                                                            //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : threading                                                                                  //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __THREAD_INL__
#define __THREAD_INL__

#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CRunnable
INLINE CRunnable::CRunnable(void)
{
}

INLINE CRunnable::~CRunnable(void)
{
}

INLINE CRunnable::CRunnable(const CRunnable&)
{
}

INLINE CRunnable& CRunnable::operator=(const CRunnable&)
{
	return (*this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CQueueTask
INLINE CQueueTask::CQueueTask(void)
{
}

INLINE CQueueTask::~CQueueTask(void)
{
}

INLINE CQueueTask::CQueueTask(const CQueueTask&)
{
}

INLINE CQueueTask& CQueueTask::operator=(const CQueueTask&)
{
	return (*this);
}

#endif // __THREAD_INL__