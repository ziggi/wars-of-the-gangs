/*
*	Created:			26.05.10
*	Author:				009
*	Description:		Лучшая система миссий с большими возможностями 
*/

#if defined _missions_included
	#endinput
#endif

#define _missions_included
#pragma library missions

// --------------------------------------------------
// defines
// --------------------------------------------------
#define MISSIONS_DIR				"missions/"
#define CUTSCENES_DIR				"cutscenes/"
#define ACTIONS_DIR					"actions/"
#define BOSS_MASK_DIR				"boss_%d/"
#define CUTSCENES_FILE				"cutscenes.txt"
#define BOSSES_FILE					"bosses.txt"
#define MISSIONS_FILE				"missions.txt"
#define CUTSCENE_MASK_FILE			"cutscene_%d_%d.txt"
#define ACTION_MASK_FILE			"action_%d_%d.txt"

#define MAX_BOSSES					50
#define MAX_BOSS_NAME				24
#define MAX_MISSION_TYPES			10
#define INVALID_BOSS_ID				255

#define MISSIONS_COLOR				0x441188FF
#define DINAMITE_MODEL 				1654
#define SAFE_OPEN_MODEL				1829
#define SAFE_CLOSED_MODEL			2332
#define MAX_BIGBOOM_VEHICLE_DAMAGE	10.0

// --------------------------------------------------
// enums
// --------------------------------------------------
enum MissionsInfo
{
	m_money,
	m_xp,
	m_rep
};
enum
{
	MISSIONS_DSET_CMD,
	MISSIONS_DGET_CMD,
	MISSIONS_DBOOM_CMD
};
enum
{
	MISSION_MAIL,
	MISSION_WEAPON,
	MISSION_VEHICLE,
	MISSION_GONE60SEC,
	MISSION_BOOM,
	MISSION_BOOMCASH,
	MISSION_BIGBOOM,
	MISSION_DESANT,
	MISSION_LONGWAY,
	MISSION_BANK
};
enum
{
	MISSION_STAGE_CUTSCENE,
	MISSION_STAGE_ACTION
};
enum
{
	MISSION_END_REASON_COMPLETE,
	MISSION_END_REASON_FAILED
};
enum
{
	CUTSCENE_ACTION_TITLE,
	CUTSCENE_ACTION_TEXT,
	CUTSCENE_ACTION_EXPLOSION,
	CUTSCENE_ACTION_SKIN,
	CUTSCENE_ACTION_POS,
	CUTSCENE_ACTION_INTERIOR,
	CUTSCENE_ACTION_LETTERBOX,
	CUTSCENE_ACTION_CAMERA_POS,
	CUTSCENE_ACTION_BLACKSCREEN
};
// mail mission
enum 
{
	MISSION_MAIL_STEP_INIT,
	MISSION_MAIL_STEP_END
};
// weapon mission
enum 
{
	MISSION_WEAPON_STEP_INIT,
	MISSION_WEAPON_STEP_GT_TARGET,
	MISSION_WEAPON_STEP_END
};
// theft auto mission
enum 
{
	MISSION_VEHICLE_STEP_INIT,
	MISSION_VEHICLE_STEP_GT_TARGET,
	MISSION_VEHICLE_STEP_END
};
// gone in 60 sec mission
enum 
{
	MISSION_GONE60SEC_STEP_INIT,
	MISSION_GONE60SEC_STEP_GT_TARG,
	MISSION_GONE60SEC_STEP_DELIVED
};
// boom mission
enum 
{
	MISSION_BOOM_STEP_INIT,
	MISSION_BOOM_STEP_TAKE_DINAMITE,
	MISSION_BOOM_STEP_PUT_DINAMITE,
	MISSION_BOOM_STEP_BOOM
};
// boom cash mission
enum 
{
	MISSION_BOOMCASH_STEP_INIT,
	MISSION_BOOMCASH_STEP_PUT_DINAM,
	MISSION_BOOMCASH_STEP_BOOM,
	MISSION_BOOMCASH_STEP_TAKE_MAIL,
	MISSION_BOOMCASH_STEP_END
};
// big boom mission
enum 
{
	MISSION_BIGBOOM_STEP_INIT,
	MISSION_BIGBOOM_STEP_GT_TARGET,
	MISSION_BIGBOOM_STEP_TIMER,
	MISSION_BIGBOOM_STEP_BOOM
};
// desant mission
enum 
{
	MISSION_DESANT_STEP_INIT,
	MISSION_DESANT_STEP_GT_TARGET,
	MISSION_DESANT_STEP_JUMP,
	MISSION_DESANT_STEP_TIMER,
	MISSION_DESANT_STEP_BOOM,
	MISSION_DESANT_STEP_ENTER,
	MISSION_DESANT_STEP_IN_CAR,
	MISSION_DESANT_STEP_END
};
// long way mission
enum 
{
	MISSION_LONGWAY_STEP_INIT,
	MISSION_LONGWAY_STEP_GT_TARGET,
	MISSION_LONGWAY_STEP_CITY,
	MISSION_LONGWAY_STEP_ENTER,
	MISSION_LONGWAY_STEP_TAKE_MAIL,
	MISSION_LONGWAY_STEP_EXIT,
	MISSION_LONGWAY_STEP_END
};
// bank rob mission
enum 
{
	MISSION_BANK_STEP_INIT,
	MISSION_BANK_STEP_ENTER,
	MISSION_BANK_STEP_START_HACK,
	MISSION_BANK_STEP_HACK,
	MISSION_BANK_STEP_RUN,
	MISSION_BANK_STEP_EXIT,
	MISSION_BANK_STEP_END
};

// --------------------------------------------------
// statics
// --------------------------------------------------
static Missions[MAX_MISSION_TYPES][MissionsInfo] = 
{
	{5000,1000,10},		// почта
	{10_000,5000,10},		// доставка оружия
	{50_000,20_000,15},	// кража транспорта
	{100_000,50_000,50},	// угнать за 60 сек
	{70_000,25_000,25},	// Взрыв здания используя динамит из тачи
	{100_000,40_000,30},	// Взрыв двери и кража денег
	{70_000,30_000,20},	// заминированая тача и взрыв здания ей
	{100_000,45_000,30},	// взять самолёт и десантироваться на крышу, там заложить мину и свалить
	{150_000,60_000,60},	// взять самолёт, улететь в куда-то, попасть куда-то, отдать бабки, улететь нахрен
	{200_000,100_000,100}	// приехать в любой банк, взломать код устройством и забрав бабки привезти их боссу
};
static
	BossCheckpointId[MAX_BOSSES],
	BossName[MAX_BOSSES][MAX_BOSS_NAME],
	BossQuest[MAX_BOSSES],
	BossMissionsTypesCount[MAX_BOSSES char],
	BossMissionsTypes[MAX_BOSSES][MAX_MISSION_TYPES char],
	BossMissionsRespect[MAX_BOSSES][MAX_MISSION_TYPES char],
	CheckpointsBossLink[MAX_STREAMED_CHECKPOINTS char]; // т.к. боссов не больше 256 то чар, как станет больше переходим на инт
static
	Text:Letterbox[2],
	Text:Blackscreen[15];
static WeaponModels[] =
{
	343,
	344,
	346,
	347,
	348,
	349,
	350,
	351,
	352,
	353,
	355,
	356,
	357,
	358,
	359,
	360,
	361,
	362,
	368,
	369,
	372,
	373
};
static 
	Float:PlayerOldPos[MAX_PLAYERS][4],
	PlayerOldInterior[MAX_PLAYERS char],
	PlayerOldSkin[MAX_PLAYERS];
	

// --------------------------------------------------
// stocks
// --------------------------------------------------
stock ShowCutscene(playerid,mboss,mtype,mrand,step)
{
	static 
		string[128],
		line[256];
	format(string,sizeof(string),MODE_DIR MISSIONS_DIR BOSS_MASK_DIR CUTSCENES_DIR CUTSCENE_MASK_FILE,mboss,mtype,mrand);
	new File:datafile = fopen(string,io_read);
	if(datafile)
	{
		// find line by step
		for(new i = 0;i <= step;i++) 
		{
			if(!fread(datafile,line))
			{
				// file end - start mission
				fclose(datafile);
				SetCameraBehindPlayer(playerid);
				// randomize mission data
				format(string,sizeof(string),MODE_DIR MISSIONS_DIR BOSS_MASK_DIR MISSIONS_FILE,mboss);
				datafile = fopen(string,io_read);
				if(datafile)
				{
					// go to our mission
					for(new j = 0;j <= GetPVarInt(playerid,"MissionType");j++) fread(datafile,line);
					// randomize
					SetPVarInt(playerid,"MissionRandomize",random(strval(line)));
					fclose(datafile);
				}
				else print("[ERROR] missions file not found");
				// set data
				TogglePlayerControllable(playerid,true);
				SetPlayerPos(playerid,PlayerOldPos[playerid][0],PlayerOldPos[playerid][1],PlayerOldPos[playerid][2]);
				SetPlayerFacingAngle(playerid,PlayerOldPos[playerid][3]);
				SetPlayerInterior(playerid,PlayerOldInterior{playerid});
				SetPlayerSkin(playerid,PlayerOldSkin[playerid]);
				TextDrawHideForPlayer(playerid,Letterbox[0]);
				TextDrawHideForPlayer(playerid,Letterbox[1]);
				for(new j = 0;j < sizeof(Blackscreen);j++) TextDrawHideForPlayer(playerid,Blackscreen[j]);
				SetPVarInt(playerid,"MissionStage",MISSION_STAGE_ACTION);
				// action
				ChangePlayerMissionStep(playerid,0);				
				return -1;
			}
		}
		// check
		if(!CheckFileLine(line)) return -1;
		// parse
		static idx;
		idx = 0;
		// time of action 
		itmp[0] = strval(strtok(line,idx,','));
		// type of action
		itmp[1] = strval(strtok(line,idx,','));
		// next by type
		switch(itmp[1])
		{
			case CUTSCENE_ACTION_TITLE:
			{
				GameTextForPlayer(playerid,line[idx],5000,1);
			}
			case CUTSCENE_ACTION_TEXT: 
			{
				switch(strval(strtok(line,idx,',')))
				{
					case 0: format(string,sizeof(string),line[idx]);
					case 1: format(string,sizeof(string),"%s: %s",BossName[ GetPVarInt(playerid,"MissionBoss") ],line[idx]);
				}
				SendClientMessage(playerid,MISSIONS_COLOR,string);
			}
			case CUTSCENE_ACTION_EXPLOSION:
			{
				ftmp[0] = floatstr(strtok(line,idx,','));
				ftmp[1] = floatstr(strtok(line,idx,','));
				ftmp[2] = floatstr(strtok(line,idx,','));
				itmp[2] = strval(strtok(line,idx,','));
				ftmp[3] = floatstr(strtok(line,idx,','));
				CreateExplosion(ftmp[0],ftmp[1],ftmp[2],itmp[2],ftmp[3]);
			}
			case CUTSCENE_ACTION_SKIN:
			{
				itmp[2] = strval(strtok(line,idx,','));
				SetPlayerSkin(playerid,itmp[2]);
			}
			case CUTSCENE_ACTION_POS:
			{
				ftmp[0] = floatstr(strtok(line,idx,','));
				ftmp[1] = floatstr(strtok(line,idx,','));
				ftmp[2] = floatstr(strtok(line,idx,','));
				ftmp[3] = floatstr(strtok(line,idx,','));
				SetPlayerPos(playerid,ftmp[0],ftmp[1],ftmp[2]);
				SetPlayerFacingAngle(playerid,ftmp[3]);
			}
			case CUTSCENE_ACTION_INTERIOR:
			{
				itmp[2] = strval(strtok(line,idx,','));
				SetPlayerInterior(playerid,itmp[2]);
			}
			case CUTSCENE_ACTION_LETTERBOX:
			{
				itmp[2] = strval(strtok(line,idx,','));
				if(itmp[2])
				{
					TextDrawShowForPlayer(playerid,Letterbox[0]);
					TextDrawShowForPlayer(playerid,Letterbox[1]);
				}
				else
				{
					TextDrawHideForPlayer(playerid,Letterbox[0]);
					TextDrawHideForPlayer(playerid,Letterbox[1]);
				}
			}
			case CUTSCENE_ACTION_CAMERA_POS:
			{
				ftmp[0] = floatstr(strtok(line,idx,','));
				ftmp[1] = floatstr(strtok(line,idx,','));
				ftmp[2] = floatstr(strtok(line,idx,','));
				ftmp[3] = floatstr(strtok(line,idx,','));
				ftmp[4] = floatstr(strtok(line,idx,','));
				ftmp[5] = floatstr(strtok(line,idx,','));
				SetPlayerCameraPos(playerid,ftmp[0],ftmp[1],ftmp[2]);
				SetPlayerCameraLookAt(playerid,ftmp[3],ftmp[4],ftmp[5]);
			}
			case CUTSCENE_ACTION_BLACKSCREEN:
			{
				for(new i = 0;i < sizeof(Blackscreen);i++) TextDrawHideForPlayer(playerid,Blackscreen[i]);
				itmp[2] = strval(strtok(line,idx,','));
				if(itmp[2] != -1) TextDrawShowForPlayer(playerid,Blackscreen[ itmp[2] ]);
			}
		}
		// file end
		fclose(datafile);

		return itmp[0];
	}
	else 
	{
		printf("[ERROR] mission cutscene file not found (%s)",string);
		return -1;
	}
}

