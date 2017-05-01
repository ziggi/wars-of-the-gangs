/*
*	Created:			04.06.10
*	Author:				009
*   Description:        Wars of the Gangs - TDM мод с RPG элементами.Присутствуют соревнования: миссии, мини-миссии, командные задания, гонки, дезматчи.
*                       Так же присутствует множество телепортов.Множество мест для станта.Система прокачки персонажей.
*                       Игроки могут улучшать свои скиллы оружия, увеличить максимальное количество жизни(начальное 100, максимум 150), увеличить регенерацию.
*                       Присутствует недвижимость.Бизнесы и дома.Дом можно выбрать местом спавна.С бизнесов идут деньги в банк.
*                       В банке можно хранить деньги.
*                       Главная черта мода - создание банд и война между ними.Есть зоны банд,любая банда может захватить зону.Если игроков владеющей банды нет на сервере то зона не может быть захвачена.
*                       При удалении банды зона становится нейтральной.Если владельцы банды очень давно не были на сервере то зона может быть захвачена.Если вы в зоне то над GPS пишется банда - владелец.
*/
/*

TODO:
	доделать сис-му оружия, /w drop

*/

//standart include
#include <a_samp>
// cmd parser
#define RegisterCmd(%1,%2,%3,%4,%5) if(!strcmp(%1,%2,true,%3) && ((%1[%3] == ' ') || (%1[%3] == 0))) return %4_Command(playerid,%5,%1[%3 + 1])
// text parser
#define RegisterText(%1,%2,%3,%4)	if(%1[0] == %2) if(%3_Text(playerid,%4,%1[1])) return 0
// dialog caller
#define RegisterDialog(%1,%2,%3,%4,%5,%6) case %6: return %5_Dialog(%1,%6,%2,%3,%4)

// global core defines
#define MAX_TMP_STRING_SIZE			1024
#define MAX_TMP_INT_SIZE			4
#define MAX_TMP_FLOAT_SIZE      	6
#define GetPlayersCount()           pcount

// global core enumerations
enum
{
	DIALOG_NONE_ACTION,
	DIALOG_LOGIN,
	DIALOG_REGISTER,
	DIALOG_TELEPORTS,
	DIALOG_DEATHMATCHES,
	DIALOG_RACES,
	DIALOG_GANG_INVITE,
	DIALOG_TEAM_QUESTS,
	DIALOG_TEAM_QUEST_TEAM,
	DIALOG_GANG_WAR,
	DIALOG_GANG_WAR_WEAPONS,
	DIALOG_MISSION_BANK,
	DIALOG_WEAPONS,
	DIALOG_UPGRADE
};
enum (+=10_000)
{
	DEATHMATCH_VIRTUAL_WORLD = 10_000,
	RACE_VIRTUAL_WORLD,
	TEAM_QUEST_VIRTUAL_WORLD,
	GANG_WAR_VIRTUAL_WORLD,
	STUNT_VIRTUAL_WORLD
};

// global core vars
stock stmp[MAX_TMP_STRING_SIZE];
stock itmp[MAX_TMP_INT_SIZE];
stock Float:ftmp[MAX_TMP_FLOAT_SIZE];
stock PlayerOnPickup[MAX_PLAYERS];
stock pcount;

// forwards
forward Update();
forward Check();

// mode info
#define MODE_NAME	            	"Wars of the Gangs"
#define MODE_VERSION            	"1.0 beta 3"
#define MODE_DIR                	"WoG/"
// settings
#define PRINT_STATS_DATA
#define SHOW_KILL_STAT
#define ON_PLAYER_UPDATE_STREAMERS
#define TIMESTAMP_IN_LOG
#define CJ_ANIMATIONS
#define DISABLE_STUNT_BONUS
//#define INTERIORS_DISABLE
//#define USE_ANTISLEEP_PLUGIN
#define VEHICLE_HEALTH_CHANGE_SPY
//#define PLAYER_HEALTH_CHANGE_SPY
//#define PLAYER_ARMOUR_CHANGE_SPY
//#define PLAYER_WEAPON_CHANGE_SPY
#define PLAYER_SPECIALACTION_CHANGE_SPY
#define SYSTEM_COLOR                0xFFFFFFFF
#define MAX_STRING                  128
#define PLAYERS_COLOR				0xAAAAAAAA

