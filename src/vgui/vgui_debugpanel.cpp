//===========================================================================//
//
// Purpose: Implements the debug panels.
//
// $NoKeywords: $
//===========================================================================//

#include <core/stdafx.h>
#include <tier1/cvar.h>
#include <windows/id3dx.h>
#include <vpc/keyvalues.h>
#include <mathlib/color.h>
#include <rtech/rtech_utils.h>
#include <rtech/rui/rui.h>
#include <vgui/vgui_debugpanel.h>
#include <vguimatsurface/MatSystemSurface.h>
#include <materialsystem/cmaterialsystem.h>
#ifndef CLIENT_DLL
#include <engine/server/server.h>
#endif // !CLIENT_DLL
#include <engine/sys_engine.h>
#include <engine/debugoverlay.h>
#include <engine/client/clientstate.h>
#include <materialsystem/cmaterialglue.h>

//-----------------------------------------------------------------------------
// Purpose: proceed a log update
//-----------------------------------------------------------------------------
void CTextOverlay::Update(void)
{
	if (!g_pMatSystemSurface)
	{
		return;
	}
	Con_NPrintf();
	if (con_drawnotify->GetBool())
	{
		DrawNotify();
	}
	if (cl_showsimstats->GetBool())
	{
		DrawSimStats();
	}
	if (cl_showgpustats->GetBool())
	{
		DrawGPUStats();
	}
	if (cl_showmaterialinfo->GetBool())
	{
		DrawCrosshairMaterial();
	}
	if (stream_overlay->GetBool())
	{
		DrawStreamOverlay();
	}
}

//-----------------------------------------------------------------------------
// Purpose: add a log to the vector.
//-----------------------------------------------------------------------------
void CTextOverlay::AddLog(const eDLL_T context, const char* pszText)
{
	if (!con_drawnotify->GetBool() || !VALID_CHARSTAR(pszText))
	{
		return;
	}

	AUTO_LOCK(m_Mutex);
	m_NotifyLines.AddToTail(CTextNotify{ context, con_notifytime->GetFloat() , pszText });

	while (m_NotifyLines.Count() > 0 &&
		(m_NotifyLines.Count() > con_notifylines->GetInt()))
	{
		m_NotifyLines.Remove(0);
	}
}