stock ChangePlayerMissionStep(playerid,step)
{
	static 
		mtype,
		mrand,
		mboss,
		File:datafile,
		string[128],
		line[256],
		tmp,
		idx;
	mtype = GetPVarInt(playerid,"MissionType");
	mrand = GetPVarInt(playerid,"MissionRandomize");
	mboss = GetPVarInt(playerid,"MissionBoss");
	format(string,sizeof(string),MODE_DIR MISSIONS_DIR BOSS_MASK_DIR ACTIONS_DIR ACTION_MASK_FILE,mboss,mtype,mrand);
	idx = 0;
	// find mission type
	switch(mtype)
	{
		case MISSION_MAIL:
		{
			/*
			mission - mail
			Просто добраться до чекпоинта
			*/
			// find needed step
			switch(step)
			{
				case MISSION_MAIL_STEP_INIT: // step - init
				{
					// read data from file
					datafile = fopen(string,io_read);
					if(datafile)
					{
						// read coords
						fread(datafile,line);
						// check
						if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
						ftmp[0] = floatstr(strtok(line,idx,','));
						ftmp[1] = floatstr(strtok(line,idx,','));
						ftmp[2] = floatstr(strtok(line,idx,','));
						// set checkpoint
						SetPlayerCheckpoint(playerid,ftmp[0],ftmp[1],ftmp[2],5.0);
						// close
						fclose(datafile);
					}
					else printf("[ERROR] mission actions file not found (%s)",string);
				}
				case MISSION_MAIL_STEP_END: // step - complete
				{
					PlayerMissionComplete(playerid,MISSION_END_REASON_COMPLETE);
					return 1;
				}
			}
		}
		case MISSION_WEAPON:
		{
			/*
			mission - weapons mail
			Спавнится определённое количество оружия-пикапов , их местоположение отмечено чекпоинтом, когда все собраны показан чекпоинт куда привезти, привёз - закончил
			*/
			// find needed step
			switch(step)
			{
				case MISSION_WEAPON_STEP_INIT: // step - init
				{
					// read data from file
					datafile = fopen(string,io_read);
					if(datafile)
					{
						// read count of pickups
						fread(datafile,line);
						// check
						if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
						// read pickups
						tmp = strval(line);
						for(new i = 0;i < tmp;i++)
						{
							fread(datafile,line);
							// check
							if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
							idx = 0;
							ftmp[0] = floatstr(strtok(line,idx,','));
							ftmp[1] = floatstr(strtok(line,idx,','));
							ftmp[2] = floatstr(strtok(line,idx,','));
							// create
							format(string,sizeof(string),"MissionPickup_%d",CreatePickup(WeaponModels[random(sizeof(WeaponModels))],42,ftmp[0],ftmp[1],ftmp[2]));
							SetPVarInt(playerid,string,1);
						}
						SetPVarInt(playerid,"MissionPickups",tmp);
						// read checkpoint of pickups
						fread(datafile,line);
						// check
						if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
						idx = 0;
						ftmp[0] = floatstr(strtok(line,idx,','));
						ftmp[1] = floatstr(strtok(line,idx,','));
						ftmp[2] = floatstr(strtok(line,idx,','));
						SetPlayerCheckpoint(playerid,ftmp[0],ftmp[1],ftmp[2],5.0);
						// close
						fclose(datafile);
					}
					else printf("[ERROR] mission actions file not found (%s)",string);
				}
				case MISSION_WEAPON_STEP_GT_TARGET: // step - go to target
				{
					// read data from file
					datafile = fopen(string,io_read);
					if(datafile)
					{
						// read count of pickups
						fread(datafile,line);
						// check
						if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
						// read pickups
						tmp = strval(line);
						for(new i = 0;i < tmp;i++) fread(datafile,line);
						// read checkpoint of pickups
						fread(datafile,line);
						// read target point
						fread(datafile,line);
						// check
						if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
						ftmp[0] = floatstr(strtok(line,idx,','));
						ftmp[1] = floatstr(strtok(line,idx,','));
						ftmp[2] = floatstr(strtok(line,idx,','));
						// set checkpoint
						SetPlayerCheckpoint(playerid,ftmp[0],ftmp[1],ftmp[2],5.0);
						// close
						fclose(datafile);
					}
					else printf("[ERROR] mission actions file not found (%s)",string);
				}
				case MISSION_WEAPON_STEP_END: // step - complete
				{
					PlayerMissionComplete(playerid,MISSION_END_REASON_COMPLETE);
					return 1;
				}
			}
		}
		case MISSION_VEHICLE:
		{
			/*
			mission - theft auto
			Спавнится транспорт, отмечается чекопинтом, нужно его украсть и тебе покажут чекпоинт - назначения куда привезти тачку
			*/
			// find needed step
			switch(step)
			{
				case MISSION_VEHICLE_STEP_INIT: // step - init
				{
					// read data from file
					datafile = fopen(string,io_read);
					if(datafile)
					{
						// read vehicle data
						fread(datafile,line);
						// check
						if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
						// vehicle type
						tmp = strval(strtok(line,idx,','));
						// vehicle pos
						ftmp[0] = floatstr(strtok(line,idx,','));
						ftmp[1] = floatstr(strtok(line,idx,','));
						ftmp[2] = floatstr(strtok(line,idx,','));
						ftmp[3] = floatstr(strtok(line,idx,','));
						// create vehicle
						tmp = CreateVehicle(GetVehicleModelByType(tmp),ftmp[0],ftmp[1],ftmp[2],ftmp[3],-1,-1,999_999);
						SetPVarInt(playerid,"MissionVehicle",tmp);
						// show on map
						SetPlayerCheckpoint(playerid,ftmp[0],ftmp[1],ftmp[2],5.0);
						// close
						fclose(datafile);
					}
					else printf("[ERROR] mission actions file not found (%s)",string);
				}
				case MISSION_VEHICLE_STEP_GT_TARGET: // step - go to target
				{
					// read data from file
					datafile = fopen(string,io_read);
					if(datafile)
					{
						// read vehicle data
						fread(datafile,line);
						// read target data
						fread(datafile,line);
						// check
						if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
						// target pos
						ftmp[0] = floatstr(strtok(line,idx,','));
						ftmp[1] = floatstr(strtok(line,idx,','));
						ftmp[2] = floatstr(strtok(line,idx,','));
						// show on map
						SetPlayerCheckpoint(playerid,ftmp[0],ftmp[1],ftmp[2],5.0);
						// close
						fclose(datafile);
					}
					else printf("[ERROR] mission actions file not found (%s)",string);
				}
				case MISSION_VEHICLE_STEP_END: // step - complete (vehicle is delivered)
				{
					PlayerMissionComplete(playerid,MISSION_END_REASON_COMPLETE);
					return 1;
				}
			}
		}
		case MISSION_GONE60SEC:
		{
			/*
			mission - gone in 60 seconds
			Спавнится определённое количество транспорта, отмечается целью, садишся в один из этих транспортов - показывают чекпоинт места назначения, когда довёз нужно следующую тачку везти (а если это все то конец)
			*/
			// find needed step
			switch(step)
			{
				case MISSION_GONE60SEC_STEP_INIT: // step - init
				{
					// read data from file
					datafile = fopen(string,io_read);
					if(datafile)
					{
						// first line - target (jump)
						fread(datafile,line);
						// second line - count of vehicles (read)
						fread(datafile,line);
						// check
						if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
						// read vehicles
						tmp = strval(line);
						for(new i = 0;i < tmp;i++)
						{
							fread(datafile,line);
							// check
							if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
							idx = 0;
							new tm = GetVehicleModelByType(strval(strtok(line,idx,',')));
							ftmp[0] = floatstr(strtok(line,idx,','));
							ftmp[1] = floatstr(strtok(line,idx,','));
							ftmp[2] = floatstr(strtok(line,idx,','));
							ftmp[3] = floatstr(strtok(line,idx,','));
							// create
							tm = CreateVehicle(tm,ftmp[0],ftmp[1],ftmp[2],ftmp[3],-1,-1,999_999);
							format(string,sizeof(string),"MissionVehicle_%d",tm);
							SetPVarInt(playerid,string,1);
							format(string,sizeof(string),"MissionMarker_%d",tm);
							tm = CreateMarker(ftmp[0],ftmp[1]);
							SetPVarInt(playerid,string,tm);
							ShowMarkerForPlayer(playerid,tm,0x0000FFFF);
						}
						SetPVarInt(playerid,"MissionVehicles",tmp);
						format(string,sizeof(string),"Осталось %d машин",tmp);
						SendClientMessage(playerid,SYSTEM_COLOR,string);
						// close
						fclose(datafile);
					}
					else printf("[ERROR] mission actions file not found (%s)",string);
				}
				case MISSION_GONE60SEC_STEP_GT_TARG: // step - go to target
				{
					// read data from file
					datafile = fopen(string,io_read);
					if(datafile)
					{
						// read target
						fread(datafile,line);
						// check
						if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
						ftmp[0] = floatstr(strtok(line,idx,','));
						ftmp[1] = floatstr(strtok(line,idx,','));
						ftmp[2] = floatstr(strtok(line,idx,','));
						// set checkpoint
						SetPlayerCheckpoint(playerid,ftmp[0],ftmp[1],ftmp[2],5.0);
						// close
						fclose(datafile);
					}
					else printf("[ERROR] mission actions file not found (%s)",string);
				}
				case MISSION_GONE60SEC_STEP_DELIVED: // step - vehicle delived
				{
					tmp = GetPlayerVehicleID(playerid);
					format(string,sizeof(string),"MissionVehicle_%d",tmp);
					if(GetPVarInt(playerid,string) == 1) 
					{
						// free
						DeletePVar(playerid,string);
						// delete vehicle
						DestroyVehicle(tmp);
						// marker destroy
						format(string,sizeof(string),"MissionMarker_%d",tmp);
						DestroyMarker(GetPVarInt(playerid,string));
						DeletePVar(playerid,string);
						// mission data
						tmp = GetPVarInt(playerid,"MissionVehicles") - 1;
						SetPVarInt(playerid,"MissionVehicles",tmp);
						if(tmp == 0)
						{
							// mission completed!
							PlayerMissionComplete(playerid,MISSION_END_REASON_COMPLETE);
							return 1;
						}
						else
						{
							// cicle :D
							format(string,sizeof(string),"Осталось %d машин",tmp);
							SendClientMessage(playerid,SYSTEM_COLOR,string);
							DisablePlayerCheckpoint(playerid);
							SetPVarInt(playerid,"MissionStep",0);
							return 1;
						}
					}
					else print("[ERROR] mission vehicle bad detected");
				}
			}
		}
		case MISSION_BOOM:
		{
			/*
			mission - building boom
			Спавнится транспорт, с определённым количеством "динамита" внутри, ставится цель-зона на карте , нужно приехать туда, расставить динамит(брать из транспорта) по зоне(минимум 10) 
			и потом взорвать всё это, тогда победа.
			*/
			// find needed step
			switch(step)
			{
				case MISSION_BOOM_STEP_INIT: // step - init
				{
					// read data from file
					datafile = fopen(string,io_read);
					if(datafile)
					{
						// first line - pos of vehicle with dinamite
						fread(datafile,line);
						// check
						if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
						new model = GetVehicleModelByType(strval(strtok(line,idx,',')));
						ftmp[0] = floatstr(strtok(line,idx,','));
						ftmp[1] = floatstr(strtok(line,idx,','));
						ftmp[2] = floatstr(strtok(line,idx,','));
						ftmp[3] = floatstr(strtok(line,idx,','));
						// create
						SetPVarInt(playerid,"MissionVehicle",CreateVehicle(model,ftmp[0],ftmp[1],ftmp[2],ftmp[3],-1,-1,999_999));
						// set data
						SetPVarInt(playerid,"MissionVehicleDinamites",strval(strtok(line,idx,',')));
						// show cp
						SetPlayerCheckpoint(playerid,ftmp[0],ftmp[1],ftmp[2],5.0);
						// second line - boom zone
						fread(datafile,line);
						// check
						if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
						// set zone
						idx = 0;
						ftmp[0] = floatstr(strtok(line,idx,','));
						ftmp[1] = floatstr(strtok(line,idx,','));
						ftmp[2] = floatstr(strtok(line,idx,','));
						ftmp[3] = floatstr(strtok(line,idx,','));
						SetPVarFloat(playerid,"MissionZoneMinX",ftmp[0]);
						SetPVarFloat(playerid,"MissionZoneMinY",ftmp[1]);
						SetPVarFloat(playerid,"MissionZoneMaxX",ftmp[2]);
						SetPVarFloat(playerid,"MissionZoneMaxY",ftmp[3]);
						// create
						tmp = GangZoneCreate(ftmp[0],ftmp[1],ftmp[2],ftmp[3]);
						GangZoneShowForPlayer(playerid,tmp,0x000000BB);
						SetPVarInt(playerid,"MissionZone",tmp);
						// set player data
						SetPVarInt(playerid,"MissionDinamites",0);
						SetPVarInt(playerid,"MissionDinamiteSet",0);
						// close
						fclose(datafile);
					}
					else printf("[ERROR] mission actions file not found (%s)",string);
				}
				case MISSION_BOOM_STEP_TAKE_DINAMITE: // step - take dinamite
				{
					// check vehicle
					if(!IsPlayerInVehicle(playerid,GetPVarInt(playerid,"MissionVehicle")))
					{
						SendClientMessage(playerid,SYSTEM_COLOR,"Вы должны быть в транспорте с динамитом");
						return 1;
					}
					// check vehicle dinamites
					if(GetPVarInt(playerid,"MissionVehicleDinamites") == 0)
					{
						SendClientMessage(playerid,SYSTEM_COLOR,"Динамита больше нет");
						return 1;
					}
					// check player dinamites
					tmp = GetPVarInt(playerid,"MissionDinamites");
					if(tmp >= 10)
					{
						SendClientMessage(playerid,SYSTEM_COLOR,"Вы не можете взять ещё больше динамита");
						return 1;
					}
					// take dinamite
					tmp = 10 - tmp;
					if(tmp > GetPVarInt(playerid,"MissionVehicleDinamites")) tmp = GetPVarInt(playerid,"MissionVehicleDinamites");
					SetPVarInt(playerid,"MissionDinamites",(GetPVarInt(playerid,"MissionDinamites") + tmp));
					SetPVarInt(playerid,"MissionVehicleDinamites",(GetPVarInt(playerid,"MissionVehicleDinamites") - tmp));
					format(string,sizeof(string),"У вас с собой %d динамита, в машине осталось %d",GetPVarInt(playerid,"MissionDinamites"),GetPVarInt(playerid,"MissionVehicleDinamites"));
					SendClientMessage(playerid,SYSTEM_COLOR,string);
					return 1;
				}
				case MISSION_BOOM_STEP_PUT_DINAMITE: // step - set dinamite
				{
					// check zone
					new Float:pos[3];
					GetPlayerPos(playerid,pos[0],pos[1],pos[2]);
					ftmp[0] = GetPVarFloat(playerid,"MissionZoneMinX");
					ftmp[1] = GetPVarFloat(playerid,"MissionZoneMinY");
					ftmp[2] = GetPVarFloat(playerid,"MissionZoneMaxX");
					ftmp[3] = GetPVarFloat(playerid,"MissionZoneMaxY");
					if((pos[0] < ftmp[0]) || (pos[0] < ftmp[1]) || (pos[1] > ftmp[2]) || (pos[1] > ftmp[3]))
					{
						SendClientMessage(playerid,SYSTEM_COLOR,"Вы должны быть в зоне цели");
						return 1;
					}
					// check dinamites count
					tmp = GetPVarInt(playerid,"MissionDinamites");
					if(tmp == 0)
					{
						SendClientMessage(playerid,SYSTEM_COLOR,"У вас нет с собой динамита, возьмите в машине");
						return 1;
					}
					SetPVarInt(playerid,"MissionDinamites",(tmp - 1));
					// set dinamite
					tmp = GetPVarInt(playerid,"MissionDinamiteSet");
					SetPVarInt(playerid,"MissionDinamiteSet",(tmp + 1));
					format(string,sizeof(string),"MissionDinamite_%d",tmp);
					tmp = CreateStreamedObject(DINAMITE_MODEL,pos[0],pos[1],pos[2],0.0,0.0,0.0);
					SetPVarInt(playerid,string,tmp);
					return 1;
				}
				case MISSION_BOOM_STEP_BOOM: // step - boom 
				{
					// check vehicle
					if(!IsPlayerInVehicle(playerid,GetPVarInt(playerid,"MissionVehicle")))
					{
						SendClientMessage(playerid,SYSTEM_COLOR,"Вы должны быть в транспорте с детонатором");
						return 1;
					}
					// check count
					tmp = GetPVarInt(playerid,"MissionDinamiteSet");
					if(tmp < 10)
					{
						SendClientMessage(playerid,SYSTEM_COLOR,"Нужно установить минимум 10 единиц динамита");
						return 1;
					}
					// boom it!
					for(new i = 0;i < tmp;i++)
					{
						format(string,sizeof(string),"MissionDinamite_%d",i);
						GetStreamedObjectPos(GetPVarInt(playerid,string),ftmp[0],ftmp[1],ftmp[2]);
						DestroyStreamedObject(GetPVarInt(playerid,string));
						// explo
						CreateExplosion(ftmp[0],ftmp[1],ftmp[2],1,50.0);
						// detele pvar
						DeletePVar(playerid,string);
					}
					// mission completed!
					PlayerMissionComplete(playerid,MISSION_END_REASON_COMPLETE);
					return 1;
				}
			}
		}
		case MISSION_BOOMCASH:
		{
			/*
			mission - boom safe
			"Даётся" динамит, создаётся сейф, появляется чекпоинт где сейф, нужно приехать туда, положить динамит, взорвать сейф, и забрать из него деньги, потом уехать к боссу
			*/
			// find needed step
			switch(step)
			{
				case MISSION_BOOMCASH_STEP_INIT: // step - init
				{
					// read data from file
					datafile = fopen(string,io_read);
					if(datafile)
					{
						// first line - pos of safe
						fread(datafile,line);
						// check
						if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
						ftmp[0] = floatstr(strtok(line,idx,','));
						ftmp[1] = floatstr(strtok(line,idx,','));
						ftmp[2] = floatstr(strtok(line,idx,','));
						ftmp[3] = floatstr(strtok(line,idx,','));
						ftmp[4] = floatstr(strtok(line,idx,','));
						ftmp[5] = floatstr(strtok(line,idx,','));
						// create
						SetPVarInt(playerid,"MissionSafe",CreateStreamedObject(SAFE_CLOSED_MODEL,ftmp[0],ftmp[1],ftmp[2],ftmp[3],ftmp[4],ftmp[5]));
						SetPlayerCheckpoint(playerid,ftmp[0],ftmp[1],ftmp[2],5.0);
						// set data
						SetPVarInt(playerid,"MissionTime",0);
						// close
						fclose(datafile);
					}
					else printf("[ERROR] mission actions file not found (%s)",string);
				}
				case MISSION_BOOMCASH_STEP_PUT_DINAM: // step - set dinamite
				{
					// set data
					DisablePlayerCheckpoint(playerid);
					SetPVarInt(playerid,"MissionTime",10);
					SendClientMessage(playerid,SYSTEM_COLOR,"Бомба взорвётся через 10 секунд");
				}
				case MISSION_BOOMCASH_STEP_BOOM: // step - boom and open
				{
					// get coords and delete object
					tmp = GetPVarInt(playerid,"MissionSafe");
					GetStreamedObjectPos(tmp,ftmp[0],ftmp[1],ftmp[2]);
					DestroyStreamedObject(tmp);
					// boom
					CreateExplosion(ftmp[0],ftmp[1],ftmp[2],1,5.0);
					// open
					SetPVarInt(playerid,"MissionSafe",CreateStreamedObject(SAFE_OPEN_MODEL,ftmp[0],ftmp[1],ftmp[2],0.0,0.0,0.0));
					// set checkpoint
					SetPlayerCheckpoint(playerid,ftmp[0],ftmp[1],ftmp[2],2.5);
				}
				case MISSION_BOOMCASH_STEP_TAKE_MAIL: // step - take money 
				{
					// read data from file
					datafile = fopen(string,io_read);
					if(datafile)
					{
						// first line - pos of safe
						fread(datafile,line);
						// second line - pos target
						fread(datafile,line);
						// check
						if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
						ftmp[0] = floatstr(strtok(line,idx,','));
						ftmp[1] = floatstr(strtok(line,idx,','));
						ftmp[2] = floatstr(strtok(line,idx,','));
						// apply
						SetPlayerCheckpoint(playerid,ftmp[0],ftmp[1],ftmp[2],5.0);
						SendClientMessage(playerid,SYSTEM_COLOR,"Везите деньги боссу");
						// close
						fclose(datafile);
					}
					else printf("[ERROR] mission actions file not found (%s)",string);
				}
				case MISSION_BOOMCASH_STEP_END: // step - mission completed!
				{
					PlayerMissionComplete(playerid,MISSION_END_REASON_COMPLETE);
					return 1;
				}
			}
		}
		case MISSION_BIGBOOM:
		{
			/*
			mission - boom auto in building
			Спавнится тача, нужно сесть в неё и ехать к чекпоинту (при получении урона транспорт может взорваться), когда доехал есть 5 секунд до взрыва,взрыв - завершение миссии
			*/
			// find needed step
			switch(step)
			{
				case MISSION_BIGBOOM_STEP_INIT: // step - init
				{
					// read data from file
					datafile = fopen(string,io_read);
					if(datafile)
					{
						// first line - vehicle data
						fread(datafile,line);
						// check
						if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
						// vehicle type
						tmp = strval(strtok(line,idx,','));
						// vehicle pos
						ftmp[0] = floatstr(strtok(line,idx,','));
						ftmp[1] = floatstr(strtok(line,idx,','));
						ftmp[2] = floatstr(strtok(line,idx,','));
						ftmp[3] = floatstr(strtok(line,idx,','));
						// create vehicle
						tmp = CreateVehicle(GetVehicleModelByType(tmp),ftmp[0],ftmp[1],ftmp[2],ftmp[3],-1,-1,999_999);
						SetPVarInt(playerid,"MissionVehicle",tmp);
						SetPlayerCheckpoint(playerid,ftmp[0],ftmp[1],ftmp[2],5.0);
						// set data
						SetPVarInt(playerid,"MissionTime",0);
						// close
						fclose(datafile);
					}
					else printf("[ERROR] mission actions file not found (%s)",string);
				}
				case MISSION_BIGBOOM_STEP_GT_TARGET: // step - show target
				{
					// read data from file
					datafile = fopen(string,io_read);
					if(datafile)
					{
						// first line - vehicle data
						fread(datafile,line);
						// second line - target data
						fread(datafile,line);
						// check
						if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
						ftmp[0] = floatstr(strtok(line,idx,','));
						ftmp[1] = floatstr(strtok(line,idx,','));
						ftmp[2] = floatstr(strtok(line,idx,','));
						// set data
						SetPlayerCheckpoint(playerid,ftmp[0],ftmp[1],ftmp[2],5.0);
						SendClientMessage(playerid,SYSTEM_COLOR,"Едте аккуратнее, большая вероятность взрыва");
						// close
						fclose(datafile);
					}
					else printf("[ERROR] mission actions file not found (%s)",string);
				}
				case MISSION_BIGBOOM_STEP_TIMER: // step - on target
				{
					// set data
					DisablePlayerCheckpoint(playerid);
					SetPVarInt(playerid,"MissionTime",10);
					SendClientMessage(playerid,SYSTEM_COLOR,"Беги как можно дальше!");
				}
				case MISSION_BIGBOOM_STEP_BOOM: // step - boom
				{
					// read data from file
					datafile = fopen(string,io_read);
					if(datafile)
					{
						// first line - vehicle data
						fread(datafile,line);
						// second line - target data
						fread(datafile,line);
						// other lines - explosion data
						while(fread(datafile,line))
						{
							// check
							if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
							idx = 0;
							ftmp[0] = floatstr(strtok(line,idx,','));
							ftmp[1] = floatstr(strtok(line,idx,','));
							ftmp[2] = floatstr(strtok(line,idx,','));
							// boom
							CreateExplosion(ftmp[0],ftmp[1],ftmp[2],1,50.0);
						}
						// close
						fclose(datafile);
						// mission completed!
						PlayerMissionComplete(playerid,MISSION_END_REASON_COMPLETE);
						return 1;
					}
					else printf("[ERROR] mission actions file not found (%s)",string);
				}
			}
		}
		case MISSION_DESANT:
		{
			/*
			mission - desant
			Спавнится самолёт, нужно сесть в него, получить там вооружение, долететь до нужного места, спрыгнуть с парашутом, взорвать дверь, украсть машину (в интерьере), привезти куды надо
			*/
			// find needed step
			switch(step)
			{
				case MISSION_DESANT_STEP_INIT: // step - init
				{
					// read data from file
					datafile = fopen(string,io_read);
					if(datafile)
					{
						// first line - plane data
						fread(datafile,line);
						// check
						if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
						// vehicle type
						tmp = strval(strtok(line,idx,','));
						// vehicle pos
						ftmp[0] = floatstr(strtok(line,idx,','));
						ftmp[1] = floatstr(strtok(line,idx,','));
						ftmp[2] = floatstr(strtok(line,idx,','));
						ftmp[3] = floatstr(strtok(line,idx,','));
						// create vehicle
						tmp = CreateVehicle(GetVehicleModelByType(tmp),ftmp[0],ftmp[1],ftmp[2],ftmp[3],-1,-1,999_999);
						SetPVarInt(playerid,"MissionVehicle",tmp);
						SetPlayerCheckpoint(playerid,ftmp[0],ftmp[1],ftmp[2],5.0);
						// set data
						SetPVarInt(playerid,"MissionTime",0);
						// close
						fclose(datafile);
					}
					else printf("[ERROR] mission actions file not found (%s)",string);
				}
				case MISSION_DESANT_STEP_GT_TARGET: // step - in plane
				{
					// read data from file
					datafile = fopen(string,io_read);
					if(datafile)
					{
						// first line - plane data
						fread(datafile,line);
						// second line - target data
						fread(datafile,line);
						// check
						if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
						ftmp[0] = floatstr(strtok(line,idx,','));
						ftmp[1] = floatstr(strtok(line,idx,','));
						ftmp[2] = floatstr(strtok(line,idx,','));
						// set data
						DisablePlayerCheckpoint(playerid);
						SetPlayerRaceCheckpoint(playerid,3,ftmp[0],ftmp[1],ftmp[2],0.0,0.0,0.0,20.0);
						SendClientMessage(playerid,SYSTEM_COLOR,"Лети к цели");
						// give weapons
						oGivePlayerWeapon(playerid,WEAPON_PARACHUTE,1);
						oGivePlayerWeapon(playerid,48,1); // glasses
						// close
						fclose(datafile);
					}
					else printf("[ERROR] mission actions file not found (%s)",string);
				}
				case MISSION_DESANT_STEP_JUMP: // step - on target
				{
					// read data from file
					datafile = fopen(string,io_read);
					if(datafile)
					{
						// first line - plane data
						fread(datafile,line);
						// second line - target data
						fread(datafile,line);
						// second line - jump target data
						fread(datafile,line);
						// check
						if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
						ftmp[0] = floatstr(strtok(line,idx,','));
						ftmp[1] = floatstr(strtok(line,idx,','));
						ftmp[2] = floatstr(strtok(line,idx,','));
						// set data
						DisablePlayerRaceCheckpoint(playerid);
						SetPlayerCheckpoint(playerid,ftmp[0],ftmp[1],ftmp[2],5.0);
						SendClientMessage(playerid,SYSTEM_COLOR,"Прыгай!");
						// close
						fclose(datafile);
					}
					else printf("[ERROR] mission actions file not found (%s)",string);
				}
				case MISSION_DESANT_STEP_TIMER: // step - set mine
				{
					DisablePlayerCheckpoint(playerid);
					SetPVarInt(playerid,"MissionTime",5);
					SendClientMessage(playerid,SYSTEM_COLOR,"Бомба на месте, отбегай!");
				}
				case MISSION_DESANT_STEP_BOOM: // step - boom
				{
					// read data from file
					datafile = fopen(string,io_read);
					if(datafile)
					{
						// first line - plane data
						fread(datafile,line);
						// second line - target data
						fread(datafile,line);
						// third line - jump target data
						fread(datafile,line);
						// check
						if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
						ftmp[0] = floatstr(strtok(line,idx,','));
						ftmp[1] = floatstr(strtok(line,idx,','));
						ftmp[2] = floatstr(strtok(line,idx,','));
						// set data
						CreateExplosion(ftmp[0],ftmp[1],ftmp[2],1,5.0);
						SetPlayerCheckpoint(playerid,ftmp[0],ftmp[1],ftmp[2],5.0);
						SendClientMessage(playerid,SYSTEM_COLOR,"Скорее внутрь");
						// destroy plane
						DestroyVehicle(GetPVarInt(playerid,"MissionVehicle"));
						// close
						fclose(datafile);
					}
					else printf("[ERROR] mission actions file not found (%s)",string);
				}
				case MISSION_DESANT_STEP_ENTER: // step - enter
				{
					// read data from file
					datafile = fopen(string,io_read);
					if(datafile)
					{
						// first line - plane data
						fread(datafile,line);
						// second line - target data
						fread(datafile,line);
						// third line - jump target data
						fread(datafile,line);
						// four line - in data
						fread(datafile,line);
						// check
						if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
						ftmp[0] = floatstr(strtok(line,idx,','));
						ftmp[1] = floatstr(strtok(line,idx,','));
						ftmp[2] = floatstr(strtok(line,idx,','));
						// set data
						SetPlayerPos(playerid,ftmp[0],ftmp[1],ftmp[2]);
						SetPlayerInterior(playerid,strval(strtok(line,idx,',')));
						SendClientMessage(playerid,SYSTEM_COLOR,"Забирай машину и сваливай");
						// five line - vehicle data
						fread(datafile,line);
						// check
						if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
						idx = 0;
						tmp = strval(strtok(line,idx,','));
						// vehicle pos
						ftmp[0] = floatstr(strtok(line,idx,','));
						ftmp[1] = floatstr(strtok(line,idx,','));
						ftmp[2] = floatstr(strtok(line,idx,','));
						ftmp[3] = floatstr(strtok(line,idx,','));
						// create vehicle
						tmp = CreateVehicle(GetVehicleModelByType(tmp),ftmp[0],ftmp[1],ftmp[2],ftmp[3],-1,-1,999_999);
						LinkVehicleToInterior(tmp,strval(strtok(line,idx,',')));
						SetPVarInt(playerid,"MissionVehicle",tmp);
						// close
						fclose(datafile);
					}
					else printf("[ERROR] mission actions file not found (%s)",string);
				}
				case MISSION_DESANT_STEP_IN_CAR: // step - in car
				{
					// read data from file
					datafile = fopen(string,io_read);
					if(datafile)
					{
						// first line - plane data
						fread(datafile,line);
						// second line - target data
						fread(datafile,line);
						// third line - jump target data
						fread(datafile,line);
						// four line - in data
						fread(datafile,line);
						// five line - vehicle data
						fread(datafile,line);
						// six line - out data
						fread(datafile,line);
						// check
						if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
						ftmp[0] = floatstr(strtok(line,idx,','));
						ftmp[1] = floatstr(strtok(line,idx,','));
						ftmp[2] = floatstr(strtok(line,idx,','));
						// set data
						SetVehiclePos(GetPVarInt(playerid,"MissionVehicle"),ftmp[0],ftmp[1],ftmp[2]);
						LinkVehicleToInterior(GetPVarInt(playerid,"MissionVehicle"),0);
						SetPlayerInterior(playerid,0);
						PutPlayerInVehicle(playerid,GetPVarInt(playerid,"MissionVehicle"),0);
						// seven line - target data
						fread(datafile,line);
						// check
						if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
						idx = 0;
						ftmp[0] = floatstr(strtok(line,idx,','));
						ftmp[1] = floatstr(strtok(line,idx,','));
						ftmp[2] = floatstr(strtok(line,idx,','));
						// set data
						SetPlayerCheckpoint(playerid,ftmp[0],ftmp[1],ftmp[2],5.0);
						SendClientMessage(playerid,SYSTEM_COLOR,"Быстро доставь машину");
						// close
						fclose(datafile);
					}
					else printf("[ERROR] mission actions file not found (%s)",string);
				}
				case MISSION_DESANT_STEP_END: // step - complete
				{
					PlayerMissionComplete(playerid,MISSION_END_REASON_COMPLETE);
					return 1;
				}
			}
		}
		case MISSION_LONGWAY:
		{
			/*
			mission - long way
			Спавнится самолёт, нужно его взять и лететь из штата кудато, долетев до чекпоинта мы появимся в другом городе, там нужно зайти в дом, забрать деньги, выйти из дома и появляешся в самолёте, нужно лететь назад в штаты
			*/
			// find needed step
			switch(step)
			{
				case MISSION_LONGWAY_STEP_INIT: // step - init
				{
					// read data from file
					datafile = fopen(string,io_read);
					if(datafile)
					{
						// first line - plane data
						fread(datafile,line);
						// check
						if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
						// plane type
						tmp = strval(strtok(line,idx,','));
						// plane pos
						ftmp[0] = floatstr(strtok(line,idx,','));
						ftmp[1] = floatstr(strtok(line,idx,','));
						ftmp[2] = floatstr(strtok(line,idx,','));
						ftmp[3] = floatstr(strtok(line,idx,','));
						// create plane
						tmp = CreateVehicle(GetVehicleModelByType(tmp),ftmp[0],ftmp[1],ftmp[2],ftmp[3],-1,-1,999_999);
						SetPlayerCheckpoint(playerid,ftmp[0],ftmp[1],ftmp[2],2.5);
						SetPVarInt(playerid,"MissionVehicle",tmp);
						// close
						fclose(datafile);
					}
					else printf("[ERROR] mission actions file not found (%s)",string);
				}
				case MISSION_LONGWAY_STEP_GT_TARGET: // step - in plane
				{
					// read data from file
					datafile = fopen(string,io_read);
					if(datafile)
					{
						// first line - plane data
						fread(datafile,line);
						// second line - target data
						fread(datafile,line);
						// check
						if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
						// target pos
						ftmp[0] = floatstr(strtok(line,idx,','));
						ftmp[1] = floatstr(strtok(line,idx,','));
						ftmp[2] = floatstr(strtok(line,idx,','));
						// create
						SetPlayerRaceCheckpoint(playerid,3,ftmp[0],ftmp[1],ftmp[2],0.0,0.0,0.0,20.0);
						DisablePlayerCheckpoint(playerid);
						// close
						fclose(datafile);
					}
					else printf("[ERROR] mission actions file not found (%s)",string);
				}
				case MISSION_LONGWAY_STEP_CITY: // step - in target
				{
					// read data from file
					datafile = fopen(string,io_read);
					if(datafile)
					{
						// first line - plane data
						fread(datafile,line);
						// second line - target data
						fread(datafile,line);
						// third line - in city data
						fread(datafile,line);
						// check
						if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
						// enter
						ftmp[0] = floatstr(strtok(line,idx,','));
						ftmp[1] = floatstr(strtok(line,idx,','));
						ftmp[2] = floatstr(strtok(line,idx,','));
						// apply
						SetPlayerPos(playerid,ftmp[0],ftmp[1],ftmp[2]);
						SetPlayerInterior(playerid,strval(strtok(line,idx,',')));
						DisablePlayerRaceCheckpoint(playerid);
						// four line - enter data
						fread(datafile,line);
						// check
						if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
						// data
						idx = 0;
						ftmp[0] = floatstr(strtok(line,idx,','));
						ftmp[1] = floatstr(strtok(line,idx,','));
						ftmp[2] = floatstr(strtok(line,idx,','));
						// apply
						SetPlayerCheckpoint(playerid,ftmp[0],ftmp[1],ftmp[2],2.5);
						// close
						fclose(datafile);
					}
					else printf("[ERROR] mission actions file not found (%s)",string);
				}
				case MISSION_LONGWAY_STEP_ENTER: // step - enter
				{
					// read data from file
					datafile = fopen(string,io_read);
					if(datafile)
					{
						// first line - plane data
						fread(datafile,line);
						// second line - target data
						fread(datafile,line);
						// third line - in city data
						fread(datafile,line);
						// four line - enter data
						fread(datafile,line);
						// five line - in build data
						fread(datafile,line);
						// check
						if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
						// data
						ftmp[0] = floatstr(strtok(line,idx,','));
						ftmp[1] = floatstr(strtok(line,idx,','));
						ftmp[2] = floatstr(strtok(line,idx,','));
						// apply
						SetPlayerPos(playerid,ftmp[0],ftmp[1],ftmp[2]);
						SetPlayerInterior(playerid,strval(strtok(line,idx,',')));
						DisablePlayerCheckpoint(playerid);
						// six line - mail data
						fread(datafile,line);
						// check
						if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
						// data
						idx = 0;
						ftmp[0] = floatstr(strtok(line,idx,','));
						ftmp[1] = floatstr(strtok(line,idx,','));
						ftmp[2] = floatstr(strtok(line,idx,','));
						// apply
						SetPlayerCheckpoint(playerid,ftmp[0],ftmp[1],ftmp[2],2.5);
						// close
						fclose(datafile);
					}
					else printf("[ERROR] mission actions file not found (%s)",string);
				}
				case MISSION_LONGWAY_STEP_TAKE_MAIL: // step - take mail
				{
					// read data from file
					datafile = fopen(string,io_read);
					if(datafile)
					{
						// first line - plane data
						fread(datafile,line);
						// second line - target data
						fread(datafile,line);
						// third line - in city data
						fread(datafile,line);
						// four line - enter data
						fread(datafile,line);
						// five line - in build data
						fread(datafile,line);
						// six line - mail data
						fread(datafile,line);
						// seven line - exit data
						fread(datafile,line);						
						// check
						if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
						// data
						ftmp[0] = floatstr(strtok(line,idx,','));
						ftmp[1] = floatstr(strtok(line,idx,','));
						ftmp[2] = floatstr(strtok(line,idx,','));
						// apply
						SetPlayerCheckpoint(playerid,ftmp[0],ftmp[1],ftmp[2],2.5);
						// close
						fclose(datafile);
					}
					else printf("[ERROR] mission actions file not found (%s)",string);
				}
				case MISSION_LONGWAY_STEP_EXIT: // step - exit
				{
					// read data from file
					datafile = fopen(string,io_read);
					if(datafile)
					{
						// first line - plane data
						fread(datafile,line);
						// second line - target data
						fread(datafile,line);
						// third line - in city data
						fread(datafile,line);
						// four line - enter data
						fread(datafile,line);
						// five line - in build data
						fread(datafile,line);
						// six line - mail data
						fread(datafile,line);
						// seven line - exit data
						fread(datafile,line);
						// eight line - out data
						fread(datafile,line);
						// check
						if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
						// data
						ftmp[0] = floatstr(strtok(line,idx,','));
						ftmp[1] = floatstr(strtok(line,idx,','));
						ftmp[2] = floatstr(strtok(line,idx,','));
						ftmp[3] = floatstr(strtok(line,idx,','));
						// apply
						SetPlayerInterior(playerid,0);
						SetVehiclePos(tmp,ftmp[0],ftmp[1],ftmp[2]);
						SetVehicleZAngle(tmp,ftmp[3]);
						PutPlayerInVehicle(playerid,tmp,0);
						// nine line - second target data
						fread(datafile,line);
						// check
						if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
						// data
						idx = 0;
						ftmp[0] = floatstr(strtok(line,idx,','));
						ftmp[1] = floatstr(strtok(line,idx,','));
						ftmp[2] = floatstr(strtok(line,idx,','));
						// apply
						SetPlayerCheckpoint(playerid,ftmp[0],ftmp[1],ftmp[2],2.5);
						// close
						fclose(datafile);
					}
					else printf("[ERROR] mission actions file not found (%s)",string);
				}
				case MISSION_LONGWAY_STEP_END: // step - complete
				{
					PlayerMissionComplete(playerid,MISSION_END_REASON_COMPLETE);
					return 1;
				}
			}
		}
		case MISSION_BANK:
		{
			/*
			mission - bank robbery
			нужно приехать к чекпоинту, зайти в здание, подойти к определённому месту, начнётся подбор пароля,после подбора будет 5 секунд чтобы ввести его иначе включится тревога и миссия ф попу
			*/
			// find needed step
			switch(step)
			{
				case MISSION_BANK_STEP_INIT: // step - init
				{
					// read data from file
					datafile = fopen(string,io_read);
					if(datafile)
					{
						// first line - enter data
						fread(datafile,line);
						// check
						if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
						ftmp[0] = floatstr(strtok(line,idx,','));
						ftmp[1] = floatstr(strtok(line,idx,','));
						ftmp[2] = floatstr(strtok(line,idx,','));
						// set checkpoint
						SetPlayerCheckpoint(playerid,ftmp[0],ftmp[1],ftmp[2],5.0);
						// close
						fclose(datafile);
					}
					else printf("[ERROR] mission actions file not found (%s)",string);
				}
				case MISSION_BANK_STEP_ENTER: // step - enter in bank
				{
					// read data from file
					datafile = fopen(string,io_read);
					if(datafile)
					{
						// first line - enter data
						fread(datafile,line);
						// second line - in data
						fread(datafile,line);
						// check
						if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
						// data
						ftmp[0] = floatstr(strtok(line,idx,','));
						ftmp[1] = floatstr(strtok(line,idx,','));
						ftmp[2] = floatstr(strtok(line,idx,','));
						// apply
						SetPlayerPos(playerid,ftmp[0],ftmp[1],ftmp[2]);
						SetPlayerInterior(playerid,strval(strtok(line,idx,',')));
						// third line - target data
						fread(datafile,line);
						// check
						if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
						// data
						idx = 0;
						ftmp[0] = floatstr(strtok(line,idx,','));
						ftmp[1] = floatstr(strtok(line,idx,','));
						ftmp[2] = floatstr(strtok(line,idx,','));
						// apply
						SetPlayerCheckpoint(playerid,ftmp[0],ftmp[1],ftmp[2],2.5);
						// close
						fclose(datafile);
					}
					else printf("[ERROR] mission actions file not found (%s)",string);
				}
				case MISSION_BANK_STEP_START_HACK: // step - in target
				{
					DisablePlayerCheckpoint(playerid);
					SetPVarInt(playerid,"MissionTime",30 + random(60)); // 30 seconds minimum , 90 - maximum
					SendClientMessage(playerid,SYSTEM_COLOR,"Начат подбор кода");
				}
				case MISSION_BANK_STEP_HACK: // step - hack
				{
					tmp = GetPVarInt(playerid,"MissionTime") - 1;
					if(tmp == 0) 
					{
						// generate code
						for(new i = 0;i < 10;i++) string[i] = (random(2) == 0?'A' + random('Z' - 'A'):'1' + random('9' - '1'));
						string[10] = 0;
						SetPVarString(playerid,"MissionCode",string);
						format(string,sizeof(string),"Код: %s , задета охранная система, если код не будет введён в течении 15 секунд - включится тревога",string);
						SendClientMessage(playerid,SYSTEM_COLOR,string);
						SetPVarInt(playerid,"MissionTime",15);
						ShowPlayerDialog(playerid,DIALOG_MISSION_BANK,DIALOG_STYLE_INPUT,"Охранная система","Введите код","Ок","Ок");
					}
					else
					{
						// generate code
						for(new i = 0;i < 10;i++) string[i] = (random(2) == 0?'A' + random('Z' - 'A'):'1' + random('9' - '1'));
						string[10] = 0;
						SendClientMessage(playerid,SYSTEM_COLOR,string);
						// tick
						SetPVarInt(playerid,"MissionTime",tmp);
						return 1;
					}
				}
				case MISSION_BANK_STEP_RUN: // step - go away
				{
					// read data from file
					datafile = fopen(string,io_read);
					if(datafile)
					{
						// first line - enter data
						fread(datafile,line);
						// second line - in data
						fread(datafile,line);
						// third line - target data
						fread(datafile,line);
						// four line - exit data
						fread(datafile,line);
						// check
						if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
						// data
						ftmp[0] = floatstr(strtok(line,idx,','));
						ftmp[1] = floatstr(strtok(line,idx,','));
						ftmp[2] = floatstr(strtok(line,idx,','));
						// apply
						SetPlayerCheckpoint(playerid,ftmp[0],ftmp[1],ftmp[2],2.5);
						// close
						fclose(datafile);
					}
					else printf("[ERROR] mission actions file not found (%s)",string);
				}
				case MISSION_BANK_STEP_EXIT: // step - exit
				{
					// read data from file
					datafile = fopen(string,io_read);
					if(datafile)
					{
						// first line - enter data
						fread(datafile,line);
						// second line - in data
						fread(datafile,line);
						// third line - target data
						fread(datafile,line);
						// four line - exit data
						fread(datafile,line);
						// five line - out data
						fread(datafile,line);
						// check
						if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
						// data
						ftmp[0] = floatstr(strtok(line,idx,','));
						ftmp[1] = floatstr(strtok(line,idx,','));
						ftmp[2] = floatstr(strtok(line,idx,','));
						// apply
						SetPlayerPos(playerid,ftmp[0],ftmp[1],ftmp[2]);
						SetPlayerInterior(playerid,0);
						// six line - target data
						fread(datafile,line);
						// check
						if(!CheckFileLine(line)) print("[ERROR] mission actions invalid file data");
						// data
						idx = 0;
						ftmp[0] = floatstr(strtok(line,idx,','));
						ftmp[1] = floatstr(strtok(line,idx,','));
						ftmp[2] = floatstr(strtok(line,idx,','));
						// apply
						SetPlayerCheckpoint(playerid,ftmp[0],ftmp[1],ftmp[2],2.5);
						// close
						fclose(datafile);
					}
					else printf("[ERROR] mission actions file not found (%s)",string);
				}				
				case MISSION_BANK_STEP_END: // step - complete
				{
					PlayerMissionComplete(playerid,MISSION_END_REASON_COMPLETE);
					return 1;
				}
			}
		}
	}
	SetPVarInt(playerid,"MissionStep",step);
	return 1;
}

