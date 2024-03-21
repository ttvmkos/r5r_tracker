//===== Copyright � 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
// $NoKeywords: $
//===========================================================================//

#ifndef BASEENTITY_H
#define BASEENTITY_H
#ifdef _WIN32
#pragma once
#endif

#include "mathlib/vector.h"
#include "tier1/string_t.h"
#include "public/iservernetworkable.h"
#include "public/iserverentity.h"
#include "engine/gl_model_private.h"
#include "game/shared/collisionproperty.h"
#include "game/shared/shareddefs.h"
#include "networkproperty.h"
#include "entitylist.h"
#include "entityoutput.h"

//-----------------------------------------------------------------------------

typedef void (CBaseEntity::* BASEPTR)(void);
typedef void (CBaseEntity::* ENTITYFUNCPTR)(CBaseEntity* pOther);
typedef void (CBaseEntity::* USEPTR)(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value);

//-----------------------------------------------------------------------------
// Purpose: think contexts
//-----------------------------------------------------------------------------
struct thinkfunc_t
{
	BASEPTR		m_pfnThink;
	bool		m_fireBeforeBaseThink;
	string_t	m_iszContext;
	int			m_nNextThinkTick;
	int			m_nLastThinkTick;
};

class CBaseEntity : public IServerEntity
{
	// non-virtual methods. Don't override these!
public:
	CCollisionProperty* CollisionProp();
	const CCollisionProperty* CollisionProp() const;
	CServerNetworkProperty* NetworkProp();
	const CServerNetworkProperty* NetworkProp() const;

	model_t*		GetModel(void);
	int				GetModelIndex(void) const; // Virtual in-engine!
	string_t		GetModelName(void) const;  // Virtual in-engine!