//gamemode includes
#tryinclude "colors"
#tryinclude "utils"
#tryinclude "zones"
#tryinclude "markers"
#tryinclude "streamer_objects"
#tryinclude "streamer_checkpoints"
#tryinclude "streamer_icons"
#tryinclude "quest"
#tryinclude "protect"
#tryinclude "player"
#tryinclude "world"
#tryinclude "admin"
#tryinclude "houses"
#tryinclude "businesses"
#tryinclude "minimissions"
#tryinclude "bank"
#tryinclude "deathmatch"
#tryinclude "race"
#tryinclude "gangs"
#tryinclude "team_quests"
#tryinclude "missions"
#tryinclude "weapons"
// plugins
#if defined USE_ANTISLEEP_PLUGIN
	#tryinclude "plugins/antisleep"
#endif

main() {}

// -----------------------------------------------------------------------------
//                                  Standart SA:MP's callbacks
// -----------------------------------------------------------------------------

public OnGameModeInit()
{
	AllowAdminTeleport(1);
    format(stmp,sizeof(stmp),"Loading version %s",MODE_VERSION);
    SetGameModeText(stmp);
	// start load
	print("----------------------------------");
	print("Running " MODE_NAME " version " MODE_VERSION);
	print("Created by:	009");
	// load all systems
	// protect init
#if defined _protect_included
    Protect_OnGameModeInit();
#endif
	// objects streamer init
#if defined _streamer_objects_included
    StreamerObj_OnGameModeInit();
#endif
	// checkpoints streamer init
#if defined _streamer_checkpoints_included
    StreamerCP_OnGameModeInit();
#endif
	// icons streamer init
#if defined _streamer_icons_included
    StreamerIcon_OnGameModeInit();
#endif
	// players handler init
#if defined _player_included
    Player_OnGameModeInit();
#endif
	// world init
#if defined _world_included
    World_OnGameModeInit();
#else
    AddPlayerClass(0,0.0,0.0,0.0,0.0,0,0,0,0,0,0);
#endif
	// houses init
#if defined _houses_included
    Houses_OnGameModeInit();
#endif
	// businesses init
#if defined _businesses_included
    Businesses_OnGameModeInit();
#endif
	// admin init
#if defined _admin_included
	Admin_OnGameModeInit();
#endif
	// mini missions
#if defined _minimissions_included
    MiniMissios_OnGameModeInit();
#endif
	// banks init
#if defined _banks_included
    Banks_OnGameModeInit();
#endif
	// deathmatch init
#if defined _deathmatch_included
    Deathmatch_OnGameModeInit();
#endif
	// race init
#if defined _race_included
    Race_OnGameModeInit();
#endif
	// gang init
#if defined _gangs_included
    Gangs_OnGameModeInit();
#endif
	// team quest init
#if defined _team_quests_included
    TeamQuest_OnGameModeInit();
#endif
	// mission init
#if defined _missions_included
    Missions_OnGameModeInit();
#endif
	// weapon init
#if defined _weapons_included
	Weapons_OnGameModeInit();
#endif
	// timers
	SetTimer("Update",1000,1);
	SetTimer("Check",100,1);
	// settings
#if defined CJ_ANIMATIONS
	UsePlayerPedAnims();
#endif
#if defined INTERIORS_DISABLE
	DisableInteriorEnterExits();
#endif
	// set real gamemode text
	SetGameModeText(MODE_NAME " " MODE_VERSION);
	// print completed data
	print(MODE_NAME " version " MODE_VERSION " loaded.");
	print("----------------------------------");
}

public OnGameModeExit()
{
#if defined _gangs_included
    Gangs_OnGameModeExit();
#endif
	print(MODE_NAME " version " MODE_VERSION " unloaded.");
}

public OnPlayerConnect(playerid)
{
    if(IsPlayerNPC(playerid)) return 1;
	IsPlayerSpawned{playerid} = 0; // fix for Player_Update()  (death on spawn)
	// counter
    if(pcount < playerid) pcount = playerid;
    // pickups
    PlayerOnPickup[playerid] = 0;
    
	// systems
#if defined _streamer_objects_included
    StreamerObj_OnPlayerConnect(playerid);
#endif
#if defined _streamer_checkpoints_included
    StreamerCP_OnPlayerConnect(playerid);
#endif
#if defined _streamer_icons_included
    StreamerIcon_OnPlayerConnect(playerid);
#endif
#if defined _player_included
    Player_OnPlayerConnect(playerid);
#endif
#if defined _houses_included
    Houses_OnPlayerConnect(playerid);
#endif
#if defined _quest_included
    Quest_OnPlayerConnect(playerid);
#endif
#if defined _minimissions_included
    MiniMissions_OnPlayerConnect(playerid);
#endif
#if defined _deathmatch_included
    Deathmatch_OnPlayerConnect(playerid);
#endif
#if defined _race_included
    Race_OnPlayerConnect(playerid);
#endif
#if defined _gangs_included
    Gangs_OnPlayerConnect(playerid);
#endif
#if defined _team_quests_included
    TeamQuest_OnPlayerConnect(playerid);
#endif

	return 1;
}

