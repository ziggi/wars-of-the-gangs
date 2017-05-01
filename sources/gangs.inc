/*
*	Created:			15.06.10
*	Author:				009
*	Description:		Банды. Создание, удаление, принятие в банду, изгнание из банды, зоны банд, захват зон, войны за зону/деньги/распад.
*/

#if defined _gangs_included
	#endinput
#endif

#define _gangs_included
#pragma library gangs

// --------------------------------------------------
// defines
// --------------------------------------------------
// gangs
#define GANGS_DIR				"gangs/"
#define GANGS_MASK_FILE			"%s.txt"
#define GANGS_NEUTRAL_COLOR		0x333333AA
#define MAX_GANG_NAME			24
#define MAX_GANG_WELCOME		128
#define MAX_GANGS				MAX_PLAYERS
#define MAX_GANG_MEMBERS		16
#define MAX_GANG_COLOR_NAME		15
#define INVALID_GANG_ID			-1
#define GANG_COST				100000
#define GANG_SCORE_RATE			10
// zones
#define GANG_ZONES_REB			8
#define GANG_ZONES_PER_CITY		GANG_ZONES_REB * GANG_ZONES_REB
#define GANG_ZONES_DIR			"gang zones/"
#define GANG_ZONES_MASK_FILE	"zone_%d.txt"
#define INVALID_GANG_ZONE_ID	-1
#define KILLS_FOR_CAPTURE		5
#define MAX_UNLOGGED_ZONE_TIME	60 * 60 * 24 * 15 // (((((((60 - минута) * 60) - час) * 24) - день) * 15) - 15 дней)
// wars
#define GANG_WAR_SCORE_PRICE	100
#define ZONE_GANG_WAR_COST		10_000
#define MAX_GANG_WAR_SPAWNS		1
#define GANG_WAR_WEAPONS_STRING	"Tec9\nM4"
// check functions
#if !defined CheckFileLine
		#error CheckFileLine not found in script.
#endif
#if !defined strtok
	#error strtok not found in script.
#endif
#if !defined oGetPlayerMoney
	#error oGetPlayerMoney not found in script.
#endif
#if !defined ChangeStatByNameInt
	#error ChangeStatByNameInt not found in script.
#endif
#if !defined oGivePlayerMoney
	#error oGivePlayerMoney not found in script.
#endif
#if !defined oGetPlayerName
	#error oGivePlayerMoney not found in script.
#endif


// --------------------------------------------------
// enums
// --------------------------------------------------
enum
{
	GANGS_HELP_CMD,
	GANGS_CREATE_CMD,
	GANGS_DELETE_CMD,
	GANGS_COLORS_CMD,
	GANGS_COLOR_CMD,
	GANGS_WELCOME_CMD,
	GANGS_INVITE_CMD,
	GANGS_QUIT_CMD,
	GANGS_KICK_CMD,
	GANGS_MEMBERS_CMD,
	GANGS_STATS_CMD,
	GANGS_ZONE_HELP_CMD,
	GANGS_ZONE_INFO_CMD,
	GANGS_WAR_HELP_CMD,
	GANGS_WAR_ATTACK_CMD,
	GANGS_WAR_JOIN_CMD,
	GANGS_WAR_LEAVE_CMD,
	GANGS_WAR_START_CMD
};
enum
{
	GANGS_TEAM_TEXT
};
enum GangColorsInfo
{
	cName[MAX_GANG_COLOR_NAME],
	cColor
};
enum
{
	GANG_WAR_STATE_OFF,
	GANG_WAR_STATE_LINEUP,
	GANG_WAR_STATE_ON
};
enum
{
	GANG_WAR_SIDE_ATTACKER,
	GANG_WAR_SIDE_DEFENDER
};
enum
{
	GANG_WAR_PRICE_ZONE,
	GANG_WAR_PRICE_MONEY,
	GANG_WAR_PRICE_SCORE,
	GANG_WAR_PRICE_DELETE
};
enum
{
	GANG_END_LIDER_QUIT,
	GANG_END_LIDER_LEAVE,
	GANG_WAR_END_KILLED
};

// --------------------------------------------------
// statics
// --------------------------------------------------
static
	// gangs
	IsGangActive[MAX_GANGS char],
	GangName[MAX_GANGS][MAX_GANG_NAME],
	GangColor[MAX_GANGS],
	GangKillsAll[MAX_GANGS],
	GangKillsGangs[MAX_GANGS],
	GangScore[MAX_GANGS],
	GangWelcome[MAX_GANGS][MAX_GANG_WELCOME],
	// members
	IsGangMemberActive[MAX_GANGS][MAX_GANG_MEMBERS char],
	GangMemberName[MAX_GANGS][MAX_GANG_MEMBERS][MAX_PLAYER_NAME],
	GangMemberPlayerid[MAX_GANGS][MAX_GANG_MEMBERS];
// colors
static GangColors[][GangColorsInfo] = 
{
	{"Red",				0xAA3333AA},
	{"LightRed",			0xFF6347AA},
	{"Orange",			0xFF9900AA},
	{"Yellow",			0xFFFF00AA},
	{"Green",			0x33AA33AA},
	{"LightGreen",		0x9ACD32AA},
	{"Blue",				0x33AAFFAA},
	{"LightBlue",			0x33CCFFAA},
	{"White",			0xFFFFFFAA},
	{"Purple",			0xC2A2DAAA},
	{"Black",			0x000000AA},
	{"Crimson",			0xDC143CAA},
	{"Limegreen",		0x32CD32AA},
	{"Aqua",			0x00FFFFAA},
	{"Flblue",			0x6495EDAA},
	{"Magenta",			0xFF00FFAA},
	{"Gold",				0xB8860BAA},
	{"Pink",				0xFFC0CBAA},
	{"MidNighBlue",		0x191970AA},
	{"Indigo",			0x4B00B0AA},
	{"Ivory",			0xFFFF82AA},
	{"SeaGreen",			0x20B2AAAA},
	{"Maroon",			0x800000AA},
	{"Olive",			0x808000AA},
	{"Orangered",		0xFF4500AA},
	{"SeaGreen2",		0x2E8B57AA},
	{"SpringGreen",		0x00FF7FAA},
	{"Tomato",			0xFF6347AA},
	{"YellowGreen",		0x9ACD32AA},
	{"MediumAqua",		0x83BFBFAA},
	{"MediumMagenta",	0x8B008BAA}
};
// zones
static
	Float:GangZonesCoords[GANG_ZONES_PER_CITY * 3][4],
	GangZonesServerId[GANG_ZONES_PER_CITY * 3],
	GangZonesOwner[GANG_ZONES_PER_CITY * 3][MAX_GANG_NAME],
	GangZonesLastOwnerLogin[GANG_ZONES_PER_CITY * 3],
	GangZonesColor[GANG_ZONES_PER_CITY * 3],
	GangZonesAttacker[GANG_ZONES_PER_CITY * 3][MAX_GANG_NAME],
	GangZonesCapture[GANG_ZONES_PER_CITY * 3 char];
// citys coords (min x,min y,max x,max y)
static Float:City[3][4] = 
{
	{44.60,		-2892.90,		2997.00,		-768.00},	// Los Santos
	{-2997.40,	-1115.50,		-1213.90,		1659.60},		// San Fierro
	{869.40,		596.30,		2997.00,		2993.80}		// Las Venturas
};
// gang wars
static
	GangWarState[MAX_GANGS char],
	GangWarSide[MAX_GANGS char],
	GangWarWith[MAX_GANGS],
	GangWarPriceType[MAX_GANGS char],
	GangWarPrice[MAX_GANGS],
	GangWarMembersCount[MAX_GANGS char],
	IsGangMemberInWar[MAX_GANGS][MAX_GANG_MEMBERS char],
	GangWarQuest;
static Float:GangWarSpawn[2][MAX_GANG_WAR_SPAWNS][3] =
{
	{
		{0.0,0.0,0.0}
	},
	{
		{0.0,0.0,0.0}
	}
};

