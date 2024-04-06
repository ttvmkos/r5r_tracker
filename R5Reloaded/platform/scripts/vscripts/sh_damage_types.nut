global function DamageTypes_Init
global function RegisterWeaponDamageSource
global function GetObitFromDamageSourceID
global function GetObitImageFromDamageSourceID
global function DamageSourceIDToString
global function DamageSourceIDHasString
global function GetRefFromDamageSourceID
global function PIN_GetDamageCause

struct
{
	table<int,string> damageSourceIDToName
	table<int,asset> damageSourceIDToImage
	table<int,string> damageSourceIDToString
} file

global enum eDamageSourceId
{
	invalid 	= -1  // used in code

	//mkos - mapped index for easy ref
	//---------------------------
	// defined in damageDef.txt. This will go away ( you can use damagedef_nuclear_core instead of eDamageSourceId.[enum id] and get rid of it from here )
	// once this list has only damagedef_*, then we can remove eDamageSourceId
	code_reserved  				// may be merged with invalid -1 above
	damagedef_unknown		   	// must start at 1 and order must match what's in damageDefs.txt //1
	damagedef_unknownBugIt //2
	damagedef_suicide //3
	damagedef_despawn //4
	damagedef_titan_step //5
	damagedef_crush //6
	damagedef_sonic_blast //7
	damagedef_nuclear_core //8
	damagedef_titan_fall //9
	damagedef_titan_hotdrop //10
	damagedef_reaper_fall //11
	damagedef_trip_wire //12
	damagedef_wrecking_ball //13
	damagedef_reaper_groundslam //14
	damagedef_reaper_nuke //15
	damagedef_frag_drone_explode //16
	damagedef_frag_drone_explode_FD //17
	damagedef_frag_drone_throwable_PLAYER //18
	damagedef_frag_drone_throwable_NPC //19
	damagedef_stalker_powersupply_explosion_small //20
	damagedef_stalker_powersupply_explosion_large //21
	damagedef_stalker_powersupply_explosion_large_at //22
	damagedef_shield_captain_arc_shield //23
	damagedef_fd_explosive_barrel //24
	damagedef_fd_tether_trap //25
	damagedef_pilot_slam //26
	damagedef_ticky_arc_blast //27
	damagedef_grenade_gas //28
	damagedef_gas_exposure //29
	damagedef_dirty_bomb_explosion //30
	damagedef_sonic_boom //31
	damagedef_bangalore_smoke_explosion //32
	damagedef_creeping_bombardment_detcord_explosion //33
	damagedef_tank_bombardment_detcord_explosion //34
	damagedef_defensive_bombardment //35
	damagedef_loot_drone_explosion //36

	//Custom
	damagedef_DocDrone //37

	//---------------------------
	mp_weapon_grenade_decoyaudio //38
	mp_weapon_grenade_cryonade //39
	
	// Pilot Weapons
	mp_weapon_hemlok //40
	mp_weapon_lmg //41
	mp_weapon_rspn101 //42
	mp_weapon_vinson //43
	mp_weapon_lstar //44
	mp_weapon_g2 //45
	mp_weapon_r97 //46
	mp_weapon_dmr //47
	mp_weapon_wingman //48
	//mp_weapon_wingman_n
	mp_weapon_wingmanelite //49
	mp_weapon_semipistol //50
	mp_weapon_autopistol //51
	mp_weapon_sniper //52
	mp_weapon_sentinel //53
	mp_weapon_shotgun //54
	mp_weapon_mastiff //55
	mp_weapon_frag_grenade //56
	mp_weapon_grenade_emp //57
	mp_weapon_arc_blast //58
	mp_weapon_thermite_grenade //59
	mp_weapon_nuke_satchel //60
	//mp_weapon_satchel
	mp_extreme_environment //61
	mp_weapon_shotgun_pistol //62
	mp_weapon_doubletake //63
	mp_weapon_alternator_smg //64
	mp_weapon_esaw //65
	mp_weapon_wrecking_ball //66
	mp_weapon_melee_survival //67
	mp_weapon_pdw //68
	mp_weapon_energy_ar //69
	mp_weapon_volt_smg //70
	mp_weapon_defender //71
	mp_weapon_warmachine //72
	mp_weapon_car //73
	mp_weapon_3030 //74
	mp_weapon_dragon_lmg //75
	//mp_weapon_energysword
	//mp_ability_birds
	mp_weapon_throwingknife //76
	//mp_weapon_softball
	//mp_weapon_epg
	//mp_weapon_smr
	//mp_weapon_rocket_launcher
	mp_weapon_grenade_electric_smoke //77
	mp_weapon_grenade_gravity //78
	//mp_weapon_rspn101_og
	//sp_weapon_arc_tool
	//
	melee_pilot_emptyhanded //79
	melee_pilot_arena //80
	melee_pilot_sword //81
	melee_titan_punch //82
	melee_titan_punch_ion //83
	melee_titan_punch_tone //84
	melee_titan_punch_legion //85
	melee_titan_punch_scorch //86
	melee_titan_punch_northstar //87
	melee_titan_punch_fighter //88
	melee_titan_punch_vanguard //89
	melee_titan_punch_stealth //90
	melee_titan_punch_rocket //91
	melee_titan_punch_drone //92
	melee_titan_sword //93
	melee_titan_sword_aoe //94

	melee_boxing_ring //95
	mp_weapon_melee_boxing_ring //96
	
	melee_data_knife //97
	mp_weapon_data_knife_primary //98

	melee_wraith_kunai //99
	mp_weapon_wraith_kunai_primary //101
	
	melee_bolo_sword //102
	mp_weapon_bolo_sword_primary //103

	melee_bloodhound_axe //104
	mp_weapon_bloodhound_axe_primary //105

	melee_lifeline_baton //106
	mp_weapon_lifeline_baton_primary //107

	melee_shadowsquad_hands //108
	melee_shadowroyale_hands //109
	mp_weapon_shadow_squad_hands_primary //110

	mp_weapon_tesla_trap //111

	// Turret Weapons
	mp_weapon_yh803 //112
	mp_weapon_yh803_bullet //113
	mp_weapon_yh803_bullet_overcharged //114
	mp_weapon_mega_turret //115
	mp_weapon_mega_turret_aa //116
	mp_turretweapon_rockets //117
	mp_turretweapon_blaster //118
	mp_turretweapon_plasma //119
	mp_turretweapon_sentry //120
	//mp_weapon_mobile_hmg
	mp_weapon_smart_pistol //121

	//Character Abilities
	mp_weapon_defensive_bombardment_weapon //122
	mp_weapon_creeping_bombardment_weapon //123
	mp_ability_octane_stim //124
	mp_ability_crypto_drone_emp //125
	mp_ability_crypto_drone_emp_trap //126
	// AI only Weapons
	mp_weapon_super_spectre //127 
	mp_weapon_dronebeam //128
	mp_weapon_dronerocket //129
	mp_weapon_droneplasma //130
	mp_weapon_turretplasma //131
	mp_weapon_turretrockets //132
	mp_weapon_turretplasma_mega //133
	mp_weapon_gunship_launcher //134
	mp_weapon_gunship_turret //135
	mp_weapon_gunship_missile //136

	// Misc
	rodeo //137
	rodeo_forced_titan_eject  //138 //For awarding points when you force a pilot to eject via rodeo
	rodeo_execution //139
	human_melee //140
	auto_titan_melee //141
	berserker_melee //142
	mind_crime //143
	charge_ball //144
	grunt_melee //145
	spectre_melee //146
	prowler_melee //147
	super_spectre_melee //148
	titan_execution //149
	human_execution //150
	eviscerate //151
	wall_smash //152
	ai_turret //153
	team_switch //154
	rocket //155
	titan_explosion //156
	flash_surge //157
	sticky_time_bomb //158
	vortex_grenade //159
	droppod_impact //160
	ai_turret_explosion //161
	rodeo_trap //162
	round_end //163
	bubble_shield //164
	evac_dropship_explosion //165
	sticky_explosive //166
	titan_grapple //167

	// streaks

	// Environmental
	fall //168
	splat //169
	crushed //170
	burn //171
	lasergrid //172
	outOfBounds //173
	deathField //174
	indoor_inferno //175
	submerged //176
	switchback_trap //177
	floor_is_lava //178
	suicideSpectreAoE //179
	titanEmpField //180
	stuck //181
	deadly_fog //182
	exploding_barrel //183
	electric_conduit //184
	turbine //185
	harvester_beam //186
	toxic_sludge //187

	mp_weapon_spectre_spawner //188

	// development
	weapon_cubemap //189

	// Prototype
	mp_weapon_zipline //190
	at_turret_override //191
	rodeo_battery_removal //192
	phase_shift //193
	gamemode_bomb_detonation //194
	nuclear_turret //195
	proto_viewmodel_test //196
	mp_titanweapon_heat_shield //197
	mp_titanweapon_sonar_pulse //198
	mp_titanability_slow_trap //199
	mp_titanability_gun_shield //200
	mp_titanability_power_shot //201
	mp_titanability_ammo_swap //202
	mp_titanability_sonar_pulse //203
	mp_titanability_rearm //204
	mp_titancore_upgrade //205
	mp_titanweapon_xo16_vanguard //206
	mp_weapon_arc_trap //207
	mp_weapon_arc_launcher //208
	core_overload //209
	mp_titanweapon_stasis //210
	mp_titanweapon_stealth_titan //211
	mp_titanweapon_rocket_titan //212
	mp_titanweapon_drone_titan //213
	mp_titanweapon_stealth_sword //214
	mp_ability_consumable //215
	snd_bomb //216
	
	bombardment //217
	bleedout //218
	mp_weapon_energy_shotgun //219
	fire //220
	//damageSourceId=eDamageSourceId.xxxxx
	//fireteam
	//marvin
	//rocketstrike
	//orbitallaser
	//explosion
	
	//custom
	mp_weapon_raygun //221
	mp_weapon_haloshotgun //222
	mp_weapon_halosmg //223
	mp_weapon_halomagnum //224
	mp_weapon_halobattlerifle //225
	mp_weapon_haloassaultrifle //226
	mp_weapon_halosniperrifle //227
	mp_weapon_haloneedler //228
	mp_weapon_energysword //229
	mp_weapon_frag_grenade_halomod //230
	mp_weapon_plasma_grenade_halomod //231
	mp_weapon_oddball_primary //232
	melee_oddball //233
}