stock PlayerMissionComplete(playerid,reason)
{
	static string[128];
	
	switch(GetPVarInt(playerid,"MissionType"))
	{
		case MISSION_WEAPON:
		{
			// delete pvars
			for(new i = 0;i < MAX_PICKUPS;i++)
			{
				format(string,sizeof(string),"MissionPickup_%d",i);
				if(!GetPVarInt(playerid,string)) continue;
				DeletePVar(playerid,string);
				DestroyPickup(i);
			}
			DeletePVar(playerid,"MissionPickups");
		}
		case MISSION_VEHICLE:
		{
			// delete pvars
			DestroyVehicle(GetPVarInt(playerid,"MissionVehicle"));
			DeletePVar(playerid,"MissionVehicle");
		}
		case MISSION_GONE60SEC:
		{
			// delete pvars
			for(new i = 0;i < MAX_VEHICLES;i++)
			{
				format(string,sizeof(string),"MissionVehicle_%d",i);
				if(!GetPVarInt(playerid,string)) continue;
				DeletePVar(playerid,string);
				DestroyVehicle(i);
				// marker
				format(string,sizeof(string),"MissionMarker_%d",i);
				DestroyMarker(GetPVarInt(playerid,string));
				DeletePVar(playerid,string);
			}
			DeletePVar(playerid,"MissionVehicles");
		}
		case MISSION_BOOM:
		{
			// delete pvars
			DestroyVehicle(GetPVarInt(playerid,"MissionVehicle"));
			DeletePVar(playerid,"MissionVehicle");
			DeletePVar(playerid,"MissionVehicleDinamites");
			DeletePVar(playerid,"MissionZoneMinX");
			DeletePVar(playerid,"MissionZoneMinY");
			DeletePVar(playerid,"MissionZoneMaxX");
			DeletePVar(playerid,"MissionZoneMaxY");
			GangZoneDestroy(GetPVarInt(playerid,"MissionZone"));			
			DeletePVar(playerid,"MissionZone");
			DeletePVar(playerid,"MissionDinamites");
			itmp[0] = GetPVarInt(playerid,"MissionDinamiteSet");
			for(new i = 0;i < itmp[0];i++)
			{
				format(string,sizeof(string),"MissionDinamite_%d",i);
				if(!GetPVarInt(playerid,string)) continue;
				DestroyStreamedObject(GetPVarInt(playerid,string));
				DeletePVar(playerid,string);
			}
			DeletePVar(playerid,"MissionDinamiteSet");
		}
		case MISSION_BOOMCASH:
		{
			// delete pvars
			DestroyStreamedObject(GetPVarInt(playerid,"MissionSafe"));
			DeletePVar(playerid,"MissionSafe");
		}
		case MISSION_BIGBOOM:
		{
			// delete pvars
			DestroyVehicle(GetPVarInt(playerid,"MissionVehicle"));
			DeletePVar(playerid,"MissionVehicle");
		}
		case MISSION_DESANT:
		{
			// delete pvars
			DestroyVehicle(GetPVarInt(playerid,"MissionVehicle"));
			DeletePVar(playerid,"MissionVehicle");
		}
		case MISSION_LONGWAY:
		{
			// delete pvars
			DestroyVehicle(GetPVarInt(playerid,"MissionVehicle"));
			DeletePVar(playerid,"MissionVehicle");
		}
		case MISSION_BANK:
		{
			// delete pvars
			DeletePVar(playerid,"MissionCode");
		}
	}
	// price by reason
	switch(reason)
	{
		case MISSION_END_REASON_COMPLETE:
		{
			// give price
			oGivePlayerMoney(playerid,Missions[ GetPVarInt(playerid,"MissionType") ][m_money]);
			GivePlayerXP(playerid,Missions[ GetPVarInt(playerid,"MissionType") ][m_xp]);
			format(stmp,sizeof(stmp),"BossRespect_%d",GetPVarInt(playerid,"MissionBoss"));
			SetPVarInt(playerid,stmp,(GetPVarInt(playerid,stmp) + Missions[ GetPVarInt(playerid,"MissionType") ][m_rep]));
			// msg
			GameTextForPlayer(playerid,"~g~Mission completed",3000,0);
		}
		case MISSION_END_REASON_FAILED:
		{
			// msg
			GameTextForPlayer(playerid,"~r~Mission failed",3000,0);
		}
	}
	// delete pvars
	DeletePVar(playerid,"MissionBoss");
	DeletePVar(playerid,"MissionType");
	DeletePVar(playerid,"MissionStage");
	DeletePVar(playerid,"MissionStep");
	// triggers
	DisablePlayerCheckpoint(playerid);
	DisablePlayerRaceCheckpoint(playerid);
	TogglePlayerCheckpointStreamer(playerid,1);
	SetPlayerQuest(playerid,INVALID_QUEST_ID);
}

