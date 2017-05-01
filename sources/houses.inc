/*
*	Created:			04.06.10
*	Author:				009
*	Description:		Дома, рандомный спавн.
*/

#if defined _houses_included
	#endinput
#endif

#define _houses_included
#pragma library houses

// --------------------------------------------------
// defines
// --------------------------------------------------
#define HOUSES_DIR				"houses/"
#define HOUSES_MASK_FILE		"house_%d.txt"
#define RANDOM_SPAWNS_FILE		"random.txt"
#define HOUSE_PICKUP			1273
#define HOUSES_LABEL_COLOR		0x336611FF
#define HOUSE_ICON				31
#define HOUSE_REBUY_COST		1.75
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
#if !defined GetPlayerQuest
	#error GetPlayerQuest not found in script.
#endif


// --------------------------------------------------
// enums
// --------------------------------------------------
enum
{
	HOUSES_HELP_CMD,
	HOUSES_INFO_CMD,
	HOUSES_BUY_CMD,
	HOUSES_SELL_CMD,
	HOUSES_SPAWNSEL_CMD
};

// --------------------------------------------------
// statics
// --------------------------------------------------
static 
	PickupsLink[MAX_PICKUPS] = {-1,...},
	PlayerHouse[MAX_PLAYERS];

// --------------------------------------------------
// stocks
// --------------------------------------------------
stock LoadHouseFromFile(filename[],houseid)
{
	new File:df = fopen(filename,io_read),
		k,
		c,
		name[24],
		owner[24];
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
			if(!strcmp(stmp,"Owner",true,k))
			{
				strcat(owner,stmp[k + 1]);
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
		k = CreatePickup(HOUSE_PICKUP,42,ftmp[0],ftmp[1],ftmp[2],0);
		PickupsLink[k] = houseid;
		// create 3D text
		Create3DTextLabel(name,HOUSES_LABEL_COLOR,ftmp[0],ftmp[1],(ftmp[2] + 1.0),50.0,0,1);
		// create icon
#if defined _streamer_icons_included
		CreateStreamedIcon(HOUSE_ICON,0x0,ftmp[0],ftmp[1],ftmp[2]);
#endif
	}
}

stock GetHouseInfo(houseid,&cost,owner[],name[],&Float:x,&Float:y,&Float:z)
{
	format(stmp,sizeof(stmp),MODE_DIR HOUSES_DIR HOUSES_MASK_FILE,houseid);
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
				if(!strcmp(stmp,"Owner",true,k))
				{
					strcat(owner,stmp[k + 1],24);
					continue;
				}
				if(!strcmp(stmp,"Name",true,k))
				{
					strcat(name,stmp[k + 1],24);
					continue;
				}
				if(!strcmp(stmp,"Cost",true,k))
				{
					cost = strval(stmp[k + 1]);
					continue;
				}
			}
			fclose(df);
			return 1;		
		}
	}
	return 0;
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
Houses_OnGameModeInit()
{
	// load all houses
	new i = 0;
	format(stmp,sizeof(stmp),MODE_DIR HOUSES_DIR HOUSES_MASK_FILE,i);
	while(fexist(stmp))
	{
		LoadHouseFromFile(stmp,i);
		// up
		i++;
		// again
		format(stmp,sizeof(stmp),MODE_DIR HOUSES_DIR HOUSES_MASK_FILE,i);
	}	
	// data
#if defined PRINT_STATS_DATA
	printf("Houses: %d",i);
#endif
	print("Houses loaded.");
}

Houses_OnPlayerConnect(playerid)
{
	SetPVarInt(playerid,"SpawnHouse",-1);
}

Houses_OnPlayerPickUpPickup(playerid,pickupid)
{
	if(PickupsLink[pickupid] == -1) return;
	
	PlayerHouse[playerid] = PickupsLink[pickupid];
}

Houses_OnPlayerSpawn(playerid)
{
	// check quest
	if(GetPlayerQuest(playerid) != INVALID_QUEST_ID) return 0;
	// interior 0
	SetPlayerInterior(playerid,0);
	// virtual world 0
	SetPlayerVirtualWorld(playerid,0);
	// position	
	if(GetPVarInt(playerid,"SpawnHouse") != -1)
	{
		new hcost,
			hname[24],
			howner[24],
			Float:hcoords[3];
		if(!GetHouseInfo(GetPVarInt(playerid,"SpawnHouse"),hcost,howner,hname,hcoords[0],hcoords[1],hcoords[2])) goto null_info;
		if(strcmp(howner,oGetPlayerName(playerid))) goto null_info;
		else return SetPlayerPos(playerid,hcoords[0],hcoords[1],hcoords[2]);
		null_info:
		SetPVarInt(playerid,"SpawnHouse",-1);
	}
	new File:df = fopen(MODE_DIR HOUSES_DIR RANDOM_SPAWNS_FILE,io_read);
	if(df)
	{
		// count of spawns
		new counts;
		while(fread(df,stmp)) counts++;
		// set on start
		fseek(df);
		// randomize spawn
		counts = random(counts);
		// go
		while(fread(df,stmp))
		{
			counts--;
			if(counts > 0) continue;
			
			new c;
			ftmp[0] = floatstr(strtok(stmp,c,','));
			ftmp[1] = floatstr(strtok(stmp,c,','));
			ftmp[2] = floatstr(strtok(stmp,c,','));
			break;
		}
		SetPlayerPos(playerid,ftmp[0],ftmp[1],ftmp[2]);
		fclose(df);
	}
	else print("[ERROR] not found random spawns file");
	return 1;
}

