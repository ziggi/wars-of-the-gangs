/*
*	Created:			19.05.10
*	Author:				009
*	Description:		Командные соревнования
*/

#if defined _team_quests_included
	#endinput
#endif

#define _team_quests_included
#pragma library team_quests

// --------------------------------------------------
// defines
// --------------------------------------------------
#define TEAM_QUESTS_DIR				"team quests/"
#define TN_QUESTIONS_FILE			"tn_questions.txt"
#define TEAM_QUESTS_VIRTUAL_WORLD 	50
#define TEAM_QUESTS_COLOR			0xFF990000
#define TEAM_QUESTS_TICK_TIME		1000
#define TEAM_QUESTS_LINEUP_TIME		60
#define TEAM_QUESTS_TACTIC_TIME		30
#define TEAM_QUESTS_ACTION_TIME		600
#define	INVALID_TEAM_QUEST_ID		255
#define MAX_TEAM_QUESTS				5
#define MAX_TEAM_QUEST_NAME			48

#define MAX_RK_SPAWNS 	4
#define MAX_MI_SPAWNS 	6
#define MAX_BL_SPAWNS 	3
#define MAX_TN_SPAWNS 	4
#define MAX_MK_SPAWNS 	3

// Rob of the Kaligula
#define TEAM_QUEST_RK_TEAM_0_CP_1 		2144.120117,1626.989624,990.388171
#define TEAM_QUEST_RK_TEAM_0_CP_2 		2144.506591,1633.969970,993.0
#define TEAM_QUEST_RK_TEAM_0_CP_3 		1962.8350,974.1068,994.4688
#define TEAM_QUEST_RK_OBJECT_1_MODEL 	3037
#define TEAM_QUEST_RK_OBJECT_1 			2144.120117,1626.989624,993.688977,0.000000,0.000000,90.000000
#define TEAM_QUEST_RK_OBJECT_1_MOVE		2144.120117,1626.989624,990.688977
#define TEAM_QUEST_RK_VEHICLE_1_MODEL 	580
#define TEAM_QUEST_RK_VEHICLE_1 		2332.9409,1674.4330,10.5405,179.9665,53,53
#define TEAM_QUEST_RK_VEHICLE_2_MODEL 	580
#define TEAM_QUEST_RK_VEHICLE_2 		2332.6904,1686.7318,10.5405,180.7858,92,92
#define TEAM_QUEST_RK_VEHICLE_3_MODEL 	598
#define TEAM_QUEST_RK_VEHICLE_3 		2316.8081,1729.6721,10.5677,231.5614,0,1
#define TEAM_QUEST_RK_VEHICLE_4_MODEL 	598
#define TEAM_QUEST_RK_VEHICLE_4 		2316.7852,1735.7813,10.5612,233.5686,0,1
#define TEAM_QUEST_RK_PICKUP_1_MODEL 	371
#define TEAM_QUEST_RK_PICKUP_1 			15,2265.0120,1672.3837,94.9219
#define TEAM_QUEST_RK_PICKUP_2_MODEL 	371
#define TEAM_QUEST_RK_PICKUP_2 			15,2265.9739,1623.4060,94.9219
// Mission imposible
#define TEAM_QUEST_MI_PICKUP_1_MODEL 	370
#define TEAM_QUEST_MI_PICKUP_1 			23,268.443756,1883.829223,-30.093750
#define TEAM_QUEST_MI_TEAM_0_CP_1 		268.443756,1883.829223,-30.093750
#define TEAM_QUEST_MI_TEAM_0_CP_2 		422.022247,2436.278564,16.5
#define TEAM_QUEST_MI_TEAM_1_CP_1 		211.479705,1812.291015,21.859405
#define TEAM_QUEST_MI_EXPLOSION_XY 		268.443756,1883.829223
// Baggage or life
#define TEAM_QUEST_BL_TEAM_1_CP 		-1844.323486,153.349609,15.117187
#define TEAM_QUEST_BL_VEHICLE_1_MODEL 	515
#define TEAM_QUEST_BL_VEHICLE_1 		-78.106750,-1119.252929,2.112474,159.753051,-1,-1
#define TEAM_QUEST_BL_VEHICLE_2_MODEL 	584
#define TEAM_QUEST_BL_VEHICLE_2 		-76.172515,-1109.588378,2.244177,159.238739,-1,-1
#define TEAM_QUEST_BL_VEHICLE_3_MODEL 	405
#define TEAM_QUEST_BL_VEHICLE_3 		-59.785350,-1132.127807,1.078125,0.000000,-1,-1
#define TEAM_QUEST_BL_VEHICLE_4_MODEL 	405
#define TEAM_QUEST_BL_VEHICLE_4 		-61.077449,-1136.625732,1.078125,0.000000,-1,-1
#define TEAM_QUEST_BL_VEHICLE_5_MODEL 	405
#define TEAM_QUEST_BL_VEHICLE_5 		-62.418975,-1141.730224,1.078125,0.000000,-1,-1
#define TEAM_QUEST_BL_VEHICLE_6_MODEL 	470
#define TEAM_QUEST_BL_VEHICLE_6 		350.557312,-1809.040649,4.529761,359.961242,-1,-1
#define TEAM_QUEST_BL_VEHICLE_7_MODEL 	490
#define TEAM_QUEST_BL_VEHICLE_7 		347.058227,-1810.006713,4.630661,1.688807,-1,-1
#define TEAM_QUEST_BL_VEHICLE_8_MODEL 	490
#define TEAM_QUEST_BL_VEHICLE_8 		353.418792,-1810.059692,4.635127,359.211639,-1,-1
// treasure of nation
#define MAX_TN_QUESTION					50
// Mountain king
#define TEAM_QUEST_MK_CP 				-2230.105224,-1740.276611,480.867401
#define MOUNTAIN_KING_DEF_TIME			30

// --------------------------------------------------
// enums
// --------------------------------------------------
enum
{
	TEAM_QUEST_RK,
	TEAM_QUEST_MI,
	TEAM_QUEST_BL,
	TEAM_QUEST_TN,
	TEAM_QUEST_MK
};
enum
{
	TEAM_QUEST_STATUS_OFF,
	TEAM_QUEST_STATUS_LINEUP,
	TEAM_QUEST_STATUS_TACTIC,
	TEAM_QUEST_STATUS_ACTION
};
enum TeamQuestInfo 
{
	tq_name[MAX_TEAM_QUEST_NAME],
	tq_xp,
	tq_money,
	tq_team_min,
	tq_team_skin[2],
	tq_team1_weapons[3],
	tq_team2_weapons[3]
};
enum
{
	TEAM_QUEST_HELP_CMD,
	TEAM_QUEST_MENU_CMD,
	TEAM_QUEST_QUIT_CMD
};
enum
{
	TEAM_QUEST_TEAM_TEXT,
	TEAM_QUEST_ANSWER_TEXT
};
enum
{
	TEAM_QUEST_REASON_NOT_PLAYERS,
	TEAM_QUEST_REASON_WIN_TEAM_1,
	TEAM_QUEST_REASON_WIN_TEAM_2,
	TEAM_QUEST_REASON_TIME_END
};
enum
{
	TEAM_QUEST_LEAVE_REASON_DEATH,
	TEAM_QUEST_LEAVE_REASON_OFF,
	TEAM_QUEST_LEAVE_REASON_CMD
};

// --------------------------------------------------
// statics
// --------------------------------------------------
static TeamQuests[MAX_TEAM_QUESTS][TeamQuestInfo] = 
{
	{"Ограбление казино Калигула",10000,50000,0,{120,164},{4,23,45},{5,22,25}},
	{"Миссия невыполнима",25000,75000,0,{285,287},{4,23,45},{22,25,31}},
	{"Груз важнее жизни",50000,100000,0,{50,57},{29,0,0},{29,0,0}},
	{"Сокровище нации",10_000_000,1_000_000,0,{83,147},{29,0,0},{29,0,0}},
	{"Царь горы",1000,5000,0,{51,52},{29,30,34},{29,30,34}}
};
static
	TeamQuestStatus[MAX_TEAM_QUESTS char],
	TeamQuestTeamCount[MAX_TEAM_QUESTS][2 char],
	TeamQuestTime[MAX_TEAM_QUESTS],
	TeamQuestQuest[MAX_TEAM_QUESTS],
	TeamQuestTmp[MAX_TEAM_QUESTS][10];	