// --------------------------------------------------
// forwards
// --------------------------------------------------
forward MissionsUpdate();
forward CutsceneNextStep(playerid);

// --------------------------------------------------
// publics
// --------------------------------------------------
public MissionsUpdate()
{
	for(new playerid = 0;playerid <= GetPlayersCount();playerid++)
	{
		if(!IsPlayerConnected(playerid)) continue;
		if(IsPlayerNPC(playerid)) continue;
		if(GetPlayerQuest(playerid) == INVALID_QUEST_ID) continue;
		// check quest
		itmp[0] = GetPVarInt(playerid,"MissionBoss");
		if(GetPlayerQuest(playerid) != BossQuest[ itmp[0] ]) continue;
		if(GetPVarInt(playerid,"MissionStage") != MISSION_STAGE_ACTION) continue; // mission not action
		
		// player in mission!
		switch(GetPVarInt(playerid,"MissionType"))
		{
			case MISSION_BOOMCASH:
			{
				switch(GetPVarInt(playerid,"MissionStep"))
				{
					case MISSION_BOOMCASH_STEP_PUT_DINAM:
					{
						itmp[0] = GetPVarInt(playerid,"MissionTime") - 1;
						SetPVarInt(playerid,"MissionTime",itmp[0]);
						if(itmp[0] == 0) ChangePlayerMissionStep(playerid,MISSION_BOOMCASH_STEP_BOOM);
					}
				}
			}
			case MISSION_BIGBOOM:
			{
				switch(GetPVarInt(playerid,"MissionStep"))
				{
					case MISSION_BIGBOOM_STEP_TIMER:
					{
						itmp[0] = GetPVarInt(playerid,"MissionTime") - 1;
						SetPVarInt(playerid,"MissionTime",itmp[0]);
						if(itmp[0] == 0) ChangePlayerMissionStep(playerid,MISSION_BIGBOOM_STEP_BOOM);
					}
				}
			}
			case MISSION_DESANT:
			{
				switch(GetPVarInt(playerid,"MissionStep"))
				{
					case MISSION_DESANT_STEP_TIMER:
					{
						itmp[0] = GetPVarInt(playerid,"MissionTime") - 1;
						SetPVarInt(playerid,"MissionTime",itmp[0]);
						if(itmp[0] == 0) ChangePlayerMissionStep(playerid,MISSION_DESANT_STEP_BOOM);
					}
				}
			}
			case MISSION_BANK:
			{
				switch(GetPVarInt(playerid,"MissionStep"))
				{
					case MISSION_BANK_STEP_START_HACK:
					{
						ChangePlayerMissionStep(playerid,MISSION_BANK_STEP_HACK);
					}
					case MISSION_BANK_STEP_HACK:
					{
						itmp[0] = GetPVarInt(playerid,"MissionTime") - 1;
						SetPVarInt(playerid,"MissionTime",itmp[0]);
						if(itmp[0] == 0) PlayerMissionComplete(playerid,MISSION_END_REASON_FAILED);
					}
				}
			}
		}
	}
}

