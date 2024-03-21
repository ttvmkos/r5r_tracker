#include "core/stdafx.h"
#include "tier0/commandline.h"
#include "host_cmd.h"
#include "common.h"
#include "client/client.h"
#ifndef DEDICATED
#include "windows/id3dx.h"
#endif // !DEDICATED

/*
==================
Host_Shutdown

 shutdown host
 systems
==================
*/
void Host_Shutdown()
{
#ifndef DEDICATED
	DirectX_Shutdown();
#endif // DEDICATED
	v_Host_Shutdown();
}

/*
==================
Host_Status_PrintClient

 Print client info 
 to console
==================
*/
void Host_Status_PrintClient(CClient* client, bool bShowAddress, void (*print) (const char* fmt, ...))
{
	CNetChan* nci = client->GetNetChan();
	const char* state = "challenging";

	if (client->IsActive())
		state = "active";
	else if (client->IsSpawned())
		state = "spawning";
	else if (client->IsConnected())
		state = "connecting";

	if (nci != NULL)
	{
		print("# %i \"%s\" %llu %s %i %i %s %d\n",
			client->GetHandle(), client->GetServerName(), client->GetNucleusID(), COM_FormatSeconds(static_cast<int>(nci->GetTimeConnected())),
			static_cast<int>(1000.0f * nci->GetAvgLatency(FLOW_OUTGOING)), static_cast<int>(100.0f * nci->GetAvgLoss(FLOW_INCOMING)), state, nci->GetDataRate());

		if (bShowAddress)
		{
			print(" %s\n", nci->GetAddress());
		}
	}
	else
	{
		print("#%2i \"%s\" %llu %s\n", client->GetHandle(), client->GetServerName(), client->GetNucleusID(), state);
	}

	//print("\n");
}

#if !defined (GAMEDLL_S0) && !defined (GAMEDLL_S1) && !defined (GAMEDLL_S2)
/*
==================
DFS_InitializeFeatureFlagDefinitions

 Initialize feature
 flag definitions
==================
*/
bool DFS_InitializeFeatureFlagDefinitions(const char* pszFeatureFlags)
{
	if (CommandLine()->CheckParm("-nodfs"))
		return false;

	return v_DFS_InitializeFeatureFlagDefinitions(pszFeatureFlags);
}
#endif // !(GAMEDLL_S0) || !(GAMEDLL_S1) || !(GAMEDLL_S2)

///////////////////////////////////////////////////////////////////////////////
void VHostCmd::Detour(const bool bAttach) const
{
	DetourSetup(&v_Host_Shutdown, &Host_Shutdown, bAttach);
	DetourSetup(&v_Host_Status_PrintClient, &Host_Status_PrintClient, bAttach);
#if !defined (GAMEDLL_S0) && !defined (GAMEDLL_S1) && !defined (GAMEDLL_S2)
	DetourSetup(&v_DFS_InitializeFeatureFlagDefinitions, &DFS_InitializeFeatureFlagDefinitions, bAttach);
#endif // !(GAMEDLL_S0) || !(GAMEDLL_S1) || !(GAMEDLL_S2)
}

///////////////////////////////////////////////////////////////////////////////
EngineParms_t* g_pEngineParms = nullptr;