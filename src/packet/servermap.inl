//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Inline File : servermap.inl                                //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : 信息数据存储                                      //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __SERVER_MAP_INL__
#define __SERVER_MAP_INL__

#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// tagSERVER_INFO : 服务器信息结构定义
INLINE tagSERVER_INFO::tagSERVER_INFO(void)
{
	Reset();
}

INLINE tagSERVER_INFO::~tagSERVER_INFO(void)
{
	usStatus = STATUSU_NONE;
}

INLINE void tagSERVER_INFO::Info(CStream& Stream, Int nStatus)
{
	if (Stream.IsRead()) {
		Stream >> uAllCount >> uOnline >> usBusy >> usIncr >> usId >> usStatus;
	}
	else {
		Stream << uAllCount << uOnline << usBusy << usIncr << usId;
		if ((nStatus & STATUSU_OKAY) == 0) {
			Stream << usStatus;
		}
		else {
			Stream << (UShort)STATUSU_LINK; // 改为link
		}
	}
}

INLINE void tagSERVER_INFO::Copy(const tagSERVER_DATA& sd)
{
	uAllCount = sd.uAllCount;
	uOnline   = sd.uOnline;
	usBusy    = sd.usBusy;
	usIncr    = sd.usIncr;
	usId      = sd.usId;
	usStatus  = sd.usStatus;
	if (usStatus != STATUSU_LINK) {
		usStatus  = STATUSU_SYNC;
	}
}
// 统计同类型服务器的客户端总连接数&在线数在一台服务器连接的增加情况
INLINE void tagSERVER_INFO::Incr(const tagSERVER_DATA& sd)
{
	uAllCount += sd.uAllCount;
	uOnline   += sd.uOnline;
	++usBusy; // 表示同类型服务器数量
	usStatus = STATUSU_SYNC;
}
// 统计同类型服务器的客户端总连接数&在线数新数据sdIncr和旧数据sdDecr的差值
INLINE void tagSERVER_INFO::Diff(const tagSERVER_DATA& sdIncr, const tagSERVER_DATA& sdDecr)
{
	uAllCount += sdIncr.uAllCount;
	if (uAllCount > sdDecr.uAllCount) {
		uAllCount -= sdDecr.uAllCount;
	}
	else {
		uAllCount = 0;
	}
	uOnline   += sdIncr.uOnline;
	if (uOnline > sdDecr.uOnline) {
		uOnline -= sdDecr.uOnline;
	}
	else {
		uOnline = 0;
	}
	usStatus = STATUSU_SYNC;
}
// 统计同类型服务器的客户端总连接数&在线数在一台服务器断开的减少情况
INLINE void tagSERVER_INFO::Decr(const tagSERVER_DATA& sd)
{
	if (uAllCount > sd.uAllCount) {
		uAllCount -= sd.uAllCount;
	}
	else {
		uAllCount = 0;
	}
	if (uOnline > sd.uOnline) {
		uOnline -= sd.uOnline;
	}
	else {
		uOnline = 0;
	}
	if (usBusy > 0) {
		--usBusy; // 表示同类型服务器数量
	}
	usStatus = STATUSU_SYNC;
}
// 服务器客户端连接增加统计人数
INLINE void tagSERVER_INFO::Incr(void)
{
	++uAllCount;
	++uOnline;

	usBusy += usIncr;
	if (usBusy > DATAD_MAX_BUSY) {
		usBusy = DATAD_MAX_BUSY;
	}
	usStatus = STATUSU_SYNC;
}
// 服务器客户端断开减少统计人数
INLINE void tagSERVER_INFO::Decr(void)
{
	if (uOnline > 0) {
		--uOnline;
	}
	if (usBusy >= usIncr) {
		usBusy -= usIncr;
	}
	else if (usBusy > 0) {
		usBusy = 0;
	}
	usStatus = STATUSU_SYNC;
}

INLINE void tagSERVER_INFO::Zero(void)
{
	uOnline  = 0;
	usBusy   = 0;
	usStatus = STATUSU_NONE;
}