public CutsceneNextStep(playerid)
{
	itmp[0] = GetPVarInt(playerid,"MissionStep") + 1;
	SetPVarInt(playerid,"MissionStep",itmp[0]);
	itmp[0] = ShowCutscene(playerid,GetPVarInt(playerid,"MissionBoss"),GetPVarInt(playerid,"MissionType"),GetPVarInt(playerid,"MissionRandomize"),itmp[0]);
	if(itmp[0] != -1) SetTimerEx("CutsceneNextStep",itmp[0],0,"d",playerid);
}

// --------------------------------------------------
// Obligatory functions
// --------------------------------------------------
Missions_OnGameModeInit()
{
	// null
	for(new i = 0;i < MAX_STREAMED_CHECKPOINTS;i++) CheckpointsBossLink{i} = INVALID_BOSS_ID;
	// load bosses
	new File:datafile = fopen(MODE_DIR MISSIONS_DIR BOSSES_FILE,io_read);
	if(datafile)
	{
		itmp[0] = 0;
		while(fread(datafile,stmp))
		{
			// check
			if(!CheckFileLine(stmp)) continue;
			static idx;
			idx = 0;
			// mask: Имя босса,иконка,координата X,координата Y,координата Z,1,10,50
			// get name
			format(BossName[ itmp[0] ],MAX_BOSS_NAME,strtok(stmp,idx,','));
			// icon type
			itmp[1] = strval(strtok(stmp,idx,','));
			// get coords
			ftmp[0] = floatstr(strtok(stmp,idx,','));
			ftmp[1] = floatstr(strtok(stmp,idx,','));
			ftmp[2] = floatstr(strtok(stmp,idx,','));
			// create icon			
			CreateStreamedIcon(itmp[1],0,ftmp[0],ftmp[1],ftmp[2]);
			// get missions info
			BossMissionsTypesCount{ itmp[0] } = strval(strtok(stmp,idx,','));
			// get types
			for(new i = 0;i < BossMissionsTypesCount{ itmp[0] };i++) BossMissionsTypes[ itmp[0] ]{i} = strval(strtok(stmp,idx,','));
			// get respect
			for(new i = 0;i < BossMissionsTypesCount{ itmp[0] };i++) BossMissionsRespect[ itmp[0] ]{i} = strval(strtok(stmp,idx,','));
			// create checkpoint
			itmp[1] = CreateStreamedCheckpoint(ftmp[0],ftmp[1],ftmp[2],5.0);
			CheckpointsBossLink{ itmp[1] } = itmp[0];
			BossCheckpointId[ itmp[0] ] = itmp[1];
			// reg quest
			BossQuest[ itmp[0] ] = RegisterQuest();
			// next boss id
			itmp[0]++;
		}
		fclose(datafile);
#if defined PRINT_STATS_DATA
		printf("Bosses count: %d",itmp[0]);
#endif
	}
	else print("[ERROR] missions bosses file not found");
	// create textdraws
	Letterbox[0] = TextDrawCreate(0.0,0.0,"_");
	TextDrawUseBox(Letterbox[0],1);
	TextDrawBoxColor(Letterbox[0],0x000000ff);
	TextDrawLetterSize(Letterbox[0],0.0,12.2);	
	Letterbox[1] = TextDrawCreate(0.0,480.0,"_");
	TextDrawUseBox(Letterbox[1],1);
	TextDrawBoxColor(Letterbox[1],0x000000ff);
	TextDrawLetterSize(Letterbox[1],0.0,-15.0);
	for(new i = 0;i < sizeof(Blackscreen);i++)
	{
		Blackscreen[i] = TextDrawCreate(0.0,0.0,"_");
		TextDrawUseBox(Blackscreen[i],1);
		TextDrawLetterSize(Blackscreen[i],0.0,100.0);	
	}
	TextDrawBoxColor(Blackscreen[0],0x00000011);
	TextDrawBoxColor(Blackscreen[1],0x00000022);
	TextDrawBoxColor(Blackscreen[2],0x00000033);
	TextDrawBoxColor(Blackscreen[3],0x00000044);
	TextDrawBoxColor(Blackscreen[4],0x00000055);
	TextDrawBoxColor(Blackscreen[5],0x00000066);
	TextDrawBoxColor(Blackscreen[6],0x00000077);
	TextDrawBoxColor(Blackscreen[7],0x00000088);
	TextDrawBoxColor(Blackscreen[8],0x00000099);
	TextDrawBoxColor(Blackscreen[9],0x000000AA);
	TextDrawBoxColor(Blackscreen[10],0x000000BB);
	TextDrawBoxColor(Blackscreen[11],0x000000CC);
	TextDrawBoxColor(Blackscreen[12],0x000000DD);
	TextDrawBoxColor(Blackscreen[13],0x000000EE);
	TextDrawBoxColor(Blackscreen[14],0x000000FF);
	// timer
	SetTimer("MissionsUpdate",1000,1);
	// print main stats
#if defined PRINT_STATS_DATA
	printf("Max bosses count: %d",MAX_BOSSES);
	printf("Missions types count: %d",MAX_MISSION_TYPES);
#endif
	print("Missions loaded.");
}

