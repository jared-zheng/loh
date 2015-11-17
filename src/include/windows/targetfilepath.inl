///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : targetfilepath.inl                                                                    //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : windows file & folder path                                                                 //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TARGET_FILE_PATH_INL__
#define __TARGET_FILE_PATH_INL__

#pragma once

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CFilePath
SELECTANY CPCXStr CFilePath::ModulePath = TF("ModulePath");
SELECTANY CPCXStr CFilePath::WildCard   = TF("*.*");
SELECTANY CPCXStr CFilePath::LinkFile   = TF("%s%s");
SELECTANY CPCXStr CFilePath::LinkFolder = TF("%s%s\\");

INLINE CFilePath::CFilePath(bool bModulePath)
{
	if (bModulePath)
	{
		SetModulePath(ModulePath);
	}
}

INLINE CFilePath::~CFilePath(void)
{
	m_FilePath.RemoveAll();
}

INLINE CFilePath::CFilePath(const CFilePath&)
{
}

INLINE CFilePath& CFilePath::operator=(const CFilePath&)
{
	return (*this);
}

INLINE bool CFilePath::Copy(PCXStr pszDst, PCXStr pszSrc, bool bReplaceExisting, bool bEvenIfReadOnly, PCXStr pszDstKey, PCXStr pszSrcKey)
{
	XChar szDst[LMT_MAX_PATH] = { 0 };
	CXChar::Copy(szDst, (size_t)LMT_MAX_PATH, pszDst);
	if ((pszDstKey != nullptr) && (GetPath(pszDstKey, szDst) == false))
	{
		DEV_DEBUG(TF("  File-Path[%p] copy get dst file path %s[key %s] failed"), this, szDst, pszDstKey);
		return false;
	}
	if (CheckFilePath(szDst, bReplaceExisting, bEvenIfReadOnly))
	{
		XChar szSrc[LMT_MAX_PATH] ={0};
		CXChar::Copy(szSrc, (size_t)LMT_MAX_PATH, pszSrc);
		if ((pszSrcKey != nullptr) && (GetPath(pszSrcKey, szSrc) == false))
		{
			DEV_DEBUG(TF("  File-Path[%p] copy get src file path %s[key %s] failed"), this, szSrc, pszSrcKey);
			return false;
		}
		DEV_DEBUG(TF("  File-Path[%p] copy %s[key %s] to %s[key %s]"), this, szSrc, pszSrcKey, szDst, pszDstKey);
		return ::CopyFile(szSrc, szDst, FALSE) ? true : false;
	}
	return false;
}

INLINE bool CFilePath::Move(PCXStr pszDst, PCXStr pszSrc, bool bReplaceExisting, bool bEvenIfReadOnly, PCXStr pszDstKey, PCXStr pszSrcKey)
{
	XChar szDst[LMT_MAX_PATH] = { 0 };
	CXChar::Copy(szDst, (size_t)LMT_MAX_PATH, pszDst);
	if ((pszDstKey != nullptr) && (GetPath(pszDstKey, szDst) == false))
	{
		DEV_DEBUG(TF("  File-Path[%p] move get dst file path %s[key %s] failed"), this, szDst, pszDstKey);
		return false;
	}
	if (CheckFilePath(szDst, bReplaceExisting, bEvenIfReadOnly))
	{
		XChar szSrc[LMT_MAX_PATH] ={0};
		CXChar::Copy(szSrc, (size_t)LMT_MAX_PATH, pszSrc);
		if ((pszSrcKey != nullptr) && (GetPath(pszSrcKey, szSrc) == false))
		{
			DEV_DEBUG(TF("  File-Path[%p] move get src file path %s[key %s] failed"), this, szSrc, pszSrcKey);
			return false;
		}
		DEV_DEBUG(TF("  File-Path[%p] move %s[key %s] to %s[key %s]"), this, szSrc, pszSrcKey, szDst, pszDstKey);
		return ::MoveFile(szSrc, szDst) ? true : false;
	}
	return false;
}