Houses_Command(playerid,cmdid,params[])
{
	#pragma unused params
	switch(cmdid)
	{
		case HOUSES_HELP_CMD:
		{
			ShowPlayerDialog(playerid,DIALOG_NONE_ACTION,DIALOG_STYLE_MSGBOX,MODE_NAME " -> Дома","Вы можете купить себе дом, и при желании выбрать его\nместом своего спавна\nКоманды:\n/h info - информация про недвижимость\n/h buy - купить\n/h sell - продать\n/h spawn - выбрать спавном","Ок","Ок");
			return 1;
		}
		case HOUSES_INFO_CMD:
		{
			new hcost,
				hname[24],
				howner[24],
				Float:hcoords[3];
			if(GetHouseInfo(PlayerHouse[playerid],hcost,howner,hname,hcoords[0],hcoords[1],hcoords[2]))
			{
				if(!IsPlayerInRangeOfPoint(playerid,2.5,hcoords[0],hcoords[1],hcoords[2])) goto no_in_range;
				// generate text
				format(stmp,sizeof(stmp),"Адрес:\t\t\t\t\t%s\n",hname);
				if(strcmp(howner,oGetPlayerName(playerid))) format(stmp,sizeof(stmp),"%sВладелец:\t\t\t\t%s\n",stmp,howner);
				else format(stmp,sizeof(stmp),"%sВладелец:\t\t\t\t%s (Вы)\n",stmp,howner);
				format(stmp,sizeof(stmp),"%sЦена:\t\t\t\t\t%d\n",stmp,hcost);
				if(strcmp(howner,"Unknown")) format(stmp,sizeof(stmp),"%sВключая перекупку:\t\t\t%d\n",stmp,floatround((hcost * HOUSE_REBUY_COST)));
				// show info
				ShowPlayerDialog(playerid,DIALOG_NONE_ACTION,DIALOG_STYLE_MSGBOX,"Информация о недвижимости",stmp,"Ок","Ок");
				return 1;
			}
			no_in_range:
			SendClientMessage(playerid,SYSTEM_COLOR,"Вы не у дома");
			return 1;
		}
		case HOUSES_BUY_CMD:
		{
			new hcost,
				hname[24],
				howner[24],
				Float:hcoords[3];
			if(GetHouseInfo(PlayerHouse[playerid],hcost,howner,hname,hcoords[0],hcoords[1],hcoords[2]))
			{
				if(!IsPlayerInRangeOfPoint(playerid,2.5,hcoords[0],hcoords[1],hcoords[2])) goto no_in_range;
				
				// check owner
				if(!strcmp(howner,oGetPlayerName(playerid)))
				{
					ShowPlayerDialog(playerid,DIALOG_NONE_ACTION,DIALOG_STYLE_MSGBOX,"Недвижимость","Это ваш дом","Ок","Ок");
					return 1;
				}
				// check cost
				if(strcmp(howner,"Unknown")) itmp[0] = floatround(hcost * HOUSE_REBUY_COST);
				else itmp[0] = hcost;
				if(itmp[0] > oGetPlayerMoney(playerid))
				{
					format(stmp,sizeof(stmp),"Вам не хватает денег, требуется %d$",itmp[0]);
					ShowPlayerDialog(playerid,DIALOG_NONE_ACTION,DIALOG_STYLE_MSGBOX,"Покупка недвижимости",stmp,"Ок","Ок");
					return 1;
				}
				// give money to owner
				if(strcmp(howner,"Unknown")) ChangeStatByNameInt(howner,"Money",itmp[0],CHANGE_STAT_ADD);
				// take money
				oGivePlayerMoney(playerid,-itmp[0]);
				// set new data
				format(stmp,sizeof(stmp),MODE_DIR HOUSES_DIR HOUSES_MASK_FILE,PlayerHouse[playerid]);
				new File:df = fopen(stmp,io_write);
				if(df)
				{
					format(stmp,sizeof(stmp),"Coords=%f,%f,%f\r\n",hcoords[0],hcoords[1],hcoords[2]);
					fwrite(df,stmp);
					format(stmp,sizeof(stmp),"Owner=%s\r\n",oGetPlayerName(playerid));
					fwrite(df,stmp);
					format(stmp,sizeof(stmp),"Name=%s\r\n",hname);
					fwrite(df,stmp);
					format(stmp,sizeof(stmp),"Cost=%d\r\n",hcost);
					fwrite(df,stmp);
					fclose(df);
				}
				// grac
				format(stmp,sizeof(stmp),"Поздравляем с приобретением %s",hname);
				ShowPlayerDialog(playerid,DIALOG_NONE_ACTION,DIALOG_STYLE_MSGBOX,"Покупка недвижимости",stmp,"Ок","Ок");
				return 1;
			}
			no_in_range:
			SendClientMessage(playerid,SYSTEM_COLOR,"Вы не у дома");
			return 1;
		}
		case HOUSES_SELL_CMD:
		{
			new hcost,
				hname[24],
				howner[24],
				Float:hcoords[3];
			if(GetHouseInfo(PlayerHouse[playerid],hcost,howner,hname,hcoords[0],hcoords[1],hcoords[2]))
			{
				if(!IsPlayerInRangeOfPoint(playerid,2.5,hcoords[0],hcoords[1],hcoords[2])) goto no_in_range;
				
				// check owner
				if(strcmp(howner,oGetPlayerName(playerid)))
				{
					ShowPlayerDialog(playerid,DIALOG_NONE_ACTION,DIALOG_STYLE_MSGBOX,"Продажа недвижимости","Это не ваш дом","Ок","Ок");
					return 1;
				}
				// take money
				oGivePlayerMoney(playerid,hcost);
				// set new data
				format(stmp,sizeof(stmp),MODE_DIR HOUSES_DIR HOUSES_MASK_FILE,PlayerHouse[playerid]);
				new File:df = fopen(stmp,io_write);
				if(df)
				{
					format(stmp,sizeof(stmp),"Coords=%f,%f,%f\r\n",hcoords[0],hcoords[1],hcoords[2]);
					fwrite(df,stmp);
					fwrite(df,"Owner=Unknown\r\n");
					format(stmp,sizeof(stmp),"Name=%s\r\n",hname);
					fwrite(df,stmp);
					format(stmp,sizeof(stmp),"Cost=%d\r\n",hcost);
					fwrite(df,stmp);
					fclose(df);
				}
				// check spawn
				if(GetPVarInt(playerid,"SpawnHouse") == PlayerHouse[playerid]) SetPVarInt(playerid,"SpawnHouse",-1);
				// grac
				format(stmp,sizeof(stmp),"Вы продали %s",hname);
				ShowPlayerDialog(playerid,DIALOG_NONE_ACTION,DIALOG_STYLE_MSGBOX,"Продажа недвижимости",stmp,"Ок","Ок");
				return 1;
			}
			no_in_range:
			SendClientMessage(playerid,SYSTEM_COLOR,"Вы не у дома");
			return 1;
		}
		case HOUSES_SPAWNSEL_CMD:
		{
			new hcost,
				hname[24],
				howner[24],
				Float:hcoords[3];
			if(GetHouseInfo(PlayerHouse[playerid],hcost,howner,hname,hcoords[0],hcoords[1],hcoords[2]))
			{
				if(!IsPlayerInRangeOfPoint(playerid,2.5,hcoords[0],hcoords[1],hcoords[2])) goto no_in_range;
				
				// check owner
				if(strcmp(howner,oGetPlayerName(playerid)))
				{
					ShowPlayerDialog(playerid,DIALOG_NONE_ACTION,DIALOG_STYLE_MSGBOX,"Недвижимость","Это не ваш дом","Ок","Ок");
					return 1;
				}
				// set data
				SetPVarInt(playerid,"SpawnHouse",PlayerHouse[playerid]);
				// grac
				format(stmp,sizeof(stmp),"Вы выбрали %s своим местом спавна",hname);
				ShowPlayerDialog(playerid,DIALOG_NONE_ACTION,DIALOG_STYLE_MSGBOX,"Недвижимость",stmp,"Ок","Ок");
				return 1;
			}
			no_in_range:
			SendClientMessage(playerid,SYSTEM_COLOR,"Вы не у дома");
			return 1;
		}
	}
	return 0;
}

Houses_OnSavePlayerData(playerid,File:datafile,reason)
{
	#pragma unused reason
	format(stmp,sizeof(stmp),"SpawnHouse=%d\r\n",GetPVarInt(playerid,"SpawnHouse"));
	fwrite(datafile,stmp);
}

Houses_OnLoadPlayerData(playerid,datastr[],separatorpos)
{
	if(!strcmp(datastr,"SpawnHouse",true,separatorpos)) 
	{
		SetPVarInt(playerid,"SpawnHouse",strval(datastr[separatorpos + 1]));
	}
}