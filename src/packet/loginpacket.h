//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : loginpacket.h                                //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : 登陆服务器信令                                     //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __LOGIN_PACKET_H__
#define __LOGIN_PACKET_H__

#pragma once

#include "commonpacket.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
enum LOGIN_ERROR
{
	LOGIN_ERROR_SESSION = (DATAD_FAIL + 1),   // session错误或者状态错误
	LOGIN_ERROR_ACK_TIMEOUT,                  // 请求超时

	LOGIN_ERROR_LINK_ID,
	LOGIN_ERROR_LINK_PW,

	LOGIN_ERROR_LINK_LIMIT,                   // 登陆验证超过最多次数, 一段时间内不能再登陆
	LOGIN_ERROR_SELECT_LIMIT,                 // 切换游戏服务器超过最多次数, 一段时间内不能再登陆
	LOGIN_ERROR_LINK_IDLE,                    // 登陆以后未进行任何操作, 登陆服务器自动关闭连接
	LOGIN_ERROR_GAME_PLAY,                    // 排队或者游戏中特定时间没有退出, 登陆服务器自动关闭连接
	LOGIN_ERROR_GAME_INDEX,                   // 游戏服务器索引号错误
	LOGIN_ERROR_NONE_GATE,                    // 没有网关服务器
	LOGIN_ERROR_QUEUE_LIMIT,                  // 游戏服务器排队队列已经满, 到达人数上限
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// login信令定义
enum PAK_EVENT_LOGIN
{
	PAK_EVENT_LOGIN_LINK = (PAK_EVENT_LOGIN_BEGIN + 1), // CPAKLoginLink
	PAK_EVENT_LOGIN_LINKACK, // CPAKLoginLinkAck or CPAKAck(error)

	PAK_EVENT_LOGIN_UNLINK,      // CPAKLoginUnlink
	PAK_EVENT_LOGIN_UNLINKACK,   // CPAKAck

	PAK_EVENT_LOGIN_SELECT_GAME, // 选择游戏服务器, CPAKLoginSelectGame or CPAKAck(error)

	PAK_EVENT_LOGIN_LINK_GAME,   // 连接游戏服务器, CPAKLoginLinkGame or CPAKAck(error)

	PAK_EVENT_LOGIN_QUEUE_PLAY,  // 排队或者进入游戏, CPAKSessionAck<PAK_EVENT_LOGIN_QUEUE_PLAY, PAK_TYPE_GATE>, ack=0加入, 1-退出
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPAKLoginLink : 账号登陆, session-id=0
class CPAKLoginLink : public CPAKSession
{
public:
	CPAKLoginLink(UInt uEvent = PAK_EVENT_LOGIN_LINK, UInt uType = PAK_TYPE_CLIENT_LOGIN);
	virtual ~CPAKLoginLink(void);

	CPAKLoginLink(const CPAKLoginLink& aSrc);
	CPAKLoginLink& operator=(const CPAKLoginLink& aSrc);

	virtual size_t Length(void) OVERRIDE;
	virtual void   Serialize(CStream&) OVERRIDE;

	CString&  GetUserId(void);
	void      SetUserId(CString& strUserId);
	void      SetUserId(PCXStr pszUserId);

	CString&  GetPassword(void);
	void      SetPassword(CString& strPassword);
	void      SetPassword(PCXStr pszPassword);
private:
	CString   m_strUserId;
	CString   m_strPassword;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPAKLoginLinkAck : 账号登陆Ack
class CPAKLoginLinkAck : public CPAKSessionAck
{
public:
	CPAKLoginLinkAck(UInt uEvent = PAK_EVENT_LOGIN_LINKACK, UInt uType = PAK_TYPE_LOGIN);
	virtual ~CPAKLoginLinkAck(void);

	CPAKLoginLinkAck(const CPAKLoginLinkAck& aSrc);
	CPAKLoginLinkAck& operator=(const CPAKLoginLinkAck& aSrc);

	virtual size_t Length(void) OVERRIDE;
	virtual void   Serialize(CStream&) OVERRIDE;

	LLong      GetUserId(void);
	void       SetUserId(LLong llUserId);

	LLong      GetTime(void);
	void       SetTime(LLong llTime);

