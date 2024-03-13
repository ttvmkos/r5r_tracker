# r5r_tracker
Stats Tracker system for R5Reloaded servers. 

Drag and drop files into your R5Reloaded folder. For: Flowstate 4.1
########################################################

NEW 3/13/2024

- General code improvements.
- Complete synced settings query system

Added persistence setting:
- start_in_rest (players start match in rest)

Added synced stat:
- score (from global tracker)

Added playlist vars:
restricted_rank  200 // int for minimum rank allowed to join restricted_server 1
give_random_legend_on_spawn             0  //  0/1
default_lock1v1_setting       1      //default setting for users without preference
enable_voice   1    //enables all_chat and enables voice convars on start (if not already enabled)

Added new cc commands:

cc sayto [player] [title] [msg] [duration]     //only needs player and title - sends message only to specified player
cc vc [0/1]  //enables or disables voice chat
cc startbr  //starts br without minimum players reached

Added to r5rdev_config.json: (playername-oid comma separated list) 
"ADMINS": ""

indev:

custom chat messages sent at configured intervals
voice / chat muting. voice overlay
challenge system



NEW 3/8/2024

Refactored/Reworked various code
Added persistence system for various settings (with stats)

- saved weapons
- lock1v1 setting
- wait time

I reccomend using a unique identifier of your choosing. Servers are identified by ip:identifier
The identifier must be alphanumeric, underscore  hypen only, less than 30 characters. identifiers that fail the check are set as 'none'


Added playlist settings:

end_match_message // 0/1 disable or enable eng game messages
endgame_delay // time in seconds 


NEW 3/7/2024

added playlist setting:  enable_lock1v1 0/1  //0 = removes panels for locking 1v1s
added panels for player wait time / lock1v1 settings

New client command: lock1v1 1/0 or on/off or true/false -- 



NEW 3/6/2024

THE CONFIG FILE TEMPLATE WAS MOVED TO THE EXAMPLE FOLDER. In order for the config file to be used it must be within your platform folder.  This move makes sure future udpates don't overwrite your saved configs.


To configure restricted servers, the following lines can be added to your playlist patch file:

// restricted servers
restricted_server 1			// 1 = restrictions enabled

restricted_join_message "Welcome to the R5R.DEV elite server. \n\n You meet the minimum requirements to play on this server."
restricted_whitelist_message "You're whitelisted for the R5R.DEV elite server"

restricted_kills 500			// minimum kills required to play
restricted_kd 0.7  			// minimum kd ratio
restricted_playtime 10000 		// minimum play time in seconds
restricted_gamesplayed 10 		// minimum games played


// END ########
Additionally, to include a whitelist of players that will always bypass any requirements, a new setting has been added to the r5rdev_config.json file:

"settings": {
        "PLAYER_WHITELIST": "1007946891142"
    }

Add a list of comma-seperated OID (uid) here. For example

"PLAYER_WHITELIST": "1007946891142,12312123,5555555555"

See the config file located in example folder
I highly reccomend adding your admins to the whitelist. 
To get a good idea of default settings for your restricted server you can always browse https://r5r.dev/hs



Commands added:

aa - players can type aa in console to see current aim assist values on server.

cc restricted 0/1 - temporarily disabled/enables restricted mode on server mid-round (wont save on reload)
cc reload_config - reloads the r5rdev_config.json file  (similar to reload_playlists)




2/1/2024:

Webhook urls can still be placed in the playlists file, 
but due to security concerns, it is highly reccomended
to use the config file in platform/r5rdev_config.JSON


THE CONFIG FILE:

apikey 
	
	will be used for special requests for future features that is tied to each host provider

identifier 

	is a label used to differentiate your servers from the same ip. This is crucial for making sure 
persistence settings a player saved goes to the correct server.  If your different servers share the same setup
you could in theory use the same identifier. NOTE: must be alpha-numeric, hyphen - and underscore only _


logfolder

	by default, logevents is where the match data is saved. You can change this here.


