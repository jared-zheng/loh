//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : packet.h                                     //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : 信令定义                                           //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __PACKET_H__
#define __PACKET_H__

#pragma once

#include "datadef.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 信令包数据头
class CPAKHead : public CEventBase
{
public:
	/// 基于CPAKHead的数据包创建入口
	/// @param[in] CStream& 数据流
	/// @retval CPAKHead* 创建成功返回指向CPAKHead对象的指针, 失败返回nullptr
	static CPAKHead* Create(CStream&);
public:
	/// 构造
	/// @param[in] 信令号
	/// @param[in] 数据包类型
	CPAKHead(UInt uEvent = 0, UInt uType = PAK_TYPE_NONE);
	virtual ~CPAKHead(void);

	CPAKHead(const CPAKHead& aSrc);
	CPAKHead& operator=(const CPAKHead& aSrc);
	/// 计算数据包大小
	/// @remarks AdjustSize调用Length返回值填充m_uSize
	virtual size_t Length(void) OVERRIDE;
	/// 将数据包序列化
	/// @param[in] CStream& 数据流
	virtual void   Serialize(CStream&) OVERRIDE;
	/// 获取信令号
	UInt   GetEvent(void);
	/// 设置信令号
	void   SetEvent(UInt uEvent);
	/// 获取类型
	UInt   GetType(void);
	/// 设置类型
	void   SetType(UInt uType);
	/// 获取数据包大小
	UInt   GetSize(void);
	/// 校验数据包大小
	void   AdjustSize(void);
private:
	UInt   m_uEvent; ///< 信令号
	UInt   m_uType;  ///< 发送数据包类型
	UInt   m_uSize;  ///< 数据包大小, 包含信令包数据头
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 信令包模板
template <UInt uEvent, UInt uType>
class CPAKSimple : public CPAKHead
{
public:
	CPAKSimple(void);
	virtual ~CPAKSimple(void);

	CPAKSimple(const CPAKSimple& aSrc);
	CPAKSimple& operator=(const CPAKSimple& aSrc);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 简单应答包
class CPAKAck : public CPAKHead
{
public:
	CPAKAck(UInt uEvent = 0, UInt uType = PAK_TYPE_NONE);
	virtual ~CPAKAck(void);

	CPAKAck(const CPAKAck& aSrc);
	CPAKAck& operator=(const CPAKAck& aSrc);

	virtual size_t Length(void) OVERRIDE;
	virtual void   Serialize(CStream&) OVERRIDE;
	/// 获取ACK
	UInt   GetAck(void);
	/// 设置ACK
	void   SetAck(UInt uAck);
private:
	UInt   m_uAck; ///< 0-成功, 非零值表示具体错误编号
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 网络层解析数据派发定义
typedef CNetworkEventHandler<CNETDispatch<CPAKHead>> CPAKHandler;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "packet.inl"

#endif // __PACKET_H__