// --------------------------------------------------
// stocks
// --------------------------------------------------
stock LoadGang(name[])
{
	// find gang in already loaded
	new gangid = INVALID_GANG_ID;
	for(new i = 0;i < MAX_GANGS;i++)
	{
		if(!IsGangActive{i})
		{
			if(gangid == INVALID_GANG_ID) gangid = i;
			continue;
		}
		if(strcmp(name,GangName[i])) continue;
		
		return i;
	}
	// build file path
	format(stmp,sizeof(stmp),MODE_DIR GANGS_DIR GANGS_MASK_FILE,name);
	// try load gang
	new File:df = fopen(stmp,io_read),
		k;
	if(df)
	{
		new mcount = 1;
		while(fread(df,stmp))
		{
			if(!CheckFileLine(stmp)) continue;
			// parse
			if((k = strfind(stmp,"=")) == -1) continue;
			
			if(!strcmp(stmp,"Name",true,k))
			{
				format(GangName[gangid],MAX_GANG_NAME,stmp[k + 1]);
				continue;
			}
			if(!strcmp(stmp,"Color",true,k))
			{
				GangColor[gangid] = strval(stmp[k + 1]);
				continue;
			}
			if(!strcmp(stmp,"KillsAll",true,k))
			{
				GangKillsAll[gangid] = strval(stmp[k + 1]);
				continue;
			}
			if(!strcmp(stmp,"KillsGangs",true,k))
			{
				GangKillsGangs[gangid] = strval(stmp[k + 1]);
				continue;
			}
			if(!strcmp(stmp,"Score",true,k))
			{
				GangScore[gangid] = strval(stmp[k + 1]);
				continue;
			}
			if(!strcmp(stmp,"Welcome",true,k))
			{
				format(GangWelcome[gangid],MAX_GANG_WELCOME,stmp[k + 1]);
				continue;
			}
			// members
			if(!strcmp(stmp,"Leader",true,k))
			{
				format(GangMemberName[gangid][0],MAX_PLAYER_NAME,stmp[k + 1]);
				IsGangMemberActive[gangid]{0} = 1;
				continue;
			}
			if(!strcmp(stmp,"Member",true,k))
			{
				format(GangMemberName[gangid][mcount],MAX_PLAYER_NAME,stmp[k + 1]);
				IsGangMemberActive[gangid]{mcount} = 1;
				mcount++;
				continue;
			}
		}
		fclose(df);
		// stat
		IsGangActive{gangid} = 1;
		for(new i = mcount;i < MAX_GANG_MEMBERS;i++) IsGangMemberActive[gangid]{i} = 0;
		// zone info
		for(new i = 0;i < sizeof(GangZonesOwner);i++)
		{
			if(strcmp(GangZonesOwner[i],GangName[ itmp[0] ])) continue;
			// owner
			new dt[6];
			getdate(dt[0],dt[1],dt[2]);
			gettime(dt[3],dt[4],dt[5]);
			GangZonesLastOwnerLogin[i] = mktime(dt[3],dt[4],dt[5],dt[2],dt[1],dt[0]);
			// save zone data
			SaveGangZone(i);
		}
		return gangid;
	}
	// not find file
	return INVALID_GANG_ID;
}

stock SaveGang(gangid)
{
	// build file path
	format(stmp,sizeof(stmp),MODE_DIR GANGS_DIR GANGS_MASK_FILE,GangName[gangid]);
	// open
	new File:df = fopen(stmp,io_write);
	// write
	format(stmp,sizeof(stmp),"Name=%s\r\n",GangName[gangid]);
	fwrite(df,stmp);
	format(stmp,sizeof(stmp),"Color=%d\r\n",GangColor[gangid]);
	fwrite(df,stmp);
	format(stmp,sizeof(stmp),"KillsAll=%d\r\n",GangKillsAll[gangid]);
	fwrite(df,stmp);
	format(stmp,sizeof(stmp),"KillsGangs=%d\r\n",GangKillsGangs[gangid]);
	fwrite(df,stmp);
	format(stmp,sizeof(stmp),"Score=%d\r\n",GangScore[gangid]);
	fwrite(df,stmp);
	format(stmp,sizeof(stmp),"Welcome=%s\r\n",GangWelcome[gangid]);
	fwrite(df,stmp);
	format(stmp,sizeof(stmp),"Leader=%s\r\n",GangMemberName[gangid][0]);
	fwrite(df,stmp);
	for(new i = 1;i < MAX_GANG_MEMBERS;i++)
	{
		if(!IsGangMemberActive[gangid]{i}) continue;
		format(stmp,sizeof(stmp),"Member=%s\r\n",GangMemberName[gangid][i]);
		fwrite(df,stmp);
	}
	// close
	fclose(df);
}

stock GangCharCheck(string[])
{
	for(new i = 0;i < strlen(string);i++)
	{
		switch(string[i])
		{
			case 'a'..'z': continue;
			case 'A'..'Z': continue;
			case '0'..'9': continue;
			case '_','[',']': continue;
		}
		return 0;
	}
	return 1;
}

stock GangDelete(gangid)
{
	// null players data
	for(new i = 0;i < MAX_GANG_MEMBERS;i++)
	{
		if(!IsGangMemberActive[gangid]{i}) continue;
		if(GangMemberPlayerid[gangid][i] == INVALID_PLAYER_ID) continue;
		
		SetPVarInt(GangMemberPlayerid[gangid][i],"Gang",INVALID_GANG_ID);
		SetPlayerColor(GangMemberPlayerid[gangid][i],PLAYERS_COLOR);
	}
	// null data
	IsGangActive{gangid} = 0;
	// delete file
	format(stmp,sizeof(stmp),MODE_DIR GANGS_DIR GANGS_MASK_FILE,GangName[gangid]);
	fremove(stmp);
	// null gang zones info
	for(new i = 0;i < sizeof(GangZonesOwner);i++)
	{
		if(strcmp(GangZonesOwner[i],GangName[gangid])) continue;
		// owner
		format(GangZonesOwner[i],MAX_GANG_NAME,"Unknown");
		GangZonesColor[i] = PLAYERS_COLOR;
		if(!GangZonesCapture{i}) GangZoneShowForAll(GangZonesServerId[i],GangZonesColor[i]);
		else 
		{
			GangZoneShowForAll(GangZonesServerId[i],GangZonesColor[i]);
			GangZoneFlashForAll(GangZonesServerId[i],COLOR_RED);
		}
		// save zone data
		SaveGangZone(i);
	}
}

stock LoadGangZone(zoneid)
{
	// build file path
	format(stmp,sizeof(stmp),MODE_DIR GANG_ZONES_DIR GANG_ZONES_MASK_FILE,zoneid);
	// try load gang zone
	new File:df = fopen(stmp,io_read),
		k;
	if(df)
	{
		while(fread(df,stmp))
		{
			if(!CheckFileLine(stmp)) continue;
			// parse
			if((k = strfind(stmp,"=")) == -1) continue;
			
			if(!strcmp(stmp,"Owner",true,k))
			{
				format(GangZonesOwner[zoneid],MAX_GANG_NAME,stmp[k + 1]);
				continue;
			}
			if(!strcmp(stmp,"Color",true,k))
			{
				GangZonesColor[zoneid] = strval(stmp[k + 1]);
				continue;
			}
			if(!strcmp(stmp,"LastOwnerLogin",true,k))
			{
				GangZonesLastOwnerLogin[zoneid] = strval(stmp[k + 1]);
				continue;
			}
		}
		fclose(df);
	}
	else
	{
		format(GangZonesOwner[zoneid],MAX_GANG_NAME,"Unknown");
		GangZonesColor[zoneid] = PLAYERS_COLOR;
		GangZonesLastOwnerLogin[zoneid] = 0;
	}
}

stock SaveGangZone(zoneid)
{
	// build file path
	format(stmp,sizeof(stmp),MODE_DIR GANG_ZONES_DIR GANG_ZONES_MASK_FILE,zoneid);
	// open
	new File:df = fopen(stmp,io_write);
	// write
	format(stmp,sizeof(stmp),"Owner=%s\r\n",GangZonesOwner[zoneid]);
	fwrite(df,stmp);
	format(stmp,sizeof(stmp),"Color=%d\r\n",GangZonesColor[zoneid]);
	fwrite(df,stmp);
	format(stmp,sizeof(stmp),"LastOwnerLogin=%d\r\n",GangZonesLastOwnerLogin[zoneid]);
	fwrite(df,stmp);
	// close
	fclose(df);
}

stock GetPlayerGangZone(playerid)
{
	static Float:pos[3];
	GetPlayerPos(playerid,pos[0],pos[1],pos[2]);
	for(new i = 0;i < sizeof(GangZonesCoords);i++)
	{
		if(GangZonesCoords[i][0] > pos[0]) continue;
		if(GangZonesCoords[i][1] > pos[1]) continue;
		if(GangZonesCoords[i][2] < pos[0]) continue;
		if(GangZonesCoords[i][3] < pos[1]) continue;
		return i;
	}
	return INVALID_GANG_ZONE_ID;
}