public OnPlayerDisconnect(playerid,reason)
{
    if(IsPlayerNPC(playerid)) return 1;
	IsPlayerSpawned{playerid} = 0; // fix for Player_Update()  (death on spawn)
	// counter
    if(pcount == playerid)
	{
		do pcount--;
		while((IsPlayerNPC(playerid) || (IsPlayerConnected(pcount) == 0)) && (pcount > 0));
	}
    // pickups
    PlayerOnPickup[playerid] = 0;
	
	// systems
#if defined _streamer_objects_included
    StreamerObj_OnPlayerDisconnect(playerid);
#endif
#if defined _streamer_checkpoints_included
    StreamerCP_OnPlayerDisconnect(playerid);
#endif
#if defined _streamer_icons_included
    StreamerIcon_OnPlayerDisconnect(playerid);
#endif
#if defined _player_included
    Player_OnPlayerDisconnect(playerid,reason);
#endif
#if defined _minimissions_included
    MiniMissions_OnPlayerDisconnect(playerid,reason);
#endif
#if defined _deathmatch_included
    Deathmatch_OnPlayerDisconnect(playerid,reason);
#endif
#if defined _race_included
    Race_OnPlayerDisconnect(playerid,reason);
#endif
#if defined _team_quests_included
    TeamQuest_OnPlayerDisconnect(playerid,reason);
#endif
#if defined _missions_included
    Missions_OnPlayerDisconnect(playerid,reason);
#endif
#if defined _quest_included
    Quest_OnPlayerDisconnect(playerid,reason);
#endif
	return 1;
}

public OnPlayerSpawn(playerid)
{
#if defined DISABLE_STUNT_BONUS
	EnableStuntBonusForPlayer(playerid,false);
#endif
	// systems
#if defined _player_included
    Player_OnPlayerSpawn(playerid);
#endif
#if defined _houses_included
    Houses_OnPlayerSpawn(playerid);
#endif
#if defined _deathmatch_included
    Deathmatch_OnPlayerSpawn(playerid);
#endif
#if defined _gangs_included
    Gangs_OnPlayerSpawn(playerid);
#endif
#if defined _team_quests_included
    TeamQuest_OnPlayerSpawn(playerid);
#endif
	IsPlayerSpawned{playerid} = 1; // fix for Player_Update()  (death on spawn)
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
	IsPlayerSpawned{playerid} = 0; // fix for Player_Update()  (death on spawn)
	// kill chat
#if defined SHOW_KILL_STAT
    SendDeathMessage(killerid,playerid,reason);
#endif
	// systems
#if defined _protect_included
    Protect_OnPlayerDeath(playerid,killerid,reason);
#endif
#if defined _player_included
    Player_OnPlayerDeath(playerid,killerid,reason);
#endif
#if defined _deathmatch_included
    Deathmatch_OnPlayerDeath(playerid,killerid,reason);
#endif
#if defined _race_included
    Race_OnPlayerDeath(playerid,killerid,reason);
#endif
#if defined _gangs_included
    Gangs_OnPlayerDeath(playerid,killerid,reason);
#endif
#if defined _team_quests_included
    TeamQuest_OnPlayerDeath(playerid,killerid,reason);
#endif
#if defined _missions_included
    Missions_OnPlayerDeath(playerid,killerid,reason);
#endif
	return 1;
}

public OnVehicleSpawn(vehicleid)
{
	return 1;
}

public OnVehicleDeath(vehicleid, killerid)
{
	return 1;
}

