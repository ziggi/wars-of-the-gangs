/*
*	Created:			19.05.10
*	Author:				009
*	Description:		Дезматчи
*/

#if defined _deathmatch_included
	#endinput
#endif

#define _deathmatch_included
#pragma library deathmatch

// --------------------------------------------------
// defines
// --------------------------------------------------
#define DEATHMATCH_COLOR			0xFF0066AA
#define MAX_DEATHMATCH				10
#define MAX_DEATHMATCH_SPAWNS		20
#define MAX_DEATHMATCH_WEAPONS		5
#define DEATHMATCH_DIR				"deathmatches/"
#define INVALID_DEATHMATCH_ID		-1
#define MAX_DEATHMATCH_NAME			30
#define MAX_DEATHMATCH_WARNINGS		150
#define XP_PER_KILL					100
// check functions
#if !defined CheckFileLine
		#error CheckFileLine not found in script.
#endif
#if !defined SetPlayerQuest
		#error SetPlayerQuest not found in script.
#endif
#if !defined SendClientMessageByQuest
		#error SendClientMessageByQuest not found in script.
#endif
#if !defined oGetPlayerName
		#error oGetPlayerName not found in script.
#endif
#if !defined GivePlayerXP
		#error GivePlayerXP not found in script.
#endif
#if !defined RegisterQuest
		#error RegisterQuest not found in script.
#endif
#if !defined GetPlayerQuest
		#error GetPlayerQuest not found in script.
#endif
#if !defined oSetPlayerHealth
		#error oSetPlayerHealth not found in script.
#endif
#if !defined SetPlayerColorAlpha
		#error SetPlayerColorAlpha not found in script.
#endif
#if !defined GetPlayersCount
		#error GetPlayersCount not found in script.
#endif

// --------------------------------------------------
// enums
// --------------------------------------------------
enum
{
	DEATHMATCH_STATUS_OFF,
	DEATHMATCH_STATUS_LINEUP,
	DEATHMATCH_STATUS_ON
};
enum
{
	DEATHMATCH_QUEST_TEXT
};
enum
{
	DEATHMATCH_HELP_CMD,
	DEATHMATCH_MENU_CMD,
	DEATHMATCH_QUIT_CMD
};

// --------------------------------------------------
// statics
// --------------------------------------------------
static 
	// dm
	DeathmatchStatus[MAX_DEATHMATCH char],
	DeathmatchQuest[MAX_DEATHMATCH],
	DeathmatchName[MAX_DEATHMATCH][MAX_DEATHMATCH_NAME],
	DeathmatchSpawnsCount[MAX_DEATHMATCH char],
	DeathmatchPlayersCount[MAX_DEATHMATCH char],
	DeathmatchInterior[MAX_DEATHMATCH char],
	Float:DeathmatchDistance[MAX_DEATHMATCH],
	Float:DeathmatchCenter[MAX_DEATHMATCH][3],
	Float:DeathmatchSpawns[MAX_DEATHMATCH][MAX_DEATHMATCH_SPAWNS][3],
	DeathmatchWeapons[MAX_DEATHMATCH][MAX_DEATHMATCH_WEAPONS char],
	// player
	DeathmatchPlayerCurrent[MAX_PLAYERS],
	DeathmatchPlayerKills[MAX_PLAYERS char],
	DeathmatchPlayerSleep[MAX_PLAYERS char],
	DeathmatchPlayerWarning[MAX_PLAYERS char],
	DeathmatchPlayerOldInterior[MAX_PLAYERS char],
	DeathmatchPlayerOldWorld[MAX_PLAYERS char],
	Float:DeathmatchPlayerOldPos[MAX_PLAYERS][3];

