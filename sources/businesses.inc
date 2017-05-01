/*
*	Created:			04.06.10
*	Author:				009
*	Description:		Бизнесы, получение денег в банк.
*/

#if defined _businesses_included
	#endinput
#endif

#define _businesses_included
#pragma library businesses

// --------------------------------------------------
// defines
// --------------------------------------------------
#define BUSINESSES_DIR			"businesses/"
#define BUSINESSES_MASK_FILE	"business_%d.txt"
#define BUSINESS_PICKUP			1272
#define BUSINESSES_LABEL_COLOR	0x663311FF
#define BUSINESS_ICON			36
#define BUSINESS_REBUY_COST		2.25
#define BUSINESS_PAY_TIME		600000
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
	BUSINESS_HELP_CMD,
	BUSINESS_INFO_CMD,
	BUSINESS_BUY_CMD,
	BUSINESS_SELL_CMD
};

// --------------------------------------------------
// statics
// --------------------------------------------------
static 
	PickupsLink[MAX_PICKUPS] = {-1,...},
	PlayerBusiness[MAX_PLAYERS],
	LastPay;

// --------------------------------------------------
// stocks
// --------------------------------------------------
stock LoadBusinessFromFile(filename[],bizid)
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
		k = CreatePickup(BUSINESS_PICKUP,42,ftmp[0],ftmp[1],ftmp[2],0);
		PickupsLink[k] = bizid;
		// create 3D text
		Create3DTextLabel(name,BUSINESSES_LABEL_COLOR,ftmp[0],ftmp[1],(ftmp[2] + 1.0),50.0,0,1);
		// create icon
#if defined _streamer_icons_included
		CreateStreamedIcon(BUSINESS_ICON,0x0,ftmp[0],ftmp[1],ftmp[2]);
#endif
	}
}