INLINE bool CFilePath::Delete(PCXStr pszFileName, PCXStr pszKey)
{
	XChar szFile[LMT_MAX_PATH] = { 0 };
	CXChar::Copy(szFile, (size_t)LMT_MAX_PATH, pszFileName);
	if ((pszKey != nullptr) && (GetPath(pszKey, szFile) == false))
	{
		DEV_DEBUG(TF("  File-Path[%p] delete get file path %s[key %s] failed"), this, szFile, pszKey);
		return false;
	}
	DEV_DEBUG(TF("  File-Path[%p] delete %s[key %s]"), this, szFile, pszKey);
	::SetFileAttributes(szFile, FILE_ATTRIBUTE_NORMAL);
	return ::DeleteFile(szFile) ? true : false;
}

INLINE Int CFilePath::Find(ARY_STRING& strFiles, PCXStr pszFileName, PCXStr pszKey)
{
	XChar szFile[LMT_MAX_PATH] = { 0 };
	CXChar::Copy(szFile, (size_t)LMT_MAX_PATH, pszFileName);
	if ((pszKey != nullptr) && (GetPath(pszKey, szFile) == false))
	{
		DEV_DEBUG(TF("  File-Path[%p] find1 get file path %s[key %s] failed"), this, szFile, pszKey);
		return -1;
	}
	return FindFiles(strFiles, szFile);
}

INLINE Int CFilePath::Find(ARY_FIXSTRING& strFixFiles, PCXStr pszFileName, PCXStr pszKey)
{
	XChar szFile[LMT_MAX_PATH] = { 0 };
	CXChar::Copy(szFile, (size_t)LMT_MAX_PATH, pszFileName);
	if ((pszKey != nullptr) && (GetPath(pszKey, szFile) == false))
	{
		DEV_DEBUG(TF("  File-Path[%p] find2 get file path %s[key %s] failed"), this, szFile, pszKey);
		return -1;
	}
	return FindFiles(strFixFiles, szFile);
}

INLINE bool CFilePath::CreateFolder(PCXStr pszPath, PCXStr pszKey)
{
	XChar szFolder[LMT_MAX_PATH] = { 0 };
	CXChar::Copy(szFolder, (size_t)LMT_MAX_PATH, pszPath);
	if ((pszKey != nullptr) && (GetPath(pszKey, szFolder) == false))
	{
		DEV_DEBUG(TF("  File-Path[%p] create folder get folder path %s[key %s] failed"), this, szFolder, pszKey);
		return false;
	}
#ifdef RUNTIME_DEBUG
	size_t stLen = CXChar::Length(szFolder, (size_t)LMT_MAX_PATH);
	assert(stLen > 0);
	assert(szFolder[stLen - 1] == CXChar::BackSlashChar);
#endif  // RUNTIME_DEBUG
	return CreateFolders(szFolder);
}

INLINE void CFilePath::DeleteFolder(PCXStr pszPath, PCXStr pszKey)
{
	XChar szFolder[LMT_MAX_PATH] = { 0 };
	CXChar::Copy(szFolder, (size_t)LMT_MAX_PATH, pszPath);
	if ((pszKey != nullptr) && (GetPath(pszKey, szFolder) == false))
	{
		DEV_DEBUG(TF("  File-Path[%p] delete folder get folder path %s[key %s] failed"), this, szFolder, pszKey);
	}
	else
	{
#ifdef RUNTIME_DEBUG
		size_t stLen = CXChar::Length(szFolder, (size_t)LMT_MAX_PATH);
		assert(stLen > 0);
		assert(szFolder[stLen - 1] == CXChar::BackSlashChar);
#endif  // RUNTIME_DEBUG
		DeleteFolders(szFolder);
	}
}

