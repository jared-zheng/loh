///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : markup.h                                                                              //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : markup language                                                                            //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __MARKUP_H__
#define __MARKUP_H__

#pragma once

#include "container.h"
#include "stream.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
class CXMLNode;
class CXMLDocument;
class CXMLElement;
class CXMLComment;
class CXMLText;
class CXMLDeclaration;
class CXMLUnknown;
class CXMLAttribute;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXMLTraits
class CXMLTraits : public MObject
{
public:
	enum XML_TYPE
	{
		XML_TYPE_NODE,
		XML_TYPE_DOCUMENT,
		XML_TYPE_ELEMENT,
		XML_TYPE_COMMENT,
		XML_TYPE_TEXT,
		XML_TYPE_DECLARATION,
		XML_TYPE_UNKNOWN,
	};

	enum XML_ENCODING
	{
		XML_ENCODING_UTF8,
		XML_ENCODING_UNICODE,
		XML_ENCODING_ASNI,
	};

	enum XML_ENCODING_LEADBYTE
	{
		XML_ENCODING_LB_UTF80        = 0xEF,
		XML_ENCODING_LB_UTF81        = 0xBB,
		XML_ENCODING_LB_UTF82        = 0xBF,

		XML_ENCODING_LB_UTF8LEN      = 3,

		XML_ENCODING_LB_UNICODE0     = 0xFF,
		XML_ENCODING_LB_UNICODE1     = 0xFE,

		XML_ENCODING_LB_UNICODELEN   = 2,

		XML_ENCODING_LB_MAXLEN       = 3,
	};

	enum XML_TAG
	{
		XML_TAG_CHAR_LEN             = 1,
		XML_TAG_CHAR_MAX_LEN         = 8,

		XML_TAG_COMMENT_LEN          = 4,
		XML_TAG_TEXT_CDATA_LEN       = 9,
		XML_TAG_DECLARATION_LEN      = 5,
		XML_TAG_UNKNOWN_DTD_LEN      = 2,
		XML_TAG_XML_LEN              = 3,

		XML_TAG_COMMENT_END_LEN      = 3,
		XML_TAG_TEXT_CDATA_END_LEN   = 3,
		XML_TAG_DECLARATION_END_LEN  = 2,
	};

	enum XML_ENTITY
	{
		XML_ENTITY_LESS_LEN          = 4,
		XML_ENTITY_GREATER_LEN       = 4,
		XML_ENTITY_AND_LEN           = 5,
		XML_ENTITY_SINGLE_LEN        = 6,
		XML_ENTITY_DOUBLE_LEN        = 6,
		XML_ENTITY_MIN_LEN           = 8,
		XML_ENTITY_MIN_CHAR          = 31,
	};
public:
	static XChar   LessChar;              // <
	static XChar   GreaterChar;           // >
	static XChar   AndChar;               // &
	static XChar   SingleQuoteChar;       // \'
	static XChar   DoubleQuoteChar;       // \"
	static XChar   ExclamChar;            // !
	static XChar   QuestionChar;          // ?
	static XChar   UnderlineChar;         // _
	static XChar   EqualChar;             // =
	static XChar   PoundChar;             // #
	static XChar   SemicolonChar;         // ;

	static CPCXStr CommentString;         // <!--
	static CPCXStr CDataString;           // <![CDATA[
	static CPCXStr DeclarationString;     // <?xml
	static CPCXStr XMLString;             // xml
	static CPCXStr DefaultDeclaration;    // version=1.0

	static CPCXStr CommentEndString;      // -->
	static CPCXStr CDataEndString;        // ]]>
	static CPCXStr DeclarationEndString;  // ?>

	static CPCXStr LessCharString;        // &lt;
	static CPCXStr GreaterCharString;     // &gt;
	static CPCXStr AndCharString;         // &amp;
	static CPCXStr SingleQuoteCharString; // &apos;
	static CPCXStr DoubleQuoteCharString; // &quot;
public:
	typedef CTList<CXMLNode*>   LST_XMLNODE;
public:
	bool IsSkipChar(XChar c);
	bool SkipChars(PXStr& pszBuf);

	bool ToName(PXStr& pszBuf, CString& strName);
	bool CheckName(PCXStr pszName);

	bool ToEntity(const CString& strValue, CString& strEntity) const;
	bool ToChars(CString& strValue);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXMLNode
class NOVTABLE CXMLNode ABSTRACT : public CXMLTraits
{
	friend class CXMLDocument;
public:
	XML_TYPE GetType(void) const;

