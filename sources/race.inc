/*
*	Created:			19.05.10
*	Author:				009
*	Description:		Гонки
*/

#if defined _race_included
	#endinput
#endif

#define _race_included
#pragma library race

// --------------------------------------------------
// defines
// --------------------------------------------------
#define RACE_COLOR						0x005407AA
#define RACE_TICK_TIME					1000
#define RACE_LINEUP_TIME				60
#define MAX_RACE						10
#define MAX_RACE_CPS					50
#define RACE_DIR						"races/"
#define INVALID_RACE_ID					-1
#define MAX_RACE_NAME					30
#define RACE_CP_SIZE					20.0
#define COUNT_SEC						5
#define GetAngleToCheckpoint(%1,%2,%3)	-atan2((RacesCps[%1][%3][0] - RacesCps[%1][%2][0]),(RacesCps[%1][%3][1] - RacesCps[%1][%2][1]))
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
	RACE_STATUS_OFF,
	RACE_STATUS_LINEUP,
	RACE_STATUS_COUNT,
	RACE_STATUS_ON
};
enum
{
	RACE_QUEST_TEXT
};
enum
{
	RACE_HELP_CMD,
	RACE_MENU_CMD,
	RACE_QUIT_CMD
};

// --------------------------------------------------
// statics
// --------------------------------------------------
static
	// races
	RaceStatus[MAX_RACE char],
	RaceQuest[MAX_RACE],
	RaceName[MAX_RACE][MAX_RACE_NAME],
	RaceTime[MAX_RACE],
	RaceMoney[MAX_RACE],
	RaceXp[MAX_RACE],
	RaceCheckpointsCount[MAX_RACE char],
	RaceInterior[MAX_RACE char],
	RacePlayersCount[MAX_RACE char],
	RacePlayersFinish[MAX_RACE char],
	Float:RacesCps[MAX_RACE][MAX_RACE_CPS][3],
	// players
	RacePlayerCurrent[MAX_PLAYERS],
	RacePlayerCheckpoint[MAX_PLAYERS char],
	RacePlayerOldInterior[MAX_PLAYERS char],
	RacePlayerOldWorld[MAX_PLAYERS char],
	Float:RacePlayerOldPos[MAX_PLAYERS][3];

// --------------------------------------------------
// forwards
// --------------------------------------------------
forward RaceTick();