public OnPlayerText(playerid, text[])
{
	if(GetPVarInt(playerid,"Logged") != 1)
	{
		SendClientMessage(playerid,COLOR_RED,"Вы должны войти под своим аккаунтом что-бы писать в чат!");
		return 0;
	}
	// admin
#if defined _admin_included
    RegisterText(text,'@',Admin,ADMIN_REPORT_TEXT);
	if(GetPVarInt(playerid,"IsMuted")) return 0;
#endif
    // systems
#if defined _deathmatch_included
    RegisterText(text,'*',Deathmatch,DEATHMATCH_QUEST_TEXT);
#endif
#if defined _race_included
    RegisterText(text,'*',Race,RACE_QUEST_TEXT);
#endif
#if defined _gangs_included
    RegisterText(text,'!',Gangs,GANGS_TEAM_TEXT);
#endif
#if defined _team_quests_included
    RegisterText(text,'*',TeamQuest,TEAM_QUEST_TEAM_TEXT);
    RegisterText(text,'&',TeamQuest,TEAM_QUEST_ANSWER_TEXT);
#endif

	return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
	if(GetPVarInt(playerid,"Logged") != 1)
		return SendClientMessage(playerid,COLOR_RED,"Вы должны войти под своим аккаунтом что-бы использовать эту команду!");
    // systems
#if defined _player_included
	RegisterCmd(cmdtext,"/stats",6,Player,PLAYER_STAT_CMD);
	RegisterCmd(cmdtext,"/help",5,Player,PLAYER_HELP_CMD);
	RegisterCmd(cmdtext,"/savechar",9,Player,PLAYER_SAVECHAR_CMD);
	RegisterCmd(cmdtext,"/changepass",11,Player,PLAYER_CHANGEPASS_CMD);
	RegisterCmd(cmdtext,"/kill",5,Player,PLAYER_KILL_CMD);
	RegisterCmd(cmdtext,"/credits",8,Player,PLAYER_CREDITS_CMD);
	RegisterCmd(cmdtext,"/givecash",9,Player,PLAYER_GIVECASH_CMD);
	RegisterCmd(cmdtext,"/flip",5,Player,PLAYER_FLIP_CMD);
	RegisterCmd(cmdtext,"/upgrade",8,Player,PLAYER_UPGRADE_CMD);
	RegisterCmd(cmdtext,"/pm",3,Player,PLAYER_PM_CMD);
#endif
#if defined _world_included
    RegisterCmd(cmdtext,"/tp",3,World,WORLD_TELEPORT_CMD);
#endif
#if defined _houses_included
    RegisterCmd(cmdtext,"/h help",7,Houses,HOUSES_HELP_CMD);
    RegisterCmd(cmdtext,"/h info",7,Houses,HOUSES_INFO_CMD);
    RegisterCmd(cmdtext,"/h buy",6,Houses,HOUSES_BUY_CMD);
    RegisterCmd(cmdtext,"/h sell",7,Houses,HOUSES_SELL_CMD);
    RegisterCmd(cmdtext,"/h spawn",8,Houses,HOUSES_SPAWNSEL_CMD);
	RegisterCmd(cmdtext,"/h free",7,Houses,ADMIN_FREEHOUSE_CMD);
#endif
#if defined _admin_included
	RegisterCmd(cmdtext,"/cmdlist",8,Admin,ADMIN_CMDLIST_CMD);
	RegisterCmd(cmdtext,"/ssay",5,Admin,ADMIN_SSAY_CMD);
	RegisterCmd(cmdtext,"/boom",5,Admin,ADMIN_BOOM_CMD);
	RegisterCmd(cmdtext,"/setlvl",7,Admin,ADMIN_SETLVL_CMD);
	RegisterCmd(cmdtext,"/disarm",7,Admin,ADMIN_DISARM_CMD);
	RegisterCmd(cmdtext,"/remmoney",9,Admin,ADMIN_REMMONEY_CMD);
	RegisterCmd(cmdtext,"/agivemoney",11,Admin,ADMIN_GIVEMONEY_CMD);
	RegisterCmd(cmdtext,"/setstatus",10,Admin,ADMIN_SETSTATUS_CMD);
	RegisterCmd(cmdtext,"/ban",4,Admin,ADMIN_BAN_CMD);
	RegisterCmd(cmdtext,"/statuses",9,Admin,ADMIN_STATUSES_CMD);
	RegisterCmd(cmdtext,"/say",4,Admin,ADMIN_SAY_CMD);
	RegisterCmd(cmdtext,"/setskin",8,Admin,ADMIN_SETSKIN_CMD);
	RegisterCmd(cmdtext,"/kick",5,Admin,ADMIN_KICK_CMD);
	RegisterCmd(cmdtext,"/akill",6,Admin,ADMIN_AKILL_CMD);
	RegisterCmd(cmdtext,"/jail",5,Admin,ADMIN_JAIL_CMD);
	RegisterCmd(cmdtext,"/unjail",7,Admin,ADMIN_UNJAIL_CMD);
	RegisterCmd(cmdtext,"/paralyze",9,Admin,ADMIN_PARALYZE_CMD);
	RegisterCmd(cmdtext,"/deparalyze",11,Admin,ADMIN_DEPARALYZE_CMD);
	RegisterCmd(cmdtext,"/mute",5,Admin,ADMIN_MUTE_CMD);
	RegisterCmd(cmdtext,"/unmute",7,Admin,ADMIN_UNMUTE_CMD);
	RegisterCmd(cmdtext,"/tele-to",8,Admin,ADMIN_TELETO_CMD);
	RegisterCmd(cmdtext,"/tele-here",10,Admin,ADMIN_TELEHERE_CMD);
	RegisterCmd(cmdtext,"/teleport",9,Admin,ADMIN_TELEPORT_CMD);
	RegisterCmd(cmdtext,"/upoint",7,Admin,ADMIN_UPGRADEPOINT_CMD);
#endif
#if defined _businesses_included
    RegisterCmd(cmdtext,"/b help",7,Businesses,BUSINESS_HELP_CMD);
    RegisterCmd(cmdtext,"/b info",7,Businesses,BUSINESS_INFO_CMD);
    RegisterCmd(cmdtext,"/b buy",6,Businesses,BUSINESS_BUY_CMD);
    RegisterCmd(cmdtext,"/b sell",7,Businesses,BUSINESS_SELL_CMD);
#endif
#if defined _banks_included
    RegisterCmd(cmdtext,"/bank help",10,Banks,BANKS_HELP_CMD);
    RegisterCmd(cmdtext,"/deposite",9,Banks,BANKS_DEPOSITE_CMD);
    RegisterCmd(cmdtext,"/withdraw",9,Banks,BANKS_WITHDRAW_CMD);
#endif
#if defined _deathmatch_included
	RegisterCmd(cmdtext,"/dm help",8,Deathmatch,DEATHMATCH_HELP_CMD);
	RegisterCmd(cmdtext,"/dm menu",8,Deathmatch,DEATHMATCH_MENU_CMD);
	RegisterCmd(cmdtext,"/dm quit",8,Deathmatch,DEATHMATCH_QUIT_CMD);
#endif
#if defined _race_included
	RegisterCmd(cmdtext,"/race help",10,Race,RACE_HELP_CMD);
	RegisterCmd(cmdtext,"/race menu",10,Race,RACE_MENU_CMD);
	RegisterCmd(cmdtext,"/race quit",10,Race,RACE_QUIT_CMD);
#endif
#if defined _gangs_included
    RegisterCmd(cmdtext,"/g help",7,Gangs,GANGS_HELP_CMD);
    RegisterCmd(cmdtext,"/g create",9,Gangs,GANGS_CREATE_CMD);
    RegisterCmd(cmdtext,"/g delete",9,Gangs,GANGS_DELETE_CMD);
    RegisterCmd(cmdtext,"/g colors",9,Gangs,GANGS_COLORS_CMD);
    RegisterCmd(cmdtext,"/g color",8,Gangs,GANGS_COLOR_CMD);
    RegisterCmd(cmdtext,"/g welcome",10,Gangs,GANGS_WELCOME_CMD);
    RegisterCmd(cmdtext,"/g invite",9,Gangs,GANGS_INVITE_CMD);
    RegisterCmd(cmdtext,"/g quit",7,Gangs,GANGS_QUIT_CMD);
    RegisterCmd(cmdtext,"/g kick",7,Gangs,GANGS_KICK_CMD);
    RegisterCmd(cmdtext,"/g members",10,Gangs,GANGS_MEMBERS_CMD);
    RegisterCmd(cmdtext,"/g stats",8,Gangs,GANGS_STATS_CMD);
    RegisterCmd(cmdtext,"/gz help",8,Gangs,GANGS_ZONE_HELP_CMD);
    RegisterCmd(cmdtext,"/gz info",8,Gangs,GANGS_ZONE_INFO_CMD);
    RegisterCmd(cmdtext,"/gw help",8,Gangs,GANGS_WAR_HELP_CMD);
    RegisterCmd(cmdtext,"/gw attack",10,Gangs,GANGS_WAR_ATTACK_CMD);
    RegisterCmd(cmdtext,"/gw join",8,Gangs,GANGS_WAR_JOIN_CMD);
    RegisterCmd(cmdtext,"/gw leave",9,Gangs,GANGS_WAR_LEAVE_CMD);
    RegisterCmd(cmdtext,"/gw start",9,Gangs,GANGS_WAR_START_CMD);
#endif
#if defined _team_quests_included
	RegisterCmd(cmdtext,"/tq help",8,TeamQuest,TEAM_QUEST_HELP_CMD);
	RegisterCmd(cmdtext,"/tq menu",8,TeamQuest,TEAM_QUEST_MENU_CMD);
	RegisterCmd(cmdtext,"/tq quit",8,TeamQuest,TEAM_QUEST_QUIT_CMD);
#endif
#if defined _missions_included
	RegisterCmd(cmdtext,"/dget",5,Missions,MISSIONS_DGET_CMD);
	RegisterCmd(cmdtext,"/dset",5,Missions,MISSIONS_DSET_CMD);
	RegisterCmd(cmdtext,"/dboom",6,Missions,MISSIONS_DBOOM_CMD);
#endif
#if defined _weapons_included
	RegisterCmd(cmdtext,"/w help",7,Weapons,WEAPONS_HELP_CMD);
	RegisterCmd(cmdtext,"/w menu",7,Weapons,WEAPONS_MENU_CMD);
	RegisterCmd(cmdtext,"/w drop",7,Weapons,WEAPONS_DROP_CMD);
#endif
	return SendClientMessage(playerid,COLOR_WHITE,"SERVER: Неизвестная команда");
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	// systems
#if defined _missions_included
    Missions_OnPlayerEnterVehicle(playerid,vehicleid,ispassenger);
#endif

	return 1;
}