Missions_OnPlayerDisconnect(playerid,reason)
{
	#pragma unused reason
	
	if(GetPlayerQuest(playerid) == INVALID_QUEST_ID) return; // player not in quest
	
	itmp[0] = GetPVarInt(playerid,"MissionBoss");
	if(GetPlayerQuest(playerid) != BossQuest[ itmp[0] ]) return; // player not in mission
	
	PlayerMissionComplete(playerid,MISSION_END_REASON_FAILED);
}

Missions_OnPlayerDeath(playerid,killerid,reason)
{
	#pragma unused killerid
	#pragma unused reason
	
	if(GetPlayerQuest(playerid) == INVALID_QUEST_ID) return; // player not in quest
	
	itmp[0] = GetPVarInt(playerid,"MissionBoss");
	if(GetPlayerQuest(playerid) != BossQuest[ itmp[0] ]) return; // player not in mission
	if(GetPVarInt(playerid,"MissionStage") != MISSION_STAGE_ACTION) return; // mission not action
	
	PlayerMissionComplete(playerid,MISSION_END_REASON_FAILED);
}

Missions_OPESC(playerid,checkpointid) // OnPlayerEnterStreamedCheckpoint
{
	if(CheckpointsBossLink{checkpointid} == INVALID_BOSS_ID) return 0; // not mission checkpoint
	
	if(GetPlayerQuest(playerid) != INVALID_QUEST_ID) return 0; // player in quest
	static types[MAX_MISSION_TYPES char];
	// null
	for(new i = 0;i < MAX_MISSION_TYPES;i++) types{i} = -1;
	// get mission types by respect
	itmp[0] = CheckpointsBossLink{checkpointid};
	itmp[1] = 0;
	for(new i = 0;i < BossMissionsTypesCount{ itmp[0] };i++)
	{
		format(stmp,sizeof(stmp),"BossRespect_%d",itmp[0]);
		if(GetPVarInt(playerid,stmp) < BossMissionsRespect[ itmp[0] ]{i}) continue;
		types{ itmp[1] } = BossMissionsTypes[ itmp[0] ]{i};
		itmp[1]++;
	}
	// check small resp
	if(itmp[1] == 0) return SendClientMessage(playerid,MISSIONS_COLOR,"Охранник: вали отсюда пока жопу не надрали");
	// randomize mission
	itmp[1] = random(itmp[1]);
	if(types{ itmp[1] } == -1) return print("[ERROR] invalid mission type");
	// set data
	TogglePlayerCheckpointStreamer(playerid,0);
	TogglePlayerControllable(playerid,false);
	SetPlayerQuest(playerid,BossQuest[ itmp[0] ]);
	SetPVarInt(playerid,"MissionBoss",itmp[0]);
	SetPVarInt(playerid,"MissionType",types{ itmp[1] });
	SetPVarInt(playerid,"MissionStage",MISSION_STAGE_CUTSCENE);
	SetPVarInt(playerid,"MissionStep",0);
	// get random cutscene
	format(stmp,sizeof(stmp),MODE_DIR MISSIONS_DIR BOSS_MASK_DIR CUTSCENES_FILE,itmp[0]);
	new File:datafile = fopen(stmp,io_read);
	itmp[2] = types{ itmp[1] };
	if(datafile)
	{
		for(new i = 0;i <= itmp[2];i++) fread(datafile,stmp);
		// check
		if(!CheckFileLine(stmp)) print("[ERROR] mission cutscenes invalid file data");
		// randomize
		SetPVarInt(playerid,"MissionRandomize",random(strval(stmp)));
		// file end
		fclose(datafile);
	}
	else print("[ERROR] missions cutscenes file not found");
	// save data
	GetPlayerPos(playerid,PlayerOldPos[playerid][0],PlayerOldPos[playerid][1],PlayerOldPos[playerid][2]);
	GetPlayerFacingAngle(playerid,PlayerOldPos[playerid][3]);
	PlayerOldInterior{playerid} = GetPlayerInterior(playerid);
	PlayerOldSkin[playerid] = GetPlayerSkin(playerid);
	// start show cutscene
	itmp[2] = ShowCutscene(playerid,itmp[0],types{ itmp[1] },GetPVarInt(playerid,"MissionRandomize"),0);
	if(itmp[2] != -1) SetTimerEx("CutsceneNextStep",itmp[2],0,"d",playerid);
	return 1;
}

