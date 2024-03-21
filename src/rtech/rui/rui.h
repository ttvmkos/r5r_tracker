#pragma once

/* ==== RUI ====================================================================================================================================================== */
inline CMemory p_Rui_Draw;
inline bool(*v_Rui_Draw)(__int64* a1, __m128* a2, const __m128i* a3, __int64 a4, __m128* a5);

inline CMemory p_Rui_LoadAsset;
inline void*(*v_Rui_LoadAsset)(const char* szRuiAssetName);

inline CMemory p_Rui_GetFontFace;
inline int16_t(*v_Rui_GetFontFace)(void);

///////////////////////////////////////////////////////////////////////////////
class V_Rui : public IDetour
{
	virtual void GetAdr(void) const
	{
		LogFunAdr("Rui_Draw", p_Rui_Draw.GetPtr());
		LogFunAdr("Rui_LoadAsset", p_Rui_LoadAsset.GetPtr());
		LogFunAdr("Rui_GetFontFace", p_Rui_GetFontFace.GetPtr());
	}
	virtual void GetFun(void) const
	{
		p_Rui_Draw = g_GameDll.FindPatternSIMD("40 53 48 83 EC 40 4C 8B 5A 18");
		v_Rui_Draw = p_Rui_Draw.RCast<bool(*)(__int64*, __m128*, const __m128i*, __int64, __m128*)>(); /* 40 53 48 83 EC 40 4C 8B 5A 18 */

		p_Rui_LoadAsset = g_GameDll.FindPatternSIMD("E8 ?? ?? ?? ?? EB 03 49 8B C6 48 89 86 ?? ?? ?? ?? 8B 86 ?? ?? ?? ??").FollowNearCallSelf();
		v_Rui_LoadAsset = p_Rui_LoadAsset.RCast<void* (*)(const char*)>(); /*E8 ?? ?? ?? ?? EB 03 49 8B C6 48 89 86 ?? ?? ?? ?? 8B 86 ?? ?? ?? ??*/

		p_Rui_GetFontFace = g_GameDll.FindPatternSIMD("F7 05 ?? ?? ?? ?? ?? ?? ?? ?? 4C 8D 0D ?? ?? ?? ?? 74 05 49 8B D1 EB 19 48 8B 05 ?? ?? ?? ?? 48 8D 15 ?? ?? ?? ?? 48 8B 48 58 48 85 C9 48 0F 45 D1 F7 05 ?? ?? ?? ?? ?? ?? ?? ?? 75 19 48 8B 05 ?? ?? ?? ?? 4C 8D 0D ?? ?? ?? ?? 4C 8B 40 58 4D 85 C0 4D 0F 45 C8 49 8B C9 48 FF 25 ?? ?? ?? ??");
		v_Rui_GetFontFace = p_Rui_GetFontFace.RCast<int16_t(*)(void)>();/*F7 05 ? ? ? ? ? ? ? ? 4C 8D 0D ? ? ? ? 74 05 49 8B D1 EB 19 48 8B 05 ? ? ? ? 48 8D 15 ? ? ? ? 48 8B 48 58 48 85 C9 48 0F 45 D1 F7 05 ? ? ? ? ? ? ? ? 75 19 48 8B 05 ? ? ? ? 4C 8D 0D ? ? ? ? 4C 8B 40 58 4D 85 C0 4D 0F 45 C8 49 8B C9 48 FF 25 ? ? ? ?*/
	}
	virtual void GetVar(void) const { }
	virtual void GetCon(void) const { }
	virtual void Detour(const bool bAttach) const;
};
///////////////////////////////////////////////////////////////////////////////