	inline edict_t GetEdict(void) { return NetworkProp()->GetEdict(); }
	inline string_t GetEntityName(void) const { return m_iName; }

protected:
	CBaseHandle m_RefEHandle;
	char gap_c[4];
	void* m_collideable;
	void* m_networkable;
	int genericKeyValueCount;
	char gap_24[4];
	void* genericKeyValues;
	void* m_pfnMoveDone;
	void* m_pfnThink;
	CServerNetworkProperty m_Network;
	string_t m_ModelName;
	int m_entIndex;
	char gap_74[8]; // Aligns properly in IDA and generated code after setting from 4 to 8.
	string_t* m_iClassname;
	float m_flAnimTime;
	float m_flSimulationTime;
	int m_creationTick;
	int m_nLastThinkTick;
	int m_PredictableID;
	int touchStamp;
	CUtlVector<thinkfunc_t> m_aThinkFunctions;
	float m_entitySpawnTime;
	int m_spawner;
	bool m_wantsDamageCallbacks;
	bool m_wantsDeathCallbacks;
	char gap_c2[2];
	int m_nNextThinkTick;
	int m_fEffects;
	bool m_thinkNextFrame;
	char gap_cd[3];
	string_t m_target;
	int m_networkedFlags;
	char m_nRenderFX;
	char m_nRenderMode;
	__int16 m_nModelIndex;
	int m_clrRender;
	char m_clIntensity;
	char gap_e5[3];
	int m_desiredHibernationType;
	int m_scriptMinHibernationType;
	int m_minSelfAndDescendantHibernationType;
	int m_actualHibernationType;
	int m_hibernationQueueIndex;
	bool m_bRenderWithViewModels;
	char gap_fd[3];
	int m_nameVisibilityFlags;
	float m_cloakEndTime;
	float m_cloakFadeInEndTime;
	float m_cloakFadeOutStartTime;
	float m_cloakFadeInDuration;
	float m_cloakFlickerAmount;
	float m_cloakFlickerEndTime;
	float m_cloakFadeOutDuration;
	Vector3D m_highlightParams[16];
	int m_highlightFunctionBits[8];
	float m_highlightServerFadeBases[2];
	float m_highlightServerFadeStartTimes[2];
	float m_highlightServerFadeEndTimes[2];
	int m_highlightServerContextID;
	int m_highlightTeamBits;
	float m_nextGrenadeTargetTime;
	float m_grenadeTargetDebounce;
	int m_nSimulationTick;
	int m_fDataObjectTypes;
	int m_iEFlags;
	int m_fFlags;
	string_t m_iName;
	int m_scriptNameIndex;
	int m_instanceNameIndex;
	char m_scriptName[64];
	char m_instanceName[64];
	__int64 m_holdUsePrompt;
	__int64 m_pressUsePrompt;
	float m_attachmentLerpStartTime;
	float m_attachmentLerpEndTime;
	Vector3D m_attachmentLerpStartOrigin;
	Vector3D m_attachmentLerpStartAngles;
	int m_parentAttachmentType;
	int m_parentAttachmentIndex;
	int m_parentAttachmentHitbox;
	int m_parentAttachmentModel;
	char m_MoveType;
	char m_MoveCollide;
	char gap_30a[2];
	int m_RestoreMoveTypeOnDetach;
	int m_hMoveParent;
	int m_hMoveChild;
	int m_hMovePeer;
	bool m_bIsActiveChild;
	bool m_bPrevAbsOriginValid;
	char gap_31e[2];
	int m_descendantZiplineCount;
	char gap_324[4];
	CCollisionProperty m_Collision;
	int m_hOwnerEntity;
	int m_CollisionGroup;
	int m_contents;
	bool m_collideWithOwner;
	char gap_3ad[3];
	int m_baseSolidType;
	char gap_3b4[4];
	void* m_pPhysicsObject;
	float m_flNavIgnoreUntilTime;
	int m_hGroundEntity;
	float m_flGroundChangeTime;
	Vector3D m_vecBaseVelocity;
	int m_baseVelocityEnt;
	Vector3D m_vecAbsVelocity;
	Vector3D m_vecAngVelocity;
	char gap_3f4[12];
	matrix3x4_t m_rgflCoordinateFrame;
	float m_flFriction;
	float m_flLocalTime;
	float m_flVPhysicsUpdateLocalTime;
	float m_flMoveDoneTime;
	int m_nPushEnumCount;
	Vector3D m_vecPrevAbsOrigin;
	Vector3D m_vecAbsOrigin;
	Vector3D m_angAbsRotation;
	Vector3D m_vecVelocity;
	char gap_474[4];
	string_t m_iParent;
	int m_iHammerID;
	float m_flSpeed;
	int m_iMaxHealth;
	int m_iHealth;
	void* m_pfnTouch;
	bool m_bClientSideRagdoll;
	char m_lifeState;
	char gap_49a[2];
	int m_scriptNetData;
	int m_phaseShiftFlags;
	char m_baseTakeDamage;
	char gap_4a5[3];
	int m_invulnerableToDamageCount;
	char m_passDamageToParent;
	char gap_4ad[3];
	Vector3D m_deathVelocity;
	float m_lastTitanFootstepDamageTime;
	float m_flMaxspeed;
	int m_visibilityFlags;
	COutputEvent m_OnUser1;
	COutputEvent m_OnDeath;
	COutputEvent m_OnDestroy;
	int m_cellWidth;
	int m_cellBits;
	int m_cellX;
	int m_cellY;
	int m_cellZ;
	Vector3D m_localOrigin;
	Vector3D m_localAngles;
	Vector3D m_vecViewOffset;
	int m_ListByClass;
	char gap_57c[4];
	void* m_pPrevByClass;
	void* m_pNextByClass;
	int m_iInitialTeamNum;
	int m_iTeamNum;
	int m_teamMemberIndex;
	int m_squadID;
	int m_grade;
	int m_ignorePredictedTriggerFlags;
	int m_passThroughFlags;
	int m_passThroughThickness;
	float m_passThroughDirection;
	int m_spawnflags;
	float m_flGravity;
	float m_entityFadeDist;
	int m_dissolveEffectEntityHandle;
	float m_fadeDist;
	string_t m_iSignifierName;
	int m_collectedInvalidateFlags;
	bool m_collectingInvalidateFlags;
	char gap_5d5[3];
	int m_lagCompensationCounter;
	bool m_bLagCompensate;
	bool m_bNetworkQuantizeOriginAndAngles;
	bool m_bForcePurgeFixedupStrings;
	char gap_5df[1];
	int m_debugOverlays;
	char gap_5e4[4];
	void* m_pTimedOverlay;
	char m_ScriptScope[32];
	char m_hScriptInstance[8];
	string_t m_iszScriptId;
	int m_bossPlayer;
	int m_usableType;
	int m_usablePriority;
	float m_usableDistanceOverride;
	float m_usableFOV;
	float m_usePromptSize;
	bool m_hasDispatchedSpawn;
	bool m_bDoDestroyCallback;
	bool m_bDoPusherCallback;
	bool m_bDoPreSpawnCallback;
	bool m_bDoOnSpawnedCallback;
	char gap_63d[3];
	float m_spottedBeginTimes[128];
	float m_spottedLatestTimes[128];
	__int64 m_spottedByTeams[4];
	char m_minimapData[88];
	int m_shieldHealth;
	int m_shieldHealthMax;
	int m_firstChildEntityLink;
	int m_firstParentEntityLink;
	bool m_bIsSoundCodeControllerValueSet;
	char gap_ac9[3];
	float m_flSoundCodeControllerValue;
	float m_pusherWithChildrenRadius;
	int m_childPusherMoveHandlerCount;
	bool m_inWater;
	char gap_ad9[7];
	void* m_statusEffectPlugin;
	__int64 m_realmsBitMask;
	char m_realmsTransmitMaskCached[16];
	int m_realmsTransmitMaskCachedSerialNumber;
};
static_assert(sizeof(CBaseEntity) == 2824);

inline CMemory p_CBaseEntity__GetBaseEntity;
inline CBaseEntity*(*v_CBaseEntity__GetBaseEntity)(CBaseEntity* thisp);

///////////////////////////////////////////////////////////////////////////////
class VBaseEntity : public IDetour
{
	virtual void GetAdr(void) const
	{
		LogFunAdr("CBaseEntity::GetBaseEntity", p_CBaseEntity__GetBaseEntity.GetPtr());
		LogVarAdr("g_pEntityList", reinterpret_cast<uintptr_t>(g_pEntityList));
	}
	virtual void GetFun(void) const
	{
		p_CBaseEntity__GetBaseEntity = g_GameDll.FindPatternSIMD("8B 91 ?? ?? ?? ?? 83 FA FF 74 1F 0F B7 C2 48 8D 0D ?? ?? ?? ?? C1 EA 10 48 8D 04 40 48 03 C0 39 54 C1 08 75 05 48 8B 04 C1 C3 33 C0 C3 CC CC CC 48 8B 41 30");
		v_CBaseEntity__GetBaseEntity = p_CBaseEntity__GetBaseEntity.RCast<CBaseEntity* (*)(CBaseEntity* thisp)>();
	}
	virtual void GetVar(void) const
	{
		g_pEntityList = p_CBaseEntity__GetBaseEntity.FindPattern("48 8D 0D").ResolveRelativeAddressSelf(0x3, 0x7).RCast<CEntInfo**>();
	}
	virtual void GetCon(void) const { }
	virtual void Detour(const bool bAttach) const { }
};
///////////////////////////////////////////////////////////////////////////////

#endif // BASEENTITY_H