	Int        GetGameId(void);
	void       SetGameId(Int nGameId);

	bool       CheckStream(void);
	CStream&   GetStream(void);
	void       SetStream(CStream& Stream);

	CNETTraits::NET_ADDR&  GetCltLast(void);
	void                   SetCltLast(CNETTraits::NET_ADDR& CltLast);
private:
	LLong                  m_llUserId;
	LLong                  m_llTime;     // last-client login time
	CStream*               m_pStream;
	Int                    m_nGameId;    // last game server id
	CNETTraits::NET_ADDR   m_Addr;       // last-client addr[ipv4 or ipv6]
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPAKLoginUnlink : 账号登出
class CPAKLoginUnlink : public CPAKSession
{
public:
	CPAKLoginUnlink(UInt uEvent = PAK_EVENT_LOGIN_UNLINK, UInt uType = PAK_TYPE_CLIENT_LOGIN);
	virtual ~CPAKLoginUnlink(void);

	CPAKLoginUnlink(const CPAKLoginUnlink& aSrc);
	CPAKLoginUnlink& operator=(const CPAKLoginUnlink& aSrc);

	virtual size_t Length(void) OVERRIDE;
	virtual void   Serialize(CStream&) OVERRIDE;

	LLong                  GetUserId(void);
	void                   SetUserId(LLong llUserId);

	LLong                  GetTime(void);
	void                   SetTime(LLong llTime);

	Int                    GetGameId(void);
	void                   SetGameId(Int nGameId);

	CNETTraits::NET_ADDR&  GetCltLast(void);
	void                   SetCltLast(CNETTraits::NET_ADDR& CltLast);
private:
	LLong                  m_llUserId;
	LLong                  m_llTime;
	Int                    m_nGameId;  // last game server id
	CNETTraits::NET_ADDR   m_Addr;     // last-client, last-server[ipv4 or ipv6]
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPAKLoginSelectGame : 选择游戏服务器
class CPAKLoginSelectGame : public CPAKSessionAck
{
public:
	CPAKLoginSelectGame(UInt uEvent = PAK_EVENT_LOGIN_SELECT_GAME, UInt uType = PAK_TYPE_CLIENT_LOGIN);
	virtual ~CPAKLoginSelectGame(void);

	CPAKLoginSelectGame(const CPAKLoginSelectGame& aSrc);
	CPAKLoginSelectGame& operator=(const CPAKLoginSelectGame& aSrc);

	virtual size_t Length(void) OVERRIDE;
	virtual void   Serialize(CStream&) OVERRIDE;

	LLong                  GetAuthCode(void);
	void                   SetAuthCode(LLong llAuthCode);

	CNETTraits::NET_ADDR&  GetAddr(void);
	void                   SetAddr(CNETTraits::NET_ADDR& Addr);
private:
	LLong                  m_llAuthCode;  // 客户端使用游戏DataRef(索引值)请求登陆游戏服务器, 登陆服务器经过游戏服务器验证以后返回验证码1
	CNETTraits::NET_ADDR   m_Addr;        // [ipv4 or ipv6]
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPAKLoginLinkGame : 验证游戏服务器
class CPAKLoginLinkGame : public CPAKSessionAck
{
public:
	CPAKLoginLinkGame(UInt uEvent = PAK_EVENT_LOGIN_LINK_GAME, UInt uType = PAK_TYPE_CLIENT);
	virtual ~CPAKLoginLinkGame(void);

	CPAKLoginLinkGame(const CPAKLoginLinkGame& aSrc);
	CPAKLoginLinkGame& operator=(const CPAKLoginLinkGame& aSrc);

	virtual size_t Length(void) OVERRIDE;
	virtual void   Serialize(CStream&) OVERRIDE;

	LLong          GetAuthCode(void);
	void           SetAuthCode(LLong llAuthCode);
private:
	// 1. session-id=0, authcode=(selectgame's m_llAuthCode); 2. session-id=user-id, authcode=(1 ret m_llAuthCode)
	// 3. ack=okay, ret session-id(暂时保留为user-id) & m_llAuthCode is queue number(0 --> into game)
	LLong         m_llAuthCode;  
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "loginpacket.inl"

#endif // __LOGIN_PACKET_H__