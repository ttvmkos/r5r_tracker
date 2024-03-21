//=============================================================================//
//
// Purpose:
//
//=============================================================================//
#include "mathlib/color.h"
#include "tier1/convar.h"
#include "tier1/cvar.h"
#include "filesystem/filesystem.h"

#define SET_CONVARFLAG(x, y) SetFlag(FCVAR_##x, #x, y)
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
ConVarFlags::ConVarFlags() : m_StringToFlags(DefLessFunc(const char*))
{
	m_Count = 0;

	SET_CONVARFLAG(NONE, "none");
	SET_CONVARFLAG(UNREGISTERED, "unregistered");
	SET_CONVARFLAG(DEVELOPMENTONLY, "development_only");
	SET_CONVARFLAG(GAMEDLL, "server");
	SET_CONVARFLAG(CLIENTDLL, "client");
	SET_CONVARFLAG(HIDDEN, "hidden");
	SET_CONVARFLAG(PROTECTED, "protected");
	SET_CONVARFLAG(SPONLY, "singleplayer");
	SET_CONVARFLAG(ARCHIVE, "archive");
	SET_CONVARFLAG(NOTIFY, "notify");
	SET_CONVARFLAG(USERINFO, "userinfo");
	SET_CONVARFLAG(PRINTABLEONLY, "printable_only");
	SET_CONVARFLAG(UNLOGGED, "unlogged");
	SET_CONVARFLAG(NEVER_AS_STRING, "never_as_string");
	SET_CONVARFLAG(REPLICATED, "replicated");
	SET_CONVARFLAG(CHEAT, "cheat");
	SET_CONVARFLAG(SS, "splitscreen");
	SET_CONVARFLAG(DEMO, "demo");
	SET_CONVARFLAG(DONTRECORD, "dont_record");
	SET_CONVARFLAG(SS_ADDED, "splitscreen_added");
	SET_CONVARFLAG(RELEASE, "release");
	SET_CONVARFLAG(RELOAD_MATERIALS, "reload_materials");
	SET_CONVARFLAG(RELOAD_TEXTURES, "reload_textures");
	SET_CONVARFLAG(NOT_CONNECTED, "not_connected");
	SET_CONVARFLAG(MATERIAL_SYSTEM_THREAD, "material_system_thread");
	SET_CONVARFLAG(ARCHIVE_PLAYERPROFILE, "playerprofile");
	SET_CONVARFLAG(ACCESSIBLE_FROM_THREADS, "accessible_from_threads");
	SET_CONVARFLAG(STUDIO_SYSTEM, "studio_system");
	SET_CONVARFLAG(SERVER_FRAME_THREAD, "server_frame_thread");
	SET_CONVARFLAG(SERVER_CAN_EXECUTE, "server_can_execute");
	SET_CONVARFLAG(SERVER_CANNOT_QUERY, "server_cannot_query");
	SET_CONVARFLAG(CLIENTCMD_CAN_EXECUTE, "clientcmd_can_execute");
	SET_CONVARFLAG(PLATFORM_SYSTEM, "platform_system");
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void ConVarFlags::SetFlag(const int nFlag, const char* szDesc, const char* szShortDesc)
{
	Assert(m_Count < SDK_ARRAYSIZE(m_FlagsToDesc));

	m_StringToFlags.Insert(szDesc, nFlag);
	m_FlagsToDesc[m_Count] = { nFlag, szDesc, szShortDesc };

	m_Count++;
}

ConVarFlags g_ConVarFlags;

//-----------------------------------------------------------------------------
// Purpose: Parse input flag string into bitfield
// Input  : *pszFlags -
//			&nFlags -
//			*pszConVarName -
//-----------------------------------------------------------------------------
bool ConVar_ParseFlagString(const char* pszFlags, int& nFlags, const char* pszConVarName)
{
	size_t len = V_strlen(pszFlags);
	int flags = FCVAR_NONE;

	CUtlString sFlag;

	for (size_t i = 0; i < len; ++i)
	{
		char c = pszFlags[i];

		if (V_isspace(c))
			continue;

		if (c != '|')
			sFlag += c;

		if (c == '|' || i == len - 1)
		{
			if (sFlag == "")
				continue;

			int find = g_ConVarFlags.m_StringToFlags.FindElement(sFlag.Get(), -1);
			if (find == -1)
			{
				Warning(eDLL_T::COMMON,
					"%s: Attempted to parse invalid flag '%s' for convar '%s'\n",
					__FUNCTION__, sFlag.Get(), pszConVarName);

				return false;
			}

			flags |= find;
			sFlag = "";
		}
	}
	nFlags = flags;

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void ConVar_AppendFlags(ConCommandBase* var, char* buf, size_t bufsize)
{
	for (int i = 0; i < ARRAYSIZE(g_ConVarFlags.m_FlagsToDesc); ++i)
	{
		const ConVarFlags::FlagDesc_t& info = g_ConVarFlags.m_FlagsToDesc[i];
		if (var->IsFlagSet(info.bit))
		{
			char append[128];
			V_snprintf(append, sizeof(append), " %s", info.shortdesc);
			V_strncat(buf, append, bufsize);
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void ConVar_PrintDescription(ConCommandBase* pVar)
{
	bool bMin, bMax;
	float fMin, fMax;
	const char* pStr;

	Assert(pVar);

	Color clr(255, 100, 100, 255);

	char outstr[4096];
	outstr[0] = 0;

	if (!pVar->IsCommand())
	{
		ConVar* var = (ConVar*)pVar;

		bMin = var->GetMin(fMin);
		bMax = var->GetMax(fMax);

		const char* value = NULL;
		char tempVal[256];

		if (var->IsFlagSet(FCVAR_NEVER_AS_STRING))
		{
			value = tempVal;

			int intVal = var->GetInt();
			float floatVal = var->GetFloat();

			if (fabs((float)intVal - floatVal) < 0.000001)
			{
				V_snprintf(tempVal, sizeof(tempVal), "%d", intVal);
			}
			else
			{
				V_snprintf(tempVal, sizeof(tempVal), "%f", floatVal);
			}
		}
		else
		{
			value = var->GetString();
		}

		if (value)
		{
			AppendPrintf(outstr, sizeof(outstr), "\"%s\" = \"%s\"", var->GetName(), value);

			if (V_stricmp(value, var->GetDefault()))
			{
				AppendPrintf(outstr, sizeof(outstr), " ( def. \"%s\" )", var->GetDefault());
			}
		}

		if (bMin)
		{
			AppendPrintf(outstr, sizeof(outstr), " min. %f", fMin);
		}
		if (bMax)
		{
			AppendPrintf(outstr, sizeof(outstr), " max. %f", fMax);
		}
	}
	else
	{
		ConCommand* var = (ConCommand*)pVar;

		AppendPrintf(outstr, sizeof(outstr), "\"%s\" ", var->GetName());
	}

	ConVar_AppendFlags(pVar, outstr, sizeof(outstr));

	pStr = pVar->GetHelpText();
	if (pStr && *pStr)
	{
		Msg(eDLL_T::COMMON, "%-80s - %.80s\n", outstr, pStr);
	}
	else
	{
		Msg(eDLL_T::COMMON, "%-80s\n", outstr);
	}
}

static void PrintListHeader(FileHandle_t& f)
{
	char csvflagstr[1024];

	csvflagstr[0] = 0;

	int c = ARRAYSIZE(g_ConVarFlags.m_FlagsToDesc);
	for (int i = 0; i < c; ++i)
	{
		char csvf[64];

		ConVarFlags::FlagDesc_t& entry = g_ConVarFlags.m_FlagsToDesc[i];
		Q_snprintf(csvf, sizeof(csvf), "\"%s\",", entry.desc);
		Q_strncat(csvflagstr, csvf, sizeof(csvflagstr) - strlen(csvflagstr) - 1);
	}

	FileSystem()->FPrintf(f, "\"%s\",\"%s\",%s,\"%s\"\n", "Name", "Value", csvflagstr, "Help Text");
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *var - 
//			*f - 
//-----------------------------------------------------------------------------
static void PrintCvar(ConVar* var, bool logging, FileHandle_t& fh)
{
	char flagstr[128];
	char csvflagstr[1024];

	flagstr[0] = 0;
	csvflagstr[0] = 0;

	int c = ARRAYSIZE(g_ConVarFlags.m_FlagsToDesc);
	for (int i = 0; i < c; ++i)
	{
		char f[32];
		char csvf[64];
		size_t flen = sizeof(csvflagstr) - strlen(csvflagstr) - 1;

		ConVarFlags::FlagDesc_t& entry = g_ConVarFlags.m_FlagsToDesc[i];
		if (var->IsFlagSet(entry.bit))
		{
			Q_snprintf(f, sizeof(f), ", %s", entry.shortdesc);
			Q_strncat(flagstr, f, sizeof(flagstr) - strlen(flagstr) - 1);
			Q_snprintf(csvf, sizeof(csvf), "\"%s\",", entry.desc);
		}
		else
		{
			Q_snprintf(csvf, sizeof(csvf), ",");
		}

		Q_strncat(csvflagstr, csvf, flen);
	}


	char valstr[32];
	char tempbuff[512] = { 0 };

	// Clean up integers
	if (var->GetInt() == (int)var->GetFloat())
	{
		Q_snprintf(valstr, sizeof(valstr), "%-8i", var->GetInt());
	}
	else
	{
		Q_snprintf(valstr, sizeof(valstr), "%-8.3f", var->GetFloat());
	}

	// Print to console
	Msg(eDLL_T::COMMON, "%-40s : %-8s : %-16s : %s\n", var->GetName(),
		valstr, flagstr, StripTabsAndReturns(var->GetHelpText(), tempbuff, sizeof(tempbuff)));
	if (logging)
	{
		FileSystem()->FPrintf(fh, "\"%s\",\"%s\",%s,\"%s\"\n", var->GetName(),
			valstr, csvflagstr, StripQuotes(var->GetHelpText(), tempbuff, sizeof(tempbuff)));
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : 
// Output :
//-----------------------------------------------------------------------------
static void PrintCommand(const ConCommand* cmd, bool logging, FileHandle_t& f)
{
	// Print to console
	char tempbuff[512] = { 0 };
	Msg(eDLL_T::COMMON, "%-40s : %-8s : %-16s : %s\n", cmd->GetName(),
		"cmd", "", StripTabsAndReturns(cmd->GetHelpText(), tempbuff, sizeof(tempbuff)));

	if (logging)
	{
		char emptyflags[256];

		emptyflags[0] = 0;

		int c = ARRAYSIZE(g_ConVarFlags.m_FlagsToDesc);
		for (int i = 0; i < c; ++i)
		{
			char csvf[64];
			size_t len = sizeof(emptyflags) - strlen(emptyflags) - 1;

			Q_snprintf(csvf, sizeof(csvf), ",");
			Q_strncat(emptyflags, csvf, len);
		}
		// Names staring with +/- need to be wrapped in single quotes
		char nameBuf[256];
		Q_snprintf(nameBuf, sizeof(nameBuf), "%s", cmd->GetName());
		if (nameBuf[0] == '+' || nameBuf[0] == '-')
		{
			Q_snprintf(nameBuf, sizeof(nameBuf), "'%s'", cmd->GetName());
		}
		FileSystem()->FPrintf(f, "\"%s\",\"%s\",%s,\"%s\"\n", nameBuf, "cmd",
			emptyflags, StripQuotes(cmd->GetHelpText(), tempbuff, sizeof(tempbuff)));
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : 
// Output : bool
//-----------------------------------------------------------------------------
static bool ConCommandBaseLessFunc(ConCommandBase* const& lhs, ConCommandBase* const& rhs)
{
	const char* left = lhs->GetName();
	const char* right = rhs->GetName();

	if (*left == '-' || *left == '+')
		left++;
	if (*right == '-' || *right == '+')
		right++;

	return (Q_stricmp(left, right) < 0);
}

//-----------------------------------------------------------------------------
// Singleton CCvarUtilities
//-----------------------------------------------------------------------------
static CCvarUtilities g_CvarUtilities;
CCvarUtilities* cv = &g_CvarUtilities;

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : 
// Output : int
//-----------------------------------------------------------------------------
int CCvarUtilities::CountVariablesWithFlags(int flags)
{
	int i = 0;
	ConCommandBase* var;
	CCvar::CCVarIteratorInternal* itint = g_pCVar->FactoryInternalIterator();

	for (itint->SetFirst(); itint->IsValid(); itint->Next()) // Loop through cvars...
	{
		var = itint->Get();
		if (!var->IsCommand())
		{
			if (var->IsFlagSet(flags))
			{
				i++;
			}
		}
	}

	delete itint;
	return i;
}

//-----------------------------------------------------------------------------
// Purpose: Removes the FCVAR_DEVELOPMENTONLY flag from all cvars, making them accessible
//-----------------------------------------------------------------------------
void CCvarUtilities::EnableDevCvars()
{
	// Loop through cvars...
	CCvar::CCVarIteratorInternal* itint = g_pCVar->FactoryInternalIterator();

	for (itint->SetFirst(); itint->IsValid(); itint->Next())
	{
		// remove flag from all cvars
		ConCommandBase* pCommandBase = itint->Get();
		pCommandBase->RemoveFlags(FCVAR_DEVELOPMENTONLY);
	}

	delete itint;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : void CCvar::CvarList_f
//-----------------------------------------------------------------------------
void CCvarUtilities::CvarList(const CCommand& args)
{
	ConCommandBase* var;	// Temporary Pointer to cvars
	int64 iArgs;						// Argument count
	const char* partial = NULL;		// Partial cvar to search for...
									// E.eg
	size_t ipLen = 0;				// Length of the partial cvar

	FileHandle_t f = FILESYSTEM_INVALID_HANDLE;         // FilePointer for logging
	bool bLogging = false;
	// Are we logging?
	iArgs = args.ArgC();		// Get count

	// Print usage?
	if (iArgs == 2 && !Q_strcasecmp(args[1], "?"))
	{
		Msg(eDLL_T::COMMON, "convar_list:  [ log logfile ] [ partial ]\n");
		return;
	}

	if (!Q_strcasecmp(args[1], "log") && iArgs >= 3)
	{
		char fn[256];
		Q_snprintf(fn, sizeof(fn), "%s", args[2]);
		f = FileSystem()->Open(fn, "wb", nullptr, 0);
		if (f)
		{
			bLogging = true;
		}
		else
		{
			Msg(eDLL_T::COMMON, "Couldn't open '%s' for writing!\n", fn);
			return;
		}

		if (iArgs == 4)
		{
			partial = args[3];
			ipLen = Q_strlen(partial);
		}
	}
	else
	{
		partial = args[1];
		ipLen = Q_strlen(partial);
	}

	// Banner
	Msg(eDLL_T::COMMON, "convar list\n--------------\n");

	CUtlRBTree< ConCommandBase* > sorted(0, 0, ConCommandBaseLessFunc);
	CCvar::CCVarIteratorInternal* itint = g_pCVar->FactoryInternalIterator();

	for (itint->SetFirst(); itint->IsValid(); itint->Next()) // Loop through all instances.
	{
		var = itint->Get();

		if (!var->IsFlagSet(FCVAR_DEVELOPMENTONLY) &&
			!var->IsFlagSet(FCVAR_HIDDEN))
		{
			bool print = false;

			if (partial)  // Partial string searching?
			{
				if (!Q_strncasecmp(var->GetName(), partial, ipLen))
				{
					print = true;
				}
			}
			else
			{
				print = true;
			}

			if (print)
			{
				sorted.Insert(var);
			}
		}
	}

	delete itint;

	if (bLogging)
	{
		PrintListHeader(f);
	}
	for (unsigned short i = sorted.FirstInorder();
		i != sorted.InvalidIndex(); i = sorted.NextInorder(i))
	{
		var = sorted[i];
		if (var->IsCommand())
		{
			PrintCommand((ConCommand*)var, bLogging, f);
		}
		else
		{
			PrintCvar((ConVar*)var, bLogging, f);
		}
	}


	// Show total and syntax help...
	if (partial && partial[0])
	{
		Msg(eDLL_T::COMMON, "--------------\n%3i convars/concommands for [%s]\n",
			sorted.Count(), partial);
	}
	else
	{
		Msg(eDLL_T::COMMON, "--------------\n%3i total convars/concommands\n",
			sorted.Count());
	}

	if (bLogging)
	{
		FileSystem()->Close(f);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CCvarUtilities::CvarHelp(const CCommand& args)
{
	const char* search;
	ConCommandBase* var;

	if (args.ArgC() != 2)
	{
		Msg(eDLL_T::COMMON, "Usage:  help <cvarname>\n");
		return;
	}

	// Get name of var to find
	search = args[1];

	// Search for it
	var = g_pCVar->FindCommandBase(search);
	if (!var)
	{
		Msg(eDLL_T::COMMON, "help:  no cvar or command named %s\n", search);
		return;
	}

	// Show info
	ConVar_PrintDescription(var);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CCvarUtilities::CvarDifferences(const CCommand& args)
{
	CCvar::CCVarIteratorInternal* itint = g_pCVar->FactoryInternalIterator();
	int i = 0;

	for (itint->SetFirst(); itint->IsValid(); itint->Next()) // Loop through all instances.
	{
		ConCommandBase* pCommandBase = itint->Get();

		if (!pCommandBase->IsCommand() &&
			!pCommandBase->IsFlagSet(FCVAR_HIDDEN))
		{
			ConVar* pConVar = reinterpret_cast<ConVar*>(pCommandBase);

			if (V_strcmp(pConVar->GetString(), "FCVAR_NEVER_AS_STRING") != NULL)
			{
				if (V_stricmp(pConVar->GetString(), pConVar->GetDefault()) != NULL)
				{
					ConVar_PrintDescription(pConVar);
					i++;
				}
			}
		}
	}

	delete itint;
	Msg(eDLL_T::COMMON, "--------------\n%3i changed convars\n", i);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CCvarUtilities::CvarFindFlags_f(const CCommand& args)
{
	if (args.ArgC() < 2)
	{
		Msg(eDLL_T::COMMON, "Usage:  convar_findByFlags <string>\n");
		Msg(eDLL_T::COMMON, "Available flags to search for: \n");

		for (int i = 0; i < ARRAYSIZE(g_ConVarFlags.m_FlagsToDesc); i++)
		{
			Msg(eDLL_T::COMMON, "   - %s\n", g_ConVarFlags.m_FlagsToDesc[i].desc);
		}
		return;
	}

	// Get substring to find
	const char* search = args[1];
	ConCommandBase* var;

	// Loop through vars and print out findings
	CCvar::CCVarIteratorInternal* itint = g_pCVar->FactoryInternalIterator();

	for (itint->SetFirst(); itint->IsValid(); itint->Next())
	{
		var = itint->Get();

		if (!var->IsFlagSet(FCVAR_DEVELOPMENTONLY) || !var->IsFlagSet(FCVAR_HIDDEN))
		{
			for (int i = 0; i < ARRAYSIZE(g_ConVarFlags.m_FlagsToDesc); i++)
			{
				if (var->IsFlagSet(g_ConVarFlags.m_FlagsToDesc[i].bit))
				{
					if (V_stristr(g_ConVarFlags.m_FlagsToDesc[i].desc, search))
					{
						ConVar_PrintDescription(var);
					}
				}
			}
		}
	}

	delete itint;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int CCvarUtilities::CvarFindFlagsCompletionCallback(const char* partial,
	char commands[COMMAND_COMPLETION_MAXITEMS][COMMAND_COMPLETION_ITEM_LENGTH])
{
	int flagC = ARRAYSIZE(g_ConVarFlags.m_FlagsToDesc);
	char const* pcmd = "findflags ";
	size_t len = Q_strlen(partial);

	if (len < Q_strlen(pcmd))
	{
		int i = 0;
		for (; i < MIN(flagC, COMMAND_COMPLETION_MAXITEMS); i++)
		{
			Q_snprintf(commands[i], sizeof(commands[i]), "%s %s",
				pcmd, g_ConVarFlags.m_FlagsToDesc[i].desc);
			Q_strlower(commands[i]);
		}
		return i;
	}

	char const* pSub = partial + Q_strlen(pcmd);
	size_t nSubLen = Q_strlen(pSub);

	int values = 0;
	for (int i = 0; i < flagC; ++i)
	{
		if (Q_strnicmp(g_ConVarFlags.m_FlagsToDesc[i].desc, pSub, nSubLen))
			continue;

		Q_snprintf(commands[values], sizeof(commands[values]),
			"%s %s", pcmd, g_ConVarFlags.m_FlagsToDesc[i].desc);
		Q_strlower(commands[values]);
		++values;

		if (values >= COMMAND_COMPLETION_MAXITEMS)
			break;
	}
	return values;
}

///////////////////////////////////////////////////////////////////////////////
CCvar* g_pCVar = nullptr;

///////////////////////////////////////////////////////////////////////////////
void VCVar::Detour(const bool bAttach) const
{
	DetourSetup(&v_ConVar_PrintDescription, &ConVar_PrintDescription, bAttach);
}