// --------------------------------------------------
// stocks
// --------------------------------------------------
stock LoadDeathmatch(name[])
{
	// find slot
	new dmid = INVALID_DEATHMATCH_ID;
	for(new i = 0;i < MAX_DEATHMATCH;i++)
	{
		if(DeathmatchStatus{i} == DEATHMATCH_STATUS_OFF)
		{
			if(dmid == INVALID_DEATHMATCH_ID) dmid = i;
		}
		else if(!strcmp(name,DeathmatchName[i])) return i;
	}
	if(dmid == INVALID_DEATHMATCH_ID) return INVALID_DEATHMATCH_ID;
	// file
	format(stmp,sizeof(stmp),MODE_DIR DEATHMATCH_DIR "%s.dm",name);
	if(!fexist(stmp)) 
	{
		printf("[ERROR] Deathcmatch file not found (%s)",stmp);
		return INVALID_DEATHMATCH_ID;
	}
	new File:datafile = fopen(stmp,io_read),
		k,
		c,
		scount,
		wcount;
	while(fread(datafile,stmp)) 
	{
		if(!CheckFileLine(stmp)) continue;
		// parse
		if((k = strfind(stmp,"=")) == -1) continue;
		
		if(!strcmp(stmp,"Center",true,k))
		{
			c = 0;
			DeathmatchCenter[dmid][0] = floatstr(strtok(stmp[k + 1],c,','));
			DeathmatchCenter[dmid][1] = floatstr(strtok(stmp[k + 1],c,','));
			DeathmatchCenter[dmid][2] = floatstr(strtok(stmp[k + 1],c,','));
			continue;
		}
		else if(!strcmp(stmp,"Spawn",true,k))
		{
			c = 0;
			DeathmatchSpawns[dmid][scount][0] = floatstr(strtok(stmp[k + 1],c,','));
			DeathmatchSpawns[dmid][scount][1] = floatstr(strtok(stmp[k + 1],c,','));
			DeathmatchSpawns[dmid][scount][2] = floatstr(strtok(stmp[k + 1],c,','));
			scount++;
			continue;
		}
		else if(!strcmp(stmp,"Interior",true,k))
		{
			DeathmatchInterior{dmid} = strval(stmp[k + 1]);
			continue;
		}
		else if(!strcmp(stmp,"Distance",true,k))
		{
			DeathmatchDistance[dmid] = floatstr(stmp[k + 1]);
			continue;
		}
		else if(!strcmp(stmp,"Weapon",true,k))
		{
			DeathmatchWeapons[dmid]{wcount} = strval(stmp[k + 1]);
			wcount++;
			continue;
		}
	}
	fclose(datafile);
	// data
	DeathmatchStatus{dmid} = DEATHMATCH_STATUS_LINEUP;
	strmid(DeathmatchName[dmid],name,0,strlen(name));
	DeathmatchSpawnsCount{dmid} = scount;
	DeathmatchPlayersCount{dmid} = 0;
	return dmid;
}

stock UnloadDeathmatch(dmid)
{
	if(DeathmatchStatus{dmid} == DEATHMATCH_STATUS_OFF) return 0;
	DeathmatchStatus{dmid} = DEATHMATCH_STATUS_OFF;
	memset(DeathmatchName[dmid],0);
	DeathmatchInterior{dmid} = 0;
	DeathmatchDistance[dmid] = 0.0;
	DeathmatchCenter[dmid][0] = 0.0;
	DeathmatchCenter[dmid][1] = 0.0;
	DeathmatchCenter[dmid][2] = 0.0;
	DeathmatchSpawnsCount{dmid} = 0;
	DeathmatchPlayersCount{dmid} = 0;
	for(new s = 0;s < MAX_DEATHMATCH_SPAWNS;s++)
	{
		DeathmatchSpawns[dmid][s][0] = 0.0;
		DeathmatchSpawns[dmid][s][1] = 0.0;
		DeathmatchSpawns[dmid][s][2] = 0.0;
	}
	for(new w = 0;w < MAX_DEATHMATCH_WEAPONS;w++) DeathmatchWeapons[dmid]{w} = 0;
	return 1;
}

