#include "tier0/utility.h"
#include "init.h"
#include "logdef.h"
#include "logger.h"
#ifndef DEDICATED
#include "vgui/vgui_debugpanel.h"
#include "gameui/IConsole.h"
#endif // !DEDICATED
#ifndef CLIENT_DLL
#include "engine/server/sv_rcon.h"
#endif // !CLIENT_DLL
#ifndef NETCONSOLE
#include "vscript/languages/squirrel_re/include/sqstdaux.h"
#endif // !NETCONSOLE
static const std::regex s_AnsiRowRegex("\\\033\\[.*?m");
std::mutex g_LogMutex;

#if !defined (DEDICATED) && !defined (NETCONSOLE)
ImVec4 CheckForWarnings(LogType_t type, eDLL_T context, const ImVec4& defaultCol)
{
	ImVec4 color = defaultCol;
	if (type == LogType_t::LOG_WARNING || context == eDLL_T::SYSTEM_WARNING)
	{
		color = ImVec4(1.00f, 1.00f, 0.00f, 0.80f);
	}
	else if (type == LogType_t::LOG_ERROR || context == eDLL_T::SYSTEM_ERROR)
	{
		color = ImVec4(1.00f, 0.00f, 0.00f, 0.80f);
	}

	return color;
}

ImVec4 GetColorForContext(LogType_t type, eDLL_T context)
{
	switch (context)
	{
	case eDLL_T::SCRIPT_SERVER:
		return CheckForWarnings(type, context, ImVec4(0.59f, 0.58f, 0.73f, 1.00f));
	case eDLL_T::SCRIPT_CLIENT:
		return CheckForWarnings(type, context, ImVec4(0.59f, 0.58f, 0.63f, 1.00f));
	case eDLL_T::SCRIPT_UI:
		return CheckForWarnings(type, context, ImVec4(0.59f, 0.48f, 0.53f, 1.00f));
	case eDLL_T::SERVER:
		return CheckForWarnings(type, context, ImVec4(0.23f, 0.47f, 0.85f, 1.00f));
	case eDLL_T::CLIENT:
		return CheckForWarnings(type, context, ImVec4(0.46f, 0.46f, 0.46f, 1.00f));
	case eDLL_T::UI:
		return CheckForWarnings(type, context, ImVec4(0.59f, 0.35f, 0.46f, 1.00f));
	case eDLL_T::ENGINE:
		return CheckForWarnings(type, context, ImVec4(0.70f, 0.70f, 0.70f, 1.00f));
	case eDLL_T::FS:
		return CheckForWarnings(type, context, ImVec4(0.32f, 0.64f, 0.72f, 1.00f));
	case eDLL_T::RTECH:
		return CheckForWarnings(type, context, ImVec4(0.36f, 0.70f, 0.35f, 1.00f));
	case eDLL_T::MS:
		return CheckForWarnings(type, context, ImVec4(0.75f, 0.30f, 0.68f, 1.00f));
	case eDLL_T::AUDIO:
		return CheckForWarnings(type, context, ImVec4(0.93f, 0.42f, 0.12f, 1.00f));
	case eDLL_T::VIDEO:
		return CheckForWarnings(type, context, ImVec4(0.73f, 0.00f, 0.92f, 1.00f));
	case eDLL_T::NETCON:
		return CheckForWarnings(type, context, ImVec4(0.81f, 0.81f, 0.81f, 1.00f));
	case eDLL_T::COMMON:
		return CheckForWarnings(type, context, ImVec4(1.00f, 0.80f, 0.60f, 1.00f));
	default:
		return CheckForWarnings(type, context, ImVec4(0.81f, 0.81f, 0.81f, 1.00f));
	}
}
#endif // !DEDICATED && !NETCONSOLE

