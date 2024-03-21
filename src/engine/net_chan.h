//=============================================================================//
// 
// Purpose: 
// 
//=============================================================================//

#ifndef NET_CHAN_H
#define NET_CHAN_H

#include "tier1/bitbuf.h"
#include "tier1/NetAdr.h"
#include "tier1/NetKey.h"
#include "tier1/utlmemory.h"
#include "tier1/utlvector.h"
#include "common/netmessages.h"
#include "common/protocol.h"
#include "public/inetchannel.h"

#define NET_FRAMES_BACKUP 128
#define NET_UNRELIABLE_STREAM_MINSIZE 256
#define NET_CHANNELNAME_MAXLEN 32
#define NET_FRAMES_MASK   (NET_FRAMES_BACKUP-1)

//-----------------------------------------------------------------------------
// Purpose: forward declarations
//-----------------------------------------------------------------------------
class CClient;
class CNetChan;

//-----------------------------------------------------------------------------
typedef struct netframe_header_s
{
	float time;
	int size;
	short choked;
	bool valid;
	float latency;
} netframe_header_t;

typedef struct netframe_s
{
	int dropped;
	float avg_latency;
} netframe_t;

//-----------------------------------------------------------------------------
typedef struct netflow_s
{
	float nextcompute;
	float avgbytespersec;
	float avgpacketspersec;
	float avgloss;
	float avgchoke;
	float avglatency;
	float latency;
	float maxlatency;
	int64_t totalpackets;
	int64_t totalbytes;
	int64_t totalupdates;
	int currentindex;
	netframe_header_t frame_headers[NET_FRAMES_BACKUP];
	netframe_t frames[NET_FRAMES_BACKUP];
	netframe_t* current_frame;
} netflow_t;

//-----------------------------------------------------------------------------
struct dataFragments_t
{
	char* buffer;
	int64_t blockSize;
	bool isCompressed;
	uint8_t gap11[7];
	int64_t uncompressedSize;
	bool firstFragment;
	bool lastFragment;
	bool isOutbound;
	int transferID;
	int transferSize;
	int currentOffset;
};

//-----------------------------------------------------------------------------
enum EBufType
{
	BUF_RELIABLE = 0,
	BUF_UNRELIABLE,
	BUF_VOICE
};

inline CMemory p_NetChan_Clear;
inline void(*v_NetChan_Clear)(CNetChan* pChan, bool bStopProcessing);

inline CMemory p_NetChan_Shutdown;
inline void(*v_NetChan_Shutdown)(CNetChan* pChan, const char* szReason, uint8_t bBadRep, bool bRemoveNow);

inline CMemory p_NetChan_CanPacket;
inline bool(*v_NetChan_CanPacket)(const CNetChan* pChan);

inline CMemory p_NetChan_FlowNewPacket;
inline void(*v_NetChan_FlowNewPacket)(CNetChan* pChan, int flow, int outSeqNr, int inSeqNr, int nChoked, int nDropped, int nSize);

inline CMemory p_NetChan_SendDatagram;
inline int(*v_NetChan_SendDatagram)(CNetChan* pChan, bf_write* pMsg);

inline CMemory p_NetChan_ProcessMessages;
inline bool(*v_NetChan_ProcessMessages)(CNetChan* pChan, bf_read* pMsg);

//-----------------------------------------------------------------------------
class CNetChan
{
public:
	~CNetChan()
	{
		Shutdown("NetChannel removed.", 1, false);
		FreeReceiveList();
	}

	inline const char* GetName(void)                     const { return m_Name; }
	inline const char* GetAddress(bool onlyBase = false) const { return remote_address.ToString(onlyBase); }
	inline int         GetPort(void)                     const { return int(ntohs(remote_address.GetPort())); }
	inline int         GetDataRate(void)                 const { return m_Rate; }
	inline int         GetBufferSize(void)               const { return NET_FRAMES_BACKUP; }

	float        GetNetworkLoss() const;

	inline float GetLatency(int flow)        const { Assert(flow >= 0 && flow < SDK_ARRAYSIZE(m_DataFlow)); return m_DataFlow[flow].latency; }
	inline float GetAvgChoke(int flow)       const { Assert(flow >= 0 && flow < SDK_ARRAYSIZE(m_DataFlow)); return m_DataFlow[flow].avgchoke; }
	inline float GetAvgLatency(int flow)     const { Assert(flow >= 0 && flow < SDK_ARRAYSIZE(m_DataFlow)); return m_DataFlow[flow].avglatency; }
	inline float GetAvgLoss(int flow)        const { Assert(flow >= 0 && flow < SDK_ARRAYSIZE(m_DataFlow)); return m_DataFlow[flow].avgloss; }
	inline float GetAvgPackets(int flow)     const { Assert(flow >= 0 && flow < SDK_ARRAYSIZE(m_DataFlow)); return m_DataFlow[flow].avgpacketspersec; }
	inline float GetAvgData(int flow)        const { Assert(flow >= 0 && flow < SDK_ARRAYSIZE(m_DataFlow)); return m_DataFlow[flow].avgbytespersec; }
	inline int64_t GetTotalData(int flow)    const { Assert(flow >= 0 && flow < SDK_ARRAYSIZE(m_DataFlow)); return m_DataFlow[flow].totalbytes; }
	inline int64_t GetTotalPackets(int flow) const { Assert(flow >= 0 && flow < SDK_ARRAYSIZE(m_DataFlow)); return m_DataFlow[flow].totalpackets; }

