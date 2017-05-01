/*
*	Created:			21.04.10
*	Author:				009
*	Description:		Регистрация, авторизация, уровни, прокачка
*	Info:				Взято из Undead Attack 0.1 beta 4, изменено под WoG
*/

#if defined _player_included
	#endinput
#endif

#define _player_included
#pragma library player

// --------------------------------------------------
// defines
// --------------------------------------------------
#define PLAYERS_DIR				"players/"
#define PLAYERS_FILE_TYPE		".ini"
#define START_HELP_FILE			"start_help.txt"
#define START_HELP_COLOR		0xFFFFFFAA
#define REGENERATION_TIME		5
#define TIME_FOR_SAVE			1800_000
#define HELP_TIME				10_000
#define XP_RATE					40
#define MAX_LEVEL				500
// --------------------------------------------------
// enums
// --------------------------------------------------
enum
{
	PLAYER_STAT_CMD,
	PLAYER_HELP_CMD,
	PLAYER_SAVECHAR_CMD,
	PLAYER_CHANGEPASS_CMD,
	PLAYER_KILL_CMD,
	PLAYER_CREDITS_CMD,
	PLAYER_GIVECASH_CMD,
	PLAYER_FLIP_CMD,
	PLAYER_UPGRADE_CMD,
	PLAYER_PM_CMD
};
enum
{
	CHANGE_STAT_ADD,
	CHANGE_STAT_SET
};
enum
{
	SAVE_REASON_SAVECHAR,
	SAVE_REASON_AUTOSAVE,
	SAVE_REASON_DISCONNECT
};
enum
{
	STARTHELP_ACTION_TITLE,
	STARTHELP_ACTION_TEXT,
	STARTHELP_ACTION_INTERIOR,
	STARTHELP_ACTION_CAMERA_POS
};
// --------------------------------------------------
// news
// --------------------------------------------------
new 
	IsPlayerSpawned[MAX_PLAYERS char];
// --------------------------------------------------
// forwards
// --------------------------------------------------
forward Autosave();
forward StartHelpNextStep(playerid);
// global
forward OnSavePlayerData(playerid,File:datafile,reason);
forward OnLoadPlayerData(playerid,datastr[],separatorpos);

// --------------------------------------------------
// publics
// --------------------------------------------------
public Autosave()
{
	// autosave
	for(new i = 0;i <= GetPlayersCount();i++)
	{
		if(!IsPlayerConnected(i)) continue;
		if(IsPlayerNPC(i)) continue;
		if(GetPVarInt(i,"Logged") != 1) continue;
		
		// save
		SavePlayer(i,SAVE_REASON_AUTOSAVE);
	}
}

public StartHelpNextStep(playerid)
{
	itmp[0] = GetPVarInt(playerid,"StartHelpStep") + 1;
	SetPVarInt(playerid,"StartHelpStep",itmp[0]);
	itmp[0] = ChangeStartHelpStep(playerid,itmp[0]);
	if(itmp[0] != -1) SetTimerEx("StartHelpNextStep",itmp[0],0,"d",playerid);
}

// --------------------------------------------------
// stocks
// --------------------------------------------------
stock SavePlayer(playerid,reason)
{
	if(GetPVarInt(playerid,"Logged") != 1) return;
	
	new name[MAX_PLAYER_NAME];
	GetPVarString(playerid,"Account",name,sizeof(name));
	
	format(stmp,sizeof(stmp),MODE_DIR PLAYERS_DIR "%s" PLAYERS_FILE_TYPE,name);
	
	new File:pfile = fopen(stmp,io_write);
	
	// player core stats
	GetPVarString(playerid,"Password",stmp,sizeof(stmp));
	format(stmp,sizeof(stmp),"Password=%s\r\n",stmp);
	fwrite(pfile,stmp);
	format(stmp,sizeof(stmp),"Level=%d\r\n",GetPVarInt(playerid,"Level"));
	fwrite(pfile,stmp);
	format(stmp,sizeof(stmp),"Kills=%d\r\n",GetPVarInt(playerid,"Kills"));
	fwrite(pfile,stmp);
	format(stmp,sizeof(stmp),"Deaths=%d\r\n",GetPVarInt(playerid,"Deaths"));
	fwrite(pfile,stmp);
	format(stmp,sizeof(stmp),"Money=%d\r\n",oGetPlayerMoney(playerid));
	fwrite(pfile,stmp);
	format(stmp,sizeof(stmp),"XP=%d\r\n",GetPVarInt(playerid,"XP"));
	fwrite(pfile,stmp);
	format(stmp,sizeof(stmp),"UpgradesPoints=%d\r\n",GetPVarInt(playerid,"UpgradesPoints"));
	fwrite(pfile,stmp);
	format(stmp,sizeof(stmp),"Regeneration=%.1f\r\n",GetPVarFloat(playerid,"Regeneration"));
	fwrite(pfile,stmp);
	format(stmp,sizeof(stmp),"MaxHealth=%.1f\r\n",GetPVarFloat(playerid,"MaxHealth"));
	fwrite(pfile,stmp);
	
	// other systems
	OnSavePlayerData(playerid,pfile,reason);
	
	fclose(pfile);
}