public OnPlayerExitVehicle(playerid, vehicleid)
{
	return 1;
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	return 1;
}

public OnPlayerEnterCheckpoint(playerid)
{
	// systems
#if defined _streamer_checkpoints_included
    StreamerCP_OnPlayerEnterCP(playerid);
#endif
#if defined _missions_included
    Missions_OPEC(playerid);
#endif

	return 1;
}

public OnPlayerLeaveCheckpoint(playerid)
{
	return 1;
}

public OnPlayerEnterRaceCheckpoint(playerid)
{
	// systems
#if defined _race_included
	Race_OnPlayerEnterRaceCP(playerid);
#endif
#if defined _missions_included
    Missions_OPERC(playerid);
#endif
	return 1;
}

public OnPlayerLeaveRaceCheckpoint(playerid)
{
	return 1;
}

public OnRconCommand(cmd[])
{
	return 1;
}

public OnPlayerRequestClass(playerid,classid)
{
	// systems
#if defined _world_included
    World_OnPlayerRequestClass(playerid,classid);
#endif
	return 1;
}

public OnObjectMoved(objectid)
{
	return 1;
}

public OnPlayerObjectMoved(playerid, objectid)
{
	return 1;
}

public OnPlayerPickUpPickup(playerid, pickupid)
{
    // pickups
    if((GetTickCount() - PlayerOnPickup[playerid]) < 1000)
	{
	    PlayerOnPickup[playerid] = GetTickCount();
		return 1;
	}
    PlayerOnPickup[playerid] = GetTickCount();
    
    // systems
#if defined _houses_included
    Houses_OnPlayerPickUpPickup(playerid,pickupid);
#endif
#if defined _businesses_included
    Businesses_OnPlayerPickUpPickup(playerid,pickupid);
#endif
#if defined _banks_included
    Banks_OnPlayerPickUpPickup(playerid,pickupid);
#endif
#if defined _missions_included
    Missions_OnPlayerPickUpPickup(playerid,pickupid);
#endif
#if defined _missions_included
	Weapons_OnPlayerPickUpPickup(playerid,pickupid);
#endif
	return 1;
}

