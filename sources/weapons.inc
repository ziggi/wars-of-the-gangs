/*
*	Created:			30.08.10
*	Author:				ZiGGi
*	Description:		Покупка, продажа оружия
*/

#if defined _weapons_included
	#endinput
#endif

#define _weapons_included
#pragma library weapons

// --------------------------------------------------
// defines
// --------------------------------------------------
#define WEAPONS_DIR				"weapons/"
#define WEAPONS_MASK_FILE		"weapon_%d.txt"
#define WEAPONS_MENU_FILE		"weapons.txt"
#define WEAPONS_PICKUP			1274
#define WEAPONS_LABEL_COLOR		0x331166FF
#define WEAPONS_ICON			52
// --------------------------------------------------
// enums
// --------------------------------------------------
enum
{
	WEAPONS_HELP_CMD,
	WEAPONS_MENU_CMD,
	WEAPONS_DROP_CMD
};
// --------------------------------------------------
// statics
// --------------------------------------------------
static 
	PickupsLink[MAX_PICKUPS] = {-1,...},
	PlayerWeapon[MAX_PLAYERS];

// --------------------------------------------------
// stocks
// --------------------------------------------------
stock LoadWeaponFromFile(filename[],bankid)
{
	new File:df = fopen(filename,io_read),
		k,
		c,
		name[24];
	if(df)
	{
		while(fread(df,stmp))
		{
			if(!CheckFileLine(stmp)) continue;
			// parse
			if((k = strfind(stmp,"=")) == -1) continue;
			
			if(!strcmp(stmp,"Coords",true,k))
			{
				c = 0;
				ftmp[0] = floatstr(strtok(stmp[k + 1],c,','));
				ftmp[1] = floatstr(strtok(stmp[k + 1],c,','));
				ftmp[2] = floatstr(strtok(stmp[k + 1],c,','));
				continue;
			}
			if(!strcmp(stmp,"Name",true,k))
			{
				strcat(name,stmp[k + 1]);
				continue;
			}
		}
		fclose(df);
		
		// create pickup
		k = CreatePickup(WEAPONS_PICKUP,42,ftmp[0],ftmp[1],ftmp[2],0);
		PickupsLink[k] = bankid;
		// create 3D text
		Create3DTextLabel(name,WEAPONS_LABEL_COLOR,ftmp[0],ftmp[1],(ftmp[2] + 1.0),50.0,0,1);
		// create icon
#if defined _streamer_icons_included
		CreateStreamedIcon(WEAPONS_ICON,0x0,ftmp[0],ftmp[1],ftmp[2]);
#endif
	}
}

stock GetWeaponInfo(bankid,name[],&Float:x,&Float:y,&Float:z)
{
	format(stmp,sizeof(stmp),MODE_DIR WEAPONS_DIR WEAPONS_MASK_FILE,bankid);
	if(fexist(stmp))
	{
		new File:df = fopen(stmp,io_read),
			k,
			c;
		if(df)
		{
			while(fread(df,stmp))
			{
				if(!CheckFileLine(stmp)) continue;
				// parse
				if((k = strfind(stmp,"=")) == -1) continue;
			
				if(!strcmp(stmp,"Coords",true,k))
				{
					c = 0;
					x = floatstr(strtok(stmp[k + 1],c,','));
					y = floatstr(strtok(stmp[k + 1],c,','));
					z = floatstr(strtok(stmp[k + 1],c,','));
					continue;
				}
				if(!strcmp(stmp,"Name",true,k))
				{
					strcat(name,stmp[k + 1],24);
					continue;
				}
			}
			fclose(df);
			return 1;		
		}
	}
	return 0;
}

