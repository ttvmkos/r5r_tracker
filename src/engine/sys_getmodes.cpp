//===========================================================================//
//
// Purpose: 
//
//===========================================================================//
#include "core/stdafx.h"
#include "windows/id3dx.h"
#include "engine/sys_getmodes.h"

//-----------------------------------------------------------------------------
// Purpose: creates the game window, obtains the rect and plays the startup movie.
//-----------------------------------------------------------------------------
bool HCVideoMode_Common__CreateGameWindow(int* pnRect)
{
	g_nWindowRect[0] = pnRect[0];
	g_nWindowRect[1] = pnRect[1];
	return CVideoMode_Common__CreateGameWindow(pnRect);
}

void HVideoMode_Common::Detour(const bool bAttach) const
{
	DetourSetup(&CVideoMode_Common__CreateGameWindow, &HCVideoMode_Common__CreateGameWindow, bAttach);
}
