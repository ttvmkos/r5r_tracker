# r5r_tracker
Stats Tracker system for R5Reloaded servers. 

Drag and drop files into your R5Reloaded folder. For: Flowstate 4.1


The following settings can be placed in your playlists_r5_patch.txt file (within fs_1v1)


	// mkos systems

	logging_enabled 						1 //enables logging system
	logging_encryption						1 //encrypts data for transfer (reccomended)
	logging_shipstats 						1 // connects to tracker server to keep player stats up to date (only verified servers count towards leaderboard)
	default_ibmm_wait						5 //0 for instant queue any input
	ibmm_wait_limit							15 //maximum wait time allowed to match input
	lg_duel_mode_60p						0  //enables beta lg mode
				
	rest_msg								1 // displays block message during rest with helpful commands information
	afk_to_rest_bool						1 // makes player rest when afk instead of kick
	enable_ping_kick						0 // 0 disables ping kick for this mode
	enable_afk_thread 						1 //0 disables afk thread for this mode
	superglide_counter_msg					1 // Shows players' superglide counts
	custom_superglide_msg					"Kewl beans" //shows custom msg under counter.
	patch_for_dropoff						1 // enables extra locations for dropoff allowing for 32 max players
	patch_waiting_area						1 //makes a barrier between waiting and fighting areas
	
	
	//ADMINS for cc command
	cc_administration						1 // enables client command administration	
	
	// usage: "adminname-oid,anotheradmin-oid,thirdadmin-01010101010"
	admins_list "R5mkos-1007946891142"

	//stats system hooks
	use_global_stats						1 //loads players global stats, uses global player stats for matchmaking in 1v1
		
	// DISCORD WEBHOOKS:
	// For Discord webhooks help:  https://support.discord.com/hc/en-us/articles/228383668-Intro-to-Webhooks
        
	stats_discord_webhook_player_count		0	   // 1 Sends player join and disconnect data to a discord channel
	DISCORD_PLAYERS_WEBHOOK					""         //player count discord webhook link ( see help above
	stats_discord_webhook_matches_enabled	0    // 1 sends match recap data to a discord webhook channel
	DISCORD_MATCHES_WEBHOOK					""         //matches discord webhook link

	// extra
	karma_server	                  1          // for karma servers
	custom_match_ending_title		"Match complete"	 //custom title before map reload/cycle
	custom_match_ending_message		"Don't leave. Server is going to change maps."	 //custom msg before map reload/cycle		
	
	//string limit in playlist is 1000 bytes. Use continue to extend list respectively

	custom_1v1_weapons_primary "" //comma separated weapons list for 1v1
	custom_1v1_weapons_primary_continue ""

	custom_1v1_weapons_secondary "" //comma separated weapons list for 1v1
	custom_1v1_weapons_secondary_continue "" 