stock WeaponBuy(playerid,weapid)
{
	new File:df = fopen(MODE_DIR WEAPONS_DIR WEAPONS_MENU_FILE,io_read),
		c;
	if(df)
	{
		while(fread(df,stmp))
		{
			if(!CheckFileLine(stmp)) continue;
			
			if(c == weapid)
			{
				c = 0;
				itmp[0] = strval(strtok(stmp,c,','));
				itmp[1] = strval(strtok(stmp,c,','));
				itmp[2] = strval(strtok(stmp,c,'|'));
				if(itmp[2] > GetPVarInt(playerid,"Level"))
				{
					format(stmp,sizeof(stmp),"Это оружие будет доступно с %d уровня!",itmp[2]);
					SendClientMessage(playerid,SYSTEM_COLOR,stmp);
					goto not_weap;
				}
				GivePlayerOwnedWeapon(playerid,itmp[0],500);
				oGivePlayerMoney(playerid,-itmp[1]);
				break;
			}
			else c++;
		}
		not_weap:
		fclose(df);
	}
	else print("[ERROR] weapons file not exist");
}

stock GivePlayerOwnedWeapon(playerid,weaponid,ammo)
{
	static tmp;
	static str[20];
	tmp	= GetWeaponSlot(weaponid);
	format(str,sizeof(str),"SlotWeapon_%d",tmp);
	SetPVarInt(playerid,str,weaponid);
	format(str,sizeof(str),"SlotAmmo_%d",tmp);
	SetPVarInt(playerid,str,GetPVarInt(playerid,str) + ammo);
	oGivePlayerWeapon(playerid,weaponid,ammo);
}

stock GetPlayerOwnedWeaponData(playerid,slot,&weapons,&ammo)
{
	static str[20];
	format(str,sizeof(str),"SlotWeapon_%d",slot);
	weapons = GetPVarInt(playerid,str);
	format(str,sizeof(str),"SlotAmmo_%d",slot);
	ammo = GetPVarInt(playerid,str);	
}

stock ResetPlayerOwnedWeapons(playerid)
{
	static str[20];
	for(new i = 0;i < MAX_WEAPON_SLOTS;i++)
	{
		format(str,sizeof(str),"SlotWeapon_%d",i);
		SetPVarInt(playerid,str,0);
		format(str,sizeof(str),"SlotAmmo_%d",i);
		SetPVarInt(playerid,str,0);
	}
	ResetPlayerWeapons(playerid);
}

// --------------------------------------------------
// Obligatory functions
// --------------------------------------------------
Weapons_OnGameModeInit()
{
	// load all weapons
	new i = 0;
	format(stmp,sizeof(stmp),MODE_DIR WEAPONS_DIR WEAPONS_MASK_FILE,i);
	while(fexist(stmp))
	{
		LoadWeaponFromFile(stmp,i);
		// up
		i++;
		// again
		format(stmp,sizeof(stmp),MODE_DIR WEAPONS_DIR WEAPONS_MASK_FILE,i);
	}	
	// data
#if defined PRINT_STATS_DATA
	printf("Weapons: %d",i);
#endif
	print("Weapons loaded.");
}

Weapons_OnPlayerPickUpPickup(playerid,pickupid)
{
	if(PickupsLink[pickupid] == -1) return;
	
	PlayerWeapon[playerid] = PickupsLink[pickupid];
}