	template <typename T, XML_TYPE eType> T* To(void);
	template <typename T, XML_TYPE eType> const T* To(void) const;
	// element : name of the element
	// comment : the comment text
	// text    : the text
	// unknown : the tag contents
	void      GetValue(CString& strValue);
	void      SetValue(PCXStr pszValue);

	bool      HasChild(void) const;
	bool      IsRootNode(void) const;
	bool      IsChildNode(void) const;
	// add new node at first/last, exist node just move to first/last
	bool      AddFirst(CXMLNode* pNode);
	bool      AddLast(CXMLNode* pNode);

	CXMLNode* RemoveFirst(void);
	CXMLNode* RemoveLast(void);
	bool      Remove(CXMLNode* pNode);
	void      RemoveAll(void);
	// add new node, exist node just move to new position
	bool      InsertBefore(CXMLNode* pNode, CXMLNode* pBefore);
	bool      InsertAfter(CXMLNode* pNode, CXMLNode* pAfter);
	// element : name of the element
	// comment : the comment text
	// text    : the text
	// unknown : the tag contents
	CXMLNode* Find(PCXStr pszValue, CXMLNode* pStartAfter = nullptr) const;

	CXMLDocument* GetDocument(void);
	const CXMLDocument* GetDocument(void) const;
	CXMLNode* GetParent(void);
	const   CXMLNode* GetParent(void) const;
	// first/last child node
	CXMLNode* GetFirst(void);
	const CXMLNode* GetFirst(void) const;
	CXMLNode* GetLast(void);
	const CXMLNode* GetLast(void) const;
	// prev/next sibling
	CXMLNode* GetPrev(void);
	const CXMLNode* GetPrev(void) const;
	CXMLNode* GetNext(void);
	const CXMLNode* GetNext(void) const;

	// clone only this node to aSrc document
	virtual CXMLNode* Clone(CXMLDocument& aSrc) const PURE_METHOD;
	// only test this node
	virtual bool Equal(const CXMLNode& aSrc) const PURE_METHOD;
	// store
	virtual void Store(CString& strBuf) const PURE_METHOD;
	virtual size_t Size(void) const PURE_METHOD;
protected:
	CXMLNode(CXMLDocument* pDocument, XML_TYPE eType);
	virtual ~CXMLNode(void);
	// ...
	virtual bool ParseNode(PXStr& pszBuf);
	// destroy this node & child node & attributes
	virtual void Destroy(bool bSelfOnly = false);
private:
	CXMLNode(const CXMLNode&);
	CXMLNode& operator=(const CXMLNode&);

	bool DetectNode(PXStr& pszBuf, CXMLNode*& pNode);
protected:
	XML_TYPE        m_eType;
	CXMLDocument*   m_pDocument;
	PINDEX          m_Index;
	//
	CXMLNode*       m_pParent;
	CXMLNode*       m_pFirst;
	CXMLNode*       m_pLast;
	CXMLNode*       m_pPrev;
	CXMLNode*       m_pNext;
	//
	size_t          m_stHash;
	CString         m_strValue;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXMLDocument
class CXMLDocument : public CXMLNode
{
public:
	CXMLDocument(void);
	~CXMLDocument(void);

	bool Load(CStream& stream, size_t stSize = 0);
	bool Save(CStream& stream, XML_ENCODING eENCODING = XML_ENCODING_UNICODE);

	CXMLElement*     CreateElement(PCXStr pszName);
	CXMLComment*     CreateComment(PCXStr pszComment);
	CXMLText*        CreateText(PCXStr pszText);
	CXMLDeclaration* CreateDeclaration(PCXStr pszDeclaration = DefaultDeclaration); 
	CXMLUnknown*     CreateUnknown(PCXStr pszUnknown);
	// destroy node , all children & attributes destroyed
	bool             Destroy(CXMLNode* pNode);

	virtual CXMLNode* Clone(CXMLDocument& aSrc) const OVERRIDE; // nullptr
	virtual bool Equal(const CXMLNode& aSrc) const OVERRIDE;    // false
	virtual void Store(CString& strBuf) const OVERRIDE; // ...
	virtual size_t Size(void) const OVERRIDE;
private:
	CXMLDocument(const CXMLDocument&);
	CXMLDocument& operator=(const CXMLDocument&);

	void DestroyNode(CXMLNode* pNode);
	void DestroyAll(void);