INLINE void tagSERVER_INFO::Reset(void)
{
	memset(this, 0, sizeof(tagSERVER_INFO));
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// tagSERVER_NOADDR : 不包含地址的服务器信息结构定义
INLINE tagSERVER_NOADDR::tagSERVER_NOADDR(void)
//: lRefCount(1)
{
}

INLINE tagSERVER_NOADDR::~tagSERVER_NOADDR(void)
{
}

INLINE Int tagSERVER_NOADDR::AddrLen(void)
{
	return 0;
}

INLINE void tagSERVER_NOADDR::Addr(CStream&, Int)
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// tagSERVER_ADDR : 包含地址的服务器信息结构定义
template <size_t stLen>
INLINE tagSERVER_ADDR<stLen>::tagSERVER_ADDR(void)
//: lRefCount(1)
{
}

template <size_t stLen>
INLINE tagSERVER_ADDR<stLen>::~tagSERVER_ADDR(void)
{
}

template <size_t stLen>
INLINE Int tagSERVER_ADDR<stLen>::AddrLen(void)
{
	return (Int)stLen;
}

template <size_t stLen>
INLINE void tagSERVER_ADDR<stLen>::Addr(CStream& Stream, Int nStatus)
{
	if ((nStatus & STATUSU_PING) == 0) {
		if (Stream.IsRead()) {
			Stream.Read(NetAddr, sizeof(NetAddr));
		}
		else {
			Stream.Write(NetAddr, sizeof(NetAddr));
		}
	}
	else {// gameserver only STATUSU_PING
		if (Stream.IsRead()) {
			Stream.Read(NetAddr, sizeof(CNETTraits::NET_ADDR));
		}
		else {
			Stream.Write(NetAddr, sizeof(CNETTraits::NET_ADDR));
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// tagZONE_ADDR_INDEX
INLINE tagZONE_ADDR_INDEX::tagZONE_ADDR_INDEX(void)
{
}

INLINE tagZONE_ADDR_INDEX::~tagZONE_ADDR_INDEX(void)
{
	assert(Index.GetSize() == 0);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// tagSERVER_MAP : 服务器信息映射表结构定义
template <typename V>
INLINE tagSERVER_MAP<V>::tagSERVER_MAP(void)
{
}

template <typename V>
INLINE tagSERVER_MAP<V>::~tagSERVER_MAP(void)
{
}

template <typename V>
INLINE CSyncLock& tagSERVER_MAP<V>::GetLock(void)
{
	return m_SyncLock;
}

template <typename V>
INLINE void tagSERVER_MAP<V>::Update(void)
{
	PINDEX index = m_SvrMap.GetFirstIndex();
	while (index != nullptr) {
		SVR_MAP_PAIR* pPair = m_SvrMap.GetNext(index);
		if (pPair->m_V.usStatus & (STATUSU_SYNC | STATUSU_LINK)) {
			DEV_DUMP(TF("Update change okay %llx"), pPair->m_K);
			pPair->m_V.usStatus = STATUSU_OKAY;
		}
		else if (pPair->m_V.usStatus & STATUSU_UNLINK) {
			DEV_DUMP(TF("Update remove unlink %llx"), pPair->m_K);
			m_SvrMap.RemoveAt(reinterpret_cast<PINDEX>(pPair));
		}
	}
}

template <typename V>
INLINE bool tagSERVER_MAP<V>::Serialize(CStream& Stream, Int nStatus)
{
	if (Stream.IsRead()) {
		return Read(Stream, nStatus);
	}
	else if (m_SvrMap.GetSize() > 0) {
		return Write(Stream, nStatus);
	}
	return false;
}

template <typename V>
INLINE bool tagSERVER_MAP<V>::Read(CStream& Stream, Int nStatus)
{
	Int     nCount = 0;
	DataRef drKey;
	V       V;
	while (Stream.IsEnd() == false) {
		Stream >> drKey;
		PINDEX index = m_SvrMap.FindIndex(drKey);

		V.Info(Stream, nStatus);
		switch (V.usStatus) {
		case STATUSU_SYNC:
			{
				assert(index != nullptr);
				if (index != nullptr) {
					if (nStatus & STATUSU_UPDATE) {
						V.usStatus = STATUSU_OKAY;
					}
					m_SvrMap[index].Copy(V);
					++nCount;
					DEV_DUMP(TF("Read %X status sync %llx"), nStatus, drKey);
				}
				else {
					DEV_DUMP(TF("Read %X status sync %llx failed"), nStatus, drKey);
				}
			}
			break;
		case STATUSU_LINK:
			{
				assert(index == nullptr);
				if (index == nullptr) {
					if (nStatus & STATUSU_UPDATE) {
						V.usStatus = STATUSU_OKAY;
					}
					V.Addr(Stream, nStatus);
					m_SvrMap.Add(drKey, V);
					++nCount;
					DEV_DUMP(TF("Read %X status link %llx"), nStatus, drKey);
				}
				else {
					DEV_DUMP(TF("Read %X status link %llx failed"), nStatus, drKey);
				}
			}
			break;
		case STATUSU_UNLINK:
			{
				assert(index != nullptr);
				if (index != nullptr) {
					if (nStatus & STATUSU_UPDATE) {
						m_SvrMap.RemoveAt(index);
					}
					else {
						m_SvrMap[index].usStatus = STATUSU_UNLINK;
					}
					++nCount;
					DEV_DUMP(TF("Read %X status unlink %llx"), nStatus, drKey);
				}
				else {
					DEV_DUMP(TF("Read %X status unlink %llx failed"), nStatus, drKey);
				}
			}
			break;
		default:
			{
				DEV_DUMP(TF("Read %X status invalid %llx"), nStatus, drKey);
			}
		}
	}
	return (nCount > 0);
}

template <typename V>
INLINE bool tagSERVER_MAP<V>::Write(CStream& Stream, Int nStatus)
{
	Int nCount = 0;
	if ((nStatus & STATUSU_OKAY) == 0) {
		PINDEX index  = m_SvrMap.GetFirstIndex();
		while (index != nullptr) {
			SVR_MAP_PAIR* pPair = m_SvrMap.GetNext(index);
			if (pPair->m_V.usStatus & (UShort)nStatus) {
				Stream << pPair->m_K;
				switch (pPair->m_V.usStatus) {
				case STATUSU_SYNC:
				case STATUSU_UNLINK:
					{
						pPair->m_V.Info(Stream, nStatus);
						++nCount;
						DEV_DUMP(TF("Write %X status sync & unlink %llx"), nStatus, pPair->m_K);
					}
					break;
				case STATUSU_LINK:
					{
						pPair->m_V.Info(Stream, nStatus);
						pPair->m_V.Addr(Stream, nStatus);
						++nCount;
						DEV_DUMP(TF("Write %X status link %llx"), nStatus, pPair->m_K);
					}
					break;
				default:
					{
						DEV_DUMP(TF("Write %X status invalid %llx"), nStatus, pPair->m_K);
					}
				}
			}
		}
		DEV_DUMP(TF("Write %X status count %d"), nStatus, nCount);
	}
	else {// 发送同步+完成的服务器信息
		PINDEX index  = m_SvrMap.GetFirstIndex();
		while (index != nullptr) {
			SVR_MAP_PAIR* pPair = m_SvrMap.GetNext(index);
			if (pPair->m_V.usStatus & (UShort)nStatus) {
				Stream << pPair->m_K;
				pPair->m_V.Info(Stream, nStatus);
				pPair->m_V.Addr(Stream, nStatus);
				++nCount;
				DEV_DUMP(TF("Write %X status sync & okay %llx"), nStatus, pPair->m_K);
			}
		}
		DEV_DUMP(TF("Write %X status sync & okay count %d"), nStatus, nCount);
	}
	return (nCount > 0);
}

template <typename V>
INLINE Int tagSERVER_MAP<V>::Size(void)
{
	return m_SvrMap.GetSize();
}

template <typename V>
INLINE PINDEX tagSERVER_MAP<V>::Add(DataRef drKey)
{
	return m_SvrMap.Add(drKey);
}

template <typename V>
INLINE PINDEX tagSERVER_MAP<V>::Add(DataRef drKey, V& v)
{
	return m_SvrMap.Add(drKey, v);
}

template <typename V>
INLINE typename tagSERVER_MAP<V>::SVR_MAP_PAIR* tagSERVER_MAP<V>::Find(DataRef drKey)
{
	return m_SvrMap.Find(drKey);
}

template <typename V>
INLINE typename tagSERVER_MAP<V>::SVR_MAP_PAIR* tagSERVER_MAP<V>::At(PINDEX index)
{
	return m_SvrMap.GetAt(index);
}

template <typename V>
INLINE PINDEX tagSERVER_MAP<V>::First(void)
{
	return m_SvrMap.GetFirstIndex();
}

template <typename V>
INLINE typename tagSERVER_MAP<V>::SVR_MAP_PAIR* tagSERVER_MAP<V>::Next(PINDEX& index)
{
	return m_SvrMap.GetNext(index);
}

template <typename V>
INLINE bool tagSERVER_MAP<V>::Remove(DataRef drKey)
{
	return m_SvrMap.Remove(drKey);
}

template <typename V>
INLINE bool tagSERVER_MAP<V>::RemoveAt(PINDEX index)
{
	return m_SvrMap.RemoveAt(index);
}

template <typename V>
INLINE void tagSERVER_MAP<V>::RemoveAll(void)
{
	return m_SvrMap.RemoveAll();
}

template <typename V>
INLINE void tagSERVER_MAP<V>::tagSVR_PAIR::Serialize(CStream& Stream)
{
	Stream.IsRead() ? (Stream >> drKey) : (Stream << drKey);
	Value.Info(Stream);
	if (Value.usStatus == STATUSU_LINK) {
		Value.Addr(Stream);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// DATA_MAP : 数据信息映射表
template <typename K, typename V, class KTraits, class VTraits>
INLINE tagDATA_MAP<K, V, KTraits, VTraits>::tagDATA_MAP(void)
{
}

template <typename K, typename V, class KTraits, class VTraits>
INLINE tagDATA_MAP<K, V, KTraits, VTraits>::~tagDATA_MAP(void)
{
}

template <typename K, typename V, class KTraits, class VTraits>
INLINE CSyncLock& tagDATA_MAP<K, V, KTraits, VTraits>::GetLock(void)
{
	return m_SyncLock;
}

template <typename K, typename V, class KTraits, class VTraits>
INLINE bool tagDATA_MAP<K, V, KTraits, VTraits>::Serialize(CStream&)
{
	return false;
}

template <typename K, typename V, class KTraits, class VTraits>
INLINE Int tagDATA_MAP<K, V, KTraits, VTraits>::Size(void)
{
	return m_DataMap.GetSize();
}

template <typename K, typename V, class KTraits, class VTraits>
INLINE void tagDATA_MAP<K, V, KTraits, VTraits>::Cache(Int nCache)
{
	m_DataMap.SetGrow(nCache);
	m_DataMap.SetHash(nCache);
}

template <typename K, typename V, class KTraits, class VTraits>
INLINE PINDEX tagDATA_MAP<K, V, KTraits, VTraits>::Add(K key)
{
	return m_DataMap.Add((K)key);
}

template <typename K, typename V, class KTraits, class VTraits>
INLINE PINDEX tagDATA_MAP<K, V, KTraits, VTraits>::Add(K key, V& val)
{
	return m_DataMap.Add((K)key, val);
}

template <typename K, typename V, class KTraits, class VTraits>
INLINE typename tagDATA_MAP<K, V, KTraits, VTraits>::DATA_MAP_PAIR* tagDATA_MAP<K, V, KTraits, VTraits>::Find(K key)
{
	return m_DataMap.Find((K)key);
}

template <typename K, typename V, class KTraits, class VTraits>
INLINE PINDEX tagDATA_MAP<K, V, KTraits, VTraits>::First(void)
{
	return m_DataMap.GetFirstIndex();
}

template <typename K, typename V, class KTraits, class VTraits>
INLINE typename tagDATA_MAP<K, V, KTraits, VTraits>::DATA_MAP_PAIR* tagDATA_MAP<K, V, KTraits, VTraits>::Next(PINDEX& index)
{
	return m_DataMap.GetNext(index);
}

template <typename K, typename V, class KTraits, class VTraits>
INLINE typename tagDATA_MAP<K, V, KTraits, VTraits>::DATA_MAP_PAIR* tagDATA_MAP<K, V, KTraits, VTraits>::Get(PINDEX index)
{
	return m_DataMap.GetAt(index);
}

template <typename K, typename V, class KTraits, class VTraits>
INLINE bool tagDATA_MAP<K, V, KTraits, VTraits>::Remove(K key)
{
	return m_DataMap.Remove(key);
}

template <typename K, typename V, class KTraits, class VTraits>
INLINE bool tagDATA_MAP<K, V, KTraits, VTraits>::RemoveAt(PINDEX index)
{
	return m_DataMap.RemoveAt(index);
}

template <typename K, typename V, class KTraits, class VTraits>
INLINE void tagDATA_MAP<K, V, KTraits, VTraits>::RemoveAll(void)
{
	return m_DataMap.RemoveAll();
}

#endif // __SERVER_MAP_INL__