//When adding new mods, they need to be added below and to persistent_player_data_version_N.pdef in r1/cfg/server.
//Then when updating that file, save a new one and increment N.

global enum eModSourceId
{
	accelerator
	afterburners
	arc_triple_threat
	aog
	burn_mod_autopistol
	burn_mod_car
	burn_mod_defender
	burn_mod_dmr
	burn_mod_emp_grenade
	burn_mod_frag_grenade
	burn_mod_grenade_electric_smoke
	burn_mod_grenade_gravity
	burn_mod_thermite_grenade
	burn_mod_g2
	burn_mod_hemlok
	burn_mod_lmg
	burn_mod_mgl
	burn_mod_r97
	burn_mod_rspn101
	burn_mod_satchel
	burn_mod_semipistol
    burn_mod_smart_pistol
	burn_mod_smr
	burn_mod_sniper
	burn_mod_rocket_launcher
	burn_mod_titan_40mm
	burn_mod_titan_arc_cannon
	burn_mod_titan_rocket_launcher
	burn_mod_titan_sniper
	burn_mod_titan_triple_threat
	burn_mod_titan_xo16
	burn_mod_titan_dumbfire_rockets
	burn_mod_titan_homing_rockets
	burn_mod_titan_salvo_rockets
	burn_mod_titan_shoulder_rockets
	burn_mod_titan_vortex_shield
	burn_mod_titan_smoke
	burn_mod_titan_particle_wall
	burst
	capacitor
	enhanced_targeting
	extended_ammo
	fast_lock
	fast_reload
	guided_missile
	instant_shot
	overcharge
	quick_shot
	rapid_fire_missiles
	burn_mod_shotgun
	silencer
	slammer
	spread_increase_ttt
	stabilizer
	titanhammer
	burn_mod_wingman
	burn_mod_lstar
	burn_mod_mastiff
	burn_mod_vinson
	ricochet
	ar_trajectory
	smart_lock
	pro_screen
}