	bool Parse(PXStr pszBuf);
private:
	LST_XMLNODE   m_XMLNode;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXMLElement
class CXMLElement : public CXMLNode
{
	friend class CXMLDocument;
public:
	void GetName(CString& strName);
	void SetName(PCXStr pszName);
	// pszName & pszValue = nullptr check has any attribute
	// pszName != nullptr check the attribute is exist
	// pszName & pszValue != nullptr check the exact attribute is exist
	bool HasAttribute(PCXStr pszName = nullptr, PCXStr pszValue = nullptr) const;

	bool GetAttribute(PCXStr pszName, CString& strValue) const;
	bool GetAttribute(PCXStr pszName, Long& lValue) const;
	bool GetAttribute(PCXStr pszName, ULong& ulValue) const;
	bool GetAttribute(PCXStr pszName, LLong& llValue) const;
	bool GetAttribute(PCXStr pszName, ULLong& ullValue) const;
	bool GetAttribute(PCXStr pszName, Double& dValue) const;
	// set exist attribute or add new attribute at last
	bool SetAttribute(PCXStr pszName, PCXStr pszValue);
	bool SetAttribute(PCXStr pszName, Long lValue);
	bool SetAttribute(PCXStr pszName, ULong ulValue);
	bool SetAttribute(PCXStr pszName, LLong llValue);
	bool SetAttribute(PCXStr pszName, ULLong ullValue);
	bool SetAttribute(PCXStr pszName, Double dValue);
	// add new attribute, exist attribute just move to first/last position
	// exist attribute change value if pszValue != nullptr
	CXMLAttribute* AddFirstAttribute(PCXStr pszName, PCXStr pszValue = nullptr);
	CXMLAttribute* AddLastAttribute(PCXStr pszName, PCXStr pszValue = nullptr);
	bool MoveAttributeToFirst(CXMLAttribute* pAttr);
	bool MoveAttributeToLast(CXMLAttribute* pAttr);

	bool RemoveFirstAttribute(void);
	bool RemoveLastAttribute(void);
	bool RemoveAttribute(PCXStr pszName);
	bool RemoveAttribute(CXMLAttribute* pAttr);
	void RemoveAllAttribute(void);
	// add new attribute, exist attribute just move to new position
	// pszValue can be nullptr
	bool InsertAttributeBefore(PCXStr pszName, PCXStr pszValue, CXMLAttribute* pBefore);
	bool InsertAttributeAfter(PCXStr pszName, PCXStr pszValue, CXMLAttribute* pAfter);
	bool InsertAttributeBefore(CXMLAttribute* pAttr, CXMLAttribute* pBefore);
	bool InsertAttributeAfter(CXMLAttribute* pAttr, CXMLAttribute* pAfter);

	CXMLAttribute* FindAttribute(PCXStr pszName, CXMLAttribute* pStartAfter = nullptr) const;

	CXMLAttribute* GetFirstAttribute(void);
	const   CXMLAttribute* GetFirstAttribute(void) const;
	CXMLAttribute* GetLastAttribute(void);
	const   CXMLAttribute* GetLastAttribute(void) const;

	virtual CXMLNode* Clone(CXMLDocument& aSrc) const OVERRIDE;
	virtual bool Equal(const CXMLNode& aSrc) const OVERRIDE;
	virtual void Store(CString& strBuf) const OVERRIDE;
	virtual size_t Size(void) const OVERRIDE;
protected:
	CXMLElement(CXMLDocument* pDocument);
	virtual ~CXMLElement(void);

	CXMLAttribute* AddAttributeAtFirst(PCXStr pszName, PCXStr pszValue = nullptr);
	CXMLAttribute* AddAttributeAtLast(PCXStr pszName, PCXStr pszValue = nullptr);