//-----------------------------------------------------------------------------
// Purpose: draw notify logs on screen.
//-----------------------------------------------------------------------------
void CTextOverlay::DrawNotify(void)
{
	int x = con_notify_invert_x->GetBool() ? g_nWindowRect[0] - con_notify_offset_x->GetInt() : con_notify_offset_x->GetInt();
	int y = con_notify_invert_y->GetBool() ? g_nWindowRect[1] - con_notify_offset_y->GetInt() : con_notify_offset_y->GetInt();

	AUTO_LOCK(m_Mutex);

	for (int i = 0, j = m_NotifyLines.Count(); i < j; i++)
	{
		const CTextNotify& notify = m_NotifyLines[i];
		Color c = GetLogColorForType(notify.m_Type);

		const float flTimeleft = notify.m_flLifeRemaining;

		if (flTimeleft < 1.0f)
		{
			const float f = clamp(flTimeleft, 0.0f, 1.0f) / 1.0f;
			c[3] = uint8_t(f * 255.0f);

			if (i == 0 && f < 0.2f)
			{
				y -= int(m_nFontHeight * (float(1.0f - f / 0.2f)));
			}
		}
		else
		{
			c[3] = 255;
		}
		CMatSystemSurface_DrawColoredText(g_pMatSystemSurface, v_Rui_GetFontFace(),
			m_nFontHeight, x, y, c.r(), c.g(), c.b(), c.a(), "%s", notify.m_Text.String());

		if (IsX360())
		{
			// For some reason the fontTall value on 360 is about twice as high as it should be
			y += 12;
		}
		else
		{
			y += m_nFontHeight;
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: draws formatted text over RUI
// Input  : x - 
//			y - 
//			pszFormat - 
//			... - 
//-----------------------------------------------------------------------------
void CTextOverlay::DrawFormat(const int x, const int y, const Color c, const char* pszFormat, ...) const
{
	static char szLogbuf[4096]{};
	{/////////////////////////////
		va_list args{};
		va_start(args, pszFormat);

		vsnprintf(szLogbuf, sizeof(szLogbuf), pszFormat, args);

		szLogbuf[sizeof(szLogbuf) - 1] = '\0';
		va_end(args);
	}/////////////////////////////

	CMatSystemSurface_DrawColoredText(g_pMatSystemSurface, v_Rui_GetFontFace(), m_nFontHeight, x, y, c.r(), c.g(), c.b(), c.a(), "%s", szLogbuf);
}

//-----------------------------------------------------------------------------
// Purpose: checks if the notify text is expired
// Input  : flFrameTime - 
//-----------------------------------------------------------------------------
void CTextOverlay::ShouldDraw(const float flFrameTime)
{
	if (con_drawnotify->GetBool())
	{
		AUTO_LOCK(m_Mutex);

		for (int i = m_NotifyLines.Count() - 1; i >= 0; i--)
		{
			CTextNotify* pNotify = &m_NotifyLines[i];
			pNotify->m_flLifeRemaining -= flFrameTime;

			if (pNotify->m_flLifeRemaining <= 0.0f)
			{
				m_NotifyLines.Remove(i);
				continue;
			}
		}
	}
	else if (!m_NotifyLines.IsEmpty())
	{
		AUTO_LOCK(m_Mutex);
		m_NotifyLines.Purge();
	}
}

//-----------------------------------------------------------------------------
// Purpose: draws console messages on screen (only used for 'host_speeds'!, deprecated!!).
//-----------------------------------------------------------------------------
void CTextOverlay::Con_NPrintf(void)
{
	if (!m_szCon_NPrintf_Buf[0])
	{
		return;
	}

	static const Color c = { 255, 255, 255, 255 };
	const int nWidth = cl_notify_invert_x->GetBool() ? g_nWindowRect[0] - cl_notify_offset_x->GetInt() : cl_notify_offset_x->GetInt() + m_nCon_NPrintf_Idx * m_nFontHeight;
	const int nHeight = cl_notify_invert_y->GetBool() ? g_nWindowRect[1] - cl_notify_offset_y->GetInt() : cl_notify_offset_y->GetInt();

	CMatSystemSurface_DrawColoredText(g_pMatSystemSurface, v_Rui_GetFontFace(), m_nFontHeight, nWidth, nHeight, c.r(), c.g(), c.b(), c.a(), "%s", m_szCon_NPrintf_Buf);

	m_nCon_NPrintf_Idx = 0;
	m_szCon_NPrintf_Buf[0] = '\0';
}

//-----------------------------------------------------------------------------
// Purpose: draws live simulation stats on screen.
//-----------------------------------------------------------------------------
void CTextOverlay::DrawSimStats(void) const
{
	static const Color c = { 255, 255, 255, 255 };
	const int nWidth  = cl_simstats_invert_x->GetBool() ? g_nWindowRect[0] - cl_simstats_offset_x->GetInt() : cl_simstats_offset_x->GetInt();
	const int nHeight = cl_simstats_invert_y->GetBool() ? g_nWindowRect[1] - cl_simstats_offset_y->GetInt() : cl_simstats_offset_y->GetInt();

	DrawFormat(nWidth, nHeight, c, "Server Frame: (%d) Client Frame: (%d) Render Frame: (%d)\n", 
		g_pClientState->GetServerTickCount(), g_pClientState->GetClientTickCount(), *g_nRenderTickCount);
}

//-----------------------------------------------------------------------------
// Purpose: draws live gpu stats on screen.
//-----------------------------------------------------------------------------
void CTextOverlay::DrawGPUStats(void) const
{
	static const Color c = { 255, 255, 255, 255 };
	const int nWidth  = cl_gpustats_invert_x->GetBool() ? g_nWindowRect[0] - cl_gpustats_offset_x->GetInt() : cl_gpustats_offset_x->GetInt();
	const int nHeight = cl_gpustats_invert_y->GetBool() ? g_nWindowRect[1] - cl_gpustats_offset_y->GetInt() : cl_gpustats_offset_y->GetInt();

	DrawFormat(nWidth, nHeight, c, "%8d/%8d/%8dkiB unusable/unfree/total GPU Streaming Texture memory\n",
		*g_nUnusableStreamingTextureMemory / 1024, *g_nUnfreeStreamingTextureMemory / 1024, *g_nTotalStreamingTextureMemory / 1024);
}

//-----------------------------------------------------------------------------
// Purpose: draws currently traced material info on screen.
//-----------------------------------------------------------------------------
void CTextOverlay::DrawCrosshairMaterial(void) const
{
	CMaterialGlue* pMaterialGlue = GetMaterialAtCrossHair();
	if (!pMaterialGlue)
		return;

	static Color c = { 255, 255, 255, 255 };
	DrawFormat(cl_materialinfo_offset_x->GetInt(), cl_materialinfo_offset_y->GetInt(), c, "name: %s\nguid: %llx\ndimensions: %d x %d\nsurface: %s/%s\nstc: %i\ntc: %i",
		pMaterialGlue->m_pszName,
		pMaterialGlue->m_GUID,
		pMaterialGlue->m_iWidth, pMaterialGlue->m_iHeight,
		pMaterialGlue->m_pszSurfaceProp, pMaterialGlue->m_pszSurfaceProp2,
		pMaterialGlue->m_nStreamableTextureCount,
		pMaterialGlue->m_pShaderGlue->m_nTextureInputCount);
}

//-----------------------------------------------------------------------------
// Purpose: draws the stream overlay on screen.
//-----------------------------------------------------------------------------
void CTextOverlay::DrawStreamOverlay(void) const
{
	static char szLogbuf[4096];
	static const Color c = { 255, 255, 255, 255 };
	
	v_GetStreamOverlay(stream_overlay_mode->GetString(), szLogbuf, sizeof(szLogbuf));
	CMatSystemSurface_DrawColoredText(g_pMatSystemSurface, v_Rui_GetFontFace(), m_nFontHeight, 20, 300, c.r(), c.g(), c.b(), c.a(), "%s", szLogbuf);
}

//-----------------------------------------------------------------------------
// Purpose: gets the log color for context.
// Input  : context - 
// Output : Color
//-----------------------------------------------------------------------------
Color CTextOverlay::GetLogColorForType(const eDLL_T context) const
{
	switch (context)
	{
	case eDLL_T::SCRIPT_SERVER:
		return { con_notify_script_server_clr->GetColor() };
	case eDLL_T::SCRIPT_CLIENT:
		return { con_notify_script_client_clr->GetColor() };
	case eDLL_T::SCRIPT_UI:
		return { con_notify_script_ui_clr->GetColor() };
	case eDLL_T::SERVER:
		return { con_notify_native_server_clr->GetColor() };
	case eDLL_T::CLIENT:
		return { con_notify_native_client_clr->GetColor() };
	case eDLL_T::UI:
		return { con_notify_native_ui_clr->GetColor() };
	case eDLL_T::ENGINE:
		return { con_notify_native_engine_clr->GetColor() };
	case eDLL_T::FS:
		return { con_notify_native_fs_clr->GetColor() };
	case eDLL_T::RTECH:
		return { con_notify_native_rtech_clr->GetColor() };
	case eDLL_T::MS:
		return { con_notify_native_ms_clr->GetColor() };
	case eDLL_T::AUDIO:
		return { con_notify_native_audio_clr->GetColor() };
	case eDLL_T::VIDEO:
		return { con_notify_native_video_clr->GetColor() };
	case eDLL_T::NETCON:
		return { con_notify_netcon_clr->GetColor() };
	case eDLL_T::COMMON:
		return { con_notify_common_clr->GetColor() };
	case eDLL_T::SYSTEM_WARNING:
		return { con_notify_warning_clr->GetColor() };
	case eDLL_T::SYSTEM_ERROR:
		return { con_notify_error_clr->GetColor() };
	default:
		return { con_notify_native_engine_clr->GetColor() };
	}
}

///////////////////////////////////////////////////////////////////////////////
CTextOverlay* g_pOverlay = new CTextOverlay();
