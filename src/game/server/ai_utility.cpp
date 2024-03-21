//=============================================================================//
//
// Purpose: AI system utility
//
//=============================================================================//

#include "core/stdafx.h"
#include "tier1/cvar.h"
#include "public/edict.h"
#include "game/server/detour_impl.h"
#include "game/server/ai_networkmanager.h"

inline uint32_t g_HullMasks[10] = // Hull mask table [r5apex_ds.exe + 131a2f8].
{
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0xfffffffb, 0xfffffffa, 0xfffffff9, 0xfffffff8, 0x00040200
};

//-----------------------------------------------------------------------------
// Purpose: gets the navmesh by hull from global array [small, med_short, medium, large, extra_large]
// input  : hull - 
// Output : pointer to navmesh
//-----------------------------------------------------------------------------
dtNavMesh* GetNavMeshForHull(int hullSize)
{
    Assert(hullSize >= NULL && hullSize < MAX_HULLS); // Programmer error.
    return g_pNavMesh[hullSize];
}

//-----------------------------------------------------------------------------
// Purpose: gets the navmesh by hull from global array [small, med_short, medium, large, extra_large]
// input  : hull - 
// Output : pointer to navmesh
//-----------------------------------------------------------------------------
void ClearNavMeshForHull(int hullSize)
{
    Assert(hullSize >= NULL && hullSize < MAX_HULLS); // Programmer error.
    dtNavMesh* nav = g_pNavMesh[hullSize];

    if (nav) // Only free if NavMesh for hull is loaded.
    {
        // Frees tiles, polys, tris, anything dynamically
        // allocated for this navmesh, and the navmesh itself.
        v_Detour_FreeNavMesh(nav);
        delete nav;

        g_pNavMesh[hullSize] = nullptr;
    }
}

//-----------------------------------------------------------------------------
// Purpose: gets hull mask by id
// input  : hullId - 
// Output : hull mask
//-----------------------------------------------------------------------------
uint32_t GetHullMaskById(int hullId)
{
    Assert(hullId >= NULL && hullId < SDK_ARRAYSIZE(g_HullMasks)); // Programmer error.
    return (hullId + g_HullMasks[hullId]);
}

//-----------------------------------------------------------------------------
// Purpose: determines whether goal poly is reachable from agent poly
// input  : *nav - 
//			fromRef - 
//			goalRef - 
//			hull_type - 
// Output : value if reachable, false otherwise
//-----------------------------------------------------------------------------
uint8_t IsGoalPolyReachable(dtNavMesh* nav, dtPolyRef fromRef, dtPolyRef goalRef, int hullId)
{
    if (navmesh_always_reachable->GetBool())
        return true;

    return v_dtNavMesh__isPolyReachable(nav, fromRef, goalRef, hullId);
}

//-----------------------------------------------------------------------------
// Purpose: initialize NavMesh and Detour query singleton for level
//-----------------------------------------------------------------------------
void Detour_LevelInit()
{
    v_Detour_LevelInit();
    Detour_IsLoaded(); // Inform user which NavMesh files had failed to load.
}

//-----------------------------------------------------------------------------
// Purpose: free's the memory used by all valid NavMesh slots
//-----------------------------------------------------------------------------
void Detour_LevelShutdown()
{
    for (int i = 0; i < MAX_HULLS; i++)
    {
        ClearNavMeshForHull(i);
    }
}

//-----------------------------------------------------------------------------
// Purpose: checks if the NavMesh has failed to load
// Output : true if a NavMesh has successfully loaded, false otherwise
//-----------------------------------------------------------------------------
bool Detour_IsLoaded()
{
    int ret = 0;
    for (int i = 0; i < MAX_HULLS; i++)
    {
        const dtNavMesh* nav = GetNavMeshForHull(i);
        if (!nav) // Failed to load...
        {
            Warning(eDLL_T::SERVER, "NavMesh '%s%s_%s%s' not loaded\n", 
                NAVMESH_PATH, g_ServerGlobalVariables->m_pszMapName, S_HULL_TYPE[i], NAVMESH_EXT);

            ret++;
        }
    }

    Assert(ret <= MAX_HULLS);
    return (ret != MAX_HULLS);
}

//-----------------------------------------------------------------------------
// Purpose: hot swaps the NavMesh with the current files on the disk
// (All hulls will be reloaded! If NavMesh for hull no longer exist, it will be kept empty!!!)
//-----------------------------------------------------------------------------
void Detour_HotSwap()
{
    // Free and re-init NavMesh.
    Detour_LevelShutdown();
    v_Detour_LevelInit();

    if (!Detour_IsLoaded())
        Error(eDLL_T::SERVER, NOERROR, "%s - Failed to hot swap NavMesh\n", __FUNCTION__);
}

///////////////////////////////////////////////////////////////////////////////
void VRecast::Detour(const bool bAttach) const
{
	DetourSetup(&v_dtNavMesh__isPolyReachable, &IsGoalPolyReachable, bAttach);
	DetourSetup(&v_Detour_LevelInit, &Detour_LevelInit, bAttach);
}