// --------------------------------------------------
// publics
// --------------------------------------------------
public RaceTick()
{
	for(new i = 0;i < MAX_RACE;i++)
	{
		if(RaceStatus{i} == RACE_STATUS_OFF) continue;
		
		RaceTime[i]++;
		switch(RaceStatus{i})
		{
			case RACE_STATUS_LINEUP:
			{
				if(RaceTime[i] == RACE_LINEUP_TIME)
				{
					new Float:angle = GetAngleToCheckpoint(i,0,1);
					for(new p = 0;p <= GetPlayersCount();p++)
					{
						if(!IsPlayerConnected(p)) continue;
						if(IsPlayerNPC(p)) continue;
						if(GetPlayerQuest(p) != RaceQuest[i]) continue;
						
						RacePlayerOldInterior{p} = GetPlayerInterior(p);
						RacePlayerOldWorld{p} = GetPlayerVirtualWorld(p);
						GetPlayerPos(p,RacePlayerOldPos[p][0],RacePlayerOldPos[p][1],RacePlayerOldPos[p][2]);
						
						SetPlayerInterior(p,RaceInterior{i});
						SetPlayerVirtualWorld(p,RACE_VIRTUAL_WORLD + i);
						if(IsPlayerInAnyVehicle(p))
						{
							new vid = GetPlayerVehicleID(p);
							LinkVehicleToInterior(vid,RaceInterior{i});
							SetVehicleVirtualWorld(vid,RACE_VIRTUAL_WORLD + i);
							SetVehiclePos(vid,(RacesCps[i][0][0] + float(random(5) - random(5))),(RacesCps[i][0][1] + float(random(5) - random(5))),RacesCps[i][0][2] + 1.0);
							SetVehicleZAngle(vid,angle);
							PutPlayerInVehicle(p,vid,0);
						}
						else
						{
							SetPlayerPos(p,(RacesCps[i][0][0] + float(random(5) - random(5))),(RacesCps[i][0][1] + float(random(5) - random(5))),RacesCps[i][0][2] + 1.0);
							SetPlayerFacingAngle(p,angle);
						}
						TogglePlayerControllable(p,false);
					}
					RaceStatus{i} = RACE_STATUS_COUNT;
					RaceTime[i] = 0;
				}
			}
			case RACE_STATUS_COUNT:
			{
				if((itmp[0] = (COUNT_SEC - RaceTime[i])) > 0) 
				{
					format(stmp,sizeof(stmp),"~r~%d...",itmp[0]);
					for(new p = 0;p <= GetPlayersCount();p++)
					{
						if(!IsPlayerConnected(p)) continue;
						if(IsPlayerNPC(p)) continue;
						if(GetPlayerQuest(p) != RaceQuest[i]) continue;
						
						GameTextForPlayer(p,stmp,2500,4);
					}
				}
				else
				{
					for(new p = 0;p <= GetPlayersCount();p++)
					{
						if(!IsPlayerConnected(p)) continue;
						if(IsPlayerNPC(p)) continue;
						if(GetPlayerQuest(p) != RaceQuest[i]) continue;
						
						TogglePlayerControllable(p,true);
						RacePlayerCheckpoint{p} = 1;
						SetPlayerRaceCheckpoint(p,0,RacesCps[i][ RacePlayerCheckpoint{p} ][0],RacesCps[i][ RacePlayerCheckpoint{p} ][1],RacesCps[i][ RacePlayerCheckpoint{p} ][2],RacesCps[i][ RacePlayerCheckpoint{p} + 1 ][0],RacesCps[i][ RacePlayerCheckpoint{p} + 1 ][1],RacesCps[i][ RacePlayerCheckpoint{p} + 1 ][2],RACE_CP_SIZE);	
						GameTextForPlayer(p,"~g~GO...GO...GO...",2500,4);
					}
					RaceStatus{i} = RACE_STATUS_ON;
					RaceTime[i] = 0;					
				}				
			}
			case RACE_STATUS_ON: RaceTime[i]++;
		}
	}
}

// --------------------------------------------------
// stocks
// --------------------------------------------------
stock LoadRace(name[])
{
	// find slot
	new rcid = INVALID_RACE_ID;
	for(new i = 0;i < MAX_RACE;i++)
	{
		if(RaceStatus{i} == RACE_STATUS_OFF)
		{
			if(rcid == INVALID_RACE_ID) rcid = i;
		}
		else if(!strcmp(name,RaceName[i])) return i;
	}
	if(rcid == INVALID_RACE_ID) return INVALID_RACE_ID;
	// file
	format(stmp,sizeof(stmp),MODE_DIR RACE_DIR "%s.rc",name);
	if(!fexist(stmp)) 
	{
		printf("[ERROR] Race file not found (%s)",stmp);
		return INVALID_RACE_ID;
	}
	new File:datafile = fopen(stmp,io_read),
		k,
		c,
		cpcount;
	while(fread(datafile,stmp)) 
	{
		if(!CheckFileLine(stmp)) continue;
		// parse
		if((k = strfind(stmp,"=")) == -1) continue;
		
		if(!strcmp(stmp,"CP",true,k))
		{
			c = 0;
			RacesCps[rcid][cpcount][0] = floatstr(strtok(stmp[k + 1],c,','));
			RacesCps[rcid][cpcount][1] = floatstr(strtok(stmp[k + 1],c,','));
			RacesCps[rcid][cpcount][2] = floatstr(strtok(stmp[k + 1],c,','));
			cpcount++;
			continue;
		}
		else if(!strcmp(stmp,"Interior",true,k))
		{
			RaceInterior{rcid} = strval(stmp[k + 1]);
			continue;
		}
		else if(!strcmp(stmp,"Money",true,k))
		{
			RaceMoney[rcid] = strval(stmp[k + 1]);
			continue;
		}
		else if(!strcmp(stmp,"XP",true,k))
		{
			RaceXp[rcid] = strval(stmp[k + 1]);
			continue;
		}
	}
	fclose(datafile);
	// data
	RaceStatus{rcid} = RACE_STATUS_LINEUP;
	format(RaceName[rcid],MAX_RACE_NAME,name);
	RaceTime[rcid] = 0;
	RaceCheckpointsCount{rcid} = cpcount - 1;
	RacePlayersCount{rcid} = 0;
	RacePlayersFinish{rcid} = 0;
	return rcid;
}