PLAYERS_WEBHOOK

	discord webhook url to a channel to display player join/leave updates (not reccomendedon busy servers)


MATCHES_WEBHOOK

	discord webhook url to a channel for displaying match winner recaps

MAX_LOGFILE_DIR_SIZE

	size in MB a log folder can be before the oldest are considered for deletion (applies to manual cc command as well)

AUTO_DELETE_STATLOGS

	if set to true, will delete oldest after MB threshold has been reached at match end during transition. 
 
PLAYER_WHITELIST
	
	comma separated list of user id's to bypass restricted 'elite' servers.

You may also add your own key values, and they will be parsed into an unordered map in the sdk. 
You can access these via the scripts as well using SQ_GetSetting("name_of_your_key") for example.
If a key contains another key, it is parsed into an object style syntax.

For example, in the below settings SQ_GetSetting("server.MAX_LOGFILE_DIR_SIZE") would access the value "20"

There will be an automatic file generator on r5r.dev for creating these files with ease. 

{
    "apikey": "exampleg5g504g5g85b8w4354",
    "identifier": "server1",
    "logfolder": "logevents",
    "webhooks": {
        "PLAYERS_WEBHOOK": "url",
        "MATCHES_WEBHOOK": "url"
    },
    "server": {
        "MAX_LOGFILE_DIR_SIZE": "20",
        "AUTO_DELETE_STATLOGS": "true"
    },
    "settings": {
	"PLAYER_WHITELIST": "",
	"ADMINS": "R5mkos-1007946891142"
    }
}



______________________________________________________________________
As normal, 
The following settings can be placed in your playlists_r5_patch.txt file (within fs_1v1)
______________________________________________________________________


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
        
	stats_discord_webhook_player_count		0 		// 1 Sends player join and disconnect data to a discord channel // CONFIGURE URL IN platform/r5rdev_config.JSON
	stats_discord_webhook_matches_enabled	0 		// 1 sends match recap data to a discord webhook channel // CONFIGURE URL IN platform/r5rdev_config.JSON


	// restricted servers
	restricted_server 0			// 1 = restrictions enabled
	restricted_kick_log 0		// 1 = prints kick msg to console
	
	restricted_join_message "Welcome to the R5R.DEV elite server. \n\n You meet the minimum requirements to play on this server."
	restricted_whitelist_message "You're whitelisted for the R5R.DEV elite server"
	
	restricted_kills 500			// minimum kills required to play
	restricted_kd 0.7  			// minimum kd ratio
	restricted_playtime 10000 		// minimum play time in seconds
	restricted_gamesplayed 10 		// minimum games played

	// extra
	karma_server	                  1          // for karma servers
	custom_match_ending_title		"Match complete"	 //custom title before map reload/cycle
	custom_match_ending_message		"Don't leave. Server is going to change maps."	 //custom msg before map reload/cycle	
	enable_lock1v1						1 // 0 removes all lock fight panels
	default_lock1v1_setting					1 // 1 = allow locking.  default setting for player preference
	end_match_message			// 0/1 disable or enable eng game messages
	endgame_delay 				// time in seconds 
	give_random_legend_on_spawn             1  // gives both opponents same random legend
	default_start_in_rest_setting			0
	enable_voice				1
	
	//string limit in playlist is 1000 bytes. Use continue to extend list respectively

	custom_1v1_weapons_primary "" //comma separated weapons list for 1v1
	custom_1v1_weapons_primary_continue ""

	custom_1v1_weapons_secondary "" //comma separated weapons list for 1v1
	custom_1v1_weapons_secondary_continue "" 
	
	//SBMM  score =  ( lifetime kd * lifetime_kd_weight ) + ( current match kd * current_kd_weight )

	lifetime_kd_weight 	0.9  // multiplies global kd stat
	current_kd_weight 	1.3	 // multiples current kd stat
	kd_difference		1.5  // threshold matchmaking excludes by (differnce of 1.5 = score of 4 wont match with score of 2.4 )
