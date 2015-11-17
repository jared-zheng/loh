///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : markup.inl                                                                            //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : markup language                                                                            //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __MARKUP_INL__
#define __MARKUP_INL__

#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXMLTraits
SELECTANY XChar   CXMLTraits::LessChar              = TF('<');
SELECTANY XChar   CXMLTraits::GreaterChar           = TF('>');
SELECTANY XChar   CXMLTraits::AndChar               = TF('&');
SELECTANY XChar   CXMLTraits::SingleQuoteChar       = TF('\'');
SELECTANY XChar   CXMLTraits::DoubleQuoteChar       = TF('\"');
SELECTANY XChar   CXMLTraits::ExclamChar            = TF('!');
SELECTANY XChar   CXMLTraits::QuestionChar          = TF('?');
SELECTANY XChar   CXMLTraits::UnderlineChar         = TF('_');
SELECTANY XChar   CXMLTraits::EqualChar             = TF('=');
SELECTANY XChar   CXMLTraits::PoundChar             = TF('#');
SELECTANY XChar   CXMLTraits::SemicolonChar         = TF(';');

SELECTANY CPCXStr CXMLTraits::CommentString         = TF("<!--");
SELECTANY CPCXStr CXMLTraits::CDataString           = TF("<![CDATA[");
SELECTANY CPCXStr CXMLTraits::DeclarationString     = TF("<?xml");
SELECTANY CPCXStr CXMLTraits::XMLString             = TF("xml");
SELECTANY CPCXStr CXMLTraits::DefaultDeclaration    = TF("version=\"1.0\"");

SELECTANY CPCXStr CXMLTraits::CommentEndString      = TF("-->");
SELECTANY CPCXStr CXMLTraits::CDataEndString        = TF("]]>");
SELECTANY CPCXStr CXMLTraits::DeclarationEndString  = TF("?>");

SELECTANY CPCXStr CXMLTraits::LessCharString        = TF("&lt;");
SELECTANY CPCXStr CXMLTraits::GreaterCharString     = TF("&gt;");
SELECTANY CPCXStr CXMLTraits::AndCharString         = TF("&amp;");
SELECTANY CPCXStr CXMLTraits::SingleQuoteCharString = TF("&apos;");
SELECTANY CPCXStr CXMLTraits::DoubleQuoteCharString = TF("&quot;");

INLINE bool CXMLTraits::IsSkipChar(XChar c)
{
	return ((c == CXChar::SpaceChar)  || 
			(c == CXChar::ReturnChar) || 
			(c == CXChar::LineChar)   || 
			(c == CXChar::TabChar));
}

INLINE bool CXMLTraits::SkipChars(PXStr& pszBuf)
{
	while (IsSkipChar(*pszBuf))
	{
		++pszBuf;
	}
	return (*pszBuf != 0);
}

INLINE bool CXMLTraits::ToName(PXStr& pszBuf, CString& strName)
{
	if (*pszBuf != 0)
	{
		if (CXChar::IsAlpha(*pszBuf) && (CXChar::CompareIgnoren(pszBuf, XMLString, XML_TAG_XML_LEN) != 0))
		{
			PXStr p = pszBuf;
			while ((*pszBuf != 0) && (CXChar::IsAlpha(*pszBuf) || CXChar::IsDigit(*pszBuf) || (*pszBuf == UnderlineChar)))
			{
				++pszBuf;
			}
			strName.Empty();
			if (pszBuf > p)
			{
				strName.FillBuffer(p, (pszBuf - p));
			}
			return (strName.IsEmpty() != true);
		}
	}
	return false;
}

INLINE bool CXMLTraits::CheckName(PCXStr pszName)
{
	if ((pszName != nullptr) && (*pszName != 0))
	{
		if (CXChar::IsAlpha(*pszName) && (CXChar::CompareIgnoren(pszName, XMLString, XML_TAG_XML_LEN) != 0))
		{
			while ((*pszName != 0) && (CXChar::IsAlpha(*pszName) || CXChar::IsDigit(*pszName) || (*pszName == UnderlineChar)))
			{
				++pszName;
			}
			return (*pszName == 0);
		}
	}
	return false;
}

INLINE bool CXMLTraits::ToEntity(const CString& strValue, CString& strEntity) const
{
	PCXStr pszBuf = *strValue;
	for (size_t i = 0; i < strValue.Length(); ++i)
	{
		if (pszBuf[i] == LessChar)
		{
			strEntity += LessCharString;
		}
		else if (pszBuf[i] == GreaterChar)
		{
			strEntity += GreaterCharString;
		}
		else if (pszBuf[i] == AndChar)
		{
			strEntity += AndCharString;
		}
		else if (pszBuf[i] == SingleQuoteChar)
		{
			strEntity += SingleQuoteCharString;
		}
		else if (pszBuf[i] == DoubleQuoteChar)
		{
			strEntity += DoubleQuoteCharString;
		}
		else
		{
			strEntity += pszBuf[i];
		}
	}
	return true;
}