stock PlayerJoinDeathmatch(playerid,dmname[])
{
	new dmid = LoadDeathmatch(dmname);
	if(dmid != INVALID_DEATHMATCH_ID)
	{
		DeathmatchPlayersCount{dmid}++;
		DeathmatchPlayerKills{playerid} = 0;
		DeathmatchPlayerSleep{playerid} = 1;
		DeathmatchPlayerCurrent[playerid] = dmid;
		SetPlayerQuest(playerid,DeathmatchQuest[dmid]);
		if(DeathmatchPlayersCount{dmid} == 2)
		{
			DeathmatchStatus{dmid} = DEATHMATCH_STATUS_ON;
			for(new p = 0;p <= GetPlayersCount();p++)
			{
				if(!IsPlayerConnected(p)) continue;
				if(IsPlayerNPC(p)) continue;
				if(GetPlayerQuest(p) != DeathmatchQuest[dmid]) continue;
				
				DeathmatchPlayerOldInterior{p} = GetPlayerInterior(p);
				DeathmatchPlayerOldWorld{p} = GetPlayerVirtualWorld(p);
				GetPlayerPos(p,DeathmatchPlayerOldPos[p][0],DeathmatchPlayerOldPos[p][1],DeathmatchPlayerOldPos[p][2]);
				SetPlayerPos(playerid,0.0,0.0,0.0); // vehicle spawn fix
				SpawnPlayer(p);
			}
		}
		else if(DeathmatchPlayersCount{dmid} > 2)
		{
			DeathmatchPlayerOldInterior{playerid} = GetPlayerInterior(playerid);
			DeathmatchPlayerOldWorld{playerid} = GetPlayerVirtualWorld(playerid);
			GetPlayerPos(playerid,DeathmatchPlayerOldPos[playerid][0],DeathmatchPlayerOldPos[playerid][1],DeathmatchPlayerOldPos[playerid][2]);
			SetPlayerPos(playerid,0.0,0.0,0.0); // vehicle spawn fix
			SpawnPlayer(playerid);
		}
		else SendClientMessage(playerid,SYSTEM_COLOR,"Дезматч начнётся когда присоединится хотя бы один противник");
	}
	else SendClientMessage(playerid,SYSTEM_COLOR,"Ошибка загрузки DM,возможно достигнуто максимальное количество одновременно запущеных DM либо название DM не верно");
}

stock PlayerLeaveDeathmatch(playerid,dmid)
{
	DeathmatchPlayersCount{dmid}--;
	if(DeathmatchPlayersCount{dmid} == 0) UnloadDeathmatch(dmid);
	DeathmatchPlayerCurrent[playerid] = INVALID_DEATHMATCH_ID;
	SetPlayerQuest(playerid,INVALID_QUEST_ID);
	GivePlayerXP(playerid,(DeathmatchPlayerKills{playerid} * XP_PER_KILL));
	format(stmp,sizeof(stmp),"%s покинул бой",oGetPlayerName(playerid));
	SendClientMessageByQuest(DeathmatchQuest[dmid],DEATHMATCH_COLOR,stmp);
	return 1;
}

// --------------------------------------------------
// Obligatory functions
// --------------------------------------------------
Deathmatch_OnGameModeInit()
{
	for(new i = 0;i < MAX_DEATHMATCH;i++)	DeathmatchQuest[i] = RegisterQuest();
	// data
#if defined PRINT_STATS_DATA
	printf("Max deathmatches: %d",MAX_DEATHMATCH);
	printf("Max spawns: %d",MAX_DEATHMATCH_SPAWNS);
	printf("Max weapons: %d",MAX_DEATHMATCH_WEAPONS);
#endif
	print("Deathmatches loaded.");
}

Deathmatch_OnPlayerConnect(playerid)
{
	DeathmatchPlayerCurrent[playerid] = INVALID_DEATHMATCH_ID;
}

Deathmatch_OnPlayerDisconnect(playerid,reason)
{
	#pragma unused reason
	static dmid;
	if(GetPlayerQuest(playerid) != INVALID_QUEST_ID)
	{
		dmid = DeathmatchPlayerCurrent[playerid];
		if((dmid != INVALID_DEATHMATCH_ID) && (GetPlayerQuest(playerid) == DeathmatchQuest[dmid])) return PlayerLeaveDeathmatch(playerid,dmid);
	}
	return 0;
}

Deathmatch_Text(playerid,textid,string[])
{
	static dmid;
	switch(textid)
	{
		case DEATHMATCH_QUEST_TEXT:
		{
			if(GetPlayerQuest(playerid) == INVALID_QUEST_ID) return 0;
			dmid = DeathmatchPlayerCurrent[playerid];
			if((dmid != INVALID_DEATHMATCH_ID) && (GetPlayerQuest(playerid) == DeathmatchQuest[dmid]))
			{
				format(stmp,sizeof(stmp),"[DM] %s: %s",oGetPlayerName(playerid),string);
				SendClientMessageByQuest(DeathmatchQuest[dmid],DEATHMATCH_COLOR,stmp);
				return 1;
			}
		}
	}
	return 0;
}