stock UnloadRace(rcid)
{
	if(RaceStatus{rcid} == RACE_STATUS_OFF) return 0;
	RaceStatus{rcid} = RACE_STATUS_OFF;
	memset(RaceName[rcid],0);
	RaceInterior{rcid} = 0;
	RaceTime[rcid] = 0;
	RaceCheckpointsCount{rcid} = 0;
	RacePlayersCount{rcid} = 0;
	RacePlayersFinish{rcid} = 0;
	RaceMoney[rcid] = 0;
	RaceXp[rcid] = 0;
	
	for(new s = 0;s < MAX_RACE_CPS;s++)
	{
		RacesCps[rcid][s][0] = 0.0;
		RacesCps[rcid][s][1] = 0.0;
		RacesCps[rcid][s][2] = 0.0;
	}
	for(new v = 0;v < MAX_VEHICLES;v++)
	{
		if(GetVehicleVirtualWorld(v) == RACE_VIRTUAL_WORLD + rcid) SetVehicleVirtualWorld(v,0);
	}
	return 1;
}

stock PlayerJoinRace(playerid,racename[])
{
	new rcid = LoadRace(racename);
	if(rcid != INVALID_RACE_ID)
	{
		if(RaceStatus{rcid} == RACE_STATUS_ON) return SendClientMessage(playerid,SYSTEM_COLOR,"Гонка уже началась");
		RacePlayersCount{rcid}++;
		RacePlayerCurrent[playerid] = rcid;
		RacePlayerCheckpoint{playerid} = 0;
		SetPlayerQuest(playerid,RaceQuest[rcid]);
		return SendClientMessage(playerid,SYSTEM_COLOR,"Гонка начнётся через минуту");
	}
	else return SendClientMessage(playerid,SYSTEM_COLOR,"Ошибка загрузки гонки,возможно достигнуто максимальное количество одновременно запущеных гонок либо название гонки не верно");
}

stock PlayerLeaveRace(playerid,rcid,reason)
{
	RacePlayersCount{rcid}--;
	RacePlayerCurrent[playerid] = INVALID_RACE_ID;
	if(reason != 0)
	{
		RacePlayersFinish{rcid}++;
 		format(stmp,sizeof(stmp),"%s финишировал на %d месте (%d sec)",oGetPlayerName(playerid),RacePlayersFinish{rcid},RaceTime[rcid]);
		SendClientMessageByQuest(RaceQuest[rcid],RACE_COLOR,stmp);
		if(RacePlayersFinish{rcid} <= 3)
		{
			oGivePlayerMoney(playerid,floatround(RaceMoney[rcid] / RacePlayersFinish{rcid}));
			GivePlayerXP(playerid,floatround(RaceXp[rcid] / RacePlayersFinish{rcid}));
		}
		SetPlayerQuest(playerid,INVALID_QUEST_ID);
	}
	else
	{
		SetPlayerQuest(playerid,INVALID_QUEST_ID);
		format(stmp,sizeof(stmp),"%s покинул гонку",oGetPlayerName(playerid));
		SendClientMessageByQuest(RaceQuest[rcid],RACE_COLOR,stmp);
	}
	SetPlayerInterior(playerid,RacePlayerOldInterior{playerid});
	SetPlayerVirtualWorld(playerid,RacePlayerOldWorld{playerid});
	if(IsPlayerInAnyVehicle(playerid))
	{
		new vid = GetPlayerVehicleID(playerid);
		LinkVehicleToInterior(vid,RacePlayerOldInterior{playerid});
		SetVehicleVirtualWorld(vid,RacePlayerOldWorld{playerid});
		SetVehiclePos(vid,RacePlayerOldPos[playerid][0],RacePlayerOldPos[playerid][1],RacePlayerOldPos[playerid][2]);
		PutPlayerInVehicle(playerid,vid,0);
	}
	else
	{
		SetPlayerPos(playerid,RacePlayerOldPos[playerid][0],RacePlayerOldPos[playerid][1],RacePlayerOldPos[playerid][2]);
	}
	if(RacePlayersCount{rcid} == 0) UnloadRace(rcid);
	return 1;
}