stock GangWarComplete(winnergang,losegang,reason)
{
	switch(reason)
	{
		case GANG_END_LIDER_QUIT,GANG_END_LIDER_LEAVE:
		{
			// show msg
			if(reason == GANG_END_LIDER_QUIT) format(stmp,sizeof(stmp),"%s vs %s не состоится по причине выхода из игры лидера банды %s",GangName[winnergang],GangName[losegang],GangName[losegang]);
			else format(stmp,sizeof(stmp),"%s vs %s не состоится по причине выхода из банды лидера банды %s",GangName[winnergang],GangName[losegang],GangName[losegang]);
			if(IsGangActive{losegang})
			{
				// score price
				GangScore[losegang] -= GANG_WAR_SCORE_PRICE * GANG_SCORE_RATE;
				// members
				for(new i = 0;i < MAX_GANG_MEMBERS;i++)
				{
					if(!IsGangMemberActive[losegang]{i}) continue;
					if(GangMemberPlayerid[losegang][i] == INVALID_PLAYER_ID) continue;
					
					SendClientMessage(GangMemberPlayerid[losegang][i],GangColor[losegang],stmp);
					
					if(IsGangMemberInWar[losegang]{i} == 0) continue;
					IsGangMemberInWar[losegang]{i} = 0;
					GangWarMembersCount{losegang}--;
					// dead null stat
					SetPlayerQuest(GangMemberPlayerid[losegang][i],INVALID_QUEST_ID);
					if(GangWarState{losegang} == GANG_WAR_STATE_ON)
					{
						SetPlayerVirtualWorld(GangMemberPlayerid[losegang][i],0);
						SpawnPlayer(GangMemberPlayerid[losegang][i]);
					}
				}
			}
			if(IsGangActive{winnergang})
			{
				// score price
				GangScore[winnergang] += GANG_WAR_SCORE_PRICE * GANG_SCORE_RATE;
				// members
				for(new i = 0;i < MAX_GANG_MEMBERS;i++)
				{
					if(!IsGangMemberActive[winnergang]{i}) continue;
					if(GangMemberPlayerid[winnergang][i] == INVALID_PLAYER_ID) continue;
					
					SendClientMessage(GangMemberPlayerid[losegang][i],GangColor[losegang],stmp);
					
					if(IsGangMemberInWar[winnergang]{i} == 0) continue;
					IsGangMemberInWar[winnergang]{i} = 0;
					GangWarMembersCount{winnergang}--;
					// dead null stat
					SetPlayerQuest(GangMemberPlayerid[winnergang][i],INVALID_QUEST_ID);
					if(GangWarState{winnergang} == GANG_WAR_STATE_ON)
					{
						SetPlayerVirtualWorld(GangMemberPlayerid[winnergang][i],0);
						SpawnPlayer(GangMemberPlayerid[winnergang][i]);
					}
				}
			}
		}
		case GANG_WAR_END_KILLED:
		{
			if(GangWarSide[winnergang] == GANG_WAR_SIDE_ATTACKER) itmp[0] = winnergang;
			else itmp[0] = losegang;
			// give price
			switch(GangWarPriceType{ itmp[0] })
			{
				case GANG_WAR_PRICE_ZONE:
				{
					if(itmp[0] == winnergang) // win attacker
					{
						itmp[1] = GangWarPrice[ itmp[0] ];
						// zone captured
						format(GangZonesOwner[ itmp[1] ],MAX_GANG_NAME,GangName[ itmp[0] ]);
						GangZonesColor[ itmp[1] ] = GangColor[ itmp[0] ];
						GangZonesCapture{ itmp[1] } = 0;
						GangZoneShowForAll(GangZonesServerId[ itmp[1] ],GangZonesColor[ itmp[1] ]);
					}
					else // win defender
					{
						oGivePlayerMoney(GangMemberPlayerid[winnergang][0],ZONE_GANG_WAR_COST);
						oGivePlayerMoney(GangMemberPlayerid[losegang][0],-ZONE_GANG_WAR_COST);
					}
				}
				case GANG_WAR_PRICE_MONEY:
				{
					oGivePlayerMoney(GangMemberPlayerid[winnergang][0],GangWarPrice[ itmp[0] ]);
					oGivePlayerMoney(GangMemberPlayerid[losegang][0],-GangWarPrice[ itmp[0] ]);
				}
				case GANG_WAR_PRICE_SCORE:
				{
					GangScore[winnergang] += GangWarPrice[ itmp[0] ];
					GangScore[losegang] -= GangWarPrice[ itmp[0] ];
				}
				case GANG_WAR_PRICE_DELETE: 
				{
					// delete gang
					GangDelete(losegang);
				}
			}
			// show msg
			format(stmp,sizeof(stmp),"%s win %s",GangName[winnergang],GangName[losegang]);
			if(IsGangActive{losegang})
			{
				// score price
				GangScore[losegang] -= GANG_WAR_SCORE_PRICE * GANG_SCORE_RATE;
				// members
				for(new i = 0;i < MAX_GANG_MEMBERS;i++)
				{
					if(!IsGangMemberActive[losegang]{i}) continue;
					if(GangMemberPlayerid[losegang][i] == INVALID_PLAYER_ID) continue;
					
					SendClientMessage(GangMemberPlayerid[losegang][i],GangColor[losegang],stmp);
					
					if(IsGangMemberInWar[losegang]{i} == 0) continue;
					IsGangMemberInWar[losegang]{i} = 0;
					GangWarMembersCount{losegang}--;
					// dead null stat
					SetPlayerQuest(GangMemberPlayerid[losegang][i],INVALID_QUEST_ID);
					if(GangWarState{losegang} == GANG_WAR_STATE_ON)
					{
						SetPlayerVirtualWorld(GangMemberPlayerid[losegang][i],0);
						SpawnPlayer(GangMemberPlayerid[losegang][i]);
					}
				}
			}
			if(IsGangActive{winnergang})
			{
				// score price
				GangScore[winnergang] += GANG_WAR_SCORE_PRICE * GANG_SCORE_RATE;
				// members
				for(new i = 0;i < MAX_GANG_MEMBERS;i++)
				{
					if(!IsGangMemberActive[winnergang]{i}) continue;
					if(GangMemberPlayerid[winnergang][i] == INVALID_PLAYER_ID) continue;
					
					SendClientMessage(GangMemberPlayerid[losegang][i],GangColor[losegang],stmp);
					
					if(IsGangMemberInWar[winnergang]{i} == 0) continue;
					IsGangMemberInWar[winnergang]{i} = 0;
					GangWarMembersCount{winnergang}--;
					// dead null stat
					SetPlayerQuest(GangMemberPlayerid[winnergang][i],INVALID_QUEST_ID);
					if(GangWarState{winnergang} == GANG_WAR_STATE_ON)
					{
						SetPlayerVirtualWorld(GangMemberPlayerid[winnergang][i],0);
						SpawnPlayer(GangMemberPlayerid[winnergang][i]);
					}
				}
			}
		}
	}
}

stock GetGangWarPriceName(price)
{
	static name[30];
	name = "Unknown";
	switch(price)
	{
		case GANG_WAR_PRICE_ZONE: name = "Зона банды";
		case GANG_WAR_PRICE_MONEY: name = "Деньги";
		case GANG_WAR_PRICE_SCORE: name = "Очки банды";
		case GANG_WAR_PRICE_DELETE: name = "Удаление банды";
	}
	return name;
}

stock oGetPlayerGangName(playerid)
{
	static name[MAX_GANG_NAME];
	if((itmp[0] = GetPVarInt(playerid,"Gang")) == INVALID_GANG_ID) name = "Нет банды";
	else name = GangName[ itmp[0] ];
	return name;
}

// --------------------------------------------------
// forwards
// --------------------------------------------------


// --------------------------------------------------
// publics
// --------------------------------------------------


// --------------------------------------------------
// Obligatory functions
// --------------------------------------------------
Gangs_OnGameModeInit()
{
	// build gang zones
	// create coords
	new gcount,
		Float:add[2],
		Float:current[2];
	for(new i = 0;i < 3;i++)
	{
		add[0] = (City[i][2] - City[i][0]) / GANG_ZONES_REB;
		add[1] = (City[i][3] - City[i][1]) / GANG_ZONES_REB;
		current[0] = City[i][0];
		current[1] = City[i][1];
		for(new a = 0;a < GANG_ZONES_REB;a++)
		{
			current[0] = City[i][0] + (add[0] * a);
			for(new b = 0;b < GANG_ZONES_REB;b++)
			{
				current[1] = City[i][1] + (add[1] * b);
				GangZonesCoords[gcount][0] = current[0];
				GangZonesCoords[gcount][1] = current[1];
				GangZonesCoords[gcount][2] = current[0] + add[0];
				GangZonesCoords[gcount][3] = current[1] + add[1];
				gcount++;
			}
		}
	}
	// create zones
	for(new i = 0;i < gcount;i++)
	{
		GangZonesServerId[i] = GangZoneCreate(GangZonesCoords[i][0],GangZonesCoords[i][1],GangZonesCoords[i][2],GangZonesCoords[i][3]);
		// read zone data
		LoadGangZone(i);
	}
	// gang war quest reg
	GangWarQuest = RegisterQuest();
	// data
#if defined PRINT_STATS_DATA
	printf("Max gangs: %d",MAX_GANGS);
	printf("Max gang members: %d",MAX_GANG_MEMBERS);
	printf("Gang cost: %d",GANG_COST);
	printf("Gang zones: %d",gcount);
	printf("Gang zones per city: %d",GANG_ZONES_PER_CITY);
#endif
	print("Gangs loaded.");
}