// spawns
static Float:TeamQuest_RK_Spawns[2][MAX_RK_SPAWNS][3] = 
{
	{ // robers
		{2243.740966,1688.556518,1008.359375},
		{2227.877197,1688.831176,1008.359375},
		{2238.508544,1703.882690,1009.179077},
		{2229.584960,1704.106201,1009.248229}
	},
	{ // cops
		{2152.296875,1622.929199,993.688171},
		{2135.090576,1623.297241,993.688171},
		{2143.512451,1606.643798,1000.971069},
		{2152.906250,1599.629882,1006.172302}
	}
};
static Float:TeamQuest_MI_Spawns[2][MAX_MI_SPAWNS][3] = 
{
	{ // attack
		{-49.294357,2072.699951,17.555721},
		{-47.585239,2057.252929,17.632946},
		{-52.384250,2056.553466,17.674926},
		{-54.134395,2072.493164,17.555400},
		{-60.181858,2072.125000,17.546016},
		{-59.031368,2057.379882,17.583068}
	},
	{ // def
		{128.770751,1934.359008,19.257883},
		{142.432418,1934.596191,19.256746},
		{161.727767,1933.309204,25.498508},
		{104.212768,1901.538696,25.498508},
		{205.451690,1898.480468,17.640625},
		{221.832519,1898.567382,17.640625}
	}
};
static Float:TeamQuest_BL_Spawns[2][MAX_BL_SPAWNS][3] = 
{
	{ // attack
		{-81.221054,-1137.284179,1.078125},
		{-84.250183,-1136.348022,1.078125},
		{-87.351417,-1135.495727,1.078125}
	},
	{ // trucker
		{355.700439,-1816.465332,4.325980},
		{350.031280,-1816.377807,4.319132},
		{342.013488,-1816.072875,4.321673}
	}
};
static Float:TeamQuest_TN_Spawns[2][MAX_TN_SPAWNS][3] = 
{
	{ // team 1
		{1712.342895,-2427.407470,13.554687},
		{1709.223510,-2410.996826,13.554687},
		{1733.555053,-2409.545410,13.554687},
		{1738.601440,-2423.662597,13.554687}
	},
	{ // team 2
		{1628.258422,-2416.765136,13.554687},
		{1629.885253,-2403.839843,13.554687},
		{1662.661499,-2407.306884,13.554687},
		{1659.526733,-2418.899902,13.554687}
	}
};
static Float:TeamQuest_MK_Spawns[2][MAX_MK_SPAWNS][3] = 
{
	{ // team 1
		{-2413.781005,-2172.080566,33.289062},
		{-2411.724609,-2172.296875,33.289062},
		{-2410.155761,-2172.382812,33.289062}
	},
	{ // team 2
		{-2414.726562,-2194.633544,33.289062},
		{-2412.753173,-2194.546875,33.289062},
		{-2411.010009,-2194.562011,33.289062}
	}
};
// team names
static TeamQuests_Teams[MAX_TEAM_QUESTS][2][24] =
{
	{"воры",			"охранники"},
	{"воры",			"охранники"},
	{"дальнобойщики",	"воры"},
	{"комманда 1",		"комманда 2"},
	{"комманда 1",		"комманда 2"}
};
// missions
static TeamQuests_Missions[MAX_TEAM_QUESTS * 2][5][MAX_STRING] =
{
	{ // RK team 1
		"Ваша задача: пройти в хранилище внизу и забрать как можно больше денег!",
		"Когда решите что вам хватит,идите на крышу,и прыгайте на задний двор казино",
		"там ждет 2 машины,садитесь и едте в казино '4 Дракона'!Если при ограблении",
		"жертвы будут минимальны(не все полицейские умрут),то вам будет бонус 500 000$",
		"Если жертв не будет вообще,то вам будет бонус 1 000 000$."
	},
	{ // RK team 2
		"Ваша задача: охранять хранилище!Находится долгое время на нижнем этаже нельзя!",
		"Поднимитесь на верх и патрулируйте игровой зал!Если заметите грабителя,вы должны",
		"смотря на него,на расстоянии не больше 5 метров нажать кнопку 'ДЕЙСТВИЕ',если",
		"после нажатия клавиши в течении 10 секунд ваши НР не изменятся,то включится тревога.",
		"После включения тревоги закроются двери хранилища!Вам нужно будет убить грабителей."
	},
	{ // MI team 1
		"Ваша задача: в глубинах базы находится секретная разработка",
		"Вам нужно добраться до неё и украсть её!после чего вы должны",
		"укрыться в ангаре заброшеного аэропорта.Сложность заключается в том",
		"что разработка охраняется военными с базы.Вам нужно скрытно добраться",
		"до разработки,после чего выбраться будет не трудно."
	},
	{ // MI team 2
		"Ваша задача: на базе хранится JetPack который очень",
		"привлекает воров своими возможностями.Вам нужно в течении 5 минут",
		"охранять его.Вы должны патрулировать месность,двигаться иначе",
		"вы покинете TeamQuest.Использовать оружие разрешается.",
		""
	},
	{ // BL team 1
		"Ваша задача: отвезти тягач с трейлером в назначеное место",
		"Тягач можно сопровождать на машинах,но отезжать на расстояние > 20 метров нельзя",
		"Если фура отцепится вы проиграете!5 минут на всё",
		"",
		""
	},
	{ // BL team 2
		"Ваша задача: уничтожить бензовоз!",
		"Его могут охранять,старайтесь пробить колёса тягача либо отцепить трейлер",
		"",
		"",
		""
	},
	{ // TN team 1
		"Ваша задача: найти сокровища, ищите подсказки, убивать противников нельзя",
		"",
		"",
		"",
		""
	},
	{ // TN team 2
		"Ваша задача: найти сокровища, ищите подсказки, убивать противников нельзя",
		"",
		"",
		"",
		""
	},
	{ // MK team 1
		"Ваша задача: добраться до точки на карте быстрее противников",
		"Требуется продержаться 3 минуты для захвата точки",
		"Если на точке никого из команды нету захват анулируется",
		"",
		""
	},
	{ // MK team 2
		"Ваша задача: добраться до точки на карте быстрее противников",
		"Требуется продержаться 3 минуты для захвата точки",
		"Если на точке никого из команды нету захват анулируется",
		"",
		""
	}
};
// Treasure of nation
static Float:TeamQuest_TN_Pickups[][3] =
{
	{-564.040039,1825.176513,143.840484},
	{-366.298614,1595.400634,83.359123},
	{-206.290405,1786.078125,99.935829},
	{-182.490783,1882.378295,115.703125},
	{-662.738830,2305.898681,136.078323},
	{-771.749389,2423.726074,157.079681},
	{-1131.922851,2261.213134,95.621635},
	{-769.554748,1420.420166,13.945312},
	{-685.294067,938.945800,13.632812},
	{-1255.957275,33.982192,14.148437},
	{-1738.954956,-580.668151,35.155700},
	{-2111.024414,-444.306976,38.734375},
	{-2185.574462,-261.734985,40.719497},
	{-2080.498779,242.908599,105.888961},
	{-2346.376220,536.659973,77.403594},
	{-2695.062011,931.967651,67.202529},
	{-2513.625488,795.727661,42.094375},
	{-2113.367919,655.416931,60.700542},
	{-1951.880249,658.475830,102.771804},
	{-2528.856201,-714.201171,139.320312},
	{-2229.824462,-1740.697875,480.871185},
	{-2157.125732,-2457.510986,35.312500},
	{-1520.875488,-2263.330566,-4.687953},
	{-1113.273193,-1636.938964,76.367187},
	{-793.277221,-1966.966430,1.371621},
	{392.227416,-1924.075805,10.250000},
	{610.008300,-1873.239257,8.339788},
	{1096.001831,-1428.539306,22.771385},
	{1039.393066,-1343.580810,21.892923},
	{1093.559082,-805.200317,107.419723},
	{1621.008056,-1112.978637,57.468750},
	{1684.202148,-1181.334716,54.101562},
	{2244.618652,-1368.952514,41.295433},
	{2505.410400,-1696.262451,13.553318},
	{2788.901855,-1416.675781,16.250000},
	{2741.692382,-2016.295654,13.554687},
	{2629.700439,-2108.169189,16.953125},
	{2099.134277,1683.160034,13.005955},
	{2117.156494,1683.101806,13.005955},
	{2135.036376,1683.266845,13.005955},
	{2219.548339,1838.309326,19.634431},
	{2080.750244,1905.987548,14.583481},
	{2066.279296,2171.972412,10.820312},
	{2104.914306,2370.189697,75.408424},
	{2186.229003,2416.346435,73.033859},
	{2317.023437,2352.844238,17.740331},
	{2267.658935,2448.478759,3.531250},
	{2816.997558,2203.779052,11.023437},
	{2296.722412,1932.637695,31.779651},
	{2399.407470,1865.877441,76.551963},
	{2323.497802,1304.999877,67.468750},
	{2301.375000,1283.105346,67.468750},
	{2323.198486,1261.829345,67.468750},
	{2345.206542,1286.239990,67.468750},
	{2176.609375,1285.848266,41.182403},
	{2106.939208,1003.798217,45.664062},
	{2247.130126,1039.486450,48.218750},
	{2236.916748,1133.837768,49.101562}
};
static TeamQuest_TN_Box[] =
{
	1220,
	1221,
	1224,
	1271
};
static TeamQuest_TN_Answer[2][MAX_TN_QUESTION];
// other
static 
	TeamQuestPlayerCurrent[MAX_PLAYERS char],
	TeamQuestPlayerTeam[MAX_PLAYERS char],
	TeamQuestPlayerTmp[MAX_PLAYERS char][2];

// --------------------------------------------------
// forwards
// --------------------------------------------------
forward TeamQuestsTick();