// --------------------------------------------------
// Obligatory functions
// --------------------------------------------------
Race_OnGameModeInit()
{
	for(new i = 0;i < MAX_RACE;i++) RaceQuest[i] = RegisterQuest();
	SetTimer("RaceTick",RACE_TICK_TIME,1);
	// data
#if defined PRINT_STATS_DATA
	printf("Max races: %d",MAX_RACE);
	printf("Max checkpoints: %d",MAX_RACE_CPS);
#endif
	print("Race system loaded.");
}

Race_OnPlayerConnect(playerid)
{
	RacePlayerCurrent[playerid] = INVALID_RACE_ID;
}

Race_OnPlayerDisconnect(playerid,reason)
{
	#pragma unused reason
	static rcid;
	if(GetPlayerQuest(playerid) != INVALID_QUEST_ID)
	{
		rcid = RacePlayerCurrent[playerid];
		if((rcid != INVALID_RACE_ID) && (GetPlayerQuest(playerid) == RaceQuest[rcid])) return PlayerLeaveRace(playerid,rcid,0);
	}
	return 0;
}

Race_Text(playerid,textid,string[])
{
	static rcid;
	switch(textid)
	{
		case RACE_QUEST_TEXT:
		{
			if(GetPlayerQuest(playerid) == INVALID_QUEST_ID) return 0;
			rcid = RacePlayerCurrent[playerid];
			if((rcid != INVALID_RACE_ID) && (GetPlayerQuest(playerid) == RaceQuest[rcid]))
			{
				format(stmp,sizeof(stmp),"[RACE] %s: %s",oGetPlayerName(playerid),string);
				SendClientMessageByQuest(RaceQuest[rcid],RACE_COLOR,stmp);
				return 1;
			}
		}
	}
	return 0;
}

Race_OnPlayerDeath(playerid,killerid,weaponid)
{
	#pragma unused killerid
	#pragma unused weaponid
	static rcid;
	if(GetPlayerQuest(playerid) != INVALID_QUEST_ID)
	{
		rcid = RacePlayerCurrent[playerid];
		if((rcid != INVALID_RACE_ID) && (GetPlayerQuest(playerid) == RaceQuest[rcid]))
		{
			if(RaceStatus{rcid} != RACE_STATUS_LINEUP) PlayerLeaveRace(playerid,rcid,0);
		}
	}
}