Deathmatch_OnPlayerSpawn(playerid)
{
	static dmid;
	if(GetPlayerQuest(playerid) != INVALID_QUEST_ID)
	{
		dmid = DeathmatchPlayerCurrent[playerid];
		if((dmid != INVALID_DEATHMATCH_ID) && (GetPlayerQuest(playerid) == DeathmatchQuest[dmid]))
		{
			if(DeathmatchStatus{dmid} != DEATHMATCH_STATUS_ON) return;
			
			new sid = random(DeathmatchSpawnsCount{dmid});
			SetPlayerPos(playerid,DeathmatchSpawns[dmid][sid][0],DeathmatchSpawns[dmid][sid][1],DeathmatchSpawns[dmid][sid][2]);
			SetPlayerInterior(playerid,DeathmatchInterior{dmid});
			SetPlayerVirtualWorld(playerid,DEATHMATCH_VIRTUAL_WORLD + dmid);
			oSetPlayerHealth(playerid,100.0);
			SetPlayerColor(playerid,DEATHMATCH_COLOR);
			SetPlayerColorAlpha(playerid,0);
			ResetPlayerWeapons(playerid);
			for(new w = 0;w < MAX_DEATHMATCH_WEAPONS;w++)
			{
				if(DeathmatchWeapons[dmid]{w} != 0) GivePlayerWeapon(playerid,DeathmatchWeapons[dmid]{w},9999);
			}
		}
	}
}

Deathmatch_OnPlayerDeath(playerid,killerid,weaponid)
{
	#pragma unused weaponid
	static dmid;
	if(GetPlayerQuest(playerid) != INVALID_QUEST_ID)
	{
		dmid = DeathmatchPlayerCurrent[playerid];
		if((dmid != INVALID_DEATHMATCH_ID) && (GetPlayerQuest(playerid) == DeathmatchQuest[dmid]))
		{
			if(DeathmatchStatus{dmid} != DEATHMATCH_STATUS_ON) return;
			
			new lider = 1;
			// death
			DeathmatchPlayerSleep{playerid} = 1;
			// kill
			if((killerid != INVALID_PLAYER_ID) && (GetPlayerQuest(killerid) == DeathmatchQuest[dmid]))
			{
				DeathmatchPlayerKills{killerid}++;
				for(new id = 0;id <= GetPlayersCount();id++)
				{
					if(!IsPlayerConnected(id)) continue;
					if(IsPlayerNPC(id)) continue;
					if(GetPlayerQuest(id) != DeathmatchQuest[dmid]) continue;
					if(id == killerid) continue;
					
					if(DeathmatchPlayerKills{id} >= DeathmatchPlayerKills{killerid}) lider = 0;
				}
				if(lider == 1)
				{
					format(stmp,sizeof(stmp),"%s лидирует в бою со счётом %d",oGetPlayerName(killerid),DeathmatchPlayerKills{killerid});
					SendClientMessageByQuest(DeathmatchQuest[dmid],DEATHMATCH_COLOR,stmp);
				}
			}
			// fast resp
			oSetPlayerHealth(playerid,100.0);
			SpawnPlayer(playerid);
		}
	}
}

Deathmatch_OnPlayerUpdate(playerid)
{
	if(DeathmatchPlayerSleep{playerid} > 0)
	{
		DeathmatchPlayerSleep{playerid}--;
		return;
	}
	static dmid;
	if(GetPlayerQuest(playerid) != INVALID_QUEST_ID)
	{
		dmid = DeathmatchPlayerCurrent[playerid];
		if((dmid != INVALID_DEATHMATCH_ID) && (GetPlayerQuest(playerid) == DeathmatchQuest[dmid]))
		{
			if(DeathmatchStatus{dmid} != DEATHMATCH_STATUS_ON) return;
			
			if(DeathmatchPlayerWarning{playerid} == 0) 
			{
				if(!IsPlayerInRangeOfPoint(playerid,DeathmatchDistance[dmid],DeathmatchCenter[dmid][0],DeathmatchCenter[dmid][1],DeathmatchCenter[dmid][2]))
				{
						GameTextForPlayer(playerid,"~w~You cant leave~n~~r~DEATHMATCH",500,4);
						DeathmatchPlayerWarning{playerid}++;
						if(DeathmatchPlayerWarning{playerid} == MAX_DEATHMATCH_WARNINGS)
						{
							PlayerLeaveDeathmatch(playerid,dmid);
							SendClientMessage(playerid,SYSTEM_COLOR,"Вы покинули бой");
							SetPlayerPos(playerid,0.0,0.0,0.0); // vehicle spawn fix
							SpawnPlayer(playerid);
							SetPlayerInterior(playerid,DeathmatchPlayerOldInterior{playerid});
							SetPlayerVirtualWorld(playerid,DeathmatchPlayerOldWorld{playerid});
							SetPlayerPos(playerid,DeathmatchPlayerOldPos[playerid][0],DeathmatchPlayerOldPos[playerid][1],DeathmatchPlayerOldPos[playerid][2]);
						}
				}
			}
			else 
			{
				if(!IsPlayerInRangeOfPoint(playerid,DeathmatchDistance[dmid],DeathmatchCenter[dmid][0],DeathmatchCenter[dmid][1],DeathmatchCenter[dmid][2])) return;
				if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return;
				DeathmatchPlayerWarning{playerid} = 0;
			}
		}
	}
}

