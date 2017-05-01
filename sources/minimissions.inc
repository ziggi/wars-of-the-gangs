/*
*	Created:			18.05.10
*	Author:				009
*	Description:		мини миссии
*/

#if defined _minimissions_included
	#endinput
#endif

#define _minimissions_included
#pragma library minimissions

// --------------------------------------------------
// defines
// --------------------------------------------------
#define MINI_MISSION_1_CP_1 2305.9739,1388.6107,42.8203
#define MINI_MISSION_1_CP_2 2015.9777,2255.3940,23.9141
#define MINI_MISSION_2_CP -1223.843994,165.252670,15.52254
#define MINI_MISSIONS_TICK_TIME		1000
#define MINI_MISSIONS_START_TIME	600000
#define MINI_MISSIONS_COLOR 		0xAA3333AA
#define MAX_MINI_MISSIONS			2
#define INVALID_MINI_MISSION_CS_ID	-1
// check functions
#if !defined RegisterQuest
		#error RegisterQuest not found in script.
#endif
#if !defined SetPlayerQuest
		#error SetPlayerQuest not found in script.
#endif
#if !defined GetPlayerQuest
		#error GetPlayerQuest not found in script.
#endif
#if !defined GivePlayerXP
		#error GivePlayerXP not found in script.
#endif

// --------------------------------------------------
// enums
// --------------------------------------------------
enum 
{
	MINI_MISSION_STATUS_OFF,
	MINI_MISSION_STATUS_ON
};
enum
{
	MINI_MISSION_REASON_NONE,
	MINI_MISSION_REASON_COMPLETE
};
enum MMinfo {
	mm_text[MAX_STRING],
	mm_money,
	mm_xp,
	mm_time,
	mm_playerid,
	mm_questid,
	mm_time_lost,
	mm_tmp
};

// --------------------------------------------------
// statics
// --------------------------------------------------
static MiniMissions[MAX_MINI_MISSIONS][MMinfo] =
{
	{"[SMS] Перевезите гробовозку из доков Лос Сантоса на стоянку в Лас Вентурсе около пирамиды,хорошая оплата гарантирована",50_000,10_000,500,INVALID_PLAYER_ID,INVALID_QUEST_ID,0,0},
	{"[SMS] В Аэропорту Лас Вентураса в одном из ангаров стоит самолёт набитый наркотой.Перевезите его в Аэропорт Сан Фиерро",70_000,13_000,300,INVALID_PLAYER_ID,INVALID_QUEST_ID,0,0}
};
static MiniMissionStatus[MAX_MINI_MISSIONS char];
static MiniMissionStep[MAX_MINI_MISSIONS char];
static IsPlayerHaveMinimission[MAX_PLAYERS char];
static LastStart;

