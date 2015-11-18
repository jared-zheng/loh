//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : serversort.h                                 //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : 服务器排序                                         //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __SERVER_SORT_H__
#define __SERVER_SORT_H__

#pragma once

#include "atomics.h"
#include "datadef.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// tagSERVER_SORT : 服务器排序信息结构定义
typedef struct tagSERVER_SORT
{
public:
	// 快速排序服务器负载情况
	static void QuickSort(tagSERVER_SORT* pSort, Int nCount);
	static void QuickSortRecursion(tagSERVER_SORT* pSort, Int nBegin, Int nEnd);
	static Int  Partition(tagSERVER_SORT* pSort, Int nBegin, Int nEnd);
public:
	tagSERVER_SORT(void);
	~tagSERVER_SORT(void);
public:
	UShort                 usBusy; // 服务器繁忙(万分比)
	UShort                 usIncr; // 每增加一个链接, 服务器繁忙增加数值
	DataRef                drServer;
	CNETTraits::NET_ADDR   Addr;
}SERVER_SORT, *PSERVER_SORT;
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// tagSORT_RESULT : 服务器按繁忙排序结果
typedef struct tagSORT_RESULT
{
public:
	enum SORT_CONST
	{
		SORTC_LIMIT = 256,    // 定时计算服务器的负载情况, 将最轻的前SORTC_LIMIT服务器保存, 轮询处理请求
	};
public:
	tagSORT_RESULT(void);
	~tagSORT_RESULT(void);

	void Reset(void);
	Long Index(void);
public:
	Long          lIndex;
	Long          lCount;
	SERVER_SORT   Sort[SORTC_LIMIT];
}SORT_RESULT, *PSORT_RESULT;
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// tagRESULT_CHAIN : 服务器按繁忙排序结果定时交换链
typedef struct tagRESULT_CHAIN
{
public:
	enum CHAIN_CONST
	{
		CHAINC_MAX = 2, // 两个交换缓存
	};
public:
	tagRESULT_CHAIN(void);
	~tagRESULT_CHAIN(void);

	void Reset(void);
	void Swap(void); // 

	PSORT_RESULT GetCur(void);
	PSORT_RESULT GetNext(void);
public:
	Long          lIndex;
	SORT_RESULT   Sort[CHAINC_MAX];
}RESULT_CHAIN, *PRESULT_CHAIN;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "serversort.inl"

#endif // __SERVER_SORT_H__