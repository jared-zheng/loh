//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : loginpacket.h                                //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
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
	LOGIN_ERROR_SESSION = (DATAD_FAIL + 1),   // 会话错误或者状态错误
	LOGIN_ERROR_ACK_TIMEOUT,                  // 请求超时

	LOGIN_ERROR_LINK_ID,
	LOGIN_ERROR_LINK_PW,

	LOGIN_ERROR_LINK_LIMIT,                   // 登陆验证超过最多次数, 一段时间内不能再登陆
	LOGIN_ERROR_SELECT_LIMIT,                 // 切换游戏服务器超过最多次数, 一段时间内不能再登陆
	LOGIN_ERROR_LINK_IDLE,                    // 登陆以后未进行任何操作, 登陆服务器自动断开客户端
	LOGIN_ERROR_GAME_PLAY,                    // 排队或者游戏中特定时间没有退出, 登陆服务器自动断开客户端
	LOGIN_ERROR_GAME_INDEX,                   // 游戏服务器索引号错误
	LOGIN_ERROR_NONE_GATE,                    // 没有网关服务器
	LOGIN_ERROR_QUEUE_LIMIT,                  // 游戏服务器排队队列已经满, 到达人数上限
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// 登陆服务器信令定义
enum PAK_EVENT_LOGIN
{
	PAK_EVENT_LOGIN_LINK = (PAK_EVENT_LOGIN_BEGIN + 1), // CPAKLoginLink
	PAK_EVENT_LOGIN_LINKACK, // CPAKLoginLinkAck or CPAKAck(error)

	PAK_EVENT_LOGIN_UNLINK,      // CPAKLoginUnlink
	PAK_EVENT_LOGIN_UNLINKACK,   // CPAKAck

	PAK_EVENT_LOGIN_SELECT_GAME, // 选择游戏服务器, CPAKLoginSelectGame or CPAKAck(error)

	PAK_EVENT_LOGIN_LINK_GAME,   // 游戏服务器验证客户端有效性, CPAKLoginLinkGame or CPAKAck(error)

	PAK_EVENT_LOGIN_QUEUE_PLAY,  // 排队或者进入游戏, CPAKSessionAck<PAK_EVENT_LOGIN_QUEUE_PLAY, PAK_TYPE_GATE>, ack=0加入, 1-退出
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 账号登陆, session-id=0
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
/// 账号登陆Ack
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
	LLong                  m_llUserId;   ///< 用户Id
	LLong                  m_llTime;     ///< 最近一次登陆的时间
	CStream*               m_pStream;    ///< 本地流对象, 网络传输序列化时忽略大小
	Int                    m_nGameId;    ///< 最近一次连接游戏服务器Id
	CNETTraits::NET_ADDR   m_Addr;       ///< 最近一次登陆的客户端地址
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 账号登出
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
	LLong                  m_llUserId; ///< 用户Id
	LLong                  m_llTime;   ///< 本次登陆成功的时间(由登陆服务器填写)
	Int                    m_nGameId;  ///< 本次登陆以后连接的游戏服务器Id
	CNETTraits::NET_ADDR   m_Addr;     ///< 本次登陆客户端地址
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 选择游戏服务器
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
	LLong                  m_llAuthCode;  ///< 客户端使用游戏对象索引值请求连接游戏服务器, 成功返回验证码1
	CNETTraits::NET_ADDR   m_Addr;        ///< 各自服务器地址 : 登录服务器填写自己的UDP地址, 应答的网关服务器填写自己的UDP地址
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 游戏服务器验证客户端是否合法连接
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
	///< 1. 第一次Session为空, 验证码为前面登陆服务器返回的验证码1
	///< 2. 第一次验证通过, 游戏服务器将验证码2通过客户端连接的登陆服务器返回, 第二次验证Session=用户Id, 验证码为返回的验证码2
	///< 3. 第二次验证通过  游戏服务器返回的AuthCode是客户端在此游戏服务器的排队序号, 序号为0表示直接可以进入游戏(和游戏服务器通信的Session暂时为用户Id)
	LLong         m_llAuthCode;  
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "loginpacket.inl"

#endif // __LOGIN_PACKET_H__