//Attachments intentionally left off. This prevents them from displaying in kill cards.
// modNameStrings should be defined when the mods are created, not in a separate table -Mackey
global const modNameStrings = {
	[ eDamageSourceId.damagedef_DocDrone ] 					= "DRONE",
	[ eModSourceId.accelerator ]						= "#MOD_ACCELERATOR_NAME",
	[ eModSourceId.afterburners ]						= "#MOD_AFTERBURNERS_NAME",
	[ eModSourceId.arc_triple_threat ] 					= "#MOD_ARC_TRIPLE_THREAT_NAME",
	[ eModSourceId.burn_mod_autopistol ] 				= "#BC_AUTOPISTOL_M2",
	[ eModSourceId.burn_mod_car ] 						= "#BC_CAR_M2",
	[ eModSourceId.burn_mod_defender ] 					= "#BC_DEFENDER_M2",
	[ eModSourceId.burn_mod_dmr ] 						= "#BC_DMR_M2",
	[ eModSourceId.burn_mod_emp_grenade ] 				= "#BC_EMP_GRENADE_M2",
	[ eModSourceId.burn_mod_frag_grenade ] 				= "#BC_FRAG_GRENADE_M2",
	[ eModSourceId.burn_mod_grenade_electric_smoke ] 	= "#BC_GRENADE_ELECTRIC_SMOKE_M2",
	[ eModSourceId.burn_mod_grenade_gravity ] 			= "#BC_GRENADE_ELECTRIC_SMOKE_M2",
	[ eModSourceId.burn_mod_thermite_grenade ] 			= "#BC_GRENADE_ELECTRIC_SMOKE_M2",
	[ eModSourceId.burn_mod_g2 ] 						= "#BC_G2_M2",
	[ eModSourceId.burn_mod_hemlok ] 					= "#BC_HEMLOK_M2",
	[ eModSourceId.burn_mod_lmg ] 						= "#BC_LMG_M2",
	[ eModSourceId.burn_mod_mgl ] 						= "#BC_MGL_M2",
	[ eModSourceId.burn_mod_r97 ] 						= "#BC_R97_M2",
	[ eModSourceId.burn_mod_rspn101 ] 					= "#BC_RSPN101_M2",
	[ eModSourceId.burn_mod_satchel ] 					= "#BC_SATCHEL_M2",
	[ eModSourceId.burn_mod_semipistol ] 				= "#BC_SEMIPISTOL_M2",
	[ eModSourceId.burn_mod_smr ] 						= "#BC_SMR_M2",
	[ eModSourceId.burn_mod_smart_pistol ] 				= "#BC_SMART_PISTOL_M2",
	[ eModSourceId.burn_mod_sniper ] 					= "#BC_SNIPER_M2",
	[ eModSourceId.burn_mod_rocket_launcher ] 			= "#BC_ROCKET_LAUNCHER_M2",
	[ eModSourceId.burn_mod_titan_40mm ] 				= "#BC_TITAN_40MM_M2",
	[ eModSourceId.burn_mod_titan_arc_cannon ] 			= "#BC_TITAN_ARC_CANNON_M2",
	[ eModSourceId.burn_mod_titan_rocket_launcher ] 	= "#BC_TITAN_ROCKET_LAUNCHER_M2",
	[ eModSourceId.burn_mod_titan_sniper ] 				= "#BC_TITAN_SNIPER_M2",
	[ eModSourceId.burn_mod_titan_triple_threat ] 		= "#BC_TITAN_TRIPLE_THREAT_M2",
	[ eModSourceId.burn_mod_titan_xo16 ]			 	= "#BC_TITAN_XO16_M2",
	[ eModSourceId.burn_mod_titan_dumbfire_rockets ] 	= "#BC_TITAN_DUMBFIRE_MISSILE_M2",
	[ eModSourceId.burn_mod_titan_homing_rockets ] 		= "#BC_TITAN_HOMING_ROCKETS_M2",
	[ eModSourceId.burn_mod_titan_salvo_rockets ] 		= "#BC_TITAN_SALVO_ROCKETS_M2",
	[ eModSourceId.burn_mod_titan_shoulder_rockets ] 	= "#BC_TITAN_SHOULDER_ROCKETS_M2",
	[ eModSourceId.burn_mod_titan_vortex_shield ] 		= "#BC_TITAN_VORTEX_SHIELD_M2",
	[ eModSourceId.burn_mod_titan_smoke ] 				= "#BC_TITAN_ELECTRIC_SMOKE_M2",
	[ eModSourceId.burn_mod_titan_particle_wall ] 		= "#BC_TITAN_SHIELD_WALL_M2",
	[ eModSourceId.burst ] 								= "#MOD_BURST_NAME",
	[ eModSourceId.capacitor ] 							= "#MOD_CAPACITOR_NAME",
	[ eModSourceId.enhanced_targeting ] 				= "#MOD_ENHANCED_TARGETING_NAME",
	[ eModSourceId.extended_ammo ] 						= "#MOD_EXTENDED_MAG_NAME",
	[ eModSourceId.fast_reload ] 						= "#MOD_FAST_RELOAD_NAME",
	[ eModSourceId.instant_shot ]						= "#MOD_INSTANT_SHOT_NAME",
	[ eModSourceId.overcharge ] 						= "#MOD_OVERCHARGE_NAME",
	[ eModSourceId.quick_shot ]							= "#MOD_QUICK_SHOT_NAME",
	[ eModSourceId.rapid_fire_missiles ] 				= "#MOD_RAPID_FIRE_MISSILES_NAME",
	[ eModSourceId.burn_mod_shotgun ] 					= "#BC_SHOTGUN_M2",
	[ eModSourceId.silencer ] 							= "#MOD_SILENCER_NAME",
	[ eModSourceId.slammer ] 							= "#MOD_SLAMMER_NAME",
	[ eModSourceId.spread_increase_ttt ]				= "#MOD_SPREAD_INCREASE_TTT_NAME",
	[ eModSourceId.stabilizer ]							= "#MOD_STABILIZER_NAME",
	[ eModSourceId.titanhammer ] 						= "#MOD_TITANHAMMER_NAME",
	[ eModSourceId.burn_mod_wingman ]					= "#BC_WINGMAN_M2",
	[ eModSourceId.burn_mod_lstar ]						= "#BC_LSTAR_M2",
	[ eModSourceId.burn_mod_mastiff ]					= "#BC_MASTIFF_M2",
	[ eModSourceId.burn_mod_vinson ]					= "#BC_VINSON_M2",
	[ eModSourceId.ricochet ]							= "Ricochet",
	[ eModSourceId.ar_trajectory ]						= "AR Trajectory",
	[ eModSourceId.smart_lock ]							= "Smart Lock",
	[ eModSourceId.pro_screen ]							= "Pro Screen",
}