INLINE bool CFilePath::SetPath(PCXStr pszKey, PCXStr pszPath)
{
	CSyncLockWaitScope scope(m_FPLock);
	if ((pszKey != nullptr) && (pszPath != nullptr))
	{
		size_t stKey = CHash::Hash(pszKey);
		MAP_FILEPATH_PAIR* pPair = m_FilePath.Find(stKey);
		if (pPair == nullptr)
		{
			PINDEX index = m_FilePath.Add(stKey);
			if (index != nullptr)
			{
				pPair = m_FilePath.GetAt(index);
				DEV_DEBUG(TF("  File-Path[%p] set path add [key %s]"), this, pszKey);
			}
		}
		else
		{
			DEV_DEBUG(TF("  File-Path[%p] set path %s[key %s] key already has path %s"), this, pszPath, pszKey, pPair->m_V.szPath);
		}
		if (pPair != nullptr)
		{
			pPair->m_V.stLen = CXChar::Length(pszPath, (size_t)LMT_MAX_PATH);
			assert(pPair->m_V.stLen > 0);
			assert(pPair->m_V.stLen < (size_t)(LMT_MAX_PATH - 1));
			CXChar::Copy(pPair->m_V.szPath, (size_t)LMT_MAX_PATH, pszPath);
			pPair->m_V.szPath[LMT_MAX_PATH - 1] = 0;
			if (pPair->m_V.szPath[pPair->m_V.stLen - 1] != CXChar::BackSlashChar)
			{
				assert(pPair->m_V.stLen < (size_t)(LMT_MAX_PATH - 2));
				pPair->m_V.szPath[pPair->m_V.stLen] = CXChar::BackSlashChar;
				++(pPair->m_V.stLen);
				pPair->m_V.szPath[pPair->m_V.stLen] = 0;
			}
#ifdef RUNTIME_DEBUG
			CXChar::Copy(pPair->m_V.szKey, (size_t)LMT_KEY, pszKey);
#endif  // RUNTIME_DEBUG
			DEV_DEBUG(TF("  File-Path[%p] set path %s[key %s] okay"), this, pPair->m_V.szPath, pszKey);
			return true;
		}
	}
	return false;
}

INLINE bool CFilePath::GetFullPath(PCXStr pszKey, CString& str)
{
	CSyncLockWaitScope scope(m_FPLock);
	if (pszKey != nullptr)
	{
		MAP_FILEPATH_PAIR* pPair = m_FilePath.Find(CHash::Hash(pszKey));
		if (pPair != nullptr)
		{
#ifdef RUNTIME_DEBUG
			assert(CXChar::Comparen(pszKey, pPair->m_V.szKey, (size_t)LMT_KEY) == 0);
#endif  // RUNTIME_DEBUG
			if (str.IsEmpty() == false)
			{
				CXChar::Copy((pPair->m_V.szPath + pPair->m_V.stLen), (size_t)(LMT_MAX_PATH - pPair->m_V.stLen - 1), *str);
			}
			str = pPair->m_V.szPath;
			pPair->m_V.szPath[pPair->m_V.stLen] = 0;
			assert(pPair->m_V.szPath[LMT_MAX_PATH - 1] == 0);
			return true;
		}
	}
	return false;
}

INLINE bool CFilePath::GetFullPath(PCXStr pszKey, CStringFix& strFix)
{
	CSyncLockWaitScope scope(m_FPLock);
	if (pszKey != nullptr)
	{
		MAP_FILEPATH_PAIR* pPair = m_FilePath.Find(CHash::Hash(pszKey));
		if (pPair != nullptr)
		{
#ifdef RUNTIME_DEBUG
			assert(CXChar::Comparen(pszKey, pPair->m_V.szKey, (size_t)LMT_KEY) == 0);
#endif  // RUNTIME_DEBUG
			if (strFix.IsEmpty() == false)
			{
				CXChar::Copy((pPair->m_V.szPath + pPair->m_V.stLen), (size_t)(LMT_MAX_PATH - pPair->m_V.stLen - 1), *strFix);
			}
			strFix = pPair->m_V.szPath;
			pPair->m_V.szPath[pPair->m_V.stLen] = 0;
			assert(pPair->m_V.szPath[LMT_MAX_PATH - 1] == 0);
			return true;
		}
	}
	return false;
}

