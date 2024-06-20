!!!NOTICE!!!

SUPPORT FOR THIS REPO HAS ENDED. 
[No updates or patches will be provided]

Support for tracker has been implemented into main flowstate branch 
and will be optional with plugin or dlls 



# r5r_tracker
Stats Tracker system for R5Reloaded servers. 

Drag and drop files into your R5Reloaded folder. For: Flowstate 4.1

______________________________________________________________


If you are a verified host, request verified status on https://r5r.dev
This will give you access to your control panel, where you can generate your api key, and create your config file.

______________________________________________________________


THE CONFIG FILE:

apikey 
	
	will be used for special requests for future features that is tied to each host provider

identifier 

	is a label used to differentiate your servers from the same ip. This is crucial for making sure 
persistence settings a player saved goes to the correct server.  If your different servers share the same setup
you could in theory use the same identifier. NOTE: must be alpha-numeric, hyphen - and underscore only _


logfolder

	by default, logevents is where the match data is saved. You can change this here.

webhooks:
PLAYERS_WEBHOOK

	discord webhook url to a channel to display player join/leave updates (not reccomendedon busy servers)


MATCHES_WEBHOOK

	discord webhook url to a channel for displaying match winner recaps
server:
MAX_LOGFILE_DIR_SIZE

	size in MB a log folder can be before the oldest are considered for deletion (applies to manual cc command as well)

AUTO_DELETE_STATLOGS

	if set to true, will delete oldest after MB threshold has been reached at match end during transition. 

DELETE_ALL_LOGS"

	if set to true, will delete all logs in specified folder after match is over and data transmits

CVAR_MAX_BUFFER

	controls the amount of log events to queue before being wrote from memory to file

settings: 
PLAYER_WHITELIST
	
	comma separated list of user id's to bypass restricted 'elite' servers.

ADMINS

	comma separated list of EAusername-uid separated players

ENABLE_CHATBOT
        
	if set to true, enables server chat bot

CHATBOT_PREFIX 

	sets the name the serverbot is prefixed with. SERVER, would result in the chatbot being named [SERVER]

INTERVAL_MESSAGES

	configure messages based on events and interval seconds. syntax is   interval= message |interval= message
	If a message exceeds 255 characters, it will be truncated at the 255th character and sent anyway.

	An example configuration where 2 start messages, 1 interval in seconds, and 1 end message are configured is:

	start=Welcome to server |start= Enjoy |60= This message repeats every 60 seconds |end= The game is over, hope it was good!

LOOP_MESSAGES_INTERVAL

	Similar to interval messages, loop messages works by displaying a list of messages 1 by 1 every x seconds (intervals of 10)

LOOP_MESSAGES

	A list of pipe | separated messages to display 1 at a time, in order. Once the last message is reached, the list repeats. 




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
        "AUTO_DELETE_STATLOGS": true,
        "DELETE_ALL_LOGS": true,
        "CVAR_MAX_BUFFER": 100
    },
    "settings": {
	"PLAYER_WHITELIST": "",
	"ADMINS": "R5mkos-1007946891142",
       	"ENABLE_CHATBOT": "true",
        	"CHATBOT_PREFIX": "SERVER",
        	"INTERVAL_MESSAGES": "start=Welcome to the server. |start= This is a test dispay message |60=This message displays once per minute. |end= This message displays at the end. ",
        "LOOP_MESSAGES_INTERVAL": "10",
        "LOOP_MESSAGES": "Loopity loop loop at 10 and 50|another loop|  yet another at 30 seconds| this one is 40"
    }
}


______________________________________________________________________
The following should and can be placed at a global level in playlist file :
______________________________________________________________________

- sv_autoReloadRate 9000 //time in seconds to auto refresh server




______________________________________________________________________
As normal, 
The following settings can be placed in your playlists_r5_patch.txt file (within fs_1v1)
______________________________________________________________________


	// mkos systems

	logging_enabled 						1 //enables logging system
	logging_encryption						1 //encrypts data for transfer (reccomended)
	logging_shipstats 						1 // connects to tracker server to keep player stats up to date (only verified servers count towards leaderboard)
	default_ibmm_wait						5 //0 for instant queue any input (reccomend 0 as a default)
	ibmm_wait_limit						15 //maximum wait time allowed to match input
	lg_duel_mode_60p						0  //enables beta lg mode
				
	rest_msg							1 // displays block message during rest with helpful commands information
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
	maplist "mp_rr_arena_composite,mp_rr_aqueduct,mp_rr_party_crasher"
	rotate_map 			1 // rotatesmaps in maplist, to add, edit maplist above
	custom_match_ending_title		"Match complete"	 //custom title before map reload/cycle
	custom_match_ending_message		"Don't leave. Server is going to change maps."	 //custom msg before map reload/cycle	
	enable_lock1v1						1 // 0 removes all lock fight panels
	default_lock1v1_setting					1 // 1 = allow locking.  default setting for player preference
	end_match_message			// 0/1 disable or enable eng game messages
	endgame_delay 				// time in seconds 
	give_random_legend_on_spawn             1  // gives both opponents same random legend ( all realms )
	default_start_in_rest_setting			0
	enable_voice				1
	default_enable_input_banner_setting     0  ///disables spammy player input banner on spawn
	COMMAND_RATE_LIMIT		0.200 //Rate limiting for all client commands
	enable_chat_commands		1 // allows typing /rest /info /id /aa  into chat. 
	
	//challenges
	enable_challenge   			1  // players can use challenge system , 0 for disabled
	give_legend_tactical		1 //gives player tactical ability for legend on spawn (challenge realm only)
	give_legend 			1 // allows giving legend ( during challenge only )
	challenge_recap_server_message	1 //enables quick kill/death recap msg in chat (requires chat bot enabled in r5rdev_config.json)


	rest_grace 			5.0 // time required from start of fight 
					// or since last damage to rest during a fight 
					// if player tries to rest, it will send them to rest after fight 
					// and tell them they can try again in (n) seconds. 
	
	//string limit in playlist is 1000 bytes. Use continue to extend list respectively

	custom_1v1_weapons_primary "" //comma separated weapons list for 1v1
	custom_1v1_weapons_primary_continue ""

	custom_1v1_weapons_secondary "" //comma separated weapons list for 1v1
	custom_1v1_weapons_secondary_continue "" 
	
	//SBMM  score =  ( lifetime kd * lifetime_kd_weight ) + ( current match kd * current_kd_weight )

	lifetime_kd_weight 	0.9  // multiplies global kd stat
	current_kd_weight 	1.3	 // multiples current kd stat
	kd_difference		1.5  // threshold matchmaking excludes by (differnce of 1.5 = score of 4 wont match with score of 2.4 )
