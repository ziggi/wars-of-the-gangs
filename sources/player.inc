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
#define REGENERATION_TIME		5
#define TIME_FOR_SAVE			1800_000
#define HELP_TIME				10_000
#define XP_RATE					150

// --------------------------------------------------
// enums
// --------------------------------------------------
enum
{
	PLAYER_STAT_CMD,
	PLAYER_HELP_CMD,
	PLAYER_SAVECHAR_CMD,
	PLAYER_CHANGEPASS_CMD,
	PLAYER_KILL_CMD
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
}

// --------------------------------------------------
// forwards
// --------------------------------------------------
forward NextHelpStep(playerid,step);
forward P_Autosave();
// global
forward OnSavePlayerData(playerid,File:datafile,reason);
forward OnLoadPlayerData(playerid,datastr[],separatorpos);

// --------------------------------------------------
// publics
// --------------------------------------------------
public NextHelpStep(playerid,step)
{
	switch(step)
	{
		case 1:
		{
			for(new i = 0;i < 15;i++) SendClientMessage(playerid,0xFFFFFFFF,"  ");
			
			SetPlayerCameraPos(playerid,-1743.5432,-567.9924,36.4844);
			SetPlayerCameraLookAt(playerid,-1787.6360,-612.7075,17.2989);
			return SetTimerEx("NextHelpStep",HELP_TIME,0,"dd",playerid,2);
		}
		case 2:
		{
			for(new i = 0;i < 15;i++) SendClientMessage(playerid,0xFFFFFFFF,"  ");
			
			SetPlayerCameraPos(playerid,-1551.5392,513.3345,27.0313);
			SetPlayerCameraLookAt(playerid,-1535.4336,486.8857,7.1797);
			return SetTimerEx("NextHelpStep",HELP_TIME,0,"dd",playerid,3);
		}
		case 3:
		{
			for(new i = 0;i < 15;i++) SendClientMessage(playerid,0xFFFFFFFF,"  ");
			
			SetPlayerCameraPos(playerid,-2627.9685,-202.0271,24.3359);
			SetPlayerCameraLookAt(playerid,-2657.6631,-227.9266,4.6831);
			return SetTimerEx("NextHelpStep",HELP_TIME,0,"dd",playerid,4);
		}
		case 4:
		{
			for(new i = 0;i < 15;i++) SendClientMessage(playerid,0xFFFFFFFF,"  ");
			
			SetPlayerCameraPos(playerid,-2705.7559,-71.3518,24.2969);
			SetPlayerCameraLookAt(playerid,-2705.9399,39.1872,4.2969);
			return SetTimerEx("NextHelpStep",HELP_TIME,0,"dd",playerid,5);
		}
		case 5:
		{
			return TogglePlayerSpectating(playerid,false);
		}
	}
	return 0;
}

public P_Autosave()
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
	for(new i = 0;i <= 10;i++)
	{
		format(stmp,sizeof(stmp),"Skill_%d",i);
		format(stmp,sizeof(stmp),"%s=%d\r\n",stmp,GetPVarInt(playerid,stmp));
		fwrite(pfile,stmp);
	}
	for(new i = 0;i <= 11;i++)
	{
		format(stmp,sizeof(stmp),"SlotWeapon_%d",i);
		format(stmp,sizeof(stmp),"%s=%d\r\n",stmp,GetPVarInt(playerid,stmp));
		fwrite(pfile,stmp);
		format(stmp,sizeof(stmp),"SlotAmmo_%d",i);
		format(stmp,sizeof(stmp),"%s=%d\r\n",stmp,GetPVarInt(playerid,stmp));
		fwrite(pfile,stmp);
	}
	// other systems
	OnSavePlayerData(playerid,pfile,reason);
	
	fclose(pfile);
}