Missions_OPEC(playerid) // OnPlayerEnterCheckpoint
{
	if(GetPlayerQuest(playerid) == INVALID_QUEST_ID) return; // player not in quest
	
	itmp[0] = GetPVarInt(playerid,"MissionBoss");
	if(GetPlayerQuest(playerid) != BossQuest[ itmp[0] ]) return; // player not in mission
	if(GetPVarInt(playerid,"MissionStage") != MISSION_STAGE_ACTION) return; // mission not action
	
	switch(GetPVarInt(playerid,"MissionType"))
	{
		case MISSION_MAIL: ChangePlayerMissionStep(playerid,MISSION_MAIL_STEP_END);
		case MISSION_WEAPON: 
		{
			switch(GetPVarInt(playerid,"MissionStep"))
			{
				case MISSION_WEAPON_STEP_GT_TARGET: ChangePlayerMissionStep(playerid,MISSION_WEAPON_STEP_END);
			}
		}
		case MISSION_VEHICLE: 
		{
			switch(GetPVarInt(playerid,"MissionStep"))
			{
				case MISSION_VEHICLE_STEP_GT_TARGET: ChangePlayerMissionStep(playerid,MISSION_VEHICLE_STEP_END);
			}
		}
		case MISSION_GONE60SEC: 
		{
			switch(GetPVarInt(playerid,"MissionStep"))
			{
				case MISSION_GONE60SEC_STEP_GT_TARG: ChangePlayerMissionStep(playerid,MISSION_GONE60SEC_STEP_DELIVED);
			}
		}
		case MISSION_BOOMCASH:
		{
			switch(GetPVarInt(playerid,"MissionStep"))
			{
				case MISSION_BOOMCASH_STEP_INIT: ChangePlayerMissionStep(playerid,MISSION_BOOMCASH_STEP_PUT_DINAM);
				case MISSION_BOOMCASH_STEP_BOOM: ChangePlayerMissionStep(playerid,MISSION_BOOMCASH_STEP_TAKE_MAIL);
				case MISSION_BOOMCASH_STEP_TAKE_MAIL: ChangePlayerMissionStep(playerid,MISSION_BOOMCASH_STEP_END);
			}
		}
		case MISSION_BIGBOOM: ChangePlayerMissionStep(playerid,MISSION_BIGBOOM_STEP_TIMER);
		case MISSION_DESANT: 
		{
			switch(GetPVarInt(playerid,"MissionStep"))
			{
				case MISSION_DESANT_STEP_GT_TARGET: ChangePlayerMissionStep(playerid,MISSION_DESANT_STEP_JUMP);
				case MISSION_DESANT_STEP_JUMP: ChangePlayerMissionStep(playerid,MISSION_DESANT_STEP_TIMER);
				case MISSION_DESANT_STEP_BOOM: ChangePlayerMissionStep(playerid,MISSION_DESANT_STEP_ENTER);
				case MISSION_DESANT_STEP_IN_CAR: ChangePlayerMissionStep(playerid,MISSION_DESANT_STEP_END);
			}
		}
		case MISSION_LONGWAY: 
		{
			switch(GetPVarInt(playerid,"MissionStep"))
			{
				case MISSION_LONGWAY_STEP_CITY: ChangePlayerMissionStep(playerid,MISSION_LONGWAY_STEP_ENTER);
				case MISSION_LONGWAY_STEP_ENTER: ChangePlayerMissionStep(playerid,MISSION_LONGWAY_STEP_TAKE_MAIL);
				case MISSION_LONGWAY_STEP_TAKE_MAIL: ChangePlayerMissionStep(playerid,MISSION_LONGWAY_STEP_EXIT);
				case MISSION_LONGWAY_STEP_EXIT: ChangePlayerMissionStep(playerid,MISSION_LONGWAY_STEP_END);
			}
		}
		case MISSION_BANK: 
		{
			switch(GetPVarInt(playerid,"MissionStep"))
			{
				case MISSION_BANK_STEP_INIT: ChangePlayerMissionStep(playerid,MISSION_BANK_STEP_ENTER);
				case MISSION_BANK_STEP_ENTER: ChangePlayerMissionStep(playerid,MISSION_BANK_STEP_START_HACK);
				case MISSION_BANK_STEP_HACK: ChangePlayerMissionStep(playerid,MISSION_BANK_STEP_RUN);
				case MISSION_BANK_STEP_RUN: ChangePlayerMissionStep(playerid,MISSION_BANK_STEP_EXIT);
				case MISSION_BANK_STEP_EXIT: ChangePlayerMissionStep(playerid,MISSION_BANK_STEP_END);
			}
		}
	}
}

