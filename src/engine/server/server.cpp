//=============================================================================//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
// server.cpp: implementation of the CServer class.
//
/////////////////////////////////////////////////////////////////////////////////
#include "core/stdafx.h"
#include "common/protocol.h"
#include "tier1/cvar.h"
#include "tier1/strtools.h"
#include "engine/server/sv_main.h"
#include "engine/server/server.h"
#include "networksystem/pylon.h"
#include "networksystem/bansystem.h"
#include "ebisusdk/EbisuSDK.h"
#include "public/edict.h"
#include "pluginsystem/pluginsystem.h"

//---------------------------------------------------------------------------------
// Purpose: Gets the number of human players on the server
// Output : int
//---------------------------------------------------------------------------------
int CServer::GetNumHumanPlayers(void) const
{
	int nHumans = 0;
	for (int i = 0; i < g_ServerGlobalVariables->m_nMaxClients; i++)
	{
		CClient* pClient = g_pServer->GetClient(i);
		if (!pClient)
			continue;

		if (pClient->IsHumanPlayer())
			nHumans++;
	}

	return nHumans;
}

//---------------------------------------------------------------------------------
// Purpose: Gets the number of fake clients on the server
// Output : int
//---------------------------------------------------------------------------------
int CServer::GetNumFakeClients(void) const
{
	int nBots = 0;
	for (int i = 0; i < g_ServerGlobalVariables->m_nMaxClients; i++)
	{
		CClient* pClient = g_pServer->GetClient(i);
		if (!pClient)
			continue;

		if (pClient->IsConnected() && pClient->IsFakeClient())
			nBots++;
	}

	return nBots;
}

//---------------------------------------------------------------------------------
// Purpose: Gets the number of clients on the server
// Output : int
//---------------------------------------------------------------------------------
int CServer::GetNumClients(void) const
{
	int nClients = 0;
	for (int i = 0; i < g_ServerGlobalVariables->m_nMaxClients; i++)
	{
		CClient* pClient = g_pServer->GetClient(i);
		if (!pClient)
			continue;

		if (pClient->IsConnected())
			nClients++;
	}

	return nClients;
}

//---------------------------------------------------------------------------------
// Purpose: Initializes a CSVClient for a new net connection. This will only be called
//			once for a player each game, not once for each level change.
// Input  : *pServer - 
//			*pChallenge - 
// Output : pointer to client instance on success, nullptr on failure
//---------------------------------------------------------------------------------
CClient* CServer::ConnectClient(CServer* pServer, user_creds_s* pChallenge)
{
	if (pServer->m_State < server_state_t::ss_active)
		return nullptr;

	char* pszPersonaName = pChallenge->personaName;
	uint64_t nNucleusID = pChallenge->personaId;

	char pszAddresBuffer[128]; // Render the client's address.
	pChallenge->netAdr.ToString(pszAddresBuffer, sizeof(pszAddresBuffer), true);

	const bool bEnableLogging = sv_showconnecting->GetBool();
	const int nPort = int(ntohs(pChallenge->netAdr.GetPort()));

	if (bEnableLogging)
		Msg(eDLL_T::SERVER, "Processing connectionless challenge for '[%s]:%i' ('%llu')\n",
			pszAddresBuffer, nPort, nNucleusID);

	bool bValidName = false;

	if (VALID_CHARSTAR(pszPersonaName) &&
		V_IsValidUTF8(pszPersonaName))
	{
		if (sv_validatePersonaName->GetBool() && 
			!IsValidPersonaName(pszPersonaName, sv_minPersonaNameLength->GetInt(), sv_maxPersonaNameLength->GetInt()))
		{
			bValidName = false;
		}
		else
		{
			bValidName = true;
		}
	}

	// Only proceed connection if the client's name is valid and UTF-8 encoded.
	if (!bValidName)
	{
		pServer->RejectConnection(pServer->m_Socket, &pChallenge->netAdr, "#Valve_Reject_Invalid_Name");
		if (bEnableLogging)
			Warning(eDLL_T::SERVER, "Connection rejected for '[%s]:%i' ('%llu' has an invalid name!)\n",
				pszAddresBuffer, nPort, nNucleusID);

		return nullptr;
	}

	if (g_pBanSystem->IsBanListValid())
	{
		if (g_pBanSystem->IsBanned(pszAddresBuffer, nNucleusID))
		{
			pServer->RejectConnection(pServer->m_Socket, &pChallenge->netAdr, "#Valve_Reject_Banned");
			if (bEnableLogging)
				Warning(eDLL_T::SERVER, "Connection rejected for '[%s]:%i' ('%llu' is banned from this server!)\n",
					pszAddresBuffer, nPort, nNucleusID);

			return nullptr;
		}
	}

	CClient* pClient = v_CServer_ConnectClient(pServer, pChallenge);

	for (auto& callback : !g_pPluginSystem->GetConnectClientCallbacks())
	{
		if (!callback(pServer, pClient, pChallenge))
		{
			pClient->Disconnect(REP_MARK_BAD, "#Valve_Reject_Banned");
			return nullptr;
		}
	}

	if (pClient && sv_globalBanlist->GetBool())
	{
		if (!pClient->GetNetChan()->GetRemoteAddress().IsLoopback())
		{
			std::thread th(SV_IsClientBanned, pClient, string(pszAddresBuffer), nNucleusID, string(pszPersonaName), nPort);
			th.detach();
		}
	}

	return pClient;
}

//---------------------------------------------------------------------------------
// Purpose: Rejects connection request and sends back a message
// Input  : iSocket - 
//			*pChallenge - 
//			*szMessage - 
//---------------------------------------------------------------------------------
void CServer::RejectConnection(int iSocket, netadr_t* pNetAdr, const char* szMessage)
{
	v_CServer_RejectConnection(this, iSocket, pNetAdr, szMessage);
}

//---------------------------------------------------------------------------------
// Purpose: Runs the server frame job
// Input  : flFrameTime - 
//			bRunOverlays - 
//			bUniformSnapshotInterval - 
//---------------------------------------------------------------------------------
void CServer::FrameJob(double flFrameTime, bool bRunOverlays, bool bUniformSnapshotInterval)
{
	v_CServer_FrameJob(flFrameTime, bRunOverlays, bUniformSnapshotInterval);
}

//---------------------------------------------------------------------------------
// Purpose: Runs the server frame
// Input  : *pServer - 
//---------------------------------------------------------------------------------
void CServer::RunFrame(CServer* pServer)
{
	v_CServer_RunFrame(pServer);
}

///////////////////////////////////////////////////////////////////////////////
void VServer::Detour(const bool bAttach) const
{
	DetourSetup(&v_CServer_RunFrame, &CServer::RunFrame, bAttach);
#if	defined(GAMEDLL_S3)
	DetourSetup(&v_CServer_ConnectClient, &CServer::ConnectClient, bAttach);
	DetourSetup(&v_CServer_FrameJob, &CServer::FrameJob, bAttach);
#endif // !TODO: S1 and S2 CServer functions require work.
}


///////////////////////////////////////////////////////////////////////////////
CServer* g_pServer = nullptr;