stock LoadPlayer(playerid,pass[])
{
	new name[MAX_PLAYER_NAME];
	GetPVarString(playerid,"Account",name,sizeof(name));
	
	format(stmp,sizeof(stmp),MODE_DIR PLAYERS_DIR "%s" PLAYERS_FILE_TYPE,name);
	
	new File:pfile = fopen(stmp,io_read),
		tmp[20];
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
		for(new i = 0;i <= 10;i++)
		{
			format(tmp,sizeof(tmp),"Skill_%d",i);
			if(!strcmp(stmp,tmp,true,itmp[0])) 
			{
				SetPVarInt(playerid,tmp,strval(stmp[itmp[0] + 1]));
				break;
			}
		}
		for(new i = 0;i <= 11;i++)
		{
			format(tmp,sizeof(tmp),"SlotWeapon_%d",i);
			if(!strcmp(stmp,tmp,true,itmp[0])) 
			{
				SetPVarInt(playerid,tmp,strval(stmp[itmp[0] + 1]));
				break;
			}
			format(tmp,sizeof(tmp),"SlotAmmo_%d",i);
			if(!strcmp(stmp,tmp,true,itmp[0])) 
			{
				SetPVarInt(playerid,tmp,strval(stmp[itmp[0] + 1]));
				break;
			}
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
					SetPVarInt(pid,"Level",CalculateLevelForXP(data));
					SetPlayerScore(pid,GetPVarInt(pid,"Level"));
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
					SetPVarInt(pid,"Level",CalculateLevelForXP(data));
					SetPlayerScore(pid,GetPVarInt(pid,"Level"));
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

stock SetPlayerXP(playerid,xp)
{
	SetPVarInt(playerid,"XP",xp);
	SetPVarInt(playerid,"Level",CalculateLevelForXP(xp));
	SetPlayerScore(playerid,GetPVarInt(playerid,"Level"));
}

stock GivePlayerXP(playerid,xp)
{
	SetPVarInt(playerid,"XP",GetPVarInt(playerid,"XP") + xp);
	SetPVarInt(playerid,"Level",CalculateLevelForXP(GetPVarInt(playerid,"XP")));
	SetPlayerScore(playerid,GetPVarInt(playerid,"Level"));
}

stock CalculateXPForLevel(level)
{
	return floatround((level * level) * XP_RATE);
}

stock CalculateLevelForXP(xp)
{
	return floatround(floatsqroot((xp / XP_RATE)));
}

stock ChangeStartHelpStep(playerid,step)
{
	
}

// --------------------------------------------------
// Obligatory functions
// --------------------------------------------------
Player_OnGameModeInit()
{
	SetTimer("P_Autosave",TIME_FOR_SAVE,1);
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
	return 1;
}

Player_OnPlayerDisconnect(playerid,reason)
{
	#pragma unused reason
	
	SavePlayer(playerid,SAVE_REASON_DISCONNECT);
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
}

Player_Update()
{
	// regeneration
	for(new i = 0;i <= GetPlayersCount();i++)
	{
		if(!IsPlayerConnected(i)) continue;
		if(IsPlayerNPC(i)) continue;
		
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
			for(new i = 0;i < 15;i++) SendClientMessage(playerid,SYSTEM_COLOR,"  ");
			SetPlayerCameraPos(playerid,-2705.7559,-71.3518,24.2969);
			SetPlayerCameraLookAt(playerid,-2705.9399,39.1872,4.2969);
			SetTimerEx("NextHelpStep",HELP_TIME * 2,0,"dd",playerid,1);
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
				Скилл обреза:					%d\n\
				Скилл S.P.A.S.:					%d\n\
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
			strcat(stmp,"Для подробной помощи используйте команды:\n/b help - помощь по бизнесам\n/h help - помощь по домам\n/g help - помощь по бандам\n/dm help - помощь по дезматчам\n/race help - помощь по гонкам\n");
			strcat(stmp,"/bank help - помощь по банкам\n/gz help - помощь по зонам банд\n/gw help - помощь по войнам банд\n/tq help - помощь по командным миссиям");			
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
	}
	return 0;
}