stock GetBusinessInfo(bizid,&cost,&income,owner[],name[],&Float:x,&Float:y,&Float:z)
{
	format(stmp,sizeof(stmp),MODE_DIR BUSINESSES_DIR BUSINESSES_MASK_FILE,bizid);
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
				if(!strcmp(stmp,"Income",true,k))
				{
					income = strval(stmp[k + 1]);
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
Businesses_OnGameModeInit()
{
	// load all houses
	new i = 0;
	format(stmp,sizeof(stmp),MODE_DIR BUSINESSES_DIR BUSINESSES_MASK_FILE,i);
	while(fexist(stmp))
	{
		LoadBusinessFromFile(stmp,i);
		// up
		i++;
		// again
		format(stmp,sizeof(stmp),MODE_DIR BUSINESSES_DIR BUSINESSES_MASK_FILE,i);
	}
	// data
#if defined PRINT_STATS_DATA
	printf("Businesses: %d",i);
#endif
	print("Businesses loaded.");
}

Businesses_OnPlayerPickUpPickup(playerid,pickupid)
{
	if(PickupsLink[pickupid] == -1) return;
	
	PlayerBusiness[playerid] = PickupsLink[pickupid];
}

Businesses_Command(playerid,cmdid,params[])
{
	#pragma unused params
	switch(cmdid)
	{
		case BUSINESS_HELP_CMD:
		{
			ShowPlayerDialog(playerid,DIALOG_NONE_ACTION,DIALOG_STYLE_MSGBOX,MODE_NAME " -> Предприятия","Вы можете купить себе предприятие, и получать\nденьги на счёт в банке\nКоманды:\n/b info - информация о предприятии\n/b buy - купить\n/b sell - продать","Ок","Ок");
			return 1;
		}
		case BUSINESS_INFO_CMD:
		{
			new hcost,
				hincome,
				hname[24],
				howner[24],
				Float:hcoords[3];
			if(GetBusinessInfo(PlayerBusiness[playerid],hcost,hincome,howner,hname,hcoords[0],hcoords[1],hcoords[2]))
			{
				if(!IsPlayerInRangeOfPoint(playerid,2.5,hcoords[0],hcoords[1],hcoords[2])) goto no_in_range;
				// generate text
				format(stmp,sizeof(stmp),"Адрес:\t\t\t\t\t%s\n",hname);
				if(strcmp(howner,oGetPlayerName(playerid))) format(stmp,sizeof(stmp),"%sВладелец:\t\t\t\t%s\n",stmp,howner);
				else format(stmp,sizeof(stmp),"%sВладелец:\t\t\t\t%s (Вы)\n",stmp,howner);
				format(stmp,sizeof(stmp),"%sЦена:\t\t\t\t\t%d\n",stmp,hcost);
				format(stmp,sizeof(stmp),"%sДоход:\t\t\t\t\t%d\n",stmp,hincome);
				if(strcmp(howner,"Unknown")) format(stmp,sizeof(stmp),"%sВключая перекупку:\t\t\t%d\n",stmp,floatround((hcost * BUSINESS_REBUY_COST)));
				// show info
				ShowPlayerDialog(playerid,DIALOG_NONE_ACTION,DIALOG_STYLE_MSGBOX,"Информация о недвижимости",stmp,"Ок","Ок");
				return 1;
			}
			no_in_range:
			SendClientMessage(playerid,SYSTEM_COLOR,"Вы не у предприятия");
			return 1;
		}
		case BUSINESS_BUY_CMD:
		{
			new hcost,
				hincome,
				hname[24],
				howner[24],
				Float:hcoords[3];
			if(GetBusinessInfo(PlayerBusiness[playerid],hcost,hincome,howner,hname,hcoords[0],hcoords[1],hcoords[2]))
			{
				if(!IsPlayerInRangeOfPoint(playerid,2.5,hcoords[0],hcoords[1],hcoords[2])) goto no_in_range;
				
				// check owner
				if(!strcmp(howner,oGetPlayerName(playerid)))
				{
					ShowPlayerDialog(playerid,DIALOG_NONE_ACTION,DIALOG_STYLE_MSGBOX,"Недвижимость","Это ваше предприятие","Ок","Ок");
					return 1;
				}
				// check cost
				if(strcmp(howner,"Unknown")) itmp[0] = floatround(hcost * BUSINESS_REBUY_COST);
				else itmp[0] = hcost;
				if(itmp[0] > oGetPlayerMoney(playerid))
				{
					format(stmp,sizeof(stmp),"Вам не хватает денег, требуется %d$",itmp[0]);
					ShowPlayerDialog(playerid,DIALOG_NONE_ACTION,DIALOG_STYLE_MSGBOX,"Покупка недвижимости",stmp,"Ок","Ок");
					return 1;
				}
				// change owner stat
				if(strcmp(howner,"Unknown")) 
				{
					ChangeStatByNameInt(howner,"Money",itmp[0],CHANGE_STAT_ADD);
					ChangeStatByNameInt(howner,"BusinessIncome",-hincome,CHANGE_STAT_ADD);
				}
				// take money
				oGivePlayerMoney(playerid,-itmp[0]);
				// set new data
				format(stmp,sizeof(stmp),MODE_DIR BUSINESSES_DIR BUSINESSES_MASK_FILE,PlayerBusiness[playerid]);
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
					format(stmp,sizeof(stmp),"Income=%d\r\n",hincome);
					fwrite(df,stmp);
					fclose(df);
				}
				// add income
				SetPVarInt(playerid,"BusinessIncome",(GetPVarInt(playerid,"BusinessIncome") + hincome));
				// grac
				format(stmp,sizeof(stmp),"Поздравляем с приобретением %s",hname);
				ShowPlayerDialog(playerid,DIALOG_NONE_ACTION,DIALOG_STYLE_MSGBOX,"Покупка недвижимости",stmp,"Ок","Ок");
				return 1;
			}
			no_in_range:
			SendClientMessage(playerid,SYSTEM_COLOR,"Вы не у предприятия");
			return 1;
		}
		case BUSINESS_SELL_CMD:
		{
			new hcost,
				hincome,
				hname[24],
				howner[24],
				Float:hcoords[3];
			if(GetBusinessInfo(PlayerBusiness[playerid],hcost,hincome,howner,hname,hcoords[0],hcoords[1],hcoords[2]))
			{
				if(!IsPlayerInRangeOfPoint(playerid,2.5,hcoords[0],hcoords[1],hcoords[2])) goto no_in_range;
				
				// check owner
				if(strcmp(howner,oGetPlayerName(playerid)))
				{
					ShowPlayerDialog(playerid,DIALOG_NONE_ACTION,DIALOG_STYLE_MSGBOX,"Продажа недвижимости","Это ваше предприятие","Ок","Ок");
					return 1;
				}
				// take money
				oGivePlayerMoney(playerid,hcost);
				// set new data
				format(stmp,sizeof(stmp),MODE_DIR BUSINESSES_DIR BUSINESSES_MASK_FILE,PlayerBusiness[playerid]);
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
					format(stmp,sizeof(stmp),"Income=%d\r\n",hincome);
					fwrite(df,stmp);
					fclose(df);
				}
				// remove income
				SetPVarInt(playerid,"BusinessIncome",(GetPVarInt(playerid,"BusinessIncome") - hincome));
				// grac
				format(stmp,sizeof(stmp),"Вы продали %s",hname);
				ShowPlayerDialog(playerid,DIALOG_NONE_ACTION,DIALOG_STYLE_MSGBOX,"Продажа недвижимости",stmp,"Ок","Ок");
				return 1;
			}
			no_in_range:
			SendClientMessage(playerid,SYSTEM_COLOR,"Вы не у предприятия");
			return 1;
		}
	}
	return 0;
}

Business_Update()
{
	if((GetTickCount() - LastPay) >= BUSINESS_PAY_TIME)
	{
		for(new i = 0;i <= GetPlayersCount();i++)
		{
			if(!IsPlayerConnected(i)) continue;
			if(IsPlayerNPC(i)) continue;			
			if(!GetPVarInt(i,"BusinessIncome")) continue;
			
			SetPVarInt(i,"BankMoney",(GetPVarInt(i,"BankMoney") + GetPVarInt(i,"BusinessIncome")));
			format(stmp,sizeof(stmp),"Доход с предприятий: %d$.Деньги высланы на ваш счёт в банке",GetPVarInt(i,"BusinessIncome"));
			SendClientMessage(i,SYSTEM_COLOR,stmp);
		}
		LastPay = GetTickCount();
	}
}

Business_OnSavePlayerData(playerid,File:datafile,reason)
{
	#pragma unused reason
	format(stmp,sizeof(stmp),"BusinessIncome=%d\r\n",GetPVarInt(playerid,"BusinessIncome"));
	fwrite(datafile,stmp);
}

Business_OnLoadPlayerData(playerid,datastr[],separatorpos)
{
	if(!strcmp(datastr,"BusinessIncome",true,separatorpos)) 
	{					
		SetPVarInt(playerid,"BusinessIncome",strval(datastr[separatorpos + 1]));
	}
}