INLINE bool CFilePath::GetFullPath(PCXStr pszKey, PXStr pszFile, size_t stLen)
{
	CSyncLockWaitScope scope(m_FPLock);
	if ((pszKey != nullptr) && (pszFile != nullptr))
	{
		MAP_FILEPATH_PAIR* pPair = m_FilePath.Find(CHash::Hash(pszKey));
		if (pPair != nullptr)
		{
#ifdef RUNTIME_DEBUG
			assert(CXChar::Comparen(pszKey, pPair->m_V.szKey, (size_t)LMT_KEY) == 0);
#endif  // RUNTIME_DEBUG
			if (*pszFile != 0)
			{
				CXChar::Copy((pPair->m_V.szPath + pPair->m_V.stLen), (size_t)(LMT_MAX_PATH - pPair->m_V.stLen - 1), pszFile);
			}
			CXChar::Copy(pszFile, stLen, pPair->m_V.szPath);
			pPair->m_V.szPath[pPair->m_V.stLen] = 0;
			assert(pPair->m_V.szPath[LMT_MAX_PATH - 1] == 0);
			return true;
		}
	}
	return false;
}

INLINE bool CFilePath::CheckFilePath(PCXStr pszPath, bool bReplaceExisting, bool bEvenIfReadOnly)
{
	ULong ulRet = ::GetFileAttributes(pszPath);
	if (ulRet != INVALID_FILE_ATTRIBUTES) // file exist
	{
		if (bReplaceExisting == false)
		{
			DEV_DEBUG(TF("  File-Path[%p] CheckFilePath %s file exist"), this, pszPath);
			return false;
		}
		if (ulRet & FILE_ATTRIBUTE_READONLY)
		{
			if (bEvenIfReadOnly == false)
			{
				DEV_DEBUG(TF("  File-Path[%p] CheckFilePath %s file is read-only"), this, pszPath);
				return false;
			}
			::SetFileAttributes(pszPath, FILE_ATTRIBUTE_NORMAL);
		}
	}
	return true;
}

INLINE Int CFilePath::FindFiles(ARY_STRING& strFiles, PCXStr pszFile)
{
	Int nCount = 0;

	WIN32_FIND_DATA FindData = { 0 };
	XChar szPath[LMT_MAX_PATH + LMT_MAX_PATH] = { 0 };
	PXStr pszMatch = (szPath + LMT_MAX_PATH);
	CXChar::Copy(szPath, (size_t)LMT_MAX_PATH, pszFile);
	PXStr p = (PXStr)CXChar::FindCharRev(szPath, CXChar::BackSlashChar);
	if (p != nullptr)
	{
		CXChar::Copy(pszMatch, (size_t)LMT_MAX_PATH, p);

		++p;
		assert(*p != 0);
		size_t stLen = (size_t)LMT_MAX_PATH - (p - szPath);

		CString strFile;
		strFile.SetBufferLength(LMT_MAX_PATH);
///////////////////////////////////////////////////////////////////////////////////////////////////////////
		KeyRef krFile = ::FindFirstFile(szPath, &FindData);
		if (krFile != KEYREF_INVALID)
		{
			do
			{
				if (FindData.cFileName[0] != CXChar::DotChar)
				{
					CXChar::Copy(p, stLen, FindData.cFileName);
					strFile = szPath;
					strFiles.Add(strFile);
					++nCount;

					if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						CXChar::Concat(p, stLen, pszMatch);
						nCount += Find(strFiles, szPath);
					}
				}
			}while (::FindNextFile(krFile, &FindData));
			::FindClose(krFile);
		}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
	return nCount;
}