public OnVehicleMod(playerid, vehicleid, componentid)
{
	return 1;
}

public OnVehiclePaintjob(playerid, vehicleid, paintjobid)
{
	return 1;
}

public OnVehicleRespray(playerid, vehicleid, color1, color2)
{
	return 1;
}

public OnPlayerSelectedMenuRow(playerid, row)
{
	return 1;
}

public OnPlayerExitedMenu(playerid)
{
	return 1;
}

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
	return 1;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	return 1;
}

public OnRconLoginAttempt(ip[], password[], success)
{
	return 1;
}

public OnPlayerUpdate(playerid)
{
	if(GetPlayerVirtualWorld(playerid) == STUNT_VIRTUAL_WORLD)
	{
		SetPlayerArmedWeapon(playerid,0);
		oSetPlayerHealth(playerid,100.0);
		if(IsPlayerInAnyVehicle(playerid)) RepairVehicle(GetPlayerVehicleID(playerid));
	}
	// protect
#if defined _protect_included
	if(IsPlayerSpawned{playerid} == 1) if(!Protect_OnPlayerUpdate(playerid)) return 0;
#endif
	// streamers
#if defined ON_PLAYER_UPDATE_STREAMERS

#if defined _streamer_objects_included
    StreamerObj_OnPlayerUpdate(playerid);
#endif
#if defined _streamer_checkpoints_included
    StreamerCP_OnPlayerUpdate(playerid);
#endif
#if defined _streamer_icons_included
    StreamerIcon_OnPlayerUpdate(playerid);
#endif

#endif
	// systems
#if defined _admin_included
    Admin_OnPlayerUpdate(playerid);
#endif
#if defined _deathmatch_included
    Deathmatch_OnPlayerUpdate(playerid);
#endif
#if defined _world_included
    World_OnPlayerUpdate(playerid);
#endif
	// fix glases
	if((GetPlayerWeapon(playerid) == WEAPON_CAMERA) || (GetPlayerWeapon(playerid) == 44) || (GetPlayerWeapon(playerid) == 45))
	{
	    new k[3];
	    GetPlayerKeys(playerid,k[0],k[1],k[2]);
	    if(k[0] & KEY_FIRE) return 0;
	}
	// spys
#if defined VEHICLE_HEALTH_CHANGE_SPY
	static
		Float:PlayerVehicleHealth[MAX_PLAYERS],
		Float:v_health,
		vid;
    if(IsPlayerInAnyVehicle(playerid))
	{
		vid = GetPlayerVehicleID(playerid);
		GetVehicleHealth(vid,v_health);
		if(v_health != PlayerVehicleHealth[playerid])
		{
			// OnPlayerVehicleHealthChange
			OnPlayerVehicleHealthChange(playerid,vid,PlayerVehicleHealth[playerid],v_health);
			// data update
			PlayerVehicleHealth[playerid] = v_health;
		}
	}
#endif
#if defined PLAYER_HEALTH_CHANGE_SPY
	static
		Float:PlayerHealth[MAX_PLAYERS],
		Float:p_health;
	GetPlayerHealth(playerid,p_health);
	if(p_health != PlayerHealth[playerid])
	{
		// OnPlayerHealthChange
		OnPlayerHealthChange(playerid,PlayerHealth[playerid],p_health);
		// data update
		PlayerHealth[playerid] = p_health;
	}
#endif
#if defined PLAYER_ARMOUR_CHANGE_SPY
	static
		Float:PlayerArmour[MAX_PLAYERS],
		Float:p_armour;
	GetPlayerHealth(playerid,p_armour);
	if(p_armour != PlayerArmour[playerid])
	{
		// OnPlayerArmourChange
		OnPlayerArmourChange(playerid,PlayerArmour[playerid],p_armour);
		// data update
		PlayerArmour[playerid] = p_armour;
	}
#endif
#if defined PLAYER_WEAPON_CHANGE_SPY
	static
		PlayerWeapon[MAX_PLAYERS char],
		p_weapon;
	p_weapon = GetPlayerWeapon(playerid);
	printf("p_weapon %d",p_weapon); // check invalid data
	if(p_weapon != PlayerWeapon{playerid})
	{
		// OnPlayerWeaponChange
		OnPlayerWeaponChange(playerid,PlayerWeapon{playerid},p_weapon);
		// data update
		PlayerWeapon{playerid} = p_weapon;
	}
#endif
#if defined PLAYER_SPECIALACTION_CHANGE_SPY
	static
		PlayerSpecialAction[MAX_PLAYERS char],
		p_specialaction;
	p_specialaction = GetPlayerSpecialAction(playerid);
	if(p_specialaction != PlayerSpecialAction{playerid})
	{
		// OnPlayerSpecialActionChange
		OnPlayerSpecialActionChange(playerid,PlayerSpecialAction{playerid},p_specialaction);
		// data update
		PlayerSpecialAction{playerid} = p_specialaction;
	}
#endif
	return 1;
}