// --------------------------------------------------
// stocks
// --------------------------------------------------
stock MiniMissionChangeStatus(minimissionid,newstatus,reason=MINI_MISSION_REASON_NONE)
{
	switch(newstatus)
	{
		case MINI_MISSION_STATUS_OFF:
		{
			if(GetPlayerQuest(MiniMissions[minimissionid][mm_playerid]) == MiniMissions[minimissionid][mm_questid])
			{
				DisablePlayerCheckpoint(MiniMissions[minimissionid][mm_playerid]);
#if defined _streamer_checkpoints_included
				TogglePlayerCheckpointStreamer(MiniMissions[minimissionid][mm_playerid],1);
#endif
				SetPlayerQuest(MiniMissions[minimissionid][mm_playerid],INVALID_QUEST_ID);
			}
			if(IsPlayerInVehicle(MiniMissions[minimissionid][mm_playerid],MiniMissions[minimissionid][mm_tmp])) RemovePlayerFromVehicle(MiniMissions[minimissionid][mm_playerid]);
			SetTimerEx("MiniMissionAction",2000,0,"dd",minimissionid,1);
			if(reason == MINI_MISSION_REASON_COMPLETE)
			{
				// give price
				GivePlayerXP(MiniMissions[minimissionid][mm_playerid],MiniMissions[minimissionid][mm_xp]);
				oGivePlayerMoney(MiniMissions[minimissionid][mm_playerid],MiniMissions[minimissionid][mm_money]);
				// text
				SendClientMessage(MiniMissions[minimissionid][mm_playerid],MINI_MISSIONS_COLOR,"[SMS] Молодец,получай свой приз");
			}
			else SendClientMessage(MiniMissions[minimissionid][mm_playerid],MINI_MISSIONS_COLOR,"[SMS] Чёрта с два я к тебе ещё обращусь!Ты провалил задание");
			IsPlayerHaveMinimission{MiniMissions[minimissionid][mm_playerid]} = 0;
			MiniMissions[minimissionid][mm_playerid]	= 	INVALID_PLAYER_ID;
			MiniMissions[minimissionid][mm_time_lost]	= 	0;
			MiniMissionStep{minimissionid}				= 	0;
			MiniMissionStatus{minimissionid}			= 	MINI_MISSION_STATUS_OFF;
		}
		case MINI_MISSION_STATUS_ON:
		{
			// find player for start
			new playerid = INVALID_PLAYER_ID,
				number = -1,
				tmp;
			for(new j = 0;j <= GetPlayersCount();j++)
			{
				if(!IsPlayerConnected(j)) continue;
				if(GetPlayerQuest(j) != INVALID_QUEST_ID) continue;
				if(IsPlayerHaveMinimission{j} == 1) continue;
				
				if((tmp = random(100)) < number) continue;
				playerid = j;
				number = tmp;
			}
			if(playerid == INVALID_PLAYER_ID) return;
			// player finded
			SendClientMessage(playerid,MINI_MISSIONS_COLOR,MiniMissions[minimissionid][mm_text]);
			MiniMissions[minimissionid][mm_playerid]	= 	playerid;
			MiniMissions[minimissionid][mm_time_lost]	= 	MiniMissions[minimissionid][mm_time];
			MiniMissionStep{minimissionid}				= 	0;
			MiniMissionStatus{minimissionid}			= 	MINI_MISSION_STATUS_ON;
			IsPlayerHaveMinimission{playerid}			= 	1;
			// mission's addition
			switch(minimissionid)
			{
				case 0: MiniMissions[minimissionid][mm_tmp] = CreateVehicle(442,2787.1646,-2494.3828,13.4801,88.9501, 4, 0, 999_999);
				case 1: MiniMissions[minimissionid][mm_tmp] = CreateVehicle(511,1306.5927,1324.3030,12.1944,270.8141, 4, 0, 999_999);
			}
		}
	}
}

// --------------------------------------------------
// forwards
// --------------------------------------------------
forward MiniMissionAction(minimissionid,action);
forward MiniMissionsTick();

// --------------------------------------------------
// publics
// --------------------------------------------------
public MiniMissionAction(minimissionid,action)
{
	switch(minimissionid)
	{
		case 0: 
		{
			switch(action)
			{
				case 0: CreateExplosion(MINI_MISSION_1_CP_1,1,30.0);
				case 1: DestroyVehicle(MiniMissions[minimissionid][mm_tmp]);
			}
		}
		case 1:
		{
			DestroyVehicle(MiniMissions[minimissionid][mm_tmp]);
		}
	}
}