INLINE Int CFilePath::FindFiles(ARY_FIXSTRING& strFixFiles, PCXStr pszFile)
{
	Int nCount = 0;

	WIN32_FIND_DATA FindData = { 0 };
	XChar szPath[LMT_MAX_PATH + LMT_MAX_PATH] = { 0 };
	PXStr pszMatch = (szPath + LMT_MAX_PATH);
	CXChar::Copy(szPath, (size_t)LMT_MAX_PATH, pszFile);
	PXStr p = (PXStr)CXChar::FindCharRev(szPath, CXChar::BackSlashChar);
	if (p != nullptr)
	{
		CXChar::Copy(pszMatch, (size_t)LMT_MAX_PATH, p);

		++p;
		assert(*p != 0);
		size_t stLen = (size_t)LMT_MAX_PATH - (p - szPath);

		CStringFix strFixFile;
///////////////////////////////////////////////////////////////////////////////////////////////////////////
		KeyRef krFile = ::FindFirstFile(szPath, &FindData);
		if (krFile != KEYREF_INVALID)
		{
			do
			{
				if (FindData.cFileName[0] != CXChar::DotChar)
				{
					CXChar::Copy(p, stLen, FindData.cFileName);
					strFixFile = szPath;
					strFixFiles.Add(strFixFile);
					++nCount;

					if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						CXChar::Concat(p, stLen, pszMatch);
						nCount += Find(strFixFiles, szPath);
					}
				}
			}while (::FindNextFile(krFile, &FindData));
			::FindClose(krFile);
		}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
	return nCount;
}

INLINE bool CFilePath::CreateFolders(PCXStr pszFolder)
{
	bool bRet = true;
	if (::CreateDirectory(pszFolder, nullptr) == FALSE)
	{
		ULong ulRet = ::GetLastError();
		if (ulRet == ERROR_ALREADY_EXISTS)
		{
			DEV_DEBUG(TF("  File-Path[%p] create folder %s exists already"), this, pszFolder);
			// return true since folder exists
		}
		else if (ulRet == ERROR_PATH_NOT_FOUND)
		{
///////////////////////////////////////////////////////////////////////////////////////////////////////////
			XChar szTemp[LMT_MAX_PATH] = { 0 };
			PCXStr p = pszFolder;
			do
			{
				p = CXChar::FindChar(p, CXChar::BackSlashChar);
				if (p != nullptr)
				{
					++p;
					size_t stLen = (p - pszFolder);
					CXChar::Copy(szTemp, (size_t)LMT_MAX_PATH, pszFolder, stLen);

					ulRet = ::GetFileAttributes(szTemp);
					if ((ulRet == INVALID_FILE_ATTRIBUTES) && (::CreateDirectory(szTemp, nullptr) == FALSE))
					{
						DEV_DEBUG(TF("  File-Path[%p] create folder %s failed return error code %d"), this, szTemp, ::GetLastError());
						bRet = false;
						break;
					}
				}
				else
				{
					DEV_DEBUG(TF("  File-Path[%p] create folder %s failed"), this, pszFolder);
					bRet = false;
					break;
				}
			} while ((p != nullptr) && (*p != 0));
///////////////////////////////////////////////////////////////////////////////////////////////////////////
		}
		else
		{
			DEV_DEBUG(TF("  File-Path[%p] create folder %s return error code %d"), this, pszFolder, ulRet);
			bRet = false;
		}
	}
	return bRet;
}