stock LoadPlayer(playerid,pass[])
{
	new name[MAX_PLAYER_NAME];
	GetPVarString(playerid,"Account",name,sizeof(name));
	
	format(stmp,sizeof(stmp),MODE_DIR PLAYERS_DIR "%s" PLAYERS_FILE_TYPE,name);
	
	new File:pfile = fopen(stmp,io_read);
	if(!pfile) return 0;
	
	while(fread(pfile,stmp))
	{
		if(!CheckFileLine(stmp)) continue;
		
		if((itmp[0] = strfind(stmp,"=")) == -1) continue;
		
		// player core stats
		if(!strcmp(stmp,"Password",true,itmp[0]))
		{
			if(!strcmp(stmp[itmp[0] + 1],pass,true))
			{
				SetPVarInt(playerid,"Logged",1);
				SetPVarString(playerid,"Password",pass);
				continue;
			}
			else return 0;
		}
		if(!strcmp(stmp,"Level",true,itmp[0])) 
		{					
			SetPVarInt(playerid,"Level",strval(stmp[itmp[0] + 1]));
			continue;
		}
		if(!strcmp(stmp,"Kills",true,itmp[0])) 
		{
			SetPVarInt(playerid,"Kills",strval(stmp[itmp[0] + 1]));
			continue;
		}
		if(!strcmp(stmp,"Deaths",true,itmp[0])) 
		{
			SetPVarInt(playerid,"Deaths",strval(stmp[itmp[0] + 1]));
			continue;
		}
		if(!strcmp(stmp,"Money",true,itmp[0])) 
		{
			oSetPlayerMoney(playerid,strval(stmp[itmp[0] + 1]));
			continue;
		}
		if(!strcmp(stmp,"XP",true,itmp[0])) 
		{
			SetPVarInt(playerid,"XP",strval(stmp[itmp[0] + 1]));
			continue;
		}
		if(!strcmp(stmp,"UpgradesPoints",true,itmp[0])) 
		{
			SetPVarInt(playerid,"UpgradesPoints",strval(stmp[itmp[0] + 1]));
			continue;
		}
		if(!strcmp(stmp,"Regeneration",true,itmp[0]))
		{
			SetPVarFloat(playerid,"Regeneration",floatstr(stmp[itmp[0] + 1]));
			continue;
		}
		if(!strcmp(stmp,"MaxHealth",true,itmp[0])) 
		{
			SetPVarFloat(playerid,"MaxHealth",floatstr(stmp[itmp[0] + 1]));
			continue;
		}
		// other systems
		OnLoadPlayerData(playerid,stmp,itmp[0]);
	}
	
	fclose(pfile);
	return 1;
}

stock ChangeStatByNameInt(name[],stat[],data,type)
{
	static 
		ntmp[MAX_PLAYER_NAME],
		pid;
	pid = INVALID_PLAYER_ID;
	for(new i = 0;i <= GetPlayersCount();i++)
	{
		GetPlayerName(i,ntmp,sizeof(ntmp));
		if(!strcmp(ntmp,name))
		{
			pid = i;
			break;
		}
	}
	if(pid != INVALID_PLAYER_ID)
	{
		switch(type)
		{
			case CHANGE_STAT_ADD:
			{
				SetPVarInt(pid,stat,GetPVarInt(pid,stat) + data);
				if(!strcmp(stat,"Money")) GivePlayerMoney(pid,data);
				else if(!strcmp(stat,"XP"))
				{
					GivePlayerXP(pid,data);
				}
			}
			case CHANGE_STAT_SET: 
			{
				SetPVarInt(pid,stat,data);
				if(!strcmp(stat,"Money")) 
				{
					oSetPlayerMoney(pid,data);
				}
				else if(!strcmp(stat,"XP"))
				{
					SetPlayerXP(pid,data);
				}
			}
		}		
	}
	else
	{
		format(stmp,sizeof(stmp),MODE_DIR PLAYERS_DIR "%s" PLAYERS_FILE_TYPE,name);
	
		new File:pfile = fopen(stmp,io_read);
		new File:tfile = fopen("temp",io_write);
		
		while(fread(pfile,stmp))
		{
			if(strfind(stmp,stat) == -1) fwrite(tfile,stmp);
			else 
			{
				itmp[0] = strfind(stmp,"=");
				new ndata = strval(stmp[itmp[0] + 1]);
				switch(type)
				{
					case CHANGE_STAT_ADD: ndata += data;
					case CHANGE_STAT_SET: ndata = data;
				}				
				format(stmp,sizeof(stmp),"%s=%d\r\n",stat,ndata);
				fwrite(tfile,stmp);
			}
		}
		fclose(pfile);
		fclose(tfile);
		// rename
		format(stmp,sizeof(stmp),MODE_DIR PLAYERS_DIR "%s" PLAYERS_FILE_TYPE,name);
		tfile = fopen(stmp,io_write);
		pfile = fopen("temp",io_read);
		while(fread(pfile,stmp)) fwrite(tfile,stmp);
		fclose(pfile);
		fclose(tfile);
		// delete temp
		fremove("temp");
	}
}

stock SetPlayerLevel(playerid,level_set) // Устанавливает игроку уровень
{
	if(level_set > MAX_LEVEL) return 0;
	SetPVarInt(playerid,"XP",0);
	SetPVarInt(playerid,"Level",level_set);
	SetPlayerScore(playerid,GetPVarInt(playerid,"Level"));
	return 1;
}

stock GivePlayerLevel(playerid,level_give) // Даёт игроку уровень
{
	if(GetPVarInt(playerid,"Level") >= MAX_LEVEL) return 0;
	if(GetPVarInt(playerid,"Level") + level_give > MAX_LEVEL)
	{
		level_give = MAX_LEVEL - GetPVarInt(playerid,"Level");
		SendClientMessage(playerid,COLOR_BLUE,"Вы достигли максимального уровня!");
	}
	SetPVarInt(playerid,"XP",0);
	SetPVarInt(playerid,"Level",GetPVarInt(playerid,"Level")+level_give);
	SetPlayerScore(playerid,GetPVarInt(playerid,"Level"));
	OnPlayerLevelChange(playerid,GetPVarInt(playerid,"Level")-level_give,GetPVarInt(playerid,"Level"));
	return 1;
}