public MiniMissionsTick()
{
	for(new i = 0;i < sizeof(MiniMissions);i++)
	{
		if(MiniMissionStatus{i} == MINI_MISSION_STATUS_OFF) continue;
		
		MiniMissions[i][mm_time_lost]--;
		if(MiniMissions[i][mm_time_lost] == 0) MiniMissionChangeStatus(i,MINI_MISSION_STATUS_OFF);
		else
		{
			if(IsPlayerInVehicle(MiniMissions[i][mm_playerid],MiniMissions[i][mm_tmp]) && (GetPlayerQuest(MiniMissions[i][mm_playerid]) == INVALID_QUEST_ID))
			{
				SendClientMessage(MiniMissions[i][mm_playerid],MINI_MISSIONS_COLOR,"[SMS] Точка назначения отмечена на твоём gps");
#if defined _streamer_checkpoints_included
				TogglePlayerCheckpointStreamer(MiniMissions[i][mm_playerid],0);
#endif
				SetPlayerQuest(MiniMissions[i][mm_playerid],MiniMissions[i][mm_questid]);
				switch(i)
				{
					case 0:
					{
						switch(MiniMissionStep{i})
						{
							case 0: SetPlayerCheckpoint(MiniMissions[i][mm_playerid],MINI_MISSION_1_CP_1,8.0);
							case 1: SetPlayerCheckpoint(MiniMissions[i][mm_playerid],MINI_MISSION_1_CP_2,8.0);
						}
					}
					case 1:
					{
						SetPlayerCheckpoint(MiniMissions[i][mm_playerid],MINI_MISSION_2_CP,8.0);
					}
				}
			}
			else if(IsPlayerInVehicle(MiniMissions[i][mm_playerid],MiniMissions[i][mm_tmp]) && (GetPlayerQuest(MiniMissions[i][mm_playerid]) == MiniMissions[i][mm_questid]))
			{
				switch(i)
				{
					case 0:
					{
						switch(MiniMissionStep{i})
						{
							case 0:
							{
								if(IsPlayerInRangeOfPoint(MiniMissions[i][mm_playerid],8.0,MINI_MISSION_1_CP_1))
								{
									SetTimerEx("MiniMissionAction",5000,0,"dd",i,0);
									MiniMissionStep{i} = 1;
									SetPlayerCheckpoint(MiniMissions[i][mm_playerid],MINI_MISSION_1_CP_2,8.0);
									SendClientMessage(MiniMissions[i][mm_playerid],MINI_MISSIONS_COLOR,"[SMS] Убирайся от туда,мне только что сообщили что туда заложили бомбу!Едь к другому месту на gps!");
								}
							}
							case 1:
							{
								if(IsPlayerInRangeOfPoint(MiniMissions[i][mm_playerid],8.0,MINI_MISSION_1_CP_2))
								{
									MiniMissionChangeStatus(i,MINI_MISSION_STATUS_OFF,MINI_MISSION_REASON_COMPLETE);
								}
							}
						}
					}
					case 1:
					{
						if(IsPlayerInRangeOfPoint(MiniMissions[i][mm_playerid],8.0,MINI_MISSION_2_CP))
						{
							MiniMissionChangeStatus(i,MINI_MISSION_STATUS_OFF,MINI_MISSION_REASON_COMPLETE);
						}
					}
				}
			}
			else if(GetPlayerQuest(MiniMissions[i][mm_playerid]) == MiniMissions[i][mm_questid])
			{
				DisablePlayerCheckpoint(MiniMissions[i][mm_playerid]);
#if defined _streamer_checkpoints_included
				TogglePlayerCheckpointStreamer(MiniMissions[i][mm_playerid],1);
#endif
				SetPlayerQuest(MiniMissions[i][mm_playerid],INVALID_QUEST_ID);
			}
		}
	}
	if((GetTickCount() - LastStart) > MINI_MISSIONS_START_TIME)
	{
		for(new i = 0;i < sizeof(MiniMissions);i++)
		{
			if(MiniMissionStatus{i} != MINI_MISSION_STATUS_OFF) continue;
			
			MiniMissionChangeStatus(i,MINI_MISSION_STATUS_ON);
		}
		LastStart = GetTickCount();
	}
}

// --------------------------------------------------
// Obligatory functions
// --------------------------------------------------
MiniMissios_OnGameModeInit()
{
	for(new i = 0;i < sizeof(MiniMissions);i++) MiniMissions[i][mm_questid] = RegisterQuest();
	SetTimer("MiniMissionsTick",MINI_MISSIONS_TICK_TIME,1);
	// data
#if defined PRINT_STATS_DATA
	printf("Mini missions: %d",MAX_MINI_MISSIONS);
#endif
	print("Mini missions loaded.");
}

MiniMissions_OnPlayerConnect(playerid)
{
	IsPlayerHaveMinimission{playerid} = 0;
}

MiniMissions_OnPlayerDisconnect(playerid,reason)
{
	#pragma unused reason
	for(new i = 0;i < sizeof(MiniMissions);i++)
	{
		if(MiniMissionStatus{i} == MINI_MISSION_STATUS_OFF) continue;
		
		if(MiniMissions[i][mm_playerid] == playerid) MiniMissions[i][mm_playerid] = INVALID_PLAYER_ID;
	}
	IsPlayerHaveMinimission{playerid} = 0;
}

MiniMissions_OnVehicleStreamIn(vehicleid,forplayerid)
{
	for(new i = 0;i < sizeof(MiniMissions);i++)
	{
		if(MiniMissionStatus{i} == MINI_MISSION_STATUS_OFF) continue;
		if(MiniMissions[i][mm_tmp] != vehicleid) continue;
		
		if(MiniMissions[i][mm_playerid] == forplayerid) SetVehicleParamsForPlayer(vehicleid,forplayerid,1,0);
		else SetVehicleParamsForPlayer(vehicleid,forplayerid,0,1);
	}
}