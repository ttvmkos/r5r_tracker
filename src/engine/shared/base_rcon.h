#ifndef BASE_RCON_H
#define BASE_RCON_H

#include "tier1/NetAdr.h"
#include "tier2/socketcreator.h"
#include "protobuf/message_lite.h"

class CNetConBase
{
public:
	CNetConBase(void)
	{}

	virtual bool Connect(const char* pHostAdr, const int nHostPort = SOCKET_ERROR);
	virtual void Disconnect(const char* szReason = nullptr) { NOTE_UNUSED(szReason); };

	virtual bool ProcessBuffer(CConnectedNetConsoleData& data, const char* pRecvBuf, int nRecvLen, const int nMaxLen = SOCKET_ERROR);
	virtual bool ProcessMessage(const char* /*pMsgBuf*/, int /*nMsgLen*/) { return true; };

	virtual bool Encode(google::protobuf::MessageLite* pMsg, char* pMsgBuf, const size_t nMsgLen) const;
	virtual bool Decode(google::protobuf::MessageLite* pMsg, const char* pMsgBuf, const size_t nMsgLen) const;

	virtual bool Send(const SocketHandle_t hSocket, const char* pMsgBuf, const int nMsgLen) const;
	virtual void Recv(CConnectedNetConsoleData& data, const int nMaxLen = SOCKET_ERROR);

	CSocketCreator* GetSocketCreator(void) { return &m_Socket; }
	netadr_t* GetNetAddress(void) { return &m_Address; }

protected:
	CSocketCreator m_Socket;
	netadr_t m_Address;
};

#endif // BASE_RCON_H