Deathmatch_Command(playerid,cmdid,params[])
{
	#pragma unused params
	switch(cmdid)
	{
		case DEATHMATCH_HELP_CMD:
		{
			ShowPlayerDialog(playerid,DIALOG_NONE_ACTION,DIALOG_STYLE_MSGBOX,MODE_NAME " -> Дезматчи","Вы можете учавствовать в боях 'один против всех' и зарабатывать опыт \n(опыт не сбрасывается если быть в бою)\nКоманды:\n/dm menu - выбрать дезматч\n/dm quit - выйти из дезматча","Ок","Ок");
			return 1;
		}
		case DEATHMATCH_MENU_CMD:
		{
			if(GetPlayerQuest(playerid) != INVALID_QUEST_ID) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы уже учавствуете в другом соревновании");
			
			stmp[0] = 0;
			// file
			new File:datafile = fopen(MODE_DIR DEATHMATCH_DIR "deathmatches.txt",io_read),
				line[MAX_STRING];
			// checker
			if(strlen(params) > 0) itmp[0] = 1;
			else itmp[0] = 0;
			if(datafile)
			{
				while(fread(datafile,line)) 
				{
					if(!CheckFileLine(line)) continue;
					if(itmp[0])
					{
						if(!strcmp(params,line,true))
						{
							PlayerJoinDeathmatch(playerid,line);
							fclose(datafile);
							return 1;
						}
					}
					format(stmp,sizeof(stmp),"%s%s\n",stmp,line);
				}
				fclose(datafile);
			}
			else return print("[ERROR] deathmathes file not found");
			
			ShowPlayerDialog(playerid,DIALOG_DEATHMATCHES,DIALOG_STYLE_LIST,"DeathMatches",stmp,"Зайти","Отмена");
			return 1;
		}
		case DEATHMATCH_QUIT_CMD:
		{
			static dmid;
			if(GetPlayerQuest(playerid) != INVALID_QUEST_ID)
			{
				dmid = DeathmatchPlayerCurrent[playerid];
				if((dmid != INVALID_DEATHMATCH_ID) && (GetPlayerQuest(playerid) == DeathmatchQuest[dmid]))
				{
					PlayerLeaveDeathmatch(playerid,dmid);
					SendClientMessage(playerid,SYSTEM_COLOR,"Вы покинули бой");
					SetPlayerPos(playerid,0.0,0.0,0.0); // vehicle spawn fix
					SpawnPlayer(playerid);
					SetPlayerInterior(playerid,DeathmatchPlayerOldInterior{playerid});
					SetPlayerVirtualWorld(playerid,DeathmatchPlayerOldWorld{playerid});
					SetPlayerPos(playerid,DeathmatchPlayerOldPos[playerid][0],DeathmatchPlayerOldPos[playerid][1],DeathmatchPlayerOldPos[playerid][2]);
					return 1;
				}
			}
			SendClientMessage(playerid,SYSTEM_COLOR,"Вы не учавствуете в дезматче");
			return 1;
		}
	}
	return 0;
}

Deathmatch_Dialog(playerid, dialogid, response, listitem, inputtext[])
{
	#pragma unused listitem
	switch(dialogid)
	{
		case DIALOG_DEATHMATCHES:
		{
			if(!response) return 1;
			
			PlayerJoinDeathmatch(playerid,inputtext);
			return 1;
		}
	}
	return 1;
}