void function DamageTypes_Init()
{
#if SERVER
	SvDemo_ConsistencyCheckString( "DamageTypes_Init()" )
#endif

	foreach ( name, number in eDamageSourceId )
	{
		file.damageSourceIDToString[ number ] <- name
	}

	PrecacheWeapon( $"mp_weapon_rspn101" ) // used by npc_soldier ><

#if DEVELOPER

	int numDamageDefs = DamageDef_GetCount()
	table damageSourceIdEnum = expect table( getconsttable().eDamageSourceId )
	foreach ( name, id in damageSourceIdEnum )
	{
		expect int( id )
		if ( id <= eDamageSourceId.code_reserved || id >= numDamageDefs )
			continue

		string damageDefName = DamageDef_GetName( id )
		Assert( damageDefName == name, "damage def (" + id + ") name: '" + damageDefName + "' doesn't match damage source id '" + name + "'" )
	}
#endif

	file.damageSourceIDToImage =
	{
	}

	file.damageSourceIDToImage[eDamageSourceId.melee_shadowsquad_hands] 	<- $"rui/gamemodes/shadow_squad/shadow_icon_small"
	file.damageSourceIDToImage[eDamageSourceId.mp_weapon_volt_smg] 			<- $"rui/weapon_icons/r5/weapon_volt"
	file.damageSourceIDToImage[eDamageSourceId.mp_weapon_car] 				<- $"rui/weapon_icons/r5/weapon_car"
	file.damageSourceIDToImage[eDamageSourceId.mp_weapon_sentinel] 			<- $"rui/weapon_icons/r5/weapon_sentinel"
	//file.damageSourceIDToImage[eDamageSourceId.mp_weapon_softball] 			<- $"r2_ui/menus/loadout_icons/primary_weapon/primary_softball"
	//file.damageSourceIDToImage[eDamageSourceId.mp_weapon_epg] 				<- $"r2_ui/menus/loadout_icons/primary_weapon/primary_epg1"
	//file.damageSourceIDToImage[eDamageSourceId.mp_weapon_rocket_launcher] 	<- $"r2_ui/menus/loadout_icons/anti_titan/at_archer"
	//file.damageSourceIDToImage[eDamageSourceId.mp_weapon_smr] 				<- $"r2_ui/menus/loadout_icons/anti_titan/at_sidewinder"
	//file.damageSourceIDToImage[eDamageSourceId.mp_weapon_wingman_n]			<- $"r2_ui/menus/loadout_icons/primary_weapon/primary_wingman_elite"
	//file.damageSourceIDToImage[eDamageSourceId.mp_weapon_mobile_hmg]		<- $"rui/weapon_icons/r5/weapon_rampart_turret"
	file.damageSourceIDToImage[eDamageSourceId.mp_weapon_smart_pistol]		<- $"rui/weapon_icons/r5/weapon_smart_pistol"
	file.damageSourceIDToImage[eDamageSourceId.snd_bomb]					<- $"rui/flowstatecustom/bombicon"
	file.damageSourceIDToImage[eDamageSourceId.mp_weapon_oddball_primary]					<- $"rui/flowstate_custom/oddball_white"
	file.damageSourceIDToImage[eDamageSourceId.melee_oddball]					<- $"rui/flowstate_custom/oddball_white"
	//file.damageSourceIDToImage[eDamageSourceId.sp_weapon_arc_tool]			<- $"r2_ui/menus/loadout_icons/primary_weapon/primary_arc_tool"

	file.damageSourceIDToName =
	{
		[ eDamageSourceId.damagedef_DocDrone ] 					= "DRONE",
		//mp
		[ eDamageSourceId.mp_extreme_environment ] 					= "#DAMAGE_EXTREME_ENVIRONMENT",

		[ eDamageSourceId.mp_weapon_yh803 ] 						= "#WPN_LIGHT_TURRET",
		[ eDamageSourceId.mp_weapon_yh803_bullet ]					= "#WPN_LIGHT_TURRET",
		[ eDamageSourceId.mp_weapon_yh803_bullet_overcharged ]		= "#WPN_LIGHT_TURRET",
		[ eDamageSourceId.mp_weapon_mega_turret ]					= "#WPN_MEGA_TURRET",
		[ eDamageSourceId.mp_weapon_mega_turret_aa ]				= "#WPN_MEGA_TURRET",
		[ eDamageSourceId.mp_turretweapon_rockets ]					= "#WPN_TURRET_ROCKETS",
		[ eDamageSourceId.mp_weapon_super_spectre ]					= "#WPN_SUPERSPECTRE_ROCKETS",
		[ eDamageSourceId.mp_weapon_dronebeam ] 					= "#WPN_DRONERBEAM",
		[ eDamageSourceId.mp_weapon_dronerocket ] 					= "#WPN_DRONEROCKET",
		[ eDamageSourceId.mp_weapon_droneplasma ] 					= "#WPN_DRONEPLASMA",
		[ eDamageSourceId.mp_weapon_turretplasma ] 					= "#WPN_TURRETPLASMA",
		[ eDamageSourceId.mp_weapon_turretrockets ] 				= "#WPN_TURRETROCKETS",
		[ eDamageSourceId.mp_weapon_turretplasma_mega ] 			= "#WPN_TURRETPLASMA_MEGA",
		[ eDamageSourceId.mp_weapon_gunship_launcher ] 				= "#WPN_GUNSHIP_LAUNCHER",
		[ eDamageSourceId.mp_weapon_gunship_turret ]				= "#WPN_GUNSHIP_TURRET",
		[ eDamageSourceId.mp_weapon_gunship_turret ]				= "#WPN_GUNSHIP_MISSILE",

		[ eDamageSourceId.mp_titanability_slow_trap ]				= "#DEATH_SLOW_TRAP",
		[ eDamageSourceId.damagedef_suicide ] 						= "#DEATH_SUICIDE",
		[ eDamageSourceId.rodeo ] 									= "#DEATH_TITAN_RODEO",
		[ eDamageSourceId.rodeo_forced_titan_eject ] 				= "#DEATH_TITAN_RODEO",
		[ eDamageSourceId.rodeo_execution ] 						= "#DEATH_RODEO_EXECUTION",
		[ eDamageSourceId.nuclear_turret ] 							= "#DEATH_NUCLEAR_TURRET",
		[ eDamageSourceId.berserker_melee ]							= "#DEATH_BERSERKER_MELEE",
		[ eDamageSourceId.human_melee ] 							= "#DEATH_HUMAN_MELEE",
		[ eDamageSourceId.auto_titan_melee ] 						= "#DEATH_AUTO_TITAN_MELEE",

		[ eDamageSourceId.prowler_melee ] 							= "#DEATH_PROWLER_MELEE",
		[ eDamageSourceId.super_spectre_melee ] 					= "#DEATH_SUPER_SPECTRE",
		[ eDamageSourceId.grunt_melee ] 							= "#DEATH_GRUNT_MELEE",
		[ eDamageSourceId.spectre_melee ] 							= "#DEATH_SPECTRE_MELEE",
		[ eDamageSourceId.eviscerate ]	 							= "#DEATH_EVISCERATE",
		[ eDamageSourceId.wall_smash ] 								= "#DEATH_WALL_SMASH",
		[ eDamageSourceId.ai_turret ] 								= "#DEATH_TURRET",
		[ eDamageSourceId.team_switch ] 							= "#DEATH_TEAM_CHANGE",
		[ eDamageSourceId.rocket ] 									= "#DEATH_ROCKET",
		[ eDamageSourceId.titan_explosion ] 						= "#DEATH_TITAN_EXPLOSION",
		[ eDamageSourceId.evac_dropship_explosion ] 				= "#DEATH_EVAC_DROPSHIP_EXPLOSION",
		[ eDamageSourceId.flash_surge ] 							= "#DEATH_FLASH_SURGE",
		[ eDamageSourceId.sticky_time_bomb ] 						= "#DEATH_STICKY_TIME_BOMB",
		[ eDamageSourceId.vortex_grenade ] 							= "#DEATH_VORTEX_GRENADE",
		[ eDamageSourceId.droppod_impact ] 							= "#DEATH_DROPPOD_CRUSH",
		[ eDamageSourceId.ai_turret_explosion ] 					= "#DEATH_TURRET_EXPLOSION",
		[ eDamageSourceId.rodeo_trap ] 								= "#DEATH_RODEO_TRAP",
		[ eDamageSourceId.round_end ] 								= "#DEATH_ROUND_END",
		[ eDamageSourceId.burn ]	 								= "#DEATH_BURN",
		[ eDamageSourceId.mind_crime ]								= "Mind Crime",
		[ eDamageSourceId.charge_ball ]								= "Charge Ball",
		[ eDamageSourceId.core_overload ]							= "#DEATH_CORE_OVERLOAD",
		[ eDamageSourceId.mp_weapon_arc_trap ]						= "#WPN_ARC_TRAP",

		[ eDamageSourceId.mp_turretweapon_sentry ] 					= "#WPN_SENTRY_TURRET",
		[ eDamageSourceId.mp_turretweapon_blaster ] 				= "#WPN_BLASTER_TURRET",
		[ eDamageSourceId.mp_turretweapon_rockets ] 				= "#WPN_ROCKET_TURRET",
		[ eDamageSourceId.mp_turretweapon_plasma ]	 				= "#WPN_PLASMA_TURRET",

		[ eDamageSourceId.bubble_shield ] 							= "#DEATH_BUBBLE_SHIELD",
		[ eDamageSourceId.sticky_explosive ] 						= "#DEATH_STICKY_EXPLOSIVE",
		[ eDamageSourceId.titan_grapple ] 							= "#DEATH_TITAN_GRAPPLE",

		// Instant death. Show no percentages on death recap.
		[ eDamageSourceId.fall ]		 							= "#DEATH_FALL",
		 //Todo: Rename eDamageSourceId.splat with a more appropriate name. This damage type was used for enviornmental damage, but it was for eject killing pilots if they were near a ceiling. I've changed the localized string to "Enviornment Damage", but this will cause confusion in the future.
		[ eDamageSourceId.splat ] 									= "#DEATH_SPLAT",
		[ eDamageSourceId.titan_execution ] 						= "#DEATH_TITAN_EXECUTION",
		[ eDamageSourceId.human_execution ] 						= "#DEATH_HUMAN_EXECUTION",
		[ eDamageSourceId.deathField ] 								= "#DEATH_DEATH_FIELD",
		[ eDamageSourceId.outOfBounds ] 							= "#DEATH_OUT_OF_BOUNDS",
		[ eDamageSourceId.indoor_inferno ]	 						= "#DEATH_INDOOR_INFERNO",
		[ eDamageSourceId.submerged ]								= "#DEATH_SUBMERGED",
		[ eDamageSourceId.switchback_trap ]							= "#DEATH_ELECTROCUTION", // Damages teammates and opposing team
		[ eDamageSourceId.floor_is_lava ]							= "#DEATH_ELECTROCUTION",
		[ eDamageSourceId.suicideSpectreAoE ]						= "#DEATH_SUICIDE_SPECTRE", // Used for distinguishing the initial spectre from allies.
		[ eDamageSourceId.titanEmpField ] 							= "#DEATH_TITAN_EMP_FIELD",
		[ eDamageSourceId.deadly_fog ] 								= "#DEATH_DEADLY_FOG",


		// Prototype
		[ eDamageSourceId.mp_weapon_zipline ]						= "Zipline",
		[ eDamageSourceId.at_turret_override ]						= "AT Turret",
		[ eDamageSourceId.phase_shift ]								= "#WPN_SHIFTER",
		[ eDamageSourceId.gamemode_bomb_detonation ]				= "Bomb Detonation",
		[ eDamageSourceId.bleedout ]								= "#DEATH_BLEEDOUT",
		[ eDamageSourceId.mp_weapon_energy_shotgun ]				= "Energy Shotgun",
		[ eDamageSourceId.damagedef_unknownBugIt ] 					= "#DEATH_GENERIC_KILLED",
		[ eDamageSourceId.damagedef_unknown ] 						= "#DEATH_GENERIC_KILLED",
		[ eDamageSourceId.weapon_cubemap ] 							= "#DEATH_GENERIC_KILLED",
		[ eDamageSourceId.stuck ]		 							= "#DEATH_GENERIC_KILLED",
		[ eDamageSourceId.rodeo_battery_removal ]					= "#DEATH_RODEO_BATTERY_REMOVAL",
		[ eDamageSourceId.melee_pilot_emptyhanded ] 				= "#DEATH_MELEE",
		[ eDamageSourceId.melee_pilot_arena ]		 				= "#DEATH_MELEE",
		[ eDamageSourceId.melee_pilot_sword ] 						= "#DEATH_SWORD",
		[ eDamageSourceId.melee_titan_punch ] 						= "#DEATH_TITAN_MELEE",
		[ eDamageSourceId.melee_titan_punch_ion ] 					= "#DEATH_TITAN_MELEE",
		[ eDamageSourceId.melee_titan_punch_tone ] 					= "#DEATH_TITAN_MELEE",
		[ eDamageSourceId.melee_titan_punch_northstar ] 			= "#DEATH_TITAN_MELEE",
		[ eDamageSourceId.melee_titan_punch_scorch ] 				= "#DEATH_TITAN_MELEE",
		[ eDamageSourceId.melee_titan_punch_legion ] 				= "#DEATH_TITAN_MELEE",
		[ eDamageSourceId.melee_titan_punch_fighter ]		 		= "#DEATH_TITAN_MELEE",
		[ eDamageSourceId.melee_titan_punch_vanguard ] 				= "#DEATH_TITAN_MELEE",
		[ eDamageSourceId.melee_titan_punch_stealth ] 				= "#DEATH_TITAN_MELEE",
		[ eDamageSourceId.melee_titan_punch_rocket ] 				= "#DEATH_TITAN_MELEE",
		[ eDamageSourceId.melee_titan_punch_drone ] 				= "#DEATH_TITAN_MELEE",
		[ eDamageSourceId.melee_titan_sword ]						= "#DEATH_TITAN_SWORD",
		[ eDamageSourceId.melee_titan_sword_aoe ]					= "#DEATH_TITAN_SWORD",
		[ eDamageSourceId.mp_weapon_volt_smg ] 						= "#WPN_VOLT_SMG",
		[ eDamageSourceId.mp_ability_octane_stim ] 					= "#WPN_OCTANE_STIM_SHORT",		
		[ eDamageSourceId.mp_weapon_tesla_trap ] 					= "#DEATH_TESLA_TRAP",
		[ eDamageSourceId.mp_ability_crypto_drone_emp ]				= "#WPN_DRONE_EMP",
		[ eDamageSourceId.mp_ability_crypto_drone_emp_trap ]		= "#WPN_DRONE_EMP",
		[ eDamageSourceId.melee_wraith_kunai ] 						= "#DEATH_MELEE_WRAITH_KUNAI",
		[ eDamageSourceId.mp_weapon_wraith_kunai_primary ] 			= "#DEATH_MELEE_WRAITH_KUNAI",
		[ eDamageSourceId.melee_bloodhound_axe ] 					= "#DEATH_MELEE_BLOODHOUND_AXE",
		[ eDamageSourceId.mp_weapon_bloodhound_axe_primary ] 		= "#DEATH_MELEE_BLOODHOUND_AXE",
		[ eDamageSourceId.melee_lifeline_baton ]					= "#DEATH_MELEE_LIFELINE_BATON",
		[ eDamageSourceId.mp_weapon_lifeline_baton_primary ]		= "#DEATH_MELEE_LIFELINE_BATON",
		[ eDamageSourceId.melee_shadowsquad_hands ] 				= "#DEATH_MELEE_SHADOWSQUAD_HANDS",
		[ eDamageSourceId.mp_weapon_shadow_squad_hands_primary ] 	= "#DEATH_MELEE_SHADOWSQUAD_HANDS",
		
		//r5r
		[ eDamageSourceId.melee_bolo_sword ] 						= "Bolo Sword Melee",
		[ eDamageSourceId.mp_weapon_bolo_sword_primary ] 			= "Bolo Sword Melee",
		[ eDamageSourceId.melee_boxing_ring ] 						= "Boxing Hands",
		[ eDamageSourceId.mp_weapon_melee_boxing_ring ] 			= "Boxing Hands",
		//[ eDamageSourceId.melee_data_knife ] 						= "Dataknife",
		//[ eDamageSourceId.mp_weapon_data_knife_primary ] 			= "Dataknife",
		[ eDamageSourceId.mp_weapon_throwingknife ] 				= "Throwing Knife",
		//[ eDamageSourceId.mp_weapon_satchel ] 	 					= "Satchel",
		//[ eDamageSourceId.mp_weapon_wingman_n ] 	 				= "Wingman Elite",
		[ eDamageSourceId.mp_weapon_sentinel ] 						= "Sentinel",
		//[ eDamageSourceId.mp_weapon_mobile_hmg ] 					= "Sheila",
		//[ eDamageSourceId.mp_weapon_softball ] 						= "Softball",
		//[ eDamageSourceId.mp_weapon_epg ] 							= "EPG",
		//[ eDamageSourceId.mp_weapon_smr ] 							= "Sidewinder SMR",
		//[ eDamageSourceId.mp_weapon_rocket_launcher ] 				= "Softball",
		[ eDamageSourceId.mp_weapon_car ] 							= "Car SMG",
		//[ eDamageSourceId.mp_ability_birds ] 						= "Arthur's Revenge",
		[ eDamageSourceId.mp_weapon_3030 ] 							= "30-30 Repeater",
		//[ eDamageSourceId.mp_weapon_energysword ] 					= "Energy Sword",
		[ eDamageSourceId.mp_weapon_dragon_lmg ] 					= "Rampage LMG",		
		[ eDamageSourceId.mp_weapon_smart_pistol ] 	 				= "Smart Pistol",
		[ eDamageSourceId.mp_weapon_grenade_electric_smoke ] 	 	= "Electric Smoke",
		[ eDamageSourceId.mp_weapon_grenade_gravity ] 	 			= "Gravity Star",
		[ eDamageSourceId.snd_bomb ] 	 							= "Bomb",
		[ eDamageSourceId.mp_weapon_oddball_primary ] 	 			= "Ball",
		[ eDamageSourceId.melee_oddball ] 	 						= "Ball" 
		//[ eDamageSourceId.mp_weapon_rspn101_og ] 	 				= "R101"
		//[ eDamageSourceId.sp_weapon_arc_tool] 	 					= "Arc Tool"
	}

	#if DEVELOPER
		//development, with retail versions incase a rare bug happens we dont want to show developer text
		file.damageSourceIDToName[ eDamageSourceId.damagedef_unknownBugIt ] 			= "UNKNOWN! BUG IT!"
		file.damageSourceIDToName[ eDamageSourceId.damagedef_unknown ] 				= "Unknown"
		file.damageSourceIDToName[ eDamageSourceId.weapon_cubemap ] 					= "Cubemap"
		//file.damageSourceIDToName[ eDamageSourceId.invalid ] 						= "INVALID (BUG IT!)"
		file.damageSourceIDToName[ eDamageSourceId.stuck ]		 					= "NPC got Stuck (Don't Bug it!)"
	#endif
}