Weapons_Command(playerid,cmdid,params[])
{
	#pragma unused params
	switch(cmdid)
	{
		case WEAPONS_HELP_CMD:
		{
			ShowPlayerDialog(playerid,DIALOG_NONE_ACTION,DIALOG_STYLE_MSGBOX,MODE_NAME " -> Магазин оружия",
				"Вы можете покупать оружее\nПри смерти это оружие у вас не исчезает\nКоманды:\n/w menu - меню покупки оружия\n/w drop - выбросить(удалить) оружие которое в руках",
				"Ок","Ок"
			);
			return 1;
		}
		case WEAPONS_MENU_CMD:
		{
			new wname[24],idx,
				Float:wcoords[3];
			if(GetWeaponInfo(PlayerWeapon[playerid],wname,wcoords[0],wcoords[1],wcoords[2]))
			{
				if(!IsPlayerInRangeOfPoint(playerid,2.5,wcoords[0],wcoords[1],wcoords[2])) goto no_in_range;
				// check quest
				if(GetPlayerQuest(playerid) != INVALID_QUEST_ID) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы выполняете задание");
				// file read
				new File:datafile = fopen(MODE_DIR WEAPONS_DIR WEAPONS_MENU_FILE,io_read),
					line[MAX_STRING];
				// checker
				itmp[1] = 0;
				if(datafile)
				{
					new stmp_big[2048];
					while(fread(datafile,line))
					{
						if(!CheckFileLine(line)) continue;
						itmp[2] = strfind(line,"|") + 1;
						if(itmp[0])
						{
							itmp[1]++;
						}
						idx = 0;
						itmp[3] = strval( strtok(line,idx,',') );
						format(stmp_big,sizeof(stmp_big),"%s%s -- [lvl:%s]  [$%s]\n",stmp_big,line[ itmp[2] ],strtok(line,idx,'|'),strtok(line,idx,','));
					}
					fclose(datafile);
					ShowPlayerDialog(playerid,DIALOG_WEAPONS,DIALOG_STYLE_LIST,"Меню покупки оружия",stmp_big,"Выбрать","Отмена");
				}
				else print("[ERROR] weapons file not exist");
				return 1;
			}
			no_in_range:
			SendClientMessage(playerid,SYSTEM_COLOR,"Вы не в оружейном магазине");
			return 1;
		}
		case WEAPONS_DROP_CMD:
		{
			
			return 1;
		}
	}
	return 0;
}

Weapons_Dialog(playerid, dialogid, response, listitem, inputtext[])
{
	#pragma unused inputtext
	switch(dialogid)
	{
		case DIALOG_WEAPONS:
		{
			if(!response) return 1;
			WeaponBuy(playerid,listitem);
			return 1;
		}
	}
	return 1;
}

Weapons_OnSavePlayerData(playerid,File:datafile,reason)
{
	#pragma unused reason
	for(new i = 0;i <= 10;i++)
	{
		format(stmp,sizeof(stmp),"Skill_%d",i);
		format(stmp,sizeof(stmp),"%s=%d\r\n",stmp,GetPVarInt(playerid,stmp));
		fwrite(datafile,stmp);
	}
	for(new i = 0;i <= 11;i++)
	{
		format(stmp,sizeof(stmp),"SlotWeapon_%d",i);
		format(stmp,sizeof(stmp),"%s=%d\r\n",stmp,GetPVarInt(playerid,stmp));
		fwrite(datafile,stmp);
		format(stmp,sizeof(stmp),"SlotAmmo_%d",i);
		format(stmp,sizeof(stmp),"%s=%d\r\n",stmp,GetPVarInt(playerid,stmp));
		fwrite(datafile,stmp);
	}
}

Weapons_OnLoadPlayerData(playerid,datastr[],separatorpos)
{
	new tmp[20];
	for(new i = 0;i <= 10;i++)
	{
		format(tmp,sizeof(tmp),"Skill_%d",i);
		if(!strcmp(datastr,tmp,true,separatorpos)) 
		{
			SetPVarInt(playerid,tmp,strval(datastr[separatorpos + 1]));
			break;
		}
	}
	for(new i = 0;i <= 11;i++)
	{
		format(tmp,sizeof(tmp),"SlotWeapon_%d",i);
		if(!strcmp(datastr,tmp,true,separatorpos)) 
		{
			SetPVarInt(playerid,tmp,strval(datastr[separatorpos + 1]));
			break;
		}
		format(tmp,sizeof(tmp),"SlotAmmo_%d",i);
		if(!strcmp(datastr,tmp,true,separatorpos)) 
		{
			SetPVarInt(playerid,tmp,strval(datastr[separatorpos + 1]));
			break;
		}
	}
}