const char* GetContextNameByIndex(eDLL_T context, const bool ansiColor = false)
{
	int index = static_cast<int>(context);
	const char* contextName = s_DefaultAnsiColor;

	switch (context)
	{
	case eDLL_T::SCRIPT_SERVER:
		contextName = s_ScriptAnsiColor[0];
		break;
	case eDLL_T::SCRIPT_CLIENT:
		contextName = s_ScriptAnsiColor[1];
		break;
	case eDLL_T::SCRIPT_UI:
		contextName = s_ScriptAnsiColor[2];
		break;
	case eDLL_T::SERVER:
	case eDLL_T::CLIENT:
	case eDLL_T::UI:
	case eDLL_T::ENGINE:
	case eDLL_T::FS:
	case eDLL_T::RTECH:
	case eDLL_T::MS:
	case eDLL_T::AUDIO:
	case eDLL_T::VIDEO:
	case eDLL_T::NETCON:
	case eDLL_T::COMMON:
	case eDLL_T::SYSTEM_WARNING:
	case eDLL_T::SYSTEM_ERROR:
		contextName = s_DllAnsiColor[index];
		break;
	case eDLL_T::NONE:
	default:
		break;
	}

	if (!ansiColor)
	{
		// Shift # chars to skip ANSI row.
		contextName += sizeof(s_DefaultAnsiColor) - 1;
	}

	return contextName;
}

bool LoggedFromClient(eDLL_T context)
{
#ifndef DEDICATED
	return (context == eDLL_T::CLIENT || context == eDLL_T::SCRIPT_CLIENT
		|| context == eDLL_T::UI || context == eDLL_T::SCRIPT_UI
		|| context == eDLL_T::NETCON);
#else
	NOTE_UNUSED(context);
	return false;
#endif // !DEDICATED
}