stock SetPlayerXP(playerid,xp_set) // Устанавливает опыт игроку
{
	new xp_all = (xp_set+GetPVarInt(playerid,"XP")); // помещаем весь опыт в переменную
	new xp_tolvl = PlayerXPToLevel(playerid); // помещаем кол-во опыта до уровня в переменную
	if(xp_all == xp_tolvl)
	{
		SetPVarInt(playerid,"XP",0);
		GivePlayerLevel(playerid,1);
	}
	else if(xp_all > xp_tolvl)
	{
		GivePlayerLevel(playerid,1);
		SetPlayerXP(playerid, (xp_set - xp_tolvl) ); // устанавливаем нужный опыт
	}
	else if(xp_all < 0)
	{
		new xp_temp = GetPVarInt(playerid,"XP"); // старый опыт игрока
		GivePlayerLevel(playerid,-1);
		SetPVarInt(playerid,"XP", PlayerXPToLevel(playerid) + (xp_temp + xp_set) ); // вычисляем кол-во опыта для уровня пониженного уровня
	}
	else if(xp_all < xp_tolvl)
	{
		SetPVarInt(playerid,"XP",xp_set);
	}
}

stock GivePlayerXP(playerid,xp_give)
{
	if(xp_give == 0) return 1;
	if(GetPVarInt(playerid,"Level") >= MAX_LEVEL) return 1;
	new xp_all = (xp_give+GetPVarInt(playerid,"XP")); // помещаем весь опыт в переменную
	new xp_tolvl = PlayerXPToLevel(playerid); // помещаем кол-во опыта до уровня в переменную
	if(xp_all == xp_tolvl)
	{
		SetPVarInt(playerid,"XP",0);
		GivePlayerLevel(playerid,1);
	}
	else if(xp_all > xp_tolvl)
	{
		GivePlayerLevel(playerid,1);
		SetPlayerXP(playerid, (xp_give - xp_tolvl) ); // устанавливаем нужный опыт
	}
	else if(xp_all < 0)
	{
		new xp_temp = GetPVarInt(playerid,"XP"); // старый опыт игрока
		GivePlayerLevel(playerid,-1);
		SetPVarInt(playerid,"XP", PlayerXPToLevel(playerid) + (xp_temp + xp_give) ); // вычисляем кол-во опыта для пониженного уровня
	}
	else if(xp_all < xp_tolvl)
	{
		SetPVarInt(playerid,"XP",GetPVarInt(playerid,"XP")+xp_give);
	}
	return 1;
}

stock OnPlayerLevelChange(playerid,old_level,new_level)
{
	if(new_level >= MAX_LEVEL) SendClientMessage(playerid,COLOR_BLUE,"УРА! Вы достигли максимального уровня!");
	SetPVarInt(playerid,"UpgradesPoints",GetPVarInt(playerid,"UpgradesPoints") + (new_level-old_level));
	return 1;
}

stock PlayerXPToLevel(playerid)// Узнаёт сколько осталось игроку набрать опыта до следующего уровня
{
	return (CalculateXPForLevel(GetPVarInt(playerid,"Level")) - GetPVarInt(playerid,"XP"));
}

stock CalculateXPForLevel(level)
{
	return floatround((level * level) * XP_RATE);
}
/*
stock CalculateLevelForXP(xp)
{
	return floatround(floatsqroot((xp / XP_RATE)));
}*/

stock ChangeStartHelpStep(playerid,step)
{
	static line[256];
	new File:datafile = fopen(MODE_DIR START_HELP_FILE,io_read);
	if(datafile)
	{
		// find line by step
		for(new i = 0;i <= step;i++) 
		{
			if(!fread(datafile,line))
			{
				// file end - complete help show
				fclose(datafile);
				TogglePlayerSpectating(playerid,false);
				return -1;
			}
			else if(!CheckFileLine(line)) i--;
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
			case STARTHELP_ACTION_TITLE:
			{
				GameTextForPlayer(playerid,line[idx],5000,1);
			}
			case STARTHELP_ACTION_TEXT: 
			{
				SendClientMessage(playerid,START_HELP_COLOR,line[idx]);
			}
			case STARTHELP_ACTION_INTERIOR:
			{
				itmp[2] = strval(strtok(line,idx,','));
				SetPlayerInterior(playerid,itmp[2]);
			}
			case STARTHELP_ACTION_CAMERA_POS:
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
		}		
		// file end
		fclose(datafile);		
		return itmp[0];
	}
	else 
	{
		printf("[ERROR] start help file not found (%s)",MODE_DIR START_HELP_FILE);
		return -1;
	}
}

// --------------------------------------------------
// Obligatory functions
// --------------------------------------------------
Player_OnGameModeInit()
{
	SetTimer("Autosave",TIME_FOR_SAVE,1);
	print("Players manager loaded.");
}

Player_OnPlayerConnect(playerid)
{
	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid,name,sizeof(name));
	if(!strcmp("Unknown",name)) 
	{
		SendClientMessage(playerid,SYSTEM_COLOR,"Использование ника Unknown недопустимо");
		return Kick(playerid);
	}
	TogglePlayerSpectating(playerid,true);
	SetPVarString(playerid,"Account",name);
	format(stmp,sizeof(stmp),MODE_DIR PLAYERS_DIR "%s" PLAYERS_FILE_TYPE,name);
	if(fexist(stmp))
	{
		format(stmp,sizeof(stmp),"С возвращением, %s!\nВведите свой пароль чтобы приступить к игре",name);
		ShowPlayerDialog(playerid,DIALOG_LOGIN,DIALOG_STYLE_INPUT,MODE_NAME,stmp,"Готово","Выход");
	}
	else
	{
		format(stmp,sizeof(stmp),"Здравствуйте, %s!\nВведите свой пароль чтобы зарегистрироваться",name);
		ShowPlayerDialog(playerid,DIALOG_REGISTER,DIALOG_STYLE_INPUT,MODE_NAME,stmp,"Готово","Выход");	
	}
	format(stmp,sizeof(stmp),".:: Игрок %s подключился к серверу ::.",oGetPlayerName(playerid));
	SendClientMessageToAll(COLOR_YELLOW,stmp);
	return 1;
}