	int         GetSequenceNr(int flow) const;
	double      GetTimeConnected(void) const;

	inline float GetTimeoutSeconds(void)          const { return m_Timeout; }
	inline int   GetSocket(void)                  const { return m_Socket; }
	inline const bf_write& GetStreamVoice(void)   const { return m_StreamVoice; }
	inline const netadr_t& GetRemoteAddress(void) const { return remote_address; }
	inline bool IsOverflowed(void)                const { return m_StreamReliable.IsOverflowed(); }

	bool HasPendingReliableData(void);

	inline bool CanPacket(void) const { return v_NetChan_CanPacket(this); }
	inline int SendDatagram(bf_write* pDatagram) { return v_NetChan_SendDatagram(this, pDatagram); }
	bool SendNetMsg(INetMessage& msg, bool bForceReliable, bool bVoice);

	INetMessage* FindMessage(int type);
	bool RegisterMessage(INetMessage* msg);

	inline void Clear(bool bStopProcessing) { v_NetChan_Clear(this, bStopProcessing); }
	inline void Shutdown(const char* szReason, uint8_t bBadRep, bool bRemoveNow) { v_NetChan_Shutdown(this, szReason, bBadRep, bRemoveNow); }
	void FreeReceiveList();
	bool ProcessMessages(bf_read* pMsg);

	static void _Shutdown(CNetChan* pChan, const char* szReason, uint8_t bBadRep, bool bRemoveNow);
	static bool _ProcessMessages(CNetChan* pChan, bf_read* pMsg);

	static void _FlowNewPacket(CNetChan* pChan, int flow, int outSeqNr, int inSeqNr, int nChoked, int nDropped, int nSize);

	void SetChoked();
	void SetRemoteFramerate(float flFrameTime, float flFrameTimeStdDeviation);
	inline void SetRemoteCPUStatistics(uint8_t nStats) { m_nServerCPU = nStats; }

	//-----------------------------------------------------------------------------
public:
	bool                m_bProcessingMessages;
	bool                m_bShouldDelete;
	bool                m_bStopProcessing;
	bool                m_bShuttingDown;
	int                 m_nOutSequenceNr;
	int                 m_nInSequenceNr;
	int                 m_nOutSequenceNrAck;
	int                 m_nChokedPackets;
	int                 unknown_challenge_var;

private:
#if defined (GAMEDLL_S0) || defined (GAMEDLL_S1) || defined (GAMEDLL_S2)
	char                pad[8];
#endif
	int                 m_nLastRecvFlags;
	RTL_SRWLOCK         m_Lock;
	bf_write            m_StreamReliable;
	CUtlMemory<byte>    m_ReliableDataBuffer;
	bf_write            m_StreamUnreliable;
	CUtlMemory<byte>    m_UnreliableDataBuffer;
	bf_write            m_StreamVoice;
	CUtlMemory<byte>    m_VoiceDataBuffer;
	int                 m_Socket;
	int                 m_MaxReliablePayloadSize;
	double              last_received;
	double              connect_time;
	uint32_t            m_Rate;
	int                 padding_maybe;
	double              m_fClearTime;
	CUtlVector<dataFragments_t*> m_WaitingList;
	dataFragments_t     m_ReceiveList;
	int                 m_nSubOutFragmentsAck;
	int                 m_nSubInFragments;
	int                 m_nNonceHost;
	uint32_t            m_nNonceRemote;
	bool                m_bReceivedRemoteNonce;
	bool                m_bInReliableState;
	bool                m_bPendingRemoteNonceAck;
	uint32_t            m_nSubOutSequenceNr;
	int                 m_nLastRecvNonce;
	bool                m_bUseCompression;
	uint32_t            m_ChallengeNr;
	float               m_Timeout;
	INetChannelHandler* m_MessageHandler;
	CUtlVector<INetMessage*> m_NetMessages;
	void*               m_UnusedInterfacePointer; // Previously: IDemoRecorder* m_DemoRecorder.
	int                 m_nQueuedPackets;
	float               m_flRemoteFrameTime;
	float               m_flRemoteFrameTimeStdDeviation;
	uint8_t             m_nServerCPU;
	int                 m_nMaxRoutablePayloadSize;
	int                 m_nSplitPacketSequence;
	int64_t             m_StreamSendBuffer;
	bf_write            m_StreamSend;
	uint8_t             m_bInMatch_maybe;
	netflow_t           m_DataFlow[MAX_FLOWS];
	int                 m_nLifetimePacketsDropped;
	int                 m_nSessionPacketsDropped;
	int                 m_nSequencesSkipped_MAYBE;
	int                 m_nSessionRecvs;
	uint32_t            m_nLiftimeRecvs;
	bool                m_bRetrySendLong;
	char                m_Name[NET_CHANNELNAME_MAXLEN];
	netadr_t            remote_address;
};
#if defined (GAMEDLL_S0) || defined (GAMEDLL_S1) || defined (GAMEDLL_S2)
static_assert(sizeof(CNetChan) == 0x1AD0);
#else
static_assert(sizeof(CNetChan) == 0x1AC8);
#endif

