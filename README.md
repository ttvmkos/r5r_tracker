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
				
				// DISCORD WEBHOOKS:
        // For Discord webhooks help:  https://support.discord.com/hc/en-us/articles/228383668-Intro-to-Webhooks
        
        stats_discord_webhook_player_count		0	   // 1 Sends player join and disconnect data to a discord channel
				DISCORD_PLAYERS_WEBHOOK					""         //player count discord webhook link ( see help above
				stats_discord_webhook_matches_enabled	0    // 1 sends match recap data to a discord webhook channel
				DISCORD_MATCHES_WEBHOOK					""         //matches discord webhook link

        // extra
				karma_server	                  1          // for karma servers
