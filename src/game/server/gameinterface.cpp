//=============================================================================//
//
// Purpose: Implement things from GameInterface.cpp. Mostly the engine interfaces.
//
// $NoKeywords: $
//=============================================================================//

#include "core/stdafx.h"
#include "tier1/cvar.h"
#include "public/server_class.h"
#include "public/eiface.h"
#include "public/const.h"
#include "common/protocol.h"
#include "engine/server/sv_main.h"
#include "gameinterface.h"
#include "entitylist.h"
#include "baseanimating.h"
#include "engine/server/server.h"
#include "game/shared/usercmd.h"
#include "game/server/util_server.h"

//-----------------------------------------------------------------------------
// This is called when a new game is started. (restart, map)
//-----------------------------------------------------------------------------
void CServerGameDLL::GameInit(void)
{
	const static int index = 1;
	CallVFunc<void>(index, this);
}

//-----------------------------------------------------------------------------
// This is called when scripts are getting recompiled. (restart, map, changelevel)
//-----------------------------------------------------------------------------
void CServerGameDLL::PrecompileScriptsJob(void)
{
	const static int index = 2;
	CallVFunc<void>(index, this);
}

//-----------------------------------------------------------------------------
// Called when a level is shutdown (including changing levels)
//-----------------------------------------------------------------------------
void CServerGameDLL::LevelShutdown(void)
{
	const static int index = 8;
	CallVFunc<void>(index, this);
}

//-----------------------------------------------------------------------------
// This is called when a game ends (server disconnect, death, restart, load)
// NOT on level transitions within a game
//-----------------------------------------------------------------------------
void CServerGameDLL::GameShutdown(void)
{
	// Game just calls a nullsub for GameShutdown lol.
	const static int index = 9;
	CallVFunc<void>(index, this);
}

//-----------------------------------------------------------------------------
// Purpose: Gets the simulation tick interval
// Output : float
//-----------------------------------------------------------------------------
float CServerGameDLL::GetTickInterval(void)
{
	const static int index = 11;
	return CallVFunc<float>(index, this);
}

//-----------------------------------------------------------------------------
// Purpose: get all server classes
// Output : ServerClass*
//-----------------------------------------------------------------------------
ServerClass* CServerGameDLL::GetAllServerClasses(void)
{
	const static int index = 12;
	return CallVFunc<ServerClass*>(index, this);
}

void __fastcall CServerGameDLL::OnReceivedSayTextMessage(void* thisptr, int senderId, const char* text, bool isTeamChat)
{
#if defined(GAMEDLL_S3)
	// set isTeamChat to false so that we can let the convar sv_forceChatToTeamOnly decide whether team chat should be enforced
	// this isn't a great way of doing it but it works so meh
	CServerGameDLL__OnReceivedSayTextMessage(thisptr, senderId, text, false);
#endif
}

void DrawServerHitbox(int iEntity)
{
	IHandleEntity* pEntity = LookupEntityByIndex(iEntity);
	CBaseAnimating* pAnimating = dynamic_cast<CBaseAnimating*>(pEntity);

	if (pAnimating)
	{
		pAnimating->DrawServerHitboxes();
	}
}

void DrawServerHitboxes(bool bRunOverlays)
{
	int nVal = sv_showhitboxes->GetInt();
	Assert(nVal < NUM_ENT_ENTRIES);

	if (nVal == -1)
		return;

	if (nVal == 0)
	{
		for (int i = 0; i < NUM_ENT_ENTRIES; i++)
		{
			DrawServerHitbox(i);
		}
	}
	else // Lookup entity manually by index from 'sv_showhitboxes'.
	{
		DrawServerHitbox(nVal);
	}
}

void CServerGameClients::ProcessUserCmds(CServerGameClients* thisp, edict_t edict,
	bf_read* buf, int numCmds, int totalCmds, int droppedPackets, bool ignore, bool paused)
{
	int i;
	CUserCmd* from, * to;

	// We track last three command in case we drop some
	// packets but get them back.
	CUserCmd cmds[MAX_BACKUP_COMMANDS_PROCESS];
	CUserCmd cmdNull;  // For delta compression

	Assert(numCmds >= 0);
	Assert((totalCmds - numCmds) >= 0);

	CPlayer* pPlayer = UTIL_PlayerByIndex(edict);

	// Too many commands?
	if (totalCmds < 0 || totalCmds >= (MAX_BACKUP_COMMANDS_PROCESS - 1) ||
		numCmds < 0 || numCmds > totalCmds)
	{
		CClient* pClient = g_pServer->GetClient(edict-1);

		Warning(eDLL_T::SERVER, "%s: Player '%s' sent too many cmds (%i)\n", __FUNCTION__, pClient->GetServerName(), totalCmds);
		buf->SetOverflowFlag();

		return;
	}

	from = &cmdNull;
	for (i = totalCmds - 1; i >= 0; i--)
	{
		to = &cmds[i];
		ReadUserCmd(buf, to, from);
		from = to;
	}

	// Client not fully connected or server has gone inactive or is paused, just ignore
	if (ignore || !pPlayer)
	{
		return;
	}

	pPlayer->ProcessUserCmds(cmds, numCmds, totalCmds, droppedPackets, paused);
}

void RunFrameServer(double flFrameTime, bool bRunOverlays, bool bUniformUpdate)
{
	DrawServerHitboxes(bRunOverlays);
	v_RunFrameServer(flFrameTime, bRunOverlays, bUniformUpdate);
}

void VServerGameDLL::Detour(const bool bAttach) const
{
#if defined(GAMEDLL_S3)
	DetourSetup(&CServerGameDLL__OnReceivedSayTextMessage, &CServerGameDLL::OnReceivedSayTextMessage, bAttach);
	DetourSetup(&v_CServerGameClients__ProcessUserCmds, CServerGameClients::ProcessUserCmds, bAttach);
#endif
	DetourSetup(&v_RunFrameServer, &RunFrameServer, bAttach);
}

CServerGameDLL* g_pServerGameDLL = nullptr;
CServerGameClients* g_pServerGameClients = nullptr;
CServerGameEnts* g_pServerGameEntities = nullptr;

// Holds global variables shared between engine and game.
CGlobalVars** g_pGlobals = nullptr;