//-----------------------------------------------------------------------------
// Purpose: Show logs to all console interfaces (va_list version)
// Input  : logType - 
//			logLevel - 
//			context - 
//			*pszLogger - 
//			*pszFormat -
//			args - 
//			exitCode - 
//			*pszUptimeOverride - 
//-----------------------------------------------------------------------------
void EngineLoggerSink(LogType_t logType, LogLevel_t logLevel, eDLL_T context,
	const char* pszLogger, const char* pszFormat, va_list args,
	const UINT exitCode /*= NO_ERROR*/, const char* pszUptimeOverride /*= nullptr*/)
{
	const char* pszUpTime = pszUptimeOverride ? pszUptimeOverride : Plat_GetProcessUpTime();
	string message(pszUpTime);

	const bool bToConsole = (logLevel >= LogLevel_t::LEVEL_CONSOLE);
	const bool bUseColor = (bToConsole && g_bSpdLog_UseAnsiClr);

	const char* pszContext = GetContextNameByIndex(context, bUseColor);
	message.append(pszContext);

#if !defined (DEDICATED) && !defined (NETCONSOLE)
	ImVec4 overlayColor = GetColorForContext(logType, context);
	eDLL_T overlayContext = context;
#endif // !DEDICATED && !NETCONSOLE

#if !defined (NETCONSOLE)
	bool bSquirrel = false;
	bool bWarning = false;
	bool bError = false;
#else
	NOTE_UNUSED(pszLogger);
#endif // !NETCONSOLE

	//-------------------------------------------------------------------------
	// Setup logger and context
	//-------------------------------------------------------------------------
	switch (logType)
	{
	case LogType_t::LOG_WARNING:
#if !defined (DEDICATED) && !defined (NETCONSOLE)
		overlayContext = eDLL_T::SYSTEM_WARNING;
#endif // !DEDICATED && !NETCONSOLE
		if (bUseColor)
		{
			message.append(g_svYellowF);
		}
		break;
	case LogType_t::LOG_ERROR:
#if !defined (DEDICATED) && !defined (NETCONSOLE)
		overlayContext = eDLL_T::SYSTEM_ERROR;
#endif // !DEDICATED && !NETCONSOLE
		if (bUseColor)
		{
			message.append(g_svRedF);
		}
		break;
#ifndef NETCONSOLE
	case LogType_t::SQ_INFO:
		bSquirrel = true;
		break;
	case LogType_t::SQ_WARNING:
#ifndef DEDICATED
		overlayContext = eDLL_T::SYSTEM_WARNING;
		overlayColor = ImVec4(1.00f, 1.00f, 0.00f, 0.80f);
#endif // !DEDICATED
		bSquirrel = true;
		bWarning = true;
		break;
#endif // !NETCONSOLE
	default:
		break;
	}

	//-------------------------------------------------------------------------
	// Format actual input
	//-------------------------------------------------------------------------
	va_list argsCopy;
	va_copy(argsCopy, args);
	const string formatted = FormatV(pszFormat, argsCopy);
	va_end(argsCopy);

#ifndef NETCONSOLE
	//-------------------------------------------------------------------------
	// Colorize script warnings and errors
	//-------------------------------------------------------------------------
	if (bToConsole && bSquirrel)
	{
		if (bWarning && g_bSQAuxError)
		{
			if (formatted.find("SCRIPT ERROR:") != string::npos ||
				formatted.find(" -> ") != string::npos)
			{
				bError = true;
			}
		}
		else if (g_bSQAuxBadLogic)
		{
			if (formatted.find("There was a problem processing game logic.") != string::npos)
			{
				bError = true;
				g_bSQAuxBadLogic = false;
			}
		}

		// Append warning/error color before appending the formatted text,
		// so that this gets marked as such while preserving context colors.
		if (bError)
		{
#ifndef DEDICATED
			overlayContext = eDLL_T::SYSTEM_ERROR;
			overlayColor = ImVec4(1.00f, 0.00f, 0.00f, 0.80f);
#endif // !DEDICATED

			if (bUseColor)
			{
				message.append(g_svRedF);
			}
		}
		else if (bUseColor && bWarning)
		{
			message.append(g_svYellowF);
		}
	}
#endif // !NETCONSOLE
	message.append(formatted);

	//-------------------------------------------------------------------------
	// Emit to all interfaces
	//-------------------------------------------------------------------------
	std::lock_guard<std::mutex> lock(g_LogMutex);
	if (bToConsole)
	{
		g_TermLogger->debug(message);

		if (bUseColor)
		{
			// Remove ANSI rows before emitting to file or over wire.
			message = std::regex_replace(message, s_AnsiRowRegex, "");
		}
	}

#ifndef NETCONSOLE
	// Output is always logged to the file.
	std::shared_ptr<spdlog::logger> ntlogger = spdlog::get(pszLogger); // <-- Obtain by 'pszLogger'.
	assert(ntlogger.get() != nullptr);
	ntlogger->debug(message);

	if (bToConsole)
	{
#ifndef CLIENT_DLL
		if (!LoggedFromClient(context) && RCONServer()->ShouldSend(sv_rcon::response_t::SERVERDATA_RESPONSE_CONSOLE_LOG))
		{
			RCONServer()->SendEncode(formatted.c_str(), pszUpTime, sv_rcon::response_t::SERVERDATA_RESPONSE_CONSOLE_LOG,
				int(context), int(logType));
		}
#endif // !CLIENT_DLL
#ifndef DEDICATED
		g_ImGuiLogger->debug(message);

		const string logStreamBuf = g_LogStream.str();
		g_pConsole->AddLog(ConLog_t(logStreamBuf, overlayColor));

		// We can only log to the in-game overlay console when the SDK has
		// been fully initialized, due to the use of ConVar's.
		if (g_bSdkInitialized && logLevel >= LogLevel_t::LEVEL_NOTIFY)
		{
			// Draw to mini console.
			g_pOverlay->AddLog(overlayContext, logStreamBuf.c_str());
		}
#endif // !DEDICATED
	}

#ifndef DEDICATED
	g_LogStream.str(string());
	g_LogStream.clear();
#endif // !DEDICATED

#endif // !NETCONSOLE

	if (exitCode) // Terminate the process if an exit code was passed.
	{
		if (MessageBoxA(NULL, Format("%s- %s", pszUpTime, formatted.c_str()).c_str(),
			"SDK Error", MB_ICONERROR | MB_OK))
		{
			TerminateProcess(GetCurrentProcess(), exitCode);
		}
	}
}