INLINE bool CXMLTraits::ToChars(CString& strValue)
{
	CString strTemp;

	PCXStr pszBuf = *strValue;
	PCXStr p = CXChar::FindChar(pszBuf, AndChar);
	if (p == nullptr)
	{
		return true;
	}

	do
	{
		if (p > pszBuf)
		{
			strTemp.AppendBuffer(pszBuf, (p - pszBuf));
		}
		if (CXChar::Comparen(p, LessCharString, XML_ENTITY_LESS_LEN) == 0)
		{
			strTemp.AppendBuffer(LessChar);
			pszBuf = p + XML_ENTITY_LESS_LEN;
		}
		else if (CXChar::Comparen(p, GreaterCharString, XML_ENTITY_GREATER_LEN) == 0)
		{
			strTemp.AppendBuffer(GreaterChar);
			pszBuf = p + XML_ENTITY_GREATER_LEN;
		}
		else if (CXChar::Comparen(p, AndCharString, XML_ENTITY_AND_LEN) == 0)
		{
			strTemp.AppendBuffer(AndChar);
			pszBuf = p + XML_ENTITY_AND_LEN;
		}
		else if (CXChar::Comparen(p, SingleQuoteCharString, XML_ENTITY_SINGLE_LEN) == 0)
		{
			strTemp.AppendBuffer(SingleQuoteChar);
			pszBuf = p + XML_ENTITY_SINGLE_LEN;
		}
		else if (CXChar::Comparen(p, DoubleQuoteCharString, XML_ENTITY_DOUBLE_LEN) == 0)
		{
			strTemp.AppendBuffer(DoubleQuoteChar);
			pszBuf = p + XML_ENTITY_DOUBLE_LEN;
		}
		else if (p[1] == PoundChar)
		{
			p += (XML_TAG_CHAR_LEN + XML_TAG_CHAR_LEN);
			Int nRadix = RADIXT_DEC;
			if ((p[0] == TF('x')) || (p[0] == TF('X')))
			{
				nRadix = RADIXT_HEX;
				++p;
			}
			WChar szWChar[XML_ENTITY_MIN_LEN] = { 0 };
			PXStr pszEnd = nullptr;
			szWChar[0] = (WChar)CXChar::ToULong(p, &pszEnd, nRadix);
			if (szWChar[0] <= XML_ENTITY_MIN_CHAR)
			{
				return false;
			}
			if ((pszEnd == nullptr) || (*pszEnd != SemicolonChar))
			{
				return false;
			}
			pszBuf = (pszEnd + XML_TAG_CHAR_LEN);
#ifdef RUNTIME_UNICODE
			strTemp.AppendBuffer(szWChar[0]);
#else  // RUNTIME_UNICODE
			XChar szAChar[XML_ENTITY_MIN_LEN] = { 0 };
			Int nRet = CXChar::Convert(szWChar, 1, szAChar, XML_ENTITY_MIN_LEN);
			assert(nRet > 0);
			strTemp.AppendBuffer(szAChar, nRet);
#endif // RUNTIME_UNICODE
		}
		p = CXChar::FindChar(pszBuf, AndChar);
	}while (p != nullptr);
	if (*pszBuf != 0)
	{
		strTemp.AppendBuffer(pszBuf);
	}
	strValue = strTemp;
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXMLNode
INLINE CXMLNode::CXMLNode(CXMLDocument* pDocument, XML_TYPE eType)
: m_eType(eType)
, m_pDocument(pDocument)
, m_Index(nullptr)
, m_pParent(nullptr)
, m_pFirst(nullptr)
, m_pLast(nullptr)
, m_pPrev(nullptr)
, m_pNext(nullptr)
{
	m_stHash = (size_t)this;
}

INLINE CXMLNode::~CXMLNode(void)
{
	assert(m_pParent == nullptr);
	assert(m_pFirst  == nullptr);
	assert(m_pLast   == nullptr);
	assert(m_pPrev   == nullptr);
	assert(m_pNext   == nullptr);
}

INLINE CXMLNode::CXMLNode(const CXMLNode&)
: m_eType(XML_TYPE_NODE)
, m_pDocument(nullptr)
, m_Index(nullptr)
, m_pParent(nullptr)
, m_pFirst(nullptr)
, m_pLast(nullptr)
, m_pPrev(nullptr)
, m_pNext(nullptr)
, m_stHash(0)
{
}

INLINE CXMLNode& CXMLNode::operator=(const CXMLNode&)
{
	return (*this);
}

INLINE CXMLTraits::XML_TYPE CXMLNode::GetType(void) const
{
	return m_eType;
}

template <typename T, CXMLTraits::XML_TYPE eType>
T* CXMLNode::To(void)
{
	if (GetType() == eType)
	{
		return static_cast<T*>(this);
	}
	return nullptr;
}

template <typename T, CXMLTraits::XML_TYPE eType>
const T* CXMLNode::To(void) const
{
	if (GetType() == eType)
	{
		return static_cast<const T*>(this);
	}
	return nullptr;
}

INLINE void CXMLNode::GetValue(CString& strValue)
{
	strValue = m_strValue;
}

INLINE void CXMLNode::SetValue(PCXStr pszValue)
{
	if ((pszValue != nullptr) && (*pszValue != 0))
	{
		if ((GetType() != XML_TYPE_ELEMENT) || CheckName(pszValue))
		{
			m_strValue = pszValue;
			m_stHash   = CHash::Hash(*m_strValue, m_strValue.Length());
		}
	}
}

INLINE bool CXMLNode::HasChild(void) const
{
	return (m_pFirst != nullptr);
}

INLINE bool CXMLNode::IsRootNode(void) const
{
	return ((m_pParent != nullptr) && (m_pParent == (CXMLNode*)m_pDocument));
}

INLINE bool CXMLNode::IsChildNode(void) const
{
	return ((m_pParent != nullptr) && (m_pParent != (CXMLNode*)m_pDocument));
}

INLINE bool CXMLNode::AddFirst(CXMLNode* pNode)
{
	assert(pNode != nullptr);
	assert(pNode->GetDocument() == m_pDocument);
	if ((pNode != this) && (pNode->GetDocument() == m_pDocument))
	{
		if (pNode->GetParent() != nullptr)
		{
			pNode->GetParent()->Remove(pNode);
		}
		if (m_pFirst != nullptr)
		{
			assert(m_pLast != nullptr);
			pNode->m_pNext = m_pFirst;
			m_pFirst->m_pPrev = pNode;
		}
		else
		{
			assert(m_pLast == nullptr);
			pNode->m_pNext = nullptr;
			m_pLast = pNode;
		}
		m_pFirst = pNode;
		pNode->m_pParent = this;
		pNode->m_pPrev   = nullptr;
		return true;
	}
	return false;
}

INLINE bool CXMLNode::AddLast(CXMLNode* pNode)
{
	assert(pNode != nullptr);
	assert(pNode->GetDocument() == m_pDocument);
	if ((pNode != this) && (pNode->GetDocument() == m_pDocument))
	{
		if (pNode->GetParent() != nullptr)
		{
			pNode->GetParent()->Remove(pNode);
		}
		if (m_pFirst != nullptr)
		{
			assert(m_pLast != nullptr);
			pNode->m_pPrev = m_pLast;
			m_pLast->m_pNext = pNode;
		}
		else
		{
			assert(m_pLast == nullptr);
			pNode->m_pPrev = nullptr;
			m_pFirst = pNode;
		}
		m_pLast = pNode;
		pNode->m_pParent = this;
		pNode->m_pNext   = nullptr;
		return true;
	}
	return false;
}

INLINE CXMLNode* CXMLNode::RemoveFirst(void)
{
	if (m_pFirst != nullptr)
	{	
		CXMLNode* pNode = m_pFirst;
		m_pFirst = pNode->m_pNext;
		if (m_pFirst != nullptr)
		{
			m_pFirst->m_pPrev = nullptr;
		}
		else
		{
			m_pLast = nullptr;
		}
		pNode->m_pParent = nullptr;
		return pNode;
	}
	return nullptr;
}

INLINE CXMLNode* CXMLNode::RemoveLast(void)
{
	if (m_pFirst != nullptr)
	{	
		CXMLNode* pNode = m_pLast;
		if (pNode->m_pPrev != nullptr)
		{
			m_pLast = pNode->m_pPrev;
			m_pLast->m_pNext = nullptr;
		}
		else
		{
			m_pFirst = nullptr;
			m_pLast  = nullptr;
		}
		pNode->m_pParent = nullptr;
		return pNode;
	}
	return nullptr;
}

INLINE bool CXMLNode::Remove(CXMLNode* pNode)
{
	assert(pNode != nullptr);
	assert(pNode->GetDocument() == m_pDocument);
	if ((pNode != nullptr) && (pNode->GetParent() == this))
	{	
		if (pNode == m_pFirst)
		{
			return (RemoveFirst() != nullptr);
		}
		else if (pNode == m_pLast)
		{
			return (RemoveLast() != nullptr);
		}
		else
		{
			assert(pNode->m_pPrev != nullptr);
			assert(pNode->m_pNext != nullptr);
			pNode->m_pPrev->m_pNext = pNode->m_pNext;
			pNode->m_pNext->m_pPrev = pNode->m_pPrev;
			pNode->m_pParent = nullptr;
		}
		return true;
	}
	return false;
}

INLINE void CXMLNode::RemoveAll(void)
{
	if (m_pFirst != nullptr)
	{	
		CXMLNode* pNext = nullptr;
		CXMLNode* pNode = m_pFirst;
		do
		{
			pNext = pNode->m_pNext;

			pNode->m_pPrev   = nullptr;
			pNode->m_pNext   = nullptr;
			pNode->m_pParent = nullptr;

			pNode = pNext;
		} while (pNode != nullptr);

		m_pFirst = nullptr;
		m_pLast  = nullptr;
	}
}

INLINE bool CXMLNode::InsertBefore(CXMLNode* pNode, CXMLNode* pBefore)
{
	assert(pNode != nullptr);
	assert(pNode->GetDocument() == m_pDocument);
	assert(pBefore != nullptr);
	assert(pBefore->GetDocument() == m_pDocument);

	if ((pNode != this) && (pNode->GetDocument() == m_pDocument) && (pNode != pBefore) && (pBefore->GetParent() == this))
	{
		if (pNode->GetParent() != nullptr)
		{
			pNode->GetParent()->Remove(pNode);
		}

		if (pBefore == m_pFirst)
		{
			return AddFirst(pNode);
		}
		else
		{
			assert(pBefore->m_pPrev != nullptr);
			pNode->m_pPrev = pBefore->m_pPrev;
			pNode->m_pNext = pBefore;
			pBefore->m_pPrev->m_pNext = pNode;
			pBefore->m_pPrev = pNode;

			pNode->m_pParent = nullptr;
		}
		return true;
	}
	return false;
}

INLINE bool CXMLNode::InsertAfter(CXMLNode* pNode, CXMLNode* pAfter)
{
	assert(pNode != nullptr);
	assert(pNode->GetDocument() == m_pDocument);
	assert(pAfter != nullptr);
	assert(pAfter->GetDocument() == m_pDocument);

	if ((pNode != this) && (pNode->GetDocument() == m_pDocument) && (pNode != pAfter) && (pAfter->GetParent() == this))
	{
		if (pNode->GetParent() != nullptr)
		{
			pNode->GetParent()->Remove(pNode);
		}

		if (pAfter == m_pLast)
		{
			return AddLast(pNode);
		}
		else
		{
			assert(pAfter->m_pNext != nullptr);
			pNode->m_pNext = pAfter->m_pNext;
			pNode->m_pPrev = pAfter;
			pAfter->m_pNext->m_pPrev = pNode;
			pAfter->m_pNext = pNode;

			pNode->m_pParent = nullptr;
		}
		return true;
	}
	return false;
}

INLINE CXMLNode* CXMLNode::Find(PCXStr pszValue, CXMLNode* pStartAfter) const
{
	if (pszValue != nullptr)
	{
		size_t stHash = CHash::Hash(pszValue);

		CXMLNode* pNode = m_pFirst;
		if ((pStartAfter != nullptr) && (pStartAfter->GetParent() == this))
		{
			pNode = pStartAfter->m_pNext;
		}
		while (pNode != nullptr)
		{
			if (stHash == pNode->m_stHash)
			{
				return pNode;
			}
			pNode = pNode->m_pNext;
		}
	}
	return nullptr;
}

INLINE CXMLDocument* CXMLNode::GetDocument(void)
{
	return m_pDocument;
}

INLINE const CXMLDocument* CXMLNode::GetDocument(void) const
{
	return m_pDocument;
}

INLINE CXMLNode* CXMLNode::GetParent(void)
{
	return m_pParent;
}

INLINE const CXMLNode* CXMLNode::GetParent(void) const
{
	return m_pParent;
}

INLINE CXMLNode* CXMLNode::GetFirst(void)
{
	return m_pFirst;
}

INLINE const CXMLNode* CXMLNode::GetFirst(void) const
{
	return m_pFirst;
}

INLINE CXMLNode* CXMLNode::GetLast(void)
{
	return m_pLast;
}

INLINE const CXMLNode* CXMLNode::GetLast(void) const
{
	return m_pLast;
}

INLINE CXMLNode* CXMLNode::GetPrev(void)
{
	return m_pPrev;
}

INLINE const CXMLNode* CXMLNode::GetPrev(void) const
{
	return m_pPrev;
}

INLINE CXMLNode* CXMLNode::GetNext(void)
{
	return m_pNext;
}

INLINE const CXMLNode* CXMLNode::GetNext(void) const
{
	return m_pNext;
}

INLINE bool CXMLNode::ParseNode(PXStr& pszBuf)
{
	if (SkipChars(pszBuf))
	{
		CXMLNode* pNode = nullptr;
		if (DetectNode(pszBuf, pNode) == false)
		{
			return false;
		}
		DEV_DEBUG(TF("CXMLNode::ParseNode create node type : %d"), pNode->GetType());
		if (pNode->ParseNode(pszBuf) == false)
		{
			m_pDocument->Destroy(pNode);
			return false;
		}
		return AddLast(pNode);
	}
	return false;
}

INLINE bool CXMLNode::DetectNode(PXStr& pszBuf, CXMLNode*& pNode)
{
	if (pszBuf[0] == LessChar)
	{
		pNode = nullptr;
		if (pszBuf[1] == ExclamChar) // dtd, cdata or comment
		{
			if (CXChar::Comparen(pszBuf, CommentString, XML_TAG_COMMENT_LEN) == 0)
			{
				CXMLComment* pComment = m_pDocument->CreateComment(nullptr);
				pNode = pComment;
				pszBuf += XML_TAG_COMMENT_LEN;
			}
			else if (CXChar::Comparen(pszBuf, CDataString, XML_TAG_TEXT_CDATA_LEN) == 0)
			{
				CXMLText* pText =  m_pDocument->CreateText(nullptr);
				pNode = pText;
				pText->SetCData(true);
				pszBuf += XML_TAG_TEXT_CDATA_LEN;
			}
			else
			{
				CXMLUnknown* pUnknown =  m_pDocument->CreateUnknown(nullptr);
				pNode = pUnknown;
				pszBuf += XML_TAG_UNKNOWN_DTD_LEN;
			}
		}
		else if (pszBuf[1] == QuestionChar)
		{
			if (CXChar::Comparen(pszBuf, DeclarationString, XML_TAG_DECLARATION_LEN) == 0)
			{
				CXMLDeclaration* pDeclaration =  m_pDocument->CreateDeclaration();
				pNode = pDeclaration;
				pszBuf += XML_TAG_DECLARATION_LEN;
			}
			else
			{
				CXMLUnknown* pUnknown =  m_pDocument->CreateUnknown(nullptr);
				pUnknown->SetDtd(false);
				pNode = pUnknown;
				pszBuf += XML_TAG_UNKNOWN_DTD_LEN;
			}
		}
		else // element
		{
			CXMLElement* pElement =  m_pDocument->CreateElement(nullptr);
			pNode = pElement;
			pszBuf += XML_TAG_CHAR_LEN;
		}
	}
	else // text
	{
		CXMLText* pText =  m_pDocument->CreateText(nullptr);
		pNode = pText;
	}
	return (pNode != nullptr);
}

INLINE void CXMLNode::Destroy(bool bSelfOnly)
{
	if (bSelfOnly == false)
	{
		CXMLNode* pNext = nullptr;
		CXMLNode* pNode = m_pFirst;
		do
		{
			pNext = pNode->m_pNext;
			m_pDocument->Destroy(pNode);
			pNode = pNext;
		} while (pNode != nullptr);
	}
	m_pParent = nullptr;
	m_pFirst  = nullptr;
	m_pLast   = nullptr;
	m_pPrev   = nullptr;
	m_pNext   = nullptr;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXMLDocument
INLINE CXMLDocument::CXMLDocument(void)
: CXMLNode(nullptr, XML_TYPE_DOCUMENT)
{
	m_pDocument = this;
}

INLINE CXMLDocument::~CXMLDocument(void)
{
	DestroyAll();
}

INLINE CXMLDocument::CXMLDocument(const CXMLDocument&)
: CXMLNode(nullptr, XML_TYPE_DOCUMENT)
{
}

INLINE CXMLDocument& CXMLDocument::operator=(const CXMLDocument&)
{
	return (*this);
}

INLINE bool CXMLDocument::Load(CStream& stream, size_t stSize)
{
	assert(stream.IsRead());
	size_t stLen = (size_t)(stream.Size() - stream.Tell());
	if ((stSize > 0) && (stLen > stSize))
	{
		stLen = stSize;
	}
	if (stLen > XML_ENCODING_LB_MAXLEN)
	{
		bool bRet = false;
		PByte pData = (PByte)ALLOC(stLen + LMT_BUF);
		if (pData != nullptr)
		{
			pData[stLen + 0] = 0;
			pData[stLen + 1] = 0;
			stream.Read(pData, stLen);
			// utf8
			if ((pData[0] == XML_ENCODING_LB_UTF80) &&
				(pData[1] == XML_ENCODING_LB_UTF81) &&
				(pData[2] == XML_ENCODING_LB_UTF82) )
			{
				PStr  pszA = (PStr)(pData + XML_ENCODING_LB_UTF8LEN);
				PWStr pszW = (PWStr)ALLOC( (stLen + LMT_BUF) * sizeof(WChar) );
				if (pszW != nullptr)
				{	// utf8 - unicode
					Int nRet = CXChar::Convert(pszA , -1, pszW, (Int)(stLen + LMT_BUF), TRAITS_PAGE_UTF8);
					assert(nRet > 0);
					pszW[nRet] = 0;
#ifdef RUNTIME_UNICODE
					// parse
					bRet = Parse((PXStr)pszW);
					FREE(pszW);
				}
#else  // RUNTIME_UNICODE
					// unicode - asni
					Int nRet = CXChar::Convert(pszW, nRet, pszA, (stLen + LMT_BUF));
					FREE(pszW);
					// parse
					assert(nRet > 0);
					pszA[nRet] = 0;
					bRet = Parse((PXStr)pszA);
				}
#endif // RUNTIME_UNICODE
			}
			else if ((pData[0] == XML_ENCODING_LB_UNICODE0) &&
					 (pData[1] == XML_ENCODING_LB_UNICODE1) )
			{
				PXStr pszBuf = (PXStr)(pData + XML_ENCODING_LB_UNICODELEN);
#ifdef RUNTIME_UNICODE
				bRet = Parse(pszBuf);
#else  // RUNTIME_UNICODE
				PStr pszA = (PStr)ALLOC( stLen + LMT_BUF );
				if (pszA != nullptr)
				{	// unicode - asni
					Int nRet = CXChar::Convert(pszBuf, -1, pszA, (stLen + LMT_BUF));
					assert(nRet > 0);
					pszA[nRet] = 0;
					// parse
					bRet = Parse(pszA);
					FREE(pszA);
				}
#endif // RUNTIME_UNICODE
			}
			else
			{
#ifdef RUNTIME_UNICODE
				PWStr pszW = (PWStr)ALLOC( (stLen + LMT_BUF) * sizeof(WChar) );
				if (pszW != nullptr)
				{	// asni - unicode
					Int nRet = CXChar::Convert((PCStr)pData, -1, pszW, (Int)(stLen + LMT_BUF));
					assert(nRet > 0);
					pszW[nRet] = 0;
					// parse
					bRet = Parse(pszW);
					FREE(pszW);
				}
#else  // RUNTIME_UNICODE
				bRet = Parse((PXStr)pData);
#endif // RUNTIME_UNICODE
			}
			FREE(pData);
		} // pBuf != nullptr
		return bRet;
	} // stLen > XML_ENCODING_LB_MAXLEN
	return false;
}

INLINE bool CXMLDocument::Save(CStream& stream, XML_ENCODING eENCODING)
{
	assert(stream.IsWrite());
	if (m_XMLNode.GetSize() > 0)
	{
		assert(m_pPrev == nullptr);
		assert(m_pNext == nullptr);

		CString strBuf;
		Store(strBuf);
		size_t stLen = strBuf.Length();

		switch (eENCODING)
		{
			case XML_ENCODING_UTF8:
			{
				stream << (UChar)XML_ENCODING_LB_UTF80;
				stream << (UChar)XML_ENCODING_LB_UTF81;
				stream << (UChar)XML_ENCODING_LB_UTF82;
#ifdef RUNTIME_UNICODE
				PStr pszBuf = (PStr)ALLOC( stLen + LMT_BUF );
				if (pszBuf != nullptr)
				{
					Int nRet = CXChar::Convert(*strBuf, (Int)stLen, pszBuf, (Int)(stLen + LMT_BUF), TRAITS_PAGE_UTF8);
					assert(nRet > 0);
					pszBuf[nRet] = 0;

					stream.Write(pszBuf, nRet);

					FREE(pszBuf);
					return true;
				}
#else  // RUNTIME_UNICODE
				PStr pszBuf = (PStr)ALLOC( (stLen + LMT_BUF) + (stLen + LMT_BUF) * sizeof(WChar) );
				if (pszBuf != nullptr)
				{
					PWStr pszW = (PWStr)(pszBuf + stLen + LMT_BUF);

					Int nRet = CXChar::Convert(*strBuf, stLen, pszW, (Int)(stLen + LMT_BUF));
					assert(nRet > 0);
					pszW[nRet] = 0;

					nRet = CXChar::Convert(pszW, nRet, pszBuf, (stLen + LMT_BUF), TRAITS_PAGE_UTF8);
					assert(nRet > 0);
					pszBuf[nRet] = 0;

					stream.Write(pszBuf, nRet);

					FREE(pszBuf);
					return true;
				}
#endif // RUNTIME_UNICODE
			}
			break;
			case XML_ENCODING_UNICODE:
			{
				stream << (UChar)XML_ENCODING_LB_UNICODE0;
				stream << (UChar)XML_ENCODING_LB_UNICODE1;
#ifdef RUNTIME_UNICODE
				stream.Write(*strBuf, stLen * sizeof(WChar));
				return true;
#else  // RUNTIME_UNICODE
				PWStr pszW = (PWStr)ALLOC( (stLen + LMT_BUF) * sizeof(WChar) );
				if (pszW != nullptr)
				{
					Int nRet = CXChar::Convert(*strBuf, stLen, pszW, (stLen + LMT_BUF));
					assert(nRet > 0);
					pszW[nRet] = 0;

					stream.Write(pszW, nRet * sizeof(WChar));

					FREE(pszW);
					return true;
				}
#endif // RUNTIME_UNICODE
			}
			break;
			case XML_ENCODING_ASNI:
			default:
			{
#ifdef RUNTIME_UNICODE
				PStr pszBuf = (PStr)ALLOC( stLen + LMT_BUF );
				if (pszBuf != nullptr)
				{
					Int nRet = CXChar::Convert(*strBuf, (Int)stLen, pszBuf, (Int)(stLen + LMT_BUF));
					assert(nRet > 0);
					pszBuf[nRet] = 0;

					stream.Write(pszBuf, nRet);

					FREE(pszBuf);
					return true;
				}
#else  // RUNTIME_UNICODE
				stream.Write(*strBuf, stLen);
				return true;
#endif // RUNTIME_UNICODE
			}
		}
	}
	return false;
}

INLINE CXMLElement* CXMLDocument::CreateElement(PCXStr pszName)
{
	CXMLElement* pNode = MNEW CXMLElement(this);
	if (pNode != nullptr)
	{
		pNode->SetValue(pszName);
		pNode->m_Index = m_XMLNode.AddTail(pNode);
	}
	return pNode;
}

INLINE CXMLComment* CXMLDocument::CreateComment(PCXStr pszComment)
{
	CXMLComment* pNode = MNEW CXMLComment(this);
	if (pNode != nullptr)
	{
		pNode->SetValue(pszComment);
		pNode->m_Index = m_XMLNode.AddTail(pNode);
	}
	return pNode;
}

INLINE CXMLText* CXMLDocument::CreateText(PCXStr pszText)
{
	CXMLText* pNode = MNEW CXMLText(this);
	if (pNode != nullptr)
	{
		pNode->SetValue(pszText);
		pNode->m_Index = m_XMLNode.AddTail(pNode);
	}
	return pNode;
}

INLINE CXMLDeclaration* CXMLDocument::CreateDeclaration(PCXStr pszDeclaration)
{
	CXMLDeclaration* pNode = MNEW CXMLDeclaration(this);
	if (pNode != nullptr)
	{
		pNode->SetValue(pszDeclaration);
		pNode->m_Index = m_XMLNode.AddTail(pNode);
	}
	return pNode;
}

INLINE CXMLUnknown* CXMLDocument::CreateUnknown(PCXStr pszUnknown)
{
	CXMLUnknown* pNode = MNEW CXMLUnknown(this);
	if (pNode != nullptr)
	{
		pNode->SetValue(pszUnknown);
		pNode->m_Index = m_XMLNode.AddTail(pNode);
	}
	return pNode;
}

INLINE bool CXMLDocument::Destroy(CXMLNode* pNode)
{
	assert(pNode != nullptr);
	assert(pNode->GetDocument() == this);
	if ((pNode != this) && Remove(pNode))
	{
		pNode->Destroy();
		DestroyNode(pNode);
		return true;
	}
	return false;
}

INLINE CXMLNode* CXMLDocument::Clone(CXMLDocument&) const
{
	return nullptr;
}

INLINE bool CXMLDocument::Equal(const CXMLNode&) const
{
	return false;
}

INLINE void CXMLDocument::Store(CString& strBuf) const
{
	if (m_XMLNode.GetSize() > 0)
	{
		assert(m_pPrev == nullptr);
		assert(m_pNext == nullptr);

		strBuf.SetBufferLength(Size());
		for (CXMLNode* pNode = m_pFirst; pNode != nullptr; pNode = pNode->GetNext())
		{
			pNode->Store(strBuf);
		}
	}
}

INLINE size_t CXMLDocument::Size(void) const
{
	if (m_XMLNode.GetSize() > 0)
	{
		assert(m_pPrev == nullptr);
		assert(m_pNext == nullptr);
		size_t stSize = 0;
		for (CXMLNode* pNode = m_pFirst; pNode != nullptr; pNode = pNode->GetNext())
		{
			stSize += pNode->Size();
		}
		return stSize;
	}
	return 0;
}

INLINE void CXMLDocument::DestroyNode(CXMLNode* pNode)
{
	m_XMLNode.RemoveAt(pNode->m_Index);
	MDELETE pNode;
}

INLINE void CXMLDocument::DestroyAll(void)
{
	PINDEX index = m_XMLNode.GetHeadIndex();
	while (index != nullptr)
	{
		CXMLNode* pNode = m_XMLNode.GetNext(index);
		pNode->Destroy(true);
		MDELETE pNode;
	}
	m_XMLNode.RemoveAll();
	m_pParent = nullptr;
	m_pFirst  = nullptr;
	m_pLast   = nullptr;
	m_pPrev   = nullptr;
	m_pNext   = nullptr;
	DEV_DEBUG(TF("CXMLDocument::DestroyAll"));
}

INLINE bool CXMLDocument::Parse(PXStr pszBuf)
{
	DestroyAll();
	while ((pszBuf != nullptr) && (*pszBuf != 0))
	{
		if (ParseNode(pszBuf) == false)
		{
			break;
		}
	}
	return (m_XMLNode.GetSize() > 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXMLElement
INLINE CXMLElement::CXMLElement(CXMLDocument* pDocument)
: CXMLNode(pDocument, XML_TYPE_ELEMENT)
, m_pFirstAttribute(nullptr)
, m_pLastAttribute(nullptr)
{
}

INLINE CXMLElement::~CXMLElement(void)
{
	assert(m_pFirstAttribute == nullptr);
	assert(m_pLastAttribute  == nullptr);
}

INLINE CXMLElement::CXMLElement(const CXMLElement&)
: CXMLNode(nullptr, XML_TYPE_ELEMENT)
, m_pFirstAttribute(nullptr)
, m_pLastAttribute(nullptr)
{
}

INLINE CXMLElement& CXMLElement::operator=(const CXMLElement&)
{
	return (*this);
}

INLINE void CXMLElement::GetName(CString& strName)
{
	GetValue(strName);
}

INLINE void CXMLElement::SetName(PCXStr pszName)
{
	SetValue(pszName);
}

INLINE bool CXMLElement::HasAttribute(PCXStr pszName, PCXStr pszValue) const
{
	if (pszName == nullptr)
	{
		return (m_pFirstAttribute != nullptr);
	}
	else if (m_pFirstAttribute != nullptr)
	{
		size_t stName  = CHash::Hash(pszName);
		size_t stValue = 0;
		if (pszValue != nullptr)
		{
			stValue = CHash::Hash(pszValue);
		}
		for (CXMLAttribute* pAttr = m_pFirstAttribute; pAttr != nullptr; pAttr = pAttr->m_pNext)
		{
			if (stName == pAttr->m_stName)
			{
				if ((stValue == 0) || (stValue == pAttr->m_stValue))
				{
					return true;
				}
			}
		}
	}
	return false;
}

INLINE bool CXMLElement::GetAttribute(PCXStr pszName, CString& strValue) const
{
	if ((pszName != nullptr) && (m_pFirstAttribute != nullptr))
	{
		CXMLAttribute* pAttr = FindAttribute(pszName);
		if (pAttr != nullptr)
		{
			return pAttr->GetValue(strValue);
		}
	}
	return false;
}

INLINE bool CXMLElement::GetAttribute(PCXStr pszName, Long& lValue) const
{
	if ((pszName != nullptr) && (m_pFirstAttribute != nullptr))
	{
		CXMLAttribute* pAttr = FindAttribute(pszName);
		if (pAttr != nullptr)
		{
			return pAttr->GetValue(lValue);
		}
	}
	return false;
}

INLINE bool CXMLElement::GetAttribute(PCXStr pszName, ULong& ulValue) const
{
	if ((pszName != nullptr) && (m_pFirstAttribute != nullptr))
	{
		CXMLAttribute* pAttr = FindAttribute(pszName);
		if (pAttr != nullptr)
		{
			return pAttr->GetValue(ulValue);
		}
	}
	return false;
}

INLINE bool CXMLElement::GetAttribute(PCXStr pszName, LLong& llValue) const
{
	if ((pszName != nullptr) && (m_pFirstAttribute != nullptr))
	{
		CXMLAttribute* pAttr = FindAttribute(pszName);
		if (pAttr != nullptr)
		{
			return pAttr->GetValue(llValue);
		}
	}
	return false;
}

INLINE bool CXMLElement::GetAttribute(PCXStr pszName, ULLong& ullValue) const
{
	if ((pszName != nullptr) && (m_pFirstAttribute != nullptr))
	{
		CXMLAttribute* pAttr = FindAttribute(pszName);
		if (pAttr != nullptr)
		{
			return pAttr->GetValue(ullValue);
		}
	}
	return false;
}

INLINE bool CXMLElement::GetAttribute(PCXStr pszName, Double& dValue) const
{
	if ((pszName != nullptr) && (m_pFirstAttribute != nullptr))
	{
		CXMLAttribute* pAttr = FindAttribute(pszName);
		if (pAttr != nullptr)
		{
			return pAttr->GetValue(dValue);
		}
	}
	return false;
}

INLINE bool CXMLElement::SetAttribute(PCXStr pszName, PCXStr pszValue)
{
	if (pszName != nullptr)
	{
		CXMLAttribute* pAttr = FindAttribute(pszName);
		if (pAttr != nullptr)
		{
			return pAttr->SetValue(pszValue);
		}
		else
		{
			return (AddAttributeAtLast(pszName, pszValue) != nullptr);
		}
	}
	return false;
}

INLINE bool CXMLElement::SetAttribute(PCXStr pszName, Long lValue)
{
	if (pszName != nullptr)
	{
		CXMLAttribute* pAttr = FindAttribute(pszName);
		if (pAttr == nullptr)
		{
			pAttr = AddAttributeAtLast(pszName);
		}
		if (pAttr != nullptr)
		{
			return pAttr->SetValue(lValue);
		}
	}
	return false;
}

INLINE bool CXMLElement::SetAttribute(PCXStr pszName, ULong ulValue)
{
	if (pszName != nullptr)
	{
		CXMLAttribute* pAttr = FindAttribute(pszName);
		if (pAttr == nullptr)
		{
			pAttr = AddAttributeAtLast(pszName);
		}
		if (pAttr != nullptr)
		{
			return pAttr->SetValue(ulValue);
		}
	}
	return false;
}

INLINE bool CXMLElement::SetAttribute(PCXStr pszName, LLong llValue)
{
	if (pszName != nullptr)
	{
		CXMLAttribute* pAttr = FindAttribute(pszName);
		if (pAttr == nullptr)
		{
			pAttr = AddAttributeAtLast(pszName);
		}
		if (pAttr != nullptr)
		{
			return pAttr->SetValue(llValue);
		}
	}
	return false;
}

INLINE bool CXMLElement::SetAttribute(PCXStr pszName, ULLong ullValue)
{
	if (pszName != nullptr)
	{
		CXMLAttribute* pAttr = FindAttribute(pszName);
		if (pAttr == nullptr)
		{
			pAttr = AddAttributeAtLast(pszName);
		}
		if (pAttr != nullptr)
		{
			return pAttr->SetValue(ullValue);
		}
	}
	return false;
}

INLINE bool CXMLElement::SetAttribute(PCXStr pszName, Double dValue)
{
	if (pszName != nullptr)
	{
		CXMLAttribute* pAttr = FindAttribute(pszName);
		if (pAttr == nullptr)
		{
			pAttr = AddAttributeAtLast(pszName);
		}
		if (pAttr != nullptr)
		{
			return pAttr->SetValue(dValue);
		}
	}
	return false;
}

INLINE CXMLAttribute* CXMLElement::AddFirstAttribute(PCXStr pszName, PCXStr pszValue)
{
	if (pszName != nullptr)
	{
		CXMLAttribute* pAttr = FindAttribute(pszName);
		if (pAttr == nullptr)
		{
			pAttr = AddAttributeAtFirst(pszName, pszValue);
		}
		else 
		{
			if (pszValue != nullptr)
			{
				pAttr->SetValue(pszValue);
			}
			MoveAttributeToFirst(pAttr);
		}
		return pAttr;
	}
	return nullptr;
}

INLINE CXMLAttribute* CXMLElement::AddLastAttribute(PCXStr pszName, PCXStr pszValue)
{
	if (pszName != nullptr)
	{
		CXMLAttribute* pAttr = FindAttribute(pszName);
		if (pAttr == nullptr)
		{
			pAttr = AddAttributeAtLast(pszName, pszValue);
		}
		else
		{
			if (pszValue != nullptr)
			{
				pAttr->SetValue(pszValue);
			}
			MoveAttributeToLast(pAttr);
		}
		return pAttr;
	}
	return nullptr;
}

INLINE bool CXMLElement::MoveAttributeToFirst(CXMLAttribute* pAttr)
{
	assert(pAttr != nullptr);
	assert(pAttr->GetElement() == this);
	if (pAttr->GetElement() == this)
	{	
		assert(m_pFirstAttribute != nullptr);
		if (pAttr != m_pFirstAttribute)
		{
			RemoveAttributeList(pAttr);
			if (m_pFirstAttribute != nullptr)
			{
				assert(m_pLastAttribute != nullptr);
				pAttr->m_pNext = m_pFirstAttribute;
				m_pFirstAttribute->m_pPrev = pAttr;
			}
			else
			{
				assert(m_pLastAttribute == nullptr);
				pAttr->m_pNext   = nullptr;
				m_pLastAttribute = pAttr;
			}
			pAttr->m_pPrev    = nullptr;
			m_pFirstAttribute = pAttr;
		}
		return true;
	}
	return false;
}

INLINE bool CXMLElement::MoveAttributeToLast(CXMLAttribute* pAttr)
{
	assert(pAttr != nullptr);
	assert(pAttr->GetElement() == this);
	if (pAttr->GetElement() == this)
	{	
		assert(m_pFirstAttribute != nullptr);
		if (pAttr != m_pLastAttribute)
		{
			RemoveAttributeList(pAttr);
			if (m_pFirstAttribute != nullptr)
			{
				assert(m_pLastAttribute != nullptr);
				pAttr->m_pPrev = m_pLastAttribute;
				m_pLastAttribute->m_pNext = pAttr;
			}
			else
			{
				assert(m_pLastAttribute == nullptr);
				pAttr->m_pPrev    = nullptr;
				m_pFirstAttribute = pAttr;
			}
			pAttr->m_pNext   = nullptr;
			m_pLastAttribute = pAttr;
		}
		return true;
	}
	return false;
}

INLINE bool CXMLElement::RemoveFirstAttribute(void)
{
	if (m_pFirstAttribute != nullptr)
	{	
		CXMLAttribute* pAttr = m_pFirstAttribute;
		m_pFirstAttribute = pAttr->m_pNext;
		if (m_pFirstAttribute != nullptr)
		{
			m_pFirstAttribute->m_pPrev = nullptr;
		}
		else
		{
			m_pLastAttribute = nullptr;
		}
		MDELETE pAttr;
		return true;
	}
	return false;
}

INLINE bool CXMLElement::RemoveLastAttribute(void)
{
	if (m_pFirstAttribute != nullptr)
	{	
		CXMLAttribute* pAttr = m_pLastAttribute;
		if (pAttr->m_pPrev != nullptr)
		{
			m_pLastAttribute = pAttr->m_pPrev;
			m_pLastAttribute->m_pNext = nullptr;
		}
		else
		{
			m_pFirstAttribute = nullptr;
			m_pLastAttribute  = nullptr;
		}
		MDELETE pAttr;
		return true;
	}
	return false;
}

INLINE bool CXMLElement::RemoveAttribute(PCXStr pszName)
{
	if ((pszName != nullptr) && (m_pFirstAttribute != nullptr))
	{
		CXMLAttribute* pAttr = FindAttribute(pszName);
		if (pAttr != nullptr)
		{
			return RemoveAttribute(pAttr);
		}
	}
	return false;
}

INLINE bool CXMLElement::RemoveAttribute(CXMLAttribute* pAttr)
{
	assert(pAttr != nullptr);
	assert(pAttr->GetElement() == this);
	if (pAttr->GetElement() == this)
	{	
		assert(m_pFirstAttribute != nullptr);
		if (pAttr == m_pFirstAttribute)
		{
			return RemoveFirstAttribute();
		}
		else if (pAttr == m_pLastAttribute)
		{
			return RemoveLastAttribute();
		}
		assert(pAttr->m_pPrev != nullptr);
		assert(pAttr->m_pNext != nullptr);
		pAttr->m_pPrev->m_pNext = pAttr->m_pNext;
		pAttr->m_pNext->m_pPrev = pAttr->m_pPrev;
		MDELETE pAttr;
		return true;
	}
	return false;
}

INLINE void CXMLElement::RemoveAllAttribute(void)
{
	if (m_pFirstAttribute != nullptr)
	{	
		CXMLAttribute* pNext = nullptr;
		CXMLAttribute* pAttr = m_pFirstAttribute;
		do
		{
			pNext = pAttr->m_pNext;

			MDELETE pAttr;

			pAttr = pNext;
		} while (pAttr != nullptr);

		m_pFirstAttribute = nullptr;
		m_pLastAttribute  = nullptr;
	}
}

INLINE bool CXMLElement::InsertAttributeBefore(PCXStr pszName, PCXStr pszValue, CXMLAttribute* pBefore)
{
	assert(pBefore != nullptr);
	assert(pBefore->GetElement() == this);

	if (CheckName(pszName) && (pBefore != nullptr) && (pBefore->GetElement() == this))
	{
		CXMLAttribute* pAttr = FindAttribute(pszName);
		if (pAttr == nullptr)
		{
			pAttr = MNEW CXMLAttribute(this, pszName, pszValue);
		}
		else if (pszValue != nullptr)
		{
			pAttr->SetValue(pszValue);
		}
		if (pAttr != nullptr)
		{
			return InsertAttributeBefore(pAttr, pBefore);
		}
	}
	return false;
}

INLINE bool CXMLElement::InsertAttributeAfter(PCXStr pszName, PCXStr pszValue, CXMLAttribute* pAfter)
{
	assert(pAfter != nullptr);
	assert(pAfter->GetElement() == this);

	if (CheckName(pszName) && (pAfter != nullptr) && (pAfter->GetElement() == this))
	{
		CXMLAttribute* pAttr = FindAttribute(pszName);
		if (pAttr == nullptr)
		{
			pAttr = MNEW CXMLAttribute(this, pszName, pszValue);
		}
		else if (pszValue != nullptr)
		{
			pAttr->SetValue(pszValue);
		}
		if (pAttr != nullptr)
		{
			return InsertAttributeAfter(pAttr, pAfter);
		}
	}
	return false;
}

INLINE bool CXMLElement::InsertAttributeBefore(CXMLAttribute* pAttr, CXMLAttribute* pBefore)
{
	assert(pAttr != nullptr);
	assert(pAttr->GetElement() == this);
	assert(pBefore != nullptr);
	assert(pBefore->GetElement() == this);

	if ((pAttr != pBefore) && (pAttr->GetElement() == pBefore->GetElement()))
	{
		if (pBefore == m_pFirstAttribute)
		{
			return MoveAttributeToFirst(pAttr);
		}
		else
		{
			RemoveAttributeList(pAttr);
			assert(pBefore->m_pPrev != nullptr);
			pAttr->m_pPrev = pBefore->m_pPrev;
			pAttr->m_pNext = pBefore;
			pBefore->m_pPrev->m_pNext = pAttr;
			pBefore->m_pPrev = pAttr;
		}
		return true;
	}
	return false;
}

INLINE bool CXMLElement::InsertAttributeAfter(CXMLAttribute* pAttr, CXMLAttribute* pAfter)
{
	assert(pAttr != nullptr);
	assert(pAttr->GetElement() == this);
	assert(pAfter != nullptr);
	assert(pAfter->GetElement() == this);

	if ((pAttr != pAfter) && (pAttr->GetElement() == pAfter->GetElement()))
	{
		if (pAfter == m_pLastAttribute)
		{
			return MoveAttributeToLast(pAttr);
		}
		else
		{
			assert(pAfter->m_pNext != nullptr);
			pAttr->m_pNext = pAfter->m_pNext;
			pAttr->m_pPrev = pAfter;
			pAfter->m_pNext->m_pPrev = pAttr;
			pAfter->m_pNext = pAttr;
		}
		return true;
	}
	return false;
}

INLINE CXMLAttribute* CXMLElement::FindAttribute(PCXStr pszName, CXMLAttribute* pStartAfter) const
{
	if (pszName != nullptr)
	{
		size_t stHash = CHash::Hash(pszName);

		CXMLAttribute* pAttr = m_pFirstAttribute;
		if ((pStartAfter != nullptr) && (pStartAfter->GetElement() == this))
		{
			pAttr = pStartAfter->m_pNext;
		}
		while (pAttr != nullptr)
		{
			if (stHash == pAttr->m_stName)
			{
				return pAttr;
			}
			pAttr = pAttr->m_pNext;
		}
	}
	return nullptr;
}

INLINE CXMLAttribute* CXMLElement::GetFirstAttribute(void)
{
	return m_pFirstAttribute;
}

INLINE const CXMLAttribute* CXMLElement::GetFirstAttribute(void) const
{
	return m_pFirstAttribute;
}

INLINE CXMLAttribute* CXMLElement::GetLastAttribute(void)
{
	return m_pLastAttribute;
}

INLINE const CXMLAttribute* CXMLElement::GetLastAttribute(void) const
{
	return m_pLastAttribute;
}

INLINE CXMLNode* CXMLElement::Clone(CXMLDocument& aSrc) const
{
	CXMLElement* pNode = aSrc.CreateElement(*m_strValue);
	if (pNode != nullptr)
	{
		for (CXMLAttribute* pAttr = m_pFirstAttribute; pAttr != nullptr; pAttr = pAttr->m_pNext)
		{
			pNode->AddLastAttribute(*(pAttr->m_strName), *(pAttr->m_strValue));
		}
		return pNode;
	}
	return nullptr;
}

INLINE bool CXMLElement::Equal(const CXMLNode& aSrc) const
{
	if (&aSrc == this)
	{
		return true;
	}
	const CXMLElement* pNode = aSrc.To<CXMLElement, XML_TYPE_ELEMENT>();
	if ((pNode == nullptr) || (pNode->m_strValue != m_strValue))
	{
		return false;
	}

	CXMLAttribute* pAttr1 = m_pFirstAttribute;
	CXMLAttribute* pAttr2 = pNode->m_pFirstAttribute;
	while ((pAttr1 != nullptr) && (pAttr2 != nullptr))
	{
		if (pAttr1->m_stName != pAttr2->m_stName)
		{
			return false;
		}
		if (pAttr1->m_stValue != pAttr2->m_stValue)
		{
			return false;
		}
	}
	if ((pAttr1 != nullptr) || (pAttr2 != nullptr))
	{
		return false;
	}
	return true;
}

INLINE void CXMLElement::Store(CString& strBuf) const
{
	strBuf += LessChar;
	strBuf += m_strValue;
	for (CXMLAttribute* pAttr = m_pFirstAttribute; pAttr != nullptr; pAttr = pAttr->m_pNext)
	{
		pAttr->Store(strBuf);
	}
	if (HasChild())
	{
		strBuf += GreaterChar;
		for (CXMLNode* pNode = m_pFirst; pNode != nullptr; pNode = pNode->GetNext())
		{
			pNode->Store(strBuf);
		}
		strBuf += LessChar;
		strBuf += CXChar::SlashChar;
		strBuf += m_strValue;
		strBuf += GreaterChar;
	}
	else
	{
		strBuf += CXChar::SlashChar;
		strBuf += GreaterChar;
	}
	strBuf += CXChar::ReturnChar;
	strBuf += CXChar::LineChar;
}

INLINE size_t CXMLElement::Size(void) const
{
	size_t stSize = XML_TAG_CHAR_MAX_LEN + m_strValue.Length();
	stSize += stSize;

	for (CXMLAttribute* pAttr = m_pFirstAttribute; pAttr != nullptr; pAttr = pAttr->m_pNext)
	{
		stSize += pAttr->Size();
	}
	for (CXMLNode* pNode = m_pFirst; pNode != nullptr; pNode = pNode->GetNext())
	{
		stSize += pNode->Size();
	}
	return stSize;
}

INLINE CXMLAttribute* CXMLElement::AddAttributeAtFirst(PCXStr pszName, PCXStr pszValue)
{
	if (CheckName(pszName))
	{
		CXMLAttribute* pAttr = MNEW CXMLAttribute(this, pszName, pszValue);
		if (pAttr != nullptr)
		{
			if (m_pFirstAttribute != nullptr)
			{
				assert(m_pLastAttribute != nullptr);
				pAttr->m_pNext = m_pFirstAttribute;
				m_pFirstAttribute->m_pPrev = pAttr;
			}
			else
			{
				assert(m_pLastAttribute == nullptr);
				pAttr->m_pNext   = nullptr;
				m_pLastAttribute = pAttr;
			}
			pAttr->m_pPrev = nullptr;
			m_pFirstAttribute = pAttr;
		}
		return pAttr;
	}
	return nullptr;
}

INLINE CXMLAttribute* CXMLElement::AddAttributeAtLast(PCXStr pszName, PCXStr pszValue)
{
	if (CheckName(pszName))
	{
		CXMLAttribute* pAttr = MNEW CXMLAttribute(this, pszName, pszValue);
		if (pAttr != nullptr)
		{
			if (m_pFirstAttribute != nullptr)
			{
				assert(m_pLastAttribute != nullptr);
				pAttr->m_pPrev = m_pLastAttribute;
				m_pLastAttribute->m_pNext = pAttr;
			}
			else
			{
				assert(m_pLastAttribute == nullptr);
				pAttr->m_pPrev = nullptr;
				m_pFirstAttribute = pAttr;
			}
			pAttr->m_pNext = nullptr;
			m_pLastAttribute = pAttr;
		}
	}
	return nullptr;
}

INLINE void CXMLElement::RemoveAttributeList(CXMLAttribute* pAttr)
{
	if (pAttr == m_pFirstAttribute)
	{
		m_pFirstAttribute = pAttr->m_pNext;
		if (m_pFirstAttribute != nullptr)
		{
			m_pFirstAttribute->m_pPrev = nullptr;
		}
		else
		{
			m_pLastAttribute = nullptr;
		}
	}
	else if (pAttr == m_pLastAttribute)
	{
		if (pAttr->m_pPrev != nullptr)
		{
			m_pLastAttribute = pAttr->m_pPrev;
			m_pLastAttribute->m_pNext = nullptr;
		}
		else
		{
			m_pFirstAttribute = nullptr;
		}
	}
	else
	{
		assert(pAttr->m_pPrev != nullptr);
		assert(pAttr->m_pNext != nullptr);
		pAttr->m_pPrev->m_pNext = pAttr->m_pNext;
		pAttr->m_pNext->m_pPrev = pAttr->m_pPrev;
	}
	pAttr->m_pPrev = nullptr;
	pAttr->m_pNext = nullptr;
}

INLINE bool CXMLElement::ParseAttributes(PXStr& pszBuf)
{
	CString strName;
	CString strValue;
	while (SkipChars(pszBuf))
	{
		if ((pszBuf[0] == GreaterChar) || (pszBuf[0] == CXChar::SlashChar))
		{
			return true;
		}
		if (ToName(pszBuf, strName) == false)
		{
			break;
		}
		if (SkipChars(pszBuf) == false)
		{
			break;
		}
		if (pszBuf[0] != EqualChar)
		{ 
			break;
		}
		++pszBuf;
		if (SkipChars(pszBuf) == false)
		{
			break;
		}
		if ((pszBuf[0] != SingleQuoteChar) && (pszBuf[0] != DoubleQuoteChar))
		{
			break;
		}
		XChar cQuote = pszBuf[0];
		++pszBuf;
		PCXStr p = CXChar::FindChar(pszBuf, cQuote);
		if (p == nullptr)
		{
			break;
		}
		if (p > pszBuf)
		{
			strValue.FillBuffer(pszBuf, (p - pszBuf));
			ToChars(strValue);
		}
		
		pszBuf = (PXStr)p;
		++pszBuf;

		DEV_DEBUG(TF("CXMLElement::ParseAttributes  %s=\"%s\""), *strName, *strValue);

		AddLastAttribute(*strName, *strValue);
	}
	return false;
}

INLINE bool CXMLElement::ParseNode(PXStr& pszBuf)
{
	if (SkipChars(pszBuf))
	{
		if (ToName(pszBuf, m_strValue) == false) 
		{
			return false;
		}
		m_stHash = CHash::Hash(*m_strValue, m_strValue.Length());
		DEV_DEBUG(TF("CXMLElement::ParseNode name %s"), *m_strValue);

		if (ParseAttributes(pszBuf) == false)
		{
			return false;
		}
		if (pszBuf[0] == GreaterChar)
		{
			pszBuf += XML_TAG_CHAR_LEN;
			while (SkipChars(pszBuf))
			{
				if ((pszBuf[0] == LessChar) && (pszBuf[1] == CXChar::SlashChar))
				{	// close tag
					pszBuf += (XML_TAG_CHAR_LEN + XML_TAG_CHAR_LEN);
					CString strCloseTag;
					if (ToName(pszBuf, strCloseTag) && (strCloseTag == m_strValue))
					{
						if (SkipChars(pszBuf) && (pszBuf[0] == GreaterChar))
						{
							DEV_DEBUG(TF("CXMLElement::ParseNode close tag %s find"), *m_strValue);
							pszBuf += XML_TAG_CHAR_LEN;
							return true;
						}
					}
					DEV_DEBUG(TF("CXMLElement::ParseNode close tag name mismatch %s <> %s"), *strCloseTag, *m_strValue);
					break;
				}
				else if (CXMLNode::ParseNode(pszBuf) == false)
				{
					break;
				}
			}
		}
		else if ((pszBuf[0] == CXChar::SlashChar) && (pszBuf[1] == GreaterChar))
		{
			DEV_DEBUG(TF("CXMLElement::ParseNode close with /"));
			pszBuf += (XML_TAG_CHAR_LEN + XML_TAG_CHAR_LEN);
			return true;
		}
	}
	return false;
}

INLINE void CXMLElement::Destroy(bool bSelfOnly)
{
	RemoveAllAttribute();
	CXMLNode::Destroy(bSelfOnly);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXMLComment
INLINE CXMLComment::CXMLComment(CXMLDocument* pDocument)
: CXMLNode(pDocument, XML_TYPE_COMMENT)
{
}

INLINE CXMLComment::~CXMLComment(void)
{
}

INLINE CXMLComment::CXMLComment(const CXMLComment&)
: CXMLNode(nullptr, XML_TYPE_COMMENT)
{
}

INLINE CXMLComment& CXMLComment::operator=(const CXMLComment&)
{
	return (*this);
}

INLINE CXMLNode* CXMLComment::Clone(CXMLDocument& aSrc) const
{
	return aSrc.CreateComment(*m_strValue);
}

INLINE bool CXMLComment::Equal(const CXMLNode& aSrc) const
{
	if (&aSrc == this)
	{
		return true;
	}
	const CXMLComment* pNode = aSrc.To<CXMLComment, XML_TYPE_COMMENT>();
	if ((pNode == nullptr) || (pNode->m_strValue != m_strValue))
	{
		return false;
	}
	return true;
}

INLINE void CXMLComment::Store(CString& strBuf) const
{
	strBuf += CommentString;
	strBuf += m_strValue;
	strBuf += CommentEndString;
	strBuf += CXChar::ReturnChar;
	strBuf += CXChar::LineChar;
}

INLINE size_t CXMLComment::Size(void) const
{
	return (XML_TAG_COMMENT_LEN + XML_TAG_COMMENT_END_LEN + XML_TAG_CHAR_MAX_LEN + m_strValue.Length());
}

INLINE bool CXMLComment::ParseNode(PXStr& pszBuf)
{
	if (SkipChars(pszBuf))
	{
		PCXStr p = CXChar::FindString(pszBuf, CommentEndString);
		if (p != nullptr)
		{	
			if (p > pszBuf)
			{	// do nothing, copy string only
				m_strValue.FillBuffer(pszBuf, (p - pszBuf));
				m_stHash = CHash::Hash(*m_strValue, m_strValue.Length());
			}
			DEV_DEBUG(TF("CXMLComment::ParseNode value %s"), *m_strValue);
			pszBuf = (PXStr)(p + XML_TAG_COMMENT_END_LEN);
			return true;
		}
#ifdef RUNTIME_DEBUG
		else
		{
			DEV_DEBUG(TF("CXMLComment::ParseNode find end string %s failed"), CommentEndString);
		}
#endif  // RUNTIME_DEBUG
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXMLText
INLINE CXMLText::CXMLText(CXMLDocument* pDocument)
: CXMLNode(pDocument, XML_TYPE_TEXT)
, m_bCData(false)
{
}

INLINE CXMLText::~CXMLText(void)
{
}

INLINE CXMLText::CXMLText(const CXMLText&)
: CXMLNode(nullptr, XML_TYPE_TEXT)
, m_bCData(false)
{
}

INLINE CXMLText& CXMLText::operator=(const CXMLText&)
{
	return (*this);
}

INLINE bool CXMLText::IsCData(void) const
{
	return m_bCData;
}

INLINE void CXMLText::SetCData(bool bCData)
{
	m_bCData = bCData;
}

INLINE CXMLNode* CXMLText::Clone(CXMLDocument& aSrc) const
{
	return aSrc.CreateText(*m_strValue);
}

INLINE bool CXMLText::Equal(const CXMLNode& aSrc) const
{
	if (&aSrc == this)
	{
		return true;
	}
	const CXMLText* pNode = aSrc.To<CXMLText, XML_TYPE_TEXT>();
	if ((pNode == nullptr) || (pNode->m_strValue != m_strValue))
	{
		return false;
	}
	return true;
}

INLINE void CXMLText::Store(CString& strBuf) const
{
	if (m_bCData)
	{
		strBuf += CDataString;
		strBuf += m_strValue;
		strBuf += CDataEndString;
	}
	else
	{
		CString strEntity;
		ToEntity(m_strValue, strEntity);
		strBuf += strEntity;
	}
}

INLINE size_t CXMLText::Size(void) const
{
	return (XML_TAG_CHAR_MAX_LEN + XML_TAG_CHAR_MAX_LEN + m_strValue.Length());
}

INLINE bool CXMLText::ParseNode(PXStr& pszBuf)
{
	if (SkipChars(pszBuf))
	{
		if (m_bCData)
		{
			PCXStr p = CXChar::FindString(pszBuf, CDataEndString);
			if (p != nullptr)
			{	
				if (p > pszBuf)
				{	// copy only
					m_strValue.FillBuffer(pszBuf, (p - pszBuf));
					m_stHash = CHash::Hash(*m_strValue, m_strValue.Length());
				}
				DEV_DEBUG(TF("CXMLText::ParseNode cdata %s"), *m_strValue);
				pszBuf = (PXStr)(p + XML_TAG_TEXT_CDATA_END_LEN);
				return true;
			}
#ifdef RUNTIME_DEBUG
			else
			{
				DEV_DEBUG(TF("CXMLText::ParseNode find cdata end string %s failed"), CDataEndString);
			}
#endif  // RUNTIME_DEBUG
		}
		else
		{	
			PCXStr p = CXChar::FindChar(pszBuf, LessChar);
			if (p != nullptr)
			{	// convert entities
				m_strValue.FillBuffer(pszBuf, (p - pszBuf));
				ToChars(m_strValue);
				m_stHash = CHash::Hash(*m_strValue, m_strValue.Length());

				DEV_DEBUG(TF("CXMLText::ParseNode value %s"), *m_strValue);
				pszBuf = (PXStr)p;
				return true;
			}
#ifdef RUNTIME_DEBUG
			else
			{
				DEV_DEBUG(TF("CXMLText::ParseNode find end char %c failed"), LessChar);
			}
#endif  // RUNTIME_DEBUG
		}
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXMLDeclaration
INLINE CXMLDeclaration::CXMLDeclaration(CXMLDocument* pDocument)
: CXMLNode(pDocument, XML_TYPE_DECLARATION)
{
}

INLINE CXMLDeclaration::~CXMLDeclaration(void)
{
}

INLINE CXMLDeclaration::CXMLDeclaration(const CXMLDeclaration&)
: CXMLNode(nullptr, XML_TYPE_DECLARATION)
{
}

INLINE CXMLDeclaration& CXMLDeclaration::operator=(const CXMLDeclaration&)
{
	return (*this);
}

INLINE CXMLNode* CXMLDeclaration::Clone(CXMLDocument& aSrc) const
{
	return aSrc.CreateDeclaration();
}

INLINE bool CXMLDeclaration::Equal(const CXMLNode& aSrc) const
{
	if (&aSrc == this)
	{
		return true;
	}
	const CXMLDeclaration* pNode = aSrc.To<CXMLDeclaration, XML_TYPE_DECLARATION>();
	return (pNode != nullptr);
}

INLINE void CXMLDeclaration::Store(CString& strBuf) const
{
	strBuf += DeclarationString;
	strBuf += CXChar::SpaceChar;
	strBuf += m_strValue;
	strBuf += DeclarationEndString;
	strBuf += CXChar::ReturnChar;
	strBuf += CXChar::LineChar;
}

INLINE size_t CXMLDeclaration::Size(void) const
{
	return (XML_TAG_DECLARATION_LEN + XML_TAG_DECLARATION_END_LEN + m_strValue.Length());
}

INLINE bool CXMLDeclaration::ParseNode(PXStr& pszBuf)
{
	if (SkipChars(pszBuf))
	{
		PCXStr p = CXChar::FindString(pszBuf, DeclarationEndString);
		if (p != nullptr)
		{	
			if (p > pszBuf)
			{	// do nothing, just copy string only
				m_strValue.FillBuffer(pszBuf, (p - pszBuf));
				m_stHash = CHash::Hash(*m_strValue, m_strValue.Length());
			}
			DEV_DEBUG(TF("CXMLDeclaration::ParseNode value %s"), *m_strValue);
			pszBuf = (PXStr)(p + XML_TAG_DECLARATION_END_LEN);
			return true;
		}
#ifdef RUNTIME_DEBUG
		else
		{
			DEV_DEBUG(TF("CXMLDeclaration::ParseNode find end string %s failed"), DeclarationEndString);
		}
#endif  // RUNTIME_DEBUG
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXMLUnknown
INLINE CXMLUnknown::CXMLUnknown(CXMLDocument* pDocument)
: CXMLNode(pDocument, XML_TYPE_UNKNOWN)
, m_bDtd(true)
{
}

INLINE CXMLUnknown::~CXMLUnknown(void)
{
}

INLINE CXMLUnknown::CXMLUnknown(const CXMLUnknown&)
: CXMLNode(nullptr, XML_TYPE_UNKNOWN)
, m_bDtd(true)
{
}

INLINE CXMLUnknown& CXMLUnknown::operator=(const CXMLUnknown&)
{
	return (*this);
}

INLINE bool CXMLUnknown::IsDtd(void) const
{
	return m_bDtd;
}

INLINE void CXMLUnknown::SetDtd(bool bDtd)
{
	m_bDtd = bDtd;
}

INLINE CXMLNode* CXMLUnknown::Clone(CXMLDocument& aSrc) const
{
	return aSrc.CreateUnknown(*m_strValue);
}

INLINE bool CXMLUnknown::Equal(const CXMLNode& aSrc) const
{
	if (&aSrc == this)
	{
		return true;
	}
	const CXMLUnknown* pNode = aSrc.To<CXMLUnknown, XML_TYPE_UNKNOWN>();
	if ((pNode == nullptr) || (pNode->m_strValue != m_strValue))
	{
		return false;
	}
	return true;
}

INLINE void CXMLUnknown::Store(CString& strBuf) const
{
	strBuf += LessChar;
	if (IsDtd())
	{
		strBuf += ExclamChar;
	}
	else
	{
		strBuf += QuestionChar;
	}
	strBuf += m_strValue;
	if (IsDtd() == false)
	{
		strBuf += QuestionChar;
	}
	strBuf += GreaterChar;
	strBuf += CXChar::ReturnChar;
	strBuf += CXChar::LineChar;
}

INLINE size_t CXMLUnknown::Size(void) const
{
	return (XML_TAG_CHAR_MAX_LEN + m_strValue.Length());
}

INLINE bool CXMLUnknown::ParseNode(PXStr& pszBuf)
{
	if (SkipChars(pszBuf))
	{
		PCXStr p = CXChar::FindChar(pszBuf, GreaterChar);
		if (p != nullptr)
		{	
			if (p > pszBuf)
			{	// do nothing, just copy string only
				m_strValue.FillBuffer(pszBuf, (p - pszBuf));
				m_stHash = CHash::Hash(*m_strValue, m_strValue.Length());
			}
			DEV_DEBUG(TF("CXMLUnknown::ParseNode value %s"), *m_strValue);
			pszBuf = (PXStr)(p + XML_TAG_CHAR_LEN);
			return true;
		}
#ifdef RUNTIME_DEBUG
		else
		{
			DEV_DEBUG(TF("CXMLUnknown::ParseNode find end char %c failed"), GreaterChar);
		}
#endif  // RUNTIME_DEBUG
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXMLAttribute
INLINE CXMLAttribute::CXMLAttribute(CXMLElement* pElement, PCXStr pszName, PCXStr pszValue)
: m_pElement(pElement)
, m_stName(0)
, m_stValue(0)
, m_pPrev(nullptr)
, m_pNext(nullptr)
{
	if ((pszName != nullptr) && (*pszName != 0))
	{
		m_strName = pszName;
		m_stName  = CHash::Hash(*m_strName, m_strName.Length());
	}
	if ((pszValue != nullptr) && (*pszValue != 0))
	{
		m_strValue = pszValue;
		m_stValue  = CHash::Hash(*m_strValue, m_strValue.Length());
	}
}

INLINE CXMLAttribute::~CXMLAttribute(void)
{
}

INLINE CXMLAttribute::CXMLAttribute(const CXMLAttribute&)
: m_pElement(nullptr)
, m_stName(0)
, m_stValue(0)
, m_pPrev(nullptr)
, m_pNext(nullptr)
{
}

INLINE CXMLAttribute& CXMLAttribute::operator=(const CXMLAttribute&)
{
	return (*this);
}

INLINE CXMLElement* CXMLAttribute::GetElement(void)
{
	return m_pElement;
}

INLINE const CXMLElement* CXMLAttribute::GetElement(void) const
{
	return m_pElement;
}

INLINE bool CXMLAttribute::GetName(CString& strName)
{
	strName = m_strName;
	return true;
}

INLINE bool CXMLAttribute::GetValue(CString& strValue)
{
	strValue = m_strValue;
	return true;
}

INLINE bool CXMLAttribute::SetValue(PCXStr pszValue)
{
	if ((pszValue != nullptr) && (*pszValue != 0))
	{
		m_strValue = pszValue;
		m_stValue  = CHash::Hash(*m_strValue, m_strValue.Length());
		return true;
	}
	return false;
}

INLINE bool CXMLAttribute::GetValue(Long& lValue) const
{
	if (m_strValue.IsEmpty() == false)
	{
		lValue = m_strValue.ToLong();
		return true;
	}
	return false;
}

INLINE bool CXMLAttribute::GetValue(ULong& ulValue) const
{
	if (m_strValue.IsEmpty() == false)
	{
		ulValue = m_strValue.ToULong();
		return true;
	}
	return false;
}

INLINE bool CXMLAttribute::GetValue(LLong& llValue) const
{
	if (m_strValue.IsEmpty() == false)
	{
		llValue = m_strValue.ToLLong();
		return true;
	}
	return false;
}

INLINE bool CXMLAttribute::GetValue(ULLong& ullValue) const
{
	if (m_strValue.IsEmpty() == false)
	{
		ullValue = m_strValue.ToULLong();
		return true;
	}
	return false;
}

INLINE bool CXMLAttribute::GetValue(Double& dValue) const
{
	if (m_strValue.IsEmpty() == false)
	{
		dValue = m_strValue.ToDouble();
		return true;
	}
	return false;
}

INLINE bool CXMLAttribute::SetValue(Long lValue)
{
	if (m_strValue.ToString(lValue))
	{
		m_stValue  = CHash::Hash(*m_strValue, m_strValue.Length());
		return true;
	}
	return false;
}

INLINE bool CXMLAttribute::SetValue(ULong ulValue)
{
	if (m_strValue.ToString(ulValue))
	{
		m_stValue  = CHash::Hash(*m_strValue, m_strValue.Length());
		return true;
	}
	return false;
}

INLINE bool CXMLAttribute::SetValue(LLong llValue)
{
	if (m_strValue.ToString(llValue))
	{
		m_stValue  = CHash::Hash(*m_strValue, m_strValue.Length());
		return true;
	}
	return false;
}

INLINE bool CXMLAttribute::SetValue(ULLong ullValue)
{
	if (m_strValue.ToString(ullValue))
	{
		m_stValue  = CHash::Hash(*m_strValue, m_strValue.Length());
		return true;
	}
	return false;
}

INLINE bool CXMLAttribute::SetValue(Double dValue)
{
	if (m_strValue.ToString(dValue))
	{
		m_stValue  = CHash::Hash(*m_strValue, m_strValue.Length());
		return true;
	}
	return false;
}

INLINE const CXMLAttribute* CXMLAttribute::Prev(void) const
{
	return m_pPrev;
}

INLINE const CXMLAttribute* CXMLAttribute::Next(void) const
{
	return m_pNext;
}

INLINE void CXMLAttribute::Store(CString& strBuf) const
{
	CString strEntity;
	ToEntity(m_strValue, strEntity);

	strBuf += CXChar::SpaceChar;
	strBuf += m_strName;
	strBuf += EqualChar;
	strBuf += DoubleQuoteChar;
	strBuf += strEntity;
	strBuf += DoubleQuoteChar;
}

INLINE size_t CXMLAttribute::Size(void) const
{
	// space + name + equal + quote + value + quote
	size_t stSize = XML_TAG_CHAR_MAX_LEN;
	stSize += m_strName.Length() + m_strValue.Length();
	return stSize;
}

#endif // __MARKUP_INL__