	void RemoveAttributeList(CXMLAttribute* pAttr);
	bool ParseAttributes(PXStr& pszBuf);
	// ...
	virtual bool ParseNode(PXStr& pszBuf) OVERRIDE;
	virtual void Destroy(bool bSelfOnly = false) OVERRIDE;
private:
	CXMLElement(const CXMLElement&);
	CXMLElement& operator=(const CXMLElement&);
private:
	CXMLAttribute*   m_pFirstAttribute;
	CXMLAttribute*   m_pLastAttribute;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXMLComment
class CXMLComment : public CXMLNode
{
	friend class CXMLDocument;
public:
	virtual CXMLNode* Clone(CXMLDocument& aSrc) const OVERRIDE;
	virtual bool Equal(const CXMLNode& aSrc) const OVERRIDE;
	virtual void Store(CString& strBuf) const OVERRIDE;
	virtual size_t Size(void) const OVERRIDE;
protected:
	CXMLComment(CXMLDocument* pDocument);
	virtual ~CXMLComment(void);
	// ...
	virtual bool ParseNode(PXStr& pszBuf) OVERRIDE;
private:
	CXMLComment(const CXMLComment&);
	CXMLComment& operator=(const CXMLComment&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXMLText
class CXMLText : public CXMLNode
{
	friend class CXMLDocument;
public:
	bool   IsCData(void) const;
	void   SetCData(bool bCData);

	virtual CXMLNode* Clone(CXMLDocument& aSrc) const OVERRIDE;
	virtual bool Equal(const CXMLNode& aSrc) const OVERRIDE;
	virtual void Store(CString& strBuf) const OVERRIDE;
	virtual size_t Size(void) const OVERRIDE;
protected:
	CXMLText(CXMLDocument* pDocument);
	virtual ~CXMLText(void);
	// ...
	virtual bool ParseNode(PXStr& pszBuf) OVERRIDE;
private:
	CXMLText(const CXMLText&);
	CXMLText& operator=(const CXMLText&);
private:
	bool   m_bCData;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXMLDeclaration
class CXMLDeclaration : public CXMLNode
{
	friend class CXMLDocument;
public:
	virtual CXMLNode* Clone(CXMLDocument& aSrc) const OVERRIDE;
	virtual bool Equal(const CXMLNode& aSrc) const OVERRIDE;
	virtual void Store(CString& strBuf) const OVERRIDE;
	virtual size_t Size(void) const OVERRIDE;
protected:
	CXMLDeclaration(CXMLDocument* pDocument);
	virtual ~CXMLDeclaration(void);
	// ...
	virtual bool ParseNode(PXStr& pszBuf) OVERRIDE;
private:
	CXMLDeclaration(const CXMLDeclaration&);
	CXMLDeclaration& operator=(const CXMLDeclaration&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXMLUnknown
class CXMLUnknown : public CXMLNode
{
	friend class CXMLDocument;
public:
	bool   IsDtd(void) const;
	void   SetDtd(bool bDtd);

	virtual CXMLNode* Clone(CXMLDocument& aSrc) const OVERRIDE;
	virtual bool Equal(const CXMLNode& aSrc) const OVERRIDE;
	virtual void Store(CString& strBuf) const OVERRIDE;
	virtual size_t Size(void) const OVERRIDE;
protected:
	CXMLUnknown(CXMLDocument* pDocument);
	virtual ~CXMLUnknown(void);
	// ...
	virtual bool ParseNode(PXStr& pszBuf) OVERRIDE;
private:
	CXMLUnknown(const CXMLUnknown&);
	CXMLUnknown& operator=(const CXMLUnknown&);
private:
	bool   m_bDtd;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXMLAttribute
class CXMLAttribute : public CXMLTraits
{
    friend class CXMLElement;
public:
	CXMLElement* GetElement(void);
	const CXMLElement* GetElement(void) const;

	bool GetName(CString& strName);
	bool GetValue(CString& strValue);
	bool SetValue(PCXStr pszValue);

	bool GetValue(Long& lValue) const;
	bool GetValue(ULong& ulValue) const;
	bool GetValue(LLong& llValue) const;
	bool GetValue(ULLong& ullValue) const;
	bool GetValue(Double& dValue) const;
	bool SetValue(Long lValue);
	bool SetValue(ULong ulValue);
	bool SetValue(LLong llValue);
	bool SetValue(ULLong ullValue);
	bool SetValue(Double dValue);

	const CXMLAttribute* Prev(void) const;
	const CXMLAttribute* Next(void) const;

	void Store(CString& strBuf) const;
	size_t Size(void) const;
protected:
	CXMLAttribute(CXMLElement* pElement, PCXStr pszName, PCXStr pszValue);
	virtual ~CXMLAttribute(void);
private:
	CXMLAttribute(const CXMLAttribute&);
	CXMLAttribute& operator=(const CXMLAttribute&);
protected:
	CXMLElement*     m_pElement;
	size_t           m_stName;
	size_t           m_stValue;
	CXMLAttribute*   m_pPrev;
	CXMLAttribute*   m_pNext;

	CString          m_strName;
	CString          m_strValue;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "markup.inl"

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // __MARKUP_H__