Missions_OPERC(playerid) // OnPlayerEnterRaceCheckpoint
{
	if(GetPlayerQuest(playerid) == INVALID_QUEST_ID) return; // player not in quest
	
	itmp[0] = GetPVarInt(playerid,"MissionBoss");
	if(GetPlayerQuest(playerid) != BossQuest[ itmp[0] ]) return; // player not in mission
	if(GetPVarInt(playerid,"MissionStage") != MISSION_STAGE_ACTION) return; // mission not action
	
	switch(GetPVarInt(playerid,"MissionType"))
	{
		case MISSION_DESANT: 
		{
			switch(GetPVarInt(playerid,"MissionStep"))
			{
				case MISSION_DESANT_STEP_GT_TARGET: ChangePlayerMissionStep(playerid,MISSION_DESANT_STEP_JUMP);
			}
		}
		case MISSION_LONGWAY: 
		{
			switch(GetPVarInt(playerid,"MissionStep"))
			{
				case MISSION_LONGWAY_STEP_GT_TARGET: ChangePlayerMissionStep(playerid,MISSION_LONGWAY_STEP_CITY);
			}
		}
	}
}

Missions_OnPlayerEnterVehicle(playerid,vehicleid,ispassenger)
{
	#pragma unused ispassenger
	
	if(GetPlayerQuest(playerid) == INVALID_QUEST_ID) return; // player not in quest
	
	itmp[0] = GetPVarInt(playerid,"MissionBoss");
	if(GetPlayerQuest(playerid) != BossQuest[ itmp[0] ]) return; // player not in mission
	if(GetPVarInt(playerid,"MissionStage") != MISSION_STAGE_ACTION) return; // mission not action
	
	switch(GetPVarInt(playerid,"MissionType"))
	{
		case MISSION_VEHICLE: 
		{
			if(GetPVarInt(playerid,"MissionStep") == MISSION_VEHICLE_STEP_INIT)
			{
				// check vehicle
				if(vehicleid == GetPVarInt(playerid,"MissionVehicle")) ChangePlayerMissionStep(playerid,MISSION_VEHICLE_STEP_GT_TARGET);
			}
		}
		case MISSION_GONE60SEC:
		{
			if(GetPVarInt(playerid,"MissionStep") == MISSION_GONE60SEC_STEP_INIT)
			{
				// check vehicle
				format(stmp,sizeof(stmp),"MissionVehicle_%d",vehicleid);
				if(GetPVarInt(playerid,stmp) == 1) ChangePlayerMissionStep(playerid,MISSION_GONE60SEC_STEP_GT_TARG);
			}
		}
		case MISSION_BOOM:
		{
			if(GetPVarInt(playerid,"MissionStep") == MISSION_BOOM_STEP_INIT)
			{
				// check vehicle
				if(vehicleid == GetPVarInt(playerid,"MissionVehicle")) DisablePlayerCheckpoint(playerid);
			}
		}
		case MISSION_BIGBOOM:
		{
			if(GetPVarInt(playerid,"MissionStep") == MISSION_BIGBOOM_STEP_INIT)
			{
				// check vehicle
				if(vehicleid == GetPVarInt(playerid,"MissionVehicle")) ChangePlayerMissionStep(playerid,MISSION_BIGBOOM_STEP_GT_TARGET);
			}
		}
		case MISSION_DESANT:
		{
			switch(GetPVarInt(playerid,"MissionStep"))
			{
				case MISSION_DESANT_STEP_INIT:
				{
					// check vehicle
					if(vehicleid == GetPVarInt(playerid,"MissionVehicle")) ChangePlayerMissionStep(playerid,MISSION_DESANT_STEP_GT_TARGET);
				}
				case MISSION_DESANT_STEP_ENTER:
				{
					// check vehicle
					if(vehicleid == GetPVarInt(playerid,"MissionVehicle")) ChangePlayerMissionStep(playerid,MISSION_DESANT_STEP_IN_CAR);
				}
			}
		}
		case MISSION_LONGWAY:
		{
			if(GetPVarInt(playerid,"MissionStep") == MISSION_LONGWAY_STEP_INIT)
			{
				// check vehicle
				if(vehicleid == GetPVarInt(playerid,"MissionVehicle")) ChangePlayerMissionStep(playerid,MISSION_LONGWAY_STEP_GT_TARGET);
			}
		}
	}
}

Missions_OnPlayerPickUpPickup(playerid,pickupid)
{
	if(GetPlayerQuest(playerid) == INVALID_QUEST_ID) return; // player not in quest
	
	itmp[0] = GetPVarInt(playerid,"MissionBoss");
	if(GetPlayerQuest(playerid) != BossQuest[ itmp[0] ]) return; // player not in mission
	if(GetPVarInt(playerid,"MissionStage") != MISSION_STAGE_ACTION) return; // mission not action
	
	switch(GetPVarInt(playerid,"MissionType"))
	{
		case MISSION_WEAPON: 
		{
			if(GetPVarInt(playerid,"MissionStep") == MISSION_WEAPON_STEP_INIT)
			{
				// check pickup
				format(stmp,sizeof(stmp),"MissionPickup_%d",pickupid);
				if(GetPVarInt(playerid,stmp))
				{
					DestroyPickup(pickupid);
					itmp[0] = GetPVarInt(playerid,"MissionPickups") - 1;
					SetPVarInt(playerid,"MissionPickups",itmp[0]);
					if(itmp[0] == 0) ChangePlayerMissionStep(playerid,MISSION_WEAPON_STEP_GT_TARGET);
				}
			}
		}
	}
}

Missions_Command(playerid,cmdid,params[])
{
	#pragma unused params
	switch(cmdid)
	{
		case MISSIONS_DSET_CMD:
		{
			itmp[0] = GetPVarInt(playerid,"MissionBoss");
			if(GetPlayerQuest(playerid) != BossQuest[ itmp[0] ]) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы не на задании подрывника");
			if(GetPVarInt(playerid,"MissionStage") != MISSION_STAGE_ACTION) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы не на задании подрывника");
			if(GetPVarInt(playerid,"MissionType") != MISSION_BOOM) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы не на задании подрывника");
			
			ChangePlayerMissionStep(playerid,MISSION_BOOM_STEP_PUT_DINAMITE);
			return 1;
		}
		case MISSIONS_DGET_CMD:
		{
			itmp[0] = GetPVarInt(playerid,"MissionBoss");
			if(GetPlayerQuest(playerid) != BossQuest[ itmp[0] ]) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы не на задании подрывника");
			if(GetPVarInt(playerid,"MissionStage") != MISSION_STAGE_ACTION) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы не на задании подрывника");
			if(GetPVarInt(playerid,"MissionType") != MISSION_BOOM) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы не на задании подрывника");
			
			ChangePlayerMissionStep(playerid,MISSION_BOOM_STEP_TAKE_DINAMITE);
			return 1;
		}
		case MISSIONS_DBOOM_CMD:
		{
			itmp[0] = GetPVarInt(playerid,"MissionBoss");
			if(GetPlayerQuest(playerid) != BossQuest[ itmp[0] ]) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы не на задании подрывника");
			if(GetPVarInt(playerid,"MissionStage") != MISSION_STAGE_ACTION) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы не на задании подрывника");
			if(GetPVarInt(playerid,"MissionType") != MISSION_BOOM) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы не на задании подрывника");
			
			ChangePlayerMissionStep(playerid,MISSION_BOOM_STEP_BOOM);
			return 1;
		}
	}
	return 0;
}

Missions_Dialog(playerid,dialogid,response,listitem,inputtext[])
{
	#pragma unused listitem
	#pragma unused response
	
	switch(dialogid)
	{
		case DIALOG_MISSION_BANK:
		{
			itmp[0] = GetPVarInt(playerid,"MissionBoss");
			if(GetPlayerQuest(playerid) != BossQuest[ itmp[0] ]) return 1;
			if(GetPVarInt(playerid,"MissionStage") != MISSION_STAGE_ACTION) return 1;
			
			GetPVarString(playerid,"MissionCode",stmp,sizeof(stmp));
			if(!strcmp(stmp,inputtext)) ChangePlayerMissionStep(playerid,MISSION_BANK_STEP_RUN);
			return 1;
		}
	}
	return 0;
}

Missions_OPVHC(playerid,vehicleid,Float:old_health,Float:new_health) // OnPlayerVehicleHealthChange
{
	// check mission
	if(GetPlayerQuest(playerid) == INVALID_QUEST_ID) return;
	
	itmp[0] = GetPVarInt(playerid,"MissionBoss");
	if(GetPlayerQuest(playerid) != BossQuest[ itmp[0] ]) return;
	if(GetPVarInt(playerid,"MissionStage") != MISSION_STAGE_ACTION) return;
	if(GetPVarInt(playerid,"MissionType") != MISSION_BIGBOOM) return;
	if(GetPVarInt(playerid,"MissionVehicle") != vehicleid) return;
	if(old_health < new_health) return;
	if((old_health - new_health) < MAX_BIGBOOM_VEHICLE_DAMAGE) return;
	
	GetPlayerPos(playerid,ftmp[0],ftmp[1],ftmp[2]);
	CreateExplosion(ftmp[0],ftmp[1],ftmp[2],1,50.0);
	SetPlayerHealth(playerid,0.0); // DIE MOTHEFUCKER!
}