//===========================================================================//
//
// Purpose: Implements all the functions exported by the GameUI dll.
//
// $NoKeywords: $
//===========================================================================//

#include <core/stdafx.h>
#include <tier1/cvar.h>
#include <engine/sys_utils.h>
#include <vgui/vgui_debugpanel.h>
#include <vgui/vgui_baseui_interface.h>
#include <vguimatsurface/MatSystemSurface.h>

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int CEngineVGui::Paint(CEngineVGui* thisptr, PaintMode_t mode)
{
	int result = CEngineVGui_Paint(thisptr, mode);

	if (/*mode == PaintMode_t::PAINT_UIPANELS ||*/ mode == PaintMode_t::PAINT_INGAMEPANELS) // Render in-main menu and in-game.
	{
		g_pOverlay->Update();
	}

	return result;
}

///////////////////////////////////////////////////////////////////////////////
void VEngineVGui::Detour(const bool bAttach) const
{
	DetourSetup(&CEngineVGui_Paint, &CEngineVGui::Paint, bAttach);
}

///////////////////////////////////////////////////////////////////////////////