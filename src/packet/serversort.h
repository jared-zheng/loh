//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : serversort.h                                 //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : 服务器排序                                         //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __SERVER_SORT_H__
#define __SERVER_SORT_H__

#pragma once

#include "atomics.h"
#include "datadef.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 快速排序服务器负载情况
struct tagSERVER_SORT : public MObject
{
public:
	static void QuickSort(tagSERVER_SORT* pSort, Int nCount);
	static void QuickSortRecursion(tagSERVER_SORT* pSort, Int nBegin, Int nEnd);
	static Int  Partition(tagSERVER_SORT* pSort, Int nBegin, Int nEnd);
public:
	tagSERVER_SORT(void);
	~tagSERVER_SORT(void);
public:
	UShort                 usBusy;   ///< 服务器负载情况(万分比)
	UShort                 usIncr;   ///< 每增加一个链接, 服务器负载情况增加数值(万分比)
	DataRef                drServer; ///< 句柄
	CNETTraits::NET_ADDR   Addr;     ///< 地址信息
};
typedef struct tagSERVER_SORT   SERVER_SORT, *PSERVER_SORT;
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 服务器按负载情况排序结果数组
struct tagSORT_RESULT : public MObject
{
public:
	/// 定时计算服务器的负载情况, 将最小负载的前SORTC_LIMIT个服务器保存, 轮询处理请求
	enum SORT_CONST
	{
		SORTC_LIMIT = 256,
	};
public:
	tagSORT_RESULT(void);
	~tagSORT_RESULT(void);
	/// 重置数据
	void Reset(void);
	/// 获取排序结果最小负载的服务器索引号
	Long Index(void);
public:
	Long          lIndex; ///< 最后更新的索引号
	Long          lCount; ///< 排序结果总数
	SERVER_SORT   Sort[SORTC_LIMIT]; ///< 排序结果数组
};
typedef struct tagSORT_RESULT   SORT_RESULT, *PSORT_RESULT;
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 服务器按负载情况排序结果数组交换链
struct tagRESULT_CHAIN : public MObject
{
public:
	/// 两个交换数组
	enum CHAIN_CONST
	{
		CHAINC_MAX = 2, 
	};
public:
	tagRESULT_CHAIN(void);
	~tagRESULT_CHAIN(void);
	/// 重置数据
	void Reset(void);
	/// 切换数组
	void Swap(void);
	/// 获取当前使用的数组
	PSORT_RESULT GetCur(void);
	/// 获取后备数组
	PSORT_RESULT GetNext(void);
public:
	Long          lIndex; ///< 当前使用的数组索引号
	SORT_RESULT   Sort[CHAINC_MAX]; ///< 交换数组
};
typedef struct tagRESULT_CHAIN   RESULT_CHAIN, *PRESULT_CHAIN;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "serversort.inl"

#endif // __SERVER_SORT_H__