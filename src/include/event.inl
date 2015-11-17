///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : event.inl                                                                             //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : asynchronous events                                                                        //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __EVENT_INL__
#define __EVENT_INL__

#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CEventBase
INLINE CEventBase::CEventBase(void)
{
}

INLINE CEventBase::~CEventBase(void)
{
}

INLINE CEventBase::CEventBase(const CEventBase&)
{
}

INLINE CEventBase& CEventBase::operator=(const CEventBase&)
{
	return (*this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CEventHandler
INLINE CEventHandler::CEventHandler(void)
{
}

INLINE CEventHandler::~CEventHandler(void)
{
}

INLINE CEventHandler::CEventHandler(const CEventHandler&)
{
}

INLINE CEventHandler& CEventHandler::operator=(const CEventHandler&)
{
	return (*this);
}

#endif // __EVENT_INL__