Player_OnPlayerDisconnect(playerid,reason)
{
	#pragma unused reason
	
	SavePlayer(playerid,SAVE_REASON_DISCONNECT);
	format(stmp,sizeof(stmp),".:: Игрок %s отключился от сервера ::.",oGetPlayerName(playerid));
	SendClientMessageToAll(COLOR_YELLOW,stmp);
}

Player_OnPlayerDeath(playerid,killerid,reason)
{
	#pragma unused reason
	SetPVarInt(playerid,"Deaths",GetPVarInt(playerid,"Deaths") + 1);
	if(killerid != INVALID_PLAYER_ID) SetPVarInt(killerid,"Kills",GetPVarInt(killerid,"Kills") + 1);
	return 1;
}

Player_OnPlayerSpawn(playerid)
{
	// check quest 
	if(GetPlayerQuest(playerid) == INVALID_QUEST_ID)
	{
		oSetPlayerHealth(playerid,GetPVarFloat(playerid,"MaxHealth"));
		SetPlayerScore(playerid,GetPVarInt(playerid,"Level"));
		for(new i = 0;i <= 11;i++)
		{
			format(stmp,sizeof(stmp),"SlotWeapon_%d",i);
			itmp[0] = GetPVarInt(playerid,stmp);
			format(stmp,sizeof(stmp),"SlotAmmo_%d",i);
			itmp[1] = GetPVarInt(playerid,stmp);
			oGivePlayerWeapon(playerid,itmp[0],itmp[1]);
		}
		SetPlayerColor(playerid,PLAYERS_COLOR);
	}
	for(new i = 0;i <= 10;i++)
	{
		format(stmp,sizeof(stmp),"Skill_%d",i);
		SetPlayerSkillLevel(playerid,i,GetPVarInt(playerid,stmp));
	}
	// preloading animations
	ApplyAnimation(playerid,"AIRPORT","null",0.0,0,0,0,0,0);
	ApplyAnimation(playerid,"Attractors","null",0.0,0,0,0,0,0);
	ApplyAnimation(playerid,"BAR","null",0.0,0,0,0,0,0);
	ApplyAnimation(playerid,"BASEBALL","null",0.0,0,0,0,0,0);
	ApplyAnimation(playerid,"BD_FIRE","null",0.0,0,0,0,0,0);
	ApplyAnimation(playerid,"benchpress","null",0.0,0,0,0,0,0);
	ApplyAnimation(playerid,"BF_injection","null",0.0,0,0,0,0,0);
	ApplyAnimation(playerid,"BIKED","null",0.0,0,0,0,0,0);
	ApplyAnimation(playerid,"BIKEH","null",0.0,0,0,0,0,0);
	ApplyAnimation(playerid,"BIKELEAP","null",0.0,0,0,0,0,0);
	ApplyAnimation(playerid,"BIKES","null",0.0,0,0,0,0,0);
	ApplyAnimation(playerid,"BIKEV","null",0.0,0,0,0,0,0);
	ApplyAnimation(playerid,"BIKE_DBZ","null",0.0,0,0,0,0,0);
	ApplyAnimation(playerid,"BLOWJOBZ","null",0.0,0,0,0,0,0);
	ApplyAnimation(playerid,"BMX","null",0.0,0,0,0,0,0);
	ApplyAnimation(playerid,"BOMBER","null",0.0,0,0,0,0,0);
	ApplyAnimation(playerid,"BOX","null",0.0,0,0,0,0,0);
	ApplyAnimation(playerid,"BSKTBALL","null",0.0,0,0,0,0,0);
	ApplyAnimation(playerid,"BUDDY","null",0.0,0,0,0,0,0);
	ApplyAnimation(playerid,"BUS","null",0.0,0,0,0,0,0);
	ApplyAnimation(playerid,"CAMERA","null",0.0,0,0,0,0,0);
	ApplyAnimation(playerid,"CAR","null",0.0,0,0,0,0,0);
	ApplyAnimation(playerid,"CARRY","null",0.0,0,0,0,0,0);
	ApplyAnimation(playerid,"CAR_CHAT","null",0.0,0,0,0,0,0);
	ApplyAnimation(playerid,"CASINO","null",0.0,0,0,0,0,0);
	ApplyAnimation(playerid,"CHAINSAW","null",0.0,0,0,0,0,0);
	ApplyAnimation(playerid,"CHOPPA","null",0.0,0,0,0,0,0);
	ApplyAnimation(playerid,"CLOTHES","null",0.0,0,0,0,0,0);
	ApplyAnimation(playerid,"COACH","null",0.0,0,0,0,0,0);
	ApplyAnimation(playerid,"COLT45","null",0.0,0,0,0,0,0);
	ApplyAnimation(playerid,"COP_AMBIENT","null",0.0,0,0,0,0,0);
	ApplyAnimation(playerid,"COP_DVBYZ","null",0.0,0,0,0,0,0);
	ApplyAnimation(playerid,"CRACK","null",0.0,0,0,0,0,0);
	ApplyAnimation(playerid,"CRIB","null",0.0,0,0,0,0,0);
	ApplyAnimation(playerid,"DAM_JUMP","null",0.0,0,0,0,0,0);
	ApplyAnimation(playerid,"DANCING","null",0.0,0,0,0,0,0);
	ApplyAnimation(playerid,"DEALER","null",0.0,0,0,0,0,0);
}