void function RegisterWeaponDamageSource( string weaponRef )
{
	int sourceID = eDamageSourceId[weaponRef]
	file.damageSourceIDToName[ sourceID ] <- GetWeaponInfoFileKeyField_GlobalString( weaponRef, "shortprintname" )
	file.damageSourceIDToImage[ sourceID ] <- GetWeaponInfoFileKeyFieldAsset_Global( weaponRef, "hud_icon" )
	file.damageSourceIDToString[ sourceID ] <- weaponRef
}

bool function DamageSourceIDHasString( int index )
{
	return (index in file.damageSourceIDToString)
}

string function DamageSourceIDToString( int index )
{
	return file.damageSourceIDToString[ index ]
}

string function GetObitFromDamageSourceID( int damageSourceID )
{
	if ( damageSourceID > 0 && damageSourceID < DamageDef_GetCount() )
	{
		return DamageDef_GetObituary( damageSourceID )
	}

	if ( damageSourceID in file.damageSourceIDToName )
		return file.damageSourceIDToName[ damageSourceID ]

	table damageSourceIdEnum = expect table( getconsttable().eDamageSourceId )
	foreach ( name, id in damageSourceIdEnum )
	{
		if ( id == damageSourceID )
			return expect string( name )
	}

	return ""
}

asset function GetObitImageFromDamageSourceID( int damageSourceID )
{
	if ( damageSourceID in file.damageSourceIDToImage )
		return file.damageSourceIDToImage[ damageSourceID ]

	return $""
}

string function GetRefFromDamageSourceID( int damageSourceID )
{
	return file.damageSourceIDToString[damageSourceID]
}

string function PIN_GetDamageCause( var damageInfo )
{
	/*
	headshot
	ability
	weapon
	melee
	item
	env
	self
	bleedout
	player_stuck
	*/

	//int id = DamageInfo_GetDamageSourceIdentifier( damageInfo )

	return ""
}