public OnPlayerStreamIn(playerid, forplayerid)
{
	return 1;
}

public OnPlayerStreamOut(playerid, forplayerid)
{
	return 1;
}

public OnVehicleStreamIn(vehicleid, forplayerid)
{
	// systems
#if defined _minimissions_included
    MiniMissions_OnVehicleStreamIn(vehicleid,forplayerid);
#endif

	return 1;
}

public OnVehicleStreamOut(vehicleid, forplayerid)
{
	return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	switch(dialogid)
	{
		// systems
#if defined _player_included
	    RegisterDialog(playerid,response,listitem,inputtext,Player,DIALOG_LOGIN);
	    RegisterDialog(playerid,response,listitem,inputtext,Player,DIALOG_REGISTER);
		RegisterDialog(playerid,response,listitem,inputtext,Player,DIALOG_UPGRADE);
#endif
#if defined _world_included
	    RegisterDialog(playerid,response,listitem,inputtext,World,DIALOG_TELEPORTS);
#endif
#if defined _deathmatch_included
	    RegisterDialog(playerid,response,listitem,inputtext,Deathmatch,DIALOG_DEATHMATCHES);
#endif
#if defined _race_included
	    RegisterDialog(playerid,response,listitem,inputtext,Race,DIALOG_RACES);
#endif
#if defined _gangs_included
	    RegisterDialog(playerid,response,listitem,inputtext,Gangs,DIALOG_GANG_INVITE);
	    RegisterDialog(playerid,response,listitem,inputtext,Gangs,DIALOG_GANG_WAR);
	    RegisterDialog(playerid,response,listitem,inputtext,Gangs,DIALOG_GANG_WAR_WEAPONS);
#endif
#if defined _team_quests_included
	    RegisterDialog(playerid,response,listitem,inputtext,TeamQuest,DIALOG_TEAM_QUESTS);
	    RegisterDialog(playerid,response,listitem,inputtext,TeamQuest,DIALOG_TEAM_QUEST_TEAM);
#endif
#if defined _missions_included
	    RegisterDialog(playerid,response,listitem,inputtext,Missions,DIALOG_MISSION_BANK);
#endif
#if defined _weapons_included
		RegisterDialog(playerid,response,listitem,inputtext,Weapons,DIALOG_WEAPONS);
#endif
	}
	return 1;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, source)
{
	return 1;
}