//-----------------------------------------------------------------------------
// Purpose: sets the remote frame times
// Input  : flFrameTime - 
//			flFrameTimeStdDeviation - 
//-----------------------------------------------------------------------------
inline void CNetChan::SetRemoteFramerate(float flFrameTime, float flFrameTimeStdDeviation)
{
	m_flRemoteFrameTime = flFrameTime;
	m_flRemoteFrameTimeStdDeviation = flFrameTimeStdDeviation;
}

//-----------------------------------------------------------------------------
// Purpose: increments choked packet count
//-----------------------------------------------------------------------------
inline void CNetChan::SetChoked(void)
{
	m_nOutSequenceNr++; // Sends to be done since move command use sequence number.
	m_nChokedPackets++;
}


///////////////////////////////////////////////////////////////////////////////
class VNetChan : public IDetour
{
	virtual void GetAdr(void) const
	{
		LogFunAdr("CNetChan::Clear", p_NetChan_Clear.GetPtr());
		LogFunAdr("CNetChan::Shutdown", p_NetChan_Shutdown.GetPtr());
		LogFunAdr("CNetChan::CanPacket", p_NetChan_CanPacket.GetPtr());
		LogFunAdr("CNetChan::FlowNewPacket", p_NetChan_FlowNewPacket.GetPtr());
		LogFunAdr("CNetChan::SendDatagram", p_NetChan_SendDatagram.GetPtr());
		LogFunAdr("CNetChan::ProcessMessages", p_NetChan_ProcessMessages.GetPtr());
	}
	virtual void GetFun(void) const
	{
		p_NetChan_Clear = g_GameDll.FindPatternSIMD("88 54 24 10 53 55 57");
		v_NetChan_Clear = p_NetChan_Clear.RCast<void (*)(CNetChan*, bool)>();

		p_NetChan_Shutdown = g_GameDll.FindPatternSIMD("48 89 6C 24 18 56 57 41 56 48 83 EC 30 83 B9");
		v_NetChan_Shutdown = p_NetChan_Shutdown.RCast<void (*)(CNetChan*, const char*, uint8_t, bool)>();

		p_NetChan_CanPacket = g_GameDll.FindPatternSIMD("40 53 48 83 EC 20 83 B9 ?? ?? ?? ?? ?? 48 8B D9 75 15 48 8B 05 ?? ?? ?? ??");
		v_NetChan_CanPacket = p_NetChan_CanPacket.RCast<bool (*)(const CNetChan*)>();

		p_NetChan_FlowNewPacket = g_GameDll.FindPatternSIMD("44 89 4C 24 ?? 44 89 44 24 ?? 89 54 24 10 56");
		v_NetChan_FlowNewPacket = p_NetChan_FlowNewPacket.RCast<void (*)(CNetChan*, int, int, int, int, int, int)>();

		p_NetChan_SendDatagram = g_GameDll.FindPatternSIMD("48 89 5C 24 ?? 55 56 57 41 56 41 57 48 83 EC 70");
		v_NetChan_SendDatagram = p_NetChan_SendDatagram.RCast<int (*)(CNetChan*, bf_write*)>();

		p_NetChan_ProcessMessages = g_GameDll.FindPatternSIMD("48 89 5C 24 ?? 48 89 6C 24 ?? 57 48 81 EC ?? ?? ?? ?? 48 8B FA");
		v_NetChan_ProcessMessages = p_NetChan_ProcessMessages.RCast<bool (*)(CNetChan*, bf_read*)>();
	}
	virtual void GetVar(void) const { }
	virtual void GetCon(void) const { }
	virtual void Detour(const bool bAttach) const;
};
///////////////////////////////////////////////////////////////////////////////

#endif // NET_CHAN_H