Gangs_OnGameModeExit()
{
	// save zones
	for(new i = 0;i < sizeof(GangZonesServerId);i++) SaveGangZone(i);	
}

Gangs_Text(playerid,textid,string[])
{
	switch(textid)
	{
		case GANGS_TEAM_TEXT:
		{
			// check player gang
			if((itmp[0] = GetPVarInt(playerid,"Gang")) == INVALID_GANG_ID) return 0;
			format(stmp,sizeof(stmp),"%s банде: %s",oGetPlayerName(playerid),string);
			// send for all gang
			for(new i = 0;i < MAX_GANG_MEMBERS;i++)
			{
				if(!IsGangMemberActive[ itmp[0] ]{i}) continue;
				if(GangMemberPlayerid[ itmp[0] ][i] == INVALID_PLAYER_ID) continue;
				
				SendClientMessage(GangMemberPlayerid[ itmp[0] ][i],GangColor[ itmp[0] ],stmp);
			}
			return 1;
		}
	}
	return 0;
}

Gangs_Command(playerid,cmdid,params[])
{
	new idx;
	switch(cmdid)
	{
		case GANGS_HELP_CMD:
		{
			stmp[0] = 0;
			strcat(stmp,"Вы можете создать свою банду либо вступить в уже существующую\nЧтобы создать свою банду вы должны иметь нужное количество денег (" #GANG_COST "$)\n");
			strcat(stmp,"Для вступления в банду требуется просто найти лидера и попросить принять.\nБанды могут захватывать зоны чтобы получать очки банды, которые нужны для лидерства\n");
			strcat(stmp,"Для захвата зоны нужно убить 3 членов владеющей банды в этой зоне, либо (если зона ничья) убить 3\nчленов любой банды в этой зоне.\nГлава банды может обьявить войну другой банде со ставкой (деньги / зона / развал банды)\n");
			strcat(stmp,"Команды:\n/g create - создать банду\n/g delete - удалить банду\n/g colors - цвета банд\n/g color - изменить цвет банды\n/g invite - пригласить в банду\n/g kick - выгнать из банды\n/g quit - выйти из банды\n/g welcome - изменить приветствие\n/g members - члены банды\n/g stats - статистика банды");
			ShowPlayerDialog(playerid,DIALOG_NONE_ACTION,DIALOG_STYLE_MSGBOX,MODE_NAME " -> Банды",stmp,"Ок","Ок");
			return 1;
		}
		case GANGS_CREATE_CMD:
		{
			// check quest
			if(GetPlayerQuest(playerid) != INVALID_QUEST_ID) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы выполняете задание");
			// check player gang
			if(GetPVarInt(playerid,"Gang") != INVALID_GANG_ID) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы уже в банде");
			// check moneys
			if(oGetPlayerMoney(playerid) < GANG_COST) return SendClientMessage(playerid,SYSTEM_COLOR,"У вас не хватает денег.Требуется " #GANG_COST "$");
			// get color name
			strtoktmp = strtok(params,idx);
			// find color
			itmp[0] = -1;
			for(new i = 0;i < sizeof(GangColors);i++)
			{
				if(strcmp(GangColors[i][cName],strtoktmp)) continue;
				itmp[0] = GangColors[i][cColor];
				break;
			}
			if(itmp[0] == -1) return SendClientMessage(playerid,SYSTEM_COLOR,"Не верный цвет, введите /g colors для просмотра списка цветов");
			// get name
			strtoktmp = strtok(params,idx);
			// check name
			itmp[1] = strlen(strtoktmp);
			if((itmp[1] < 3) || (itmp[1] > MAX_GANG_NAME)) return SendClientMessage(playerid,SYSTEM_COLOR,"Не верная длинна названия банды, должна быть от 3 до " #MAX_GANG_NAME);
			if(!GangCharCheck(strtoktmp)) return SendClientMessage(playerid,SYSTEM_COLOR,"Используйте в названии банды только a-z , A-Z , 0-9 , _ , [ , ]");
			if(!strcmp("Unknown",strtoktmp)) return SendClientMessage(playerid,SYSTEM_COLOR,"Название банды Unknown недопустимо");
			// check exist
			format(stmp,sizeof(stmp),MODE_DIR GANGS_DIR GANGS_MASK_FILE,strtoktmp);
			if(fexist(stmp)) return SendClientMessage(playerid,SYSTEM_COLOR,"Банда с таким именем уже существует");
			// find free slot
			itmp[1] = INVALID_GANG_ID;
			for(new i = 0;i < MAX_GANGS;i++)
			{
				if(IsGangActive{i}) continue;
				itmp[1] = i;
				break;
			}
			if(itmp[1] == INVALID_GANG_ID) return print("[ERROR] cant find free slot for gang");
			// set data
			IsGangActive{ itmp[1] } = 1;
			format(GangName[ itmp[1] ],MAX_GANG_NAME,strtoktmp);
			GangColor[ itmp[1] ] = itmp[0];
			GangKillsAll[ itmp[1] ] = 0;
			GangKillsGangs[ itmp[1] ] = 0;
			GangScore[ itmp[1] ] = 0;
			format(GangWelcome[ itmp[1] ],MAX_GANG_WELCOME,"Добро пожаловать в %s! (вы можете сменить приветствие командой /g welcome)",strtoktmp);
			format(GangMemberName[ itmp[1] ][0],MAX_PLAYER_NAME,oGetPlayerName(playerid));
			IsGangMemberActive[ itmp[1] ]{0} = 1;
			GangMemberPlayerid[ itmp[1] ][0] = playerid;
			for(new i = 1;i < MAX_GANG_MEMBERS;i++) IsGangMemberActive[ itmp[1] ]{i} = 0;
			// save file
			SaveGang(itmp[1]);
			// set data for player
			SetPVarInt(playerid,"Gang",itmp[1]);
			oGivePlayerMoney(playerid,-GANG_COST);
			SetPlayerColor(playerid,itmp[0]);
			SendClientMessage(playerid,itmp[0],GangWelcome[ itmp[1] ]);
			// result
			SendClientMessage(playerid,SYSTEM_COLOR,"Банда успешно создана");
			return 1;
		}
		case GANGS_DELETE_CMD:
		{
			// check quest
			if(GetPlayerQuest(playerid) != INVALID_QUEST_ID) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы выполняете задание");
			// check player gang
			if((itmp[0] = GetPVarInt(playerid,"Gang")) == INVALID_GANG_ID) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы не в банде");
			// check lider
			if(GangMemberPlayerid[ itmp[0] ][0] != playerid) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы не лидер банды");
			// check gang war
			if(GangWarState{ itmp[0] } != GANG_WAR_STATE_OFF) GangWarComplete(GangWarWith[ itmp[0] ],itmp[0],GANG_END_LIDER_LEAVE);
			// delete gang
			GangDelete(itmp[0]);
			// result
			SendClientMessage(playerid,SYSTEM_COLOR,"Банда удалена");
			return 1;
		}
		case GANGS_COLORS_CMD:
		{
			for(new i = 0;i < sizeof(GangColors);i++) SendClientMessage(playerid,GangColors[i][cColor],GangColors[i][cName]);
			return SendClientMessage(playerid,SYSTEM_COLOR,"Page Up/Down для просмотра всего списка");
		}
		case GANGS_COLOR_CMD:
		{
			// check quest
			if(GetPlayerQuest(playerid) != INVALID_QUEST_ID) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы выполняете задание");
			// check player gang
			if((itmp[0] = GetPVarInt(playerid,"Gang")) == INVALID_GANG_ID) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы не в банде");
			// check lider
			if(GangMemberPlayerid[ itmp[0] ][0] != playerid) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы не лидер банды");
			// get color name
			strtoktmp = strtok(params,idx);
			// find color
			itmp[1] = -1;
			for(new i = 0;i < sizeof(GangColors);i++)
			{
				if(strcmp(GangColors[i][cName],strtoktmp)) continue;
				itmp[1] = GangColors[i][cColor];
				break;
			}
			if(itmp[1] == -1) return SendClientMessage(playerid,SYSTEM_COLOR,"Не верный цвет, введите /g colors для просмотра списка цветов");
			// set data
			GangColor[ itmp[0] ] = itmp[1];
			for(new i = 0;i < MAX_GANG_MEMBERS;i++)
			{
				if(!IsGangMemberActive[ itmp[0] ]{i}) continue;
				if(GangMemberPlayerid[ itmp[0] ][i] == INVALID_PLAYER_ID) continue;
				
				SetPlayerColor(GangMemberPlayerid[ itmp[0] ][i],itmp[1]);
			}
			// change gang zones info
			for(new i = 0;i < sizeof(GangZonesOwner);i++)
			{
				if(strcmp(GangZonesOwner[i],GangName[ itmp[0] ])) continue;
				// owner
				GangZonesColor[i] = itmp[1];
				if(!GangZonesCapture{i}) GangZoneShowForAll(GangZonesServerId[i],GangZonesColor[i]);
				else 
				{
					GangZoneShowForAll(GangZonesServerId[i],GangZonesColor[i]);
					GangZoneFlashForAll(GangZonesServerId[i],COLOR_RED);
				}
				// save zone data
				SaveGangZone(i);
			}
			// result
			SendClientMessage(playerid,SYSTEM_COLOR,"Цвет банды изменён");
			return 1;
		}
		case GANGS_WELCOME_CMD:
		{
			// check quest
			if(GetPlayerQuest(playerid) != INVALID_QUEST_ID) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы выполняете задание");
			// check player gang
			if((itmp[0] = GetPVarInt(playerid,"Gang")) == INVALID_GANG_ID) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы не в банде");
			format(GangWelcome[ itmp[0] ],MAX_GANG_WELCOME,"%s (Изменено %s)",params,oGetPlayerName(playerid));
			SendClientMessage(playerid,GangColor[ itmp[0] ],GangWelcome[ itmp[0] ]);
			// result
			SendClientMessage(playerid,SYSTEM_COLOR,"Приветствие банды изменено");
			return 1;
		}
		case GANGS_INVITE_CMD:
		{
			// check quest
			if(GetPlayerQuest(playerid) != INVALID_QUEST_ID) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы выполняете задание");
			// check player gang
			if((itmp[0] = GetPVarInt(playerid,"Gang")) == INVALID_GANG_ID) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы не в банде");
			// check lider
			if(GangMemberPlayerid[ itmp[0] ][0] != playerid) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы не лидер банды");
			// get player id
			itmp[1] = strval(strtok(params,idx));
			// check online
			if(!IsPlayerConnected(itmp[1])) return SendClientMessage(playerid,SYSTEM_COLOR,"Не верный id игрока");
			// check quest
			if(GetPlayerQuest(itmp[1]) != INVALID_QUEST_ID) return SendClientMessage(playerid,SYSTEM_COLOR,"Игрок на задании");
			// check gang
			if(GetPVarInt(itmp[1],"Gang") != INVALID_GANG_ID) return SendClientMessage(playerid,SYSTEM_COLOR,"Игрок уже в банде");
			// send invite
			SetPVarInt(itmp[1],"GangInvite",itmp[0]);
			format(stmp,sizeof(stmp),"%s приглашает вас в банду %s",oGetPlayerName(playerid),GangName[ itmp[0] ]);
			ShowPlayerDialog(itmp[1],DIALOG_GANG_INVITE,DIALOG_STYLE_MSGBOX,"Приглашение",stmp,"Согласиться","Отказаться");
			format(stmp,sizeof(stmp),"Вы пригласили %s в банду",oGetPlayerName(itmp[1]));
			SendClientMessage(playerid,SYSTEM_COLOR,stmp);
			return 1;
		}
		case GANGS_QUIT_CMD:
		{
			// check quest
			if(GetPlayerQuest(playerid) != INVALID_QUEST_ID) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы выполняете задание");
			// check player gang
			if((itmp[0] = GetPVarInt(playerid,"Gang")) == INVALID_GANG_ID) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы не в банде");
			// check gang war
			if(GangWarState{ itmp[0] } != GANG_WAR_STATE_OFF)
			{
				// check lider
				if(GangMemberPlayerid[ itmp[0] ][0] == playerid) GangWarComplete(GangWarWith[ itmp[0] ],itmp[0],GANG_END_LIDER_LEAVE);
			}
			// leave gang
			new count[2];
			for(new i = 0;i < MAX_GANG_MEMBERS;i++)
			{
				if(!IsGangMemberActive[ itmp[0] ]{i}) continue;
				if(GangMemberPlayerid[ itmp[0] ][i] == playerid) 
				{
					IsGangMemberActive[ itmp[0] ]{i} = 0;
					GangMemberPlayerid[ itmp[0] ][i] = INVALID_PLAYER_ID;
					continue;
				}
				if(GangMemberPlayerid[ itmp[0] ][i] != INVALID_PLAYER_ID) count[0]++;
				count[1]++;
			}
			// if all offline
			if(count[0] == 0) 
			{
				SaveGang(itmp[0]);
				IsGangActive{ itmp[0] } = 0;
			}
			// if gang null
			if(count[1] == 0) GangDelete(itmp[0]);
			// set data
			SetPVarInt(playerid,"Gang",INVALID_GANG_ID);
			SetPlayerColor(playerid,PLAYERS_COLOR);
			return SendClientMessage(playerid,SYSTEM_COLOR,"Вы покинули банду");
		}
		case GANGS_KICK_CMD:
		{
			// check quest
			if(GetPlayerQuest(playerid) != INVALID_QUEST_ID) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы выполняете задание");
			// check player gang
			if((itmp[0] = GetPVarInt(playerid,"Gang")) == INVALID_GANG_ID) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы не в банде");
			// check lider
			if(GangMemberPlayerid[ itmp[0] ][0] != playerid) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы не лидер банды");
			// get name
			strtoktmp = strtok(params,idx);
			// check name
			if(strlen(strtoktmp) < 3) return SendClientMessage(playerid,SYSTEM_COLOR,"Не верный ник");
			// leave gang
			itmp[1] = -2;
			new count[2];
			for(new i = 0;i < MAX_GANG_MEMBERS;i++)
			{
				if(!IsGangMemberActive[ itmp[0] ]{i}) continue;
				if(!strcmp(strtoktmp,GangMemberName[ itmp[0] ][i]))
				{
					itmp[1] = GangMemberPlayerid[ itmp[0] ][i];
					IsGangMemberActive[ itmp[0] ]{i} = 0;
					GangMemberPlayerid[ itmp[0] ][i] = INVALID_PLAYER_ID;
					continue;
				}
				if(GangMemberPlayerid[ itmp[0] ][i] != INVALID_PLAYER_ID) count[0]++;
				count[1]++;
			}
			// check find
			if(itmp[1] == -2) return SendClientMessage(playerid,SYSTEM_COLOR,"Члена банды с таким ником не найдено");
			// check gang war
			if(GangWarState{ itmp[0] } != GANG_WAR_STATE_OFF)
			{
				// check lider
				if(GangMemberPlayerid[ itmp[0] ][0] == itmp[1]) GangWarComplete(GangWarWith[ itmp[0] ],itmp[0],GANG_END_LIDER_LEAVE);
			}
			// if all offline
			if(count[0] == 0) 
			{
				SaveGang(itmp[0]);
				IsGangActive{ itmp[0] } = 0;
			}
			// if gang null
			if(count[1] == 0) GangDelete(itmp[0]);
			// set data
			if(itmp[1] != INVALID_PLAYER_ID)
			{
				SetPVarInt(itmp[1],"Gang",INVALID_GANG_ID);
				SetPlayerColor(itmp[1],PLAYERS_COLOR);
				SendClientMessage(itmp[1],SYSTEM_COLOR,"Вы изгнаны из банды");
			}
			format(stmp,sizeof(stmp),"Вы изгнали %s из банды",strtoktmp);
			return SendClientMessage(playerid,SYSTEM_COLOR,stmp);
		}
		case GANGS_MEMBERS_CMD:
		{
			// check player gang
			if((itmp[0] = GetPVarInt(playerid,"Gang")) == INVALID_GANG_ID) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы не в банде");
			// null str
			stmp[0] = 0;
			// build string
			for(new i = 0;i < MAX_GANG_MEMBERS;i++)
			{
				if(!IsGangMemberActive[ itmp[0] ]{i}) continue;
				format(stmp,sizeof(stmp),"%s%s\n",stmp,GangMemberName[ itmp[0] ][i]);
			}
			// show
			ShowPlayerDialog(playerid,DIALOG_NONE_ACTION,DIALOG_STYLE_MSGBOX,"Члены банды",stmp,"Ок","Ок");
			return 1;
		}
		case GANGS_STATS_CMD:
		{
			// check player gang
			if((itmp[0] = GetPVarInt(playerid,"Gang")) == INVALID_GANG_ID) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы не в банде");
			// null str
			stmp[0] = 0;
			// build string
			format(stmp,sizeof(stmp),"Название:\t\t\t%s\n",GangName[ itmp[0] ]);
			format(stmp,sizeof(stmp),"%sВсего убийств:\t\t\t%d\n",stmp,GangKillsAll[ itmp[0] ]);
			format(stmp,sizeof(stmp),"%sУбийств членов банд:\t\t%d\n",stmp,GangKillsGangs[ itmp[0] ]);
			format(stmp,sizeof(stmp),"%sОчки банды:\t\t\t%d\n",stmp,GangScore[ itmp[0] ]);
			format(stmp,sizeof(stmp),"%sПриветствие банды:\t\t%s\n",stmp,GangWelcome[ itmp[0] ]);
			// show
			ShowPlayerDialog(playerid,DIALOG_NONE_ACTION,DIALOG_STYLE_MSGBOX,"Статистика банды",stmp,"Ок","Ок");
			return 1;
		}
		case GANGS_ZONE_HELP_CMD:
		{
			stmp[0] = 0;
			strcat(stmp,"Каждая банда может захватывать зоны, за захват даются очки банды\n");
			strcat(stmp,"Чтобы захватить зону надо убить 5 членов владеющей банды, либо, если зона ничья или владелец не был на сервере 15 дней,\n5 членов любой банды\n");
			strcat(stmp,"Команды:\n/gz info - информация о зоне в которой находитесь");
			ShowPlayerDialog(playerid,DIALOG_NONE_ACTION,DIALOG_STYLE_MSGBOX,MODE_NAME " -> Зоны банд",stmp,"Ок","Ок");
			return 1;
		}
		case GANGS_ZONE_INFO_CMD:
		{
			// check zone
			if((itmp[0] = GetPlayerGangZone(playerid)) == INVALID_GANG_ZONE_ID) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы не в зоне");
			// null str
			stmp[0] = 0;
			// build string
			format(stmp,sizeof(stmp),"Зона номер:\t\t%d\n",itmp[0]);
			format(stmp,sizeof(stmp),"%sБанда владелец:\t%s\n",stmp,GangZonesOwner[ itmp[0] ]);
			// show
			ShowPlayerDialog(playerid,DIALOG_NONE_ACTION,DIALOG_STYLE_MSGBOX,"Информация о зоне",stmp,"Ок","Ок");
			return 1;
		}
		case GANGS_WAR_HELP_CMD:
		{
			stmp[0] = 0;
			strcat(stmp,"Каждая банда может обьявить войну другой банде, такое \"официальное\" обьявление войны даёт гарантию\nчто заявленая ставка будет передана победителю\n");
			strcat(stmp,"Чтобы обьявить войну лидеры обоих банд должны быть онлайн и заявленые условия боя реальны\n");
			strcat(stmp,"Команды:\n/gw attack - обьявить войну банде\n/gw join - принять участие в войне\n/gw quit - отменить участие в войне");
			ShowPlayerDialog(playerid,DIALOG_NONE_ACTION,DIALOG_STYLE_MSGBOX,MODE_NAME " -> Войны банд",stmp,"Ок","Ок");
			return 1;
		}
		case GANGS_WAR_ATTACK_CMD:
		{
			// check quest
			if(GetPlayerQuest(playerid) != INVALID_QUEST_ID) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы выполняете задание");
			// check player gang
			if((itmp[0] = GetPVarInt(playerid,"Gang")) == INVALID_GANG_ID) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы не в банде");
			// check lider
			if(GangMemberPlayerid[ itmp[0] ][0] != playerid) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы не лидер банды");
			// get gang name
			strtoktmp = strtok(params,idx);
			// check name
			if(strlen(strtoktmp) < 3) return SendClientMessage(playerid,SYSTEM_COLOR,"Не верное название банды");
			// find def gang
			for(new i = 0;i < MAX_GANGS;i++)
			{
				if(strcmp(strtoktmp,GangName[i])) continue;
				// check lider online
				if((itmp[1] = GangMemberPlayerid[i][0]) == INVALID_PLAYER_ID) return SendClientMessage(playerid,SYSTEM_COLOR,"Лидер банды не в сети");
				// get war params
				GangWarPriceType{ itmp[0] } = strval(strtok(params,idx));
				GangWarPrice[ itmp[0] ] = strval(strtok(params,idx));
				// check war params
				switch(GangWarPriceType{ itmp[0] })
				{
					case GANG_WAR_PRICE_ZONE:
					{
						// check zone id
						if((GangWarPrice[ itmp[0] ] < 0) || (GangWarPrice[ itmp[0] ] > GANG_ZONES_PER_CITY * 3)) return SendClientMessage(playerid,SYSTEM_COLOR,"Не верная зона");
						// check zone own
						if(strcmp(GangZonesOwner[ GangWarPrice[ itmp[0] ] ],GangName[i])) return SendClientMessage(playerid,SYSTEM_COLOR,"Зона-приз не принадлежит атакованой банде");
						// check money
						if(oGetPlayerMoney(playerid) < ZONE_GANG_WAR_COST) return SendClientMessage(playerid,SYSTEM_COLOR,"Вам требуется иметь " #ZONE_GANG_WAR_COST " для войны за зону");
					}
					case GANG_WAR_PRICE_MONEY:
					{
						// check money
						if(oGetPlayerMoney(playerid) < GangWarPrice[ itmp[0] ]) return SendClientMessage(playerid,SYSTEM_COLOR,"У вас нет столько денег");
						// check money
						if(oGetPlayerMoney(itmp[1]) < GangWarPrice[ itmp[0] ]) return SendClientMessage(playerid,SYSTEM_COLOR,"У лидера атакованой банды нет столько денег");
					}
					case GANG_WAR_PRICE_SCORE:
					{
						// check score
						if(GangScore[ itmp[0] ] < GangWarPrice[ itmp[0] ]) return SendClientMessage(playerid,SYSTEM_COLOR,"У вашей банды недостаточно очков банды");
						// check score
						if(GangScore[i] < GangWarPrice[ itmp[0] ]) return SendClientMessage(playerid,SYSTEM_COLOR,"У атакованой банды недостаточно очков банды");
					}
					case GANG_WAR_PRICE_DELETE: {}
					default: return SendClientMessage(playerid,SYSTEM_COLOR,"Неизвестный тип ставки");
				}
				// set war data
				GangWarState{ itmp[0] } = GANG_WAR_STATE_LINEUP;
				GangWarState{i} = GANG_WAR_STATE_LINEUP;
				GangWarSide{ itmp[0] } = GANG_WAR_SIDE_ATTACKER;
				GangWarSide{i} = GANG_WAR_SIDE_DEFENDER;
				GangWarWith[ itmp[0] ] = i;
				GangWarWith[i] = itmp[0];
				GangWarMembersCount{ itmp[0] } = 0;
				GangWarMembersCount{i} = 0;
				for(new j = 0;j < MAX_GANG_MEMBERS;j++)
				{
					IsGangMemberInWar[ itmp[0] ]{j} = 0;
					IsGangMemberInWar[i]{j} = 0;
				}
				// send info for def-lider
				// null str
				stmp[0] = 0;
				// build string
				format(stmp,sizeof(stmp),"Банда \"%s\" обьявила вам войну, ставка %s (%d)",GangName[ itmp[0] ],GetGangWarPriceName(GangWarPriceType{ itmp[0] }),GangWarPrice[ itmp[0] ]);
				ShowPlayerDialog(playerid,DIALOG_GANG_WAR,DIALOG_STYLE_MSGBOX,"Война!",stmp,"Согласиться","Отказаться");
				return 1;
			}
			SendClientMessage(playerid,SYSTEM_COLOR,"Не верное название банды");
			return 1;
		}
		case GANGS_WAR_JOIN_CMD:
		{
			// check quest
			if(GetPlayerQuest(playerid) != INVALID_QUEST_ID) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы выполняете задание");
			// check player gang
			if((itmp[0] = GetPVarInt(playerid,"Gang")) == INVALID_GANG_ID) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы не в банде");
			// check gang war
			if(GangWarState{ itmp[0] } != GANG_WAR_STATE_LINEUP) return SendClientMessage(playerid,SYSTEM_COLOR,"В данный момент нет войны");
			// check already exist
			for(new i = 0;i < MAX_GANG_MEMBERS;i++)
			{
				if(!IsGangMemberActive[ itmp[0] ]{i}) continue;
				if(GangMemberPlayerid[ itmp[0] ][i] != playerid) continue;
				if(IsGangMemberInWar[ itmp[0] ]{i} == 1) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы уже записались на войну");
				IsGangMemberInWar[ itmp[0] ]{i} = 1;
				GangWarMembersCount{ itmp[0] }++;
				format(stmp,sizeof(stmp),"%s записался на участие в войне.Записано %d человек",oGetPlayerName(playerid),GangWarMembersCount{ itmp[0] });
				for(new j = 0;j < MAX_GANG_MEMBERS;j++)
				{
					if(!IsGangMemberActive[ itmp[0] ]{j}) continue;
					if(GangMemberPlayerid[ itmp[0] ][j] == INVALID_PLAYER_ID) continue;
					
					SendClientMessage(GangMemberPlayerid[ itmp[0] ][j],GangColor[ itmp[0] ],stmp);
				}
				SetPlayerQuest(playerid,GangWarQuest);
				return 1;
			}
			print("[ERROR] gang war join -> player not find in gang list");
			return 1;
		}
		case GANGS_WAR_LEAVE_CMD:
		{
			// check quest
			if(GetPlayerQuest(playerid) != GangWarQuest) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы не участник войны");
			// check player gang
			if((itmp[0] = GetPVarInt(playerid,"Gang")) == INVALID_GANG_ID) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы не в банде");
			// check gang war
			if(GangWarState{ itmp[0] } != GANG_WAR_STATE_LINEUP) return SendClientMessage(playerid,SYSTEM_COLOR,"В данный момент нет войны");
			// check already exist
			for(new i = 0;i < MAX_GANG_MEMBERS;i++)
			{
				if(!IsGangMemberActive[ itmp[0] ]{i}) continue;
				if(GangMemberPlayerid[ itmp[0] ][i] != playerid) continue;
				if(IsGangMemberInWar[ itmp[0] ]{i} == 0) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы не записаны на войну");
				IsGangMemberInWar[ itmp[0] ]{i} = 0;
				GangWarMembersCount{ itmp[0] }--;
				format(stmp,sizeof(stmp),"%s вышел из участия в войне.Записано %d человек",oGetPlayerName(playerid),GangWarMembersCount{ itmp[0] });
				for(new j = 0;j < MAX_GANG_MEMBERS;j++)
				{
					if(!IsGangMemberActive[ itmp[0] ]{j}) continue;
					if(GangMemberPlayerid[ itmp[0] ][j] == INVALID_PLAYER_ID) continue;
					
					SendClientMessage(GangMemberPlayerid[ itmp[0] ][j],GangColor[ itmp[0] ],stmp);
				}
				SetPlayerQuest(playerid,INVALID_QUEST_ID);
				return 1;
			}
			print("[ERROR] gang war leave -> player not find in gang list");
			return 1;
		}
		case GANGS_WAR_START_CMD:
		{
			// check quest
			if(GetPlayerQuest(playerid) != GangWarQuest) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы не участник войны");
			// check player gang
			if((itmp[0] = GetPVarInt(playerid,"Gang")) == INVALID_GANG_ID) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы не в банде");
			// check lider
			if(GangMemberPlayerid[ itmp[0] ][0] != playerid) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы не лидер банды");
			// check gang war
			if(GangWarState{ itmp[0] } != GANG_WAR_STATE_LINEUP) return SendClientMessage(playerid,SYSTEM_COLOR,"В данный момент нет войны");
			// check members count
			if(GangWarMembersCount{ itmp[0] } != GangWarMembersCount{ GangWarWith[ itmp[0] ] }) return SendClientMessage(playerid,SYSTEM_COLOR,"Не равное количество участников");
			// start
			itmp[1] = GangWarWith[ itmp[0] ];
			GangWarState[ itmp[0] ] = GANG_WAR_STATE_ON;
			GangWarState[ itmp[1] ] = GANG_WAR_STATE_ON;
			// members teleport
			format(stmp,sizeof(stmp),"%s vs %s (%dx%d)",GangName[ itmp[0] ],GangName[ itmp[1] ],GangWarMembersCount{ itmp[0] },GangWarMembersCount{ itmp[1] });
			for(new g = 0;g < 2;g++)
			{
				for(new i = 0;i < MAX_GANG_MEMBERS;i++)
				{
					if(!IsGangMemberActive[ itmp[g] ]{i}) continue;
					if(GangMemberPlayerid[ itmp[g] ][i] == INVALID_PLAYER_ID) continue;
					if(IsGangMemberInWar[ itmp[g] ]{i} == 0) continue;
					// teleport
					itmp[2] = random(MAX_GANG_WAR_SPAWNS);
					SetPlayerPos(GangMemberPlayerid[ itmp[g] ][i],GangWarSpawn[g][ itmp[2] ][0],GangWarSpawn[g][ itmp[2] ][1],GangWarSpawn[g][ itmp[2] ][2]);
					SetPlayerVirtualWorld(GangMemberPlayerid[ itmp[g] ][i],GANG_WAR_VIRTUAL_WORLD + itmp[0]);
					// stat
					oSetPlayerHealth(GangMemberPlayerid[ itmp[g] ][i],100.0);
					oSetPlayerArmour(GangMemberPlayerid[ itmp[g] ][i],100.0);
					// show weapon dialog
					ShowPlayerDialog(GangMemberPlayerid[ itmp[g] ][i],DIALOG_GANG_WAR_WEAPONS,DIALOG_STYLE_LIST,"Оружие",GANG_WAR_WEAPONS_STRING,"Взять","Готово");
					// send msg
					SendClientMessage(GangMemberPlayerid[ itmp[g] ][i],GangColor[ itmp[g] ],stmp);
				}
			}
			return 1;
		}
	}
	return 0;
}

Gangs_OnPlayerConnect(playerid)
{
	SetPVarInt(playerid,"Gang",INVALID_GANG_ID);
	SetPVarInt(playerid,"GangInvite",INVALID_GANG_ID);
}

Gangs_OnPlayerSpawn(playerid)
{
	for(new i = 0;i < sizeof(GangZonesServerId);i++) 
	{
		if(!GangZonesCapture{i}) GangZoneShowForPlayer(playerid,GangZonesServerId[i],GangZonesColor[i]);
		else 
		{
			GangZoneShowForPlayer(playerid,GangZonesServerId[i],GangZonesColor[i]);
			GangZoneFlashForPlayer(playerid,GangZonesServerId[i],COLOR_RED);
		}
	}
	// check quest
	if(GetPlayerQuest(playerid) != INVALID_QUEST_ID) return;
	// check gang
	if((itmp[0] = GetPVarInt(playerid,"Gang")) != INVALID_GANG_ID) SetPlayerColor(playerid,GangColor[ itmp[0] ]);
}

Gangs_OnPlayerDeath(playerid,killerid,reason)
{
	#pragma unused reason
	// check gang war
	if((GetPlayerQuest(playerid) == GangWarQuest) && (GetPlayerQuest(killerid) == GangWarQuest))
	{
		// check killer gang
		if((itmp[0] = GetPVarInt(killerid,"Gang")) == INVALID_GANG_ID) return;
		// check player gang
		if((itmp[1] = GetPVarInt(playerid,"Gang")) == INVALID_GANG_ID) return;
		if((GangWarState[ itmp[0] ] == GANG_WAR_STATE_ON) && (GangWarState[ itmp[1] ] == GANG_WAR_STATE_ON))
		{
			static mem[2];
			mem[0] = -1;
			mem[1] = -1;
			// find killer
			for(new i = 0;i < MAX_GANG_MEMBERS;i++)
			{
				if(!IsGangMemberActive[ itmp[0] ]{i}) continue;
				if(GangMemberPlayerid[ itmp[0] ][i] != killerid) continue;
				if(IsGangMemberInWar[ itmp[0] ]{i} == 0) continue;
				
				mem[0] = i;
				break;
			}
			// find dead
			for(new i = 0;i < MAX_GANG_MEMBERS;i++)
			{
				if(!IsGangMemberActive[ itmp[1] ]{i}) continue;
				if(GangMemberPlayerid[ itmp[1] ][i] != playerid) continue;
				if(IsGangMemberInWar[ itmp[1] ]{i} == 0) continue;
				
				mem[1] = i;
				break;
			}
			// check finded
			if((mem[0] != -1) && (mem[1] != -1))
			{
				// players in war
				IsGangMemberInWar[ itmp[1] ]{ mem[1] } = 0;
				GangWarMembersCount{ itmp[1] }--;
				// dead null stat
				SetPlayerQuest(playerid,INVALID_QUEST_ID);
				SetPlayerVirtualWorld(playerid,0);
				// check end
				if(GangWarMembersCount{ itmp[1] } == 0)
				{
					// gang war end
					if(itmp[0] != itmp[1]) GangWarComplete(itmp[0],itmp[1],GANG_WAR_END_KILLED);
					else GangWarComplete(GangWarWith[ itmp[1] ],itmp[1],GANG_WAR_END_KILLED);
					// end work
					return;
				}
			}
		}
	}
	// check quest
	if(GetPlayerQuest(playerid) != INVALID_QUEST_ID) return;
	if(GetPlayerQuest(killerid) != INVALID_QUEST_ID) return;
	// check killer gang
	if((itmp[0] = GetPVarInt(killerid,"Gang")) == INVALID_GANG_ID) return;
	GangKillsAll[ itmp[0] ]++;
	// check player gang
	if((itmp[1] = GetPVarInt(playerid,"Gang")) == INVALID_GANG_ID) return;
	// check team kill
	if(itmp[0] == itmp[1])
	{
		format(stmp,sizeof(stmp),"%s убил %s",oGetPlayerName(playerid),oGetPlayerName(killerid));
		// send for all gang
		for(new i = 0;i < MAX_GANG_MEMBERS;i++)
		{
			if(!IsGangMemberActive[ itmp[1] ]{i}) continue;
			if(GangMemberPlayerid[ itmp[1] ][i] == INVALID_PLAYER_ID) continue;
			
			SendClientMessage(GangMemberPlayerid[ itmp[1] ][i],GangColor[ itmp[1] ],stmp);
		}
		return;
	}
	// rate
	if(GangScore[ itmp[1] ] > GangScore[ itmp[0] ]) ftmp[0] = 1.5;
	else if(GangScore[ itmp[1] ] == GangScore[ itmp[0] ]) ftmp[0] = 1.0;
	else if(GangScore[ itmp[1] ] < GangScore[ itmp[0] ]) ftmp[0] = 0.25;
	// add score
	GangScore[ itmp[0] ] += floatround(GANG_SCORE_RATE * ftmp[0]);
	// add gang kill
	GangKillsGangs[ itmp[0] ] ++;
	// find zone
	if((itmp[2] = GetPlayerGangZone(playerid)) == INVALID_GANG_ZONE_ID) return;
	// check player
	if((strcmp(GangZonesOwner[ itmp[2] ],GangName[ itmp[1] ]) == 0) || (strcmp(GangZonesOwner[ itmp[2] ],"Unknown") == 0))
	{
		// killed owner of zone
		if(!strcmp(GangZonesAttacker[ itmp[2] ],GangName[ itmp[0] ])) GangZonesCapture{ itmp[2] }++;
		else GangZonesCapture{ itmp[2] } = 0;
		if(GangZonesCapture{ itmp[2] } == KILLS_FOR_CAPTURE)
		{
			// zone captured
			format(GangZonesOwner[ itmp[2] ],MAX_GANG_NAME,GangName[ itmp[0] ]);
			GangZonesColor[ itmp[2] ] = GangColor[ itmp[0] ];
			GangZonesCapture{ itmp[2] } = 0;
			GangZoneShowForAll(GangZonesServerId[ itmp[2] ],GangZonesColor[ itmp[2] ]);
			// attacker bonus
			GangScore[ itmp[0] ] += floatround((GANG_SCORE_RATE * 10) * ftmp[0]);
		}
		else if(GangZonesCapture{ itmp[2] } == 1) GangZoneFlashForAll(GangZonesServerId[ itmp[2] ],COLOR_RED);
		else if(GangZonesCapture{ itmp[2] } == 0) GangZoneShowForAll(GangZonesServerId[ itmp[2] ],GangZonesColor[ itmp[2] ]);
		// attacker
		format(GangZonesAttacker[ itmp[2] ],MAX_GANG_NAME,GangName[ itmp[0] ]);
	}
	else if(strcmp(GangZonesOwner[ itmp[2] ],"Unknown"))
	{
		new dt[6];
		getdate(dt[0],dt[1],dt[2]);
		gettime(dt[3],dt[4],dt[5]);
		itmp[3] = mktime(dt[3],dt[4],dt[5],dt[2],dt[1],dt[0]);
		if((itmp[3] - GangZonesLastOwnerLogin[ itmp[2] ]) > MAX_UNLOGGED_ZONE_TIME)
		{
			// killed owner of zone
			if(!strcmp(GangZonesAttacker[ itmp[2] ],GangName[ itmp[0] ])) GangZonesCapture{ itmp[2] }++;
			else GangZonesCapture{ itmp[2] } = 0;
			if(GangZonesCapture{ itmp[2] } == KILLS_FOR_CAPTURE)
			{
				// zone captured
				format(GangZonesOwner[ itmp[2] ],MAX_GANG_NAME,GangName[ itmp[0] ]);
				GangZonesColor[ itmp[2] ] = GangColor[ itmp[0] ];
				GangZonesCapture{ itmp[2] } = 0;
				GangZoneShowForAll(GangZonesServerId[ itmp[2] ],GangZonesColor[ itmp[2] ]);
				// attacker bonus
				GangScore[ itmp[0] ] += floatround((GANG_SCORE_RATE * 10) * ftmp[0]);
			}
			else if(GangZonesCapture{ itmp[2] } == 1) GangZoneFlashForAll(GangZonesServerId[ itmp[2] ],COLOR_RED);
			else if(GangZonesCapture{ itmp[2] } == 0) GangZoneShowForAll(GangZonesServerId[ itmp[2] ],GangZonesColor[ itmp[2] ]);
			// attacker
			format(GangZonesAttacker[ itmp[2] ],MAX_GANG_NAME,GangName[ itmp[0] ]);
		}
	}
}

Gangs_Dialog(playerid,dialogid,response,listitem,inputtext[])
{
	#pragma unused listitem
	#pragma unused inputtext
	switch(dialogid)
	{
		case DIALOG_GANG_INVITE:
		{
			if(response)
			{
				itmp[0] = GetPVarInt(playerid,"GangInvite");
				if(itmp[0] == INVALID_GANG_ID) return 1;
				// find free slot
				new mid = -1;
				for(new i = 0;i < MAX_GANG_MEMBERS;i++)
				{
					if(IsGangMemberActive[ itmp[0] ]{i}) continue;
					mid = i;
				}
				// check slot
				if(mid == -1)
				{
					SendClientMessage(playerid,SYSTEM_COLOR,"В банде нет мест");
					return 1;
				}
				// set data for gang
				IsGangMemberActive[ itmp[0] ]{mid} = 1;
				format(GangMemberName[ itmp[0] ][mid],MAX_PLAYER_NAME,oGetPlayerName(playerid));
				GangMemberPlayerid[ itmp[0] ][mid] = playerid;
				// set data for player
				SetPVarInt(playerid,"Gang",itmp[0]);
				SetPlayerColor(playerid,GangColor[ itmp[0] ]);
				SendClientMessage(playerid,GangColor[ itmp[0] ],GangWelcome[ itmp[0] ]);
			}
			else
			{
				itmp[0] = GetPVarInt(playerid,"GangInvite");
				if(itmp[0] == INVALID_GANG_ID) return 1;
				if(GangMemberPlayerid[ itmp[0] ][0] == INVALID_PLAYER_ID) return 1;
				format(stmp,sizeof(stmp),"%s отказался вступать в банду",oGetPlayerName(playerid));
				SendClientMessage(GangMemberPlayerid[ itmp[0] ][0],SYSTEM_COLOR,stmp);
			}
			return 1;
		}
	}
	return 1;
}

Gangs_OnSavePlayerData(playerid,File:datafile,reason)
{
	static gangid;
	gangid = GetPVarInt(playerid,"Gang");
	if(gangid != INVALID_GANG_ID)
	{
		format(stmp,sizeof(stmp),"Gang=%s\r\n",GangName[gangid]);
		fwrite(datafile,stmp);
		SaveGang(gangid);
		if(reason == SAVE_REASON_DISCONNECT)
		{
			// check gang war
			if(GangWarState{gangid} != GANG_WAR_STATE_OFF)
			{
				// check lider
				if(GangMemberPlayerid[gangid][0] == playerid) GangWarComplete(GangWarWith[gangid],gangid,GANG_END_LIDER_QUIT);
			}
			new count;
			for(new i = 0;i < MAX_GANG_MEMBERS;i++)
			{
				if(!IsGangMemberActive[gangid]{i}) continue;
				if(GangMemberPlayerid[gangid][i] == playerid) GangMemberPlayerid[gangid][i] = INVALID_PLAYER_ID;
				if(GangMemberPlayerid[gangid][i] != INVALID_PLAYER_ID) count++;
			}
			if(count == 0) IsGangActive{gangid} = 0;
		}
	}
	else
	{
		fwrite(datafile,"Gang=\r\n");
	}
	// save zones
	for(new i = 0;i < sizeof(GangZonesServerId);i++) SaveGangZone(i);
}

Gangs_OnLoadPlayerData(playerid,datastr[],separatorpos)
{
	if(!strcmp(datastr,"Gang",true,separatorpos)) 
	{
		if(strlen(datastr[separatorpos + 1]) > 0)
		{
			new gangid = LoadGang(datastr[separatorpos + 1]);
			new name[MAX_PLAYER_NAME];
			GetPlayerName(playerid,name,sizeof(name));
			SetPVarInt(playerid,"Gang",gangid);
			if(gangid != INVALID_GANG_ID)
			{
				for(new i = 0;i < MAX_GANG_MEMBERS;i++)
				{
					if(!IsGangMemberActive[gangid]{i}) continue;
					if(strcmp(name,GangMemberName[gangid][i])) continue;
					
					GangMemberPlayerid[gangid][i] = playerid;
					// gang member logged
					break;
				}
				SendClientMessage(playerid,GangColor[gangid],GangWelcome[gangid]);
			}						
		}
		else SetPVarInt(playerid,"Gang",INVALID_GANG_ID);
	}
}