Player_Update()
{
	// regeneration
	for(new i = 0;i <= GetPlayersCount();i++)
	{
		if(!IsPlayerConnected(i)) continue;
		if(IsPlayerNPC(i)) continue;
		if(GetPlayerState(i) == PLAYER_STATE_NONE) continue;
		if(GetPlayerState(i) == PLAYER_STATE_WASTED) continue;
		if(GetPlayerState(i) == PLAYER_STATE_SPECTATING) continue;
		
		itmp[0] = GetPVarInt(i,"RegenerationTicks") + 1;
		if(itmp[0] >= REGENERATION_TIME)
		{
			SetPVarInt(i,"RegenerationTicks",0);
			ftmp[0] = oGetPlayerHealth(i);
			if(ftmp[0] >= GetPVarFloat(i,"MaxHealth")) continue;
			ftmp[0] += GetPVarFloat(i,"Regeneration");
			if(ftmp[0] > GetPVarFloat(i,"MaxHealth")) ftmp[0] = GetPVarFloat(i,"MaxHealth");
			oSetPlayerHealth(i,ftmp[0]);
		}
		else SetPVarInt(i,"RegenerationTicks",itmp[0]);
	}
}

Player_Dialog(playerid,dialogid,response,listitem,inputtext[])
{
	#pragma unused listitem
	
	switch(dialogid)
	{
		case DIALOG_LOGIN:
		{
			if(!response) return ShowPlayerDialog(playerid,DIALOG_LOGIN,DIALOG_STYLE_INPUT,MODE_NAME,"Введите свой пароль чтобы приступить к игре","Готово","Выход");
			if(!strlen(inputtext)) return ShowPlayerDialog(playerid,DIALOG_LOGIN,DIALOG_STYLE_INPUT,MODE_NAME,"Введите свой пароль чтобы приступить к игре","Готово","Выход");
			// load
			if(!LoadPlayer(playerid,inputtext)) return ShowPlayerDialog(playerid,DIALOG_LOGIN,DIALOG_STYLE_INPUT,MODE_NAME,"Введите свой пароль чтобы приступить к игре","Готово","Выход");
			// selecting
			TogglePlayerSpectating(playerid,false);
			return 1;
		}
		case DIALOG_REGISTER:
		{
			if(!response) return ShowPlayerDialog(playerid,DIALOG_REGISTER,DIALOG_STYLE_INPUT,MODE_NAME,"Введите свой пароль чтобы зарегистрироваться","Готово","Выход");
			if(!strlen(inputtext)) return ShowPlayerDialog(playerid,DIALOG_REGISTER,DIALOG_STYLE_INPUT,MODE_NAME,"Введите свой пароль чтобы зарегистрироваться","Готово","Выход");

			// pvar
			SetPVarInt(playerid,"Logged",1);
			SetPVarString(playerid,"Password",inputtext);
			SetPVarInt(playerid,"Level",0);
			SetPVarInt(playerid,"Kills",0);
			SetPVarInt(playerid,"Deaths",0);
			oSetPlayerMoney(playerid,0);
			SetPVarInt(playerid,"BankMoney",0);
			SetPVarInt(playerid,"XP",0);
			SetPVarInt(playerid,"UpgradesPoints",0);
			SetPVarFloat(playerid,"Regeneration",0.0);
			SetPVarFloat(playerid,"MaxHealth",100.0);
			for(new i = 0;i <= 10;i++)
			{
				format(stmp,sizeof(stmp),"Skill_%d",i);
				SetPVarInt(playerid,stmp,0);
				SetPlayerSkillLevel(playerid,i,0);
			}
			SavePlayer(playerid,SAVE_REASON_AUTOSAVE);
			// start show help
			SetPVarInt(playerid,"StartHelpStep",0);
			itmp[0] = ChangeStartHelpStep(playerid,0);
			if(itmp[0] != -1) SetTimerEx("StartHelpNextStep",itmp[0],0,"d",playerid);
			return 1;
		}
		case DIALOG_UPGRADE:
		{
			if(!response) return 1;
			
			if((itmp[0] = GetPVarInt(playerid,"UpgradesPoints")) < 1)
			return
			ShowPlayerDialog(playerid,DIALOG_NONE_ACTION,DIALOG_STYLE_MSGBOX,"Меню апгрейдов","У вас нету upgrade point'ов!","Ок","Ок");
			switch(listitem)
			{
				case 0: // max health
				{
					if((ftmp[0] = GetPVarFloat(playerid,"MaxHealth")) < 150.0)
					{
						SetPVarFloat(playerid,"MaxHealth",ftmp[0] + 10.0);
						SetPVarInt(playerid,"UpgradesPoints",itmp[0] - 1);
					}
				}
				case 1: // regeneration
				{
					if((ftmp[0] = GetPVarFloat(playerid,"Regeneration")) < 25.0)
					{
						SetPVarFloat(playerid,"Regeneration",ftmp[0] + 5.0);
						SetPVarInt(playerid,"UpgradesPoints",itmp[0] - 1);
					}
				}
				case 2: // skill
				{
					if((itmp[1] = GetPVarInt(playerid,"Skill_" #WEAPONSKILL_PISTOL)) < 999)
					{
						itmp[1] += 100;
						SetPVarInt(playerid,"Skill_" #WEAPONSKILL_PISTOL,itmp[1]);
						SetPlayerSkillLevel(playerid,WEAPONSKILL_PISTOL,itmp[1]);
						SetPVarInt(playerid,"UpgradesPoints",itmp[0] - 1);
					}
				}
				case 3: // skill
				{
					if((itmp[1] = GetPVarInt(playerid,"Skill_" #WEAPONSKILL_PISTOL_SILENCED)) < 999)
					{
						itmp[1] += 100;
						SetPVarInt(playerid,"Skill_" #WEAPONSKILL_PISTOL_SILENCED,itmp[1]);
						SetPlayerSkillLevel(playerid,WEAPONSKILL_PISTOL_SILENCED,itmp[1]);
						SetPVarInt(playerid,"UpgradesPoints",itmp[0] - 1);
					}
				}
				case 4: // skill
				{
					if((itmp[1] = GetPVarInt(playerid,"Skill_" #WEAPONSKILL_DESERT_EAGLE)) < 999)
					{
						itmp[1] += 100;
						SetPVarInt(playerid,"Skill_" #WEAPONSKILL_DESERT_EAGLE,itmp[1]);
						SetPlayerSkillLevel(playerid,WEAPONSKILL_DESERT_EAGLE,itmp[1]);
						SetPVarInt(playerid,"UpgradesPoints",itmp[0] - 1);
					}
				}
				case 5: // skill
				{
					if((itmp[1] = GetPVarInt(playerid,"Skill_" #WEAPONSKILL_SHOTGUN)) < 999)
					{
						itmp[1] += 100;
						SetPVarInt(playerid,"Skill_" #WEAPONSKILL_SHOTGUN,itmp[1]);
						SetPlayerSkillLevel(playerid,WEAPONSKILL_SHOTGUN,itmp[1]);
						SetPVarInt(playerid,"UpgradesPoints",itmp[0] - 1);
					}
				}
				case 6: // skill
				{
					if((itmp[1] = GetPVarInt(playerid,"Skill_" #WEAPONSKILL_SAWNOFF_SHOTGUN)) < 999)
					{
						itmp[1] += 100;
						SetPVarInt(playerid,"Skill_" #WEAPONSKILL_SAWNOFF_SHOTGUN,itmp[1]);
						SetPlayerSkillLevel(playerid,WEAPONSKILL_SAWNOFF_SHOTGUN,itmp[1]);
						SetPVarInt(playerid,"UpgradesPoints",itmp[0] - 1);
					}
				}
				case 7: // skill
				{
					if((itmp[1] = GetPVarInt(playerid,"Skill_" #WEAPONSKILL_SPAS12_SHOTGUN)) < 999)
					{
						itmp[1] += 100;
						SetPVarInt(playerid,"Skill_" #WEAPONSKILL_SPAS12_SHOTGUN,itmp[1]);
						SetPlayerSkillLevel(playerid,WEAPONSKILL_SPAS12_SHOTGUN,itmp[1]);
						SetPVarInt(playerid,"UpgradesPoints",itmp[0] - 1);
					}
				}
				case 8: // skill
				{
					if((itmp[1] = GetPVarInt(playerid,"Skill_" #WEAPONSKILL_MICRO_UZI)) < 999)
					{
						itmp[1] += 100;
						SetPVarInt(playerid,"Skill_" #WEAPONSKILL_MICRO_UZI,itmp[1]);
						SetPlayerSkillLevel(playerid,WEAPONSKILL_MICRO_UZI,itmp[1]);
						SetPVarInt(playerid,"UpgradesPoints",itmp[0] - 1);
					}
				}
				case 9: // skill
				{
					if((itmp[1] = GetPVarInt(playerid,"Skill_" #WEAPONSKILL_MP5)) < 999)
					{
						itmp[1] += 100;
						SetPVarInt(playerid,"Skill_" #WEAPONSKILL_MP5,itmp[1]);
						SetPlayerSkillLevel(playerid,WEAPONSKILL_MP5,itmp[1]);
						SetPVarInt(playerid,"UpgradesPoints",itmp[0] - 1);
					}
				}
				case 10: // skill
				{
					if((itmp[1] = GetPVarInt(playerid,"Skill_" #WEAPONSKILL_AK47)) < 999)
					{
						itmp[1] += 100;
						SetPVarInt(playerid,"Skill_" #WEAPONSKILL_AK47,itmp[1]);
						SetPlayerSkillLevel(playerid,WEAPONSKILL_AK47,itmp[1]);
						SetPVarInt(playerid,"UpgradesPoints",itmp[0] - 1);
					}
				}
				case 11: // skill
				{
					if((itmp[1] = GetPVarInt(playerid,"Skill_" #WEAPONSKILL_M4)) < 999)
					{
						itmp[1] += 100;
						SetPVarInt(playerid,"Skill_" #WEAPONSKILL_M4,itmp[1]);
						SetPlayerSkillLevel(playerid,WEAPONSKILL_M4,itmp[1]);
						SetPVarInt(playerid,"UpgradesPoints",itmp[0] - 1);
					}
				}
				case 12: // skill
				{
					if((itmp[1] = GetPVarInt(playerid,"Skill_" #WEAPONSKILL_SNIPERRIFLE)) < 999)
					{
						itmp[1] += 100;
						SetPVarInt(playerid,"Skill_" #WEAPONSKILL_SNIPERRIFLE,itmp[1]);
						SetPlayerSkillLevel(playerid,WEAPONSKILL_SNIPERRIFLE,itmp[1]);
						SetPVarInt(playerid,"UpgradesPoints",itmp[0] - 1);
					}
				}
			}
			format(stmp,sizeof(stmp),"Max health (%.0f)\nRegeneration (%.0f)\nColt skill (%d)\n",GetPVarFloat(playerid,"MaxHealth"),GetPVarFloat(playerid,"Regeneration"),GetPVarInt(playerid,"Skill_" #WEAPONSKILL_PISTOL));
			format(stmp,sizeof(stmp),"%sSilenced skill (%d)\nDesert eagle skill (%d)\nShotgun skill (%d)\n",stmp,GetPVarInt(playerid,"Skill_" #WEAPONSKILL_PISTOL_SILENCED),GetPVarInt(playerid,"Skill_" #WEAPONSKILL_DESERT_EAGLE),GetPVarInt(playerid,"Skill_" #WEAPONSKILL_SHOTGUN));
			format(stmp,sizeof(stmp),"%sSawnoff skill (%d)\nS.P.A.S. skill (%d)\nMicro UZI skill (%d)\n",stmp,GetPVarInt(playerid,"Skill_" #WEAPONSKILL_SAWNOFF_SHOTGUN),GetPVarInt(playerid,"Skill_" #WEAPONSKILL_SPAS12_SHOTGUN),GetPVarInt(playerid,"Skill_" #WEAPONSKILL_MICRO_UZI));
			format(stmp,sizeof(stmp),"%sMP5 skill (%d)\nAK47 skill (%d)\nM4 skill (%d)\nSniper rifle skill (%d)",stmp,GetPVarInt(playerid,"Skill_" #WEAPONSKILL_MP5),GetPVarInt(playerid,"Skill_" #WEAPONSKILL_AK47),GetPVarInt(playerid,"Skill_" #WEAPONSKILL_M4),GetPVarInt(playerid,"Skill_" #WEAPONSKILL_SNIPERRIFLE));
			ShowPlayerDialog(playerid,DIALOG_UPGRADE,DIALOG_STYLE_LIST,"Меню апгрейдов",stmp,"Улучшить","Отмена");
			return 1;
		}
	}
	return 0;
}

Player_Command(playerid,cmdid,params[])
{
	switch(cmdid)
	{
		case PLAYER_STAT_CMD:
		{
			stmp[0] = 0;
			format(stmp,sizeof(stmp),"\
				Уровень доступа:				%s\n\
				Банда:						%s\n\
				Уровень:					%d\n\
				Опыт:						%d\n\
				До следующего уровня:			%d\n\
				Деньги:					%d\n\
				Доход от бизнесов:				%d\n\
				Убийства:					%d\n\
				Смерти:					%d\n\
				Очки прокачки:				%d\n\
				Максимум жизни:				%.0f\n\
				Регенерация:					%.0f\n\
				",
				oGetPlayerAdminLevelName(playerid),
				oGetPlayerGangName(playerid),
				GetPVarInt(playerid,"Level"),
				GetPVarInt(playerid,"XP"),
				PlayerXPToLevel(playerid),
				oGetPlayerMoney(playerid),
				GetPVarInt(playerid,"BusinessIncome"),
				GetPVarInt(playerid,"Kills"),
				GetPVarInt(playerid,"Deaths"),
				GetPVarInt(playerid,"UpgradesPoints"),
				GetPVarFloat(playerid,"MaxHealth"),
				GetPVarFloat(playerid,"Regeneration")
			);
			format(stmp,sizeof(stmp),"%s\
				Скилл пистолета:				%d\n\
				Скилл пистолета с глушителем:		%d\n\
				Скилл \"Пустынного орла\"			%d\n\
				Скилл дробовика:				%d\n\
				Скилл обреза:				%d\n\
				Скилл S.P.A.S.:				%d\n\
				Скилл Микро узи:				%d\n\
				Скилл MP5:					%d\n\
				Скилл АК47:					%d\n\
				Скилл М4:					%d\n\
				Скилл снайперской винтовки:		%d\n\
				",
				stmp,
				GetPVarInt(playerid,"Skill_" #WEAPONSKILL_PISTOL),
				GetPVarInt(playerid,"Skill_" #WEAPONSKILL_PISTOL_SILENCED),
				GetPVarInt(playerid,"Skill_" #WEAPONSKILL_DESERT_EAGLE),
				GetPVarInt(playerid,"Skill_" #WEAPONSKILL_SHOTGUN),
				GetPVarInt(playerid,"Skill_" #WEAPONSKILL_SAWNOFF_SHOTGUN),
				GetPVarInt(playerid,"Skill_" #WEAPONSKILL_SPAS12_SHOTGUN),
				GetPVarInt(playerid,"Skill_" #WEAPONSKILL_MICRO_UZI),
				GetPVarInt(playerid,"Skill_" #WEAPONSKILL_MP5),
				GetPVarInt(playerid,"Skill_" #WEAPONSKILL_AK47),
				GetPVarInt(playerid,"Skill_" #WEAPONSKILL_M4),
				GetPVarInt(playerid,"Skill_" #WEAPONSKILL_SNIPERRIFLE)
			);
			ShowPlayerDialog(playerid,DIALOG_NONE_ACTION,DIALOG_STYLE_MSGBOX,"Статистика",stmp,"Ок","Ок");
			return 1;
		}
		case PLAYER_HELP_CMD:
		{
			stmp[0] = 0;
			strcat(stmp,MODE_NAME " - TDM-RPG мод, здесь вы можете улучшать своего персонажа, вступать в банды, учавствовать в гонках,\nдезматчах, мини-миссиях, командных миссиях и даже проходить настоящие миссии.\n");
			strcat(stmp,"/stats - ваша статистика\n/savechar - сохранить персонажа(автоматически сохраняется при выходе из игры)\n/changepass - изменить пароль от аккаунта\n/kill - убить себя\n/credits - узнать список разработчиков\n");
			strcat(stmp,"/tp - список телепортов\n/flip - перевернуть транспорт с крыши(только в стант мире)\n/upgrade - меню модификации персонажа\n/pm [id] [сообщение] - послать личное сообщение игроку\n/car [id] - создает транспорт(только в стант мире)\n/givecash [id] [кол-во] - передать игроку денег");
			strcat(stmp,"Для подробной помощи используйте команды:\n/b help - помощь по бизнесам\n/h help - помощь по домам\n/g help - помощь по бандам\n/dm help - помощь по дезматчам\n/race help - помощь по гонкам\n");
			strcat(stmp,"/bank help - помощь по банкам\n/gz help - помощь по зонам банд\n/gw help - помощь по войнам банд\n/tq help - помощь по командным миссиям\n/w help - помощь по вечному оружию");			
			ShowPlayerDialog(playerid,DIALOG_NONE_ACTION,DIALOG_STYLE_MSGBOX,"Помощь",stmp,"Ок","Ок");
			return 1;
		}
		case PLAYER_SAVECHAR_CMD:
		{
			SavePlayer(playerid,SAVE_REASON_SAVECHAR);
			SendClientMessage(playerid,SYSTEM_COLOR,"Аккаунт успешно сохранён");
			return 1;
		}
		case PLAYER_CHANGEPASS_CMD:
		{
			if(GetPVarInt(playerid,"Logged") != 1) return 0;
			
			SetPVarString(playerid,"Password",params);
			SavePlayer(playerid,SAVE_REASON_AUTOSAVE);
			SendClientMessage(playerid,SYSTEM_COLOR,"Пароль успешно сменён.");
			return 1;
		}
		case PLAYER_KILL_CMD:
		{
			oSetPlayerHealth(playerid,0.0);
			return 1;
		}
		case PLAYER_CREDITS_CMD:
		{
			ShowPlayerDialog(playerid,DIALOG_NONE_ACTION,DIALOG_STYLE_MSGBOX,"Создатели",
				"Главный разработчик: 009\n\
				Второй разработчик: ZiGGi\n\
				Спасибо: MadCat(за координаты домов, бизнесов, гонок, десматчей)"
				,"Ок","Ок");
			return 1;
		}
		case PLAYER_GIVECASH_CMD:
		{
			new idx;
			new id = strval(strtok(params,idx)),
				amount = strval(strtok(params,idx));
			
			if(!IsPlayerConnected(id))
			{
				format(stmp,sizeof(stmp),"Этот игрок(%d) оффлайн",id);
				SendClientMessage(playerid,COLOR_WHITE,stmp);
				return 1;
			}
			if(amount <= 0)
			{
				return SendClientMessage(playerid,COLOR_WHITE,"Вы ввели неверное количество денег!");
			}
			
			oGivePlayerMoney(playerid,-amount);
			oGivePlayerMoney(id,amount);

			format(stmp,sizeof(stmp),"Игрок %s(%d) передал вам %d$",oGetPlayerName(playerid),playerid,amount);
			SendClientMessage(id,COLOR_GREEN,stmp);
			format(stmp,sizeof(stmp),"Вы передали %d$ Игроку %s(%d)",amount,oGetPlayerName(id),id);
			SendClientMessage(playerid,COLOR_GREEN,stmp);
			return 1;
		}
		case PLAYER_FLIP_CMD:
		{
			if(GetPlayerVirtualWorld(playerid) != STUNT_VIRTUAL_WORLD)
				return SendClientMessage(playerid,SYSTEM_COLOR,"Эта команда доступна только в станотовом мире!");
			itmp[0] = GetPlayerVehicleID(playerid);
			GetVehicleZAngle(itmp[0], ftmp[0]);
			SetVehicleZAngle(itmp[0], ftmp[0]);
			return 1;
		}
		case PLAYER_UPGRADE_CMD:
		{
			format(stmp,sizeof(stmp),"Max health (%.0f)\nRegeneration (%.0f)\nColt skill (%d)\n",GetPVarFloat(playerid,"MaxHealth"),GetPVarFloat(playerid,"Regeneration"),GetPVarInt(playerid,"Skill_" #WEAPONSKILL_PISTOL));
			format(stmp,sizeof(stmp),"%sSilenced skill (%d)\nDesert eagle skill (%d)\nShotgun skill (%d)\n",stmp,GetPVarInt(playerid,"Skill_" #WEAPONSKILL_PISTOL_SILENCED),GetPVarInt(playerid,"Skill_" #WEAPONSKILL_DESERT_EAGLE),GetPVarInt(playerid,"Skill_" #WEAPONSKILL_SHOTGUN));
			format(stmp,sizeof(stmp),"%sSawnoff skill (%d)\nS.P.A.S. skill (%d)\nMicro UZI skill (%d)\n",stmp,GetPVarInt(playerid,"Skill_" #WEAPONSKILL_SAWNOFF_SHOTGUN),GetPVarInt(playerid,"Skill_" #WEAPONSKILL_SPAS12_SHOTGUN),GetPVarInt(playerid,"Skill_" #WEAPONSKILL_MICRO_UZI));
			format(stmp,sizeof(stmp),"%sMP5 skill (%d)\nAK47 skill (%d)\nM4 skill (%d)\nSniper rifle skill (%d)",stmp,GetPVarInt(playerid,"Skill_" #WEAPONSKILL_MP5),GetPVarInt(playerid,"Skill_" #WEAPONSKILL_AK47),GetPVarInt(playerid,"Skill_" #WEAPONSKILL_M4),GetPVarInt(playerid,"Skill_" #WEAPONSKILL_SNIPERRIFLE));
			ShowPlayerDialog(playerid,DIALOG_UPGRADE,DIALOG_STYLE_LIST,"Меню апгрейдов",stmp,"Улучшить","Отмена");
			return 1;
		}
		case PLAYER_PM_CMD:
		{
			stmp[0] = 0;
			new idx=0;
			stmp = strtok(params,idx);
			if(strlen(stmp) == 0) return SendClientMessage(playerid,0xFF0000FF,"Применение: /pm [id] [сообщение]");
			new id = strval(stmp);
			if(!IsPlayerConnected(id) || id == playerid) return SendClientMessage(playerid,0xFF0000FF,"/pm: Вы ввели неправильный id");
			
			if(strlen(params[idx]) == 0) return SendClientMessage(playerid,0xFF0000FF,"Применение: /pm [id] [сообщение]");
			
			format(stmp,sizeof(stmp),">> %s(%i): %s",oGetPlayerName(id),id,params[idx]);
			SendClientMessage(playerid,0xFFD720FF,stmp);
			
			format(stmp,sizeof(stmp),"** %s(%i): %s",oGetPlayerName(playerid),playerid,params[idx]);
			SendClientMessage(id,0xFFD720FF,stmp);
			return 1;
		}
	}
	return 0;
}