Race_OnPlayerEnterRaceCP(playerid)
{
	static rcid;
	if(GetPlayerQuest(playerid) != INVALID_QUEST_ID)
	{
		rcid = RacePlayerCurrent[playerid];
		if((rcid != INVALID_RACE_ID) && (GetPlayerQuest(playerid) == RaceQuest[rcid]))
		{
			if(RaceStatus{rcid} == RACE_STATUS_ON)
			{
				if(IsPlayerInRangeOfPoint(playerid,RACE_CP_SIZE,RacesCps[rcid][ RacePlayerCheckpoint{playerid} ][0],RacesCps[rcid][ RacePlayerCheckpoint{playerid} ][1],RacesCps[rcid][ RacePlayerCheckpoint{playerid} ][2]))
				{
					DisablePlayerRaceCheckpoint(playerid);
					RacePlayerCheckpoint{playerid}++;
					if(RacePlayerCheckpoint{playerid} == RaceCheckpointsCount{rcid}) PlayerLeaveRace(playerid,rcid,1);
					else if(RacePlayerCheckpoint{playerid} == (RaceCheckpointsCount{rcid} - 1)) SetPlayerRaceCheckpoint(playerid,1,RacesCps[rcid][ RacePlayerCheckpoint{playerid} ][0],RacesCps[rcid][ RacePlayerCheckpoint{playerid} ][1],RacesCps[rcid][ RacePlayerCheckpoint{playerid} ][2],RacesCps[rcid][ RacePlayerCheckpoint{playerid} + 1 ][0],RacesCps[rcid][ RacePlayerCheckpoint{playerid} + 1 ][1],RacesCps[rcid][ RacePlayerCheckpoint{playerid} + 1 ][2],RACE_CP_SIZE);	
					else SetPlayerRaceCheckpoint(playerid,0,RacesCps[rcid][ RacePlayerCheckpoint{playerid} ][0],RacesCps[rcid][ RacePlayerCheckpoint{playerid} ][1],RacesCps[rcid][ RacePlayerCheckpoint{playerid} ][2],RacesCps[rcid][ RacePlayerCheckpoint{playerid} + 1 ][0],RacesCps[rcid][ RacePlayerCheckpoint{playerid} + 1 ][1],RacesCps[rcid][ RacePlayerCheckpoint{playerid} + 1 ][2],RACE_CP_SIZE);
				}
			}
		}
	}
}

Race_Command(playerid,cmdid,params[])
{
	#pragma unused params
	switch(cmdid)
	{
		case RACE_HELP_CMD:
		{
			ShowPlayerDialog(playerid,DIALOG_NONE_ACTION,DIALOG_STYLE_MSGBOX,MODE_NAME " -> Гонки","Вы можете учавствовать в гонках зарабатывая опыт и деньги\nКоманды:\n/race menu - выбрать гонку\n/race quit - выйти из гонки","Ок","Ок");
			return 1;
		}
		case RACE_MENU_CMD:
		{
			if(GetPlayerQuest(playerid) != INVALID_QUEST_ID) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы уже учавствуете в другом соревновании");
			
			stmp[0] = 0;
			// file
			new File:datafile = fopen(MODE_DIR RACE_DIR "races.txt",io_read),
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
							PlayerJoinRace(playerid,line);
							fclose(datafile);
							return 1;
						}
					}
					
					format(stmp,sizeof(stmp),"%s%s\n",stmp,line);
				}
				fclose(datafile);
			}
			else return print("[ERROR] races file not found");
			
			ShowPlayerDialog(playerid,DIALOG_RACES,DIALOG_STYLE_LIST,"Races",stmp,"Зайти","Отмена");
			return 1;
		}
		case RACE_QUIT_CMD:
		{
			static rcid;
			if(GetPlayerQuest(playerid) != INVALID_QUEST_ID)
			{
				rcid = RacePlayerCurrent[playerid];
				if((rcid != INVALID_RACE_ID) && (GetPlayerQuest(playerid) == RaceQuest[rcid]))
				{
					PlayerLeaveRace(playerid,rcid,0);
					SendClientMessage(playerid,SYSTEM_COLOR,"Вы покинули гонку");
					SetPlayerInterior(playerid,RacePlayerOldInterior{playerid});
					SetPlayerVirtualWorld(playerid,RacePlayerOldWorld{playerid});
					SetPlayerPos(playerid,RacePlayerOldPos[playerid][0],RacePlayerOldPos[playerid][1],RacePlayerOldPos[playerid][2]);
					return 1;
				}
			}
			SendClientMessage(playerid,SYSTEM_COLOR,"Вы не учавствуете в гонке");
			return 1;
		}
	}
	return 0;
}

Race_Dialog(playerid, dialogid, response, listitem, inputtext[])
{
	#pragma unused listitem
	switch(dialogid)
	{
		case DIALOG_RACES:
		{
			if(!response) return 1;
			
			PlayerJoinRace(playerid,inputtext);
			return 1;
		}
	}
	return 1;
}