// -----------------------------------------------------------------------------
//                                  Other callbacks
// -----------------------------------------------------------------------------
public Update()
{
	// systems callbacks
#if defined _player_included
	Player_Update();
#endif
#if defined _businesses_included
    Business_Update();
#endif
}

public Check()
{
	// antisleep plugin
#if defined _antisleep_included
    AntiSleepUpdate();
#endif
}

#if defined _streamer_checkpoints_included
public OnPlayerEnterStreamedCheckpoint(playerid,checkpointid)
{

#if defined _missions_included
    Missions_OPESC(playerid,checkpointid);
#endif

}
#endif

#if defined _quest_included
public OnPlayerChangeQuest(playerid,oldquest,newquest)
{

}
#endif

#if defined VEHICLE_HEALTH_CHANGE_SPY
forward OnPlayerVehicleHealthChange(playerid,vehicleid,Float:old_health,Float:new_health);
public OnPlayerVehicleHealthChange(playerid,vehicleid,Float:old_health,Float:new_health)
{

#if defined _missions_included
    Missions_OPVHC(playerid,vehicleid,old_health,new_health);
#endif

}
#endif

#if defined PLAYER_HEALTH_CHANGE_SPY
forward OnPlayerHealthChange(playerid,Float:old_health,Float:new_health);
public OnPlayerHealthChange(playerid,Float:old_health,Float:new_health)
{

}
#endif

#if defined PLAYER_ARMOUR_CHANGE_SPY
forward OnPlayerArmourChange(playerid,Float:old_armour,Float:new_armour);
public OnPlayerArmourChange(playerid,Float:old_armour,Float:new_armour)
{

}
#endif

#if defined PLAYER_WEAPON_CHANGE_SPY
forward OnPlayerWeaponChange(playerid,old_weapon,new_weapon);
public OnPlayerWeaponChange(playerid,old_weapon,new_weapon)
{

}
#endif

#if defined PLAYER_SPECIALACTION_CHANGE_SPY
forward OnPlayerSpecialActionChange(playerid,old_special_action,new_special_action);
public OnPlayerSpecialActionChange(playerid,old_special_action,new_special_action)
{

#if defined _protect_included
    Protect_OPSAC(playerid,old_special_action,new_special_action);
#endif

}
#endif

#if defined _player_included
public OnSavePlayerData(playerid,File:datafile,reason)
{
#if defined _businesses_included
    Business_OnSavePlayerData(playerid,datafile,reason);
#endif
#if defined _houses_included
    Houses_OnSavePlayerData(playerid,datafile,reason);
#endif
#if defined _banks_included
    Banks_OnSavePlayerData(playerid,datafile,reason);
#endif
#if defined _admin_included
    Admin_OnSavePlayerData(playerid,datafile,reason);
#endif
#if defined _gangs_included
    Gangs_OnSavePlayerData(playerid,datafile,reason);
#endif
#if defined _weapons_included
	Weapons_OnSavePlayerData(playerid,datafile,reason);
#endif
}

public OnLoadPlayerData(playerid,datastr[],separatorpos)
{
#if defined _businesses_included
    Business_OnLoadPlayerData(playerid,datastr,separatorpos);
#endif
#if defined _houses_included
    Houses_OnLoadPlayerData(playerid,datastr,separatorpos);
#endif
#if defined _banks_included
    Banks_OnLoadPlayerData(playerid,datastr,separatorpos);
#endif
#if defined _admin_included
    Admin_OnLoadPlayerData(playerid,datastr,separatorpos);
#endif
#if defined _gangs_included
    Gangs_OnLoadPlayerData(playerid,datastr,separatorpos);
#endif
#if defined _weapons_included
	Weapons_OnLoadPlayerData(playerid,datastr,separatorpos);
#endif
}
#endif