INLINE void CFilePath::DeleteFolders(PCXStr pszFolder)
{
	if (::RemoveDirectory(pszFolder) == FALSE)
	{
		ULong ulRet = ::GetLastError();
		if (ulRet == ERROR_DIR_NOT_EMPTY)
		{
			WIN32_FIND_DATA FindData = { 0 };
			XChar szTemp[LMT_MAX_PATH] = { 0 };
			CXChar::Format(szTemp, (size_t)LMT_MAX_PATH, LinkFile, pszFolder, WildCard);
			DEV_DEBUG(TF("  File-Path[%p] delete folder %s's subfolders"), this, pszFolder);
///////////////////////////////////////////////////////////////////////////////////////////////////////////
			KeyRef krFile = ::FindFirstFile(szTemp, &FindData);
			if (krFile != KEYREF_INVALID)
			{
				do
				{
					if (FindData.cFileName[0] != CXChar::DotChar)
					{
						if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
						{
							CXChar::Format(szTemp, (size_t)LMT_MAX_PATH, LinkFolder, pszFolder, FindData.cFileName);
							DeleteFolders(szTemp);
						}
						else
						{
							CXChar::Format(szTemp, (size_t)LMT_MAX_PATH, LinkFile, pszFolder, FindData.cFileName);
							::SetFileAttributes(szTemp, FILE_ATTRIBUTE_NORMAL);
							::DeleteFile(szTemp);
						}
					}
				}while (::FindNextFile(krFile, &FindData));
				::FindClose(krFile);
			}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
			::SetFileAttributes(pszFolder, FILE_ATTRIBUTE_NORMAL);
			::RemoveDirectory(pszFolder);
		}
		else
		{
			DEV_DEBUG(TF("  File-Path[%p] delete folder %s return error code %d"), this, pszFolder, ulRet);
		}
	}
}

INLINE bool CFilePath::GetPath(PCXStr pszKey, PXStr pszFile)
{
	CSyncLockWaitScope scope(m_FPLock);
	MAP_FILEPATH_PAIR* pPair = m_FilePath.Find(CHash::Hash(pszKey));
	if (pPair != nullptr)
	{
#ifdef RUNTIME_DEBUG
		assert(CXChar::Comparen(pszKey, pPair->m_V.szKey, (size_t)LMT_KEY) == 0);
#endif  // RUNTIME_DEBUG
		if (*pszFile != 0)
		{
			CXChar::Copy((pPair->m_V.szPath + pPair->m_V.stLen), (size_t)(LMT_MAX_PATH - pPair->m_V.stLen - 1), pszFile);
		}
		CXChar::Copy(pszFile, (size_t)LMT_MAX_PATH, pPair->m_V.szPath);
		pPair->m_V.szPath[pPair->m_V.stLen] = 0;
		assert(pPair->m_V.szPath[LMT_MAX_PATH - 1] == 0);
		return true;
	}
	return false;
}

INLINE void CFilePath::SetModulePath(PCXStr pszKey)
{
	size_t stKey = CHash::Hash(pszKey);
	MAP_FILEPATH_PAIR* pPair = m_FilePath.Find(stKey);
	if (pPair == nullptr)
	{
		PINDEX index = m_FilePath.Add(stKey);
		if (index != nullptr)
		{
			pPair = m_FilePath.GetAt(index);
			pPair->m_V.stLen = (size_t)::GetModuleFileName(nullptr, pPair->m_V.szPath, LMT_MAX_PATH);
			assert(pPair->m_V.stLen > 0);

			PXStr p = (PXStr)CXChar::FindCharRev(pPair->m_V.szPath, CXChar::BackSlashChar);
			assert(p > pPair->m_V.szPath);

			++p;
			*p = 0;

			pPair->m_V.stLen = p - pPair->m_V.szPath;
			assert(pPair->m_V.stLen > 0);
			assert(pPair->m_V.szPath[pPair->m_V.stLen - 1] == CXChar::BackSlashChar);

			pPair->m_V.szPath[LMT_MAX_PATH - 1] = 0;
#ifdef RUNTIME_DEBUG
			CXChar::Copy(pPair->m_V.szKey, (size_t)LMT_KEY, pszKey);
#endif  // RUNTIME_DEBUG
			DEV_DEBUG(TF("  File-Path[%p] set path %s[key %s] okay"), this, pPair->m_V.szPath, pszKey);
		} // index != nullptr
	}
}

#endif // (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#endif // __TARGET_FILE_PATH_INL__