// --------------------------------------------------
// publics
// --------------------------------------------------
public TeamQuestsTick()
{
	for(new i = 0;i < sizeof(TeamQuests);i++)
	{
		switch(TeamQuestStatus{i})
		{
			case TEAM_QUEST_STATUS_LINEUP:
			{
				TeamQuestTime[i]--;
				if(TeamQuestTime[i] != 0) continue;
				
				if((TeamQuestTeamCount[i]{0} < TeamQuests[i][tq_team_min]) || (TeamQuestTeamCount[i]{1} < TeamQuests[i][tq_team_min]))
				{
					TeamQuestComplete(i,TEAM_QUEST_REASON_NOT_PLAYERS);
				}
				else
				{
					// message
					format(stmp,sizeof(stmp),"Набор в Team Quest \"%s\" завершён",TeamQuests[i][tq_name]);
					SendClientMessageByQuest(INVALID_QUEST_ID,TEAM_QUESTS_COLOR,stmp);
					SendClientMessageByQuest(TeamQuestQuest[i],TEAM_QUESTS_COLOR,stmp);
					// players
					for(new j = 0;j <= GetPlayersCount();j++)
					{
						if(!IsPlayerConnected(j)) continue;
						if(IsPlayerNPC(j)) continue;
						if(GetPlayerQuest(j) != TeamQuestQuest[i]) continue;
						
						// set pos
						switch(i)
						{
							case TEAM_QUEST_RK: // RK
							{
								new sid = random(MAX_RK_SPAWNS);
								SetPlayerInterior(j,1);
								SetPlayerPos(j,TeamQuest_RK_Spawns[ TeamQuestPlayerTeam{j} ][sid][0],TeamQuest_RK_Spawns[ TeamQuestPlayerTeam{j} ][sid][1],TeamQuest_RK_Spawns[ TeamQuestPlayerTeam{j} ][sid][2]);
							}
							case TEAM_QUEST_MI: // MI
							{
								new sid = random(MAX_MI_SPAWNS);
								SetPlayerInterior(j,0);
								SetPlayerPos(j,TeamQuest_MI_Spawns[ TeamQuestPlayerTeam{j} ][sid][0],TeamQuest_MI_Spawns[ TeamQuestPlayerTeam{j} ][sid][1],TeamQuest_MI_Spawns[ TeamQuestPlayerTeam{j} ][sid][2]);
							}
							case TEAM_QUEST_BL: // BL
							{
								new sid = random(MAX_BL_SPAWNS);
								SetPlayerInterior(j,0);
								SetPlayerPos(j,TeamQuest_BL_Spawns[ TeamQuestPlayerTeam{j} ][sid][0],TeamQuest_BL_Spawns[ TeamQuestPlayerTeam{j} ][sid][1],TeamQuest_BL_Spawns[ TeamQuestPlayerTeam{j} ][sid][2]);
							}
							case TEAM_QUEST_TN: // TN
							{
								new sid = random(MAX_TN_SPAWNS);
								SetPlayerInterior(j,0);
								SetPlayerPos(j,TeamQuest_TN_Spawns[ TeamQuestPlayerTeam{j} ][sid][0],TeamQuest_TN_Spawns[ TeamQuestPlayerTeam{j} ][sid][1],TeamQuest_TN_Spawns[ TeamQuestPlayerTeam{j} ][sid][2]);
							}
							case TEAM_QUEST_MK: // MK
							{
								new sid = random(MAX_MK_SPAWNS);
								SetPlayerInterior(j,0);
								SetPlayerPos(j,TeamQuest_MK_Spawns[ TeamQuestPlayerTeam{j} ][sid][0],TeamQuest_MK_Spawns[ TeamQuestPlayerTeam{j} ][sid][1],TeamQuest_MK_Spawns[ TeamQuestPlayerTeam{j} ][sid][2]);
							}
						}
						// set data
						SetPlayerColor(j,TEAM_QUESTS_COLOR);
						if(i != TEAM_QUEST_TN) SetPlayerVirtualWorld(j,TEAM_QUEST_VIRTUAL_WORLD + i);
						else SetPlayerVirtualWorld(j,0); // Treasure of nation in global world
						TogglePlayerControllable(j,false);						
						ResetPlayerWeapons(j);
						oSetPlayerHealth(j,100.0);
						oSetPlayerArmour(j,100.0);
						SetPlayerSkin(j,TeamQuests[i][tq_team_skin][ TeamQuestPlayerTeam{j} ]);
						SendClientMessage(j,TEAM_QUESTS_COLOR,#TEAM_QUESTS_TACTIC_TIME " секунд на обсуждение тактики");
						switch(TeamQuestPlayerTeam{j})
						{
							case 0:
							{
								for(new w = 0;w < 3;w++) GivePlayerWeapon(j,TeamQuests[i][tq_team1_weapons][w],9999);
								new t = 2 * i;
								for(new m = 0;m < 5;m++)
								{
									if(TeamQuests_Missions[t][m][0] != 0) SendClientMessage(j,TEAM_QUESTS_COLOR,TeamQuests_Missions[t][m]);
								}
								
							}
							case 1:
							{
								for(new w = 0;w < 3;w++) GivePlayerWeapon(j,TeamQuests[i][tq_team2_weapons][w],9999);
								new t = (2 * i) + 1;
								for(new m = 0;m < 5;m++)
								{
									if(TeamQuests_Missions[t][m][0] != 0) SendClientMessage(j,TEAM_QUESTS_COLOR,TeamQuests_Missions[t][m]);
								}
							}
						}
					}
					// tq data
					TeamQuestStatus{i} = TEAM_QUEST_STATUS_TACTIC;
					TeamQuestTime[i] = TEAM_QUESTS_TACTIC_TIME;
				}
			}
			case TEAM_QUEST_STATUS_TACTIC:
			{
				TeamQuestTime[i]--;
				if(TeamQuestTime[i] != 0) continue;
				
				// message
				format(stmp,sizeof(stmp),"TeamQuest \"%s\" начался",TeamQuests[i][tq_name]);
				SendClientMessageToAll(TEAM_QUESTS_COLOR,stmp);
				// players
				for(new j = 0;j < MAX_PLAYERS;j++)
				{
					if(!IsPlayerConnected(j)) continue;
					if(GetPlayerQuest(j) != TeamQuestQuest[i]) continue;

					TogglePlayerControllable(j,true);
					TogglePlayerCheckpointStreamer(j,0);
					for(new m = 0;m < 2;m++) TeamQuestPlayerTmp[j][m] = 0;
					
					switch(i)
					{
						case TEAM_QUEST_RK: // RK
						{
							if(TeamQuestPlayerTeam{j} == 0)
							{
								DisablePlayerCheckpoint(j);
								SetPlayerCheckpoint(j,TEAM_QUEST_RK_TEAM_0_CP_1,5.0);
								TeamQuestPlayerTmp[j][0] = 1;
							}
						}
						case TEAM_QUEST_MI: // MI
						{
							DisablePlayerCheckpoint(j);
							switch(TeamQuestPlayerTeam{j})
							{
								case 0: SetPlayerCheckpoint(j,TEAM_QUEST_MI_TEAM_0_CP_1,5.0);
								case 1: SetPlayerCheckpoint(j,TEAM_QUEST_MI_TEAM_1_CP_1,5.0);
							}							
						}
						case TEAM_QUEST_MK: // MK
						{
							DisablePlayerCheckpoint(j);
							SetPlayerCheckpoint(j,TEAM_QUEST_MK_CP,5.0);
						}
					}
				}
				// tq data
				TeamQuestStatus{i} = TEAM_QUEST_STATUS_ACTION;
				TeamQuestTime[i] = TEAM_QUESTS_ACTION_TIME;
				switch(i)
				{
					case TEAM_QUEST_RK: // RK
					{
						// object
						TeamQuestTmp[i][0] = CreateObject(TEAM_QUEST_RK_OBJECT_1_MODEL,TEAM_QUEST_RK_OBJECT_1);
						// vehicles
						TeamQuestTmp[i][1] = CreateVehicle(TEAM_QUEST_RK_VEHICLE_1_MODEL,TEAM_QUEST_RK_VEHICLE_1,999999);
						TeamQuestTmp[i][2] = CreateVehicle(TEAM_QUEST_RK_VEHICLE_2_MODEL,TEAM_QUEST_RK_VEHICLE_2,999999);
						TeamQuestTmp[i][3] = CreateVehicle(TEAM_QUEST_RK_VEHICLE_3_MODEL,TEAM_QUEST_RK_VEHICLE_3,999999);
						TeamQuestTmp[i][4] = CreateVehicle(TEAM_QUEST_RK_VEHICLE_4_MODEL,TEAM_QUEST_RK_VEHICLE_4,999999);
						for(new o = 1;o <= 4;o++) SetVehicleVirtualWorld(TeamQuestTmp[i][o],TEAM_QUEST_VIRTUAL_WORLD + i);
						// pickups
						TeamQuestTmp[i][5] = CreatePickup(TEAM_QUEST_RK_PICKUP_1_MODEL,TEAM_QUEST_RK_PICKUP_1,TEAM_QUEST_VIRTUAL_WORLD + i);
						TeamQuestTmp[i][6] = CreatePickup(TEAM_QUEST_RK_PICKUP_2_MODEL,TEAM_QUEST_RK_PICKUP_2,TEAM_QUEST_VIRTUAL_WORLD + i);
						TeamQuestTmp[i][7] = 0; // money robbed
						TeamQuestTmp[i][8] = 500_000 + (random(500_000) * 2); // money in casino
						TeamQuestTmp[i][9] = 0; // door open time
					}
					case TEAM_QUEST_MI: // MI
					{
						TeamQuestTmp[i][0] = CreatePickup(TEAM_QUEST_MI_PICKUP_1_MODEL,TEAM_QUEST_MI_PICKUP_1,TEAM_QUEST_VIRTUAL_WORLD + i);
						TeamQuestTmp[i][1] = 0;
						TeamQuestTmp[i][2] = 0;
					}
					case TEAM_QUEST_BL: // BL
					{
						TeamQuestTmp[i][0] = CreateVehicle(TEAM_QUEST_BL_VEHICLE_1_MODEL,TEAM_QUEST_BL_VEHICLE_1,999999);
						TeamQuestTmp[i][1] = CreateVehicle(TEAM_QUEST_BL_VEHICLE_2_MODEL,TEAM_QUEST_BL_VEHICLE_2,999999);
						TeamQuestTmp[i][2] = CreateVehicle(TEAM_QUEST_BL_VEHICLE_3_MODEL,TEAM_QUEST_BL_VEHICLE_3,999999);
						TeamQuestTmp[i][3] = CreateVehicle(TEAM_QUEST_BL_VEHICLE_4_MODEL,TEAM_QUEST_BL_VEHICLE_4,999999);
						TeamQuestTmp[i][4] = CreateVehicle(TEAM_QUEST_BL_VEHICLE_5_MODEL,TEAM_QUEST_BL_VEHICLE_5,999999);
						TeamQuestTmp[i][5] = CreateVehicle(TEAM_QUEST_BL_VEHICLE_6_MODEL,TEAM_QUEST_BL_VEHICLE_6,999999);
						TeamQuestTmp[i][6] = CreateVehicle(TEAM_QUEST_BL_VEHICLE_7_MODEL,TEAM_QUEST_BL_VEHICLE_7,999999);
						TeamQuestTmp[i][7] = CreateVehicle(TEAM_QUEST_BL_VEHICLE_8_MODEL,TEAM_QUEST_BL_VEHICLE_8,999999);
						for(new o = 0;o <= 7;o++) SetVehicleVirtualWorld(TeamQuestTmp[i][o],TEAM_QUEST_VIRTUAL_WORLD + i);
						TeamQuestTmp[i][8] = 0;
					}
					case TEAM_QUEST_TN: // TN
					{
						TeamQuestTime[i] = -1;
						TeamQuestTmp[i][0] = 0; // team 0 count of finded helps
						TeamQuestTmp[i][1] = 0; // team 1 count of finded helps
						
						 // team 0 current pickup help
						TeamQuestTmp[i][2] = random(sizeof(TeamQuest_TN_Pickups));
						ftmp[0] = TeamQuest_TN_Pickups[ TeamQuestTmp[i][2] ][0];
						ftmp[1] = TeamQuest_TN_Pickups[ TeamQuestTmp[i][2] ][1];
						ftmp[2] = TeamQuest_TN_Pickups[ TeamQuestTmp[i][2] ][2];
						TeamQuestTmp[i][4] = CreatePickup(TeamQuest_TN_Box[random(sizeof(TeamQuest_TN_Box))],23,ftmp[0],ftmp[1],ftmp[2],0);
						format(stmp,sizeof(stmp),"Первая подсказка находится в зоне '%s'",GetXYZZoneName(ftmp[0],ftmp[1],ftmp[2]));
						for(new j = 0;j <= GetPlayersCount();j++)
						{
							if(!IsPlayerConnected(j)) continue;
							if(IsPlayerNPC(j)) continue;
							if(GetPlayerQuest(j) != TeamQuestQuest[i]) continue;
							if(TeamQuestPlayerTeam{j} != 0) continue;
							
							SendClientMessage(j,TEAM_QUESTS_COLOR,stmp);
						}
						 // team 1 current pickup help
						TeamQuestTmp[i][3] = random(sizeof(TeamQuest_TN_Pickups));
						ftmp[0] = TeamQuest_TN_Pickups[ TeamQuestTmp[i][3] ][0];
						ftmp[1] = TeamQuest_TN_Pickups[ TeamQuestTmp[i][3] ][1];
						ftmp[2] = TeamQuest_TN_Pickups[ TeamQuestTmp[i][3] ][2];
						TeamQuestTmp[i][5] = CreatePickup(TeamQuest_TN_Box[random(sizeof(TeamQuest_TN_Box))],23,ftmp[0],ftmp[1],ftmp[2],0);
						format(stmp,sizeof(stmp),"Первая подсказка находится в зоне '%s'",GetXYZZoneName(ftmp[0],ftmp[1],ftmp[2]));
						for(new j = 0;j <= GetPlayersCount();j++)
						{
							if(!IsPlayerConnected(j)) continue;
							if(IsPlayerNPC(j)) continue;
							if(GetPlayerQuest(j) != TeamQuestQuest[i]) continue;
							if(TeamQuestPlayerTeam{j} != 1) continue;
							
							SendClientMessage(j,TEAM_QUESTS_COLOR,stmp);
						}
						
						TeamQuestTmp[i][6] = -1; // team 0 current question
						TeamQuestTmp[i][7] = -1; // team 1 current question
						
						TeamQuestTmp[i][8] = 0; // team 0 bonus
						TeamQuestTmp[i][9] = 0; // team 1 bonus
					}
					case TEAM_QUEST_MK: // MK
					{
						TeamQuestTmp[i][0] = 0; // team 0 count on point
						TeamQuestTmp[i][1] = 0; // team 1 count on point
						TeamQuestTmp[i][2] = 0; // team 0 time lost
						TeamQuestTmp[i][3] = 0; // team 1 time lost
					}
				}
			}
			case TEAM_QUEST_STATUS_ACTION:
			{
				if(TeamQuestTime[i] > 0) // need time check
				{
					TeamQuestTime[i]--;
					// complete tq
					if(TeamQuestTime[i] == 0) TeamQuestComplete(i,TEAM_QUEST_REASON_TIME_END);
					else if(TeamQuestTime[i] == 5) SendClientMessageByQuest(TeamQuestQuest[i],TEAM_QUESTS_COLOR,"Осталось 5 секунд");
					else if(TeamQuestTime[i] == 10) SendClientMessageByQuest(TeamQuestQuest[i],TEAM_QUESTS_COLOR,"Осталось 10 секунд");
					else if(TeamQuestTime[i] == 15) SendClientMessageByQuest(TeamQuestQuest[i],TEAM_QUESTS_COLOR,"Осталось 15 секунд");
					else if(TeamQuestTime[i] == 30) SendClientMessageByQuest(TeamQuestQuest[i],TEAM_QUESTS_COLOR,"Осталось 30 секунд");
					else if(TeamQuestTime[i] == 60) SendClientMessageByQuest(TeamQuestQuest[i],TEAM_QUESTS_COLOR,"Осталась 1 минута");
					else if(TeamQuestTime[i] == 120) SendClientMessageByQuest(TeamQuestQuest[i],TEAM_QUESTS_COLOR,"Осталось 2 минуты");
					else if(TeamQuestTime[i] == 300) SendClientMessageByQuest(TeamQuestQuest[i],TEAM_QUESTS_COLOR,"Осталось 5 минут");
				}
				switch(i)
				{
					case TEAM_QUEST_RK: // Rob of the Kaligula
					{
						// check players
						for(new j = 0;j <= GetPlayersCount();j++)
						{
							if(!IsPlayerConnected(j)) continue;
							if(IsPlayerNPC(j)) continue;
							if(GetPlayerQuest(j) != TeamQuestQuest[i]) continue;

							switch(TeamQuestPlayerTeam{j})
							{
								case 0: // robber
								{
									if(TeamQuestTmp[i][9] < 10)
									{
										if(IsPlayerInRangeOfPoint(j,5.0,TEAM_QUEST_RK_TEAM_0_CP_1))
										{
											TeamQuestTmp[i][9]++;
										}
										if(TeamQuestPlayerTmp[j][0] == 0)
										{
											TeamQuestPlayerTmp[j][0] = 1;
											SetPlayerCheckpoint(j,TEAM_QUEST_RK_TEAM_0_CP_1,5.0);
										}
									}
									else if(TeamQuestTmp[i][9] == 10)
									{
										TeamQuestTmp[i][9]++;
										MoveObject(TeamQuestTmp[i][0],TEAM_QUEST_RK_OBJECT_1_MOVE,0.1);
										DisablePlayerCheckpoint(j);
									}
									else
									{
										if(IsPlayerInRangeOfPoint(j,5.0,TEAM_QUEST_RK_TEAM_0_CP_2))
										{
											if(TeamQuestTmp[i][7] < TeamQuestTmp[i][8])
											{
												TeamQuestTmp[i][7] += 10_000;
												if(TeamQuestTmp[i][7] > TeamQuestTmp[i][8]) TeamQuestTmp[i][7] = TeamQuestTmp[i][8];
												format(stmp,sizeof(stmp),"%d$ / %d$",TeamQuestTmp[i][7],TeamQuestTmp[i][8]);
												GameTextForPlayer(j,stmp,1000,5);
											}
											else GameTextForPlayer(j,"All money is taken",1000,5);
										}
										else if(IsPlayerInRangeOfPoint(j,5.0,TEAM_QUEST_RK_TEAM_0_CP_3))
										{
											new count;
											for(new o = 0;o <= GetPlayersCount();o++)
											{
												if(!IsPlayerConnected(o)) continue;
												if(IsPlayerNPC(o)) continue;
												if(GetPlayerQuest(o) != TeamQuestQuest[i]) continue;
												if(TeamQuestPlayerTeam{o} != 0) continue;
												if(!IsPlayerInRangeOfPoint(o,5.0,TEAM_QUEST_RK_TEAM_0_CP_3)) continue;
												
												count++;
											}
											if(count == TeamQuestTeamCount[i]{0})
											{
												TeamQuestComplete(i,TEAM_QUEST_REASON_WIN_TEAM_1);
												break;
											}
										}
										if((TeamQuestPlayerTmp[j][0] != 2) && IsPlayerInZone(j,0)) // 0 - Kaligula
										{
											TeamQuestPlayerTmp[j][0] = 2;
											DisablePlayerCheckpoint(j);
											SetPlayerCheckpoint(j,TEAM_QUEST_RK_TEAM_0_CP_2,5.0);
										}
										else if((TeamQuestPlayerTmp[j][0] != 3) && IsPlayerInZone(j,1)) // 1 - Four dragons
										{
											TeamQuestPlayerTmp[j][0] = 3;
											DisablePlayerCheckpoint(j);
											SetPlayerCheckpoint(j,TEAM_QUEST_RK_TEAM_0_CP_3,5.0);
										}
									}
								}
							}
						}
					}
					case TEAM_QUEST_MI: // Mission Imposible
					{
						// check players
						for(new j = 0;j <= GetPlayersCount();j++)
						{
							if(!IsPlayerConnected(j)) continue;
							if(IsPlayerNPC(j)) continue;
							if(GetPlayerQuest(j) != TeamQuestQuest[i]) continue;

							switch(TeamQuestPlayerTeam{j})
							{
								case 0: // attack
								{
									if(IsPlayerInRangeOfPoint(j,5.0,TEAM_QUEST_MI_TEAM_0_CP_1) && (TeamQuestTmp[i][1] == 0))
									{
										DestroyPickup(TeamQuestTmp[i][0]);
										for(new o = 0;o <= GetPlayersCount();o++)
										{
											if(!IsPlayerConnected(o)) continue;
											if(IsPlayerNPC(o)) continue;
											if(GetPlayerQuest(o) != TeamQuestQuest[i]) continue;
											if(TeamQuestPlayerTeam{o} != 0) continue;
											
											DisablePlayerCheckpoint(o);
											SetPlayerCheckpoint(o,TEAM_QUEST_MI_TEAM_0_CP_2,5.0);
										}
										oSetPlayerSpecialAction(j,SPECIAL_ACTION_USEJETPACK);
										TeamQuestTmp[i][1] = 1;
									}
									else if(IsPlayerInRangeOfPoint(j,5.0,TEAM_QUEST_MI_TEAM_0_CP_2) && (TeamQuestTmp[i][1] == 1))
									{
										if(GetPlayerSpecialAction(j) == SPECIAL_ACTION_USEJETPACK)
										{
											new count;
											for(new o = 0;o <= GetPlayersCount();o++)
											{
												if(!IsPlayerConnected(o)) continue;
												if(IsPlayerNPC(o)) continue;
												if(GetPlayerQuest(o) != TeamQuestQuest[i]) continue;
												if(TeamQuestPlayerTeam{o} != 0) continue;
												if(!IsPlayerInRangeOfPoint(o,5.0,TEAM_QUEST_MI_TEAM_0_CP_2)) continue;
												
												count++;
											}
											if(count == TeamQuestTeamCount[i]{0})
											{
												TeamQuestComplete(i,TEAM_QUEST_REASON_WIN_TEAM_1);
												break;
											}
										}
									}
									else if((TeamQuestTmp[i][1] == 1) && (TeamQuestPlayerTmp[j][0] != 1))
									{
										TeamQuestPlayerTmp[j][0] = 1;
										DisablePlayerCheckpoint(j);
										SetPlayerCheckpoint(j,TEAM_QUEST_MI_TEAM_0_CP_2,5.0);
									}
								}
								case 1: // def
								{
									new keys[3];
									GetPlayerKeys(j,keys[0],keys[1],keys[2]);
									if(IsPlayerInRangeOfPoint(j,5.0,TEAM_QUEST_MI_TEAM_1_CP_1) && (keys[2] & KEY_WALK))
									{
										if((TeamQuestTmp[i][1] == 1) && (TeamQuestTmp[i][2] == 0))
										{
											for(new z = -30;z < 0;z++) CreateExplosion(TEAM_QUEST_MI_EXPLOSION_XY,float(z),1,10.0);
											SendClientMessage(j,TEAM_QUESTS_COLOR,"Шахта взорвана");	
											TeamQuestTmp[i][2] = 1;
										}
										else
										{
											SendClientMessage(j,TEAM_QUESTS_COLOR,"JetPack на месте, взрыв шахты невозможен");
										}
									}
								}
							}
						}
					}
					case TEAM_QUEST_BL: // Baggage or life
					{
						if(IsTrailerAttachedToVehicle(TeamQuestTmp[i][0]) && (GetVehicleTrailer(TeamQuestTmp[i][0]) == TeamQuestTmp[i][1]))
						{
							if(TeamQuestTmp[i][8] == 0) TeamQuestTmp[i][8] = 1;
						}
						else if(TeamQuestTmp[i][8] == 1)
						{
							TeamQuestComplete(i,TEAM_QUEST_REASON_WIN_TEAM_2);
						}
						// check players
						for(new j = 0;j <= GetPlayersCount();j++)
						{
							if(!IsPlayerConnected(j)) continue;
							if(IsPlayerNPC(j)) continue;
							if(GetPlayerQuest(j) != TeamQuestQuest[i]) continue;

							switch(TeamQuestPlayerTeam{j})
							{
								case 0: // trucker
								{
									if(IsPlayerInVehicle(j,TeamQuestTmp[i][0]))
									{
										if(IsPlayerInRangeOfPoint(j,5.0,TEAM_QUEST_BL_TEAM_1_CP))
										{
											TeamQuestComplete(i,TEAM_QUEST_REASON_WIN_TEAM_1);
											break;
										}
										else if(TeamQuestPlayerTmp[j][0] != 2)
										{
											DisablePlayerCheckpoint(j);
											SetPlayerCheckpoint(j,TEAM_QUEST_BL_TEAM_1_CP,5.0);
											TeamQuestPlayerTmp[j][0] = 2;
										}
									}
									else
									{
										new Float:pos[3];
										GetVehiclePos(TeamQuestTmp[i][0],pos[0],pos[1],pos[2]);
										DisablePlayerCheckpoint(j);
										SetPlayerCheckpoint(j,pos[0],pos[1],pos[2],0.0);
										TeamQuestPlayerTmp[j][0] = 1;
									}
								}
								case 1: // attack
								{
									new Float:pos[3];
									GetVehiclePos(TeamQuestTmp[i][1],pos[0],pos[1],pos[2]);
									DisablePlayerCheckpoint(j);
									SetPlayerCheckpoint(j,pos[0],pos[1],pos[2],0.0);
								}
							}
						}
					}
					case TEAM_QUEST_TN: // Treasure of nation
					{
						// check players
						for(new j = 0;j <= GetPlayersCount();j++)
						{
							if(!IsPlayerConnected(j)) continue;
							if(IsPlayerNPC(j)) continue;
							if(GetPlayerQuest(j) != TeamQuestQuest[i]) continue;

							switch(TeamQuestPlayerTeam{j})
							{
								case 0: // team 1
								{
									if(IsPlayerInRangeOfPoint(j,5.0,TeamQuest_TN_Pickups[ TeamQuestTmp[i][2] ][0],TeamQuest_TN_Pickups[ TeamQuestTmp[i][2] ][1],TeamQuest_TN_Pickups[ TeamQuestTmp[i][2] ][2]))
									{
										if(!IsPlayerInAnyVehicle(j)) TN_GiveQuestionForTeam(0);
									}
									else if(IsPlayerInRangeOfPoint(j,200.0,TeamQuest_TN_Pickups[ TeamQuestTmp[i][2] ][0],TeamQuest_TN_Pickups[ TeamQuestTmp[i][2] ][1],TeamQuest_TN_Pickups[ TeamQuestTmp[i][2] ][2]))
									{
										if(TeamQuestPlayerTmp[j][0] == 0)
										{
											TeamQuestPlayerTmp[j][0] = 1;
											DisablePlayerCheckpoint(j);
											SetPlayerCheckpoint(j,TeamQuest_TN_Pickups[ TeamQuestTmp[i][2] ][0],TeamQuest_TN_Pickups[ TeamQuestTmp[i][2] ][1],TeamQuest_TN_Pickups[ TeamQuestTmp[i][2] ][2],0.0);
										}
									}
									else if(TeamQuestPlayerTmp[j][0] == 1)
									{
										TeamQuestPlayerTmp[j][0] = 0;
										DisablePlayerCheckpoint(j);
									}
								}
								case 1: // team 2
								{
									if(IsPlayerInRangeOfPoint(j,5.0,TeamQuest_TN_Pickups[ TeamQuestTmp[i][3] ][0],TeamQuest_TN_Pickups[ TeamQuestTmp[i][3] ][1],TeamQuest_TN_Pickups[ TeamQuestTmp[i][3] ][2]))
									{
										if(!IsPlayerInAnyVehicle(j)) TN_GiveQuestionForTeam(1);
									}
									else if(IsPlayerInRangeOfPoint(j,200.0,TeamQuest_TN_Pickups[ TeamQuestTmp[i][3] ][0],TeamQuest_TN_Pickups[ TeamQuestTmp[i][3] ][1],TeamQuest_TN_Pickups[ TeamQuestTmp[i][3] ][2]))
									{
										if(TeamQuestPlayerTmp[j][0] == 0)
										{
											TeamQuestPlayerTmp[j][0] = 1;
											DisablePlayerCheckpoint(j);
											SetPlayerCheckpoint(j,TeamQuest_TN_Pickups[ TeamQuestTmp[i][3] ][0],TeamQuest_TN_Pickups[ TeamQuestTmp[i][3] ][1],TeamQuest_TN_Pickups[ TeamQuestTmp[i][3] ][2],0.0);
										}
									}
									else if(TeamQuestPlayerTmp[j][0] == 1)
									{
										TeamQuestPlayerTmp[j][0] = 0;
										DisablePlayerCheckpoint(j);
									}
								}
							}
						}
					}
					case TEAM_QUEST_MK: // Mountain King
					{
						// check players
						for(new j = 0;j <= GetPlayersCount();j++)
						{
							if(!IsPlayerConnected(j)) continue;
							if(IsPlayerNPC(j)) continue;
							if(GetPlayerQuest(j) != TeamQuestQuest[i]) continue;

							switch(TeamQuestPlayerTeam{j})
							{
								case 0: // team 1
								{
									if(IsPlayerInRangeOfPoint(j,5.0,TEAM_QUEST_MK_CP))
									{
										if(TeamQuestPlayerTmp[j][0] == 0)
										{
											TeamQuestPlayerTmp[j][0] = 1;
											TeamQuestTmp[i][0]++;
										}
									}
									else if(TeamQuestPlayerTmp[j][0] == 1)
									{
										TeamQuestPlayerTmp[j][0] = 0;
										TeamQuestTmp[i][0]--;
									}
								}
								case 1: // team 2
								{
									if(IsPlayerInRangeOfPoint(j,5.0,TEAM_QUEST_MK_CP))
									{
										if(TeamQuestPlayerTmp[j][0] == 0)
										{
											TeamQuestPlayerTmp[j][0] = 1;
											TeamQuestTmp[i][1]++;
										}
									}
									else if(TeamQuestPlayerTmp[j][0] == 1)
									{
										TeamQuestPlayerTmp[j][0] = 0;
										TeamQuestTmp[i][1]--;
									}
								}
							}
						}
						// check time
						TeamQuestTmp[i][2] += TeamQuestTmp[i][0];
						TeamQuestTmp[i][3] += TeamQuestTmp[i][1];
						if((TeamQuestTmp[i][2] >= MOUNTAIN_KING_DEF_TIME) && (TeamQuestTmp[i][3] >= MOUNTAIN_KING_DEF_TIME)) TeamQuestComplete(i,TEAM_QUEST_REASON_TIME_END);
						else if(TeamQuestTmp[i][2] >= MOUNTAIN_KING_DEF_TIME) TeamQuestComplete(i,TEAM_QUEST_REASON_WIN_TEAM_1);
						else if(TeamQuestTmp[i][3] >= MOUNTAIN_KING_DEF_TIME) TeamQuestComplete(i,TEAM_QUEST_REASON_WIN_TEAM_2);						
					}
				}
			}
		}
	}
}

// --------------------------------------------------
// stocks
// --------------------------------------------------
stock TeamQuestComplete(tqid,reason)
{
	switch(reason)
	{
		case TEAM_QUEST_REASON_NOT_PLAYERS:
		{
			// players null
			for(new playerid = 0;playerid <= GetPlayersCount();playerid++)
			{
				if(!IsPlayerConnected(playerid)) continue;
				if(IsPlayerNPC(playerid)) continue;
				if(GetPlayerQuest(playerid) != TeamQuestQuest[tqid]) continue;
			
				TeamQuestPlayerCurrent{playerid} = INVALID_TEAM_QUEST_ID;
				SetPlayerQuest(playerid,INVALID_QUEST_ID);
				if(TeamQuestStatus{tqid} != TEAM_QUEST_STATUS_LINEUP)
				{
					DisablePlayerCheckpoint(playerid);
					TogglePlayerCheckpointStreamer(playerid,1);
					SetPlayerPos(playerid,0.0,0.0,0.0); // vehicle spawn fix
					SpawnPlayer(playerid);
				}
			}
			// info
			format(stmp,sizeof(stmp),"Team Quest \"%s\" сорвался по причине нехватки игроков",TeamQuests[tqid][tq_name]);
			SendClientMessageByQuest(INVALID_QUEST_ID,TEAM_QUESTS_COLOR,stmp);
			// tq data
			TeamQuestStatus{tqid} = TEAM_QUEST_STATUS_OFF;
			TeamQuestTime[tqid] = 0;
			TeamQuestTeamCount[tqid]{0} = 0;
			TeamQuestTeamCount[tqid]{1} = 0;
			return 1;
		}
		case TEAM_QUEST_REASON_WIN_TEAM_1:
		{
			itmp[0] = 0; // winner
			if(tqid == TEAM_QUEST_RK) format(stmp,sizeof(stmp),"Вы украли %d$ из %d$.Ваша доля %d$",TeamQuestTmp[tqid][7],TeamQuestTmp[tqid][8],(itmp[1] = (TeamQuestTmp[tqid][7] / TeamQuestTeamCount[tqid]{0})));
		}
		case TEAM_QUEST_REASON_WIN_TEAM_2:
		{
			itmp[0] = 1; // winner
		}
		case TEAM_QUEST_REASON_TIME_END:
		{
			if(tqid == TEAM_QUEST_RK) itmp[0] = 1; // winner
			else if(tqid == TEAM_QUEST_MI) itmp[0] = 1; // winner
			else itmp[0] = -1; // winner
		}
	}
	// price
	for(new playerid = 0;playerid <= GetPlayersCount();playerid++)
	{
		if(!IsPlayerConnected(playerid)) continue;
		if(IsPlayerNPC(playerid)) continue;
		if(GetPlayerQuest(playerid) != TeamQuestQuest[tqid]) continue;
		// price
		if((tqid == TEAM_QUEST_RK) && (itmp[0] == 0) && (TeamQuestPlayerTeam{playerid} == itmp[0]))
		{
			SendClientMessage(playerid,TEAM_QUESTS_COLOR,stmp);
			oGivePlayerMoney(playerid,itmp[1]);
			GivePlayerXP(playerid,TeamQuests[tqid][tq_xp]);
		}
		else if(TeamQuestPlayerTeam{playerid} == itmp[0])
		{
			oGivePlayerMoney(playerid,TeamQuests[tqid][tq_money]);
			GivePlayerXP(playerid,TeamQuests[tqid][tq_xp]);
		}
		TeamQuestPlayerCurrent{playerid} = INVALID_TEAM_QUEST_ID;
		SetPlayerQuest(playerid,INVALID_QUEST_ID);
		DisablePlayerCheckpoint(playerid);
		TogglePlayerCheckpointStreamer(playerid,1);
		SetPlayerPos(playerid,0.0,0.0,0.0); // vehicle spawn fix		
		SpawnPlayer(playerid);
	}
	// info
	if(itmp[0] == -1) format(stmp,sizeof(stmp),"Team Quest \"%s\" завершён, победителей нет",TeamQuests[tqid][tq_name]);
	else format(stmp,sizeof(stmp),"Team Quest \"%s\" завершён, победила команда %s",TeamQuests[tqid][tq_name],TeamQuests_Teams[tqid][ itmp[0] ]);
	SendClientMessageByQuest(INVALID_QUEST_ID,TEAM_QUESTS_COLOR,stmp);
	// tq data
	TeamQuestStatus{tqid} = TEAM_QUEST_STATUS_OFF;
	TeamQuestTime[tqid] = 0;
	TeamQuestTeamCount[tqid]{0} = 0;
	TeamQuestTeamCount[tqid]{1} = 0;
	// tmp destroy
	switch(tqid)
	{
		case TEAM_QUEST_RK: // RK
		{
			// object
			DestroyObject(TeamQuestTmp[tqid][0]);
			// vehicles
			DestroyVehicle(TeamQuestTmp[tqid][1]);
			DestroyVehicle(TeamQuestTmp[tqid][2]);
			DestroyVehicle(TeamQuestTmp[tqid][3]);
			DestroyVehicle(TeamQuestTmp[tqid][4]);
			// pickups
			DestroyPickup(TeamQuestTmp[tqid][5]);
			DestroyPickup(TeamQuestTmp[tqid][6]);
		}
		case TEAM_QUEST_MI: // MI
		{
			if(TeamQuestTmp[tqid][1] == 0) DestroyPickup(TeamQuestTmp[tqid][0]);
		}
		case TEAM_QUEST_BL: // BL
		{
			DestroyVehicle(TeamQuestTmp[tqid][0]);		
			DestroyVehicle(TeamQuestTmp[tqid][1]);
			DestroyVehicle(TeamQuestTmp[tqid][2]);
			DestroyVehicle(TeamQuestTmp[tqid][3]);
			DestroyVehicle(TeamQuestTmp[tqid][4]);
			DestroyVehicle(TeamQuestTmp[tqid][5]);
			DestroyVehicle(TeamQuestTmp[tqid][6]);
			DestroyVehicle(TeamQuestTmp[tqid][7]);
		}
		case TEAM_QUEST_TN: // TN
		{
			DestroyPickup(TeamQuestTmp[tqid][4]);
			DestroyPickup(TeamQuestTmp[tqid][5]);
		}
	}
	return 1;
}

stock PlayerJoinTeamQuest(playerid,tqid)
{
	switch(TeamQuestStatus{tqid})
	{
		case TEAM_QUEST_STATUS_ACTION,TEAM_QUEST_STATUS_TACTIC: return SendClientMessage(playerid,SYSTEM_COLOR,"Соревнование уже началось");
		case TEAM_QUEST_STATUS_OFF:
		{
			TeamQuestStatus{tqid} = TEAM_QUEST_STATUS_LINEUP;
			TeamQuestTime[tqid] = TEAM_QUESTS_LINEUP_TIME;
			TeamQuestTeamCount[tqid]{0} = 0;
			TeamQuestTeamCount[tqid]{1} = 0;
			format(stmp,sizeof(stmp),"Начат набор в Team Quest \"%s\", осталось " #TEAM_QUESTS_LINEUP_TIME " секунд",TeamQuests[tqid][tq_name]);
			SendClientMessageByQuest(INVALID_QUEST_ID,TEAM_QUESTS_COLOR,stmp);
		}
	}
	TeamQuestPlayerTeam{playerid} = 0;
	TeamQuestPlayerCurrent{playerid} = tqid;
	SetPlayerQuest(playerid,TeamQuestQuest[tqid]);
	ShowPlayerDialog(playerid,DIALOG_TEAM_QUEST_TEAM,DIALOG_STYLE_MSGBOX,"Team Quests","Вы вступили в очередь на Team Quest...\nВыбeрите команду...",TeamQuests_Teams[tqid][0],TeamQuests_Teams[tqid][1]);
	return 1;
}

stock PlayerLeaveTeamQuest(playerid,tqid,reason)
{
	new team = TeamQuestPlayerTeam{playerid};
	TeamQuestTeamCount[tqid]{team}--;
	TeamQuestPlayerCurrent{playerid} = INVALID_TEAM_QUEST_ID;
	SetPlayerQuest(playerid,INVALID_QUEST_ID);
	DisablePlayerCheckpoint(playerid);
	TogglePlayerCheckpointStreamer(playerid,1);
	SendClientMessage(playerid,TEAM_QUESTS_COLOR,"Вы покинули Team Quest");
	switch(reason)
	{
		case TEAM_QUEST_LEAVE_REASON_DEATH:
		{
			if((team == 0) && (TeamQuestTeamCount[tqid]{team} == 0)) // robers
			{
				if((tqid == TEAM_QUEST_RK) || (tqid == TEAM_QUEST_MI))
					return TeamQuestComplete(tqid,TEAM_QUEST_REASON_WIN_TEAM_2);
			}
		}
		case TEAM_QUEST_LEAVE_REASON_OFF,TEAM_QUEST_LEAVE_REASON_CMD:
		{
			if(TeamQuestTeamCount[tqid]{team} == 0) 
			{
				if(TeamQuestStatus{tqid} != TEAM_QUEST_STATUS_LINEUP)
					return TeamQuestComplete(tqid,TEAM_QUEST_REASON_NOT_PLAYERS);
			}
		}
	}
	if(TeamQuestStatus{tqid} != TEAM_QUEST_STATUS_LINEUP)
	{
		SetPlayerPos(playerid,0.0,0.0,0.0); // vehicle spawn fix
		SpawnPlayer(playerid);
	}
	return 1;
}

stock TN_GiveQuestionForTeam(team)
{
	static 
		line[128],
		tmp;
	if(TeamQuestTmp[TEAM_QUEST_TN][6 + team] == -1) // havent question
	{
		// open questions file
		new File:datafile = fopen(MODE_DIR TEAM_QUESTS_DIR TN_QUESTIONS_FILE,io_read);
		if(datafile)
		{
			// get count of questions
			fread(datafile,line);
			tmp = strval(line);
			// randomize
			tmp = random(tmp) + 1;
			// find line
			while(tmp-- > 0)
			{
				fread(datafile,line);
			}
			// find parser
			tmp = strfind(line,"|");
			// parse
			format(TeamQuest_TN_Answer[team],MAX_TN_QUESTION,line[tmp + 1]);
			strdel(line,tmp,strlen(line));
			// send msg
			for(new playerid = 0;playerid <= GetPlayersCount();playerid++)
			{
				if(!IsPlayerConnected(playerid)) continue;
				if(IsPlayerNPC(playerid)) continue;
				if(GetPlayerQuest(playerid) != TeamQuestQuest[TEAM_QUEST_TN]) continue;
				if(TeamQuestPlayerTeam{playerid} != team) continue;

				SendClientMessage(playerid,TEAM_QUESTS_COLOR,"Чтобы открыть ящик с положением следующей подсказки нужно ответить на вопрос(& <ответ>):");
				SendClientMessage(playerid,TEAM_QUESTS_COLOR,line);
			}
			// stat
			TeamQuestTmp[TEAM_QUEST_TN][6 + team] = 1;
		}
		else print("[ERROR] Treasure of nation questions file not find");
	}
	else if(TeamQuestTmp[TEAM_QUEST_TN][6 + team] == -2) // questions end
	{
		switch(team)
		{
			case 0: TeamQuestComplete(TEAM_QUEST_TN,TEAM_QUEST_REASON_WIN_TEAM_1);
			case 1: TeamQuestComplete(TEAM_QUEST_TN,TEAM_QUEST_REASON_WIN_TEAM_2);
		}
	}
}

// --------------------------------------------------
// Obligatory functions
// --------------------------------------------------
TeamQuest_OnGameModeInit()
{
	for(new i = 0;i < MAX_TEAM_QUESTS;i++)	TeamQuestQuest[i] = RegisterQuest();
	SetTimer("TeamQuestsTick",TEAM_QUESTS_TICK_TIME,1);
	// data
#if defined PRINT_STATS_DATA
	printf("Team quests: %d",MAX_TEAM_QUESTS);
#endif	
	print("Team Quests system loaded.");
}

TeamQuest_OnPlayerConnect(playerid)
{
	TeamQuestPlayerCurrent{playerid} = INVALID_TEAM_QUEST_ID;
}

TeamQuest_OnPlayerDisconnect(playerid,reason)
{
	#pragma unused reason
	static tqid;
	if(GetPlayerQuest(playerid) != INVALID_QUEST_ID)
	{
		tqid = TeamQuestPlayerCurrent{playerid};
		if((tqid != INVALID_TEAM_QUEST_ID) && (GetPlayerQuest(playerid) == TeamQuestQuest[tqid]))
			return PlayerLeaveTeamQuest(playerid,tqid,TEAM_QUEST_LEAVE_REASON_OFF);
	}
	return 1;
}

TeamQuest_OnPlayerDeath(playerid,killerid,reason)
{
	#pragma unused killerid,reason
	static tqid;
	if(GetPlayerQuest(playerid) != INVALID_QUEST_ID)
	{
		tqid = TeamQuestPlayerCurrent{playerid};
		if((tqid != INVALID_TEAM_QUEST_ID) && (GetPlayerQuest(playerid) == TeamQuestQuest[tqid])) 
		{
			if(
				TeamQuestStatus{tqid} != TEAM_QUEST_STATUS_ACTION
				|| tqid == TEAM_QUEST_TN
				|| tqid == TEAM_QUEST_MK
			) return 1;

			return PlayerLeaveTeamQuest(playerid,tqid,TEAM_QUEST_LEAVE_REASON_DEATH);
		}
	}
	return 1;
}

TeamQuest_OnPlayerSpawn(playerid)
{
	static tqid;
	if(GetPlayerQuest(playerid) != INVALID_QUEST_ID)
	{
		tqid = TeamQuestPlayerCurrent{playerid};
		if((tqid != INVALID_TEAM_QUEST_ID) && (GetPlayerQuest(playerid) == TeamQuestQuest[tqid]))
		{
			if(TeamQuestStatus{tqid} != TEAM_QUEST_STATUS_ACTION) return;
			new team = TeamQuestPlayerTeam{playerid};
			// set pos
			switch(tqid)
			{
				case TEAM_QUEST_TN: // TN
				{
					new sid = random(MAX_TN_SPAWNS);
					SetPlayerInterior(playerid,0);
					SetPlayerPos(playerid,TeamQuest_TN_Spawns[team][sid][0],TeamQuest_TN_Spawns[team][sid][1],TeamQuest_TN_Spawns[team][sid][2]);
				}
				case TEAM_QUEST_MK: // MK
				{
					new sid = random(MAX_MK_SPAWNS);
					SetPlayerInterior(playerid,0);
					SetPlayerPos(playerid,TeamQuest_MK_Spawns[team][sid][0],TeamQuest_MK_Spawns[team][sid][1],TeamQuest_MK_Spawns[team][sid][2]);
				}
			}
			// set data
			SetPlayerColor(playerid,TEAM_QUESTS_COLOR);
			if(tqid != TEAM_QUEST_TN) SetPlayerVirtualWorld(playerid,TEAM_QUEST_VIRTUAL_WORLD + tqid);
			else SetPlayerVirtualWorld(playerid,0); // Treasure of nation in global world
			ResetPlayerWeapons(playerid);
			oSetPlayerHealth(playerid,100.0);
			oSetPlayerArmour(playerid,100.0);
			SetPlayerSkin(playerid,TeamQuests[tqid][tq_team_skin][team]);
			switch(team)
			{
				case 0:
				{
					for(new w = 0;w < 3;w++) GivePlayerWeapon(playerid,TeamQuests[tqid][tq_team1_weapons][w],9999);
				}
				case 1:
				{
					for(new w = 0;w < 3;w++) GivePlayerWeapon(playerid,TeamQuests[tqid][tq_team2_weapons][w],9999);
				}
			}
		}
	}
}

TeamQuest_Text(playerid,textid,string[])
{
	static tqid;
	switch(textid)
	{
		case TEAM_QUEST_TEAM_TEXT:
		{
			if(GetPlayerQuest(playerid) == INVALID_QUEST_ID) return 0;
			tqid = TeamQuestPlayerCurrent{playerid};
			if((tqid != INVALID_TEAM_QUEST_ID) && (GetPlayerQuest(playerid) == TeamQuestQuest[tqid]))
			{
				format(stmp,sizeof(stmp),"[TQ] %s: %s",oGetPlayerName(playerid),string);
				for(new j = 0;j <= GetPlayersCount();j++)
				{
					if(!IsPlayerConnected(j)) continue;
					if(IsPlayerNPC(j)) continue;
					if(GetPlayerQuest(j) != TeamQuestQuest[tqid]) continue;
					if(TeamQuestPlayerTeam{j} != TeamQuestPlayerTeam{playerid}) continue;
					
					SendClientMessage(j,TEAM_QUESTS_COLOR,stmp);
				}
				return 1;
			}
		}
		case TEAM_QUEST_ANSWER_TEXT:
		{
			if(GetPlayerQuest(playerid) == INVALID_QUEST_ID) return 0;
			tqid = TeamQuestPlayerCurrent{playerid};
			if((tqid == TEAM_QUEST_TN) && (GetPlayerQuest(playerid) == TeamQuestQuest[tqid]))
			{
				if(TeamQuestStatus{tqid} != TEAM_QUEST_STATUS_ACTION) return 0;
				if(strlen(string) == 0) return 0;
				static team;
				team = TeamQuestPlayerTeam{playerid};

				if(TeamQuestTmp[tqid][6 + team] < 0)
				{
					SendClientMessage(playerid,TEAM_QUESTS_COLOR,"Вопрос не задан");
					return 1;
				}
				// del space
				while(string[0] == ' ') strdel(string,0,1);
				// check
				if(strcmp(string,TeamQuest_TN_Answer[team],true))
				{
					if(TeamQuestTmp[tqid][8 + team] == 0)
					{
						TeamQuestTmp[tqid][2 + team] = random(sizeof(TeamQuest_TN_Pickups));
						TeamQuestTmp[tqid][4 + team] = CreatePickup(TeamQuest_TN_Box[random(sizeof(TeamQuest_TN_Box))],23,TeamQuest_TN_Pickups[ TeamQuestTmp[tqid][2 + team] ][0],TeamQuest_TN_Pickups[ TeamQuestTmp[tqid][2 + team] ][1],TeamQuest_TN_Pickups[ TeamQuestTmp[tqid][2 + team] ][2],0);
						format(stmp,sizeof(stmp),"Ответ '%s' не верен.Следующая подсказка находится в зоне '%s'",string,GetXYZZoneName(TeamQuest_TN_Pickups[ TeamQuestTmp[tqid][2 + team] ][0],TeamQuest_TN_Pickups[ TeamQuestTmp[tqid][2 + team] ][1],TeamQuest_TN_Pickups[ TeamQuestTmp[tqid][2 + team] ][2]));
						for(new j = 0;j < MAX_PLAYERS;j++)
						{
							if(!IsPlayerConnected(j)) continue;
							if(GetPlayerQuest(j) != TeamQuestQuest[tqid]) continue;
							if(TeamQuestPlayerTeam{j} != team) continue;
							
							SendClientMessage(j,TEAM_QUESTS_COLOR,stmp);
						}
						TeamQuestTmp[tqid][6 + team] = -1;
					}
					else
					{
						TeamQuestTmp[tqid][8 + team]--;
						
						format(stmp,sizeof(stmp),"Ответ '%s' не верен.У вас осталось %d попыток",string,TeamQuestTmp[tqid][8 + team]);
						for(new j = 0;j < MAX_PLAYERS;j++)
						{
							if(!IsPlayerConnected(j)) continue;
							if(GetPlayerQuest(j) != TeamQuestQuest[tqid]) continue;
							if(TeamQuestPlayerTeam{j} != team) continue;
							
							SendClientMessage(j,TEAM_QUESTS_COLOR,stmp);
						}
					}
				}
				else
				{
					TeamQuestTmp[tqid][0 + team]++;
					if(TeamQuestTmp[tqid][0 + team] == 10)
					{
						TeamQuestTmp[tqid][2 + team] = random(sizeof(TeamQuest_TN_Pickups));
						TeamQuestTmp[tqid][3 + team] = CreatePickup(TeamQuest_TN_Box[random(sizeof(TeamQuest_TN_Box))],23,TeamQuest_TN_Pickups[ TeamQuestTmp[tqid][2 + team] ][0],TeamQuest_TN_Pickups[ TeamQuestTmp[tqid][2 + team] ][1],TeamQuest_TN_Pickups[ TeamQuestTmp[tqid][2 + team] ][2],0);
						format(stmp,sizeof(stmp),"Ответ '%s' верен.Вы набрали %d бонусных очков.В ящике было положение сокровищ,они находятся в зоне '%s'",string,TeamQuestTmp[tqid][0 + team],GetXYZZoneName(TeamQuest_TN_Pickups[ TeamQuestTmp[tqid][2 + team] ][0],TeamQuest_TN_Pickups[ TeamQuestTmp[tqid][2 + team] ][1],TeamQuest_TN_Pickups[ TeamQuestTmp[tqid][2 + team] ][2]));
						for(new j = 0;j < MAX_PLAYERS;j++)
						{
							if(!IsPlayerConnected(j)) continue;
							if(GetPlayerQuest(j) != TeamQuestQuest[tqid]) continue;
							if(TeamQuestPlayerTeam{j} != team) continue;
							
							SendClientMessage(j,TEAM_QUESTS_COLOR,stmp);
						}
						TeamQuestTmp[tqid][6 + team] = -2;
					}
					else
					{
						TeamQuestTmp[tqid][2 + team] = random(sizeof(TeamQuest_TN_Pickups));
						TeamQuestTmp[tqid][3 + team] = CreatePickup(TeamQuest_TN_Box[random(sizeof(TeamQuest_TN_Box))],23,TeamQuest_TN_Pickups[ TeamQuestTmp[tqid][2 + team] ][0],TeamQuest_TN_Pickups[ TeamQuestTmp[tqid][2 + team] ][1],TeamQuest_TN_Pickups[ TeamQuestTmp[tqid][2 + team] ][2],0);
						format(stmp,sizeof(stmp),"Ответ '%s' верен.Вы набрали %d бонусных очков.Следующая подсказка находится в зоне '%s'",string,TeamQuestTmp[tqid][0 + team],GetXYZZoneName(TeamQuest_TN_Pickups[ TeamQuestTmp[tqid][2 + team] ][0],TeamQuest_TN_Pickups[ TeamQuestTmp[tqid][2 + team] ][1],TeamQuest_TN_Pickups[ TeamQuestTmp[tqid][2 + team] ][2]));
						for(new j = 0;j < MAX_PLAYERS;j++)
						{
							if(!IsPlayerConnected(j)) continue;
							if(GetPlayerQuest(j) != TeamQuestQuest[tqid]) continue;
							if(TeamQuestPlayerTeam{j} != team) continue;
							
							SendClientMessage(j,TEAM_QUESTS_COLOR,stmp);
						}
						TeamQuestTmp[tqid][6 + team] = -1;
					}
				}
				return 1;
			}
		}
	}
	return 0;
}

TeamQuest_Command(playerid,cmdid,params[])
{
	#pragma unused params
	switch(cmdid)
	{
		case TEAM_QUEST_HELP_CMD:
		{
			stmp[0] = 0;
			strcat(stmp,"Вы можете учавствовать в командных соревнованияъ\nКаждой команде даётся определённое задание и 1 минута на обсуждение тактики\n");
			strcat(stmp,"после чего начинается соревнование\nВ некоторых случаях соревнование предполагает отыгрывание роли (к примеру роли охранника сейфа)\n");
			strcat(stmp,"Интересно играть на таких сорвенованиях когда все участники отыгрывают роль\nа не пытаются получить лёгкую прибыль.\n");
			strcat(stmp,"Команды:\n/tq menu - выбрать соревнование\n/tq quit - покинуть сорвенование");
			ShowPlayerDialog(playerid,DIALOG_NONE_ACTION,DIALOG_STYLE_MSGBOX,MODE_NAME " -> Командные соревнования",stmp,"Ок","Ок");
			return 1;
		}
		case TEAM_QUEST_MENU_CMD:
		{
			if(GetPlayerQuest(playerid) != INVALID_QUEST_ID) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы уже учавствуете в другом соревновании");
			
			stmp[0] = 0;
			// checker
			if(strlen(params) > 0) itmp[0] = 1;
			else itmp[0] = 0;
			for(new i = 0;i < MAX_TEAM_QUESTS;i++) 
			{
				if(itmp[0])
				{
					if(!strcmp(params,TeamQuests[i][tq_name],true))
					{
						PlayerJoinTeamQuest(playerid,i);
						return 1;
					}
				}
				
				format(stmp,sizeof(stmp),"%s%s\n",stmp,TeamQuests[i][tq_name]);
			}

			ShowPlayerDialog(playerid,DIALOG_TEAM_QUESTS,DIALOG_STYLE_LIST,"Team Quests",stmp,"Зайти","Отмена");
			return 1;
		}
		case TEAM_QUEST_QUIT_CMD:
		{
			static tqid;
			if(GetPlayerQuest(playerid) != INVALID_QUEST_ID)
			{
				tqid = TeamQuestPlayerCurrent{playerid};
				if((tqid != INVALID_TEAM_QUEST_ID) && (GetPlayerQuest(playerid) == TeamQuestQuest[tqid]))
					PlayerLeaveTeamQuest(playerid,tqid,TEAM_QUEST_LEAVE_REASON_CMD);
				return 1;
			}
			SendClientMessage(playerid,SYSTEM_COLOR,"Вы не учавствуете соревновании");
			return 1;
		}
	}
	return 0;
}

TeamQuest_Dialog(playerid, dialogid, response, listitem, inputtext[])
{
	#pragma unused inputtext
	switch(dialogid)
	{
		case DIALOG_TEAM_QUESTS:
		{
			if(!response) return 1;
			
			PlayerJoinTeamQuest(playerid,listitem);
			return 1;
		}
		case DIALOG_TEAM_QUEST_TEAM:
		{
			static 
				tqid,
				team;
			team = (response == 0?1:0);
			tqid = TeamQuestPlayerCurrent{playerid};
			if((tqid != INVALID_TEAM_QUEST_ID) && (GetPlayerQuest(playerid) == TeamQuestQuest[tqid]))
			{
				if(TeamQuestStatus{tqid} != TEAM_QUEST_STATUS_LINEUP) return SendClientMessage(playerid,SYSTEM_COLOR,"Соревнование уже началось");
				TeamQuestTeamCount[tqid]{team}++;
				TeamQuestPlayerTeam{playerid} = team;
				format(stmp,sizeof(stmp),"Team Quest \"%s\", %s - %d, %s - %d",TeamQuests[tqid][tq_name],TeamQuests_Teams[tqid][0],TeamQuestTeamCount[tqid]{0},TeamQuests_Teams[tqid][1],TeamQuestTeamCount[tqid]{1});
				SendClientMessageByQuest(INVALID_QUEST_ID,TEAM_QUESTS_COLOR,stmp);
				SendClientMessageByQuest(TeamQuestQuest[tqid],TEAM_QUESTS_COLOR,stmp);
				return 1;
			}
		}
	}
	return 0;
}