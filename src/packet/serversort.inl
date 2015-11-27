//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Inline File : serversort.inl                               //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : 信息数据                                           //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __SERVER_SORT_INL__
#define __SERVER_SORT_INL__

#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// tagSERVER_SORT : 快速排序服务器负载情况
INLINE void tagSERVER_SORT::QuickSort(tagSERVER_SORT* pSort, Int nCount)
{
	QuickSortRecursion(pSort, 0, nCount);
}
// 排序的递归调用
INLINE void tagSERVER_SORT::QuickSortRecursion(tagSERVER_SORT* pSort, Int nBegin, Int nEnd)
{
    if (nBegin < (nEnd - 1)){// 如果区域不存在或只有一个数据则不递归排序
		// 这里因为分割的时候, 分割点处的数据就是排序中他的位置
		// 也就是说他的左边的数据都小于等于他, 他右边的数据都大于他
		// 所以他不在递归调用的数据中
		Int nPart = Partition(pSort, nBegin, nEnd);  // 找到分割点
		QuickSortRecursion(pSort, nBegin, nPart);    // 递归左边的排序
		QuickSortRecursion(pSort, nPart + 1, nEnd);  // 递归右边的排序
	}
}
// 化分区间, 找到最后元素的排序位置, 并返回分隔的点(即最后一数据排序的位置)
// 划分的区间是[nBegin, nEnd), pSort是保存数据的指针
INLINE Int tagSERVER_SORT::Partition(tagSERVER_SORT* pSort, Int nBegin, Int nEnd)
{
	Int nPart = nBegin - 1;    // 最后Comp保存在这里
    --nEnd;

	tagSERVER_SORT Temp;       // 交换用的临时数据
    tagSERVER_SORT Comp = pSort[nEnd]; // 比较的数据

    // 遍历数据比较, 找到Comp的位置, 这里注意, 比较结果是:
    // 如果i的左边是小于等于Comp的, i的右边是大于Comp的
    for (Int i = nBegin; i < nEnd; ++i){
        if (pSort[i].usBusy <= Comp.usBusy){ // 如果数据比要比较的小, 则在该数据的左边, 与 nPart + 1 交换
            ++nPart;                        // 小于Comp的数据多一个, 所以要加1, i的左边数据都比Comp小
            Temp         = pSort[nPart];    // 交换数据
			pSort[nPart] = pSort[i];
			pSort[i]     = Temp;
        }
    }
    // 最后不要忘了把Comp和 nPart + 1 交换, 因为这里就是Comp的位置
    ++nPart;
    pSort[nEnd]  = pSort[nPart];
    pSort[nPart] = Comp;

    return nPart;   // 返回nPart的位置, 就是分割的位置
}

INLINE tagSERVER_SORT::tagSERVER_SORT(void)
: usBusy(0)
, usIncr(0)
#if (__INTRIN_TARGET__ == INTRIN_TARGET_X86)
, drServer(0)
#elif ((__INTRIN_TARGET__ == INTRIN_TARGET_X64) || (__INTRIN_TARGET__ == INTRIN_TARGET_IA64))
, drServer(nullptr)
#endif
{
}

INLINE tagSERVER_SORT::~tagSERVER_SORT(void)
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// tagSORT_RESULT : 服务器按负载情况排序结果数组
INLINE tagSORT_RESULT::tagSORT_RESULT(void)
: lIndex(0)
, lCount(0)
{
}

INLINE tagSORT_RESULT::~tagSORT_RESULT(void)
{
}

INLINE void tagSORT_RESULT::Reset(void)
{ 
	lIndex = 0; 
	lCount = 0; 
}

INLINE Long tagSORT_RESULT::Index(void)
{
	if (lIndex > lCount){
		lIndex = 0;
	}
	Long lRet = lIndex;

	Sort[lRet].usBusy += Sort[lRet].usIncr;
	Long lNext = 0;
	if (lRet < (lCount - 1)) {
		lNext = lRet + 1;
	}
	if (Sort[lRet].usBusy > Sort[lNext].usBusy)
	{
		lIndex = lNext;
	}
	return lRet;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// tagRESULT_CHAIN : 服务器按负载情况排序结果数组交换链
INLINE tagRESULT_CHAIN::tagRESULT_CHAIN(void)
: lIndex(0)
{
}

INLINE tagRESULT_CHAIN::~tagRESULT_CHAIN(void)
{
}

INLINE void tagRESULT_CHAIN::Reset(void)
{
	lIndex = 0;
	Sort[0].Reset();
	Sort[1].Reset();
}

INLINE void tagRESULT_CHAIN::Swap(void)
{
	if (lIndex > 0){
		CAtomics::Decrement<Long>(&lIndex);
	}
	else{
		CAtomics::Increment<Long>(&lIndex);
	}
}

INLINE PSORT_RESULT tagRESULT_CHAIN::GetCur(void)
{
	return (Sort + lIndex);
}

INLINE PSORT_RESULT tagRESULT_CHAIN::GetNext(void)
{
	if (lIndex > 0){
		Sort[0].Reset();
		return (Sort + 0);
	}
	else{
		Sort[1].Reset();
		return (Sort + 1);
	}
}

#endif // __SERVER_SORT_INL__